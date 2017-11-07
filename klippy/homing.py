# Code for state tracking during homing operations
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

HOMING_DELAY = 0.250
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

class Homing:
    def __init__(self, toolhead, changed_axes):
        self.toolhead = toolhead
        self.changed_axes = changed_axes
        self.verify_retract = True
    def set_no_verify_retract(self):
        self.verify_retract = False
    def set_axes(self, axes):
        self.changed_axes = axes
    def get_axes(self):
        return self.changed_axes
    def _fill_coord(self, coord):
        # Fill in any None entries in 'coord' with current toolhead position
        thcoord = list(self.toolhead.get_position())
        for i in range(len(coord)):
            if coord[i] is not None:
                thcoord[i] = coord[i]
        return thcoord
    def retract(self, newpos, speed):
        self.toolhead.move(self._fill_coord(newpos), speed)
    def home(self, forcepos, movepos, steppers, speed, second_home=False):
        # Alter kinematics class to think printer is at forcepos
        self.toolhead.set_position(self._fill_coord(forcepos))
        # Start homing and issue move
        if not second_home:
            self.toolhead.dwell(HOMING_DELAY)
        print_time = self.toolhead.get_last_move_time()
        endstops = []
        for s in steppers:
            for mcu_endstop, mcu_stepper, name in s.get_endstops():
                mcu_endstop.home_start(
                    print_time, ENDSTOP_SAMPLE_TIME, ENDSTOP_SAMPLE_COUNT,
                    mcu_stepper.get_step_dist() / speed)
                endstops.append((mcu_endstop, mcu_stepper, name,
                                 mcu_stepper.get_mcu_position()))
        self.toolhead.move(self._fill_coord(movepos), speed)
        move_end_print_time = self.toolhead.get_last_move_time()
        self.toolhead.reset_print_time(print_time)
        for mcu_endstop, mcu_stepper, name, last_pos in endstops:
            mcu_endstop.home_finalize(move_end_print_time)
        # Wait for endstops to trigger
        for mcu_endstop, mcu_stepper, name, last_pos in endstops:
            try:
                mcu_endstop.home_wait()
            except mcu_endstop.error as e:
                raise EndstopError("Failed to home stepper %s: %s" % (
                    name, str(e)))
            post_home_pos = mcu_stepper.get_mcu_position()
            if second_home and self.verify_retract and last_pos == post_home_pos:
                raise EndstopError("Endstop %s still triggered after retract" % (
                    name,))
    def set_homed_position(self, pos):
        self.toolhead.set_position(self._fill_coord(pos))

def query_endstops(print_time, query_flags, steppers):
    if query_flags == "get_mcu_position":
        # Only the commanded position is requested
        return [(name.upper(), mcu_stepper.get_mcu_position())
                for s in steppers
                for mcu_endstop, mcu_stepper, name in s.get_endstops()]
    for s in steppers:
        for mcu_endstop, mcu_stepper, name in s.get_endstops():
            mcu_endstop.query_endstop(print_time)
    out = []
    for s in steppers:
        for mcu_endstop, mcu_stepper, name in s.get_endstops():
            try:
                out.append((name, mcu_endstop.query_endstop_wait()))
            except mcu_endstop.error as e:
                raise EndstopError(str(e))
    return out

class EndstopError(Exception):
    pass

def EndstopMoveError(pos, msg="Move out of range"):
    return EndstopError("%s: %.3f %.3f %.3f [%.3f]" % (
            msg, pos[0], pos[1], pos[2], pos[3]))
