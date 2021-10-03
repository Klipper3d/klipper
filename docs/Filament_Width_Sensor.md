# Filament width Sensor

This module supports both the Hall filament width sensor and the TSL1401 based filament width sensor.

## Hall filament width sensor

Hardware used for developing this host module is based on Two Hall liniar sensors (ss49e for example). Sensors in the body are located opposite sides. Principle of operation : two hall sensors work in differential mode, temperature drift same for sensor. Special temperature compensation not needed. You can find designs at [thingiverse.com](https://www.thingiverse.com/thing:4138933)

[Hall based filament width sensor assembly video](https://www.youtube.com/watch?v=TDO9tME8vp4)

How does it work?
Sensor generates two analog output based on calculated filament width. Sum of output voltage always equals to detected filament width . Host module monitors voltage changes and adjusts extrusion multiplier. I use aux2 connector on ramps-like board analog11 and analog12 pins. You can use different pins and differenr boards

## TSL1401CL filament width sensor

Hardware used for developing this host module is based on TSL1401CL linear sensor array but it can work with any sensor array that has analog output. You can find designs at [thingiverse.com](https://www.thingiverse.com/search?q=filament%20width%20sensor)

How does it work?
Sensor generates analog output based on calculated filament width. Output voltage always equals to detected filament width (Ex. 1.65v, 1.70v, 3.0v). Host module monitors voltage changes and adjusts extrusion multiplier.

## Configuration

    [filament_width_sensor]

    # Lines commented with "Hall based sensor only" are needed for the hall based
    # filament width sensor. Leave these lines empty, if you use the tsl1401 based
    # filament width sensor.

    adc1: #analog pin. e.g. PA5
    adc2: #Hall based sensor only

    # adc1 and adc2 are analog input pins on 3d printer board.
    # Sensor power supply can be 3.3v or 5v

    #cal_dia1: 1.50 # Reference diameter point 1 (mm); Hall based sensor only
    #cal_dia2: 2.00 # Reference diameter point 2 (mm); Hall based sensor only

    # The measurement principle provides for two-point calibration
    # In calibration process you must use rods of known diameter
    # You can use e.g. drill rods as the base diameter.
    # nominal filament diameter must be between Cal_dia1 and Cal_dia2
    # Your size may differ from the indicated ones, for example 2.05

    #raw_dia1: 9500 # Raw sensor value for reference point 1; Hall based sensor only
    #raw_dia2: 10500 # Raw sensor value for reference point 2; Hall based sensor only

    # Raw value of sensor in units
    # can be read via command QUERY_RAW_FILAMENT_WIDTH

    #offset1: 0
    #slope1: 1
    #offset2: 0 #Hall based sensor only
    #slope2: 1 #Hall based sensor only

    # Linear calibration of voltage readings according to y=mx+b,
    # Filament width = slope * adc_value + offset

    #default_nominal_filament_diameter: 1.75 # This parameter is in millimeters (mm)

    #   The nominal filament diameter. This parameter must be provided.

    #min_difference: 0.200
    #max_difference: 0.200

    #   Maximum allowed filament diameter difference in millimeters (mm).
    #   If the difference between nominal filament diameter and sensor
    #   output is smaller than the values above, extrusion multiplier is
    #   set back to %100. The default is 0.200.

    #measurement_delay: 70

    #   The distance from sensor to the melting chamber/hot-end in
    #   millimeters (mm). The filament between the sensor and the hot-end
    #   will be treated as the default_nominal_filament_diameter. Host
    #   module works with FIFO logic. It keeps each sensor value and
    #   position in an array and POP them back in correct position. This
    #   parameter must be provided.

    #enable: False

    #   Sensor enabled or disabled after power on. The default is to
    #   disable.

    #measurement_interval: 10

    #   The approximate distance (in mm) between sensor readings. The
    #   default is 10mm.
    #   Sensor readings are done with 10 mm intervals by default.
    #   If necessary you are free to change this setting

    #logging: False

    #   Out diameter to width.log in the /tmp folder. Can be turned on|off by
    #   command.

    #min_diameter: 1.0
    #max_diameter: 2.0

    #   Virtual filament_switch_sensor support. Creates a sensor named filament_width_sensor.
    #   Minimum/Maximum diameter for trigger virtual filament_switch_sensor.

    #extrude_factor_enable: False

    #   Enable extrusion modifier and/or use sensor as Runout/Clog switch only.
    #   'True' enables the extrusion modifier according to measured filament width.
    #   In any case the Module works as a Runout/Clog switch.

    #use_current_dia_while_delay: False

    #   Use the current diameter instead of the nominal diameter while
    #   the measurement delay has not run through.

    #Values from filament_switch_sensor. See the "filament_switch_sensor" section for information on these parameters.

    #pause_on_runout:

    #   Set this parameter to 'False' to disable the runout/clog switch.

    #runout_gcode:
    #insert_gcode:
    #event_delay:
    #pause_delay:

## Commands
**QUERY_FILAMENT_WIDTH** - Return the current measured filament width as result

**RESET_FILAMENT_WIDTH_SENSOR** - Clear all sensor readings. Can be used after filament change.

**DISABLE_FILAMENT_WIDTH_SENSOR** - Turn off the filament width sensor and stop using it to do flow control

**ENABLE_FILAMENT_WIDTH_SENSOR** - Turn on the filament width sensor and start using it to do flow control

**QUERY_RAW_FILAMENT_WIDTH** Return the current ADC channel values and RAW sensor value for calibration points

**ENABLE_FILAMENT_WIDTH_LOG** - Turn on diameter logging

**DISABLE_FILAMENT_WIDTH_LOG** - Turn off diameter logging

## Menu variables

**filament_width_sensor.Diameter** current measured filament width in mm

**filament_width_sensor.Raw** current raw measured filament width in units

**filament_width_sensor.is_active** Sensor on or off

## Template for menu variables
    [menu __main __filament __width_current]
    type: command
    enable: {'filament_width_sensor' in printer}
    name: Dia: {'%.2F' % printer.filament_width_sensor.Diameter}
    index: 0

    [menu __main __filament __raw_width_current]
    type: command
    enable: {'filament_width_sensor' in printer}
    name: Raw: {'%4.0F' % printer.filament_width_sensor.Raw}
    index: 1

## Calibration procedure

### Hall based sensor
Insert first calibration rod (1.5 mm size) get first raw sensor value

To get raw sensor value you can use menu item or  **QUERY_RAW_FILAMENT_WIDTH** command in terminal

Insert second calibration rod (2.0 mm size) get second raw sensor value

Save raw values in config

### Hall based sensor or TSL1401 based sensor

Insert first calibration rod (1.5 mm size) get first raw sensor value

Insert second calibration rod (2.0 mm size) get second raw sensor value

Repeat with rods of different/same diameters and note values.

Calculate slope and offset via linear regression (e.g. with Excel):

    y-values: diameters of the calibration rods.
    x-values: measured values for calibration rods.

    y = mx + b;
    m = slope 
    b = offset

Save raw values in config as:

    slope1:
    offset1:
    slope2: # Hall based sensor only
    offset2: # Hall based sensor only

## How to enable sensor
After power on by default sensor disabled.
Enable sensor in start g-code by command **ENABLE_FILAMENT_WIDTH_SENSOR** or change enable parameter in config

## Logging
After power on by default diameter Logging disabled.
Data to log added on every measurement interval (10 mm by default)

The log (width.log) is saved in the /tmp folder on your raspberry pi.
