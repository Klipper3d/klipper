# Set the state of a list of digital output pins
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class PrinterStaticDigitalOut:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        pin_list = config.getlist('pins')
        for pin_desc in pin_list:
            pin_params = ppins.lookup_pin(pin_desc, can_invert=True)
            mcu = pin_params['chip']
            mcu.add_config_cmd("set_digital_out pin=%s value=%d"
                               % (pin_params['pin'], not pin_params['invert']))

def load_config_prefix(config):
    return PrinterStaticDigitalOut(config)
