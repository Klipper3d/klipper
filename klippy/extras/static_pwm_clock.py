# Define GPIO as clock output
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class PrinterClockOutputPin:
    def __init__(self, config):
        self.name = config.get_name()
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        mcu_pin = ppins.lookup_pin(config.get('pin'), can_invert=True)
        self.mcu = mcu_pin["chip"]
        self.pin = mcu_pin["pin"]
        self.invert = mcu_pin["invert"]
        self.frequency = config.getfloat('frequency', 100, above=(1/0.3),
                                          maxval=520000000)
        self.mcu.register_config_callback(self._build_config)
    def _build_config(self):
        self.mcu.lookup_command(
            "stm32_timer_output pin=%u cycle_ticks=%u on_ticks=%hu")
        mcu_freq = self.mcu.seconds_to_clock(1.0)
        cycle_ticks = int(mcu_freq // self.frequency)
        # validate frequency
        mcu_freq_rev = int(cycle_ticks * self.frequency)
        if mcu_freq_rev != mcu_freq:
            msg = """
            # Frequency output must be without remainder, %i != %i
            [%s]
            frequency = %f
            """ % (mcu_freq, mcu_freq_rev, self.name, self.frequency)
            raise self.printer.config_error(msg)
        value = int(0.5 * cycle_ticks)
        if self.invert:
            value = cycle_ticks - value
        if value/cycle_ticks < 0.4:
            logging.warning("[%s] pulse width < 40%%" % (self.name))
        if value/cycle_ticks > 0.6:
            logging.warning("[%s] pulse width > 60%%" % (self.name))
        self.mcu.add_config_cmd(
            "stm32_timer_output pin=%s cycle_ticks=%d on_ticks=%d"
            % (self.pin, cycle_ticks, value))

def load_config_prefix(config):
    return PrinterClockOutputPin(config)
