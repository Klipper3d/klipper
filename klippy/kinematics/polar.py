# Code for handling the kinematics of polar robots
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import stepper, homing

class PolarKinematics:
    def __init__(self, toolhead, config):
        # Setup axis steppers
        stepper_bed = stepper.PrinterStepper(config.getsection('stepper_bed'))
        rail_arm = stepper.PrinterRail(config.getsection('stepper_arm'))
        rail_z = stepper.LookupMultiRail(config.getsection('stepper_z'))
        stepper_bed.setup_itersolve('polar_stepper_alloc', 'a')
        rail_arm.setup_itersolve('polar_stepper_alloc', 'r')
        rail_z.setup_itersolve('cartesian_stepper_alloc', 'z')
        self.rails = [rail_arm, rail_z]
        self.steppers = [stepper_bed] + [ s for r in self.rails
                                          for s in r.get_steppers() ]
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', max_velocity, above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        self.need_motor_enable = True
        self.limit_z = [(1.0, -1.0)]
        self.limit_xy2 = -1.
        # Setup stepper max halt velocity
        max_halt_velocity = toolhead.get_max_axis_halt()
        stepper_bed.set_max_jerk(max_halt_velocity, max_accel)
        rail_arm.set_max_jerk(max_halt_velocity, max_accel)
        rail_z.set_max_jerk(max_halt_velocity, max_accel)
    def get_steppers(self, flags=""):
        if flags == "Z":
            return self.rails[1].get_steppers()
        return list(self.steppers)
    def calc_position(self):
        bed_angle = self.steppers[0].get_commanded_position()
        arm_pos = self.rails[0].get_commanded_position()
        z_pos = self.rails[1].get_commanded_position()
        return [math.cos(bed_angle) * arm_pos, math.sin(bed_angle) * arm_pos,
                z_pos]
    def set_position(self, newpos, homing_axes):
        for s in self.steppers:
            s.set_position(newpos)
        if 2 in homing_axes:
            self.limit_z = self.rails[1].get_range()
        if 0 in homing_axes and 1 in homing_axes:
            self.limit_xy2 = self.rails[0].get_range()[1]**2
    def _home_axis(self, homing_state, axis, rail):
        # Determine movement
        position_min, position_max = rail.get_range()
        hi = rail.get_homing_info()
        homepos = [None, None, None, None]
        homepos[axis] = hi.position_endstop
        if axis == 0:
            homepos[1] = 0.
        forcepos = list(homepos)
        if hi.positive_dir:
            forcepos[axis] -= hi.position_endstop - position_min
        else:
            forcepos[axis] += position_max - hi.position_endstop
        # Perform homing
        homing_state.home_rails([rail], forcepos, homepos)
    def home(self, homing_state):
        # Always home XY together
        homing_axes = homing_state.get_axes()
        home_xy = 0 in homing_axes or 1 in homing_axes
        home_z = 2 in homing_axes
        updated_axes = []
        if home_xy:
            updated_axes = [0, 1]
        if home_z:
            updated_axes.append(2)
        homing_state.set_axes(updated_axes)
        # Do actual homing
        if home_xy:
            self._home_axis(homing_state, 0, self.rails[0])
        if home_z:
            self._home_axis(homing_state, 2, self.rails[1])
    def motor_off(self, print_time):
        self.limit_z = [(1.0, -1.0)]
        self.limit_xy2 = -1.
        for s in self.steppers:
            s.motor_enable(print_time, 0)
        self.need_motor_enable = True
    def _check_motor_enable(self, print_time, move):
        if move.axes_d[0] or move.axes_d[1]:
            self.steppers[0].motor_enable(print_time, 1)
            self.rails[0].motor_enable(print_time, 1)
        if move.axes_d[2]:
            self.rails[1].motor_enable(print_time, 1)
        need_motor_enable = not self.steppers[0].is_motor_enabled()
        for rail in self.rails:
            need_motor_enable |= not rail.is_motor_enabled()
        self.need_motor_enable = need_motor_enable
    def check_move(self, move):
        end_pos = move.end_pos
        xy2 = end_pos[0]**2 + end_pos[1]**2
        if xy2 > self.limit_xy2:
            if self.limit_xy2 < 0.:
                raise homing.EndstopMoveError(end_pos, "Must home axis first")
            raise homing.EndstopMoveError(end_pos)
        if move.axes_d[2]:
            if end_pos[2] < self.limit_z[0] or end_pos[2] > self.limit_z[1]:
                if self.limit_z[0] > self.limit_z[1]:
                    raise homing.EndstopMoveError(
                        end_pos, "Must home axis first")
                raise homing.EndstopMoveError(end_pos)
            # Move with Z - update velocity and accel for slower Z axis
            z_ratio = move.move_d / abs(move.axes_d[2])
            move.limit_speed(
                self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
    def move(self, print_time, move):
        if self.need_motor_enable:
            self._check_motor_enable(print_time, move)
        axes_d = move.axes_d
        cmove = move.cmove
        if axes_d[0] or axes_d[1]:
            self.rails[0].step_itersolve(cmove)
            stepper_bed = self.steppers[0]
            stepper_bed.step_itersolve(cmove)
            # Normalize the stepper_bed angle
            angle = stepper_bed.get_commanded_position()
            if angle < -math.pi:
                stepper_bed.set_commanded_position(angle + 2. * math.pi)
            elif angle > math.pi:
                stepper_bed.set_commanded_position(angle - 2. * math.pi)
        if axes_d[2]:
            self.rails[1].step_itersolve(cmove)

def load_kinematics(toolhead, config):
    return PolarKinematics(toolhead, config)
