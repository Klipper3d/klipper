# Support for emergency stop buttons
#
# Copyright (C) 2022 Rene Henzinger <chibisuke@web.de>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class EStopButton:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.pin = config.get('pin')
        buttons = self.printer.load_object(config, "buttons")
        buttons.register_buttons([self.pin], self.button_callback)

    def button_callback(self, eventtime, state):
        if state:
            self.printer.action_emergency_stop()

def load_config(config):
    return EStopButton(config)
