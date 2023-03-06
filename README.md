# Mightyboard Rev G and H support - Fork of Klipper3d/klipper

This fork adds support for Makerbot Replicator2/2X
(Mightyboard rev g and h) and some clones.

**STATUS** - I would consider master to be at a late beta stage.
Everything that can be supported is at this point and all
features are working.  This has been tested on a small number
of printers and more wide spread testing would be appreciated.
The example config file will be updated to incorporate
setting hold currents while pre-heating.

**Supported clones**
Currently, no clones have example config files or instructions
for flashing Klipper.  If you have a Replicator clone that
uses k type thermocouples and/or an ADS1118 adc chip and
want to get Klipper running on it let me know and submit
a sample config file and flashing instructions and I will
include them in this repo.

Printers that may use ADS1118 ADCs and may work with this
repo include:
- FlashForge DreamerNX
- FlashForge Dreamer
- FlashForge Inventor
- Dremel 3D20
- Dremel 3D40
- PowerSpec Ultra 3D
- Monoprice Inventor 1

**Changes include:**
* Documents what is necessary to flash the MCU (57600 baud and correct 
    avrdude protocol)
* Adds support for MCU reset (toggle connection baud rate to 57600)
* Adds support for ADS1118 and k-type thermocouples (one or two extruders)
* Adds support for hardware blinking of the LEDs
* Adds support for multiple buttons with same action in display
* Removes the need for specifying dummy pins for software spi devices
* Adds example g code macros to emulate some original Makerbot behaviors

/config/printer-makerbot-replicator2x-2012.cfg can be used as a starting point 
for both 2 and 2x printers.  Replicator2 printers can simply delete or comment
the second extruder and the heated bed plate settings.

# Installation

NOTE - I am using Mainsail OS for these install instructions but you can
use any frontend and install method you want.  The only changes needed from
this repo are in Klipper.

Create an SD card with Mainsail OS (using Raspberry Pi Imager).  Follow the
instructions from Mainsail OS https://docs.mainsail.xyz/setup/mainsail-os
Note that there is a bug in Mainsail OS v1.0.1 that will cause the wifi
connection to bounce up and down until Sonar is updated.  To work around
this bug, once the Pi has booted IMMEDIATELY SSH into the Pi and execute
"systemctl stop sonar".  After that navigate to the Machine page and update
all componentshttps.

SSH into the Pi (user pi, password is what you set up when you created
the SD card).  Execute the following commands:
mv klipper klipper-orig
git clone https://github.com/dockterj/klipper
systemctl restart klipper

At this point if you refresh update manager on the machine page it will
say that klipper is invalid.  Ignore this and don't click on "hard recovery"
or "soft recovery."  Klipper should also report ERROR.  This is normal as
there is no printer configured yet.

Copy /config/printer-makerbot-replicator2x-2012.cfg to printer.cfg.  An
easy way to do this is to change the root directory in Config File
(on the Machine tab) to config_examples.  Find the
printer-makerbot-replicator2x-2012.cfg file, right click and "download"
it.  Change the root directory back to config, upload that file, then
rename it to printer.cfg.

Plug the printer in to the Pi's usb port and verify that the device
appears in /dev/serial/by-id by executing the following:
ls /dev/serial/by-id
Make a note of the name of the printer so you can update
printer.cfg in the next steps.

Edit this file to add/remove features specific to your printer
(e.g. remove HBP, change the HBP sensor to match what you have,
change x,y, and z limits).  Update the "serial:" line under [mcu]
to match the filename found in the previous step.  Add the following
two lines at the top of you printer.cfg file to enable Mainsail
support.
[include mainsail.cfg]
Click save and restart.

Following the normal installation steps, run make menuconfig.  Choose 
an atmega1280, 16mhz, and uart0.  (see below for note about atmega2560).

Run make flash.  This should flash your mightyboard.  If not, I have
found times where I needed to power the mightboard off and back on
or attempt to connect and disconnect with Klipper (i.e. connect at a
speed other than 57600 first) before it would flash.

At this point you should have Klipper running on your Replicator.
Follow the normal Klipper documentation for further tuning.

# Notes
* The Sailfish firmware has profiles for mightyboard rev g and rev
h printers that have atmega2560 instead of atmega1280.  If you have
one of these please let me know so I can get a known working config.
I believe you should be able to get this working by changing the MCU
processor in make menuconfig and editing src/arv/Makefile and changing
the last line from "-C stk500v1" to "-C stk500v2".  Software reset
should work but needs to be tested to confirm.
* The generic-mightyboard.cfg in the main repo should NOT be use
for these printers.  That config file is for the original
Makerbot Replicator and clones and does not work with printers
that have ADS1118 adc and thermocouples.  Make sure to use the
config file specific for the rep2x as a basis for these printers.
* There is no specific error for a disconnected thermocouple however
the printer will shut down if a thermocouple is not attached (this
triggers a temperature out of range error).
* The included
printer.cfg changes the origin (0,0) to be in the left front of the build
plate to be consistent with other cartesian printers.  Keeping the original
Replicator origin (which resembles a delta printer) requires updating
position_min, position_max, position_endstop, and bed_screws values for the
x and y axix
* Please watch or star this repo if you are interested.  The more
people that use this the better the chances of getting it included
upstream.  Feel free to file issues in this repo for questions or
problems (DON'T USE ISSUES IN UPSTREAM FOR THIS)

*************************************************************************
# Welcome to the Klipper project!

[![Klipper](docs/img/klipper-logo-small.png)](https://www.klipper3d.org/)

https://www.klipper3d.org/

Klipper is a 3d-Printer firmware. It combines the power of a general
purpose computer with one or more micro-controllers. See the
[features document](https://www.klipper3d.org/Features.html) for more
information on why you should use Klipper.

To begin using Klipper start by
[installing](https://www.klipper3d.org/Installation.html) it.

Klipper is Free Software. See the [license](COPYING) or read the
[documentation](https://www.klipper3d.org/Overview.html). We depend on
the generous support from our
[sponsors](https://www.klipper3d.org/Sponsors.html).
