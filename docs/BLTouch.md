Connecting BL-Touch
===================
Once you've hooked up the BL-Touch to a PWM enabled pin, yo need to configure these pins with Klipper:

    [bltouch]
    sensor_pin: P1.24
    #   Pin connected to the BL-Touch sensor pin. This parameter must be
    #   provided.
    control_pin: P1.26

Callibrating the BL-Touch
=========================

### Z Offset
In order to make klipper work properly you need to tell it in which relation to the nozzle
the probe is exactly located. Lets start with the `z_offset` - the height of the probe when
its expanded:

Make sure the probe is above the bed by setting an appropriate x/y position:

    g0 x0 y0
    
Now aproach the bed and make the BL-Touch lower the pin:

    g0 z5
    BLTOUCH_DEBUG COMMAND=pin_down

It should be able to freely do that, the bed shouldn't be touched yet. Now you need to slowly
lower the probe until it triggers using commands like `g0 z2.5`. Once the probe lights up red,
you've found the propper `z_offset` - its right above.

### X/Y Offset
In order to find the propper vertical offset of the probe you need to know a certain point of
your bed. Use a piece of paper you fix to the bed using duct tape. X/Y = 0 may be a good point
for this.
Create a mark on it by lowering the nozzle into it using `g0 z0.5` and heat it up. Either you
should have a small dot of filament on the paper, or see it being burnt a little. 

Now move the probe over that point i.e. using `g0 x15 y10` - once its exactly over the point,
this is your `x_offset` and `y_offset` to configure.

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

