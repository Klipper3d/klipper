# TMC2660 configuration
#
# Copyright (C) 2018-2019  Florian Heilmann <Florian.Heilmann@gmx.net>
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import bus, tmc

Registers = {
    "DRVCONF": 0xE, "SGCSCONF": 0xC, "SMARTEN": 0xA,
    "CHOPCONF": 0x8, "DRVCTRL": 0x0
}

ReadRegisters = [ "READRSP@RDSEL0", "READRSP@RDSEL1", "READRSP@RDSEL2" ]

Fields = {}

Fields["DRVCTRL"] = {
    "MRES": 0x0f,
    "DEDGE": 0x01 << 8,
    "intpol": 0x01 << 9,
}

Fields["CHOPCONF"] = {
    "toff": 0x0f,
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
    "intpol": (lambda v: "1(On)" if v else "0(Off)"),
    "toff": (lambda v: ("%d" % v) if v else "0(Driver Disabled!)"),
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


######################################################################
# TMC stepper current config helper
######################################################################

MAX_CURRENT = 2.400

class TMC2660CurrentHelper:
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.current = config.getfloat('run_current', minval=0.1,
                                       maxval=MAX_CURRENT)
        self.sense_resistor = config.getfloat('sense_resistor')
        vsense, cs = self._calc_current(self.current)
        self.fields.set_field("CS", cs)
        self.fields.set_field("VSENSE", vsense)

        # Register ready/printing handlers
        self.idle_current_percentage = config.getint(
            'idle_current_percent', default=100, minval=0, maxval=100)
        if self.idle_current_percentage < 100:
            self.printer.register_event_handler("idle_timeout:printing",
                                                self.handle_printing)
            self.printer.register_event_handler("idle_timeout:ready",
                                                self.handle_ready)

        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command(
            "SET_TMC_CURRENT", "STEPPER", self.name,
            self.cmd_SET_TMC_CURRENT, desc=self.cmd_SET_TMC_CURRENT_help)

    def _calc_current_bits(self, current, vsense):
        vref = 0.165 if vsense else 0.310
        cs = int(32 * current * self.sense_resistor * math.sqrt(2.) / vref
                 - 1. + .5)
        return max(0, min(31, cs))

    def _calc_current(self, run_current):
        vsense = False
        cs = self._calc_current_bits(run_current, vsense)
        if cs < 16:
            vsense = True
            cs = self._calc_current_bits(run_current, vsense)
        return vsense, cs

    def handle_printing(self, print_time):
        print_time -= 0.100 # Schedule slightly before deadline
        self.printer.get_reactor().register_callback(
            (lambda ev: self.set_current(print_time, self.current)))

    def handle_ready(self, print_time):
        current = self.current * float(self.idle_current_percentage) / 100.
        self.printer.get_reactor().register_callback(
            (lambda ev: self.set_current(print_time, current)))

    def set_current(self, print_time, current):
        vsense, cs = self._calc_current(current)
        val = self.fields.set_field("CS", cs)
        self.mcu_tmc.set_register("SGCSCONF", val, print_time)
        # Only update VSENSE if we need to
        if vsense != self.fields.get_field("VSENSE"):
            val = self.fields.set_field("VSENSE", vsense)
            self.mcu_tmc.set_register("DRVCONF", val, print_time)

    cmd_SET_TMC_CURRENT_help = "Set the current of a TMC2660 driver"
    def cmd_SET_TMC_CURRENT(self, params):
        gcode = self.printer.lookup_object('gcode')
        if 'CURRENT' in params:
            self.current = gcode.get_float(
                'CURRENT', params, minval=0.1, maxval=MAX_CURRENT)
            self.set_current(
                self.printer.lookup_object('toolhead').get_last_move_time(),
                self.current)


######################################################################
# TMC2660 SPI
######################################################################

# Helper code for working with TMC2660 devices via SPI
class MCU_TMC2660_SPI:
    def __init__(self, config, name_to_reg, fields):
        self.printer = config.get_printer()
        self.mutex = self.printer.get_reactor().mutex()
        self.spi = bus.MCU_SPI_from_config(config, 0, default_speed=4000000)
        self.name_to_reg = name_to_reg
        self.fields = fields
    def get_fields(self):
        return self.fields
    def get_register(self, reg_name):
        reg = self.name_to_reg["DRVCONF"]
        val = self.fields.set_field("RDSEL", ReadRegisters.index(reg_name))
        if self.printer.get_start_args().get('debugoutput') is not None:
            return 0
        msg = [((val >> 16) | reg) & 0xff, (val >> 8) & 0xff, val & 0xff]
        with self.mutex:
            params = self.spi.spi_transfer(msg)
        pr = bytearray(params['response'])
        return (pr[0] << 16) | (pr[1] << 8) | pr[2]
    def set_register(self, reg_name, val, print_time=None):
        minclock = 0
        if print_time is not None:
            minclock = self.spi.get_mcu().print_time_to_clock(print_time)
        reg = self.name_to_reg[reg_name]
        msg = [((val >> 16) | reg) & 0xff, (val >> 8) & 0xff, val & 0xff]
        with self.mutex:
            self.spi.spi_send(msg, minclock)


######################################################################
# TMC2660 printer object
######################################################################

class TMC2660:
    def __init__(self, config):
        # Setup mcu communication
        self.fields = tmc.FieldHelper(Fields, SignedFields, FieldFormatters)
        self.fields.set_field("SDOFF", 0) # Access DRVCTRL in step/dir mode
        self.mcu_tmc = MCU_TMC2660_SPI(config, Registers, self.fields)
        # Allow virtual pins to be created
        tmc.TMCVirtualPinHelper(config, self.mcu_tmc)
        # Register commands
        cmdhelper = tmc.TMCCommandHelper(config, self.mcu_tmc)
        cmdhelper.setup_register_dump(ReadRegisters)

        # DRVCTRL
        mh = tmc.TMCMicrostepHelper(config, self.mcu_tmc)
        self.get_microsteps = mh.get_microsteps
        self.get_phase = mh.get_phase
        # CHOPCONF
        set_config_field = self.fields.set_config_field
        set_config_field(config, "TBL", 2)
        set_config_field(config, "RNDTF", 0)
        set_config_field(config, "HDEC", 0)
        set_config_field(config, "CHM", 0)
        set_config_field(config, "HEND", 3)
        set_config_field(config, "HSTRT", 3)
        set_config_field(config, "toff", 4)
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
        TMC2660CurrentHelper(config, self.mcu_tmc)

        # DRVCONF
        set_config_field(config, "SLPH", 0)
        set_config_field(config, "SLPL", 0)
        set_config_field(config, "DISS2G", 0)
        set_config_field(config, "TS2G", 3)

def load_config_prefix(config):
    return TMC2660(config)
