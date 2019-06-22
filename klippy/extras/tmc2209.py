# TMC2209 configuration
#
# Copyright (C) 2019  Stephan Oelze stephan.oelze@gmail.com
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import tmc2208, tmc2130, tmc, tmc_uart
TMC_FREQUENCY=12000000.

Registers = dict(tmc2208.Registers)

Registers.update({
    "TCOOLTHRS":    0x14,
    "COOLCONF":     0x42,
    "SGTHRS":       0x40,
    "SG_RESULT":    0x41
})

ReadRegisters = tmc2208.ReadRegisters
ReadRegisters.append("SG_RESULT")

fields = dict(tmc2208.Fields)
fields["COOLCONF"] = {
    "semin":                    0x0F << 0,
    "seup":                     0x03 << 5,
    "semax":                    0x0F << 8,
    "sedn":                     0x03 << 13,
    "seimin":                   0x01 << 15
}
fields["IOIN"] = {
    "ENN":       0x01 << 0,
    "0":         0x01 << 1,
    "MS1":       0x01 << 2,
    "MS2":       0x01 << 3,
    "DIAG":      0x01 << 4,
    "0":         0x01 << 5,
    "PDN_UART":  0x01 << 6,
    "STEP":      0x01 << 7,
    "SPREAD_EN": 0x01 << 8,
    "DIR":       0x01 << 9,
    "VERSION":   0xff << 24
}
fields["SGTHRS"] = {
    "SGTHRS":      0xFF << 0
}
fields["SG_RESULT"] = {
    "SG_RESULT": 0x3FF << 0
}

FieldFormatters = dict(tmc2208.FieldFormatters)

#

######################################################################
# TMC2209 printer object
######################################################################

class TMC2209:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(fields, tmc2208.SignedFields,\
            FieldFormatters)
        self.mcu_tmc = tmc_uart.MCU_TMC_uart(config, Registers, self.fields)
        # Allow virtual endstop to be created
        diag_pin = config.get('diag_pin', None)
        tmc.TMCEndstopHelper(config, self.mcu_tmc, diag_pin, tmc_type=2209)
        # Register commands
        cmdhelper = tmc.TMCCommandHelper(config, self.mcu_tmc)
        cmdhelper.setup_register_dump(self.query_registers)
        # Setup basic register values
        self.fields.set_field("pdn_disable", True)
        self.fields.set_field("mstep_reg_select", True)
        self.fields.set_field("multistep_filt", True)
        tmc2130.TMCCurrentHelper(config, self.mcu_tmc)
        mh = tmc.TMCMicrostepHelper(config, self.mcu_tmc)
        tmc2208.get_microsteps = mh.get_microsteps
        self.get_phase = mh.get_phase
        tmc.TMCStealthchopHelper(config, self.mcu_tmc, TMC_FREQUENCY,\
            tmc_type=2209)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 0)
        set_config_field(config, "TBL", 2)
        set_config_field(config, "intpol", True, "interpolate")
        set_config_field(config, "IHOLDDELAY", 8)
        set_config_field(config, "TPOWERDOWN", 20)
        set_config_field(config, "PWM_OFS", 36)
        set_config_field(config, "PWM_GRAD", 14)
        set_config_field(config, "pwm_freq", 1)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "pwm_autograd", True)
        set_config_field(config, "PWM_REG", 8)
        set_config_field(config, "PWM_LIM", 12)
        set_config_field(config, "SGTHRS", 0)
    def query_registers(self, print_time=0.):
        return [(reg_name, self.mcu_tmc.get_register(reg_name))
                for reg_name in ReadRegisters]

def load_config_prefix(config):
    return TMC2209(config)
