# adds support fro ARC commands via G2/G3
#
# Copyright (C) 2019  Aleksej Vasiljkovic <achmed21@gmail.com>
#
# function planArc() originates from https://github.com/MarlinFirmware/Marlin
# Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math

# Coordinates created by this are converted into G1 commands.
#
# note: only IJ version available

class ArcSupport:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.mm_per_arc_segment = config.getfloat('resolution', 1., above=0.0)

        self.gcode_move = self.printer.load_object(config, 'gcode_move')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G2", self.cmd_G2)
        self.gcode.register_command("G3", self.cmd_G3)

    def cmd_G2(self, gcmd):
        self._cmd_inner(gcmd, True)

    def cmd_G3(self, gcmd):
        self._cmd_inner(gcmd, False)

    def _cmd_inner(self, gcmd, clockwise):
        gcodestatus = self.gcode_move.get_status()
        if not gcodestatus['absolute_coordinates']:
            raise gcmd.error("G2/G3 does not support relative move mode")
        currentPos = gcodestatus['gcode_position']

        # Parse parameters
        asX = gcmd.get_float("X", currentPos[0])
        asY = gcmd.get_float("Y", currentPos[1])
        asZ = gcmd.get_float("Z", currentPos[2])
        if gcmd.get_float("R", None) is not None:
            raise gcmd.error("G2/G3 does not support R moves")
        asI = gcmd.get_float("I", 0.)
        asJ = gcmd.get_float("J", 0.)
        if not asI and not asJ:
            raise gcmd.error("G2/G3 neither I nor J given")
        asE = gcmd.get_float("E", None)
        asF = gcmd.get_float("F", None)

        # Build list of linear coordinates to move to
        coords = self.planArc(currentPos, [asX, asY, asZ], [asI, asJ],
                              clockwise)
        e_per_move = e_base = 0.
        if asE is not None:
            if gcodestatus['absolute_extrude']:
                e_base = currentPos[3]
            e_per_move = (asE - e_base) / len(coords)

        # Convert coords into G1 commands
        for coord in coords:
            g1_params = {'X': coord[0], 'Y': coord[1], 'Z': coord[2]}
            if e_per_move:
                g1_params['E'] = e_base + e_per_move
                if gcodestatus['absolute_extrude']:
                    e_base += e_per_move
            if asF is not None:
                g1_params['F'] = asF
            g1_gcmd = self.gcode.create_gcode_command("G1", "G1", g1_params)
            self.gcode_move.cmd_G1(g1_gcmd)

    # function planArc() originates from marlin plan_arc()
    # https://github.com/MarlinFirmware/Marlin
    #
    # The arc is approximated by generating many small linear segments.
    # The length of each segment is configured in MM_PER_ARC_SEGMENT
    # Arcs smaller then this value, will be a Line only
    def planArc(self, currentPos, targetPos, offset, clockwise):
        # todo: sometimes produces full circles
        X_AXIS = 0
        Y_AXIS = 1
        Z_AXIS = 2

        # Radius vector from center to current location
        r_P = -offset[0]
        r_Q = -offset[1]

        # Determine angular travel
        center_P = currentPos[X_AXIS] - r_P
        center_Q = currentPos[Y_AXIS] - r_Q
        rt_X = targetPos[X_AXIS] - center_P
        rt_Y = targetPos[Y_AXIS] - center_Q
        angular_travel = math.atan2(r_P * rt_Y - r_Q * rt_X,
                                    r_P * rt_X + r_Q * rt_Y)
        if angular_travel < 0.:
            angular_travel += 2. * math.pi
        if clockwise:
            angular_travel -= 2. * math.pi

        if (angular_travel == 0.
            and currentPos[X_AXIS] == targetPos[X_AXIS]
            and currentPos[Y_AXIS] == targetPos[Y_AXIS]):
            # Make a circle if the angular rotation is 0 and the
            # target is current position
            angular_travel = 2. * math.pi

        # Determine number of segments
        linear_travel = targetPos[Z_AXIS] - currentPos[Z_AXIS]
        radius = math.hypot(r_P, r_Q)
        flat_mm = radius * angular_travel
        if linear_travel:
            mm_of_travel = math.hypot(flat_mm, linear_travel)
        else:
            mm_of_travel = math.fabs(flat_mm)
        segments = max(1., math.floor(mm_of_travel / self.mm_per_arc_segment))

        # Generate coordinates
        theta_per_segment = angular_travel / segments
        linear_per_segment = linear_travel / segments
        coords = []
        for i in range(1, int(segments)):
            dist_Z = i * linear_per_segment
            cos_Ti = math.cos(i * theta_per_segment)
            sin_Ti = math.sin(i * theta_per_segment)
            r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti
            r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti

            c = [center_P + r_P, center_Q + r_Q, currentPos[Z_AXIS] + dist_Z]
            coords.append(c)

        coords.append(targetPos)
        return coords

def load_config(config):
    return ArcSupport(config)
