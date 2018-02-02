# AD5206 digipot code
#
# Copyright (C) 2017,2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins

class ad5206:
    def __init__(self, config):
        printer = config.get_printer()
        enable_pin_params = pins.get_printer_pins(printer).lookup_pin(
            'digital_out', config.get('enable_pin'))
        self.mcu = enable_pin_params['chip']
        self.pin = enable_pin_params['pin']
        self.mcu.add_config_object(self)
        scale = config.getfloat('scale', 1., above=0.)
        self.channels = [None] * 6
        for i in range(len(self.channels)):
            val = config.getfloat('channel_%d' % (i+1,), None,
                                  minval=0., maxval=scale)
            if val is not None:
                self.channels[i] = int(val * 256. / scale + .5)
    def build_config(self):
        for i, val in enumerate(self.channels):
            if val is not None:
                self.mcu.add_config_cmd(
                    "send_spi_message pin=%s msg=%02x%02x" % (self.pin, i, val))

def load_config(config):
    return ad5206(config)
