# Code for handling the kinematics of cartesian robots
# with per-axis limits for velocity and acceleration
#
# Copyright (C) 2020-2021  Mael Kerbiriou <piezo.wdimd@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import cartesian

class CartSKinematics(cartesian.CartKinematics):
    def __init__(self, toolhead, config):
        cartesian.CartKinematics.__init__(self, toolhead, config)
        # Setup y axis limits
        max_velocity = toolhead.max_velocity
        max_accel = toolhead.max_accel
        self.max_velocities = [
            config.getfloat('max_%s_velocity' % ax, max_velocity, above=0., maxval=max_velocity)
            for ax in 'xyz'
        ]
        self.max_accels = [
            config.getfloat('max_%s_accel' % ax, max_accel, above=0., maxval=max_accel)
            for ax in 'xyz'
        ]
    def check_move(self, move):
        if not move.is_kinematic_move:
            return
        self._check_endstops(move)
        move.limit_speed(*map(min, zip(*(
            (max_velocity / axis_r, max_accel / axis_r)
            for axis_r, max_velocity, max_accel in zip(map(abs, move.axes_r[:3]), self.max_velocities, self.max_accels)
            if axis_r
        ))))

def load_kinematics(toolhead, config):
    return CartSKinematics(toolhead, config)
