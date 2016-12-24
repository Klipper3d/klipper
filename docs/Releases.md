History of Klipper releases. Please see
[installation](Installation.md) for information on installing Klipper.

Klipper 0.3.0
=============

Available on 20161223. Major changes in this release:

* Improved documentation
* Support for robots with delta kinematics
* Support for Arduino Due micro-controller (ARM cortex-M3)
* Support for USB based AVR micro-controllers
* Support for "pressure advance" algorithm - it reduces ooze during
  prints.
* New "stepper phased based endstop" feature - enables higher
  precision on endstop homing.
* Support for "extended g-code" commands such as "help", "restart",
  and "status".
* Support for reloading the Klipper config and restarting the host
  software by issuing a "restart" command from the terminal.
* Stepper performance improvements (20Mhz AVRs up to 158K steps per
  second).
* Improved error reporting. Most errors now shown via the terminal
  along with help on how to resolve.
* Several bug fixes and code cleanups

Klipper 0.2.0
=============

Initial release of Klipper. Available on 20160525. Major features
available in the initial release include:

* Basic support for cartesian printers (steppers, extruder, heated
  bed, cooling fan).
* Support for common g-code commands. Support for interfacing with
  OctoPrint.
* Acceleration and lookahead handling
* Support for AVR micro-controllers via standard serial ports
