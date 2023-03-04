# TMC2208 UART communication and configuration
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import tmc, tmc_uart, tmc2130

TMC_FREQUENCY=12000000.

Registers = {
    "GCONF": 0x00, "GSTAT": 0x01, "IFCNT": 0x02, "SLAVECONF": 0x03,
    "OTP_PROG": 0x04, "OTP_READ": 0x05, "IOIN": 0x06, "FACTORY_CONF": 0x07,
    "IHOLD_IRUN": 0x10, "TPOWERDOWN": 0x11, "TSTEP": 0x12, "TPWMTHRS": 0x13,
    "VACTUAL": 0x22, "MSCNT": 0x6a, "MSCURACT": 0x6b, "CHOPCONF": 0x6c,
    "DRV_STATUS": 0x6f, "PWMCONF": 0x70, "PWM_SCALE": 0x71, "PWM_AUTO": 0x72
}

ReadRegisters = [
    "GCONF", "GSTAT", "IFCNT", "OTP_READ", "IOIN", "FACTORY_CONF", "TSTEP",
    "MSCNT", "MSCURACT", "CHOPCONF", "DRV_STATUS",
    "PWMCONF", "PWM_SCALE", "PWM_AUTO"
]

Fields = {}

Fields["GCONF"] = {
    "i_scale_analog":      0x01,
    "internal_rsense":     0x01 << 1,
    "en_spreadcycle":      0x01 << 2,
    "shaft":               0x01 << 3,
    "index_otpw":          0x01 << 4,
    "index_step":          0x01 << 5,
    "pdn_disable":         0x01 << 6,
    "mstep_reg_select":    0x01 << 7,
    "multistep_filt":      0x01 << 8,
    "test_mode":           0x01 << 9
}
Fields["GSTAT"] = {
    "reset":               0x01,
    "drv_err":             0x01 << 1,
    "uv_cp":               0x01 << 2
}
Fields["IFCNT"] = {
    "ifcnt":               0xff
}
Fields["SLAVECONF"] = {
    "senddelay":           0x0f << 8
}
Fields["OTP_PROG"] = {
    "otpbit":              0x07,
    "otpbyte":             0x03 << 4,
    "otpmagic":            0xff << 8
}
Fields["OTP_READ"] = {
    "otp_fclktrim":        0x1f,
    "otp_ottrim":          0x01 << 5,
    "otp_internalrsense":  0x01 << 6,
    "otp_tbl":             0x01 << 7,
    "otp_pwm_grad":        0x0f << 8,
    "otp_pwm_autograd":    0x01 << 12,
    "otp_tpwmthrs":        0x07 << 13,
    "otp_pwm_ofs":         0x01 << 16,
    "otp_pwm_reg":         0x01 << 17,
    "otp_pwm_freq":        0x01 << 18,
    "otp_iholddelay":      0x03 << 19,
    "otp_ihold":           0x03 << 21,
    "otp_en_spreadcycle":  0x01 << 23
}
# IOIN mapping depends on the driver type (SEL_A field)
# TMC222x (SEL_A == 0)
Fields["IOIN@TMC222x"] = {
    "pdn_uart":            0x01 << 1,
    "spread":              0x01 << 2,
    "dir":                 0x01 << 3,
    "enn":                 0x01 << 4,
    "step":                0x01 << 5,
    "ms1":                 0x01 << 6,
    "ms2":                 0x01 << 7,
    "sel_a":               0x01 << 8,
    "version":             0xff << 24
}
# TMC220x (SEL_A == 1)
Fields["IOIN@TMC220x"] = {
    "enn":                 0x01,
    "ms1":                 0x01 << 2,
    "ms2":                 0x01 << 3,
    "diag":                0x01 << 4,
    "pdn_uart":            0x01 << 6,
    "step":                0x01 << 7,
    "sel_a":               0x01 << 8,
    "dir":                 0x01 << 9,
    "version":             0xff << 24,
}
Fields["FACTORY_CONF"] = {
    "fclktrim":            0x1f,
    "ottrim":              0x03 << 8
}
Fields["IHOLD_IRUN"] = {
    "ihold":               0x1f,
    "irun":                0x1f << 8,
    "iholddelay":          0x0f << 16
}
Fields["TPOWERDOWN"] = {
    "tpowerdown":          0xff
}
Fields["TSTEP"] = {
    "tstep":               0xfffff
}
Fields["TPWMTHRS"] = {
    "tpwmthrs":            0xfffff
}
Fields["VACTUAL"] = {
    "vactual":             0xffffff
}
Fields["MSCNT"] = {
    "mscnt":               0x3ff
}
Fields["MSCURACT"] = {
    "cur_a":               0x1ff,
    "cur_b":               0x1ff << 16
}
Fields["CHOPCONF"] = {
    "toff":                0x0f,
    "hstrt":               0x07 << 4,
    "hend":                0x0f << 7,
    "tbl":                 0x03 << 15,
    "vsense":              0x01 << 17,
    "mres":                0x0f << 24,
    "intpol":              0x01 << 28,
    "dedge":               0x01 << 29,
    "diss2g":              0x01 << 30,
    "diss2vs":             0x01 << 31
}
Fields["DRV_STATUS"] = {
    "otpw":                0x01,
    "ot":                  0x01 << 1,
    "s2ga":                0x01 << 2,
    "s2gb":                0x01 << 3,
    "s2vsa":               0x01 << 4,
    "s2vsb":               0x01 << 5,
    "ola":                 0x01 << 6,
    "olb":                 0x01 << 7,
    "t120":                0x01 << 8,
    "t143":                0x01 << 9,
    "t150":                0x01 << 10,
    "t157":                0x01 << 11,
    "cs_actual":           0x1f << 16,
    "stealth":             0x01 << 30,
    "stst":                0x01 << 31
}
Fields["PWMCONF"] = {
    "pwm_ofs":             0xff,
    "pwm_grad":            0xff << 8,
    "pwm_freq":            0x03 << 16,
    "pwm_autoscale":       0x01 << 18,
    "pwm_autograd":        0x01 << 19,
    "freewheel":           0x03 << 20,
    "pwm_reg":             0xf << 24,
    "pwm_lim":             0xf << 28
}
Fields["PWM_SCALE"] = {
    "pwm_scale_sum":       0xff,
    "pwm_scale_auto":      0x1ff << 16
}
Fields["PWM_AUTO"] = {
    "pwm_ofs_auto":        0xff,
    "pwm_grad_auto":       0xff << 16
}

