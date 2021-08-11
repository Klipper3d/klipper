# Code for handling the kinematics of corexy robots
# with per-axis limits for acceleration
#
# Copyright (C) 2020-2021  Mael Kerbiriou <piezo.wdimd@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
# Usage:
# Copying this file under `klipper/klippy/kinematics/` should be enough
# (click the `raw` button on github, then save as)
# Then your config's [printer] should look like:
# [printer]
# kinematics: limited_corexy
# max_velocity: [141% original value]
# max_z_velocity: [untouched]
# max_accel: [max_x_accel, or less if you want to clip the per-axis limiting]
# max_x_accel: [empirically determined, max_accel taken if omitted]
# max_y_accel: [empirically determined, less than max_x_accel because of gantry mass]
# max_z_accel: [untouched]
#
# Derivation of the formulae described here: http://bl.ocks.org/Piezoid/raw/368e4ca48c65724e419cfb8198cfee0e/
# (notebook source: /docs/PerAxisLimits.ipynb)

from . import corexy

class LCoreXYKinematics(corexy.CoreXYKinematics):
    def __init__(self, toolhead, config):
        corexy.CoreXYKinematics.__init__(self, toolhead, config)
        # Setup x/y axis limits
        max_accel = toolhead.max_accel
        self.max_x_accel = config.getfloat('max_x_accel', max_accel, above=0)
        self.max_y_accel = config.getfloat('max_y_accel', max_accel, above=0)
    def check_move(self, move):
        if not move.is_kinematic_move:
            return
        self._check_endstops(move)
        toolhead = move.toolhead
        max_v = toolhead.max_velocity
        max_a = toolhead.max_accel
        move_d = move.move_d
        x,y,z = move.axes_d[:3]
        ab_linf = max(abs(x+y), abs(x-y))
        if ab_linf > 0:
            max_v *= move_d / ab_linf
            x_o_a = x / self.max_x_accel
            y_o_a = y / self.max_y_accel
            max_a = move_d / max(abs(x_o_a + y_o_a), abs(x_o_a - y_o_a))
        if z:
            z_ratio = move_d / abs(z)
            max_v = min(max_v, self.max_z_velocity * z_ratio)
            max_a = min(max_a, self.max_z_accel * z_ratio)
        move.limit_speed(max_v, max_a)

def load_kinematics(toolhead, config):
    return LCoreXYKinematics(toolhead, config)
