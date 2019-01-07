Connecting BL-Touch
===================
Once you've hooked up the BL-Touch servo connector to a `control_pin` according to the BL-Touch
documentation or of your MCU. Using the original wiring, the yellow wire from the tripple is
the `control_pin`, the white wire from the pair is the `sensor_pin`. You need to configure
these pins according to your wiring with Klipper:

    [bltouch]
    sensor_pin: P1.24
    #   Pin connected to the BL-Touch sensor pin. This parameter must be
    #   provided.
    control_pin: P1.26

Calibrating the BL-Touch
========================

### Z Offset
In order to make klipper work properly you need to tell it in which relation to the nozzle
the probe is exactly located. Lets start with the `z_offset` - the height of the probe when
its expanded:

Make sure the probe is above the bed by moving the head into an appropriate x/y position:

    g0 x0 y0

Now make klipper aproach the bed until the probe triggers using:

    PROBE

This should deploy the BL-Touch probe, stop the run when it triggers by touching the bed.
Now acquire the current position from the reached `z_offset` using:

    GET_POSITION

In the output lines the `toolhead` line with the `Z:` contains the required value.

### X/Y Offset
In order to find the propper vertical offset of the probe you need to know a certain point of
your bed. X/Y = 0 may be a good point for this, any other will do as long as you know it.
Find it by lowering the nozzle next to it using `g0 z0.5`. Create a removeable mark on your
bed by i.e. using a non permanent marker.

Now move the tip of the BL-Touch over that point by jogging there with the conrols in the `control` tab of
OctoPrint. Once the BL-Touch is roughly over the point acquire it using the `GET_POSITION` command.
The difference to your marked point is your `x_offset` and `y_offset` to configure in the `printer.cfg`.


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
