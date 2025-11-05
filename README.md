# [ℹ️ New version available for testing](https://github.com/Desuuuu/klipper/discussions/74)

Welcome to the Klipper project!

[![Klipper](docs/img/klipper-logo-small.png)](https://www.klipper3d.org/)

https://www.klipper3d.org/

The Klipper firmware controls 3d-Printers. It combines the power of a
general purpose computer with one or more micro-controllers. See the
[features document](https://www.klipper3d.org/Features.html) for more
information on why you should use the Klipper software.

Start by [installing Klipper software](https://www.klipper3d.org/Installation.html).

Klipper software is Free Software. See the [license](COPYING) or read
the [documentation](https://www.klipper3d.org/Overview.html). We
depend on the generous support from our
[sponsors](https://www.klipper3d.org/Sponsors.html).

## Modifications

The scope of modifications is limited to adding support for DWIN T5UID1
touchscreens (except for the addition of a `--warn` CLI option, which sets the
logging level to WARNING).

The touchscreen feature is only available for AVR/LPC176X/STM32/SAMD
micro-controllers and it needs to be configured before compilation.

The touchscreen firmware compatible with this fork is available in
[this repository](https://github.com/Desuuuu/DGUS-reloaded-Klipper).

Example configurations are available in
[this repository](https://github.com/Desuuuu/DGUS-reloaded-Klipper-config).

Available configuration options are documented in the
[sample-t5uid1.cfg](/config/sample-t5uid1.cfg) file.
