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

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G2", self.cmd_G2)
        self.gcode.register_command("G3", self.cmd_G2)

    def cmd_G2(self, params):
        gcodestatus = self.gcode.get_status(None)
        if not gcodestatus['absolute_coordinates']:
            raise self.gcode.error("G2/G3 does not support relative move mode")
        currentPos = gcodestatus['gcode_position']

        # Parse parameters
        asX = self.gcode.get_float("X", params, currentPos[0])
        asY = self.gcode.get_float("Y", params, currentPos[1])
        asZ = self.gcode.get_float("Z", params, currentPos[2])
        if self.gcode.get_float("R", params, None) is not None:
            raise self.gcode.error("G2/G3 does not support R moves")
        asI = self.gcode.get_float("I", params, 0.)
        asJ = self.gcode.get_float("J", params, 0.)
        if not asI and not asJ:
            raise self.gcode.error("G2/G3 neither I nor J given")
        asE = self.gcode.get_float("E", params, None)
        if asE is not None and gcodestatus['absolute_extrude']:
            raise self.gcode.error("G2/G3 only supports relative extrude mode")
        asF = self.gcode.get_float("F", params, None)
        clockwise = (params['#command'] == 'G2')

        # Build list of linear coordinates to move to
        coords = self.planArc(currentPos, [asX, asY, asZ], [asI, asJ],
                              clockwise)

        # Convert coords into G1 commands
        for coord in coords:
            g1_params = {'X': coord[0], 'Y': coord[1], 'Z': coord[2]}
            if asE is not None:
                g1_params['E'] = asE / len(coords)
            if asF is not None:
                g1_params['F'] = asF
            self.gcode.cmd_G1(g1_params)

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
