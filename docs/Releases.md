History of Klipper releases. Please see
[installation](Installation.md) for information on installing Klipper.

Klipper 0.4.0
=============

Available on 20170503. Major changes in this release:

* Improved installation on Raspberry Pi machines. Most of the install
  is now scripted.
* Support for corexy kinematics
* Documentation updates: New Kinematics document, new Pressure Advance
  tuning guide, new example config files, and more
* Stepper performance improvements (20Mhz AVRs over 175K steps per
  second, Arduino Due over 460K)
* Support for automatic micro-controller resets. Support for resets
  via toggling USB power on Raspberry Pi.
* The pressure advance algorithm now works with look-ahead to reduce
  pressure changes during cornering.
* Support for limiting the top speed of short zigzag moves
* Support for AD595 sensors
* Several bug fixes and code cleanups

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
