# Set the state of a list of digital output pins
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterStaticDigitalOut:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        pin_list = [pin.strip() for pin in config.get('pins').split(',')]
        for pin_desc in pin_list:
            mcu_pin = ppins.setup_pin('digital_out', pin_desc)
            mcu_pin.setup_start_value(1, 1, True)

def load_config_prefix(config):
    return PrinterStaticDigitalOut(config)
