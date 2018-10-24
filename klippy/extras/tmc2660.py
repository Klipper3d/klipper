# TMC2660 configuration
#
# Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

CURRENT_MAX = 2.4
CURRENT_MIN = 0.1

REG_DRVCONF = 0xe << 16
REG_SGCSCONF = 0xc  << 16
REG_SMARTEN = 0xa  << 16
REG_CHOPCONF = 0x8  << 16
REG_DRVCTRL = 0x0  << 16

def get_bits(reg, field, value):
    return ((value << reg[field][0]) & reg[field][1])

def read_field(bits, reg, field):
    return (bits & reg[field][1]) >> reg[field][0]

def current_to_reg(current):
    current_constrained = min(CURRENT_MAX, max(CURRENT_MIN, current))
    return int((32 * 1000 * current_constrained - 1600)/3236) # Taken from RRF

DRVCTRL = {
    "INTPOL" : (9, 0x00100),
    "DEDGE"  : (8, 0x00080),
    "MRES"   : (0, 0x0000F)
}

CHOPCONF = {
    "TBL"   : (15, 0x18000),
    "CHM"   : (14, 0x04000),
    "RNDTF" : (13, 0x02000),
    "HDEC"  : (11, 0x01800),
    "HEND"  : (7,  0x00780),
    "HSTRT" : (4,  0x00070),
    "TOFF"  : (0,  0x0000F)
}

SMARTEN = {
    "SEIMIN" : (15, 0x08000),
    "SEDN"   : (14, 0x06000),
    "SEMAX"  : (8,  0x00F00),
    "SEUP"   : (5,  0x00060),
    "SEMIN"  : (0,  0x0000F)
}

SGCSCONF = {
    "SFILT"  : (16, 0x10000),
    "SGT"    : (8,  0x07800),
    "CS"     : (0,  0x0001F)
}

DRVCONF = {
    "TST"    : (16, 0x10000),
    "SLPH"   : (14, 0x0C000),
    "SLPL"   : (12, 0x03000),
    "DISS2G" : (10, 0x00400),
    "TS2G"   : (8,  0x00300),
    "SDOFF"  : (7,  0x00080),
    "VSENSE" : (6,  0x00040),
    "RDSEL"  : (4,  0x00030)
}

READRSP = {
    "MSTEP" : (10, 0xFFC00),
    "STST"  : (7,  0x00080),
    "OLB"   : (6,  0x00040),
    "OLA"   : (5,  0x00020),
    "S2GB"  : (4,  0x00010),
    "S2GA"  : (3,  0x00008),
    "OTPW"  : (2,  0x00004),
    "OT"    : (1,  0x00002),
    "SG"    : (0,  0x00001)
}

