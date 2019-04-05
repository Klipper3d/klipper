This document covers recent software changes to the config file that
are not backwards compatible. It is a good idea to review this
document when upgrading the Klipper software.

All dates in this document are approximate.

# Changes

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
