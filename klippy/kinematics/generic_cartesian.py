# Code for generic handling the kinematics of cartesian-like printers
#
# Copyright (C) 2024  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import mathutil, stepper
import gcode
import extras.kinematic_stepper as ks
from . import idex_modes

def mat_mul(a, b):
    if len(a[0]) != len(b):
        return None
    res = []
    for i in range(len(a)):
        res.append([])
        for j in range(len(b[0])):
            res[i].append(sum(a[i][k] * b[k][j] for k in range(len(b))))
    return res

def mat_transp(a):
    res = []
    for i in range(len(a[0])):
        res.append([a[j][i] for j in range(len(a))])
    return res

class GenericCartesianKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        self.rails = [stepper.GenericPrinterRail(config.getsection('rail_' + n))
                      for n in 'xyz']
        self.dc_module = dual_carriage_axis = None
        if config.has_section('dual_carriage'):
            dc_config = config.getsection('dual_carriage')
            dual_carriage_axis = dc_config.getchoice('axis', {'x': 0, 'y': 1})
            # setup second dual carriage rail
            self.rails.append(stepper.GenericPrinterRail(dc_config))
        self.steppers = ks.LoadKinematicSteppers(config)
        allowed_carriages = [r.get_name() for r in self.rails] + [None]
        primary_carriage_steppers = []
        dual_carriage_steppers = []
        dc_rails = [None]
        for s in self.steppers:
            carriage = s.get_carriage()
            if carriage not in allowed_carriages:
                raise config.error(
                        "Invalid carriage '%s' for kinematic_stepper '%s'" % (
                            carriage, s.get_name()))
            for i in s.get_active_axes():
                if i == dual_carriage_axis:
                    dc_rails = [self.rails[i], self.rails[3]]
                    if dc_rails[0].get_name() == carriage:
                        s.add_to_rail(i, dc_rails[0])
                        primary_carriage_steppers.append(s)
                    elif dc_rails[1].get_name() == carriage:
                        s.add_to_rail(i, dc_rails[1])
                        dual_carriage_steppers.append(s)
                    else:
                        s.add_to_rail(i, dc_rails[0])
                        s.add_to_rail(i, dc_rails[1])
                else:
                    s.add_to_rail(i, self.rails[i])
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        if dual_carriage_axis is not None:
            if not primary_carriage_steppers:
                raise config.error(
                        "At least one kinematic_stepper must be declared with "
                        "'carriage: %s' when '[dual_carriage]' is used" % (
                            self.rails[dual_carriage_axis].get_name()))
            if not dual_carriage_steppers:
                raise config.error(
                        "At least one kinematic_stepper must be declared with "
                        "'carriage: %s' when '[dual_carriage]' is used" % (
                            self.rails[3].get_name()))
            dc_rail_0 = idex_modes.DualCarriagesRail(
                    self.rails[dual_carriage_axis], primary_carriage_steppers,
                    axis=dual_carriage_axis, active=True)
            dc_rail_1 = idex_modes.DualCarriagesRail(
                    self.rails[3], dual_carriage_steppers,
                    axis=dual_carriage_axis, active=False)
            self.dc_module = idex_modes.DualCarriages(
                    dc_config, dc_rail_0, dc_rail_1, axis=dual_carriage_axis)
        if not all(self._check_kinematics(skip_dc_rail=r) for r in dc_rails):
            raise config.error(
                    "Verify configured 'kinematic_stepper'(s) and their "
                    "'kinematics' specifications, the current configuration "
                    "does not allow independent movements of all printer axes.")
        for r in self.rails:
            r.add_steppers_to_endstops()
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.limits = [(1.0, -1.0)] * 3
    def get_steppers(self):
        return self.steppers
    def get_primary_rails(self):
        rails = self.rails
        if self.dc_module:
            primary_rail = self.dc_module.get_primary_rail().get_rail()
            return (rails[:self.dc_module.get_axis()] + [primary_rail] +
                    rails[self.dc_module.get_axis()+1:3])
        return rails
    def get_active_steppers(self):
        return {s.get_name() : s for s in self.steppers
                if self.dc_module is None or s.get_carriage() is None
                or all(self.dc_module.get_status()['carriage_%d' % i] !=
                       'INACTIVE' for i in [0, 1] if s.get_carriage() ==
                       self.dc_module.get_rails()[i].get_rail().get_name())}
    def _check_kinematics(self, skip_dc_rail):
        matr = [s.get_kin_coeffs() for s in self.steppers
                if skip_dc_rail is None or s.get_carriage() is None
                or s.get_carriage() != skip_dc_rail.get_name()]
        det = mathutil.matrix_det(mat_mul(mat_transp(matr), matr))
        return abs(det) > 0.00001
    def calc_position(self, stepper_positions):
        steppers = self.get_active_steppers()
        matr = [s.get_kin_coeffs() for s in steppers.values()]
        inv = mathutil.matrix_inv(mat_mul(mat_transp(matr), matr))
        proj = mat_mul(matr, inv)
        spos = [stepper_positions[sname] for sname in steppers.keys()]
        pos = mat_mul([spos], proj)
        return pos[0]
    def update_limits(self, i, range):
        l, h = self.limits[i]
        # Only update limits if this axis was already homed,
        # otherwise leave in un-homed state.
        if l <= h:
            self.limits[i] = range
    def set_position(self, newpos, homing_axes):
        for s in self.steppers:
            s.set_position(newpos)
        for axis_name in homing_axes:
            axis = "xyz".index(axis_name)
            if self.dc_module and axis == self.dc_module.get_axis():
                rail = self.dc_module.get_primary_rail().get_rail()
            else:
                rail = self.rails[axis]
            self.limits[axis] = rail.get_range()
    def clear_homing_state(self, clear_axes):
        for axis, axis_name in enumerate("xyz"):
            if axis_name in clear_axes:
                self.limits[axis] = (1.0, -1.0)
    def home_axis(self, homing_state, axis, rail):
        # Determine movement
        position_min, position_max = rail.get_range()
        hi = rail.get_homing_info()
        homepos = [None, None, None, None]
        homepos[axis] = hi.position_endstop
        forcepos = list(homepos)
        if hi.positive_dir:
            forcepos[axis] -= 1.5 * (hi.position_endstop - position_min)
        else:
            forcepos[axis] += 1.5 * (position_max - hi.position_endstop)
        # Perform homing
        homing_state.home_rails([rail], forcepos, homepos)
    def home(self, homing_state):
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            if self.dc_module is not None and axis == self.dc_module.get_axis():
                self.dc_module.home(homing_state)
            else:
                self.home_axis(homing_state, axis, self.rails[axis])
    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in (0, 1, 2):
            if (move.axes_d[i]
                and (end_pos[i] < self.limits[i][0]
                     or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise move.move_error("Must home axis first")
                raise move.move_error()
    def check_move(self, move):
        limits = self.limits
        xpos, ypos = move.end_pos[:2]
        if (xpos < limits[0][0] or xpos > limits[0][1]
            or ypos < limits[1][0] or ypos > limits[1][1]):
            self._check_endstops(move)
        if not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        # Move with Z - update velocity and accel for slower Z axis
        self._check_endstops(move)
        z_ratio = move.move_d / abs(move.axes_d[2])
        move.limit_speed(
            self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
    def get_status(self, eventtime):
        axes = [a for a, (l, h) in zip("xyz", self.limits) if l <= h]
        ranges = [r.get_range() for r in self.get_primary_rails()]
        axes_min = gcode.Coord(*[r[0] for r in ranges], e=0.)
        axes_max = gcode.Coord(*[r[1] for r in ranges], e=0.)
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': axes_min,
            'axis_maximum': axes_max,
        }

def load_kinematics(toolhead, config):
    return GenericCartesianKinematics(toolhead, config)
