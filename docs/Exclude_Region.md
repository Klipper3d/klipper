# Cancel Obect and Exclude Region

The `[exclude_region]` module allows Klipper to exclude regions or
objects while a print is in progress.

The region or object to be excluded is defined by bounds set by the slicer.  The slicer
configuration may need to be updated to insert the proper markup into the gcode file.

The supported slicers are:

 - *prusaslicer*: The `Label objects` option needs to be enabled.  It is on the `Output
   options` section of the `Print` (not "Printer") tab.

### Extended GCode Commands
`START_CURRENT_OBJECT`: This command takes a `NAME` parameter and denotes the start of
the gcode for an object on the current layer.  The name should be unique between all
objects in the file being printed, but should be consistent across all layers.

`END_CURRENT_OBJECT`:  Denotes the end of the object's gcode for the layer.  Is paired with
`START_CURRENT_OBJECT`.

`CANCEL_OBJECT`: This command takes a `NAME` parameter and instructs Klipper to ignore
gcode for that object.

`REMOVE_ALL_EXCLUDED`: Clears the current list of excluded areas and objects.
