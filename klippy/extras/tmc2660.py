# TMC2660 configuration
#
# Copyright (C) 2018-2019  Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, collections, logging
import bus, tmc2130

def current_bits(current, sense_resistor, vsense_on):
    vsense = 0.165 if vsense_on else 0.310
    cs = int(32 * current * sense_resistor * math.sqrt(2.) / vsense - 1. + .5)
    return max(0, min(cs, 31))

def get_config_current(run_current, sense_resistor):
    vsense = False
    cs = current_bits(run_current, sense_resistor, vsense)
    if cs < 16:
        vsense = True
        cs = current_bits(run_current, sense_resistor, vsense)
    return cs, vsense

Registers = {
    "DRVCONF": 0xE, "SGCSCONF": 0xC, "SMARTEN": 0xA,
    "CHOPCONF": 0x8, "DRVCTRL": 0x0
}

Fields = {}

Fields["DRVCTRL"] = {
    "MRES": 0x0f,
    "DEDGE": 0x01 << 8,
    "INTPOL": 0x01 << 9,
}

Fields["CHOPCONF"] = {
    "TOFF": 0x0f,
    "HSTRT": 0x7 << 4,
    "HEND": 0x0f << 7,
    "HDEC": 0x03 << 11,
    "RNDTF": 0x01 << 13,
    "CHM": 0x01 << 14,
    "TBL": 0x03 << 15
}

Fields["SMARTEN"] = {
    "SEMIN" : 0x0f,
    "SEUP": 0x03 << 5,
    "SEMAX": 0x0f << 8,
    "SEDN": 0x03 << 13,
    "SEIMIN": 0x01 << 15
}

Fields["SGCSCONF"] = {
    "CS": 0x1f,
    "SGT": 0x7F << 8,
    "SFILT": 0x01 << 16
}

Fields["DRVCONF"] = {
    "RDSEL": 0x03 << 4,
    "VSENSE": 0x01 << 6,
    "SDOFF": 0x01 << 7,
    "TS2G": 0x03 << 8,
    "DISS2G": 0x01 << 10,
    "SLPL": 0x03 << 12,
    "SLPH": 0x03 << 14,
    "TST": 0x01 << 16
}

Fields["READRSP@RDSEL0"] = {
    "SG": 0x01,
    "OT": 0x01 << 1,
    "OTPW": 0x01 << 2,
    "S2GA": 0x01 << 3,
    "S2GB": 0x01 << 4,
    "OLA": 0x01 << 5,
    "OLB": 0x01 << 6,
    "STST": 0x01 << 7,
    "MSTEP": 0x3ff << 10
}

Fields["READRSP@RDSEL1"] = {
    "SG": 0x01,
    "OT": 0x01 << 1,
    "OTPW": 0x01 << 2,
    "S2GA": 0x01 << 3,
    "S2GB": 0x01 << 4,
    "OLA": 0x01 << 5,
    "OLB": 0x01 << 6,
    "STST": 0x01 << 7,
    "SG@RDSEL1": 0x3ff << 10
}

Fields["READRSP@RDSEL2"] = {
    "SG": 0x01,
    "OT": 0x01 << 1,
    "OTPW": 0x01 << 2,
    "S2GA": 0x01 << 3,
    "S2GB": 0x01 << 4,
    "OLA": 0x01 << 5,
    "OLB": 0x01 << 6,
    "STST": 0x01 << 7,
    "SG@RDSEL2": 0x1f << 15,
    "SE": 0x1f << 10
}

SignedFields = ["SGT"]

