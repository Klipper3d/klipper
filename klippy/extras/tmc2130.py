# TMC2130 configuration
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
import pins

IHOLDDELAY = 4
TPOWERDOWN = 8
BLANK_TIME_SELECT = 1
TOFF = 5
HSTRT = 3
HEND = 2

class tmc2130:
    def __init__(self, config):
        self.printer = config.get_printer()
        # pin setup
        ppins = self.printer.lookup_object("pins")
        enable_pin = config.get('enable_pin')
        enable_pin_params = ppins.lookup_pin('digital_out', enable_pin)
        if enable_pin_params['invert']:
            raise pins.error("tmc2130 can not invert pin")
        self.mcu = enable_pin_params['chip']
        bus = config.get('bus', 0)
        self.oid = self.mcu.create_oid()
        pin = enable_pin_params['pin']
        self.mcu.add_config_cmd("config_spi oid=%d bus=%u pin=%s mode=%u rate=%u shutdown_msg=%s" % (
            self.oid, bus, pin, 3, 2000000, ''))
        if config.getboolean('full_reset', True):
            self.full_reset()
        run_current = config.getfloat('run_current', above=0.)
        hold_current = config.getfloat('hold_current', above=0.)
        sense_resistor = config.getfloat('sense_resistor', 0.110, above=0.)
        steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
                 '8': 5, '4': 6, '2': 7, '1': 8}
        microsteps = config.getchoice('microsteps', steps, '16')
        interpolate = config.getboolean('interpolate', True)
        # configure CHOPCONF
        vsense = False
        irun = self.current_bits(run_current, sense_resistor, vsense)
        ihold = self.current_bits(hold_current, sense_resistor, vsense)
        if irun < 16 and ihold < 16:
            vsense = True
            irun = self.current_bits(run_current, sense_resistor, vsense)
            ihold = self.current_bits(hold_current, sense_resistor, vsense)
        if config.getboolean('slow_decay_only', False):
            self.add_config_cmd(
                0x6c, TOFF | (1<<12) | (1<<14) | (BLANK_TIME_SELECT << 15)
                | (vsense << 17) | (microsteps << 24) | (interpolate << 28))
        else:
            self.add_config_cmd(
                0x6c, TOFF | (HSTRT << 4) | (HEND << 7) | (BLANK_TIME_SELECT << 15)
                | (vsense << 17) | (microsteps << 24) | (interpolate << 28))
        # configure IHOLD_IRUN
        self.add_config_cmd(0x10, ihold | (irun << 8) | (IHOLDDELAY << 16))
        # configure TPOWERDOWN
        self.add_config_cmd(0x11, TPOWERDOWN)
    def add_config_cmd(self, addr, val):
        self.mcu.add_config_cmd("spi_send oid=%u data=%02x%08x" % (
            self.oid, (addr | 0x80) & 0xff, val & 0xffffffff))
    def full_reset(self):
        regs = [0x00, 0x10, 0x11, 0x13, 0x14, 0x15, 0x2d, 0x33,
                0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                0x68, 0x69, 0x6c, 0x6e, 0x70, 0x72]
        state = {0x60: 0xAAAAB554, 0x61: 0x4A9554AA, 0x62: 0x24492929,
                 0x63: 0x10104222, 0x64: 0xFBFFFFFF, 0x65: 0xB5BB777D,
                 0x66: 0x49295556, 0x67: 0x00404222,
                 0x68: 0xFFFF8056, 0x69: 0x00F70000,
                 0x72: 0x00050480}
        for reg in regs:
            self.add_config_cmd(reg, state.get(reg, 0x00000000))
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
