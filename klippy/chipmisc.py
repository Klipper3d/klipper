# Code to configure miscellaneous chips
#
# Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins


######################################################################
# Statically configured output pins
######################################################################

class PrinterStaticDigitalOut:
    def __init__(self, printer, config):
        pin_list = [pin.strip() for pin in config.get('pins').split(',')]
        for pin_desc in pin_list:
            mcu_pin = pins.setup_pin(printer, 'digital_out', pin_desc)
            mcu_pin.setup_static()

class PrinterStaticPWM:
    def __init__(self, printer, config):
        mcu_pwm = pins.setup_pin(printer, 'pwm', config.get('pin'))
        mcu_pwm.setup_max_duration(0.)
        hard_pwm = config.getint('hard_pwm', None, minval=1)
        if hard_pwm is None:
            mcu_pwm.setup_cycle_time(config.getfloat(
                'cycle_time', 0.100, above=0.))
        else:
            mcu_pwm.setup_hard_pwm(hard_pwm)
        scale = config.getfloat('scale', 1., above=0.)
        value = config.getfloat('value', minval=0., maxval=scale)
        mcu_pwm.setup_static_pwm(value / scale)


######################################################################
# Setup
######################################################################

def add_printer_objects(printer, config):
    for s in config.get_prefix_sections('static_digital_output '):
        printer.add_object(s.section, PrinterStaticDigitalOut(printer, s))
    for s in config.get_prefix_sections('static_pwm_output '):
        printer.add_object(s.section, PrinterStaticPWM(printer, s))
