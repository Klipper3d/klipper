# Mixing extruder

This document describes how to configure and use a filament mixing extruder.
Please also refer to the [config reference](Config_Reference.md) and supported
[G-Codes](G-Codes.md).

## Configuring a mixing extruder.

A mixing extruder has N-filament inputs and a single nozzle. This requires
N independent filament drives. They have to be defined in the config as regular
extruders.

Since they all drive filament to the same hotend they must have the
`shared_heater` property set to point to one of them. For this reason all
heater-related parameters should be set only for that one.

Following individual filament drivers definitions there should be the
`[mixingextruder]` section which groups all of them to tell Klipper that
they in fact drive the same mixing hotend.

Here is an example of configuration snippet for 3-to-1 mixing extruder:
```
# "Alpha" stepper
[extruder]
step_pin: PC7
dir_pin: !PC8
enable_pin: !PA18
microsteps: 16
rotation_distance: 7.15
nozzle_diameter: 0.400
filament_diameter: 1.75
heater_pin: PC22
sensor_type: EPCOS 100K B57560G104F
sensor_pin: PA6
control: pid
pid_kp: 37.919
pid_ki: 0.950
pid_kd: 378.241
min_temp: 0
max_temp: 275
pressure_advance: 0.1
min_extrude_temp: 0

# "Beta" stepper
[extruder1]
step_pin: PB3
dir_pin: !PC10
enable_pin: !PB4
microsteps: 16
rotation_distance: 7.15
nozzle_diameter: 0.400
filament_diameter: 1.75
pressure_advance: 0.1
shared_heater: extruder

# "Gamma" stepper
[extruder2]
step_pin: PB1
dir_pin: !PB0
enable_pin: !PB2
microsteps: 16
rotation_distance: 7.15
nozzle_diameter: 0.400
filament_diameter: 1.75
pressure_advance: 0.1
shared_heater: extruder

[mixingextruder]
extruders: extruder,extruder1,extruder2
```

Stepper parameters may vary among all the drivers but in most systems they are
identical.

## Retractions

For correct operation of a mixing extruder all N input filaments should be
retracted by the same amount regardless of the mixing ratio used. Most
3D printer firmware implement that via firmware retractions.

The current implementation of retractions in Klipper is different and is based
on automatic tracking of extrusion moves. When Klipper detects a retraction
(which is backward extrusion move) it sets internally the mixing ratio for
each filament driver to 1/N ensuring that all filaments are moved by the same
distance. The retracted distance is accumulated and used to detect the end
of subsequent unretraction move. When that happens the mixing ratio is restored.

Because retractions happen with mixing ratio of 1/N for each stepper the amount
the filament actually moves and the move speed is also divided by N. This
currently has to be accounted for in a slicer firmware. For example whe one
wants to retract by 3mm with the speed of 50mm/s a 3-to-1 mixing extruder then
the values in the slicer need to be 3 * 3mm = 9mm and 3 * 50mm/s = 150mm/s.

A mixing extruder should be thought of as if it was driving a "virtual filament"
at the point where it enters the nozzle.
