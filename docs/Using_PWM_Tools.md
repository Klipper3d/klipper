This document describes how to setup a PWM-Controlled laser or spindle
using `output_pin` and some macros.


## How does it work?
With re-purposing the printhead's fan pwm output, you can control
lasers or spindles.
This is useful if you use switchable print heads, for example.
Usually, cam-tools such as LaserWeb can be configured to use `M3-M5`
commands, which stand for _spindle speed CW_ (`M3 S[0-255]`),
_spindle speed CCW_ (`M4 S[0-255]`) and _spindle stop_ (`M5`).

For safety reasons, you should configure a safety timeout,
so that when your host or MCU encounters an error, the tool will stop.


## Example Configuration
    [output_pin TOOL]
    pin: !ar9       # use your fan's pin number
    pwm: True
    hardware_pwm: True
    cycle_time: 0.001
    shutdown_value: 0

    host_acknowledge_timeout: 5
    # Default: 0 (disabled)
    # Amount of time in which the host has to acknowledge
    # a non-shutdown output value.
    # Suggested value is around 5 seconds.
    # Use a value that does not burn up your stock.
    # Please note that during homing, your tool
    # needs to be in default speed.

    [gcode_macro M3]
    default_parameter_S: 0
    gcode:
        SET_PIN PIN=TOOL VALUE={S|float / 255}

    [gcode_macro M4]
    default_parameter_S: 0
    gcode:
        SET_PIN PIN=TOOL VALUE={S|float / 255}

    [gcode_macro M5]
    gcode:
        SET_PIN PIN=TOOL VALUE=0

    [menu __main __control __toolonoff]
    type: input
    enable: {'output_pin TOOL' in printer}
    name: Fan: {'ON ' if menu.input else 'OFF'}
    input: {printer['output_pin TOOL'].value}
    input_min: 0
    input_max: 1
    input_step: 1
    gcode:
        M3 S{255 if menu.input else 0}

    [menu __main __control __toolspeed]
    type: input
    enable: {'output_pin TOOL' in printer}
    name: Tool speed: {'%3d' % (menu.input*100)}%
    input: {printer['output_pin TOOL'].value}
    input_min: 0
    input_max: 1
    input_step: 0.01
    gcode:
        M3 S{'%d' % (menu.input*255)}

## Commands

`M3/M4 S<value>` : Set PWM duty-cycle. Values between 0 and 255.
`M5` : Stop PWM output to shutdown value.
