# GD32 platform layout

This directory contains Klipper runtime implementations for GD32 MCUs.

- Common peripheral backends stay directly in `src/gd32`: ADC, serial, USB,
  CAN, SPI and I2C.
- Family-specific startup, GPIO and timer code uses an explicit family prefix:
  `gd32f30x_*` or `gd32e23x_*`.
- Vendor device headers live under `lib/gd32f30x/include` and
  `lib/gd32e23x/include`.  No STM32 device header is used to describe a GD32.
- `Kconfig` owns chip capacity, clock, boot offset and communication choices;
  source files must not infer a board model from a similar STM32 part.

The split is intentionally flat, like Klipper's `src/stm32` platform: one GD32
board directory with family differences selected by Kconfig and Makefile.
