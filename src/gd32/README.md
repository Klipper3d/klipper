# GD32E23x platform layout

This directory contains the Klipper runtime implementation for the GD32E23x
Cortex-M23 family.

- Peripheral backends stay directly in `src/gd32`; startup, GPIO, and timer
  code uses the `gd32e23x_*` prefix.
- Vendor device headers live under `lib/gd32e23x/include`.
- `Kconfig` owns chip capacity, clock, boot offset and communication choices;
  source files do not infer a board model.

GD32F303 is register-compatible with STM32F103 and therefore uses Klipper's
existing `src/stm32` implementation with a small clock-tree specialization.