FieldFormatters = {
    "MRES": (lambda v: "%d(%dusteps)" % (v, 0x100 >> v)),
    "DEDGE": (lambda v:
        "1(Both Edges Active)" if v else "0(Only Rising Edge active)"),
    "INTPOL": (lambda v: "1(On)" if v else "0(Off)"),
    "TOFF": (lambda v: ("%d" % v) if v else "0(Driver Disabled!)"),
    "CHM": (lambda v: "1(constant toff)" if v else "0(spreadCycle)"),
    "SFILT": (lambda v: "1(Filtered mode)" if v else "0(Standard mode)"),
    "VSENSE": (lambda v: "%d(%dmV)" % (v, 165 if v else 305)),
    "SDOFF": (lambda v: "1(Step/Dir disabled" if v else "0(Step/dir enabled)"),
    "DISS2G": (lambda v: "%d(Short to GND protection %s)" % (v,
                                          "disabled" if v else "enabled")),
    "MSTEP": (lambda v: "%d(%d, OA1 %s OA2)" % (v, v & 0xff,
                                                "<=" if v & 0x100 else "=>")),
    "SG": (lambda v: "%d(%s)" % (v, "Stall!" if v else "No Stall!")),
    "OT": (lambda v: "1(Overtemp shutdown!)" if v else ""),
    "OTPW": (lambda v: "1(Overtemp warning!)" if v else ""),
    "S2GA": (lambda v: "1(Short to GND Coil A!)" if v else ""),
    "S2GB": (lambda v: "1(Short to GND Coil B!)" if v else ""),
    "OLA": (lambda v: "1(Open Load Coil A at slow speed!)" if v else ""),
    "OLB": (lambda v: "1(Open Load Coil B at slow speed!)" if v else ""),
    "STST": (lambda v: "1(Standstill detected!)" if v else ""),
}

