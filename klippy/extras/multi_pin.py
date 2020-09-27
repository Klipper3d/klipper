# Virtual pin that propagates its changes to multiple output pins
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterMultiPin:
    def __init__(self, config):
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        try:
            ppins.register_chip('multi_pin', self)
        except ppins.error:
            pass
        self.pin_type = None
        self.pin_list = [pin.strip() for pin in config.get('pins').split(',')]
        self.mcu_pins = []
    def setup_pin(self, pin_type, pin_params):
        ppins = self.printer.lookup_object('pins')
        pin_name = pin_params['pin']
        pin = self.printer.lookup_object('multi_pin ' + pin_name, None)
        if pin is not self:
            if pin is None:
                raise ppins.error("multi_pin %s not configured" % (pin_name,))
            return pin.setup_pin(pin_type, pin_params)
        if self.pin_type is not None:
            raise ppins.error("Can't setup multi_pin %s twice" % (pin_name,))
        self.pin_type = pin_type
        invert = ""
        if pin_params['invert']:
            invert = "!"
        self.mcu_pins = [ppins.setup_pin(pin_type, invert + pin_desc)
                         for pin_desc in self.pin_list]
        return self
    def get_mcu(self):
        return self.mcu_pins[0].get_mcu()
    def setup_max_duration(self, max_duration):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_max_duration(max_duration)
    def setup_start_value(self, start_value, shutdown_value):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_start_value(start_value, shutdown_value)
    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        for mcu_pin in self.mcu_pins:
            mcu_pin.setup_cycle_time(cycle_time, hardware_pwm)
    def set_digital(self, print_time, value):
        for mcu_pin in self.mcu_pins:
            mcu_pin.set_digital(print_time, value)
    def set_pwm(self, print_time, value, cycle_time=None):
        for mcu_pin in self.mcu_pins:
            mcu_pin.set_pwm(print_time, value, cycle_time)

def load_config_prefix(config):
    return PrinterMultiPin(config)
