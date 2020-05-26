#!/bin/bash
# This script installs the Linux MCU code to /usr/local/bin/

if [ "$#" -ge 1 ]; then
    OUT=$1
else
    OUT=out/
fi

if [ "$EUID" -ne 0 ]; then
    echo "This script must be run as root"
    exit -1
fi
set -e

# Install new micro-controller code
echo "Installing mirco-controller code to /usr/local/bin/"
rm -f /usr/local/bin/klipper_mcu
cp $OUT/klipper.elf /usr/local/bin/klipper_mcu
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
