# TMC5160 configuration
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, collections
import bus, tmc2130

vsense = 0.325
GLOBAL_SCALER = 256
TMC_FREQUENCY=12000000.

registers = {
    "GCONF":            0x00 ,
    "GSTAT":            0x01 ,
    "IFCNT":            0x02 ,
    "SLAVECONF":        0x03 ,
    "IOIN":             0x04 ,
    "X_COMPARE":        0x05 ,
    "OTP_READ":         0x07 ,
    "FACTORY_CONF":     0x08 ,
    "SHORT_CONF":       0x09 ,
    "DRV_CONF":         0x0A ,
    "GLOBAL_SCALER":    0x0B ,
    "OFFSET_READ":      0x0C ,
    "IHOLD_IRUN":       0x10 ,
    "TPOWERDOWN":       0x11 ,
    "TSTEP":            0x12 ,
    "TPWMTHRS":         0x13 ,
    "TCOOLTHRS":        0x14 ,
    "THIGH":            0x15 ,
    "RAMPMODE":         0x20 ,
    "XACTUAL":          0x21 ,
    "VACTUAL":          0x22 ,
    "VSTART":           0x23 ,
    "A1":               0x24 ,
    "V1":               0x25 ,
    "AMAX":             0x26 ,
    "VMAX":             0x27 ,
    "DMAX":             0x28 ,
    "D1":               0x2A ,
    "VSTOP":            0x2B ,
    "TZEROWAIT":        0x2C ,
    "XTARGET":          0x2D ,
    "VDCMIN":           0x33 ,
    "SW_MODE":          0x34 ,
    "RAMP_STAT":        0x35 ,
    "XLATCH":           0x36 ,
    "ENCMODE":          0x38 ,
    "X_ENC":            0x39 ,
    "ENC_CONST":        0x3A ,
    "ENC_STATUS":       0x3B ,
    "ENC_LATCH":        0x3C ,
    "ENC_DEVIATION":    0x3D ,
    "MSLUT0":           0x60 ,
    "MSLUT1":           0x61 ,
    "MSLUT2":           0x62 ,
    "MSLUT3":           0x63 ,
    "MSLUT4":           0x64 ,
    "MSLUT5":           0x65 ,
    "MSLUT6":           0x66 ,
    "MSLUT7":           0x67 ,
    "MSLUTSEL":         0x68 ,
    "MSLUTSTART":       0x69 ,
    "MSCNT":            0x6A ,
    "MSCURACT":         0x6B ,
    "CHOPCONF":         0x6C ,
    "COOLCONF":         0x6D ,
    "DCCTRL":           0x6E ,
    "DRV_STATUS":       0x6F ,
    "PWMCONF":          0x70 ,
    "PWM_SCALE":        0x71 ,
    "PWM_AUTO":         0x72 ,
    "LOST_STEPS":       0x73
}

ReadRegisters = [
    "GCONF", "CHOPCONF", "GSTAT", "DRV_STATUS", "FACTORY_CONF", "IOIN",
    "LOST_STEPS", "MSCNT", "MSCURACT", "OTP_READ", "PWMCONF", "PWM_SCALE",
    "PWM_AUTO", "TSTEP"
]

