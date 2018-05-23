# TMC2130 configuration
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math

TMC_FREQUENCY=13200000.
REG_GCONF=0x00
GCONF_EN_PWM_MODE=1<<2
GCONF_DIAG1_STALL=1<<8
REG_TCOOLTHRS=0x14
REG_COOLCONF=0x6d

class TMC2130:
    def __init__(self, config):
        self.printer = config.get_printer()
        # pin setup
        ppins = self.printer.lookup_object("pins")
        cs_pin = config.get('cs_pin')
        cs_pin_params = ppins.lookup_pin('digital_out', cs_pin)
        if cs_pin_params['invert']:
            raise pins.error("tmc2130 can not invert pin")
        self.mcu = cs_pin_params['chip']
        pin = cs_pin_params['pin']
        self.oid = self.mcu.create_oid()
        self.mcu.add_config_cmd(
            "config_spi oid=%d bus=%d pin=%s mode=%d rate=%d shutdown_msg=" % (
                self.oid, 0, cs_pin_params['pin'], 3, 4000000))
        run_current = config.getfloat('run_current', above=0., maxval=2.)
        hold_current = config.getfloat('hold_current', run_current,
                                       above=0., maxval=2.)
        sense_resistor = config.getfloat('sense_resistor', 0.110, above=0.)
        steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
                 '8': 5, '4': 6, '2': 7, '1': 8}
        self.mres = config.getchoice('microsteps', steps)
        interpolate = config.getboolean('interpolate', True)
        sc_velocity = config.getfloat('stealthchop_threshold', 0., minval=0.)
        sc_threshold = self.velocity_to_clock(config, sc_velocity)
        iholddelay = config.getint('driver_IHOLDDELAY', 8, minval=0, maxval=15)
        tpowerdown = config.getint('driver_TPOWERDOWN', 0, minval=0, maxval=255)
        blank_time_select = config.getint('driver_BLANK_TIME_SELECT', 1,
                                          minval=0, maxval=3)
        toff = config.getint('driver_TOFF', 4, minval=1, maxval=15)
        hend = config.getint('driver_HEND', 7, minval=0, maxval=15)
        hstrt = config.getint('driver_HSTRT', 0, minval=0, maxval=7)
        sgt = config.getint('driver_SGT', 0, minval=-64, maxval=63) & 0x7f
        # Allow virtual endstop to be created
        self.diag1_pin = config.get('diag1_pin', None)
        ppins.register_chip("_".join(config.get_name().split()[:2]), self)
        self.send_spi_cmd = None
        self.mcu.add_config_object(self)
        # calculate current
        vsense = False
        irun = self.current_bits(run_current, sense_resistor, vsense)
        ihold = self.current_bits(hold_current, sense_resistor, vsense)
        if irun < 16 and ihold < 16:
            vsense = True
            irun = self.current_bits(run_current, sense_resistor, vsense)
            ihold = self.current_bits(hold_current, sense_resistor, vsense)
        # configure GCONF
        self.reg_GCONF = (sc_velocity > 0.) << 2
        self.add_config_cmd(REG_GCONF, self.reg_GCONF)
        # configure CHOPCONF
        self.add_config_cmd(
            0x6c, toff | (hstrt << 4) | (hend << 7) | (blank_time_select << 15)
            | (vsense << 17) | (self.mres << 24) | (interpolate << 28))
        # configure IHOLD_IRUN
        self.add_config_cmd(0x10, ihold | (irun << 8) | (iholddelay << 16))
        # configure TPOWERDOWN
        self.add_config_cmd(0x11, tpowerdown)
        # configure TPWMTHRS
        self.add_config_cmd(0x13, max(0, min(0xfffff, sc_threshold)))
        # configure COOLCONF
        self.add_config_cmd(REG_COOLCONF, sgt << 16)
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
    def velocity_to_clock(self, config, velocity):
        if not velocity:
            return 0
        stepper_name = config.get_name().split()[1]
        stepper_config = config.getsection(stepper_name)
        step_dist = stepper_config.getfloat('step_distance')
        step_dist_256 = step_dist / (1 << self.mres)
        return int(TMC_FREQUENCY * step_dist_256 / velocity + .5)
    def setup_pin(self, pin_params):
        if (pin_params['pin'] != 'virtual_endstop'
            or pin_params['type'] != 'endstop'):
            raise pins.error("tmc2130 virtual endstop only useful as endstop")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert tmc2130 virtual endstop")
        return TMC2130VirtualEndstop(self)
    def build_config(self):
        cmd_queue = self.mcu.alloc_command_queue()
        self.spi_send_cmd = self.mcu.lookup_command(
            "spi_send oid=%c data=%*s", cq=cmd_queue)
    def set_register(self, addr, val):
        data = [(addr | 0x80) & 0xff, (val >> 24) & 0xff, (val >> 16) & 0xff,
                (val >> 8) & 0xff, val & 0xff]
        self.spi_send_cmd.send([self.oid, data])

# Endstop wrapper that enables tmc2130 "sensorless homing"
class TMC2130VirtualEndstop:
    def __init__(self, tmc2130):
        self.tmc2130 = tmc2130
        if tmc2130.diag1_pin is None:
            raise pins.error("tmc2130 virtual endstop requires diag1_pin")
        ppins = tmc2130.printer.lookup_object('pins')
        self.mcu_endstop = ppins.setup_pin('endstop', tmc2130.diag1_pin)
        if self.mcu_endstop.get_mcu() is not tmc2130.mcu:
            raise pins.error("tmc2130 virtual endstop must be on same mcu")
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.query_endstop_wait = self.mcu_endstop.query_endstop_wait
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def home_prepare(self):
        gconf = self.tmc2130.reg_GCONF
        gconf &= ~GCONF_EN_PWM_MODE
        gconf |= GCONF_DIAG1_STALL
        self.tmc2130.set_register(REG_GCONF, gconf)
        self.tmc2130.set_register(REG_TCOOLTHRS, 0xfffff)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        self.tmc2130.set_register(REG_GCONF, self.tmc2130.reg_GCONF)
        self.tmc2130.set_register(REG_TCOOLTHRS, 0)
        self.mcu_endstop.home_finalize()

def load_config_prefix(config):
    return TMC2130(config)
