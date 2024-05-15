# TMC2130 configuration
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import bus, tmc

TMC_FREQUENCY=13200000.

Registers = {
    "GCONF": 0x00, "GSTAT": 0x01, "IOIN": 0x04, "IHOLD_IRUN": 0x10,
    "TPOWERDOWN": 0x11, "TSTEP": 0x12, "TPWMTHRS": 0x13, "TCOOLTHRS": 0x14,
    "THIGH": 0x15, "XDIRECT": 0x2d, "MSLUT0": 0x60, "MSLUT1": 0x61,
    "MSLUT2": 0x62, "MSLUT3": 0x63, "MSLUT4": 0x64, "MSLUT5": 0x65,
    "MSLUT6": 0x66, "MSLUT7": 0x67, "MSLUTSEL": 0x68, "MSLUTSTART": 0x69,
    "MSCNT": 0x6a, "MSCURACT": 0x6b, "CHOPCONF": 0x6c, "COOLCONF": 0x6d,
    "DCCTRL": 0x6e, "DRV_STATUS": 0x6f, "PWMCONF": 0x70, "PWM_SCALE": 0x71,
    "ENCM_CTRL": 0x72, "LOST_STEPS": 0x73,
}

ReadRegisters = [
    "GCONF", "GSTAT", "IOIN", "TSTEP", "XDIRECT", "MSCNT", "MSCURACT",
    "CHOPCONF", "DRV_STATUS", "PWM_SCALE", "LOST_STEPS",
]

Fields = {}
Fields["GCONF"] = {
    "i_scale_analog": 1<<0, "internal_rsense": 1<<1, "en_pwm_mode": 1<<2,
    "enc_commutation": 1<<3, "shaft": 1<<4, "diag0_error": 1<<5,
    "diag0_otpw": 1<<6, "diag0_stall": 1<<7, "diag1_stall": 1<<8,
    "diag1_index": 1<<9, "diag1_onstate": 1<<10, "diag1_steps_skipped": 1<<11,
    "diag0_int_pushpull": 1<<12, "diag1_pushpull": 1<<13,
    "small_hysteresis": 1<<14, "stop_enable": 1<<15, "direct_mode": 1<<16,
    "test_mode": 1<<17
}
Fields["GSTAT"] = { "reset": 1<<0, "drv_err": 1<<1, "uv_cp": 1<<2 }
Fields["IOIN"] = {
    "step": 1<<0, "dir": 1<<1, "dcen_cfg4": 1<<2, "dcin_cfg5": 1<<3,
    "drv_enn_cfg6": 1<<4, "dco": 1<<5, "version": 0xff << 24
}
Fields["IHOLD_IRUN"] = {
    "ihold": 0x1f << 0, "irun": 0x1f << 8, "iholddelay": 0x0f << 16
}
Fields["TPOWERDOWN"] = { "tpowerdown": 0xff }
Fields["TSTEP"] = { "tstep": 0xfffff }
Fields["TPWMTHRS"] = { "tpwmthrs": 0xfffff }
Fields["TCOOLTHRS"] = { "tcoolthrs": 0xfffff }
Fields["THIGH"] = { "thigh": 0xfffff }
Fields["MSLUT0"] = { "mslut0": 0xffffffff }
Fields["MSLUT1"] = { "mslut1": 0xffffffff }
Fields["MSLUT2"] = { "mslut2": 0xffffffff }
Fields["MSLUT3"] = { "mslut3": 0xffffffff }
Fields["MSLUT4"] = { "mslut4": 0xffffffff }
Fields["MSLUT5"] = { "mslut5": 0xffffffff }
Fields["MSLUT6"] = { "mslut6": 0xffffffff }
Fields["MSLUT7"] = { "mslut7": 0xffffffff }
Fields["MSLUTSEL"] = {
    "x3":                       0xFF << 24,
    "x2":                       0xFF << 16,
    "x1":                       0xFF << 8,
    "w3":                       0x03 << 6,
    "w2":                       0x03 << 4,
    "w1":                       0x03 << 2,
    "w0":                       0x03 << 0,
}
Fields["MSLUTSTART"] = {
    "start_sin":                0xFF << 0,
    "start_sin90":              0xFF << 16,
}
Fields["MSCNT"] = { "mscnt": 0x3ff }
Fields["MSCURACT"] = { "cur_a": 0x1ff, "cur_b": 0x1ff << 16 }
Fields["CHOPCONF"] = {
    "toff": 0x0f, "hstrt": 0x07 << 4, "hend": 0x0f << 7, "fd3": 1<<11,
    "disfdcc": 1<<12, "rndtf": 1<<13, "chm": 1<<14, "tbl": 0x03 << 15,
    "vsense": 1<<17, "vhighfs": 1<<18, "vhighchm": 1<<19, "sync": 0x0f << 20,
    "mres": 0x0f << 24, "intpol": 1<<28, "dedge": 1<<29, "diss2g": 1<<30
}
Fields["COOLCONF"] = {
    "semin": 0x0f, "seup": 0x03 << 5, "semax": 0x0f << 8, "sedn": 0x03 << 13,
    "seimin": 1<<15, "sgt": 0x7f << 16, "sfilt": 1<<24
}
Fields["DRV_STATUS"] = {
    "sg_result": 0x3ff, "fsactive": 1<<15, "cs_actual": 0x1f << 16,
    "stallguard": 1<<24, "ot": 1<<25, "otpw": 1<<26, "s2ga": 1<<27,
    "s2gb": 1<<28, "ola": 1<<29, "olb": 1<<30, "stst": 1<<31
}
Fields["PWMCONF"] = {
    "pwm_ampl": 0xff, "pwm_grad": 0xff << 8, "pwm_freq": 0x03 << 16,
    "pwm_autoscale": 1<<18, "pwm_symmetric": 1<<19, "freewheel": 0x03 << 20
}
Fields["PWM_SCALE"] = { "pwm_scale": 0xff }
Fields["LOST_STEPS"] = { "lost_steps": 0xfffff }

