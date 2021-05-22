Annexed Probe


This document is meant to describe behavior related to annexed probes.
These probes are typically microswitches mounted to a body that attaches
to the toolhead through some means of mechanical coupling. This coupling
is commonly done with magnets though this module will support a variety
of designs.

## Basic Configuration

# Dock Position

The configuration options for the probe include all of the standard options
found on a normal `[probe]` with some additional parameters that facilitate
the necessary toolhead movements and gcode commands to attach and detatch
the probe.

```
    dock_position: 300, 295, 0
```

The first option to specify is the `dock_location`. This is simply the XYZ
coordinates where the toolhead needs to be positioned in order to interface
with the probe body. This parameter is X, Y and, Z separated by commas

Many configurations have the dock attached to a moving
gantry so therefor the Z axis positioning is irrelevant. However, it may be
necessary to move the gantry clear of the bed or other components before
performing docking operations. If a Z axis parameter is specified for this
type of dock, the tool will do a Z hop equal to the Z axis value.

Other configurations may have the dock mounted next to the printer bed so
that the Z position _must_ be known prior to attaching the probe. If the
probe is mounted in such a way, the Z axis parameter _must_ be supplied,
and the Z axis _must_ be homed prior to attaching the probe.

# Angles

```
    dock_angle: 90
```

The dock may "open" to any angle in a circle. The angle is relative to the
_direction of travel_ of the toolhead as it leaves the dock location.
For instance, if the dock is located at the upper right of the Y axis near
the X Y max and the tool must move to X max and then in a positive direction
along the X axis to extract the probe, then the dock angle is 270 degrees.

```
                  🡹
          🡼       90     🡽
            135         45
      🡸 180    [Dock]      0 🡺
            225        315
          🡿      270     🡾
                  🡻
```

```
detach_angle:
```

Most probes that use magnets require the tool to "slide" the probe off the
magnetic mount. This is typically done in a move that is perpendicular to the
angle needed to approach and attach the probe. By default, if the `detach_angle`
option is not supplied, the probe will be removed by moving perpendicular to
the dock in whichever direction it believes it has room to maneuver. The
specific left or right side of the move is determined by the min/max values for
the axes as well as the location of the dock.


# Minimum Safe Distance

```
    dock_safe_distance: 15
```

For magnetically coupled probes, the magnets may begin to try and attach
themselves to the toolhead too early and out of alignment if the toolhead
is not lined up properly with the dock. Other docks may be affixed to a
servo motor which moves the dock into position and may collide the with
the toolhead as it actuates.

The dock angle determines an approach vector. This vector is a straight
line that extends into the dock starting at the point that is equal to
the radius of the `dock_safe_distance`.

During probe attach, the toolhead will move directly to this point in a
straight line. If the toolhead is already closer than the
`dock_safe_distance`, the tool will move into the approach vector line,
perpendicular to it. The intent is to prevent the tool from repeately
moving to the approach start point while still respecting the angle of
the approach vector.

# Dock Z location

```
dock_fixed_z:   default false
```

There are various locations that probe docks have been mounted. It seems the
more common locations are next to a bed or on a moving gantry. If the probe
is "fixed" on the Z axis, meaning, it's position relative to XY plane of the
toolhead is always the same, this option should be set to "true".

An example of this is a corexy printer with a bed that moves up and down along
the Z axis and the probe dock is mounted to the frame next to a fixed gantry.
Another example would be a printer with a bed that is fixed in the bottom of the
printer and the gantry moves along all 3 axes. The dock is mounted to a point on
this moving gantry. A final example would be a i3 style printer where the dock
is located on the moving bed. In this example, the `dock_fixed_z` option would
be "false" since when the Z axis moves, the dock's position changes relative to
it.


# Tool velocities

```
    attach_speed:   default is probe speed or 5mm/s
    decouple_speed: default is probe speed or 5mm/s
    travel_speed:   default is 50 mm/s
```

Various optional speeds can be defined for approaching, decoupling and
travelling. If these parameters are omitted, the module will try to
use the `speed` parameter of the probe if defined. If this option is not
supplied, 5mm/s will be used.


# Probe attachment verification

```
    check_open_attach: default false
```

Certain probes when wired a certain way will report `OPEN` when they are
attached and `TRIGGERED` when they are detached in a non-probing state. The
`check_open_attach` option when set to `true` will check the state of the
probe pin after performing a probe attach or docking maneuver. If the probe
reports `TRIGGERED` immediately after the toolhead leaves the dock radius, an
error will be raised and any further action will be aborted. There is a retry
mechanism built in that will attempt to attach or dock the probe 2 additional
times before aborting.

