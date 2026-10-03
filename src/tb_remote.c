/*
 * Prospector trackball sensitivity remote (scanner side)
 *
 * While the trackball sensitivity screen is open, broadcast a small
 * non-connectable advertisement addressed to the selected keyboard. The
 * keyboard treats every packet as a heartbeat (keeps its listen window wide
 * open) and executes a command once per new sequence number.
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/random/random.h>
#include <zephyr/logging/log.h>
#include <string.h>

#include <zmk/prospector_tb.h>

LOG_MODULE_REGISTER(prospector_tb_remote, CONFIG_ZMK_LOG_LEVEL);

/* 30-40 ms: fast enough that a command lands within one keyboard scan window */
#define TB_REMOTE_ADV_INT_MIN 0x0030
#define TB_REMOTE_ADV_INT_MAX 0x0040
/* Stop broadcasting when the screen sits untouched this long */
#define TB_REMOTE_IDLE_TIMEOUT K_SECONDS(120)

static K_MUTEX_DEFINE(tb_lock);
static struct prospector_tb_packet pkt;
static bool session_open;
static bool adv_running;

static struct bt_data tb_ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, &pkt, sizeof(pkt)),
};

static const struct bt_le_adv_param tb_adv_param =
    BT_LE_ADV_PARAM_INIT(0, TB_REMOTE_ADV_INT_MIN, TB_REMOTE_ADV_INT_MAX, NULL);

static void idle_timeout_handler(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(idle_work, idle_timeout_handler);

static int adv_apply_locked(void) {
    int err;

    if (adv_running) {
        err = bt_le_adv_update_data(tb_ad, ARRAY_SIZE(tb_ad), NULL, 0);
        if (err == 0) {
            return 0;
        }
        /* Advertising set may have been stopped underneath us; restart */
        bt_le_adv_stop();
        adv_running = false;
    }

    err = bt_le_adv_start(&tb_adv_param, tb_ad, ARRAY_SIZE(tb_ad), NULL, 0);
    if (err == 0 || err == -EALREADY) {
        adv_running = true;
        return 0;
    }
    LOG_WRN("tb remote adv start failed: %d", err);
    return err;
}

static void adv_stop_locked(void) {
    if (adv_running) {
        bt_le_adv_stop();
        adv_running = false;
    }
}

static void idle_timeout_handler(struct k_work *work) {
    ARG_UNUSED(work);
    k_mutex_lock(&tb_lock, K_FOREVER);
    if (session_open) {
        LOG_INF("tb remote idle - pausing broadcast");
        adv_stop_locked();
    }
    k_mutex_unlock(&tb_lock);
}

int prospector_tb_remote_begin(const uint8_t keyboard_id[4]) {
    int err;

    k_mutex_lock(&tb_lock, K_FOREVER);
    memset(&pkt, 0, sizeof(pkt));
    pkt.manufacturer_id[0] = 0xFF;
    pkt.manufacturer_id[1] = 0xFF;
    pkt.uuid[0] = PROSPECTOR_TB_UUID_0;
    pkt.uuid[1] = PROSPECTOR_TB_UUID_1;
    memcpy(pkt.keyboard_id, keyboard_id, 4);
    pkt.seq = (uint8_t)sys_rand32_get();
    pkt.cmd = PROSPECTOR_TB_CMD_HEARTBEAT;
    pkt.target = PROSPECTOR_TB_TARGET_ACTIVE;
    session_open = true;
    err = adv_apply_locked();
    k_mutex_unlock(&tb_lock);

    k_work_reschedule(&idle_work, TB_REMOTE_IDLE_TIMEOUT);
    LOG_INF("tb remote begin for %02X%02X%02X%02X", keyboard_id[0], keyboard_id[1],
            keyboard_id[2], keyboard_id[3]);
    return err;
}

int prospector_tb_remote_send(uint8_t cmd, uint8_t target, int8_t value) {
    int err;

    k_mutex_lock(&tb_lock, K_FOREVER);
    if (!session_open) {
        k_mutex_unlock(&tb_lock);
        return -ENOTCONN;
    }
    pkt.seq++;
    pkt.cmd = cmd;
    pkt.target = target;
    pkt.value = value;
    err = adv_apply_locked();
    k_mutex_unlock(&tb_lock);

    k_work_reschedule(&idle_work, TB_REMOTE_IDLE_TIMEOUT);
    LOG_DBG("tb remote cmd=%d target=%d value=%d seq=%d", cmd, target, value, pkt.seq);
    return err;
}

void prospector_tb_remote_end(void) {
    k_work_cancel_delayable(&idle_work);
    k_mutex_lock(&tb_lock, K_FOREVER);
    session_open = false;
    adv_stop_locked();
    k_mutex_unlock(&tb_lock);
    LOG_INF("tb remote end");
}

bool prospector_tb_remote_active(void) {
    return session_open && adv_running;
}
