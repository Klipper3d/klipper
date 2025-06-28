# Support for duplication and mirroring modes for IDEX printers
#
# Copyright (C) 2021  Fabrice Gallet <tircown@gmail.com>
# Copyright (C) 2023-2025  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections, logging, math
import chelper

INACTIVE = 'INACTIVE'
PRIMARY = 'PRIMARY'
COPY = 'COPY'
MIRROR = 'MIRROR'

class DualCarriages:
    VALID_MODES = [PRIMARY, COPY, MIRROR]
    def __init__(self, printer, primary_rails, dual_rails, axes,
                 safe_dist={}):
        self.printer = printer
        self.axes = axes
        self._init_steppers(primary_rails + dual_rails)
        self.primary_rails = [
                DualCarriagesRail(printer, c, dual_rails[i],
                                  axes[i], active=True)
                for i, c in enumerate(primary_rails)]
        self.dual_rails = [
                DualCarriagesRail(printer, c, primary_rails[i],
                                  axes[i], active=False)
                for i, c in enumerate(dual_rails)]
        self.dc_rails = collections.OrderedDict(
                [(c.rail.get_name(short=True), c)
                 for c in self.primary_rails + self.dual_rails])
        self.saved_states = {}
        self.safe_dist = {}
        for i, dc in enumerate(dual_rails):
            axis = axes[i]
            if isinstance(safe_dist, dict):
                if axis in safe_dist:
                    self.safe_dist[axis] = safe_dist[axis]
                    continue
            elif safe_dist is not None:
                self.safe_dist[axis] = safe_dist
                continue
            pc = primary_rails[i]
            self.safe_dist[axis] = min(abs(pc.position_min - dc.position_min),
                                       abs(pc.position_max - dc.position_max))
        self.printer.add_object('dual_carriage', self)
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command(
                   'SET_DUAL_CARRIAGE', self.cmd_SET_DUAL_CARRIAGE,
                   desc=self.cmd_SET_DUAL_CARRIAGE_help)
        gcode.register_command(
                   'SAVE_DUAL_CARRIAGE_STATE',
                   self.cmd_SAVE_DUAL_CARRIAGE_STATE,
                   desc=self.cmd_SAVE_DUAL_CARRIAGE_STATE_help)
        gcode.register_command(
                   'RESTORE_DUAL_CARRIAGE_STATE',
                   self.cmd_RESTORE_DUAL_CARRIAGE_STATE,
                   desc=self.cmd_RESTORE_DUAL_CARRIAGE_STATE_help)
    def _init_steppers(self, rails):
        ffi_main, ffi_lib = chelper.get_ffi()
        self.dc_stepper_kinematics = []
        self.orig_stepper_kinematics = []
        steppers = set()
        for rail in rails:
            c_steppers = rail.get_steppers()
            if not c_steppers:
                raise self.printer.config_error(
                        "At least one stepper must be "
                        "associated with carriage: %s" % rail.get_name())
            steppers.update(c_steppers)
        for s in steppers:
            sk = ffi_main.gc(ffi_lib.dual_carriage_alloc(), ffi_lib.free)
            orig_sk = s.get_stepper_kinematics()
            ffi_lib.dual_carriage_set_sk(sk, orig_sk)
            self.dc_stepper_kinematics.append(sk)
            self.orig_stepper_kinematics.append(orig_sk)
            s.set_stepper_kinematics(sk)
    def get_axes(self):
        return self.axes
    def get_primary_rail(self, axis):
        for dc_rail in self.dc_rails.values():
            if dc_rail.mode == PRIMARY and dc_rail.axis == axis:
                return dc_rail.rail
        return None
    def get_dc_rail_wrapper(self, rail):
        for dc_rail in self.dc_rails.values():
            if dc_rail.rail == rail:
                return dc_rail
        return None
    def get_transform(self, rail):
        dc_rail = self.get_dc_rail_wrapper(rail)
        if dc_rail is not None:
            return (dc_rail.scale, dc_rail.offset)
        return (0., 0.)
    def is_active(self, rail):
        dc_rail = self.get_dc_rail_wrapper(rail)
        return dc_rail.is_active() if dc_rail is not None else False
    def toggle_active_dc_rail(self, target_dc):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        pos = toolhead.get_position()
        kin = toolhead.get_kinematics()
        axis = target_dc.axis
        for dc in self.dc_rails.values():
            if dc != target_dc and dc.axis == axis and dc.is_active():
                dc.inactivate(pos)
        if target_dc.mode != PRIMARY:
            newpos = pos[:axis] + [target_dc.get_axis_position(pos)] \
                        + pos[axis+1:]
            target_dc.activate(PRIMARY, newpos, old_position=pos)
            toolhead.set_position(newpos)
        kin.update_limits(axis, target_dc.rail.get_range())
    def home(self, homing_state, axis):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        dcs = [dc for dc in self.dc_rails.values() if dc.axis == axis]
        if (self.get_dc_order(dcs[0], dcs[1]) > 0) != \
                dcs[0].rail.get_homing_info().positive_dir:
            # The second carriage must home first, because the carriages home in
            # the same direction and the first carriage homes on the second one
            dcs.reverse()
        for dc in dcs:
            self.toggle_active_dc_rail(dc)
            kin.home_axis(homing_state, axis, dc.rail)
        # Restore the original rails ordering
        self.toggle_active_dc_rail(dcs[0])
    def get_status(self, eventtime=None):
        status = {'carriages' : {dc.get_name() : dc.mode
                                 for dc in self.dc_rails.values()}}
        if len(self.dc_rails) == 2:
            status.update({('carriage_%d' % (i,)) : dc.mode
                           for i, dc in enumerate(self.dc_rails.values())})
        return status
    def get_kin_range(self, toolhead, mode, axis):
        pos = toolhead.get_position()
        dcs = [dc for dc in self.dc_rails.values() if dc.axis == axis]
        axes_pos = [dc.get_axis_position(pos) for dc in dcs]
        dc0_rail = dcs[0].rail
        dc1_rail = dcs[1].rail
        if mode != PRIMARY or dcs[0].is_active():
            range_min = dc0_rail.position_min
            range_max = dc0_rail.position_max
        else:
            range_min = dc1_rail.position_min
            range_max = dc1_rail.position_max
        safe_dist = self.safe_dist[axis]
        if not safe_dist:
            return (range_min, range_max)

        if mode == COPY:
            range_min = max(range_min,
                            axes_pos[0] - axes_pos[1] + dc1_rail.position_min)
            range_max = min(range_max,
                            axes_pos[0] - axes_pos[1] + dc1_rail.position_max)
        elif mode == MIRROR:
            if self.get_dc_order(dcs[0], dcs[1]) > 0:
                range_min = max(range_min,
                                0.5 * (sum(axes_pos) + safe_dist))
                range_max = min(range_max,
                                sum(axes_pos) - dc1_rail.position_min)
            else:
                range_max = min(range_max,
                                0.5 * (sum(axes_pos) - safe_dist))
                range_min = max(range_min,
                                sum(axes_pos) - dc1_rail.position_max)
        else:
            # mode == PRIMARY
            active_idx = 1 if dcs[1].is_active() else 0
            inactive_idx = 1 - active_idx
            if self.get_dc_order(dcs[active_idx], dcs[inactive_idx]) > 0:
                range_min = max(range_min, axes_pos[inactive_idx] + safe_dist)
            else:
                range_max = min(range_max, axes_pos[inactive_idx] - safe_dist)
        if range_min > range_max:
            # During multi-MCU homing it is possible that the carriage
            # position will end up below position_min or above position_max
            # if position_endstop is too close to the rail motion ends due
            # to inherent latencies of the data transmission between MCUs.
            # This can result in an invalid range_min > range_max range
            # in certain modes, which may confuse the kinematics code.
            # So, return an empty range instead, which will correctly
            # block the carriage motion until a different mode is selected
            # which actually permits carriage motion.
            return (range_min, range_min)
        return (range_min, range_max)
    def get_dc_order(self, first_dc, second_dc):
        if first_dc == second_dc:
            return 0
        # Check the relative order of the first and second carriages and
        # return -1 if the first carriage position is always smaller
        # than the second one and 1 otherwise
        first_rail = first_dc.rail
        second_rail = second_dc.rail
        first_homing_info = first_rail.get_homing_info()
        second_homing_info = second_rail.get_homing_info()
        if first_homing_info.positive_dir != second_homing_info.positive_dir:
            # Carriages home away from each other
            return 1 if first_homing_info.positive_dir else -1
        # Carriages home in the same direction
        if first_rail.position_endstop > second_rail.position_endstop:
            return 1
        return -1
    def activate_dc_mode(self, dc, mode):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        kin = toolhead.get_kinematics()
        axis = dc.axis
        if mode == INACTIVE:
            dc.inactivate(toolhead.get_position())
        elif mode == PRIMARY:
            self.toggle_active_dc_rail(dc)
        else:
            self.toggle_active_dc_rail(self.get_dc_rail_wrapper(dc.dual_rail))
            dc.activate(mode, toolhead.get_position())
        kin.update_limits(axis, self.get_kin_range(toolhead, mode, axis))
    def _handle_ready(self):
        for dc_rail in self.dc_rails.values():
            dc_rail.apply_transform()
    cmd_SET_DUAL_CARRIAGE_help = "Configure the dual carriages mode"
    def cmd_SET_DUAL_CARRIAGE(self, gcmd):
        carriage_str = gcmd.get('CARRIAGE', None)
        if carriage_str is None:
            raise gcmd.error('CARRIAGE must be specified')
        if carriage_str in self.dc_rails:
            dc_rail = self.dc_rails[carriage_str]
        else:
            dc_rail = None
            if len(self.dc_rails) == 2:
                try:
                    index = int(carriage_str.strip())
                    if index < 0 or index > 1:
                        raise gcmd.error('Invalid CARRIAGE=%d index' % index)
                    dc_rail = (self.dual_rails if index
                               else self.primary_rails)[0]
                except ValueError:
                    pass
            if dc_rail is None:
                raise gcmd.error('Invalid CARRIAGE=%s specified' % carriage_str)
        mode = gcmd.get('MODE', PRIMARY).upper()
        if mode not in self.VALID_MODES:
            raise gcmd.error("Invalid mode=%s specified" % (mode,))
        if mode in [COPY, MIRROR]:
            if dc_rail in self.primary_rails:
                raise gcmd.error(
                        "Mode=%s is not supported for carriage=%s" % (
                            mode, dc_rail.get_name()))
            curtime = self.printer.get_reactor().monotonic()
            kin = self.printer.lookup_object('toolhead').get_kinematics()
            axis = 'xyz'[dc_rail.axis]
            if axis not in kin.get_status(curtime)['homed_axes']:
                raise gcmd.error(
                        "Axis %s must be homed prior to enabling mode=%s" %
                        (axis.upper(), mode))
        self.activate_dc_mode(dc_rail, mode)
    cmd_SAVE_DUAL_CARRIAGE_STATE_help = \
            "Save dual carriages modes and positions"
    def cmd_SAVE_DUAL_CARRIAGE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        self.saved_states[state_name] = self.save_dual_carriage_state()
    def save_dual_carriage_state(self):
        pos = self.printer.lookup_object('toolhead').get_position()
        return {'carriage_modes': {dc.get_name() : dc.mode
                                   for dc in self.dc_rails.values()},
                'carriage_positions': {dc.get_name() : dc.get_axis_position(pos)
                                       for dc in self.dc_rails.values()}}
    cmd_RESTORE_DUAL_CARRIAGE_STATE_help = \
            "Restore dual carriages modes and positions"
    def cmd_RESTORE_DUAL_CARRIAGE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        saved_state = self.saved_states.get(state_name)
        if saved_state is None:
            raise gcmd.error("Unknown DUAL_CARRIAGE state: %s" % (state_name,))
        move_speed = gcmd.get_float('MOVE_SPEED', 0., above=0.)
        move = gcmd.get_int('MOVE', 1)
        self.restore_dual_carriage_state(saved_state, move, move_speed)
    def restore_dual_carriage_state(self, saved_state, move, move_speed=0.):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        if move:
            homing_speed = 99999999.
            move_pos = list(toolhead.get_position())
            cur_pos = []
            carriage_positions = saved_state['carriage_positions']
            dcs = list(self.dc_rails.values())
            for dc in dcs:
                self.toggle_active_dc_rail(dc)
                homing_speed = min(homing_speed, dc.rail.homing_speed)
                cur_pos.append(toolhead.get_position())
            dl = [carriage_positions[dc.get_name()] - cur_pos[i][dc.axis]
                  for i, dc in enumerate(dcs)]
            for axis in self.axes:
                dc_ind = [i for i, dc in enumerate(dcs) if dc.axis == axis]
                if abs(dl[dc_ind[0]]) >= abs(dl[dc_ind[1]]):
                    primary_ind, secondary_ind = dc_ind[0], dc_ind[1]
                else:
                    primary_ind, secondary_ind = dc_ind[1], dc_ind[0]
                primary_dc = dcs[primary_ind]
                self.toggle_active_dc_rail(primary_dc)
                move_pos[axis] = carriage_positions[primary_dc.get_name()]
                dc_mode = INACTIVE if min(abs(dl[primary_ind]),
                                          abs(dl[secondary_ind])) < .000000001 \
                        else COPY if dl[primary_ind] * dl[secondary_ind] > 0 \
                        else MIRROR
                if dc_mode != INACTIVE:
                    dcs[secondary_ind].activate(dc_mode, cur_pos[primary_ind])
                    dcs[secondary_ind].override_axis_scaling(
                            abs(dl[secondary_ind] / dl[primary_ind]),
                            cur_pos[primary_ind])
            toolhead.manual_move(move_pos, move_speed or homing_speed)
            toolhead.flush_step_generation()
            # Make sure the scaling coefficients are restored with the mode
            for dc in dcs:
                dc.inactivate(move_pos)
        for dc in self.dc_rails.values():
            saved_mode = saved_state['carriage_modes'][dc.get_name()]
            self.activate_dc_mode(dc, saved_mode)

