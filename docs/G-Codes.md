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
- Set bed temperature: `M140 [S<temperature>]`
- Set bed temperature and wait: `M190 S<temperature>`
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
  [Y=<pos>|Y_ADJUST=<adjust>] [Z=<pos>|Z_ADJUST=<adjust>]`: Set a
  positional offset to apply to future G-Code commands. This is
  commonly used to virtually change the Z bed offset or to set nozzle
  XY offsets when switching extruders. For example, if
  "SET_GCODE_OFFSET Z=0.2" is sent, then future G-Code moves will
  have 0.2mm added to their Z height. If the X_ADJUST style parameters
  are used, then the adjustment will be added to any existing offset
  (eg, "SET_GCODE_OFFSET Z=-0.2" followed by "SET_GCODE_OFFSET
  Z_ADJUST=0.3" would result in a total Z offset of 0.1).
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
- `SET_PRESSURE_ADVANCE [EXTRUDER=<config_name>] [ADVANCE=<pressure_advance>]
  [ADVANCE_LOOKAHEAD_TIME=<pressure_advance_lookahead_time>]`:
  Set pressure advance parameters. If EXTRUDER is not specified, it
  defaults to the active extruder.
- `STEPPER_BUZZ STEPPER=<config_name>`: Move the given stepper forward
  one mm and then backward one mm, repeated 10 times. This is a
  diagnostic tool to help verify stepper connectivity.
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

## Custom Pin Commands

The following command is available when an "output_pin" config section
is enabled:
- `SET_PIN PIN=config_name VALUE=<value>`

## Servo Commands

The following commands are available when a "servo" config section is
enabled:
- `SET_SERVO SERVO=config_name WIDTH=<seconds>`
- `SET_SERVO SERVO=config_name ANGLE=<degrees>`

## Probe

The following commands are available when a "probe" config section is
enabled:
- `PROBE`: Move the nozzle downwards until the probe triggers.
- `QUERY_PROBE`: Report the current status of the probe ("triggered"
  or "open").

## Delta Calibration

The following commands are available when the "delta_calibrate" config
section is enabled:
- `DELTA_CALIBRATE [METHOD=manual]`: This command will probe seven
  points on the bed and recommend updated endstop positions, tower
  angles, and radius.
  - `NEXT`: If manual bed probing is enabled, then one can use this
    command to move to the next probing point during a DELTA_CALIBRATE
    operation.
- `DELTA_ANALYZE`: This command is used during enhanced delta
  calibration. See [Delta Calibrate](Delta_Calibrate.md) for details.

## Bed Tilt

The following commands are available when the "bed_tilt" config
section is enabled:
- `BED_TILT_CALIBRATE [METHOD=manual]`: This command will probe the
  points specified in the config and then recommend updated x and y
  tilt adjustments.
  - `NEXT`: If manual bed probing is enabled, then one can use this
    command to move to the next probing point during a
    BED_TILT_CALIBRATE operation.

## Mesh Bed Leveling

The following commands are available when the "bed_mesh" config
section is enabled:
- `BED_MESH_CALIBRATE [METHOD=manual]`: This command probes the bed
  using generated points specified by the parameters in the
  config. After probing, a mesh is generated and z-movement is
  adjusted according to the mesh.
  - `NEXT`: If manual bed probing is enabled, then one can use this
    command to move to the next probing point during a
    BED_MESH_CALIBRATE operation.
- `BED_MESH_OUTPUT`: This command outputs the current probed z values
  and current mesh values to the terminal.
- `BED_MESH_MAP`: This command probes the bed in a similar fashion
  to BED_MESH_CALIBRATE, however no mesh is generated.  Instead,
  the probed z values are serialized to json and output to the
  terminal.  This allows octoprint plugins to easily capture the
  data and generate maps approximating the bed's surface.  Note
  that although no mesh is generated, any currently stored mesh
  will be cleared as the process rehomes the printer.
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

## Z Tilt

The following commands are available when the "z_tilt" config section
is enabled:
- `Z_TILT_ADJUST`: This command will probe the points specified in the
  config and then make independent adjustments to each Z stepper to
  compensate for tilt.

## Dual Carriages

The following command is available when the "dual_carriage" config
section is enabled:
- `SET_DUAL_CARRIAGE CARRIAGE=[0|1]`: This command will set the active
  carriage. It is typically invoked from the activate_gcode and
  deactivate_gcode fields in a multiple extruder configuration.

## TMC2130

The following command is available when the "tmc2130" config section
is enabled:
- `DUMP_TMC STEPPER=<name>`: This command will read the TMC2130 driver
  registers and report their values.

## Force movement

The following commands are available when the "force_move" config
section is enabled:
- `FORCE_MOVE STEPPER=<config_name> DISTANCE=<value>
  VELOCITY=<value>`: This command will forcibly move the given stepper
  the given distance (in mm) at the given constant velocity (in
  mm/s). No acceleration is performed; no boundary checks are
  performed; no kinematic updates are made; other parallel steppers on
  an axis will not be moved. Use caution as an incorrect command could
  cause damage! Using this command will almost certainly place the
  low-level kinematics in an incorrect state; issue a G28 afterwards
  to reset the kinematics. This command is intended for low-level
  diagnostics and debugging.
- `SET_KINEMATIC_POSITION [X=<value>] [Y=<value>] [Z=<value>]`: Force
  the low-level kinematic code to believe the toolhead is at the given
  cartesian position. This is a diagnostic and debugging command; use
  SET_GCODE_OFFSET and/or G92 for regular axis transformations. If an
  axis is not specified then it will default to the position that the
  head was last commanded to. Setting an incorrect or invalid position
  may lead to internal software errors. This command may invalidate
  future boundary checks; issue a G28 afterwards to reset the
  kinematics.
