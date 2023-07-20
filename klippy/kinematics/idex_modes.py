# Support for duplication and mirroring modes for IDEX printers
#
# Copyright (C) 2021  Fabrice Gallet <tircown@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math
import chelper

class DualCarriages:
    def __init__(self, printer, rail_0, rail_1, axis):
        self.printer = printer
        self.axis = axis
        self.dc = (rail_0, rail_1)
        self.saved_state = None
        self.printer.add_object('dual_carriage', self)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command(
                   'SET_DUAL_CARRIAGE', self.cmd_SET_DUAL_CARRIAGE,
                   desc=self.cmd_SET_DUAL_CARRIAGE_help)
    def toggle_active_dc_rail(self, index):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        pos = toolhead.get_position()
        kin = toolhead.get_kinematics()
        for i, dc in enumerate(self.dc):
            dc_rail = dc.get_rail()
            if i != index:
                if dc.is_active():
                    dc.inactivate(pos)
                kin.override_rail(3, dc_rail)
            elif dc.is_active() is False:
                newpos = pos[:self.axis] + [dc.axis_position] \
                            + pos[self.axis+1:]
                dc.activate(newpos)
                kin.override_rail(self.axis, dc_rail)
                toolhead.set_position(newpos)
                kin.update_limits(self.axis, dc_rail.get_range())
    def get_status(self, eventtime=None):
        dc0, dc1 = self.dc
        if (dc0.is_active() is True):
            return { 'mode': 'FULL_CONTROL', 'active_carriage': 'CARRIAGE_0' }
        else:
            return { 'mode': 'FULL_CONTROL', 'active_carriage': 'CARRIAGE_1' }
    def save_idex_state(self):
        dc0, dc1 = self.dc
        if (dc0.is_active() is True):
            mode, active_carriage = ('FULL_CONTROL', 'CARRIAGE_0')
        else:
            mode, active_carriage = ('FULL_CONTROL', 'CARRIAGE_1')
        self.saved_state = {
            'mode': mode,
            'active_carriage': active_carriage,
            'axis_positions': (dc0.axis_position, dc1.axis_position)
            }
    def restore_idex_state(self):
        if self.saved_state is not None:
            # set carriage 0 active
            if (self.saved_state['active_carriage'] == 'CARRIAGE_0'
                        and self.dc[0].is_active() is False):
                self.toggle_active_dc_rail(0)
            # set carriage 1 active
            elif (self.saved_state['active_carriage'] == 'CARRIAGE_1'
                        and self.dc[1].is_active() is False):
                self.toggle_active_dc_rail(1)
    cmd_SET_DUAL_CARRIAGE_help = "Set which carriage is active"
    def cmd_SET_DUAL_CARRIAGE(self, gcmd):
        index = gcmd.get_int('CARRIAGE', minval=0, maxval=1)
        if (not(self.dc[0].is_active() == self.dc[1].is_active() == True)
                    and self.dc[index].is_active() is False):
            self.toggle_active_dc_rail(index)

class DualCarriagesRail:
    ACTIVE=1
    INACTIVE=2
    def __init__(self, printer, rail, axis, active, stepper_alloc_active,
                 stepper_alloc_inactive=None):
        self.printer = printer
        self.rail = rail
        self.axis = axis
        self.status = (self.INACTIVE, self.ACTIVE)[active]
        self.stepper_alloc_active = stepper_alloc_active
        self.stepper_alloc_inactive = stepper_alloc_inactive
        self.axis_position = -1
        self.stepper_active_sk = {}
        self.stepper_inactive_sk = {}
        for s in rail.get_steppers():
            self._save_sk(self.status, s, s.get_stepper_kinematics())
    def _alloc_sk(self, alloc_func, *params):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_main.gc(getattr(ffi_lib, alloc_func)(*params), ffi_lib.free)
    def _get_sk(self, status, stepper):
        sk = None
        if status == self.ACTIVE:
            sk = self.stepper_active_sk.get(stepper, None)
            if sk is None and self.stepper_alloc_active:
                sk = self._alloc_sk(*self.stepper_alloc_active)
                self._save_sk(status, stepper, sk)
        elif status == self.INACTIVE:
            sk = self.stepper_inactive_sk.get(stepper, None)
            if sk is None and self.stepper_alloc_inactive:
                sk = self._alloc_sk(*self.stepper_alloc_inactive)
                self._save_sk(status, stepper, sk)
        return sk
    def _save_sk(self, status, stepper, sk):
        if status == self.ACTIVE:
            self.stepper_active_sk[stepper] = sk
        elif status == self.INACTIVE:
            self.stepper_inactive_sk[stepper] = sk
    def _update_stepper_alloc(self, position, active=True):
        toolhead = self.printer.lookup_object('toolhead')
        self.axis_position = position[self.axis]
        self.rail.set_trapq(None)
        old_status = self.status
        self.status = (self.INACTIVE, self.ACTIVE)[active]
        for s in self.rail.get_steppers():
            sk = self._get_sk(self.status, s)
            if sk is None:
                return
            old_sk = s.set_stepper_kinematics(sk)
            self._save_sk(old_status, s, old_sk)
        self.rail.set_position(position)
        self.rail.set_trapq(toolhead.get_trapq())
    def get_rail(self):
        return self.rail
    def is_active(self):
        return self.status == self.ACTIVE
    def activate(self, position):
        self._update_stepper_alloc(position, active=True)
    def inactivate(self, position):
        self._update_stepper_alloc(position, active=False)
