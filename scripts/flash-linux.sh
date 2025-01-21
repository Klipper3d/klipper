#!/bin/bash
# This script installs the Linux MCU code to /usr/local/bin/

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
rm -f /usr/local/bin/klipper_mcu
cp ${out}/klipper.elf /usr/local/bin/klipper_mcu
sync

# Restart (if system install script present)
if [ -f /etc/init.d/klipper_pru ]; then
    echo "Attempting host PRU restart..."
    service klipper_pru restart
fi

# Restart (if system install script present)
if [ -f /etc/init.d/klipper_mcu ]; then
    echo "Attempting host MCU restart..."
    service klipper_mcu restart
fi

if [ -f /etc/systemd/system/klipper-mcu.service ]; then
    echo "Attempting host MCU restart..."
    systemctl restart klipper-mcu
fi
