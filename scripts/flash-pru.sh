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

# Shutdown existing Klipper instance (if applicable). The goal is to
# put the GPIO pins in a safe state.
if [ -c /dev/rpmsg_pru30 ]; then
    echo "Attempting to shutdown existing firmware..."
    ( echo "FORCE_SHUTDOWN" > /dev/rpmsg_pru30 ) 2> /dev/null || ( echo "Firmware busy! Please shutdown Klipper and then retry." && exit 1 )
    sleep 1
    ( echo "FORCE_SHUTDOWN" > /dev/rpmsg_pru30 ) 2> /dev/null || ( echo "Firmware busy! Please shutdown Klipper and then retry." && exit 1 )
    sleep 1
fi
set +e

# Restart the PRU
echo "Restarting pru_rproc module"
rmmod -f pru_rproc
modprobe pru_rproc
