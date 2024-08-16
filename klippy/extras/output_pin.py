# PWM and digital output pin handling
#
# Copyright (C) 2017-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

PIN_MIN_TIME = 0.100
MAX_SCHEDULE_TIME = 5.0

class PrinterOutputPin:
    def __init__(self, config):
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        # Determine pin type
        self.is_pwm = config.getboolean('pwm', False)
        if self.is_pwm:
            self.mcu_pin = ppins.setup_pin('pwm', config.get('pin'))
            cycle_time = config.getfloat('cycle_time', 0.100, above=0.,
                                         maxval=MAX_SCHEDULE_TIME)
            hardware_pwm = config.getboolean('hardware_pwm', False)
            self.mcu_pin.setup_cycle_time(cycle_time, hardware_pwm)
            self.scale = config.getfloat('scale', 1., above=0.)
        else:
            self.mcu_pin = ppins.setup_pin('digital_out', config.get('pin'))
            self.scale = 1.
        self.mcu_pin.setup_max_duration(0.)
        self.last_print_time = 0.
        # Determine start and shutdown values
        self.last_value = config.getfloat(
            'value', 0., minval=0., maxval=self.scale) / self.scale
        self.shutdown_value = config.getfloat(
            'shutdown_value', 0., minval=0., maxval=self.scale) / self.scale
        self.mcu_pin.setup_start_value(self.last_value, self.shutdown_value)
        # Register commands
        pin_name = config.get_name().split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command("SET_PIN", "PIN", pin_name,
                                   self.cmd_SET_PIN,
                                   desc=self.cmd_SET_PIN_help)
    def get_status(self, eventtime):
        return {'value': self.last_value}
    def _set_pin(self, print_time, value):
        if value == self.last_value:
            return
        print_time = max(print_time, self.last_print_time + PIN_MIN_TIME)
        if self.is_pwm:
            self.mcu_pin.set_pwm(print_time, value)
        else:
            self.mcu_pin.set_digital(print_time, value)
        self.last_value = value
        self.last_print_time = print_time
    cmd_SET_PIN_help = "Set the value of an output pin"
    def cmd_SET_PIN(self, gcmd):
        # Read requested value
        value = gcmd.get_float('VALUE', minval=0., maxval=self.scale)
        value /= self.scale
        if not self.is_pwm and value not in [0., 1.]:
            raise gcmd.error("Invalid pin value")
        # Obtain print_time and apply requested settings
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self._set_pin(print_time, value))

def load_config_prefix(config):
    return PrinterOutputPin(config)
