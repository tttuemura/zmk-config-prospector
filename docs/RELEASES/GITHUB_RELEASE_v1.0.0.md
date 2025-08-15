# GitHub Release Instructions for v1.0.0

## 📋 Release Information

**Choose a tag**: `v1.0.0`

**Release title**: 
```
Prospector Scanner v1.0.0 - Initial Stable Release
```

**Release description**: 

---

## 🎉 Prospector Scanner v1.0.0 - Initial Stable Release

**The first stable release of Prospector Scanner!** This release establishes the foundation for non-intrusive ZMK keyboard status monitoring with a professional YADS-style UI.

### 🚀 Key Features

#### 📱 YADS-Style Professional UI
- **Multi-widget display system** with connection status, layer indicators, and battery monitoring
- **Split keyboard support** with unified left/right display
- **Pastel color scheme** with 7 unique layer colors
- **Real-time updates** with 2-second refresh intervals
- **240x280 round LCD** optimized layout

#### 🎮 Universal Compatibility
- **Any ZMK keyboard** with BLE support
- **Non-intrusive design** - keyboards maintain full 5-device connectivity
- **Multi-keyboard support** - monitor up to 3 keyboards simultaneously
- **No pairing required** - uses BLE advertisements (observer mode)

#### 🔋 Smart Power Management
- **Activity-based intervals** with 2-second updates
- **Sleep mode compatibility** with advertisement control
- **Split keyboard battery display** showing both sides
- **Connection status monitoring** with visual indicators

### 📊 Technical Specifications

#### BLE Advertisement Protocol (26 bytes)
- **Manufacturer ID**: 0xFF 0xFF (Local use)
- **Service UUID**: 0xAB 0xCD (Prospector identifier)
- **Protocol version**: 0x01
- **Comprehensive status data**: Battery, layer, profile, connection status

#### Display Features
- **Montserrat font family** for professional typography
- **7-layer color system** with unique pastel colors
- **Split keyboard detection** and unified display
- **Real-time status updates** across all widgets

### 📦 Hardware Requirements

**Scanner Device (Prospector Hardware):**
- Seeeduino XIAO BLE (nRF52840)
- Waveshare 1.69" Round LCD (ST7789V, 240x280 pixels)
- USB Type-C power (5V)
- 3D printed Prospector enclosure

**Supported Keyboards:**
- Any ZMK-compatible keyboard with BLE support
- Split keyboards (Corne, Lily58, Sofle, etc.)
- Unibody keyboards (60%, TKL, etc.)
- Custom ZMK builds

### 📦 Downloads

**Scanner Firmware:**
- `prospector_scanner-seeeduino_xiao_ble-zmk.uf2` - Main scanner firmware
- `settings_reset-seeeduino_xiao_ble-zmk.uf2` - Settings reset utility

**Installation:**
1. Download the firmware file above
2. Enter bootloader mode (double-tap reset on XIAO)
3. Copy .uf2 file to XIAO drive
4. Device auto-reboots with v1.0.0

### ⚙️ Configuration

#### Keyboard Configuration
Add to your keyboard's `.conf` file:
```kconfig
# Essential configuration
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"
CONFIG_ZMK_STATUS_ADV_INTERVAL_MS=2000

# For split keyboards
CONFIG_ZMK_SPLIT_BLE_CENTRAL_BATTERY_LEVEL_FETCHING=y
CONFIG_ZMK_BATTERY_REPORTING=y
```

#### Scanner Configuration
```kconfig
CONFIG_PROSPECTOR_MODE_SCANNER=y
CONFIG_PROSPECTOR_MAX_KEYBOARDS=3
CONFIG_PROSPECTOR_MAX_LAYERS=7
```

### 🔄 Module Integration

Update your keyboard's `config/west.yml`:
```yaml
- name: prospector-zmk-module
  remote: prospector
  revision: v1.0.0
  path: modules/prospector-zmk-module
```

### 📖 Documentation
- [Complete Setup Guide](https://github.com/t-ogura/zmk-config-prospector#readme)
- [Hardware Wiring Guide](https://github.com/t-ogura/zmk-config-prospector#hardware-requirements)
- [Configuration Examples](https://github.com/t-ogura/zmk-config-prospector#quick-setup-examples)

### 🐛 Known Limitations
- Fixed 2-second update interval (improved in v1.1.0)
- Manual brightness control only (auto-brightness in v1.1.0)
- No scanner battery support (added in v1.1.0)
- Limited power optimization (15x improvement in v1.1.0)

### 🤝 Acknowledgments
Special thanks to:
- **ZMK firmware framework** for the excellent platform
- **YADS project** for UI design inspiration
- **Original Prospector project** by @carrefinho for hardware platform
- **Community testers** for validation and feedback

### 🔗 Links
- [Full Release Notes](docs/RELEASES/v1.0.0.md)
- [GitHub Repository](https://github.com/t-ogura/zmk-config-prospector)
- [Report Issues](https://github.com/t-ogura/zmk-config-prospector/issues)
- [Original Prospector](https://github.com/carrefinho/prospector)

---

**Prospector Scanner v1.0.0** - The beginning of professional ZMK keyboard monitoring.

*Built with ❤️ by the ZMK community.*