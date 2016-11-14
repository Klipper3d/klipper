# Code for handling the kinematics of cartesian robots
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper, homing

StepList = (0, 1, 2)

class CartKinematics:
    def __init__(self, printer, config):
        steppers = ['stepper_x', 'stepper_y', 'stepper_z']
        self.steppers = [stepper.PrinterStepper(printer, config.getsection(n))
                         for n in steppers]
        self.need_motor_enable = True
        self.limits = [(1.0, -1.0)] * 3
        self.stepper_pos = [0, 0, 0]
    def build_config(self):
        for stepper in self.steppers[:2]:
            stepper.set_max_jerk(0.005 * stepper.max_accel) # XXX
        for stepper in self.steppers:
            stepper.build_config()
    def set_position(self, newpos):
        self.stepper_pos = [int(newpos[i]*self.steppers[i].inv_step_dist + 0.5)
                            for i in StepList]
    def get_max_speed(self):
        max_xy_speed = min(s.max_velocity for s in self.steppers[:2])
        max_xy_accel = min(s.max_accel for s in self.steppers[:2])
        return max_xy_speed, max_xy_accel
    def get_homed_position(self):
        return [s.position_endstop + s.get_homed_offset()*s.step_dist
                for s in self.steppers]
    def home(self, toolhead, axes):
        # Each axis is homed independently and in order
        homing_state = homing.Homing(toolhead, axes)
        for axis in axes:
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
            homepos = [None, None, None, None]
            homepos[axis] = s.position_endstop
            coord = [None, None, None, None]
            coord[axis] = pos
            homing_state.plan_home(list(coord), homepos, [s], s.homing_speed)
            # Retract
            coord[axis] = rpos
            homing_state.plan_move(list(coord), s.homing_speed)
            # Home again
            coord[axis] = r2pos
            homing_state.plan_home(list(coord), homepos, [s], s.homing_speed/2.0)
        return homing_state
    def motor_off(self, move_time):
        self.limits = [(1.0, -1.0)] * 3
        for stepper in self.steppers:
            stepper.motor_enable(move_time, 0)
        self.need_motor_enable = True
    def check_motor_enable(self, move_time, move):
        need_motor_enable = False
        for i in StepList:
            if move.axes_d[i]:
                self.steppers[i].motor_enable(move_time, 1)
            need_motor_enable |= self.steppers[i].need_motor_enable
        self.need_motor_enable = need_motor_enable
    def query_endstops(self, move_time):
        return homing.QueryEndstops(["x", "y", "z"], self.steppers)
    def check_endstops(self, move):
        end_pos = move.end_pos
        for i in StepList:
            if (move.axes_d[i]
                and (end_pos[i] < self.limits[i][0]
                     or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise homing.EndstopError(end_pos, "Must home axis first")
                raise homing.EndstopError(end_pos)
    def check_move(self, move):
        limits = self.limits
        xpos, ypos = move.end_pos[:2]
        if (xpos < limits[0][0] or xpos > limits[0][1]
            or ypos < limits[1][0] or ypos > limits[1][1]):
            self.check_endstops(move)
        if not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        # Move with Z - update velocity and accel for slower Z axis
        self.check_endstops(move)
        axes_d = move.axes_d
        move_d = move.move_d
        velocity_factor = min([self.steppers[i].max_velocity / abs(axes_d[i])
                               for i in StepList if axes_d[i]])
        accel_factor = min([self.steppers[i].max_accel / abs(axes_d[i])
                            for i in StepList if axes_d[i]])
        move.limit_speed(velocity_factor * move_d, accel_factor * move_d)
    def move(self, move_time, move):
        if self.need_motor_enable:
            self.check_motor_enable(move_time, move)
        inv_accel = 1. / move.accel
        inv_cruise_v = 1. / move.cruise_v
        for i in StepList:
            if not move.axes_d[i]:
                continue
            mcu_stepper = self.steppers[i].mcu_stepper
            mcu_time = mcu_stepper.print_to_mcu_time(move_time)
            inv_step_dist = self.steppers[i].inv_step_dist
            steps = move.axes_d[i] * inv_step_dist
            move_step_d = move.move_d / abs(steps)

            step_pos = self.stepper_pos[i]
            step_offset = step_pos - move.start_pos[i] * inv_step_dist

            # Acceleration steps
            accel_multiplier = 2.0 * move_step_d * inv_accel
            if move.accel_r:
                #t = sqrt(2*pos/accel + (start_v/accel)**2) - start_v/accel
                accel_time_offset = move.start_v * inv_accel
                accel_sqrt_offset = accel_time_offset**2
                accel_steps = move.accel_r * steps
                count = mcu_stepper.step_sqrt(
                    mcu_time - accel_time_offset, accel_steps, step_offset
                    , accel_sqrt_offset, accel_multiplier)
                step_pos += count
                step_offset += count - accel_steps
                mcu_time += move.accel_t
            # Cruising steps
            if move.cruise_r:
                #t = pos/cruise_v
                cruise_multiplier = move_step_d * inv_cruise_v
                cruise_steps = move.cruise_r * steps
                count = mcu_stepper.step_factor(
                    mcu_time, cruise_steps, step_offset, cruise_multiplier)
                step_pos += count
                step_offset += count - cruise_steps
                mcu_time += move.cruise_t
            # Deceleration steps
            if move.decel_r:
                #t = cruise_v/accel - sqrt((cruise_v/accel)**2 - 2*pos/accel)
                decel_time_offset = move.cruise_v * inv_accel
                decel_sqrt_offset = decel_time_offset**2
                decel_steps = move.decel_r * steps
                count = mcu_stepper.step_sqrt(
                    mcu_time + decel_time_offset, decel_steps, step_offset
                    , decel_sqrt_offset, -accel_multiplier)
                step_pos += count
            self.stepper_pos[i] = step_pos
