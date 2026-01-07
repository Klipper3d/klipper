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
z_offset: 0.250 # noise 0.000200mm, MAD_Hz=11.000
z_offset: 0.530 # noise 0.000300mm, MAD_Hz=12.000
z_offset: 1.010 # noise 0.000400mm, MAD_Hz=14.000
z_offset: 2.010 # noise 0.000600mm, MAD_Hz=12.000
z_offset: 3.010 # noise 0.000700mm, MAD_Hz=9.000
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

## Tap calibration

The Eddy probe measures the resonance frequency of the coil.
By the absolute value of the frequency and the calibration curve from
`PROBE_EDDY_CURRENT_CALIBRATE`, it is therefore possible to detect
where the bed is without physical contact.

By use of the same knowledge, we know that frequency changes with
the distance. It is possible to track that change in real time and
detect the time/position where contact happens - a change of frequency
starts to change in a different way.
For example, stopped to change because of the collision.

Because eddy output is not perfect: there is sensor noise,
mechanical oscillation, thermal expansion and other discrepancies,
it is required to calibrate the stop threshold for your machine.
Practically, it ensures that the Eddy's output data absolute value
change per second (velocity) is high enough - higher than the noise level,
and that upon collision it always decreases by at least this value.

```
[probe_eddy_current my_probe]
# eddy probe configuration...
tap_threshold: 0
```

The suggested calibration routine works as follows:
1. Home Z
2. Place the toolhead at the center of the bed.
3. Move the Z axis far away (30 mm, for example).
4. Run `PROBE METHOD=tap`
5. If it stops before colliding, increase the `tap_threshold`.

Repeat until the nozzle softly touches the bed.
This is easier to do with a clean nozzle and
by visually inspecting the process.

You can streamline the process by placing the toolhead in the center once.
Then, upon config restart, trick the machine into thinking that Z is homed.
```
SET_KINEMATIC_POSITION X=<middle> Y=<middle> Z=0
G0 Z5 # Optional retract
PROBE METHOD=tap
```

Here is an example sequence of threshold values to test:
```
1 -> 5 -> 10 -> 20 -> 40 -> 80 -> 160
160 -> 120 -> 100
```
Your value will normally be between those.
- Too high a value leaves a less safe margin for early collision -
if something is between the nozzle and the bed, or if the nozzle
is too close to the bed before the tap.
- Too low - can make the toolhead stop in mid-air
because of the noise.

You can estimate the initial threshold value by analyzing your own
calibration routine output:
```
probe_eddy_current: noise 0.000642mm, MAD_Hz=11.314
...
z_offset: 1.010 # noise 0.000400mm, MAD_Hz=14.000
```
The estimation will be:
```
MAD_Hz * 2
11.314 * 2 = 22.628
```

You can validate the tap precision by measuring the paper thickness
from the initial calibration guide. It is expected to be ~0.1mm.

Tap precision is limited by the sampling frequency and
the speed of the descent.
If you take 24 photos per second of the moving train, you can only estimate
where the train was between photos.

It is possible to reduce the descending speed. It may require decrease of
absolute `tap_threshold` value.

It is possible to tap over non-conductive surfaces as long as there is metal
behind it within the sensor's sensitivity range.
Max distance can be approximated to be about 1.5x of the coil's narrowest part.

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

Possible homing errors and actionables:

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
