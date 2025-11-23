# Prospector ZMK Config

Configuration files for Prospector Scanner (v2.0.0+)

## Configuration Files

- `prospector_scanner.conf`: Default configuration (non-touch mode, 7 layers)
- `prospector_scanner_touch.conf`: Touch mode configuration (10 layers with runtime adjustment)

## Building

### Default (Non-Touch Mode)

```bash
west build -b seeeduino_xiao_ble -s zmk/app -- \
  -DSHIELD=prospector_scanner \
  -DZMK_CONFIG="/absolute/path/to/zmk-config-prospector/config"
```

### Touch Mode

**Method 1: Specify touch config file** (Recommended)
```bash
# Copy touch config to default location
cp config/prospector_scanner_touch.conf config/prospector_scanner.conf

# Then build as normal
west build -b seeeduino_xiao_ble -s zmk/app -- \
  -DSHIELD=prospector_scanner \
  -DZMK_CONFIG="/absolute/path/to/zmk-config-prospector/config"
```

**Method 2: Edit config file**
```bash
# Edit config/prospector_scanner.conf
# Change: CONFIG_PROSPECTOR_TOUCH_ENABLED=n
# To:     CONFIG_PROSPECTOR_TOUCH_ENABLED=y
# Change: CONFIG_PROSPECTOR_MAX_LAYERS=7
# To:     CONFIG_PROSPECTOR_MAX_LAYERS=10

# Then build
west build ...
```

## Key Configuration Options

### Touch Mode Control
```conf
CONFIG_PROSPECTOR_TOUCH_ENABLED=y   # Enable touch mode
CONFIG_PROSPECTOR_TOUCH_ENABLED=n   # Disable touch mode (default)
```

### Layer Display
```conf
CONFIG_PROSPECTOR_MAX_LAYERS=7   # Non-touch mode (fixed)
CONFIG_PROSPECTOR_MAX_LAYERS=10  # Touch mode (adjustable via Display Settings)
```

### Brightness Control
```conf
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=y  # Auto brightness (requires APDS9960)
CONFIG_PROSPECTOR_USE_AMBIENT_LIGHT_SENSOR=n  # Fixed brightness (default)
CONFIG_PROSPECTOR_FIXED_BRIGHTNESS=85         # Fixed brightness level
```

### Battery Support
```conf
CONFIG_PROSPECTOR_BATTERY_SUPPORT=y  # Enable battery monitoring
CONFIG_PROSPECTOR_BATTERY_SUPPORT=n  # Disable (default)
```

## Version

- Module: `v2.0.0-scanner-touch`
- Config: `feature/v2.0-scanner-touch`

See [CLAUDE.md](../../CLAUDE.md) for detailed development documentation.