SignedFields = ["cur_a", "cur_b", "sgt"]

FieldFormatters = {
    "i_scale_analog":   (lambda v: "1(ExtVREF)" if v else ""),
    "shaft":            (lambda v: "1(Reverse)" if v else ""),
    "reset":            (lambda v: "1(Reset)" if v else ""),
    "drv_err":          (lambda v: "1(ErrorShutdown!)" if v else ""),
    "uv_cp":            (lambda v: "1(Undervoltage!)" if v else ""),
    "version":          (lambda v: "%#x" % v),
    "mres":             (lambda v: "%d(%dusteps)" % (v, 0x100 >> v)),
    "otpw":             (lambda v: "1(OvertempWarning!)" if v else ""),
    "ot":               (lambda v: "1(OvertempError!)" if v else ""),
    "s2ga":             (lambda v: "1(ShortToGND_A!)" if v else ""),
    "s2gb":             (lambda v: "1(ShortToGND_B!)" if v else ""),
    "ola":              (lambda v: "1(OpenLoad_A!)" if v else ""),
    "olb":              (lambda v: "1(OpenLoad_B!)" if v else ""),
    "cs_actual":        (lambda v: ("%d" % v) if v else "0(Reset?)"),
}


######################################################################
# TMC stepper current config helper
######################################################################

MAX_CURRENT = 2.000

