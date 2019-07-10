This document covers recent software changes to the config file that
are not backwards compatible. It is a good idea to review this
document when upgrading the Klipper software.

All dates in this document are approximate.

# Changes

20190710: The z_hop option was removed from the [firmware_retract]
config section. The z_hop support was incomplete and could cause
incorrect behavior with several common slicers.

20190710: The optional parameters of the PROBE_ACCURACY command have
changed. It may be necessary to update any macros or scripts that use
that command.

20190628: All configuration options have been removed from the
[skew_correction] section.  Configuration for skew_correction
is now done via the SET_SKEW gcode.  See skew_correction.md
for recommended usage.

20190607: The "variable_X" parameters of gcode_macro (along with the
VALUE parameter of SET_GCODE_VARIABLE) are now parsed as Python
literals. If a value needs to be assigned a string then wrap the value
in quotes so that it is evaluated as a string.

20190606: The "samples", "samples_result", and "sample_retract_dist"
config options have been moved to the "probe" config section. These
options are no longer supported in the "delta_calibrate", "bed_tilt",
"bed_mesh", "screws_tilt_adjust", "z_tilt", or "quad_gantry_level"
config sections.

20190528: The magic "status" variable in gcode_macro template
evaluation has been renamed to "printer".

20190520: The SET_GCODE_OFFSET command has changed; update any g-code
macros accordingly. The command will no longer apply the requested
offset to the next G1 command. The old behavior may be approximated by
using the new "MOVE=1" parameter.

20190404: The Python host software packages were updated. Users will
need to rerun the ~/klipper/scripts/install-octopi.sh script (or
otherwise upgrade the python dependencies if not using a standard
OctoPi installation).

20190404: The i2c_bus and spi_bus parameters (in various config
sections) now take a bus name instead of a number.

20190404: The sx1509 config parameters have changed. The 'address'
parameter is now 'i2c_address' and it must be specified as a decimal
number. Where 0x3E was previously used, specify 62.

20190328: The min_speed value in [temperature_fan] config
will now be respected and the fan will always run at this
speed or higher in PID mode.

20190322: The default value for "driver_HEND" in [tmc2660] config
sections was changed from 6 to 3. The "driver_VSENSE" field was
removed (it is now automatically calculated from run_current).

20190310: The [controller_fan] config section now always takes a name
(such as [controller_fan my_controller_fan]).

20190308: The "driver_BLANK_TIME_SELECT" field in [tmc2130] and
[tmc2208] config sections has been renamed to "driver_TBL".

20190308: The [tmc2660] config section has changed. A new
sense_resistor config parameter must now be provided. The meaning of
several of the driver_XXX parameters has changed.

20190228: Users of SPI or I2C on SAMD21 boards must now specify the
bus pins via a [samd_sercom] config section.

20190224: The bed_shape option has been removed from bed_mesh.  The
radius option has been renamed to bed_radius.  Users with round beds
should supply the bed_radius and round_probe_count options.

20190107: The i2c_address parameter in the mcp4451 config section
changed. This is a common setting on Smoothieboards. The new value is
half the old value (88 should be changed to 44, and 90 should be
changed to 45).

20181220: Klipper v0.7.0 released
