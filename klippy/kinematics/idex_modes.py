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
    VALID_MODES = [INACTIVE, PRIMARY, COPY, MIRROR]
    def __init__(self, printer, primary_rails, dual_rails, axes, safe_dist):
        self.printer = printer
        self._init_steppers(primary_rails + dual_rails)
        safe_dist = list(safe_dist)
        for i, dc in enumerate(dual_rails):
            if dc is None or safe_dist[i] is not None:
                continue
            pc = primary_rails[i]
            safe_dist[i] = min(abs(pc.position_min - dc.position_min),
                               abs(pc.position_max - dc.position_max))
        activated = [False] * 3
        self.primary_rails = []
        for i, c in enumerate(primary_rails):
            activate, activated[axes[i]] = not activated[axes[i]], True
            dc_rail = DualCarriagesRail(
                    printer, c, dual_rails[i], axes[i], safe_dist[i],
                    active=activate)
            self.primary_rails.append(dc_rail)
        self.dual_rails = [
                DualCarriagesRail(printer, c, primary_rails[i],
                                  axes[i], safe_dist[i], active=False)
                if c is not None else None
                for i, c in enumerate(dual_rails)]
        self.dc_rails = collections.OrderedDict(
                [(c.rail.get_name(short=True), c)
                 for c in self.primary_rails + self.dual_rails
                 if c is not None])
        self.saved_states = {}
        self.axes = sorted(set(axes))
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
            if rail is None:
                continue
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
        homing_rails = [r for r in self.primary_rails if r.axis == axis]
        for dc_rail in homing_rails:
            dcs = [dc for dc in self.dc_rails.values()
                   if dc_rail.rail in [dc.rail, dc.dual_rail]]
            if len(dcs) > 1 and (self.get_dc_order(dcs[0], dcs[1]) > 0) != \
                    dcs[0].rail.get_homing_info().positive_dir:
                # The second carriage must home first, because the carriages
                # home in the same direction and the first carriage homes on
                # the second one, so reversing the oder
                dcs.reverse()
            for dc in dcs:
                self.toggle_active_dc_rail(dc)
                kin.home_axis(homing_state, dc.axis, dc.rail)
        # Restore the first rail as primary after all homed
        self.activate_dc_mode(homing_rails[0], PRIMARY)
    def get_status(self, eventtime=None):
        status = {'carriages' : {dc.get_name() : dc.mode
                                 for dc in self.dc_rails.values()}}
        if len(self.dc_rails) == 2:
            status.update({('carriage_%d' % (i,)) : dc.mode
                           for i, dc in enumerate(self.dc_rails.values())})
        return status
    def get_kin_range(self, toolhead, axis):
        pos = toolhead.get_position()
        axis_pos = pos[axis]
        range_min, range_max = -1e10, 1e10
        for carriage in self.dc_rails.values():
            if carriage.axis != axis:
                continue
            dcs = [carriage] + [dc for dc in self.dc_rails.values()
                                if carriage.rail is dc.dual_rail]
            axes_pos = [dc.get_axis_position(pos) for dc in dcs]
            # Check how dcs[0] affects the motion range
            if not dcs[0].is_active():
                continue
            elif dcs[0].mode == PRIMARY:
                range_min = max(range_min, dcs[0].rail.position_min)
                range_max = min(range_max, dcs[0].rail.position_max)
            elif dcs[0].mode == COPY:
                range_min = max(range_min, axis_pos
                                + dcs[0].rail.position_min - axes_pos[0])
                range_max = min(range_max, axis_pos
                                + dcs[0].rail.position_max - axes_pos[0])
            elif dcs[0].mode == MIRROR:
                range_min = max(range_min, axis_pos
                                + axes_pos[0] - dcs[0].rail.position_max)
                range_max = min(range_max, axis_pos
                                + axes_pos[0] - dcs[0].rail.position_min)
            safe_dist = dcs[0].safe_dist
            if not safe_dist or len(dcs) == 1:
                continue
            if dcs[0].mode == dcs[1].mode or \
                    set((dcs[0].mode, dcs[1].mode)) == set((PRIMARY, COPY)):
                # dcs[0] and dcs[1] carriages move in the same direction and
                # cannot collide with each other
                continue

            # Compute how much dcs[0] can move towards dcs[1]
            dcs_dist = axes_pos[1] - axes_pos[0]
            if self.get_dc_order(dcs[0], dcs[1]) > 0:
                safe_move_dist = dcs_dist + safe_dist
            else:
                safe_move_dist = dcs_dist - safe_dist
            if dcs[1].is_active():
                safe_move_dist *= 0.5

            if dcs[0].mode in (PRIMARY, COPY):
                if self.get_dc_order(dcs[0], dcs[1]) > 0:
                    range_min = max(range_min, axis_pos + safe_move_dist)
                else:
                    range_max = min(range_max, axis_pos + safe_move_dist)
            else:  # dcs[0].mode == MIRROR
                if self.get_dc_order(dcs[0], dcs[1]) > 0:
                    range_max = min(range_max, axis_pos - safe_move_dist)
                else:
                    range_min = max(range_min, axis_pos - safe_move_dist)

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
            dc.activate(mode, toolhead.get_position())
        kin.update_limits(axis, self.get_kin_range(toolhead, axis))
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
            if self.get_primary_rail(dc_rail.axis) in [None, dc_rail.rail]:
                raise gcmd.error(
                        "Must activate another carriage as PRIMARY first")
            curtime = self.printer.get_reactor().monotonic()
            kin = self.printer.lookup_object('toolhead').get_kinematics()
            axis = 'xyz'[dc_rail.axis]
            if axis not in kin.get_status(curtime)['homed_axes']:
                raise gcmd.error(
                        "Axis %s must be homed prior to enabling mode=%s" %
                        (axis.upper(), mode))
        if mode == INACTIVE:
            active_dcs = [dc for dc in self.dc_rails.values()
                          if dc.is_active() and dc.axis == dc_rail.axis]
            if active_dcs == [dc_rail]:
                raise gcmd.error(
                        "Cannot deactivate the only active carriage for axis %s"
                        % 'XYZ'[dc_rail.axis])
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
                                       for dc in self.dc_rails.values()},
                'toolhead_position': tuple(pos)}
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
        move_pos = list(toolhead.get_position())
        dcs = list(self.dc_rails.values())
        if move:
            homing_speed = 99999999.
            cur_pos = []
            carriage_positions = saved_state['carriage_positions']
            for dc in dcs:
                self.toggle_active_dc_rail(dc)
                homing_speed = min(homing_speed, dc.rail.homing_speed)
                cur_pos.append(toolhead.get_position())
            dl = [carriage_positions[dc.get_name()] - cur_pos[i][dc.axis]
                  for i, dc in enumerate(dcs)]
            for axis in self.axes:
                dc_ind = [i for i, dc in enumerate(dcs) if dc.axis == axis]
                abs_dl = [abs(dl[i]) for i in dc_ind]
                primary_ind = dc_ind[abs_dl.index(max(abs_dl))]
                primary_dc = dcs[primary_ind]
                self.toggle_active_dc_rail(primary_dc)
                move_pos[axis] = carriage_positions[primary_dc.get_name()]
                for secondary_ind in dc_ind:
                    if secondary_ind == primary_ind:
                        continue
                    if min(abs(dl[primary_ind]),
                           abs(dl[secondary_ind])) < .000000001:
                        continue
                    if dl[primary_ind] * dl[secondary_ind] > 0:
                        dc_mode = COPY
                    else:
                        dc_mode = MIRROR
                    dcs[secondary_ind].activate(dc_mode, cur_pos[primary_ind])
                    dcs[secondary_ind].override_axis_scaling(
                            abs(dl[secondary_ind] / dl[primary_ind]),
                            cur_pos[primary_ind])
            toolhead.manual_move(move_pos, move_speed or homing_speed)
            toolhead.flush_step_generation()
        # Inactivate all carriages in order to restore scaling coefficients
        # (if a move was requested) and correctly compute kinematics limits
        for dc in dcs:
            dc.inactivate(move_pos)
        # Restore toolhead position in case some axes have no primary carriages
        for axis in self.axes:
            move_pos[axis] = saved_state['toolhead_position'][axis]
        toolhead.set_position(move_pos)
        saved_modes = saved_state['carriage_modes']
        saved_primary_dcs = [dc for dc in self.dc_rails.values()
                             if saved_modes[dc.get_name()] == PRIMARY]
        # First activate all primary carriages
        for dc in saved_primary_dcs:
            self.activate_dc_mode(dc, PRIMARY)
        # Then set the modes the remaining carriages
        for dc in self.dc_rails.values():
            if dc not in saved_primary_dcs:
                self.activate_dc_mode(dc, saved_modes[dc.get_name()])

class DualCarriagesRail:
    ENC_AXES = [b'x', b'y']
    def __init__(self, printer, rail, dual_rail, axis, safe_dist, active):
        self.printer = printer
        self.rail = rail
        self.dual_rail = dual_rail
        self.sks = [s.get_stepper_kinematics() for s in rail.get_steppers()]
        self.axis = axis
        self.safe_dist = safe_dist
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
