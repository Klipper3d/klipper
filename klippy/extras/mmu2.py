# Prusa MMU2 support.
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins, mcu
from snx4hc595 import SNx4HC595

FLASH_DELAY = .5

class MMU2:
    def __init__(self, config):
        logging.info("Initializing Klippy MMU2 Module")
        self._printer = config.get_printer()
        self._mcu_name = config.get('mcu', None)
        if self._mcu_name is None:
            raise config.error("mmu2 mcu not specified")
        ppins = self._printer.lookup_object('pins')
        self._mcu = mcu.get_printer_mcu(self._printer, self._mcu_name)
        self._sr16_data_pin = ppins.setup_pin('digital_out', '%s:PB5' % self._mcu_name)
        self._sr16_latch_pin = ppins.setup_pin('digital_out', '%s:PB6' % self._mcu_name)
        self._sr16_clock_pin = ppins.setup_pin('digital_out', '%s:PC7' % self._mcu_name)
        self._shift_register = SNx4HC595(self._printer, 16, self._sr16_data_pin, self._sr16_latch_pin,
                                         self._sr16_clock_pin, None, None)
        self._mcu.register_config_callback(self._build_config)

        self.reactor = self._printer.get_reactor()
        self.left_flash = None
        self.right_flash = None

    def _build_config(self):
        logging.info("Configuring Klippy MMU2 Module")

    def _get_reactor(self):
        return self._printer.get_reactor()

    def printer_state(self, state):
        if state == 'ready':
            self.left_flash = self.reactor.register_timer(self.left_leds, FLASH_DELAY/2)
            self.right_flash = self.reactor.register_timer(self.right_leds, FLASH_DELAY)

    def left_leds(self, time):
        self._shift_register.write_bits([1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0])
        return time + FLASH_DELAY

    def right_leds(self, time):
        self._shift_register.write_bits([0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0])
        return time + FLASH_DELAY


def load_config(config):
    return MMU2(config)
