# TMC2130 configuration
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, collections
import bus

TMC_FREQUENCY=13200000.

Registers = {
    "GCONF": 0x00, "GSTAT": 0x01, "IOIN": 0x04, "IHOLD_IRUN": 0x10,
    "TPOWERDOWN": 0x11, "TSTEP": 0x12, "TPWMTHRS": 0x13, "TCOOLTHRS": 0x14,
    "THIGH": 0x15, "XDIRECT": 0x2d, "MSLUT0": 0x60, "MSLUTSEL": 0x68,
    "MSLUTSTART": 0x69, "MSCNT": 0x6a, "MSCURACT": 0x6b, "CHOPCONF": 0x6c,
    "COOLCONF": 0x6d, "DCCTRL": 0x6e, "DRV_STATUS": 0x6f, "PWMCONF": 0x70,
    "PWM_SCALE": 0x71, "ENCM_CTRL": 0x72, "LOST_STEPS": 0x73,
}

ReadRegisters = [
    "GCONF", "GSTAT", "IOIN", "TSTEP", "XDIRECT", "MSCNT", "MSCURACT",
    "CHOPCONF", "DRV_STATUS", "PWM_SCALE", "LOST_STEPS",
]

Fields = {}
Fields["GCONF"] = {
    "I_scale_analog": 1<<0, "internal_Rsense": 1<<1, "en_pwm_mode": 1<<2,
    "enc_commutation": 1<<3, "shaft": 1<<4, "diag0_error": 1<<5,
    "diag0_otpw": 1<<6, "diag0_stall": 1<<7, "diag1_stall": 1<<8,
    "diag1_index": 1<<9, "diag1_onstate": 1<<10, "diag1_steps_skipped": 1<<11,
    "diag0_int_pushpull": 1<<12, "diag1_pushpull": 1<<13,
    "small_hysteresis": 1<<14, "stop_enable": 1<<15, "direct_mode": 1<<16,
    "test_mode": 1<<17
}
Fields["GSTAT"] = { "reset": 1<<0, "drv_err": 1<<1, "uv_cp": 1<<2 }
Fields["IOIN"] = {
    "STEP": 1<<0, "DIR": 1<<1, "DCEN_CFG4": 1<<2, "DCIN_CFG5": 1<<3,
    "DRV_ENN_CFG6": 1<<4, "DCO": 1<<5, "VERSION": 0xff << 24
}
Fields["IHOLD_IRUN"] = {
    "IHOLD": 0x1f << 0, "IRUN": 0x1f << 8, "IHOLDDELAY": 0x0f << 16
}
Fields["TPOWERDOWN"] = { "TPOWERDOWN": 0xff }
Fields["TSTEP"] = { "TSTEP": 0xfffff }
Fields["TPWMTHRS"] = { "TPWMTHRS": 0xfffff }
Fields["TCOOLTHRS"] = { "TCOOLTHRS": 0xfffff }
Fields["THIGH"] = { "THIGH": 0xfffff }
Fields["MSCNT"] = { "MSCNT": 0x3ff }
Fields["MSCURACT"] = { "CUR_A": 0x1ff, "CUR_B": 0x1ff << 16 }
Fields["CHOPCONF"] = {
    "toff": 0x0f, "hstrt": 0x07 << 4, "hend": 0x0f << 7, "fd3": 1<<11,
    "disfdcc": 1<<12, "rndtf": 1<<13, "chm": 1<<14, "TBL": 0x03 << 15,
    "vsense": 1<<17, "vhighfs": 1<<18, "vhighchm": 1<<19, "sync": 0x0f << 20,
    "MRES": 0x0f << 24, "intpol": 1<<28, "dedge": 1<<29, "diss2g": 1<<30
}
Fields["COOLCONF"] = {
    "semin": 0x0f, "seup": 0x03 << 5, "semax": 0x0f << 8, "sedn": 0x03 << 13,
    "seimin": 1<<15, "sgt": 0x7f << 16, "sfilt": 1<<24
}
Fields["DRV_STATUS"] = {
    "SG_RESULT": 0x3ff, "fsactive": 1<<15, "CS_ACTUAL": 0x1f << 16,
    "stallGuard": 1<<24, "ot": 1<<25, "otpw": 1<<26, "s2ga": 1<<27,
    "s2gb": 1<<28, "ola": 1<<29, "olb": 1<<30, "stst": 1<<31
}
Fields["PWMCONF"] = {
    "PWM_AMPL": 0xff, "PWM_GRAD": 0xff << 8, "pwm_freq": 0x03 << 16,
    "pwm_autoscale": 1<<18, "pwm_symmetric": 1<<19, "freewheel": 0x03 << 20
}
Fields["PWM_SCALE"] = { "PWM_SCALE": 0xff }
Fields["LOST_STEPS"] = { "LOST_STEPS": 0xfffff }

