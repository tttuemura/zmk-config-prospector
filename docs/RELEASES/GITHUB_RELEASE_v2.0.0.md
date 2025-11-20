# 🎉 Prospector Scanner v2.0.0 "Touch & Precision"

**🎯 Touch Panel Support • 🔌 USB Detection Fix • 🔒 Thread-Safe Architecture**

---

## 🌟 **Major New Features**

### 🎯 **Touch Panel Support** (Optional)
**Interactive On-Device Configuration** - CST816S touch integration with swipe gestures!

- ✅ **Four-Direction Gestures**: Swipe Up/Down/Left/Right for navigation
- ✅ **Display Settings Screen**: Adjust max layers without rebuilding firmware
- ✅ **Event-Driven Architecture**: ZMK event system for thread-safe touch handling
- ✅ **Dual Build Modes**: Choose touch or non-touch firmware
- ✅ **Zero Overhead**: Non-touch builds completely exclude touch code

**Related**: GitHub Issue #1 (Touch panel integration request)

### 🔌 **USB Connection Display Fix**
**Accurate Connection Status** - Shows "> USB" when keyboard is USB-connected!

- ✅ **Protocol-Based Detection**: Uses `USB_HID_READY` flag from status advertisement
- ✅ **Real-Time Updates**: Immediate response to connection state changes
- ✅ **BLE Profile Display**: Maintains BLE profile information alongside USB status
- ✅ **Long-Standing Issue**: Finally fixed after multiple user reports

**Fixed**: GitHub Issue #5 (USB connection display)

### 🔒 **Thread-Safety Enhancements**
**Complete Freeze Elimination** - LVGL mutex protection and work queue safety!

- ✅ **LVGL Mutex Protection**: All LVGL operations properly synchronized
- ✅ **ZMK Event System**: Touch gestures use thread-safe event propagation
- ✅ **Work Queue Safety**: Prevents concurrent access to shared resources
- ✅ **Proven Stable**: Tested with 5+ keyboards and intensive gesture use

**Background**: Previous versions experienced random freezes during screen transitions due to race conditions.

### ⏱️ **Timeout Brightness Control**
**Battery-Saving Dimming** - Automatic brightness reduction on keyboard inactivity!

- ✅ **Configurable Timeout**: Dims to 5% after no keyboard reception (default: 8 minutes)
- ✅ **Automatic Recovery**: Restores brightness when keyboard data received
- ✅ **Universal Operation**: Works with both USB and battery power
- ✅ **Disable Support**: Set timeout to 0 to disable feature

### 🌞 **APDS9960 4-Pin Connection Support**
**Direct I2C Implementation** - Works without interrupt pin!

- ✅ **Polling Mode**: Efficient I2C register polling for ambient light data
- ✅ **No Interrupt Required**: 4-pin connection (VCC/GND/SDA/SCL) works perfectly
- ✅ **Reliable Operation**: Eliminates sensor blocking on `sensor_sample_fetch()`
- ✅ **Backward Compatible**: 5-pin connections still supported

**Background**: v1.1 users reported sensor freezing due to Zephyr APDS9960 driver blocking forever without interrupt pin.

---

## 📊 **Version Comparison**

| Feature | v1.1.1 | v2.0.0 | Improvement |
|---------|--------|--------|-------------|
| **Touch Panel** | Not supported | CST816S with gestures | ✅ **New capability** |
| **USB Detection** | Always shows "BLE" | Correct "USB" display | ✅ **Accurate status** |
| **Thread Safety** | Race conditions possible | Mutex + events | ✅ **Freeze-free** |
| **Timeout Dimming** | Not supported | Configurable 5% dim | ✅ **Battery savings** |
| **Signal Updates** | Irregular intervals | Stable 1Hz | ✅ **Consistent display** |
| **APDS9960 Pins** | 5-pin required | 4-pin supported | ✅ **Simplified wiring** |
| **Memory Model** | Static allocation | Dynamic allocation | ✅ **Efficient usage** |
| **Build Modes** | Single mode | Touch/non-touch | ✅ **User choice** |

---

## 📦 **What's Included**

### **Pre-Built Firmware** 📥
- `prospector_scanner-seeeduino_xiao_ble-zmk.uf2` - **Non-touch mode** (standard)
- `prospector_scanner_touch-seeeduino_xiao_ble-zmk.uf2` - **Touch mode** (interactive)
- `settings_reset-seeeduino_xiao_ble-zmk.uf2` - Settings reset firmware
- Ready to flash - no building required!

### **Hardware Requirements** 🔧

#### **Both Modes**
- **MCU**: Seeeduino XIAO BLE (nRF52840)
- **Display**: Waveshare 1.69" Round LCD with touch panel (ST7789V + CST816S)
- **Power**: USB Type-C (5V) or LiPo battery
- **Optional**: APDS9960 ambient light sensor (4-pin: VCC/GND/SDA=D4/SCL=D5)

