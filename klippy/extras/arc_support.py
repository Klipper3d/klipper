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
        self.gcode.register_command("G3", self.cmd_G2, desc=self.cmd_G2_help)
            # self.gcode.register_command("G3", self.cmd_G3)
        
        # raise config.error("Arc support test")

    cmd_G2_help = "testing ..."
    # def cmd_Ga(self, params):
    #     self.arc(self, params)
    # def cmd_G3(self, params):
    #     self.GArc(self, params)


    def cmd_G2(self, params):
        msg = params['#original']
    
        # # set vars

        toolhead = self.printer.lookup_object('toolhead')
        pos =  self.printer.lookup_object('toolhead').get_position()
        asStartX = pos[0] 
        asStartY = pos[1]

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
            rev = params['#command'].lower().startswith("g3") # direction, reversed for g3
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

                # calc start angle
                radian = math.atan2(asStartY - cY, asStartX - cX)
                startAngle = radian * (180 / math.pi)
                if rev :
                    startAngle-= float(360)

                # # calc end angle
                radian = math.atan2(asY - cY, asX - cX)
                endAngle = radian * (float(180) / math.pi)

                if rev :
                    coords=calcRadCoords(cX, cY, radius, endAngle, startAngle, self.degree_steps, rev)
                else:
                    coords=calcRadCoords(cX, cY, radius, startAngle, endAngle, self.degree_steps, rev)
                

            #####################################################################

            if coords.__sizeof__()>0:
                asOut = ""
                self.gcode.respond_info("arc_support: tranlated from '" + msg + "'")
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
    if startAngle>endAngle:
        step*= -1.0
    ra = frange(startAngle, endAngle+step, step)

#    if rev:
#       ra = reversed(ra)
        
    for i in ra:
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
