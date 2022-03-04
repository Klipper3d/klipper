# Slicers

This document provides some tips for configuring a "slicer"
application for use with Klipper. Common slicers used with Klipper are
Slic3r, Cura, Simplify3D, etc.

## Set the G-Code flavor to Marlin

Many slicers have an option to configure the "G-Code flavor". The
default is frequently "Marlin" and that works well with Klipper. The
"Smoothieware" setting also works well with Klipper.

## Klipper gcode_macro

Slicers will often allow one to configure "Start G-Code" and "End
G-Code" sequences. It is often convenient to define custom macros in
the Klipper config file instead - such as: `[gcode_macro START_PRINT]`
and `[gcode_macro END_PRINT]`. Then one can just run START_PRINT and
END_PRINT in the slicer's configuration. Defining these actions in the
Klipper configuration may make it easier to tweak the printer's start
and end steps as changes do not require re-slicing.

See [sample-macros.cfg](../config/sample-macros.cfg) for example
START_PRINT and END_PRINT macros.

See the [config reference](Config_Reference.md#gcode_macro) for
details on defining a gcode_macro.

## Large retraction settings may require tuning Klipper

The maximum speed and acceleration of retraction moves are controlled
in Klipper by the `max_extrude_only_velocity` and
`max_extrude_only_accel` config settings. These settings have a
default value that should work well on many printers. However, if one
has configured a large retraction in the slicer (eg, 5mm or greater)
then one may find they limit the desired speed of retractions.

If using a large retraction, consider tuning Klipper's
[pressure advance](Pressure_Advance.md) instead. Otherwise, if one
finds the toolhead seems to "pause" during retraction and priming,
then consider explicitly defining `max_extrude_only_velocity` and
`max_extrude_only_accel` in the Klipper config file.

## Do not enable "coasting"

The "coasting" feature is likely to result in poor quality prints with
Klipper. Consider using Klipper's
[pressure advance](Pressure_Advance.md) instead.

Specifically, if the slicer dramatically changes the extrusion rate
between moves then Klipper will perform deceleration and acceleration
between moves. This is likely to make blobbing worse, not better.

In contrast, it is okay (and often helpful) to use a slicer's
"retract" setting, "wipe" setting, and/or "wipe on retract" setting.

## Do not use "extra restart distance" on Simplify3d

This setting can cause dramatic changes to extrusion rates which can
trigger Klipper's maximum extrusion cross-section check. Consider
using Klipper's [pressure advance](Pressure_Advance.md) or the regular
Simplify3d retract setting instead.

## Disable "PreloadVE" on KISSlicer

If using KISSlicer slicing software then set "PreloadVE" to
zero. Consider using Klipper's [pressure advance](Pressure_Advance.md)
instead.

## Disable any "advanced extruder pressure" settings

Some slicers advertise an "advanced extruder pressure" capability. It
is recommended to keep these options disabled when using Klipper as
they are likely to result in poor quality prints. Consider using
Klipper's [pressure advance](Pressure_Advance.md) instead.

Specifically, these slicer settings can instruct the firmware to make
wild changes to the extrusion rate in the hope that the firmware will
approximate those requests and the printer will roughly obtain a
desirable extruder pressure. Klipper, however, utilizes precise
kinematic calculations and timing. When Klipper is commanded to make
significant changes to the extrusion rate it will plan out the
corresponding changes to velocity, acceleration, and extruder
movement - which is not the slicer's intent. The slicer may even
command excessive extrusion rates to the point that it triggers
Klipper's maximum extrusion cross-section check.

In contrast, it is okay (and often helpful) to use a slicer's
"retract" setting, "wipe" setting, and/or "wipe on retract" setting.


## Lower resolution if you have small blobs/zits on curved/round walls

Especially when printing at higher speeds, one may notice blobs/zits
on curved/round surfaces. These can occur when the slicer creates a
lot of small lines to over-accurately reproduce an arc, possibly
with alternating extrusion amounts. These tiny G-Code lines can cause
klipper to slow down slightly, causing these mini-blobs. You can tell
these blobs apart from other issues by the fact that these are
reproducible on the same spots if you print the same model twice.
If you print a 3d benchy, you may see them on the front hull lines.

In Cura, these settings can be found under
"Mesh Fixes" -> "Maximum Resolution", "Maximum Travel Resolution"
and "Maximum Devaiation".

Another solution can be using the Arc-Welder Plugin to post-process
linear moves into G02/G03 moves and let klipper convert them back.
Klipper needs to be configured to accept these G02/G03 commands by
adding a new config section [gcode_arcs](Config_Reference.md#gcode_arcs).
Please note that this aproach may introduce other problems and
inaccuicies, so setting the slicer to work at a reasonable accuracy
level should be prefered.
