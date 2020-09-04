# Code for handling the kinematics of rotary delta robots
#
# Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper, mathutil, chelper

class RotaryDeltaKinematics:
    def __init__(self, toolhead, config):
        # Setup tower rails
        stepper_configs = [config.getsection('stepper_' + a) for a in 'abc']
        rail_a = stepper.PrinterRail(
            stepper_configs[0], need_position_minmax=False,
            units_in_radians=True)
        a_endstop = rail_a.get_homing_info().position_endstop
        rail_b = stepper.PrinterRail(
            stepper_configs[1], need_position_minmax=False,
            default_position_endstop=a_endstop, units_in_radians=True)
        rail_c = stepper.PrinterRail(
            stepper_configs[2], need_position_minmax=False,
            default_position_endstop=a_endstop, units_in_radians=True)
        self.rails = [rail_a, rail_b, rail_c]
        config.get_printer().register_event_handler("stepper_enable:motor_off",
                                                    self._motor_off)
        # Setup stepper max halt velocity
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        for rail in self.rails:
            rail.set_max_jerk(9999999.9, 9999999.9)
        # Read config
        shoulder_radius = config.getfloat('shoulder_radius', above=0.)
        shoulder_height = config.getfloat('shoulder_height', above=0.)
        a_upper_arm = stepper_configs[0].getfloat('upper_arm_length', above=0.)
        upper_arms = [
            sconfig.getfloat('upper_arm_length', a_upper_arm, above=0.)
            for sconfig in stepper_configs]
        a_lower_arm = stepper_configs[0].getfloat('lower_arm_length', above=0.)
        lower_arms = [
            sconfig.getfloat('lower_arm_length', a_lower_arm, above=0.)
            for sconfig in stepper_configs]
        angles = [sconfig.getfloat('angle', angle)
                  for sconfig, angle in zip(stepper_configs, [30., 150., 270.])]
        # Setup rotary delta calibration helper
        endstops = [rail.get_homing_info().position_endstop
                    for rail in self.rails]
        stepdists = [rail.get_steppers()[0].get_step_dist()
                     for rail in self.rails]
        self.calibration = RotaryDeltaCalibration(
            shoulder_radius, shoulder_height, angles, upper_arms, lower_arms,
            endstops, stepdists)
        # Setup iterative solver
        for r, a, ua, la in zip(self.rails, angles, upper_arms, lower_arms):
            r.setup_itersolve('rotary_delta_stepper_alloc',
                              shoulder_radius, shoulder_height,
                              math.radians(a), ua, la)
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        # Setup boundary checks
        self.need_home = True
        self.limit_xy2 = -1.
        eangles = [r.calc_position_from_coord([0., 0., ep])
                   for r, ep in zip(self.rails, endstops)]
        self.home_position = tuple(
            self.calibration.actuator_to_cartesian(eangles))
        self.max_z = min(endstops)
        self.min_z = config.getfloat('minimum_z_position', 0, maxval=self.max_z)
        min_ua = min([shoulder_radius + ua for ua in upper_arms])
        min_la = min([la - shoulder_radius for la in lower_arms])
        self.max_xy2 = min(min_ua, min_la)**2
        arm_z = [self.calibration.elbow_coord(i, ea)[2]
                 for i, ea in enumerate(eangles)]
        self.limit_z = min([az - la for az, la in zip(arm_z, lower_arms)])
        logging.info(
            "Delta max build height %.2fmm (radius tapered above %.2fmm)"
            % (self.max_z, self.limit_z))
        self.set_position([0., 0., 0.], ())
    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_tag_position(self):
        spos = [rail.get_tag_position() for rail in self.rails]
        return self.calibration.actuator_to_cartesian(spos)
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
        #min_angles = [-.5 * math.pi] * 3
        #forcepos[2] = self.calibration.actuator_to_cartesian(min_angles)[2]
        forcepos[2] = -1.
        homing_state.home_rails(self.rails, forcepos, self.home_position)
    def _motor_off(self, print_time):
        self.limit_xy2 = -1.
        self.need_home = True
    def check_move(self, move):
        end_pos = move.end_pos
        end_xy2 = end_pos[0]**2 + end_pos[1]**2
        if end_xy2 <= self.limit_xy2 and not move.axes_d[2]:
            # Normal XY move
            return
        if self.need_home:
            raise move.move_error("Must home first")
        end_z = end_pos[2]
        limit_xy2 = self.max_xy2
        if end_z > self.limit_z:
            limit_xy2 = min(limit_xy2, (self.max_z - end_z)**2)
        if end_xy2 > limit_xy2 or end_z > self.max_z or end_z < self.min_z:
            # Move out of range - verify not a homing move
            if (end_pos[:2] != self.home_position[:2]
                or end_z < self.min_z or end_z > self.home_position[2]):
                raise move.move_error()
            limit_xy2 = -1.
        if move.axes_d[2]:
            move.limit_speed(self.max_z_velocity, move.accel)
            limit_xy2 = -1.
        self.limit_xy2 = limit_xy2
    def get_status(self, eventtime):
        return {'homed_axes': '' if self.need_home else 'XYZ'}
    def get_calibration(self):
        return self.calibration

