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


                coords = planArc(currentPos, [asX,asY,0,0], [asI, asJ], clockwise)

                # does not work like i want it to. totaly messes up in some coords
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
                    logging.info(asOut)
                    f= open("test/g2/arc.gcode","w")
                    f.write("\n;--------------\n"+asOut)
                    f.close()



            else:
                self.gcode.respond_info("could not tranlate from '" + msg + "'")
            


def load_config(config):
    return ArcSupport(config)

################ internal functions ##########################
# My sad attempt!
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

    return angle

######################################### Marlin Implementaion ############################################################

def planArc(currentPos, targetPos=[0,0,0,0], offset=[0,0], clockwise=False):
    coords = []
    MM_PER_ARC_SEGMENT = 1
    
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
    # extruder_travel = targetPos[E_CART] - currentPos[E_CART]

    # CCW angle of rotation between position and target from the circle center. Only one atan2() trig computation required.
    angular_travel = math.atan2(r_P * rt_Y - r_Q * rt_X, r_P * rt_X + r_Q * rt_Y)
    if (angular_travel < 0): angular_travel+= math.radians(360)
    if (clockwise): angular_travel-= math.radians(360)
    


    # Make a circle if the angular rotation is 0 and the target is current position
    if (angular_travel == 0 and currentPos[X_AXIS] == targetPos[X_AXIS] and currentPos[Y_AXIS] == targetPos[Y_AXIS]):
        angular_travel = RADIANS(360);


    flat_mm = radius * angular_travel
    mm_of_travel = linear_travel    # todo: needs checking
    if(mm_of_travel == linear_travel):
        mm_of_travel = math.hypot(flat_mm, linear_travel)
    else:
       mm_of_travel = math.abs(flat_mm)


    if (mm_of_travel < 0.001):
        return coords

    segments = int(math.floor(mm_of_travel / (MM_PER_ARC_SEGMENT)))
    if(segments<1): #NOLESS(segments, 1);
        segments=1


#     // Vector rotation matrix values
    raw = [0,0,0,0]
    theta_per_segment = float(angular_travel / segments)
    linear_per_segment = float(linear_travel / segments)
    sin_T = theta_per_segment
    cos_T = float(1 - 0.5 * (theta_per_segment**2)) # Small angle approximation

    # Initialize the linear axis
    raw[Z_AXIS] = currentPos[Z_AXIS];


    for i in range(1,segments):
        cos_Ti = math.cos(i * theta_per_segment)
        sin_Ti = math.sin(i * theta_per_segment)
        r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti
        r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti

      # Update raw location
        raw[X_AXIS] = center_P + r_P
        raw[Y_AXIS] = center_Q + r_Q
        raw[Z_AXIS] += linear_per_segment

        logging.debug(raw)

        coords.append([raw[X_AXIS],  raw[Y_AXIS], raw[Z_AXIS] ])
    

    return coords
