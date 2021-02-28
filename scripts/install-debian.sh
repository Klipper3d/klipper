#!/bin/bash
# This script installs Klipper on an debian
#

PYTHONDIR="${HOME}/klippy-env"
SYSTEMDDIR="/etc/systemd/system"
KLIPPER_USER=$USER
KLIPPER_GROUP=$KLIPPER_USER

# Step 1: Install system packages
install_packages()
{
    # Packages for python cffi
    PKGLIST="virtualenv python-dev libffi-dev build-essential"
    # kconfig requirements
    PKGLIST="${PKGLIST} libncurses-dev"
    # hub-ctrl
    PKGLIST="${PKGLIST} libusb-dev"
    # AVR chip installation and building
    PKGLIST="${PKGLIST} avrdude gcc-avr binutils-avr avr-libc"
    # ARM chip installation and building
    PKGLIST="${PKGLIST} stm32flash libnewlib-arm-none-eabi"
    PKGLIST="${PKGLIST} gcc-arm-none-eabi binutils-arm-none-eabi libusb-1.0"

    # Update system package info
    report_status "Running apt-get update..."
    sudo apt-get update

    # Install desired packages
    report_status "Installing packages..."
    sudo apt-get install --yes ${PKGLIST}
}

# Step 2: Create python virtual environment
create_virtualenv()
{
    report_status "Updating python virtual environment..."

    # Create virtualenv if it doesn't already exist
    [ ! -d ${PYTHONDIR} ] && virtualenv -p python2 ${PYTHONDIR}

    # Install/update dependencies
    ${PYTHONDIR}/bin/pip install -r ${SRCDIR}/scripts/klippy-requirements.txt
}

# Step 3: Install startup script
install_script()
{
# Create systemd service file
    KLIPPER_LOG=/tmp/klippy.log
    report_status "Installing system start script..."
    sudo /bin/sh -c "cat > $SYSTEMDDIR/klipper.service" << EOF
#Systemd service file for klipper
[Unit]
Description=Starts klipper on startup
After=network.target

[Install]
WantedBy=multi-user.target

[Service]
Type=simple
User=$KLIPPER_USER
ExecStart=${PYTHONDIR}/bin/python ${SRCDIR}/klippy/klippy.py ${HOME}/klipper_config/printer.cfg -l ${KLIPPER_LOG} -a /tmp/klippy_uds
Restart=always
RestartSec=5
EOF
# Use systemctl to enable the klipper systemd service script
    sudo systemctl enable klipper.service
}

# Step 4: Install linux mcu startup script
install_script1()
{
# Create systemd service file
    PIDFILE=/var/run/klipper_mcu.pid
    report_status "Installing linux mcu system start script1..."
    sudo /bin/sh -c "cat > $SYSTEMDDIR/klipper_host_mcu.service" << EOF
#Systemd service file for klipper-linux-host-mcu
[Unit]
Description=klipper linux host
PartOf=klipper.service

[Service]
Type=simple
User=$KLIPPER_USER
ExecStart=/usr/local/bin/klipper_mcu
Restart=always
RestartSec=5

[Install]
WantedBy=multi-user.target
EOF
# Use systemctl to enable the klipper systemd service script
    sudo systemctl enable klipper_host_mcu.service
}

#step 5 make klipper_config directory
add_klipconf()
{
    report_status "make klipper_config directory "
    FILE=~/klipper_config
    if [ -d "$FILE" ]; then
        echo "$FILE exist"
    else
        echo "$FILE does not exist"
        mkdir ~/klipper_config
        fi
}

# Step 6: Start host software
start_software()
{
    report_status "Launching Klipper host software..."
    sudo systemctl start klipper
}

# Helper functions
report_status()
{
    echo -e "\n\n###### $1"
}

verify_ready()
{
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
install_script
install_script1
add_klipconf
start_software
