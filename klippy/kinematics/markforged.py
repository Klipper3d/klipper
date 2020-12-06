# Code for handling the kinematics of markforged robots
#
# Copyright (C) 2020-2021  Fabrice Gallet <tircown@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math, copy
import stepper, homing

class MarkforgedKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        printer_config = config.getsection('printer')
        # itersolve parameters
        self.second_axis = printer_config.getchoice(
            'gantry', {'xy': 'y', 'xz': 'z'}, 'xy')
        self.rails = [ stepper.PrinterRail(config.getsection('stepper_x')),
                       stepper.LookupMultiRail(config.getsection('stepper_y')),
                       stepper.LookupMultiRail(config.getsection('stepper_z'))]
        self.rails[1].get_endstops()[0][0].add_stepper(
            self.rails[0].get_steppers()[0])
        self.rails[0].setup_itersolve(
            'markforged_stepper_alloc', self.second_axis, 'p')
        self.rails[1].setup_itersolve('cartesian_stepper_alloc', 'y')
        self.rails[2].setup_itersolve('cartesian_stepper_alloc', 'z')
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        self.printer.register_event_handler("stepper_enable:motor_off",
                                                    self._motor_off)
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', max_velocity, above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        self.limits = [(1.0, -1.0)] * 3
        # Setup stepper max halt velocity
        max_halt_velocity = toolhead.get_max_axis_halt()
        self.rails[0].set_max_jerk(max_halt_velocity, max_accel)
        self.rails[1].set_max_jerk(max_halt_velocity, max_accel)
        self.rails[2].set_max_jerk(
            min(max_halt_velocity, self.max_z_velocity), self.max_z_accel)
        if config.has_section('dual_carriage'):
            self.printer.add_object("dual_carriage", self)
            dc_config = config.getsection('dual_carriage')
            # dummy for cartesian config users
            dc_axis = dc_config.getchoice('axis', {'x': 'x'}, default='x')
            # setup second dual carriage rail
            self.rails.append(stepper.PrinterRail(dc_config))
            self.dc_rails = [self.rails[0], self.rails[3]]
            self.dc_rails[0].kin_infos = {
                'active': True,
                'positive_dir': True,
                'position_at_change': None}
            self.dc_rails[1].kin_infos = {
                'active': False,
                'positive_dir': True,
                'position_at_change': None}
            self.rails[1].get_endstops()[0][0].add_stepper(
                self.rails[3].get_steppers()[0])
            self.rails[3].setup_itersolve(
                'markforged_stepper_alloc',
                self.second_axis,
                'w')
            self.rails[3].set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(self.rails[3].generate_steps)
            self.rails[3].set_max_jerk(max_halt_velocity, max_accel)
            gcode = self.printer.lookup_object('gcode')
            # dual-carriage commands
            gcode.register_command(
                'SET_DUAL_CARRIAGE', self.cmd_SET_DUAL_CARRIAGE,
                desc=self.cmd_SET_DUAL_CARRIAGE_help)
            gcode.register_command(
                'SET_DUAL_CARRIAGE_MODE', self.cmd_SET_DUAL_CARRIAGE_MODE,
                desc=self.cmd_SET_DUAL_CARRIAGE_MODE_help)
    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_tag_position(self):
        pos = [rail.get_tag_position() for rail in self.rails]
        if self.second_axis == 'y':
            return [pos[0] + pos[1], pos[1], pos[2]]
        elif self.second_axis == 'z':
            return [pos[0] + pos[2], pos[1], pos[2]]
    def set_position(self, newpos, homing_axes):
        for i, rail in enumerate(self.rails):
            rail.set_position(newpos)
            if i in homing_axes:
                self.limits[i] = rail.get_range()
    def note_z_not_homed(self):
        # Helper for Safe Z Home
        self.limits[2] = (1.0, -1.0)
    def _home_axis(self, homing_state, axis, rail):
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
        for axis in homing_state.get_axes():
            if (hasattr(self, 'dc_rails') and axis == 0):
                kin_init_state = (
                    copy.deepcopy(self.dc_rails[0].kin_infos),
                    copy.deepcopy(self.dc_rails[1].kin_infos))
                for dc_rail in self.dc_rails:
                    self._toggle_active_dc_rail(dc_rail)
                    self._home_axis(homing_state, axis, dc_rail)
                # set back duplicate or mirror mode
                if (kin_init_state[0]['active'] == kin_init_state[1]['active']
                    and kin_init_state[0]['active'] == True):
                    toolhead = self.printer.lookup_object('toolhead')
                    toolhead.flush_step_generation()
                    pos = toolhead.get_position()
                    toolhead.manual_move(
                        [kin_init_state[1]['position_at_change']] + pos[1:],
                        self.dc_rails[1].homing_speed)
                    self._toggle_active_dc_rail(self.dc_rails[0])
                    toolhead.manual_move(
                        [kin_init_state[0]['position_at_change']] + pos[1:],
                        self.dc_rails[0].homing_speed)
                    self._activate_dc_mode(('MIRRORED', 'DUPLICATION')
                        [kin_init_state[1]['positive_dir']])
                # set carriage 0 active
                elif kin_init_state[0]['active'] is True:
                    self._toggle_active_dc_rail(self.dc_rails[0])
                # continue with carriage 1 active
                else:
                    self.limits[0] = self.rails[0].get_range()
            else:
                self._home_axis(homing_state, axis, self.rails[axis])
    def _motor_off(self, print_time):
        self.limits = [(1.0, -1.0)] * 3
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
        dc_active = None
        dc_mode = None
        if hasattr(self, 'dc_rails'):
            if (self.dc_rails[0].kin_infos['active']
                == self.dc_rails[1].kin_infos['active'] 
                == True):
                dc_active = 'BOTH'
                if self.dc_rails[1].kin_infos['positive_dir']:
                    dc_mode = 'DUPLICATION'
                else:
                    dc_mode = 'MIRRORED'
            else:
                dc_active = ('T0', 'T1')[self.dc_rails[1].kin_infos['active']]
                dc_mode = 'FULL_CONTROL'
        return {
            'homed_axes': "".join(axes),
            'dc_mode': dc_mode,
            'dc_active': dc_active}
    # Dual carriage support
    def _dc_rail_stepper_alloc(self, rail, toolhead, position, active,
        positive_dir =True):
        rail.kin_infos = {
            'active': active,
            'positive_dir': positive_dir,
            'position_at_change': position[0]
            }
        rail.set_trapq(None)
        rail.setup_itersolve(
            'markforged_stepper_alloc',
            self.second_axis,
            'w' if not active else 'p' if positive_dir else 'n')
        rail.set_position(position)
        rail.set_trapq(toolhead.get_trapq())
    def _toggle_active_dc_rail(self, rail):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        pos = toolhead.get_position()
        for dc_rail in enumerate(self.dc_rails):
            i, r = dc_rail
            if rail != r:
                self._dc_rail_stepper_alloc(r, toolhead, pos, active=False)
                self.rails[3] = r
            elif rail.kin_infos['active'] is False:
                newpos = [rail.kin_infos['position_at_change']] + pos[1:]
                self._dc_rail_stepper_alloc(r, toolhead, newpos, active=True)
                self.rails[0] = r
                toolhead.set_position(newpos)
                if self.limits[0][0] <= self.limits[0][1]:
                    self.limits[0] = rail.get_range()
    def _calc_dual_x_position(self, pos_x):
        dc0_pos = self.dc_rails[0].kin_infos['position_at_change']
        dc1_pos = self.dc_rails[1].kin_infos['position_at_change']
        # duplication
        if (self.dc_rails[0].kin_infos['active'] is True
            and self.dc_rails[1].kin_infos['active'] is True
            and self.dc_rails[1].kin_infos['positive_dir'] is True):
            return pos_x, dc1_pos - dc0_pos + pos_x
        # mirrored
        elif (self.dc_rails[0].kin_infos['active'] is True
            and self.dc_rails[1].kin_infos['active'] is True
            and self.dc_rails[1].kin_infos['positive_dir'] is False):
            return pos_x, dc1_pos + dc0_pos - pos_x
        # full-control: T0 active
        elif (self.dc_rails[0].kin_infos['active'] is True
            and self.dc_rails[1].kin_infos['active'] is False):
            return pos_x, dc1_pos
        # full-control: T1 active
        elif (self.dc_rails[0].kin_infos['active'] is False
            and self.dc_rails[1].kin_infos['active'] is True):
            return dc0_pos, pos_x
    def _activate_dc_mode(self, mode):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        pos = toolhead.get_position()
        dc0_pos, dc1_pos = self._calc_dual_x_position(pos[0])
        self._dc_rail_stepper_alloc(
            self.dc_rails[0],
            toolhead,
            [dc0_pos] + pos[1:],
            active=True)
        self.rails[0] = self.dc_rails[0]
        self.rails[3] = self.dc_rails[1]
        if 'FULL_CONTROL' == mode:
            self._dc_rail_stepper_alloc(
                self.dc_rails[1],
                toolhead,
                [dc1_pos] + pos[1:],
                active=False)
            toolhead.set_position([dc0_pos] + pos[1:])
            self.limits[0] = self.dc_rails[0].get_range()
            self.printer.lookup_object('gcode').respond_info(
                "Dual carriage mode is now set to %s" % 'FULL_CONTROL')
        elif 'DUPLICATION' == mode:
            self._dc_rail_stepper_alloc(
                self.dc_rails[1],
                toolhead,
                [dc1_pos] + pos[1:],
                active=True,
                positive_dir=True)
            toolhead.set_position([dc0_pos] + pos[1:])
            dc_rail_min= min(
                self.dc_rails[0].position_min,
                self.dc_rails[1].position_min)
            dc_rail_max= max(
                self.dc_rails[0].position_max,
                self.dc_rails[1].position_max)
            if self.dc_rails[0].get_homing_info().positive_dir is False:
                self.limits[0] = (
                    dc_rail_min,
                    math.floor(dc_rail_max - dc1_pos + dc0_pos))
            else:
                self.limits[0] = (
                    math.ceil(dc0_pos - dc1_pos - dc_rail_min),
                    dc_rail_max)
            self.printer.lookup_object('gcode').respond_info(
                "Dual carriage mode is now set to %s" % 'DUPLICATION')
        elif 'MIRRORED' == mode:
            self._dc_rail_stepper_alloc(
                self.dc_rails[1],
                toolhead,
                [dc1_pos] + pos[1:],
                active=True,
                positive_dir=False)
            toolhead.set_position([dc0_pos] + pos[1:])
            dc_rail_min= min(
                self.dc_rails[0].position_min,
                self.dc_rails[1].position_min)
            dc_rail_max= max(
                self.dc_rails[0].position_max,
                self.dc_rails[1].position_max)
            dc_rail_diff= abs(
                self.dc_rails[0].position_min - self.dc_rails[1].position_min)
            if self.dc_rails[0].get_homing_info().positive_dir is False:
                self.limits[0] = (
                    math.ceil(dc0_pos - min(
                        abs(dc0_pos - dc_rail_min),
                        abs(dc0_pos - dc_rail_max),
                        abs(dc1_pos - dc_rail_min),
                        abs(dc1_pos - dc_rail_max))),
                    math.floor(0.5 * (dc1_pos + dc0_pos - dc_rail_diff)))
            else:
                self.limits[0] = (
                    math.ceil(0.5 * (dc1_pos + dc0_pos + dc_rail_diff)),
                    math.floor(dc0_pos + min(
                        abs(dc0_pos - dc_rail_min),
                        abs(dc0_pos - dc_rail_max),
                        abs(dc1_pos - dc_rail_min),
                        abs(dc1_pos - dc_rail_max))))
            self.printer.lookup_object('gcode').respond_info(
                "Dual carriage mode is now set to %s" % 'MIRRORED')
        else:
            raise self.printer.lookup_object('gcode').error(
                "'%s' is not a valid mode." % mode)
    cmd_SET_DUAL_CARRIAGE_help = "Set which carriage is active"
    def cmd_SET_DUAL_CARRIAGE(self, gcmd):
        carriage = gcmd.get_int('CARRIAGE', minval=0, maxval=1)
        force_homing = gcmd.get_int('HOMING', 0, minval=0, maxval=1)
        if (not(self.dc_rails[0].kin_infos['active']
                == self.dc_rails[1].kin_infos['active'] == True)
            and self.dc_rails[carriage] != self.rails[0]):
            self._toggle_active_dc_rail(self.dc_rails[carriage])
            if force_homing:
                homing_state = homing.Homing(self.printer)
                self._home_axis(homing_state, 0, self.dc_rails[carriage])
    cmd_SET_DUAL_CARRIAGE_MODE_help = "Set which mode is active"
    def cmd_SET_DUAL_CARRIAGE_MODE(self, gcmd):
        mode = gcmd.get('MODE')
        self._activate_dc_mode(mode)

def load_kinematics(toolhead, config):
    return MarkforgedKinematics(toolhead, config)