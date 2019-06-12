# Execute Z Homing at specific XY coordinates. Also allows to lift Z prior to
# probing
#
# Copyright (C) 2019 Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import probe

class SafeZHoming:
    def __init__(self, config):
        self.printer = config.get_printer()
        (self.home_x_pos,
          self.home_y_pos) = config.get("home_xy_position").split(',')
        self.z_hop = config.getfloat("z_hop", default=0.0)
        self.z_hop_speed = config.getfloat('z_hop_speed', 15., above=0.)
        self.speed = config.getfloat('speed', 50.0, above=0.)
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G28", None)
        self.gcode.register_command("G28", self.cmd_G28)

    def cmd_G28(self, params):
        need_x, need_y, need_z = [False] * 3

        # Perform Z Hop if necessary
        if self.z_hop != 0.0:
            toolhead = self.printer.lookup_object('toolhead')
            pos = toolhead.get_position()
            toolhead.set_position(pos, homing_axes=[2])
            pos[2] = pos[2] + self.z_hop
            toolhead.move(pos, self.z_hop_speed)
            self.gcode.reset_last_position()

        # Determine which axes we need to home
        if not any([axis in params.keys() for axis in ['X', 'Y', 'Z']]):
            need_x, need_y, need_z = [True] * 3
        else:
            need_z = 'Z' in params
            need_x = 'X' in params
            need_y = 'Y' in params

        # Home XY axes if necessary
        new_params = {}
        if need_x:
            new_params['X'] = '0'
        if need_y:
            new_params['Y'] = '0'
        if new_params:
            self.gcode.cmd_G28(new_params)
        # Home Z axis if necessary
        if need_z:
            # Move to safe XY homing position
            self.gcode.cmd_G1({'X': self.home_x_pos,
                               'Y': self.home_y_pos,
                               'F': self.speed*60.0})
            # Home Z
            self.gcode.cmd_G28({'Z': '0'})

def load_config(config):
    return SafeZHoming(config)