#### **Touch Mode Additional Wiring**
- **+4 pins required**: TP_SDA (D4), TP_SCL (D5), TP_INT (D0), TP_RST (D1)
- **Note**: Non-touch mode uses same display but leaves touch pins unconnected

### **Display Wiring** 🔌

**Standard (Non-Touch Mode)**:
```
LCD_DIN  -> Pin 10 (MOSI)    LCD_DC   -> Pin 7  (Data/Command)
LCD_CLK  -> Pin 8  (SCK)     LCD_RST  -> Pin 3  (Reset)
LCD_CS   -> Pin 9  (CS)      LCD_BL   -> Pin 6  (Backlight PWM)
VCC      -> 3.3V             GND      -> GND
```

**Touch Mode Additional Pins**:
```
TP_SDA -> D4 (P0.04)    TP_INT -> D0 (P0.02)
TP_SCL -> D5 (P0.05)    TP_RST -> D1 (P0.28)
```

---

## 🚀 **Quick Setup Guide**

### **Step 1: Choose Your Mode**

| Choose Non-Touch If: | Choose Touch If: |
|----------------------|------------------|
| ✅ Simpler wiring (6 pins) | ✅ Want interactive settings |
| ✅ Following original Prospector | ✅ Have +4 pins to wire |
| ✅ Don't need on-device adjustment | ✅ Want swipe gestures |

### **Step 2: Flash Scanner Firmware**
1. Download appropriate firmware:
   - Non-touch: `prospector_scanner-seeeduino_xiao_ble-zmk.uf2`
   - Touch: `prospector_scanner_touch-seeeduino_xiao_ble-zmk.uf2`
2. Connect Seeeduino XIAO BLE in bootloader mode (double-press reset)
3. Copy firmware file to XIAO-SENSE drive
4. Device will restart with Prospector Scanner v2.0

### **Step 3: Add to Your Keyboard**

Add to your keyboard's `config/west.yml`:
```yaml
- name: prospector-zmk-module
  remote: prospector
  revision: v2.0.0
  path: modules/prospector-zmk-module
```

Add to your keyboard's `.conf` file:
```kconfig
# Essential configuration
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"

# Power optimization (10Hz active, 0.03Hz idle)
CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y
CONFIG_ZMK_STATUS_ADV_ACTIVE_INTERVAL_MS=100
CONFIG_ZMK_STATUS_ADV_IDLE_INTERVAL_MS=30000

# Split keyboard battery monitoring (if applicable)
CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING=y
```

Rebuild and flash your keyboard firmware.

### **Step 4: Test**
1. Power on scanner (shows "Waiting for keyboards...")
2. Power on keyboard
3. Scanner detects keyboard within 1-2 seconds
4. **Touch mode**: Try DOWN swipe to open Display Settings

---

## 🎯 **Touch Mode Features**

### **Gesture System**
- **DOWN Swipe**: Open Display Settings screen
- **UP Swipe**: Return to main display
- **LEFT/RIGHT**: Reserved for future features

### **Display Settings Screen**
- **Max Layers**: Adjust visible layer count (4-10) with slider
- **Real-Time Preview**: Main screen updates immediately
- **Navigation**: UP swipe to return

**Note**: Settings reset on power cycle - use Kconfig for permanent defaults.

### **Thread-Safe Architecture**
Touch uses ZMK event system:
```c
// Touch handler raises event from IRQ context (thread-safe)
raise_zmk_swipe_gesture_event(...);

// Scanner display listens in main thread (safe LVGL calls)
ZMK_LISTENER(swipe_gesture, swipe_gesture_listener);
```

This eliminates race conditions that caused freezes in earlier experimental implementations.

---

## 🔧 **Key Configuration Options**

### **Touch Mode Enable** (Touch Build Only)
```kconfig
CONFIG_PROSPECTOR_TOUCH_ENABLED=y
```

### **Timeout Brightness** (Both Modes)
```kconfig
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_MS=480000      # 8 minutes (0=disabled)
CONFIG_PROSPECTOR_SCANNER_TIMEOUT_BRIGHTNESS=5   # Dim to 5%
```

### **Ambient Light Sensor** (Optional Hardware)
```kconfig
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y     # Requires APDS9960
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS=85            # Used when sensor disabled
```

### **Layer Display**
```kconfig
CONFIG_PROSPECTOR_MAX_LAYERS=7        # Range: 4-10
# Touch mode: adjustable on-device
# Non-touch: rebuild required to change
```

---

## 🐛 **Fixes & Improvements**