class TMC2660:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        self.spi = bus.MCU_SPI_from_config(config, 0, default_speed=4000000)
        # Add SET_CURRENT and DUMP_TMC commands
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
        # Setup driver registers
        self.regs = collections.OrderedDict()
        self.fields = tmc2130.FieldHelper(Fields, SignedFields, FieldFormatters,
                                          self.regs)
        set_config_field = self.fields.set_config_field

        # DRVCTRL
        steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
                 '8': 5, '4': 6, '2': 7, '1': 8}
        self.driver_mres = config.getchoice('microsteps', steps)
        self.fields.set_field("MRES", self.driver_mres)
        set_config_field(config, "DEDGE", 0)
        set_config_field(config, "INTPOL", True, 'interpolate')
        # CHOPCONF
        set_config_field(config, "TBL", 2)
        set_config_field(config, "RNDTF", 0)
        set_config_field(config, "HDEC", 0)
        set_config_field(config, "CHM", 0)
        set_config_field(config, "HEND", 3)
        set_config_field(config, "HSTRT", 3)
        set_config_field(config, "TOFF", 4)
        if not self.fields.get_field("CHM"):
            if (self.fields.get_field("HSTRT") +
                self.fields.get_field("HEND")) > 15:
                raise config.error("driver_HEND + driver_HSTRT must be <= 15")
        # SMARTEN
        set_config_field(config, "SEIMIN", 0)
        set_config_field(config, "SEDN", 0)
        set_config_field(config, "SEMAX", 0)
        set_config_field(config, "SEUP", 0)
        set_config_field(config, "SEMIN", 0)

        # SGSCONF
        set_config_field(config, "SFILT", 1)
        set_config_field(config, "SGT", 0)
        self.current = config.getfloat('run_current', minval=0.1,
                                       maxval=2.4)
        self.sense_resistor = config.getfloat('sense_resistor')
        (self.driver_cs,
         self.driver_vsense) = get_config_current(self.current,
                                                  self.sense_resistor)
        self.fields.set_field("CS", self.driver_cs)

        # DRVCONF
        set_config_field(config, "SLPH", 0)
        set_config_field(config, "SLPL", 0)
        set_config_field(config, "DISS2G", 0)
        set_config_field(config, "TS2G", 3)
        self.fields.set_field("VSENSE", self.driver_vsense)
        self.fields.set_field("RDSEL", 0) # needed for phase calculations
        self.fields.set_field("SDOFF", 0) # only step/dir mode supported

        # Init Registers
        self._init_registers(self)

        # Register ready/printing handlers
        self.idle_current_percentage = config.getint(
            'idle_current_percent', default=100, minval=0, maxval=100)
        if self.idle_current_percentage < 100:
            self.printer.register_event_handler("idle_timeout:printing",
                                                self.handle_printing)
            self.printer.register_event_handler("idle_timeout:ready",
                                                self.handle_ready)

    def _init_registers(self, min_clock=0):
        for reg_name in Registers:
            self.set_register(reg_name, self.regs[reg_name])

    def set_register(self, reg_name, val, min_clock=0):
        reg = Registers[reg_name]
        self.spi.spi_send([((val >> 16) | reg) & 0xff,
                            (val >> 8) & 0xff, val & 0xff], min_clock)

    def get_response(self):
        reg = Registers["DRVCTRL"]
        val = self.regs["DRVCTRL"]
        if self.printer.get_start_args().get('debugoutput') is not None:
            return 0
        params = self.spi.spi_transfer([((val >> 16) | reg) & 0xff,
                            (val >> 8) & 0xff, val & 0xff])
        pr = bytearray(params['response'])
        return (pr[0] << 16) | (pr[1] << 8) | pr[2]

    def get_microsteps(self):
        return 256 >> self.fields.get_field("MRES")

    def get_phase(self):
        mscnt =  self.fields.get_field("MSTEP", self.get_response())
        return mscnt >> self.driver_mres

    def handle_printing(self, print_time):
        self.set_current(0., self.current) # workaround

    def handle_ready(self, print_time):
        self.set_current(print_time, (float(self.idle_current_percentage)
                                      * self.current / 100))

    def set_current(self, print_time, current):
        (self.driver_cs,
         self.driver_vsense) = get_config_current(current, self.sense_resistor)
        self.fields.set_field("CS", self.driver_cs)
        clock = self.spi.get_mcu().print_time_to_clock(print_time)
        self.set_register("SGCSCONF", self.regs["SGCSCONF"], min_clock=clock)
        # Only update VSENSE if we need to
        if self.driver_vsense != self.fields.get_field("VSENSE"):
            self.fields.set_field("VSENSE", self.driver_vsense)
            self.set_register("DRVCONF", self.regs["DRVCONF"], min_clock=clock)

    cmd_SET_TMC_CURRENT_help = "Set the current of a TMC2660 driver"
    def cmd_SET_TMC_CURRENT(self, params):
        gcode = self.printer.lookup_object('gcode')
        if 'CURRENT' in params:
            self.current = gcode.get_float(
                'CURRENT', params, minval=0.1, maxval=2.4)
            self.set_current(
                self.printer.lookup_object('toolhead').get_last_move_time(),
                self.current)

    cmd_DUMP_TMC_help = "Read and display TMC stepper driver registers"
    def cmd_DUMP_TMC(self, params):
        self.printer.lookup_object('toolhead').get_last_move_time()
        gcode = self.printer.lookup_object('gcode')
        gcode.respond_info("========== Write-only registers ==========")
        for reg_name in Registers:
            msg = self.fields.pretty_format(reg_name, self.regs[reg_name])
            gcode.respond_info(msg)

        # Send one register to get the return data
        gcode.respond_info("========== Queried registers ==========")
        return_format = "READRSP@RDSEL" + str(self.fields.get_field("RDSEL"))
        msg = self.fields.pretty_format(return_format, self.get_response())
        gcode.respond_info(msg)

    cmd_INIT_TMC_help = "Initialize TMC stepper driver registers"
    def cmd_INIT_TMC(self, params):
        logging.info("INIT_TMC 2660 %s", self.name)
        pt = self.printer.lookup_object('toolhead').get_last_move_time()
        min_clock = self.spi.get_mcu().print_time_to_clock(pt)
        self._init_registers(min_clock)

    cmd_SET_TMC_FIELD_help = "Set a register field of a TMC2660 driver"
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
        pt = self.printer.lookup_object('toolhead').get_last_move_time()
        clock = self.spi.get_mcu().print_time_to_clock(pt)
        self.set_register(reg, self.regs[reg], min_clock=clock)

def load_config_prefix(config):
    return TMC2660(config)
