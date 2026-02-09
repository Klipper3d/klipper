# Axis Twist Compensation

This document describes the `[axis_twist_compensation]` module.

Some printers may have a small twist in their X rail which can skew the results
of a probe attached to the X carriage.
This is common in printers with designs like the Prusa MK3, Sovol SV06 etc and is
further described under [probe location
bias](Probe_Calibrate.md#location-bias-check). It may result in
probe operations such as [Bed Mesh](Bed_Mesh.md),
[Screws Tilt Adjust](G-Codes.md#screws_tilt_adjust),
[Z Tilt Adjust](G-Codes.md#z_tilt_adjust) etc returning inaccurate
representations of the bed.

This module uses manual measurements by the user to correct the probe's results.
Note that if your axis is significantly twisted it is strongly recommended to
first use mechanical means to fix it prior to applying software corrections.

**Warning**: This module is not compatible with dockable probes yet and will
try to probe the bed without attaching the probe if you use it.

## Overview of compensation usage

> **Tip:** Make sure the [probe X and Y offsets](Config_Reference.md#probe) are
> correctly set as they greatly influence calibration.

### Basic Usage: X-Axis Calibration
1. After setting up the `[axis_twist_compensation]` module, run:
```
AXIS_TWIST_COMPENSATION_CALIBRATE
```
This command will calibrate the X-axis by default.
  - The calibration wizard will prompt you to measure the probe Z offset at
    several points along the bed.
  - By default, the calibration uses 3 points, but you can specify a different
    number with the option:
``
SAMPLE_COUNT=<value>
``

2. **Adjust Your Z Offset:**
After completing the calibration, be sure to
[adjust your Z offset](Probe_Calibrate.md#calibrating-probe-z-offset).

3. **Perform Bed Leveling Operations:**
Use probe-based operations as needed, such as:
  - [Screws Tilt Adjust](G-Codes.md#screws_tilt_adjust)
  - [Z Tilt Adjust](G-Codes.md#z_tilt_adjust)

4. **Finalize the Setup:**
  - Home all axes, and perform a [Bed Mesh](Bed_Mesh.md) if necessary.
  - Run a test print, followed by any
    [fine-tuning](Axis_Twist_Compensation.md#fine-tuning)
    if needed.

### For Y-Axis Calibration
The calibration process for the Y-axis is similar to the X-axis. To calibrate
the Y-axis, use:
```
AXIS_TWIST_COMPENSATION_CALIBRATE AXIS=Y
```
This will guide you through the same measuring process as for the X-axis.

> **Tip:** Bed temperature and nozzle temperature and size do not seem to have
> an influence to the calibration process.

## [axis_twist_compensation] setup and commands

Configuration options for `[axis_twist_compensation]` can be found in the
[Configuration Reference](Config_Reference.md#axis_twist_compensation).

Commands for `[axis_twist_compensation]` can be found in the
[G-Codes Reference](G-Codes.md#axis_twist_compensation)
