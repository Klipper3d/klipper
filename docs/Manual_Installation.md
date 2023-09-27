# Manual Installation

These instructions are for people who want to run klipper on
custom system that's not debian-based. Flashing steps are same
as in [Installation](Installation.md) instruction.
Naturally this instruction is for more advanced users.

# Preparing for installation

First you need to install required packages. This step is different
for every other package manager and distribution so provided are
debian packages and fiding package names for your distribution are
up to you.

```
virtualenv python-dev libffi-dev build-essential libncurses-dev
libusb-dev avrdude gcc-avr binutils-avr avr-libc stm32flash
libnewlib-arm-none-eabi gcc-arm-none-eabi binutils-arm-none-eabi
libusb-1.0 pkg-config
```

Next up as it's not recommended to run klipper as root you need to
make user that will be running klipper

```
useradd -ms /bin/bash klippy
usermod -aG tty klippy
usermod -aG dialout klippy
```

Then after you login as a user you need to go to desired installation
directory which can be home directory, clone klipper repo

```
git clone https://github.com/Klipper3d/klipper.git
```

At this point it would be best to follow step second step in [Installation](Installation.md#obtain-a-klipper-configuration-file)
and obtain the configuration file.

# Creating python virtual environment

We need to create python virtual environment now. Place for it is
entirely up to you but it has to be done from user and accessible.

```
# Assuming we're in desired directory
virtualenv .
# Replace klipper_directory with path to klipper
./bin/pip install -r klipper_directory/scripts/klippy-requirements.txt
```

Python3 works fine, you might have issues with gcc13 and above.
After successful installation you need to install service.
Depending on if you're on systemd or init there are two options.

## Init

Copy `scripts/klipper-start.sh` from klipper directory to `/etc/init.d/klipper`
then run: `sudo update-rc.d klipper defaults`
Afterwards we need to create defaults file

```
sudo /bin/sh -c "cat > /etc/default/klipper" <<EOF
# Configuration for /etc/init.d/klipper

KLIPPY_USER=klippy

KLIPPY_EXEC=venv_directory/bin/python

KLIPPY_ARGS="klipper_directory/klippy/klippy.py config_directory/printer.cfg -l /tmp/klippy.log"

EOF
```

Of course you need to adjust user, `venv_directory`, `klipper_directory`,
`config_directory` and log file location.
Then you can start it using `sudo /etc/init.d/klipper restart`

## Systemd

As a root, create file in `/etc/systemd/system/klipper.service` with content:

```
#Systemd service file for klipper
[Unit]
Description=Starts klipper on startup
After=network.target

[Service]
Type=simple
User=klippy
RemainAfterExit=yes
ExecStart=venv_directory/bin/python klipper_directory/klippy/klippy.py config_directory/printer.cfg -l /tmp/klippy.log
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

Of course you need to adjust user, `venv_directory`, `klipper_directory`,
`config_directory` and log file location.
Afterwards you can start it using `systemctl start klipper`.

# Finally

Installation is now done and you can proceed with rest
of the [Installation](Installation.md).
