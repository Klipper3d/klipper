# Ender 3 V3 SE Stock Display Architecture

**English** | [Português brasileiro](pt-BR/E3V3SE_Display.md)

## Data path

```text
Mainsail / Moonraker
        |
        v
Klippy objects and G-code commands
        |
        v
e3v3se_display Python package
        |
        v
Klipper MCU protocol over USART1 PA10/PA9
        |
        v
GD32F303RET6 display bridge
        |
        v
USART2 PA3/PA2 at 115200 baud
        |
        v
Stock TJC3224 / DWIN display
```

The rotary encoder is not received from the display UART. It is connected to
the mainboard on PA12, PA11, and PB1 and uses Klipper's upstream `MenuKeys` and
`buttons` implementation.

## MCU module

`src/stm32/e3v3se_display.c` is intentionally target-specific. It avoids the
historical generic bridge's unused ports, invalid configurations, mutable
global instances, and oversized response frames.

The module provides:

- `config_e3v3se_display oid=%c baud=%u`
- `e3v3se_display_send oid=%c data=%*s`
- `e3v3se_display_response oid=%c data=%*s`
- `e3v3se_display_get_status oid=%c`
- `e3v3se_display_status ... rx_overflows ... tx_overflows`

It also embeds `E3V3SE_DISPLAY_BRIDGE=1` and
`E3V3SE_DISPLAY_MAX_CHUNK=40` in the MCU dictionary. Klippy checks the first
constant before sending configuration. A host paired with firmware that lacks
the bridge fails early with an actionable configuration error instead of the
historical `Unknown command: serial_bridge_send` loop.

USART2 uses 256-byte RX and TX rings. Overflow drops display data rather than
shutting down motion control or corrupting the host-MCU protocol.

## Message limit

Current Klipper uses:

- `MESSAGE_MAX = 64`
- Two header bytes
- Three CRC/sync trailer bytes
- At most 59 bytes of encoded payload

The command message still needs an encoded msgid, OID, and dynamic-string
length. This implementation therefore limits each display transport fragment
to 40 bytes. The host fragments every frame, including raw drawing frames; the
MCU also reports incoming display data in blocks no larger than 40 bytes.

Chunking occurs after UTF-8 encoding. UI truncation also operates on encoded
bytes and removes only an incomplete final codepoint. Long selected filenames
scroll, while transport fragmentation remains independent of visible length.

## Host package

The package under `klippy/extras/e3v3se_display/` contains:

- `transport.py`: current MCU APIs, bounded queue, 40-byte chunking, and UART
  pacing.
- `tjc3224.py`: immutable frame creation and the minimal DWIN drawing subset.
- `model.py`: pure file-browser and filename-validation helpers.
- `ui.py`: text-based status, files, movement, temperature, tuning,
  calibration, print control, and custom macro menus.
- `__init__.py`: Klipper object integration and dependency validation.

There are no mutable class-level frame buffers. Every `RESTART` creates a new
transport, protocol driver, file browser, and UI state. No data from a previous
printer instance can be appended to the first frame of the new instance.

All periodic and key callbacks catch display exceptions. A UI rendering error
is logged but is not allowed to invoke a Klipper shutdown.

Selected filenames are passed directly to `virtual_sdcard` through a constructed
`GCodeCommand`; they are not interpolated into a G-code script. This preserves
spaces, quotes, non-ASCII text, `#`, `;`, and literal backslashes without
creating command-injection paths.

## Source of truth

The display reads fresh status every second from:

- `extruder`
- `heater_bed`
- `fan` when configured
- `toolhead`
- `gcode_move`
- `print_stats`
- `virtual_sdcard`
- `pause_resume`
- `display_status`
- `manual_probe`

The display does not maintain a second print state. Actions from Mainsail and
Moonraker become visible on the next update. Display actions invoke the same
public G-code commands used by the web interface.

## Screen firmware

The historical implementation required Creality display firmware 1.0.6 because
its menus used hardcoded IDs for language images, icons, and full-screen assets.
Those IDs can move between display firmware releases.

This implementation draws its interface with protocol primitives and text and
does not ship the opaque `e3v3sedisplay.bin`. Version 1.0.6 remains the validated
baseline because it is the version documented by the reference project. Other
versions may implement the same drawing protocol, but are not claimed to be
compatible until physically tested.

