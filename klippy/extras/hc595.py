# SNx4HC595 Shift Register Support via manual pin toggling
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging, mcu


class SNx4HC595(object):
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        self._mcu = mcu.get_printer_mcu(self.printer, self.name)
        ppins = self.printer.lookup_object('pins')
        self._data_pin = config.get('data_pin')
        ppins.setup_pin('digital_out', self._data_pin)
        self._latch_pin = config.get('latch_pin')
        ppins.setup_pin('digital_out', self._latch_pin)
        self._clock_pin = config.get('clock_pin')
        ppins.setup_pin('digital_out', self._clock_pin)
        self._mcu.register_config_callback(self._build_config)
        self._oid = self._mcu.create_oid()
        self._write_bits_cmd = None
        self._query_bits_cmd = None
        self.current_bits = 0

    def _build_config(self):
        self._mcu.add_config_cmd("config_hc595 oid=%d data_pin=%s latch_pin=%s clock_pin=%s" % (
            self._oid, self._data_pin, self._latch_pin, self._clock_pin))
        cmd_queue = self._mcu.alloc_command_queue()
        self._write_bits_cmd = self._mcu.lookup_command(
            "hc595_write_bits oid=%c value=%u", cq=cmd_queue)
        self._query_bits_cmd = self._mcu.lookup_command(
            "hc595_query_bits oid=%c", cq=cmd_queue)
        self._mcu.register_msg(self._update_host_bit_values, 'hc595_bits', self._oid)

    def set_bit(self, bit, value):
        self.write_bits(self.current_bits | (value << bit))

    def clear_bits(self, mask):
        self.write_bits(self.current_bits & ~mask)

    def write_bits(self, bits):
        self._write_bits_cmd.send([self._oid, bits])
        self.current_bits = bits

    def query_bits(self):
        self._query_bits_cmd.send([self._oid])

    def _update_host_bit_values(self, params):
        self.current_bits = params['value']
        logging.info("\n\n hc595 value: %s \n\n" % self.current_bits)


def load_config_prefix(config):
    return SNx4HC595(config)
