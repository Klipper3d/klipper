#

import tmc2208, tmc2130, tmc5160, tmc
TMC_FREQUENCY=12000000.

Registers = dict(tmc2208.Registers)

Registers.update({
	"TCOOLTHRS": 0x14,
	"COOLCONF": 0x42
})

#

ReadRegisters = tmc2208.ReadRegisters
#ReadRegisters.append()
#2208
#ReadRegisters = [
#    "GCONF", "GSTAT", "IFCNT", "OTP_READ", "IOIN", "FACTORY_CONF", "TSTEP",
#    "MSCNT", "MSCURACT", "CHOPCONF", "DRV_STATUS",
#    "PWMCONF", "PWM_SCALE", "PWM_AUTO"
#]

#2130
#ReadRegisters = [
#    "GCONF", "GSTAT", "IOIN", "TSTEP", "XDIRECT", "MSCNT", "MSCURACT",
#    "CHOPCONF", "DRV_STATUS", "PWM_SCALE", "LOST_STEPS",
#]

#

fields = dict(tmc2208.Fields)
fields.update({
	"COOLCONF": tmc5160.Fields["COOLCONF"]
})

FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update({
    "SEL_A":            (lambda v: "%d(%s)" % (v, ["TMC222x", "TMC220x"][v])),
    "s2vsa":            (lambda v: "1(LowSideShort_A!)" if v else ""),
    "s2vsb":            (lambda v: "1(LowSideShort_B!)" if v else ""),
})

#

######################################################################
# TMC2209 printer object
######################################################################

class TMC2209:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(fields, tmc2208.SignedFields, FieldFormatters)
        self.mcu_tmc = tmc2208.MCU_TMC_uart(config, Registers, self.fields)
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
        tmc.TMCStealthchopHelper(config, self.mcu_tmc, TMC_FREQUENCY)
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
        set_config_field(config, "sgt", 0)
    def query_registers(self, print_time=0.):
        out = []
        for reg_name in ReadRegisters:
            val = self.mcu_tmc.get_register(reg_name)
            # IOIN has different mappings depending on the driver type
            # (SEL_A field of IOIN reg)
            if reg_name == "IOIN":
                drv_type = self.fields.get_field("SEL_A", val)
                reg_name = "IOIN@TMC220x" if drv_type else "IOIN@TMC222x"
            out.append((reg_name, val))
        return out

def load_config_prefix(config):
    return TMC2209(config)
