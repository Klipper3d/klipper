# Mechanicaly conforms a moving gantry to the bed with 4 Z steppers
#
# Copyright (C) 2018  Maks Zolin <mzolin@vorondesign.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import probe

class QuadGantryLevel:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.horizontal_move_z = config.getfloat("horizontal_move_z", 5.0)
        self.printer.register_event_handler("klippy:connect",
                                            self.handle_connect)
        self.probe_helper = probe.ProbePointsHelper(config, self.probe_finalize)
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
        self.z_steppers = []
        # Register QUAD_GANTRY_LEVEL command
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command(
            'QUAD_GANTRY_LEVEL', self.cmd_QUAD_GANTRY_LEVEL,
            desc=self.cmd_QUAD_GANTRY_LEVEL_help)
    def handle_connect(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        z_steppers = kin.get_steppers('Z')
        if len(z_steppers) != 4:
            raise self.printer.config_error(
                "quad_gantry_level needs exactly 4 z steppers")
        self.z_steppers = z_steppers
    cmd_QUAD_GANTRY_LEVEL_help = (
        "Conform a moving, twistable gantry to the shape of a stationary bed")
    def cmd_QUAD_GANTRY_LEVEL(self, params):
        self.probe_helper.start_probe(params)
    def probe_finalize(self, offsets, positions):
        # Mirror our perspective so the adjustments make sense
        # from the perspective of the gantry
        z_positions = [self.horizontal_move_z - p[2] for p in positions]
        points_message = "Gantry-relative probe points:\n%s\n" % (
            "\n".join(["z%s = %.6f" % (z_id, z_positions[z_id])
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
        self.gcode.respond_info("Actuator Positions:\n z1: %0.6f\n z2: %0.6f\
            \n z3: %0.6f\n z4: %0.6f\n" % (
                z_height[0],z_height[1],z_height[2],z_height[3]))
        z_ave = sum(z_height) / len(z_height)
        self.gcode.respond_info("Average: %0.6f" % z_ave)
        z_adjust = []
        for z in z_height:
            z_adjust.append(z_ave - z)
        try:
            self.adjust_steppers(z_adjust)
        except:
            logging.exception("quad_gantry_level adjust_steppers")
            for s in self.z_steppers:
                s.set_ignore_move(False)
            raise
    def linefit(self,p1,p2):
        if p1[1] == p2[1]:
            # Straight line
            return 0,p1[1]
        m = (p2[1] - p1[1])/(p2[0] - p1[0])
        b = p1[1] - m * p1[0]
        return m,b
    def plot(self,f,x):
        return f[0]*x + f[1]
    def adjust_steppers(self, z_adjust):
        msg = "Making the following gantry adjustments:\n%s\n" % (
            "\n".join(["%s = %.6f" % (
                self.z_steppers[z_id].get_name(), z_adjust[z_id]
                ) for z_id in range(4)]))
        self.gcode.respond_info(msg)
        toolhead = self.printer.lookup_object('toolhead')
        cur_pos = toolhead.get_position()
        speed = self.probe_helper.get_lift_speed()
        # Disable moves on all Z steppers
        for s in self.z_steppers:
            s.set_ignore_move(True)
        for z_id in range(len(z_adjust)):
            stepper = self.z_steppers[z_id]
            stepper.set_ignore_move(False)
            cur_pos[2] = cur_pos[2] + z_adjust[z_id]
            toolhead.move(cur_pos, speed)
            toolhead.set_position(cur_pos)
            stepper.set_ignore_move(True)
        # Re-enable moves on all Z steppers
        for s in self.z_steppers:
            s.set_ignore_move(False)
        self.gcode.reset_last_position()

def load_config(config):
    return QuadGantryLevel(config)
