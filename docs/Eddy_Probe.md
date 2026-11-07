# Eddy Current Inductive probe

This document describes how to use an
[eddy current](https://en.wikipedia.org/wiki/Eddy_current) inductive
probe in Klipper.

Currently, an eddy current probe can not precisely home Z (i.e., `G28 Z`).
The sensor can precisely do Z probing (i.e., `PROBE ...`).
Look at the [homing correction](Eddy_Probe.md#homing-correction-macros)
for further details.

Start by declaring a
[probe_eddy_current config section](Config_Reference.md#probe_eddy_current)
in the printer.cfg file. It is recommended to set `descend_z` to
0.5mm. It is typical for the sensor to require an `x_offset` and
`y_offset`. If these values are not known, one should estimate the
values during initial calibration.

The first step in calibration is to determine the appropriate
DRIVE_CURRENT for the sensor. Home the printer and navigate the
toolhead so that the sensor is near the center of the bed and is about
20mm above the bed. Then issue an `LDC_CALIBRATE_DRIVE_CURRENT
CHIP=<config_name>` command. For example, if the config section was
named `[probe_eddy_current my_eddy_probe]` then one would run
`LDC_CALIBRATE_DRIVE_CURRENT CHIP=my_eddy_probe`. This command should
complete in a few seconds.  After it completes, issue a `SAVE_CONFIG`
command to save the results to the printer.cfg and restart.

Eddy current is used as a proximity/distance sensor (similar to a laser ruler).
The second step in calibration is to correlate the sensor readings to
the corresponding Z heights. Home the printer and navigate the
toolhead so that the nozzle is near the center of the bed. Then run a
`PROBE_EDDY_CURRENT_CALIBRATE CHIP=my_eddy_probe` command. Once the
tool starts, follow the steps described at
["the paper test"](Bed_Level.md#the-paper-test) to determine the
actual distance between the nozzle and bed at the given location. Once
those steps are complete one can `ACCEPT` the position. The tool will
then move the toolhead so that the sensor is above the point where the
nozzle used to be and run a series of movements to correlate the
sensor to Z positions. This will take a couple of minutes. After the
tool completes it will output the sensor performance data:
```
probe_eddy_current: noise 0.000642mm, MAD_Hz=11.314 in 2525 queries
Total frequency range: 45000.012 Hz
z: 0.250 # noise 0.000200mm, MAD_Hz=11.000
z: 0.530 # noise 0.000300mm, MAD_Hz=12.000
z: 1.010 # noise 0.000400mm, MAD_Hz=14.000
z: 2.010 # noise 0.000600mm, MAD_Hz=12.000
z: 3.010 # noise 0.000700mm, MAD_Hz=9.000
```
issue a `SAVE_CONFIG` command to save the results to
the printer.cfg and restart.

After initial calibration it is a good idea to verify that the
`x_offset` and `y_offset` are accurate. Follow the steps to
[calibrate probe x and y offsets](Probe_Calibrate.md#calibrating-probe-x-and-y-offsets).
If either the `x_offset` or `y_offset` is modified then be sure to run
the `PROBE_EDDY_CURRENT_CALIBRATE` command (as described above) after
making the change.

Once calibration is complete, one may use all the standard Klipper
tools that use a Z probe.

Note that eddy current sensors (and inductive probes in general) are
susceptible to "thermal drift". That is, changes in temperature can
result in changes in reported Z height. Changes in either the bed
surface temperature or sensor hardware temperature can skew the
results. It is important that calibration and probing is only done
when the printer is at a stable temperature.

## Homing correction macros

Because of current limitations, homing and probing
are implemented differently for the eddy sensors.
As a result, homing suffers from an offset error,
while probing handles this correctly.

To correct the homing offset.
One can use the suggested macro inside the homing override or
inside the starting G-Code.

[Force move](Config_Reference.md#force_move) section
have to be defined in the config.

```
[gcode_macro _RELOAD_Z_OFFSET_FROM_PROBE]
gcode:
  {% set Z = printer.toolhead.position.z %}
  SET_KINEMATIC_POSITION Z={Z - printer.probe.last_probe_position.z}

[gcode_macro SET_Z_FROM_PROBE]
gcode:
  {% set METHOD = params.METHOD | default("automatic") %}
  PROBE METHOD={METHOD}
  _RELOAD_Z_OFFSET_FROM_PROBE
  G0 Z5
```

## Tap calibration

Eddy current probes support a special kind of probing referred to as
"tap" probing. This mechanism directs the toolhead to descend until
the nozzle makes contact with the bed. That bed contact may cause a
change in sensor measurements which can be detected and used to halt
further downward movement. The nozzle is then lifted away from the bed
and the sensor results from the lifting movement are analyzed to
determine the location where the nozzle breaks contact with the bed.

In order to utilize "tap" probing it is necessary to configure a
`tap_threshold` parameter. This parameter determines when downward
toolhead movement during a "tap" probe should be halted. A value too
large could result in a nozzle/bed contact not detected, which could
result in the nozzle crashing uncontrollably into the bed. A value too
small could result in a "tap" probe attempt halting before making
contact with the bed, which could result in wildly inaccurate probe
results.

The following process is recommended to select an appropriate
`tap_threshold` value.

1. Enable the printer, home the printer, move the toolhead to a
   position near the center of the bed, and make sure the nozzle is
   between 3 - 10 millimeters from the bed.

2. Run the `PROBE_EDDY_CURRENT_TAP_ANALYZE` command. The output should
   contain a line similar to:
   `Calibration: f=3314735.082 s=-48.267 q=7.667`
   Identify, the reported "calibration" `s` parameter, and calculate
   an initial threshold value as `initial_threshold = -s / 10`. So,
   for example, if the value for `s` was reported as `-48.267` then
   one would have an initial value of `4.8267`. Be sure to use the
   values from the output on your printer as each printer will have
   different values.

3. The next step is to run a tap probe attempt with the
   `initial_threshold` setting. Commanding the nozzle into the bed
   always has some risks, so be prepared to issue an emergency halt
   (`M112`) if the probing descent does not stop after contacting the
   bed. When ready issue a tap probe attempt with a command like the
   following:
   `PROBE METHOD=tap TAP_THRESHOLD=<initial_threshold>`
   Ideally the above command will cause the probe to descend until it
   hits the bed, lift away from the bed, and then report a valid probe
   result. If not, see the paragraphs at the end of this section to
   troubleshoot tap results. If the attempt was successful then
   continue to the next step.

4. If the initial tap succeeds then the next step is to calculate a
   refined tap threshold. Run the following command:
   `PROBE_EDDY_CURRENT_TAP_ANALYZE`
   which should produce output with a line similar to:
   `contact_slope_delta=11.468`
   Calculate the refined threshold value as
   `refined_threshold = contact_slope_delta / 5` . So, for example, if
   the value for `contact_slope_delta` was reported as `11.468` then
   one would have a refined threshold value of `2.2936`. Be sure to
   use the values from the output on your printer as each printer will
   have different values.

5. The next step is to run a tap probe attempt with the
   `refined_threshold` setting. Make sure that the nozzle is near the
   center of the bed, that it is between 3 - 10mm above the bed, be
   ready to issue an emergency halt, and then run the following
   command:
   `PROBE METHOD=tap TAP_THRESHOLD=<refined_threshold>`
   Ideally the above command will also succeed; if not, see the
   paragraphs at the end of this section to troubleshoot tap
   results. If the attempt was successful then continue to the next
   step.

6. If probing with the refined threshold is successful then the next
   test is to verify that it is stable over multiple probe attempts.
   Make sure that the nozzle is near the center of the bed, that it is
   between 3 - 10mm above the bed, be ready to issue an emergency
   halt, and then run the following command:
   `PROBE METHOD=tap SAMPLES=5 TAP_THRESHOLD=<refined_threshold>`
   Ideally the above command will also succeed; if not, see the
   paragraphs at the end of this section to troubleshoot tap
   results. If the attempt was successful then continue to the next
   step.

7. If all of the above steps are successful then set the
   `tap_threshold` parameter in the printer.cfg file to the "refined
   threshold" found in the steps above. Calibration should now be
   complete.

If any of the steps above did not succeed then it may be necessary to
troubleshoot and manually determine an appropriate `tap_threshold`.

In general, if a probe attempt halts before making contact with the
bed, then this indicates that the provided `TAP_THRESHOLD` parameter
is too low. Try increasing it by about 10% and retry.

Similarly, if a probe attempt does not halt after making contact with
the bed then it indicates that `TAP_THRESHOLD` is too high. Consider
decreasing the attempted value in half.

## Thermal Drift Calibration

As with all inductive probes, eddy current probes are subject to
significant thermal drift.  If the eddy probe has a temperature
sensor on the coil it is possible to configure a `[temperature_probe]`
to report coil temperature and enable software drift compensation. To
link a temperature probe to an eddy current probe the
`[temperature_probe]` section must share a name with the
`[probe_eddy_current]` section.  For example:

```
[probe_eddy_current my_probe]
# eddy probe configuration...

[temperature_probe my_probe]
# temperature probe configuration...
```

See the [configuration reference](Config_Reference.md#temperature_probe)
for further details on how to configure a `temperature_probe`.  It is
advised to configure the `calibration_position`,
`calibration_extruder_temp`, `extruder_heating_z`, and
`calibration_bed_temp` options, as doing so will automate some of the
steps outlined below.  If the printer to be calibrated is enclosed, it
is strongly recommended to set the `max_validation_temp` option to a value
between 100 and 120.

Eddy probe manufacturers may offer a stock drift calibration that can be
manually added to `drift_calibration` option of the `[probe_eddy_current]`
section. If they do not, or if the stock calibration does not perform well on
your system, the `temperature_probe` module offers a manual calibration
procedure via the `TEMPERATURE_PROBE_CALIBRATE` gcode command.

Prior to performing calibration the user should have an idea of what the
maximum attainable temperature probe coil temperature is.  This temperature
should be used to set the `TARGET` parameter of the
`TEMPERATURE_PROBE_CALIBRATE` command.  The goal is to calibrate across the
widest temperature range possible, thus its desirable to start with the printer
cold and finish with the coil at the maximum temperature it can reach.

Once a `[temperature_probe]` is configured, the following steps may be taken
to perform thermal drift calibration:

- The probe must be calibrated using `PROBE_EDDY_CURRENT_CALIBRATE`
  when a `[temperature_probe]` is configured and linked.  This captures
  the temperature during calibration which is necessary to perform
  thermal drift compensation.
- Make sure the nozzle is free of debris and filament.
- The bed, nozzle, and probe coil should be cold prior to calibration.
- The following steps are required if the `calibration_position`,
  `calibration_extruder_temp`, and `extruder_heating_z` options in
  `[temperature_probe]` are **NOT** configured:
  - Move the tool to the center of the bed.  Z should be 30mm+ above the bed.
  - Heat the extruder to a temperature above the maximum safe bed temperature.
    150-170C should be sufficient for most configurations.  The purpose of
    heating the extruder is to avoid nozzle expansion during calibration.
  - When the extruder temperature has settled, move the Z axis down to about 1mm
    above the bed.
- Start drift calibration.  If the probe's name is `my_probe` and the maximum
  probe temperature we can achieve is 80C, the appropriate gcode command is
  `TEMPERATURE_PROBE_CALIBRATE PROBE=my_probe TARGET=80`.  If configured, the
  tool will move to the X,Y coordinate specified by the `calibration_position`
  and the Z value specified by `extruder_heating_z`.  After heating the extruder
  to the specified temperature the tool will move to the Z value specified
  by the`calibration_position`.
- The procedure will request a manual probe.  Perform the manual probe with
  the paper test and `ACCEPT`.  The calibration procedure will take the first
  set of samples with the probe then park the probe in the heating position.
- If the `calibration_bed_temp` is **NOT** configured turn on the bed heat
  to the maximum safe temperature.  Otherwise this step will be performed
  automatically.
- By default the calibration procedure will request a manual probe every
  2C between samples until the `TARGET` is reached.  The temperature delta
  between samples can be customized by setting the `STEP` parameter in
  `TEMPERATURE_PROBE_CALIBRATE`.  Care should be taken when setting a custom
  `STEP` value, a value too high may request too few samples resulting in
  a poor calibration.
- The following additional gcode commands are available during drift
  calibration:
  - `TEMPERATURE_PROBE_NEXT` may be used to force a new sample before the step
    delta has been reached.
  - `TEMPERATURE_PROBE_COMPLETE` may be used to complete calibration before the
    `TARGET` has been reached.
  - `ABORT` may be used to end calibration and discard results.
- When calibration is finished use `SAVE_CONFIG` to store the drift
  calibration.

As one may conclude, the calibration process outlined above is more challenging
and time consuming than most other procedures.  It may require practice and several attempts to achieve an optimal calibration.

## Errors description

### Possible homing errors and actionables:

- Sensor error
  - Check logs for detailed error
- Eddy I2C STATUS/DATA error.
  - Check loose wiring.
  - Try software I2C/decrease I2C rate
- Invalid read data
  - Same as I2C

Possible sensor errors and actionables:
- Frequency over valid hard range
  - Check frequency configuration
  - Hardware fault
- Frequency over valid soft range
  - Check frequency configuration
- Conversion Watchdog timeout
  - Hardware fault

Amplitude Low/High warning messages can mean:
- Sensor close to the bed
- Sensor far from the bed
- Higher temperature than was at the current calibration
- Capacitor missing

On some sensors, it is not possible to completely avoid amplitude
warning indicator.

You can try to redo the `LDC_CALIBRATE_DRIVE_CURRENT` calibration at work
temperature or increase `reg_drive_current` by 1-2 from the calibrated value.

Generally, it is like an engine check light. It may indicate an issue.

### Possible TAP errors and actionables:

- insufficient lift (A vs B) - too small Z window received by the code
  - LIFT_SPEED/printer.max_z_velocity too low - must be at least 2.5
    Recommended >0.5 RPS. For motor cogging reasons.
  - printer.max_z_accel too low - must be larger 35 mm/s^2.
    Recommended 300 mm/s^2.
  - SAMPLE_RETRACT_DIST too low
    Recommended >2 mm.
- no contact found at z=Z (slope delta A vs B) - retraction analysis outputs
  not expected data. Often toolhead has stopped in mid-air
  because of the measurement noise.
  - Too high printer.max_z_accel - Z resonances, consider Z input shaper
  - Mechanical vibrations - ex. toolhead fan
  - Sensor noise (elevated chamber/bed temperature)
  - Increase of tap_threshold normally allows to alleviate symptoms
- invalid depress distance (Z vs A:B)
  - if Z < A
    - tap_threshold too low
    - PROBE_SPEED too low
    - printer.max_z_accel too low
  - if Z > B
    - Large toolhead tilt (mechanical/installation issue)
    - PROBE_SPEED too high
    - tap_threshold too high
