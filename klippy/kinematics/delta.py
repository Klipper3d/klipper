# Code for handling the kinematics of linear delta robots
#
# Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, homing, mathutil

# Slow moves once the ratio of tower to XY movement exceeds SLOW_RATIO
SLOW_RATIO = 3.

class DeltaKinematics:
    def __init__(self, toolhead, config):
        # Setup tower rails
        stepper_configs = [config.getsection('stepper_' + a) for a in 'abc']
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
        # Setup stepper max halt velocity
        self.max_velocity, self.max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', self.max_velocity,
            above=0., maxval=self.max_velocity)
        max_halt_velocity = toolhead.get_max_axis_halt() * SLOW_RATIO
        max_halt_accel = self.max_accel * SLOW_RATIO
        for rail in self.rails:
            rail.set_max_jerk(max_halt_velocity, max_halt_accel)
        # Read radius and arm lengths
        self.radius = radius = config.getfloat('delta_radius', above=0.)
        arm_length_a = stepper_configs[0].getfloat('arm_length', above=radius)
        self.arm_lengths = arm_lengths = [
            sconfig.getfloat('arm_length', arm_length_a, above=radius)
            for sconfig in stepper_configs]
        self.arm2 = [arm**2 for arm in arm_lengths]
        self.abs_endstops = [(rail.get_homing_info().position_endstop
                              + math.sqrt(arm2 - radius**2))
                             for rail, arm2 in zip(self.rails, self.arm2)]
        # Determine tower locations in cartesian space
        self.angles = [sconfig.getfloat('angle', angle)
                       for sconfig, angle in zip(stepper_configs,
                                                 [210., 330., 90.])]
        self.towers = [(math.cos(math.radians(angle)) * radius,
                        math.sin(math.radians(angle)) * radius)
                       for angle in self.angles]
        for r, a, t in zip(self.rails, self.arm2, self.towers):
            r.setup_itersolve('delta_stepper_alloc', a, t[0], t[1])
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_move_handler(s.generate_steps)
        # Setup boundary checks
        self.need_home = True
        self.limit_xy2 = -1.
        self.home_position = tuple(
            self._actuator_to_cartesian(self.abs_endstops))
        self.max_z = min([rail.get_homing_info().position_endstop
                          for rail in self.rails])
        self.min_z = config.getfloat('minimum_z_position', 0, maxval=self.max_z)
        self.limit_z = min([ep - arm
                            for ep, arm in zip(self.abs_endstops, arm_lengths)])
        logging.info(
            "Delta max build height %.2fmm (radius tapered above %.2fmm)" % (
                self.max_z, self.limit_z))
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
        logging.info("Delta max build radius %.2fmm (moves slowed past %.2fmm"
                     " and %.2fmm)" % (
                         math.sqrt(self.max_xy2), math.sqrt(self.slow_xy2),
                         math.sqrt(self.very_slow_xy2)))
        self.set_position([0., 0., 0.], ())
    def get_steppers(self, flags=""):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def _actuator_to_cartesian(self, spos):
        sphere_coords = [(t[0], t[1], sp) for t, sp in zip(self.towers, spos)]
        return mathutil.trilateration(sphere_coords, self.arm2)
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
        forcepos = list(self.home_position)
        forcepos[2] = -1.5 * math.sqrt(max(self.arm2)-self.max_xy2)
        homing_state.home_rails(self.rails, forcepos, self.home_position)
    def motor_off(self, print_time):
        self.limit_xy2 = -1.
        for rail in self.rails:
            rail.motor_enable(print_time, 0)
        self.need_home = True
    def check_move(self, move):
        end_pos = move.end_pos
        end_xy2 = end_pos[0]**2 + end_pos[1]**2
        if end_xy2 <= self.limit_xy2 and not move.axes_d[2]:
            # Normal XY move
            return
        if self.need_home:
            raise homing.EndstopMoveError(end_pos, "Must home first")
        end_z = end_pos[2]
        limit_xy2 = self.max_xy2
        if end_z > self.limit_z:
            limit_xy2 = min(limit_xy2, (self.max_z - end_z)**2)
        if end_xy2 > limit_xy2 or end_z > self.max_z or end_z < self.min_z:
            # Move out of range - verify not a homing move
            if (end_pos[:2] != self.home_position[:2]
                or end_z < self.min_z or end_z > self.home_position[2]):
                raise homing.EndstopMoveError(end_pos)
            limit_xy2 = -1.
        if move.axes_d[2]:
            move.limit_speed(self.max_z_velocity, move.accel)
            limit_xy2 = -1.
        # Limit the speed/accel of this move if is is at the extreme
        # end of the build envelope
        extreme_xy2 = max(end_xy2, move.start_pos[0]**2 + move.start_pos[1]**2)
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
        pass
    def get_status(self):
        return {'homed_axes': '' if self.need_home else 'XYZ'}

    # Helper function for DELTA_CALIBRATE script
    def get_calibrate_params(self):
        out = { 'radius': self.radius }
        for i, axis in enumerate('abc'):
            rail = self.rails[i]
            out['angle_'+axis] = self.angles[i]
            out['arm_'+axis] = self.arm_lengths[i]
            out['endstop_'+axis] = rail.get_homing_info().position_endstop
            out['stepdist_'+axis] = rail.get_steppers()[0].get_step_dist()
        return out

def load_kinematics(toolhead, config):
    return DeltaKinematics(toolhead, config)
