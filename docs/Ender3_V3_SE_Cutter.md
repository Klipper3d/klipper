# Optional filament cutter and Orange Pi servo

**English** | [Português brasileiro](pt-BR/Ender3_V3_SE_Cutter.md)

The mechanical reference is [matarsaed's Ender 3 V3 SE/KE cutter](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke), including its [assembly discussion](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke/comments). The gantry servo mount is adapted from [jmillerfo's Filametrix mount](https://www.printables.com/model/1097275-voron-24r2-filametrix-filament-cutter-gantry-servo). Download CAD from the authors; model licenses remain separate from the software license.

The servo positions a gantry stop. The X carriage presses the cutter lever against that stop; the servo does not directly push the blade. This integration is intended for a cut **below the extruder gears**, leaving the severed nozzle-side filament to be pushed out during the next load and purge. An upstream cutter that leaves an ungripped loose tail needs a different transport sequence and is not supported by this profile.

## Wiring the SG90 to Orange Pi Zero 3

Use a positional SG90, not a continuous-rotation model. The original gantry mount targets an MG90S; fitting an SG90 does not establish its durability under repeated contact loads.

| Connection | Destination |
| --- | --- |
| Servo signal, usually orange/yellow | Proposed physical header pin **15, PC8** |
| Servo ground, usually brown/black | Regulated supply ground and Orange Pi **GND**, e.g. physical pin **6** |
| Servo supply, usually red | Separate regulated **5 V** supply with capacity for servo current peaks |

The GPIO carries the 3.3 V control signal, not motor power. Keep the Orange Pi's supply unchanged, join grounds, and do not backfeed the external supply into the Pi's 5 V header. Never connect the servo to 24 V. Tower Pro reports substantial current peaks; a regulated supply rated for at least 2 A is a conservative starting point for one SG90.

The validated entry switch occupies PC15, physical pin 16. Keep the BMCU's verified UART unchanged; do not enable a PWM overlay on its RX/TX pins. PC8 uses the existing Linux MCU's software PWM and does not require changing UART or boot overlays. Check pulse stability with the camera and normal host load; use hardware PWM or a dedicated MCU if this arrangement jitters. Software tests do not verify electrical timing or actual blade travel.

Before wiring, run on the Orange Pi:

```sh
sudo gpiodetect
sudo gpioinfo
```

Confirm which gpiochip contains PC8 and that its line is unused. In the observed gpiochip layout the proposed PC8 address is `host:gpiochip1/gpio72`, but gpiochip numbering depends on the kernel. The physical pin number is not the GPIO offset. Cross-check [the board pinout](http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_Zero_3#26_Pin_Interface_Pin_Description), the [GPIO library mapping](https://github.com/rm-hull/OPi.GPIO/issues/79) and [Klipper's Linux MCU guidance](https://www.klipper3d.org/RPi_microcontroller.html).

PC9 (physical pin 7) is connected to the AXP313A power-management interrupt in the [Linux Zero 3 device tree](https://github.com/torvalds/linux/blob/v6.18/arch/arm64/boot/dts/allwinner/sun50i-h618-orangepi-zero3.dts). Do not use it as a servo output or disable the PMIC interrupt to free it. A line without a consumer in `gpioinfo` is not proof that it is available as an output: IRQ and pin-mux ownership must also be considered. The installer rejects GPIO offset 73 when the board model identifies an Orange Pi Zero 3. The previous PC9 recommendation was incorrect.

If installation failed on PC9, retain the restored configuration. Power the system down before moving only the servo signal wire to PC8; keep supply and common ground as documented. After booting into a ready state, rerun the installer with the checked PC8 mapping. If the Linux MCU remains shut down after rollback, use `FIRMWARE_RESTART` with the restored configuration before retrying. Do not send servo commands until installation succeeds.

## Installation and calibration

Cancel any print, manually recover a jam, and confirm all routes empty or parked first. Back up the configuration. For an existing installation, stage the published v2 source separately so the installer can preserve the entire previous host/display checkout. **Do not update the active checkout with `git pull` first.** After v2.0.0 is published, and after confirming the proposed GPIO is free:

```sh
git clone --branch v2.0.0 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source" \
  --cutter-servo-pin host:gpiochip1/gpio72
```

Substitute the actual GPIO mapping if different. The staged updater is restricted to host-only changes: MCU sources, libraries, target configurations and Python requirements must match the current checkout. Otherwise it refuses the update and requires a separately reviewed firmware/environment update. It never flashes a printer `.bin`. It preserves the previous checkout, installed optional modules, cutter angles, positions, pulse widths, purge length, PID, mesh and Z offset. Startup failure restores the previous host/display and configuration while retaining current route evidence. A different existing servo pin must be reviewed manually rather than overwritten. Without `--cutter-servo-pin`, the installer adds the optional profile and reset macro without reserving a servo pin. The integration-only installer without `--host-source` cannot restore a checkout changed beforehand.

The new `bmcu-cortador.cfg` ships with `variable_enabled: 0`, `variable_calibrated: 0` and invalid coordinate sentinels. No cutting or faster cutter profile starts by default. `bmcu-cortador-servo.cfg` has no startup angle, so installing the file does not automatically move the servo.

1. With the carriage physically clear of the stop, use `CUTTER_SERVO ANGLE=<safe_angle> CONFIRM=1` to find safe retracted and deployed angles. Start with the horn unloaded if its direction is unknown. Standard pulse limits are only a starting point, not an SG90 clone calibration.
2. Set `variable_retracted_angle`, `variable_deployed_angle`, `variable_approach_x`, `variable_cut_x` and `variable_cut_y` in `bmcu-cortador.cfg`. Measure the actual approach and full cutting stroke on this assembly. Purge coordinates are not cutter coordinates. Keep every coordinate within the configured travel; do not expand machine limits to hide a collision. Set Z clearance for the installed bucket, camera, fan and hub.
3. Restart Klipper, command the calibrated retracted angle, then home. With the hotend/shared path empty and channels parked, run `TEST_CUTTER CONFIRM=1`. This performs a dry mechanical cycle without heating or extruding; inspect full lever travel, spring return and clearance. Failure after deploying the stop leaves its position uncertain and blocks further automatic use.
4. Only after the dry cycle works, set both `variable_calibrated: 1` and `variable_enabled: 1`, then restart.
5. Test a heated load/unload, inspect the cut end and remaining hotend filament, then repeat a short four-color test. Verify nozzle flow during every purge. There is no blade-position, cut-success or post-gears capture sensor in this setup; command completion is not proof of a successful physical cut or unattended four-color reliability.

On the stock display, **Move → Open cutter stop / Close cutter stop** moves only the servo after the operator confirms clearance. **Move → Home all** sends only `G28`, without early retraction or a normal startup confirmation. **Filament / BMCU** owns load/unload and the guarded **Reset BMCU** action; **Temperature** owns all preheat profiles and cooldown. Select **Settings → Language → English / Português** to apply the display language immediately; **Settings → Save config** persists it and restarts Klipper after confirmation. The console and display use the same sequence below, with English commands in both screen languages. Manual `CUTTER_SERVO POSITION=RETRACTED CONFIRM=1` remains available for calibration and recovery with the carriage clear.

With the cutter enabled and calibrated, every `G28` first homes X against its actual configured endstop (−6 mm on this machine), **without any servo command, even `WIDTH=0`, before or during X homing**. Only after X completes and confirms a clear position does it retract the stop, wait `variable_servo_wait_ms` and release PWM according to the setting below. A full `G28` then homes Y (bed) and Z (vertical). Native `safe_z_home` retains one initial protective lift, which may precede X, its Z probing location and its post-probing lift.

**The entire path to the X endstop must be clear with the stop left in its existing position**, including after power loss. Do not use this sequence on a mechanism requiring servo movement to clear X homing itself. No unreferenced X move or artificial reference is used. `G28 X` ends after retraction; `G28 Y` performs X → retract → Y; `G28 Z` performs X → retract → Z and retains the native requirement for Y to be referenced already. The old `recolher_sem_referencia` option is obsolete and cannot bypass X homing. An X, servo or validation failure prevents subsequent stages. An interrupted cut leaving an uncertain stop in the current session still requires manual recovery. Manual filament load/unload uses this same `G28` before claiming its operation when homing or retraction is needed.

If the cutter is enabled but uncalibrated, missing its servo, outside travel limits or left uncertain, operations stop rather than falling back to hot tip forming. With the optional profile disabled, the previous tip-forming workflow is preserved.

`variable_release_retracted_servo: 1` is now the default in `bmcu-cortador.cfg` and when the variable is absent. After commanding the retracted angle and waiting for the horn to settle, the integration sends `SET_SERVO SERVO=bmcu_cortador WIDTH=0` so the servo stops continuously correcting the angle and jittering. **The installer preserves existing profiles:** if your file still contains `variable_release_retracted_servo: 0`, change it to `1` in `[gcode_macro _BMCU_ENDER_CUTTER_SETTINGS]` and restart Klipper to adopt this behavior.

Verify in a dry cycle that the retracted stop remains clear of the entire carriage path without torque, including under vibration. If your mechanism requires holding torque while retracted, explicitly set `0` to keep PWM active. When deployed, the servo retains torque throughout blade travel and carriage retreat; it releases only after retraction. An interrupted cut does not automatically release a deployed stop. Disabling PWM does not switch off the 5 V supply.

If there is free travel between deploying the stop and touching the lever, set `variable_contact_x` to the first contact position. `variable_maximum_stroke_mm` then limits the distance from contact to full cut. With `None` or no setting, the limit still starts at the approach point. Both forward and return moves with the stop deployed use `variable_cut_feedrate` throughout.

Set `variable_keep_cut_y: 1` only if the entire cutting path is clear at every bed position used for tool changes. The cutting cycle ignores `variable_cut_y` in this mode. The purge still uses the configured chute position.

### Retracting before cutting

`variable_pre_cut_retract_mm` defaults to 0 and accepts up to 5 mm, but cannot exceed `unload_slack_mm`. An excessive value rejects the profile before carriage motion. At the approach point, the extruder retracts in monitored increments with the blade clear and the feeder stopped, using the buffer to absorb the difference. A cleared sensor, buffer at its limit, changed session or active feeder motor blocks the blade. The dry test does not move filament.

If filament catches at the heatsink entrance, check for the model author’s `Cutter_Base with guide v1`. Verify alignment and ensure the total retract cannot bring softened material to the blade. Any slicer retract still in effect adds to the integration's retract; there is no automatic compensation. The pre-cut retract uses the integration's `retract_feedrate`; purge distances and blade positions stay the same.

The installer also prevents the native manager from starting withdrawal assistance for 200 ms before `BMCU_RELEASE_ENDER`. It passes the channel and session explicitly because stopping the motors can clear `now_channel`. Assistance starts only during withdrawal after the blade stroke and stop retraction. Run the complete installer rather than copying only the optional modules: the native-manager adjustment belongs to the same update. Other callbacks retain native behavior.

## Faster exchanges

With a calibrated, enabled cutter, the integration replaces separation/cooling moves with the mechanical cutting cycle. It retains switch clearance, encoder feedback, compression limits and the confirmed 55 mm parking withdrawal; it does not allow a new filament into an occupied or uncertain route.

The starting cutter profile uses separate rates, all in mm/min:

| Phase | Starting value |
| --- | --- |
| Extruder withdrawal after cutting | 600 |
| Feed towards the nozzle after capture | 240 |
| Purge | 180 |

Capture distance/speed and purge/parking distances remain those already configured. Verify the BMCU can sustain the withdrawal rate; the validated BMCU pull speed is 10 mm/s. Reduce rates if the extruder skips or nozzle flow is insufficient. Never increase the encoder lag allowance to conceal a feeder that is not following.

`variable_parallel_positioning: 1` allows carriage positioning during the native arrival search **only after all routes are confirmed empty and the entry sensor is clear**. The native BMCU remains responsible for stopping at the entry sensor. Extruder capture waits for queued carriage moves to finish. There is no background loading of a second filament into the shared path, and no native route/tail checks are bypassed.

Compare `unload_timing` and `last_cut` in `bmcu_ender_integration` status. Failed cuts record `last_cut.stage`, including validation and initial retraction failures, instead of retaining an earlier success. `pre_cut_sync` and `post_cut_sync` record commanded retract, encoder, buffer, PWM, motion mode and elapsed time around increments. Movement time includes completion waits, not just motor travel. Tests validate sequencing and interlocks; physical timings and reliability require printer measurements.

Purge positioning and wiping use physical machine coordinates, like cutting, independently of `G92`, G-code offsets and `M220`. Chute and wiper limits and room for the complete Z lift are checked before positioning; insufficient Z clearance blocks XY travel. Configuration distances, rates, coordinates and timeouts must be finite: NaN and infinity are rejected. Final parking verification requires encoder travel, a clear sensor, the same session and idle motors; an earlier completion reply alone is insufficient.

While the entry sensor remains triggered, the extruder continues retracting with BMCU assistance, bounded by timeout, buffer and encoder-progress checks. A feeder motor temporarily stopped at its buffer target does not prove that the tip cleared the gears. The PR #22 attempt to replace extruder retraction at 30 mm with a lower buffer target was reverted after a physical test failed. Pre-cut retraction of 2 mm remains configurable. That revert removes the premature stop but does not, by itself, explain the earlier timeouts near 62 mm.

## Reset after manual withdrawal

After canceling the print and physically clearing the hotend/shared path:

```gcode
RESET_BMCU CONFIRM=1
```

The command stops BMCU motion and uses native route confirmation for all four channels: `PARKED` if its input still detects filament, `EMPTY` otherwise. It requires a clear entry sensor and no active/paused print or BMCU operation. Only after all confirmations succeed does it clear the print session and pending tool change. A partial failure is reported, not presented as a completed reset.

The display's **Filament / BMCU → Reset BMCU** confirms the same action through `BMCU_ENDER_RESET CONFIRM=1`. This is route-state reconciliation, not a factory reset or firmware reboot.

This does not physically unload, heat, extrude, clear a clog, erase calibration, reset the servo's uncertain position or replace paused-print recovery. For a paused tool-change failure, use the existing [recovery procedure](Ender3_V3_SE_Print_Recovery.md): the display's contextual **Recover filament → Confirm clear path** invokes `BMCU_CONFIRM_CLEAR_PATH CHANNEL=<1-4> CONFIRM=1` without erasing the print map. An uncertain cutter must first be physically freed; `CUTTER_SERVO` permits retraction during that pending paused recovery, with explicit confirmation and no BMCU operation active.

Sources: [Klipper servo reference](https://www.klipper3d.org/Config_Reference.html#servo), [Tower Pro SG90](https://towerpro.com.tw/product/sg90-7/) and the mechanical/GPIO references above.
