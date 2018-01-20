# Code to configure miscellaneous chips
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins


######################################################################
# Output pins
######################################################################

class PrinterStaticDigitalOut:
    def __init__(self, printer, config):
        pin_list = [pin.strip() for pin in config.get('pins').split(',')]
        for pin_desc in pin_list:
            mcu_pin = pins.setup_pin(printer, 'digital_out', pin_desc)
            mcu_pin.setup_start_value(1, 1, True)

PIN_MIN_TIME = 0.100

class PrinterPin:
    def __init__(self, printer, config):
        self.printer = printer
        self.is_pwm = 'pwm' in config.get_name().split()[0]
        if self.is_pwm:
            self.mcu_pin = pins.setup_pin(printer, 'pwm', config.get('pin'))
            hard_pwm = config.getint('hard_pwm', None, minval=1)
            if hard_pwm is None:
                self.mcu_pin.setup_cycle_time(config.getfloat(
                    'cycle_time', 0.100, above=0.))
            else:
                self.mcu_pin.setup_hard_pwm(hard_pwm)
            self.scale = config.getfloat('scale', 1., above=0.)
        else:
            self.mcu_pin = pins.setup_pin(
                printer, 'digital_out', config.get('pin'))
            self.scale = 1.
        self.mcu_pin.setup_max_duration(0.)
        self.last_value_time = 0.
        self.last_value = config.getfloat(
            'value', 0., minval=0., maxval=self.scale) / self.scale
        self.is_static = config.get_name().startswith('static_')
        if self.is_static:
            self.mcu_pin.setup_start_value(
                self.last_value, self.last_value, True)
        else:
            shutdown_value = config.getfloat(
                'shutdown_value', 0., minval=0., maxval=self.scale) / self.scale
            self.mcu_pin.setup_start_value(self.last_value, shutdown_value)
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command("SET_PIN", self.cmd_SET_PIN,
                                    desc=self.cmd_SET_PIN_help)
    cmd_SET_PIN_help = "Set the value of an output pin"
    def cmd_SET_PIN(self, params):
        pin_name = self.gcode.get_str('PIN', params)
        pin = self.printer.lookup_object('pin ' + pin_name, None)
        if pin is not self:
            if pin is None:
                raise self.gcode.error("Pin not configured")
            return pin.cmd_SET_PIN(params)
        if self.is_static:
            raise self.gcode.error("Static pin can not be changed at run-time")
        value = self.gcode.get_float('VALUE', params) / self.scale
        if value == self.last_value:
            return
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        print_time = max(print_time, self.last_value_time + PIN_MIN_TIME)
        if self.is_pwm:
            if value < 0. or value > 1.:
                raise self.gcode.error("Invalid pin value")
            self.mcu_pin.set_pwm(print_time, value)
        else:
            if value not in [0., 1.]:
                raise self.gcode.error("Invalid pin value")
            self.mcu_pin.set_digital(print_time, value)
        self.last_value = value
        self.last_value_time = print_time


######################################################################
# Setup
######################################################################

def add_printer_objects(printer, config):
    for s in config.get_prefix_sections('static_digital_output '):
        printer.add_object(s.section, PrinterStaticDigitalOut(printer, s))
    for s in config.get_prefix_sections('digital_output '):
        printer.add_object('pin' + s.section[14:], PrinterPin(printer, s))
    for s in config.get_prefix_sections('static_pwm_output '):
        printer.add_object('pin' + s.section[17:], PrinterPin(printer, s))
    for s in config.get_prefix_sections('pwm_output '):
        printer.add_object('pin' + s.section[10:], PrinterPin(printer, s))
