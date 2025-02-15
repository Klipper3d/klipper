# Code for handling the kinematics of corexy robots
#
# Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import stepper

class CoreXYAKinematics:
    def __init__(self, toolhead, config):
        # Setup axis rails
        self.rails = [stepper.LookupMultiRail(config.getsection('stepper_' + n))
                      for n in 'xyza']
        for s in self.rails[1].get_steppers():
            self.rails[0].get_endstops()[0][0].add_stepper(s)
        for s in self.rails[0].get_steppers():
            self.rails[1].get_endstops()[0][0].add_stepper(s)
        self.rails[0].setup_itersolve('corexya_stepper_alloc', b'+')
        self.rails[1].setup_itersolve('corexya_stepper_alloc', b'-')
        self.rails[2].setup_itersolve('cartesian_xyza_stepper_alloc', b'z')
        self.rails[3].setup_itersolve('cartesian_xyza_stepper_alloc', b'a')
        for s in self.get_steppers():
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_step_generator(s.generate_steps)
        config.get_printer().register_event_handler("stepper_enable:motor_off",
                                                    self._motor_off)
        # Setup boundary checks
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', max_velocity, above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        self.max_a_velocity = config.getfloat(
            'max_a_velocity', max_velocity, above=0., maxval=1800)
        self.limits = [(1.0, -1.0)] * 4
        ranges = [r.get_range() for r in self.rails]
        self.axes_min = toolhead.Coord(*[r[0] for r in ranges], e=0.)
        self.axes_max = toolhead.Coord(*[r[1] for r in ranges], e=0.)
    def get_steppers(self):
        return [s for rail in self.rails for s in rail.get_steppers()]
    def calc_position(self, stepper_positions):
        pos = [stepper_positions[rail.get_name()] for rail in self.rails]
        return [0.5 * (pos[0] + pos[1]), 0.5 * (pos[0] - pos[1]), pos[2], pos[3]]
    def set_position(self, newpos, homing_axes):
        for i, rail in enumerate(self.rails):
            rail.set_position(newpos)
            if i in homing_axes:
                self.limits[i] = rail.get_range()
    def note_z_not_homed(self):
        # Helper for Safe Z Home
        self.limits[2] = (1.0, -1.0)
    def home(self, homing_state):
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            rail = self.rails[axis]
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
                if i != 3:
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
    return CoreXYAKinematics(toolhead, config)
