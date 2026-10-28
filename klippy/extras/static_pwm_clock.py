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
        self.mcu_pin = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu = self.mcu_pin.get_mcu()
        self.frequency = config.getfloat('frequency', 100, above=(1/0.3),
                                          maxval=520000000)
        self.mcu_pin.setup_cycle_time(1. / self.frequency, True)
        self.mcu_pin.setup_max_duration(0.)
        self.mcu_pin.setup_start_value(0.5, 0.5)
        self.mcu.register_config_callback(self._build_config)
    def _build_config(self):
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
        if value/cycle_ticks < 0.4:
            logging.warning("[%s] pulse width < 40%%" % (self.name))
        if value/cycle_ticks > 0.6:
            logging.warning("[%s] pulse width > 60%%" % (self.name))

def load_config_prefix(config):
    return PrinterClockOutputPin(config)
