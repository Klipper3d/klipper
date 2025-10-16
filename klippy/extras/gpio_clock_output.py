# Define GPIO as clock output
#
# Copyright (C) 2025  Timofey Titovets <nefelim4ag@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterClockOutputPin:
    def __init__(self, config):
        self.printer = config.get_printer()
        ppins = self.printer.lookup_object('pins')
        self.mcu_pin = ppins.setup_pin('pwm', config.get('pin'))
        self.mcu = self.mcu_pin.get_mcu()
        self.frequency = config.getfloat('frequency', 100, above=3.33,
                                          maxval=520000000)
        # Determine start and shutdown values
        self.scale = config.getfloat('scale', 1., above=0.)
        self.value = config.getfloat('pulse_width', 0., minval=0.,
                                     maxval=self.scale)
        self.value = self.value / self.scale
        self.mcu.register_config_callback(self._build_config)
    def _build_config(self):
        # Check support
        self.mcu.lookup_command(
            "stm32_timer_output pin=%s cycle_ticks=%d on_ticks=%d")
        mcu_freq = self.mcu.seconds_to_clock(1.0)
        cycle_ticks = self.frequency // mcu_freq
        value = int(self.value * cycle_ticks)
        pin = self.mcu_pin['pin']
        self.mcu.add_config_cmd(
            "stm32_timer_output pin=%s cycle_ticks=%d on_ticks=%d"
            % (pin, cycle_ticks, value))

def load_config_prefix(config):
    return PrinterClockOutputPin(config)
