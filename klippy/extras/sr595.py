# Code to create virtual pins for shift registers
# Tested with SN74HC595
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

class Sr:
    def __init__(self, printer, chip_count, data_pin, clock_pin, latch_pin):
        self.printer = printer
        self.toolhead = None
        self.chip_count = chip_count
        ppins = self.printer.lookup_object('pins')
        data_pin_params = ppins.lookup_pin(data_pin)
        clock_pin_params = ppins.lookup_pin(clock_pin)
        self.latch_pin = ppins.setup_pin('digital_out', latch_pin)

        mcu = data_pin_params['chip']
        if mcu is not clock_pin_params['chip']:
            raise self.printer.command_error(
                "Shift register pins must be on same mcu")

        self.old_data = -1

        sw_spi_pins = (
            data_pin_params['pin'],
            data_pin_params['pin'],
            clock_pin_params['pin']
        )
        self.spi = bus.MCU_SPI(mcu, None, None, 0, 500000, sw_spi_pins)

        self.latch_pin.setup_max_duration(0.)
        self.latch_pin.setup_start_value(0, 0)
        ppins.register_chip('sr', self)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object('toolhead')
        self.set_value(0)

    def setup_pin(self, pin_type, pin_params):
        return VirtualPin(pin_params, self)

    def get_status(self, eventtime):
        result = {
            'chip_count': self.chip_count,
            'registers': {}
        }
        register = result['registers']
        reg_cnt = 0
        for val in list((self.old_data >> i) & 0xFF for i in range(
                0, 8 * self.chip_count, 8)):
            register['register' + str(reg_cnt)] = {}
            pin = 0
            for i in bin(val)[2:].zfill(8)[::-1]:
                register['register' + str(reg_cnt)]['pin'+str(pin)] = i
                pin += 1
            reg_cnt += 1
        return result

    def _toggle_latch(self, print_time):
        self.latch_pin.set_digital(print_time, 0)
        self.latch_pin.set_digital(print_time + .05, 1)

    def set_value(self, value):
        if value == self.old_data:
            return
        self.old_data = value

        for val in list((value >> i) & 0xFF for i in range(
                0, 8 * self.chip_count, 8))[::-1]:
            self.spi.spi_send([val])

        self.toolhead.register_lookahead_callback(
            lambda print_time: self._toggle_latch(print_time))

    def set_pin(self, pin, bit_value):
        self.set_multiple_pins(((pin, bit_value),))

    def set_multiple_pins(self, bit_list):
        value = self.old_data
        for pin, bit_value in bit_list:
            self._is_valid_pin(pin)
            if bit_value == 0:
                value = self._clear_bit(value, pin)
            else:
                value = self._set_bit(value, pin)
        self.set_value(value)

    def toggle_pin(self, pin):
        self._is_valid_pin(pin)
        self.set_value(self._toggle_bit(self.old_data, pin))

    def _is_valid_pin(self, pin):
        if pin < 0 or pin >= 8 * self.chip_count:
            raise error("Invalid pin number %d - Must be between 0 and %d" %
                        (pin, (8 * self.chip_count) - 1))

    def _set_bit(self, value, offset):
        mask = 1 << offset
        return(value | mask)

    def _clear_bit(self, value, offset):
        mask = ~(1 << offset)
        return(value & mask)

    def _toggle_bit(self, value, offset):
        mask = 1 << offset
        return(value ^ mask)


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
