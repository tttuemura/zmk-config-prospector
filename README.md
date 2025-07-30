# Prospector Scanner - ZMK Status Display Device

<p align="center">
  <img src="https://img.shields.io/badge/status-beta-orange" alt="Beta Status">
  <img src="https://img.shields.io/badge/ZMK-compatible-blue" alt="ZMK Compatible">
  <img src="https://img.shields.io/badge/license-MIT-green" alt="MIT License">
</p>

**Prospector Scanner** is an independent BLE status display device for ZMK keyboards. It provides real-time monitoring of keyboard status including battery levels, active layers, modifier keys, and connection states without interfering with your keyboard's normal connectivity.

## 🎯 Key Features

### 📱 **YADS-Style Professional UI**
- **Multi-Widget Display**: Connection status, layer indicators, modifier keys, and battery visualization
- **Split Keyboard Support**: Unified display showing both left and right side information
- **Color-Coded Status**: Green/Yellow/Red battery indicators, pastel layer colors
- **Real-time Updates**: Instant response to keyboard state changes

### 🔋 **Smart Power Management**
- **Activity-Based Intervals**: 5Hz (200ms) when typing, 1Hz (1000ms) when idle
- **Automatic Transitions**: Seamless switching between active/idle states
- **Minimal Impact**: ~25% battery consumption increase on keyboards
- **USB Powered Scanner**: No battery concerns for the display device

### 🎮 **Universal Compatibility**
- **Any ZMK Keyboard**: Works with split, unibody, or any ZMK-compatible device
- **Non-Intrusive**: Keyboards maintain full 5-device connectivity
- **Multi-Keyboard**: Monitor up to 3 keyboards simultaneously
- **No Pairing Required**: Uses BLE advertisements (observer mode)

## 🏗️ Architecture Overview

### Original vs Current Design

**❌ Original Design (Dongle Mode)**:
- Keyboard → Prospector → PC (USB/BLE)
- Limited keyboard to single device connection
- Required keyboard to connect through Prospector

**✅ Current Design (Independent Scanner)**:
- Keyboard → Multiple Devices (up to 5)
- Keyboard → Scanner (BLE Advertisement only)
- Scanner operates independently, no connection limits

### System Components

```
┌─────────────┐    BLE Adv     ┌──────────────┐
│   Keyboard  │ ──────────────→│   Scanner    │
│             │    26-byte     │   Display    │
│ (5Hz/1Hz)   │   Protocol     │  (USB Power) │
└─────────────┘                └──────────────┘
       │
       ├── Device 1 (PC)
       ├── Device 2 (Tablet)  
       ├── Device 3 (Phone)
       └── ... (up to 5 total)
```

## 🛠️ Hardware Requirements

### Scanner Device (Prospector Hardware)
This project uses the **Prospector** hardware design as the base platform:

