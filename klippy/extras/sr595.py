# Code to create virtual pins for shift registers
# Tested with SN74HC595
#
# [sr595 sr]
# data_pin:
# clock_pin:
# latch_pin:
#
# Turn on Qd / Q3 pin on the first shift register
# SET_SR_PIN SR=sr NUMBER=4 VALUE=1
#
# Turn off Qb / Q2 pin on the second shift register
# SET_SR_PIN SR=sr NUMBER=10 VALUE=0
#
# Set the value of all pins on all connected registers
# SR2 7-0   SR1 7-0
# 10101010  01010101    = 43605
# SET_SR_PINS SR=sr VALUE=43605
#
# Copyright (C) 2021  Heino Mortensen <heino@no-mercy.dk>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import bus


class error(Exception):
    pass


class _SrFromConfig:
    def __init__(self, config):
        printer = config.get_printer()
        self.chip_count = config.getint('chip_count', 1, minval=1, maxval=4)
        self.sr = Sr(printer, self.chip_count, config.get(
            'data_pin'), config.get('clock_pin'), config.get('latch_pin'))
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
        value = gcmd.get_int('VALUE', minval=0, maxval=1)
        pin = gcmd.get_int('NUMBER', minval=1, maxval=8 * self.chip_count) - 1
        self.sr.set_value(value, pin)

    cmd_SET_SR_PINS_help = "Set the value of all output pins"

    def cmd_SET_SR_PINS(self, gcmd):
        value = gcmd.get_int('VALUE', minval=0, maxval=255 * self.chip_count)
        self.sr.set_value(value)


class Sr:
    def __init__(self, printer, chip_count, data_pin, clock_pin, latch_pin, miso_pin='PD3'):
        self.printer = printer
        self.chip_count = chip_count
        ppins = self.printer.lookup_object('pins')
        data_pin_params = ppins.lookup_pin(data_pin)
        clock_pin_params = ppins.lookup_pin(clock_pin)
        miso_pin_params = ppins.lookup_pin(miso_pin)
        self.latch_pin = ppins.setup_pin('digital_out', latch_pin)

        mcu = data_pin_params['chip']
        if mcu is not clock_pin_params['chip']:
            raise self.printer.command_error(
                "Shift register pins must be on same mcu")

        self.old_data = self.data = 0

        sw_spi_pins = (
            miso_pin_params['pin'], data_pin_params['pin'], clock_pin_params['pin'])
        self.spi = bus.MCU_SPI(mcu, None, None, 0, 500000, sw_spi_pins)

        self.latch_pin.setup_max_duration(0.)
        self.latch_pin.setup_start_value(0, 0)

        ppins.register_chip('sr', self)

    def setup_pin(self, pin_type, pin_params):
        return VirtualPin(pin_params, self)

    def get_status(self, eventtime):
        return {'value': 1}

    def _toggle_latch(self, print_time, value):
        self.latch_pin.set_digital(print_time, 0)
        self.latch_pin.set_digital(print_time + .003, 1)

    def set_value(self, value):
        if value == self.old_data:
            return
        self.old_data = value

        for val in list((value >> i) & 0xFF for i in range(
                0, 8 * self.chip_count, 8))[::-1]:
            self.spi.spi_send([val])

        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda print_time: self._toggle_latch(print_time, value))

    def set_pin(self, bit_value, pin):
        self.set_multiple_pins(((pin, bit_value),))

    def set_multiple_pins(self, bit_list):
        value = self.old_data
        for pin, bit_value in bit_list:
            if pin < 0 or pin >= 8 * self.chip_count:
                raise error("Invalid pin number %d - Must be between 0 and %d" %
                            (pin, (8 * self.chip_count) - 1))
            if bit_value == 0:
                value = self._clear_bit(value, pin)
            else:
                value = self._set_bit(value, pin)
        self.set_value(value)

    def _set_bit(self, value, offset):
        mask = 1 << offset
        return(value | mask)

    def _clear_bit(self, value, offset):
        mask = ~(1 << offset)
        return(value & mask)


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
