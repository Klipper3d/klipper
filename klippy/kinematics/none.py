# Dummy "none" kinematics support (for developer testing)
#
# Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import homing

class NoneKinematics:
    def __init__(self, toolhead, config):
        pass
    def get_steppers(self):
        return []
    def calc_tag_position(self):
        return [0, 0, 0]
    def set_position(self, newpos, homing_axes):
        pass
    def home(self, homing_state):
        pass
    def check_move(self, move):
        pass
    def get_status(self, eventtime):
        axes_lim = [0.0, 0.0, 0.0, 0.0]
        return {
            'homed_axes': '',
            'axis_minimum': homing.Coord(*axes_lim),
            'axis_maximum': homing.Coord(*axes_lim)
        }

def load_kinematics(toolhead, config):
    return NoneKinematics(toolhead, config)
