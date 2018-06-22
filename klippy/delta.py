# Code for handling the kinematics of linear delta robots
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, homing, chelper

# Slow moves once the ratio of tower to XY movement exceeds SLOW_RATIO
SLOW_RATIO = 3.

class DeltaKinematics:
    def __init__(self, toolhead, config):
        stepper_configs = [config.getsection('stepper_' + n)
                           for n in ['a', 'b', 'c']]
        rail_a = stepper.PrinterRail(
            stepper_configs[0], need_position_minmax = False)
        a_endstop = rail_a.get_homing_info().position_endstop
        rail_b = stepper.PrinterRail(
            stepper_configs[1], need_position_minmax = False,
            default_position_endstop=a_endstop)
        rail_c = stepper.PrinterRail(
            stepper_configs[2], need_position_minmax = False,
            default_position_endstop=a_endstop)
        self.rails = [rail_a, rail_b, rail_c]
        self.need_motor_enable = self.need_home = True
        self.radius = radius = config.getfloat('delta_radius', above=0.)
        arm_length_a = stepper_configs[0].getfloat('arm_length', above=radius)
        self.arm_lengths = arm_lengths = [
            sconfig.getfloat('arm_length', arm_length_a, above=radius)
            for sconfig in stepper_configs]
        self.arm2 = [arm**2 for arm in arm_lengths]
        self.endstops = [(rail.get_homing_info().position_endstop
                          + math.sqrt(arm2 - radius**2))
                         for rail, arm2 in zip(self.rails, self.arm2)]
        self.limit_xy2 = -1.
        self.max_z = min([rail.get_homing_info().position_endstop
                          for rail in self.rails])
        self.min_z = config.getfloat('minimum_z_position', 0, maxval=self.max_z)
        self.limit_z = min([ep - arm
                            for ep, arm in zip(self.endstops, arm_lengths)])
        logging.info(
            "Delta max build height %.2fmm (radius tapered above %.2fmm)" % (
                self.max_z, self.limit_z))
        # Setup stepper max halt velocity
        self.max_velocity, self.max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', self.max_velocity,
            above=0., maxval=self.max_velocity)
        max_halt_velocity = toolhead.get_max_axis_halt()
        for rail in self.rails:
            rail.set_max_jerk(max_halt_velocity, self.max_accel)
        # Determine tower locations in cartesian space
        self.angles = [sconfig.getfloat('angle', angle)
                       for sconfig, angle in zip(stepper_configs,
                                                 [210., 330., 90.])]
        self.towers = [(math.cos(math.radians(angle)) * radius,
                        math.sin(math.radians(angle)) * radius)
                       for angle in self.angles]
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.cmove = ffi_main.gc(ffi_lib.move_alloc(), ffi_lib.free)
        self.move_fill = ffi_lib.move_fill
        for r, a, t in zip(self.rails, self.arm2, self.towers):
            sk = ffi_main.gc(ffi_lib.delta_stepper_alloc(a, t[0], t[1]),
                             ffi_lib.free)
            r.setup_itersolve(sk)
        # Find the point where an XY move could result in excessive
        # tower movement
        half_min_step_dist = min([r.get_steppers()[0].get_step_dist()
                                  for r in self.rails]) * .5
        min_arm_length = min(arm_lengths)
        def ratio_to_dist(ratio):
            return (ratio * math.sqrt(min_arm_length**2 / (ratio**2 + 1.)
                                      - half_min_step_dist**2)
                    + half_min_step_dist)
        self.slow_xy2 = (ratio_to_dist(SLOW_RATIO) - radius)**2
        self.very_slow_xy2 = (ratio_to_dist(2. * SLOW_RATIO) - radius)**2
        self.max_xy2 = min(radius, min_arm_length - radius,
                           ratio_to_dist(4. * SLOW_RATIO) - radius)**2
        logging.info(
            "Delta max build radius %.2fmm (moves slowed past %.2fmm and %.2fmm)"
            % (math.sqrt(self.max_xy2), math.sqrt(self.slow_xy2),
               math.sqrt(self.very_slow_xy2)))
        self.set_position([0., 0., 0.], ())
    def get_rails(self, flags=""):
        return list(self.rails)
    def _actuator_to_cartesian(self, pos):
        return actuator_to_cartesian(self.towers, self.arm2, pos)
    def calc_position(self):
        spos = [rail.get_commanded_position() for rail in self.rails]
        return self._actuator_to_cartesian(spos)
    def set_position(self, newpos, homing_axes):
        for rail in self.rails:
            rail.set_position(newpos)
        self.limit_xy2 = -1.
        if tuple(homing_axes) == (0, 1, 2):
            self.need_home = False
    def home(self, homing_state):
        # All axes are homed simultaneously
        homing_state.set_axes([0, 1, 2])
        endstops = [es for rail in self.rails for es in rail.get_endstops()]
        # Initial homing - assume homing speed same for all steppers
        hi = self.rails[0].get_homing_info()
        homing_speed = min(hi.speed, self.max_z_velocity)
        homepos = [0., 0., self.max_z, None]
        coord = list(homepos)
        coord[2] = -1.5 * math.sqrt(max(self.arm2)-self.max_xy2)
        homing_state.home(coord, homepos, endstops, homing_speed)
        # Retract
        coord[2] = homepos[2] - hi.retract_dist
        homing_state.retract(coord, homing_speed)
        # Home again
        coord[2] -= hi.retract_dist
        homing_state.home(coord, homepos, endstops,
                          homing_speed/2.0, second_home=True)
        # Set final homed position
        spos = [ep + rail.get_homed_offset()
                for ep, rail in zip(self.endstops, self.rails)]
        homing_state.set_homed_position(self._actuator_to_cartesian(spos))
    def motor_off(self, print_time):
        self.limit_xy2 = -1.
        for rail in self.rails:
            rail.motor_enable(print_time, 0)
        self.need_motor_enable = self.need_home = True
    def _check_motor_enable(self, print_time):
        for rail in self.rails:
            rail.motor_enable(print_time, 1)
        self.need_motor_enable = False
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
        if xy2 > limit_xy2 or end_pos[2] < self.min_z or end_pos[2] > self.max_z:
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
    def move(self, print_time, move):
        if self.need_motor_enable:
            self._check_motor_enable(print_time)
        self.move_fill(
            self.cmove, print_time,
            move.accel_t, move.cruise_t, move.decel_t,
            move.start_pos[0], move.start_pos[1], move.start_pos[2],
            move.axes_d[0], move.axes_d[1], move.axes_d[2],
            move.start_v, move.cruise_v, move.accel)
        for rail in self.rails:
            rail.step_itersolve(self.cmove)
    # Helper functions for DELTA_CALIBRATE script
    def get_stable_position(self):
        steppers = [rail.get_steppers()[0] for rail in self.rails]
        return [int((ep - s.get_commanded_position()) / s.get_step_dist() + .5)
                * s.get_step_dist()
                for ep, s in zip(self.endstops, steppers)]
    def get_calibrate_params(self):
        return {
            'endstop_a': self.rails[0].position_endstop,
            'endstop_b': self.rails[1].position_endstop,
            'endstop_c': self.rails[2].position_endstop,
            'angle_a': self.angles[0], 'angle_b': self.angles[1],
            'angle_c': self.angles[2], 'radius': self.radius,
            'arm_a': self.arm_lengths[0], 'arm_b': self.arm_lengths[1],
            'arm_c': self.arm_lengths[2] }


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

