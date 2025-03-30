# Code for handling the kinematics of cartesian robots
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
# Copyright (C) 2025       Rob Niccum <klober@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper
from . import idex_modes

class CartKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        printer_config = config.getsection('printer')
        # Define axis indices for X, Y, Z mapping
        self.axis_map = {'x': 0, 'y': 1, 'z': 2}
        # ### Axis Determination ###
        # Determine carriage and gantry axes based on configuration
        self.carriage_axis = None
        multi_carriage_sections = [s for s in config.get_prefix_sections('multi_carriage')]
        if multi_carriage_sections:
            self.carriage_axis = multi_carriage_sections[0].getchoice('axis', ['x', 'y'])
            for section in multi_carriage_sections[1:]:
                if section.getchoice('axis', ['x', 'y']) != self.carriage_axis:
                    raise config.error("All multi_carriage sections must have the same axis")
        elif config.has_section('dual_carriage'):
            self.carriage_axis = config.getsection('dual_carriage').getchoice('axis', ['x', 'y'])
        else:
            self.carriage_axis = 'x'  # Default for standard setups
        self.gantry_axis = None
        multi_gantry_sections = [s for s in config.get_prefix_sections('multi_gantry')]
        if multi_gantry_sections:
            self.gantry_axis = multi_gantry_sections[0].getchoice('axis', ['x', 'y'])
            for section in multi_gantry_sections[1:]:
                if section.getchoice('axis', ['x', 'y']) != self.gantry_axis:
                    raise config.error("All multi_gantry sections must have the same axis")
        else:
            self.gantry_axis = 'y' if self.carriage_axis == 'x' else 'x'
        if self.carriage_axis == self.gantry_axis:
            raise config.error("Carriage and gantry axes must be different")
        # ### Rail Setup ###
        # Setup carriage rails
        self.carriage_rails = []
        dc_config = None
        self.carriage_rails = [stepper.LookupMultiRail(config.getsection('stepper_' + self.carriage_axis))]
        if multi_carriage_sections:
            for section in multi_carriage_sections:
                self.carriage_rails.append(stepper.LookupMultiRail(section))
        elif config.has_section('dual_carriage'):
            dc_config = config.getsection('dual_carriage')
            self.carriage_rails.append(stepper.LookupMultiRail(dc_config))
        for rail in self.carriage_rails:
            rail.setup_itersolve('cartesian_stepper_alloc', self.carriage_axis.encode())
        # Setup gantry rails
        self.gantry_rails = [stepper.LookupMultiRail(config.getsection('stepper_' + self.gantry_axis))]
        if multi_gantry_sections:
            for section in multi_gantry_sections:
                self.gantry_rails.append(stepper.LookupMultiRail(section))
        for rail in self.gantry_rails:
            rail.setup_itersolve('cartesian_stepper_alloc', self.gantry_axis.encode())
        # Setup Z rail
        self.z_rail = stepper.LookupMultiRail(config.getsection('stepper_z'))
        self.z_rail.setup_itersolve('cartesian_stepper_alloc', b'z')
        # ### Multi-Component Setup ###
        # Parse homing order options from [printer]
        carriage_homing_order = printer_config.getintlist('carriage_homing_order', None)
        gantry_homing_order = printer_config.getintlist('gantry_homing_order', None)
        # Setup multi_gantries
        if len(self.gantry_rails) > 1:
            gantry_rails_objs = [idex_modes.DualCarriagesRail(rail,
                                    axis=self.axis_map[self.gantry_axis], active=(i==0))
                                    for i, rail in enumerate(self.gantry_rails)]
            self.multi_gantries = idex_modes.MultiGantries(
                config, gantry_rails_objs, self.axis_map[self.gantry_axis], gantry_homing_order)
        else:
            self.multi_gantries = None
        # Setup multi_carriages
        if len(self.carriage_rails) > 1:
            carriage_rails_objs = [idex_modes.DualCarriagesRail(rail,
                                    axis=self.axis_map[self.carriage_axis], active=(i==0))
                                    for i, rail in enumerate(self.carriage_rails)]
            if multi_carriage_sections:
                self.multi_carriages = idex_modes.MultiCarriages(
                    config, carriage_rails_objs, self.axis_map[self.carriage_axis],
                    self.multi_gantries if multi_gantry_sections else None, carriage_homing_order)
                self.dc_module = None
            else:
                self.dc_module = idex_modes.DualCarriages(
                    config.getsection('dual_carriage'), carriage_rails_objs[0],
                    carriage_rails_objs[1], axis=self.axis_map[self.carriage_axis])
                self.multi_carriages = None
        else:
            self.multi_carriages = None
            self.dc_module = None
        # ### Axis Ranges ###
        # Compute ranges for axes_min and axes_max
        carriage_range = self.carriage_rails[0].get_range()
        gantry_range = self.gantry_rails[0].get_range()
        z_range = self.z_rail.get_range()
        ranges = [(0., 0.)] * 3
        ranges[self.axis_map[self.carriage_axis]] = carriage_range
        ranges[self.axis_map[self.gantry_axis]] = gantry_range
        ranges[2] = z_range
        self.axes_min = toolhead.Coord(*[r[0] for r in ranges], e=0.)
        self.axes_max = toolhead.Coord(*[r[1] for r in ranges], e=0.)
        # ### Stepper Registration ###
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        # ### Boundary Checks ###
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.limits = [(1.0, -1.0)] * 3
    def get_steppers(self):
        """Return a list of all steppers from carriage, gantry, and Z rails."""
        steppers = []
        for rail in self.carriage_rails:
            steppers.extend(rail.get_steppers())
        for rail in self.gantry_rails:
            steppers.extend(rail.get_steppers())
        steppers.extend(self.z_rail.get_steppers())
        return steppers
    def calc_position(self, stepper_positions):
        """Calculate the current position based on active carriage and gantry."""
        if self.multi_carriages:
            active_carriage_rail = self.multi_carriages.get_primary_rail().get_rail()
        elif self.dc_module:
            active_carriage_rail = self.dc_module.get_primary_rail().get_rail()
        else:
            active_carriage_rail = self.carriage_rails[0]
        if self.multi_gantries:
            active_gantry_rail = self.multi_gantries.get_primary_rail().get_rail()
        else:
            active_gantry_rail = self.gantry_rails[0]
        carriage_pos = stepper_positions[active_carriage_rail.get_name()]
        gantry_pos = stepper_positions[active_gantry_rail.get_name()]
        z_pos = stepper_positions[self.z_rail.get_name()]
        pos = [0., 0., z_pos]
        pos[self.axis_map[self.carriage_axis]] = carriage_pos
        pos[self.axis_map[self.gantry_axis]] = gantry_pos
        return pos
    def update_limits(self, i, range):
        """Update axis limits if the axis is already homed."""
        l, h = self.limits[i]
        if l <= h:  # Only update if previously homed
            self.limits[i] = range
    def set_position(self, newpos, homing_axes):
        """Set the position for all rails and update limits for homed axes."""
        for rail in self.carriage_rails:
            rail.set_position(newpos)
        for rail in self.gantry_rails:
            rail.set_position(newpos)
        self.z_rail.set_position(newpos)
        for axis_name in homing_axes:
            axis = self.axis_map[axis_name]
            if axis == self.axis_map[self.carriage_axis]:
                if self.multi_carriages:
                    active_rail = self.multi_carriages.get_primary_rail().get_rail()
                elif self.dc_module:
                    active_rail = self.dc_module.get_primary_rail().get_rail()
                else:
                    active_rail = self.carriage_rails[0]
            elif axis == self.axis_map[self.gantry_axis]:
                if self.multi_gantries:
                    active_rail = self.multi_gantries.get_primary_rail().get_rail()
                else:
                    active_rail = self.gantry_rails[0]
            elif axis == 2:  # Z
                active_rail = self.z_rail
            self.limits[axis] = active_rail.get_range()
    def clear_homing_state(self, clear_axes):
        """Clear the homing state for specified axes."""
        for axis, axis_name in enumerate("xyz"):
            if axis_name in clear_axes:
                self.limits[axis] = (1.0, -1.0)
    def home_axis(self, homing_state, axis, rail):
        """Home a single axis using the provided rail."""
        position_min, position_max = rail.get_range()
        hi = rail.get_homing_info()
        homepos = [None, None, None, None]
        homepos[axis] = hi.position_endstop
        forcepos = list(homepos)
        if hi.positive_dir:
            forcepos[axis] -= 1.5 * (hi.position_endstop - position_min)
        else:
            forcepos[axis] += 1.5 * (position_max - hi.position_endstop)
        homing_state.home_rails([rail], forcepos, homepos)
    def home(self, homing_state):
        """Home each axis independently, delegating to multi-component logic where applicable."""
        for axis in homing_state.get_axes():
            if axis == self.axis_map[self.carriage_axis]:
                if self.multi_carriages:
                    self.multi_carriages.home(homing_state)
                elif self.dc_module:
                    self.dc_module.home(homing_state)
                else:
                    self.home_axis(homing_state, axis, self.carriage_rails[0])
            elif axis == self.axis_map[self.gantry_axis]:
                if self.multi_gantries:
                    self.multi_gantries.home(homing_state)
                else:
                    self.home_axis(homing_state, axis, self.gantry_rails[0])
            elif axis == 2:  # Z
                self.home_axis(homing_state, axis, self.z_rail)
    def _check_endstops(self, move):
        """Check if the move exceeds current limits."""
        end_pos = move.end_pos
        for i in (0, 1, 2):
            if (move.axes_d[i]
                and (end_pos[i] < self.limits[i][0]
                     or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise move.move_error("Must home axis first")
                raise move.move_error()
    def check_move(self, move):
        """Validate the move against limits and adjust Z-axis speed if necessary."""
        limits = self.limits
        xpos, ypos, zpos = move.end_pos[:3]
        if (xpos < limits[0][0] or xpos > limits[0][1]
            or ypos < limits[1][0] or ypos > limits[1][1]
                or zpos < limits[2][0] or zpos > limits[2][1]):
            self._check_endstops(move)
        if not move.axes_d[2]:
            return  # Normal XY move uses default speeds
        self._check_endstops(move)
        z_ratio = move.move_d / abs(move.axes_d[2])
        move.limit_speed(self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
    def get_status(self, eventtime):
        """Return the current homing status and axis ranges."""
        axes = [a for a, (l, h) in zip("xyz", self.limits) if l <= h]
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max,
        }
def load_kinematics(toolhead, config):
    return CartKinematics(toolhead, config)