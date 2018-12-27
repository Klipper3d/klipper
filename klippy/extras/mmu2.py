# Prusa MMU2 support.
#
# Copyright (C) 2018  Trevor Jones <trevorjones141@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins, mcu


class MMU2:
    FLASH_DELAY = .1

    sr_pins = {"stepper_1_dir": 'PG0', "stepper_1_enable": 'PG1',
               "stepper_2_dir": 'PG2', "stepper_2_enable": 'PG3',
               "stepper_3_dir": 'PG4', "stepper_3_enable": 'PG5'}

    def __init__(self, config):
        logging.info("Initializing Klippy MMU2 Module")
        self.printer = config.get_printer()
        self._mcu_name = config.get('mcu', None)
        if self._mcu_name is None:
            raise config.error("mmu2 mcu not specified")
        self.pins = self.printer.lookup_object("pins")
        self.reactor = self.printer.get_reactor()
        self._mcu = mcu.get_printer_mcu(self.printer, self._mcu_name)
        self._mcu.register_config_callback(self._build_config)
        self._rs_led = 0

        pins.MCU_PINS["atmega32u4"] = pins.port_pins(8)

        self.led_g0 = self.pins.setup_pin('digital_out', "%s:PH0" % self._mcu_name)
        self.led_r0 = self.pins.setup_pin('digital_out', "%s:PH1" % self._mcu_name)
        self.led_g1 = self.pins.setup_pin('digital_out', "%s:PH2" % self._mcu_name)
        self.led_r1 = self.pins.setup_pin('digital_out', "%s:PH3" % self._mcu_name)
        self.led_g2 = self.pins.setup_pin('digital_out', "%s:PH4" % self._mcu_name)
        self.led_r2 = self.pins.setup_pin('digital_out', "%s:PH5" % self._mcu_name)
        self.led_g3 = self.pins.setup_pin('digital_out', "%s:PH6" % self._mcu_name)
        self.led_r3 = self.pins.setup_pin('digital_out', "%s:PH7" % self._mcu_name)
        self.led_g4 = self.pins.setup_pin('digital_out', "%s:PG6" % self._mcu_name)
        self.led_r4 = self.pins.setup_pin('digital_out', "%s:PG7" % self._mcu_name)
        self.leds = [self.led_g0, self.led_r0,
                     self.led_g1, self.led_r1,
                     self.led_g2, self.led_r2,
                     self.led_g3, self.led_r3,
                     self.led_g4, self.led_r4]

        for led in self.leds:
            led.setup_max_duration(0.)

    def _build_config(self):
        logging.info("config mmu2")

    def printer_state(self, state):
        if state == 'ready':
            self.reactor.register_timer(self._ready_led_sequence, self.reactor.NOW)

    def _ready_led_sequence(self, time):
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.leds[self._rs_led].set_digital(print_time, 1)
        self._rs_led += 1
        if self._rs_led is 10:
            self.reactor.register_timer(self._clear_led, (time + self.FLASH_DELAY * 5))
        return time + self.FLASH_DELAY if self._rs_led < 10 else self.reactor.NEVER

    def _clear_led(self, time):
        if self._rs_led == 10:
            self._rs_led -= 1
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.leds[self._rs_led].set_digital(print_time, 0)
        self._rs_led -= 1
        return self.reactor.NEVER if self._rs_led == -1 else time + self.FLASH_DELAY


def load_config(config):
    return MMU2(config)
