# Code for generic handling the kinematics of cartesian-like printers
#
# Copyright (C) 2024-2025  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import copy, itertools, logging, math
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

def find_eigenvectors(m, ek):
    res = []
    for i in range(3):
        if abs(m[i][i]) < 1e-8:
            for j in range(i+1, 3):
                if abs(m[j][i]) > 1e-8:
                    m[i], m[j] = m[j], m[i]
                    break
        if abs(m[i][i]) < 1e-8:
            vr = [-m[k][i] for k in range(3)]
            vr[i] = 1.
            nrm_recipr = 1. / math.sqrt(mathutil.matrix_magsq(vr))
            res.append([x * nrm_recipr for x in vr])
            if len(res) == ek:
                return res
            continue
        recipr = 1. / m[i][i]
        for j in range(i+1, 3):
            m[i][j] *= recipr
        m[i][i] = 1.
        for j in range(3):
            if i != j:
                c = m[j][i]
                for k in range(i, 3):
                    m[j][k] -= c * m[i][k]

def mat_eigen(mtm):
    a, d, f = mtm[0]
    b, e = mtm[1][1:]
    c = mtm[2][2]
    x1 = a**2 + b**2 + c**2 - a*b - a*c - b*c + 3. * (d**2 + f**2 + e**2)
    x2 = -(2. * a - b - c) * (2. * b - a - c) * (2. * c - a - b) \
            + 9. * ((2. * c - a - b) * d**2 + (2. * b - a - c) * f**2 \
            + (2. * a - b - c) * e**2) - 54. * d * e * f
    phi = math.atan2(math.sqrt(4. * x1**3 - x2**2), x2)
    sqrt_x1 = math.sqrt(x1)
    # Closed-form expressions for eigenvalues of a symmetric matrix
    l = [li / 3. if abs(li) > 1e-8 else 0. for li in
         [a + b + c - 2. * sqrt_x1 * math.cos(phi / 3.),
          a + b + c + 2. * sqrt_x1 * math.cos((phi - math.pi) / 3.),
          a + b + c + 2. * sqrt_x1 * math.cos((phi + math.pi) / 3.)]]
    l.sort(reverse=True)
    # Count different eigenvalues
    lc = {l[0]: 1}
    j = 0
    for i in range(1, 3):
        if abs(l[i]-l[j]) < 1e-8:
            lc[l[j]] += 1
        else:
            j = i
            lc[l[j]] = 1
    v = []
    # Find eigenvector(s) for each eigenvalue and its multiplicity
    for li in sorted(lc.keys(), reverse=True):
        mc = copy.deepcopy(mtm)
        for j in range(3):
            mc[j][j] -= li
        v.extend(find_eigenvectors(mc, lc[li]))
    return l, mat_transp(v)