```
    probe_sense_pin:
```

The probe may include a separate pin for attachment verification. This is a
standard pin definition similar to an endstop pin that defines how to handle the
input from the sensor. Much like the `check_open_attach` option, the check is
done immediately after the tool attaches or docks the probe. If the probe
is not attached after attempting to attach it, or it remains attached after
attempting to dock it, an error will be raised and further action will be
aborted.

```
    dock_sense_pin:
```

Certain docks may have a sensor switch incorperated into their design in order
to report that the probe is presently located in the dock. A `dock_sense_pin`
may be provided to provide verification that the probe is correctly positioned
in the dock. This is a standard pin definition similar to an endstop pin that
defines how to handle the input from the sensor. Prior to attempting to attach
the probe, and after attempting to dock it, this pin is checked. If the probe
is not stowed, an error will be raised and further action will be aborted.

# Custom Actions

```
pre_attach_gcode:
attach_gcode:
post_attach_gcode:
pre_detach_gcode:
detach_gcode:
post_detach_gcode:
```

As universal as this module attempts to be, all use cases cannot be accounted
for. As such several "custom" gcode command templates may be supplied to be
executed at various times during probe maneuvers. These optional gcode sections
may be configured like standard gcode macro command templates with the exception
that gcode macro variables, default parameters, and rename_existing options are
excluded.

These command templates are inserted at various points of the probe execution
flow (below) so they may be used to activate a servo or move the toolhead to
a position that aligns cabling or tubes so that they do not snag on parts of
the printer.

# Typical probe execution flow

    - A gcode command requiring the use of the probe is executed.

    - This triggers the probe to attach

    - If configured, the dock sense pin is checked to see if the probe is
      presently in the dock

    - If specified, the pre-attach gcode is executed

    - The toolhead position is compared to the dock position.

    - If the toolhead is outside of the minimum safe radius, the toolhead is
      commanded to move to the the approach vector, that is, a position that is
      the minimum safe distance from the dock in line with the dock angle

    - If the toolhead is inside of the minimum safe radius, the toolhead is
      commanded to move to the nearest point on the line of the approach vector

    - The tool is moved along the approach vector to the dock coordinates

    - If specified, the attach gcode is executed

    - The toolhead is commanded to move out of the dock back to the minimum
      safe distance in the reverse direction along the dock angle.

    - If configured, the probe is checked to see if it is attached

    - If the probe is not attached, the module retries until it is or an error
      is raised

    - If configured, the dock sense pin is checked to see if the probe is still
      present, the module retries until it not or an error is raised

    - If specified, the post-attach gcode is executed

    - The probe moves to the first probing point and beings probing



    - After the probe is no longer needed, the probe is triggered to detach.

    - If specified, the pre-detatch gcode is executed

    - The toolhead position is compared to the dock position.

    - If the toolhead is outside of the minimum safe radius, the toolhead is
      commanded to move to the the approach vector, that is, a position that is
      the minimum safe distance from the dock in line with the dock angle

    - If the toolhead is inside of the minimum safe radius, the toolhead is
      commanded to move to the nearest point on the line of the approach vector

    - The tool is moved along the approach vector to the dock coordinates

    - If specified, the detach gcode is executed

    - The tool is commanded to move along the detach vector if supplied or a
      calculated direction based on axis parameters

    - If configured, the probe is checked to see if it is detached

    - If the probe is not detached, the module moves the tool back to the
      approach vecotor and retries until it detaches or an error is raised

    - If configured, the dock sense pin is checked to see if the probe is
      present in the dock. If it is not the module moves the tool back to the
      approach vecotor and retries until it detaches or an error is raised

    - If specified, the post-detach gcode is executed


# Use in macros

The annexed_probe module uses a delayed detach call so that it does not
repeatedly dock and undock the probe from the toolhead when it is being
used in the context of a homing macro. This works in most use cases, but
if a call to heat-and-wait (M109, M190) comes too soon after the macros
are done with the probe, it may result in the probe staying attached to
the toolhead until after the heater is done heating. This can be avoided
by placing a call to `DETACH_PROBE` after the probing commands, but before
the heater commands.

example:

Gcode file from slicer
```
start_print b=90 e=250
```

Macro definition in Klipper
```
[gcode_macro start_print]
gcode:
    M190 S{b}           ;set bed temp and wait
    G28                 ;home all axes
    BED_MESH_CALIBRATE  ;generate a bed mesh
    G28 Z0              ;rehome z axis
    DETACH_PROBE        ;ensure probe is detached
    M109 S{e}
```