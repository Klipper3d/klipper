# Filament tip forming and repeated-load test

**English** | [Português brasileiro](pt-BR/Ender3_V3_SE_Tip_Forming.md)

The integration installer now adds `bmcu-formacao-ponta.cfg`. This editable
file is preserved on subsequent installations. It contains an **experimental
starting profile**, not a physically validated calibration for every hotend.

The existing capture and nozzle-feed distances remain unchanged. The example
purge is now 35 mm, reduced from 70 mm following an observed PETG test. After
extruder-first capture, the feeder uses `MOTION_ON_USE`, the same buffer-follow
mode used during printing, instead of repeatedly entering pre-capture push mode.

## Unload sequence

1. Stop the feeder before moving the head or waiting for temperature.
2. Move to the configured purge position and save extrusion state.
3. Optionally wait for a dedicated tip-forming temperature; zero preserves the
   current temperature. Keep the original heater target for the next load.
4. Temporarily disable pressure advance. Keep withdrawal assistance
   (`MOTION_BEFORE_PULL_BACK`) active for both retractions and forward cooling
   moves, preserving the same buffer target throughout tip forming.
5. Retract 15 mm at 20 mm/s, then approach a 35–45 mm cooling region at
   progressively lower speeds.
6. Perform four 10 mm cooling cycles at progressively increasing speeds.
7. Restore pressure advance and the previous heater target.
8. Retract until the entry sensor clears, then park the additional 55 mm with
   the existing encoder verification.

The final switch search now defaults to F300 (5 mm/s). During forming and the
switch search, extruder motion waits between completed moves when buffer
compression exceeds 70%, leaving feeder assistance active. It resumes at 65%
or below and aborts if the buffer does not recover within three seconds. These
checks do not synchronize motor steps or measure force; a loose fitting or a
restriction may still cause failure within one move. Inspect PTFE fittings,
cut tube ends square and seat them fully before another test.

The recovery threshold accounts for BMCU 1.0.1 buffer-follow control: its idle
band can extend up to 65%, depending on load pressure. Waiting for 55% could
incorrectly time out while the feeder is deliberately holding inside that band.

The final search also aborts if the feeder encoder records less than 1 mm of
net pullback over three seconds. A timeout or stalled encoder reports commanded
distance, encoder displacement after forming, buffer percentage, PWM and motion
mode. No failed operation is confirmed as parked. These checks cannot detect
every kind of slippage or prove that the nozzle is clear.

### Encoder checks throughout tip forming

Tip forming previously checked buffer pressure only between movements as long
as 15 or 21 mm, while keeping the feeder in printing FOLLOW mode. A reported
PTFE disconnection during withdrawal exposed the limitation of that sequence:
the final-search checks did not bound uncollected filament during forming.

All forming and final-search extrusion moves now run in monitored increments
of at most 1 mm by default. Both negative and positive forming moves use the
configured withdrawal-assistance target. Both encoder and
entry sensor are checked between increments; UART requests remain outside
Klipper's movement critical sections.

The previous direction-based mode selection switched to printing FOLLOW for
positive cooling strokes. The printing and withdrawal controllers have different
buffer targets, so changing controllers can refill the buffer during a small
forward stroke and trip the cumulative encoder allowance. An observed failure
occurred at net -44 mm on the first positive stroke, with 2.63 mm mismatch,
printing mode 3, buffer 53% and zero motor PWM at the reported snapshot.

In BMCU-Klipper v1.0.6, the withdrawal controller is bidirectional: it supplies
filament below its configured buffer target and retracts above it. Keeping
`MOTION_BEFORE_PULL_BACK` throughout cooling avoids the printing-target refill.
The 2 mm allowance is unchanged and still stops unexpected excess feeder motion
in either extrusion direction. This removes the identified mode-transition
problem; physical tip quality and repeated reloads still need verification.
Normal capture, purge and printing continue using their existing modes.

One encoder reference is kept across the entire unload, including forward and
backward cooling strokes. Before each retraction increment, the integration
compares cumulative commanded extrusion with signed feeder encoder travel.
With the default 2 mm allowance, it does not command another retraction that
would exceed that difference. The extruder waits while withdrawal assistance
remains active; failure to catch up within `unload_progress_timeout` aborts and stops
the feeder without confirming parking. An unexpected assistance-mode change
also aborts the operation.

The defaults `unload_step_mm: 1` and `unload_slack_mm: 2` can be configured in
`[bmcu_ender_integration]`. Existing configurations use these defaults without
overwriting custom distances or tip settings. The capture, nozzle feed, 35 mm
purge, 55 mm parking, cooling distances, requested speeds and temperatures are
unchanged. Actual forming time can increase because each increment waits for
completion and feedback. The resulting tip still needs physical inspection.

Encoder travel is measured at the feeder; it cannot detect every downstream
slip, blockage or damaged fitting. These are feedback checks, not synchronized
motor steps or a force measurement. The new withdrawal sequence requires a
physical unload/reload test before multi-color printing.

### Withdrawal timing and status traffic

Each completed withdrawal increment now supplies one fresh status snapshot for
the following buffer, session, assistance-mode and encoder checks. Consecutive
checks reuse that snapshot only until another move or explicit wait occurs.
Buffer recovery and encoder catch-up waits continue to request fresh status
after yielding. The existing 1 mm increments, 2 mm encoder allowance, buffer
thresholds and requested forming speeds remain in effect.

