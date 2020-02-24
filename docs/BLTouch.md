Connecting BL-Touch
===================

A **warning** before you start: Avoid touching the BL-Touch pin with
your bare fingers, since it is quite sensitive to finger grease. And
if you do touch it, be very gentle, in order to not bend or push
anything.

Hook up the BL-Touch "servo" connector to a `control_pin` according to
the BL-Touch documentation or your MCU documentation. Using the
original wiring, the yellow wire from the triple is the `control_pin`
and the white wire from the pair is the `sensor_pin`. You need to
configure these pins according to your wiring. For example:

```
[bltouch]
sensor_pin: P1.24
control_pin: P1.26
```

If the BL-Touch will be used to home the Z axis then set `endstop_pin:
probe:z_virtual_endstop` in the `[stepper_z]` config section and add a
`[safe_z_home]` config section to raise the z axis, home the xy axes,
move to the center of the bed, and home the z axis. For example:

```
[safe_z_home]
home_xy_position: 100,100 # Change coordinates to the center of your print bed
speed: 50
z_hop: 10                 # Move up 10mm
z_hop_speed: 5
```

It's important that the z_hop movement in safe_z_home is high enough
that the probe doesn't hit anything even if the probe pin happens to
be in its lowest state.

Initial tests
=============

Before moving on, verify that the BL-Touch is mounted at the correct
height, the pin should be roughly 2 mm above the nozzle when retracted

When you turn on the printer, the BL-Touch probe should perform a
self-test and move the pin up and down a couple of times. Once the
self-test is completed, the pin should be retracted and the red LED on
the probe should be lit. If there are any errors, for example the
probe is flashing red or the pin is down instead of up, please turn
off the printer and check the wiring and configuration.

If the above is looking good, it's time to test that the probe
responds to commands from the firmware. First run `BLTOUCH_DEBUG
COMMAND=pin_down` in your printer terminal. Verify that the pin moves
down, and that the red LED on the probe turns off. If not, check your
wiring and configuration again. Next issue a `BLTOUCH_DEBUG
COMMAND=pin_up`, verify that the pin moves up, and that the red light
turns on again. If it's flashing then there's some problem.

Now, it's time to test homing with a twist. Instead of letting the
probe pin touch the print bed, let it touch the nail on your
finger. So issue a `G28`, wait until it starts to move down, and stop
the movement by very gently touching the pin with your nail. You
probably have to do it twice, since the default configuration makes it
probe twice. But be prepared to turn off the printer, to avoid damage,
if it doesn't stop when you touch the pin.

If that was successful, do another `G28` but this time let it touch
the bed as it should.

Calibrating the BL-Touch offsets
================================

Follow the directions in the [Probe Calibrate](Probe_Calibrate.md)
guide to set the x_offset, y_offset, and z_offset config parameters.

It's a good idea to verify that the Z offset is close to 1mm. If not,
then you probably want to move the probe up or down to fix this. You
want it to trigger well before the nozzle hits the bed, so that
possible stuck filament or a warped bed doesn't affect any probing
action. But at the same time, you want the retracted position to be as
far above the nozzle as possible to avoid it touching printed parts.
If an adjustment is made to the probe position, then rerun the probe
calibration steps.

BL-Touch gone bad
=================

Once the BL-Touch is in inconsistent state, it starts blinking
red. You can force it to leave that state by issuing:

 BLTOUCH_DEBUG COMMAND=reset

This may happen if its calibration is interrupted by the probe being
blocked from being extracted.

However, the BL-Touch may also not be able to calibrate itself
anymore. This happens if the screw on its top is in the wrong position
or the magnetic core inside the probe pin has moved. If it has moved
up so that it sticks to the screw, it may not be able to lower its pin
anymore. With this behavior you need to open the screw and use a
ball-point pen to push it gently back into place. Re-Insert the pin
into the BL-Touch so that it falls into the extracted
position. Carefully readjust the headless screw into place. You need
to find the right position so it is able to lower and raise the pin
and the red light turns on and of. Use the `reset`, `pin_up` and
`pin_down` commands to achieve this.

BL-Touch output mode
====================

* A BL-Touch V3.0 supports setting a 5V or OPEN-DRAIN output mode,
  a BL-Touch V3.1 supports this too, but can also store this in its
  internal EEPROM. If your controller board needs the fixed 5V high
  logic level of the 5V mode you may set the 'set_output_mode'
  parameter in the [bltouch] section of the printer config file to
  "5V".

  *** Only use the 5V mode if your controller boards input line is
  5V tolerant. This is why the default configuration of these BL-Touch
  versions is OPEN-DRAIN mode. You could potentially damage your
  controller boards CPU ***

  So therefore:
  If a controller board NEEDs 5V mode AND it is 5V tolerant on its
  input signal line AND if

  - you have a BL-Touch Smart V3.0, you need the use 'set_output_mode: 5V'
    parameter to ensure this setting at each startup, since the probe
    cannot remember the needed setting.
  - you have a BL-Touch Smart V3.1, you have the choice of using
    'set_output_mode: 5V' or storing the mode once by use of a
    'BLTOUCH_STORE MODE=5V' command manually and NOT using the parameter
    'set_output_mode:'.
  - you have some other probe: Some probes have a trace on the circuit board
    to cut or a jumper to set in order to (permanently) set the output mode.
    In that case, omit the 'set_output_mode' parameter completely.

  If you have a V3.1, do not automate or repeat storing the output mode to
  avoid wearing out the EEPROM of the probe.The BLTouch EEPROM is good for
  about 100.000 updates. 100 stores per day would add up to about 3 years
  of operation prior to wearing it out. Thus, storing the output mode in a
  V3.1 is designed by the vendor to be a complicated operation (the factory
  default being a safe OPEN DRAIN mode) and is not suited to be repeatedly
  issued by any slicer, macro or anything else, it is preferably only to be
  used when first integrating the probe into a printers electronics.


Troubleshooting
===============

* If you are sure the wiring of the BL-Touch is correct and every
  attempt to probe with the BL-Touch reports "BLTouch failed to verify
  sensor state" then it may be necessary to add
  `pin_up_touch_mode_reports_triggered: False` to the bltouch config
  section. The BL-Touch v3 and many clones require this setting.

* A BL-Touch v3 may not work correctly when its signal wire is
  connected to the Z end-stop pin on some printer boards. The symptoms
  of this problem are: the BL-Touch probe deploys, the printer
  descends, the probe contacts a surface, the BL-Touch raises the
  probe, the BL-Touch does not successfully notify the
  micro-controller, and the printer continues to descend. The Z
  end-stop pin on some printer boards have a capacitor to filter the
  signal which the BL-Touch v3 may not support. The simplest solution
  is to connect the BL-Touch v3 sensor wire to an available pin on the
  printer board that is not associated with an end-stop (and thus is
  unlikely to have a capacitor). An alternative solution is to
  physically alter the printer board to disable the given end-stop
  capacitor or to add a hardware "pull up resistor" to the BL-Touch v3
  sensor wire.
