# Code for handling the kinematics of linear delta robots with
# motorized flying extruders
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import stepper, importlib

SLOW_RATIO = 3.

class DeltaMFEKinematics:
    def __init__(self, toolhead, config):
        mod = importlib.import_module('kinematics.delta')
        #Init from DeltaKinematics
        self.delta_kinematics = mod.load_kinematics(toolhead, config)
        rails = self.delta_kinematics.rails
        a_endstop = rails[0].get_homing_info().position_endstop
        #Set up new rail for motorized flying extruder
        rail_mfe = stepper.PrinterRail(
            config.getsection('stepper_mfe'), need_position_minmax = False,
            default_position_endstop = a_endstop)
        self.max_velocity, self.max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', self.max_velocity,
            above=0., maxval=self.max_velocity)
        max_halt_velocity = toolhead.get_max_axis_halt() * SLOW_RATIO
        max_halt_accel = self.max_accel * SLOW_RATIO
        rail_mfe.set_max_jerk(max_halt_velocity, max_halt_accel)
        # Use the cartesian stepper solver for Z
        rail_mfe.setup_itersolve('cartesian_stepper_alloc', 'z')
        # Add rail_mfe to delta_kinematics' list of rails for homing
        self.delta_kinematics.rails = rails + [rail_mfe]
    # All methods just forward calls delta_kinematics
    def get_steppers(self, flags=""):
        return self.delta_kinematics.get_steppers(flags)
    def calc_position(self):
        return self.delta_kinematics.calc_position()
    def set_position(self, newpos, homing_axis):
        self.delta_kinematics.set_position(newpos, homing_axis)
    def home(self, homing_state):
        self.delta_kinematics.home(homing_state)
    def motor_off(self, print_time):
        self.delta_kinematics.motor_off(print_time)
    def check_move(self, move):
        self.delta_kinematics.check_move(move)
    def move(self, print_time, move):
        self.delta_kinematics.move(print_time, move)
    def get_status(self):
        return self.delta_kinematics.get_status()
    def get_calibrate_params(self):
        return self.delta_kinematics.get_calibrate_params()
def load_kinematics(toolhead, config):
    return DeltaMFEKinematics(toolhead, config)