In the synthetic successful 66 mm unload fixture, status requests dropped from
578 to 181 for the same 146 forming/cooling/withdrawal moves. This measures
communication traffic, not a percentage improvement in physical print time.
Every increment still waits for motion completion. Further overlap of motion
and communication needs separate validation of the bounded motion queue.

After encoder-confirmed parking, the console reports elapsed positioning,
tip-forming, sensor-release, feeder-parking and total sequence times. The same
values are exposed as `bmcu_ender_integration.unload_timing`, with a result of
`awaiting_parking`, `complete`, `failed` or `parking_failed`. The total begins
inside the extruder-release hook; it excludes earlier manual preheating and
native load/unload preparation. Parking time includes the final encoder check.
These measurements identify the next bottleneck without changing temperature,
cooling cycles or filament travel based on an unmeasured total button time.

The buffer limit, recovery threshold and timeouts are editable in
`[bmcu_ender_integration]` as `unload_buffer_limit`, `unload_buffer_resume`,
`unload_buffer_timeout` and `unload_progress_timeout`. Existing custom sequence settings
remain preserved. Follow the
[staged installation flow](Ender3_V3_SE_Configuracao_Validada.md#install-or-update-an-existing-integration)
to preserve the complete previous host/display. Do not update the active
checkout with `git pull` first. After preparing that clean source, only if you
explicitly want the shorter purge and slower final search, use:

```sh
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source" \
  --purge-mm 35 --pullback-speed 300
```

Omit both override flags to preserve their current calibrated values. The
host-only updater refuses changed MCU/build inputs or Python requirements; it
does not flash firmware or install dependencies. Commands and parameters use
English in both display languages. Re-slice old jobs with the v2 Orca profile;
stored G-code is not silently converted.

Install only while idle, with physically parked channels and an empty shared
path. Do not clear an uncertain route by assertion while filament remains in
the shared path. The tip-forming temperatures, distances and cooling cycles are
unchanged by this update. Halving the separate purge does not halve all plastic
extruded during loading: the 20 mm capture and 80 mm nozzle-feed stages remain.

All distances describe commanded extruder travel; an entry sensor cannot prove
that the filament was gripped or that plastic left the nozzle. The cooling
region must remain below the extruder gears. Inspect the unloaded tip and test
recapture before attempting a long print. A restarted controller, missing
filament, cleared entry sensor or failed forming move aborts the operation.

`*_f` settings are in **mm/min**, while the reference Happy Hare profiles use
mm/s. The starting speeds are deliberately below the reference KE profile.
Klipper's configured extrusion distance and velocity limits still apply.
Temperature overrides must remain above `min_extrude_temp`; this is not an
automatic cold-pull routine.

## Calibration

Use `UNLOAD_ACTIVE_CHANNEL` to test the complete unload from a known loaded route.
Do not confirm a route as parked until its physical shared path is clear.

Change one variable at a time without restarting:

```gcode
SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_TIP_SETTINGS VARIABLE=separation_f VALUE=1500
```

For a separate 220 °C experiment:

```gcode
SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_TIP_SETTINGS VARIABLE=temperature VALUE=220
```

To restore the old withdrawal sequence:

```gcode
SET_GCODE_VARIABLE MACRO=_BMCU_ENDER_TIP_SETTINGS VARIABLE=enabled VALUE=0
```

Runtime changes are lost on restart. Persist accepted values in
`bmcu-formacao-ponta.cfg`. No firmware flash is required for these Python and
configuration changes. Start with the 230 °C PETG baseline, inspect the tip and
watch both extruder and feeder; repeated clicks, grinding or compressed filament
are failed tests, even if software reports completion.

## Four-color test

The test block is 24 × 16 × 6.4 mm, with 32 layers at 0.2 mm. Each color occupies
two layers. Channels 1, 2, 3 and 4 repeat four times: 16 loads and 15 inter-color
changes, giving each channel three reloads after its first use. Slice by layer,
without adaptive layers or a wipe tower; the integration performs bucket purging.
The four tool indices map explicitly to physical channels 0–3 on `bmcu0`.

Cut damaged ends, park all channels and confirm an empty shared path before
installing. Check the four channels contain PETG and the selector routes match
the physical connections. Observe the first unload/reload pair. Three reloads
per channel are an initial functional check, not proof of long-print reliability.
The final channel remains loaded so its last tip can be inspected using the
normal unload control after printing.

## References and attribution

- [Happy Hare tip forming](https://github.com/moggieuk/Happy-Hare/blob/main/config/macros/mmu_form_tip.cfg),
  Paul Morgan and contributors, GPLv3: staged nozzle separation, cooling moves
  and temporary pressure-advance removal.
- [Ender 3 V3 KE community profile](https://github.com/3DCoded/MMU-Tip-Configs/blob/main/Ender3-V3-KE.cfg),
  matthew73210: 35 mm cooling position, 10 mm cooling travel, four cycles and no
  temperature change. Its stock hardware and unspecified material do not
  establish compatibility with an upgraded SE ceramic hotend.
- [3MS rapid tip-shaping experiment](https://github.com/3DCoded/3MS/issues/25):
  an alternative staged sequence; its historical timing claims are not a
  performance guarantee for this integration.
- [BMCU generic endpoint contract](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/printers/Generic/README.md):
  tip formation and extruder release belong before long feeder pullback.
