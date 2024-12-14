# Eddy Current Inductive probe

This document describes how to use an
[eddy current](https://en.wikipedia.org/wiki/Eddy_current) inductive
probe in Klipper.

Currently, an eddy current probe can not be used for Z homing. The
sensor can only be used for Z probing.

Start by declaring a
[probe_eddy_current config section](Config_Reference.md#probe_eddy_current)
in the printer.cfg file. It is recommended to set the `z_offset` to
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

The second step in calibration is to correlate the sensor readings to
the corresponding Z heights. Home the printer and navigate the
toolhead so that the nozzle is near the center of the bed. Then run an
`PROBE_EDDY_CURRENT_CALIBRATE CHIP=my_eddy_probe` command. Once the
tool starts, follow the steps described at
["the paper test"](Bed_Level.md#the-paper-test) to determine the
actual distance between the nozzle and bed at the given location. Once
those steps are complete one can `ACCEPT` the position. The tool will
then move the the toolhead so that the sensor is above the point where
the nozzle used to be and run a series of movements to correlate the
sensor to Z positions. This will take a couple of minutes. After the
tool completes, issue a `SAVE_CONFIG` command to save the results to
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
