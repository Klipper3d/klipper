# Support for led strips
#
# Copyright (C) 2022  Christoph Schoening <schoning.christoph@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

MAX_SCHEDULE_TIME = 5.0


class PrinterLedStrip:
    def __init__(self, config):
        self.printer = config.get_printer()

        self.KEYS = ["RED", "GREEN", "BLUE"]

        # just use white if defined
        if config.get('pin_WHITE', None):
            self.KEYS += ["WHITE"]

        cycle_time = config.getfloat(
            'cycle_time', 0.100, above=0., maxval=MAX_SCHEDULE_TIME)
        self.default_cycle_time = cycle_time

        hardware_pwm = config.getboolean('hardware_pwm', False)
        ppins = self.printer.lookup_object('pins')

        def setup_pin(key):
            params = ppins.setup_pin('pwm', config.get('pin_%s' % key))
            params.setup_cycle_time(cycle_time, hardware_pwm)
            params.setup_max_duration(0.)

            value = config.getfloat('initial_%s' %
                                    key, 0., minval=0., maxval=1.)
            params.setup_start_value(value, value)

            return {'params': params, 'last_value': value}

        self.pins = {
            key: setup_pin(key) for key in self.KEYS
        }

        # register command
        name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_LED", "LED", name, self.cmd_SET_LED,
                                   desc=self.cmd_SET_LED_help)

    def get_status(self, eventtime):
        return {'values': [self.pins[key]['last_value'] for key in self.KEYS]}

    def _set_led(self, print_time, values, cycle_time):
        for key in self.KEYS:
            self.pins[key]['params'].set_pwm(
                print_time, values[key], cycle_time)
            self.pins[key]['last_value'] = values[key]

    cmd_SET_LED_help = "Set the color of a LED-Strip"

    def cmd_SET_LED(self, gcmd):
        cycle_time = gcmd.get_float('CYCLE_TIME', self.default_cycle_time,
                                    above=0., maxval=MAX_SCHEDULE_TIME)
        values = {
            key: gcmd.get_float(key, 0.,
                                minval=0., maxval=1.) for key in self.KEYS
        }
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self._set_led(print_time, values, cycle_time))


def load_config_prefix(config):
    return PrinterLedStrip(config)
