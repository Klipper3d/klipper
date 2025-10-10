# Multiple Micro-controller Homing and Probing

Klipper supports a mechanism for homing with an endstop attached to
one micro-controller while its stepper motors are on a different
micro-controller. This support is referred to as "multi-mcu
homing". This feature is also used when a Z probe is on a different
micro-controller than the Z stepper motors.

This feature can be useful to simplify wiring, as it may be more
convenient to attach an endstop or probe to a closer micro-controller.
However, using this feature may result in "overshoot" of the stepper
motors during homing and probing operations.

The overshoot occurs due to possible message transmission delays
between the micro-controller monitoring the endstop and the
micro-controllers moving the stepper motors. The Klipper code is
designed to limit this delay to no more than 25ms. (When multi-mcu
homing is activated, the micro-controllers send periodic status
messages and check that corresponding status messages are received
within 25ms.)

So, for example, if homing at 10mm/s then it is possible for an
overshoot of up to 0.250mm (10mm/s * .025s == 0.250mm). Care should be
taken when configuring multi-mcu homing to account for this type of
overshoot. Using slower homing or probing speeds can reduce the
overshoot.

Stepper motor overshoot should not adversely impact the precision of
the homing and probing procedure. The Klipper code will detect the
overshoot and account for it in its calculations. However, it is
important that the hardware design is capable of handling overshoot
without causing damage to the machine.

In order to use this "multi-mcu homing" capability the hardware must
have predictably low latency between the host computer and all of the
micro-controllers. Typically the round-trip time must be consistently
less than 10ms. High latency (even for short periods) is likely to
result in homing failures.

Should high latency result in a failure (or if some other
communication issue is detected) then Klipper will raise a
"Communication timeout during homing" error.

Note that an axis with multiple steppers (eg, `stepper_z` and
`stepper_z1`) need to be on the same micro-controller in order to use
multi-mcu homing. For example, if an endstop is on a separate
micro-controller from `stepper_z` then `stepper_z1` must be on the
same micro-controller as `stepper_z`.
