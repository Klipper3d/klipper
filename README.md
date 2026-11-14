# Klipper for Ender 3 V3 SE + BMCU

**English** | [Português brasileiro](README.pt-BR.md)

[![Ender CI](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/actions/workflows/ender3-v3-se.yml/badge.svg)](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/actions/workflows/ender3-v3-se.yml)
[![Release](https://img.shields.io/github/v/release/bernardokcosta/ender3-v3-se-klipper-bmcu)](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases)
[![GPLv3](https://img.shields.io/badge/license-GPLv3-blue.svg)](COPYING)

An unofficial Klipper fork for the **Creality Ender 3 V3 SE**, combining stock
display support and optional four-channel BMCU filament transport. One physical
extruder serves all four channels. BMCU-Klipper remains a separate dependency.

<a id="version-200"></a>
## Features

- Stock TJC3224/DWIN 3.2-inch display, with English and Brazilian Portuguese UI.
- BMCU 1.0.6 / firmware 1.0.1 integration using a generic single-extruder endpoint.
- Switch-based arrival, extruder-first capture, purge, wiping and assisted unload.
- OrcaSlicer 2.4.2 automatic `T0` handling through the committed print map.
- Upstream Klipper merged through `0cee841e31d701595b3e509a0b502f6e320f52a8`.
- Regression tests, a reproducible target build and matching release checksums.
- English-only integration commands, parameters and status fields.
- Organized bilingual display menus and complete EN/PT-BR public documentation.

See [release notes](docs/releases/v2.0.0.md) for changes and compatibility, and
[translation provenance and policy](docs/Translations.md) for language coverage.
Fork, BMCU package and BMCU firmware versions are independent. Calibration and
physical commissioning remain necessary.

## Hardware scope

The supported MCU target is **GD32F303RET6**, on the verified CR4NS200320C14
mainboard, using Klipper's STM32F103-compatible target, 28 KiB bootloader and
USART1. Check the chip itself; the board marking alone is insufficient.
**Do not flash this binary onto an STM32F401 board.**

CR-Touch uses upstream Klipper support. Historical PRTouch/HX711 automatic
Z-offset firmware is not included. BMCU and the purge mechanism are optional;
installation-specific travel, probe offsets, GPIO mappings and filament
sequences must be measured on the actual printer.

<a id="install-and-upgrade"></a>
## Installation

1. Read the [hardware and flashing guide](docs/Ender3_V3_SE.md).
2. Back up your configuration and calibration before changing an existing setup.
3. Use the host source and MCU firmware from the same release tag. Download
   `klipper.bin`, `klipper.dict`, `defconfig`, `BUILD-METADATA.txt` and
   `SHA256SUMS` from [Releases](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases).
4. For a new setup, use
   [`config/printer-creality-ender3-v3-se-gd32f303-2023.cfg`](config/printer-creality-ender3-v3-se-gd32f303-2023.cfg)
   with the modular `config/ender3-v3-se/` directory. Do not overwrite a calibrated
   `printer.cfg` with this template.
5. For BMCU, install [BMCU-Klipper 1.0.6](https://github.com/jarczakpawel/BMCU-Klipper/tree/v1.0.6)
   and its services first, then follow the [integration guide](docs/Ender3_V3_SE_Configuracao_Validada.md).

For existing installations, follow the
[release upgrade instructions](docs/releases/v2.0.0.md#upgrade-and-rollback).

## Operation

The display and Mainsail share the same operational backend. See
[everyday filament controls](docs/Ender3_V3_SE_Configuracao_Validada.md#everyday-filament-controls)
for channel loading, unloading, preheat and guarded reset, and
[print recovery](docs/Ender3_V3_SE_Print_Recovery.md) for a failed filament change.

Select **Settings → Language → English / Português** on the screen to apply
the language immediately. **Settings → Save config** persists it and restarts
Klipper after confirmation when idle. Display language does not change the
English command API.

The [tip-forming profile and four-color reload test](docs/Ender3_V3_SE_Tip_Forming.md)
are experimental and require physical calibration. The optional
[filament cutter with an Orange Pi servo](docs/Ender3_V3_SE_Cutter.md) ships
disabled and requires measured cutting coordinates and verified GPIO wiring.

## OrcaSlicer

Use the [reusable start, end and tool-change snippets](profiles/orca/ender3-v3-se-bmcu/README.md).
Replace `YOUR_BMCU_UID` with the UID reported by your own BMCU before slicing.
With a committed BMCU print map, `T0` through `T3` select logical tools 1 through
4; the map resolves physical channels. Without an active map, native commands
retain their original meaning, including external feeding through `T0`.
No computer-side post-processing script is required.

## Printable hardware

| Part | Model and download |
| --- | --- |
| BMCU mount for Creality Ender 3 V3 SE | [MakerWorld](https://makerworld.com/pt/models/3516373-bmcu-mount-for-creality-ender-3-v3-se) |
| Four-to-one PTFE filament hub with entry sensor | [Printables](https://www.printables.com/model/1320106-hub-filament-have-sensor-for-pico-mmu-for-ender-3) |
| Minimal X endstop and purge chute/wiper | [Printables](https://www.printables.com/model/1600776-minimal-x-endstop-and-purge-chutewiper-for-ender-3/files) |
| Left-side purge waste bucket | [Printables](https://www.printables.com/model/899282-ender-3-v3-se-left-bin-bucket-but-for-people-with/files) |
| Filament spool holder for four spools | [MakerWorld](https://makerworld.com/pt/models/2671446-filament-spool-holder-for-4-spools#profileId-3406834) |
| Optional filament cutter for Ender 3 V3 SE/KE | [Printables](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke) · [Integration and calibration](docs/Ender3_V3_SE_Cutter.md) |

Download the model files from their respective pages. They are not duplicated
in this repository; each model retains its own license and creator attribution.

## Backups and privacy

Machine snapshots, service environments, runtime state, device IDs and personal
slicer exports are not public installation templates. Keep them in private
storage. The backup tools are local utilities and never upload their results.
Historical commits may still contain files removed from the current tree;
removing a file does not erase Git history. Rotate any credential that has been
published. Copyright and upstream attribution are intentionally preserved.

## Development and validation

See the [local development guide](docs/Development.md) for Linux/WSL setup and
the complete whitespace/build checks without connecting to a printer.

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
python3 -m unittest discover -s test/public_docs -p 'test_*.py'
python3 scripts/check_public_docs.py
```

CI builds the GD32F303 firmware and validates the complete Klippy configuration.
Backup and restoration tests use synthetic data. Hardware testing has confirmed
filament capture, switch-controlled unloading, purge/wipe and a two-color PETG
print on one assembly. Other board revisions, materials, purge geometries and
four-color printing require their own physical acceptance tests.

## Documentation

- [Installation, flashing and commissioning](docs/Ender3_V3_SE.md)
- [Switch-based BMCU integration and calibration](docs/Ender3_V3_SE_Configuracao_Validada.md)
- [BMCU setup, wiring and calibration concepts](docs/Ender3_V3_SE_BMCU.md)
- [Optional cutter, Orange Pi servo and manual reset](docs/Ender3_V3_SE_Cutter.md)
- [OrcaSlicer configuration](profiles/orca/ender3-v3-se-bmcu/README.md)
- [Display architecture](docs/E3V3SE_Display.md)
- [Port analysis and provenance](docs/E3V3SE_Port_Analysis.md)
- [Official Klipper documentation](https://www.klipper3d.org/)

## Credits and provenance

This repository preserves Klipper's Git history and is distributed under the
same GNU GPLv3 license. The Ender 3 V3 SE work was made possible by:

- [Klipper](https://github.com/Klipper3d/klipper), created by Kevin O'Connor and
  maintained by the Klipper contributors, for the firmware, host architecture,
  build system, documentation, and GPLv3 codebase.
- [Joao Pedro Curti](https://github.com/jpcurti) and contributors to
  [ender3-v3-se-klipper-with-display](https://github.com/jpcurti/ender3-v3-se-klipper-with-display)
  for the original Ender 3 V3 SE display integration, behavior, protocol
  research, and issue history used as the basis for this rewritten port.
- [odwdinc/DWIN_T5UIC1_LCD](https://github.com/odwdinc/DWIN_T5UIC1_LCD) for the
  earlier DWIN T5L display work on which the TJC3224 implementation was based.
- [E4ST2W3ST](https://github.com/E4ST2W3ST) for the
  [Klipper serial bridge proposal](https://github.com/Klipper3d/klipper/commit/6469418d73be6743a7130b50fdb5a57d311435ca).
- [Clark Scheff / 0xD34D](https://github.com/0xD34D) for the
  [machine fork](https://github.com/0xD34D/klipper_ender3_v3_se) and
  [configuration reference](https://github.com/0xD34D/ender3-v3-se-klipper-config)
  used to study the hardware, community configuration, and historical PRTouch
  implementation.
- [huhen/Ender3_V3_SE_for_klipper](https://github.com/huhen/Ender3_V3_SE_for_klipper),
  distributed under GPLv3, for the documented CR4NS200320C13 pinout and
  configuration used as the licensed electrical pin-map basis for this port.
- [bootuz-dinamon](https://github.com/bootuz-dinamon/ender3-v3-se-full-klipper)
  and [FinalX1992](https://github.com/FinalX1992) for additional community
  hardware findings used to cross-check TMC UART and screw configuration.
- Everyone who reported, diagnosed, and fixed issues in those projects.
- [Pawel Jarczak / jarczakpawel](https://github.com/jarczakpawel) and
  [BMCU-Klipper](https://github.com/jarczakpawel/BMCU-Klipper) contributors for
  the external filament transport package and Generic endpoint contract.

The detailed revision and commit provenance is recorded in
[`docs/E3V3SE_Port_Analysis.md`](docs/E3V3SE_Port_Analysis.md).

## License

Klipper and the software and configuration distributed by this modified fork
are Free Software under the [GNU General Public License, version 3](COPYING).
Source files retain their applicable copyright and license notices. The
complete corresponding source for each binary release is the repository state
at that release tag.

No Creality display firmware, image pack, or other proprietary binary asset is
redistributed here. Creality, Ender, DWIN, and TJC names may be trademarks of
their respective owners. This project is not affiliated with or endorsed by
Creality, DWIN, TJC, or the upstream Klipper project.
