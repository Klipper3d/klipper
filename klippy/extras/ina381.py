# Current monitor circuit
#
# Copyright (C) 2024  Elias Bakken <elias@iagent.no>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
class ina381:
    def __init__(self, config):
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        ppins.register_chip('ina381', self)

        # Reset over current alarm
        reset_pin = config.get('reset_pin', 'ar100:PF4')
        oc_reset = ppins.setup_pin('digital_out', reset_pin)
        mcu = oc_reset.get_mcu()
        pin = oc_reset._pin
        mcu.add_config_cmd("set_digital_out pin=%s value=%d" % (pin, 0), True)
        mcu.add_config_cmd("set_digital_out pin=%s value=%d" % (pin, 1), True)

def load_config(config):
    return ina381(config)
