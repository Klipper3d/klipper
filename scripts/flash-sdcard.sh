#!/bin/bash
#  This script launches flash_sdcard.py, a utitlity that enables
#  unattended firmware updates on boards with "SD Card" bootloaders

# Non-standard installations may need to change this location
KLIPPY_ENV="${HOME}/klippy-env/bin/python"
SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"
KLIPPER_BIN="${SRCDIR}/out/klipper.bin"
SPI_FLASH="${SRCDIR}/scripts/spi_flash/spi_flash.py"
BAUD_ARG=""
# Force script to exit if an error occurs
set -e

print_help_message()
{
    echo "SD Card upload utility for Klipper"
    echo
    echo "usage: flash_sdcard.sh [-h] [-l] [-b <baud>] [-f <firmware>]"
    echo "                       <device> <board>"
    echo
    echo "positional arguments:"
    echo "  <device>        device serial port"
    echo "  <board>         board type"
    echo
    echo "optional arguments:"
    echo "  -h              show this message"
    echo "  -l              list available boards"
    echo "  -b <baud>       serial baud rate (default is 250000)"
    echo "  -f <firmware>   path to klipper.bin"
}

# Parse command line "optional args"
while getopts "hlb:f:" arg; do
    case $arg in
        h)
            print_help_message
            exit 0
            ;;
        l)
            ${KLIPPY_ENV} ${SPI_FLASH} -l
            exit 0
            ;;
        b) BAUD_ARG="-b ${OPTARG}";;
        f) KLIPPER_BIN=$OPTARG;;
    esac
done

# Make sure that we have the correct number of positional args
if [ $(($# - $OPTIND + 1)) -ne 2 ]; then
    echo "Invalid number of args: $(($# - $OPTIND + 1))"
    exit -1
fi

DEVICE=${@:$OPTIND:1}
BOARD=${@:$OPTIND+1:1}

if [ ! -f $KLIPPER_BIN ]; then
    echo "No file found at '${KLIPPER_BIN}'"
    exit -1
fi

if [ ! -e $DEVICE ]; then
    echo "No device found at '${DEVICE}'"
    exit -1
fi

# Run Script
echo "Flashing ${KLIPPER_BIN} to ${DEVICE}"
${KLIPPY_ENV} ${SPI_FLASH} ${BAUD_ARG} ${DEVICE} ${BOARD} ${KLIPPER_BIN}
