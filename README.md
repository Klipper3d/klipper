Fork of Klipper3d/klipper

Adds support for Makerbot Replicator2/2X (Mightyboard rev h and g). 
Currently supports all features of both printers except SD cards 
and dual extruders (single extruder is supported on a 2X).

Please watch or star this repo if you are interested.  The more
people that use this the better the chances of getting it included
upstream.  Feel free to file issues in this repo for questions or
problems (DON'T USE ISSUES IN UPSTREAM FOR THIS)

Changes include:
* Documents what is necessary to flash the MCU (57600 baud and correct 
    avrdude protocol)
* Adds support for MCU reset (toggle connection baud rate to 57600)
* Adds support for ADS1118 and k-type thermocouples
* Adds support for hardware blinking of the LEDs (not committed yet)
* Adds support for multiple buttons with same action in display
* Adds example g code macros to emulate some original Makerbot behaviors

/config/printer-makerbot-replicator2-2012.cfg can be used as a starting point 
for both 2 and 2x printers.  The included printer.cfg changes the origin
(0,0) to be in the left front of the build plate to be consistent with other 
cartesian printers.  Keeping the original Replicator origin (which resembles
a delta printer) requires updating position_min, position_max,
position_endstop, and bed_screws values for the x and y axix

TODOs:
* look at capabilities of the on board sd card
* add support for dual extruders
* add documentation for configuring slicers (rep2 specifics)

Installation

Clone this repo and do the normal installation steps.

Copy /config/printer-makerbot-replicator2-2012.cfg to printer.cfg.  Edit this
file to add/remove features specific to your printer (e.g. remove HBP,
change the HBP sensor to match what you have, change x,y, and z limits).

Following the normal installation steps, run make menuconfig.  Choose 
an atmega1280, 16mhz, and uart0.  (see below for note about atmega2560).

Run make flash.  This should flash your mightyboard.  If not, I have
found times where I needed to power the mightboard off and back on
before it would flash.

At this point you should have Klipper running on your Replicator.
Follow the normal Klipper documentation for further tuning.

NOTE:  The Sailfish firmware has profiles for mightyboard rev g and rev
h printers that have atmega2560 instead of atmega1280.  If you have
one of these please let me know so I can get a known working config.
I believe you should be able to get this working by changing the MCU
processor in make menuconfig and editing src/arv/Makefile and changing
the last line from "-C stk500v1" to "-C stk500v2".  Software reset
should work but needs to be tested to confirm.

*************************************************************************
Welcome to the Klipper project!

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
