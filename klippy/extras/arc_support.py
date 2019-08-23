# adds g2/g3 support. converts both commands into g1 code in 360 degree steps
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import math
import re

class ArcSupport:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.degree_steps = config.getfloat('degree_steps', 1)
        self.debug = config.getboolean('debug', False)
        # if self.degree_steps>0:

        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command("G2", self.cmd_G2, desc=self.cmd_G2_help)
        self.gcode.register_command("G3", self.cmd_G2, desc=self.cmd_G3_help)
            # self.gcode.register_command("G3", self.cmd_G3)
        
        # raise config.error("Arc support test")

    cmd_G2_help = cmd_G3_help = "testing ..."
    def cmd_G2(self, params):
        msg = params['#original']
    
        # # set vars

        # toolhead = self.printer.lookup_object('toolhead')
        currentPos =  self.printer.lookup_object('toolhead').get_position()
        asStartX = currentPos[0] 
        asStartY = currentPos[1]
        asStartZ = currentPos[2]

        asX = params.get("X", "")
        asY = params.get("Y", "")

        asR = float(params.get("R", 0))    #radius
        asI = float(params.get("I", 0))
        asJ = float(params.get("J", 0))

        asE = float(params.get("E", 0))
        asF = float(params.get("F", -1))

        # --------- health checks of code -----------
        if (asX == "" or asY == ""):
            raise self.gcode.error("g2/g3: Coords missing")

        elif asR == 0 and asI == 0 and asJ==0:
            raise self.gcode.error("g2/g3: neither R nor I and J given")

        elif asR > 0 and (asI !=0 or asJ!=0):
            raise self.gcode.error("g2/g3: R, I and J were given. no idea what to do ...")
        else:   # -------- execute conversion -----------
            coords = []
            clockwise = params['#command'].lower().startswith("g2") # direction, reversed for g3
            asY = float(asY)
            asX = float(asX)

            # use radius
            # if asR > 0: 
            #     startAngle = 0.0
            #     endAngle = 360.0
            #     if rev :
            #         startAngle = 360.0
            #         endAngle = 0.0

            #     coords=calcRadCoords(asX, asY, asR, 0, 360, self.degree_steps, rev)
            # use IJK

            if asI != 0 or asJ!=0:
                cX = asStartX + asI
                cY = asStartY + asJ
                radius = math.sqrt((asStartX - cX)**2 + (asStartY - cY)**2)

                startAngle = getAngle(asStartX, asStartY, cX, cY, radius)
                endAngle = getAngle(asX, asY, cX, cY, radius)

                sa = startAngle
                ea = endAngle
 
                print("cX=%f cY=%f | X=%f Y=%f | I=%f J=%f | rad=%f | %f deg" % (cX, cX, asStartX, asStartY, asI, asJ, radius, startAngle))
                if clockwise:
                    # sa-=360
                    ea-=360
                    if (asI<>0 and asJ<>0) and asI<asJ:
                            sa-=360
                else:
                    if (asI<>0 and asJ<>0) and asI<asJ:
                            sa-=360
                
                if(ea == sa):   #both the same, do a full circle
                    ea+=360


                print("start: %f, end: %f" % (sa, ea))

                coords=calcRadCoords(cX, cY, radius, sa, ea, self.degree_steps)

                planArc(currentPos, [asX,asY,0,0], [asI, asJ], clockwise)

                # if clockwise :
                #     coords=calcRadCoords(cX, cY, radius, sa, ea, self.degree_steps)
                # else:
                #     coords=calcRadCoords(cX, cY, radius, sa, ea, self.degree_steps)
                

            #####################################################################

            if coords.__sizeof__()>0:
                asOut = ""
                msg="G1 X%f Y%f Z%f\n"%(asStartX,asStartX,asStartZ)+msg 
                self.gcode.respond_info("arc_support: tranlated from:" + msg)
                # asOut = "arc_support: tranlated from '" + msg + "'"  #spew info

                for coord in coords:
                    asOut+= "G1 X%f Y%f" % (coord[0], coord[1])
                    if asE>0:
                        asOut+= " E%f" % ((asE/coords.__sizeof__()))
                    if asF>0:
                        asOut+= " F%f" % (asF)
                    asOut+= "\n"
    
                # throw g1 commands in queue
                # logging.info(asOut)
                self.gcode.run_script_from_command(asOut)
                if self.debug:
                    # logging.info(asOut)
                    f= open("test/g2/arc.gcode","w")
                    f.write("\n;--------------\n"+asOut)
                    f.close()



            else:
                self.gcode.respond_info("could not tranlate from '" + msg + "'")
            


