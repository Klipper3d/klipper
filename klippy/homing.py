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
    def plan_home(self, forcepos, movepos, steppers, speed):
        self.states.append((self.do_home, (forcepos, movepos, steppers, speed)))
        self.states.append((self.do_wait_endstop, ()))
    def plan_move(self, newpos, speed):
        self.states.append((self.do_move, (newpos, speed)))
    def plan_axes_update(self, callback):
        self.states.append((callback, (self.changed_axes,)))
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
        thcoord = self.toolhead.get_position()
        coord = list(coord)
        for i in range(len(coord)):
            if coord[i] is None:
                coord[i] = thcoord[i]
        return coord
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

class QueryEndstops:
    def __init__(self, names, steppers):
        self.endstops = []
        self.busy = []
        for name, stepper in zip(names, steppers):
            es = stepper.query_endstop()
            if es is None:
                continue
            self.endstops.append((name, es))
            self.busy.append(es)
    def check_busy(self, eventtime):
        while self.busy:
            busy = self.busy[0].check_busy(eventtime)
            if busy:
                return True
            self.busy.pop(0)
        return False
    def get_msg(self):
        msgs = []
        for name, es in self.endstops:
            msg = "open"
            if es.get_last_triggered():
                msg = "TRIGGERED"
            msgs.append("%s:%s" % (name, msg))
        return " ".join(msgs)

class EndstopError(Exception):
    def __init__(self, pos, msg="Move out of range"):
        self.pos = pos
        self.msg = msg
    def __str__(self):
        return "%s: %.3f %.3f %.3f [%.3f]" % (
            self.msg, self.pos[0], self.pos[1], self.pos[2], self.pos[3])
