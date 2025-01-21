#!/bin/bash
# This script installs the PRU firmware on a beaglebone machine.

if [ "$EUID" -ne 0 ]; then
    echo "This script must be run as root"
    exit -1
fi
set -e

# Install new firmware
echo "Installing firmware to /lib/firmware/"
cp out/pru0.elf /lib/firmware/am335x-pru0-fw
cp out/pru1.elf /lib/firmware/am335x-pru1-fw
sync

# Restart (if system install script present)
if [ -f /etc/init.d/klipper_pru ]; then
    echo "Attempting PRU restart..."
    service klipper_pru restart
fi
