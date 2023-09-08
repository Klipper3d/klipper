#!/bin/bash
# This script installs the necessary build tools for the MCU code on MacOS

install_packages()
{
    PKGLIST=""
    TAPLIST="osx-cross/homebrew-avr osx-cross/homebrew-arm"
    # AVR chip installation and building
    PKGLIST="${PKGLIST} avrdude avr-gcc"
    # ARM chip installation and building
    PKGLIST="${PKGLIST} stm32flash wxwidgets arm-gcc-bin"

    # Tap necessary package repos
    report_status "Tapping formula repositories..."
    for TAP in $TAPLIST
    do
        brew tap $TAP
    done
    brew update

    # Install desired packages
    report_status "Installing packages..."
    brew install ${PKGLIST}
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

verify_ready
install_packages
