# Common helper code for TMC stepper drivers
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, collections


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
            self.registers = collections.OrderedDict()
        self.field_to_register = { f: r for r, fields in self.all_fields.items()
                                   for f in fields }
    def lookup_register(self, field_name, default=None):
        return self.field_to_register.get(field_name, default)
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
    def set_config_field(self, config, field_name, default):
        # Allow a field to be set from the config file
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
# G-Code command helpers
######################################################################

class TMCCommandHelper:
    def __init__(self, config, mcu_tmc):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.read_registers = self.read_translate = None
        self.gcode = self.printer.lookup_object("gcode")
        self.gcode.register_mux_command(
            "SET_TMC_FIELD", "STEPPER", self.name,
            self.cmd_SET_TMC_FIELD, desc=self.cmd_SET_TMC_FIELD_help)
        self.gcode.register_mux_command(
            "INIT_TMC", "STEPPER", self.name,
            self.cmd_INIT_TMC, desc=self.cmd_INIT_TMC_help)
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
    def _init_registers(self, print_time=None):
        # Send registers
        for reg_name, val in self.fields.registers.items():
            self.mcu_tmc.set_register(reg_name, val, print_time)
    def _handle_connect(self):
        retry_count = 0
        while 1:
            try:
                self._init_registers()
                return
            except self.printer.command_error as e:
                logging.exception("TMC init error")
                retry_count += 1
                if retry_count > 5:
                    raise self.printer.config_error(str(e))
                reactor = self.printer.get_reactor()
                reactor.pause(reactor.monotonic() + 1.)
    cmd_INIT_TMC_help = "Initialize TMC stepper driver registers"
    def cmd_INIT_TMC(self, params):
        logging.info("INIT_TMC %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self._init_registers(print_time)
    cmd_SET_TMC_FIELD_help = "Set a register field of a TMC driver"
    def cmd_SET_TMC_FIELD(self, params):
        if 'FIELD' not in params or 'VALUE' not in params:
            raise self.gcode.error("Invalid command format")
        field_name = self.gcode.get_str('FIELD', params)
        reg_name = self.fields.lookup_register(field_name, None)
        if reg_name is None:
            raise self.gcode.error("Unknown field name '%s'" % (field_name,))
        value = self.gcode.get_int('VALUE', params)
        reg_val = self.fields.set_field(field_name, value)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.mcu_tmc.set_register(reg_name, reg_val, print_time)
    # DUMP_TMC support
    def setup_register_dump(self, read_registers, read_translate=None):
        self.read_registers = read_registers
        self.read_translate = read_translate
        self.gcode.register_mux_command(
            "DUMP_TMC", "STEPPER", self.name,
            self.cmd_DUMP_TMC, desc=self.cmd_DUMP_TMC_help)
    cmd_DUMP_TMC_help = "Read and display TMC stepper driver registers"
    def cmd_DUMP_TMC(self, params):
        logging.info("DUMP_TMC %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.gcode.respond_info("========== Write-only registers ==========")
        for reg_name, val in self.fields.registers.items():
            if reg_name not in self.read_registers:
                self.gcode.respond_info(
                    self.fields.pretty_format(reg_name, val))
        self.gcode.respond_info("========== Queried registers ==========")
        for reg_name in self.read_registers:
            val = self.mcu_tmc.get_register(reg_name)
            if self.read_translate is not None:
                reg_name, val = self.read_translate(reg_name, val)
            self.gcode.respond_info(self.fields.pretty_format(reg_name, val))


######################################################################
# TMC virtual pins
######################################################################

# Endstop wrapper that enables "sensorless homing"
class TMCVirtualEndstop:
    def __init__(self, mcu_tmc, mcu_endstop):
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.mcu_endstop = mcu_endstop
        self.en_pwm = self.fields.get_field("en_pwm_mode")
        # Wrappers
        self.get_mcu = self.mcu_endstop.get_mcu
        self.add_stepper = self.mcu_endstop.add_stepper
        self.get_steppers = self.mcu_endstop.get_steppers
        self.home_start = self.mcu_endstop.home_start
        self.home_wait = self.mcu_endstop.home_wait
        self.query_endstop = self.mcu_endstop.query_endstop
        self.TimeoutError = self.mcu_endstop.TimeoutError
    def home_prepare(self):
        self.fields.set_field("en_pwm_mode", 0)
        val = self.fields.set_field("diag1_stall", 1)
        self.mcu_tmc.set_register("GCONF", val)
        self.mcu_tmc.set_register("TCOOLTHRS", 0xfffff)
        self.mcu_endstop.home_prepare()
    def home_finalize(self):
        self.fields.set_field("en_pwm_mode", self.en_pwm)
        val = self.fields.set_field("diag1_stall", 0)
        self.mcu_tmc.set_register("GCONF", val)
        self.mcu_tmc.set_register("TCOOLTHRS", 0)
        self.mcu_endstop.home_finalize()

# Digital output wrapper for virtual enable
class TMCVirtualEnable:
    def __init__(self, printer, mcu_tmc):
        self.reactor = printer.get_reactor()
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        self.toff = self.fields.get_field("toff")
        self.fields.set_field("toff", 0)
    def setup_max_duration(self, max_duration):
        pass
    def _do_set_digital(self, print_time, value):
        toff_val = 0
        if value:
            toff_val = self.toff
            print_time -= 0.100 # Schedule slightly before deadline
        val = self.fields.set_field("toff", toff_val)
        reg_name = self.fields.lookup_register("toff")
        self.mcu_tmc.set_register(reg_name, val, print_time)
    def set_digital(self, print_time, value):
        self.reactor.register_callback(
            (lambda ev: self._do_set_digital(print_time, value)))

class TMCVirtualPinHelper:
    def __init__(self, config, mcu_tmc, diag_pin=None):
        self.printer = config.get_printer()
        self.mcu_tmc = mcu_tmc
        self.diag_pin = diag_pin
        name_parts = config.get_name().split()
        ppins = self.printer.lookup_object("pins")
        ppins.register_chip("%s_%s" % (name_parts[0], name_parts[-1]), self)
    def setup_pin(self, pin_type, pin_params):
        ppins = self.printer.lookup_object('pins')
        if pin_params['pin'] not in ('virtual_endstop', 'virtual_enable'):
            raise ppins.error("Unknown tmc virtual pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise ppins.error("Can not pullup/invert tmc virtual pin")
        if pin_params['pin'] == 'virtual_enable':
            if pin_type != 'digital_out':
                raise ppins.error("tmc virtual enable only useful for enable")
            return TMCVirtualEnable(self.printer, self.mcu_tmc)
        if self.diag_pin is None:
            raise ppins.error("tmc virtual endstop requires diag pin config")
        if pin_type != 'endstop':
            raise ppins.error("tmc virtual endstop only useful as endstop")
        mcu_endstop = ppins.setup_pin('endstop', self.diag_pin)
        return TMCVirtualEndstop(self.mcu_tmc, mcu_endstop)


######################################################################
# Config reading helpers
######################################################################

# Helper to configure and query the microstep settings
class TMCMicrostepHelper:
    def __init__(self, config, mcu_tmc):
        self.mcu_tmc = mcu_tmc
        self.fields = mcu_tmc.get_fields()
        steps = {'256': 0, '128': 1, '64': 2, '32': 3, '16': 4,
                 '8': 5, '4': 6, '2': 7, '1': 8}
        mres = config.getchoice('microsteps', steps)
        self.fields.set_field("MRES", mres)
        self.fields.set_field("intpol", config.getboolean("interpolate", True))
    def get_microsteps(self):
        return 256 >> self.fields.get_field("MRES")
    def get_phase(self):
        field_name = "MSCNT"
        if self.fields.lookup_register(field_name, None) is None:
            # TMC2660 uses MSTEP
            field_name = "MSTEP"
        reg = self.mcu_tmc.get_register(self.fields.lookup_register(field_name))
        mscnt = self.fields.get_field(field_name, reg)
        return (1023 - mscnt) >> self.fields.get_field("MRES")

# Helper to configure "stealthchop" mode
def TMCStealthchopHelper(config, mcu_tmc, tmc_freq):
    fields = mcu_tmc.get_fields()
    en_pwm_mode = False
    velocity = config.getfloat('stealthchop_threshold', 0., minval=0.)
    if velocity:
        stepper_name = " ".join(config.get_name().split()[1:])
        stepper_config = config.getsection(stepper_name)
        step_dist = stepper_config.getfloat('step_distance')
        step_dist_256 = step_dist / (1 << fields.get_field("MRES"))
        threshold = int(tmc_freq * step_dist_256 / velocity + .5)
        fields.set_field("TPWMTHRS", max(0, min(0xfffff, threshold)))
        en_pwm_mode = True
    reg = fields.lookup_register("en_pwm_mode", None)
    if reg is not None:
        fields.set_field("en_pwm_mode", en_pwm_mode)
    else:
        # TMC2208 uses en_spreadCycle
        fields.set_field("en_spreadCycle", not en_pwm_mode)
