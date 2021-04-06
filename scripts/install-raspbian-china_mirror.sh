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
        
    # backing up the source list
    sudo mv /etc/apt/sources.list /etc/apt/sources.list.bak-klipper
    sudo mv /etc/apt/sources.list.d/raspi.list /etc/apt/sources.list.d/raspi.list.bak-klipper
    # sudo apt-get update
    # sudo apt-get install --yes apt-transport-https ca-certificates
    
    # Using Tsinghua University mirror
    # distro=$(lsb_release -d | sed -n 's/.*(\([a-z]*\))/\1/p')
    # source="deb https://mirrors.tuna.tsinghua.edu.cn/debian/ ${distro} main contrib non-free\n"
    # source="${source}deb https://mirrors.tuna.tsinghua.edu.cn/debian/ ${distro}-updates main contrib non-free\n"
    # source="${source}deb https://mirrors.tuna.tsinghua.edu.cn/debian/ ${distro}-backports main contrib non-free\n"
    # source="${source}deb https://mirrors.tuna.tsinghua.edu.cn/debian-security ${distro}/updates main contrib non-free\n"
    # sudo sed -i "1i\\${source}" /etc/apt/sources.list
    
    distro=$(cat /etc/os-release | sed -n '1s/.*(\([a-z]*\)).*/\1/p')
    source="deb http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ ${distro} main non-free contrib rpi\n"
    source="${source}deb-src http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ ${distro} main non-free contrib rpi"
    sudo cat ${source} > /etc/apt/source.list
    
    source="deb http://mirrors.tuna.tsinghua.edu.cn/raspberrypi/ ${distro} main ui"
    sudo cat ${source} > /etc/apt/sources.list.d/raspi.list
    
# sudo /bin/sh -c "cat > /etc/apt/source.list" << EOF
# deb http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ buster main non-free contrib rpi
# deb-src http://mirrors.tuna.tsinghua.edu.cn/raspbian/raspbian/ buster main non-free contrib rpi
# EOF
    
    # Update system package info
    report_status "Running apt-get update..."
    sudo apt-get update

    # Install desired packages
    report_status "Installing packages..."
    sudo apt-get install --yes ${PKGLIST}
    
    report_status "restore apt source list"
    sudo mv /etc/apt/sources.list.bak-klipper /etc/apt/sources.list
    sudo mv /etc/apt/sources.list.d/raspi.list.bak-klipper /etc/apt/sources.list.d/raspi.list
}

# Step 2: Create python virtual environment
create_virtualenv()
{
    report_status "Updating python virtual environment..."
    
    # Create virtualenv if it doesn't already exist
    [ ! -d ${PYTHONDIR} ] && virtualenv -p python2 ${PYTHONDIR} --no-setuptools --no-pip --no-wheel
    
    RM_CONFIG=false
    if [ -f ${HOME}/.config/pip/pip.conf ] ; then
        report_status 'backup pip config'
        cp ${HOME}/.config/pip/pip.conf ${HOME}/.config/pip/pip.conf.bak-klipper
    else
        RM_CONFIG=true
    fi
    
    ${PYTHONDIR}/bin/python <(curl -SsL https://gitee.com/miroky/get-pip/attach_files/638019/download/get-pip_2.7.py) -i https://pypi.tuna.tsinghua.edu.cn/simple
    # add tsinghua mirror
    ${PYTHONDIR}/bin/pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
    # Install/update dependencies
    ${PYTHONDIR}/bin/pip install -r ${SRCDIR}/scripts/klippy-requirements.txt
    
    if  ! ${RM_CONFIG}  ; then
        mv ${HOME}/.config/pip/pip.conf.bak-klipper ${HOME}/.config/pip/pip.conf
    else
        report_status "Cleaning up...."
        rm ${HOME}/.config/pip/pip.conf
    fi
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
ExecStart=${PYTHONDIR}/bin/python ${SRCDIR}/klippy/klippy.py ${HOME}/printer.cfg -l ${KLIPPER_LOG}
Restart=always
RestartSec=10
EOF
# Use systemctl to enable the klipper systemd service script
    sudo systemctl enable klipper.service
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
