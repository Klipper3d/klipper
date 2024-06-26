# TMC5160 configuration
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
from . import bus, tmc, tmc2130

TMC_FREQUENCY=12000000.

Registers = {
    "GCONF":            0x00,
    "GSTAT":            0x01,
    "IFCNT":            0x02,
    "SLAVECONF":        0x03,
    "IOIN":             0x04,
    "X_COMPARE":        0x05,
    "OTP_READ":         0x07,
    "FACTORY_CONF":     0x08,
    "SHORT_CONF":       0x09,
    "DRV_CONF":         0x0A,
    "GLOBALSCALER":     0x0B,
    "OFFSET_READ":      0x0C,
    "IHOLD_IRUN":       0x10,
    "TPOWERDOWN":       0x11,
    "TSTEP":            0x12,
    "TPWMTHRS":         0x13,
    "TCOOLTHRS":        0x14,
    "THIGH":            0x15,
    "RAMPMODE":         0x20,
    "XACTUAL":          0x21,
    "VACTUAL":          0x22,
    "VSTART":           0x23,
    "A1":               0x24,
    "V1":               0x25,
    "AMAX":             0x26,
    "VMAX":             0x27,
    "DMAX":             0x28,
    "D1":               0x2A,
    "VSTOP":            0x2B,
    "TZEROWAIT":        0x2C,
    "XTARGET":          0x2D,
    "VDCMIN":           0x33,
    "SW_MODE":          0x34,
    "RAMP_STAT":        0x35,
    "XLATCH":           0x36,
    "ENCMODE":          0x38,
    "X_ENC":            0x39,
    "ENC_CONST":        0x3A,
    "ENC_STATUS":       0x3B,
    "ENC_LATCH":        0x3C,
    "ENC_DEVIATION":    0x3D,
    "MSLUT0":           0x60,
    "MSLUT1":           0x61,
    "MSLUT2":           0x62,
    "MSLUT3":           0x63,
    "MSLUT4":           0x64,
    "MSLUT5":           0x65,
    "MSLUT6":           0x66,
    "MSLUT7":           0x67,
    "MSLUTSEL":         0x68,
    "MSLUTSTART":       0x69,
    "MSCNT":            0x6A,
    "MSCURACT":         0x6B,
    "CHOPCONF":         0x6C,
    "COOLCONF":         0x6D,
    "DCCTRL":           0x6E,
    "DRV_STATUS":       0x6F,
    "PWMCONF":          0x70,
    "PWM_SCALE":        0x71,
    "PWM_AUTO":         0x72,
    "LOST_STEPS":       0x73,
}

ReadRegisters = [
    "GCONF", "CHOPCONF", "GSTAT", "DRV_STATUS", "FACTORY_CONF", "IOIN",
    "LOST_STEPS", "MSCNT", "MSCURACT", "OTP_READ", "PWM_SCALE",
    "PWM_AUTO", "TSTEP"
]