### ✅ **Display & UI**
- **USB connection display**: Fixed to show "> USB" when keyboard is USB-connected (Issue #5)
- **Signal widget updates**: Stable 1Hz periodic update for reception strength
- **Timeout brightness**: Configurable dimming when no keyboard activity

### ✅ **Hardware & Sensors**
- **APDS9960 blocking**: Direct I2C eliminates driver blocking on 4-pin connections
- **Touch panel integration**: Full CST816S support with gesture recognition (Issue #1)

### ✅ **Stability & Thread Safety**
- **Random freezes**: LVGL mutex protection prevents deadlocks
- **Touch deadlock**: ZMK event system eliminates race conditions
- **Work queue conflicts**: Swipe handler stops periodic work during transitions

### ✅ **Configuration & Build**
- **Touch mode selection**: Clear Kconfig option for touch/non-touch builds
- **Code elimination**: Touch code completely excluded when disabled
- **Font configuration**: Proper LVGL font enablement for all widgets

---

## 🆘 **Troubleshooting Quick Reference**

| Issue | Solution |
|-------|----------|
| **Touch not responding** | Verify 4 touch pins wired: TP_SDA/SCL/INT/RST |
| **Shows "BLE" when USB** | Update to v2.0 firmware (both scanner and keyboard) |
| **Scanner freezes** | Upgrade to v2.0 - thread safety fixes included |
| **APDS9960 not working** | v2.0 supports 4-pin - no interrupt pin needed |
| **Settings reset on boot** | Expected in v2.0 - edit Kconfig for permanent defaults |

---

## 📞 **Support & Documentation**

- **📖 Complete Guide**: [README.md](../../README.md) - Full setup documentation
- **🎯 Touch Mode Guide**: [TOUCH_MODE.md](../TOUCH_MODE.md) - Touch-specific setup
- **📋 Release Notes**: [v2.0.0.md](v2.0.0.md) - Complete changelog
- **🐛 Bug Reports**: [GitHub Issues](https://github.com/t-ogura/zmk-config-prospector/issues)
- **💬 Questions**: [GitHub Discussions](https://github.com/t-ogura/zmk-config-prospector/discussions)
- **🎮 ZMK Community**: [ZMK Discord](https://discord.gg/8Y8y9u5)

---

## 🔄 **Migration from v1.1.1**

**✅ No Breaking Changes** - All existing configurations continue to work!

**Quick Migration**:
1. Update `west.yml` revision: `v1.1.1` → `v2.0.0`
2. Flash new firmware:
   - Non-touch: Same wiring as before
   - Touch: Add 4 touch pins if desired
3. Keyboard firmware: Rebuild with v2.0.0 module for USB display fix

**Optional**: Enable timeout brightness for battery savings.

---

## ⚠️ **Known Limitations**

### **Touch Mode**
- Display Settings limited to max layers adjustment in v2.0
- Settings reset on power cycle - use Kconfig for permanent values
- Gesture sensitivity may need adjustment for different environments

### **General**
- Multi-keyboard limit: Maximum 5 keyboards (protocol limitation)
- Layer range: 0-9 layers supported (10 total)
- Battery widget: Requires LiPo battery connected to XIAO BLE

---

## 🎉 **Credits & Thanks**

**Lead Development**: Claude (Anthropic Sonnet 4.5)
**Community Testing**: Multiple hardware configurations validated
**Hardware Integration**: Comprehensive real-device testing

**Special Recognition**:
- **carrefinho** - Original Prospector hardware platform ([prospector](https://github.com/carrefinho/prospector))
- **kot149** - Touch panel integration pioneering work ([Zenn article](https://zenn.dev/kot149/articles/prospector-touch))
- **ZMK Community** - Excellent firmware framework and support
- **Issue Reporters** - GitHub #1 (touch), #5 (USB) for driving improvements
- **Early Adopters** - Critical testing and feedback

**Technical Inspirations**:
- **YADS (Yet Another Dongle Screen)** - UI widget designs and NerdFont modifier symbols
- **ZMK Event System** - Thread-safe architecture patterns
- **LVGL Dynamic Allocation** - Memory-efficient UI management

---

## 🔮 **About This Project**

**Prospector Scanner** transforms the Prospector hardware into an independent BLE status display. Unlike the original dongle mode (keyboard connects to Prospector), scanner mode preserves your keyboard's full 5-device connectivity while providing professional status monitoring.

**Key Difference**:
- **Original Prospector**: Dongle mode (keyboard → Prospector → PC)
- **This Project**: Scanner mode (keyboard broadcasts to Prospector via BLE Advertisement)

Both are valid uses of the excellent Prospector hardware platform.

---

**🏷️ Version**: v2.0.0 "Touch & Precision"
**📅 Released**: November 20, 2025
**🛡️ Status**: **STABLE - PRODUCTION READY**
**📜 License**: MIT License

---

*Experience the future of ZMK keyboard monitoring with Prospector Scanner v2.0.0 - now with optional touch control!*
