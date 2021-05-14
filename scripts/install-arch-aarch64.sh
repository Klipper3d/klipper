#!/bin/bash
# This script installs Klipper on an aarch64 Arch Linux system

PYTHONDIR="${HOME}/klippy-env"
SYSTEMDDIR="/etc/systemd/system"
CONFIGDIR="${HOME}/klipper_config"
KLIPPER_USER=$USER
KLIPPER_GROUP=$KLIPPER_USER

# Step 1: Install Paru to handle AUR builds
install_paru()
{
    # Packages needed to install Paru
    PKGLIST="git base-devel"

    # Install desired packages
     report_status "Installing Paru dependencies..."

     sudo pacman -S --needed ${PKGLIST}

     # Clone paru-bin
     git clone https://aur.archlinux.org/paru-bin.git ~/paru-bin

     # Install Paru
     report_status "Installing Paru..."
     cd ~/paru-bin
     makepkg -si

     # Remove git repo
     cd ~
     sudo rm -rf paru-bin
}

# Step 2: Install Packages
install_packages()
{
    # Packages for python cffi
    PKGLIST="python2-virtualenv libffi"
    # kconfig requirements
    PKGLIST="${PKGLIST} ncurses"
    # hub-ctrl
    PKGLIST="${PKGLIST} libusb"
    # AVR chip installation and building
    PKGLIST="${PKGLIST} avrdude avr-gcc avr-binutils avr-libc"

    # Install desired packages
    report_status "Installing packages..."
    paru -S --needed ${PKGLIST}
     
    # Install ARM build package. PKGBUILD recompresses it, so this takes a *LONG* time
    # even on a Pi 4
    report_status "Installing arm-none-eabi-gcc. This will take some time..."
    paru -S --skip-review gcc-arm-none-eabi-bin

    # Install stm32flash. Paru will warn about incompatable architecture, but aarch64 is backwards
    # compatabile, so this works fine.
    report_status "Installing stm32flash. Please ignore incompatability error warning"
    paru -S --skip-review stm32flash
}

# Step 2: Create python virtual environment
create_virtualenv()
{
    report_status "Updating python virtual environment..."

    # Create virtualenv if it doesn't already exist
    [ ! -d ${PYTHONDIR} ] && virtualenv2 ${PYTHONDIR}

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
RemainAfterExit=yes
ExecStart=${PYTHONDIR}/bin/python ${SRCDIR}/klippy/klippy.py ${CONFIGDIR}/printer.cfg -l ${KLIPPER_LOG} -a /tmp/klippy_uds
EOF
# Use systemctl to enable the klipper systemd service script
    sudo systemctl enable klipper.service
    report_status "Make sure to add $KLIPPER_USER to the user group controlling your serial printer port"
    report_status "On Arch use sudo usermod -aG uucp $KLIPPER_USER"
}

# Step 4: Start host software
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

    if [[ "$(uname -m)" != "aarch64" ]]; then
        echo "This script is only compatabile with aarch64-based installations"
        exit -1
    fi
}

# Force script to exit if an error occurs
set -e

# Find SRCDIR from the pathname of this script
SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"

# Run installation steps defined above
verify_ready
install_paru
install_packages
create_virtualenv
install_script
start_software
