# Mechanicaly conforms a moving gantry to the bed with 4 Z steppers
#
# Copyright (C) 2018  Maks Zolin <mzolin@vorondesign.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import probe
import z_tilt
import traceback
from decimal import Decimal


def list_to_decimal(v):
    return [Decimal(str(p)) for p in v]


def list_plus(v1, v2):
    return [a + b for a, b in zip(v1, v2)]


def list_minus(v1, v2):
    return [a - b for a, b in zip(v1, v2)]


def list_cross_product(v1, v2):
    return [v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]]


def get_estimated_z_value(normal_v, base_point_v, xy_for_estimate):
    x = xy_for_estimate[0]
    y = xy_for_estimate[1]
    return base_point_v[2] - ((((x - base_point_v[0]) * normal_v[0]) + ((y - base_point_v[1]) * normal_v[1])) / normal_v[2])


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
        gantry_corners = config.get('gantry_corners').split('\n')
        try:
            gantry_corners = [line.split(',', 1)
                              for line in gantry_corners if line.strip()]
            self.gantry_corners = [(Decimal(zp[0].strip()), Decimal(zp[1].strip()))
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
        # Build plate should be parallel with gantry, so don't flip. make it simple.
        # + is upward.
        # offsets -> offset of probe from nozzle.
        #           all positions should be considered nozzle position based. (for voron2, (0, 25, 0))
        # positions -> 4 probing result height

        # get 4 normal vectors from each probed point.
        # all of direction of normal vector is upward from build plate.
        # Theoretically, 4 normal vector which will be use to get plane equation of build plate should be same.
        # but practically, different.
        # decided to use 4 planes to estimate each corner adjustment.

        # use 'Decimal' data type to reduce floating point arithmetic error
        # set measured values by probe as (x,y,z) format.
        probe_points = []
        for z_idx in range(4):
            ps = list_to_decimal(positions[z_idx])
            os = list_to_decimal(offsets)
            probe_points.append(list_plus(ps, os))

        points_message = "probe points:\n%s\n" % (
            " ".join(["%s: (%s)" % (z_id, ", ".join(['{:.6f}'.format(val) for val in probe_points[z_id]]))
                      for z_id in range(len(probe_points))]))
        self.gcode.respond_info(points_message)

        # get normal vectors. upward. right-hand rule
        # 0 : (v3 - v0) x (v1 - v0)
        # 1 : (v0 - v1) x (v2 - v1)
        # 2 : (v1 - v2) x (v3 - v2)
        # 3 : (v2 - v3) x (v0 - v3)
        normal_vectors = []
        for idx in range(4):
            cur_point = probe_points[idx]
            prev_point = probe_points[idx - 1]
            next_point = probe_points[(idx + 1) % 4]
            f1 = list_minus(prev_point, cur_point)
            f2 = list_minus(next_point, cur_point)
            normal_vectors.append(list_cross_product(f1, f2))

        points_message = "normal_vectors of each point:\n%s\n" % (
            " ".join(["%s: (%s)" % (z_id, ", ".join(['{:.6f}'.format(val) for val in normal_vectors[z_id]]))
                      for z_id in range(len(normal_vectors))]))
        self.gcode.respond_info(points_message)

        # center position of belts.
        belt_min_x = self.gantry_corners[0][0]
        belt_max_x = self.gantry_corners[1][0]
        belt_min_y = self.gantry_corners[0][1]
        belt_max_y = self.gantry_corners[1][1]
        belt_points = [[belt_min_x, belt_min_y],
                       [belt_min_x, belt_max_y],
                       [belt_max_x, belt_max_y],
                       [belt_max_x, belt_min_y]]
        z_corner_heights = [get_estimated_z_value(normal_vectors[idx], probe_points[idx], bt)
                            for idx, bt in enumerate(belt_points)]
        points_message = "corner_heights of each point:\n%s\n" % (
            " ".join(["%s: %.6f" % (z_id, z_corner_heights[z_id])
                      for z_id in range(len(z_corner_heights))]))
        self.gcode.respond_info(points_message)

        z_ave = sum(z_corner_heights) / len(z_corner_heights)
        # higher than average height means gantry came down less, so have to lift up the corner
        # (adjust value should be +)
        z_adjust = [z - z_ave for z in z_corner_heights]

        points_message = "z_adjust:\n%s\n" % (
            " ".join(["%s: %.6f" % (z_id, z_adjust[z_id])
                      for z_id in range(len(z_adjust))]))
        self.gcode.respond_info(points_message)

        adjust_max = max([abs(v) for v in z_adjust])
        if adjust_max > self.max_adjust:
            self.gcode.respond_error(
                "Aborting quad_gantry_level " +
                "required adjustment %0.6f " % (adjust_max) +
                "is greater than max_adjust %0.6f" % (self.max_adjust))
            return

        self.adjust_z_steppers(z_adjust)
        return self.retry_helper.check_retry([p[2] for p in probe_points])

    # don't merge with z_tilt.ZAdjustHelper.adjust_steppers until logic is same.
    def adjust_z_steppers(self, adjust_heights):
        tool_head = self.printer.lookup_object('toolhead')
        kin = tool_head.get_kinematics()
        z_steppers = kin.get_steppers('Z')
        current_position = tool_head.get_position()
        # Report on movements
        step_strs = ["%s = %.6f" % (s.get_name(), float(a))
                     for s, a in zip(z_steppers, adjust_heights)]
        msg = "Making the following Z adjustments:\n%s" % ("\n".join(step_strs))
        self.gcode.respond_info(msg)

        # Move each z stepper (sorted from lowest to highest) until they match
        positions = [(float(a), s) for a, s in zip(adjust_heights, z_steppers)]
        speed = self.probe_helper.get_lift_speed()

        try:
            for v in positions:
                tool_head.flush_step_generation()
                for s in z_steppers:
                    s.set_trapq(None)
                stepper_offset, stepper = v
                stepper.set_trapq(tool_head.get_trapq())
                new_pos = current_position
                new_pos[2] = new_pos[2] + stepper_offset
                tool_head.move(new_pos, speed)
                tool_head.set_position(current_position)
        except Exception as e:
            self.gcode.respond_info(str(e))
            self.gcode.respond_info(traceback.format_exc())
            logging.exception("ZAdjustHelper adjust_steppers")
            raise
        finally:
            tool_head.flush_step_generation()
            for s in z_steppers:
                s.set_trapq(tool_head.get_trapq())
            tool_head.set_position(current_position)
            self.gcode.reset_last_position()


def load_config(config):
    return QuadGantryLevel(config)