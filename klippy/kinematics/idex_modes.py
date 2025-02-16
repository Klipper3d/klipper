# Support for duplication and mirroring modes for IDEX printers
#
# Copyright (C) 2021  Fabrice Gallet <tircown@gmail.com>
# Copyright (C) 2023  Dmitry Butyugin <dmbutyugin@google.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import chelper

INACTIVE = 'INACTIVE'
PRIMARY = 'PRIMARY'
COPY = 'COPY'
MIRROR = 'MIRROR'

class DualCarriages:
    VALID_MODES = [PRIMARY, COPY, MIRROR]
    def __init__(self, printer, carriages0, carriages1, axes, safe_dist=-1):
        self.printer = printer
        self.axes = axes
        self.dc = (DualCarriage(printer, carriages0, active=True),
                   DualCarriage(printer, carriages1, active=False))
        self.saved_states = {}
        self.safe_dist = {}
        for axis in self.axes:
            if safe_dist is not None and safe_dist < 0:
                axis_safe_dist = self.dc[1].get_carriage(axis).get_safe_dist()
            else:
                axis_safe_dist = safe_dist
            if axis_safe_dist is not None:
                self.safe_dist[axis] = axis_safe_dist
                continue
            c0 = self.dc[0].get_carriage(axis)
            c1 = self.dc[1].get_carriage(axis)
            self.safe_dist[axis] = min(abs(c0.position_min - c1.position_min),
                                       abs(c0.position_max - c1.position_max))
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
    def get_axes(self):
        return self.axes
    def get_dc(self):
        return self.dc
    def get_primary_carriage(self, axis):
        for dc in self.dc:
            if dc.mode[axis] == PRIMARY:
                return dc.get_carriage(axis)
        return None
    def get_transform(self, axis, index):
        dc = self.dc[index]
        return (dc.scale[axis], dc.offset[axis])
    def is_active(self, axis, index):
        return self.dc[index].is_active(axis)
    def toggle_active_dc_carriage(self, axis, index):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        pos = toolhead.get_position()
        kin = toolhead.get_kinematics()
        for i, dc in enumerate(self.dc):
            if i != index:
                if dc.is_active(axis):
                    dc.inactivate(axis, pos)
        target_dc = self.dc[index]
        if target_dc.mode[axis] != PRIMARY:
            newpos = pos[:axis] + [target_dc.get_axis_position(axis, pos)] \
                        + pos[axis+1:]
            target_dc.activate(axis, PRIMARY, newpos, old_position=pos)
            toolhead.set_position(newpos)
        kin.update_limits(axis, target_dc.get_carriage(axis).get_range())
    def home(self, homing_state, axis):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        enumerated_dcs = list(enumerate(self.dc))
        if (self.get_dc_order(axis, 0, 1) > 0) != \
                self.dc[0].get_carriage(axis).get_homing_info().positive_dir:
            # The second carriage must home first, because the carriages home in
            # the same direction and the first carriage homes on the second one
            enumerated_dcs.reverse()
        for i, dc_rail in enumerated_dcs:
            self.toggle_active_dc_carriage(axis, i)
            kin.home_axis(homing_state, axis, dc_rail.get_carriage(axis))
        # Restore the original rails ordering
        self.toggle_active_dc_carriage(axis, 0)
    def get_status(self, eventtime=None):
        return {('carriage_%d' % (i,)) :
                dc.mode[self.axes[0]] if len(self.axes) == 1 else dc.mode
                for (i, dc) in enumerate(self.dc)}
    def get_kin_range(self, toolhead, mode, axis):
        pos = toolhead.get_position()
        axes_pos = [dc.get_axis_position(axis, pos) for dc in self.dc]
        dc0 = self.dc[0].get_carriage(axis)
        dc1 = self.dc[1].get_carriage(axis)
        if mode != PRIMARY or self.dc[0].is_active(axis):
            range_min = dc0.position_min
            range_max = dc0.position_max
        else:
            range_min = dc1.position_min
            range_max = dc1.position_max
        safe_dist = self.safe_dist[axis]
        if not safe_dist:
            return (range_min, range_max)

        if mode == COPY:
            range_min = max(range_min,
                            axes_pos[0] - axes_pos[1] + dc1.position_min)
            range_max = min(range_max,
                            axes_pos[0] - axes_pos[1] + dc1.position_max)
        elif mode == MIRROR:
            if self.get_dc_order(axis, 0, 1) > 0:
                range_min = max(range_min,
                                0.5 * (sum(axes_pos) + safe_dist))
                range_max = min(range_max,
                                sum(axes_pos) - dc1.position_min)
            else:
                range_max = min(range_max,
                                0.5 * (sum(axes_pos) - safe_dist))
                range_min = max(range_min,
                                sum(axes_pos) - dc1.position_max)
        else:
            # mode == PRIMARY
            active_idx = 1 if self.dc[1].is_active(axis) else 0
            inactive_idx = 1 - active_idx
            if self.get_dc_order(axis, active_idx, inactive_idx) > 0:
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
    def get_dc_order(self, axis, first, second):
        if first == second:
            return 0
        # Check the relative order of the first and second carriages and
        # return -1 if the first carriage position is always smaller
        # than the second one and 1 otherwise
        first_carriage = self.dc[first].get_carriage(axis)
        second_carriage = self.dc[second].get_carriage(axis)
        first_homing_info = first_carriage.get_homing_info()
        second_homing_info = second_carriage.get_homing_info()
        if first_homing_info.positive_dir != second_homing_info.positive_dir:
            # Carriages home away from each other
            return 1 if first_homing_info.positive_dir else -1
        # Carriages home in the same direction
        if first_carriage.position_endstop > second_carriage.position_endstop:
            return 1
        return -1
    def activate_dc_mode(self, axis, index, mode):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        kin = toolhead.get_kinematics()
        if mode == INACTIVE:
            self.dc[index].inactivate(axis, toolhead.get_position())
        elif mode == PRIMARY:
            self.toggle_active_dc_carriage(axis, index)
        else:
            self.toggle_active_dc_carriage(axis, 0)
            self.dc[index].activate(axis, mode, toolhead.get_position())
        kin.update_limits(axis, self.get_kin_range(toolhead, mode, axis))
    def _handle_ready(self):
        # Apply the transform later during Klipper initialization to make sure
        # that input shaping can pick up the correct stepper kinematic flags.
        for dc in self.dc:
            for axis in self.axes:
                dc.apply_transform(axis)
    cmd_SET_DUAL_CARRIAGE_help = "Configure the dual carriages mode"
    def cmd_SET_DUAL_CARRIAGE(self, gcmd):
        index = gcmd.get_int('CARRIAGE', minval=0, maxval=1)
        mode = gcmd.get('MODE', PRIMARY).upper()
        if mode not in self.VALID_MODES:
            raise gcmd.error("Invalid mode=%s specified" % (mode,))
        if len(self.axes) == 1 or mode == PRIMARY:
            axis = gcmd.get('AXIS', None)
        else:
            axis = gcmd.get('AXIS')
        if mode == PRIMARY and axis is None:
            for axis_ind in self.axes:
                self.activate_dc_mode(axis_ind, index, mode)
            return
        elif axis is None:
            axis = "XY"[self.axes[0]]
        axis = axis.lower()
        if len(axis) != 1 or axis not in 'xy' or \
                ord(axis) - ord('x') not in self.axes:
            raise gcmd.error("Invalid axis '%s'" % axis)
        if mode in [COPY, MIRROR]:
            if index == 0:
                raise gcmd.error(
                        "Mode=%s is not supported for carriage=0" % (mode,))
            curtime = self.printer.get_reactor().monotonic()
            kin = self.printer.lookup_object('toolhead').get_kinematics()
            if axis not in kin.get_status(curtime)['homed_axes']:
                raise gcmd.error(
                        "Axis %s must be homed prior to enabling mode=%s" %
                        (axis, mode))
        axis_ind = ord(axis) - ord('x')
        self.activate_dc_mode(axis_ind, index, mode)
    cmd_SAVE_DUAL_CARRIAGE_STATE_help = \
            "Save dual carriages modes and positions"
    def cmd_SAVE_DUAL_CARRIAGE_STATE(self, gcmd):
        state_name = gcmd.get('NAME', 'default')
        self.saved_states[state_name] = self.save_dual_carriage_state()
    def save_dual_carriage_state(self):
        pos = self.printer.lookup_object('toolhead').get_position()
        return {
            'carriage_modes': [tuple(dc.mode) for dc in self.dc],
            'axes_positions': {axis : [dc.get_axis_position(axis, pos)
                                       for dc in self.dc]
                               for axis in self.axes},
        }
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
            cur_pos = []
            dst_pos = []
            for i, dc in enumerate(self.dc):
                for axis in self.axes:
                    self.toggle_active_dc_carriage(axis, i)
                    homing_speed = min(homing_speed,
                                       dc.get_carriage(axis).homing_speed)
                cur_pos.append(toolhead.get_position())
                dst_pos.append(list(cur_pos[-1]))
                for axis in self.axes:
                    dst_pos[-1][axis] = saved_state['axes_positions'][axis][i]
            move_pos = list(cur_pos[0])
            for axis in self.axes:
                dl = [dst_pos[i][axis] - cur_pos[i][axis] for i in range(2)]
                primary_ind = 0 if abs(dl[0]) >= abs(dl[1]) else 1
                self.toggle_active_dc_carriage(axis, primary_ind)
                move_pos[axis] = dst_pos[primary_ind][axis]
                dc_mode = INACTIVE if min(abs(dl[0]), abs(dl[1])) < .000000001 \
                        else COPY if dl[0] * dl[1] > 0 else MIRROR
                if dc_mode != INACTIVE:
                    self.dc[1-primary_ind].activate(axis, dc_mode,
                                                    cur_pos[primary_ind])
                    self.dc[1-primary_ind].override_axis_scaling(
                            axis, abs(dl[1-primary_ind] / dl[primary_ind]),
                            cur_pos[primary_ind])
            toolhead.manual_move(move_pos, move_speed or homing_speed)
            toolhead.flush_step_generation()
            # Make sure the scaling coefficients are restored with the mode
            for axis in self.axes:
                self.dc[0].inactivate(axis, move_pos)
                self.dc[1].inactivate(axis, move_pos)
        for i, dc in enumerate(self.dc):
            saved_mode = saved_state['carriage_modes'][i]
            for axis in self.axes:
                self.activate_dc_mode(axis, i, saved_mode[axis])

