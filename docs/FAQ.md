Frequently asked questions
==========================

### How do I calculate the step_distance parameter in the printer config file?

If you know the steps per millimeter for the axis then use a
calculator to divide 1.0 by steps_per_mm. Then round this number to
six decimal places and place it in the config (six decimal places is
nano-meter precision).

The step_distance defines the distance that the axis will travel on
each motor driver pulse. It can also be calculated from the axis
pitch, motor step angle, and driver microstepping. If unsure, do a web
search for "calculate steps per mm" to find an online calculator.

### When I restart my micro-controller the device changes to /dev/ttyACM1

If the device sometimes changes from /dev/ttyACM0 to /dev/ttyACM1 then
ssh into the host computer and run: ```ls -l /dev/serial/by-id/```

The name found in the above command is stable and it is possible to
use it in the config file. For example, an updated config might look
like:
```
[mcu]
serial: /dev/serial/by-id/usb-UltiMachine__ultimachine.com__RAMBo_12345678912345678912-if00
```

Be sure to copy-and-paste the name from the "ls" command above as the
name will be different on each printer.

### The "make flash" command doesn't work.

The code attempts to flash the device using the most common method for
each platform. Unfortunately, there is a lot of variance in flashing
methods, so the "make flash" command may not work on all boards.

If you're having an intermittent failure or you do have a standard
setup, then double check that Klipper isn't running when flashing
(sudo service klipper stop), make sure Octoprint isn't trying to
connect directly to the device (open the Connection tab in the web
page and click Disconnect if the Serial Port is set to the device),
and make sure FLASH_DEVICE is set correctly for your board (see the
[question above](#when-i-restart-my-micro-controller-the-device-changes-to-devttyacm1)).

However, if "make flash" just doesn't work for your board, then you
will need to manually flash. See if there is a config file in the
[config directory](../config) with specific instructions for flashing
the device. Also, check the board manufacturer's documentation to see
if it describes how to flash the device. Finally, on AVR devices, it
may be possible to manually flash the device using
[avrdude](http://www.nongnu.org/avrdude/) with custom command-line
parameters - see the avrdude documentation for further information.

### Can I run Klipper on something other than a Raspberry Pi?

Klipper only requires Python running on a Linux (or similar)
computer. However, if you wish to run it on a different machine you
will need Linux admin knowledge to install the system prerequisites
for the Linux distribution running on that particular machine. See the
[install-octopi.sh](../scripts/install-octopi.sh) script for further
information on the necessary Linux admin steps.

### Why can't I move the stepper before homing the printer?

The code does this to reduce the chance of accidentally commanding the
head into the bed or a wall. Once the printer is homed the software
attempts to verify each move is within the position_min/max defined in
the config file. If the motors are disabled (via an M84 or M18
command) then the motors will need to be homed again prior to
movement.

If you want to move the head after canceling a print via Octoprint,
consider changing the Octoprint cancel sequence to do that for
you. It's configured in Octoprint via a web browser under:
Settings->GCODE Scripts

If you want to move the head after a print finishes, consider adding
the desired movement to the "custom g-code" section of your slicer.

### Why is the Z position_endstop set to 0.5 in the default configs?

For cartesian style printers the Z position_endstop specifies how far
the nozzle is from the bed when the endstop triggers. If possible, it
is recommended to use a Z-max endstop and home away from the bed (as
this reduces the potential for bed collisions). However, if one must
home towards the bed then it is recommended to position the endstop so
it triggers when the nozzle is still a small distance away from the
bed. This way, when homing the axis, it will stop before the nozzle
touches the bed.

Almost all mechanical switches can still move a small distance
(eg, 0.5mm) after they are triggered. So, for example, if the
position_endstop is set to 0.5mm then one may still command the
printer to move to Z0.2. The position_min config setting (which
defaults to 0) is used to specify the minimum Z position one may
command the printer to move to.

Note, the Z position_endstop specifies the distance from the nozzle to
the bed when the nozzle and bed (if applicable) are hot. It is typical
for thermal expansion to cause nozzle expansion of around .1mm, which
is also the typical thickness of a sheet of printer paper. Thus, it is
common to use the "paper test" to confirm calibration of the Z
height - check that the bed and nozzle are at room temperature, check
that there is no plastic on the head or bed, home the printer, place a
piece of paper between the nozzle and bed, and repeatedly command the
head to move closer to the bed checking each time if you feel a small
amount of friction when sliding the paper between bed and nozzle - if
all is calibrated well a small amount of friction would be felt when
the height is at Z0.

### I converted my config from Marlin and the X/Y axes work fine, but I just get a screeching noise when homing the Z axis

Short answer: Try reducing the max_z_velocity setting in the printer
config. Also, if the Z stepper is moving in the wrong direction, try
inverting the dir_pin setting in the config (eg, "dir_pin: !xyz"
instead of "dir_pin: xyz").

Long answer: In practice Marlin can typically only step at a rate of
around 10000 steps per second. If it is requested to move at a speed
that would require a higher step rate then Marlin will generally just
step as fast as it can. Klipper is able to achieve much higher step
rates, but the stepper motor may not have sufficient torque to move at
a higher speed. So, for a Z axis with a very precise step_distance the
actual obtainable max_z_velocity may be smaller than what is
configured in Marlin.

### When I set "restart_method=command" my AVR device just hangs on a restart

Some old versions of the AVR bootloader have a known bug in watchdog
event handling. This typically manifests when the printer.cfg file has
restart_method set to "command". When the bug occurs, the AVR device
will be unresponsive until power is removed and reapplied to the
device (the power or status LEDs may also blink repeatedly until the
power is removed).

The workaround is to use a restart_method other than "command" or to
flash an updated bootloader to the AVR devices. Flashing a new
bootloader is a one time step that typically requires an external
programmer - search the web to find the instructions for your
particular device.
