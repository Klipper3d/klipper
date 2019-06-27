# Code for handling the kinematics of corexy robots
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import stepper, homing

class CoreXYKinematics:
    def __init__(self, toolhead, config):
        # Setup axis rails
        self.rails = [ stepper.PrinterRail(config.getsection('stepper_x')),
                       stepper.PrinterRail(config.getsection('stepper_y')),
                       stepper.LookupMultiRail(config.getsection('stepper_z')) ]
        self.rails[0].add_to_endstop(self.rails[1].get_endstops()[0][0])
        self.rails[1].add_to_endstop(self.rails[0].get_endstops()[0][0])
        self.rails[0].setup_itersolve('corexy_stepper_alloc', '+')
        self.rails[1].setup_itersolve('corexy_stepper_alloc', '-')
        self.rails[2].setup_itersolve('cartesian_stepper_alloc', 'z')
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', max_velocity, above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        self.need_motor_enable = True
        self.limits = [(1.0, -1.0)] * 3
        # Setup stepper max halt velocity
        max_halt_velocity = toolhead.get_max_axis_halt()
        max_xy_halt_velocity = max_halt_velocity * math.sqrt(2.)
        max_xy_accel = max_accel * math.sqrt(2.)
        self.rails[0].set_max_jerk(max_xy_halt_velocity, max_xy_accel)
        self.rails[1].set_max_jerk(max_xy_halt_velocity, max_xy_accel)
        self.rails[2].set_max_jerk(
            min(max_halt_velocity, self.max_z_velocity), self.max_z_accel)
    def get_steppers(self, flags=""):
        if flags == "Z":
            return self.rails[2].get_steppers()
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_position(self):
        pos = [rail.get_commanded_position() for rail in self.rails]
        return [0.5 * (pos[0] + pos[1]), 0.5 * (pos[0] - pos[1]), pos[2]]
    def set_position(self, newpos, homing_axes):
        for i, rail in enumerate(self.rails):
            rail.set_position(newpos)
            if i in homing_axes:
                self.limits[i] = rail.get_range()
    def home(self, homing_state):
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            rail = self.rails[axis]
            # Determine movement
            position_min, position_max = rail.get_range()
            hi = rail.get_homing_info()
            homepos = [None, None, None, None]
            homepos[axis] = hi.position_endstop
            forcepos = list(homepos)
            if hi.positive_dir:
                forcepos[axis] -= 1.5 * (hi.position_endstop - position_min)
            else:
                forcepos[axis] += 1.5 * (position_max - hi.position_endstop)
            # Perform homing
            homing_state.home_rails([rail], forcepos, homepos)
    def motor_off(self, print_time):
        self.limits = [(1.0, -1.0)] * 3
        for rail in self.rails:
            rail.motor_enable(print_time, 0)
        self.need_motor_enable = True
    def _check_motor_enable(self, print_time, move):
        if move.axes_d[0] or move.axes_d[1]:
            self.rails[0].motor_enable(print_time, 1)
            self.rails[1].motor_enable(print_time, 1)
        if move.axes_d[2]:
            self.rails[2].motor_enable(print_time, 1)
        need_motor_enable = False
        for rail in self.rails:
            need_motor_enable |= not rail.is_motor_enabled()
        self.need_motor_enable = need_motor_enable
    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in (0, 1, 2):
            if (move.axes_d[i]
                and (end_pos[i] < self.limits[i][0]
                     or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise homing.EndstopMoveError(
                        end_pos, "Must home axis first")
                raise homing.EndstopMoveError(end_pos)
    def check_move(self, move):
        limits = self.limits
        xpos, ypos = move.end_pos[:2]
        if (xpos < limits[0][0] or xpos > limits[0][1]
            or ypos < limits[1][0] or ypos > limits[1][1]):
            self._check_endstops(move)
        if not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        # Move with Z - update velocity and accel for slower Z axis
        self._check_endstops(move)
        z_ratio = move.move_d / abs(move.axes_d[2])
        move.limit_speed(
            self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
    def move(self, print_time, move):
        if self.need_motor_enable:
            self._check_motor_enable(print_time, move)
        axes_d = move.axes_d
        cmove = move.cmove
        rail_x, rail_y, rail_z = self.rails
        if axes_d[0] or axes_d[1]:
            rail_x.step_itersolve(cmove)
            rail_y.step_itersolve(cmove)
        if axes_d[2]:
            rail_z.step_itersolve(cmove)

def load_kinematics(toolhead, config):
    return CoreXYKinematics(toolhead, config)
