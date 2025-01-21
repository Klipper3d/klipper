# BL-Touch

## Connecting BL-Touch

A **warning** before you start: Avoid touching the BL-Touch pin with
your bare fingers, since it is quite sensitive to finger grease. And
if you do touch it, be very gentle, in order to not bend or push
anything.

Hook up the BL-Touch "servo" connector to a `control_pin` according to
the BL-Touch documentation or your MCU documentation. Using the
original wiring, the yellow wire from the triple is the `control_pin`
and the white wire from the pair is the `sensor_pin`. You need to
configure these pins according to your wiring. Most BL-Touch devices
require a pullup on the sensor pin (prefix the pin name with "^"). For
example:

```
[bltouch]
sensor_pin: ^P1.24
control_pin: P1.26
```

If the BL-Touch will be used to home the Z axis then set `endstop_pin:
probe:z_virtual_endstop` and remove `position_endstop` in the `[stepper_z]` config section,
then add a `[safe_z_home]` config section to raise the z axis, home the xy axes,
move to the center of the bed, and home the z axis. For example:

```
[safe_z_home]
home_xy_position: 100, 100 # Change coordinates to the center of your print bed
speed: 50
z_hop: 10                 # Move up 10mm
z_hop_speed: 5
```

It's important that the z_hop movement in safe_z_home is high enough
that the probe doesn't hit anything even if the probe pin happens to
be in its lowest state.

## Initial tests

Before moving on, verify that the BL-Touch is mounted at the correct
height, the pin should be roughly 2 mm above the nozzle when retracted

When you turn on the printer, the BL-Touch probe should perform a
self-test and move the pin up and down a couple of times. Once the
self-test is completed, the pin should be retracted and the red LED on
the probe should be lit. If there are any errors, for example the
probe is flashing red or the pin is down instead of up, please turn
off the printer and check the wiring and configuration.

If the above is looking good, it's time to test that the control pin
is working correctly. First run `BLTOUCH_DEBUG COMMAND=pin_down` in
your printer terminal. Verify that the pin moves down and that the red
LED on the probe turns off. If not, check your wiring and
configuration again. Next issue a `BLTOUCH_DEBUG COMMAND=pin_up`,
verify that the pin moves up, and that the red light turns on
again. If it's flashing then there's some problem.

