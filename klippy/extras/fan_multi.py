# Printer cooling fan
#
# Copyright (C) 2016-2020  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import pulse_counter

FAN_MIN_TIME = 0.100

class FanMulti:
    cmd_SET_FAN_SPEED_help = "Sets the speed of a fan"
    def __init__(self, config, default_shutdown_speed=0.):
        self.printer = config.get_printer()
        self.fan_name = config.get_name().split()[-1]
        self.last_fan_value = 0.
        self.last_fan_time = 0.
        # Read config
        self.max_power = config.getfloat('max_power', 1., above=0., maxval=1.)
        self.kick_start_time = config.getfloat('kick_start_time', 0.1,
                                               minval=0.)
        self.off_below = config.getfloat('off_below', default=0.,
                                         minval=0., maxval=1.)
        cycle_time = config.getfloat('cycle_time', 0.010, above=0.)
        hardware_pwm = config.getboolean('hardware_pwm', False)
        shutdown_speed = config.getfloat(
            'shutdown_speed', default_shutdown_speed, minval=0., maxval=1.)
        # Setup pwm object
        ppins = self.printer.lookup_object('pins')
        self.mcu_fan = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu_fan.setup_max_duration(0.)
        self.mcu_fan.setup_cycle_time(cycle_time, hardware_pwm)
        shutdown_power = max(0., min(self.max_power, shutdown_speed))
        self.mcu_fan.setup_start_value(0., shutdown_power)

        self.enable_pin = None
        enable_pin = config.get('enable_pin', None)
        if enable_pin is not None:
            self.enable_pin = ppins.setup_pin('digital_out', enable_pin)
            self.enable_pin.setup_max_duration(0.)

        # Setup tachometer
        self.tachometer = FanTachometer(config)

        # Register callbacks
        self.printer.register_event_handler("gcode:request_restart",
                                            self._handle_request_restart)

        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_FAN_SPEED", "FAN",
                                   self.fan_name,
                                   self.cmd_SET_FAN_SPEED,
                                   desc=self.cmd_SET_FAN_SPEED_help)

    def get_mcu(self):
        return self.mcu_fan.get_mcu()
    def set_speed(self, print_time, value):
        if value < self.off_below:
            value = 0.
        value = max(0., min(self.max_power, value * self.max_power))
        if value == self.last_fan_value:
            return
        print_time = max(self.last_fan_time + FAN_MIN_TIME, print_time)
        if self.enable_pin:
            if value > 0 and self.last_fan_value == 0:
                self.enable_pin.set_digital(print_time, 1)
            elif value == 0 and self.last_fan_value > 0:
                self.enable_pin.set_digital(print_time, 0)
        if (value and value < self.max_power and self.kick_start_time
            and (not self.last_fan_value or value - self.last_fan_value > .5)):
            # Run fan at full speed for specified kick_start_time
            self.mcu_fan.set_pwm(print_time, self.max_power)
            print_time += self.kick_start_time
        self.mcu_fan.set_pwm(print_time, value)
        self.last_fan_time = print_time
        self.last_fan_value = value
    def set_speed_from_command(self, value):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback((lambda pt:
                                              self.set_speed(pt, value)))
    def _handle_request_restart(self, print_time):
        self.set_speed(print_time, 0.)

    def get_status(self, eventtime):
        tachometer_status = self.tachometer.get_status(eventtime)
        return {
            'speed': self.last_fan_value,
            'rpm': tachometer_status['rpm'],
        }

    def cmd_SET_FAN_SPEED(self, gcmd):
        speed = gcmd.get_float('SPEED', 0.)
        self.set_speed_from_command(speed)

class FanTachometer:
    def __init__(self, config):
        printer = config.get_printer()
        self._freq_counter = []

        for pin in config.get('tachometer_pins', None).split(','):
            if pin is not None:
                self.ppr = config.getint('tachometer_ppr', 2, minval=1)
                poll_time = config.getfloat('tachometer_poll_interval',
                                            0.0015, above=0.)
                sample_time = 1.
                self._freq_counter.append(pulse_counter.FrequencyCounter(
                    printer, pin, sample_time, poll_time))

    def get_status(self, eventtime):
        rpm_ = []
        for _freq_counter in self._freq_counter:
            if _freq_counter is not None:
                rpm = _freq_counter.get_frequency() * 30. / self.ppr
            else:
                rpm = None
            rpm_.append(rpm)

        return {'rpm': rpm_}

class PrinterFanMulti:
    def __init__(self, config):
        self.fan = FanMulti(config)
    def get_status(self, eventtime):
        return self.fan.get_status(eventtime)

def load_config_prefix(config):
    return PrinterFanMulti(config)