def load_config(config):
    return ArcSupport(config)

################ internal functions ##########################

# def getParams(gcode):
#     list = {}
#     matches = re.finditer(r"([xyijefr])(\d?\.\d+|\-?\d+)", gcode, re.MULTILINE | re.IGNORECASE)
    
#     for matchnum, match in enumerate(matches, start=1):
#         list[match[1].lower()] = float(match[2])
#     return list

def calcRadCoords(x,y,r,startAngle=0, endAngle=360, step=0.0, rev=False):
    coords = []
    rangelist = []
    if startAngle>endAngle:
        step*= -1.0

    for i in frange(startAngle, endAngle+step, step):
        rangelist.append(i)


    if rev and rangelist.__sizeof__>1: 
        rangelist = reversed(rangelist)

    for i in rangelist:
        rad = i * (math.pi / 180)
        tX = x + r * math.cos(rad)
        tY = y + r * math.sin(rad)
        coords.append([tX, tY])
    return coords

def frange(start, stop=None, step=None):
    #Use float number in range() function
    # if stop and step argument is null set start=0.0 and step = 1.0
    if stop == None:
        stop = start + 0.0
        start = 0.0
    if step == None:
        step = 1.0
    while True:
        if step > 0 and start >= stop:
            break
        elif step < 0 and start <= stop:
            break
        yield start # return float number
        start = start + step

def getAngle(x, y,cx, cy, radius):
    rad = math.atan2((y - cy)*-1, (x - cx)*-1)
    angle = angle = rad * (180 / math.pi) + 180

    # rad = math.atan2((y - cy)*1, (x - cx)*1)
    # angle = angle = rad * (180 / math.pi)

    # print("cX=%f cY=%f | X=%f Y=%f | %f deg (%f)" % (cx, cy, x, y, angle, degrees))

    return angle

#####################################################################################################

def planArc(currentPos, targetPos=[0,0,0,0], offset=[0,0], clockwise=False):
    MM_PER_ARC_SEGMENT = 1

    p_axis = 0
    q_axis = 1
    l_axis = 2
    
    X_AXIS = 0
    A_AXIS = 0
    Y_AXIS = 1
    B_AXIS = 1
    Z_AXIS = 2
    C_AXIS = 2
    E_CART = 3

    # Radius vector from center to current location
    r_P = offset[0]*-1  # was -offset[0]
    r_Q = offset[1]*-1  # was -offset[1]

    radius = math.hypot(r_P, r_Q)
    center_P = currentPos[p_axis] - r_P
    center_Q = currentPos[q_axis] - r_Q
    rt_X = targetPos[p_axis] - center_P
    rt_Y = targetPos[q_axis] - center_Q
    linear_travel = targetPos[l_axis] - currentPos[l_axis]
    extruder_travel = targetPos[E_CART] - currentPos[E_CART]

    # CCW angle of rotation between position and target from the circle center. Only one atan2() trig computation required.
    angular_travel = math.atan2(r_P * rt_Y - r_Q * rt_X, r_P * rt_X + r_Q * rt_Y)
    if (angular_travel < 0): angular_travel+= math.radians(360)
    if (clockwise): angular_travel-= math.radians(360)
    


    # Make a circle if the angular rotation is 0 and the target is current position
    if (angular_travel == 0 and currentPos[p_axis] == targetPos[p_axis] and currentPos[q_axis] == targetPos[q_axis]):
        angular_travel = RADIANS(360);


    flat_mm = radius * angular_travel
    mm_of_travel = linear_travel    # todo: needs checking
    if(mm_of_travel == linear_travel):
        math.hypot(flat_mm, linear_travel)
    else:
        math.abs(flat_mm)
    if (mm_of_travel < 0.001):
        return

    segments = FLOOR(mm_of_travel / (MM_PER_ARC_SEGMENT))
    if(segments<1): #NOLESS(segments, 1);
        segments=1