The next step is to confirm that the sensor pin is working correctly.
Run `BLTOUCH_DEBUG COMMAND=pin_down`, verify that the pin moves down,
run `BLTOUCH_DEBUG COMMAND=touch_mode`, run `QUERY_PROBE`, and verify
that command reports "probe: open". Then while gently pushing the pin
up slightly with the nail of your finger run `QUERY_PROBE` again.
Verify the command reports "probe: TRIGGERED". If either query does
not report the correct message then it usually indicates an incorrect
wiring or configuration (though some [clones](#bl-touch-clones) may
require special handling). At the completion of this test run
`BLTOUCH_DEBUG COMMAND=pin_up` and verify that the pin moves up.

After completing the BL-Touch control pin and sensor pin tests, it is
now time to test probing, but with a twist. Instead of letting the
probe pin touch the print bed, let it touch the nail on your finger.
Position the toolhead far from the bed, issue a `G28` (or `PROBE` if
not using probe:z_virtual_endstop), wait until the toolhead starts to
move down, and stop the movement by very gently touching the pin with
your nail. You may have to do it twice, since the default homing
configuration probes twice. Be prepared to turn off the printer if it
doesn't stop when you touch the pin.

If that was successful, do another `G28` (or `PROBE`) but this time
let it touch the bed as it should.

## BL-Touch gone bad

Once the BL-Touch is in inconsistent state, it starts blinking red.
You can force it to leave that state by issuing:

 BLTOUCH_DEBUG COMMAND=reset

This may happen if its calibration is interrupted by the probe being
blocked from being extracted.

However, the BL-Touch may also not be able to calibrate itself
anymore. This happens if the screw on its top is in the wrong position
or the magnetic core inside the probe pin has moved. If it has moved
up so that it sticks to the screw, it may not be able to lower its pin
anymore. With this behavior you need to open the screw and use a
ball-point pen to push it gently back into place. Re-Insert the pin
into the BL-Touch so that it falls into the extracted position.
Carefully readjust the headless screw into place. You need to find the
right position so it is able to lower and raise the pin and the red
light turns on and of. Use the `reset`, `pin_up` and `pin_down`
commands to achieve this.

## BL-Touch "clones"

Many BL-Touch "clone" devices work correctly with Klipper using the
default configuration. However, some "clone" devices may not support
the `QUERY_PROBE` command and some "clone" devices may require
configuration of `pin_up_reports_not_triggered` or
`pin_up_touch_mode_reports_triggered`.

Important! Do not configure `pin_up_reports_not_triggered` or
`pin_up_touch_mode_reports_triggered` to False without first following
these directions. Do not configure either of these to False on a
genuine BL-Touch. Incorrectly setting these to False can increase
probing time and can increase the risk of damaging the printer.

Some "clone" devices do not support `touch_mode` and as a result the
`QUERY_PROBE` command does not work. Despite this, it may still be
possible to perform probing and homing with these devices. On these
devices the `QUERY_PROBE` command during the
[initial tests](#initial-tests) will not succeed, however the
subsequent `G28` (or `PROBE`) test does succeed. It may be possible to
use these "clone" devices with Klipper if one does not utilize the
`QUERY_PROBE` command and one does not enable the
`probe_with_touch_mode` feature.

Some "clone" devices are unable to perform Klipper's internal sensor
verification test. On these devices, attempts to home or probe can
result in Klipper reporting a "BLTouch failed to verify sensor state"
error. If this occurs, then manually run the steps to confirm the
sensor pin is working as described in the
[initial tests section](#initial-tests). If the `QUERY_PROBE` commands
in that test always produce the expected results and "BLTouch failed
to verify sensor state" errors still occur, then it may be necessary
to set `pin_up_touch_mode_reports_triggered` to False in the Klipper
config file.

A rare number of old "clone" devices are unable to report when they
have successfully raised their probe. On these devices Klipper will
report a "BLTouch failed to raise probe" error after every home or
probe attempt. One can test for these devices - move the head far from
the bed, run `BLTOUCH_DEBUG COMMAND=pin_down`, verify the pin has
moved down, run `QUERY_PROBE`, verify that command reports "probe:
open", run `BLTOUCH_DEBUG COMMAND=pin_up`, verify the pin has moved
up, and run `QUERY_PROBE`. If the pin remains up, the device does not
enter an error state, and the first query reports "probe: open" while
the second query reports "probe: TRIGGERED" then it indicates that
`pin_up_reports_not_triggered` should be set to False in the Klipper
config file.

## BL-Touch v3

Some BL-Touch v3.0 and BL-Touch 3.1 devices may require configuring
`probe_with_touch_mode` in the printer config file.

If the BL-Touch v3.0 has its signal wire connected to an endstop pin
(with a noise filtering capacitor), then the BL-Touch v3.0 may not be
able to consistently send a signal during homing and probing. If the
`QUERY_PROBE` commands in the [initial tests section](#initial-tests)
always produce the expected results, but the toolhead does not always
stop during G28/PROBE commands, then it is indicative of this issue. A
workaround is to set `probe_with_touch_mode: True` in the config file.

The BL-Touch v3.1 may incorrectly enter an error state after a
successful probe attempt. The symptoms are an occasional flashing
light on the BL-Touch v3.1 that lasts for a couple of seconds after it
successfully contacts the bed. Klipper should clear this error
automatically and it is generally harmless. However, one may set
`probe_with_touch_mode` in the config file to avoid this issue.

Important! Some "clone" devices and the BL-Touch v2.0 (and earlier)
may have reduced accuracy when `probe_with_touch_mode` is set to True.
Setting this to True also increases the time it takes to deploy the
probe. If configuring this value on a "clone" or older BL-Touch
device, be sure to test the probe accuracy before and after setting
this value (use the `PROBE_ACCURACY` command to test).

## Multi-probing without stowing

By default, Klipper will deploy the probe at the start of each probe
attempt and then stow the probe afterwards. This repetitive deploying
and stowing of the probe may increase the total time of calibration
sequences that involve many probe measurements. Klipper supports
leaving the probe deployed between consecutive probes, which can
reduce the total time of probing. This mode is enabled by configuring
`stow_on_each_sample` to False in the config file.

Important! Setting `stow_on_each_sample` to False can lead to Klipper
making horizontal toolhead movements while the probe is deployed. Be
sure to verify all probing operations have sufficient Z clearance
prior to setting this value to False. If there is insufficient
clearance then a horizontal move may cause the pin to catch on an
obstruction and result in damage to the printer.

Important! It is recommended to use `probe_with_touch_mode` configured
to True when using `stow_on_each_sample` configured to False. Some
"clone" devices may not detect a subsequent bed contact if
`probe_with_touch_mode` is not set. On all devices, using the
combination of these two settings simplifies the device signaling,
which can improve overall stability.

Note, however, that some "clone" devices and the BL-Touch v2.0 (and
earlier) may have reduced accuracy when `probe_with_touch_mode` is set
to True. On these devices it is a good idea to test the probe accuracy
before and after setting `probe_with_touch_mode` (use the
`PROBE_ACCURACY` command to test).

## Calibrating the BL-Touch offsets

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

## BL-Touch output mode

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
