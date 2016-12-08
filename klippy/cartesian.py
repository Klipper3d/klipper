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
        self.steppers = [stepper.PrinterStepper(
            printer, config.getsection('stepper_' + n), n)
                         for n in ['x', 'y', 'z']]
        self.max_z_velocity = config.getfloat('max_z_velocity', 9999999.9)
        self.max_z_accel = config.getfloat('max_z_accel', 9999999.9)
        self.need_motor_enable = True
        self.limits = [(1.0, -1.0)] * 3
    def set_max_jerk(self, max_xy_halt_velocity, max_accel):
        self.steppers[0].set_max_jerk(max_xy_halt_velocity, max_accel)
        self.steppers[1].set_max_jerk(max_xy_halt_velocity, max_accel)
        self.steppers[2].set_max_jerk(0., self.max_z_accel)
    def build_config(self):
        for stepper in self.steppers:
            stepper.build_config()
    def set_position(self, newpos):
        for i in StepList:
            s = self.steppers[i]
            s.mcu_stepper.set_position(int(newpos[i]*s.inv_step_dist + 0.5))
    def _get_homed_position(self, homing_state):
        pos = [None]*3
        for axis in homing_state.get_axes():
            s = self.steppers[axis]
            pos[axis] = s.position_endstop + s.get_homed_offset()*s.step_dist
        return pos
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
            homepos = [None, None, None, None]
            homepos[axis] = s.position_endstop
            coord = [None, None, None, None]
            coord[axis] = pos
            homing_state.plan_home(list(coord), homepos, [s], s.homing_speed)
            # Retract
            coord[axis] = rpos
            homing_state.plan_retract(list(coord), [s], s.homing_speed)
            # Home again
            coord[axis] = r2pos
            homing_state.plan_second_home(
                list(coord), homepos, [s], s.homing_speed/2.0)
        homing_state.plan_calc_position(self._get_homed_position)
    def motor_off(self, move_time):
        self.limits = [(1.0, -1.0)] * 3
        for stepper in self.steppers:
            stepper.motor_enable(move_time, 0)
        self.need_motor_enable = True
    def _check_motor_enable(self, move_time, move):
        need_motor_enable = False
        for i in StepList:
            if move.axes_d[i]:
                self.steppers[i].motor_enable(move_time, 1)
            need_motor_enable |= self.steppers[i].need_motor_enable
        self.need_motor_enable = need_motor_enable
    def query_endstops(self, print_time):
        endstops = [(s, s.query_endstop(print_time)) for s in self.steppers]
        return [(s.name, es.query_endstop_wait()) for s, es in endstops]
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
    def move(self, move_time, move):
        if self.need_motor_enable:
            self._check_motor_enable(move_time, move)
        inv_accel = 1. / move.accel
        inv_cruise_v = 1. / move.cruise_v
        for i in StepList:
            if not move.axes_d[i]:
                continue
            mcu_stepper = self.steppers[i].mcu_stepper
            mcu_time = mcu_stepper.print_to_mcu_time(move_time)
            step_pos = mcu_stepper.commanded_position
            inv_step_dist = self.steppers[i].inv_step_dist
            step_offset = step_pos - move.start_pos[i] * inv_step_dist
            steps = move.axes_d[i] * inv_step_dist
            move_step_d = move.move_d / abs(steps)

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
                step_offset += count - accel_steps
                mcu_time += move.accel_t
            # Cruising steps
            if move.cruise_r:
                #t = pos/cruise_v
                cruise_multiplier = move_step_d * inv_cruise_v
                cruise_steps = move.cruise_r * steps
                count = mcu_stepper.step_factor(
                    mcu_time, cruise_steps, step_offset, cruise_multiplier)
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
