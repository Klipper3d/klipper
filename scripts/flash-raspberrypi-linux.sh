#!/bin/bash
# This script installs the Raspberry Pi Linux MCU code to /usr/local/bin/

if [ "$EUID" -ne 0 ]; then
    echo "This script must be run as root"
    exit -1
fi
set -e

# Setting build output directory
if [ -z "${1}" ]; then
    out='out'
else
    out=${1}
fi

# Install new micro-controller code
echo "Installing micro-controller code to /usr/local/bin/"
rm -f /usr/local/bin/klipper_raspberrypi_mcu
cp ${out}/klipper.elf /usr/local/bin/klipper_raspberrypi_mcu

# Restart (if system install script present)
if [ -f /etc/init.d/klipper_raspberrypi_mcu ]; then
    echo "Attempting host MCU restart..."
    service klipper_raspberrypi_mcu restart
fi

if [ -f /etc/systemd/system/klipper-raspberrypi-mcu.service ]; then
    echo "Attempting host MCU restart..."
    systemctl restart klipper-raspberrypi-mcu
fi
