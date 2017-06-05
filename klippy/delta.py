# Code for handling the kinematics of linear delta robots
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, homing

StepList = (0, 1, 2)

# Slow moves once the ratio of tower to XY movement exceeds SLOW_RATIO
SLOW_RATIO = 3.

class DeltaKinematics:
    def __init__(self, printer, config):
        self.config = config
        self.steppers = [stepper.PrinterStepper(
            printer, config.getsection('stepper_' + n), n)
                         for n in ['a', 'b', 'c']]
        self.need_motor_enable = self.need_home = True
        self.max_velocity = self.max_z_velocity = self.max_accel = 0.
        radius = config.getfloat('delta_radius', above=0.)
        arm_length = config.getfloat('delta_arm_length', above=radius)
        self.arm_length2 = arm_length**2
        self.limit_xy2 = -1.
        tower_height_at_zeros = math.sqrt(self.arm_length2 - radius**2)
        self.max_z = max([s.position_endstop for s in self.steppers])
        self.limit_z = self.max_z - (arm_length - tower_height_at_zeros)
        logging.info(
            "Delta max build height %.2fmm (radius tapered above %.2fmm)" % (
                self.max_z, self.limit_z))
        # Determine tower locations in cartesian space
        angles = [config.getsection('stepper_a').getfloat('angle', 210.),
                  config.getsection('stepper_b').getfloat('angle', 330.),
                  config.getsection('stepper_c').getfloat('angle', 90.)]
        self.towers = [(math.cos(math.radians(angle)) * radius,
                        math.sin(math.radians(angle)) * radius)
                       for angle in angles]
        # Find the point where an XY move could result in excessive
        # tower movement
        half_min_step_dist = min([s.step_dist for s in self.steppers]) * .5
        def ratio_to_dist(ratio):
            return (ratio * math.sqrt(self.arm_length2 / (ratio**2 + 1.)
                                      - half_min_step_dist**2)
                    + half_min_step_dist)
        self.slow_xy2 = (ratio_to_dist(SLOW_RATIO) - radius)**2
        self.very_slow_xy2 = (ratio_to_dist(2. * SLOW_RATIO) - radius)**2
        self.max_xy2 = min(radius, arm_length - radius,
                           ratio_to_dist(4. * SLOW_RATIO) - radius)**2
        logging.info(
            "Delta max build radius %.2fmm (moves slowed past %.2fmm and %.2fmm)"
            % (math.sqrt(self.max_xy2), math.sqrt(self.slow_xy2),
               math.sqrt(self.very_slow_xy2)))
        self.set_position([0., 0., 0.])
    def set_max_jerk(self, max_xy_halt_velocity, max_velocity, max_accel):
        self.max_velocity = max_velocity
        max_z_velocity = self.config.getfloat(
            'max_z_velocity', max_velocity, above=0.)
        self.max_z_velocity = min(max_velocity, max_z_velocity)
        self.max_accel = max_accel
        for stepper in self.steppers:
            stepper.set_max_jerk(max_xy_halt_velocity, max_accel)
    def _cartesian_to_actuator(self, coord):
        return [math.sqrt(self.arm_length2
                          - (self.towers[i][0] - coord[0])**2
                          - (self.towers[i][1] - coord[1])**2) + coord[2]
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
        s = self.steppers[0] # Assume homing speed same for all steppers
        self.need_home = False
        # Initial homing
        homepos = [0., 0., self.max_z, None]
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
        coord = [s.mcu_stepper.get_commanded_position() + s.get_homed_offset()
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
        # Limit the speed/accel of this move if is is at the extreme
        # end of the build envelope
        extreme_xy2 = max(xy2, move.start_pos[0]**2 + move.start_pos[1]**2)
        if extreme_xy2 > self.slow_xy2:
            r = 0.5
            if extreme_xy2 > self.very_slow_xy2:
                r = 0.25
            max_velocity = self.max_velocity
            if move.axes_d[2]:
                max_velocity = self.max_z_velocity
            move.limit_speed(max_velocity * r, self.max_accel * r)
            limit_xy2 = -1.
        self.limit_xy2 = min(limit_xy2, self.slow_xy2)
    def move(self, move_time, move):
        if self.need_motor_enable:
            self._check_motor_enable(move_time)
        axes_d = move.axes_d
        move_d = move.move_d
        movexy_r = 1.
        movez_r = 0.
        inv_movexy_d = 1. / move_d
        if not axes_d[0] and not axes_d[1]:
            # Z only move
            movez_r = axes_d[2] * inv_movexy_d
            movexy_r = inv_movexy_d = 0.
        elif axes_d[2]:
            # XY+Z move
            movexy_d = math.sqrt(axes_d[0]**2 + axes_d[1]**2)
            movexy_r = movexy_d * inv_movexy_d
            movez_r = axes_d[2] * inv_movexy_d
            inv_movexy_d = 1. / movexy_d

        origx, origy, origz = move.start_pos[:3]

        accel = move.accel
        cruise_v = move.cruise_v
        accel_d = move.accel_r * move_d
        cruise_d = move.cruise_r * move_d
        decel_d = move.decel_r * move_d

        for i in StepList:
            # Calculate a virtual tower along the line of movement at
            # the point closest to this stepper's tower.
            towerx_d = self.towers[i][0] - origx
            towery_d = self.towers[i][1] - origy
            vt_startxy_d = (towerx_d*axes_d[0] + towery_d*axes_d[1])*inv_movexy_d
            tangentxy_d2 = towerx_d**2 + towery_d**2 - vt_startxy_d**2
            vt_arm_d = math.sqrt(self.arm_length2 - tangentxy_d2)
            vt_startz = origz

            # Generate steps
            mcu_stepper = self.steppers[i].mcu_stepper
            mcu_time = mcu_stepper.print_to_mcu_time(move_time)
            if accel_d:
                mcu_stepper.step_delta(
                    mcu_time, accel_d, move.start_v, accel,
                    vt_startz, vt_startxy_d, vt_arm_d, movez_r)
                vt_startz += accel_d * movez_r
                vt_startxy_d -= accel_d * movexy_r
                mcu_time += move.accel_t
            if cruise_d:
                mcu_stepper.step_delta(
                    mcu_time, cruise_d, cruise_v, 0.,
                    vt_startz, vt_startxy_d, vt_arm_d, movez_r)
                vt_startz += cruise_d * movez_r
                vt_startxy_d -= cruise_d * movexy_r
                mcu_time += move.cruise_t
            if decel_d:
                mcu_stepper.step_delta(
                    mcu_time, decel_d, cruise_v, -accel,
                    vt_startz, vt_startxy_d, vt_arm_d, movez_r)


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
