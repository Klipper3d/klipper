# Creality Ender 3 V3 SE (GD32F303RET6)

This profile is specifically for an Ender 3 V3 SE with:

- Mainboard marked `CR4NS200320C14`
- A physically verified `GD32F303RET6` MCU
- The stock 3.2 inch TJC3224/DWIN display
- USB serial between the printer and a Linux host

The board revision alone does not identify the MCU. Do not use an STM32F401
build on this target merely because another C14 board contains that processor.

## Safety status

This repository can validate software behavior and compile the firmware, but it
cannot physically validate motion, heater output control, probe behavior, or the
bootloader on a particular printer. Keep one hand on emergency power during the
first checks. Do not start a print until homing, heater verification, CR-Touch,
Z offset, and travel limits have been checked at low speed.

The load cell based PRTouch auto Z-offset code is intentionally not included.
The historical implementation is incompatible with current probe APIs and has
documented cases of incorrect offsets and nozzle-to-bed collisions. This profile
uses the stock CR-Touch through Klipper's upstream `bltouch` implementation.

## Host installation

Download the source and firmware files from the same
[GitHub release](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases).
The release tag is the authoritative source version for its `klipper.bin` and
`klipper.dict`; do not run a host checkout from another commit with that binary.

On MainsailOS for the Orange Pi Zero 3, install Klippy from this repository and
checkout the same commit that will be used to build the MCU firmware. Keep the
standard Moonraker and Mainsail installations; neither requires a fork.

Copy the example configuration into the Mainsail configuration directory as a
starting point:

```sh
cp config/printer-creality-ender3-v3-se-gd32f303-2023.cfg \
  ~/printer_data/config/printer.cfg
cp -r config/ender3-v3-se ~/printer_data/config/
```

Confirm the `[mcu] serial` path with:

```sh
ls /dev/serial/by-id/
```

The reference configuration requires `[virtual_sdcard]`, `[pause_resume]`, and
`[display_status]`. These sections are already included by
`config/ender3-v3-se/display.cfg`.

## Deterministic firmware build

The proven compatibility target for this GD32F303 board is Klipper's STM32F103
target. It deliberately uses the conservative upstream F103 flash and RAM map;
this project does not claim that the GD32F303 is natively supported by Klipper.

The exact `make menuconfig` selections are:

- Enable extra low-level configuration options: yes
- Micro-controller architecture: STMicroelectronics STM32
- Processor model: STM32F103
- Only 10KiB of RAM: no
- Disable SWD at startup: no
- Bootloader offset: 28KiB bootloader
- Clock reference: 8 MHz crystal
- Communication interface: Serial on USART1 PA10/PA9
- Baud rate: 250000
- Creality Ender 3 V3 SE stock display bridge: yes

The checked-in build configuration is easier and less error-prone:

```sh
make clean
make distclean
cp test/configs/e3v3se-gd32f303.config .config
make olddefconfig
make
```

Use a checkout path without spaces. Klipper's upstream Makefile expands its
working directory in shell commands that do not support whitespace in the path.

Expected outputs:

- `out/klipper.bin`: firmware copied to the printer SD card
- `out/klipper.dict`: MCU protocol dictionary, including display commands
- `out/defconfig`: normalized build configuration

The firmware embeds Klipper's Git version and normalized Kconfig. Klippy logs
both values during connection. Release downloads also include the dictionary,
normalized configuration, checksums, commit SHA, upstream base, hardware
target, MCU target, and UTC build date.

## Flashing

1. Use a small microSD card formatted as FAT32 with a 4096-byte allocation unit.
2. Copy `out/klipper.bin` to the card.
3. Rename it if that filename was used for the previous flash. Use a short 8.3
   filename ending in `.bin`, for example `kli0916.bin`.
4. Power the printer off, insert the card, and power it on.
5. Wait before removing power or the card.
6. Verify the MCU version in `klippy.log` before enabling the display section.

If the host reports missing `e3v3se_display_send` or a protocol mismatch, do
not keep restarting it. Rebuild and reflash the MCU from the same commit as the
host. A firmware built without the display option is intentionally rejected by
the host module with a configuration error.

## Commissioning order

1. Verify that all heaters remain off and both sensors show plausible ambient
   temperatures after connection.
2. Heat only the hotend to 50 C. Confirm only its matching sensor rises, turn it
   off, and verify cooldown. Cut power immediately on unexpected behavior.
3. Repeat with only the bed at 40 C, then turn it off and verify cooldown.
4. Check X and Y endstop state with `QUERY_ENDSTOPS` by actuating them manually.
5. Verify CR-Touch deploy/stow and triggered state without homing Z.
6. Home X and Y at low speed, ready to cut power.
7. Home Z over the bed with the nozzle safely above it.
8. Verify travel directions and configured limits.
9. Run `PID_CALIBRATE` separately for the hotend and bed, then `SAVE_CONFIG`.
10. Run `PROBE_CALIBRATE`, use manual `TESTZ`, then `ACCEPT` and `SAVE_CONFIG`.
11. Run `BED_MESH_CALIBRATE`, inspect the mesh, then save it if appropriate.
12. Calibrate rotation distance, pressure advance, and input shaping on the
    physical machine. No values for those calibrations are shipped here.

The initial heater control is `watermark` specifically to avoid shipping PID
values measured on someone else's printer. The initial BLTouch `z_offset: 0.0`
is a safe placeholder that must be calibrated before printing. These three
autosave-managed placeholders intentionally remain in the top-level printer
configuration; moving them into an included file prevents current Klipper from
saving PID and probe calibration results.

## Updating

Keep `origin` pointed at this fork and `upstream` pointed at
`https://github.com/Klipper3d/klipper.git`. For every update:

1. Integrate the desired upstream commit into this repository.
2. Run the display unit tests and Klipper test suite.
3. Build the GD32F303 target.
4. Update Klippy on the Orange Pi to that exact commit.
5. Flash the MCU artifact produced from that exact commit.
6. Keep the machine configuration under separate version control or backup.

Do not update only Klippy or only the MCU when a display protocol command has
changed.

## Physical acceptance tests

The following tests require the printer and are not performed by CI:

- 50 consecutive `RESTART` cycles
- 50 consecutive `FIRMWARE_RESTART` cycles
- Repeated navigation through every menu
- Empty, single-file, multi-file, nested, long-name, UTF-8, and special-name
  virtual SD directories
- Negative X, Y, and Z positions
- Pause, resume, and cancel from both Mainsail and the stock display
- Temperature, speed factor, flow factor, position, Z offset, print start, and
  print completion synchronization between Mainsail and the display
- Long print while monitoring MCU `bytes_retransmit`, timeouts, and bridge
  dropped-frame status

Software stress tests cover frame state, byte limits, UTF-8 boundaries, signed
values, and file navigation. They do not replace physical validation.

## BMCU and display languages

See [BMCU 1.0.6 integration](Ender3_V3_SE_BMCU.md) for UART1, the PC15 switch,
filament sequences, language selection and physical display acceptance tests.
