# Perform Z Homing at specific XY coordinates.
#
# Copyright (C) 2019 Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

class SafeZHoming:
    def __init__(self, config):
        self.printer = config.get_printer()
        try:
            x_pos, y_pos = config.get("home_xy_position",
                                      default=",").split(',')
            self.home_x_pos, self.home_y_pos = float(x_pos), float(y_pos)
        except:
            raise config.error("Unable to parse home_xy_position in %s" % (
                config.get_name()))

        self.z_hop = config.getfloat("z_hop", default=0.0)
        self.z_hop_speed = config.getfloat('z_hop_speed', 15., above=0.)
        self.max_z = config.getsection('stepper_z').getfloat('position_max')
        self.speed = config.getfloat('speed', 50.0, above=0.)
        self.move_to_previous = config.getboolean('move_to_previous', False)
        self.gcode = self.printer.lookup_object('gcode')
        self.prev_G28 = self.gcode.register_command("G28", None)
        self.gcode.register_command("G28", self.cmd_G28)

        if config.has_section("homing_override"):
            raise config.error("homing_override and safe_z_homing cannot"
                               +" be used simultaneously")

    def cmd_G28(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        curtime = self.printer.get_reactor().monotonic()
        kin_status = toolhead.get_kinematics().get_status(curtime)

        # Perform Z Hop if necessary
        if self.z_hop != 0.0:
            pos = toolhead.get_position()
            # Check if Z axis is homed or has a known position
            if 'z' in kin_status['homed_axes']:
                # Check if the zhop would exceed the printer limits
                if pos[2] + self.z_hop > self.max_z:
                    gcmd.respond_info(
                        "No zhop performed, target Z out of bounds: " +
                        str(pos[2] + self.z_hop))
                elif pos[2] < self.z_hop:
                    self._perform_z_hop(pos)
            else:
                self._perform_z_hop(pos)
                if hasattr(toolhead.get_kinematics(), "note_z_not_homed"):
                    toolhead.get_kinematics().note_z_not_homed()

        # Determine which axes we need to home
        need_x, need_y, need_z = [gcmd.get(axis, None) is not None
                                  for axis in "XYZ"]
        if not need_x and not need_y and not need_z:
            need_x = need_y = need_z = True

        # Home XY axes if necessary
        new_params = {}
        if need_x:
            new_params['X'] = '0'
        if need_y:
            new_params['Y'] = '0'
        if new_params:
            g28_gcmd = self.gcode.create_gcode_command("G28", "G28", new_params)
            self.prev_G28(g28_gcmd)

        # Update the currently homed axes
        curtime = self.printer.get_reactor().monotonic()
        kin_status = toolhead.get_kinematics().get_status(curtime)

        # Home Z axis if necessary
        if need_z:
            pos = toolhead.get_position()
            prev_x = pos[0]
            prev_y = pos[1]
            pos[0] = self.home_x_pos
            pos[1] = self.home_y_pos
            # Throw an error if X or Y are not homed
            if ('x' not in kin_status['homed_axes'] or
                'y' not in kin_status['homed_axes']):
                raise gcmd.error("Must home X and Y axes first")
            # Move to safe XY homing position
            toolhead.move(pos, self.speed)
            self.gcode.reset_last_position()
            # Home Z
            g28_gcmd = self.gcode.create_gcode_command("G28", "G28", {'Z': '0'})
            self.prev_G28(g28_gcmd)
            # Perform Z Hop again for pressure-based probes
            pos = toolhead.get_position()
            if self.z_hop:
                pos[2] = self.z_hop
                toolhead.move(pos, self.z_hop_speed)
            # Move XY back to previous positions
            if self.move_to_previous:
                pos[0] = prev_x
                pos[1] = prev_y
                toolhead.move(pos, self.speed)
            self.gcode.reset_last_position()

    def _perform_z_hop(self, pos):
        toolhead = self.printer.lookup_object('toolhead')
        # Perform the Z-Hop
        toolhead.set_position(pos, homing_axes=[2])
        pos[2] = pos[2] + self.z_hop
        toolhead.move(pos, self.z_hop_speed)
        self.gcode.reset_last_position()

def load_config(config):
    return SafeZHoming(config)
