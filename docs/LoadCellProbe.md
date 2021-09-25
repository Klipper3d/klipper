# Load-cell based probe

A load-cell based probe offers an offset-free measurement of the precise
Z coordinate of the contact position between nozzle and print bed. Under certain
mechanical conditions, a sensitivity can be even achieved in X and Y directions,
which can be useful for CNC routers. Downside of a load-cell probe compared to
other probe types is the slower speed and higher complexity.

Please also read the
[reference of the config section](Config_Reference.md#load_cell_probe), and the
[reference of the g-code commands](G-Codes.md#load-cell-probe).

## Mechanical and electrical setup

<p align="center">
  <img src="img/load-cell-probe-setup.png" width="30%"/>
</p>

There may be many possibilities how to realise a probe based on load cells. The
setup used for testing and developing this algorithm is as follows:

- The hotend is suspended on a pair of load cells such that any force applied
  on the nozzle by the bed can be measured. This is shown in the picture above:
  The two load cells are the bars on left and right with the green arrow.

- The two load cells form a rectangle, together with the guiding carriage
  (shown in red/green/gray in the above image) and a traverse carrying the
  hotend (the grey bar on top of the load cells).

- The two load cells are connected in parallel to a measurment amplifier and
  an ADC.

- When the nozzle is not in contact with the bed, the load cells will measure
  the weight of the hotend and other mechanical parts attached to the cells.

- Since the cells are designed for weighing, the measured signal will change
  into the negative direction when the nozzle touches the bed (less weight or
  even upwards pointing force).

- In the picture, the extruder motor is not shown, but it is attached to the
  carriage directly, not to the load cells. This has the advantage that the
  load cells can measure the extrusion force, which can be used to optimise
  print settings manually, or for some advanced algorithms to optimise the
  print automatically. This presents a major advantage over other probes. The
  disadvantage is though that high extrusion forces will cause the hotend to
  move closed to the bed as the load cells bend. This effect does not play a big
  role, since high extrusion forces anyway indicate issues.

## Working principle of the probe algorithm

A very basic approach is to move the nozzle in small steps towards the bed and
measure the force after each step. If the force exceeds a certain threshold, a
contact between nozzle and bed is assumed. This simple approach leads to very
coarse results, hence a more sophisticated algorithm is used.

The load cell measurement is subject to certain instabilities and drifts over
time for several reasons: The rectangle formed by the two load cells, the
carraige and the traverse will have some internal tension, which may change
slightly if forces are applied (e.g. when the nozzle makes contact with the
bed), or sometimes even if the carriage is just moved. Also thermal effects can
lead to drifts of the measurement. For this reason, it is important to make the
algorithm insensitive against changed of the baseline.

### Drift-compensated measurement and averaging

This can be achieved by a special procedure for each measurement point to be
taken: After each force measurement, the nozzle is lifted away from the bed a
bit to measure the baseline. The baseline measurement is then subtracted from
the force measurement to obtain the drift-compensated result. The distance to
move away from the bed can be configured through the `compensation_z_lift`
parameter.

To increase precision, averaging can be used for both the force and baseline
measurement. At both positions, `adc_n_average` readouts are taken and averaged.
To make sure no mechanical vibrations can disturb the measurement, the biggest
difference of all readouts is required to be smaller than `max_variance`,
otherwise the readouts are repeated after a short delay
(`delay_exceeding_max_variance`).

### The phases of a probe approach

As the drift-compensated measurement takes too long to simply scan in very small
steps, the follow phases starting with a low-precision fast approach are
implemented:

#### Fast approach

To get fast close to the surface, a simple scan without drift-compensated
measurements and a relative big step size is chosen. This requires a certain
optimisation of the parameters `step_size` and `threshold`, so that the nozzle
does not hit the surface too hard and the scan runs fast and reliably enough.
A single compensated measurement is made for verification, when the threshold is
exceeded. Only if the bed contact is confirmed, the next phase is started.
Otherwise the fast approach is continued.

#### Find fit start position

Due to possible hysteresis effects, it has been found to be more reliable to
perform the series of measurements for the fit into the negative z direction.
For maximum precision, the series should start at a position with a low or zero
force. Since the fast approach has already moved a bit too far, the nozzle now
needs to be moved away from the bed again. In the interest of saving time, it is
important not to move too far away - ideally the nozzle should be right above
the bed without touching it after this second phase. This is achieved by
extrapolating the measured force linearily and finding the z position with zero
force. To avoid overshooting, several iterations of at most `step_size/2` are
performed until the force is below `fit_threshold`.

#### The fit

This final phase will determine the z position of the contact position
precisely. Starting from the position determined in the last phase, the force
is measured in dependence of the z position in regular intervals, while moving
into the negative z direction. Each measurement is dift-compensated as described
above for maximum precision. The result of this scan is shown in the following
figure:

<p align="center">
  <img src="img/load-cell-probe-fit.svg" width="50%"/>
</p>

The scan is done while moving into the negative z direction, hence the data
points on the right side are measured first. Since the start position determiend
in the last phase is intentionally slightly above the bed, the first few
measurement points will have measured forces very close to 0 (in the image,
these are the rightmost 4 measurement points). Once the force exceeds the given
`fit_threshold`, as many as `fit_points` measurements are taken (6 in the
image). Then, a linear fit is applied (red line in the image) to determine the
z position at which the nozzle touches the bed but the force is still zero
(intersection of the dashed lines).

Finally, the printer will move to the determined z position, such that
additional commands like `G92` can be used easily. Also the last measured z
position can be obtained in g-code macros through
`printer["load_cell_probe"].last_z_result`.

#### Accessing the load cell measured force in other modules

Other modules can subscribe to the continuous read out of the load cell to
receive the current force even while no probe command is executed. Any number of
modules can subscribe by registering a callback function as follows:

```
  self.printer.lookup_object('load_cell').subscribe_force(self.force_callback)
```

The callback function must expect a single argument (besides `self`) which is
the current force. It will be called periodically whenever the ADC sends its
data. This mechanism is necessary, since ADCs normally allow only one single
subscriber. The force will be already corrected with the last known compensation
offset, e.g. from a compensated measurement.

Also the last measured force can be accessed in g-code macros and display_data
config sections trough `printer["load_cell_probe"].last_force`. This value will
be compensated with the last known offset, too.

This feature can be used to implement advanced features, especially if also the
extrusion force can be measured, in case the extruder motor is *not* suspended
on the load cells.

#### Probing in X/Y direction for CNC routers

Depending on the exact geometry of the setup, the load cells might also have a
sensitivity for forces in the X and Y directions. This can be used to implement
a touch probe for a CNC router. If the milling spindle is suspended on the load
cells, any shaft with a known diameter can be chucked into the collet and used
as a probe. For this purpose, the `PROBE` command supports the optional
`DIRECTION` parameter to select other probing directions than the default
negative z direction.

Since moving the printer in X/Y direction can influence the baseline of the
force measurement (see above), the compensation measurement is also done after
moving the probe away from the workpart in z direction. Usually a much larger
distance is required in that case to make sure the probe has no contact with the
workpart any more, hence this can be configured individually through the
`compensation_z_lift_xy` parameter.

## Setup for new printer types

So far, this load cell probe algorithm has been setup only on one single class
of 3D printers, which can double as a CNC router. Hence, a good procedure how
to set this up on other printer models is not yet known. Still, a few hints are
provided below to get you started. If you are setting this up on a new printer,
please share your experience with the community so others can benefit.

For all configuration parameters, the default values may provide a good
starting point. They have been optimised for the geometry shown in the above
picture with two 5kg rated load cells (type TAL220B).

1. Make sure the load cell reading is correct. Ideally, place the force readout
   (`printer["load_cell_probe"].last_force`) on the display, so it can be
   monitored throughout the process. Alternatively, the `READ_LOAD_CELL` can be
   used. Absolute values do not matter, also the direction of the changes (sign
   of the differences) should not play any role.
2. Observe the idle load cell reading (without force). It should be relatively
   stable. If fluctuations are too big, increase `adc_n_average`. Keep in mind
   that the force shown on the display is not averaged, while the force printed
   by `READ_LOAD_CELL` is.
3. When pushing the nozzle upwards with a finger, the force readout must
   change. Small forces (e.g. just touching) already need to result in
   significant value changes.
4. Adjust `max_variance` to be 3 to 5 times bigger than the idle fluctuations.
5. Move the hotend around (though gcode commands) and observe the readings.
   Choose the `threshold` for the fast approach to be a bit bigger than the
   maximum observed fluctuations (peak to peak). Make sure the force
   represented by the chosen value is far away from any dangerous level for
   hotend and bed.
6. Choose the `step_size` for the fast approach such that you are comfortable
   with moving two steps into the negative z direction even when the nozzle
   already touches the bed. Keep in mind, too small step sizes will slow down
   the fast approach significantly, while not increasing the precision at all.
7. Choose a `compensation_z_lift` which is bigger than the `step_size`. A good
   value might be twice as big.
8. For the fit, choose a `fit_threshold` to be around 2-3 times bigger than the
   idle fluctuations, but also bigger than the fluctuations while moving the
   hotend.
9. Choosing the ideal `fit_step_size` may need a bit trial and error. The
   distance must not be too small for the printer to move precisely. On the
   other hand, the product `fit_points * fit_step_size` must be a distance
   which is safe to move into negative z direction when the nozzle already
   touches the bed (i.e. `fit_points * fit_step_size < step_size`). These
   are the hard boundaries which should not be exceeded. The ideal value may
   depend on various mechanical and electrical properties of the printer
   (geometry, load cell type and amplifier). Bigger values may increase
   precision, if the force changes too little between the measurement points.
   Too big values may decrease precision again, because the measurements are
   taken further away from the zero-force contact position to find. For the
   same reason, increasing `fit_points` might not increase the precision, since
   the additional points will be away from the position to find. Measurement
   errors on these far away points can alter the slope of the fitted line such
   that the determined result gets wrong.
10. Before running the probe for the first time, set `max_abs_force` to a safe
    value at which no damage to the printer could occur. Ideally,
    `2*max_abs_force` should still be safe as well.
11. Move the nozzle to a position around the center of the bed and place it in
    a short distance over the bed (e.g. 1mm). Execute the `PROBE` command.
    Observe both the printer movements and the log output carefully. Keep an
    eye on the force readings (as printed to the log) and be ready to turn of
    the printer in case they exceed sane levels.
12. Once the first probe run was successful, optimise the fit parameters by
    using the `PROBE_ACCURACY` command. If the speed is too slow, you might
    optimise mainly the fast approach parameters (which do not have any
    influence on the probe accuracy).

## Best practises

The probe can be used with the bed_mesh plugin to map the surface of the bed.
Due to the slowness of the load cell probe, a bed mesh calibration takes
considerable amount of time. Hence, it is recommended to use a probe measurement
at a single point to compensate for any changes which do not alter the surface
of the bed, e.g. when changing the hotend nozzle. For this, the following
approach is recommended:

* Perform probe measurement at a fixed, known position (e.g. X=Y=100mm).
* Calibate Z endstop position such that the probe result position is Z=0 (at the
  chosen X/Y coordinate).
* Perform bed mesh calibration and store result to configuration
* When needed (e.g. after swapping the nozzle), repeat the first 2 points to
  correct for the changed gloal z offset. It is important to use the same
  X/Y coordinates as before.
* When a new bed mesh calibration is required, repeat all 3 points. Never
  execute a bed mesh calibration without calibrating the Z endstop position as
  described in the first two points.

The following g-code macros implement this:

```
[gcode_macro z_offset_from_probe_result]
gcode:
    SET_GCODE_OFFSET Z={printer["load_cell_probe"].last_z_result}

[gcode_macro bed_calibrate]
gcode:
    G0 Z10
    G0 X100 Y100
    G0 Z2
    PROBE
    Z_OFFSET_FROM_PROBE_RESULT
    Z_OFFSET_APPLY_ENDSTOP
    SET_KINEMATIC_POSITION Z=0
    G0 Z2
    BED_MESH_CALIBRATE
    SAVE_CONFIG

[gcode_macro z_calibrate]
gcode:
    G0 Z10
    G0 X100 Y100
    G0 Z2
    PROBE
    Z_OFFSET_FROM_PROBE_RESULT
    Z_OFFSET_APPLY_ENDSTOP
    G0 Z2
```

Execute `bed_calibrate` to perform a full calibration of the endstop and the
bed mesh. Later, e.g. after nozzle change, just execute `z_calibrate` to only
calibrate the endstop position while keeping the bed mesh unchanged. Note: if
you need to choose a different X/Y position for the scan, make sure to use the
same position in both macros.
