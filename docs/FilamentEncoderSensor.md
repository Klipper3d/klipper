This module will monitor the movement of the filament, and raise an event when no movement has been detected
for a predetermined length of time.

Because Klipper does not have much granularity in its status flags, you must enable/disable the sensor during
critical points in the printing process.

Use SET_ENCODER_SENSOR SENSOR=my_sensor ENABLE=1 directly before the beginning of actual extrusion. This could
be within your start_print macro, or start_print gcode in the slicer. What ever method you choose, let the printer
finish heating the tool head and the bed, and just before it starts to print anything, even a purge line, enable the
sensor. This prevents the module from raising an event while the printer is sitting idle waiting for the temps to
stabilize.

In a similar manner, you must disable the sensor immediately upon the conclusion of the print, i.e. the first line of
your end_print module, or end print gcode in the slicer. This should also be done if you use a cancel_print macro,
and if you don't, consider making one just for this purpose. If you don't, the module will notice that no filament is
being used as the tool head is parked and the bed moved to its final position, and an event will be declared.
