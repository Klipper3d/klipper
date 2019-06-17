# Execute Z Homing at specific XY coordinates. Also allows to lift Z prior to
# probing
#
# Copyright (C) 2019 Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import probe, toolhead

class SafeZHoming:
    def __init__(self, config):
        self.printer = config.get_printer()
        try:
            (self.home_x_pos,
            self.home_y_pos) = config.get("home_xy_position",
                                            default=",").split(',')
        except:
            raise config.error("Unable to parse home_xy_position in %s" % (
                config.get_name()))

        self.z_hop = config.getfloat("z_hop", default=0.0)
        self.z_hop_speed = config.getfloat('z_hop_speed', 15., above=0.)
        self.speed = config.getfloat('speed', 50.0, above=0.)
        self.gcode = self.printer.lookup_object('gcode')
        self.toolhead = None
        self.gcode.register_command("G28", None)
        self.gcode.register_command("G28", self.cmd_G28)

        if self.printer.lookup_object("homing_override", default=None):
            raise config.error("homing_override and safe_z_homing cannot"
                                    +" be used simultaneously")

    def cmd_G28(self, params):
        need_x, need_y, need_z = [False] * 3
        if not self.toolhead:
            self.toolhead = self.printer.lookup_object('toolhead')
        # Perform Z Hop if necessary
        if self.z_hop != 0.0:
            pos = self.toolhead.get_position()
            pos[2] = pos[2] + self.z_hop
            move = toolhead.Move(self.toolhead, self.toolhead.commanded_pos,
                                 pos, self.z_hop_speed)
            self.toolhead.move_queue.add_move(move)
            if self.toolhead.print_time > self.toolhead.need_check_stall:
                self.toolhead._check_stall()
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
            pos = self.toolhead.get_position()
            if self.home_x_pos:
                pos[0] = float(self.home_x_pos)
            if self.home_y_pos:
                pos[1] = float(self.home_y_pos)
            self.toolhead.move(pos, self.speed)
            # Home Z
            self.gcode.cmd_G28({'Z': '0'})

def load_config(config):
    return SafeZHoming(config)
