# Code for handling the kinematics of linear delta robots
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, collections
import stepper, homing, mathutil

# Slow moves once the ratio of tower to XY movement exceeds SLOW_RATIO
SLOW_RATIO = 3.

# Storage helper for delta parameters
DeltaParams = collections.namedtuple('DeltaParams', [
    'radius', 'angles', 'arms', 'endstops', 'stepdists',
    'towers', 'abs_endstops'])

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
        max_halt_velocity = toolhead.get_max_axis_halt()
        for rail in self.rails:
            rail.set_max_jerk(max_halt_velocity, self.max_accel)
        # Read delta parameters
        radius = config.getfloat('delta_radius', above=0.)
        angles = [sconfig.getfloat('angle', angle)
                  for sconfig, angle in zip(stepper_configs, [210., 330., 90.])]
        arm_length_a = stepper_configs[0].getfloat('arm_length')
        arms = [sconfig.getfloat('arm_length', arm_length_a, above=radius)
                for sconfig in stepper_configs]
        endstops = [rail.get_homing_info().position_endstop
                    for rail in self.rails]
        stepdists = [rail.get_steppers()[0].get_step_dist()
                     for rail in self.rails]
        self.delta_params = self.build_delta_params(
            radius, angles, arms, endstops, stepdists)
        # Setup boundary checks
        self.min_z = config.getfloat('minimum_z_position', 0.)
        self.need_motor_enable = self.need_home = True
        self.limit_xy2 = self.max_z = self.limit_z = -1.
        self.slow_xy2 = self.very_slow_xy2 = self.max_xy2 = -1.
        self.set_delta_params(self.delta_params)
    def build_delta_params(self, radius, angles, arms, endstops, stepdists):
        # Calculate the XY cartesian coordinates of the delta towers
        radian_angles = [math.radians(a) for a in angles]
        towers = [(math.cos(a) * radius, math.sin(a) * radius)
                  for a in radian_angles]
        # Calculate the absolute Z height of each tower endstop
        radius2 = radius**2
        abs_endstops = [e + math.sqrt(a**2 - radius2)
                        for e, a in zip(endstops, arms)]
        return DeltaParams(radius, angles, arms, endstops, stepdists,
                           towers, abs_endstops)
    def set_delta_params(self, delta_params):
        # Update delta_params
        self.delta_params = dp = delta_params
        for r, a, t in zip(self.rails, dp.arms, dp.towers):
            r.setup_itersolve('delta_stepper_alloc', a**2, t[0], t[1])
        # Reset boundary checks
        self.need_home = True
        self.max_z = min(dp.endstops)
        self.limit_z = min([ep - a for ep, a in zip(dp.abs_endstops, dp.arms)])
        logging.info(
            "Delta max build height %.2fmm (radius tapered above %.2fmm)" % (
                self.max_z, self.limit_z))
        self.set_position([0., 0., 0.], ())
        # Find the point where an XY move could result in excessive
        # tower movement
        half_min_step_dist = min(dp.stepdists) * .5
        min_arm_length = min(dp.arms)
        def ratio_to_dist(ratio):
            return (ratio * math.sqrt(min_arm_length**2 / (ratio**2 + 1.)
                                      - half_min_step_dist**2)
                    + half_min_step_dist)
        self.slow_xy2 = (ratio_to_dist(SLOW_RATIO) - dp.radius)**2
        self.very_slow_xy2 = (ratio_to_dist(2. * SLOW_RATIO) - dp.radius)**2
        self.max_xy2 = min(dp.radius, min_arm_length - dp.radius,
                           ratio_to_dist(4. * SLOW_RATIO) - dp.radius)**2
        logging.info(
            "Delta max build radius %.2fmm (moves slowed past %.2fmm and %.2fmm)"
            % (math.sqrt(self.max_xy2), math.sqrt(self.slow_xy2),
               math.sqrt(self.very_slow_xy2)))
    def get_delta_params(self):
        return self.delta_params
    def get_steppers(self, flags=""):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def _actuator_to_cartesian(self, spos):
        dp = self.delta_params
        sphere_coords = [(t[0], t[1], sp) for t, sp in zip(dp.towers, spos)]
        return mathutil.trilateration(sphere_coords, [a**2 for a in dp.arms])
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
        max_arm2 = max([a**2 for a in self.delta_params.arms])
        coord[2] = -1.5 * math.sqrt(max_arm2-self.max_xy2)
        homing_state.home(coord, homepos, endstops, homing_speed)
        # Retract
        coord[2] = homepos[2] - hi.retract_dist
        homing_state.retract(coord, homing_speed)
        # Home again
        coord[2] -= hi.retract_dist
        homing_state.home(coord, homepos, endstops,
                          homing_speed/2.0, second_home=True)
        # Set final homed position
        abs_endstops = self.delta_params.abs_endstops
        spos = [ep + rail.get_homed_offset()
                for ep, rail in zip(abs_endstops, self.rails)]
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
        for rail in self.rails:
            rail.step_itersolve(move.cmove)

def load_kinematics(toolhead, config):
    return DeltaKinematics(toolhead, config)
