# Prospector Scanner Releases

This document contains detailed release notes and upgrade instructions for the Prospector Scanner system.

## 🚨 v1.1.1 - Critical Safety Update (2025-08-26)

### 🔥 **URGENT: Critical Startup Fix**

**Issue Fixed**: Multiple users reported scanner devices failing to boot after v1.1.0 update.

**Root Cause**: Devices without APDS9960 ambient light sensor hardware experienced startup failures due to mandatory driver initialization.

**Solution**: Implemented comprehensive safety systems with automatic hardware detection and graceful fallbacks.

### 🛠️ **What's Fixed**

#### **Startup Safety System**
- ✅ **Automatic Hardware Detection**: Scanner automatically detects if APDS9960 sensor is present
- ✅ **Safe Fallback Mode**: Seamlessly switches to fixed brightness when sensor is missing
- ✅ **Error Prevention**: Eliminates all startup failures related to optional hardware
- ✅ **Zero Configuration**: Works out-of-the-box regardless of hardware configuration

#### **Enhanced Configuration System**
- ✅ **Safe Defaults**: `CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=n` by default
- ✅ **Runtime Detection**: Sensor presence checked at runtime, not compile time
- ✅ **Comprehensive Documentation**: Clear setup instructions for both configurations
- ✅ **Migration Guide**: Step-by-step upgrade instructions

### 📦 **Repository Improvements**

#### **Streamlined Module Structure**
- 🗂️ **56,927 lines removed**: Eliminated unnecessary 3D models, images, test files
- 🚫 **No more CI/CD conflicts**: Removed GitHub Actions from module (proper ZMK practice)
- 📁 **Clean structure**: Module now contains only essential ZMK functionality
- 📝 **Focused documentation**: Development notes moved to local-only files

#### **Performance Optimizations**
- ⚡ **Faster builds**: Reduced module size speeds up west update and builds
- 🔧 **Better maintainability**: Clear separation between module code and examples
- 📊 **Improved reliability**: Eliminated complex nested dependencies

### 🚀 **Upgrade Instructions**

#### **For Users WITHOUT APDS9960 Sensor** (Majority)
```bash
# 1. Update west.yml in your scanner config
# Change revision to: feature/v1.1.1-safety-fix

# 2. Build and flash - no config changes needed!
west build --pristine
```
**Result**: ✅ Scanner will automatically use safe fixed brightness mode

#### **For Users WITH APDS9960 Sensor**
```bash
# 1. Update west.yml revision to: feature/v1.1.1-safety-fix

# 2. Enable sensor in your prospector_scanner.conf:
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y
CONFIG_SENSOR=y
CONFIG_APDS9960=y
CONFIG_I2C=y

# 3. Build and flash
west build --pristine
```
**Result**: ✅ Scanner will automatically detect sensor and enable auto-brightness

### 🔧 **Hardware Compatibility**

| Hardware Configuration | v1.1.0 Status | v1.1.1 Status |
|------------------------|---------------|---------------|
| **XIAO + Display only** | ❌ Boot failure | ✅ Works perfectly |
| **XIAO + Display + APDS9960** | ✅ Works with config | ✅ Works with config |
| **Custom I2C devices** | ⚠️ Potential conflicts | ✅ Safe detection |

### 🎯 **Tested Configurations**

- ✅ Seeeduino XIAO nRF52840 + ST7789V display (no sensor)
- ✅ Seeeduino XIAO nRF52840 + ST7789V display + APDS9960 sensor
- ✅ Multiple keyboard types (Split keyboards, regular keyboards)
- ✅ Battery vs USB power configurations
- ✅ Various brightness settings and ranges

### 🐛 **Known Issues Resolved**

1. **"ALS: No I2C Devices" error** → ✅ Fixed with runtime detection
2. **Black screen on startup** → ✅ Fixed with safe fallback mode  
3. **I2C initialization failures** → ✅ Fixed with conditional compilation
4. **Kconfig circular dependencies** → ✅ Fixed with restructured dependencies
5. **Boot failure on sensor-less devices** → ✅ Fixed by removing I2C force-enable from board overlay (commit 2d4caae)

### ⚠️ **Breaking Changes**

- **Default sensor setting**: Now `CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=n`
- **Module structure**: Development files removed (cleaner, but different file layout)
- **Build process**: GitHub Actions removed from module (proper ZMK module practice)

### 📋 **Migration Checklist**

**Before Upgrading:**
- [ ] Note your current brightness settings
- [ ] Check if you have APDS9960 sensor connected
- [ ] Backup your current firmware

**After Upgrading:**
- [ ] Update west.yml revision to `feature/v1.1.1-safety-fix`
- [ ] Add sensor config if you have APDS9960 hardware
- [ ] Test startup behavior
- [ ] Verify auto-brightness (if using sensor)
- [ ] Check brightness levels meet your preferences

### 🆘 **Troubleshooting**

#### **Scanner won't boot after upgrade**
1. Check power connections and USB cable
2. Try reflashing with `west build --pristine`
3. Verify no extra sensor configs if you don't have APDS9960

#### **Auto-brightness not working**
1. Confirm APDS9960 is connected to correct pins (SDA=D4, SCL=D5)
2. Enable sensor config: `CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y`
3. Check I2C wiring and sensor power (3.3V)

#### **Build errors**
1. Run `west update` to get latest module
2. Use `west build --pristine` for clean build
3. Check Kconfig syntax in your .conf files

---

## v1.1.0 - Enhanced Experience (2025-01-30)

### Major Features
- **🔆 APDS9960 Ambient Light Sensor**: Automatic screen brightness adjustment
- **🔋 Split Keyboard Battery Display**: Left/right battery monitoring with configurable sides
- **📊 Enhanced Analytics**: WPM tracking, reception rate monitoring  
- **⚡ Power Optimizations**: Activity-based intervals, smart sleep modes
- **🎨 UI Polish**: 5-level battery colors, improved typography

### Technical Improvements
- I2C sensor support with proper pin mapping (SDA=D4, SCL=D5)
- Runtime sensor detection with fallback modes
- Enhanced split keyboard support with role detection
- Improved power management for extended battery life

---

## v1.0.0 - Stable Release (2025-01-29)

### Core Features
- **BLE Advertisement Protocol**: 26-byte efficient status broadcasting
- **YADS-Style UI**: Beautiful pastel color scheme with elegant typography
- **Multi-Keyboard Support**: Up to 3 keyboards simultaneously
- **Split Keyboard Integration**: Left/right battery display and status
- **Real-Time Monitoring**: Sub-second status updates
- **Activity-Based Power Management**: Intelligent power consumption

### System Architecture
- Non-intrusive design preserving keyboard connectivity
- Universal compatibility with all ZMK keyboards
- Modular widget system for extensible display
- Professional-grade UI/UX matching YADS quality

---

## v0.9.0 - Core Foundation (2025-01-28)

### Initial Implementation  
- Basic scanner functionality
- BLE advertisement support
- Layer detection system
- Initial UI framework

---

## 📞 Support & Community

- **Issues**: Report bugs at https://github.com/t-ogura/prospector-zmk-module/issues
- **Discussions**: https://github.com/t-ogura/zmk-config-prospector/discussions
- **Documentation**: https://github.com/t-ogura/zmk-config-prospector/blob/main/README.md

## 🔄 Update Frequency

- **Critical fixes**: Released immediately (like v1.1.1)
- **Feature updates**: Monthly releases
- **Major versions**: Quarterly releases with comprehensive testing

---

*Last updated: 2025-08-26*
*Version: v1.1.1*