def matrix_add(m1, m2):
    return [m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]]

def matrix_sub(m1, m2):
    return [m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]]

def matrix_mul(m1, s):
    return [m1[0]*s, m1[1]*s, m1[2]*s]

def actuator_to_cartesian(towers, arm2, pos):
    # Find nozzle position using trilateration (see wikipedia)
    carriage1 = list(towers[0]) + [pos[0]]
    carriage2 = list(towers[1]) + [pos[1]]
    carriage3 = list(towers[2]) + [pos[2]]

    s21 = matrix_sub(carriage2, carriage1)
    s31 = matrix_sub(carriage3, carriage1)

    d = math.sqrt(matrix_magsq(s21))
    ex = matrix_mul(s21, 1. / d)
    i = matrix_dot(ex, s31)
    vect_ey = matrix_sub(s31, matrix_mul(ex, i))
    ey = matrix_mul(vect_ey, 1. / math.sqrt(matrix_magsq(vect_ey)))
    ez = matrix_cross(ex, ey)
    j = matrix_dot(ey, s31)

    x = (arm2[0] - arm2[1] + d**2) / (2. * d)
    y = (arm2[0] - arm2[2] - x**2 + (x-i)**2 + j**2) / (2. * j)
    z = -math.sqrt(arm2[0] - x**2 - y**2)

    ex_x = matrix_mul(ex, x)
    ey_y = matrix_mul(ey, y)
    ez_z = matrix_mul(ez, z)
    return matrix_add(carriage1, matrix_add(ex_x, matrix_add(ey_y, ez_z)))

def get_position_from_stable(spos, params):
    angles = [params['angle_a'], params['angle_b'], params['angle_c']]
    radius = params['radius']
    radius2 = radius**2
    towers = [(math.cos(angle) * radius, math.sin(angle) * radius)
              for angle in map(math.radians, angles)]
    arm2 = [a**2 for a in [params['arm_a'], params['arm_b'], params['arm_c']]]
    endstops = [params['endstop_a'], params['endstop_b'], params['endstop_c']]
    pos = [es + math.sqrt(a2 - radius2) - p
           for es, a2, p in zip(endstops, arm2, spos)]
    return actuator_to_cartesian(towers, arm2, pos)