def mat_pseudo_inverse(m):
    mtm = mat_mul(mat_transp(m), m)
    l, v = mat_eigen(mtm)
    # Compute matrix SVD and S pseudo-inverse
    s = [[0.]*3 for i in range(3)]
    for i in range(3):
        s[i][i] = 1. / math.sqrt(l[i]) if l[i] else 0.
    u = mat_mul(m, mat_mul(v, s))
    pinv = mat_mul(v, mat_mul(s, mat_transp(u)))
    return pinv

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
    def del_stepper(self, stepper):
        self.primary_carriage.del_stepper(stepper)

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
                self._check_kinematics(config.error)
            for axis in axes:
                self.dc_module.get_dc()[0].activate(axis, idex_modes.PRIMARY,
                                                    zero_pos)
                self.dc_module.get_dc()[1].inactivate(axis, zero_pos)
        else:
            self._check_kinematics(config.error)
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.limits = [(1.0, -1.0)] * 3
        # Register gcode commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command("SET_STEPPER_KINEMATICS",
                               self.cmd_SET_STEPPER_KINEMATICS,
                               desc=self.cmd_SET_STEPPER_KINEMATICS_help)
    def _load_kinematics(self, config):
        carriages = {a : MainCarriage(config.getsection('carriage ' + a), a)
                     for a in 'xyz'}
        dc_carriages = []
        for c in config.get_prefix_sections('dual_carriage '):
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
        for c in config.get_prefix_sections('extra_carriage '):
            ec_carriages.append(ExtraCarriage(c, carriages))
        for ec in ec_carriages:
            name = ec.get_name()
            if name in carriages:
                raise config.error(
                        "Redefinition of carriage %s" % ec.get_name())
            carriages[name] = ec
        self.steppers = self._load_steppers(config, carriages)
        self.all_carriages = carriages
        self._check_carriages_references(config.error)
        self._check_multi_mcu_homing(config.error)
    def _check_carriages_references(self, report_error):
        carriages = dict(self.all_carriages)
        for s in self.steppers:
            for c in s.get_carriages():
                carriages.pop(c.get_name(), None)
        if carriages:
            raise report_error(
                    "Carriage(s) %s must be referenced by some "
                    "stepper(s) kinematics" % (", ".join(carriages),))
    def _check_multi_mcu_homing(self, report_error):
        for carriage in self.carriages.values():
            for es in carriage.get_endstops():
                stepper_mcus = set([s.get_mcu() for s in es[0].get_steppers()
                                    if s in carriage.get_steppers()])
                if len(stepper_mcus) > 1:
                    raise report_error("Multi-mcu homing not supported on"
                                       " multi-mcu shared carriage %s" % es[1])
    def _load_steppers(self, config, carriages):
        return [ks.KinematicStepper(c, carriages)
                for c in config.get_prefix_sections('stepper ')]
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
    def _check_kinematics(self, report_error):
        matr, _ = self._get_kinematics_coeffs()
        det = mathutil.matrix_det(mat_mul(mat_transp(matr), matr))
        if abs(det) < 0.00001:
            raise report_error(
                    "Verify configured stepper(s) and their 'kinematics' "
                    "specifications, the current configuration does not "
                    "allow independent movements of all printer axes.")
    def calc_position(self, stepper_positions):
        matr, offs = self._get_kinematics_coeffs()
        spos = [stepper_positions[s.get_name()] for s in self.steppers]
        pinv = mat_pseudo_inverse(matr)
        pos = mat_mul([[sp-o for sp, o in zip(spos, offs)]], mat_transp(pinv))
        for i in range(3):
            if not any(pinv[i]):
                pos[0][i] = None
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
        P = mat_transp(mat_pseudo_inverse(A))
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': axes_min,
            'axis_maximum': axes_max,
            'stepper_kinematics': {s.get_name() : A[i]
                                   for i, s in enumerate(self.steppers)},
            'toolhead_kinematics': {s.get_name() : P[i]
                                    for i, s in enumerate(self.steppers)},
        }
    cmd_SET_STEPPER_KINEMATICS_help = "Set stepper kinematics"
    def cmd_SET_STEPPER_KINEMATICS(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        stepper_name = gcmd.get("STEPPER")
        steppers = [stepper for stepper in self.steppers
                    if stepper.get_name() == stepper_name]
        if len(steppers) == 0:
            raise gcmd.error("Invalid STEPPER '%s' specified" % stepper_name)
        stepper = steppers[0]
        kinematics_str = gcmd.get("KINEMATICS").lower()
        validate = not gcmd.get_int("DISABLE_CHECKS", 0)
        old_carriages = stepper.get_carriages()
        stepper.update_kinematics(kinematics_str, self.all_carriages,
                                  report_error=gcmd.error if validate else None)
        new_carriages = stepper.get_carriages()
        for c in old_carriages:
            if c not in new_carriages:
                c.del_stepper(stepper)
        for c in new_carriages:
            if c not in old_carriages:
                c.add_stepper(stepper)
        if not new_carriages:
            stepper.set_trapq(None)
        elif not old_carriages:
            stepper.set_trapq(toolhead.get_trapq())
        pos = toolhead.get_position()
        stepper.set_position(pos)
        if not validate:
            return
        self._check_carriages_references(gcmd.error)
        self._check_multi_mcu_homing(gcmd.error)
        if self.dc_module:
            dc_state = self.dc_module.save_dual_carriage_state()
            axes = [dc.get_axis() for dc in self.dc_carriages]
            for active_dc_per_axis in itertools.product(*[(0, 1)]*len(axes)):
                pos = toolhead.get_position()
                for i, axis in enumerate(axes):
                    active_dc = active_dc_per_axis[i]
                    dcs = self.dc_module.get_dc()
                    dcs[active_dc].activate(axis, idex_modes.PRIMARY, pos)
                    dcs[1-active_dc].inactivate(axis, pos)
                self._check_kinematics(gcmd.error)
            self.dc_module.restore_dual_carriage_state(dc_state, move=0)
        else:
            self._check_kinematics(gcmd.error)

def load_kinematics(toolhead, config):
    return GenericCartesianKinematics(toolhead, config)
