# Trackball sensitivity link (tttuemura fork)

Branch `module/tb-sensitivity` = upstream v2.2.3 + a small scanner -> keyboard
link used by TrackBallPad_kbd (`FourDirSwithZmkv4`).

## Status advertisement (keyboard -> scanner), still 26 bytes

| field | before | now |
|---|---|---|
| `peripheral_battery[2]` (3rd aux battery) | aux2 battery | `tb_cursor` level (-8..8, `0x80` = n/a) |
| `wpm_value` | WPM | `tb_scroll` level (-8..8, `0x80` = n/a) |
| `status_flags` bit 6 | - | `TB_LINK`: keyboard is listening to a scanner |
| `status_flags` bit 7 | - | `TB_SCROLL`: scroll is the active target (scroll layer) |

Keyboards provide the values by implementing `zmk_status_adv_tb_fill()`
(weak default = not available) and call `zmk_status_advertisement_burst()`
on changes.

## Command advertisement (scanner -> keyboard)

Non-connectable manufacturer data `struct prospector_tb_packet`
(`include/zmk/prospector_tb.h`, 12 bytes, UUID `AB CE`), addressed by
`keyboard_id`. Sent only while the scanner's **Trackball** screen is open
(Main -> swipe to Quick Actions -> swipe again). Every packet is a heartbeat;
a command runs once per new `seq`. The broadcast pauses after 120 s without
touching the screen.

Scanner UI: the main screen shows `TB-C`/`TB-S` + level where WPM used to be.
The Trackball screen has a slider (-8..8), -/+ and BASE / SAVE / DEFAULT, and
follows the keyboard's active target (cursor / scroll) automatically.
