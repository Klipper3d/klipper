# Code to create virtual pins for shift registers
# Tested with SN74HC595
#
# Copyright (C) 2021  Heino Mortensen <heino@no-mercy.dk>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus

class _SrFromConfig:
    def __init__(self, config):
        printer = config.get_printer()
        self.sr = Sr(printer, config.get('data_pin'), config.get('clock_pin'), config.get('latch_pin'))
        gcode = printer.lookup_object('gcode')
        
        name = config.get_name().split()[1]
        gcode.register_mux_command("SET_SR_PIN", "SR", name,
                                    self.cmd_SET_SR_PIN,
                                    desc=self.cmd_SET_SR_PIN_help)

        gcode.register_mux_command("SET_SR_PINS", "SR", name,
                                    self.cmd_SET_SR_PINS,
                                    desc=self.cmd_SET_SR_PINS_help)

    cmd_SET_SR_PIN_help = "Set the value of an output pin"
    def cmd_SET_SR_PIN(self, gcmd):
        value = gcmd.get_int('VALUE', minval=0, maxval=255)
        pin = gcmd.get_int('NUMBER', minval=1, maxval=8) - 1
        self.sr.set_value(value, pin)

    cmd_SET_SR_PINS_help = "Set the value of all output pins"
    def cmd_SET_SR_PINS(self, gcmd):
        value = gcmd.get_int('VALUE', minval=0, maxval=255)
        self.sr.set_value(value)
    

class Sr:
    def __init__(self, printer, data_pin, clock_pin, latch_pin, miso_pin='PD3'):
        self.printer = printer
        ppins = self.printer.lookup_object('pins')
        data_pin_params = ppins.lookup_pin(data_pin)
        clock_pin_params = ppins.lookup_pin(clock_pin)
        miso_pin_params = ppins.lookup_pin(miso_pin)
        self.latch_pin = ppins.setup_pin('digital_out', latch_pin)
        

        mcu = data_pin_params['chip']
        if mcu is not clock_pin_params['chip']:
            raise self.printer.command_error("Shift register pins must be on same mcu")

        self.old_data = self.data = 0

        sw_spi_pins = (miso_pin_params['pin'], data_pin_params['pin'], clock_pin_params['pin'])
        self.spi = bus.MCU_SPI(mcu, None, None, 0, 500000, sw_spi_pins)

        self.latch_pin.setup_max_duration(0.)
        self.latch_pin.setup_start_value(0, 0)
        
        ppins.register_chip('sr', self)

    def setup_pin(self, pin_type, pin_params):
        vp = VirtualPin(pin_params, self)
        return vp

    def get_status(self, eventtime):
        return {'value': 1}

    def _set_register_value(self, print_time, value):
        if value == self.old_data:
            return
        self.old_data = value

        self.latch_pin.set_digital(print_time, 0)
        print_time += 0.003
        self.spi.spi_send([value])
        print_time += 0.003

        self.latch_pin.set_digital(print_time, 1)
        print_time += 0.003

    def set_value(self, value, pin=None):
        if pin is not None:
            value = self._clearBit(self.old_data, int(pin)) if value == 0 else self._setBit(self.old_data, int(pin))
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self._set_register_value(print_time, value))

    def _setBit(self, int_type, offset):
       mask = 1 << offset
       return(int_type | mask)
   
    def _clearBit(self, int_type, offset):
       mask = ~(1 << offset)
       return(int_type & mask)

class VirtualPin:
    def __init__(self, pin_params, sr):
        self.pin_params = pin_params
        self.sr = sr

    def setup_max_duration(self, max_duration):
        pass
    def setup_start_value(self, start_value, shutdown_value):
        pass
    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        pass

    def set_digital(self, print_time, value):
        if self.pin_params['invert'] == 1:
            value = int(value) ^ 1
        self.sr.set_value(value, int(self.pin_params['pin']))

    def set_pwm(self, print_time, value, cycle_time=None):
        pass

def load_config_prefix(config):
    return _SrFromConfig(config)
