This document describes the method for calibrating the x, y, and z
offsets of an "automatic z probe" in Klipper. This is useful for users
that have a `[probe]` or `[bltouch]` section in their config file.

# Calibrating probe X and Y offsets

To calibrate the X and Y offset, navigate to the OctoPrint "Control"
tab, home the printer, and then use the OctoPrint jogging buttons to
move the head to a position near the center of the bed.

Place a piece of blue painters tape (or similar) on the bed underneath
the probe. Navigate to the OctoPrint "Terminal" tab and issue a PROBE
command:
```
PROBE
```
Place a mark on the tape directly under where the probe is (or use a
similar method to note the location on the bed).

Issue a `GET_POSITION` command and record the toolhead XY location
reported by that command. For example if one sees:
```
Recv: // toolhead: X:46.500000 Y:27.000000 Z:15.000000 E:0.000000
```
then one would record a probe X position of 46.5 and probe Y position
of 27.

After recording the probe position, issue a series of G1 commands
until the nozzle is directly above the mark on the bed. For example,
one might issue:
```
G1 F300 X57 Y30 Z15
```
to move the nozzle to an X position of 57 and Y of 30. Once one finds
the position directly above the mark, use the `GET_POSITION` command
to report that position. This is the nozzle position.

The x_offset is then the `nozzle_x_position - probe_x_position` and
y_offset is similarly the `nozzle_y_position - probe_y_position`.
Update the printer.cfg file with the given values, remove the
tape/marks from the bed, and then issue a `RESTART` command so that
the new values take effect.

# Calibrating probe Z offset

Providing an accurate probe Z offset is critical to obtaining high
quality prints.

Start by homing the printer and then move the head to a position near
the center of the bed. Navigate to the OctoPrint terminal tab and run
the `PROBE_CALIBRATE` command to start Klipper's probe calibration
tool.

This tool will perform an automatic probe, then lift the head, move
the nozzle over the location of the probe point, and start the manual
probe tool. Once the manual probe tool starts, perform the
["paper test"](Bed_Level.md#the-paper-test) to determine the actual
nozzle height at the given point.

If the nozzle does not move to a position above the probe point, then
`ABORT` the manual probe tool and perform the XY probe offset
calibration described above.

After completing the "paper test", use the `ACCEPT` command to
accept the current Z height and then use the `SAVE_CONFIG` command to
save the given probe z_offset to the config file.
