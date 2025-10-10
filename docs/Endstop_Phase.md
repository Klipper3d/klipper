# Endstop phase

This document describes Klipper's stepper phase adjusted endstop
system. This functionality can improve the accuracy of traditional
endstop switches. It is most useful when using a Trinamic stepper
motor driver that has run-time configuration.

A typical endstop switch has an accuracy of around 100 microns. (Each
time an axis is homed the switch may trigger slightly earlier or
slightly later.) Although this is a relatively small error, it can
result in unwanted artifacts. In particular, this positional deviation
may be noticeable when printing the first layer of an object. In
contrast, typical stepper motors can obtain significantly higher
precision.

The stepper phase adjusted endstop mechanism can use the precision of
the stepper motors to improve the precision of the endstop switches.
A stepper motor moves by cycling through a series of phases until in
completes four "full steps". So, a stepper motor using 16 micro-steps
would have 64 phases and when moving in a positive direction it would
cycle through phases: 0, 1, 2, ... 61, 62, 63, 0, 1, 2, etc.
Crucially, when the stepper motor is at a particular position on a
linear rail it should always be at the same stepper phase. Thus, when
a carriage triggers the endstop switch the stepper controlling that
carriage should always be at the same stepper motor phase. Klipper's
endstop phase system combines the stepper phase with the endstop
trigger to improve the accuracy of the endstop.

In order to use this functionality it is necessary to be able to
identify the phase of the stepper motor. If one is using Trinamic
TMC2130, TMC2208, TMC2224 or TMC2660 drivers in run-time configuration
mode (ie, not stand-alone mode) then Klipper can query the stepper
phase from the driver. (It is also possible to use this system on
traditional stepper drivers if one can reliably reset the stepper
drivers - see below for details.)

## Calibrating endstop phases

If using Trinamic stepper motor drivers with run-time configuration
then one can calibrate the endstop phases using the
ENDSTOP_PHASE_CALIBRATE command. Start by adding the following to the
config file:
```
[endstop_phase]
```

Then RESTART the printer and run a `G28` command followed by an
`ENDSTOP_PHASE_CALIBRATE` command. Then move the toolhead to a new
location and run `G28` again. Try moving the toolhead to several
different locations and rerun `G28` from each position. Run at least
five `G28` commands.

After performing the above, the `ENDSTOP_PHASE_CALIBRATE` command will
often report the same (or nearly the same) phase for the stepper. This
phase can be saved in the config file so that all future G28 commands
use that phase. (So, in future homing operations, Klipper will obtain
the same position even if the endstop triggers a little earlier or a
little later.)

To save the endstop phase for a particular stepper motor, run
something like the following:
```
ENDSTOP_PHASE_CALIBRATE STEPPER=stepper_z
```

Run the above for all the steppers one wishes to save. Typically, one
would use this on stepper_z for cartesian and corexy printers, and for
stepper_a, stepper_b, and stepper_c on delta printers. Finally, run
the following to update the configuration file with the data:
```
SAVE_CONFIG
```

### Additional notes

* This feature is most useful on delta printers and on the Z endstop
  of cartesian/corexy printers. It is possible to use this feature on
  the XY endstops of cartesian printers, but that isn't particularly
  useful as a minor error in X/Y endstop position is unlikely to
  impact print quality. It is not valid to use this feature on the XY
  endstops of corexy printers (as the XY position is not determined by
  a single stepper on corexy kinematics). It is not valid to use this
  feature on a printer using a "probe:z_virtual_endstop" Z endstop (as
  the stepper phase is only stable if the endstop is at a static
  location on a rail).

* After calibrating the endstop phase, if the endstop is later moved
  or adjusted then it will be necessary to recalibrate the endstop.
  Remove the calibration data from the config file and rerun the steps
  above.

* In order to use this system the endstop must be accurate enough to
  identify the stepper position within two "full steps". So, for
  example, if a stepper is using 16 micro-steps with a step distance
  of 0.005mm then the endstop must have an accuracy of at least
  0.160mm. If one gets "Endstop stepper_z incorrect phase" type error
  messages than in may be due to an endstop that is not sufficiently
  accurate. If recalibration does not help then disable endstop phase
  adjustments by removing them from the config file.

* If one is using a traditional stepper controlled Z axis (as on a
  cartesian or corexy printer) along with traditional bed leveling
  screws then it is also possible to use this system to arrange for
  each print layer to be performed on a "full step" boundary. To
  enable this feature be sure the G-Code slicer is configured with a
  layer height that is a multiple of a "full step", manually enable
  the endstop_align_zero option in the endstop_phase config section
  (see [config reference](Config_Reference.md#endstop_phase) for
  further details), and then re-level the bed screws.

* It is possible to use this system with traditional (non-Trinamic)
  stepper motor drivers. However, doing this requires making sure that
  the stepper motor drivers are reset every time the micro-controller
  is reset. (If the two are always reset together then Klipper can
  determine the stepper phase by tracking the total number of steps it
  has commanded the stepper to move.) Currently, the only way to do
  this reliably is if both the micro-controller and stepper motor
  drivers are powered solely from USB and that USB power is provided
  from a host running on a Raspberry Pi. In this situation one can
  specify an mcu config with "restart_method: rpi_usb" - that option
  will arrange for the micro-controller to always be reset via a USB
  power reset, which would arrange for both the micro-controller and
  stepper motor drivers to be reset together. If using this mechanism,
  one would then need to manually configure the "trigger_phase" config
  sections (see [config reference](Config_Reference.md#endstop_phase)
  for the details).