- **MCU**: [Seeeduino XIAO BLE](https://www.seeedstudio.com/Seeed-XIAO-BLE-nRF52840-p-5201.html) (nRF52840)
- **Display**: Waveshare 1.69" Round LCD (ST7789V, 240x280 pixels)
- **Optional**: Adafruit APDS9960 ambient light sensor
- **Power**: USB Type-C (5V)
- **Case**: 3D printed Prospector enclosure
- **Hardware Design**: Based on original Prospector project specifications

### Display Wiring
```
LCD_DIN  -> Pin 10 (MOSI)
LCD_CLK  -> Pin 8  (SCK)
LCD_CS   -> Pin 9  (CS)
LCD_DC   -> Pin 7  (Data/Command)
LCD_RST  -> Pin 3  (Reset)
LCD_BL   -> Pin 6  (Backlight PWM)
```

### Supported Keyboards
- Any ZMK-compatible keyboard with BLE support
- Split keyboards (Corne, Lily58, Sofle, etc.)
- Unibody keyboards (60%, TKL, etc.)
- Custom ZMK builds

## 📦 Installation Guide

### Step 1: Build Scanner Firmware

#### Option A: GitHub Actions (Recommended)
1. Fork this repository: `https://github.com/t-ogura/zmk-config-prospector`
2. Enable GitHub Actions in your fork
3. Push any commit to trigger automated build
4. Download `prospector-scanner-firmware.zip` from Actions artifacts
5. Extract and flash `zmk.uf2` to your Seeeduino XIAO BLE

#### Option B: Local Build
```bash
# Clone repository
git clone https://github.com/t-ogura/zmk-config-prospector
cd zmk-config-prospector

# Initialize and build
west init -l config
west update
west build -s zmk/app -b seeeduino_xiao_ble -- -DSHIELD=prospector_scanner

# Flash the generated firmware
# File: build/zephyr/zmk.uf2
```

### Step 2: Add Scanner Support to Your Keyboard

#### A. Update west.yml
Add the prospector module to your keyboard's `config/west.yml`:

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
      revision: feature/yads-widget-integration
      path: modules/prospector-zmk-module
```

#### B. Configure Advertisement
Add to your keyboard's `.conf` file:

```kconfig
# Essential: Enable Prospector status advertisement
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"  # ≤8 characters

# Recommended: Activity-based power management for battery efficiency
CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y
CONFIG_ZMK_STATUS_ADV_ACTIVE_INTERVAL_MS=200    # 5Hz when active
CONFIG_ZMK_STATUS_ADV_IDLE_INTERVAL_MS=1000     # 1Hz when idle  
CONFIG_ZMK_STATUS_ADV_ACTIVITY_TIMEOUT_MS=2000  # 2s to idle transition

# Required for split keyboards: Enable peripheral battery fetching
CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING=y

# Recommended: Enable battery reporting if not already enabled
CONFIG_ZMK_BATTERY_REPORTING=y

# Required for proper BLE functionality (usually enabled by default)
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
```

#### C. Rebuild and Flash
1. Rebuild your keyboard firmware with the new configuration
2. Flash the updated firmware to your keyboard
3. Power on both scanner and keyboard

### Step 3: Verification

1. **Scanner Display**: Should show "Initializing..." then "Scanning..." initially
2. **Keyboard Detection**: Scanner displays keyboard name within 5 seconds
3. **Status Updates**: Battery, layer, and modifier information should update in real-time
4. **BLE Verification**: Use nRF Connect app to verify advertisements with manufacturer data `FF FF AB CD`

## 📊 Protocol Specification

### BLE Advertisement Format (26 bytes)

The scanner listens for BLE manufacturer data with this structure:

| Offset | Field | Size | Description | Example |
|--------|-------|------|-------------|---------|
| 0-1 | Manufacturer ID | 2 bytes | `0xFF 0xFF` (Local use) | `FF FF` |
| 2-3 | Service UUID | 2 bytes | `0xAB 0xCD` (Prospector ID) | `AB CD` |
| 4 | Protocol Version | 1 byte | Current version: `0x01` | `01` |
| 5 | Battery Level | 1 byte | 0-100% (Central/Main) | `5A` (90%) |
| 6 | Active Layer | 1 byte | Current layer 0-15 | `02` (Layer 2) |
| 7 | Profile Slot | 1 byte | BLE profile 0-4 | `01` (Profile 1) |
| 8 | Connection Count | 1 byte | Connected devices 0-5 | `03` (3 devices) |
| 9 | Status Flags | 1 byte | USB/BLE/Caps flags | `05` (USB+Caps) |
| 10 | Device Role | 1 byte | 0=Standalone, 1=Central, 2=Peripheral | `01` (Central) |
| 11 | Device Index | 1 byte | Split keyboard index | `00` |
| 12-14 | Peripheral Batteries | 3 bytes | Left/Right/Aux battery levels | `52 00 00` (82%, none, none) |
| 15-20 | Layer Name | 6 bytes | Layer identifier | `4C3000...` ("L0") |
| 21-24 | Keyboard ID | 4 bytes | Hash of keyboard name | `12345678` |
| 25 | Modifier Flags | 1 byte | CSAG modifier states | `05` (Ctrl+Alt) |

#### Status Flags Breakdown (Bit 9)
```
Bit 0: USB HID Ready
Bit 1: BLE Connected  
Bit 2: BLE Bonded
Bit 3: Caps Word Active
Bit 4: Charging Status
Bit 5-7: Reserved
```

#### Modifier Flags Breakdown (Bit 25)
```
Bit 0: Left Ctrl    Bit 4: Right Ctrl
Bit 1: Left Shift   Bit 5: Right Shift  
Bit 2: Left Alt     Bit 6: Right Alt
Bit 3: Left GUI     Bit 7: Right GUI
```

## 🎨 Display Features

### Widget Layout
```
┌─────────────────────────────┐
│         Device Name         │ ← Retro pixel font
│                    USB/BLE  │ ← Connection status
│                         3   │ ← Profile number
│                             │
│          Layer              │ ← Layer title
│    0  1  2  3  4  5  6      │ ← Pastel colored numbers
│                             │
│        󰘴  󰘶  󰘵              │ ← NerdFont modifier symbols
│                             │
│    ████████████ 85%         │ ← Color-coded battery bar
│    L: ████████ 78%          │ ← Split keyboard left side
└─────────────────────────────┘
```

### Visual Elements
- **Battery Colors**: Green (>60%), Yellow (20-60%), Red (<20%)
- **Layer Colors**: Each layer has unique pastel color, inactive layers in gray
- **Connection Status**: Color-coded USB (red/white) and BLE (green/blue/white)
- **Modifier Keys**: YADS-style NerdFont symbols for Ctrl/Shift/Alt/GUI

### Auto-Brightness
- **Ambient Light Sensor**: APDS9960 automatically adjusts display brightness
- **Range**: 10-100% based on 10-1000 lux light levels
- **Fallback**: Fixed 80% brightness if sensor unavailable

## 🔧 Configuration Options

### Scanner Device (`prospector_scanner.conf`)
```kconfig
# Core features
CONFIG_PROSPECTOR_MODE_SCANNER=y
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y  # Auto-brightness
CONFIG_PROSPECTOR_MAX_KEYBOARDS=2             # Multi-keyboard support

# Display settings
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS=80         # Fallback brightness %
CONFIG_PROSPECTOR_ROTATE_DISPLAY_180=n        # Display orientation

# Font support
CONFIG_LV_FONT_MONTSERRAT_12=y
CONFIG_LV_FONT_MONTSERRAT_18=y 
CONFIG_LV_FONT_MONTSERRAT_28=y
CONFIG_LV_FONT_UNSCII_16=y                    # Retro device name font
```

### Keyboard Integration
```kconfig
# Basic advertisement
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="YourBoard"  # ≤8 characters

# Power optimization (recommended)
CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y
CONFIG_ZMK_STATUS_ADV_ACTIVE_INTERVAL_MS=200     # 5Hz active
CONFIG_ZMK_STATUS_ADV_IDLE_INTERVAL_MS=1000      # 1Hz idle
CONFIG_ZMK_STATUS_ADV_ACTIVITY_TIMEOUT_MS=2000   # Idle after 2s

# Split keyboard support
CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING=y
```

## 🐛 Troubleshooting

### Scanner Issues

| Problem | Symptoms | Solution |
|---------|----------|----------|
| **No Display** | Black screen | • Check USB power connection<br>• Verify display wiring<br>• Reflash firmware |
| **"Scanning..." Forever** | No keyboard detection | • Check keyboard advertisement config<br>• Verify keyboard is powered on<br>• Use BLE scanner app to verify ads |
| **Garbled Display** | Corrupted text/graphics | • Check display SPI connections<br>• Verify display orientation setting<br>• Try different USB cable/power |

### Keyboard Issues

| Problem | Symptoms | Solution |
|---------|----------|----------|
| **No Advertisement** | Scanner never detects keyboard | • Add `CONFIG_ZMK_STATUS_ADVERTISEMENT=y`<br>• Rebuild and flash keyboard<br>• Check power management settings |
| **Battery Shows 0%** | Battery always zero | • Verify `CONFIG_ZMK_BATTERY_REPORTING=y`<br>• Check battery sensor wiring<br>• Test with USB power |
| **Name Too Long** | Build errors or truncation | • Limit keyboard name to 8 characters<br>• Use shorter identifier |

### Common BLE Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| **Intermittent Detection** | Power management | • Enable `CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y`<br>• Increase active interval frequency |
| **Multiple Keyboard Conflicts** | Same keyboard ID | • Use unique names for each keyboard<br>• Check `CONFIG_PROSPECTOR_MAX_KEYBOARDS` setting |
| **High Battery Drain** | Frequent advertisements | • Enable activity-based intervals<br>• Increase idle interval (1000ms+) |

### Debug Tools

#### BLE Advertisement Verification
Use **nRF Connect** (Android/iOS) to verify advertisements:
1. Open nRF Connect Scanner
2. Look for your keyboard name
3. Check "Raw Data" for manufacturer data starting with `FF FF AB CD`
4. Verify data updates at expected intervals

#### Scanner Debug Logging
Enable detailed logging in scanner config:
```kconfig
CONFIG_LOG=y
CONFIG_ZMK_LOG_LEVEL_DBG=y
```

## 📈 Performance Characteristics

### Power Consumption Impact
- **Keyboard Active Mode**: +20-30% battery usage (short periods)
- **Keyboard Idle Mode**: +15-25% battery usage (most of the time)  
- **Overall Impact**: ~25% increase in keyboard battery consumption
- **Scanner Device**: USB powered, no battery concerns

### Update Latency
- **Key Press Response**: <200ms (5Hz active mode)
- **Layer Changes**: Immediate (next advertisement)
- **Battery Updates**: 1-5 seconds depending on activity
- **Connection Changes**: 1-2 seconds

### BLE Range
- **Typical Indoor**: 5-10 meters
- **Line of Sight**: Up to 15 meters
- **Through Walls**: 2-5 meters
- **Interference**: May vary with WiFi/BLE traffic

## 🧾 Licenses and Attribution

### Project License
This project is licensed under the **MIT License**. See `LICENSE` file for details.

### Hardware License
This project uses the **Prospector** hardware design:
- **License**: MIT License
- **Original Design**: Multiple contributors to the Prospector project
- **Hardware Repository**: Various Prospector implementations
- **Usage**: Complete hardware design including PCB, case, and component selection

### Third-Party Components

#### ZMK Firmware
- **License**: MIT License
- **Source**: https://github.com/zmkfirmware/zmk
- **Usage**: Base firmware platform

#### YADS (Yet Another Dongle Screen)
- **License**: MIT License  
- **Source**: https://github.com/pashutk/yads
- **Attribution**: UI widget designs and NerdFont modifier symbols
- **Usage**: 
  - Modifier status widget layout and styling
  - NerdFont symbol mappings (Ctrl: 󰘴, Shift: 󰘶, Alt: 󰘵, GUI: 󰘳)
  - Connection status widget color scheme
  - LVGL widget architecture patterns

#### Original Prospector
- **Authors**: Multiple contributors
- **License**: MIT License
- **Usage**: Hardware design inspiration and display driver foundation

#### NerdFonts
- **License**: MIT License
- **Source**: https://www.nerdfonts.com/
- **Usage**: Modifier key symbols (20px and 40px variants)

### Font Licenses
- **Montserrat**: SIL Open Font License 1.1
- **Unscii**: Public Domain

## 🚀 Future Enhancements

### Planned Features
- [ ] **WPM (Words Per Minute)** widget with typing speed tracking
- [ ] **Advanced power profiling** with machine learning-based predictions
- [ ] **Custom themes** and color scheme support
- [ ] **Web configuration** interface via USB/BLE
- [ ] **Multiple scanner sync** for large setups

### Hardware Roadmap
- [ ] **Wireless charging** support for battery operation
- [ ] **E-ink display** variant for ultra-low power
- [ ] **Compact form factor** with smaller displays
- [ ] **RGB accent lighting** for status indication

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Setup
```bash
# Clone with submodules
git clone --recursive https://github.com/t-ogura/zmk-config-prospector
cd zmk-config-prospector

# Install dependencies
west init -l config
west update

# Build and test
west build -s zmk/app -b seeeduino_xiao_ble -- -DSHIELD=prospector_scanner
```

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/t-ogura/zmk-config-prospector/issues)
- **Discussions**: [GitHub Discussions](https://github.com/t-ogura/zmk-config-prospector/discussions)  
- **ZMK Community**: [ZMK Discord](https://discord.gg/8Y8y9u5)

## 📚 Additional Resources

- **ZMK Documentation**: https://zmk.dev/
- **Hardware Guide**: [Building Your Scanner](docs/hardware-guide.md)
- **Advanced Configuration**: [Configuration Reference](docs/configuration.md)
- **API Reference**: [Protocol Documentation](docs/protocol-spec.md)

---

**Prospector Scanner** - Making ZMK keyboard status visible and beautiful.

Built with ❤️ by the ZMK community.