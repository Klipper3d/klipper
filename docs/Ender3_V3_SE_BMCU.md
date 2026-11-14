# Ender 3 V3 SE: BMCU 1.0.6 and stock display

**English** | [Português brasileiro](pt-BR/Ender3_V3_SE_BMCU.md)

For switch-based integration, installation and private recovery tools, see
[Switch-based integration](Ender3_V3_SE_Configuracao_Validada.md).
The remainder of this page describes the original, uncommissioned v0.2.0 template.
Do not run `BMCU_SETUP_ENDER` over the validated endpoint `extruder`.

This integration uses **one physical extruder** (`extruder`) and four channels
routed to endpoint `main`, with driver `generic_single_extruder`.
Use the official [v1.0.6 installer](https://github.com/jarczakpawel/BMCU-Klipper/tree/v1.0.6)
for BMCU modules, `[bmcu]`, transport and services. Do not copy its modules into
this fork or add a second `[bmcu]` section. Matching firmware is **1.0.1**, per
the [version file](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/version).
The supplied file has 56,492 bytes and SHA-256:
`b85a21a14e61754db021302e0d54e4c6f9a309d0276d4fb5842b45e3e9a27327`.
This identifies the received file; verify the flashed version in BMCU status.

## Configuration and wiring

- `macros.cfg`: physical extruder sequences and calibration parameters.
- `bmcu.cfg`: Generic callbacks and manual channel setup.
- `bmcu-sensor.cfg`: Orange Pi switch configuration template.
- `display.cfg`: language, material profiles and shortcuts.

These files live in `config/ender3-v3-se/`.

Orange Pi Zero 3 UART1 uses TX **PG06** and RX **PG07**. Connect host TX to BMCU
RX and host RX to BMCU TX, with common GND. Enable UART1 in the installed Linux
image, identify its TTY, and check that no console or other process owns it.
Do not assume `/dev/ttyS1`. Give the verified port to the official installer.
The package format is `devices: bmcu0,<UART1 port>`, default baud 115200.
Preserve the installer's configuration, services and persistent state.
Pin multiplexing reference:
[Orange Pi Zero 3 DTS](https://github.com/orangepi-xunlong/orangepi-build/blob/next/external/packages/pack-uboot/sun50iw9/bin/dts/orangepizero3-u-boot-current.dts).

The switch is **after BMCU**: COM connects to physical pin **6/GND**, NO to
physical pin **16/PC15**. Set up the Linux MCU per
[RPi_microcontroller.md](RPi_microcontroller.md). On the host run `gpiodetect`
and `gpioinfo`, identify PC15's chip/offset and confirm the line is unused.
Uncomment `bmcu-sensor.cfg`, replacing `N` and `O` in
`^!host:gpiochipN/gpioO` with the actual indices. Reuse any existing
`[mcu host]`. Pull-up plus inversion detects NO closing to GND.
Verify both states with `QUERY_FILAMENT_SENSOR SENSOR=bmcu_entry`.

## Display language

English is the default. Select **Settings → Language → English / Português**
on the screen to change it immediately. This stages the preference through
Klipper's configuration API without restarting. **Settings → Save config**
persists it and restarts Klipper after confirmation, only outside a print or
busy operation. For a manual preference, put this in the top-level `printer.cfg`,
not in an include that would conflict with `SAVE_CONFIG`:

```ini
[e3v3se_display]
language: pt_BR
```

Use `language: en` for English. Manual file edits require a Klipper restart;
screen selection does not. Code, comments and macro descriptions use English;
public documentation is paired EN/PT-BR. Built-in screen
labels, states, operation messages and known errors are localized. Custom labels
and unknown external diagnostics retain their configured/original text.

The idle menu is **Print / Filament / Temperature / Move / Calibration /
Settings**, with **Macros** only for custom shortcuts. **Temperature** owns
preheat/cooldown; **Move** owns homing, jogging, motors and the optional cutter
stop. **Filament / BMCU** owns channel load/unload. The validated integration
also adds the guarded post-manual-removal **Reset BMCU** there. It invokes
`BMCU_ENDER_RESET CONFIRM=1`, is not a factory reset and does not erase
calibration. Active/paused prints and busy operations block it. A pending paused
tool change uses **Recover filament** and
`BMCU_CONFIRM_CLEAR_PATH CHANNEL=<1-4> CONFIRM=1`, never a global reset.

## Arrival strategy and the unmeasured distance

Measure switch-to-gears filament travel and record `sensor_to_gears_mm`;
`-1` means unknown. This is **not converted into extruder movement**:
the extruder cannot capture a tip that has not reached its gears.

In [BMCU 1.0.6](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/klippy/extras/bmcu_core/manager.py),
`ENTRY_SENSOR` detection stops BMCU feed immediately. `FINAL_SEARCH_MM` limits
search, not additional feed after detection. `BMCU_ROUTE_FEED` requires a
standalone operation, cannot run in the load callback, and leaves the route
uncertain. Do not nest it inside `BMCU_TOOLHEAD_PREPARE`.

Choose and physically validate `_ENDER_BMCU_SETTINGS.arrival_mode`:

- `"contact"`: BMCU continues feeding to contact at the gears, using its own
  transport/buffer logic. A distant PC15 switch is diagnostic, not
  `ENTRY_SENSOR`. Verify contact occurs at the gears without bending filament
  or meeting an intermediate PTFE obstruction.
- `"sensor"`: only if triggering the switch guarantees the tip is already
  capturable by the extruder. Reposition the switch if needed.

Set `max_route_mm` to a measured total-route limit and tune `contact_timeout`.
Run `BMCU_SETUP_ENDER` **manually**, with idle printer and empty routes. It
configures `main`, assigns four channels and clears incompatible endpoint
sensors. It does not run at startup or erase package state.
Set `arrival_validated: 1` after physical verification. The initial mode is
`"unconfigured"`; the display cannot start transport before validation.
Sensor mode requires a configured and verified PC15 sensor object.

## Complete load and cutter-free unload

Edit `_ENDER_FILAMENT_SETTINGS`. Distances start at zero and `calibrated: 0`
blocks macros before heating/motion. Unit-test distances are synthetic, not
recommended printer values.

| Parameter | Measurement/effect |
| --- | --- |
| `capture_mm` | capture the tip delivered at the gears |
| `melt_mm` | additional travel to the melt zone/nozzle |
| `purge_mm` | purge after reaching the nozzle |
| `tip_push_mm` | short extrusion before tip forming |
| `tip_retract_mm` | initial fast retraction |
| `tip_dwell_ms` | tip cooling wait |
| `cooling_move_mm`, `cooling_moves` | cooling push/pull cycles, zero net travel |
| `unload_mm` | total net withdrawal from nozzle until clear of the gears |
| `*_speed` | stage speeds in mm/s |
| `chunk_mm` | chunks up to 20 mm, cancellation checked between chunks |

Test capture, nozzle travel and purge separately. Inspect the unloaded tip,
tune cooling, cycles, distances and speeds for the material, and verify free
passage through PTFE/combiner. Confirm complete gear release before BMCU
pullback, then set `calibrated: 1`. No physical calibration was performed here.

`LOAD_FILAMENT`/`UNLOAD_FILAMENT` operate the extruder. For full BMCU transport,
use **Filament**, or `BMCU_LOAD DEVICE=bmcu0 CHANNEL=0` /
`BMCU_UNLOAD DEVICE=bmcu0 CHANNEL=0`. Screen channels 1–4 correspond to commands
0–3. When BMCU is ready, screen load opens channels and unload uses the confirmed
loaded route. Logical `T1+` sources do not create physical extruders.

Generic callbacks `BMCU_TOOLHEAD_PREPARE`/`BMCU_BEFORE_PULLBACK` use `WAIT=1`
and return only after purge/release and `M400`, as required by the
[Generic contract](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/printers/Generic/README.md).
Without `WAIT=1`, manual macros heat asynchronously; do not use that mode for
slicer tool changes or immediately before printing.

## Temperature, state and cancellation

Display and macros share a controller: reuse a valid target or select the
PLA/PETG profile. Callback material selects the fallback; other materials need
an appropriate existing target or explicit `TEMP`, for example
`LOAD_FILAMENT TEMP=215 WAIT=1`. Cold-extrusion protection stays enabled.
Motion waits for current temperature within 1 degree below target and
`can_extrude` true. Target changes, timeout or failures abort visibly.
Cancellation/failure restores the previous target unless another client changed
it. Success retains the target.

During **Heating nozzle**, current/target temperatures keep updating.
Clicking cancels; repeated start requests do not enqueue moves. During motion,
cancellation takes effect after the submitted chunk and before the next.
G-code state is restored on success/failure, including extrusion mode, E
coordinates, feedrate, speed and flow multipliers. Sequences use 100% speed
and flow so calibration is independent of print overrides.

`PAUSE` keeps the route and package hooks. `RESUME` is blocked during filament
operations before the package refill hook and preserves that hook when idle.
The guard is installed after ready and checked again before operations to
cover hooks added by manual endpoint setup. `CANCEL_PRINT` cancels local
operations, requests `BMCU_STOP`, turns heaters/fan off and cancels virtual SD.
Screen cancellation works during synchronous callback heating. External G-code
is serialized by Klipper's mutex and may wait for that callback.
In 1.0.6 `BMCU_STOP` stops motors but is not a complete host-flow cancellation
API. Screen cancellation prevents the next extruder callback from moving and
requests BMCU stop when the current transport phase returns.
Wait for physical motion to stop and inspect the route before recovery;
never confirm `LOADED`/`EMPTY` without inspection.

The active channel comes from the runtime `loaded_tools` route map, not the
last click or `now_channel`. Version 1.0.6 freezes public device snapshots
during prints and pauses. A read-only adapter uses its in-memory connection
and route fields without UART I/O; runtime errors and uncertain routes block
actions. Missing/offline/version-mismatched BMCU does not prevent
navigation. Unavailable channels are disabled, uncertain routes block motion.
Panel/slicer operations remain owned by BMCU; callbacks also require calibration.
Without an endpoint sensor Generic refill pauses for manual replacement.
Enable automatic refill only after physical validation.

## Updating an existing installation

For the switch-based v2 integration, follow the separate-source update and full
host/display rollback instructions in the
[installation guide](Ender3_V3_SE_Configuracao_Validada.md#install-or-update-an-existing-integration).
Do not update active `~/klipper` with `git pull` before preserving it. The
installer must run from a separate clean source with `--host-source` pointing
there; changed MCU/build inputs or Python requirements require separate review.
It does not flash firmware or install dependencies. Existing calibrated values
are preserved. **Re-slice old jobs using the v2 profile**; no Portuguese command
aliases or automatic conversion of stored G-code are provided. The original
template documented above remains separate: do not replace its endpoint setup
with the validated integration's commissioning command.

## Pending physical acceptance

Test channels 1–4, capture/purge, cutter-free tip forming, pause/resume,
heating/motion cancellation, timeout/disconnection and uncertain-route recovery.
Check both screen languages, Portuguese accents, all rows, long filename
scrolling and state/channel readability at 240×320. Tests verify UTF-8 boundaries
and protocol limits, not the display's internal font glyphs.
