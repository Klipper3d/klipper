#!/bin/bash
# This script installs Klipper on Debian

SYSTEMDDIR="/etc/systemd/system"
PYTHONDIR="$HOME/klippy-env"
PRINTER_DATA="$HOME/printer_data"
KLIPPER_SERVICE="$HOME/klipper/scripts/klipper.service"
KLIPPER_MCU_SERVICE="$HOME/klipper/scripts/klipper-mcu.service"

# Step 1: Install system packages
install_packages() {
    PKGLIST="virtualenv python3-dev libffi-dev build-essential"
    PKGLIST+=" libncurses-dev libusb-dev"
    PKGLIST+=" avrdude gcc-avr binutils-avr avr-libc"
    PKGLIST+=" stm32flash libnewlib-arm-none-eabi gcc-arm-none-eabi"
    PKGLIST+=" binutils-arm-none-eabi libusb-1.0 pkg-config"

    report_status "Running apt-get update..."
    sudo apt-get update

    report_status "Installing packages..."
    sudo apt-get install --yes ${PKGLIST}
}

# Step 2: Create python virtual environment
create_virtualenv() {
    report_status "Updating python virtual environment..."

    [ ! -d ${PYTHONDIR} ] && virtualenv -p python3 ${PYTHONDIR}
    ${PYTHONDIR}/bin/pip install -r ${SRCDIR}/scripts/klippy-requirements.txt
}

# Step 3: Make directories
make_dir() {
    report_status "Making directories..."
    mkdir -p ${PRINTER_DATA}/{config,logs,gcodes,systemd,comms}
    touch ${PRINTER_DATA}/config/printer.cfg
}

# Step 4: Install args
install_args() {
    /bin/sh -c "cat > ${PRINTER_DATA}/systemd/klipper.env" << EOF
KLIPPER_ARGS="${HOME}/klipper/klippy/klippy.py
${PRINTER_DATA}/config/printer.cfg -l
${PRINTER_DATA}/logs/klippy.log -I
${PRINTER_DATA}/comms/klippy.serial -a
${PRINTER_DATA}/comms/klippy.sock"
EOF
}

# Step 4: Install startup script
install_klipper_service() {
    report_status "Installing klipper service..."
    sudo /bin/sh -c "cp $KLIPPER_SERVICE $SYSTEMDDIR/klipper.service"
    sudo /bin/sh -c "cp $KLIPPER_MCU_SERVICE $SYSTEMDDIR/klipper-mcu.service"

    sudo systemctl enable klipper-mcu.service
    sudo systemctl enable klipper.service
}

# Step 5: Start host software
start_software() {
    report_status "Launching Klipper host software..."
    sudo systemctl start klipper
}

# Helper functions
report_status() {
    printf "\n\n###### %s\n" "$1"
}

verify_ready() {
    if [ "$EUID" -eq 0 ]; then
        echo "This script must not run as root"
        exit -1
    fi
}

# Force script to exit if an error occurs
set -e

# Find SRCDIR from the pathname of this script
SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"

# Run installation steps defined above
verify_ready
install_packages
create_virtualenv
make_dir
install_args
install_klipper_service
start_software