# Rotary delta parameter calibration for DELTA_CALIBRATE tool
class RotaryDeltaCalibration:
    def __init__(self, shoulder_radius, shoulder_height, angles,
                 upper_arms, lower_arms, endstops, stepdists):
        self.shoulder_radius = shoulder_radius
        self.shoulder_height = shoulder_height
        self.angles = angles
        self.upper_arms = upper_arms
        self.lower_arms = lower_arms
        self.endstops = endstops
        self.stepdists = stepdists
        # Calculate the absolute angle of each endstop
        ffi_main, self.ffi_lib = chelper.get_ffi()
        self.sks = [ffi_main.gc(self.ffi_lib.rotary_delta_stepper_alloc(
            shoulder_radius, shoulder_height, math.radians(a), ua, la),
                                self.ffi_lib.free)
                    for a, ua, la in zip(angles, upper_arms, lower_arms)]
        self.abs_endstops = [
            self.ffi_lib.itersolve_calc_position_from_coord(sk, 0., 0., es)
            for sk, es in zip(self.sks, endstops)]
    def coordinate_descent_params(self, is_extended):
        # Determine adjustment parameters (for use with coordinate_descent)
        adj_params = ('shoulder_height', 'endstop_a', 'endstop_b', 'endstop_c')
        if is_extended:
            adj_params += ('shoulder_radius', 'angle_a', 'angle_b')
        params = { 'shoulder_radius': self.shoulder_radius,
                   'shoulder_height': self.shoulder_height }
        for i, axis in enumerate('abc'):
            params['angle_'+axis] = self.angles[i]
            params['upper_arm_'+axis] = self.upper_arms[i]
            params['lower_arm_'+axis] = self.lower_arms[i]
            params['endstop_'+axis] = self.endstops[i]
            params['stepdist_'+axis] = self.stepdists[i]
        return adj_params, params
    def new_calibration(self, params):
        # Create a new calibration object from coordinate_descent params
        shoulder_radius = params['shoulder_radius']
        shoulder_height = params['shoulder_height']
        angles = [params['angle_'+a] for a in 'abc']
        upper_arms = [params['upper_arm_'+a] for a in 'abc']
        lower_arms = [params['lower_arm_'+a] for a in 'abc']
        endstops = [params['endstop_'+a] for a in 'abc']
        stepdists = [params['stepdist_'+a] for a in 'abc']
        return RotaryDeltaCalibration(
            shoulder_radius, shoulder_height, angles, upper_arms, lower_arms,
            endstops, stepdists)
    def elbow_coord(self, elbow_id, spos):
        # Calculate elbow position in coordinate system at shoulder joint
        sj_elbow_x = self.upper_arms[elbow_id] * math.cos(spos)
        sj_elbow_y = self.upper_arms[elbow_id] * math.sin(spos)
        # Shift and rotate to main cartesian coordinate system
        angle = math.radians(self.angles[elbow_id])
        x = (sj_elbow_x + self.shoulder_radius) * math.cos(angle)
        y = (sj_elbow_x + self.shoulder_radius) * math.sin(angle)
        z = sj_elbow_y + self.shoulder_height
        return (x, y, z)
    def actuator_to_cartesian(self, spos):
        sphere_coords = [self.elbow_coord(i, sp) for i, sp in enumerate(spos)]
        lower_arm2 = [la**2 for la in self.lower_arms]
        return mathutil.trilateration(sphere_coords, lower_arm2)
    def get_position_from_stable(self, stable_position):
        # Return cartesian coordinates for the given stable_position
        spos = [ea - sp * sd
                for ea, sp, sd in zip(self.abs_endstops, stable_position,
                                      self.stepdists)]
        return self.actuator_to_cartesian(spos)
    def calc_stable_position(self, coord):
        # Return a stable_position from a cartesian coordinate
        pos = [ self.ffi_lib.itersolve_calc_position_from_coord(
            sk, coord[0], coord[1], coord[2])
                for sk in self.sks ]
        return [(ep - sp) / sd
                for sd, ep, sp in zip(self.stepdists, self.abs_endstops, pos)]
    def save_state(self, configfile):
        # Save the current parameters (for use with SAVE_CONFIG)
        configfile.set('printer', 'shoulder_radius', "%.6f"
                       % (self.shoulder_radius,))
        configfile.set('printer', 'shoulder_height', "%.6f"
                       % (self.shoulder_height,))
        for i, axis in enumerate('abc'):
            configfile.set('stepper_'+axis, 'angle', "%.6f" % (self.angles[i],))
            configfile.set('stepper_'+axis, 'position_endstop',
                           "%.6f" % (self.endstops[i],))
        gcode = configfile.get_printer().lookup_object("gcode")
        gcode.respond_info(
            "stepper_a: position_endstop: %.6f angle: %.6f\n"
            "stepper_b: position_endstop: %.6f angle: %.6f\n"
            "stepper_c: position_endstop: %.6f angle: %.6f\n"
            "shoulder_radius: %.6f shoulder_height: %.6f"
            % (self.endstops[0], self.angles[0],
               self.endstops[1], self.angles[1],
               self.endstops[2], self.angles[2],
               self.shoulder_radius, self.shoulder_height))

def load_kinematics(toolhead, config):
    return RotaryDeltaKinematics(toolhead, config)
