# SNx4HC595 Shift Register Support via manual pin toggling
# http://www.ti.com/lit/ds/symlink/sn74hc595.pdf
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

PIN_MIN_TIME = 0.01

class error(Exception):
    pass


class SNx4HC595(object):
    error = error

    def __init__(self, printer, length, data_pin, latch_pin, clock_pin, output_enable_pin, clear_pin):
        self._printer = printer
        self._length = length
        self._data_pin = data_pin
        self._latch_pin = latch_pin
        self._clock_pin = clock_pin
        self._output_enable_pin = output_enable_pin
        self._clear_pin = clear_pin
        self._last_value_time = 0.
        self._reg_bits = self._bitfield(0)

    def write_bits(self, bits):
        if len(bits) > self._length:
            raise self.error("Invalid arg: bit length > SR length")
        self._set_pin(self._latch_pin, 0)
        for bit in bits:
            self._set_pin(self._data_pin, bit)
            self._set_pin(self._clock_pin, 1)
            self._set_pin(self._clock_pin, 0)
        self._set_pin(self._latch_pin, 1)
        self._reg_bits = bits

    def _set_pin(self, pin, value):
        if value not in [0., 1.]:
            raise self.error("Invalid pin value")
        print_time = self._printer.lookup_object('toolhead').get_last_move_time()
        print_time = max(print_time, self._last_value_time + PIN_MIN_TIME)
        pin.set_digital(print_time, value)
        self._last_value_time = print_time

    def _bitfield(self, n):
        return [1 if digit == '1' else 0 for digit in format(n, '0%ib' % self._length)]
