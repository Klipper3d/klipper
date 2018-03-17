# Save and restore position
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class error(Exception):
    pass

class PositionRecall:
    def __init__(self, config):
        self.slots = {}
        self.printer = config.get_printer()
        self.gcode = self.toolhead = None
    def printer_state(self, state):
        if state == 'ready':
            self.gcode = self.printer.lookup_object('gcode')
            self.toolhead = self.printer.lookup_object('toolhead')
            self.gcode.register_command("SAVE_CURRENT_POSITION", self.cmd_SAVE_CURRENT_POSITION,
                                    desc=self.cmd_SAVE_CURRENT_POSITION_help)
            self.gcode.register_command("RESTORE_POSITION", self.cmd_RESTORE_POSITION,
                                    desc=self.cmd_RESTORE_POSITION_help)
    cmd_SAVE_CURRENT_POSITION_help = "Save the current position to a slot"
    def cmd_SAVE_CURRENT_POSITION(self, params):
        name = self.gcode.get_str('NAME', params, 'default')
        self.slots[name] = (self.toolhead.get_position(), self.gcode.speed)
    cmd_RESTORE_POSITION_help = "Move to the position previously saved in a slot"
    def cmd_RESTORE_POSITION(self, params):
        name = self.gcode.get_str('NAME', params, 'default')
        if name not in self.slots:
            raise error("Invalid slot '%s' not previously set" % (name,))
        pos = self.toolhead.get_position()
        changed = False
        for axis in params.keys():
            if axis in self.gcode.axis2pos:
                pos[self.gcode.axis2pos[axis]] = self.slots[name][0][self.gcode.axis2pos[axis]] + self.gcode.get_float(axis, params, 0.)
                changed = True
        if not changed:
            pos = self.slots[name][0]
        if 'F' in params.keys():
            speed = float(params['F']) * self.gcode.speed_factor
            if speed < 0.:
                raise error("Invalid speed '%f'" % (speed,))
            elif speed == 0.:
                speed = self.slots[name][1]
        else:
            speed = self.slots[name][1]
        self.toolhead.move(pos, speed)
        self.gcode.reset_last_position()

def load_config(config):
    return PositionRecall(config)
