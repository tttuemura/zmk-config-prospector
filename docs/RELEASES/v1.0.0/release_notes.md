# Prospector Scanner v1.0.0 Release Notes

**Release Date**: 2025-08-03  
**Version**: 1.0.0  
**Codename**: "Initial Stable Release"  
**Status**: Stable  

## 🎉 Overview

Prospector Scanner v1.0.0 marks the first stable release of the independent BLE scanner mode for ZMK keyboards. This release establishes the foundation for non-intrusive keyboard status monitoring with a professional YADS-style UI.

## 🚀 Key Features

### 📱 YADS-Style Professional UI
- Multi-widget display system
- Connection status indicators
- Layer visualization with pastel colors
- Modifier key status
- Battery level monitoring
- Split keyboard support

### 🔋 Smart Power Management
- Activity-based update intervals
- 2-second update frequency
- Sleep mode compatibility
- Non-intrusive BLE advertising

### 🎮 Universal Compatibility
- Works with any ZMK keyboard
- Split keyboard ready
- Multi-keyboard support (up to 3)
- No pairing required

## 📊 Technical Specifications

### BLE Advertisement Protocol (26 bytes)
- Manufacturer ID: 0xFF 0xFF
- Service UUID: 0xAB 0xCD
- Protocol version: 0x01
- Comprehensive status data

### Display Features
- 240x280 pixel round LCD
- Montserrat font family
- 7-layer color system
- Real-time updates

## 📦 Hardware Requirements

- Seeeduino XIAO BLE (nRF52840)
- Waveshare 1.69" Round LCD
- USB Type-C power
- 3D printed Prospector case

## 🔧 Configuration

### Keyboard Configuration
```kconfig
CONFIG_ZMK_STATUS_ADVERTISEMENT=y
CONFIG_ZMK_STATUS_ADV_KEYBOARD_NAME="MyKeyboard"
CONFIG_ZMK_STATUS_ADV_INTERVAL_MS=2000
```

### Scanner Configuration
```kconfig
CONFIG_PROSPECTOR_MODE_SCANNER=y
CONFIG_PROSPECTOR_MAX_KEYBOARDS=3
CONFIG_PROSPECTOR_MAX_LAYERS=7
```

## 🐛 Known Limitations

- Fixed 2-second update interval
- No battery operation support
- Manual brightness control only
- Limited power optimization

## 🤝 Acknowledgments

- ZMK firmware framework
- YADS project for UI inspiration
- Community testers and contributors
- Original Prospector hardware platform

---

**Prospector Scanner v1.0.0** - The beginning of professional ZMK keyboard monitoring.

*Built with ❤️ by the ZMK community.*