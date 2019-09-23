# adds support fro ARC commands via G2/G3
#
# Copyright (C) 2019  Aleksej Vasiljkovic <achmed21@gmail.com>
#
# function planArc() originates from https://github.com/MarlinFirmware/Marlin
# Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
#
# This file may be distributed under the terms of the GNU GPLv3 license.


# uses the plan_arc function from marlin which does steps in mm rather then
# in degrees. # Coordinates created by this are converted into G1 commands.
#
# note: only IJ version available

import math
import re

class ArcSupport:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.mm_per_arc_segment = config.getfloat('resolution', 1, above=0.0)

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G2", self.cmd_G2, desc=self.cmd_G2_help)
        self.gcode.register_command("G3", self.cmd_G2, desc=self.cmd_G3_help)

    cmd_G2_help = "Counterclockwise rotation move"
    cmd_G3_help = "Clockwise rotaion move"

    def cmd_G2(self, params):
        # set vars
        currentPos =  self.printer.lookup_object('toolhead').get_position()

        asX = params.get("X", None)
        asY = params.get("Y", None)
        asZ = params.get("Z", None)

        asR = float(params.get("R", 0.))    #radius
        asI = float(params.get("I", 0.))
        asJ = float(params.get("J", 0.))

        asE = float(params.get("E", 0.))
        asF = float(params.get("F", -1))

        # --------- health checks of code -----------
        if (asX is None or asY is None):
            raise self.gcode.error("g2/g3: Coords missing")

        elif asR == 0 and asI == 0 and asJ==0:
            raise self.gcode.error("g2/g3: neither R nor I and J given")

        elif asR > 0 and (asI !=0 or asJ!=0):
            raise self.gcode.error("g2/g3: R, I and J were given. Invalid")
        else:   # -------- execute conversion -----------
            coords = []
            clockwise = params['#command'].lower().startswith("g2")
            asY = float(asY)
            asX = float(asX)

            # use radius
            # if asR > 0:
                # not sure if neccessary since R barely seems to be used

            # use IJK

            if asI != 0 or asJ!=0:
                coords = self.planArc(currentPos,
                            [asX,asY,0.,0.],
                            [asI, asJ],
                            clockwise)
            ###############################
            # converting coords into G1 codes (lazy aproch)
            if len(coords)>0:

                # build dict and call cmd_G1
                for coord in coords:
                    g1_params = {'X': coord[0], 'Y': coord[1]}
                    if asZ!=None:
                        g1_params['Z']= float(asZ)
                    if asE>0:
                        g1_params['E']= float(asE)/len(coords)
                    if asF>0:
                        g1_params['F']= asF

                    self.gcode.cmd_G1(g1_params)




            else:
                self.gcode.respond_info(
                    "could not tranlate from '" + params['#original'] + "'")


    # function planArc() originates from marlin plan_arc()
    # https://github.com/MarlinFirmware/Marlin
    #
    # The arc is approximated by generating many small linear segments.
    # The length of each segment is configured in MM_PER_ARC_SEGMENT
    # Arcs smaller then this value, will be a Line only

    def planArc(
            self,
            currentPos,
            targetPos=[0.,0.,0.,0.],
            offset=[0.,0.],
            clockwise=False):
        # todo: sometimes produces full circles
        coords = []
        MM_PER_ARC_SEGMENT = self.mm_per_arc_segment

        X_AXIS = 0
        Y_AXIS = 1
        Z_AXIS = 2

        # Radius vector from center to current location
        r_P = offset[0]*-1
        r_Q = offset[1]*-1

        radius = math.hypot(r_P, r_Q)
        center_P = currentPos[X_AXIS] - r_P
        center_Q = currentPos[Y_AXIS] - r_Q
        rt_X = targetPos[X_AXIS] - center_P
        rt_Y = targetPos[Y_AXIS] - center_Q
        linear_travel = targetPos[Z_AXIS] - currentPos[Z_AXIS]

        angular_travel = math.atan2(r_P * rt_Y - r_Q * rt_X,
            r_P * rt_X + r_Q * rt_Y)
        if (angular_travel < 0): angular_travel+= math.radians(360)
        if (clockwise): angular_travel-= math.radians(360)

        # Make a circle if the angular rotation is 0
        # and the target is current position
        if (angular_travel == 0
            and currentPos[X_AXIS] == targetPos[X_AXIS]
            and currentPos[Y_AXIS] == targetPos[Y_AXIS]):
            angular_travel = math.radians(360)


        flat_mm = radius * angular_travel
        mm_of_travel = linear_travel
        if(mm_of_travel == linear_travel):
            mm_of_travel = math.hypot(flat_mm, linear_travel)
        else:
            mm_of_travel = math.abs(flat_mm)


        if (mm_of_travel < 0.001):
            return coords

        segments = int(math.floor(mm_of_travel / (MM_PER_ARC_SEGMENT)))
        if(segments<1):
            segments=1


        raw = [0.,0.,0.,0.]
        theta_per_segment = float(angular_travel / segments)
        linear_per_segment = float(linear_travel / segments)

        # Initialize the linear axis
        raw[Z_AXIS] = currentPos[Z_AXIS];


        for i in range(1,segments+1):
            cos_Ti = math.cos(i * theta_per_segment)
            sin_Ti = math.sin(i * theta_per_segment)
            r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti
            r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti

            raw[X_AXIS] = center_P + r_P
            raw[Y_AXIS] = center_Q + r_Q
            raw[Z_AXIS] += linear_per_segment

            coords.append([raw[X_AXIS],  raw[Y_AXIS], raw[Z_AXIS] ])

        return coords


def load_config(config):
    return ArcSupport(config)
