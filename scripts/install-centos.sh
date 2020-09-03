#!/bin/bash
# This script installs Klipper on an x86_64 machine running the
# CentOS 7 distribution.

PYTHONDIR="${HOME}/klippy-env"
SYSTEMDDIR="/etc/systemd/system"

# Step 1: Install system packages
install_packages()
{
    # Packages for python cffi
    PKGLIST="python-virtualenv libffi-devel"
    # kconfig requirements
    PKGLIST="${PKGLIST} ncurses-devel"
    # hub-ctrl
    PKGLIST="${PKGLIST} libusb-devel"
    # AVR chip installation and building
    PKGLIST="${PKGLIST} avrdude gcc-avr32-linux-gnu binutils-avr32-linux-gnu avr-libc"
    # ARM chip installation and building
    # CentOS/Fedora do not appear to have these packages available at this time
    PKGLIST="${PKGLIST} arm-none-eabi-gcc-cs arm-none-eabi-newlib"

    # Install desired packages
    report_status "Installing packages..."
    sudo yum install -y ${PKGLIST}
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
User=$USER
RemainAfterExit=yes
ExecStart=${PYTHONDIR}/bin/python ${SRCDIR}/klippy/klippy.py ${HOME}/printer.cfg -l /var/log/klippy.log
EOF
# Use systemctl to enable the klipper systemd service script
    sudo systemctl enable klipper.service
}

# Configuration for systemctl klipper

KLIPPY_USER=$USER


# Step 5: Start host software
start_software()
{
    report_status "Launching Klipper host software..."
    sudo systemctl klipper restart
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
start_software
