# Support for HX711 ADC chip
#
# Copyright (C) 2023  Konstantin Vogel <konstantin.vogel@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging


class MCU_hx711:

    def __init__(self, main):
        self._main = main
        self.printer = main.printer
        self.reactor = self.printer.get_reactor()
        self.mcu = main.mcu
        self._last_value = 0.
        self._last_clock = 0
        self._callbacks = []
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self._build_config)
    def _build_config(self):
        cmd_queue = self.mcu.alloc_command_queue()
        self.mcu._serial.register_response(self._handle_adc_state, "hx711_in_state", self.oid)
        self.mcu.add_config_cmd(f"config_hx711 oid={self.oid} dout_pin={self._main.dout_pin} sck_pin={self._main.sck_pin} gain={self._main.gain}")
        self.query_adc_cmd = self.mcu.lookup_command("query_hx711 oid=%c enable=%u endstop_oid=%i", cq=cmd_queue)
    def setup_adc_callback(self, report_time, callback):
        self._callbacks.append(callback)
    def _handle_adc_state(self, params):
        params['value'] *= self._main.direction
        self._last_value = params['value']
        self._last_clock = params['clock']
        for cb in self._callbacks:
            cb(self._last_clock, self._last_value)

class PrinterHx711:

    def __init__(self, config):
        self.printer = config.get_printer()
        self.config = config
        self.name = config.get_name().split()[1]
        ppins = self.printer.lookup_object('pins')
        dout_pin_params = ppins.lookup_pin(config.get('dout_pin'))
        sck_pin_params = ppins.lookup_pin(config.get('sck_pin'))
        self.dout_pin = dout_pin_params['pin']
        self.sck_pin = sck_pin_params['pin']
        self.mcu = dout_pin_params['chip']
        self.gain = config.getchoice('gain', {32: 2, 64: 3, 128: 1}, default=64)
        self.direction = -1 if config.getboolean('invert', False) else 1
        ppins.register_chip(self.name, self)

    def setup_pin(self, pin_type, pin_params):
        self.sensor = MCU_hx711(self)
        return self.sensor

def load_config_prefix(config):
    return PrinterHx711(config)
