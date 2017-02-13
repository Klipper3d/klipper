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
        self.need_motor_enable = self.need_home = True
        self.max_z_velocity = config.getfloat('max_z_velocity', 9999999.9)
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
    def set_max_jerk(self, max_xy_halt_velocity, max_velocity, max_accel):
        for stepper in self.steppers:
            stepper.set_max_jerk(max_xy_halt_velocity, max_accel)
    def build_config(self):
        for stepper in self.steppers:
            stepper.build_config()
        self.set_position([0., 0., 0.])
    def _cartesian_to_actuator(self, coord):
        return [int((math.sqrt(self.arm_length2
                               - (self.towers[i][0] - coord[0])**2
                               - (self.towers[i][1] - coord[1])**2) + coord[2])
                    * self.steppers[i].inv_step_dist + 0.5)
                for i in StepList]
    def _actuator_to_cartesian(self, pos):
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
        pos = self._cartesian_to_actuator(newpos)
        for i in StepList:
            self.steppers[i].mcu_stepper.set_position(pos[i])
        self.limit_xy2 = -1.
    def home(self, homing_state):
        # All axes are homed simultaneously
        homing_state.set_axes([0, 1, 2])
        s = self.steppers[0] # Assume homing parameters same for all steppers
        self.need_home = False
        # Initial homing
        homepos = [0., 0., s.position_endstop, None]
        coord = list(homepos)
        coord[2] = -1.5 * math.sqrt(self.arm_length2-self.max_xy2)
        homing_state.home(list(coord), homepos, self.steppers, s.homing_speed)
        # Retract
        coord[2] = homepos[2] - s.homing_retract_dist
        homing_state.retract(list(coord), s.homing_speed)
        # Home again
        coord[2] -= s.homing_retract_dist
        homing_state.home(list(coord), homepos, self.steppers
                          , s.homing_speed/2.0, second_home=True)
        # Set final homed position
        coord = [(s.mcu_stepper.commanded_position + s.get_homed_offset())
                 * s.step_dist
                 for s in self.steppers]
        homing_state.set_homed_position(self._actuator_to_cartesian(coord))
    def motor_off(self, move_time):
        self.limit_xy2 = -1.
        for stepper in self.steppers:
            stepper.motor_enable(move_time, 0)
        self.need_motor_enable = self.need_home = True
    def _check_motor_enable(self, move_time):
        for i in StepList:
            self.steppers[i].motor_enable(move_time, 1)
        self.need_motor_enable = False
    def query_endstops(self, print_time):
        endstops = [(s, s.query_endstop(print_time)) for s in self.steppers]
        return [(s.name, es.query_endstop_wait()) for s, es in endstops]
    def check_move(self, move):
        end_pos = move.end_pos
        xy2 = end_pos[0]**2 + end_pos[1]**2
        if xy2 <= self.limit_xy2 and not move.axes_d[2]:
            # Normal XY move
            return
        if self.need_home:
            raise homing.EndstopMoveError(end_pos, "Must home first")
        limit_xy2 = self.max_xy2
        if end_pos[2] > self.limit_z:
            limit_xy2 = min(limit_xy2, (self.max_z - end_pos[2])**2)
        if xy2 > limit_xy2 or end_pos[2] < 0. or end_pos[2] > self.max_z:
            raise homing.EndstopMoveError(end_pos)
        if move.axes_d[2]:
            move.limit_speed(self.max_z_velocity, move.accel)
            limit_xy2 = -1.
        self.limit_xy2 = limit_xy2
    def move(self, move_time, move):
        axes_d = move.axes_d
        move_d = movexy_d = move.move_d
        movexy_r = 1.
        movez_r = 0.
        inv_movexy_d = 1. / movexy_d
        if not axes_d[0] and not axes_d[1]:
            movez_r = axes_d[2] * inv_movexy_d
            movexy_d = movexy_r = inv_movexy_d = 0.
        elif axes_d[2]:
            movexy_d = math.sqrt(axes_d[0]**2 + axes_d[1]**2)
            movexy_r = movexy_d * inv_movexy_d
            movez_r = axes_d[2] * inv_movexy_d
            inv_movexy_d = 1. / movexy_d

        if self.need_motor_enable:
            self._check_motor_enable(move_time)

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

            # Calculate accel/cruise/decel portions of move
            reversexy_d = closestxy_d + math.sqrt(closest_height2)*movez_r
            accel_up_d = cruise_up_d = decel_up_d = 0.
            accel_down_d = cruise_down_d = decel_down_d = 0.
            if reversexy_d <= 0.:
                accel_down_d = accel_d
                cruise_down_d = cruise_end_d
                decel_down_d = move_d
            elif reversexy_d >= movexy_d:
                accel_up_d = accel_d
                cruise_up_d = cruise_end_d
                decel_up_d = move_d
            elif reversexy_d < accel_d * movexy_r:
                accel_up_d = reversexy_d * move_d * inv_movexy_d
                accel_down_d = accel_d
                cruise_down_d = cruise_end_d
                decel_down_d = move_d
            elif reversexy_d < cruise_end_d * movexy_r:
                accel_up_d = accel_d
                cruise_up_d = reversexy_d * move_d * inv_movexy_d
                cruise_down_d = cruise_end_d
                decel_down_d = move_d
            else:
                accel_up_d = accel_d
                cruise_up_d = cruise_end_d
                decel_up_d = reversexy_d * move_d * inv_movexy_d
                decel_down_d = move_d

            # Generate steps
            mcu_stepper = self.steppers[i].mcu_stepper
            mcu_time = mcu_stepper.print_to_mcu_time(move_time)
            step_pos = mcu_stepper.commanded_position
            step_dist = self.steppers[i].step_dist
            height = step_pos*step_dist - origz
            if accel_up_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time - accel_time_offset, accel_up_d,
                    accel_offset, accel_multiplier, step_dist,
                    height, closestxy_d, closest_height2, movez_r)
                height += count * step_dist
            if cruise_up_d > 0.:
                count = mcu_stepper.step_delta_const(
                    mcu_time + accel_t, cruise_up_d,
                    -accel_d, inv_cruise_v, step_dist,
                    height, closestxy_d, closest_height2, movez_r)
                height += count * step_dist
            if decel_up_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time + decel_time_offset, decel_up_d,
                    -decel_offset, -accel_multiplier, step_dist,
                    height, closestxy_d, closest_height2, movez_r)
                height += count * step_dist
            if accel_down_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time - accel_time_offset, accel_down_d,
                    accel_offset, accel_multiplier, -step_dist,
                    height, closestxy_d, closest_height2, movez_r)
                height += count * step_dist
            if cruise_down_d > 0.:
                count = mcu_stepper.step_delta_const(
                    mcu_time + accel_t, cruise_down_d,
                    -accel_d, inv_cruise_v, -step_dist,
                    height, closestxy_d, closest_height2, movez_r)
                height += count * step_dist
            if decel_down_d > 0.:
                count = mcu_stepper.step_delta_accel(
                    mcu_time + decel_time_offset, decel_down_d,
                    -decel_offset, -accel_multiplier, -step_dist,
                    height, closestxy_d, closest_height2, movez_r)


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
