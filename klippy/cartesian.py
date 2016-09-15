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
        self.stepper_pos = [0, 0, 0]
    def build_config(self):
        for stepper in self.steppers[:2]:
            stepper.set_max_jerk(0.005 * stepper.max_accel) # XXX
        for stepper in self.steppers:
            stepper.build_config()
    def set_position(self, newpos):
        self.stepper_pos = [int(newpos[i]*self.steppers[i].inv_step_dist + 0.5)
                            for i in StepList]
    def get_max_xy_speed(self):
        max_xy_speed = min(s.max_velocity for s in self.steppers[:2])
        max_xy_accel = min(s.max_accel for s in self.steppers[:2])
        return max_xy_speed, max_xy_accel
    def get_max_speed(self, axes_d, move_d):
        # Calculate max speed and accel for a given move
        velocity_factor = min([self.steppers[i].max_velocity / abs(axes_d[i])
                               for i in StepList if axes_d[i]])
        accel_factor = min([self.steppers[i].max_accel / abs(axes_d[i])
                            for i in StepList if axes_d[i]])
        return velocity_factor * move_d, accel_factor * move_d
    def get_homed_position(self):
        return [s.get_homed_position() for s in self.steppers]
    def home(self, toolhead, axes):
        # Each axis is homed independently and in order
        homing_state = homing.Homing(toolhead, axes)
        for axis in axes:
            s = self.steppers[axis]
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
        for stepper in self.steppers:
            stepper.motor_enable(move_time, 0)
    def move(self, move_time, move):
        inv_accel = 1. / move.accel
        inv_cruise_v = 1. / move.cruise_v
        for i in StepList:
            new_step_pos = int(move.pos[i]*self.steppers[i].inv_step_dist + 0.5)
            steps = new_step_pos - self.stepper_pos[i]
            if not steps:
                continue
            self.stepper_pos[i] = new_step_pos
            sdir = 0
            if steps < 0:
                sdir = 1
                steps = -steps
            mcu_time, so = self.steppers[i].prep_move(move_time, sdir)

            step_dist = move.move_d / steps
            step_offset = 0.5

            # Acceleration steps
            #t = sqrt(2*pos/accel + (start_v/accel)**2) - start_v/accel
            accel_time_offset = move.start_v * inv_accel
            accel_sqrt_offset = accel_time_offset**2
            accel_multiplier = 2.0 * step_dist * inv_accel
            accel_steps = move.accel_r * steps
            step_offset = so.step_sqrt(
                mcu_time - accel_time_offset, accel_steps, step_offset
                , accel_sqrt_offset, accel_multiplier)
            mcu_time += move.accel_t
            # Cruising steps
            #t = pos/cruise_v
            cruise_multiplier = step_dist * inv_cruise_v
            cruise_steps = move.cruise_r * steps
            step_offset = so.step_factor(
                mcu_time, cruise_steps, step_offset, cruise_multiplier)
            mcu_time += move.cruise_t
            # Deceleration steps
            #t = cruise_v/accel - sqrt((cruise_v/accel)**2 - 2*pos/accel)
            decel_time_offset = move.cruise_v * inv_accel
            decel_sqrt_offset = decel_time_offset**2
            decel_steps = move.decel_r * steps
            so.step_sqrt(
                mcu_time + decel_time_offset, decel_steps, step_offset
                , decel_sqrt_offset, -accel_multiplier)
