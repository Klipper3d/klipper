# Code for handling the kinematics of linear delta robots
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, homing

StepList = (0, 1, 2)

class DeltaKinematics:
    def __init__(self, printer, config):
        self.steppers = [stepper.PrinterStepper(
            printer, config.getsection('stepper_' + n), n)
                         for n in ['a', 'b', 'c']]
        self.need_motor_enable = True
        radius = config.getfloat('delta_radius')
        arm_length = config.getfloat('delta_arm_length')
        self.arm_length2 = arm_length**2
        self.max_xy2 = min(radius, arm_length - radius)**2
        self.limit_xy2 = -1.
        tower_height_at_zeros = math.sqrt(self.arm_length2 - radius**2)
        self.max_z = self.steppers[0].position_max
        self.limit_z = self.max_z - (arm_length - tower_height_at_zeros)
        sin = lambda angle: math.sin(math.radians(angle))
        cos = lambda angle: math.cos(math.radians(angle))
        self.towers = [
            (cos(210.)*radius, sin(210.)*radius),
            (cos(330.)*radius, sin(330.)*radius),
            (cos(90.)*radius, sin(90.)*radius)]
        self.stepper_pos = self.cartesian_to_actuator([0., 0., 0.])
    def build_config(self):
        for stepper in self.steppers:
            stepper.set_max_jerk(0.005 * stepper.max_accel) # XXX
        for stepper in self.steppers:
            stepper.build_config()
    def get_max_speed(self):
        # XXX - this returns conservative values
        max_xy_speed = min(s.max_velocity for s in self.steppers)
        max_xy_accel = min(s.max_accel for s in self.steppers)
        return max_xy_speed, max_xy_accel
    def cartesian_to_actuator(self, coord):
        return [int((math.sqrt(self.arm_length2
                               - (self.towers[i][0] - coord[0])**2
                               - (self.towers[i][1] - coord[1])**2) + coord[2])
                    * self.steppers[i].inv_step_dist + 0.5)
                for i in StepList]
    def actuator_to_cartesian(self, pos):
        # Based on code from Smoothieware
        tower1 = list(self.towers[0]) + [pos[0]]
        tower2 = list(self.towers[1]) + [pos[1]]
        tower3 = list(self.towers[2]) + [pos[2]]

        s12 = matrix_sub(tower1, tower2)
        s23 = matrix_sub(tower2, tower3)
        s13 = matrix_sub(tower1, tower3)

        normal = matrix_cross(s12, s23)

        magsq_s12 = matrix_magsq(s12)
        magsq_s23 = matrix_magsq(s23)
        magsq_s13 = matrix_magsq(s13)

        inv_nmag_sq = 1.0 / matrix_magsq(normal)
        q = 0.5 * inv_nmag_sq

        a = q * magsq_s23 * matrix_dot(s12, s13)
        b = -q * magsq_s13 * matrix_dot(s12, s23) # negate because we use s12 instead of s21
        c = q * magsq_s12 * matrix_dot(s13, s23)

        circumcenter = [tower1[0] * a + tower2[0] * b + tower3[0] * c,
                        tower1[1] * a + tower2[1] * b + tower3[1] * c,
                        tower1[2] * a + tower2[2] * b + tower3[2] * c]

        r_sq = 0.5 * q * magsq_s12 * magsq_s23 * magsq_s13
        dist = math.sqrt(inv_nmag_sq * (self.arm_length2 - r_sq))

        return matrix_sub(circumcenter, matrix_mul(normal, dist))
    def set_position(self, newpos):
        self.stepper_pos = self.cartesian_to_actuator(newpos)
    def get_homed_position(self):
        pos = [(self.stepper_pos[i] + self.steppers[i].get_homed_offset())
               * self.steppers[i].step_dist
               for i in StepList]
        return self.actuator_to_cartesian(pos)
    def home(self, toolhead, axes):
        # All axes are homed simultaneously
        homing_state = homing.Homing(toolhead, [0, 1, 2])
        s = self.steppers[0] # Assume homing parameters same for all steppers
        self.limit_xy2 = self.max_xy2
        # Initial homing
        homepos = [0., 0., s.position_endstop, None]
        coord = list(homepos)
        coord[2] -= 1.5*(s.position_endstop)
        homing_state.plan_home(list(coord), homepos, self.steppers
                               , s.homing_speed)
        # Retract
        coord[2] = homepos[2] - s.homing_retract_dist
        homing_state.plan_move(list(coord), s.homing_speed)
        # Home again
        coord[2] -= s.homing_retract_dist
        homing_state.plan_home(list(coord), homepos, self.steppers
                               , s.homing_speed/2.0)
        return homing_state
    def motor_off(self, move_time):
        self.limit_xy2 = -1.
        for stepper in self.steppers:
            stepper.motor_enable(move_time, 0)
        self.need_motor_enable = True
    def check_motor_enable(self, move_time):
        for i in StepList:
            self.steppers[i].motor_enable(move_time, 1)
        self.need_motor_enable = False
    def query_endstops(self, query_state):
        query_state.set_steppers(self.steppers)
    def check_move(self, move):
        end_pos = move.end_pos
        xy2 = end_pos[0]**2 + end_pos[1]**2
        if xy2 > self.limit_xy2 or end_pos[2] < 0.:
            if self.limit_xy2 < 0.:
                raise homing.EndstopError(end_pos, "Must home first")
            raise homing.EndstopError(end_pos)
        if end_pos[2] > self.limit_z:
            if end_pos[2] > self.max_z or xy2 > (self.max_z - end_pos[2])**2:
                raise homing.EndstopError(end_pos)
    def move_z(self, move_time, move):
        if not move.axes_d[2]:
            return
        if self.need_motor_enable:
            self.check_motor_enable(move_time)
        inv_accel = 1. / move.accel
        inv_cruise_v = 1. / move.cruise_v
        for i in StepList:
            towerx_d = self.towers[i][0] - move.start_pos[0]
            towery_d = self.towers[i][1] - move.start_pos[1]
            tower_d2 = towerx_d**2 + towery_d**2
            height = math.sqrt(self.arm_length2 - tower_d2) + move.start_pos[2]

            mcu_stepper = self.steppers[i].mcu_stepper
            mcu_time = mcu_stepper.print_to_mcu_time(move_time)
            inv_step_dist = self.steppers[i].inv_step_dist
            step_dist = self.steppers[i].step_dist
            steps = move.axes_d[2] * inv_step_dist

            step_pos = self.stepper_pos[i]
            step_offset = step_pos - height * inv_step_dist

            # Acceleration steps
            accel_multiplier = 2.0 * step_dist * inv_accel
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
                cruise_multiplier = step_dist * inv_cruise_v
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
    def move(self, move_time, move):
        axes_d = move.axes_d
        if not axes_d[0] and not axes_d[1]:
            self.move_z(move_time, move)
            return
        if self.need_motor_enable:
            self.check_motor_enable(move_time)
        move_d = move.move_d
        movez_r = 0.
        inv_movexy_d = 1. / move_d
        inv_movexy_r = 1.
        if axes_d[2]:
            movez_r = axes_d[2] * inv_movexy_d
            inv_movexy_d = 1. / math.sqrt(axes_d[0]**2 + axes_d[1]**2)
            inv_movexy_r = move_d * inv_movexy_d

        origx, origy, origz = move.start_pos[:3]

        accel_t = move.accel_t
        cruise_end_t = accel_t + move.cruise_t
        accel_d = move.accel_r * move_d
        cruise_end_d = accel_d + move.cruise_r * move_d

        inv_cruise_v = 1. / move.cruise_v
        inv_accel = 1. / move.accel
        accel_time_offset = move.start_v * inv_accel
        accel_multiplier = 2.0 * inv_accel
        accel_offset = move.start_v**2 * 0.5 * inv_accel
        decel_time_offset = move.cruise_v * inv_accel + cruise_end_t
        decel_offset = move.cruise_v**2 * 0.5 * inv_accel + cruise_end_d

        for i in StepList:
            # Find point on line of movement closest to tower
            towerx_d = self.towers[i][0] - origx
            towery_d = self.towers[i][1] - origy
            closestxy_d = (towerx_d*axes_d[0] + towery_d*axes_d[1])*inv_movexy_d
            tangentxy_d2 = towerx_d**2 + towery_d**2 - closestxy_d**2
            closest_height2 = self.arm_length2 - tangentxy_d2
            closest_height = math.sqrt(closest_height2)
            closest_d = closestxy_d * inv_movexy_r
            closestz = origz + closest_d*movez_r

            # Calculate accel/cruise/decel portions of move
            reverse_d = closest_d + closest_height*movez_r*inv_movexy_r
            accel_up_d = cruise_up_d = decel_up_d = 0.
            accel_down_d = cruise_down_d = decel_down_d = 0.
            if reverse_d <= 0.:
                accel_down_d = accel_d
                cruise_down_d = cruise_end_d
                decel_down_d = move_d
            elif reverse_d >= move_d:
                accel_up_d = accel_d
                cruise_up_d = cruise_end_d
                decel_up_d = move_d
            elif reverse_d < accel_d:
                accel_up_d = reverse_d
                accel_down_d = accel_d
                cruise_down_d = cruise_end_d
                decel_down_d = move_d
            elif reverse_d < cruise_end_d:
                accel_up_d = accel_d
                cruise_up_d = reverse_d
                cruise_down_d = cruise_end_d
                decel_down_d = move_d
            else:
                accel_up_d = accel_d
                cruise_up_d = cruise_end_d
                decel_up_d = reverse_d
                decel_down_d = move_d

            # Generate steps
            inv_step_dist = self.steppers[i].inv_step_dist
            step_dist = self.steppers[i].step_dist
            step_pos = self.stepper_pos[i]
            height = step_pos*step_dist - closestz
            mcu_stepper = self.steppers[i].mcu_stepper
            mcu_time = mcu_stepper.print_to_mcu_time(move_time)
            if accel_up_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time - accel_time_offset, closest_d - accel_up_d,
                    step_dist, closest_d + accel_offset,
                    closest_height2, height, movez_r, accel_multiplier)
                step_pos += count
                height += count * step_dist
            if cruise_up_d > 0.:
                count = mcu_stepper.step_delta_const(
                    mcu_time + accel_t, closest_d - cruise_up_d,
                    step_dist, closest_d - accel_d,
                    closest_height2, height, movez_r, inv_cruise_v)
                step_pos += count
                height += count * step_dist
            if decel_up_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time + decel_time_offset, closest_d - decel_up_d,
                    step_dist, closest_d - decel_offset,
                    closest_height2, height, movez_r, -accel_multiplier)
                step_pos += count
                height += count * step_dist
            if accel_down_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time - accel_time_offset, closest_d - accel_down_d,
                    -step_dist, closest_d + accel_offset,
                    closest_height2, height, movez_r, accel_multiplier)
                step_pos += count
                height += count * step_dist
            if cruise_down_d > 0.:
                count = mcu_stepper.step_delta_const(
                    mcu_time + accel_t, closest_d - cruise_down_d,
                    -step_dist, closest_d - accel_d,
                    closest_height2, height, movez_r, inv_cruise_v)
                step_pos += count
                height += count * step_dist
            if decel_down_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time + decel_time_offset, closest_d - decel_down_d,
                    -step_dist, closest_d - decel_offset,
                    closest_height2, height, movez_r, -accel_multiplier)
                step_pos += count
            self.stepper_pos[i] = step_pos


######################################################################
# Matrix helper functions for 3x1 matrices
######################################################################

def matrix_cross(m1, m2):
    return [m1[1] * m2[2] - m1[2] * m2[1],
            m1[2] * m2[0] - m1[0] * m2[2],
            m1[0] * m2[1] - m1[1] * m2[0]]

def matrix_dot(m1, m2):
    return m1[0] * m2[0] + m1[1] * m2[1] + m1[2] * m2[2]

def matrix_magsq(m1):
    return m1[0]**2 + m1[1]**2 + m1[2]**2

def matrix_sub(m1, m2):
    return [m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]]

def matrix_mul(m1, s):
    return [m1[0]*s, m1[1]*s, m1[2]*s]
