# SAMD Sercom configuration
#
# Copyright (C) 2019  Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class SamdSERCOM:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        ppins = self.printer.lookup_object("pins")
        try:
            int(self.name)
        except ValueError:
            raise ppins.error("%s: SERCOM name must be an integer" % (
                    config.get_name(),))

        self.tx_pin = config.get("tx_pin", None)
        self.rx_pin = config.get("rx_pin", None)
        self.clk_pin = config.get("clk_pin", None)

        tx_pin_params = ppins.lookup_pin(self.tx_pin)
        rx_pin_params = ppins.lookup_pin(self.rx_pin)
        clk_pin_params = ppins.lookup_pin(self.clk_pin)

        self.mcu = tx_pin_params['chip']

        if self.mcu is not rx_pin_params['chip'] or self.mcu is not clk_pin_params['chip']:
           raise ppins.error("%s: SERCOM pins must be on same mcu" % (
                    config.get_name(),))

        self.mcu.register_config_callback(self.build_config)

    def build_config(self):
        if self.tx_pin:
            self.mcu.add_config_cmd("set_sercom_pin sercom_id=%s pin_type=%d pin=%s" % (
                self.name, 0, self.tx_pin))
        if self.rx_pin:
            self.mcu.add_config_cmd("set_sercom_pin sercom_id=%s pin_type=%d pin=%s" % (
                self.name, 1, self.rx_pin))
        if self.clk_pin:
            self.mcu.add_config_cmd("set_sercom_pin sercom_id=%s pin_type=%d pin=%s" % (
                self.name, 2, self.clk_pin))

def load_config_prefix(config):
    return SamdSERCOM(config)