fields = {}
fields["COOLCONF"] = {
    "semin":                    0x0F << 0,
    "seup":                     0x03 << 5,
    "semax":                    0x0F << 8,
    "sedn":                     0x03 << 13,
    "seimin":                   0x01 << 15,
    "sgt":                      0x7F << 16,
    "sfilt":                    0x01 << 24
}
fields["CHOPCONF"] = {
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
fields["DRV_STATUS"] = {
    "SG_RESULT":                0x3FF << 0,
    "s2vsa":                    0x01 << 12,
    "s2vsb":                    0x01 << 13,
    "stealth":                  0x01 << 14,
    "fsactive":                 0x01 << 15,
    "CSACTUAL":                 0xFF << 16,
    "stallGuard":               0x01 << 24,
    "ot":                       0x01 << 25,
    "otpw":                     0x01 << 26,
    "s2ga":                     0x01 << 27,
    "s2gb":                     0x01 << 28,
    "ola":                      0x01 << 29,
    "olb":                      0x01 << 30,
    "stst":                     0x01 << 31
}
fields["FACTORY_CONF"] = {
    "FACTORY_CONF":             0x1F << 0
}
fields["GCONF"] = {
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
fields["GSTAT"] = {
    "reset":                    0x01 << 0,
    "drv_err":                  0x01 << 1,
    "uv_cp":                    0x01 << 2
}
fields["IHOLD_IRUN"] = {
    "IHOLD":                    0x1F << 0,
    "IRUN":                     0x1F << 8,
    "IHOLDDELAY":               0x0F << 16
}
fields["IOIN"] = {
    "REFL_STEP":                0x01 << 0,
    "REFR_DIR":                 0x01 << 1,
    "ENCB_DCEN_CFG4":           0x01 << 2,
    "ENCA_DCIN_CFG5":           0x01 << 3,
    "DRV_ENN":                  0x01 << 4,
    "ENC_N_DCO_CFG6":           0x01 << 5,
    "SD_MODE":                  0x01 << 6,
    "SWCOMP_IN":                0x01 << 7,
    "VERSION":                  0xFF << 24
}
fields["LOST_STEPS"] = {
    "LOST_STEPS":               0xfffff << 0
}
fields["MSCNT"] = {
    "MSCNT":                    0x3ff << 0
}
fields["MSCURACT"] = {
    "CUR_A":                    0x1ff << 0,
    "CUR_B":                    0x1ff << 16
}
fields["OTP_READ"] = {
    "OTP_FCLKTRIM":             0x1f << 0,
    "otp_S2_LEVEL":             0x01 << 5,
    "otp_BBM":                  0x01 << 6,
    "otp_TBL":                  0x01 << 7
}
fields["PWM_AUTO"] = {
    "PWM_OFS_AUTO":             0xff << 0,
    "PWM_GRAD_AUTO":            0xff << 16
}
fields["PWMCONF"] = {
    "PWM_OFS":                  0xFF << 0,
    "PWM_GRAD":                 0xFF << 8,
    "pwm_freq":                 0x03 << 16,
    "pwm_autoscale":            0x01 << 18,
    "pwm_autograd":             0x01 << 19,
    "freewheel":                0x03 << 20,
    "PWM_REG":                  0x0F << 24,
    "PWM_LIM":                  0x0F << 28
}
fields["PWM_SCALE"] = {
    "PWM_SCALE_SUM":            0xff << 0,
    "PWM_SCALE_AUTO":           0x1ff << 16
}
fields["TPOWERDOWN"] = {
    "TPOWERDOWN":               0xff << 0
}
fields["TPWMTHRS"] = {
    "TPWMTHRS":                 0xfffff << 0
}
fields["TSTEP"] = {
    "TSTEP":                    0xfffff << 0
}

SignedFields = ["CUR_A", "CUR_B", "sgt", "XACTUAL", "VACTUAL", "PWM_SCALE_AUTO"]

FieldFormatters = dict(tmc2130.FieldFormatters)
FieldFormatters.update({
    "reset":            (lambda v: "1(reset)" if v else ""),
    "drv_err":          (lambda v: "1(ErrorShutdown!)" if v else ""),
    "uv_cp":            (lambda v: "1(Undervoltage!)" if v else ""),
})

######################################################################
# Config reading helpers
######################################################################

def bits_to_current(bits, sense_resistor, vsense_on):
    #current = (bits + 1) * vsense / (32 * sense_resistor * math.sqrt(2.))
    current = (bits + 1) * (GLOBAL_SCALER *vsense) \
        / (32*GLOBAL_SCALER*math.sqrt(2.)*sense_resistor)
    return round(current, 2)

def calc_current_config(run_current, hold_current, sense_resistor):
    irun = int( (32 * GLOBAL_SCALER * math.sqrt(2.) * run_current * \
        sense_resistor) / (GLOBAL_SCALER * vsense) -1 )
    ihold = int( (32 * GLOBAL_SCALER * math.sqrt(2.) * hold_current * \
        sense_resistor) / (GLOBAL_SCALER * vsense) -1 )
    return irun, ihold

def get_config_current(config):
    run_current = config.getfloat('run_current', above=0., maxval=3.)
    hold_current = config.getfloat('hold_current', run_current, above=0.,\
        maxval=3.)
    sense_resistor = config.getfloat('sense_resistor', 0.075, above=0.)
    irun, ihold = calc_current_config(run_current, hold_current,\
        sense_resistor)
    return irun, ihold, sense_resistor

######################################################################
# TMC5160 printer object
######################################################################

class TMC5160:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=100000)
        # Allow virtual endstop to be created
        self.diag1_pin = config.get('diag1_pin', None)
        ppins = self.printer.lookup_object("pins")
        ppins.register_chip("tmc5160_" + self.name, self)
        # Add DUMP_TMC, INIT_TMC command
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command(
            "SET_TMC_CURRENT", "STEPPER", self.name,
            self.cmd_SET_TMC_CURRENT, desc=self.cmd_SET_TMC_CURRENT_help)
        gcode.register_mux_command(
            "DUMP_TMC", "STEPPER", self.name,
            self.cmd_DUMP_TMC, desc=self.cmd_DUMP_TMC_help)
        gcode.register_mux_command(
            "SET_TMC_FIELD", "STEPPER", self.name,
            self.cmd_SET_TMC_FIELD, desc=self.cmd_SET_TMC_FIELD_help)
        gcode.register_mux_command(
            "INIT_TMC", "STEPPER", self.name,
            self.cmd_INIT_TMC, desc=self.cmd_INIT_TMC_help)
        # Setup basic register values
        self.regs = collections.OrderedDict()
        self.fields = tmc2130.FieldHelper(fields, SignedFields, FieldFormatters,
                                          self.regs)
        irun, ihold, self.sense_resistor = get_config_current(config)
        msteps, en_pwm, thresh = \
            tmc2130.get_config_stealthchop(config, TMC_FREQUENCY)
        set_config_field = self.fields.set_config_field
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
        self.fields.set_field("mres", msteps)       # microsteps
        set_config_field(config, "intpol", True, "interpolate")
        set_config_field(config, "dedge", 0)
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
        #   GCONF
        self.fields.set_field("en_pwm_mode", en_pwm)
        #   IHOLDIRUN
        self.fields.set_field("IHOLD", ihold)
        self.fields.set_field("IRUN", irun)
        set_config_field(config, "IHOLDDELAY", 6)
        #   PWMCONF
        set_config_field(config, "PWM_OFS", 30)
        set_config_field(config, "PWM_GRAD", 0)
        set_config_field(config, "pwm_freq", 1)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "pwm_autograd", True)
        set_config_field(config, "freewheel", 0)
        set_config_field(config, "PWM_REG", 4)
        set_config_field(config, "PWM_LIM", 12)
        #   TPWMTHRS
        self.fields.set_field("TPWMTHRS", thresh)
        #   TPOWERDOWN
        set_config_field(config, "TPOWERDOWN", 10)

        self._init_registers()
    def decode_hex(self, hex_, reg_name=False):
        reg = int( (hex_ >> 32) & 0xff - long(0x80) )
        if reg not in registers.values():
            reg = int( (hex_ >> 32) & 0xff )
            if reg not in registers.values():
                reg_name = "UFO"
        if not reg_name:
            for name, adr_ in registers.items():
                if adr_ == reg:
                    reg_name = name
        val = hex_ & 0xFFFFFFFF
        logging.debug(self.fields.pretty_format(reg_name, val))
    def _init_registers(self, min_clock = 0):
        for reg_name, val in self.regs.items():
            self.set_register(reg_name, val, min_clock)
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'virtual_endstop':
            raise pins.error("tmc5160 virtual endstop only useful as endstop")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert tmc5160 virtual endstop")
        return tmc2130.TMC2130VirtualEndstop(self)
    def get_register(self, reg_name):
        reg = registers[reg_name]
        self.spi.spi_send([reg, 0x00, 0x00, 0x00, 0x00])
        if self.printer.get_start_args().get('debugoutput') is not None:
            return 0
        params = self.spi.spi_transfer([reg, 0x00, 0x00, 0x00, 0x00])
        pr = bytearray(params['response'])
        return (pr[1] << 24) | (pr[2] << 16) | (pr[3] << 8) | pr[4]
    def set_register(self, reg_name, val, min_clock = 0):
        reg = registers[reg_name]
        data = [(reg | 0x80) & 0xff, (val >> 24) & 0xff, (val >> 16) & 0xff,
            (val >> 8) & 0xff, val & 0xff]
        self.decode_hex( val, reg_name=reg_name )
        self.spi.spi_send(data, min_clock)
    def set_adress(self, val, min_clock=0):
        data = [(val >> 32) & 0xff, (val >> 24) & 0xff,
                (val >> 16) & 0xff, (val >> 8) & 0xff, val & 0xff]
        self.spi.spi_send(data, min_clock)
    def get_microsteps(self):
        return 256 >> self.fields.get_field("MRES")
    def get_phase(self):
        mscnt = self.fields.get_field("MSCNT", self.get_register("MSCNT"))
        return mscnt >> self.fields.get_field("MRES")
    cmd_SET_TMC_CURRENT_help = "Set the current of a TMC5160 driver"
    def cmd_SET_TMC_CURRENT(self, params):
        gcode = self.printer.lookup_object('gcode')
        vsense = True
        if 'HOLDCURRENT' in params:
            hold_current = gcode.get_float(
                'HOLDCURRENT', params, above=0., maxval=3.)
        else:
            hold_current = bits_to_current(
                    self.fields.get_field("IHOLD"),
                    self.sense_resistor,
                    vsense)
        if 'CURRENT' in params:
            run_current = gcode.get_float(
                'CURRENT', params, minval=hold_current, maxval=2.)
        else:
            run_current = bits_to_current(
                    self.fields.get_field("IRUN"),
                    self.sense_resistor,
                    vsense)
        if 'HOLDCURRENT' in params or 'CURRENT' in params:
            print_time = self.printer.lookup_object('toolhead')\
                             .get_last_move_time()
            min_clock = self.spi.get_mcu().print_time_to_clock(print_time)
            irun, ihold = calc_current_config(run_current,
                                            hold_current, self.sense_resistor)
            self.set_register("CHOPCONF", self.regs["CHOPCONF"], min_clock)
            self.fields.set_field("IHOLD", ihold)
            self.fields.set_field("IRUN", irun)
            self.set_register("IHOLD_IRUN", self.regs["IHOLD_IRUN"], min_clock)
        else:
            gcode.respond_info(
                "Run Current: %0.2fA Hold Current: %0.2fA"
                % (run_current, hold_current))
    cmd_DUMP_TMC_help = "Read and display TMC stepper driver registers"
    def cmd_DUMP_TMC(self, params):
        self.printer.lookup_object('toolhead').get_last_move_time()
        gcode = self.printer.lookup_object('gcode')
        logging.info("DUMP_TMC %s", self.name)
        gcode.respond_info("========== Write-only registers ==========")
        for reg_name, val in self.regs.items():
            if reg_name not in ReadRegisters:
                gcode.respond_info(self.fields.pretty_format(reg_name, val))
        gcode.respond_info("========== Queried registers ==========")
        for reg_name in ReadRegisters:
            val = self.get_register(reg_name)
            gcode.respond_info(self.fields.pretty_format(reg_name, val))
    cmd_INIT_TMC_help = "Initialize TMC stepper driver registers"
    def cmd_INIT_TMC(self, params):
        logging.info("INIT_TMC 5160 %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        min_clock = self.spi.get_mcu().print_time_to_clock(print_time)
        self._init_registers(min_clock)
    cmd_SET_TMC_FIELD_help = "Set a register field of a TMC5160 driver"
    def cmd_SET_TMC_FIELD(self, params):
        gcode = self.printer.lookup_object('gcode')
        if ('FIELD' not in params or
            'VALUE' not in params):
            raise gcode.error("Invalid command format")
        field = gcode.get_str('FIELD', params)
        reg = self.fields.field_to_register.get(field)
        if reg is None:
            raise gcode.error("Unknown field name '%s'" % field)
        value = gcode.get_int('VALUE', params)
        self.fields.set_field(field, value)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        min_clock = self.spi.get_mcu().print_time_to_clock(print_time)
        self.set_register(reg, self.regs[reg], min_clock)

def load_config_prefix(config):
    return TMC5160(config)
