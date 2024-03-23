# Mechanicaly conforms a moving bed to the bed with 4 Z steppers
#
# Copyright (C) 2018  Maks Zolin <mzolin@vorondesign.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import probe, z_tilt

# Leveling code for XY rails that are controlled by Z steppers as in:
#
# Z stepper1 ----> O                             O <---- Z stepper2
#                  | * <-- probe1   probe2 --> * |
#                  |                             |
#                  |                             | <--- Y2 rail
#   Y1 rail -----> |                             |
#                  |                             |
#                  |=============================|
#                  |            ^                |
#                  |            |                |
#                  |   X rail --/                |
#                  |                             |
#                  | * <-- probe0   probe3 --> * |
# Z stepper0 ----> O                             O <---- Z stepper3

class QuadBedLevel:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.retry_helper = z_tilt.RetryHelper(config,
            "Possibly Z motor numbering is wrong")
        self.max_adjust = config.getfloat("max_adjust", 4, above=0)
        self.horizontal_move_z = config.getfloat("horizontal_move_z", 5.0)
        self.probe_helper = probe.ProbePointsHelper(config, self.probe_finalize)
        if len(self.probe_helper.probe_points) != 4:
            raise config.error(
                "Need exactly 4 probe points for quad_bed_level")
        self.z_status = z_tilt.ZAdjustStatus(self.printer)
        self.z_helper = z_tilt.ZAdjustHelper(config, 4)
        self.bed_corners = config.getlists('bed_corners', parser=float,
                                              seps=(',', '\n'), count=2)
        if len(self.bed_corners) < 2:
            raise config.error(
                "quad_bed_level requires at least two bed_corners")
        # Register QUAD_BED_LEVEL command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'QUAD_BED_LEVEL', self.cmd_QUAD_BED_LEVEL,
            desc=self.cmd_QUAD_BED_LEVEL_help)
    cmd_QUAD_BED_LEVEL_help = (
        "Bend a bed to the shape of the gantry")
    def cmd_QUAD_BED_LEVEL(self, gcmd):
        self.z_status.reset()
        self.retry_helper.start(gcmd)
        self.probe_helper.start_probe(gcmd)
    def probe_finalize(self, offsets, positions):
        # Mirror our perspective so the adjustments make sense
        # from the perspective of the bed
        z_positions = [self.horizontal_move_z - p[2] for p in positions]
        points_message = "Bed-relative probe points:\n%s\n" % (
            " ".join(["%s: %.6f" % (z_id, z_positions[z_id])
                for z_id in range(len(z_positions))]))
        self.gcode.respond_info(points_message)
        # Calculate slope along X axis between probe point 0 and 3
        ppx0 = [positions[0][0] + offsets[0], z_positions[0]]
        ppx3 = [positions[3][0] + offsets[0], z_positions[3]]
        slope_x_pp03 = self.linefit(ppx0, ppx3)
        # Calculate slope along X axis between probe point 1 and 2
        ppx1 = [positions[1][0] + offsets[0], z_positions[1]]
        ppx2 = [positions[2][0] + offsets[0], z_positions[2]]
        slope_x_pp12 = self.linefit(ppx1, ppx2)
        logging.info("quad_bed_level f1: %s, f2: %s"
                     % (slope_x_pp03, slope_x_pp12))
        # Calculate bed slope along Y axis between stepper 0 and 1
        a1 = [positions[0][1] + offsets[1],
              self.plot(slope_x_pp03, self.bed_corners[0][0])]
        a2 = [positions[1][1] + offsets[1],
              self.plot(slope_x_pp12, self.bed_corners[1][0])]
        slope_y_s01 = self.linefit(a1, a2)
        # Calculate bed slope along Y axis between stepper 2 and 3
        b1 = [positions[3][1] + offsets[1],
              self.plot(slope_x_pp03, self.bed_corners[3][0])]
        b2 = [positions[2][1] + offsets[1],
              self.plot(slope_x_pp12, self.bed_corners[2][0])]
        slope_y_s32 = self.linefit(b1, b2)
        logging.info("quad_bed_level af: %s, bf: %s"
                     % (slope_y_s01, slope_y_s32))
        # Calculate z height of each stepper
        z_height = [0,0,0,0]
        z_height[0] = (1) * self.plot(slope_y_s01, self.bed_corners[0][1])
        z_height[1] = (1) * self.plot(slope_y_s01, self.bed_corners[1][1])
        z_height[2] = (1) * self.plot(slope_y_s32, self.bed_corners[2][1])
        z_height[3] = (1) * self.plot(slope_y_s32, self.bed_corners[3][1])

        ainfo = zip(["z","z1","z2","z3"], z_height[0:4])
        apos = " ".join(["%s: %06f" % (x) for x in ainfo])
        self.gcode.respond_info("Actuator Positions:\n" + apos)

        # take average height
        z_ave = sum(z_height) / len(z_height)
        self.gcode.respond_info("Average: %0.6f" % z_ave)
        # adjust all steppers to the average height
        z_adjust = []
        for z in z_height:
            z_adjust.append(z_ave - z)

        adjustinfo = zip(["z","z1","z2","z3"], z_adjust[0:4])
        adjustpos = " ".join(["%s: %06f" % (x) for x in adjustinfo])

        self.gcode.respond_info("Adjustment positions:\n" + adjustpos)

        adjust_max = max(z_adjust)
        if adjust_max > self.max_adjust:
            raise self.gcode.error("Aborting quad_bed_level"
                                   " required adjustment %0.6f"
                                   " is greater than max_adjust %0.6f"
                                   % (adjust_max, self.max_adjust))

        speed = self.probe_helper.get_lift_speed()
        self.z_helper.adjust_steppers(z_adjust, speed)
        return self.z_status.check_retry_result(
            self.retry_helper.check_retry(z_positions))

    def linefit(self,p1,p2):
        if p1[1] == p2[1]:
            # Straight line
            return 0,p1[1]
        m = (p2[1] - p1[1])/(p2[0] - p1[0])
        b = p1[1] - m * p1[0]
        return m,b
    def plot(self,f,x):
        return f[0]*x + f[1]
    def get_status(self, eventtime):
        return self.z_status.get_status(eventtime)

def load_config(config):
    return QuadBedLevel(config)
