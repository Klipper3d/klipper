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

        self.move_to_previous = config.getboolean('move_to_previous', False)

        self.park_position = None;
        park = config.get('park_position', None)

        if park:

            try:
                self.park_position = [float(i) for i in park.split(',')]

            except:
                raise config.error("Unable to parse park_position in %s" % (
                            config.get_name()))

        self.z_hop = config.getfloat("z_hop", default=0.0)
        self.z_hop_speed = config.getfloat('z_hop_speed', 15., above=0.)
        self.max_z = config.getsection('stepper_z').getfloat('position_max')
        self.speed = config.getfloat('speed', 50.0, above=0.)

        xy_orders = { "xy" : "xy", "yx" : "yx" }
        self.xy_order = config.getchoice("xy_order", xy_orders, "xy")

        self.gcode = self.printer.lookup_object('gcode')
        self.prev_G28 = self.gcode.register_command("G28", None)
        self.gcode.register_command("G28", self.cmd_G28)

        if config.has_section("homing_override"):
            raise config.error("homing_override and safe_z_homing cannot"
                               +" be used simultaneously")

    def cmd_G28(self, params):
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
                    self.gcode.respond_info(
                        "No zhop performed, target Z out of bounds: " +
                        str(pos[2] + self.z_hop)
                    )
                elif pos[2] < self.z_hop:
                    self._perform_z_hop(pos)
            else:
                self._perform_z_hop(pos)
                if hasattr(toolhead.get_kinematics(), "note_z_not_homed"):
                    toolhead.get_kinematics().note_z_not_homed()

        # Determine which axes we need to home
        if not any([axis in params.keys() for axis in ['X', 'Y', 'Z']]):
            need_x, need_y, need_z = [True] * 3
        else:
            need_x, need_y, need_z = tuple(axis in params
                                           for axis in ['X', 'Y', 'Z'])


        prev_pos = toolhead.get_position()

        # Home XY axes if necessary
        for axis in self.xy_order:

            if axis == 'x' and need_x:
               self.prev_G28('X0')

            if axis == 'y' and need_y:
               self.prev_G28('Y0')

        # Home Z axis if necessary
        if need_z:

            # Move to safe XY homing position
            pos = toolhead.get_position()
            pos[0] = self.home_x_pos
            pos[1] = self.home_y_pos
            toolhead.move(pos, self.speed)
            self.gcode.reset_last_position()

            # Home Z
            self.prev_G28({'Z': '0'})
            pos = toolhead.get_position()

            if self.park_position:
                pos[2] = self.park_position[2]

            if 'z' in kin_status['homed_axes']:

                #return z to prev position if previously homed
                #or re-hop it for pressure based probes
                if self.move_to_previous:
                    pos[2] = prev_pos[2]
                    toolhead.move(pos, self.z_hop_speed)

                elif self.z_hop:
                    pos[2] = self.z_hop
                    toolhead.move(pos, self.z_hop_speed)

        #if axis was already homed, and return is specified,
        #return it to previous position, otherwise park it
        #at park coordinate if parking is specified
        if 'x' in kin_status['homed_axes'] and self.move_to_previous:
                pos[0] = prev_pos[0]
        elif self.park_position:
                pos[0] = self.park_position[0]

        if 'y' in kin_status['homed_axes'] and self.move_to_previous:
                pos[1] = prev_pos[1]
        elif self.park_position:
                pos[1] = self.park_position[1]

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
