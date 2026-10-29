# Switch-based BMCU integration and private recovery

This guide covers the optional Ender 3 V3 SE integration for BMCU-Klipper 1.0.6
with firmware 1.0.1. Four feeder channels share one physical extruder through an
entry switch above its gears. A purge chute/wiper is used during tool changes.
The external package owns transport, route state, print maps and services.

## Installation scope

The provided sequence was physically tested on one GD32F303RET6 Ender 3 V3 SE,
with a ceramic hotend, an Orange Pi Zero3 and PETG. A two-color print succeeded.
These observations do not validate other hardware, materials or four-color
printing. Example distances and coordinates are starting points to verify,
not universal calibration values.

Install the official BMCU package and services before this integration. Keep a
single `[bmcu]` section and one physical extruder. The installer currently
supports one feeder named `bmcu0` and the endpoint `extruder`; it does not bind
the installation to a particular device UID.

## Files

| Path | Purpose |
| --- | --- |
| `integrations/bmcu/bmcu_integracao_ender.py` | Capture, unloading, wiping and Orca tool adaptation |
| `integrations/bmcu/bmcu_medicao.py` | Arrival measurement and assisted calibration commands |
| `config/ender3-v3-se-validada/bmcu-multicor-ender.cfg` | Example sequence and print macros |
| `config/ender3-v3-se-validada/bmcu-switch.cfg` | Orange Pi entry-switch example |
| `profiles/orca/ender3-v3-se-bmcu/` | Reusable slicer snippets |
| `scripts/ender3-v3-se/install_multicolor.py` | Backup, compatibility checks, installation and rollback |

The Portuguese module paths, configuration keys and command names are legacy
interfaces retained for existing installations. Implementations and maintained
documentation use English. The private `_capturar_extrusora_primeiro` hook and
its `sensor_gerenciado` keyword remain compatible with the previously installed
measurement module. Test fixtures for that older module remain verbatim.

## Wiring and configuration

Connect host TX to feeder RX, host RX to feeder TX and share DC ground. Identify
the actual UART path and ensure that no serial console owns it. The Orange Pi
example uses `/dev/ttyS1`; another image may expose a different path.

For the normally-open entry switch, connect COM to GND and NO to the verified
GPIO input. On the documented Orange Pi Zero3 arrangement, PC15 is physical
pin 16 and GND is pin 6. `gpiochip1/gpio79` is image-dependent: confirm it using
`gpiodetect` and `gpioinfo`. Reuse an existing `[mcu host]`; do not define it twice.
Test both physical states with `QUERY_FILAMENT_SENSOR SENSOR=bmcu_entry`.

`klipper-mcu-switch.conf` removes real-time scheduling (`-r`) from the Linux MCU
service for an image that rejected it. Apply it only when that error is present.
The host MCU endpoint is `/tmp/klipper_host_mcu`.

## Commissioning example

| Phase | Example used on the tested assembly |
| --- | --- |
| Arrival | Feed to entry sensor; maximum route 1500 mm |
| Capture | 20 mm at F180; extruder starts before feeder assistance |
| Feed to nozzle | Another 80 mm at F120 |
| Purge | 70 mm at F120 |
| Unload | Extruder at F600 until the entry sensor clears |
| Parking | Another 55 mm through BMCU, verified by encoder |
| Purge/wipe | X=-6 / X=-4, Y=123; 2 mm relative Z lift |
| PETG | 230 °C nozzle; 80 °C first-layer bed, then 75 °C |

Check travel and clearances before using purge coordinates. Measure capture,
nozzle feed, purge and parking on the assembled path. Do not replace the switch
transition with a fixed 60 or 66 mm retraction. The entry sensor determines when
the extruder stops; the encoder then verifies parking. Native channel retraction
settings are restored after the operation.

Do not assume that an entry-sensor transition proves that filament is already
between the gears. The extruder-first capture stage bridges that gap. Keep the
hotend at the appropriate material temperature for capture and unloading.

## Install or update an existing integration

Back up first. With idle printer, parked channels and clear entry sensor, run
from this checkout:

```sh
sudo python3 scripts/ender3-v3-se/install_multicolor.py
```

The installer verifies the installed 1.0.6 structure, patches the generic
contract and pre-capture phases, installs optional modules, preserves
`SAVE_CONFIG` and existing `bmcu-multicor-ender.cfg` settings, and restarts the
Klipper process. Files are restored if Klipper does not become ready. Backups
and a rollback utility are written locally. Git updates alone do not replace
optional modules installed outside the tracked checkout.

For initial endpoint setup, review the example macros before running
`BMCU_CONFIGURAR_MULTICOR`. Setup does not overwrite channel material/color
assignments. Do not run endpoint setup at every boot or print start.

## Orca selection

Keep `T{next_extruder} B{next_extruder+1}` in the tool-change field. The explicit
`B` parameter selects the integration at any time. Once `BMCU_PRINT_COMMIT`
activates a print map, automatic `T0` through `T3` select logical tools 1 through
4 through that map, including a remapped physical channel. Missing logical
tools fail before selecting an unrelated source. Re-selecting a loaded tool
does not request an additional purge.

Without an active print map, commands without `B` retain native behavior. Native
`T0` selects external feeding; use `BMCU_TOOL_CHANGE TOOL=0` to request that
explicitly while a map is active. No slicer post-processing script is needed.

## Private backup and recovery

Personal snapshots and slicer exports are intentionally excluded from public
releases. Keep calibrated configuration, state, service environments and tokens
in private storage. Removing files from the current tree does not erase their
historical Git versions. Rotate any exposed credentials.

Create a private archive with an idle printer:

```sh
sudo python3 scripts/ender3-v3-se/capture_configuration.py
```

The collector records configuration, calibration, BMCU state, installed modules,
source revision, local patches, available firmware artifacts and service
metadata. It does not upload files, move motors or restart services. It rejects
files that change during collection. Resolve reported missing-file warnings
before relying on the archive for recovery.

Verify and extract into a new review directory:

```sh
python3 scripts/ender3-v3-se/verify_backup.py backup.tar.gz --extract recovery
```

The archive format retains its original field names and CLI aliases for
compatibility. Restore system packages, source revisions and services before
copying reviewed configuration into an existing installation. Do not restore
saved runtime PID records as proof of a running process or saved `LOADED` state
as proof that filament remains in the nozzle.

The advanced `restore_snapshot.py` helper requires an explicitly supplied
private `--snapshot` directory in its checksum-manifest format. No personal
snapshot is bundled or selected by default. It prepares a plan first and only
applies it with `--apply`; it is not an SD-image installer. Source archives
and MCU status versions do not independently prove what binary is flashed.

## Validation

```sh
python3 -m unittest discover -s test/bmcu_integracao -p 'test_*.py'
python3 -m unittest discover -s test/e3v3se_display -p 'test_*.py'
```

Tests cover route selection, capture/unload contracts, failure recovery,
configuration parsing, backup integrity and restoration using synthetic data.
Complete hardware commissioning remains necessary after a new installation.
