# Code for handling the kinematics of cartesian robots
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper
from . import idex_modes

class CartKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        # Get configurable axis names (default to xyz for backward compatibility)
        self.axis_names = config.get('axis_names', 'xyz')
        self.axis_count = len(self.axis_names)

        # Setup axis rails
        self.dual_carriage_axis = None
        self.dual_carriage_rails = []
        self.rails = [stepper.LookupMultiRail(config.getsection('stepper_' + n))
                      for n in self.axis_names]
        for rail, axis_index in zip(self.rails, range(self.axis_count)):
            rail.setup_itersolve('cartesian_stepper_alloc', axis_index)
        ranges = [r.get_range() for r in self.rails]
        self.axes_min = toolhead.Coord(*[r[0] for r in ranges], e=0.)
        self.axes_max = toolhead.Coord(*[r[1] for r in ranges], e=0.)
        self.dc_module = None
        if config.has_section('dual_carriage'):
            dc_config = config.getsection('dual_carriage')
            dc_axis_name = dc_config.getchoice('axis', list(self.axis_names[:2]))  # Only first 2 axes for dual carriage
            self.dual_carriage_axis = self.axis_names.index(dc_axis_name)
            # setup second dual carriage rail
            self.rails.append(stepper.LookupMultiRail(dc_config))
            self.rails[self.axis_count].setup_itersolve('cartesian_stepper_alloc',
                                              self.dual_carriage_axis)
            self.dc_module = idex_modes.DualCarriages(
                    self.printer, [self.rails[self.dual_carriage_axis]],
                    [self.rails[self.axis_count]], axes=[self.dual_carriage_axis],
                    safe_dist=dc_config.getfloat(
                        'safe_distance', None, minval=0.))
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        # Backward compatibility: Z velocity/accel limits for 3rd axis if it exists
        if self.axis_count >= 3:
            self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                                  above=0., maxval=max_velocity)
            self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                               above=0., maxval=max_accel)
        else:
            self.max_z_velocity = max_velocity
            self.max_z_accel = max_accel
        self.limits = [(1.0, -1.0)] * self.axis_count
    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_position(self, stepper_positions):
        rails = self.rails
        if self.dc_module:
            primary_rail = self.dc_module.get_primary_rail(
                    self.dual_carriage_axis)
            rails = (rails[:self.dual_carriage_axis] +
                     [primary_rail] + rails[self.dual_carriage_axis+1:])
        return [stepper_positions[rail.get_name()] for rail in rails]
    def update_limits(self, i, range):
        l, h = self.limits[i]
        # Only update limits if this axis was already homed,
        # otherwise leave in un-homed state.
        if l <= h:
            self.limits[i] = range
    def set_position(self, newpos, homing_axes):
        for i, rail in enumerate(self.rails):
            rail.set_position(newpos)
        for axis_name in homing_axes:
            if axis_name in self.axis_names:
                axis = self.axis_names.index(axis_name)
                if self.dc_module and axis == self.dual_carriage_axis:
                    rail = self.dc_module.get_primary_rail(self.dual_carriage_axis)
                else:
                    rail = self.rails[axis]
                self.limits[axis] = rail.get_range()
    def clear_homing_state(self, clear_axes):
        for axis, axis_name in enumerate(self.axis_names):
            if axis_name in clear_axes:
                self.limits[axis] = (1.0, -1.0)
    def home_axis(self, homing_state, axis, rail):
        # Determine movement
        position_min, position_max = rail.get_range()
        hi = rail.get_homing_info()
        homepos = [None] * (self.axis_count + 1)  # +1 for extruder
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
            if self.dc_module is not None and axis == self.dual_carriage_axis:
                self.dc_module.home(homing_state, self.dual_carriage_axis)
            else:
                self.home_axis(homing_state, axis, self.rails[axis])
    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in range(self.axis_count):
            if (move.axes_d[i]
                and (end_pos[i] < self.limits[i][0]
                     or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise move.move_error("Must home axis first")
                raise move.move_error()
    def check_move(self, move):
        limits = self.limits
        # Check first two axes bounds for initial validation (XY for backward compatibility)
        if self.axis_count >= 2:
            xpos, ypos = move.end_pos[:2]
            if (xpos < limits[0][0] or xpos > limits[0][1]
                or ypos < limits[1][0] or ypos > limits[1][1]):
                self._check_endstops(move)

        # Check Z axis movement if it exists (3rd axis for backward compatibility)
        if self.axis_count >= 3 and not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        elif self.axis_count >= 3:
            # Move with Z - update velocity and accel for slower Z axis
            self._check_endstops(move)
            z_ratio = move.move_d / abs(move.axes_d[2])
            move.limit_speed(
                self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
        else:
            # For non-3-axis configurations, just check all endstops
            self._check_endstops(move)
    def get_status(self, eventtime):
        axes = [a for a, (l, h) in zip(self.axis_names, self.limits) if l <= h]
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max,
        }

def load_kinematics(toolhead, config):
    return CartKinematics(toolhead, config)
