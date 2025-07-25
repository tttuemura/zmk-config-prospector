# Prospector ZMK Configuration

This repository contains the ZMK configuration files for the Prospector keyboard scanner mode.

## Features

- **Scanner Mode**: Receives BLE advertisements from ZMK keyboards
- **Multi-keyboard Support**: Can monitor up to 3 keyboards simultaneously  
- **Display**: Shows keyboard status including battery, layer, and connection info
- **Hardware**: Based on Seeeduino XIAO BLE with ST7789V display
- **Non-intrusive**: Keyboards maintain full multi-device connectivity (up to 5 devices)

## Quick Start

### 1. Build Scanner Device Firmware

1. Fork this repository
2. Enable GitHub Actions in your fork
3. Push a commit to trigger the build
4. Download `prospector-scanner-firmware.zip` from the build artifacts
5. Flash `zmk.uf2` to your Seeeduino XIAO BLE

### 2. Configure Your ZMK Keyboard

Add these lines to your keyboard's `.conf` file:

```kconfig
# Enable Status Advertisement
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_INTERVAL_MS=1000
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"
```

Then rebuild and flash your keyboard firmware.

### 3. Test the Connection

1. Power on both devices
2. The scanner should show "Scanning..." then display your keyboard info
3. Check the scanner display for battery level, layer name, and connection count

## Hardware Requirements

### Scanner Device
- Seeeduino XIAO BLE (nRF52840)
- 1.69" ST7789V LCD display (240x280 pixels)
- Optional: APDS9960 ambient light sensor

### Keyboard
- Any ZMK-compatible keyboard with BLE support
- Must include the prospector-zmk-module in your keyboard config

## Debugging & Troubleshooting

### Scanner Device Debug

1. **Check if scanner is working**:
   - Power on the scanner
   - You should see "Prospector Scanner" on the display
   - Status should show "Scanning..." or "Waiting for keyboards"

2. **Enable debug logging** (optional):
   ```kconfig
   CONFIG_LOG=y
   CONFIG_ZMK_LOG_LEVEL_DBG=y
   ```

3. **Check display issues**:
   - Verify ST7789V display wiring
   - Check if LVGL is properly initialized
   - Display should show text even without keyboard connection

### Keyboard Debug

1. **Verify advertisement is enabled**:
   ```kconfig
   CONFIG_ZMK_STATUS_ADVERTISEMENT=y
   ```

2. **Check keyboard name length**:
   - `CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME` must be ≤ 8 characters
   - Example: "MyBoard" (7 chars) ✅, "MyKeyboard" (10 chars) ❌

3. **Test with BLE scanner apps**:
   - Use nRF Connect (Android/iOS) or similar
   - Look for manufacturer data with bytes: `FF FF AB CD`
   - Should appear every 1000ms (default interval)

### Common Issues

| Problem | Solution |
|---------|----------|
| Scanner shows "Scanning..." forever | Check keyboard advertisement config |
| Display is blank | Verify ST7789V wiring and power |
| Multiple keyboards not detected | Check `CONFIG_PROSPECTOR_MAX_KEYBOARDS` |
| Battery level shows 0% | Verify keyboard battery sensor is working |

## Status Advertisement Protocol

The scanner listens for BLE advertisements containing:

| Field | Size | Description |
|-------|------|-------------|
| Manufacturer ID | 2 bytes | `0xFF 0xFF` (reserved for local use) |
| Service UUID | 2 bytes | `0xAB 0xCD` (Prospector identifier) |
| Version | 1 byte | Protocol version (currently 1) |
| Battery Level | 1 byte | 0-100% |
| Active Layer | 1 byte | 0-15 |
| Profile Slot | 1 byte | 0-4 |
| Connection Count | 1 byte | 0-5 |
| Status Flags | 1 byte | Caps/USB/Charging bits |
| Layer Name | 8 bytes | Null-terminated string |
| Keyboard ID | 4 bytes | Hash of keyboard name |
| Reserved | 8 bytes | For future use |

**Total: 31 bytes** (BLE advertisement limit)

## Building Locally

```bash
west init -l config
west update
west build -s zmk/app -b seeeduino_xiao_ble -- -DSHIELD=prospector_scanner
```

## Adding to Your Keyboard

To add status advertisement to your existing ZMK keyboard:

1. Add the prospector-zmk-module to your `west.yml`:
   ```yaml
   manifest:
     remotes:
       - name: zmkfirmware
         url-base: https://github.com/zmkfirmware
       - name: prospector
         url-base: https://github.com/t-ogura
     projects:
       - name: zmk
         remote: zmkfirmware
         revision: main
         import: app/west.yml
       - name: prospector-zmk-module
         remote: prospector
         revision: feature/scanner-mode
         path: modules/prospector-zmk-module
   ```

2. Add to your keyboard's `.conf` file:
   ```kconfig
   CONFIG_ZMK_STATUS_ADVERTISEMENT=y
   CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="YourBoard"
   ```

3. Rebuild and flash your keyboard firmware

## Architecture

- **Scanner Mode**: This repository builds the status display device
- **Keyboard Module**: The prospector-zmk-module adds advertisement capability to any ZMK keyboard
- **Non-intrusive**: Keyboards work normally without the scanner present
- **Multi-device**: Keyboards maintain all standard ZMK connectivity features

Built with ZMK firmware and the Prospector ZMK module.
# Trigger build
# Test backlight control fix
# Test correct backlight pin
# Test PWM brightness control
# Test BLE scanner functionality
# Fix initialization level error
