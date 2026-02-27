# Hall filament width sensor

This document describes Filament Width Sensor host module. Hardware used for
developing this host module is based on two Hall linear sensors (ss49e for
example). Sensors in the body are located on opposite sides. Principle of operation:
two hall sensors work in differential mode, temperature drift same for sensor.
Special temperature compensation not needed.

You can find designs at [Thingiverse](https://www.thingiverse.com/thing:4138933),
an assembly video is also available on [Youtube](https://www.youtube.com/watch?v=TDO9tME8vp4)

To use Hall filament width sensor, read
[Config Reference](Config_Reference.md#hall_filament_width_sensor) and
[G-Code documentation](G-Codes.md#hall_filament_width_sensor).


## How does it work?

Sensor generates two analog output based on calculated filament width. Sum of
output voltage always equals to detected filament width. Host module monitors
voltage changes and adjusts extrusion multiplier. I use the aux2 connector on
a ramps-like board with the analog11 and analog12 pins. You can use different pins
and different boards.

## Template for menu variables

```
[menu __main __filament __width_current]
type: command
enable: {'hall_filament_width_sensor' in printer}
name: Dia: {'%.2F' % printer.hall_filament_width_sensor.Diameter}
index: 0

[menu __main __filament __raw_width_current]
type: command
enable: {'hall_filament_width_sensor' in printer}
name: Raw: {'%4.0F' % printer.hall_filament_width_sensor.Raw}
index: 1
```

## Calibration procedure

To get raw sensor value you can use menu item or **QUERY_RAW_FILAMENT_WIDTH**
command in terminal.

1. Insert first calibration rod (1.5 mm size) get first raw sensor value

2. Insert second calibration rod (2.0 mm size) get second raw sensor value

3. Save raw sensor values in config parameter `Raw_dia1` and `Raw_dia2`

## How to enable sensor

By default, the sensor is disabled at power-on.

To enable the sensor, issue **ENABLE_FILAMENT_WIDTH_SENSOR** command or
set the `enable` parameter to `true`.

## Use as a runout switch only

By default, the sensor measures filament diameter and adjusts extrusion multiplier
to compensate for variations.

If you want to use the sensor as a runout switch only, set the `enable_flow_compensation`
config parameter to `false`. In this mode, the sensor will only trigger runout
events when filament is not detected, it will not modify the extrusion multiplier.

This is useful for printers where the filament sensor is not accurate enough for
flow compensation but can reliably detect filament runout, or when printing with
flexible filaments which have unstable diameter characteristics.

Issue **ENABLE_FILAMENT_WIDTH_COMPENSATION** to enable flow compensation
or **DISABLE_FILAMENT_WIDTH_COMPENSATION** to disable it.

Note that disabling filament width compensation automatically resets the extrusion
multiplier to 100%.

**QUERY_FILAMENT_WIDTH_COMPENSATION** returns the current state of flow compensation.

## Logging

By default, diameter logging is disabled at power-on.

Issue **ENABLE_FILAMENT_WIDTH_LOG** command to start logging and issue
**DISABLE_FILAMENT_WIDTH_LOG** command to stop logging. To enable logging
at power-on, set the `logging` parameter to `true`.

Filament diameter is logged on every measurement interval (10 mm by default).
