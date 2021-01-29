This document describes Filament Motion Sensor host module. Hardware used for developing this host module is based on a rotating encoder that toggles the signal pin as filament moves through the sensor (BTT Smart Filament Sensor for example).

## How does it work?
Sensor returns either a high signal or a low signal depending on the position of the encoder wheel. The extruder pulls the filament through the sensor which rotates the encoder wheel resulting in the continuous toggling of the signal pin. Due to this toggling, the sensor is unable to determine if filament is present when the extruder is not operating.  The length of filament required to rotate the encoder wheel enough to toggle the signal is constant and must be specified in order for this module to determine if the filament is moving and not jammed or runout.

## Configuration

    [filament_motion_sensor my_sensor]

    #pause_on_runout: True
    #   When set to True, a PAUSE will execute immediately after a runout
    #   or jam is detected. Note that if pause_on_runout is False and the
    #   [pause_resume] section is omitted then runout detection is disabled.
    #   Default is True.
    #runout_gcode:
    #   A list of G-Code commands to execute after a filament runout or jam
    #   is detected. See docs/Command_Templates.md for G-Code format. If
    #   pause_on_runout is set to True this G-Code will run after the
    #   PAUSE is complete. The default is not to run any G-Code commands.
    #   runout_gcode can be used with or without pause_on_runout.  If 
    #   pause_on_runout is True then runout_gcode can include moves to put the
    #   extruder in a convenient spot for loading new filament or fixing a jam.
    #   If pause_on_runout is False then the printer will continue moving and
    #   printing when a runout/jam is detected, so it is not recommended to put
    #   moves in the runout_gcode in this case.
    #insert_gcode:
    #   A list of G-Code commands to execute after a filament insert is
    #   detected. See docs/Command_Templates.md for G-Code format. The
    #   default is not to run any G-Code commands, which disables insert
    #   detection.
    #pause_delay: 0.5
    #   The amount of time to delay, in seconds, between the pause command
    #   dispatch and execution of the runout_gcode. It may be useful to
    #   increase this delay if OctoPrint exhibits strange pause behavior.
    #   Default is 0.5 seconds.
    switch_pin:
    #   The pin on which the switch is connected. This parameter must be
    #   provided.
    detection_length: 7.0
    #   The minimum length of filament pulled through the sensor to trigger
    #   a state change on the switch_pin
    #   Default is 7 mm.
    extruder:
    #   The name of the extruder section this sensor is associated with.
    #   This parameter must be provided.
    
### pause_resume
If pause_on_runout is True then you must add the following to printer.cfg:
   [pause_resume]

A note on pause_resume, due to the nature of moves being queued, pause can
only stop future moves from being queued.  Previously queued moves will
continue, so there must be at least
max_extrude_only_distance+detection_length of filament between the filament
motion sensor and the extruder so queued moves can be completed after the
runout/jam is detected and pause is commanded.

#### Printer LCD Menu
If pause_resume is enabled, its also beneficial to use the pause_resume
g-code in the menu so the printer LCD functions correctly for pause and
resume by adding the following to printer.cfg:
[menu __main __octoprint __pause]
type: command
enable: {printer.idle_timeout.state == "Printing"}
name: Pause printing
gcode: PAUSE

[menu __main __octoprint __resume]
type: command
enable: {not printer.idle_timeout.state == "Printing"}
name: Resume printing
gcode: RESUME

[menu __main __octoprint __abort]
type: command
enable: {printer.idle_timeout.state == "Printing"}
name: Abort printing
gcode: CANCEL_PRINT

#### OctoPrint
If pause_resume is enabled, its also beneficial to use the pause_resume g-code
in OctoPrint. Go to Settings, GCODE Scripts, and add the following (can be used
in conjunction with existing GCODE, but put it first):
- After print job is cancelled: CANCEL_PRINT
- After print job is paused: PAUSE
- Before print job is resumed: RESUME

This will ensure printer state is properly saved and restored on pause/resume,
and allows the print head to be safely moved around for filament change, etc
during pause, and for printing to correctly continue on resume.

## Commands
**QUERY_FILAMENT_MOTION_SENSOR** - Return the current state of the sensor (toggles 0/1)

**SET_FILAMENT_MOTION_SENSOR** - Enable/disable and set detection length of the sensor

## Menu variables

## Template for menu variables

## Calibration procedure
The manufacturer of the sensor should specify a detection length
(sensor accuracy).  This is the length of the material that must pass through
the sensor before it detects movement.

To check this, manually pull filament through the sensor about 1 mm at a time 
and run QUERY_FILAMENT_MOTION_SENSOR until its state changes, then continue
this process until it changes state again, measure the filament length pulled
through and set this as the detection_length in printer.cfg.  If the state
doesn't change, check the switch_pin is correct in printer.cfg otherwise
check wiring, etc.

While printing if getting false runout/jam detections, then increase the
detection_length.

## How to enable sensor
After power on by default sensor is enabled.
Disable sensor in start g-code by command **SET_FILAMENT_MOTION_SENSOR** or change enable parameter in printer.cfg

## Logging
Basic logging is available
