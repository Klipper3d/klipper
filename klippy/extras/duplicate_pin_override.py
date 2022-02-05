# Tool to disable config checks for duplicate pins
#
# Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterDupPinOverride:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        for pin_desc in config.getlist('pins'):
            ppins.allow_multi_use_pin(pin_desc)

def load_config(config):
    return PrinterDupPinOverride(config)
