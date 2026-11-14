# Switch-based BMCU integration and private recovery

**English** | [Português brasileiro](pt-BR/Ender3_V3_SE_Configuracao_Validada.md)

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
| `integrations/bmcu/bmcu_ender_integration.py` | Capture, unloading, wiping and Orca tool adaptation |
| `integrations/bmcu/bmcu_measurement.py` | Arrival measurement and assisted calibration commands |
| `config/ender3-v3-se-validada/bmcu-multicor-ender.cfg` | Example sequence and print macros |
| `config/ender3-v3-se-validada/bmcu-switch.cfg` | Orange Pi entry-switch example |
| `profiles/orca/ender3-v3-se-bmcu/` | Reusable slicer snippets |
| `scripts/ender3-v3-se/install_multicolor.py` | Backup, compatibility checks, installation and rollback |

Commands, parameters, enum values, configuration options and public status
fields use the English v2 API in both display languages. The installer migrates
known v1 configuration names and persisted endpoint callbacks; it rejects
ambiguous old/new name collisions without activating files. There are no public
Portuguese aliases. **Re-slice existing print jobs with the v2 Orca profile**;
stored G-code is not automatically converted. Private motion helpers and legacy
fixtures retain their tested implementation independently of the public names.

Custom Jinja expressions that combine integration status with legacy diagnostic
enum literals are rejected for manual review before activation. Update those
conditions to the English public status values; the installer does not blindly
rewrite private labels or assume that changing a field name preserves a
condition's meaning. Dotted integration-status lookups and copied, derived or
rebound integration-status aliases also require manual review instead of being
activated with uncertain semantics. Use direct bracket lookups and review
custom expressions against the English status schema. Arbitrary custom
templates still require a separate review.

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
| Purge | 35 mm at F120; reduced from the earlier 70 mm test |
| Unload | Extruder at F300 until the entry sensor clears; buffer and encoder checks |
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

Back up first. Keep the printer idle, all channels parked and the entry sensor
clear. For a complete host/display rollback boundary, run the new installer
from a separate clean checkout of the published v2 release. Do not update the
active `~/klipper` checkout with `git pull` beforehand. After v2.0.0 is published:

```sh
git clone --branch v2.0.0 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source"
```

The installer verifies the installed 1.0.6 structure, patches the generic
contract and pre-capture phases, installs optional modules and preserves
`SAVE_CONFIG` and existing integration settings. It serializes installation
and rollback with one lock, stops the relevant services, then re-reads state
before taking the authoritative backup. The previous complete host checkout,
including display code, Git history and untracked user files, is preserved.
Installed optional modules follow the new checkout; other untracked files
remain in the preserved tree and are listed in its private manifest.

This path accepts host-only upgrades: MCU/build inputs and Python requirements
must match. A firmware or environment change is refused for separate review;
the installer never flashes MCUs or installs Python dependencies. Dirty tracked
host/target files, unsupported Git layouts, addon conflicts or changing staged
files are rejected instead of discarding user work. Staging artifacts are kept
for diagnosis, not deleted automatically.

The changed transport daemon starts before Klipper. If startup fails, the
previous host/display and affected configuration are restored. Only known
callback renames are undone in the **current** BMCU state; stale routes and
calibrations are not copied over current evidence. The restored host must
become ready before recovery is reported as successful; otherwise services stay
stopped. The failed checkout remains available for inspection. Backups and a
standalone rollback utility are written privately under the installation home.

Running `install_multicolor.py` without `--host-source` still updates only the
integration. It cannot reconstruct a host checkout changed beforehand and
therefore leaves services stopped after a failed update. Git updates alone
do not replace installed optional modules.

The installer also updates the supported print-start heating sequence in
`bmcu-multicor-ender.cfg` and an existing `impressao.cfg`. After homing and
loading the mesh, the head reaches the configured purge chute before waiting
for the nozzle to rise from its standby target (up to 150 degrees C) to the
print temperature. The positioning command does not extrude or enable cold
extrusion. Custom startup layouts that cannot be migrated are rejected for
manual review; calibrated distances, coordinates and `SAVE_CONFIG` are kept.

For initial endpoint setup, review the example macros before running
`BMCU_SETUP_MULTICOLOR`. Setup does not overwrite channel material/color
assignments. Do not run endpoint setup at every boot or print start.

## Everyday filament controls

The stock display groups each operation in one place:

- **Filament / BMCU:** load one of four physical channels, unload the active
  channel, or confirm **Reset BMCU** after manual filament removal. An uncertain
  route disables loading but does not hide the guarded reset action.
- **Temperature:** PLA/PETG/TPU preheat, cooldown, nozzle/bed targets and fan.
- **Move:** homing, axis/extruder adjustments, motor release and the optional
  cutter stop, with its existing clearance confirmation.
- **Calibration:** CR-Touch Z offset, bed mesh and screw tilt where configured.
- **Settings:** select **English / Português** immediately and explicitly save
  configuration when idle to persist it. Saving confirms a Klipper restart.
- During printing, pause/tune/cancel remain available. A failed paused tool
  change exposes **Recover filament**, not a global BMCU reset. Its guided
  confirmation preserves the pending print map and uses the failed channel.

Without BMCU, the filament menu offers the ordinary single-extruder controls.
Preheat/cooldown and load/unload are no longer duplicated under Prepare. With
this integration installed, the display reuses the same backend as Mainsail:

