# Prospector ZMK Configuration

This repository contains the ZMK configuration files for the Prospector keyboard scanner mode.

## Features

- **Scanner Mode**: Receives BLE advertisements from ZMK keyboards
- **Multi-keyboard Support**: Can monitor up to 3 keyboards simultaneously  
- **Display**: Shows keyboard status including battery, layer, and connection info
- **Hardware**: Based on Seeeduino XIAO BLE with ST7789V display

## Usage

1. Build the firmware using GitHub Actions
2. Download the `zmk.uf2` file from the build artifacts
3. Flash to your Seeeduino XIAO BLE device
4. Configure your ZMK keyboards to broadcast status advertisements

## Hardware Requirements

- Seeeduino XIAO BLE (nRF52840)
- 1.69" ST7789V LCD display
- Optional: APDS9960 ambient light sensor

## Configuration

The main configuration is in `config/prospector_scanner.conf` with the shield defined in the prospector-zmk-module.

## Building Locally

```bash
west init -l config
west update
west build -s zmk/app -b seeeduino_xiao_ble -- -DSHIELD=prospector_scanner
```

## Status Advertisement Protocol

The scanner listens for BLE advertisements containing:
- Battery level
- Active layer name
- Connection count
- Status flags (caps lock, charging, etc.)

Built with ZMK firmware and the Prospector ZMK module.