Fields = {}
Fields["COOLCONF"] = {
    "semin":                    0x0F << 0,
    "seup":                     0x03 << 5,
    "semax":                    0x0F << 8,
    "sedn":                     0x03 << 13,
    "seimin":                   0x01 << 15,
    "sgt":                      0x7F << 16,
    "sfilt":                    0x01 << 24
}
Fields["CHOPCONF"] = {
    "toff":                     0x0F << 0,
    "hstrt":                    0x07 << 4,
    "hend":                     0x0F << 7,
    "fd3":                      0x01 << 11,
    "disfdcc":                  0x01 << 12,
    "chm":                      0x01 << 14,
    "tbl":                      0x03 << 15,
    "vhighfs":                  0x01 << 18,
    "vhighchm":                 0x01 << 19,
    "tpfd":                     0x0F << 20, # midrange resonances
    "mres":                     0x0F << 24,
    "intpol":                   0x01 << 28,
    "dedge":                    0x01 << 29,
    "diss2g":                   0x01 << 30,
    "diss2vs":                  0x01 << 31
}
Fields["DRV_CONF"] = {
    "bbmtime":                  0x1F << 0,
    "bbmclks":                  0x0F << 8,
    "otselect":                 0x03 << 16,
    "drvstrength":              0x03 << 18,
    "filt_isense":              0x03 << 20,
}
Fields["DRV_STATUS"] = {
    "sg_result":                0x3FF << 0,
    "s2vsa":                    0x01 << 12,
    "s2vsb":                    0x01 << 13,
    "stealth":                  0x01 << 14,
    "fsactive":                 0x01 << 15,
    "cs_actual":                0x1F << 16,
    "stallguard":               0x01 << 24,
    "ot":                       0x01 << 25,
    "otpw":                     0x01 << 26,
    "s2ga":                     0x01 << 27,
    "s2gb":                     0x01 << 28,
    "ola":                      0x01 << 29,
    "olb":                      0x01 << 30,
    "stst":                     0x01 << 31
}
Fields["FACTORY_CONF"] = {
    "factory_conf":             0x1F << 0
}
Fields["GCONF"] = {
    "recalibrate":              0x01 << 0,
    "faststandstill":           0x01 << 1,
    "en_pwm_mode":              0x01 << 2,
    "multistep_filt":           0x01 << 3,
    "shaft":                    0x01 << 4,
    "diag0_error":              0x01 << 5,
    "diag0_otpw":               0x01 << 6,
    "diag0_stall":              0x01 << 7,
    "diag1_stall":              0x01 << 8,
    "diag1_index":              0x01 << 9,
    "diag1_onstate":            0x01 << 10,
    "diag1_steps_skipped":      0x01 << 11,
    "diag0_int_pushpull":       0x01 << 12,
    "diag1_poscomp_pushpull":   0x01 << 13,
    "small_hysteresis":         0x01 << 14,
    "stop_enable":              0x01 << 15,
    "direct_mode":              0x01 << 16,
    "test_mode":                0x01 << 17
}
Fields["GSTAT"] = {
    "reset":                    0x01 << 0,
    "drv_err":                  0x01 << 1,
    "uv_cp":                    0x01 << 2
}
Fields["GLOBALSCALER"] = {
    "globalscaler":             0xFF << 0
}
Fields["IHOLD_IRUN"] = {
    "ihold":                    0x1F << 0,
    "irun":                     0x1F << 8,
    "iholddelay":               0x0F << 16
}
Fields["IOIN"] = {
    "refl_step":                0x01 << 0,
    "refr_dir":                 0x01 << 1,
    "encb_dcen_cfg4":           0x01 << 2,
    "enca_dcin_cfg5":           0x01 << 3,
    "drv_enn":                  0x01 << 4,
    "enc_n_dco_cfg6":           0x01 << 5,
    "sd_mode":                  0x01 << 6,
    "swcomp_in":                0x01 << 7,
    "version":                  0xFF << 24
}
Fields["LOST_STEPS"] = {
    "lost_steps":               0xfffff << 0
}
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
Fields["MSCNT"] = {
    "mscnt":                    0x3ff << 0
}
Fields["MSCURACT"] = {
    "cur_a":                    0x1ff << 0,
    "cur_b":                    0x1ff << 16
}
Fields["OTP_READ"] = {
    "otp_fclktrim":             0x1f << 0,
    "otp_s2_level":             0x01 << 5,
    "otp_bbm":                  0x01 << 6,
    "otp_tbl":                  0x01 << 7
}
Fields["PWM_AUTO"] = {
    "pwm_ofs_auto":             0xff << 0,
    "pwm_grad_auto":            0xff << 16
}
Fields["PWMCONF"] = {
    "pwm_ofs":                  0xFF << 0,
    "pwm_grad":                 0xFF << 8,
    "pwm_freq":                 0x03 << 16,
    "pwm_autoscale":            0x01 << 18,
    "pwm_autograd":             0x01 << 19,
    "freewheel":                0x03 << 20,
    "pwm_reg":                  0x0F << 24,
    "pwm_lim":                  0x0F << 28
}
Fields["PWM_SCALE"] = {
    "pwm_scale_sum":            0xff << 0,
    "pwm_scale_auto":           0x1ff << 16
}
Fields["TPOWERDOWN"] = {
    "tpowerdown":               0xff << 0
}
Fields["TPWMTHRS"] = {
    "tpwmthrs":                 0xfffff << 0
}
Fields["TCOOLTHRS"] = {
    "tcoolthrs":                0xfffff << 0
}
Fields["TSTEP"] = {
    "tstep":                    0xfffff << 0
}
Fields["THIGH"] = {
    "thigh":                    0xfffff << 0
}

SignedFields = ["cur_a", "cur_b", "sgt", "xactual", "vactual", "pwm_scale_auto"]

FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update({
    "s2vsa":            (lambda v: "1(ShortToSupply_A!)" if v else ""),
    "s2vsb":            (lambda v: "1(ShortToSupply_B!)" if v else ""),
})


######################################################################
# TMC stepper current config helper
######################################################################

