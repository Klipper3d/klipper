# Greetings!

Welcome to a branch of the Klipper project.  Briefly, the feature set added is listed below.
Currently, the focus of this branch is the Raspberry Pi PICO RP2040 microprocessor.  Check out 
the [Wiki](https://github.com/kpishere/klipper/wiki) for representative boards that use this MCU. 

- ADC1X1X support - this family of i2c ADC chips can be used for thermocouple measurement, 
  current measurement, transducer measurement, etc.  Both heater and extruder elements 
  can use this device for temperature measurement
- XRA140X GPIO Expander Support - A gpio expander for, well, more gpio pins.  Interrupts are used
  to minimize traffic on the SPI bus.
- Klipper builds on the Mac with Homebrew pours installed for GCC cross-compiler, etc.

Dear Klipper Team :

Additions here-in follow the GNU V3 Licence and you're welcome to pull whatever you think is worthy.

=====

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
[documentation](https://www.klipper3d.org/Overview.html).
