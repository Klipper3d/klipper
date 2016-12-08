# Code for state tracking during homing operations
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import mcu

class Homing:
    def __init__(self, toolhead, changed_axes):
        self.toolhead = toolhead
        self.changed_axes = changed_axes
        self.verify_retract = True

        self.eventtime = 0.
        self.states = []
        self.endstops = []
    def set_no_verify_retract(self):
        self.verify_retract = False
    def set_axes(self, axes):
        self.changed_axes = axes
    def get_axes(self):
        return self.changed_axes
    def plan_home(self, forcepos, movepos, steppers, speed):
        self.states.append((self.do_home, (forcepos, movepos, steppers, speed)))
        self.states.append((self.do_wait_endstop, (False,)))
    def plan_second_home(self, forcepos, movepos, steppers, speed):
        self.states.append((self.do_home, (forcepos, movepos, steppers, speed)))
        self.states.append((self.do_wait_endstop, (self.verify_retract,)))
    def plan_retract(self, newpos, steppers, speed):
        self.states.append((self.do_retract, (newpos, steppers, speed)))
    def plan_calc_position(self, callback):
        self.states.append((self.do_calc_position, (callback,)))
    def plan_axes_update(self, callback):
        self.states.append((callback, (self,)))
    def check_busy(self, eventtime):
        self.eventtime = eventtime
        while self.states:
            first = self.states[0]
            try:
                ret = first[0](*first[1])
            except EndstopError, e:
                self.toolhead.motor_off()
                raise
            if ret:
                return True
            self.states.pop(0)
        return False

    def fill_coord(self, coord):
        # Fill in any None entries in 'coord' with current toolhead position
        thcoord = list(self.toolhead.get_position())
        for i in range(len(coord)):
            if coord[i] is not None:
                thcoord[i] = coord[i]
        return thcoord
    def do_retract(self, newpos, steppers, speed):
        self.toolhead.move(self.fill_coord(newpos), speed)
        return False
    def do_home(self, forcepos, movepos, steppers, speed):
        # Alter kinematics class to think printer is at forcepos
        self.toolhead.set_position(self.fill_coord(forcepos))
        # Start homing and issue move
        print_time = self.toolhead.get_last_move_time()
        for s in steppers:
            es = s.enable_endstop_checking(print_time, s.step_dist / speed)
            self.endstops.append((s, es, s.mcu_stepper.get_mcu_position()))
        self.toolhead.move(self.fill_coord(movepos), speed)
        move_end_print_time = self.toolhead.get_last_move_time()
        self.toolhead.reset_print_time()
        for s, es, last_pos in self.endstops:
            es.home_finalize(es.print_to_mcu_time(move_end_print_time))
        return False
    def do_wait_endstop(self, verify_retract):
        # Check if endstops have triggered
        while self.endstops:
            stepper, es, last_pos = self.endstops[0]
            try:
                if es.check_busy(self.eventtime):
                    return True
            except mcu.error, e:
                raise EndstopError("Failed to home stepper %s: %s" % (
                    stepper.name, str(e)))
            post_home_pos = stepper.mcu_stepper.get_mcu_position()
            if verify_retract and last_pos == post_home_pos:
                raise EndstopError("Endstop %s still triggered after retract" % (
                    stepper.name,))
            self.endstops.pop(0)
        return False
    def do_calc_position(self, callback):
        self.toolhead.set_position(self.fill_coord(callback(self)))

class EndstopError(Exception):
    pass

def EndstopMoveError(pos, msg="Move out of range"):
    return EndstopError("%s: %.3f %.3f %.3f [%.3f]" % (
            msg, pos[0], pos[1], pos[2], pos[3]))
