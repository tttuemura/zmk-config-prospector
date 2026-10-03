/*
 * Prospector trackball sensitivity link (scanner -> keyboard)
 *
 * The scanner broadcasts this packet as non-connectable manufacturer data
 * while its trackball sensitivity screen is open. The keyboard listens
 * (low duty normally, continuously while a link is active), applies commands
 * addressed to its keyboard_id and reports state back in its status
 * advertisement (tb_cursor / tb_scroll / ZMK_STATUS_FLAG_TB_*).
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define PROSPECTOR_TB_UUID_0 0xAB
#define PROSPECTOR_TB_UUID_1 0xCE

enum prospector_tb_cmd {
    PROSPECTOR_TB_CMD_HEARTBEAT = 0, /* keep link alive, no change */
    PROSPECTOR_TB_CMD_SET_LEVEL = 1, /* value = level */
    PROSPECTOR_TB_CMD_TO_BASE = 2,
    PROSPECTOR_TB_CMD_SAVE = 3,
    PROSPECTOR_TB_CMD_TO_DEFAULT = 4,
    PROSPECTOR_TB_CMD_STEP = 5, /* value = delta */
};

#define PROSPECTOR_TB_TARGET_CURSOR 0
#define PROSPECTOR_TB_TARGET_SCROLL 1
#define PROSPECTOR_TB_TARGET_ACTIVE 0xFF

struct prospector_tb_packet {
    uint8_t manufacturer_id[2]; /* 0xFF 0xFF */
    uint8_t uuid[2];            /* 0xAB 0xCE */
    uint8_t keyboard_id[4];     /* target keyboard */
    uint8_t seq;                /* incremented for each new command */
    uint8_t cmd;                /* enum prospector_tb_cmd */
    uint8_t target;             /* PROSPECTOR_TB_TARGET_* */
    int8_t value;
} __packed;

/* Scanner side API (src/tb_remote.c) */
int prospector_tb_remote_begin(const uint8_t keyboard_id[4]);
int prospector_tb_remote_send(uint8_t cmd, uint8_t target, int8_t value);
void prospector_tb_remote_end(void);
bool prospector_tb_remote_active(void);
int prospector_tb_remote_last_error(void); /* last advertising error, 0 = ok */
