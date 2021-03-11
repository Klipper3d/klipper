This document provides information on using Trinamic stepper motor
drivers in SPI/UART mode on Klipper.

Klipper can also use Trinamic drivers in their "standalone mode".
However, when the drivers are in this mode, no special Klipper
configuration is needed and the advanced Klipper features discussed in
this document are not available.

In addition to this document, be sure to review the [TMC driver config
reference](Config_Reference.md#tmc-stepper-driver-configuration).

# Enabling "Stealthchop" mode

By default, Klipper places the TMC drivers in "spreadcycle" mode. If
the driver supports "stealthchop" then it can be enabled by adding
`stealthchop_threshold: 999999` to the TMC config section.

It is recommended to always use "spreadcycle" mode (by not specifying
`stealthchop_threshold`) or to always use "stealthchop" mode (by
setting `stealthchop_threshold` to 999999). Unfortunately, the drivers
often produce poor and confusing results if the mode changes while the
motor is at a non-zero velocity.

# Sensorless Homing

Sensorless homing allows to home an axis without the need for a
physical limit switch. Instead, the carriage on the axis is moved into
the mechanical limit making the stepper motor lose steps. The stepper
driver senses the lost steps and indicates this to the controlling MCU
(Klipper) by toggling a pin. This information can be used by Klipper
as end stop for the axis.

This guide covers the setup of sensorless homing for the X axis of
your (cartesian) printer. However, it works the same with all other
axes (that require an end stop). You should configure and tune it for
one axis at a time.

## Prerequisites

A few prerequisites are needed to use sensorless homing:

1. StallGuard capable TMCxxxx stepper driver
2. SPI / UART interface of the TMCxxxx wired to MCU (stand-alone mode
   does not work)
3. DIAG1/DIAG pin of TMCxxxx connected to the MCU

## Limitations

Be sure that your mechanical components are able to handle the load of
the carriage bumping into the limit of the axis repeatedly. Especially
spindles (on the Z axis) might generate a lot of force. Homing a Z
axis by bumping the nozzle into the printing surface might not be a
good idea.

Further, sensorless homing might not be accurate enough for your
printer. While homing X and Y axes on a cartesian machine can work
well, homing the Z axis is generally not accurate enough and results
in inconsistent first layer height. Homing a delta printer sensorless
is not advisable due to missing accuracy.

Further, the stall detection of the stepper driver is dependent on the
mechanical load on the motor, the motor current and the motor
temperature (coil resistance).

Sensorless homing works best at medium motor speeds. For very slow
speeds (less than 10 RPM) the motor does not generate significant back
EMF and the TMC cannot reliably detect motor stalls. Further, at very
high speeds, the back EMF of the motor approaches the supply voltage
of the motor, so the TMC cannot detect stalls anymore. It is advised
to have a look in the datasheet of your specific TMCs. There you can
also find more details on limitations of this setup.

## Configuration

To enable sensorless homing add a section to configure the TMC stepper
driver to your `printer.cfg`.

In this guide we'll be using a TMC2130. The configuration however is
similar to the other TMCs with StallGuard:

```
[tmc2130 stepper_x]
cs_pin:        # chip select pin of the SPI interface
microsteps:    # number of microsteps per full step of the motor
run_current:   # value in amps
diag1_pin: !   # pin on the MCU where DIAG1 is connected (active low)
driver_SGT:    # tuning value for sensorless homing
```

The above snippet configures a TMC2130 for the stepper on the X axis.
Make sure to fill in the missing values based on your configuration.

The `driver_SGT` value describes the threshold when the driver
reports a stall. Values have to be in between -64 (most sensitive) and
64 (least sensitive). On some TMCs like the TMC2209 this value doesn't
exist in this form as the behavior is different to the TMC2130. In the
case of the TMC2209 the threshold is defined by the `driver_SGTHRS`
value in the config and go from 0 (least sensitive) to 255 (most
sensitive). Have a look at the datasheet of your specific TMC to avoid
mistakes.

If you have a CoreXY machine, you can configure one stepper driver for
X and the other for Y homing as you would on a cartesian printer. Be
aware that Klipper needs both `DIAG1` pins connected to the MCU. It is
not sufficient to use only one signal from one of the stepper drivers
(as it is possible on e.g. Marlin).

The `diag1_pin` of the TMC2130 is configured as open-collector pin.
This means, the stepper driver pulls the pin low to indicate a stalled
motor (active low) and the pin must be inverted by adding a `!` in
front of the pin name. Further, you need a pull-up resistor on the
connection. If your PCB has no external pull-up, you can enable the
internal pull-up of your MCU by adding a `^` in front of the pin name.
The resulting line might look like this:

```
diag1_pin: ^!PA1  # DIAG1 connected to PA1, internal pull-up is enabled, signal is active low
```

By configuring the `diag1_pin`, Klipper allows you to use a special
virtual end stop for the axis. You can use this instead of a physical
end stop pin by changing the `endstop_pin` of the corresponding axis:

```
[stepper_x]
...
endstop_pin: tmc2130_stepper_x:virtual_endstop  # use the virtual end stop generated by the [tmc2130 stepper_x] section of this config file
...
homing_retract_dist: 0
...
```

The name of the virtual end stop pin is derived from the name of the
TMC2130 section. The `homing_retract_dist` setting should be set to
zero to disable the second homing move as a second pass is not needed,
and attempts to do so are error prone.

The TMC2130 and TMC5160 have both a `diag0_pin` and `diag1_pin` in
most known hardware the `diag1_pin` is appropriate. In order for
klipper to correctly configure the driver for sensorless homing, the
correct configuration property name `diag0_pin` or `diag1_pin` must be
used. Which is used is determined by which driver pin is connected to
the MCU pin.

ATTENTION: This guide only mentions the mandatory parameters and the
ones needed to set up sensorless homing. There are many other options
to configure on a TMC2130, make sure to take a look at [config
reference](Config_Reference.md#tmc2130) for all the available options.

## Homing and Tuning

Let's try the first sensorless homing now. It will likely not work as
intended. There are three possible outcomes of this experiment:

1. The axis stops moving before hitting the mechanical limit or does
   not move at all
2. The axis homes correctly (which is unlikely at this point)
3. The axis bumps into the mechanical limit and keeps moving while
   making horrible noise

If the third outcome happens to you, disable the stepper (by cutting
the power or issuing a `M112` emergency stop).

Ok, now that you know what can happen, let's try it out. Put the
carriage somewhere in the middle of the X axis. Home the X axis by
sending the following G-Code command to Klipper and observe the
outcome:

```
G28 X
```

If the axis stopped early (first outcome), the stepper driver detected
a motor stall even though there was none. To trigger stall detection
at a higher load, increase the value of `driver_SGT` (for example from
0 to 5). The values can be any integer between `-64` and `63`. The
higher the value, the later it triggers stall detection.

If your axis did not stop (third outcome), the stepper driver was not
able to detect the stall, because the load on the motor still seemed
reasonable to the driver. To trigger stall detection at a lighter
load, decrease the value of `driver_SGT`.

Even if your axis homed correctly, it might be worth to try a few
different values for `driver_SGT`. If you think that it bumps too hard
into the mechanical limit, try to decrease the value by 1 or 2.

At this point, your axis should be able to home based on the stall
detection of the TMC2130. Congratulations! You can now proceed with
the next axis of your printer.

# Querying and diagnosing driver settings

The `[DUMP_TMC command](G-Codes.md#tmc-stepper-drivers) is a useful
tool when configuring and diagnosing the drivers. It will report all
fields configured by Klipper as well as all fields that can be queried
from the driver.

All of the reported fields are defined in the Trinamic datasheet for
each driver. These datasheets can be found on the [Trinamic
website](https://www.trinamic.com/). Obtain and review the Trinamic
datasheet for the driver to interpret the results of DUMP_TMC.

# Configuring driver_XXX settings

Klipper supports configuring many low-level driver fields using
`driver_XXX` settings. The [TMC driver config
reference](Config_Reference.md#tmc-stepper-driver-configuration) has
the full list of fields available for each type of driver.

In addition, almost all fields can be modified at run-time using the
[SET_TMC_FIELD command](G-Codes.md#tmc-stepper-drivers).

Each of these fields is defined in the Trinamic datasheet for each
driver. These datasheets can be found on the [Trinamic
website](https://www.trinamic.com/).

Note that the Trinamic datasheets sometime use wording that can
confuse a high-level setting (such as "hysteresis end") with a
low-level field value (eg, "HEND"). In Klipper, `driver_XXX` and
SET_TMC_FIELD always set the low-level field value that is actually
written to the driver. So, for example, if the Trinamic datasheet
states that a value of 3 must be written to the HEND field to obtain a
"hysteresis end" of 0, then set `driver_HEND=3` to obtain the
high-level value of 0.

# Common Questions

## Can I use stealthchop mode on an extruder with pressure advance?

Many people successfully use "stealthchop" mode with Klipper's
pressure advance. Klipper implements [smooth pressure
advance](Kinematics.md#pressure-advance) which does not introduce any
instantaneous velocity changes.

However, "stealthchop" mode may produce lower motor torque and/or
produce higher motor heat. It may or may not be an adequate mode for
your particular printer.

## I keep getting "Unable to read tmc uart 'stepper_x' register IFCNT" errors?

This occurs when Klipper is unable to communicate with a tmc2208 or
tmc2209 driver.

Make sure that the motor power is enabled, as the stepper motor driver
generally needs motor power before it can communicate with the
micro-controller.

Otherwise, this error is typically the result of incorrect UART pin
wiring or an incorrect Klipper configuration of the UART pin settings.

## I keep getting "Unable to write tmc spi 'stepper_x' register ..." errors?

This occurs when Klipper is unable to communicate with a tmc2130 or
tmc5160 driver.

Make sure that the motor power is enabled, as the stepper motor driver
generally needs motor power before it can communicate with the
micro-controller.

Otherwise, this error is typically the result of incorrect SPI wiring,
an incorrect Klipper configuration of the SPI settings, or an
incomplete configuration of devices on an SPI bus.

Note that if the driver is on a shared SPI bus with multiple devices
then be sure to fully configure every device on that shared SPI bus in
Klipper. If a device on a shared SPI bus is not configured, then it
may incorrectly respond to commands not intended for it and corrupt
the communication to the intended device. If there is a device on a
shared SPI bus that can not be configured in Klipper, then use a
[static_digital_output config
section](Config_Reference.md#static_digital_output) to set the CS pin
of the unused device high (so that it will not attempt to use the SPI
bus). The board's schematic is often a useful reference for finding
which devices are on an SPI bus and their associated pins.

## How do I tune spreadcycle/coolstep/etc. mode on my drivers?

The [Trinamic website](https://www.trinamic.com/) has guides on
configuring the drivers. These guides are often technical, low-level,
and may require specialized hardware. Regardless, they are the best
source of information.
