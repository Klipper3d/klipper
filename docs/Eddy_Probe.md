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
