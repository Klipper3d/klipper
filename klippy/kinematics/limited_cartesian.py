# -*- coding: utf-8 -*-
# Code for handling the kinematics of cartesian robots
# with per-axis limits for velocity and acceleration
#
# Copyright (C) 2020-2021  Mael Kerbiriou <piezo.wdimd@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Usage:
# Copying this file under `klipper/klippy/kinematics/` should be enough (click
# the `raw` button on github, then save as)
# Then your config's [printer] should look like:
# [printer]
# kinematics: limited_cartesian
# max_velocity: 875
# max_x_velocity: 750
# max_y_velocity: 450
# max_z_velocity: 5
# max_accel: 15000
# max_x_accel: 12000
# max_y_accel: 9000
# max_z_accel: 100
# scale_xy_accel: [True/False, default False]
#
# max_accel/velocity are usually the hypotenuses of X and Y values, For example:
# with max_x_accel = 15000 and max_y_accel = 8000, the recommended value is
# max_accel = 17000.
#
# If scale_xy_accel is False, `max_accel`, set by M204 or SET_VELOCITY_LIMIT,
# acts as a third limit. This means that moves with an acceleration lower than
# max_x_accel and max_y_accel, have no per-axis limits applied. When True,
# max_x_accel and max_y_accel are scaled by the ratio of the dynamically set
# acceleration and the max_accel value from the config. This means that the
# actual acceleration will always depend on the direction.

from sys import float_info
from math import hypot, atan2, pi

from . import cartesian

EPSILON = float_info.epsilon
SQRT2 = hypot(1., 1.)

class LimitedCartKinematics(cartesian.CartKinematics):
    def __init__(self, toolhead, config):
        cartesian.CartKinematics.__init__(self, toolhead, config)
        # Setup y axis limits
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.config_max_velocity = max_velocity
        self.config_max_accel = max_accel
        self.max_velocities = [
            config.getfloat('max_%s_velocity' % ax, SQRT2 * max_velocity,
                             above=0., maxval=max_velocity)
            for ax in 'xyz']
        self.max_accels = [
            config.getfloat('max_%s_accel' % ax, max_accel,
                            above=0., maxval=max_accel)
            for ax in 'xyz']
        self.scale_per_axis = config.getboolean('scale_xy_accel', False)
        config.get_printer().lookup_object('gcode').register_command(
            'SET_KINEMATICS_LIMIT', self.cmd_SET_KINEMATICS_LIMIT)
    def cmd_SET_KINEMATICS_LIMIT(self,gcmd):
        config_max_velocity = self.config_max_velocity
        config_max_accel = self.config_max_accel
        self.max_velocities = [
            gcmd.get_float('%s_VELOCITY' % ax, max_v, above=0., maxval=config_max_velocity)
            for max_v, ax in zip(self.max_velocities, 'XYZ')
        ]
        self.max_accels = [
            gcmd.get_float('%s_ACCEL' % ax, max_a, above=0., maxval=config_max_accel)
            for max_a, ax in zip(self.max_accels, 'XYZ')
        ]
        self.scale_per_axis = bool(gcmd.get_int('SCALE', self.scale_per_axis, minval=0, maxval=1))
        msg = ("x,y,z max_velocities: %r\n"
               "x,y,z max_accels: %r\n") % (
                   self.max_velocities,
                   self.max_accels)
        if self.scale_per_axis:
            msg += "Per axis accelerations limits scale with current acceleration.\n"
        else:
            msg += "Per axis accelerations limits are independant of current acceleration.\n"
        msg += ("Maximum XY velocity of %.1f mm/s reached on %.0f° diagonals.\n"
                "Maximum XY acceleration of %.0f mm/s² reached on %.0f° diagonals.") % (
            hypot(self.max_velocities[0], self.max_velocities[1]),
            180*atan2(self.max_velocities[1], self.max_velocities[0]) / pi,
            hypot(self.max_accels[0], self.max_accels[1]),
            180*atan2(self.max_accels[1], self.max_accels[0]) / pi
        )
        gcmd.respond_info(msg)
    def check_move(self, move):
        if not move.is_kinematic_move:
            return
        self._check_endstops(move)
        x_r, y_r, z_r = move.axes_r[:3]
        x_max_v, y_max_v, z_max_v = self.max_velocities
        x_max_a, y_max_a, z_max_a = self.max_accels
        x_r = max(abs(x_r), EPSILON)
        y_r = max(abs(y_r), EPSILON)
        max_v = min(x_max_v / x_r, y_max_v / y_r)
        max_a = min(x_max_a / x_r, y_max_a / y_r)
        if self.scale_per_axis:
            _, toolhead_max_a = move.toolhead.get_max_velocity()
            max_a *= toolhead_max_a / self.config_max_accel
        if z_r:
            z_r = abs(z_r)
            max_v = min(max_v, z_max_v / z_r)
            max_a = min(max_a, z_max_a / z_r)
        move.limit_speed(max_v, max_a)

def load_kinematics(toolhead, config):
    return LimitedCartKinematics(toolhead, config)
