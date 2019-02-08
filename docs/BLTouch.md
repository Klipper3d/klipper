Connecting BL-Touch
===================

A **warning** before you start: Avoid touching the BLTouch pin with your bare fingers, since it is
quite sensitive to finger grease. And if you do touch it, be very gentle, in order to not bend or
push anything.

Once you've hooked up the BL-Touch servo connector to a `control_pin` according to the BL-Touch
documentation or of your MCU. Using the original wiring, the yellow wire from the triple is
the `control_pin`, the white wire from the pair is the `sensor_pin`. You need to configure
these pins according to your wiring with Klipper:

```

    [bltouch]
    sensor_pin: P1.24
    #   Pin connected to the BL-Touch sensor pin. This parameter must be
    #   provided.
    control_pin: P1.26
```

You also most likely need to configure a homing override to first raise the z-axis, then home the x-
and y-axis, and finally move to the center and home the z-axis. Like this:

```
    [homing_override]
    gcode:
        G1 Z10 ; Move up 10mm
        G28 X Y
        G1 X166 Y120 F6000 ; Change the X and Y coordinates to the center of your print bed
        G28 Z
    set_position_z: 0.0
```

It's important that the upwards movement is high enough, so that the probe or the nozzle don't hit
any binder clips, even if the probe pin happens to be in it's lowest state.

Initial tests
=============

Before moving on, verify that the BLTouch is mounted at the correct height, the pin should be
roughly 2 mm above the nozzle when retracted

When you turn on the printer, the BLTouch probe should perform a self-test and move the pin up and
down a couple of times. Once the self-test is completed, the pin should be retracted and the red LED
on the probe should be lit. If there are any errors, for example the probe is flashing red or the
pin is down instead of up, please turn off the printer and check the wiring and configuration.

If the above is looking good, it's time to test that the probe responds to commands from the
firmware. First run `BLTOUCH_DEBUG COMMAND=pin_down` in your printer terminal. Verify that the pin
moves down, and that the red LED on the probe turns off. If not, check your wiring and configuration
again. Next issue a `BLTOUCH_DEBUG COMMAND=pin_up` and verify that the pin moves up, and that the
red light turns on again. If it's flashing then there's some problem.

Now, it's time to test homing with a twist. Instead of letting the probe pin touch the print bed,
let it touch the nail on your finger. So issue a `G28`, wait until it starts to move down, and stop
the movement by very gently touching the pin with your nail. You probably have to do it twice, since
the default configuration makes it probe twice. But be prepared to turn off the printer, to avoid
damage, if it doesn't stop when you touch the pin.

If that was successful, do another `G28` but this time let it touch the bed as it should.

Calibrating the BL-Touch
========================

### X/Y Offset

In order to make klipper work properly you need to tell it in which relation to the nozzle
the probe is exactly located. Lets start with the `x_offset` and the `y_offset`

In order to find the proper vertical offset of the probe you need to know a certain point of
your bed. X/Y = 0 may be a good point for this, any other will do as long as you know it.
Find it by lowering the nozzle next to it using `g0 z0.5`. Create a removable mark on your
bed by i.e. using a non permanent marker.

Now move the tip of the BL-Touch over that point by jogging there with the controls in the `control` tab of
OctoPrint. Once the BL-Touch is roughly over the point acquire it using the `GET_POSITION` command.
The difference to your marked point is your `x_offset` and `y_offset` to configure in the `printer.cfg`.


### Z Offset

1. We start by changing the `z_offset` in the `bltouch` section of the configuration to 10. This
setting is very wrong, but it will make it possible to move down to the right height using the
menus. Run `RESTART` after changing it to reload the configuration.

2. Now run `G28`, to home the printer. Take a note of the point where the pin hits the bed and use
the menu to move the nozzle to that point. Now take a *folded* paper and put it under the nozzle.
Move the nozzle down using the menu, in 0.1mm steps, until it grabs the paper. It doesn't matter how
much it grabs, you are searching for the higest position that grabs the paper. The nozzle should now
be between 0.1mm and 0.2mm above the printbed. If you wonder why we used a folded paper, the answer
is that the granularity of the movement is only 0.1mm, or the same as a normal paper thickness, that
means that you could easily move too far and hit the bed.

3. Now update the `z_offset` to reflect this, by substracting the current offset (10) by the reading
on the printer display. So if it says 8.2, then `10-8.2` = 1.8

4. Restart the printer, issue a `G28`, move to the pin location, then `G1 Z0`, and verify that it's
still grabbing the paper as it was before. If not, then repeat step 2. and 3. until you are
satisfied.

5. At this point it's a good idea to verify that the offset is close to 1mm, if not, then you
probably want to move the probe up or down to fix this. You want it to trigger well before the
nozzle hits the bed, so that possible stuck filament or a warped bed don't affect any probing
action. But at the same time, you want that the retracted position is as far above the nozzle as
possible, to avoid damage by touching printed parts.

6. Now it's time to fine tune with a real print. First make sure that your flow rate and steps are
calibrated properly, so that wrongly configured flow rates don't affect the calibration. Then slice
a one layer thick object, located close to where you are homing. I recommend a simple 50x50 mm
square.

7. While the object is printing, use the tune menu to adjust the "Offset Z", until it looks good.
With some practice, it's possible to find the exact point where it goes from underextrusion to
overextrusion, but some filaments are harder to see than others. Also notice that the tune command
does not have any effect immediately, it takes a couple of movements until the printer uses the new
value. It might also be hard to see exactly what's happening while it's printing, so you can adjust
for example every cm, and then find the best setting when the print is done.

8. Update the `z_offset` configuration again by using subtraction. So if the display shows that you
tuned the z-offset to -0.100, then it's calculated like this `1.8 -(-0.100) = 1.9`, notice the
double minus sign.

9. Issue a `RESTART`, and test printing the same thing, this time without adjusting anything.
Perform steps 7-9 until you are happy.

BL-Touch gone bad
=================
Once the BL-Touch is in inconsistent state, it starts blinking red. You can force it to leave that
state by issuing:

 BLTOUCH_DEBUG COMMAND=reset

This may happen if its calibration is interrupted by the probe being blocked from being extracted.

However, the BL-Touch may also not be able to calibrate itself anymore. This happenes if the
screw on its top is in the wrong position, or the magnetic core inside the probe pin has moved.
If its moved that way up that it sticks to the screw, it may not be able to lower its pin anymore.
With this behaviour you need to open the screw, pick i.e. a ball-pen and push it gently back in place.
Re-Insert the pin into the BL-Touch so that it falls into extracted position. Carefully readjust the
headless screw into place. You need to find the right position so it is able to lower and raise the
pin, and the red light turns on and of. Use the `reset`, `pin_up` and `pin_down` commands to
achieve this.
