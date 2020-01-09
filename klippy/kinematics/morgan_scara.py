# Code for handling the kinematics of RepRap Morgan Scara robots
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2019  Noah <noah@hack.se>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Usage:
#
#    [stepper_a]
#    ...
#    [stepper_b]
#    ...
#    [stepper_z]
#    ...
#    [printer]
#    kinematics: morgan_scara
#    inner_arm_length: 80.0
#    # Length (in mm) of the active (driven) arms between the base and elbow joints
#    outer_arm_length: 80.0
#    # Length (in mm) of the passive arms between the elbow and effector joints
#    min_base_distance: 40.0
#    # Reject moves with a magnitude (distance in mm) too close to the base joint
#

import math, logging
import stepper, homing, mathutil, chelper

class MorganScaraKinematics:
    def __init__(self, toolhead, config):
        # Read config
        self.inner_arm_length = config.getfloat('inner_arm_length', above=0.)
        self.outer_arm_length = config.getfloat('outer_arm_length', above=0.)
        stepper_configs = [config.getsection('stepper_' + s) for s in 'abz']
        if 1:
            # Left arm
            rail_a = stepper.PrinterStepper(
                stepper_configs[0], units_in_radians=True)
            rail_a.setup_itersolve('morgan_scara_stepper_alloc',
                'a', self.inner_arm_length, self.outer_arm_length)
            # Right arm
            rail_b = stepper.PrinterStepper(
                stepper_configs[1], units_in_radians=True)
            rail_b.setup_itersolve('morgan_scara_stepper_alloc',
                'b', self.inner_arm_length, self.outer_arm_length)
            # Elevator
            rail_z = stepper.LookupMultiRail(stepper_configs[2])
            rail_z.setup_itersolve('cartesian_stepper_alloc', 'z')
            self.steppers = [rail_a, rail_b] + rail_z.get_steppers()
            self.rails = [rail_a]
        else: # TODO:
            # Left arm
            rail_a = stepper.PrinterRail(
                stepper_configs[0], need_position_minmax=False,
                units_in_radians=True)
            rail_a.setup_itersolve('morgan_scara_stepper_alloc',
                'a', self.inner_arm_length, self.outer_arm_length)
            # Right arm
            rail_b = stepper.PrinterRail(
                stepper_configs[1], need_position_minmax=False,
                units_in_radians=True)
            rail_b.setup_itersolve('morgan_scara_stepper_alloc',
                'b', self.inner_arm_length, self.outer_arm_length)
            # Elevator
            rail_z = stepper.LookupMultiRail(stepper_configs[2])
            rail_z.setup_itersolve('cartesian_stepper_alloc', 'z')
            self.steppers = [rail_a, rail_b] + rail_z.get_steppers()
            self.rails = [rail_a, rail_b, rail_z]

        config.get_printer().register_event_handler("stepper_enable:motor_off",
                                                    self._motor_off)
        # Setup stepper max halt velocity
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        for rail in self.rails:
            rail.set_max_jerk(9999999.9, 9999999.9)

        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)

        # Setup boundary checks
        self.need_home = True    # TODO: not yet implemented
        # Reject moves:
        # - too close to the base joint
        # - exceeding the reach of the arms
        # - approaching a dangerous singularity (the "-1" below)
        self.limit_xy_magnitude = (
            config.getfloat('min_base_distance', 10., above=0.),
            self.inner_arm_length + self.outer_arm_length - 1
        )
        #self.max_z = min(endstops)
        #self.min_z = config.getfloat('minimum_z_position', 0, maxval=self.max_z)

        logging.info('SCARA with inner/outer arms {:.1f}/{:.1f}mm'
            .format(self.inner_arm_length, self.outer_arm_length))
        logging.info('SCARA with min/max (x,y) distances {:.1f}/{:.1f}mm'
            .format(self.limit_xy_magnitude[0], self.limit_xy_magnitude[1]))
        self.home_position = (0, self.limit_xy_magnitude[0], 0)
        self.set_position(self.home_position, ())

    def get_steppers(self, flags=""):
        if flags == "Z":
            return self.rails[-1].get_steppers()
        return self.steppers

    def calc_tag_position(self):
        # The pair of arms form a kite (or a rhomb if all have the same length).
        # We need to determine the distance to the end effector and know two
        # sides (arm lengths) of the triangle plus the angle at the base joint.
        spos = [s.get_tag_position() for s in self.steppers]
        a_angle, b_angle, z = spos
        midpoint = (a_angle - b_angle) / 2.0
        distance = self.inner_arm_length*math.cos(midpoint) - math.sqrt(
                     self.outer_arm_length**2 - (self.inner_arm_length**2)*(math.sin(midpoint)**2)
                   )
        x, y = math.cos(a_angle-midpoint)*distance, math.sin(a_angle-midpoint)*distance
        return [x, y, z]

    def set_position(self, newpos, homing_axes):
        for s in self.steppers:
            s.set_position(newpos)
        if tuple(homing_axes) == (0, 1, 2):
            self.need_home = False

    def _motor_off(self, print_time):
        self.need_home = True

    def home(self, homing_state):
        # All axes are homed simultaneously
        homing_state.set_axes([0, 1, 2])
        forcepos = list(self.home_position)
        forcepos[2] = -1.
        homing_state.home_rails(self.rails, forcepos, self.home_position)

    def check_move(self, move):
        end_pos = move.end_pos
        if self.need_home:
            raise homing.EndstopMoveError(end_pos, "Must home (G28) first")

        distance = math.sqrt(end_pos[0]**2 + end_pos[1]**2)
        if distance < self.limit_xy_magnitude[0]:
            raise homing.EndstopMoveError(end_pos, "(x,y) pos too close to base joint")
        elif distance >= self.limit_xy_magnitude[1]:
            raise homing.EndstopMoveError(end_pos, "(x,y) pos exceeds reach of arms")
        elif not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        # Move with Z - update velocity and accel for slower Z axis
        z_ratio = move.move_d / abs(move.axes_d[2])
        move.limit_speed(
            self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)

    def get_status(self):
        return {'homed_axes': '' if self.need_home else 'XYZ'}

def load_kinematics(toolhead, config):
    return MorganScaraKinematics(toolhead, config)