#     /**
#      * Vector rotation by transformation matrix: r is the original vector, r_T is the rotated vector,
#      * and phi is the angle of rotation. Based on the solution approach by Jens Geisler.
#      *     r_T = [cos(phi) -sin(phi);
#      *            sin(phi)  cos(phi)] * r ;
#      *
#      * For arc generation, the center of the circle is the axis of rotation and the radius vector is
#      * defined from the circle center to the initial position. Each line segment is formed by successive
#      * vector rotations. This requires only two cos() and sin() computations to form the rotation
#      * matrix for the duration of the entire arc. Error may accumulate from numerical round-off, since
#      * all double numbers are single precision on the Arduino. (True double precision will not have
#      * round off issues for CNC applications.) Single precision error can accumulate to be greater than
#      * tool precision in some cases. Therefore, arc path correction is implemented.
#      *
#      * Small angle approximation may be used to reduce computation overhead further. This approximation
#      * holds for everything, but very small circles and large MM_PER_ARC_SEGMENT values. In other words,
#      * theta_per_segment would need to be greater than 0.1 rad and N_ARC_CORRECTION would need to be large
#      * to cause an appreciable drift error. N_ARC_CORRECTION~=25 is more than small enough to correct for
#      * numerical drift error. N_ARC_CORRECTION may be on the order a hundred(s) before error becomes an
#      * issue for CNC machines with the single precision Arduino calculations.
#      *
#      * This approximation also allows plan_arc to immediately insert a line segment into the planner
#      * without the initial overhead of computing cos() or sin(). By the time the arc needs to be applied
#      * a correction, the planner should have caught up to the lag caused by the initial plan_arc overhead.
#      * This is important when there are successive arc motions.
#      */
#     // Vector rotation matrix values
    raw = [0,0,0,0]
    theta_per_segment = float(angular_travel / segments)
    linear_per_segment = float(linear_travel / segments)
    extruder_per_segment = float(extruder_travel / segments)
    sin_T = theta_per_segment
    cos_T = float(1 - 0.5 * (theta_per_segment**2)) # Small angle approximation

    # Initialize the linear axis
    raw[l_axis] = currentPos[l_axis];

    # Initialize the extruder axis
    raw[E_CART] = currentPos[E_CART];

#     const float fr_mm_s = MMS_SCALED(feedrate_mm_s);

#     millis_t next_idle_ms = millis() + 200UL;

#     #if HAS_FEEDRATE_SCALING
#       // SCARA needs to scale the feed rate from mm/s to degrees/s
#       const float inv_segment_length = 1.0f / (MM_PER_ARC_SEGMENT),
#                   inverse_secs = inv_segment_length * fr_mm_s;
#       float oldA = planner.position_float[A_AXIS],
#             oldB = planner.position_float[B_AXIS]
#             #if ENABLED(DELTA_FEEDRATE_SCALING)
#               , oldC = planner.position_float[C_AXIS]
#             #endif
#             ;
#     #endif

#     #if N_ARC_CORRECTION > 1
#       int8_t arc_recalc_count = N_ARC_CORRECTION;
#     #endif

#     for (uint16_t i = 1; i < segments; i++) { // Iterate (segments-1) times

#       thermalManager.manage_heater();
#       if (ELAPSED(millis(), next_idle_ms)) {
#         next_idle_ms = millis() + 200UL;
#         idle();
#       }

#       #if N_ARC_CORRECTION > 1
#         if (--arc_recalc_count) {
#           // Apply vector rotation matrix to previous r_P / 1
#           const float r_new_Y = r_P * sin_T + r_Q * cos_T;
#           r_P = r_P * cos_T - r_Q * sin_T;
#           r_Q = r_new_Y;
#         }
#         else
#       #endif
#       {
#         #if N_ARC_CORRECTION > 1
#           arc_recalc_count = N_ARC_CORRECTION;
#         #endif

