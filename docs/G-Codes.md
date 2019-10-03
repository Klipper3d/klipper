This document describes the commands that Klipper supports. These are
commands that one may enter into the OctoPrint terminal tab.

# G-Code commands

Klipper supports the following standard G-Code commands:
- Move (G0 or G1): `G1 [X<pos>] [Y<pos>] [Z<pos>] [E<pos>] [F<speed>]`
- Dwell: `G4 P<milliseconds>`
- Move to origin: `G28 [X] [Y] [Z]`
- Turn off motors: `M18` or `M84`
- Wait for current moves to finish: `M400`
- Select tool: `T<index>`
- Use absolute/relative distances for extrusion: `M82`, `M83`
- Use absolute/relative coordinates: `G90`, `G91`
- Set position: `G92 [X<pos>] [Y<pos>] [Z<pos>] [E<pos>]`
- Set speed factor override percentage: `M220 S<percent>`
- Set extrude factor override percentage: `M221 S<percent>`
- Set acceleration: `M204 S<value>`
- Get extruder temperature: `M105`
- Set extruder temperature: `M104 [T<index>] [S<temperature>]`
- Set extruder temperature and wait: `M109 [T<index>] S<temperature>`
  - Note: M109 always waits for temperature to settle at requested
    value
- Set bed temperature: `M140 [S<temperature>]`
- Set bed temperature and wait: `M190 S<temperature>`
  - Note: M190 always waits for temperature to settle at requested
    value
- Set fan speed: `M106 S<value>`
- Turn fan off: `M107`
- Emergency stop: `M112`
- Get current position: `M114`
- Get firmware version: `M115`

