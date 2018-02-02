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
    def __init__(self, toolhead, printer, config):
        stepper_configs = [config.getsection('stepper_' + n)
                           for n in ['a', 'b', 'c']]
        stepper_a = stepper.PrinterHomingStepper(printer, stepper_configs[0])
        stepper_b = stepper.PrinterHomingStepper(
            printer, stepper_configs[1],
            default_position=stepper_a.position_endstop)
        stepper_c = stepper.PrinterHomingStepper(
            printer, stepper_configs[2],
            default_position=stepper_a.position_endstop)
        self.steppers = [stepper_a, stepper_b, stepper_c]
        self.need_motor_enable = self.need_home = True
        self.radius = radius = config.getfloat('delta_radius', above=0.)
        arm_length_a = stepper_configs[0].getfloat('arm_length', above=radius)
        self.arm_lengths = arm_lengths = [
            sconfig.getfloat('arm_length', arm_length_a, above=radius)
            for sconfig in stepper_configs]
        self.arm2 = [arm**2 for arm in arm_lengths]
        self.endstops = [s.position_endstop + math.sqrt(arm2 - radius**2)
                         for s, arm2 in zip(self.steppers, self.arm2)]
        self.limit_xy2 = -1.
        self.max_z = min([s.position_endstop for s in self.steppers])
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
        for s in self.steppers:
            s.set_max_jerk(max_halt_velocity, self.max_accel)
        # Determine tower locations in cartesian space
        self.angles = [sconfig.getfloat('angle', angle)
                       for sconfig, angle in zip(stepper_configs,
                                                 [210., 330., 90.])]
        self.towers = [(math.cos(math.radians(angle)) * radius,
                        math.sin(math.radians(angle)) * radius)
                       for angle in self.angles]
        # Find the point where an XY move could result in excessive
        # tower movement
        half_min_step_dist = min([s.step_dist for s in self.steppers]) * .5
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
    def get_steppers(self, flags=""):
        return list(self.steppers)
    def _cartesian_to_actuator(self, coord):
        return [math.sqrt(self.arm2[i] - (self.towers[i][0] - coord[0])**2
                          - (self.towers[i][1] - coord[1])**2) + coord[2]
                for i in StepList]
    def _actuator_to_cartesian(self, pos):
        return actuator_to_cartesian(self.towers, self.arm2, pos)
    def get_position(self):
        spos = [s.mcu_stepper.get_commanded_position() for s in self.steppers]
        return self._actuator_to_cartesian(spos)
    def set_position(self, newpos, homing_axes):
        pos = self._cartesian_to_actuator(newpos)
        for i in StepList:
            self.steppers[i].set_position(pos[i])
        self.limit_xy2 = -1.
        if tuple(homing_axes) == StepList:
            self.need_home = False
    def home(self, homing_state):
        # All axes are homed simultaneously
        homing_state.set_axes([0, 1, 2])
        endstops = [es for s in self.steppers for es in s.get_endstops()]
        s = self.steppers[0] # Assume homing speed same for all steppers
        # Initial homing
        homing_speed = min(s.homing_speed, self.max_z_velocity)
        homepos = [0., 0., self.max_z, None]
        coord = list(homepos)
        coord[2] = -1.5 * math.sqrt(max(self.arm2)-self.max_xy2)
        homing_state.home(coord, homepos, endstops, homing_speed)
        # Retract
        coord[2] = homepos[2] - s.homing_retract_dist
        homing_state.retract(coord, homing_speed)
        # Home again
        coord[2] -= s.homing_retract_dist
        homing_state.home(coord, homepos, endstops,
                          homing_speed/2.0, second_home=True)
        # Set final homed position
        spos = [ep + s.get_homed_offset()
                for ep, s in zip(self.endstops, self.steppers)]
        homing_state.set_homed_position(self._actuator_to_cartesian(spos))
    def motor_off(self, print_time):
        self.limit_xy2 = -1.
        for stepper in self.steppers:
            stepper.motor_enable(print_time, 0)
        self.need_motor_enable = self.need_home = True
    def _check_motor_enable(self, print_time):
        for i in StepList:
            self.steppers[i].motor_enable(print_time, 1)
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
            vt_arm_d = math.sqrt(self.arm2[i] - tangentxy_d2)
            vt_startz = origz

            # Generate steps
            step_delta = self.steppers[i].step_delta
            move_time = print_time
            if accel_d:
                step_delta(move_time, accel_d, move.start_v, accel,
                           vt_startz, vt_startxy_d, vt_arm_d, movez_r)
                vt_startz += accel_d * movez_r
                vt_startxy_d -= accel_d * movexy_r
                move_time += move.accel_t
            if cruise_d:
                step_delta(move_time, cruise_d, cruise_v, 0.,
                           vt_startz, vt_startxy_d, vt_arm_d, movez_r)
                vt_startz += cruise_d * movez_r
                vt_startxy_d -= cruise_d * movexy_r
                move_time += move.cruise_t
            if decel_d:
                step_delta(move_time, decel_d, cruise_v, -accel,
                           vt_startz, vt_startxy_d, vt_arm_d, movez_r)
    # Helper functions for DELTA_CALIBRATE script
    def get_stable_position(self):
        return [int((ep - s.mcu_stepper.get_commanded_position())
                    /  s.mcu_stepper.get_step_dist() + .5)
                * s.mcu_stepper.get_step_dist()
                for ep, s in zip(self.endstops, self.steppers)]
    def get_calibrate_params(self):
        return {
            'endstop_a': self.steppers[0].position_endstop,
            'endstop_b': self.steppers[1].position_endstop,
            'endstop_c': self.steppers[2].position_endstop,
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
