> ⚠️ **Important Notice (Aug 2025)**
>
> The release **v1.1.0** contains an incorrect I²C pin mapping and may not work
> with the original Prospector wiring.  
> Please use **v1.0.0** until this issue is fixed in the upcoming **v1.1.1** release.  
>
> Details: [Issue #1](https://github.com/t-ogura/zmk-config-prospector/issues/1)

# Prospector Scanner - ZMK Status Display Device

<p align="center">
  <img src="https://img.shields.io/badge/version-v1.1.0-brightgreen" alt="Version 1.1.0">
  <img src="https://img.shields.io/badge/status-stable-green" alt="Stable Status">
  <img src="https://img.shields.io/badge/ZMK-compatible-blue" alt="ZMK Compatible">
  <img src="https://img.shields.io/badge/license-MIT-green" alt="MIT License">
</p>

## 🔍 **What is Prospector?**

**Prospector** is a community-developed hardware platform designed for ZMK keyboard enhancement and monitoring. Originally created as a universal dongle solution, Prospector has evolved into multiple specialized modes:

- **🎯 Original Prospector**: Universal ZMK keyboard dongle for USB/wireless connectivity
  - **Original Project**: [prospector](https://github.com/carrefinho/prospector) by carrefinho
  - **Original Firmware**: [prospector-zmk-module](https://github.com/carrefinho/prospector-zmk-module)
  - **Hardware Platform**: Seeeduino XIAO BLE + Waveshare 1.69" Round LCD display
  - **Community Project**: Multiple implementations and variants across the ZMK ecosystem
  - **Open Source**: MIT licensed hardware design with 3D-printable case

- **📱 This Project (Prospector Scanner)**: Advanced status monitoring system
  - Independent BLE scanner mode using Prospector hardware
  - Professional YADS-style UI for real-time keyboard monitoring
  - Non-intrusive design that preserves full ZMK keyboard connectivity
  - **v1.1.0**: Scanner battery support, enhanced power management, improved stability

**Prospector Scanner** transforms the Prospector hardware into an independent BLE status display device for ZMK keyboards. It provides real-time monitoring of keyboard status including battery levels, active layers, modifier keys, and connection states without interfering with your keyboard's normal connectivity.

## 🎯 Key Features

### 📱 **YADS-Style Professional UI**
- **Multi-Widget Display**: Connection status, layer indicators, modifier keys, WPM tracking, and battery visualization
- **Split Keyboard Support**: Unified display showing both left and right side information with configurable positioning
- **Color-Coded Status**: 5-level battery indicators (Green/Light Green/Yellow/Orange/Red), unique pastel layer colors
- **Real-time Updates**: Instant response to keyboard state changes with sub-second latency
- **WPM Tracking**: Real-time Words Per Minute calculation with intelligent decay during idle periods

### 🔋 **Smart Power Management** (v1.1.0 Enhanced)
- **Activity-Based Intervals**: 10Hz (100ms) when typing, 0.03Hz (30s) when idle (improved from v1.0.0)
- **Automatic Transitions**: Seamless switching between active/idle states with configurable timeouts
- **WPM-Aware Updates**: Higher frequency during active typing sessions with decay algorithm
- **Scanner Battery Support**: Real-time battery monitoring for scanner device with charging indicator
- **Adaptive Brightness**: Automatic dimming when keyboards go idle to save battery
- **USB/Battery Profiles**: Different brightness and power settings for USB vs battery operation

### 🎮 **Universal Compatibility**
- **Any ZMK Keyboard**: Works with split, unibody, or any ZMK-compatible device
- **Non-Intrusive**: Keyboards maintain full 5-device connectivity
- **Multi-Keyboard**: Monitor up to 3 keyboards simultaneously
- **No Pairing Required**: Uses BLE advertisements (observer mode)

### 🌟 **New in v1.1.0**
- **Scanner Battery Widget**: Displays scanner's own battery level and charging status
- **Ambient Light Sensor**: Automatic brightness adjustment with APDS9960 (fully functional)
- **Enhanced Power Profiles**: Separate USB/battery brightness settings
- **Improved Split Display**: Configurable left/right battery positioning
- **Production-Ready**: Optimized update intervals and removed debug features

## 🏗️ Architecture Overview

### System Design

**✅ Current Design (Independent Scanner)**:
- Keyboard → Multiple Devices (up to 5)
- Keyboard → Scanner (BLE Advertisement only)
- Scanner operates independently, no connection limits

### System Components

```
┌─────────────┐    BLE Adv     ┌──────────────┐
│   Keyboard  │ ──────────────→│   Scanner    │
│             │    26-byte     │   Display    │
│ (10Hz/0.03Hz)│   Protocol    │  (USB/Battery)│
└─────────────┘                └──────────────┘
       │
       ├── Device 1 (PC)
       ├── Device 2 (Tablet)  
       ├── Device 3 (Phone)
       └── ... (up to 5 total)
```

## 🛠️ Hardware Requirements

### Scanner Device (Prospector Hardware)
- **MCU**: [Seeeduino XIAO BLE](https://www.seeedstudio.com/Seeed-XIAO-BLE-nRF52840-p-5201.html) (nRF52840)
- **Display**: Waveshare 1.69" Round LCD (ST7789V, 240x280 pixels)
- **Light Sensor**: Adafruit APDS9960 ambient light sensor (optional, for auto-brightness)
- **Battery**: 702030 size (350mAh) recommended, or 772435 size (600mAh) tight fit (optional, for portable operation)
- **Power**: USB Type-C (5V) or battery
- **Case**: 3D printed Prospector enclosure

### Display Wiring
```
LCD_DIN  -> Pin 10 (MOSI)
LCD_CLK  -> Pin 8  (SCK)
LCD_CS   -> Pin 9  (CS)
LCD_DC   -> Pin 7  (Data/Command)
LCD_RST  -> Pin 3  (Reset)
LCD_BL   -> Pin 6  (Backlight PWM)
```

### APDS9960 Wiring (v1.1.0)
```
SDA -> Pin D4
SCL -> Pin D5
VCC -> 3.3V
GND -> GND
```

### Supported Keyboards
- Any ZMK-compatible keyboard with BLE support
- Split keyboards (Corne, Lily58, Sofle, etc.)
- Unibody keyboards (60%, TKL, etc.)
- Custom ZMK builds

## 📦 Installation Guide

### Step 1: Get Scanner Firmware

#### Option A: Download Pre-Built (Easiest)
📥 **[Download v1.1.0 Release](https://github.com/t-ogura/zmk-config-prospector/releases/tag/v1.1.0)**
- `prospector_scanner-seeeduino_xiao_ble-zmk.uf2` - Scanner mode firmware
- `settings_reset-seeeduino_xiao_ble-zmk.uf2` - Settings reset firmware
- Simply download and flash to your Seeeduino XIAO BLE

#### Option B: Build Your Own (Recommended for Customization)
1. Fork this repository: `https://github.com/t-ogura/zmk-config-prospector`
   - Use the `main` branch for stable v1.1.0 release
2. Enable GitHub Actions in your fork
3. Push any commit to trigger automated build
4. Download `zmk.uf2` from your Actions artifacts
5. Flash to your Seeeduino XIAO BLE

#### Option C: Local Build
```bash
# Clone repository (main branch for v1.1.0)
git clone https://github.com/t-ogura/zmk-config-prospector
cd zmk-config-prospector
git checkout main

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
      revision: v1.1.0
      path: modules/prospector-zmk-module
```

#### B. Configure Advertisement
Add to your keyboard's `.conf` file:

```kconfig
# ========================================
# ESSENTIAL CONFIGURATION (Required)
# ========================================
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"  # Display name

# Required for proper BLE functionality
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y

# Required for split keyboards: Enable peripheral battery fetching
CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING=y

# Enable battery reporting
CONFIG_ZMK_BATTERY_REPORTING=y

# ========================================
# POWER OPTIMIZATION (v1.1.0 Enhanced - 15x improvement from v1.0)
# ========================================
CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y
# 10Hz when typing (ultra-responsive, was 0.5Hz in v1.0)
CONFIG_ZMK_STATUS_ADV_ACTIVE_INTERVAL_MS=100
# 0.03Hz when idle (major battery savings, was 0.5Hz in v1.0)
CONFIG_ZMK_STATUS_ADV_IDLE_INTERVAL_MS=30000
# 10 seconds before idle mode (configurable)
CONFIG_ZMK_STATUS_ADV_ACTIVITY_TIMEOUT_MS=10000

# ========================================
# SPLIT KEYBOARD CONFIGURATION (Optional)
# ========================================
# Specify which side is the central device (default: RIGHT)
# Set to "LEFT" if your central device is on the left side
# CONFIG_ZMK_STATUS_ADV_CENTRAL_SIDE="LEFT"

# ========================================
# WPM TRACKING CONFIGURATION (v1.1.0 New)
# ========================================
# Configure WPM calculation window (default: 30 seconds)
# Ultra-responsive: 10s window (6x multiplier)
# CONFIG_ZMK_STATUS_ADV_WPM_WINDOW_SECONDS=10
# Balanced: 30s window (2x multiplier) - DEFAULT
# CONFIG_ZMK_STATUS_ADV_WPM_WINDOW_SECONDS=30
# Traditional: 60s window (1x multiplier)
# CONFIG_ZMK_STATUS_ADV_WPM_WINDOW_SECONDS=60

# ========================================
# DISPLAY CUSTOMIZATION (Optional)
# ========================================
# Adjust scanner's layer display (default: 7 layers, 0-6)
CONFIG_PROSPECTOR_MAX_LAYERS=7

# Stop advertisements in deep sleep (max battery save)
# CONFIG_ZMK_STATUS_ADV_STOP_ON_SLEEP=y

# ========================================
# DEBUGGING (Development Only)
# ========================================
# CONFIG_LOG=y
# CONFIG_ZMK_LOG_LEVEL_DBG=y
```

#### C. Rebuild and Flash
1. Rebuild your keyboard firmware with the new configuration
2. Flash the updated firmware to your keyboard
3. Power on both scanner and keyboard

### Step 3: Verification

1. **Scanner Display**: Shows "Initializing..." then "Scanning..."
2. **Keyboard Detection**: Scanner displays keyboard name within 5 seconds
3. **Status Updates**: Battery, layer, WPM update in real-time
4. **Scanner Battery**: If battery connected, shows level in top-right (v1.1.0)

## 📊 Protocol Specification

### BLE Advertisement Format (26 bytes)

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
| 15-18 | Layer Name | 4 bytes | Layer identifier | `4C30...` ("L0") |
| 19-22 | Keyboard ID | 4 bytes | Hash of keyboard name | `12345678` |
| 23 | Modifier Flags | 1 byte | L/R Ctrl,Shift,Alt,GUI | `05` (LCtrl+LShift) |
| 24 | WPM Value | 1 byte | Words per minute (0-255) | `3C` (60 WPM) |
| 25 | Reserved | 1 byte | Future expansion | `00` |

## 🎨 Display Features

### Widget Layout (v1.1.0)
```
┌─────────────────────────────┐
│ 🔋85%  Device Name          │ ← Scanner battery (v1.1.0)
│                    USB [P0] │ ← Connection status
│                             │
│ WPM                    RX   │ ← WPM + Signal info
│ 045             -45dBm 10Hz │
│                             │
│          Layer              │ ← Layer title
│    0  1  2  3  4  5  6      │ ← Pastel colored layers
│                             │
│        󰘴  󰘶  󰘵              │ ← Modifier symbols
│                             │
│    ████████████ 85%         │ ← Keyboard battery
│    ████████████ 78%         │ ← Split keyboard
└─────────────────────────────┘
```

### Visual Elements
- **5-Level Battery Colors**: 
  - 🟢 **80%+**: Green (#00CC66)
  - 🟡 **60-79%**: Light Green (#66CC00)  
  - 🟡 **40-59%**: Yellow (#FFCC00)
  - 🟠 **20-39%**: Orange (#FF9900)
  - 🔴 **<20%**: Red (#FF3333)
- **Layer Colors**: 7-10 unique pastel colors, configurable count
- **WPM Display**: Real-time with decay algorithm
- **Scanner Battery**: Top-right with charging indicator (v1.1.0)

### Display Brightness (v1.1.0 Enhanced)
- **Auto-Brightness**: APDS9960 sensor with non-linear response
- **USB/Battery Profiles**: Different max brightness for each power source
- **Activity-Based Dimming**: Automatic reduction when keyboards idle
- **Smooth Transitions**: Configurable fade duration and steps

## 🔧 Configuration Options

### Scanner Device (`prospector_scanner.conf`)
```kconfig
# ========================================
# CORE SCANNER FEATURES
# ========================================
CONFIG_PROSPECTOR_MODE_SCANNER=y
CONFIG_PROSPECTOR_MAX_KEYBOARDS=3

# ========================================
# BATTERY SUPPORT (v1.1.0 - Optional)
# ========================================
# ⚠️  DISABLED BY DEFAULT - See "Battery Operation Setup" section below
# CONFIG_PROSPECTOR_BATTERY_SUPPORT=y
# CONFIG_ZMK_BATTERY_REPORTING=y
# CONFIG_USB_DEVICE_STACK=y
# CONFIG_PROSPECTOR_BATTERY_UPDATE_INTERVAL_S=120  # 2 minutes

# ========================================
# DISPLAY SETTINGS (v1.1.0 Enhanced)
# ========================================
# Auto-brightness with APDS9960
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y
CONFIG_SENSOR=y
CONFIG_APDS9960=y
CONFIG_I2C=y

# Brightness ranges (USB vs Battery)
CONFIG_PROSPECTOR_ALS_MIN_BRIGHTNESS=5          # 5% minimum
CONFIG_PROSPECTOR_ALS_MAX_BRIGHTNESS_BATTERY=50 # 50% max on battery
CONFIG_PROSPECTOR_ALS_MAX_BRIGHTNESS_USB=90     # 90% max on USB

# Fixed brightness when ALS disabled
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS_BATTERY=40   # 40% on battery
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS_USB=85       # 85% on USB

# Light sensor tuning
CONFIG_PROSPECTOR_ALS_UPDATE_INTERVAL_MS=2000   # 2 second updates
CONFIG_PROSPECTOR_ALS_SENSOR_THRESHOLD=200      # Indoor-optimized

# ========================================
# POWER SAVING FEATURES (v1.1.0)
# ========================================
# Scanner timeout (8 minutes default)
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_MS=480000

# Auto-dim when keyboards idle
CONFIG_PROSPECTOR_SCANNER_IDLE_BRIGHTNESS_MS=120000    # 2 minutes
CONFIG_PROSPECTOR_SCANNER_IDLE_BRIGHTNESS_PERCENT=20   # 20% brightness

# Advertisement frequency dimming
CONFIG_PROSPECTOR_ADVERTISEMENT_FREQUENCY_DIM=y
CONFIG_PROSPECTOR_ADV_FREQUENCY_DIM_THRESHOLD_MS=2000  # 2 seconds
CONFIG_PROSPECTOR_ADV_FREQUENCY_DIM_BRIGHTNESS=25      # 25% brightness

# ========================================
# LAYER DISPLAY
# ========================================
CONFIG_PROSPECTOR_MAX_LAYERS=7                   # Show layers 0-6

# ========================================
# FONTS (Required)
# ========================================
CONFIG_LV_FONT_MONTSERRAT_12=y
CONFIG_LV_FONT_MONTSERRAT_16=y
CONFIG_LV_FONT_MONTSERRAT_18=y
CONFIG_LV_FONT_MONTSERRAT_20=y
CONFIG_LV_FONT_MONTSERRAT_22=y
CONFIG_LV_FONT_MONTSERRAT_24=y
CONFIG_LV_FONT_MONTSERRAT_28=y
CONFIG_LV_FONT_UNSCII_8=y
CONFIG_LV_FONT_UNSCII_16=y
```

## 🔋 Battery Operation Setup

### 🔌 USB-Only Operation (Default)

**Prospector Scanner works perfectly without battery hardware.** By default, the scanner is configured for USB-only operation:

- ✅ **Always powered**: Stable power from USB
- ✅ **Simple setup**: No additional hardware needed
- ✅ **Lower cost**: No battery purchase required
- ✅ **Maintenance-free**: No battery charging management

### 🔋 Battery Operation (Optional)

If you want portable operation, follow these steps to enable battery support:

#### Hardware Requirements
- **Recommended Battery**: 702030 size (350mAh) - fits perfectly in Prospector case
- **Alternative**: 772435 size (600mAh) - fits but tight clearance
- **Connector**: Standard JST connector (red=positive, black=negative)
- **Connection**: Connect to BAT+ and BAT- pins on Seeeduino XIAO BLE
- **Charging**: USB-C port provides automatic charging when connected

#### Software Configuration

**Step 1: Enable Battery Support in Scanner Config**
Edit `config/prospector_scanner.conf` and uncomment the following lines:

```kconfig
# Enable battery support
CONFIG_PROSPECTOR_BATTERY_SUPPORT=y
CONFIG_ZMK_BATTERY_REPORTING=y
CONFIG_USB_DEVICE_STACK=y

# Battery update interval (2 minutes)
CONFIG_PROSPECTOR_BATTERY_UPDATE_INTERVAL_S=120

# Optional: Show percentage and position
CONFIG_PROSPECTOR_BATTERY_SHOW_PERCENTAGE=y
CONFIG_PROSPECTOR_BATTERY_WIDGET_POSITION="TOP_RIGHT"
```

**Step 2: Rebuild and Flash**
- Rebuild the scanner firmware with battery support enabled
- Flash the new firmware to your scanner device

**Step 3: Verify Battery Operation**
- Connect battery hardware
- Power on without USB - scanner should boot from battery
- Battery level widget should appear in top-right corner
- USB charging indicator shows when USB is connected

#### Battery Management Features

When battery support is enabled, you get:

- 📊 **Real-time Battery Level**: Percentage display in top-right corner
- ⚡ **Charging Indicator**: Shows charging status when USB connected  
- 🔋 **Power-Aware Brightness**: Lower max brightness on battery vs USB
- 💾 **Activity-Based Power Saving**: Dims display when keyboards idle
- 🔄 **Automatic Detection**: Works with or without battery hardware

#### Power Consumption (350mAh Battery)

- **Active Mode**: 4-6 hours typical usage (bright display, frequent updates)
- **Idle Mode**: 8-12 hours (dimmed display, reduced update frequency)  
- **Mixed Usage**: 6-8 hours typical daily use
- **Charging**: 1.5-2 hours from empty to full via USB-C
- **600mAh Battery**: Add 60-70% to above times for larger battery option

#### Recommended Battery Settings

```kconfig
# Optimized for battery life
CONFIG_PROSPECTOR_ALS_MAX_BRIGHTNESS_BATTERY=50  # 50% max on battery
CONFIG_PROSPECTOR_ALS_MAX_BRIGHTNESS_USB=90      # 90% max on USB
CONFIG_PROSPECTOR_SCANNER_IDLE_BRIGHTNESS_PERCENT=20  # 20% when idle

# Faster display dimming for power saving
CONFIG_PROSPECTOR_SCANNER_IDLE_BRIGHTNESS_MS=120000  # 2 minutes
CONFIG_PROSPECTOR_ADVERTISEMENT_FREQUENCY_DIM=y     # Frequency-based dimming
```

### 🛠️ Troubleshooting Battery Issues

| Issue | Solution |
|-------|----------|
| **No Battery Widget** | • Verify CONFIG_PROSPECTOR_BATTERY_SUPPORT=y<br>• Check battery connection<br>• Ensure ZMK_BATTERY_REPORTING=y |
| **0% Battery Display** | • Check JST connector polarity (red=+, black=-)<br>• Verify battery voltage (>3.0V)<br>• Try different battery<br>• Recommended: 702030 (350mAh) |
| **No Charging** | • Check USB-C cable and connection<br>• Battery may be over-discharged<br>• Try leaving plugged for 30+ minutes |
| **Short Battery Life** | • Reduce max brightness settings<br>• Enable idle dimming<br>• Check for stuck-on backlight |

## 🐛 Troubleshooting

### Scanner Issues

| Problem | Symptoms | Solution |
|---------|----------|----------|
| **No Display** | Black screen | • Check USB power<br>• Verify display wiring<br>• Reflash firmware |
| **"Scanning..." Forever** | No keyboards detected | • Check keyboard CONFIG_ZMK_STATUS_ADVERTISEMENT=y<br>• Verify keyboard power<br>• Use BLE scanner app |
| **No Scanner Battery** | Widget not shown | • See "Battery Operation Setup" section<br>• Enable CONFIG_PROSPECTOR_BATTERY_SUPPORT=y<br>• Check battery hardware connection |
| **Dim Display** | Too dark/bright | • Adjust ALS_SENSOR_THRESHOLD<br>• Check APDS9960 wiring<br>• Try fixed brightness mode |

### Keyboard Issues

| Problem | Symptoms | Solution |
|---------|----------|----------|
| **No Advertisement** | Scanner can't find | • Add CONFIG_ZMK_STATUS_ADVERTISEMENT=y<br>• Set activity-based intervals<br>• Check BLE is enabled |
| **High Battery Drain** | Fast discharge | • Enable activity-based mode<br>• Increase idle interval to 30000ms<br>• Use v1.1.0 power settings |
| **Wrong Battery Side** | L/R swapped | • Set CONFIG_ZMK_STATUS_ADV_CENTRAL_SIDE="LEFT"<br>• Default is "RIGHT" |

### Debug Tools

#### BLE Advertisement Verification
Use **nRF Connect** (Android/iOS):
1. Look for your keyboard name
2. Check manufacturer data: `FF FF AB CD`
3. Verify update intervals match config

#### Scanner Debug (v1.1.0)
```kconfig
# Enable debug logging
CONFIG_LOG=y
CONFIG_ZMK_LOG_LEVEL_DBG=y

# Battery investigation (development only)
CONFIG_PROSPECTOR_DEBUG_WIDGET=y
```

## 📈 Performance Characteristics

### Power Consumption (v1.1.0 Optimized)
- **Keyboard Active**: +20-30% battery (100ms intervals)
- **Keyboard Idle**: +10-15% battery (30s intervals)
- **Scanner on Battery**: 8-12 hours typical operation
- **Scanner on USB**: Unlimited with charging

### Update Latency
- **Key Press Response**: <200ms (10Hz in v1.1.0)
- **Layer Changes**: Immediate
- **Battery Updates**: 2 minutes (configurable)
- **WPM Updates**: Real-time with decay

### BLE Range
- **Typical Indoor**: 5-10 meters
- **Line of Sight**: Up to 15 meters
- **Through Walls**: 2-5 meters

## 🧾 Licenses and Attribution

### Project License
This project is licensed under the **MIT License**. See `LICENSE` file for details.

### Third-Party Components

#### ZMK Firmware
- **License**: MIT License
- **Source**: https://github.com/zmkfirmware/zmk

#### YADS (Yet Another Dongle Screen)
- **License**: MIT License  
- **Source**: https://github.com/janpfischer/zmk-dongle-screen
- **Attribution**: UI widget designs and NerdFont modifier symbols

#### NerdFonts
- **License**: MIT License
- **Source**: https://www.nerdfonts.com/
- **Usage**: Modifier key symbols

### Font Licenses
- **Montserrat**: SIL Open Font License 1.1
- **Unscii**: Public Domain

## 🚀 Future Enhancements

### Completed in v1.1.0 ✅
- [x] Scanner battery support with charging indicator
- [x] APDS9960 ambient light sensor integration
- [x] USB/Battery separate brightness profiles
- [x] Enhanced power management with activity dimming
- [x] Improved WPM calculation with decay
- [x] Production-ready optimizations

### Planned Features 🚧
- [ ] E-ink display variant for ultra-low power
- [ ] Web configuration interface
- [ ] Custom themes and color schemes
- [ ] Machine learning power prediction
- [ ] Wireless charging support

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

### Development Setup
```bash
# Clone with submodules
git clone --recursive https://github.com/t-ogura/zmk-config-prospector
cd zmk-config-prospector

# Initialize build environment
west init -l config
west update

# Build and test
west build -s zmk/app -b seeeduino_xiao_ble -- -DSHIELD=prospector_scanner
```

## 💡 Quick Setup Examples

### Split Keyboard (v1.1.0 Optimized)
```kconfig
# Corne, Lily58, Sofle, etc.
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="Corne"
CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING=y

# v1.1.0 enhanced power optimization (15x improvement from v1.0)
CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y
CONFIG_ZMK_STATUS_ADV_ACTIVE_INTERVAL_MS=100    # 10Hz active (was 0.5Hz)
CONFIG_ZMK_STATUS_ADV_IDLE_INTERVAL_MS=30000    # 0.03Hz idle (was 0.5Hz)
CONFIG_ZMK_STATUS_ADV_ACTIVITY_TIMEOUT_MS=10000 # 10s timeout

# WPM tracking (v1.1.0 new feature)
CONFIG_ZMK_STATUS_ADV_WPM_WINDOW_SECONDS=30     # 30s window (configurable)

# If central is on left side (uncommon)
# CONFIG_ZMK_STATUS_ADV_CENTRAL_SIDE="LEFT"
```

### 60% Keyboard
```kconfig
CONFIG_ZMK_STATUS_ADVERTISEMENT=y  
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="GH60"
CONFIG_ZMK_BATTERY_REPORTING=y

# Fewer layers
CONFIG_PROSPECTOR_MAX_LAYERS=4
```

### High-Layer Keyboard
```kconfig
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="Planck"

# Display 10 layers (0-9)
CONFIG_PROSPECTOR_MAX_LAYERS=10
```

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/t-ogura/zmk-config-prospector/issues)
- **Discussions**: [GitHub Discussions](https://github.com/t-ogura/zmk-config-prospector/discussions)  
- **ZMK Community**: [ZMK Discord](https://discord.gg/8Y8y9u5)

## 📚 Version History

### v1.1.0 (2025-08-08)
- ✨ Scanner battery support with real-time monitoring
- ✨ APDS9960 ambient light sensor integration (enabled by default)
- ✨ USB/Battery separate brightness profiles
- ✨ Enhanced power management (30s idle intervals vs 2s in v1.0)
- ✨ Improved WPM calculation with configurable window (5-120s)
- ✨ Enhanced RX rate display with 10-sample smoothing and accurate IDLE detection
- 🐛 Fixed scanner battery update stuck at 23%
- 🐛 Fixed RX showing 5Hz during IDLE mode (now shows actual 0.03Hz)
- 🐛 Fixed display timeout and reset issues
- 🔧 Removed debug features for production
- 📚 Comprehensive documentation update with battery specifications

### v1.0.0 (2025-01-29)
- 🎉 Initial stable release
- ✨ YADS-style UI with pastel colors
- ✨ Multi-keyboard support
- ✨ Split keyboard battery display
- ✨ Real-time status monitoring
- ✨ Activity-based power management

---

**Prospector Scanner v1.1.0** - Making ZMK keyboard status visible, beautiful, and battery-efficient.

Built with ❤️ by the ZMK community.