SignedFields = ["cur_a", "cur_b", "pwm_scale_auto"]

FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update({
    "sel_a":            (lambda v: "%d(%s)" % (v, ["TMC222x", "TMC220x"][v])),
    "s2vsa":            (lambda v: "1(ShortToSupply_A!)" if v else ""),
    "s2vsb":            (lambda v: "1(ShortToSupply_B!)" if v else ""),
})


######################################################################
# TMC2208 printer object
######################################################################

class TMC2208:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.mcu_tmc = tmc_uart.MCU_TMC_uart(config, Registers, self.fields)
        self.fields.set_field("pdn_disable", True)
        # Register commands
        current_helper = tmc2130.TMCCurrentHelper(config, self.mcu_tmc)
        cmdhelper = tmc.TMCCommandHelper(config, self.mcu_tmc, current_helper)
        cmdhelper.setup_register_dump(ReadRegisters, self.read_translate)
        self.get_phase_offset = cmdhelper.get_phase_offset
        self.get_status = cmdhelper.get_status
        # Setup basic register values
        self.fields.set_field("mstep_reg_select", True)
        self.fields.set_field("multistep_filt", True)
        tmc.TMCStealthchopHelper(config, self.mcu_tmc, TMC_FREQUENCY)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        # CHOPCONF
        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 0)
        set_config_field(config, "tbl", 2)
        # IHOLDIRUN
        set_config_field(config, "iholddelay", 8)
        # PWMCONF
        set_config_field(config, "pwm_ofs", 36)
        set_config_field(config, "pwm_grad", 14)
        set_config_field(config, "pwm_freq", 1)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "pwm_autograd", True)
        set_config_field(config, "pwm_reg", 8)
        set_config_field(config, "pwm_lim", 12)
        # TPOWERDOWN
        set_config_field(config, "tpowerdown", 20)
    def read_translate(self, reg_name, val):
        if reg_name == "IOIN":
            drv_type = self.fields.get_field("sel_a", val)
            reg_name = "IOIN@TMC220x" if drv_type else "IOIN@TMC222x"
        return reg_name, val

def load_config_prefix(config):
    return TMC2208(config)
