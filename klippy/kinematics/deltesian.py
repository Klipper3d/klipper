# Code for handling the kinematics of deltesian robots
#
# Copyright (C) 2022  Fabrice Gallet <tircown@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import stepper

# Slow moves once the ratio of tower to XY movement exceeds SLOW_RATIO
SLOW_RATIO = 3.

# Minimum angle with the horizontal for the arm to not exceed - in degrees
MIN_ANGLE = 5.

class DeltesianKinematics:
    def __init__(self, toolhead, config):
        self.rails = [None] * 3
        stepper_configs = [config.getsection('stepper_' + s)
                                    for s in ['left', 'right', 'y']]
        self.rails[0] = stepper.LookupRail(
            stepper_configs[0], need_position_minmax = False)
        def_pos_es = self.rails[0].get_homing_info().position_endstop
        self.rails[1] = stepper.LookupRail(
            stepper_configs[1], need_position_minmax = False,
            default_position_endstop = def_pos_es)
        self.rails[2] = stepper.LookupMultiRail(stepper_configs[2])
        arm_x = self.arm_x = [None] * 2
        arm_x[0] = stepper_configs[0].getfloat('arm_x_length', above=0.)
        arm_x[1] = stepper_configs[1].getfloat('arm_x_length', arm_x[0],
                                                above=0.)
        arm = [None] * 2
        arm[0] = stepper_configs[0].getfloat('arm_length', above=arm_x[0])
        arm[1] = stepper_configs[1].getfloat('arm_length', arm[0],
                                              above=arm_x[1])
        arm2 = self.arm2 = [a**2 for a in arm]
        self.rails[0].setup_itersolve(
            'deltesian_stepper_alloc', arm2[0], -arm_x[0])
        self.rails[1].setup_itersolve(
            'deltesian_stepper_alloc', arm2[1], arm_x[1])
        self.rails[2].setup_itersolve('cartesian_stepper_alloc', b'y')
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        self.limits = [(1.0, -1.0)] * 3
        # X axis limits
        min_angle = config.getfloat('min_angle', MIN_ANGLE,
                                    minval=0., maxval=90.)
        cos_angle = math.cos(math.radians(min_angle))
        x_kin_min = math.ceil( -min(arm_x[0], cos_angle * arm[1] - arm_x[1]))
        x_kin_max = math.floor( min(arm_x[1], cos_angle * arm[0] - arm_x[0]))
        x_kin_range = min(x_kin_max - x_kin_min, x_kin_max * 2, -x_kin_min * 2)
        print_width = config.getfloat('print_width', None, minval=0.,
                                      maxval=x_kin_range)
        if print_width:
            self.limits[0] = (-print_width * 0.5, print_width * 0.5)
        else:
            self.limits[0] = (x_kin_min, x_kin_max)
        # Y axis limits
        self.limits[1] = self.rails[2].get_range()
        # Z axis limits
        pmax = [r.get_homing_info().position_endstop for r in self.rails[:2]]
        self._abs_endstop = [p + math.sqrt(a2 - ax**2) for p, a2, ax
                    in zip( pmax, arm2, arm_x )]
        self.home_z = self._actuator_to_cartesian(self._abs_endstop)[1]
        z_max = min([self._pillars_z_max(x) for x in self.limits[0]])
        z_min = config.getfloat('minimum_z_position', 0, maxval=z_max)
        self.limits[2] = (z_min, z_max)
        # Limit the speed/accel if is is at the extreme values of the x axis
        slow_ratio = config.getfloat('slow_ratio', SLOW_RATIO, minval=0.)
        self.slow_x2 = self.very_slow_x2 = None
        if slow_ratio > 0.:
            sr2 = slow_ratio ** 2
            self.slow_x2 = min([math.sqrt((sr2 * a2) / (sr2 + 1))
                        - axl for a2, axl in zip(arm2, arm_x)]) ** 2
            self.very_slow_x2 = min([math.sqrt((2 * sr2 * a2) / (2 * sr2 + 1))
                        - axl for a2, axl in zip(arm2, arm_x)]) ** 2
            logging.info("Deltesian kinematics: moves slowed past %.2fmm"
                         " and %.2fmm"
                         % (math.sqrt(self.slow_x2),
                            math.sqrt(self.very_slow_x2)))
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.axes_min = toolhead.Coord(*[l[0] for l in self.limits], e=0.)
        self.axes_max = toolhead.Coord(*[l[1] for l in self.limits], e=0.)
        self.homed_axis = [False] * 3
        self.set_position([0., 0., 0.], "")
    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def _actuator_to_cartesian(self, sp):
        arm_x, arm2 = self.arm_x, self.arm2
        dx, dz = sum(arm_x), sp[1] - sp[0]
        pivots = math.sqrt(dx**2 + dz**2)
        # Trilateration w/ reference frame along left to right pivots
        xt = (pivots**2 + arm2[0] - arm2[1]) / (2 * pivots)
        zt = math.sqrt(arm2[0] - xt**2)
        # Rotation and translation of the reference frame
        x = xt * dx / pivots + zt * dz / pivots - arm_x[0]
        z = xt * dz / pivots - zt * dx / pivots + sp[0]
        return [x, z]
    def _pillars_z_max(self, x):
        arm_x, arm2 = self.arm_x, self.arm2
        dz = (math.sqrt(arm2[0] - (arm_x[0] + x)**2),
              math.sqrt(arm2[1] - (arm_x[1] - x)**2))
        return min([o - z for o, z in zip(self._abs_endstop, dz)])
    def calc_position(self, stepper_positions):
        sp = [stepper_positions[rail.get_name()] for rail in self.rails]
        x, z = self._actuator_to_cartesian(sp[:2])
        return [x, sp[2], z]
    def set_position(self, newpos, homing_axes):
        for rail in self.rails:
            rail.set_position(newpos)
        for axis_name in homing_axes:
            axis = "xyz".index(axis_name)
            self.homed_axis[axis] = True
    def clear_homing_state(self, clear_axes):
        for axis, axis_name in enumerate("xyz"):
            if axis_name in clear_axes:
                self.homed_axis[axis] = False
    def home(self, homing_state):
        homing_axes = homing_state.get_axes()
        home_xz = 0 in homing_axes or 2 in homing_axes
        home_y = 1 in homing_axes
        forceaxes = ([0, 1, 2] if (home_xz and home_y) else
                     [0, 2] if home_xz else [1] if home_y else [])
        homing_state.set_axes(forceaxes)
        homepos = [None] * 4
        if home_xz:
            homing_state.set_axes([0, 1, 2] if home_y else [0, 2])
            homepos[0], homepos[2] = 0., self.home_z
            forcepos = list(homepos)
            dz2 = [(a2 - ax ** 2) for a2, ax in zip(self.arm2, self.arm_x)]
            forcepos[2] = -1.5 * math.sqrt(max(dz2))
            homing_state.home_rails(self.rails[:2], forcepos, homepos)
        if home_y:
            position_min, position_max = self.rails[2].get_range()
            hi = self.rails[2].get_homing_info()
            homepos[1] = hi.position_endstop
            forcepos = list(homepos)
            if hi.positive_dir:
                forcepos[1] -= 1.5 * (hi.position_endstop - position_min)
            else:
                forcepos[1] += 1.5 * (position_max - hi.position_endstop)
            homing_state.home_rails([self.rails[2]], forcepos, homepos)
    def check_move(self, move):
        limits = list(map(list, self.limits))
        spos, epos = move.start_pos, move.end_pos
        homing_move = False
        if epos[0] == 0. and epos[2] == self.home_z and not move.axes_d[1]:
            # Identify a homing move
            homing_move = True
        elif epos[2] > limits[2][1]:
            # Moves at the very top - adapt limits depending on the X position
            limits[2][1] = self._pillars_z_max(epos[0])
        for i in (i for i, v in enumerate(move.axes_d[:3]) if v):
            if not self.homed_axis[i]:
                raise move.move_error("Must home axis first")
            # Move out of range - verify not a homing move
            if epos[i] < limits[i][0] or epos[i] > limits[i][1]:
                if not homing_move:
                    raise move.move_error()
        if move.axes_d[2]:
            z_ratio = move.move_d / abs(move.axes_d[2])
            move.limit_speed(
                self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
        # Limit the speed/accel if is is at the extreme values of the x axis
        if move.axes_d[0] and self.slow_x2 and self.very_slow_x2:
            move_x2 = max(spos[0] ** 2, epos[0] ** 2)
            if move_x2 > self.very_slow_x2:
                move.limit_speed(self.max_velocity *0.25, self.max_accel *0.25)
            elif move_x2 > self.slow_x2:
                move.limit_speed(self.max_velocity *0.50, self.max_accel *0.50)
    def get_status(self, eventtime):
        axes = [a for a, b in zip("xyz", self.homed_axis) if b]
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max,
        }

def load_kinematics(toolhead, config):
    return DeltesianKinematics(toolhead, config)
