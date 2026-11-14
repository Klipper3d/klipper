# Filament operation identifiers and print recovery

**English** | [Português brasileiro](pt-BR/Ender3_V3_SE_Print_Recovery.md)

The optional integration requires BMCU-Klipper 1.1.2 and firmware 1.0.4.
Package/firmware maintenance from older versions is separate; see the
[2.0.1 upgrade boundary](releases/v2.0.1.md#upgrade-and-rollback).
The Ender installer changes host files only and does not flash either MCU.

## Cached operation results after a host restart

The transport daemon originally restarted its wire command counter at 1. The
controller retains four completed operation results across a host session
handshake. Reusing a retained identifier can replay an old result instead of
starting the requested operation, including an old success or abort.

The installed `EnderTransportGuard` first requests fresh controller status and
requires an idle operation state in the same session. It then queries the
controller history for a candidate operation identifier. A retained identifier
is skipped without sending a motion command. Only the protocol's explicit
"result unavailable" response authorizes starting the new operation. Up to four
retained results can be skipped; malformed replies, an occupied controller,
session changes, cancellation and expired verification stop the request.

This applies to native feed, retract, encoder-test, autoload and automatic
calibration operations. Normal printing buffer-follow control is unchanged.
The guard does not retry a physical load or withdrawal blindly.

BMCU 1.1.2 randomizes the initial wire counter. The explicit history check is
still retained: randomization alone does not prove a candidate is unused.

## Manual replacement after runout

With automatic refill disabled, native BMCU marks the same-channel replacement
pending and pauses the print. The stock display and
[Mainsail card](Ender3_V3_SE_Mainsail.md) show the physical channel to refill.
Insert the replacement and use normal **Resume** or `RESUME`; do not clear the
print map, reset routes or issue a separate manual load during this pause.
The native wrapper verifies device identity, endpoint, paused state, reconciled
routes, replacement presence and the detached-tail context before loading.
The Ender callback completes capture, nozzle feed, purge and wiping before
native pause/resume returns to the part. Failure retains the pending request
and paused file. Physical arrival and extrusion still need supervised testing.

## Failed tool changes during printing

When `BMCU_CHANGE_TOOL_ENDER` fails while executing a virtual-SD print command, the
integration stops the feeder and pauses the file without propagating the
filament error into `virtual_sdcard.on_error_gcode`. The file remains selected,
the next G-code line does not execute, and the logical tool mapping is retained.
The original print position, coordinate/extrusion state and nozzle target are
saved separately from the purge position.

After checking the filament path, use the normal **Resume** button or `RESUME`.
The pending tool change is completed at the purge position before normal
resumption. An already loaded target is purged/wiped again. The original print
temperature and saved printing state are restored before returning to the part.
If the recovery attempt fails, the file stays paused with the same pending tool.

The `bmcu_ender_integration` status object exposes `recovery_pending`,
`recovery_tool`, `recovery_channel` and `recovery_error`, alongside `manual_busy`,
`unload_timing`, `cutter_uncertain` and `last_cut`. The display uses this existing
state instead of reconstructing or clearing a print session independently.

A paused print with a pending change offers **Recover filament**. The recovery
menu shows the failed physical channel and **Failure details**. **Confirm clear
path** asks for explicit confirmation before calling the guarded handler for
that channel. **Resume print** and **Cancel print** retain the existing recovery
and cancellation guards. A global **Reset BMCU** is not offered in this menu.

If the cutter is uncertain, **Close cutter stop** requires the existing head
clearance confirmation and emits only:

```gcode
CUTTER_SERVO POSITION=RETRACTED CONFIRM=1
```

It returns to recovery without homing, jogging or opening the stop. Both display
languages use the same English commands. Change the display language through
**Settings → Language**; saving it is a separate, guarded configuration restart.

Recovery is blocked when the file or its logical position changed, a route is
uncertain, sensor and route state disagree, another operation is active, the
controller is unavailable, coordinates were changed, motors were disabled or
the machine was homed again. A firmware shutdown or lost mechanical position
cannot be repaired by this pause mechanism. Existing idle-timeout behavior is
preserved; do not leave a pending recovery unattended until motors are disabled.
An entry sensor alone cannot confirm physical nozzle extrusion.

## Confirming a manually cleared path during a paused recovery

For an uncertain route, physically remove any obstruction from the shared
PTFE/extruder path and park that channel's filament behind the entry sensor,
clear of the shared junction. The sensor must read clear. The following command
does not move filament; it records the operator's explicit physical check:

```gcode
BMCU_CONFIRM_CLEAR_PATH CHANNEL=3 CONFIRM=1
```

Replace `3` with the physical channel recorded in the paused failure. This
command is restricted to that channel, the same paused print and an idle feeder.
It rejects an active entry sensor, another occupied route or a detached tail.
Firmware route readback is required before host recovery state is cleared. The
print map is preserved. Then use `RESUME` to finish the pending change.

Do not confirm an empty path merely to bypass an error. The entry switch cannot
detect filament left elsewhere in the shared path. Normal native route
confirmation remains unchanged outside this guarded print-recovery command.

Use `RESET_BMCU CONFIRM=1` only after cancelling the print and manually clearing
the hotend/shared path. It reconciles route state without erasing calibration;
it is not a factory reset or a substitute for paused-print recovery. Its display
entry is under **Filament / BMCU**, outside an active/paused print.

`CANCEL_PRINT` and `SDCARD_RESET_FILE` discard pending recovery. Manual load
errors continue to report normally. Other G-code errors retain the configured
virtual-SD error policy. Restarting Klipper discards the in-memory print recovery;
this is not recovery across a controller restart or power loss.

## Installation and verification

Finish or cancel any print, physically park channels and verify the entry sensor
is clear. Use a separate clean source so the updater can preserve the complete
previous host/display checkout; do not `git pull` the active tree first.
Complete the separate package/firmware maintenance first. After v2.0.1 is published:

```sh
git clone --branch v2.0.1 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source"
```

The staged installer preserves the entire old host/display, affected
configuration and existing calibration. A startup failure restores the previous
checkout and configuration, undoing only callback renames in current BMCU state
so new route evidence is not lost. Recovery must validate the previous host as
ready; otherwise services remain stopped. MCU/build inputs and Python
requirements must match for this host-only update; other changes require
separate firmware/environment review. It does not flash MCUs. See the
[installation guide](Ender3_V3_SE_Configuracao_Validada.md) for rollback details.

Tip-forming, PID, Z offset and mesh settings remain preserved. Omit
`--purge-mm` and `--pullback-speed` to retain their current calibrated values;
those flags are explicit overrides, not required migration steps. Verify a
single-channel load/unload and a short multicolor print before a long print.
Automated tests exercise the actual Klipper virtual-SD dispatcher, pause/resume
and G-code state restoration, plus native transport methods with simulated
controller history. These tests do not replace physical verification.

Native protocol and transport references:

- [BMCU transport daemon v1.1.2](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/scripts/bmcu_transportd.py)
- [BMCU protocol and retained operation results](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/firmware/src/bmcu_protocol.cpp)
