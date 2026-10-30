# Filament tip forming and repeated-load test

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
4. Temporarily disable pressure advance and use buffer-follow assistance.
5. Retract 15 mm at 20 mm/s, then approach a 35–45 mm cooling region at
   progressively lower speeds.
6. Perform four 10 mm cooling cycles at progressively increasing speeds.
7. Restore pressure advance and the previous heater target.
8. Retract until the entry sensor clears, then park the additional 55 mm with
   the existing encoder verification.

The final switch search now defaults to F300 (5 mm/s). During forming and the
switch search, extruder motion waits between completed moves when buffer
compression exceeds 60%, leaving feeder assistance active. It resumes at 55%
or below and aborts if the buffer does not recover within three seconds. These
checks do not synchronize motor steps or measure force; a loose fitting or a
restriction may still cause failure within one move. Inspect PTFE fittings,
cut tube ends square and seat them fully before another test.

The final search also aborts if the feeder encoder records less than 1 mm of
net pullback over three seconds. A timeout or stalled encoder reports commanded
distance, encoder displacement after forming, buffer percentage, PWM and motion
mode. No failed operation is confirmed as parked. These checks cannot detect
every kind of slippage or prove that the nozzle is clear.

The buffer limit, recovery threshold and timeouts are editable in
`[bmcu_integracao_ender]` as `limite_buffer_recuo`, `retomar_buffer_recuo`,
`tempo_buffer_recuo` and `tempo_sem_recuo`. Existing custom sequence settings
remain preserved. To explicitly apply the shorter purge and slower final search
through the installer's backup and rollback workflow, use:

```sh
sudo python3 scripts/ender3-v3-se/install_multicolor.py \
  --purge-mm 35 --pullback-speed 300
```

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

Use `RETIRAR_FILAMENTO` to test the complete unload from a known loaded route.
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
