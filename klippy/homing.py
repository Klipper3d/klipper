# Code for state tracking during homing operations
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

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
        print_time = self.toolhead.get_last_move_time()
        endstops = []
        for s in steppers:
            es = s.enable_endstop_checking(print_time, s.step_dist / speed)
            endstops.append((s, es, s.mcu_stepper.get_mcu_position()))
        self.toolhead.move(self._fill_coord(movepos), speed)
        move_end_print_time = self.toolhead.get_last_move_time()
        self.toolhead.reset_print_time()
        for s, es, last_pos in endstops:
            es.home_finalize(es.print_to_mcu_time(move_end_print_time))
        # Wait for endstops to trigger
        for s, es, last_pos in endstops:
            try:
                es.home_wait()
            except es.error as e:
                raise EndstopError("Failed to home stepper %s: %s" % (
                    s.name, str(e)))
            post_home_pos = s.mcu_stepper.get_mcu_position()
            if second_home and self.verify_retract and last_pos == post_home_pos:
                raise EndstopError("Endstop %s still triggered after retract" % (
                    s.name,))
    def set_homed_position(self, pos):
        self.toolhead.set_position(self._fill_coord(pos))

class EndstopError(Exception):
    pass

def EndstopMoveError(pos, msg="Move out of range"):
    return EndstopError("%s: %.3f %.3f %.3f [%.3f]" % (
            msg, pos[0], pos[1], pos[2], pos[3]))
