# Code for handling the kinematics of cartesian robots
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper, homing

StepList = (0, 1, 2)

class CartKinematics:
    def __init__(self, toolhead, printer, config):
        self.steppers = [stepper.PrinterHomingStepper(
            printer, config.getsection('stepper_' + n))
                         for n in ['x', 'y', 'z']]
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', max_velocity, above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        self.need_motor_enable = True
        self.limits = [(1.0, -1.0)] * 3
        # Setup stepper max halt velocity
        max_halt_velocity = toolhead.get_max_axis_halt()
        self.steppers[0].set_max_jerk(max_halt_velocity, max_accel)
        self.steppers[1].set_max_jerk(max_halt_velocity, max_accel)
        self.steppers[2].set_max_jerk(
            min(max_halt_velocity, self.max_z_velocity), max_accel)
    def set_position(self, newpos):
        for i in StepList:
            self.steppers[i].mcu_stepper.set_position(newpos[i])
    def home(self, homing_state):
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            s = self.steppers[axis]
            self.limits[axis] = (s.position_min, s.position_max)
            # Determine moves
            if s.homing_positive_dir:
                pos = s.position_endstop - 1.5*(
                    s.position_endstop - s.position_min)
                rpos = s.position_endstop - s.homing_retract_dist
                r2pos = rpos - s.homing_retract_dist
            else:
                pos = s.position_endstop + 1.5*(
                    s.position_max - s.position_endstop)
                rpos = s.position_endstop + s.homing_retract_dist
                r2pos = rpos + s.homing_retract_dist
            # Initial homing
            homing_speed = s.get_homing_speed()
            homepos = [None, None, None, None]
            homepos[axis] = s.position_endstop
            coord = [None, None, None, None]
            coord[axis] = pos
            homing_state.home(list(coord), homepos, [s], homing_speed)
            # Retract
            coord[axis] = rpos
            homing_state.retract(list(coord), homing_speed)
            # Home again
            coord[axis] = r2pos
            homing_state.home(
                list(coord), homepos, [s], homing_speed/2.0, second_home=True)
            # Set final homed position
            coord[axis] = s.position_endstop + s.get_homed_offset()
            homing_state.set_homed_position(coord)
    def query_endstops(self, print_time, query_flags):
        return homing.query_endstops(print_time, query_flags, self.steppers)
    def motor_off(self, print_time):
        self.limits = [(1.0, -1.0)] * 3
        for stepper in self.steppers:
            stepper.motor_enable(print_time, 0)
        self.need_motor_enable = True
    def _check_motor_enable(self, print_time, move):
        need_motor_enable = False
        for i in StepList:
            if move.axes_d[i]:
                self.steppers[i].motor_enable(print_time, 1)
            need_motor_enable |= self.steppers[i].need_motor_enable
        self.need_motor_enable = need_motor_enable
    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in StepList:
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
        for i in StepList:
            axis_d = move.axes_d[i]
            if not axis_d:
                continue
            mcu_stepper = self.steppers[i].mcu_stepper
            move_time = print_time
            start_pos = move.start_pos[i]
            axis_r = abs(axis_d) / move.move_d
            accel = move.accel * axis_r
            cruise_v = move.cruise_v * axis_r

            # Acceleration steps
            if move.accel_r:
                accel_d = move.accel_r * axis_d
                mcu_stepper.step_const(
                    move_time, start_pos, accel_d, move.start_v * axis_r, accel)
                start_pos += accel_d
                move_time += move.accel_t
            # Cruising steps
            if move.cruise_r:
                cruise_d = move.cruise_r * axis_d
                mcu_stepper.step_const(
                    move_time, start_pos, cruise_d, cruise_v, 0.)
                start_pos += cruise_d
                move_time += move.cruise_t
            # Deceleration steps
            if move.decel_r:
                decel_d = move.decel_r * axis_d
                mcu_stepper.step_const(
                    move_time, start_pos, decel_d, cruise_v, -accel)
