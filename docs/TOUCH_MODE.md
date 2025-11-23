# Prospector Scanner Touch Mode Guide

**Version**: v2.0.0
**Last Updated**: November 20, 2025

## 📋 Table of Contents

1. [What's Different?](#whats-different) ⚠️ **Start Here**
2. [Quick Start](#quick-start)
3. [Configuration Reference](#configuration-reference)
4. [Hardware Requirements](#hardware-requirements)
5. [Complete Wiring Guide](#complete-wiring-guide)
6. [Screen Guide](#screen-guide)
7. [Troubleshooting](#troubleshooting)

---

## ⚠️ What's Different?

**IMPORTANT**: Touch Mode requires **additional wiring** compared to standard Prospector Scanner.

### Key Differences from Non-Touch Mode

| Feature | Non-Touch Mode | Touch Mode |
|---------|---------------|------------|
| **Wiring** | 6 display pins + VCC/GND | **+4 touch pins required** |
| **Build Configuration** | `prospector_scanner.conf` | `prospector_scanner_touch.conf` |
| Display Settings | Kconfig only | Interactive screen |
| Gestures | Not supported | 4-direction swipes |
| Firmware Size | ~900KB | ~920KB (+20KB) |
| On-Device Adjustment | Not supported | Yes (max layers, etc.) |

### ⚠️ Additional Wiring Required

Touch Mode adds **4 extra connections** to the standard Prospector Scanner wiring:

```
Standard Scanner: 6 pins (LCD_DIN, LCD_CLK, LCD_CS, LCD_DC, LCD_RST, LCD_BL) + Power
Touch Mode: +4 pins (TP_SDA, TP_SCL, TP_INT, TP_RST)
Total: 10 signal pins + VCC/GND
```

**You cannot use touch firmware without these 4 additional connections.**

### What Touch Mode Enables

- ✅ **Swipe Gestures**: Navigate between screens with 4-direction swipes
- ✅ **Display Settings Screen**: Adjust max layers without rebuilding firmware
- ✅ **On-Device Configuration**: Change settings in real-time
- ✅ **Future Expandability**: More interactive features in upcoming releases

---

## Quick Start

### Step 1: Verify Hardware

**How to Identify Touch Display**:
- Product page mentions "CST816S" or "touch sensor"
- Has **extra 4-pin connector** for touch (TP_RST, TP_INT, TP_SDA, TP_SCL)
- Product name includes "Touch" - e.g., "1.69inch Touch LCD"
- [Waveshare Official Link](https://www.waveshare.com/1.69inch-touch-lcd.htm)

### Step 2: Add Touch Wiring (4 Additional Pins)

**⚠️ These 4 connections are REQUIRED in addition to standard scanner wiring:**

| Touch Pin | XIAO BLE Pin | Description |
|-----------|--------------|-------------|
| TP_SDA | D4 (P0.04) | I2C Data |
| TP_SCL | D5 (P0.05) | I2C Clock |
| TP_INT | D0 (P0.02) | Interrupt |
| TP_RST | D1 (P0.28) | Reset |

**Note**: If you already have standard Prospector Scanner wired (6 display pins), just add these 4 touch pins. See [Complete Wiring Guide](#complete-wiring-guide) for full pinout.

### Step 3: Build Touch Firmware

#### Option A: GitHub Actions (Recommended)

**Step 3.1: Fork Repository**

1. Go to [zmk-config-prospector](https://github.com/t-ogura/zmk-config-prospector)
2. Click **"Fork"** button (top-right)
3. Wait for fork to complete

**Step 3.2: Enable GitHub Actions**

1. In your forked repository, go to **"Actions"** tab
2. Click **"I understand my workflows, enable them"**
3. GitHub Actions is now ready to build firmware

**Step 3.3: Download Firmware**

By default, GitHub Actions builds **both** non-touch and touch firmware.

**No configuration needed** - both firmware files will be available in the build artifacts:
- `prospector_scanner-seeeduino_xiao_ble-zmk.uf2` - Non-touch mode
- `prospector_scanner-seeeduino_xiao_ble-zmk.uf2` (with touch config) - Touch mode

**Optional: Build Only Touch Mode**

If you want to build ONLY touch firmware (saves build time):

1. Click `build.yaml` file in your fork
2. Click **pencil icon** (Edit)
3. Find and comment out the non-touch entry:

```yaml
---
include:
  # Non-touch mode - comment out if you only need touch firmware
  # - board: seeeduino_xiao_ble
  #   shield: prospector_scanner

  # Touch mode (keep this)
  - board: seeeduino_xiao_ble
    shield: prospector_scanner
    cmake-args: -DCONF_FILE=prospector_scanner_touch.conf
```

4. Click **"Commit changes"** → **"Commit directly to main branch"**

**Step 3.4: Trigger and Download Build**

1. Go to **"Actions"** tab
2. Click **"Build"** workflow → **"Run workflow"** → **"Run workflow"**
3. Wait for build to complete (~5-10 minutes)
4. Scroll down to **"Artifacts"** section
5. Download **"firmware"** zip file
6. Extract the touch firmware file (look for the one built with `prospector_scanner_touch.conf` in the build log)

### Step 4: Flash Firmware

1. Connect XIAO BLE to computer via USB
2. **Enter bootloader mode**:
   - Press **RESET button twice quickly** (within 0.5 seconds)
   - RGB LED should pulse green/blue
   - `XIAO-SENSE` drive appears
3. **Copy firmware**:
   - Drag `.uf2` file to `XIAO-SENSE` drive
   - Drive will disconnect automatically
   - Scanner boots with touch firmware

#### Option B: Local Build

**Prerequisites**

```bash
# Install dependencies
python3 -m venv .venv
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
pip install west

# Initialize workspace
west init -l config
west update
```

**Build Command**

```bash
# From zmk-config-prospector directory
west build -b seeeduino_xiao_ble -s zmk/app -- \
  -DSHIELD=prospector_scanner \
  -DZMK_CONFIG="$(pwd)/config" \
  -DCONF_FILE=prospector_scanner_touch.conf

# Output: build/zephyr/zmk.uf2
```

**Flash**: Same as Option A - copy `build/zephyr/zmk.uf2` to XIAO-SENSE drive.

### Step 5: Test Touch

1. Scanner should boot and show main screen
2. Try **DOWN swipe** - Display Settings screen should appear
3. Try **UP swipe** - Return to main screen
4. Success! Touch mode is working

**If touch doesn't work**: See [Troubleshooting](#troubleshooting) section.

---

## Configuration Reference

### Key Configuration Settings

These are the **essential settings** for touch mode. For complete reference, see the full config file.

#### Touch Mode Enable (REQUIRED)

```conf
# Enable touch panel support
CONFIG_PROSPECTOR_TOUCH_ENABLED=y
```

This single setting enables touch mode and automatically includes touch dependencies.

#### Max Layers (Adjustable On-Device)

```conf
# Maximum layers displayable (compile-time limit)
CONFIG_PROSPECTOR_MAX_LAYERS=10
```

**Important**: This sets the **maximum** value for the on-device slider. If set to 7, you cannot adjust beyond 7 layers in Display Settings without rebuilding firmware.

#### Timeout Brightness

```conf
# Dim display after no keyboard reception
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_MS=480000      # 8 minutes (0=disabled)
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_BRIGHTNESS=5   # Dim to 5%
```

#### Ambient Light Sensor (Optional)

```conf
# Enable APDS9960 auto-brightness
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y     # Requires hardware
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS=85            # Used when sensor disabled
```

### Complete Configuration File

**File**: `config/prospector_scanner_touch.conf`

#### Essential Touch Settings

```conf
# Touch Panel Support - REQUIRED for touch mode
CONFIG_PROSPECTOR_TOUCH_ENABLED=y

# Touch panel dependencies (auto-enabled by above)
CONFIG_INPUT=y                        # Zephyr input subsystem
CONFIG_INPUT_CST816S=y                # CST816S driver
CONFIG_INPUT_CST816S_INTERRUPT=y      # Interrupt-based detection
```

#### Display Configuration

```conf
# Display subsystem
CONFIG_ZMK_DISPLAY=y
CONFIG_DISPLAY=y
CONFIG_LVGL=y

# LVGL widgets for settings screens
CONFIG_LV_USE_BTN=y                   # Buttons
CONFIG_LV_USE_SLIDER=y                # Sliders
CONFIG_LV_USE_SWITCH=y                # Toggle switches

# LVGL fonts (required for all widgets)
CONFIG_LV_FONT_MONTSERRAT_12=y
CONFIG_LV_FONT_MONTSERRAT_16=y
CONFIG_LV_FONT_MONTSERRAT_18=y
CONFIG_LV_FONT_MONTSERRAT_20=y        # Default font
CONFIG_LV_FONT_MONTSERRAT_22=y
CONFIG_LV_FONT_MONTSERRAT_24=y
CONFIG_LV_FONT_MONTSERRAT_28=y
CONFIG_LV_FONT_UNSCII_8=y             # Pixel fonts
CONFIG_LV_FONT_UNSCII_16=y
```

#### Scanner Settings

```conf
# Multi-keyboard support
CONFIG_PROSPECTOR_MULTI_KEYBOARD=y
CONFIG_PROSPECTOR_MAX_KEYBOARDS=5     # Maximum 5 keyboards

# Layer display (adjustable in Display Settings)
CONFIG_PROSPECTOR_MAX_LAYERS=7        # Default: 7 layers (0-6)
                                      # Can be changed on-device: 4-10
```

#### Brightness Control

```conf
# Ambient light sensor (optional)
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y  # Requires APDS9960 hardware
CONFIG_PROSPECTOR_ALS_MIN_BRIGHTNESS=20       # Minimum brightness %
CONFIG_PROSPECTOR_ALS_MAX_BRIGHTNESS_USB=100  # Maximum when USB powered

# Smooth brightness transitions
CONFIG_PROSPECTOR_BRIGHTNESS_FADE_DURATION_MS=800  # 800ms fade
CONFIG_PROSPECTOR_BRIGHTNESS_FADE_STEPS=12         # 12 steps

# Fixed brightness (when sensor disabled or unavailable)
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS=85         # Default 85%
```

#### Timeout Settings

```conf
# Timeout brightness control
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_MS=480000           # 8 minutes (0=disabled)
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_BRIGHTNESS=5       # Dim to 5% on timeout
```

#### Battery Support

```conf
# Scanner battery monitoring (requires LiPo connected)
CONFIG_PROSPECTOR_BATTERY_SUPPORT=n   # Disabled by default
CONFIG_ZMK_BATTERY_REPORTING=y        # Enable if battery connected
```

#### Debug Options

```conf
# USB logging (for development)
CONFIG_LOG=y
CONFIG_ZMK_LOG_LEVEL_DBG=y
CONFIG_BT_LOG_LEVEL_WRN=y             # Reduce BT noise
CONFIG_LOG_DEFAULT_LEVEL=4            # Info level

# Debug widget (development only)
CONFIG_PROSPECTOR_DEBUG_WIDGET=n      # Disable for production
```

### Device Tree Configuration

**File**: `modules/prospector-zmk-module/boards/shields/prospector_scanner/prospector_scanner.overlay`

Key sections for touch mode:

```dts
&i2c0 {
    status = "okay";
    pinctrl-0 = <&i2c0_default>;
    pinctrl-1 = <&i2c0_sleep>;
    pinctrl-names = "default", "sleep";

    // CST816S Touch Controller
    touch_sensor: cst816s@15 {
        compatible = "hynitron,cst816s";
        reg = <0x15>;
        status = "okay";

        // Touch panel control pins
        irq-gpios = <&xiao_d 0 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>;  // D0
        rst-gpios = <&xiao_d 1 GPIO_ACTIVE_LOW>;                    // D1
    };
};
```

---

## Screen Guide

### Main Screen (Default)

The main status display - always visible at startup.

#### Layout

```
┌─────────────────────────────┐
│    Device Name          🔋  │ ← Scanner battery (if available)
│ WPM: 45         [USB/BLE 2] │ ← WPM + Connection status
│                             │
│      Layer: 0 1 2 3 4       │ ← Active layers (dynamic centering)
│     [Ctrl][Alt][Shift]      │ ← Active modifiers
│                             │
│   ████████ 85% ████████     │ ← Keyboard battery bars
│         -45dBm 5.0Hz        │ ← Signal strength
└─────────────────────────────┘
```

#### Widgets Explained

1. **Device Name**: Keyboard name from `CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME`
2. **Scanner Battery**: Top-right, shows scanner's own battery (if LiPo connected)
3. **WPM**: Words per minute (typing speed)
4. **Connection Status**: Shows active transport (USB or BLE) with profile number
5. **Layer Display**: Active layer highlighted, count adjustable in Display Settings
6. **Modifiers**: Ctrl/Alt/Shift/GUI shown when active (NerdFont icons)
7. **Keyboard Battery**: Split keyboard left/right or single keyboard battery
8. **Signal Status**: RSSI (dBm) and reception rate (Hz)

#### Gestures from Main Screen

- **DOWN Swipe**: Open Display Settings
- **UP/LEFT/RIGHT**: No action (reserved for future features)

---

### Display Settings Screen

Interactive configuration screen - accessed via **DOWN swipe** from main screen.

#### Layout

```
┌─────────────────────────────┐
│     Display Settings        │ ← Screen title
│                             │
│  Max Layers: [====●===] 7   │ ← Slider widget
│                             │
│  [More settings coming...]  │ ← Future features
│                             │
│                             │
│   ↑ Swipe up to return      │ ← Hint text
└─────────────────────────────┘
```

#### Available Settings

##### 1. Max Layers Setting

**What it does**: Controls how many layer indicators are shown on main screen.

**Range**: 4 - 10 layers

**Default**: 7 layers (from `CONFIG_PROSPECTOR_MAX_LAYERS`)

**How to adjust**:
1. Touch and drag the slider knob left/right
2. Number updates in real-time
3. Main screen layer display updates immediately

**Visual Effect**:
- **4 layers**: Wide spacing, large indicators
- **7 layers**: Medium spacing (default)
- **10 layers**: Tight spacing, maximum capacity

**Configuration Dependency**:

⚠️ **IMPORTANT**: Slider range is LIMITED by Kconfig setting:

```conf
# In prospector_scanner_touch.conf
CONFIG_PROSPECTOR_MAX_LAYERS=10   # Allows adjusting up to 10 layers
```

If `CONFIG_PROSPECTOR_MAX_LAYERS=7`, slider maximum is 7 - cannot go higher without rebuilding firmware with increased Kconfig value.

**Why the limit?**: Memory is pre-allocated at compile time based on Kconfig value. Runtime adjustment cannot exceed compile-time maximum.

##### 2. Future Settings (Coming in v2.1+)

Planned settings for future releases:
- **Brightness**: Manual brightness override (if auto-brightness disabled)
- **Contrast**: Display contrast adjustment
- **Color Scheme**: Dark/Light/Pastel theme selection
- **WPM Window**: WPM calculation window (10s/30s/60s)
- **Signal Averaging**: RSSI smoothing settings

#### Persistence Behavior

⚠️ **v2.0 LIMITATION**: Settings do **NOT persist** across power cycles.

**What this means**:
- Adjustments apply immediately while powered on
- Settings reset to Kconfig defaults on reboot/power loss
- **Workaround**: Adjust Kconfig values for permanent changes

**Coming in v2.1**: NVS (Non-Volatile Storage) will save settings to flash memory.

#### Gestures from Display Settings

- **UP Swipe**: Return to main screen (saves settings temporarily)
- **DOWN/LEFT/RIGHT**: No action

---

## Gesture Reference

### Swipe Detection

**Minimum Distance**: ~30 pixels (~20% of screen width)
**Timeout**: Must complete within 500ms
**Directions**: 4-way (Up/Down/Left/Right)

### Gesture Map (v2.0)

| Gesture | From Main Screen | From Display Settings |
|---------|-----------------|----------------------|
| **DOWN** | Open Display Settings | No action |
| **UP** | No action | Return to main screen |
| **LEFT** | Reserved (future) | No action |
| **RIGHT** | Reserved (future) | No action |

### Planned Gestures (v2.1+)

| Gesture | Planned Function |
|---------|------------------|
| **LEFT** | Quick brightness decrease |
| **RIGHT** | Quick brightness increase |
| **LONG PRESS** | Lock screen / screensaver |
| **DOUBLE TAP** | Cycle through multiple main screens |

### Gesture Feedback

**Visual**: Screen transitions with LVGL animation (fade/slide)
**No Haptic**: Waveshare display has no vibration motor
**Audible**: No speaker - silent operation

---

## Troubleshooting

### Touch Not Working

#### Symptom: No response to touch input

**Check List**:

1. **Verify Touch Hardware**:
   ```bash
   # Check I2C device detection (requires USB logging enabled)
   # Should show device at address 0x15
   ```

2. **Check Wiring**:
   - TP_SDA → D4 (P0.04)
   - TP_SCL → D5 (P0.05)
   - TP_INT → D0 (P0.02)
   - TP_RST → D1 (P0.28)

3. **Verify Configuration**:
   ```conf
   # Must be enabled
   CONFIG_PROSPECTOR_TOUCH_ENABLED=y
   CONFIG_INPUT_CST816S=y
   ```

4. **Check Firmware Build**:
   - Did you flash touch firmware? (`prospector_scanner_touch.conf`)
   - GitHub Actions: Did you specify `CONF_FILE=prospector_scanner_touch.conf`?

5. **I2C Bus Conflict**:
   - If APDS9960 also connected, verify both devices can coexist
   - Try disconnecting sensor temporarily to isolate issue

#### Symptom: Touch works but gestures don't register

**Possible Causes**:

1. **Swipe Too Slow**: Must complete within 500ms
2. **Swipe Too Short**: Minimum ~30 pixels distance required
3. **Wrong Direction**: Only DOWN swipe works from main screen
4. **Screen Busy**: Wait for animations to complete before next gesture

### Display Settings Issues

#### Symptom: Cannot adjust slider beyond certain value

**Cause**: Kconfig `CONFIG_PROSPECTOR_MAX_LAYERS` limits maximum.

**Solution**: Edit `prospector_scanner_touch.conf`:
```conf
CONFIG_PROSPECTOR_MAX_LAYERS=10  # Increase to desired maximum
```

Rebuild and reflash firmware.

#### Symptom: Settings reset after reboot

**Expected Behavior**: v2.0 does not persist settings.

**Workaround**: Set desired defaults in Kconfig:
```conf
CONFIG_PROSPECTOR_MAX_LAYERS=7  # Your preferred default
```

**Permanent Fix**: Coming in v2.1 with NVS storage.

### Build Errors

#### Error: `'lv_font_montserrat_XX' undeclared`

**Cause**: LVGL font not enabled.

**Solution**: Enable all required fonts in `.conf`:
```conf
CONFIG_LV_FONT_MONTSERRAT_12=y
CONFIG_LV_FONT_MONTSERRAT_16=y
CONFIG_LV_FONT_MONTSERRAT_18=y
CONFIG_LV_FONT_MONTSERRAT_20=y
# ... (all fonts listed in Configuration Reference)
```

#### Error: `undefined reference to touch_handler_register_lvgl_indev`

**Cause**: Touch code compiled but not properly initialized.

**Solution**: Verify widget creation order in code (developers only - this should not occur in release builds).

### Performance Issues

#### Symptom: Screen freezes during swipes

**Rare Issue**: Should be resolved in v2.0 with mutex protection.

**If it occurs**:
1. Check firmware version - update to latest v2.0.0+
2. Reduce number of active keyboards being scanned (< 5)
3. Disable debug widget if enabled: `CONFIG_PROSPECTOR_DEBUG_WIDGET=n`

**Report**: If freezing persists, please open GitHub issue with:
- Firmware version
- Number of keyboards being scanned
- Gesture that triggers freeze
- USB log output (if available)

#### Symptom: Slow response to gestures

**Normal Behavior**: Slight delay (100-200ms) is expected for:
- LVGL mutex acquisition
- Screen transition animations

**Abnormal Delay** (>500ms):
- Check CPU load - multiple keyboards + sensor + touch may be intensive
- Disable ambient light sensor temporarily: `CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=n`
- Reduce update frequency: `CONFIG_PROSPECTOR_SCANNER_TIMEOUT_MS` to higher value

---

## Advanced Topics

### Shared I2C Bus

Touch panel (0x15) and APDS9960 (0x39) share I2C bus on D4/D5:

```
I2C Bus (D4/D5)
├── CST816S Touch (0x15)
│   └── Interrupt-driven
└── APDS9960 Sensor (0x39)
    └── Polling mode (no interrupt)
```

**Compatibility**: Both devices tested working simultaneously - no conflicts observed.

### Memory Usage

Touch mode adds ~20KB to firmware size:

| Component | Size |
|-----------|------|
| CST816S Driver | ~8KB |
| Touch Handler | ~3KB |
| Swipe Detection | ~2KB |
| Display Settings Screen | ~7KB |
| **Total Touch Overhead** | **~20KB** |

**Memory Safety**: XIAO BLE has 1MB flash - plenty of room for touch features.

### Power Consumption

Touch panel impact on battery life:

- **Touch Idle**: ~1mA (interrupt mode - waiting for touch)
- **Touch Active**: ~5mA (during gesture detection)
- **Negligible Impact**: Touch adds <2% to total power draw

**Battery Life Estimate** (with 500mAh LiPo):
- Display active: ~10 hours
- With timeout dimming: ~20+ hours
- Similar to non-touch mode

---

## FAQ

### Can I use touch firmware on non-touch display?

**No** - touch firmware requires CST816S hardware. It will build but touch features won't work and I2C errors may occur.

**Solution**: Use non-touch firmware (`prospector_scanner.conf`) for non-touch displays.

### Can I disable touch temporarily?

**Yes** - set in Kconfig:
```conf
CONFIG_PROSPECTOR_TOUCH_ENABLED=n
```

Rebuild firmware. All touch code excluded at compile time.

### Will settings persist across reboots?

**Not in v2.0** - settings reset to Kconfig defaults on power cycle.

**For permanent settings**, edit Kconfig values and rebuild:
```conf
CONFIG_PROSPECTOR_MAX_LAYERS=7  # Your preferred default
```

**Future consideration**: Settings persistence may be added in a future release if there is user demand. This would require implementing flash storage for configuration values.

---

## Additional Resources

### Documentation
- [Main README](../README.md) - Project overview
- [v2.0 Release Notes](RELEASES/v2.0.0/release_notes.md) - Complete changelog
- [Architecture Design](../SCANNER_RECONSTRUCTION_DESIGN.md) - Technical details (local dev file)

### Hardware Guides
- [Waveshare 1.69" Touch LCD Datasheet](https://www.waveshare.com/wiki/1.69inch_Touch_LCD)
- [CST816S Touch Controller Datasheet](https://files.waveshare.com/upload/6/66/CST816S_Datasheet_EN.pdf)
- [XIAO BLE Pinout](https://wiki.seeedstudio.com/XIAO_BLE/#pin-diagram)

### Community
- [GitHub Issues](https://github.com/t-ogura/zmk-config-prospector/issues) - Bug reports and feature requests
- [ZMK Discord](https://zmk.dev/community/discord/invite) - General ZMK support

---

## Special Thanks

### Touch Implementation Reference

This touch mode implementation was inspired by and references the excellent work by **kot149**:

- **Article**: [Prospector Touch Panel Integration](https://zenn.dev/kot149/articles/prospector-touch) (Japanese)
- **Author**: kot149
- **Contribution**: Pioneering work on CST816S touch integration with Prospector hardware

We are deeply grateful for kot149's detailed documentation and exploration of touch panel possibilities, which provided invaluable insights for developing this touch mode implementation.

---

**Version History**:
- **v2.0.0** (2025-11-20): Initial touch mode documentation