VREF = 0.325
MAX_CURRENT = 10.000 # Maximum dependent on board, but 10 is safe sanity check

class TMC5160CurrentHelper:
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
        self.sense_resistor = config.getfloat('sense_resistor', 0.075, above=0.)
        gscaler, irun, ihold = self._calc_current(run_current, hold_current)
        self.fields.set_field("globalscaler", gscaler)
        self.fields.set_field("ihold", ihold)
        self.fields.set_field("irun", irun)
    def _calc_globalscaler(self, current):
        globalscaler = int((current * 256. * math.sqrt(2.)
                            * self.sense_resistor / VREF) + .5)
        globalscaler = max(32, globalscaler)
        if globalscaler >= 256:
            globalscaler = 0
        return globalscaler
    def _calc_current_bits(self, current, globalscaler):
        if not globalscaler:
            globalscaler = 256
        cs = int((current * 256. * 32. * math.sqrt(2.) * self.sense_resistor)
                 / (globalscaler * VREF)
                 - 1. + .5)
        return max(0, min(31, cs))
    def _calc_current(self, run_current, hold_current):
        gscaler = self._calc_globalscaler(run_current)
        irun = self._calc_current_bits(run_current, gscaler)
        ihold = self._calc_current_bits(min(hold_current, run_current), gscaler)
        return gscaler, irun, ihold
    def _calc_current_from_field(self, field_name):
        globalscaler = self.fields.get_field("globalscaler")
        if not globalscaler:
            globalscaler = 256
        bits = self.fields.get_field(field_name)
        return (globalscaler * (bits + 1) * VREF
                / (256. * 32. * math.sqrt(2.) * self.sense_resistor))
    def get_current(self):
        run_current = self._calc_current_from_field("irun")
        hold_current = self._calc_current_from_field("ihold")
        return run_current, hold_current, self.req_hold_current, MAX_CURRENT
    def set_current(self, run_current, hold_current, print_time):
        self.req_hold_current = hold_current
        gscaler, irun, ihold = self._calc_current(run_current, hold_current)
        val = self.fields.set_field("globalscaler", gscaler)
        self.mcu_tmc.set_register("GLOBALSCALER", val, print_time)
        self.fields.set_field("ihold", ihold)
        val = self.fields.set_field("irun", irun)
        self.mcu_tmc.set_register("IHOLD_IRUN", val, print_time)


######################################################################
# TMC5160 printer object
######################################################################

class TMC5160:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_tmc = tmc2130.MCU_TMC_SPI(config, Registers, self.fields,
                                           TMC_FREQUENCY)
        # Allow virtual pins to be created
        tmc.TMCVirtualPinHelper(config, self.mcu_tmc)
        # Register commands
        current_helper = TMC5160CurrentHelper(config, self.mcu_tmc)
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
        #   GCONF
        set_config_field(config, "multistep_filt", True)
        #   CHOPCONF
        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 2)
        set_config_field(config, "fd3", 0)
        set_config_field(config, "disfdcc", 0)
        set_config_field(config, "chm", 0)
        set_config_field(config, "tbl", 2)
        set_config_field(config, "vhighfs", 0)
        set_config_field(config, "vhighchm", 0)
        set_config_field(config, "tpfd", 4)
        set_config_field(config, "diss2g", 0)
        set_config_field(config, "diss2vs", 0)
        #   COOLCONF
        set_config_field(config, "semin", 0)    # page 52
        set_config_field(config, "seup", 0)
        set_config_field(config, "semax", 0)
        set_config_field(config, "sedn", 0)
        set_config_field(config, "seimin", 0)
        set_config_field(config, "sgt", 0)
        set_config_field(config, "sfilt", 0)
        #   DRV_CONF
        set_config_field(config, "drvstrength", 0)
        set_config_field(config, "bbmclks", 4)
        set_config_field(config, "bbmtime", 0)
        set_config_field(config, "filt_isense", 0)
        #   IHOLDIRUN
        set_config_field(config, "iholddelay", 6)
        #   PWMCONF
        set_config_field(config, "pwm_ofs", 30)
        set_config_field(config, "pwm_grad", 0)
        set_config_field(config, "pwm_freq", 0)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "pwm_autograd", True)
        set_config_field(config, "freewheel", 0)
        set_config_field(config, "pwm_reg", 4)
        set_config_field(config, "pwm_lim", 12)
        #   TPOWERDOWN
        set_config_field(config, "tpowerdown", 10)

def load_config_prefix(config):
    return TMC5160(config)
