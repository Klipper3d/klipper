# TMC2130 configuration
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math

IHOLDDELAY = 4
TPOWERDOWN = 8
BLANK_TIME_SELECT = 1
TOFF = 5
HSTRT = 3
HEND = 2

class tmc2130:
    def __init__(self, config):
        printer = config.get_printer()
        # pin setup
        ppins = printer.lookup_object("pins")
        enable_pin = config.get('enable_pin')
        enable_pin_params = ppins.lookup_pin('digital_out', enable_pin)
        if enable_pin_params['invert']:
            raise pins.error("tmc2130 can not invert pin")
        self.mcu = enable_pin_params['chip']
        pin = enable_pin_params['pin']
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd(
            "config_spi oid=%d bus=%d pin=%s mode=%d rate=%d shutdown_msg=" % (
                self.oid, 0, enable_pin_params['pin'], 3, 3600000))
        run_current = config.getfloat('run_current', above=0.)
        hold_current = config.getfloat('hold_current', above=0.)
        sense_resistor = config.getfloat('sense_resistor', 0.110, above=0.)
        steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
                 '8': 5, '4': 6, '2': 7, '1': 8}
        microsteps = config.getchoice('microsteps', steps, '16')
        # configure CHOPCONF
        vsense = False
        irun = self.current_bits(run_current, sense_resistor, vsense)
        ihold = self.current_bits(hold_current, sense_resistor, vsense)
        if irun < 16 and ihold < 16:
            vsense = True
            irun = self.current_bits(run_current, sense_resistor, vsense)
            ihold = self.current_bits(hold_current, sense_resistor, vsense)
        self.add_config_cmd(
            0x6c, TOFF | (HSTRT << 4) | (HEND << 7) | (BLANK_TIME_SELECT << 15)
            | (vsense << 17) | (microsteps << 24))
        # configure IHOLD_IRUN
        self.add_config_cmd(0x10, ihold | (irun << 8) | (IHOLDDELAY << 16))
        # configure TPOWERDOWN
        self.add_config_cmd(0x11, TPOWERDOWN)
    def add_config_cmd(self, addr, val):
        self.mcu.add_config_cmd("spi_send oid=%d data=%02x%08x" % (
            self.oid, (addr | 0x80) & 0xff, val & 0xffffffff), is_init=True)
    def current_bits(self, current, sense_resistor, vsense_on):
        sense_resistor += 0.020
        vsense = 0.32
        if vsense_on:
            vsense = 0.18
        cs = int(32. * current * sense_resistor * math.sqrt(2.) / vsense
                 - 1. + .5)
        return max(0, min(31, cs))

def load_config_prefix(config):
    return tmc2130(config)
