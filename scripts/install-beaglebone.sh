#!/bin/bash
# This script installs Klipper on a Beaglebone running Debian Bullseye
# for use with its PRU micro-controller.

# Step 1: Do main install
install_main()
{
    # Run the debian script - should
    # work.
    ${SRCDIR}/scripts/install-debian.sh
}

# Step 2: Install additional system packages
install_packages()
{
    # Remove conflicting AVR packages
    PKGLIST_REMOVE="avrdude gcc-avr binutils-avr avr-libc"

    report_status "Removing ARM packages because of conflicts with PRU packages"
    sudo apt-get remove --yes ${PKGLIST_REMOVE}

    # Install desired packages
    PKGLIST="gcc-pru"

    report_status "Installing beaglebone packages..."
    sudo apt-get install --yes ${PKGLIST}
}

# Step 3: Install startup script
install_script()
{
    report_status "Installing pru start script..."
    sudo cp "${SRCDIR}/scripts/klipper-pru-start.sh" /etc/init.d/klipper_pru
    sudo update-rc.d klipper_pru defaults-disabled
}

# Step 4: Install pru udev rule
install_udev()
{
    report_status "Installing pru udev rule..."
    sudo /bin/sh -c "cat > /etc/udev/rules.d/pru.rules" <<EOF
SUBSYSTEM=="remoteproc", ENV{REMOTEPROC_NAME}!="", TAG+="systemd", ENV{SYSTEMD_WANTS}="klipper_pru.service"
KERNEL=="rpmsg_pru30", GROUP="tty", MODE="0660"
EOF
}

# Step 5: Add user to tty group
install_groups()
{
    report_status "Adding $USER to tty group..."
    sudo adduser $USER tty
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
install_main
install_packages
install_script
install_udev
install_groups