class DualCarriage:
    ENC_AXES = [b'x', b'y']
    def __init__(self, printer, carriages, active):
        self.carriages = carriages
        mode = (INACTIVE, PRIMARY)[active]
        self.mode = [mode, mode]
        self.offset = [0., 0.]
        scale = 1. if active else 0.
        self.scale = [scale, scale]
        ffi_main, ffi_lib = chelper.get_ffi()
        self.dc_stepper_kinematics = []
        self.orig_stepper_kinematics = []
        steppers = set()
        for c in carriages:
            c_steppers = c.get_steppers()
            if not c_steppers:
                raise printer.config_error(
                        "At least one stepper must be "
                        "associated with carriage: %s" % c.get_name())
            steppers.update(c_steppers)
        for s in steppers:
            sk = ffi_main.gc(ffi_lib.dual_carriage_alloc(), ffi_lib.free)
            orig_sk = s.get_stepper_kinematics()
            ffi_lib.dual_carriage_set_sk(sk, orig_sk)
            for a in self.ENC_AXES:
                ffi_lib.dual_carriage_set_transform(sk, a, 1., 0.)
            self.dc_stepper_kinematics.append(sk)
            self.orig_stepper_kinematics.append(orig_sk)
            s.set_stepper_kinematics(sk)
    def get_carriage(self, axis):
        if len(self.carriages) == 1:
            return self.carriages[0]
        for c in self.carriages:
            if c.get_axis() == axis:
                return c;
        return None
    def is_active(self, axis):
        return self.mode[axis] != INACTIVE
    def get_axis_position(self, axis, position):
        return position[axis] * self.scale[axis] + self.offset[axis]
    def apply_transform(self, axis):
        ffi_main, ffi_lib = chelper.get_ffi()
        for sk in self.dc_stepper_kinematics:
            ffi_lib.dual_carriage_set_transform(
                    sk, self.ENC_AXES[axis], self.scale[axis],
                    self.offset[axis])
    def activate(self, axis, mode, position, old_position=None):
        old_axis_position = self.get_axis_position(axis,
                                                   old_position or position)
        self.scale[axis] = -1. if mode == MIRROR else 1.
        self.offset[axis] = (old_axis_position
                             - position[axis] * self.scale[axis])
        self.apply_transform(axis)
        self.mode[axis] = mode
    def inactivate(self, axis, position):
        self.offset[axis] = self.get_axis_position(axis, position)
        self.scale[axis] = 0.
        self.apply_transform(axis)
        self.mode[axis] = INACTIVE
    def override_axis_scaling(self, axis, new_scale, position):
        old_axis_position = self.get_axis_position(axis, position)
        self.scale[axis] = math.copysign(new_scale, self.scale[axis])
        self.offset[axis] = (old_axis_position
                             - position[axis] * self.scale[axis])
        self.apply_transform(axis)
