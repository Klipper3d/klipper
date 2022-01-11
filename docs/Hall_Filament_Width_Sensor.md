# Hall filament width sensor

This document describes Filament Width Sensor host module. Hardware used for
developing this host module is based on two Hall linear sensors (ss49e for
example). Sensors in the body are located opposite sides. Principle of operation:
two hall sensors work in differential mode, temperature drift same for sensor.
Special temperature compensation not needed.

You can find designs at [Thingiverse](https://www.thingiverse.com/thing:4138933),
an assembly video is also available on [Youtube](https://www.youtube.com/watch?v=TDO9tME8vp4)

To use Hall filament width sensor, read
[Config Reference](Config_Reference.md#hall_filament_width_sensor) and
[G-Code documentation](G-Codes.md#Hall_Filament_Width_Sensor_Commands).


## How does it work?

Sensor generates two analog output based on calculated filament width. Sum of
output voltage always equals to detected filament width. Host module monitors
voltage changes and adjusts extrusion multiplier. I use aux2 connector on
ramps-like board analog11 and analog12 pins. You can use different pins and
differenr boards.

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
set the `enable` parameter to `true.`

## Logging

By default, diameter logging is disabled at power-on.

Issue **ENABLE_FILAMENT_WIDTH_LOG** command to start logging and issue
**DISABLE_FILAMENT_WIDTH_LOG** command to stop logging. To enable logging
at power-on, set the `logging` parameter to `true`.

Filament diameter is logged on every measurement interval (10 mm by default).
