# Code for state tracking during homing operations
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math

HOMING_STEP_DELAY = 0.00000025
ENDSTOP_SAMPLE_TIME = .000015
ENDSTOP_SAMPLE_COUNT = 4

class Homing:
    def __init__(self, toolhead):
        self.toolhead = toolhead
        self.changed_axes = []
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
    def set_homed_position(self, pos):
        self.toolhead.set_position(self._fill_coord(pos))
    def _get_homing_speed(self, speed, endstops):
        # Round the requested homing speed so that it is an even
        # number of ticks per step.
        speed = min(speed, self.toolhead.get_max_velocity()[0])
        mcu_stepper = endstops[0][0].get_steppers()[0]
        adjusted_freq = mcu_stepper.get_mcu().get_adjusted_freq()
        dist_ticks = adjusted_freq * mcu_stepper.get_step_dist()
        ticks_per_step = math.ceil(dist_ticks / speed)
        return dist_ticks / ticks_per_step
    def homing_move(self, movepos, endstops, speed, probe_pos=False):
        # Start endstop checking
        for mcu_endstop, name in endstops:
            mcu_endstop.home_prepare()
        print_time = self.toolhead.get_last_move_time()
        for mcu_endstop, name in endstops:
            min_step_dist = min([s.get_step_dist()
                                 for s in mcu_endstop.get_steppers()])
            mcu_endstop.home_start(
                print_time, ENDSTOP_SAMPLE_TIME, ENDSTOP_SAMPLE_COUNT,
                min_step_dist / speed)
        # Issue move
        movepos = self._fill_coord(movepos)
        error = None
        try:
            self.toolhead.move(movepos, speed)
        except EndstopError as e:
            error = "Error during homing move: %s" % (str(e),)
        # Wait for endstops to trigger
        move_end_print_time = self.toolhead.get_last_move_time()
        self.toolhead.reset_print_time(print_time)
        for mcu_endstop, name in endstops:
            try:
                mcu_endstop.home_wait(move_end_print_time)
            except mcu_endstop.TimeoutError as e:
                if error is None:
                    error = "Failed to home %s: %s" % (name, str(e))
        if probe_pos:
            self.set_homed_position(
                list(self.toolhead.get_kinematics().get_position()) + [None])
        else:
            self.toolhead.set_position(movepos)
        for mcu_endstop, name in endstops:
            mcu_endstop.home_finalize()
        if error is not None:
            raise EndstopError(error)
    def home(self, forcepos, movepos, endstops, speed, second_home=False):
        # Alter kinematics class to think printer is at forcepos
        homing_axes = [axis for axis in range(3) if forcepos[axis] is not None]
        self.toolhead.set_position(
            self._fill_coord(forcepos), homing_axes=homing_axes)
        # Add a CPU delay when homing a large axis
        if not second_home:
            est_move_d = sum([abs(forcepos[i]-movepos[i])
                              for i in range(3) if movepos[i] is not None])
            est_steps = sum([est_move_d / s.get_step_dist()
                             for es, n in endstops for s in es.get_steppers()])
            self.toolhead.dwell(est_steps * HOMING_STEP_DELAY, check_stall=False)
            speed = self._get_homing_speed(speed, endstops)
        # Setup for retract verification
        self.toolhead.get_last_move_time()
        start_mcu_pos = [(s, name, s.get_mcu_position())
                         for es, name in endstops for s in es.get_steppers()]
        # Issue homing move
        self.homing_move(movepos, endstops, speed)
        # Verify retract led to some movement on second home
        if second_home and self.verify_retract:
            for s, name, pos in start_mcu_pos:
                if s.get_mcu_position() == pos:
                    raise EndstopError(
                        "Endstop %s still triggered after retract" % (name,))
    def home_axes(self, axes):
        self.changed_axes = axes
        try:
            self.toolhead.get_kinematics().home(self)
        except EndstopError:
            self.toolhead.motor_off()
            raise

def query_endstops(toolhead):
    print_time = toolhead.get_last_move_time()
    steppers = toolhead.get_kinematics().get_steppers()
    out = []
    for s in steppers:
        for mcu_endstop, name in s.get_endstops():
            mcu_endstop.query_endstop(print_time)
    for s in steppers:
        for mcu_endstop, name in s.get_endstops():
            out.append((name, mcu_endstop.query_endstop_wait()))
    return out

def query_position(toolhead):
    steppers = toolhead.get_kinematics().get_steppers()
    return [(s.name.upper(), s.mcu_stepper.get_mcu_position()) for s in steppers]

class EndstopError(Exception):
    pass

def EndstopMoveError(pos, msg="Move out of range"):
    return EndstopError("%s: %.3f %.3f %.3f [%.3f]" % (
            msg, pos[0], pos[1], pos[2], pos[3]))