class TMC2660:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        self.toolhead = None

        # Generic setup
        ppins = self.printer.lookup_object("pins")
        cs_pin = config.get('cs_pin')
        cs_pin_params = ppins.lookup_pin(cs_pin)
        self.mcu = cs_pin_params['chip']
        if cs_pin_params['invert']:
            raise pins.error("tmc2660 can not invert pin")
        pin = cs_pin_params['pin']
        self.oid = self.mcu.create_oid()
        self.bus = config.getint('bus', minval=0, maxval=3)
        self.freq = config.getint('freq', default=2000000, minval=1000000, maxval=4000000)
        self.mcu.add_config_cmd(
            "config_spi oid=%d bus=%d pin=%s mode=%d rate=%d shutdown_msg=" % (
                self.oid, self.bus, cs_pin_params['pin'], 0, self.freq))
        self.spi_send_cmd = self.spi_transfer_cmd = None
        self.mcu.register_config_callback(self.build_config)
        # Add SET_CURRENT command
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command(
            "SET_TMC_CURRENT", "STEPPER", self.name,
            self.cmd_SET_TMC_CURRENT, desc=self.cmd_SET_TMC_CURRENT_help)
        # Setup driver registers
        # DRVCTRL
        steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
                 '8': 5, '4': 6, '2': 7, '1': 8}
        self.driver_mres = config.getchoice('microsteps', steps)
        self.driver_intpol = config.getboolean('interpolate', default=True)
        self.current = config.getfloat('run_current',  minval=CURRENT_MIN, maxval=CURRENT_MAX)
        self.driver_dedge = config.getboolean('driver_DEDGE', default=False)

        # CHOPCONF
        btime = {'16': 0, '24': 1, '36': 2, '54': 3}
        self.driver_tbl = config.getchoice('driver_TBL', btime, default='36')
        chm = {'spreadcycle': 0, 'constant_toff': 1}
        self.driver_chm = config.getchoice('driver_CHM', chm, default='spreadcycle')
        self.driver_rndtf = config.getboolean('driver_RNDTF', False)
        if (self.driver_chm):
            self.driver_hend = config.getint('driver_HEND', default=7, minval=-3, maxval=12) + 3
            self.driver_hstrt = config.getint('driver_HSTRT', default=5, minval=0, maxval=15)
            self.driver_toff = config.getint('driver_TOFF', default=7, minval=0, maxval=15)
            self.driver_hdec = config.getboolean('driver_HDEC', default=False) | ((self.hstrt & 0x8) >> 1) # if chm is 1, HDEC1 is the MSB of HSTRT
        else:
            self.driver_hdec = config.getboolean('driver_HDEC', default=False)
            self.driver_hend = config.getint('driver_HEND', default=3, minval=-3, maxval=12) + 3
            self.driver_hstrt = config.getint('driver_HSTRT', default=4, minval=1, maxval=8) - 1
            self.driver_toff = config.getint('driver_TOFF', default=4, minval=0, maxval=15)
            if (self.driver_hstrt + self.driver_hend > 15):
                raise pins.error("driver_HEND + HSTRT must be <= 15")

        # SMARTEN
        csc = {'quarter': 1, 'half': 0}
        self.driver_seimin = config.getchoice('driver_SEIMIN', csc, default='half')
        cds = {'32': 0, '8': 1, '2': 2, '1': 3}
        self.driver_sedn = config.getchoice('driver_SEDN', cds, default='32')
        self.driver_semax = config.getint('driver_SEMAX', default=0, minval=0, maxval=15)
        cis = {'1': 0, '2': 1, '4': 2, '8': 3}
        self.driver_seup = config.getchoice('driver_SEUP', cis, default='1')
        self.driver_semin = config.getint('driver_SEMIN', default=0, minval=0, maxval=15)

        # SGSCONF
        self.driver_sfilt = config.getboolean('driver_SFILT', default=True)
        self.driver_sgt = config.getint('driver_sgt', default=-64, minval=-64, maxval=63) + 64
        self.driver_cs = current_to_reg(self.current)

        # DRVCONF
        slph = {'min': 0, 'min_tc': 1, 'med_tc': 2, 'max': 3}
        self.driver_slph = config.getchoice('driver_SLPH', slph, default='min')
        slpl = {'min': 0, 'med': 2, 'max': 3}
        self.driver_slpl = config.getchoice('driver_SLPL', slpl, default='min')
        self.driver_diss2g = config.getboolean('driver_DISS2G', default=False)
        ts2g = {'3.2': 0, '1.6': 1, '1.2': 2, '0.8': 3}
        self.driver_ts2g = config.getchoice('driver_TS2G', ts2g, default='0.8')
        self.driver_sdoff = False # since we don't support SPI mode yet, this has to be False
        vsense = {'low': 0, 'high': 1}
        self.driver_vsense = config.getchoice('driver_VSENSE', vsense, default='high')
        self.driver_rdsel = 2  # stallguard2 and coolstep current level

        # Build and send registers
        self.reg_drvconf =  REG_DRVCONF | \
                            get_bits(DRVCONF, "TST", 0) | \
                            get_bits(DRVCONF, "SLPH", self.driver_slph) | \
                            get_bits(DRVCONF, "SLPL", self.driver_slpl) | \
                            get_bits(DRVCONF, "DISS2G", self.driver_diss2g) | \
                            get_bits(DRVCONF, "TS2G", self.driver_ts2g) | \
                            get_bits(DRVCONF, "SDOFF", self.driver_sdoff) | \
                            get_bits(DRVCONF, "VSENSE", self.driver_vsense) | \
                            get_bits(DRVCONF, "RDSEL", self.driver_rdsel)
        self.add_config_cmd(self.reg_drvconf)

        self.reg_drvctrl = REG_DRVCTRL | \
                           get_bits(DRVCTRL, "INTPOL", self.driver_intpol) | \
                           get_bits(DRVCTRL, "DEDGE", self.driver_dedge) | \
                           get_bits(DRVCTRL, "MRES", self.driver_mres)
        self.add_config_cmd(self.reg_drvctrl)

        self.reg_chopconf = REG_CHOPCONF | \
                            get_bits(CHOPCONF, "TBL", self.driver_tbl) | \
                            get_bits(CHOPCONF, "CHM", self.driver_chm) | \
                            get_bits(CHOPCONF, "RNDTF", self.driver_rndtf) | \
                            get_bits(CHOPCONF, "HDEC", self.driver_hdec) | \
                            get_bits(CHOPCONF, "HEND", self.driver_hend) | \
                            get_bits(CHOPCONF, "HSTRT", self.driver_hstrt) | \
                            get_bits(CHOPCONF, "TOFF", self.driver_toff)
        self.add_config_cmd(self.reg_chopconf)

        self.reg_sgcsconf = REG_SGCSCONF | \
                            get_bits(SGCSCONF, "SFILT", self.driver_sfilt) | \
                            get_bits(SGCSCONF, "SGT", self.driver_sgt) | \
                            get_bits(SGCSCONF, "CS", self.driver_cs)
        self.add_config_cmd(self.reg_sgcsconf)

        self.reg_smarten = REG_SMARTEN | \
                           get_bits(SMARTEN, "SEIMIN", self.driver_seimin) | \
                           get_bits(SMARTEN, "SEDN", self.driver_sedn) | \
                           get_bits(SMARTEN, "SEMAX", self.driver_semax) | \
                           get_bits(SMARTEN, "SEUP", self.driver_seup) | \
                           get_bits(SMARTEN, "SEMIN", self.driver_semin)
        self.add_config_cmd(self.reg_smarten)

        # Idle timeout
        self.idle_current_percentage = config.getint('idle_current_percent', default=100, minval=0, maxval=100)
        if self.idle_current_percentage < 100:
            self.printer.register_event_handler("idle_timeout:printing",
                                                self.handle_printing)
            self.printer.register_event_handler("idle_timeout:ready",
                                                self.handle_ready)

    def add_config_cmd(self, val):
        self.mcu.add_config_cmd("spi_send oid=%d data=%06x" % (
            self.oid, val & 0xffffff))

    def build_config(self):
        cmd_queue = self.mcu.alloc_command_queue()
        self.spi_send_cmd = self.mcu.lookup_command(
            "spi_send oid=%c data=%*s", cq=cmd_queue)
        self.spi_transfer_cmd = self.mcu.lookup_command(
            "spi_transfer oid=%c data=%*s", cq=cmd_queue)

    def handle_printing(self, print_time):
        self.set_current(print_time, self.current)

    def handle_ready(self, print_time):
        self.set_current(print_time, float(self.idle_current_percentage) * self.current / 100)

    def set_current(self, print_time, current):
        self.driver_cs = current_to_reg(current)
        reg = self.reg_sgcsconf
        reg &= ~(SGCSCONF["CS"][1])
        reg |= get_bits(SGCSCONF, "CS", self.driver_cs)
        reg_data = [(reg >> 16) & 0xff, (reg >> 8) & 0xff, reg & 0xff]
        clock = self.mcu.print_time_to_clock(print_time)
        params = self.spi_send_cmd.send([self.oid, reg_data], minclock=clock, reqclock=clock)

    cmd_SET_TMC_CURRENT_help = "Set the current of a TMC2660 driver (between %d and %d)" % (CURRENT_MIN, CURRENT_MAX)
    def cmd_SET_TMC_CURRENT(self, params):
        gcode = self.printer.lookup_object('gcode')
        if 'CURRENT' in params:
            self.current = gcode.get_float('CURRENT', params, minval=CURRENT_MIN, maxval=CURRENT_MAX)
            self.set_current(self.printer.lookup_object('toolhead').get_last_move_time(), self.current)

def load_config_prefix(config):
    return TMC2660(config)
