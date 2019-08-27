Frequently asked questions
==========================

1. [How can I donate to the project?](#how-can-i-donate-to-the-project)
2. [How do I calculate the step_distance parameter in the printer config file?](#how-do-i-calculate-the-step_distance-parameter-in-the-printer-config-file)
3. [Where's my serial port?](#wheres-my-serial-port)
4. [When the micro-controller restarts the device changes to /dev/ttyUSB1](#when-the-micro-controller-restarts-the-device-changes-to-devttyusb1)
5. [The "make flash" command doesn't work](#the-make-flash-command-doesnt-work)
6. [How do I change the serial baud rate?](#how-do-i-change-the-serial-baud-rate)
7. [Can I run Klipper on something other than a Raspberry Pi 3?](#can-i-run-klipper-on-something-other-than-a-raspberry-pi-3)
8. [Can I run multiple instances of Klipper on the same host machine?](#can-i-run-multiple-instances-of-klipper-on-the-same-host-machine)
9. [Do I have to use OctoPrint?](#do-i-have-to-use-octoprint)
10. [Why can't I move the stepper before homing the printer?](#why-cant-i-move-the-stepper-before-homing-the-printer)
11. [Why is the Z position_endstop set to 0.5 in the default configs?](#why-is-the-z-position_endstop-set-to-05-in-the-default-configs)
12. [I converted my config from Marlin and the X/Y axes work fine, but I just get a screeching noise when homing the Z axis](#i-converted-my-config-from-marlin-and-the-xy-axes-work-fine-but-i-just-get-a-screeching-noise-when-homing-the-z-axis)
13. [My TMC motor driver turns off in the middle of a print](#my-tmc-motor-driver-turns-off-in-the-middle-of-a-print)
14. [I keep getting random "Lost communication with MCU" errors](#i-keep-getting-random-lost-communication-with-mcu-errors)
15. [My Raspberry Pi keeps rebooting during prints](#my-raspberry-pi-keeps-rebooting-during-prints)
16. [When I set "restart_method=command" my AVR device just hangs on a restart](#when-i-set-restart_methodcommand-my-avr-device-just-hangs-on-a-restart)
17. [Will the heaters be left on if the Raspberry Pi crashes?](#will-the-heaters-be-left-on-if-the-raspberry-pi-crashes)
18. [How do I convert a Marlin pin number to a Klipper pin name?](#how-do-i-convert-a-marlin-pin-number-to-a-klipper-pin-name)
19. [How do I cancel an M109/M190 "wait for temperature" request?](#how-do-i-cancel-an-m109m190-wait-for-temperature-request)
20. [How do I upgrade to the latest software?](#how-do-i-upgrade-to-the-latest-software)
21. [Can I find out whether the printer has lost steps?](#can-i-find-out-whether-the-printer-has-lost-steps)

### How can I donate to the project?

Thanks. Kevin has a Patreon page at: https://www.patreon.com/koconnor

### How do I calculate the step_distance parameter in the printer config file?

If you know the steps per millimeter for the axis then use a
calculator to divide 1.0 by steps_per_mm. Then round this number to
six decimal places and place it in the config (six decimal places is
nano-meter precision).

The step_distance defines the distance that the axis will travel on
each motor driver pulse. It can also be calculated from the axis
pitch, motor step angle, and driver microstepping. If unsure, do a web
search for "calculate steps per mm" to find an online calculator.

Klipper uses step_distance instead of steps_per_mm in order to use
consistent units of measurement in the config file. (The config uses
millimeters for all distance measurements.) It is believed that
steps_per_mm originated as an optimization on old 8-bit
micro-controllers (the desire to use a multiply instead of a divide in
some low-level code). Continuing to configure this one distance in
units of "inverse millimeters" is felt to be quirky and unnecessary.

### Where's my serial port?

The general way to find a USB serial port is to run `ls -l
/dev/serial/by-id/` from an ssh terminal on the host machine. It will
likely produce output similar to the following:
```
lrwxrwxrwx 1 root root 13 Jun  1 21:12 usb-1a86_USB2.0-Serial-if00-port0 -> ../../ttyUSB0
```

The name found in the above command is stable and it is possible to
use it in the config file and while flashing the micro-controller
code. For example, a flash command might look similar to:
```
sudo service klipper stop
make flash FLASH_DEVICE=/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
sudo service klipper start
```
and the updated config might look like:
```
[mcu]
serial: /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

Be sure to copy-and-paste the name from the "ls" command that you ran
above as the name will be different for each printer.

If you are using multiple micro-controllers and they do not have
unique ids (common on boards with a CH340 USB chip) then follow the
directions above using the directory `/dev/serial/by-path/` instead.

### When the micro-controller restarts the device changes to /dev/ttyUSB1

Follow the directions in the
"[Where's my serial port?](#wheres-my-serial-port)" section to prevent
this from occurring.

### The "make flash" command doesn't work

The code attempts to flash the device using the most common method for
each platform. Unfortunately, there is a lot of variance in flashing
methods, so the "make flash" command may not work on all boards.

If you're having an intermittent failure or you do have a standard
setup, then double check that Klipper isn't running when flashing
(sudo service klipper stop), make sure OctoPrint isn't trying to
connect directly to the device (open the Connection tab in the web
page and click Disconnect if the Serial Port is set to the device),
and make sure FLASH_DEVICE is set correctly for your board (see the
[question above](#wheres-my-serial-port)).

However, if "make flash" just doesn't work for your board, then you
will need to manually flash. See if there is a config file in the
[config directory](https://github.com/KevinOConnor/klipper/tree/master/config)
with specific instructions for flashing the device. Also, check the
board manufacturer's documentation to see if it describes how to flash
the device. Finally, it may be possible to manually flash the device
using tools such as "avrdude" or "bossac" - see the
[bootloader document](Bootloaders.md) for additional information.

### How do I change the serial baud rate?

The recommended baud rate for Klipper is 250000. This baud rate works
well on all micro-controller boards that Klipper supports. If you've
found an online guide recommending a different baud rate, then ignore
that part of the guide and continue with the default value of 250000.

If you want to change the baud rate anyway, then the new rate will
need to be configured in the micro-controller (during **make
menuconfig**) and that updated code will need to be compiled and
flashed to the micro-controller. The Klipper printer.cfg file will
also need to be updated to match that baud rate (see the example.cfg
file for details).  For example:
```
[mcu]
baud: 250000
```

The baud rate shown on the OctoPrint web page has no impact on the
internal Klipper micro-controller baud rate. Always set the OctoPrint
baud rate to 250000 when using Klipper.

The Klipper micro-controller baud rate is not related to the baud rate
of the micro-controller's bootloader. See the
[bootloader document](Bootloaders.md) for additional information on
bootloaders.

### Can I run Klipper on something other than a Raspberry Pi 3?

The recommended hardware is a Raspberry Pi 2 or a Raspberry
Pi 3.

Klipper will run on a Raspberry Pi 1 and on the Raspberry Pi Zero, but
these boards don't have enough processing power to run OctoPrint
well. It's not uncommon for print stalls to occur on these slower
machines (the printer may move faster than OctoPrint can send movement
commands) when printing directly from OctoPrint. If you wish to run on
one one of these slower boards anyway, consider using the
"virtual_sdcard" feature (see
[config/example-extras.cfg](https://github.com/KevinOConnor/klipper/tree/master/config/example-extras.cfg)
for details) when printing.

For running on the Beaglebone, see the
[Beaglebone specific installation instructions](beaglebone.md).

Klipper has been run on other machines.  The Klipper host software
only requires Python running on a Linux (or similar)
computer. However, if you wish to run it on a different machine you
will need Linux admin knowledge to install the system prerequisites
for that particular machine. See the
[install-octopi.sh](https://github.com/KevinOConnor/klipper/tree/master/scripts/install-octopi.sh)
script for further information on the necessary Linux admin steps.

### Can I run multiple instances of Klipper on the same host machine?

It is possible to run multiple instances of the Klipper host software,
but doing so requires Linux admin knowledge. The Klipper installation
scripts ultimately cause the following Unix command to be run:
```
~/klippy-env/bin/python ~/klipper/klippy/klippy.py ~/printer.cfg -l /tmp/klippy.log
```
One can run multiple instances of the above command as long as each
instance has its own printer config file, its own log file, and its
own pseudo-tty. For example:
```
~/klippy-env/bin/python ~/klipper/klippy/klippy.py ~/printer2.cfg -l /tmp/klippy2.log -I /tmp/printer2
```

If you choose to do this, you will need to implement the necessary
start, stop, and installation scripts (if any). The
[install-octopi.sh](https://github.com/KevinOConnor/klipper/tree/master/scripts/install-octopi.sh)
script and the
[klipper-start.sh](https://github.com/KevinOConnor/klipper/tree/master/scripts/klipper-start.sh)
script may be useful as examples.

### Do I have to use OctoPrint?

The Klipper software is not dependent on OctoPrint. It is possible to
use alternative software to send commands to Klipper, but doing so
requires Linux admin knowledge.

Klipper creates a "virtual serial port" via the "/tmp/printer" file,
and it emulates a classic 3d-printer serial interface via that file.
In general, alternative software may work with Klipper as long as it
can be configured to use "/tmp/printer" for the printer serial port.

### Why can't I move the stepper before homing the printer?

The code does this to reduce the chance of accidentally commanding the
head into the bed or a wall. Once the printer is homed the software
attempts to verify each move is within the position_min/max defined in
the config file. If the motors are disabled (via an M84 or M18
command) then the motors will need to be homed again prior to
movement.

If you want to move the head after canceling a print via OctoPrint,
consider changing the OctoPrint cancel sequence to do that for
you. It's configured in OctoPrint via a web browser under:
Settings->GCODE Scripts

If you want to move the head after a print finishes, consider adding
the desired movement to the "custom g-code" section of your slicer.

If the printer requires some additional movement as part of the homing
process itself (or fundamentally does not have a homing process) then
consider using a homing_override section in the config file. If you
need to move a stepper for diagnostic or debugging purposes then
consider adding a force_move section to the config file. See
[example-extras.cfg](https://github.com/KevinOConnor/klipper/tree/master/config/example-extras.cfg)
for further details on these options.

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

### My TMC motor driver turns off in the middle of a print

Short answer: Do not use the TMC2208 driver in "standalone mode" with
Klipper! Do not use the TMC2224 driver in "stealthchop standalone
mode" with Klipper!

Long answer: Klipper implements very precise timing.

![tmc2208](img/tmc2208.svg.png)

In the above picture, if Klipper is requested to move along the red
line and if each black line represents the nominal location to step a
stepper, then in the middle of that movement Klipper will arrange to
take a step, change the step direction, and then step back. Klipper
can perform this step, direction change, and step back in a very small
amount of time.

It is our current understanding that the TMC2208 and TMC2224 will
react poorly to this when they are in "stealthchop" mode. (It is not
believed any other TMC drivers are impacted.) It is believed that when
the driver sees the two step requests in a small time frame that it
dramatically increases current in anticipation of high acceleration.
That high current can trip the driver's internal "over current"
detection which causes the driver to disable itself.

This pattern of steps can occur on all stepper motors and on all
robot kinematics.

The TMC2208 and TMC2224 do work well with Klipper when run-time
configuration mode is used (that is, when a wire is routed from the
micro-controller to the PDN-UART pin and the printer config file has a
corresponding [tmc2208] config section). When using run-time
configuration, either configure the drivers to use "spreadcycle mode"
or configure them to use "stealthchop mode" with a reasonable
"stealthchop threshold". If one wishes to exclusively use
"stealthchop" mode with run-time UART configuration then make sure the
stealthchop_threshold is no more than about 10% greater than the
maximum velocity of the given axis. It is speculated that with a
reasonable stealthchop threshold, then if Klipper sends a "step,
direction change, step back" sequence, the driver will briefly
transition from stealthchop mode, to spreadcycle mode, and back to
stealthchop mode, which should be harmless.

### I keep getting random "Lost communication with MCU" errors

This is commonly caused by hardware errors on the USB connection
between the host machine and the micro-controller. Things to look for:
- Use a good quality USB cable between the host machine and
  micro-controller. Make sure the plugs are secure.
- If using a Raspberry Pi, use a
  [good quality power supply](https://www.raspberrypi.org/documentation/hardware/raspberrypi/power/README.md)
  for the Raspberry Pi and use a
  [good quality USB cable](https://www.raspberrypi.org/forums/viewtopic.php?p=589877#p589877)
  to connect that power supply to the Pi. If you get "under voltage"
  warnings from OctoPrint, this is related to the power supply and it
  must be fixed.
- Make sure the printer's power supply is not being overloaded. (Power
  fluctuations to the micro-controller's USB chip may result in resets
  of that chip.)
- Verify stepper, heater, and other printer wires are not crimped or
  frayed. (Printer movement may place stress on a faulty wire causing
  it to lose contact, briefly short, or generate excessive noise.)
- There have been reports of high USB noise when both the printer's
  power supply and the host's 5V power supply are mixed. (If you find
  that the micro-controller powers on when either the printer's power
  supply is on or the USB cable is plugged in, then it indicates the
  5V power supplies are being mixed.) It may help to configure the
  micro-controller to use power from only one source. (Alternatively,
  if the micro-controller board can not configure its power source,
  one may modify a USB cable so that it does not carry 5V power
  between the host and micro-controller.)

### My Raspberry Pi keeps rebooting during prints

This is most likely do to voltage fluctuations. Follow the same
troubleshooting steps for a
["Lost communication with MCU"](#i-keep-getting-random-lost-communication-with-mcu-errors)
error.

### When I set "restart_method=command" my AVR device just hangs on a restart

Some old versions of the AVR bootloader have a known bug in watchdog
event handling. This typically manifests when the printer.cfg file has
restart_method set to "command". When the bug occurs, the AVR device
will be unresponsive until power is removed and reapplied to the
device (the power or status LEDs may also blink repeatedly until the
power is removed).

The workaround is to use a restart_method other than "command" or to
flash an updated bootloader to the AVR device. Flashing a new
bootloader is a one time step that typically requires an external
programmer - see [Bootloaders](Bootloaders.md) for further details.

### Will the heaters be left on if the Raspberry Pi crashes?

The software has been designed to prevent that. Once the host enables
a heater, the host software needs to confirm that enablement every 5
seconds. If the micro-controller does not receive a confirmation every
5 seconds it goes into a "shutdown" state which is designed to turn
off all heaters and stepper motors.

See the "config_digital_out" command in the
[MCU commands](MCU_Commands.md) document for further details.

In addition, the micro-controller software is configured with a
minimum and maximum temperature range for each heater at startup (see
the min_temp and max_temp parameters in the
[example.cfg](https://github.com/KevinOConnor/klipper/tree/master/config/example.cfg)
file for details). If the micro-controller detects that the
temperature is outside of that range then it will also enter a
"shutdown" state.

Separately, the host software also implements code to check that
heaters and temperature sensors are functioning correctly. See the
"verify_heater" section of the
[example-extras.cfg](https://github.com/KevinOConnor/klipper/tree/master/config/example-extras.cfg)
for further details.

### How do I convert a Marlin pin number to a Klipper pin name?

Short answer: In some cases one can use Klipper's `pin_map: arduino`
feature. Otherwise, for "digital" pins, one method is to search for
the requested pin in Marlin's fastio header files. The Atmega2560 and
Atmega1280 chips use
[fastio_1280.h](https://github.com/MarlinFirmware/Marlin/blob/1.1.9/Marlin/fastio_1280.h),
while the Atmega644p and Atmega1284p chips use
[fastio_644.h](https://github.com/MarlinFirmware/Marlin/blob/1.1.9/Marlin/fastio_644.h).
For example, if you are looking to translate Marlin's digital pin
number 23 on an atmega2560 then one could find the following line in
Marlin's fastio_1280.h file:
```
#define DIO23_PIN PINA1
```
The `DIO23` indicates the line is for Marlin's pin 23 and the `PINA1`
indicates the pin uses the hardware name of `PA1`. Klipper uses the
hardware names (eg, `PA1`).

Long answer: Klipper uses the standard pin names defined by the
micro-controller. On the Atmega chips these hardware pins have names
like `PA4`, `PC7`, or `PD2`.

Long ago, the Arduino project decided to avoid using the standard
hardware names in favor of their own pin names based on incrementing
numbers - these Arduino names generally look like `D23` or `A14`. This
was an unfortunate choice that has lead to a great deal of confusion.
In particular the Arduino pin numbers frequently don't translate to
the same hardware names. For example, `D21` is `PD0` on one common
Arduino board, but is `PC7` on another common Arduino board.

In order to support 3d printers based on real Arduino boards, Klipper
supports the Arduino pin aliases. This feature is enabled by adding
`pin_map: arduino` to the [mcu] section of the config file. When these
aliases are enabled, Klipper understands pin names that start with the
prefix "ar" (eg, Arduino pin `D23` is Klipper alias `ar23`) and the
prefix "analog" (eg, Arduino pin `A14` is Klipper alias `analog14`).
Klipper does not use the Arduino names directly because we feel a name
like D7 is too easily confused with the hardware name PD7.

Marlin primarily follows the Arduino pin numbering scheme.  However,
Marlin supports a few chips that Arduino does not support and in some
cases it supports pins that Arduino boards do not expose. In these
cases, Marlin chose their own pin numbering scheme. Klipper does not
support these custom pin numbers - check Marlin's fastio headers (see
above) to translate these pin numbers to their standard hardware
names.

### How do I cancel an M109/M190 "wait for temperature" request?

Navigate to the OctoPrint terminal tab and issue an M112 command in
the terminal box. The M112 command will cause Klipper to enter into a
"shutdown" state, and it will cause OctoPrint to disconnect from
Klipper. Navigate to the OctoPrint connection area and click on
"Connect" to cause OctoPrint to reconnect. Navigate back to the
terminal tab and issue a FIRMWARE_RESTART command to clear the Klipper
error state.  After completing this sequence, the previous heating
request will be canceled and a new print may be started.

### How do I upgrade to the latest software?

The general way to upgrade is to ssh into the Raspberry Pi and run:

```
cd ~/klipper
git pull
~/klipper/scripts/install-octopi.sh
```

Then one can recompile and flash the micro-controller code. For
example:

```
make menuconfig
make clean
make

sudo service klipper stop
make flash FLASH_DEVICE=/dev/ttyACM0
sudo service klipper start
```

However, it's often the case that only the host software changes. In
this case, one can update and restart just the host software with:

```
cd ~/klipper
git pull
sudo service klipper restart
```

If after using this shortcut the software warns about needing to
reflash the micro-controller or some other unusual error occurs, then
follow the full upgrade steps outlined above. Note that the RESTART
and FIRMWARE_RESTART g-code commands do not load new software - the
above "sudo service klipper restart" and "make flash" commands are
needed for a software change to take effect.

When upgrading the software, be sure to check the
[config changes](Config_Changes.md) document for information on
software changes that may require updates to your printer.cfg file.

### Can I find out whether the printer has lost steps?

In a way, yes. Home the printer, issue a `GET_POSITION` command, run
your print, home again and issue another `GET_POSITION`. Then compare
the values in the `mcu:` line.

This might be helpful to tune settings like stepper motor currents,
accelerations and speeds without needing to actually print something
and waste filament: just run some high-speed moves in between the
`GET_POSITION` commands.

Note that endstop switches themselves tend to trigger at slightly
different positions, so a difference of a couple of microsteps is
likely the result of endstop inaccuracies. A stepper motor itself can
only lose steps in increments of 4 full steps. (So, if one is using 16
microsteps, then a lost step on the stepper would result in the "mcu:"
step counter being off by a multiple of 64 microsteps.)
