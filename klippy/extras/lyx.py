# Common helper code for LYX stepper drivers
#
# Author: zylo117
# Based on tmc.py architecture
# License: GNU GPLv3

import logging, collections
import stepper

######################################################################
# Field helpers (adapted for 16-bit registers)
######################################################################

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

        print("DEBUG field_to_register keys:", list(self.field_to_register.keys()))

    def lookup_register(self, field_name, default=None):
        return self.field_to_register.get(field_name, default)

    def get_field(self, field_name, reg_value=None, reg_name=None):
        if reg_name is None:
            reg_name = self.field_to_register[field_name]
        if reg_value is None:
            reg_value = self.registers.get(reg_name, 0)
        mask = self.all_fields[reg_name][field_name]
        field_value = (reg_value & mask) >> ffs(mask)
        if field_name in self.signed_fields:
            # Sign extend 16-bit signed values
            if field_value & (1 << (15 - ffs(mask))):
                field_value -= (1 << (16 - ffs(mask)))
        return field_value

    def set_field(self, field_name, field_value, reg_value=None, reg_name=None):
        if reg_name is None:
            reg_name = self.field_to_register[field_name]
        if reg_value is None:
            reg_value = self.registers.get(reg_name, 0)
        mask = self.all_fields[reg_name][field_name]
        # Clamp to 16-bit
        field_value = int(field_value) & 0xFFFF
        new_value = (reg_value & ~mask) | ((field_value << ffs(mask)) & mask)
        self.registers[reg_name] = new_value & 0xFFFF
        return new_value & 0xFFFF

    def set_config_field(self, config, field_name, default):
        # 统一小写，和 Klipper 配置系统内部归一化规则保持一致
        config_name = "driver_" + field_name
        reg_name = self.field_to_register[field_name]
        mask = self.all_fields[reg_name][field_name]
        maxval = mask >> ffs(mask)

        if maxval == 1:
            val = config.getboolean(config_name, default)
        elif field_name in self.signed_fields:
            val = config.getint(config_name, default,
                                minval=-(maxval // 2 + 1), maxval=maxval // 2)
        else:
            val = config.getint(config_name, default, minval=0, maxval=maxval)
        return self.set_field(field_name, val)

    def pretty_format(self, reg_name, reg_value):
        reg_fields = self.all_fields.get(reg_name, {})
        reg_fields = sorted([(mask, name) for name, mask in reg_fields.items()])
        fields = []
        for mask, field_name in reg_fields:
            field_value = self.get_field(field_name, reg_value, reg_name)
            sval = self.field_formatters.get(field_name, str)(field_value)
            if sval and sval != "0":
                fields.append(" %s=%s" % (field_name, sval))
        return "%-11s %04x%s" % (reg_name + ":", reg_value, "".join(fields))

######################################################################
# Current helper
######################################################################

class LYXCurrentHelper:
    def __init__(self, config, mcu_lyx):
        self.printer = config.get_printer()
        self.mcu_lyx = mcu_lyx
        self.fields = mcu_lyx.get_fields()

        # 显式读取驱动级配置（必须显式读，否则Klipper报未使用错误）
        self.sense_resistor = config.getfloat('sense_resistor', 0.050, above=0.)
        run_current = config.getfloat('run_current', 1.4, above=0.)
        hold_current = config.getfloat('hold_current', None, above=0.)
        if hold_current is None:
            hold_current = run_current * 0.5

        # 电流换算公式（规格书6.2节）：I = reg / 2048 * 0.025Ω / R_sense * 6.4A
        self._current_scale = (0.025 / self.sense_resistor) * 6.4 / 2048.0
        self.max_current = 1900 * self._current_scale

        # 将配置的电流值写入寄存器字段
        self.set_current(run_current, hold_current, None)

    def get_current(self):
        run_reg = self.fields.get_field("run_current")
        run_current = run_reg * self._current_scale
        half_ratio = self.fields.get_field("half_cur_ratio") / 128.0
        hold_current = run_current * half_ratio
        return run_current, hold_current, hold_current, self.max_current

    def set_current(self, run_current, hold_current, print_time):
        run_reg = int(run_current / self._current_scale + 0.5)
        run_reg = max(50, min(1900, run_reg))
        self.fields.set_field("run_current", run_reg)

        half_ratio = int(hold_current / run_current * 128.0 + 0.5)
        half_ratio = max(0, min(128, half_ratio))
        self.fields.set_field("half_cur_ratio", half_ratio)

        if print_time is not None:
            reg_name = self.fields.lookup_register("run_current")
            self.mcu_lyx.set_register(reg_name,
                                      self.fields.registers[reg_name],
                                      print_time)

######################################################################
# G-Code command helpers
######################################################################

class LYXCommandHelper:
    def __init__(self, config, mcu_lyx, current_helper):
        self.printer = config.get_printer()
        self.stepper_name = ' '.join(config.get_name().split()[1:])
        self.name = config.get_name().split()[-1]
        self.mcu_lyx = mcu_lyx
        self.current_helper = current_helper
        self.fields = mcu_lyx.get_fields()
        self.stepper = None
        self.mcu_phase_offset = None

        # Register commands
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("SET_LYX_FIELD", "STEPPER", self.name,
                                   self.cmd_SET_LYX_FIELD,
                                   desc=self.cmd_SET_LYX_FIELD_help)
        gcode.register_mux_command("INIT_LYX", "STEPPER", self.name,
                                   self.cmd_INIT_LYX,
                                   desc=self.cmd_INIT_LYX_help)
        gcode.register_mux_command("SET_LYX_CURRENT", "STEPPER", self.name,
                                   self.cmd_SET_LYX_CURRENT,
                                   desc=self.cmd_SET_LYX_CURRENT_help)

    def _init_registers(self, print_time=None):
        for reg_name in list(self.fields.registers.keys()):
            val = self.fields.registers[reg_name]
            self.mcu_lyx.set_register(reg_name, val, print_time)

    cmd_INIT_LYX_help = "Initialize LYX stepper driver registers"
    def cmd_INIT_LYX(self, gcmd):
        logging.info("INIT_LYX %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self._init_registers(print_time)

    cmd_SET_LYX_FIELD_help = "Set a register field of a LYX driver"
    def cmd_SET_LYX_FIELD(self, gcmd):
        field_name = gcmd.get('FIELD').lower()
        reg_name = self.fields.lookup_register(field_name, None)
        if reg_name is None:
            raise gcmd.error("Unknown field name '%s'" % (field_name,))
        value = gcmd.get_int('VALUE')
        reg_val = self.fields.set_field(field_name, value)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.mcu_lyx.set_register(reg_name, reg_val, print_time)

    cmd_SET_LYX_CURRENT_help = "Set the current of a LYX driver"
    def cmd_SET_LYX_CURRENT(self, gcmd):
        ch = self.current_helper
        prev_cur, prev_hold_cur, req_hold_cur, max_cur = ch.get_current()
        run_current = gcmd.get_float('CURRENT', None, minval=0., maxval=max_cur)
        hold_current = gcmd.get_float('HOLDCURRENT', None,
                                      above=0., maxval=max_cur)
        if run_current is not None or hold_current is not None:
            if run_current is None:
                run_current = prev_cur
            if hold_current is None:
                hold_current = req_hold_cur
            toolhead = self.printer.lookup_object('toolhead')
            print_time = toolhead.get_last_move_time()
            ch.set_current(run_current, hold_current, print_time)
            prev_cur, prev_hold_cur, req_hold_cur, max_cur = ch.get_current()
        gcmd.respond_info("Run Current: %0.2fA Hold Current: %0.2fA"
                          % (prev_cur, prev_hold_cur))

    def get_phase_offset(self):
        # LYX9231 sensorless: phase offset not applicable
        return None, 0

    def get_status(self, eventtime=None):
        current = self.current_helper.get_current()
        return {
            'run_current': current[0],
            'hold_current': current[1],
            'alarm_code': None,  # could be read periodically
        }

    def setup_register_dump(self, read_registers):
        self.read_registers = read_registers
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("DUMP_LYX", "STEPPER", self.name,
                                   self.cmd_DUMP_LYX,
                                   desc=self.cmd_DUMP_LYX_help)

    cmd_DUMP_LYX_help = "Read and display LYX stepper driver registers"
    def cmd_DUMP_LYX(self, gcmd):
        logging.info("DUMP_LYX %s", self.name)
        reg_name = gcmd.get('REGISTER', None)
        if reg_name is not None:
            reg_name = reg_name.upper()
            val = self.fields.registers.get(reg_name)
            if val is not None and reg_name not in self.read_registers:
                gcmd.respond_info(self.fields.pretty_format(reg_name, val))
            elif reg_name in self.read_registers:
                val = self.mcu_lyx.get_register(reg_name)
                gcmd.respond_info(self.fields.pretty_format(reg_name, val))
            else:
                raise gcmd.error("Unknown register name '%s'" % (reg_name))
        else:
            gcmd.respond_info("========== Write-only registers ==========")
            for reg_name, val in self.fields.registers.items():
                if reg_name not in self.read_registers:
                    gcmd.respond_info(self.fields.pretty_format(reg_name, val))
            gcmd.respond_info("========== Queried registers ==========")
            for reg_name in self.read_registers:
                val = self.mcu_lyx.get_register(reg_name)
                gcmd.respond_info(self.fields.pretty_format(reg_name, val))
