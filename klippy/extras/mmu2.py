# Prusa MMU2 support.
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins, mcu

FLASH_DELAY = .1


class MMU2:
    sr_pins = {"stepper_1_dir": 0, "stepper_1_enable": 1,
               "stepper_2_dir": 2, "stepper_2_enable": 3,
               "stepper_3_dir": 4, "stepper_3_enable": 5,
               "green_led_1": 8, "red_led_1": 9,
               "green_led_2": 10, "red_led_2": 11,
               "green_led_3": 12, "red_led_3": 13,
               "green_led_4": 14, "red_led_4": 15,
               "green_led_5": 6, "red_led_5": 7}

    mmu_leds = [sr_pins["green_led_1"], sr_pins["red_led_1"],
                sr_pins["green_led_2"], sr_pins["red_led_2"],
                sr_pins["green_led_3"], sr_pins["red_led_3"],
                sr_pins["green_led_4"], sr_pins["red_led_4"],
                sr_pins["green_led_5"], sr_pins["red_led_5"]]

    mmu_led_mask = 0b1111111111000000

    def __init__(self, config):
        logging.info("Initializing Klippy MMU2 Module")
        self._printer = config.get_printer()
        self._mcu_name = config.get('mcu', None)
        if self._mcu_name is None:
            raise config.error("mmu2 mcu not specified")
        self.reactor = self._printer.get_reactor()
        self._shift_register = None
        self._mcu = mcu.get_printer_mcu(self._printer, self._mcu_name)
        self._mcu.register_config_callback(self._build_config)
        self._rs_led = 0

    def _build_config(self):
        self._shift_register = self._printer.lookup_object("hc595 %s" % self._mcu_name)

    def printer_state(self, state):
        if state == 'ready':
            self.reactor.register_timer(self._ready_led_sequence, self.reactor.NOW)

    def _ready_led_sequence(self, time):
        self._shift_register.set_bit(self.mmu_leds[self._rs_led], 1)
        self._rs_led += 1
        if self._rs_led is 10:
            self.reactor.register_timer(self._clear_led, (time + FLASH_DELAY * 5))
        return time + FLASH_DELAY if self._rs_led < 10 else self.reactor.NEVER

    def _clear_led(self, time):
        self._shift_register.clear_bits(self.mmu_led_mask)
        return self.reactor.NEVER


def load_config(config):
    return MMU2(config)
