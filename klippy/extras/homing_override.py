# Run user defined actions in place of a normal G28 homing command
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class HomingOverride:
    def __init__(self, config):
        printer = config.get_printer()
        self.script = config.get('gcode')
        self.in_script = False
        self.gcode = printer.lookup_object('gcode')
        self.gcode.register_command("G28", self.cmd_G28)
    def cmd_G28(self, params):
        if self.in_script:
            # Was called recursively - invoke the real G28 command
            self.gcode.cmd_G28(params)
            return
        try:
            self.in_script = True
            self.gcode.run_script(self.script)
        finally:
            self.in_script = False

def load_config(config):
    if config.get_name() != 'homing_override':
        raise config.error("Invalid homing_override config name")
    return HomingOverride(config)
