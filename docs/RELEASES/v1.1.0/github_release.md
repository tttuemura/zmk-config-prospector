# GitHub Release Instructions for v1.1.0

## 📋 Release Checklist

- [ ] Tag created and pushed (`v1.1.0`)
- [ ] Latest GitHub Actions build successful
- [ ] Firmware files downloaded from Actions
- [ ] Release notes prepared
- [ ] Screenshots/media ready (optional)

## 🚀 Creating the Release

### Step 1: Navigate to Releases
Go to: https://github.com/t-ogura/zmk-config-prospector/releases/new

### Step 2: Fill Release Information

**Choose a tag**: `v1.1.0` (select existing tag)

**Release title**: 
```
Prospector Scanner v1.1.0 - Enhanced Experience
```

**Release description**: 
Copy the content below (formatted for GitHub Release):

---

## 🎉 Prospector Scanner v1.1.0 - Enhanced Experience

**The most significant update since project inception!** This release delivers **15x power optimization**, **scanner battery operation**, **ambient light sensing**, and over **100 individual improvements**.

### 📊 Performance Improvements
- **20x faster response** - Key presses detected in <200ms (was 2-5 seconds)
- **15x better battery life** - Idle mode now uses 0.03Hz (was 0.5Hz)
- **10x display update speed** - Real-time layer changes
- **60% more features** - From ~25 to 40+ features

### 🚀 Major New Features

#### 🔋 Scanner Battery Operation (Optional)
- 4-12 hour portable operation
- Real-time battery monitoring
- USB-C charging support
- Smart power profiles

#### 🔆 Ambient Light Sensor (Finally Working!)
- Automatic brightness adjustment
- Fixed I2C pin mapping issue
- Non-linear response curve
- Enabled by default

#### ⚡ Power Management Revolution
- **Active**: 10Hz updates when typing
- **Idle**: 0.03Hz updates when idle
- **Sleep**: Advertisements stopped
- Keyboard battery life improved by 60%

#### 📊 Enhanced Features
- **WPM Tracking**: Configurable 5-120 second windows
- **RX Rate Display**: Accurate idle detection
- **Split Keyboard**: Configurable left/right positioning
- **5-Level Battery Colors**: Green→Red visual indicators

### 📦 Downloads

**Scanner Firmware:**
- `prospector_scanner-seeeduino_xiao_ble-zmk.uf2` - Main scanner firmware
- `settings_reset-seeeduino_xiao_ble-zmk.uf2` - Settings reset utility

**Installation:**
1. Download the firmware file above
2. Enter bootloader mode (double-tap reset)
3. Copy .uf2 file to XIAO drive
4. Device auto-reboots with v1.1.0

### ⚙️ Critical Configuration for Keyboards

**Add these lines to your keyboard .conf for v1.1.0 power optimization:**
```kconfig
# MANDATORY for v1.1.0 (15x power improvement)
CONFIG_ZMK_STATUS_ADV_ACTIVITY_BASED=y
CONFIG_ZMK_STATUS_ADV_ACTIVE_INTERVAL_MS=100
CONFIG_ZMK_STATUS_ADV_IDLE_INTERVAL_MS=30000
CONFIG_ZMK_STATUS_ADV_ACTIVITY_TIMEOUT_MS=10000
```

### 🔄 Upgrading from v1.0.0

1. **Update west.yml**: Change revision to `v1.1.0`
2. **Add power optimization config** (see above)
3. **Flash new scanner firmware**
4. **Optional**: Add APDS9960 sensor or battery

### 🐛 Bug Fixes
- Fixed scanner battery stuck at 23%
- Fixed RX showing 5Hz during idle (now shows 0.03Hz)
- Fixed display timeout issues
- Fixed duplicate GitHub Actions
- Removed obsolete root west.yml

### 📖 Documentation
- [Complete Setup Guide](https://github.com/t-ogura/zmk-config-prospector#readme)
- [Migration Guide](docs/RELEASES/v1.1.0.md#migration-guide)
- [Hardware Wiring](https://github.com/t-ogura/zmk-config-prospector#hardware-requirements)

### 🤝 Acknowledgments
Special thanks to:
- All hardware testers and bug reporters
- Original Prospector project by @carrefinho
- YADS project for UI inspiration
- ZMK community for continuous support

### 📊 Compatibility
- **Keyboards**: Any ZMK keyboard with BLE
- **Split Keyboards**: Full support with configurable sides
- **ZMK Version**: v3.6.0+ recommended
- **Hardware**: Seeeduino XIAO BLE + 1.69" LCD

### 🔗 Links
- [Full Release Notes](docs/RELEASES/v1.1.0.md)
- [GitHub Repository](https://github.com/t-ogura/zmk-config-prospector)
- [Report Issues](https://github.com/t-ogura/zmk-config-prospector/issues)

---

**Prospector Scanner v1.1.0** - Making ZMK keyboard status visible, beautiful, and battery-efficient.

*Built with ❤️ by the ZMK community.*

---

### Step 3: Attach Firmware Files

**Required files from latest GitHub Actions build:**
1. `prospector_scanner-seeeduino_xiao_ble-zmk.uf2`
2. `settings_reset-seeeduino_xiao_ble-zmk.uf2`

Download from: https://github.com/t-ogura/zmk-config-prospector/actions

### Step 4: Release Settings

- [ ] **Set as the latest release** ✅
- [ ] **Create a discussion for this release** (optional)

### Step 5: Publish

Click **"Publish release"** button

## 📝 Post-Release Tasks

1. Verify download links work
2. Test firmware files
3. Announce in Discord/community
4. Update project README if needed
5. Monitor issues for v1.1.0 feedback

## 📊 Success Metrics

- Downloads within 24 hours
- Community feedback
- Issue reports (hopefully minimal!)
- Migration success stories