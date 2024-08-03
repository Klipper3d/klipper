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
# supports XY, XZ & YZ planes with remaining axis as helical

# Enum
ARC_PLANE_X_Y = 0
ARC_PLANE_X_Z = 1
ARC_PLANE_Y_Z = 2

# Enum
X_AXIS = 0
Y_AXIS = 1
Z_AXIS = 2
E_AXIS = 3


class ArcSupport:

    def __init__(self, config):
        self.printer = config.get_printer()
        self.mm_per_arc_segment = config.getfloat('resolution', 1., above=0.0)

        self.gcode_move = self.printer.load_object(config, 'gcode_move')
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G2", self.cmd_G2)
        self.gcode.register_command("G3", self.cmd_G3)

        self.gcode.register_command("G17", self.cmd_G17)
        self.gcode.register_command("G18", self.cmd_G18)
        self.gcode.register_command("G19", self.cmd_G19)

        # backwards compatibility, prior implementation only supported XY
        self.plane = ARC_PLANE_X_Y

    def cmd_G2(self, gcmd):
        self._cmd_inner(gcmd, True)

    def cmd_G3(self, gcmd):
        self._cmd_inner(gcmd, False)

    def cmd_G17(self, gcmd):
        self.plane = ARC_PLANE_X_Y

    def cmd_G18(self, gcmd):
        self.plane = ARC_PLANE_X_Z

    def cmd_G19(self, gcmd):
        self.plane = ARC_PLANE_Y_Z

    def _cmd_inner(self, gcmd, clockwise):
        gcodestatus = self.gcode_move.get_status()
        if not gcodestatus['absolute_coordinates']:
            raise gcmd.error("G2/G3 does not support relative move mode")
        currentPos = gcodestatus['gcode_position']
        absolut_extrude = gcodestatus['absolute_extrude']

        # Parse parameters
        asTarget = [gcmd.get_float("X", currentPos[0]),
                    gcmd.get_float("Y", currentPos[1]),
                    gcmd.get_float("Z", currentPos[2])]

        if gcmd.get_float("R", None) is not None:
            raise gcmd.error("G2/G3 does not support R moves")

        # determine the plane coordinates and the helical axis
        I = gcmd.get_float('I', 0.)
        J = gcmd.get_float('J', 0.)
        asPlanar = (I, J)
        axes = (X_AXIS, Y_AXIS, Z_AXIS)
        if self.plane == ARC_PLANE_X_Z:
            K = gcmd.get_float('K', 0.)
            asPlanar = (I, K)
            axes = (X_AXIS, Z_AXIS, Y_AXIS)
        elif self.plane == ARC_PLANE_Y_Z:
            K = gcmd.get_float('K', 0.)
            asPlanar = (J, K)
            axes = (Y_AXIS, Z_AXIS, X_AXIS)

        if not (asPlanar[0] or asPlanar[1]):
            raise gcmd.error("G2/G3 requires IJ, IK or JK parameters")

        asE = gcmd.get_float("E", None)
        asF = gcmd.get_float("F", None)

        # Build list of linear coordinates to move
        segments, coords = self.planArc(currentPos, asTarget, asPlanar,
                                        clockwise, *axes)
        e_per_move = e_base = 0.
        if asE is not None:
            if absolut_extrude:
                e_base = currentPos[3]
            e_per_move = (asE - e_base) / segments

        # Convert coords into G1 commands
        for coord in coords:
            g1_params = {'X': coord[0], 'Y': coord[1], 'Z': coord[2]}
            if e_per_move:
                g1_params['E'] = e_base + e_per_move
                if absolut_extrude:
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
    #
    # alpha and beta axes are the current plane, helical axis is linear travel
    def planArc(self, currentPos, targetPos, offset, clockwise,
                alpha_axis, beta_axis, helical_axis):
        # todo: sometimes produces full circles

        # Radius vector from center to current location
        r_P = -offset[0]
        r_Q = -offset[1]

        # Determine angular travel
        center_P = currentPos[alpha_axis] - r_P
        center_Q = currentPos[beta_axis] - r_Q
        rt_Alpha = targetPos[alpha_axis] - center_P
        rt_Beta = targetPos[beta_axis] - center_Q
        angular_travel = math.atan2(r_P * rt_Beta - r_Q * rt_Alpha,
                                    r_P * rt_Alpha + r_Q * rt_Beta)
        if angular_travel < 0.:
            angular_travel += 2. * math.pi
        if clockwise:
            angular_travel -= 2. * math.pi

        if (angular_travel == 0.
            and currentPos[alpha_axis] == targetPos[alpha_axis]
            and currentPos[beta_axis] == targetPos[beta_axis]):
            # Make a circle if the angular rotation is 0 and the
            # target is current position
            angular_travel = 2. * math.pi

        # Determine number of segments
        linear_travel = targetPos[helical_axis] - currentPos[helical_axis]
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
            dist_Helical = i * linear_per_segment
            c_theta = i * theta_per_segment
            cos_Ti = math.cos(c_theta)
            sin_Ti = math.sin(c_theta)
            r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti
            r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti

            c = [None, None, None]
            c[alpha_axis] = center_P + r_P
            c[beta_axis] = center_Q + r_Q
            c[helical_axis] = currentPos[helical_axis] + dist_Helical
            coords.append(c)

        coords.append(targetPos)
        return segments, coords

def load_config(config):
    return ArcSupport(config)
