# Mechanicaly conforms a moving gantry to the bed with 4 Z steppers
#
# Copyright (C) 2018  Maks Zolin <mzolin@vorondesign.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import probe, z_tilt

class QuadGantryLevel:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.retry_helper = z_tilt.RetryHelper(config,
            "Possibly Z motor numbering is wrong")
        self.max_adjust = config.getfloat("max_adjust", 4, above=0)
        self.horizontal_move_z = config.getfloat("horizontal_move_z", 5.0)
        self.probe_helper = probe.ProbePointsHelper(config, self.probe_finalize)
        if len(self.probe_helper.probe_points) != 4:
            raise config.error(
                "Need exactly 4 probe points for quad_gantry_level")
        self.z_helper = z_tilt.ZAdjustHelper(config, 4)
        gantry_corners = config.get('gantry_corners').split('\n')
        try:
            gantry_corners = [line.split(',', 1)
                           for line in gantry_corners if line.strip()]
            self.gantry_corners = [(float(zp[0].strip()), float(zp[1].strip()))
                                for zp in gantry_corners]
        except:
            raise config.error("Unable to parse gantry_corners in %s" % (
                config.get_name()))
        if len(self.gantry_corners) < 2:
            raise config.error(
                "quad_gantry_level requires at least two gantry_corners")
        # Register QUAD_GANTRY_LEVEL command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'QUAD_GANTRY_LEVEL', self.cmd_QUAD_GANTRY_LEVEL,
            desc=self.cmd_QUAD_GANTRY_LEVEL_help)
    cmd_QUAD_GANTRY_LEVEL_help = (
        "Conform a moving, twistable gantry to the shape of a stationary bed")
    def cmd_QUAD_GANTRY_LEVEL(self, params):
        self.retry_helper.start(params)
        self.probe_helper.start_probe(params)
    def probe_finalize(self, offsets, positions):
        # Mirror our perspective so the adjustments make sense
        # from the perspective of the gantry
        z_positions = [self.horizontal_move_z - p[2] for p in positions]
        points_message = "Gantry-relative probe points:\n%s\n" % (
            " ".join(["%s: %.6f" % (z_id, z_positions[z_id])
                for z_id in range(len(z_positions))]))
        self.gcode.respond_info(points_message)
        p1 = [positions[0][0] + offsets[0],z_positions[0]]
        p2 = [positions[1][0] + offsets[0],z_positions[1]]
        p3 = [positions[2][0] + offsets[0],z_positions[2]]
        p4 = [positions[3][0] + offsets[0],z_positions[3]]
        f1 = self.linefit(p1,p4)
        f2 = self.linefit(p2,p3)
        logging.info("quad_gantry_level f1: %s, f2: %s" % (f1,f2))
        a1 = [positions[0][1] + offsets[1],
              self.plot(f1,self.gantry_corners[0][0])]
        a2 = [positions[1][1] + offsets[1],
              self.plot(f2,self.gantry_corners[0][0])]
        b1 = [positions[0][1] + offsets[1],
              self.plot(f1,self.gantry_corners[1][0])]
        b2 = [positions[1][1] + offsets[1],
              self.plot(f2,self.gantry_corners[1][0])]
        af = self.linefit(a1,a2)
        bf = self.linefit(b1,b2)
        logging.info("quad_gantry_level af: %s, bf: %s" % (af,bf))
        z_height = [0,0,0,0]
        z_height[0] = self.plot(af,self.gantry_corners[0][1])
        z_height[1] = self.plot(af,self.gantry_corners[1][1])
        z_height[2] = self.plot(bf,self.gantry_corners[1][1])
        z_height[3] = self.plot(bf,self.gantry_corners[0][1])

        ainfo = zip(["z","z1","z2","z3"], z_height[0:4])
        apos = " ".join(["%s: %06f" % (x) for x in ainfo])
        self.gcode.respond_info("Actuator Positions:\n" + apos)

        z_ave = sum(z_height) / len(z_height)
        self.gcode.respond_info("Average: %0.6f" % z_ave)
        z_adjust = []
        for z in z_height:
            z_adjust.append(z_ave - z)

        adjust_max = max(z_adjust)
        if adjust_max > self.max_adjust:
            self.gcode.respond_error(
                "Aborting quad_gantry_level " +
                "required adjustment %0.6f " % ( adjust_max ) +
                "is greater than max_adjust %0.6f" % (self.max_adjust))
            return

        speed = self.probe_helper.get_lift_speed()
        self.z_helper.adjust_steppers(z_adjust, speed)
        return self.retry_helper.check_retry(z_positions)
    def linefit(self,p1,p2):
        if p1[1] == p2[1]:
            # Straight line
            return 0,p1[1]
        m = (p2[1] - p1[1])/(p2[0] - p1[0])
        b = p1[1] - m * p1[0]
        return m,b
    def plot(self,f,x):
        return f[0]*x + f[1]

def load_config(config):
    return QuadGantryLevel(config)
