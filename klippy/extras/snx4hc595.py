# SNx4HC595 Shift Register Support via manual pin toggling
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import copy, logging

PIN_MIN_TIME = 0.01

class error(Exception):
    pass

class SNx4HC595(object):
    error = error

    def __init__(self, printer, length, data_pin, latch_pin, clock_pin, output_enable_pin=None, clear_pin=None):
        self._printer = printer
        self._length = length
        self._data_pin = data_pin
        self._latch_pin = latch_pin
        self._clock_pin = clock_pin
        self._output_enable_pin = output_enable_pin
        self._clear_pin = clear_pin
        self._last_value_time = 0.
        self._reg_bits = self._bitfield(0)
        self._printer.add_object("sr%i_%s_%s_%s" % (length, data_pin, latch_pin, clock_pin), self)

    def printer_state(self, state):
        if state == 'connect':
            self.clear_bits()

    def current_bits(self):
        return copy.deepcopy(self._reg_bits)

    # Best case time cost of this function:
    #   (self._length * 3 + 2) * PIN_MIN_TIME
    def write_bits(self, bits):
        if len(bits) > self._length:
            raise self.error("Invalid arg: bit length > SR length")
        self._set_pin(self._latch_pin, 0)
        for bit in reversed(bits):
            self._set_pin(self._data_pin, bit)
            self._set_pin(self._clock_pin, 1)
            self._set_pin(self._clock_pin, 0)
        self._set_pin(self._latch_pin, 1)
        self._reg_bits = bits

    def set_bit(self, bit, value):
        if value not in [0, 1]:
            raise self.error("invalid bit value %i" % bit)
        updated_bits = self.current_bits()
        updated_bits[bit] = value
        self.write_bits(updated_bits)

    def get_bit(self, bit):
        return self._reg_bits[bit]

    def clear_bits(self):
        if self._clear_pin is not None:
            self._set_pin(self._clear_pin, 1)
            self._set_pin(self._clear_pin, 0)
        else:
            logging.info("clearing sr_%i" % self._length)
            self.write_bits(self._bitfield(0))

    def enable_output(self):
        if self._output_enable_pin is not None:
            self._set_pin(self._output_enable_pin, 1)

    def disable_output(self):
        if self._output_enable_pin is not None:
            self._set_pin(self._output_enable_pin, 0)

    def _set_pin(self, pin, value):
        if value not in [0., 1.]:
            raise self.error("Invalid pin value")
        print_time = self._printer.lookup_object('toolhead').get_last_move_time()
        print_time = max(print_time, self._last_value_time + PIN_MIN_TIME)
        pin.set_digital(print_time, value)
        self._last_value_time = print_time

    def _bitfield(self, n):
        return [1 if digit == '1' else 0 for digit in format(n, '0%ib' % self._length)]
