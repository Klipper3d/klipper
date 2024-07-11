# Code for generic handling the kinematics of cartesian-like printers
#
# Copyright (C) 2024  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import copy, itertools, logging
import gcode, mathutil, stepper
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

class MainCarriage(stepper.GenericPrinterCarriage):
    def __init__(self, config, axis):
        stepper.GenericPrinterCarriage.__init__(self, config)
        self.axis = ord(axis) - ord('x')
        self.axis_name = axis
        self.dual_carriage = None
    def get_axis(self):
        return self.axis
    def is_active(self):
        if self.dual_carriage is None:
            return True
        return self.dual_carriage.get_dc_module().is_active(self.axis, 0)
    def set_dual_carriage(self, carriage):
        old_dc = self.dual_carriage
        self.dual_carriage = carriage
        return old_dc
    def get_dual_carriage(self):
        return self.dual_carriage

class ExtraCarriage:
    def __init__(self, config, carriages):
        self.name = config.get_name().split()[-1]
        self.primary_carriage = config.getchoice('primary_carriage', carriages)
        self.endstop_pin = config.get('endstop_pin')
    def get_name(self):
        return self.name
    def get_axis(self):
        return self.primary_carriage.get_axis()
    def add_stepper(self, stepper):
        self.primary_carriage.add_stepper(stepper, self.endstop_pin, self.name)

class DualCarriage(stepper.GenericPrinterCarriage):
    def __init__(self, config, carriages):
        stepper.GenericPrinterCarriage.__init__(self, config)
        self.printer = config.get_printer()
        self.primary_carriage = config.getchoice('primary_carriage', carriages)
        if self.primary_carriage.set_dual_carriage(self) is not None:
            raise config.error(
                    "Redefinition of dual_carriage for carriage '%s'" %
                    self.primary_carriage.get_name())
        self.axis = self.primary_carriage.get_axis()
        if self.axis > 1:
            raise config.error("Invalid axis '%s' for dual_carriage" %
                               self.primary_carriage.get_axis_name())
        self.safe_dist = config.getfloat('safe_distance', None, minval=0.)
    def get_axis(self):
        return self.primary_carriage.get_axis()
    def get_safe_dist(self):
        return self.safe_dist
    def get_dc_module(self):
        return self.printer.lookup_object('dual_carriage')
    def is_active(self):
        return self.get_dc_module().is_active(self.axis, 1)
    def get_primary_carriage(self):
        return self.primary_carriage

class GenericCartesianKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        self._load_kinematics(config)
        for s in self.steppers:
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        self.dc_module = None
        if self.dc_carriages:
            pcs = [dc.get_primary_carriage()
                   for dc in self.dc_carriages]
            axes = [dc.get_axis() for dc in self.dc_carriages]
            self.dc_module = idex_modes.DualCarriages(
                    self.printer, pcs, self.dc_carriages, axes)
            zero_pos = (0., 0.)
            for active_dc_per_axis in itertools.product(*[(0, 1)]*len(axes)):
                for i, axis in enumerate(axes):
                    active_dc = active_dc_per_axis[i]
                    dcs = self.dc_module.get_dc()
                    dcs[active_dc].activate(axis, idex_modes.PRIMARY, zero_pos)
                    dcs[1-active_dc].inactivate(axis, zero_pos)
                self._check_kinematics()
            for axis in axes:
                self.dc_module.get_dc()[0].activate(axis, idex_modes.PRIMARY,
                                                    zero_pos)
                self.dc_module.get_dc()[1].inactivate(axis, zero_pos)
        else:
            self._check_kinematics()
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.limits = [(1.0, -1.0)] * 3
    def _load_kinematics(self, config):
        carriages = {a : MainCarriage(config.getsection('carriage ' + a), a)
                     for a in 'xyz'}
        dc_carriages = []
        for c in config.get_prefix_sections('dual_carriage'):
            dc_carriages.append(DualCarriage(c, carriages))
        for dc in dc_carriages:
            name = dc.get_name()
            if name in carriages:
                raise config.error(
                        "Redefinition of carriage %s" % dc.get_name())
            carriages[name] = dc
        self.carriages = dict(carriages)
        self.dc_carriages = dc_carriages
        ec_carriages = []
        for c in config.get_prefix_sections('extra_carriage'):
            ec_carriages.append(ExtraCarriage(c, carriages))
        for ec in ec_carriages:
            name = ec.get_name()
            if name in carriages:
                raise config.error(
                        "Redefinition of carriage %s" % ec.get_name())
            carriages[name] = ec
        self.steppers = ks.LoadKinematicSteppers(config, carriages)
        for s in self.steppers:
            for c in s.get_carriages():
                carriages.pop(c.get_name(), None)
        if carriages:
            raise config.error(
                    "Carriage(s) %s must be referenced by some "
                    "kinematic_stepper(s) kinematics" % (", ".join(carriages),))
    def get_steppers(self):
        return self.steppers
    def get_primary_carriages(self):
        carriages = [self.carriages[a] for a in "xyz"]
        if self.dc_module:
            for a in self.dc_module.get_axes():
                carriages[a] = self.dc_module.get_primary_carriage(a)
        return carriages
    def _get_kinematics_coeffs(self):
        matr = {s.get_name() : list(s.get_kin_coeffs()) for s in self.steppers}
        offs = {s.get_name() : [0.] * 3 for s in self.steppers}
        if self.dc_module is None:
            return [matr[s.get_name()] for s in self.steppers], \
                    [0. for s in self.steppers]
        axes = [dc.get_axis() for dc in self.dc_carriages]
        orig_matr = copy.deepcopy(matr)
        for dc in self.dc_carriages:
            axis = dc.get_axis()
            for i, c in enumerate([dc.get_primary_carriage(), dc]):
                m, o = self.dc_module.get_transform(axis, i)
                for s in c.get_steppers():
                    matr[s.get_name()][axis] *= m
                    offs[s.get_name()][axis] += o
        return [matr[s.get_name()] for s in self.steppers], \
                [mathutil.matrix_dot(orig_matr[s.get_name()],
                                     offs[s.get_name()]) for s in self.steppers]
    def _check_kinematics(self):
        matr, _ = self._get_kinematics_coeffs()
        det = mathutil.matrix_det(mat_mul(mat_transp(matr), matr))
        if abs(det) < 0.00001:
            raise self.printer.config_error(
                    "Verify configured 'kinematic_stepper'(s) and their "
                    "'kinematics' specifications, the current configuration "
                    "does not allow independent movements of all printer axes.")
    def calc_position(self, stepper_positions):
        matr, offs = self._get_kinematics_coeffs()
        inv = mathutil.matrix_inv(mat_mul(mat_transp(matr), matr))
        proj = mat_mul(matr, inv)
        spos = [stepper_positions[s.get_name()] for s in self.steppers]
        pos = mat_mul([[sp-o for sp, o in zip(spos, offs)]], proj)
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
            for c in self.carriages.values():
                if c.get_axis() == axis and c.is_active():
                    self.limits[axis] = c.get_range()
                    break
    def clear_homing_state(self, clear_axes):
        for axis, axis_name in enumerate("xyz"):
            if axis_name in clear_axes:
                self.limits[axis] = (1.0, -1.0)
    def home_axis(self, homing_state, axis, carriage):
        # Determine movement
        position_min, position_max = carriage.get_range()
        hi = carriage.get_homing_info()
        homepos = [None, None, None, None]
        homepos[axis] = hi.position_endstop
        forcepos = list(homepos)
        if hi.positive_dir:
            forcepos[axis] -= 1.5 * (hi.position_endstop - position_min)
        else:
            forcepos[axis] += 1.5 * (position_max - hi.position_endstop)
        # Perform homing
        homing_state.home_rails([carriage], forcepos, homepos)
    def home(self, homing_state):
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            carriage = self.carriages["xyz"[axis]]
            if carriage.get_dual_carriage() != None:
                self.dc_module.home(homing_state, axis)
            else:
                self.home_axis(homing_state, axis, carriage)
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
        ranges = [c.get_range() for c in self.get_primary_carriages()]
        axes_min = gcode.Coord(*[r[0] for r in ranges], e=0.)
        axes_max = gcode.Coord(*[r[1] for r in ranges], e=0.)
        A = self._get_kinematics_coeffs()[0]
        P = mat_mul(A, mathutil.matrix_inv(mat_mul(mat_transp(A), A)))
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': axes_min,
            'axis_maximum': axes_max,
            'stepper_kinematics': {s.get_name() : A[i]
                                   for i, s in enumerate(self.steppers)},
            'toolhead_kinematics': {s.get_name() : P[i]
                                    for i, s in enumerate(self.steppers)},
        }

def load_kinematics(toolhead, config):
    return GenericCartesianKinematics(toolhead, config)
