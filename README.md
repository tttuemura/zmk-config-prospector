# ZMK Config for Prospector Scanner

This repository contains the ZMK configuration for building Prospector Scanner firmware.

## What is Prospector Scanner?

Prospector Scanner is an independent status display device that shows information from ZMK keyboards without interfering with their normal operation. It receives keyboard status via BLE advertisements and displays:

- Current layer information
- Battery level
- Connection status
- Caps Word indicator
- Charging status

## Features

- **Independent Operation**: Works without affecting keyboard's ability to connect to multiple devices
- **Multiple Keyboard Support**: Can display status from up to 3 keyboards simultaneously
- **Automatic Brightness**: Uses ambient light sensor for optimal visibility
- **USB or Battery Powered**: Flexible power options

## Hardware Requirements

- Seeed Studio XIAO nRF52840
- Waveshare 1.69" Round LCD Display Module (with touch)
- Adafruit APDS9960 sensor (optional, for auto-brightness)
- 3D-printed case from [Prospector hardware repository](https://github.com/carrefinho/prospector)

## Quick Start

1. **Fork this repository**
2. **Enable GitHub Actions** in your forked repository
3. **Wait for the build** to complete
4. **Download the firmware** from the Actions artifacts
5. **Flash to your Prospector** hardware

## Keyboard Setup

For your keyboards to work with Prospector Scanner, they need to broadcast status information. Add this to your keyboard's ZMK configuration:

### 1. Add the module to your keyboard's `config/west.yml`:

```yaml
manifest:
  remotes:
    - name: zmkfirmware
      url-base: https://github.com/zmkfirmware
    - name: t-ogura
      url-base: https://github.com/t-ogura
  projects:
    - name: zmk
      remote: zmkfirmware
      revision: main
      import: app/west.yml
    - name: prospector-zmk-module
      remote: t-ogura
      revision: feature/scanner-mode
      path: prospector-zmk-module
  self:
    path: config
```

### 2. Add to your keyboard's `.conf` file:

```conf
# Enable Status Advertisement
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"
```

### 3. Build and flash your keyboard firmware as usual

## Customization

You can customize the scanner behavior by editing `config/prospector_scanner.conf`:

```conf
# Change maximum keyboards to track
CONFIG_PROSPECTOR_MAX_KEYBOARDS=5

# Disable ambient light sensor if not installed
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=n

# Enable debug logging
CONFIG_LOG=y
CONFIG_ZMK_LOG_LEVEL_DBG=y
```

## Usage

1. **Power on** your Prospector Scanner (via USB or battery)
2. **Enable advertisement** on your keyboards (must have the module installed)
3. **Automatic discovery** - Scanner will find and display keyboard status
4. **Multiple keyboards** - Cycles through or displays multiple keyboards

## Troubleshooting

### Scanner shows "Scanning..." but no keyboards found
- Ensure your keyboards have the prospector-zmk-module installed
- Check that `CONFIG_ZMK_STATUS_ADVERTISEMENT=y` is enabled
- Verify keyboards are powered on and within range

### Display is too dim/bright
- Check ambient light sensor installation
- Adjust `CONFIG_PROSPECTOR_FIXED_BRIGHTNESS` if not using sensor

### Build fails
- Verify your fork is up to date
- Check GitHub Actions logs for specific error messages
- Ensure all required files are present

## Contributing

This is a template repository. For issues with the scanner functionality, please report them in the [prospector-zmk-module repository](https://github.com/t-ogura/prospector-zmk-module).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.