class TMCCurrentHelper:
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        run_current = config.getfloat('run_current',
                                      above=0., maxval=MAX_CURRENT)
        hold_current = config.getfloat('hold_current', MAX_CURRENT,
                                       above=0., maxval=MAX_CURRENT)
        self.req_hold_current = hold_current
        self.sense_resistor = config.getfloat('sense_resistor', 0.110, above=0.)
        vsense, irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("vsense", vsense)
        self.fields.set_field("ihold", ihold)
        self.fields.set_field("irun", irun)
    def _calc_current_bits(self, current, vsense):
        sense_resistor = self.sense_resistor + 0.020
        vref = 0.32
        if vsense:
            vref = 0.18
        cs = int(32. * sense_resistor * current * math.sqrt(2.) / vref + .5) - 1
        return max(0, min(31, cs))
    def _calc_current_from_bits(self, cs, vsense):
        sense_resistor = self.sense_resistor + 0.020
        vref = 0.32
        if vsense:
            vref = 0.18
        return (cs + 1) * vref / (32. * sense_resistor * math.sqrt(2.))
    def _calc_current(self, run_current, hold_current):
        vsense = True
        irun = self._calc_current_bits(run_current, True)
        if irun == 31:
            cur = self._calc_current_from_bits(irun, True)
            if cur < run_current:
                irun2 = self._calc_current_bits(run_current, False)
                cur2 = self._calc_current_from_bits(irun2, False)
                if abs(run_current - cur2) < abs(run_current - cur):
                    vsense = False
                    irun = irun2
        ihold = self._calc_current_bits(min(hold_current, run_current), vsense)
        return vsense, irun, ihold
    def get_current(self):
        irun = self.fields.get_field("irun")
        ihold = self.fields.get_field("ihold")
        vsense = self.fields.get_field("vsense")
        run_current = self._calc_current_from_bits(irun, vsense)
        hold_current = self._calc_current_from_bits(ihold, vsense)
        return run_current, hold_current, self.req_hold_current, MAX_CURRENT
    def set_current(self, run_current, hold_current, print_time):
        self.req_hold_current = hold_current
        vsense, irun, ihold = self._calc_current(run_current, hold_current)
        if vsense != self.fields.get_field("vsense"):
            val = self.fields.set_field("vsense", vsense)
            self.mcu_tmc.set_register("CHOPCONF", val, print_time)
        self.fields.set_field("ihold", ihold)
        val = self.fields.set_field("irun", irun)
        self.mcu_tmc.set_register("IHOLD_IRUN", val, print_time)


######################################################################
# TMC2130 SPI
######################################################################

class MCU_TMC_SPI_chain:
    def __init__(self, config, chain_len=1):
        self.printer = config.get_printer()
        self.chain_len = chain_len
        self.mutex = self.printer.get_reactor().mutex()
        share = None
        if chain_len > 1:
            share = "tmc_spi_cs"
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=4000000,
                                           share_type=share)
        self.taken_chain_positions = []
    def _build_cmd(self, data, chain_pos):
        return ([0x00] * ((self.chain_len - chain_pos) * 5) +
                data + [0x00] * ((chain_pos - 1) * 5))
    def reg_read(self, reg, chain_pos):
        cmd = self._build_cmd([reg, 0x00, 0x00, 0x00, 0x00], chain_pos)
        self.spi.spi_send(cmd)
        if self.printer.get_start_args().get('debugoutput') is not None:
            return 0
        params = self.spi.spi_transfer(cmd)
        pr = bytearray(params['response'])
        pr = pr[(self.chain_len - chain_pos) * 5 :
                (self.chain_len - chain_pos + 1) * 5]
        return (pr[1] << 24) | (pr[2] << 16) | (pr[3] << 8) | pr[4]
    def reg_write(self, reg, val, chain_pos, print_time=None):
        minclock = 0
        if print_time is not None:
            minclock = self.spi.get_mcu().print_time_to_clock(print_time)
        data = [(reg | 0x80) & 0xff, (val >> 24) & 0xff, (val >> 16) & 0xff,
                (val >> 8) & 0xff, val & 0xff]
        if self.printer.get_start_args().get('debugoutput') is not None:
            self.spi.spi_send(self._build_cmd(data, chain_pos), minclock)
            return val
        write_cmd = self._build_cmd(data, chain_pos)
        dummy_read = self._build_cmd([0x00, 0x00, 0x00, 0x00, 0x00], chain_pos)
        params = self.spi.spi_transfer_with_preface(write_cmd, dummy_read,
                                                    minclock=minclock)
        pr = bytearray(params['response'])
        pr = pr[(self.chain_len - chain_pos) * 5 :
                (self.chain_len - chain_pos + 1) * 5]
        return (pr[1] << 24) | (pr[2] << 16) | (pr[3] << 8) | pr[4]

