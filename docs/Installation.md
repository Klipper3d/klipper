# Installation

These instructions assume the software will run on a linux based host
running a Klipper compatible front end. It is recommended that a
SBC(Small Board Computer) such as a Raspberry Pi or Debian based Linux
device be used as the host machine (see the
[FAQ](FAQ.md#can-i-run-klipper-on-something-other-than-a-raspberry-pi-3)
for other options).

For the purposes of these instructions host relates to the Linux device and
mcu relates to the printboard. SBC relates to the term Small Board Computer
such as the Raspberry Pi.

## Obtain a Klipper Configuration File

Most Klipper settings are determined by a "printer configuration file"
printer.cfg, that will be stored on the host. An appropriate configuration
file can often be found by looking in the Klipper
[config directory](../config/) for a file starting with a "printer-"
prefix that corresponds to the target printer. The Klipper
configuration file contains technical information about the printer
that will be needed during the installation.

If there isn't an appropriate printer configuration file in the
Klipper config directory then try searching the printer manufacturer's
website to see if they have an appropriate Klipper configuration file.

If no configuration file for the printer can be found, but the type of
printer control board is known, then look for an appropriate
[config file](../config/) starting with a "generic-" prefix. These
example printer board files should allow one to successfully complete
the initial installation, but will require some customization to
obtain full printer functionality.

It is also possible to define a new printer configuration from
scratch. However, this requires significant technical knowledge about
the printer and its electronics. It is recommended that most users
start with an appropriate configuration file. If creating a new custom
printer configuration file, then start with the closest example
[config file](../config/) and use the Klipper
[config reference](Config_Reference.md) for further information.

## Interacting with Klipper

Klipper is a 3d printer firmware, so it needs some way for the user to
interact with it.

Currently the best choices are front ends that retrieve information through
the [Moonraker web API](https://moonraker.readthedocs.io/) and there is also
the option to use [Octoprint](https://octoprint.org/) to control Klipper.

The choice is up to the user on what to use, but the underlying Klipper is the
same in all cases. We encourage users to research the options available and
make an informed decision.

## Obtaining an OS image for SBC's

There are many ways to obtain an OS image for Klipper for SBC use, most depend on
what front end you wish to use. Some manafactures of these SBC boards also provide
their own Klipper-centric images.

The two main Moonraker based front ends are [Fluidd](https://docs.fluidd.xyz/)
and [Mainsail](https://docs.mainsail.xyz/), the latter of which has a premade install
image ["MainsailOS"](http://docs.mainsailOS.xyz), this has the option for Raspberry Pi
and some OrangePi varianta.

Fluidd can be installed via KIAUH(Klipper Install And Update Helper), which
is explained below and is a 3rd party installer for all things Klipper.

OctoPrint can be installed via the popular OctoPi image or via KIAUH, this
process is explained in [OctoPrint.md](OctoPrint.md)

## Installing via KIAUH

Normally you would start with a base image for your SBC, RPiOS Lite for example,
or in the case of a x86 Linux device, Ubuntu Server. Please note that Desktop
variants are not recommended due to certain helper programs that can stop some
Klipper functions working and even mask access to some print boards.

KIAUH can be used to install Klipper and its associated programs on a variety
of Linux based systems that run a form of Debian. More information can be found
at https://github.com/dw-0/kiauh

## Building and flashing the micro-controller

To compile the micro-controller code, start by running these commands
on your host device:

```
cd ~/klipper/
make menuconfig
```

The comments at the top of the
[printer configuration file](#obtain-a-klipper-configuration-file)
should describe the settings that need to be set during "make
menuconfig". Open the file in a web browser or text editor and look
for these instructions near the top of the file. Once the appropriate
"menuconfig" settings have been configured, press "Q" to exit, and
then "Y" to save. Then run:

```
make
```

If the comments at the top of the
[printer configuration file](#obtain-a-klipper-configuration-file)
describe custom steps for "flashing" the final image to the printer
control board then follow those steps and then proceed to
[configuring OctoPrint](#configuring-octoprint-to-use-klipper).

Otherwise, the following steps are often used to "flash" the printer
control board. First, it is necessary to determine the serial port
connected to the micro-controller. Run the following:

```
ls /dev/serial/by-id/*
```

It should report something similar to the following:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

It's common for each printer to have its own unique serial port name.
This unique name will be used when flashing the micro-controller. It's
possible there may be multiple lines in the above output - if so,
choose the line corresponding to the micro-controller. If many
items are listed and the choice is ambiguous, unplug the board and
run the command again, the missing item will be your print board(see the
[FAQ](FAQ.md#wheres-my-serial-port) for more information).

For common micro-controllers with STM32 or clone chips, LPC chips and
others it is usual that these need an initial Klipper flash via SD card.

When flashing with this method, it is important to make sure that the
print board is not connected with USB to the host, due to some boards
being able to feed power back to the board and stopping a flash from
occuring.

For common micro-controllers using Atmega chips, for example the 2560,
the code can be flashed with something
similar to:

```
sudo service klipper stop
make flash FLASH_DEVICE=/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
sudo service klipper start
```

Be sure to update the FLASH_DEVICE with the printer's unique serial
port name.

For common micro-controllers using RP2040 chips, the code can be flashed
with something similar to:

```
sudo service klipper stop
make flash FLASH_DEVICE=first
sudo service klipper start
```

It is important to note that RP2040 chips may need to be put into Boot mode
before this operation.


## Configuring Klipper

The next step is to copy the
[printer configuration file](#obtain-a-klipper-configuration-file) to
the host.

Arguably the easiest way to set the Klipper configuration file is using the
built in editors in Mainsail or Fluidd. These will allow the user to open
the configuration examples and save them to be printer.cfg.

Another option is to use a desktop editor that supports editing files
over the "scp" and/or "sftp" protocols. There are freely available tools
that support this (eg, Notepad++, WinSCP, and Cyberduck).
Load the printer config file in the editor and then save it as a file
named "printer.cfg" in the home directory of the pi user
(ie, /home/pi/printer.cfg).

Alternatively, one can also copy and edit the file directly on the
host via ssh. That may look something like the following (be
sure to update the command to use the appropriate printer config
filename):

```
cp ~/klipper/config/example-cartesian.cfg ~/printer.cfg
nano ~/printer.cfg
```

It's common for each printer to have its own unique name for the
micro-controller. The name may change after flashing Klipper, so rerun
these steps again even if they were already done when flashing. Run:

```
ls /dev/serial/by-id/*
```

It should report something similar to the following:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

Then update the config file with the unique name. For example, update
the `[mcu]` section to look something similar to:

```
[mcu]
serial: /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

After creating and editing the file it will be necessary to issue a
"restart" command in the command console to load the config. A
"status" command will report the printer is ready if the Klipper
config file is successfully read and the micro-controller is
successfully found and configured.

When customizing the printer config file, it is not uncommon for
Klipper to report a configuration error. If an error occurs, make any
necessary corrections to the printer config file and issue "restart"
until "status" reports the printer is ready.

Klipper reports error messages via the command console and via pop up in
Fluidd and Mainsail. The "status" command can be used to re-report error
messages. A log is available and usually located in ~/printer_data/logs
this is named klippy.log

After Klipper reports that the printer is ready, proceed to the
[config check document](Config_checks.md) to perform some basic checks
on the definitions in the config file. See the main
[documentation reference](Overview.md) for other information.
