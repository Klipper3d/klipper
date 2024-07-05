# Code for handling the kinematics of cartesian robots
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper
import math
from . import idex_modes

class CartKinematics:
    def __init__(self, toolhead, config):
        self.printer = config.get_printer()
        # Setup axis rails
        self.dual_carriage_axis = None
        self.dual_carriage_rails = []
        self.rails = [stepper.LookupMultiRail(config.getsection('stepper_' + n))
                      for n in 'xyza']
        for rail, axis in zip(self.rails, 'xyza'):
            rail.setup_itersolve('cartesian_xyza_stepper_alloc', axis.encode())
        ranges = [r.get_range() for r in self.rails]
        self.axes_min = toolhead.Coord(*[r[0] for r in ranges], e=0.)
        self.axes_max = toolhead.Coord(*[r[1] for r in ranges], e=0.)
        self.dc_module = None
        if config.has_section('dual_carriage'):
            dc_config = config.getsection('dual_carriage')
            dc_axis = dc_config.getchoice('axis', {'x': 'x', 'y': 'y'})
            self.dual_carriage_axis = {'x': 0, 'y': 1}[dc_axis]
            # setup second dual carriage rail
            self.rails.append(stepper.LookupMultiRail(dc_config))
            self.rails[4].setup_itersolve('cartesian_xyza_stepper_alloc',
                                          dc_axis.encode())
            dc_rail_0 = idex_modes.DualCarriagesRail(
                    self.rails[self.dual_carriage_axis],
                    axis=self.dual_carriage_axis, active=True)
            dc_rail_1 = idex_modes.DualCarriagesRail(
                    self.rails[4], axis=self.dual_carriage_axis, active=False)
            self.dc_module = idex_modes.DualCarriages(
                    dc_config, dc_rail_0, dc_rail_1,
                    axis=self.dual_carriage_axis)
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        self.printer.register_event_handler("stepper_enable:motor_off",
                                            self._motor_off)
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat('max_z_velocity', max_velocity,
                                              above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat('max_z_accel', max_accel,
                                           above=0., maxval=max_accel)
        self.max_a_velocity = config.getfloat('max_a_velocity', max_velocity,
                                              above=0., maxval=1800)
        self.limits = [(1.0, -1.0)] * 4
    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_position(self, stepper_positions):
        return [stepper_positions[rail.get_name()] for rail in self.rails]
    def update_limits(self, i, range):
        l, h = self.limits[i]
        # Only update limits if this axis was already homed,
        # otherwise leave in un-homed state.
        if l <= h:
            self.limits[i] = range
    def override_rail(self, i, rail):
        self.rails[i] = rail
    def set_position(self, newpos, homing_axes):
        for i, rail in enumerate(self.rails):
            rail.set_position(newpos)
            if i in homing_axes:
                self.limits[i] = rail.get_range()
    def note_z_not_homed(self):
        # Helper for Safe Z Home
        self.limits[2] = (1.0, -1.0)
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
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            if self.dc_module is not None and axis == self.dual_carriage_axis:
                self.dc_module.home(homing_state)
            else:
                self.home_axis(homing_state, axis, self.rails[axis])
    def _motor_off(self, print_time):
        self.limits = [(1.0, -1.0)] * 4
    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in (0, 1, 2, 3):
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
            
        min_speed_a = math.sqrt(math.pow(move.axes_d[3] / move.min_move_t,2))
        logging.info("### Check Move Start ### \nMinimum Speed A: %f " % (min_speed_a))
        
        if move.axes_d[2] and (min_speed_a > self.max_a_velocity):
            self._check_endstops(move)
            z_ratio = move.move_d / abs(move.axes_d[2])
            a_speed_ratio =   self.max_a_velocity / min_speed_a * 0.9
            z_limit_speed = self.max_z_velocity * z_ratio
            z_limit_accel = self.max_z_accel * z_ratio
            a_limit_speed = math.sqrt(move.max_cruise_v2) * a_speed_ratio
            a_limit_accel = move.accel * a_speed_ratio
            
            limit_speed = a_limit_speed if z_limit_speed > a_limit_speed else z_limit_speed
            limit_accel = a_limit_accel if z_limit_accel > a_limit_accel else z_limit_accel
            
            move.limit_speed(limit_speed, limit_accel)
            logging.info("# A and Z Axis are Moving | Limit Speed: %f | Limit Accel: %f" % (limit_speed, limit_accel))
            
        elif move.axes_d[2]: 
            self._check_endstops(move)
            z_ratio = move.move_d / abs(move.axes_d[2])
            z_limit_speed = self.max_z_velocity * z_ratio
            z_limit_accel = self.max_z_accel * z_ratio
            move.limit_speed(z_limit_speed, z_limit_accel)
            logging.info("# Z Axis is Moving | Limit Speed: %f | Limit Accel: %f" % (z_limit_speed,z_limit_accel))
            
        elif min_speed_a > self.max_a_velocity:
            self._check_endstops(move)
            a_speed_ratio =   self.max_a_velocity / min_speed_a * 0.9
            a_limit_speed = math.sqrt(move.max_cruise_v2) * a_speed_ratio
            a_limit_accel = move.accel * a_speed_ratio
            move.limit_speed(a_limit_speed, a_limit_accel)
            logging.info("# A Axis is Moving | Limit Speed: %f | Limit Accel: %f" % (a_limit_speed,a_limit_accel))
        
        logging.info("### Check Move End ###")
        
    def get_status(self, eventtime):
        axes = [a for a, (l, h) in zip("xyza", self.limits) if l <= h]
        return {
            'homed_axes': "".join(axes),
            'axis_minimum': self.axes_min,
            'axis_maximum': self.axes_max,
        }

def load_kinematics(toolhead, config):
    return CartKinematics(toolhead, config)
