#!/bin/bash
# Local build script for Prospector Scanner
# This script handles the correct paths and build sequence

set -e  # Exit on error

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CONFIG_DIR="${SCRIPT_DIR}/config"

echo "=== Prospector Scanner Local Build ===="
echo "Config directory: ${CONFIG_DIR}"

# Activate virtual environment
source "${SCRIPT_DIR}/.venv/bin/activate"

# Build the firmware
west build --pristine \
  -s zmk/app \
  -b seeeduino_xiao_ble \
  -S zmk-usb-logging \
  -- \
  -DZMK_CONFIG="${CONFIG_DIR}" \
  -DSHIELD=prospector_scanner

echo ""
echo "=== Build Complete ===="
echo "Firmware: ${SCRIPT_DIR}/build/zephyr/zmk.uf2"
echo "Size: $(du -h ${SCRIPT_DIR}/build/zephyr/zmk.uf2 | cut -f1)"