SignedFields = ["CUR_A", "CUR_B", "sgt"]

FieldFormatters = {
    "I_scale_analog":   (lambda v: "1(ExtVREF)" if v else ""),
    "shaft":            (lambda v: "1(Reverse)" if v else ""),
    "drv_err":          (lambda v: "1(ErrorShutdown!)" if v else ""),
    "uv_cp":            (lambda v: "1(Undervoltage!)" if v else ""),
    "VERSION":          (lambda v: "%#x" % v),
    "MRES":             (lambda v: "%d(%dusteps)" % (v, 0x100 >> v)),
    "otpw":             (lambda v: "1(OvertempWarning!)" if v else ""),
    "ot":               (lambda v: "1(OvertempError!)" if v else ""),
    "s2ga":             (lambda v: "1(ShortToGND_A!)" if v else ""),
    "s2gb":             (lambda v: "1(ShortToGND_B!)" if v else ""),
    "ola":              (lambda v: "1(OpenLoad_A!)" if v else ""),
    "olb":              (lambda v: "1(OpenLoad_B!)" if v else ""),
}


######################################################################
# Field helpers
######################################################################

# Return the position of the first bit set in a mask
def ffs(mask):
    return (mask & -mask).bit_length() - 1

class FieldHelper:
    def __init__(self, all_fields, signed_fields=[], field_formatters={},
                 registers=None):
        self.all_fields = all_fields
        self.signed_fields = {sf: 1 for sf in signed_fields}
        self.field_formatters = field_formatters
        self.registers = registers
        if self.registers is None:
            self.registers = {}
        self.field_to_register = { f: r for r, fields in self.all_fields.items()
                                   for f in fields }
    def get_field(self, field_name, reg_value=None, reg_name=None):
        # Returns value of the register field
        if reg_name is None:
            reg_name = self.field_to_register[field_name]
        if reg_value is None:
            reg_value = self.registers[reg_name]
        mask = self.all_fields[reg_name][field_name]
        field_value = (reg_value & mask) >> ffs(mask)
        if field_name in self.signed_fields and ((reg_value & mask)<<1) > mask:
            field_value -= (1 << field_value.bit_length())
        return field_value
    def set_field(self, field_name, field_value, reg_value=None, reg_name=None):
        # Returns register value with field bits filled with supplied value
        if reg_name is None:
            reg_name = self.field_to_register[field_name]
        if reg_value is None:
            reg_value = self.registers.get(reg_name, 0)
        mask = self.all_fields[reg_name][field_name]
        new_value = (reg_value & ~mask) | ((field_value << ffs(mask)) & mask)
        self.registers[reg_name] = new_value
        return new_value
    def set_config_field(self, config, field_name, default, config_name=None):
        # Allow a field to be set from the config file
        if config_name is None:
            config_name = "driver_" + field_name.upper()
        reg_name = self.field_to_register[field_name]
        mask = self.all_fields[reg_name][field_name]
        maxval = mask >> ffs(mask)
        if maxval == 1:
            val = config.getboolean(config_name, default)
        elif field_name in self.signed_fields:
            val = config.getint(config_name, default,
                                minval=-(maxval//2 + 1), maxval=maxval//2)
        else:
            val = config.getint(config_name, default, minval=0, maxval=maxval)
        return self.set_field(field_name, val)
    def pretty_format(self, reg_name, reg_value):
        # Provide a string description of a register
        reg_fields = self.all_fields.get(reg_name, {})
        reg_fields = sorted([(mask, name) for name, mask in reg_fields.items()])
        fields = []
        for mask, field_name in reg_fields:
            field_value = self.get_field(field_name, reg_value, reg_name)
            sval = self.field_formatters.get(field_name, str)(field_value)
            if sval and sval != "0":
                fields.append(" %s=%s" % (field_name, sval))
        return "%-11s %08x%s" % (reg_name + ":", reg_value, "".join(fields))


######################################################################
# Config reading helpers
######################################################################

def current_bits(current, sense_resistor, vsense_on):
    sense_resistor += 0.020
    vsense = 0.32
    if vsense_on:
        vsense = 0.18
    cs = int(32. * current * sense_resistor * math.sqrt(2.) / vsense - 1. + .5)
    return max(0, min(31, cs))

def bits_to_current(bits, sense_resistor, vsense_on):
    sense_resistor += 0.020
    vsense = 0.32
    if vsense_on:
        vsense = 0.18
    current = (bits + 1) * vsense / (32 * sense_resistor * math.sqrt(2.))
    return round(current, 2)

def calc_current_config(run_current, hold_current, sense_resistor):
    vsense = False
    irun = current_bits(run_current, sense_resistor, vsense)
    ihold = current_bits(hold_current, sense_resistor, vsense)
    if irun < 16 and ihold < 16:
        vsense = True
        irun = current_bits(run_current, sense_resistor, vsense)
        ihold = current_bits(hold_current, sense_resistor, vsense)
    return vsense, irun, ihold

def get_config_current(config):
    run_current = config.getfloat('run_current', above=0., maxval=2.)
    hold_current = config.getfloat('hold_current', run_current,
                                   above=0., maxval=2.)
    sense_resistor = config.getfloat('sense_resistor', 0.110, above=0.)
    vsense, irun, ihold = calc_current_config(
                              run_current, hold_current, sense_resistor)
    return vsense, irun, ihold, sense_resistor

def get_config_microsteps(config):
    steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
             '8': 5, '4': 6, '2': 7, '1': 8}
    return config.getchoice('microsteps', steps)

def get_config_stealthchop(config, tmc_freq):
    mres = get_config_microsteps(config)
    velocity = config.getfloat('stealthchop_threshold', 0., minval=0.)
    if not velocity:
        return mres, False, 0
    stepper_name = " ".join(config.get_name().split()[1:])
    stepper_config = config.getsection(stepper_name)
    step_dist = stepper_config.getfloat('step_distance')
    step_dist_256 = step_dist / (1 << mres)
    threshold = int(tmc_freq * step_dist_256 / velocity + .5)
    return mres, True, max(0, min(0xfffff, threshold))


######################################################################
# TMC2130 printer object
######################################################################

class TMC2130:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.spi = bus.MCU_SPI_from_config(config, 3, default_speed=4000000)
        # Allow virtual endstop to be created
        self.diag1_pin = config.get('diag1_pin', None)
        ppins = self.printer.lookup_object("pins")
        ppins.register_chip("tmc2130_" + self.name, self)
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
        self.fields = FieldHelper(Fields, SignedFields, FieldFormatters,
                                  self.regs)
        vsense, irun, ihold, self.sense_resistor = get_config_current(config)
        self.fields.set_field("vsense", vsense)
        self.fields.set_field("IHOLD", ihold)
        self.fields.set_field("IRUN", irun)
        mres, en_pwm, thresh = get_config_stealthchop(config, TMC_FREQUENCY)
        self.fields.set_field("MRES", mres)
        self.fields.set_field("en_pwm_mode", en_pwm)
        self.fields.set_field("TPWMTHRS", thresh)
        # Allow other registers to be set from the config
        set_config_field = self.fields.set_config_field
        set_config_field(config, "toff", 4)
        set_config_field(config, "hstrt", 0)
        set_config_field(config, "hend", 7)
        set_config_field(config, "TBL", 1)
        set_config_field(config, "intpol", True, "interpolate")
        set_config_field(config, "IHOLDDELAY", 8)
        set_config_field(config, "TPOWERDOWN", 0)
        set_config_field(config, "PWM_AMPL", 128)
        set_config_field(config, "PWM_GRAD", 4)
        set_config_field(config, "pwm_freq", 1)
        set_config_field(config, "pwm_autoscale", True)
        set_config_field(config, "sgt", 0)
        self._init_registers()
    def _init_registers(self, min_clock = 0):
        # Send registers
        for reg_name, val in self.regs.items():
            self.set_register(reg_name, val, min_clock)
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'virtual_endstop':
            raise pins.error("tmc2130 virtual endstop only useful as endstop")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert tmc2130 virtual endstop")
        return TMC2130VirtualEndstop(self)
    def get_register(self, reg_name):
        reg = Registers[reg_name]
        self.spi.spi_send([reg, 0x00, 0x00, 0x00, 0x00])
        if self.printer.get_start_args().get('debugoutput') is not None:
            return 0
        params = self.spi.spi_transfer([reg, 0x00, 0x00, 0x00, 0x00])
        pr = bytearray(params['response'])
        return (pr[1] << 24) | (pr[2] << 16) | (pr[3] << 8) | pr[4]
    def set_register(self, reg_name, val, min_clock = 0):
        reg = Registers[reg_name]
        data = [(reg | 0x80) & 0xff, (val >> 24) & 0xff, (val >> 16) & 0xff,
                (val >> 8) & 0xff, val & 0xff]
        self.spi.spi_send(data, min_clock)
    def get_microsteps(self):
        return 256 >> self.fields.get_field("MRES")
    def get_phase(self):
        mscnt = self.fields.get_field("MSCNT", self.get_register("MSCNT"))
        return mscnt >> self.fields.get_field("MRES")
    cmd_SET_TMC_CURRENT_help = "Set the current of a TMC2130 driver"
    def cmd_SET_TMC_CURRENT(self, params):
        gcode = self.printer.lookup_object('gcode')
        vsense = bool(self.fields.get_field("vsense"))
        if 'HOLDCURRENT' in params:
            hold_current = gcode.get_float(
                'HOLDCURRENT', params, above=0., maxval=2.)
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
            vsense_calc, irun, ihold = calc_current_config(run_current,
                                            hold_current, self.sense_resistor)
            if (vsense_calc != vsense):
                self.fields.set_field("vsense", vsense_calc)
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
        logging.info("INIT_TMC 2130 %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        min_clock = self.spi.get_mcu().print_time_to_clock(print_time)
        self._init_registers(min_clock)
    cmd_SET_TMC_FIELD_help = "Set a register field of a TMC2130 driver"
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

# Endstop wrapper that enables tmc2130 "sensorless homing"
class TMC2130VirtualEndstop:
    def __init__(self, tmc2130):
        self.tmc2130 = tmc2130
        if tmc2130.diag1_pin is None:
            raise pins.error("tmc2130 virtual endstop requires diag1_pin")
        ppins = tmc2130.printer.lookup_object('pins')
        self.mcu_endstop = ppins.setup_pin('endstop', tmc2130.diag1_pin)
        if self.mcu_endstop.get_mcu() is not tmc2130.spi.get_mcu():
            raise pins.error("tmc2130 virtual endstop must be on same mcu")
        self.en_pwm = tmc2130.fields.get_field("en_pwm_mode")
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
        self.tmc2130.fields.set_field("en_pwm_mode", 0)
        self.tmc2130.fields.set_field("diag1_stall", 1)
        self.tmc2130.set_register("GCONF", self.tmc2130.regs['GCONF'])
        self.tmc2130.set_register("TCOOLTHRS", 0xfffff)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        self.tmc2130.fields.set_field("en_pwm_mode", self.en_pwm)
        self.tmc2130.fields.set_field("diag1_stall", 0)
        self.tmc2130.set_register("GCONF", self.tmc2130.regs['GCONF'])
        self.tmc2130.set_register("TCOOLTHRS", 0)
        self.mcu_endstop.home_finalize()

def load_config_prefix(config):
    return TMC2130(config)