| Mainsail button | Action |
| --- | --- |
| `PREHEAT_PLA` | Default nozzle/bed: 200/60 °C |
| `PREHEAT_PETG` | Default nozzle/bed: 230/80 °C |
| `PREHEAT_TPU` | Default nozzle/bed: 230/50 °C |
| `UNLOAD_ACTIVE_CHANNEL` | Unload the confirmed active BMCU channel |
| `LOAD_CHANNEL_1` through `LOAD_CHANNEL_4` | Load the physical channel |
| `RESET_BMCU CONFIRM=1` | Reconcile route state after manual removal, outside a print |

Change preheat values in `bmcu-controles-ajustes.cfg`; the installer preserves
this file on subsequent updates. The shared `bmcu-controles.cfg` definitions
are managed by the installer. Screen preheat actions use these macros when
available; without them, the display's `preheat_*` configuration applies.
Temperature examples must be checked against the actual filament.

Manual load/unload always sets the nozzle target to 230 °C and waits until it
is hot enough. It homes unreferenced axes and moves to the configured purge
position before feeding. Loading another channel unloads the existing channel,
verifies that the entry switch cleared and the route parked, then uses the
calibrated capture, nozzle-feed, purge and wipe sequence. Selecting the already
loaded channel does nothing. Unloading an empty route does not heat or move.
An absent target filament, uncertain route or sensor/route disagreement stops
the action before heating. Active prints, paused prints and open BMCU print
contexts must be finished before using these manual controls.

The 230 °C manual-change target is independent of the preheat profiles. The TPU
preset does not establish that a particular flexible filament can travel through
the BMCU; only load materials verified on the actual feeder and path.

Use the staged update flow above to install these controls. Existing calibrated
sequence values and the `SAVE_CONFIG` block are preserved. Language preferences
in an included display config are moved to the top-level `printer.cfg` so
Klipper's native `SAVE_CONFIG` does not conflict with an included option.
Only the exact default load/unload display shortcuts are removed because the
built-in filament menu already provides them; custom labels/scripts are kept.

## Macro purpose and caller audit

Short wrappers are not necessarily unused macros. The audit found no safely
removable G-code macro among these operational paths:

| Macro group | Required caller or purpose |
| --- | --- |
| `PAUSE`, `RESUME`, `CANCEL_PRINT` | Print lifecycle, guarded recovery and safe cancellation |
| `LOAD_FILAMENT`, `UNLOAD_FILAMENT` | Ordinary extruder controls and the template's generic callbacks |
| `PREHEAT_*`, `LOAD_CHANNEL_*`, `UNLOAD_ACTIVE_CHANNEL` | Mainsail buttons sharing the display backend |
| `BMCU_PREPARE_ENDER`, `BMCU_RELEASE_ENDER` | Native endpoint capture/pullback callbacks; not manual load buttons |
| `BMCU_SETUP_MULTICOLOR` | Explicit one-time endpoint commissioning, not startup automation |
| `ENDER_START_MULTICOLOR`, `ENDER_END_MULTICOLOR` | Orca start/end entry points |
| `ENDER_START_PRINT`, `ENDER_END_PRINT` | Single-color start and shared print-end behavior |
| `CUTTER_SERVO`, `TEST_CUTTER`, `RESET_BMCU` | Guarded manual calibration, mechanical test and recovery |
| `_...` settings/validation/body macros | Hidden implementation/calibration helpers, still called internally |
| `BMCU_SETUP_ENDER`, `BMCU_TOOLHEAD_PREPARE`, `BMCU_BEFORE_PULLBACK` | Older uncommissioned template only; do not run its setup over the validated endpoint |

The removed default display shortcut sections were **not** G-code macros.
Removing required callback or slicer macros would break an operation rather
than declutter its presentation. Keep custom shortcuts only for actions not
already offered by the built-in menus.

## Restoring the prepared update

Keep both private backup directories printed/created for the same update:

| Location under the installation home | Contents |
| --- | --- |
| `backup-multicor-<timestamp>/` | `restaurar.py`, its saved helper, configuration archive, per-file originals and `host.json` |
| `backup-host-update-<timestamp>/` | `host-record.json` and the preserved/staged checkout trees |
| `host-before/` inside the host backup | Entire previous checkout before rollback |
| `host-failed/` inside the host backup | Replaced v2 checkout retained during rollback |
| `host-staged/` inside the host backup | Prepared checkout before activation, or retained staging evidence |

The two timestamps need not match. Read `host.json` in the configuration backup
to find its exact `active`, `previous` and `failed` paths; do not guess which
directories belong together. Both manifests and archives may contain private
paths, configuration or state. Do not publish them.

For a manual rollback, finish/cancel the print, physically clear the shared
path and park the channels first. Preserve any configuration changes made since
the update in a separate private backup: the restorer restores the files from
that update, not later calibrations. Substitute the exact backup directory:

```sh
BACKUP="$HOME/backup-multicor-<timestamp>"
sudo python3 "$BACKUP/restaurar.py"
```

This command **applies** the rollback; it is not a preview. It shares the update
lock with the installer. When Klipper is running, it checks the idle/parked
preconditions first. It stops relevant services, restores the complete old
checkout and affected files, and reverses recognized callbacks in current state.
It starts transport before Klipper and validates the old integration section.
Any failed restore/validation must be investigated with services stopped; do not
force a startup or manually assert route state to bypass an error.

A power loss between activation renames may leave `~/klipper` absent while
`host-before/` and `host-staged/` remain. The generated restorer can put the old
checkout back. It also handles interruption after the new checkout was moved
to `host-failed/` but before the old tree returned to its active path. Existing
snapshots are not overwritten. If the recorded revision/path checks fail,
preserve every tree and manifest for manual diagnosis; do not use Git reset,
delete the evidence or rename an unrelated checkout over the active path.

Without `--host-source`, the backup lacks `host.json`: its restorer restores
integration files only and leaves services stopped. It cannot recover an old
display/host version that was never preserved.

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