#         // Arc correction to radius vector. Computed only every N_ARC_CORRECTION increments.
#         // Compute exact location by applying transformation matrix from initial radius vector(=-offset).
#         // To reduce stuttering, the sin and cos could be computed at different times.
#         // For now, compute both at the same time.
#         const float cos_Ti = cos(i * theta_per_segment), sin_Ti = sin(i * theta_per_segment);
#         r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti;
#         r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti;
#       }

#       // Update raw location
#       raw[p_axis] = center_P + r_P;
#       raw[q_axis] = center_Q + r_Q;
#       raw[l_axis] += linear_per_segment;
#       raw[E_CART] += extruder_per_segment;

#       clamp_to_software_endstops(raw);

#       #if HAS_FEEDRATE_SCALING
#         inverse_kinematics(raw);
#         ADJUST_DELTA(raw);
#       #endif

#       #if ENABLED(SCARA_FEEDRATE_SCALING)
#         // For SCARA scale the feed rate from mm/s to degrees/s
#         // i.e., Complete the angular vector in the given time.
#         if (!planner.buffer_segment(delta[A_AXIS], delta[B_AXIS], raw[Z_AXIS], raw[E_CART], HYPOT(delta[A_AXIS] - oldA, delta[B_AXIS] - oldB) * inverse_secs, active_extruder, MM_PER_ARC_SEGMENT))
#           break;
#         oldA = delta[A_AXIS]; oldB = delta[B_AXIS];
#       #elif ENABLED(DELTA_FEEDRATE_SCALING)
#         // For DELTA scale the feed rate from Effector mm/s to Carriage mm/s
#         // i.e., Complete the linear vector in the given time.
#         if (!planner.buffer_segment(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], raw[E_AXIS], SQRT(sq(delta[A_AXIS] - oldA) + sq(delta[B_AXIS] - oldB) + sq(delta[C_AXIS] - oldC)) * inverse_secs, active_extruder, MM_PER_ARC_SEGMENT))
#           break;
#         oldA = delta[A_AXIS]; oldB = delta[B_AXIS]; oldC = delta[C_AXIS];
#       #elif HAS_UBL_AND_CURVES
#         float pos[XYZ] = { raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS] };
#         planner.apply_leveling(pos);
#         if (!planner.buffer_segment(pos[X_AXIS], pos[Y_AXIS], pos[Z_AXIS], raw[E_CART], fr_mm_s, active_extruder, MM_PER_ARC_SEGMENT))
#           break;
#       #else
#         if (!planner.buffer_line_kinematic(raw, fr_mm_s, active_extruder))
#           break;
#       #endif
#     }

#     // Ensure last segment arrives at target location.
#     #if HAS_FEEDRATE_SCALING
#       inverse_kinematics(cart);
#       ADJUST_DELTA(cart);
#     #endif

#     #if ENABLED(SCARA_FEEDRATE_SCALING)
#       const float diff2 = HYPOT2(delta[A_AXIS] - oldA, delta[B_AXIS] - oldB);
#       if (diff2)
#         planner.buffer_segment(delta[A_AXIS], delta[B_AXIS], targetPos[Z_AXIS], targetPos[E_CART], SQRT(diff2) * inverse_secs, active_extruder, MM_PER_ARC_SEGMENT);
#     #elif ENABLED(DELTA_FEEDRATE_SCALING)
#       const float diff2 = sq(delta[A_AXIS] - oldA) + sq(delta[B_AXIS] - oldB) + sq(delta[C_AXIS] - oldC);
#       if (diff2)
#         planner.buffer_segment(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], targetPos[E_CART], SQRT(diff2) * inverse_secs, active_extruder, MM_PER_ARC_SEGMENT);
#     #elif HAS_UBL_AND_CURVES
#       float pos[XYZ] = { targetPos[X_AXIS], targetPos[Y_AXIS], targetPos[Z_AXIS] };
#       planner.apply_leveling(pos);
#       planner.buffer_segment(pos[X_AXIS], pos[Y_AXIS], pos[Z_AXIS], targetPos[E_CART], fr_mm_s, active_extruder, MM_PER_ARC_SEGMENT);
#     #else
#       planner.buffer_line_kinematic(cart, fr_mm_s, active_extruder);
#     #endif

#     COPY(currentPos, cart);
#   } // plan_arc
