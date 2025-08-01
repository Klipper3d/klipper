# Code for handling the kinematics of SCARA robots
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper
import math
from . import idex_modes

class SCARAKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()

        # SCARA specific parameters
        self.arm1 = config.getfloat('arm1', above=0.)
        self.arm2 = config.getfloat('arm2', above=0.)
        self.j1 = config.getfloat('j1', -180.)
        self.j2 = config.getfloat('j2', 180.)
        self.g1 = config.getfloat('g1', -180.)
        self.g2 = config.getfloat('g2', 180.)
        # Setup axis rails
        self.dual_carriage_axis = None
        self.dual_carriage_rails = []
        self.rails = [stepper.LookupMultiRail(config.getsection('stepper_' + n))
                      for n in 'xyz']

        for rail, axis in zip(self.rails, 'xyz'):
            rail.setup_itersolve(
            'scara_stepper_alloc',
            axis.encode(),
            self.arm1,
            self.arm2,
            self.j1,
            self.j2,
            self.g1,
            self.g2
            )
        ranges = [r.get_range() for r in self.rails]
        self.axes_min = toolhead.Coord(*[r[0] for r in ranges], e=0.)
        self.axes_max = toolhead.Coord(*[r[1] for r in ranges], e=0.)
        self.dc_module = None
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
            # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.limits = [(1.0, -1.0)] * 3

    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_position(self, stepper_positions):
        # SCARA inverse kinematics
        theta = stepper_positions[self.rails[0].get_name()]
        phi = stepper_positions[self.rails[1].get_name()]
        z = stepper_positions[self.rails[2].get_name()]  # 添加Z轴
        x = self.arm1 * math.cos(math.radians(theta)) + self.arm2 * math.cos(math.radians(theta + phi))
        y = self.arm1 * math.sin(math.radians(theta)) + self.arm2 * math.sin(math.radians(theta + phi))
        # return [x, y, z]
        return [stepper_positions[rail.get_name()] for rail in self.rails]


    def update_limits(self, i, range):
        l, h = self.limits[i]
        if l <= h:
            self.limits[i] = range

    def set_position(self, newpos, homing_axes):
        for i, rail in enumerate(self.rails):
            rail.set_position(newpos)
        for axis_name in homing_axes:
            axis = "xyz".index(axis_name)
            self.limits[axis] = self.rails[axis].get_range()

    def clear_homing_state(self, clear_axes):
        for axis, axis_name in enumerate("xyz"):
            if axis_name in clear_axes:
                self.limits[axis] = (1.0, -1.0)

    def home_axis(self, homing_state, axis, rail):
        position_min, position_max = rail.get_range()
        hi = rail.get_homing_info()
        homepos = [None, None, None, None]
        homepos[axis] = hi.position_endstop
        forcepos = list(homepos)
        if hi.positive_dir:
            forcepos[axis] -= 1.5 * (hi.position_endstop - position_min)
        else:
            forcepos[axis] += 1.5 * (position_max - hi.position_endstop)
        homing_state.home_rails([rail], forcepos, homepos)

    def home(self, homing_state):
        for axis in homing_state.get_axes():
            if axis == 'x' or axis == 'y' or axis == 'z':  # 添加Z轴
                self.home_axis(homing_state, axis, self.rails[0 if axis == 'x' else 1 if axis == 'y' else 2])  # 添加Z轴

    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in (0, 1, 2):
            if (move.axes_d[i]
                    and (end_pos[i] < self.limits[i][0]
                         or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise move.move_error("Must home axis first")
                raise move.move_error()


    def check_move(self, move):
        limits = self.limits
        xpos, ypos, zpos = move.end_pos[:3]
        if (xpos < limits[0][0] or xpos > limits[0][1]

        or ypos < limits[1][0] or ypos > limits[1][1]
        or zpos < limits[2][0] or zpos > limits[2][1]):# 添加Z轴
            self._check_endstops(move)
        if not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        # Move with Z - update velocity and accel for slower Z axis
        self._check_endstops(move)
        z_ratio = move.move_d / abs(move.axes_d[2])
        move.limit_speed(
            self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
    def get_status(self, eventtime):
        axes = [a for a, (l, h) in zip("xyz", self.limits) if l <= h]
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max,
        }

    def _calculate_joint_angles(self, x, y):
        # SCARA forward kinematics
        cos_phi = (x ** 2 + y ** 2 - self.arm1 ** 2 - self.arm2 ** 2) / (
                    2 * self.arm1 * self.arm2)
        phi = math.degrees(math.acos(cos_phi))
        theta = math.degrees(math.atan2(y, x) - math.atan2(self.arm2 * math.sin(math.radians(phi)),
                                                           self.arm1 + self.arm2 * math.cos(
                                                               math.radians(phi))))
        return theta, phi

def load_kinematics(toolhead, config):
    return SCARAKinematics(toolhead, config)