For further details on the above commands see the
[RepRap G-Code documentation](http://reprap.org/wiki/G-code).

Klipper's goal is to support the G-Code commands produced by common
3rd party software (eg, OctoPrint, Printrun, Slic3r, Cura, etc.) in
their standard configurations. It is not a goal to support every
possible G-Code command. Instead, Klipper prefers human readable
["extended G-Code commands"](#extended-g-code-commands).

If one requires a less common G-Code command then it may be possible
to implement it with a custom Klipper gcode_macro (see
[example-extras.cfg](https://github.com/KevinOConnor/klipper/tree/master/config/example-extras.cfg)
for details). For example, one might use this to implement: `G10`,
`G11`, `G12`, `G29`, `G30`, `G31`, `M42`, `M80`, `M81`, etc.

## G-Code SD card commands

Klipper also supports the following standard G-Code commands if the
"virtual_sdcard" config section is enabled:
- List SD card: `M20`
- Initialize SD card: `M21`
- Select SD file: `M23 <filename>`
- Start/resume SD print: `M24`
- Pause SD print: `M25`
- Set SD position: `M26 S<offset>`
- Report SD print status: `M27`

## G-Code display commands

The following standard G-Code commands are available if a "display"
config section is enabled:
- Display Message: `M117 <message>`
- Set build percentage: `M73 P<percent>`

## Other available G-Code commands

The following standard G-Code commands are currently available, but
using them is not recommended:
- Offset axes: `M206 [X<offset>] [Y<offset>] [Z<offset>]` (Use
  SET_GCODE_OFFSET instead.)
- Get Endstop Status: `M119` (Use QUERY_ENDSTOPS instead.)

# Extended G-Code Commands

Klipper uses "extended" G-Code commands for general configuration and
status.  These extended commands all follow a similar format - they
start with a command name and may be followed by one or more
parameters. For example: `SET_SERVO SERVO=myservo ANGLE=5.3`. In this
document, the commands and parameters are shown in uppercase, however
they are not case sensitive. (So, "SET_SERVO" and "set_servo" both run
the same command.)

The following standard commands are supported:
- `QUERY_ENDSTOPS`: Probe the axis endstops and report if they are
  "triggered" or in an "open" state. This command is typically used to
  verify that an endstop is working correctly.
- `GET_POSITION`: Return information on the current location of the
  toolhead.
- `SET_GCODE_OFFSET [X=<pos>|X_ADJUST=<adjust>]
  [Y=<pos>|Y_ADJUST=<adjust>] [Z=<pos>|Z_ADJUST=<adjust>]
  [MOVE=1 [MOVE_SPEED=<speed>]]`: Set a positional offset to apply to
  future G-Code commands. This is commonly used to virtually change
  the Z bed offset or to set nozzle XY offsets when switching
  extruders. For example, if "SET_GCODE_OFFSET Z=0.2" is sent, then
  future G-Code moves will have 0.2mm added to their Z height. If the
  X_ADJUST style parameters are used, then the adjustment will be
  added to any existing offset (eg, "SET_GCODE_OFFSET Z=-0.2" followed
  by "SET_GCODE_OFFSET Z_ADJUST=0.3" would result in a total Z offset
  of 0.1). If "MOVE=1" is specified then a toolhead move will be
  issued to apply the given offset (otherwise the offset will take
  effect on the next absolute G-Code move that specifies the given
  axis). If "MOVE_SPEED" is specified then the toolhead move will be
  performed with the given speed (in mm/s); otherwise the toolhead
  move will use the last specified G-Code speed.
- `SAVE_GCODE_STATE [NAME=<state_name>]`: Save the current
  g-code coordinate parsing state. Saving and restoring the g-code
  state is useful in scripts and macros. This command saves the
  current g-code absolute coordinate mode (G90/G91), absolute extrude
  mode (M82/M83), origin (G92), offset (SET_GCODE_OFFSET), speed
  override (M220), extruder override (M221), move speed, current XYZ
  position, and relative extruder "E" position. If NAME is provided it
  allows one to name the saved state to the given string. If NAME is
  not provided it defaults to "default".
- `RESTORE_GCODE_STATE [NAME=<state_name>]
  [MOVE=1 [MOVE_SPEED=<speed>]]`: Restore a state previously saved via
  SAVE_GCODE_STATE. If "MOVE=1" is specified then a toolhead move will
  be issued to move back to the previous XYZ position. If "MOVE_SPEED"
  is specified then the toolhead move will be performed with the given
  speed (in mm/s); otherwise the toolhead move will use the restored
  g-code speed.
- `PID_CALIBRATE HEATER=<config_name> TARGET=<temperature>
  [WRITE_FILE=1]`: Perform a PID calibration test. The specified
  heater will be enabled until the specified target temperature is
  reached, and then the heater will be turned off and on for several
  cycles. If the WRITE_FILE parameter is enabled, then the file
  /tmp/heattest.txt will be created with a log of all temperature
  samples taken during the test.
- `TURN_OFF_HEATERS`: Turn off all heaters.
- `SET_VELOCITY_LIMIT [VELOCITY=<value>] [ACCEL=<value>]
  [ACCEL_TO_DECEL=<value>] [SQUARE_CORNER_VELOCITY=<value>]`: Modify
  the printer's velocity limits. Note that one may only set values
  less than or equal to the limits specified in the config file.
- `SET_HEATER_TEMPERATURE HEATER=<heater_name> [TARGET=<target_temperature>]`:
  Sets the target temperature for a heater. If a target temperature is
  not supplied, the target is 0.
- `SET_PRESSURE_ADVANCE [EXTRUDER=<config_name>] [ADVANCE=<pressure_advance>]
  [ADVANCE_LOOKAHEAD_TIME=<pressure_advance_lookahead_time>]`:
  Set pressure advance parameters. If EXTRUDER is not specified, it
  defaults to the active extruder.
- `STEPPER_BUZZ STEPPER=<config_name>`: Move the given stepper forward
  one mm and then backward one mm, repeated 10 times. This is a
  diagnostic tool to help verify stepper connectivity.
- `MANUAL_PROBE [SPEED=<speed>]`: Run a helper script useful for
  measuring the height of the nozzle at a given location. If SPEED is
  specified, it sets the speed of TESTZ commands (the default is
  5mm/s). During a manual probe, the following additional commands are
  available:
  - `ACCEPT`: This command accepts the current Z position and
  concludes the manual probing tool.
  - `ABORT`: This command terminates the manual probing tool.
  - `TESTZ Z=<value>`: This command moves the nozzle up or down by the
    amount specified in "value". For example, `TESTZ Z=-.1` would move
    the nozzle down .1mm while `TESTZ Z=.1` would move the nozzle up
    .1mm. The value may also be `+`, `-`, `++`, or `--` to move the
    nozzle up or down an amount relative to previous attempts.
- `Z_ENDSTOP_CALIBRATE [SPEED=<speed>]`: Run a helper script useful
  for calibrating a Z position_endstop config setting. See the
  MANUAL_PROBE command for details on the parameters and the
  additional commands available while the tool is active.
- `TUNING_TOWER COMMAND=<command> PARAMETER=<name> START=<value>
  FACTOR=<value> [BAND=<value>]`: A tool for tuning a parameter on
  each Z height during a print. The tool will run the given COMMAND
  with the given PARAMETER assigned to the value using the formula
  `value = start + factor * z_height`. If BAND is provided then the
  adjustment will only be made every BAND millimeters of z height - in
  that case the formula used is `value = start + factor *
  ((floor(z_height / band) + .5) * band)`.
- `SET_IDLE_TIMEOUT [TIMEOUT=<timeout>]`:  Allows the user to set the
  idle timeout (in seconds).
- `RESTART`: This will cause the host software to reload its config
  and perform an internal reset. This command will not clear error
  state from the micro-controller (see FIRMWARE_RESTART) nor will it
  load new software (see
  [the FAQ](FAQ.md#how-do-i-upgrade-to-the-latest-software)).
- `FIRMWARE_RESTART`: This is similar to a RESTART command, but it
  also clears any error state from the micro-controller.
- `SAVE_CONFIG`: This command will overwrite the main printer config
  file and restart the host software. This command is used in
  conjunction with other calibration commands to store the results of
  calibration tests.
- `STATUS`: Report the Klipper host software status.
- `HELP`: Report the list of available extended G-Code commands.

## G-Code Macro Commands

The following command is available when a "gcode_macro" config section
is enabled:
- `SET_GCODE_VARIABLE MACRO=<macro_name> VARIABLE=<name>
  VALUE=<value>`: This command allows one to change the value of a
  gcode_macro variable at run-time. The provided VALUE is parsed as a
  Python literal.

## Custom Pin Commands

The following command is available when an "output_pin" config section
is enabled:
- `SET_PIN PIN=config_name VALUE=<value>`

## Neopixel and Dotstar Commands

The following command is available when "neopixel" or "dotstar" config
sections are enabled:
- `SET_LED LED=<config_name> INDEX=<index> RED=<value> GREEN=<value>
  BLUE=<value>`: This sets the LED output. Each color <value> must be
  between 0.0 and 1.0. If multiple LED chips are daisy-chained then
  one may specify INDEX to alter the color of just the given chip (1
  for the first chip, 2 for the second, etc.). If INDEX is not
  provided then all LEDs in the daisy-chain will be set to the
  provided color.

## Servo Commands

The following commands are available when a "servo" config section is
enabled:
- `SET_SERVO SERVO=config_name [WIDTH=<seconds>] [ENABLE=<0|1>]`
- `SET_SERVO SERVO=config_name [ANGLE=<degrees>] [ENABLE=<0|1>]`

## Manual stepper Commands

The following command is available when a "manual_stepper" config
section is enabled:
- `MANUAL_STEPPER STEPPER=config_name [ENABLE=[0|1]]
  [SET_POSITION=<pos>] [SPEED=<speed>] [ACCEL=<accel>]
  [MOVE=<pos> [STOP_ON_ENDSTOP=1]]`: This command will alter the state
  of the stepper. Use the ENABLE parameter to enable/disable the
  stepper. Use the SET_POSITION parameter to force the stepper to
  think it is at the given position. Use the MOVE parameter to request
  a movement to the given position. If SPEED and/or ACCEL is specified
  then the given values will be used instead of the defaults specified
  in the config file. If an ACCEL of zero is specified then no
  acceleration will be preformed. If STOP_ON_ENDSTOP is specified then
  the move will end early should the endstop report as triggered (use
  STOP_ON_ENDSTOP=-1 to stop early should the endstop report not
  triggered).

## Probe

The following commands are available when a "probe" config section is
enabled:
- `PROBE [PROBE_SPEED=<mm/s>] [SAMPLES=<count>]
  [SAMPLE_RETRACT_DIST=<mm>] [SAMPLES_TOLERANCE=<mm>]
  [SAMPLES_TOLERANCE_RETRIES=<count>]
  [SAMPLES_RESULT=median|average]`: Move the nozzle downwards until
  the probe triggers. If any of the optional parameters are provided
  they override their equivalent setting in the probe config section
  (see
  [example-extras.cfg](https://github.com/KevinOConnor/klipper/tree/master/config/example-extras.cfg)
  for details).
- `QUERY_PROBE`: Report the current status of the probe ("triggered"
  or "open").
- `PROBE_ACCURACY [PROBE_SPEED=<mm/s>] [SAMPLES=<count>]
  [SAMPLE_RETRACT_DIST=<mm>]`: Calculate the maximum, minimum,
  average, median, and standard deviation of multiple probe
  samples. By default, 10 SAMPLES are taken. Otherwise the optional
  parameters default to their equivalent setting in the probe config
  section.
- `PROBE_CALIBRATE [SPEED=<speed>] [<probe_parameter>=<value>]`: Run a
  helper script useful for calibrating the probe's z_offset. See the
  PROBE command for details on the optional probe parameters. See the
  MANUAL_PROBE command for details on the SPEED parameter and the
  additional commands available while the tool is active.

## BLTouch

The following command is available when a "bltouch" config section is
enabled:
- `BLTOUCH_DEBUG COMMAND=<command>`: This sends a command to the
  BLTouch. It may be useful for debugging. Available commands are:
  pin_down, touch_mode, pin_up, self_test, reset.

See [Working with the BL-Touch](BLTouch.md) for more details.

## Delta Calibration

The following commands are available when the "delta_calibrate" config
section is enabled:
- `DELTA_CALIBRATE [METHOD=manual] [<probe_parameter>=<value>]`: This
  command will probe seven points on the bed and recommend updated
  endstop positions, tower angles, and radius. See the PROBE command
  for details on the optional probe parameters. If METHOD=manual is
  specified then the manual probing tool is activated - see the
  MANUAL_PROBE command above for details on the additional commands
  available while this tool is active.
- `DELTA_ANALYZE`: This command is used during enhanced delta
  calibration. See [Delta Calibrate](Delta_Calibrate.md) for details.

## Bed Tilt

The following commands are available when the "bed_tilt" config
section is enabled:
- `BED_TILT_CALIBRATE [METHOD=manual] [<probe_parameter>=<value>]`:
  This command will probe the points specified in the config and then
  recommend updated x and y tilt adjustments. See the PROBE command
  for details on the optional probe parameters. If METHOD=manual is
  specified then the manual probing tool is activated - see the
  MANUAL_PROBE command above for details on the additional commands
  available while this tool is active.

## Mesh Bed Leveling

The following commands are available when the "bed_mesh" config
section is enabled:
- `BED_MESH_CALIBRATE [METHOD=manual] [<probe_parameter>=<value>]`:
  This command probes the bed using generated points specified by the
  parameters in the config. After probing, a mesh is generated and
  z-movement is adjusted according to the mesh. See the PROBE command
  for details on the optional probe parameters. If METHOD=manual is
  specified then the manual probing tool is activated - see the
  MANUAL_PROBE command above for details on the additional commands
  available while this tool is active.
- `BED_MESH_OUTPUT`: This command outputs the current probed z values
  and current mesh values to the terminal.
- `BED_MESH_MAP`: This command probes the bed in a similar fashion
  to BED_MESH_CALIBRATE, however no mesh is generated.  Instead,
  the probed z values are serialized to json and output to the
  terminal.  This allows octoprint plugins to easily capture the
  data and generate maps approximating the bed's surface.  Note
  that although no mesh is generated, any currently stored mesh
  will be cleared.
- `BED_MESH_CLEAR`: This command clears the mesh and removes all
  z adjustment.  It is recommended to put this in your end-gcode.
- `BED_MESH_PROFILE LOAD=<name> SAVE=<name> REMOVE=<name>`: This
  command provides profile management for mesh state.  LOAD will
  restore the mesh state from the profile matching the supplied name.
  SAVE will save the current mesh state to a profile matching the
  supplied name.  Remove will delete the profile matching the
  supplied name from persistent memory.  Note that after SAVE or
  REMOVE operations have been run the SAVE_CONFIG gcode must be run
  to make the changes to peristent memory permanent.

## Bed Screws Helper

The following commands are available when the "bed_screws" config
section is enabled:
- `BED_SCREWS_ADJUST`: This command will invoke the bed screws
  adjustment tool. It will command the nozzle to different locations
  (as defined in the config file) and allow one to make adjustments to
  the bed screws so that the bed is a constant distance from the
  nozzle.

## Bed Screws Tilt adjust Helper

The following commands are available when the "screws_tilt_adjust"
config section is enabled:
- `SCREWS_TILT_CALCULATE [<probe_parameter>=<value>]`: This command
  will invoke the bed screws adjustment tool. It will command the
  nozzle to different locations (as defined in the config file)
  probing the z height and calculate the number of knob turns to
  adjust the bed level. See the PROBE command for details on the
  optional probe parameters.
  IMPORTANT: You MUST always do a G28 before using this command.

## Z Tilt

The following commands are available when the "z_tilt" config section
is enabled:
- `Z_TILT_ADJUST [<probe_parameter>=<value>]`: This command will probe
  the points specified in the config and then make independent
  adjustments to each Z stepper to compensate for tilt. See the PROBE
  command for details on the optional probe parameters.

## Dual Carriages

The following command is available when the "dual_carriage" config
section is enabled:
- `SET_DUAL_CARRIAGE CARRIAGE=[0|1]`: This command will set the active
  carriage. It is typically invoked from the activate_gcode and
  deactivate_gcode fields in a multiple extruder configuration.

## TMC2130, TMC2660, TMC2208, TMC2209 and TMC5160

The following commands are available when any of the "tmcXXXX" config sections is enabled:
- `DUMP_TMC STEPPER=<name>`: This command will read the TMC driver
  registers and report their values.
- `INIT_TMC STEPPER=<name>`: This command will intitialize the TMC
  registers. Needed to re-enable the driver if power to the chip is
  turned off then back on.
- `SET_TMC_CURRENT STEPPER=<name> CURRENT=<amps> HOLDCURRENT=<amps>`:
  This will adjust the run and hold currents of the TMC driver.
  HOLDCURRENT is applicable only to the tmc2130, tmc2208, tmc2209 and tmc5160.
- `SET_TMC_FIELD STEPPER=<name> FIELD=<field> VALUE=<value>`: This will
  alter the value of the specified register field of the TMC driver.
  This command is intended for low-level diagnostics and debugging only because
  changing the fields during run-time can lead to undesired and potentially
  dangerous behavior of your printer. Permanent changes should be made using
  the printer configuration file instead. No sanity checks are performed for the
  given values.

## Endstop adjustments by stepper phase

The following commands are available when an "endstop_phase" config
section is enabled:
- `ENDSTOP_PHASE_CALIBRATE [STEPPER=<config_name>]`: If no STEPPER
  parameter is provided then this command will reports statistics on
  endstop stepper phases during past homing operations. When a STEPPER
  parameter is provided it arranges for the given endstop phase
  setting to be written to the config file (in conjunction with the
  SAVE_CONFIG command).

## Force movement

The following commands are available when the "force_move" config
section is enabled:
- `FORCE_MOVE STEPPER=<config_name> DISTANCE=<value> VELOCITY=<value>
  [ACCEL=<value>]`: This command will forcibly move the given stepper
  the given distance (in mm) at the given constant velocity (in
  mm/s). If ACCEL is specified and is greater than zero, then the
  given acceleration (in mm/s^2) will be used; otherwise no
  acceleration is performed. No boundary checks are performed; no
  kinematic updates are made; other parallel steppers on an axis will
  not be moved. Use caution as an incorrect command could cause
  damage! Using this command will almost certainly place the low-level
  kinematics in an incorrect state; issue a G28 afterwards to reset
  the kinematics. This command is intended for low-level diagnostics
  and debugging.
- `SET_KINEMATIC_POSITION [X=<value>] [Y=<value>] [Z=<value>]`: Force
  the low-level kinematic code to believe the toolhead is at the given
  cartesian position. This is a diagnostic and debugging command; use
  SET_GCODE_OFFSET and/or G92 for regular axis transformations. If an
  axis is not specified then it will default to the position that the
  head was last commanded to. Setting an incorrect or invalid position
  may lead to internal software errors. This command may invalidate
  future boundary checks; issue a G28 afterwards to reset the
  kinematics.

## Send message (respond) to host

The following commands are availabe when the "respond" config section is
enabled.
  - `M118 <message>`: echo the message prepended with the configured default
    prefix (or `echo: ` if no prefix is configured).
  - `RESPOND MSG="<message>"`: echo the message prepended with the configured default
    prefix (or `echo: ` if no prefix is configured).
  - `RESPOND TYPE=echo MSG="<message>"`: echo the message prepended with `echo: `.
  - `RESPOND TYPE=command MSG="<message>"`: echo the message prepended with `// `.
    Octopint can be configured to respond to these messages (e.g.
    `RESPOND TYPE=command MSG=action:pause`).
  - `RESPOND TYPE=error MSG="<message>"`: echo the message prepended with `!! `.
  - `RESPOND PREFIX=<prefix> MSG="<message>"`: echo the message prepended with `<prefix>`
    (The `PREFIX` parameter will take priority over the `TYPE` parameter)

## Pause Resume

The following commands are available when the "pause_resume" config section
is enabled:
  - `PAUSE`:  Pauses the current print.  The current position is captured for
  restoration upon resume.
  - `RESUME [VELOCITY=<value>]`: Resumes the print from a pause, first restoring
  the previously captured position.  The VELOCITY parameter determines the speed
  at which the tool should return to the original captured position.
  - `CLEAR_PAUSE`:  Clears the current paused state without resuming the print.
  This is useful if one decides to cancel a print after a PAUSE.  It is recommended
  to add this to your start gcode to make sure the paused state is fresh for each
  print.

## Filament Sensor

The following command is available when the "filament_switch_sensor" config
section is enabled.
 - `QUERY_FILAMENT_SENSOR SENSOR=<sensor_name>`: Queries the current status of
  the filament sensor.  The data displayed on the terminal will depend on the
  sensor type defined in the confguration.
 - `SET_FILAMENT_SENSOR SENSOR=<sensor_name> ENABLE=[0|1]`:  Sets the
   filament sensor on/off.  If ENABLE is set to 0, the filament sensor will
   be disabled, if set to 1 it is enabled.

## Firmware Retraction

The following commands are available when the "firmware_retraction"
config section is enabled.  These commands allow you to utilise the
firmware retraction feature available in many slicers, to reduce
stringing during non-extrusion moves from one part of the print to
another.  Appropriately configuring pressure advance reduces the length
of retraction required.
 - `SET_RETRACTION [RETRACT_LENGTH=<mm>] [RETRACT_SPEED=<mm/s>]
   [UNRETRACT_EXTRA_LENGTH=<mm>] [UNRETRACT_SPEED=<mm/s>]`: Adjust the
   parameters used by firmware retraction. RETRACT_LENGTH determines
   the length of filament to retract and unretract. The speed of
   retraction is adjusted via RETRACT_SPEED, and is typically set
   relatively high. The speed of unretraction is adjusted via
   UNRETRACT_SPEED, and is not particularly critical, although often
   lower than RETRACT_SPEED. In some cases it is useful to add a small
   amount of additional length on unretraction, and this is set via
   UNRETRACT_EXTRA_LENGTH. SET_RETRACTION is commonly set as part of
   slicer per-filament configuration, as different filaments require
   different parameter settings.
 - `GET_RETRACTION`: Queries the current parameters used by firmware
   retraction and displays them on the terminal.
 - `G10`: Retracts the extruder using the currently configured
   parameters.
 - `G11`: Unretracts the extruder using the currently configured
   parameters.

## Skew Correction

The following commands are available when the "skew_correction" config
section is enabled.
  - `SET_SKEW [XY=<ac_length,bd_length,ad_length>] [XZ=<ac,bd,ad>]
    [YZ=<ac,bd,ad>] [CLEAR=<0|1>]`:  Configures the [skew_correction] module
    with measurements (in mm) taken from a calibration print.  One may
    enter measurements for any combination of planes, planes not entered will
    retain their current value.  If `CLEAR=1` is entered then all skew
    correction will be disabled.
  - `GET_CURRENT_SKEW`: Reports the current printer skew for each plane in
    both radians and degrees.  The skew is calculated based on parameters
    provided via the `SET_SKEW` gcode.
  - `CALC_MEASURED_SKEW [AC=<ac_length>] [BD=<bd_length>] [AD=<ad_length>]`:
    Calculates and reports the skew (in radians and degrees) based on a
    measured print.  This can be useful for determining the printer's current
    skew after correction has been applied.  It may also be useful before
    correction is applied to determine if skew correction is necessary.   See
    skew_correction.md for details on skew calibration objects and
    measurements.
  - `SKEW_PROFILE [LOAD=<name>] [SAVE=<name>] [REMOVE=<name>]`: Profile
    management for skew_correction.  LOAD will restore skew state from the
    profile matching the supplied name. SAVE will save the current skew state
    to a profile matching the supplied name.  Remove will delete the profile
    matching the supplied name from persistent memory.  Note that after SAVE
    or REMOVE operations have been run the SAVE_CONFIG gcode must be run
    to make the changes to peristent memory permanent.

## Delayed GCode

The following command is enabled if a [delayed_gcode] config section has
been enabled:
  - `UPDATE_DELAYED_GCODE [ID=<name>] [DURATION=<seconds>]`:  Updates the
    delay duration for the identified [delayed_gcode] and starts the timer
    for gcode execution.  A value of 0 will cancel a pending delayed gcode
    from executing.
