This document describes Filament Jam Sensor module. Hardware used for developing this module is based on optical endstop or encoder(1 line) count the filament usage within one LOW/HIGH cycle. You can find designs at [thingiverse.com](https://www.thingiverse.com/thing:3067904)

## How does it work?
Filaments spins the wheel in the sensor, the sensor goes LOW to HIGH. The module calculates the filament usage between a LOW/HIGH cycle compare with base line(setting base_usage) usage.
If the usage reached slow%(setting slow_usage) will trigger the slow down action.
If the usage reached jam%(setting jam_usage) will trigger the jam/pause action.

## Configuration
    # Pause/Resume module must define
    [pause_resume]
    #recover_velocity: 50.
    #
    # on/off / encoder(user only 1 line) Filament Jam Sensor
    [filament_jam_sensor jamsensor]
    pin: ar22
    timer: 1.0
    # timer ; in sec ; setup how often the system check the usage ; default = 1.0
    extruder_index: 0
    # sequence of the extruder to monitor ;
    # 0 mean 1st extruder , 1 mean 2nd extruder
    base_usage: 3.85
    # base_usage ; in mm (must define) for one up/down cycle
    slow_usage: 200.0
    # slow_usage ; in % (must define) (must > 100.0) for system detect in slow
    # and will trigger slow_speed & slow_gcode
    # 0 to disable slow down function.
    slow_speed: 10.0
    # slow_speed ; in % that the printer (speed_factor) will show down
    # default 10.0
    slow_speed_resume: 1.0
    # slow_speed_resume ; in % that the printer (speed_factor) will speed up
    # default 1.0
    slow_gcode: GET_POSITION
    # gcode that will run at slow_usage triggered
    jam_usage: 400.0
    # jam_usage ; in % (must define) (must > 100.0) (must > slow_usage) for system
    # detect in Jam or filament run out and will trigger PAUSE & jam_gcode
    jam_gcode: M300
    # gcode that will run at jam_usage triggered

## Commands
  - `SET_FILAMENT_JAM_SENSOR SENSOR=<sensor name> [TIMER=<sec>] [BASE_USAGE=<mm>]
    [SLOW_USAGE=<%>] [SLOW_SPEED=<slow%>] [SLOW_SPEED_RESUME=<fast%>]
    [JAM_USAGE=<%>] [ACTION=<0|1>] [ENABLE=<0|1>] [DEBUG=<0|1>]
    `:  Configures the [filament_jam_sensor] module. TIMER, BASE_USAGE, SLOW_USAGE,
    SLOW_SPEED, SLOW_SPEED_RESUME, JAM_USAGE correspond to same name in configuration.
    ACTION default is 1 and set to 0 will bypass the action of slow down/speed up,
    pause and gcode execute defined in the config. ENABLE default is 1 and set to 0
    will disable the checking logic of this module. DEBUG default is 0 and set to 1
    will output the information for debug.
