# TMCHAX frankensteined configuration
#
# Copyright (C) 2019  Stephan Oelze <stephan.oelze@gmail.com>
# Copyright (C) 2023  Jookia <contact@jookia.org>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import tmc2208, tmc2130, tmc, tmc_uart
import logging

TMC_FREQUENCY=12000000.

Registers = {
    "GCONF": 0x00, "IFCNT": 0x02, "SLAVECONF": 0x03, "FACTORY_CONF": 0x07,
    "IHOLD_IRUN": 0x10, "TPOWERDOWN": 0x11, "TSTEP": 0x12, "TPWMTHRS": 0x13,
    "TCOOLTHRS": 0x14, "VACTUAL": 0x22, "COOLCONF": 0x42, "SGTHRS": 0x40,
    "CHOPCONF": 0x6c, "PWMCONF": 0x70
}

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
Fields["IFCNT"] = {
    "ifcnt":               0xff
}
Fields["SLAVECONF"] = {
    "senddelay":           0x0f << 8
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
Fields["COOLCONF"] = {
    "semin":        0x0F << 0,
    "seup":         0x03 << 5,
    "semax":        0x0F << 8,
    "sedn":         0x03 << 13,
    "seimin":       0x01 << 15
}
Fields["SGTHRS"] = {
    "sgthrs":       0xFF << 0
}
Fields["TCOOLTHRS"] = {
    "tcoolthrs": 0xfffff
}

class TMCCommandHelperF:
    def __init__(self, config, mcu_tmc, current_helper):
        self.printer = config.get_printer()
        self.stepper_name = ' '.join(config.get_name().split()[1:])
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.current_helper = current_helper
        self.fields = mcu_tmc.get_fields()
        self.toff = None
        self.mcu_phase_offset = None
        self.stepper = None
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        # Set microstep config options
        tmc.TMCMicrostepHelper(config, mcu_tmc)
    def _init_registers(self, print_time=None):
        # Send registers
        for reg_name, val in self.fields.registers.items():
            self.mcu_tmc.set_register(reg_name, val, print_time)
    def _handle_connect(self):
        self._init_registers()

# Helper code for communicating via TMC uart
class MCU_TMC_uart_f:
    def __init__(self, config, name_to_reg, fields, max_addr=0):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.name_to_reg = name_to_reg
        self.fields = fields
        self.ifcnt = None
        self.instance_id, self.addr, self.mcu_uart = tmc_uart.lookup_tmc_uart_bitbang(
            config, max_addr)
        self.mutex = self.mcu_uart.mutex
    def get_fields(self):
        return self.fields
    def _do_get_register(self, reg_name):
        raise self.printer.command_error(
            "f Unable to read tmc uart '%s' register %s" % (self.name, reg_name))
    def get_register(self, reg_name):
        with self.mutex:
            return self._do_get_register(reg_name)
    def set_register(self, reg_name, val, print_time=None):
        reg = self.name_to_reg[reg_name]
        if self.printer.get_start_args().get('debugoutput') is not None:
            return
        with self.mutex:
            for retry in range(5):
                self.mcu_uart.reg_write(self.instance_id, self.addr, reg, val,
                                        print_time)
        return


######################################################################
# TMC2209 printer object
######################################################################

mytmc = None

class TMCHAX:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(Fields)
        self.mcu_tmc = MCU_TMC_uart_f(config, Registers, self.fields, 3)
        # Setup fields for UART
        self.fields.set_field("pdn_disable", True)
        self.fields.set_field("senddelay", 2) # Avoid tx errors on shared uart
        # Allow virtual pins to be created
        tmc.TMCVirtualPinHelper(config, self.mcu_tmc)
        # Register commands
        current_helper = tmc2130.TMCCurrentHelper(config, self.mcu_tmc)
        cmdhelper = TMCCommandHelperF(config, self.mcu_tmc, current_helper)
        # Setup basic register values
        self.fields.set_field("pdn_disable", True)
        self.fields.set_field("mstep_reg_select", True)
        self.fields.set_field("multistep_filt", True)
        tmc.TMCStealthchopHelper(config, self.mcu_tmc, TMC_FREQUENCY)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        set_config_field(config, "toff", 3)
        set_config_field(config, "hstrt", 5)
        set_config_field(config, "hend", 0)
        set_config_field(config, "tbl", 2)
        set_config_field(config, "iholddelay", 8)
        set_config_field(config, "tpowerdown", 20)
        set_config_field(config, "pwm_ofs", 36)
        set_config_field(config, "pwm_grad", 14)
        set_config_field(config, "pwm_freq", 1)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "pwm_autograd", True)
        set_config_field(config, "pwm_reg", 8)
        set_config_field(config, "pwm_lim", 12)
        set_config_field(config, "sgthrs", 0)

def load_config_prefix(config):
    global mytmc
    if mytmc is None:
        mytmc = TMCHAX(config)
    return mytmc
