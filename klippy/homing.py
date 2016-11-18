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

        self.eventtime = 0.
        self.states = []
        self.endstops = []
    def set_axes(self, axes):
        self.changed_axes = axes
    def get_axes(self):
        return self.changed_axes
    def plan_home(self, forcepos, movepos, steppers, speed):
        self.states.append((self.do_home, (forcepos, movepos, steppers, speed)))
        self.states.append((self.do_wait_endstop, ()))
    def plan_move(self, newpos, speed):
        self.states.append((self.do_move, (newpos, speed)))
    def plan_calc_position(self, callback):
        self.states.append((self.do_calc_position, (callback,)))
    def plan_axes_update(self, callback):
        self.states.append((callback, (self,)))
    def check_busy(self, eventtime):
        self.eventtime = eventtime
        while self.states:
            first = self.states[0]
            ret = first[0](*first[1])
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
    def do_move(self, newpos, speed):
        # Issue a move command
        self.toolhead.move(self.fill_coord(newpos), speed)
        return False
    def do_home(self, forcepos, movepos, steppers, speed):
        # Alter kinematics class to think printer is at forcepos
        self.toolhead.set_position(self.fill_coord(forcepos))
        # Start homing and issue move
        print_time = self.toolhead.get_last_move_time()
        for s in steppers:
            es = s.enable_endstop_checking(print_time, s.step_dist / speed)
            self.endstops.append(es)
        self.toolhead.move(self.fill_coord(movepos), speed)
        self.toolhead.reset_print_time()
        for es in self.endstops:
            es.home_finalize()
        return False
    def do_wait_endstop(self):
        # Check if endstops have triggered
        for es in self.endstops:
            if es.check_busy(self.eventtime):
                return True
        # Finished
        del self.endstops[:]
        return False
    def do_calc_position(self, callback):
        self.toolhead.set_position(self.fill_coord(callback(self)))

# Helper code for querying and reporting the status of the endstops
class QueryEndstops:
    def __init__(self, print_time, respond_cb):
        self.print_time = print_time
        self.respond_cb = respond_cb
        self.endstops = []
        self.busy = []
    def set_steppers(self, steppers):
        for stepper in steppers:
            es = stepper.query_endstop(self.print_time)
            if es is None:
                continue
            self.endstops.append((stepper.name, es))
            self.busy.append(es)
    def check_busy(self, eventtime):
        # Check if all endstop queries have been received
        while self.busy:
            busy = self.busy[0].check_busy(eventtime)
            if busy:
                return True
            self.busy.pop(0)
        # All responses received - report status
        msgs = []
        for name, es in self.endstops:
            msg = "open"
            if es.get_last_triggered():
                msg = "TRIGGERED"
            msgs.append("%s:%s" % (name, msg))
        self.respond_cb(" ".join(msgs))
        return False

class EndstopError(Exception):
    def __init__(self, msg="Endstop error"):
        self._msg = msg
    def __str__(self):
        return self._msg

def EndstopMoveError(pos, msg="Move out of range"):
    return EndstopError("%s: %.3f %.3f %.3f [%.3f]" % (
            msg, pos[0], pos[1], pos[2], pos[3]))