class DualCarriagesRail:
    ENC_AXES = [b'x', b'y']
    def __init__(self, printer, rail, dual_rail, axis, active):
        self.printer = printer
        self.rail = rail
        self.dual_rail = dual_rail
        self.sks = [s.get_stepper_kinematics() for s in rail.get_steppers()]
        self.axis = axis
        self.mode = (INACTIVE, PRIMARY)[active]
        self.offset = 0.
        self.scale = 1. if active else 0.
    def get_name(self):
        return self.rail.get_name()
    def is_active(self):
        return self.mode != INACTIVE
    def get_axis_position(self, position):
        return position[self.axis] * self.scale + self.offset
    def apply_transform(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        for sk in self.sks:
            ffi_lib.dual_carriage_set_transform(
                    sk, self.ENC_AXES[self.axis], self.scale, self.offset)
        self.printer.send_event('dual_carriage:update_kinematics')
    def activate(self, mode, position, old_position=None):
        old_axis_position = self.get_axis_position(old_position or position)
        self.scale = -1. if mode == MIRROR else 1.
        self.offset = old_axis_position - position[self.axis] * self.scale
        self.apply_transform()
        self.mode = mode
    def inactivate(self, position):
        self.offset = self.get_axis_position(position)
        self.scale = 0.
        self.apply_transform()
        self.mode = INACTIVE
    def override_axis_scaling(self, new_scale, position):
        old_axis_position = self.get_axis_position(position)
        self.scale = math.copysign(new_scale, self.scale)
        self.offset = old_axis_position - position[self.axis] * self.scale
        self.apply_transform()
