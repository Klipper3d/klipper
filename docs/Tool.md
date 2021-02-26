This document describes the _tool_ module.


## How does it work?
This enables the use of _M3 M4 M5_ GCODE commands.
It is based on the generic fan PWM module, but adds a shutdown value
and makes use of the MCU's safety timeout.

## Configuration
    [tool]
    pin: !ar9

    hardware_pwm: True
    # Default: True

    cycle_time: 0.001
    # Default: 0.01

    max_power: 1
    # Default: 1

    off_below: 0
    # Default: 0

    shutdown_value: 0
    # Default: 0

    safety_timeout: 20
    # Default: 0 (disabled)
    # Suggested value is a minimal amount longer
    # than the longest running command.
    # Currently, moves are not split into individual sections.

## Commands

`M3/M4 S<value>` : Set PWM duty-cycle. Values between 0 and 255.
`M5` : Stop PWM output to shutdown value.
