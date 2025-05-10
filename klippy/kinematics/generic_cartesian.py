# Code for generic handling the kinematics of cartesian-like printers
#
# Copyright (C) 2024-2025  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import copy, itertools, logging, math
import gcode, mathutil, stepper
from . import idex_modes
from . import kinematic_stepper as ks

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

def mat_pseudo_inverse(m):
    mt = mat_transp(m)
    mtm = mat_mul(mt, m)
    pinv = mat_mul(mathutil.matrix_inv(mtm), mt)
    return pinv

class MainCarriage:
    def __init__(self, config, axis):
        self.rail = stepper.GenericPrinterRail(config)
        self.axis = ord(axis) - ord('x')
        self.axis_name = axis
        self.dual_carriage = None
    def get_name(self):
        return self.rail.get_name(short=True)
    def get_axis(self):
        return self.axis
    def get_rail(self):
        return self.rail
    def add_stepper(self, kin_stepper):
        self.rail.add_stepper(kin_stepper.get_stepper())
    def is_active(self):
        if self.dual_carriage is None:
            return True
        return self.dual_carriage.get_dc_module().is_active(self.rail)
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
    def get_rail(self):
        return self.primary_carriage.get_rail()
    def add_stepper(self, kin_stepper):
        self.get_rail().add_stepper(kin_stepper.get_stepper(),
                                    self.endstop_pin, self.name)

class DualCarriage:
    def __init__(self, config, carriages):
        self.printer = config.get_printer()
        self.rail = stepper.GenericPrinterRail(config)
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
    def get_name(self):
        return self.rail.get_name(short=True)
    def get_axis(self):
        return self.primary_carriage.get_axis()
    def get_rail(self):
        return self.rail
    def get_safe_dist(self):
        return self.safe_dist
    def get_dc_module(self):
        return self.printer.lookup_object('dual_carriage')
    def is_active(self):
        return self.get_dc_module().is_active(self.rail)
    def get_dual_carriage(self):
        return self.primary_carriage
    def add_stepper(self, kin_stepper):
        self.rail.add_stepper(kin_stepper.get_stepper())

class GenericCartesianKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        self._load_kinematics(config)
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        self.dc_module = None
        if self.dc_carriages:
            pcs = [dc.get_dual_carriage() for dc in self.dc_carriages]
            primary_rails = [pc.get_rail() for pc in pcs]
            dual_rails = [dc.get_rail() for dc in self.dc_carriages]
            axes = [dc.get_axis() for dc in self.dc_carriages]
            safe_dist = {dc.get_axis() : dc.get_safe_dist()
                         for dc in self.dc_carriages}
            self.dc_module = dc_module = idex_modes.DualCarriages(
                    self.printer, primary_rails, dual_rails, axes, safe_dist)
            zero_pos = (0., 0.)
            for acs in itertools.product(*zip(pcs, self.dc_carriages)):
                for c in acs:
                    dc_module.get_dc_rail_wrapper(c.get_rail()).activate(
                            idex_modes.PRIMARY, zero_pos)
                    dc_rail = c.get_dual_carriage().get_rail()
                    dc_module.get_dc_rail_wrapper(dc_rail).inactivate(zero_pos)
                self._check_kinematics(config.error)
            for c in pcs:
                dc_module.get_dc_rail_wrapper(c.get_rail()).activate(
                        idex_modes.PRIMARY, zero_pos)
                dc_rail = c.get_dual_carriage().get_rail()
                dc_module.get_dc_rail_wrapper(dc_rail).inactivate(zero_pos)
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
        gcode.register_command("SET_STEPPER_CARRIAGES",
                               self.cmd_SET_STEPPER_CARRIAGES,
                               desc=self.cmd_SET_STEPPER_CARRIAGES_help)
    def _load_kinematics(self, config):
        carriages = {a : MainCarriage(config.getsection('carriage ' + a), a)
                     for a in 'xyz'}
        dc_carriages = []
        for c in config.get_prefix_sections('dual_carriage '):
            dc_carriages.append(DualCarriage(c, carriages))
        for dc in dc_carriages:
            name = dc.get_name()
            if name in carriages:
                raise config.error("Redefinition of carriage %s" % name)
            carriages[name] = dc
        self.carriages = dict(carriages)
        self.dc_carriages = dc_carriages
        ec_carriages = []
        for c in config.get_prefix_sections('extra_carriage '):
            ec_carriages.append(ExtraCarriage(c, carriages))
        for ec in ec_carriages:
            name = ec.get_name()
            if name in carriages:
                raise config.error("Redefinition of carriage %s" % name)
            carriages[name] = ec
        self.kin_steppers = self._load_steppers(config, carriages)
        self.all_carriages = carriages
        self._check_carriages_references(config.error)
        self._check_multi_mcu_homing(config.error)
    def _check_carriages_references(self, report_error):
        carriages = dict(self.all_carriages)
        for s in self.kin_steppers:
            for c in s.get_carriages():
                carriages.pop(c.get_name(), None)
        if carriages:
            raise report_error(
                    "Carriage(s) %s must be referenced by some "
                    "stepper(s)" % (", ".join(carriages),))
    def _check_multi_mcu_homing(self, report_error):
        for carriage in self.carriages.values():
            for es in carriage.get_rail().get_endstops():
                stepper_mcus = set([s.get_mcu() for s in es[0].get_steppers()
                                    if s in carriage.get_rail().get_steppers()])
                if len(stepper_mcus) > 1:
                    raise report_error("Multi-mcu homing not supported on"
                                       " multi-mcu shared carriage %s" % es[1])
    def _load_steppers(self, config, carriages):
        return [ks.KinematicStepper(c, carriages)
                for c in config.get_prefix_sections('stepper ')]
    def get_steppers(self):
        return [s.get_stepper() for s in self.kin_steppers]
    def get_primary_carriages(self):
        carriages = [self.carriages[a] for a in "xyz"]
        if self.dc_module:
            for a in self.dc_module.get_axes():
                primary_rail = self.dc_module.get_primary_rail(a)
                for c in self.carriages.values():
                    if c.get_rail() == primary_rail:
                        carriages[a] = c
        return carriages
    def _get_kinematics_coeffs(self):
        matr = {s.get_name() : list(s.get_kin_coeffs())
                for s in self.kin_steppers}
        offs = {s.get_name() : [0.] * 3 for s in self.kin_steppers}
        if self.dc_module is None:
            return ([matr[s.get_name()] for s in self.kin_steppers],
                    [0. for s in self.kin_steppers])
        axes = [dc.get_axis() for dc in self.dc_carriages]
        orig_matr = copy.deepcopy(matr)
        for dc in self.dc_carriages:
            axis = dc.get_axis()
            for c in [dc.get_dual_carriage(), dc]:
                m, o = self.dc_module.get_transform(c.get_rail())
                for s in c.get_rail().get_steppers():
                    matr[s.get_name()][axis] *= m
                    offs[s.get_name()][axis] += o
        return ([matr[s.get_name()] for s in self.kin_steppers],
                [mathutil.matrix_dot(orig_matr[s.get_name()],
                                     offs[s.get_name()])
                 for s in self.kin_steppers])
    def _check_kinematics(self, report_error):
        matr, _ = self._get_kinematics_coeffs()
        det = mathutil.matrix_det(mat_mul(mat_transp(matr), matr))
        if abs(det) < 0.00001:
            raise report_error(
                    "Verify configured stepper(s) and their 'carriages' "
                    "specifications, the current configuration does not "
                    "allow independent movements of all printer axes.")
    def calc_position(self, stepper_positions):
        matr, offs = self._get_kinematics_coeffs()
        spos = [stepper_positions[s.get_name()] for s in self.kin_steppers]
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
        for s in self.kin_steppers:
            s.set_position(newpos)
        for axis_name in homing_axes:
            axis = "xyz".index(axis_name)
            for c in self.carriages.values():
                if c.get_axis() == axis and c.is_active():
                    self.limits[axis] = c.get_rail().get_range()
                    break
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
        self._check_kinematics(self.printer.command_error)
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            carriage = self.carriages["xyz"[axis]]
            if carriage.get_dual_carriage() != None:
                self.dc_module.home(homing_state, axis)
            else:
                self.home_axis(homing_state, axis, carriage.get_rail())
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
        ranges = [c.get_rail().get_range()
                  for c in self.get_primary_carriages()]
        axes_min = gcode.Coord(*[r[0] for r in ranges], e=0.)
        axes_max = gcode.Coord(*[r[1] for r in ranges], e=0.)
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': axes_min,
            'axis_maximum': axes_max,
        }
    cmd_SET_STEPPER_CARRIAGES_help = "Set stepper carriages"
    def cmd_SET_STEPPER_CARRIAGES(self, gcmd):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        stepper_name = gcmd.get("STEPPER")
        steppers = [stepper for stepper in self.kin_steppers
                    if stepper.get_name() == stepper_name
                    or stepper.get_name(short=True) == stepper_name]
        if len(steppers) != 1:
            raise gcmd.error("Invalid STEPPER '%s' specified" % stepper_name)
        stepper = steppers[0]
        carriages_str = gcmd.get("CARRIAGES").lower()
        validate = not gcmd.get_int("DISABLE_CHECKS", 0)
        old_carriages = stepper.get_carriages()
        old_kin_coeffs = stepper.get_kin_coeffs()
        stepper.update_carriages(carriages_str, self.all_carriages, gcmd.error)
        new_carriages = stepper.get_carriages()
        if new_carriages != old_carriages:
            stepper.update_kin_coeffs(old_kin_coeffs)
            raise gcmd.error("SET_STEPPER_CARRIAGES cannot add or remove "
                             "carriages that the stepper controls")
        pos = toolhead.get_position()
        stepper.set_position(pos)
        if not validate:
            return
        if self.dc_module:
            dc_state = self.dc_module.save_dual_carriage_state()
            pcs = [dc.get_dual_carriage() for dc in self.dc_carriages]
            axes = [dc.get_axis() for dc in self.dc_carriages]
            for acs in itertools.product(*zip(pcs, self.dc_carriages)):
                for c in acs:
                    self.dc_module.get_dc_rail_wrapper(c.get_rail()).activate(
                            idex_modes.PRIMARY, pos)
                    self.dc_module.get_dc_rail_wrapper(
                            c.get_dual_carriage().get_rail()).inactivate(pos)
                self._check_kinematics(gcmd.error)
            self.dc_module.restore_dual_carriage_state(dc_state, move=0)
        else:
            self._check_kinematics(gcmd.error)

def load_kinematics(toolhead, config):
    return GenericCartesianKinematics(toolhead, config)
