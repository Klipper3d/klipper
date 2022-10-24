Fork of Klipper3d/klipper

Adds support for Makerbot Replicator2/2X (Mightyboard rev h and g). 
Currently supports all features of both printers except SD cards 
and dual extruders (single extruder is supported on a 2X).

Changes include:
* Documents what is necessary to flash the MCU (57600 baud and correct 
    avrdude protocol)
* Adds support for MCU reset (toggle connection baud rate to 57600)
* Adds support for ADS1118 and k-type thermocouples
* Adds support for hardware blinking of the LEDs

/config/printer-makerbot-replicator2.cfg can be used as a starting point 
for both 2 and 2x printers.  The included printer.cfg changes the origin
(0,0) to be in the center of the build plate to be consistent with other 
corexy printers.  Keeping the original Replicator origin requires 
updating position_min, position_max, position_endstop, and bed_screws 
values for the x and y axix

TODOs:
* fix buttons (right arrow does nothing)
* look at capabilities of the on board sd card
* add support for dual extruders
* fix configuration for heated bed sensor

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
