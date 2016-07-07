# Code for state tracking during homing operations
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging

class Homing:
    def __init__(self, toolhead, steppers):
        self.toolhead = toolhead
        self.steppers = steppers

        self.states = []
        self.endstops = []
        self.changed_axis = []
    def plan_home(self, axis, precise=False):
        s = self.steppers[axis]
        state = self.states
        self.changed_axis.append(axis)
        speed = s.homing_speed
        if s.homing_positive_dir:
            pos = s.position_endstop + 1.5*(s.position_min - s.position_endstop)
            rpos = s.position_endstop - s.homing_retract_dist
            r2pos = rpos - s.homing_retract_dist
        else:
            pos = s.position_endstop + 1.5*(s.position_max - s.position_endstop)
            rpos = s.position_endstop + s.homing_retract_dist
            r2pos = rpos + s.homing_retract_dist
        # Initial homing
        state.append((self.do_home, ({axis: pos}, speed)))
        state.append((self.do_wait_endstop, ({axis: 1},)))
        # Retract
        state.append((self.do_move, ({axis: rpos}, speed)))
        # Home again
        state.append((self.do_home, ({axis: r2pos}, speed/2.0)))
        state.append((self.do_wait_endstop, ({axis: 1},)))
    def plan_axis_update(self, callback):
        self.states.append((callback, (self.changed_axis,)))
    def check_busy(self, eventtime):
        while self.states:
            first = self.states[0]
            ret = first[0](*first[1])
            if ret:
                return True
            self.states.pop(0)
        return False

    def do_move(self, axis_pos, speed):
        # Issue a move command to axis_pos
        newpos = self.toolhead.get_position()
        for axis, pos in axis_pos.items():
            newpos[axis] = pos
        self.toolhead.move(newpos, speed)
        return False
    def do_home(self, axis_pos, speed):
        # Alter kinematics class to think printer is at axis_pos
        newpos = self.toolhead.get_position()
        forcepos = list(newpos)
        for axis, pos in axis_pos.items():
            newpos[axis] = self.steppers[axis].position_endstop
            forcepos[axis] = pos
        self.toolhead.set_position(forcepos)
        # Start homing and issue move
        print_time = self.toolhead.get_last_move_time()
        for axis in axis_pos:
            hz = speed * self.steppers[axis].inv_step_dist
            es = self.steppers[axis].enable_endstop_checking(print_time, hz)
            self.endstops.append(es)
        self.toolhead.move(newpos, speed)
        self.toolhead.reset_print_time()
        for es in self.endstops:
            es.home_finalize()
        return False
    def do_wait_endstop(self, axis_wait):
        # Check if axis_wait endstops have triggered
        for es in self.endstops:
            if es.is_homing():
                return True
        # Finished
        del self.endstops[:]
        return False