# Helper to setup an spi daisy chain bus from settings in a config section
def lookup_tmc_spi_chain(config):
    chain_len = config.getint('chain_length', None, minval=2)
    if chain_len is None:
        # Simple, non daisy chained SPI connection
        return MCU_TMC_SPI_chain(config, 1), 1

    # Shared SPI bus - lookup existing MCU_TMC_SPI_chain
    ppins = config.get_printer().lookup_object("pins")
    cs_pin_params = ppins.lookup_pin(config.get('cs_pin'),
                                     share_type="tmc_spi_cs")
    tmc_spi = cs_pin_params.get('class')
    if tmc_spi is None:
        tmc_spi = cs_pin_params['class'] = MCU_TMC_SPI_chain(config, chain_len)
    if chain_len != tmc_spi.chain_len:
        raise config.error("TMC SPI chain must have same length")
    chain_pos = config.getint('chain_position', minval=1, maxval=chain_len)
    if chain_pos in tmc_spi.taken_chain_positions:
        raise config.error("TMC SPI chain can not have duplicate position")
    tmc_spi.taken_chain_positions.append(chain_pos)
    return tmc_spi, chain_pos

# Helper code for working with TMC devices via SPI
class MCU_TMC_SPI:
    def __init__(self, config, name_to_reg, fields, tmc_frequency):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.tmc_spi, self.chain_pos = lookup_tmc_spi_chain(config)
        self.mutex = self.tmc_spi.mutex
        self.name_to_reg = name_to_reg
        self.fields = fields
        self.tmc_frequency = tmc_frequency
    def get_fields(self):
        return self.fields
    def get_register(self, reg_name):
        reg = self.name_to_reg[reg_name]
        with self.mutex:
            read = self.tmc_spi.reg_read(reg, self.chain_pos)
        return read
    def set_register(self, reg_name, val, print_time=None):
        reg = self.name_to_reg[reg_name]
        with self.mutex:
            for retry in range(5):
                v = self.tmc_spi.reg_write(reg, val, self.chain_pos, print_time)
                if v == val:
                    return
        raise self.printer.command_error(
            "Unable to write tmc spi '%s' register %s" % (self.name, reg_name))
    def get_tmc_frequency(self):
        return self.tmc_frequency


######################################################################
# TMC2130 printer object
######################################################################

class TMC2130:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_tmc = MCU_TMC_SPI(config, Registers, self.fields,
                                   TMC_FREQUENCY)
        # Allow virtual pins to be created
        tmc.TMCVirtualPinHelper(config, self.mcu_tmc)
        # Register commands
        current_helper = TMCCurrentHelper(config, self.mcu_tmc)
        cmdhelper = tmc.TMCCommandHelper(config, self.mcu_tmc, current_helper)
        cmdhelper.setup_register_dump(ReadRegisters)
        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status
        # Setup basic register values
        tmc.TMCWaveTableHelper(config, self.mcu_tmc)
        tmc.TMCStealthchopHelper(config, self.mcu_tmc)
        tmc.TMCVcoolthrsHelper(config, self.mcu_tmc)
        tmc.TMCVhighHelper(config, self.mcu_tmc)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        # CHOPCONF
        set_config_field(config, "toff", 4)
        set_config_field(config, "hstrt", 0)
        set_config_field(config, "hend", 7)
        set_config_field(config, "tbl", 1)
        set_config_field(config, "vhighfs", 0)
        set_config_field(config, "vhighchm", 0)
        # COOLCONF
        set_config_field(config, "semin", 0)
        set_config_field(config, "seup", 0)
        set_config_field(config, "semax", 0)
        set_config_field(config, "sedn", 0)
        set_config_field(config, "seimin", 0)
        set_config_field(config, "sgt", 0)
        set_config_field(config, "sfilt", 0)
        # IHOLDIRUN
        set_config_field(config, "iholddelay", 8)
        # PWMCONF
        set_config_field(config, "pwm_ampl", 128)
        set_config_field(config, "pwm_grad", 4)
        set_config_field(config, "pwm_freq", 1)
        set_config_field(config, "pwm_autoscale", True)
        # TPOWERDOWN
        set_config_field(config, "tpowerdown", 0)

def load_config_prefix(config):
    return TMC2130(config)