## UI behavior

The dashboard shows temperatures, signed X/Y/Z positions, print state,
progress, filename, and `M117`/`SET_DISPLAY_TEXT` messages. The menu provides:

- Recursive virtual SD file selection with an explicit empty state
- Home and bounded relative movement
- Hotend, bed, fan, speed factor, and flow factor adjustment
- Pause, resume, and confirmed cancel
- Live Z-offset adjustment after Z is homed
- Explicit CR-Touch `PROBE_CALIBRATE`
- Explicit `BED_MESH_CALIBRATE`
- Manual probe `TESTZ`, `ACCEPT`, and `ABORT`
- Safe load/unload macros that refuse cold extrusion
- User-defined display macro entries

Movement is refused when the selected axis is not homed. Extrusion is refused
when Klipper reports `can_extrude=false`. No nozzle cleaning, PRTouch, or
automatic Z-offset procedure is run by the display.

## Menus and language

The idle main menu is **Print / Filament / Temperature / Move / Calibration /
Settings**, plus **Macros** only when custom display macros exist.

- **Print:** recursive virtual-SD browser and confirmed file start.
- **Filament:** ordinary single-extruder load/unload without BMCU; otherwise
  **Filament / BMCU** with four physical channel loads and active-channel unload.
- **Temperature:** nozzle/bed targets, fan, PLA/PETG/TPU preheat and cooldown.
- **Move:** home, bounded X/Y/Z/E movement, motors and optional cutter stop.
- **Calibration:** CR-Touch, bed mesh and screw tilt when configured. Manual
  probing uses the existing `TESTZ`, `ACCEPT` and `ABORT` commands.
- **Settings:** language and a separate confirmed configuration save.

**Settings → Language → English / Português** applies the selection immediately
and stages it through `configfile.set`; it does not restart Klipper.
**Settings → Save config** runs `SAVE_CONFIG` after confirmation to persist it
and restart. Saving is refused during printing, pause or a busy operation.
The values are `en` and `pt_BR`. Included language preferences are moved to
top-level `printer.cfg` by the installer to avoid native autosave conflicts.
Commands and parameters remain English in both screen languages. Custom labels
and unknown external diagnostics retain their original text.

## BMCU reset and paused recovery

The validated integration's **Reset BMCU** requires confirmation that the hotend
and shared filament path have been manually cleared, then invokes
`BMCU_ENDER_RESET CONFIRM=1`. It reconciles route state, not factory settings;
it does not move filament, erase calibration or recover a paused print.
Printing, pause, pending recovery and busy operations block the action.
Loading uncertain routes is disabled, but the guarded reset remains available
outside a print. The backend revalidates conditions when confirmation executes.

During printing, pause/tune/cancel remain contextual. A paused failed tool
change offers **Recover filament**, showing the failed physical channel and
failure details. **Confirm clear path** confirms the operator's physical
inspection and calls `BMCU_CONFIRM_CLEAR_PATH CHANNEL=<1-4> CONFIRM=1` for that
channel. Resume/cancel reuse existing print recovery, never a separate print
state or global reset. See the [recovery guide](Ender3_V3_SE_Print_Recovery.md).

When an installed calibrated cutter is uncertain, recovery offers only
**Close cutter stop**. After the head-clearance confirmation it sends
`CUTTER_SERVO POSITION=RETRACTED CONFIRM=1` and returns to recovery, without
homing, jogging or deploying the stop. Idle **Move** offers both stop positions
with that same confirmation and backend. It only commands the servo; the
integration retains its existing interlocks and X-first homing sequence.

## Custom shortcuts

The template no longer duplicates load/unload in default display-macro sections.
The G-code macros behind those operations remain available. Custom
`[e3v3se_display macro <name>]` entries with `label` and `gcode` remain supported;
duplicate names cause a configuration error. The installer removes only the
exact redundant defaults, preserving customized labels and scripts.

## Diagnostics

Use:

```text
ENDER3V3SE_DISPLAY_STATUS
ENDER3V3SE_DISPLAY_REFRESH
```

`ENDER3V3SE_DISPLAY_STATUS` reports connection state, queued bytes, frames dropped
by the bounded host queue, and both MCU UART overflow counters. Klippy polls the
counters and logs every increase. MCU firmware version, Kconfig, constants, and
command dictionary are already logged by current upstream Klipper.
