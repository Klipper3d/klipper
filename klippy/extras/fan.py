# Printer cooling fan
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

FAN_MIN_TIME = 0.100

class PrinterFan:
    def __init__(self, config, default_shutdown_speed=0.):
        self.last_fan_value = 0.
        self.last_fan_time = 0.
        self.printer = config.get_printer()
        self.printer.register_event_handler("gcode:request_restart",
                                            self.handle_request_restart)
        self.max_power = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.kick_start_time = config.getfloat('kick_start_time', 0.1,
                                               minval=0.)
        self.off_below = config.getfloat(
            'off_below', default=0., minval=0., maxval=1.)
        ppins = self.printer.lookup_object('pins')
        self.mcu_fan = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu_fan.setup_max_duration(0.)
        cycle_time = config.getfloat('cycle_time', 0.010, above=0.)
        hardware_pwm = config.getboolean('hardware_pwm', False)
        self.mcu_fan.setup_cycle_time(cycle_time, hardware_pwm)
        shutdown_speed = config.getfloat(
            'shutdown_speed', default_shutdown_speed, minval=0., maxval=1.)
        self.mcu_fan.setup_start_value(
            0., max(0., min(self.max_power, shutdown_speed)))
        # Register commands
        if config.get_name() == 'fan':
            gcode = self.printer.lookup_object('gcode')
            gcode.register_command("M106", self.cmd_M106)
            gcode.register_command("M107", self.cmd_M107)
    def handle_request_restart(self, print_time):
        self.set_speed(print_time, 0.)
    def set_speed(self, print_time, value):
        if value < self.off_below:
            value = 0.
        value = max(0., min(self.max_power, value * self.max_power))
        if value == self.last_fan_value:
            return
        print_time = max(self.last_fan_time + FAN_MIN_TIME, print_time)
        if (value and value < self.max_power and self.kick_start_time
            and (not self.last_fan_value or value - self.last_fan_value > .5)):
            # Run fan at full speed for specified kick_start_time
            self.mcu_fan.set_pwm(print_time, self.max_power)
            print_time += self.kick_start_time
        self.mcu_fan.set_pwm(print_time, value)
        self.last_fan_time = print_time
        self.last_fan_value = value
    def get_status(self, eventtime):
        return {'speed': self.last_fan_value}
    def cmd_M106(self, params):
        # Set fan speed
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        gcode = self.printer.lookup_object('gcode')
        value = gcode.get_float('S', params, 255., minval=0.) / 255.
        self.set_speed(print_time, value)
    def cmd_M107(self, params):
        # Turn fan off
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.set_speed(print_time, 0.)

def load_config(config):
    return PrinterFan(config)
