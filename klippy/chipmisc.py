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
# AD5206 digipot
######################################################################

class ad5206:
    def __init__(self, printer, config):
        enable_pin_params = pins.get_printer_pins(printer).parse_pin_desc(
            config.get('enable_pin'))
        self.mcu = enable_pin_params['chip']
        self.pin = enable_pin_params['pin']
        self.mcu.add_config_object(self)
        scale = config.getfloat('scale', 1., above=0.)
        self.channels = [None] * 6
        for i in range(len(self.channels)):
            val = config.getfloat('channel_%d' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.channels[i] = int(val * 256. / scale + .5)
    def build_config(self):
        for i, val in enumerate(self.channels):
            if val is not None:
                self.mcu.add_config_cmd(
                    "send_spi_message pin=%s msg=%02x%02x" % (self.pin, i, val))


######################################################################
# Setup
######################################################################

def add_printer_objects(printer, config):
    for s in config.get_prefix_sections('static_digital_output '):
        printer.add_object(s.section, PrinterStaticDigitalOut(printer, s))
    for s in config.get_prefix_sections('static_pwm_output '):
        printer.add_object(s.section, PrinterStaticPWM(printer, s))
    for s in config.get_prefix_sections('ad5206 '):
        printer.add_object(s.section, ad5206(printer, s))
