# Common helper code for L647x stepper drivers
#
# Copyright (C) 2020-2021  HOUIDAR Oussama
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, collections
import stepper


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
            reg_value = self.registers.get(reg_name, 0)
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
    def set_config_field(self, config, field_name,default):
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

class L647xCommandHelper:
    def __init__(self, config, mcu_L647x, current_helper, clear_gstat=True):
        self.printer = config.get_printer()
        self.stepper_name = ' '.join(config.get_name().split()[1:])
        self.name = config.get_name().split()[-1]
        self.mcu_L647x = mcu_L647x
        self.current_helper = current_helper

        self.fields = mcu_L647x.get_fields()
        self.commands = mcu_L647x.get_commands()
        self.read_registers = self.read_translate = None
        self.toff = None
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        # Register commands
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("SET_FIELD_L6474", "STEPPER", self.name,
                                   self.cmd_SET_L647x_FIELD,
                                   desc=self.cmd_SET_L647x_FIELD_help)
        gcode.register_mux_command("INIT_L6474", "STEPPER", self.name,
                                   self.cmd_INIT_L647x,
                                   desc=self.cmd_INIT_L647x_help)

    def _init_registers(self, print_time=None):
        # Send registers
        for reg_name, val in self.fields.registers.items():
            self.mcu_L647x.set_register(reg_name, val, print_time)
    cmd_INIT_L647x_help = "Initialize L647x stepper driver registers"
    def cmd_INIT_L647x(self, gcmd):
        logging.info("INIT_L647x %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self._init_registers(print_time)
    cmd_SET_L647x_FIELD_help = "Set a register field of a L647x driver"
    def cmd_SET_L647x_FIELD(self, gcmd):
        field_name = gcmd.get('FIELD')
        reg_name = self.fields.lookup_register(field_name, None)
        if reg_name is None:
            raise gcmd.error("Unknown field name '%s'" % (field_name,))
        value = gcmd.get_int('VALUE')
        reg_val = self.fields.set_field(field_name, value)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        self.mcu_L647x.set_register(reg_name, reg_val, print_time)


    def _do_enable(self, print_time):
        try:
            self.mcu_L647x.set_register("CMD_ENABLE",0, print_time)
            self._init_registers()

        except self.printer.command_error as e:
            self.printer.invoke_shutdown(str(e))
    def _do_disable(self, print_time):
        try:
            self.mcu_L647x.set_register("CMD_DISABLE",0, print_time)
        except self.printer.command_error as e:
            self.printer.invoke_shutdown(str(e))
    def handle_stepper_enable(self, print_time, is_enable):
        if is_enable:
            cb = (lambda ev: self._do_enable(print_time))
        else:
            cb = (lambda ev: self._do_disable(print_time))
        self.printer.get_reactor().register_callback(cb)
    def _handle_connect(self):
        # Check for soft stepper enable/disable
        stepper_enable = self.printer.lookup_object('stepper_enable')
        if not(self.stepper_name.startswith('dummy')):
            enable_line = stepper_enable.lookup_enable(self.stepper_name)
            enable_line.register_state_callback(self.handle_stepper_enable)

        # Send init
        try:
            self._init_registers()
            print_time = self.printer.lookup_object('toolhead')\
                .get_last_move_time()
            self._do_disable(print_time)
            val = self.cmd_GET_STATUS()

        except self.printer.command_error as e:
            logging.info("L647x %s failed to init: %s", self.name, str(e))
    # DUMP_L647x support
    def setup_register_dump(self, read_registers, read_translate=None):
        self.read_registers = read_registers
        self.read_translate = read_translate
        gcode = self.printer.lookup_object("gcode")
        gcode.register_mux_command("DUMP_L6474", "STEPPER", self.name,
                                   self.cmd_DUMP_L647x,
                                   desc=self.cmd_DUMP_L647x_help)
    cmd_DUMP_L647x_help = "Read and display L647x stepper driver registers"
    def cmd_DUMP_L647x(self, gcmd):
        logging.info("DUMP_L647x %s", self.name)
        print_time = self.printer.lookup_object('toolhead').get_last_move_time()
        gcmd.respond_info("========== Queried registers ==========")
        for reg_name in self.read_registers:
            if (reg_name not in self.commands):
                val = self.mcu_L647x.get_register(reg_name)
                if self.read_translate is not None:
                    reg_name, val = self.read_translate(reg_name, val)
                gcmd.respond_info(self.fields.pretty_format(reg_name, val))

    def cmd_GET_STATUS(self):
        return self.mcu_L647x.get_register("CMD_GET_STATUS")
