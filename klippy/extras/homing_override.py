# Run user defined actions in place of a normal G28 homing command
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class HomingOverride:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.start_pos = [config.getfloat('set_position_' + a, None)
                          for a in 'xyz']
        self.script = config.get('gcode')
        self.in_script = False
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G28", self.cmd_G28)
    def cmd_G28(self, params):
        if self.in_script:
            # Was called recursively - invoke the real G28 command
            self.gcode.cmd_G28(params)
            return
        # Calculate forced position (if configured)
        toolhead = self.printer.lookup_object('toolhead')
        pos = toolhead.get_position()
        homing_axes = []
        for axis, loc in enumerate(self.start_pos):
            if loc is not None:
                pos[axis] = loc
                homing_axes.append(axis)
        toolhead.set_position(pos, homing_axes=homing_axes)
        self.gcode.reset_last_position()
        # Perform homing
        try:
            self.in_script = True
            self.gcode.run_script(self.script)
        finally:
            self.in_script = False

def load_config(config):
    if config.get_name() != 'homing_override':
        raise config.error("Invalid homing_override config name")
    return HomingOverride(config)
