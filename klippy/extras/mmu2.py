# Prusa MMU2 support.
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins, mcu
from snx4hc595 import SNx4HC595

FLASH_DELAY = 1.1


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

    def __init__(self, config):
        logging.info("Initializing Klippy MMU2 Module")
        self._printer = config.get_printer()
        self._mcu_name = config.get('mcu', None)
        if self._mcu_name is None:
            raise config.error("mmu2 mcu not specified")
        ppins = self._printer.lookup_object('pins')
        self._mcu = mcu.get_printer_mcu(self._printer, self._mcu_name)
        ppins.register_chip('%s_sr16' % self._mcu_name, self)
        sr16_data_pin = ppins.setup_pin('digital_out', '%s:PB5' % self._mcu_name)
        sr16_latch_pin = ppins.setup_pin('digital_out', '%s:PB6' % self._mcu_name)
        sr16_clock_pin = ppins.setup_pin('digital_out', '%s:PC7' % self._mcu_name)
        self._shift_register = SNx4HC595(self._printer, 16, sr16_data_pin, sr16_latch_pin, sr16_clock_pin)
        self._mcu.register_config_callback(self._build_config)

        self.reactor = self._printer.get_reactor()

    def _build_config(self):
        logging.info("Configuring Klippy MMU2 Module")

    def printer_state(self, state):
        if state == 'ready':
            self.led = 0
            self.inc = 1
            def ready_sequence(time):
                if self.led is 9 and self.inc is 0:
                    self.inc = -1
                self.set_led(self.mmu_leds[self.led], 1 if self.inc > 0 else 0)
                if self.led is 9 and self.inc is 1:
                    self.inc = 0
                self.led += self.inc
                if self.led is -1:
                    self.led = 0
                    self.inc = 1
                return time + FLASH_DELAY
#                return FLASH_DELAY if led >= 0 else self.reactor.NEVER
            self.reactor.register_timer(ready_sequence, self.reactor.NOW)

    def set_led(self, bit, value):
        logging.info("set %i = %i" % (bit, value))
        self._shift_register.set_bit(bit, value)

    def setup_pin(self, pin_type, pin_params):
        if pin_type is not 'digital_out':
            raise pins.error("Pin type not supported on mmu2_sr16")
        pin = pin_params['pin']
        if pin not in self.pins:
            raise pins.error("Unknown mmu2_sr16 pin %s" % (pin,))


def load_config(config):
    return MMU2(config)
