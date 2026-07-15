# Common helper code for LYX stepper drivers
#
# Author: zylo117
# License: GNU GPLv3
import collections

######################################################################
# Field helper
######################################################################
def ffs(mask):
    return (mask & -mask).bit_length() - 1

class FieldHelper:
    def __init__(self, all_fields, signed_fields=[], field_formatters={}):
        self.all_fields = all_fields
        self.signed_fields = set(signed_fields)
        self.field_formatters = field_formatters
        self.registers = collections.OrderedDict()
        self.field_to_register = {
            f: r for r, fields in self.all_fields.items() for f in fields
        }

    def get_field(self, field_name):
        reg_name = self.field_to_register[field_name]
        reg_val = self.registers.get(reg_name, 0)
        mask = self.all_fields[reg_name][field_name]
        val = (reg_val & mask) >> ffs(mask)
        if field_name in self.signed_fields and val & (1 << 15):
            val -= (1 << 16)
        return val

    def set_field(self, field_name, value):
        reg_name = self.field_to_register[field_name]
        reg_val = self.registers.get(reg_name, 0)
        mask = self.all_fields[reg_name][field_name]
        value = int(value) & 0xFFFF
        new_val = (reg_val & ~mask) | ((value << ffs(mask)) & mask)
        self.registers[reg_name] = new_val & 0xFFFF
        return new_val & 0xFFFF

    def set_config_field(self, config, field_name, default):
        config_name = "driver_" + field_name
        reg_name = self.field_to_register[field_name]
        mask = self.all_fields[reg_name][field_name]
        maxval = mask >> ffs(mask)

        if maxval == 1:
            val = config.getboolean(config_name, default)
        elif field_name in self.signed_fields:
            val = config.getint(config_name, default,
                                minval=-32768, maxval=32767)
        else:
            val = config.getint(config_name, default, minval=0, maxval=maxval)
        return self.set_field(field_name, val)

    def pretty_format(self, reg_name, reg_value):
        fields = []
        for field_name, mask in self.all_fields.get(reg_name, {}).items():
            val = self.get_field(field_name)
            sval = self.field_formatters.get(field_name, str)(val)
            if sval and sval != "0":
                fields.append(f" {field_name}={sval}")
        return "%-12s %04x%s" % (reg_name + ":", reg_value, "".join(fields))

######################################################################
# Current helper
######################################################################
class LYXCurrentHelper:
    def __init__(self, config, mcu_lyx):
        self.printer = config.get_printer()
        self.fields = mcu_lyx.get_fields()

        self.sense_resistor = config.getfloat('sense_resistor', 0.050, above=0.)
        run_current = config.getfloat('run_current', 1.4, above=0.)
        hold_current = config.getfloat('hold_current', None, above=0.)
        if hold_current is None:
            hold_current = run_current * 0.5

        self._current_scale = (0.025 / self.sense_resistor) * 6.4 / 2048.0
        self.max_current = 1900 * self._current_scale

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

######################################################################
# G-Code command helper
######################################################################
class LYXCommandHelper:
    def __init__(self, config, mcu_lyx, current_helper):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[-1]
        self.mcu_lyx = mcu_lyx
        self.current_helper = current_helper
        self.fields = mcu_lyx.get_fields()
        self.read_registers = []

        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("SET_LYX_FIELD", "STEPPER", self.name,
                                   self.cmd_SET_LYX_FIELD)
        gcode.register_mux_command("SET_LYX_CURRENT", "STEPPER", self.name,
                                   self.cmd_SET_LYX_CURRENT)

    def cmd_SET_LYX_FIELD(self, gcmd):
        field_name = gcmd.get('FIELD').lower()
        reg_name = self.fields.field_to_register.get(field_name)
        if reg_name is None:
            raise gcmd.error(f"Unknown field: {field_name}")
        value = gcmd.get_int('VALUE')
        self.fields.set_field(field_name, value)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.mcu_lyx.set_register(reg_name, value, print_time)

    def cmd_SET_LYX_CURRENT(self, gcmd):
        ch = self.current_helper
        prev_run, prev_hold, _, max_cur = ch.get_current()
        run = gcmd.get_float('CURRENT', None, minval=0., maxval=max_cur)
        hold = gcmd.get_float('HOLDCURRENT', None, above=0., maxval=max_cur)
        if run is None and hold is None:
            gcmd.respond_info(f"Run: {prev_run:.2f}A  Hold: {prev_hold:.2f}A")
            return
        run = run if run is not None else prev_run
        hold = hold if hold is not None else prev_hold
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        ch.set_current(run, hold, print_time)
        gcmd.respond_info(f"Run: {run:.2f}A  Hold: {hold:.2f}A")

    def setup_register_dump(self, read_registers):
        self.read_registers = read_registers
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("DUMP_LYX", "STEPPER", self.name,
                                   self.cmd_DUMP_LYX)

    def cmd_DUMP_LYX(self, gcmd):
        gcmd.respond_info("=== Write registers ===")
        for reg_name, val in self.fields.registers.items():
            if reg_name not in self.read_registers:
                gcmd.respond_info(self.fields.pretty_format(reg_name, val))
        gcmd.respond_info("=== Live registers ===")
        for reg_name in self.read_registers:
            val = self.mcu_lyx.get_register(reg_name)
            gcmd.respond_info(self.fields.pretty_format(reg_name, val))

    def get_status(self, eventtime=None):
        cur = self.current_helper.get_current()
        return {
            'run_current': cur[0],
            'hold_current': cur[1],
        }
