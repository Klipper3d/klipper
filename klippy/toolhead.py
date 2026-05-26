# Code for coordinating events on the printer toolhead
#
# Copyright (C) 2016-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, importlib
import mcu, chelper, kinematics.extruder

# Common suffixes: _d is distance (in mm), _v is velocity (in
#   mm/second), _v2 is velocity squared (mm^2/s^2), _t is time (in
#   seconds), _r is ratio (scalar between 0.0 and 1.0)

# Class to track each move request
class Move:
    def __init__(self, toolhead, start_pos, end_pos, speed):
        self.toolhead = toolhead
        self.start_pos = tuple(start_pos)
        self.end_pos = tuple(end_pos)
        self.accel = toolhead.max_accel
        self.junction_deviation = toolhead.junction_deviation
        self.timing_callbacks = []
        velocity = min(speed, toolhead.max_velocity)
        self.is_kinematic_move = True
        self.axes_d = axes_d = [ep - sp for sp, ep in zip(start_pos, end_pos)]
        self.move_d = move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
        if move_d < .000000001:
            # Extrude only move
            self.end_pos = ((start_pos[0], start_pos[1], start_pos[2])
                            + self.end_pos[3:])
            axes_d[0] = axes_d[1] = axes_d[2] = 0.
            self.move_d = move_d = max([abs(ad) for ad in axes_d[3:]])
            inv_move_d = 0.
            if move_d:
                inv_move_d = 1. / move_d
            self.accel = 99999999.9
            velocity = speed
            self.is_kinematic_move = False
        else:
            inv_move_d = 1. / move_d
        self.axes_r = [d * inv_move_d for d in axes_d]
        self.min_move_t = move_d / velocity
        # Junction speeds are tracked in velocity squared.  The
        # delta_v2 is the maximum amount of this squared-velocity that
        # can change in this move.
        self.max_start_v2 = 0.
        self.max_cruise_v2 = velocity**2
        self.delta_v2 = 2.0 * move_d * self.accel
        self.next_junction_v2 = 999999999.9
        # Setup for minimum_cruise_ratio checks
        self.max_mcr_start_v2 = 0.
        self.mcr_delta_v2 = 2.0 * move_d * toolhead.mcr_pseudo_accel
    def limit_speed(self, speed, accel):
        speed2 = speed**2
        if speed2 < self.max_cruise_v2:
            self.max_cruise_v2 = speed2
            self.min_move_t = self.move_d / speed
        self.accel = min(self.accel, accel)
        self.delta_v2 = 2.0 * self.move_d * self.accel
        self.mcr_delta_v2 = min(self.mcr_delta_v2, self.delta_v2)
    def limit_next_junction_speed(self, speed):
        self.next_junction_v2 = min(self.next_junction_v2, speed**2)
    def move_error(self, msg="Move out of range"):
        ep = self.end_pos
        m = "%s: %.3f %.3f %.3f [%.3f]" % (msg, ep[0], ep[1], ep[2], ep[3])
        return self.toolhead.printer.command_error(m)
    def calc_junction(self, prev_move):
        if not self.is_kinematic_move or not prev_move.is_kinematic_move:
            return
        # Allow extra axes to calculate maximum junction
        ea_v2 = [ea.calc_junction(prev_move, self, e_index+3)
                 for e_index, ea in enumerate(self.toolhead.extra_axes)]
        max_start_v2 = min([self.max_cruise_v2,
                            prev_move.max_cruise_v2, prev_move.next_junction_v2,
                            prev_move.max_start_v2 + prev_move.delta_v2]
                           + ea_v2)
        # Find max velocity using "approximated centripetal velocity"
        axes_r = self.axes_r
        prev_axes_r = prev_move.axes_r
        junction_cos_theta = -(axes_r[0] * prev_axes_r[0]
                               + axes_r[1] * prev_axes_r[1]
                               + axes_r[2] * prev_axes_r[2])
        sin_theta_d2 = math.sqrt(max(0.5*(1.0-junction_cos_theta), 0.))
        cos_theta_d2 = math.sqrt(max(0.5*(1.0+junction_cos_theta), 0.))
        one_minus_sin_theta_d2 = 1. - sin_theta_d2
        if one_minus_sin_theta_d2 > 0. and cos_theta_d2 > 0.:
            R_jd = sin_theta_d2 / one_minus_sin_theta_d2
            move_jd_v2 = R_jd * self.junction_deviation * self.accel
            pmove_jd_v2 = R_jd * prev_move.junction_deviation * prev_move.accel
            # Approximated circle must contact moves no further than mid-move
            #   centripetal_v2 = .5 * self.move_d * self.accel * tan_theta_d2
            quarter_tan_theta_d2 = .25 * sin_theta_d2 / cos_theta_d2
            move_centripetal_v2 = self.delta_v2 * quarter_tan_theta_d2
            pmove_centripetal_v2 = prev_move.delta_v2 * quarter_tan_theta_d2
            max_start_v2 = min(max_start_v2, move_jd_v2, pmove_jd_v2,
                               move_centripetal_v2, pmove_centripetal_v2)
        # Apply limits
        self.max_start_v2 = max_start_v2
        self.max_mcr_start_v2 = min(
            max_start_v2, prev_move.max_mcr_start_v2 + prev_move.mcr_delta_v2)
    def set_junction(self, start_v2, cruise_v2, end_v2):
        # Determine accel, cruise, and decel portions of the move distance
        half_inv_accel = .5 / self.accel
        accel_d = (cruise_v2 - start_v2) * half_inv_accel
        decel_d = (cruise_v2 - end_v2) * half_inv_accel
        cruise_d = self.move_d - accel_d - decel_d
        # Determine move velocities
        self.start_v = start_v = math.sqrt(start_v2)
        self.cruise_v = cruise_v = math.sqrt(cruise_v2)
        self.end_v = end_v = math.sqrt(end_v2)
        # Determine time spent in each portion of move (time is the
        # distance divided by average velocity)
        self.accel_t = accel_d / ((start_v + cruise_v) * 0.5)
        self.cruise_t = cruise_d / cruise_v
        self.decel_t = decel_d / ((end_v + cruise_v) * 0.5)

LOOKAHEAD_FLUSH_TIME = 0.150

# Class to track a list of pending move requests and to facilitate
# "look-ahead" across moves to reduce acceleration between moves.
class LookAheadQueue:
    def __init__(self):
        self.queue = []
        self.junction_flush = LOOKAHEAD_FLUSH_TIME
    def reset(self):
        del self.queue[:]
        self.junction_flush = LOOKAHEAD_FLUSH_TIME
    def set_flush_time(self, flush_time):
        self.junction_flush = flush_time
    def is_empty(self):
        return not self.queue
    def get_last(self):
        if self.queue:
            return self.queue[-1]
        return None
    def flush(self, lazy=False):
        self.junction_flush = LOOKAHEAD_FLUSH_TIME
        update_flush_count = lazy
        queue = self.queue
        flush_count = len(queue)
        # Traverse queue from last to first move and determine maximum
        # junction speed assuming the robot comes to a complete stop
        # after the last move.
        junction_info = [None] * flush_count
        next_start_v2 = next_mcr_start_v2 = peak_cruise_v2 = 0.
        pending_cv2_assign = 0
        for i in range(flush_count-1, -1, -1):
            move = queue[i]
            reachable_start_v2 = next_start_v2 + move.delta_v2
            start_v2 = min(move.max_start_v2, reachable_start_v2)
            cruise_v2 = None
            pending_cv2_assign += 1
            reach_mcr_start_v2 = next_mcr_start_v2 + move.mcr_delta_v2
            mcr_start_v2 = min(move.max_mcr_start_v2, reach_mcr_start_v2)
            if mcr_start_v2 < reach_mcr_start_v2:
                # It's possible for this move to accelerate
                if (mcr_start_v2 + move.mcr_delta_v2 > next_mcr_start_v2
                    or pending_cv2_assign > 1):
                    # This move can both accel and decel, or this is a
                    # full accel move followed by a full decel move
                    if update_flush_count and peak_cruise_v2:
                        flush_count = i + pending_cv2_assign
                        update_flush_count = False
                    peak_cruise_v2 = (mcr_start_v2 + reach_mcr_start_v2) * .5
                cruise_v2 = min((start_v2 + reachable_start_v2) * .5
                                , move.max_cruise_v2, peak_cruise_v2)
                pending_cv2_assign = 0
            junction_info[i] = (move, start_v2, cruise_v2, next_start_v2)
            next_start_v2 = start_v2
            next_mcr_start_v2 = mcr_start_v2
        if update_flush_count or not flush_count:
            return []
        # Traverse queue in forward direction to propagate cruise_v2
        prev_cruise_v2 = 0.
        for i in range(flush_count):
            move, start_v2, cruise_v2, next_start_v2 = junction_info[i]
            if cruise_v2 is None:
                # This move can't accelerate - propagate cruise_v2 from previous
                cruise_v2 = min(prev_cruise_v2, start_v2)
            move.set_junction(min(start_v2, cruise_v2), cruise_v2
                              , min(next_start_v2, cruise_v2))
            prev_cruise_v2 = cruise_v2
        # Remove processed moves from the queue
        res = queue[:flush_count]
        del queue[:flush_count]
        return res
    def add_move(self, move):
        self.queue.append(move)
        if len(self.queue) == 1:
            return
        move.calc_junction(self.queue[-2])
        self.junction_flush -= move.min_move_t
        # Check if enough moves have been queued to reach the target flush time.
        return self.junction_flush <= 0.

BUFFER_TIME_HIGH = 1.0
BUFFER_TIME_START = 0.250
PRIMING_CMD_TIME = 0.100

# Main code to track events (and their timing) on the printer toolhead
class ToolHead:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.mcu = self.printer.lookup_object('mcu')
        self.lookahead = LookAheadQueue()
        self.lookahead.set_flush_time(BUFFER_TIME_HIGH)
        self.commanded_pos = [0., 0., 0., 0.]
        # Velocity and acceleration control
        self.max_velocity = config.getfloat('max_velocity', above=0.)
        self.max_accel = config.getfloat('max_accel', above=0.)
        self.min_cruise_ratio = config.getfloat('minimum_cruise_ratio',
                                                0.5, below=1., minval=0.)
        self.square_corner_velocity = config.getfloat(
            'square_corner_velocity', 5., minval=0.)
        self.junction_deviation = self.mcr_pseudo_accel = 0.
        self._calc_junction_deviation()
        # Input stall detection
        self.check_stall_time = 0.
        self.print_stall = 0
        # Input pause tracking
        self.can_pause = True
        if self.mcu.is_fileoutput():
            self.can_pause = False
        self.need_check_pause = -1.
        # Print time tracking
        self.print_time = 0.
        self.special_queuing_state = "NeedPrime"
        self.priming_timer = None
        # Setup for generating moves
        self.motion_queuing = self.printer.load_object(config, 'motion_queuing')
        self.motion_queuing.register_flush_callback(self._handle_step_flush,
                                                    can_add_trapq=True)
        self.trapq = self.motion_queuing.allocate_trapq()
        self.trapq_append = self.motion_queuing.lookup_trapq_append()
        # Create kinematics class
        gcode = self.printer.lookup_object('gcode')
        self.Coord = gcode.Coord
        extruder = kinematics.extruder.DummyExtruder(self.printer)
        self.extra_axes = [extruder]
        self.extra_axes_status = {}
        self._build_extra_axes_status()
        kin_name = config.get('kinematics')
        try:
            mod = importlib.import_module('kinematics.' + kin_name)
            self.kin = mod.load_kinematics(self, config)
        except config.error as e:
            raise
        except self.printer.lookup_object('pins').error as e:
            raise
        except:
            msg = "Error loading kinematics '%s'" % (kin_name,)
            logging.exception(msg)
            raise config.error(msg)
        # Register handlers
        self.printer.register_event_handler("klippy:shutdown",
                                            self._handle_shutdown)
    # Print time tracking
    def _advance_move_time(self, next_print_time):
        self.print_time = max(self.print_time, next_print_time)
    def _calc_print_time(self):
        curtime = self.reactor.monotonic()
        est_print_time = self.mcu.estimated_print_time(curtime)
        kin_time = self.motion_queuing.calc_step_gen_restart(est_print_time)
        min_print_time = max(est_print_time + BUFFER_TIME_START, kin_time)
        if min_print_time > self.print_time:
            self.print_time = min_print_time
            self.printer.send_event("toolhead:sync_print_time",
                                    curtime, est_print_time, self.print_time)
    def _process_lookahead(self, lazy=False):
        moves = self.lookahead.flush(lazy=lazy)
        if not moves:
            return
        # Resync print_time if necessary
        if self.special_queuing_state:
            # Transition from "NeedPrime"/"Priming" state to main state
            self.special_queuing_state = ""
            self.need_check_pause = -1.
            self._calc_print_time()
        # Queue moves into trapezoid motion queue (trapq)
        next_move_time = self.print_time
        with self.reactor.assert_no_pause():
            for move in moves:
                if move.is_kinematic_move:
                    self.trapq_append(
                        self.trapq, next_move_time,
                        move.accel_t, move.cruise_t, move.decel_t,
                        move.start_pos[0], move.start_pos[1], move.start_pos[2],
                        move.axes_r[0], move.axes_r[1], move.axes_r[2],
                        move.start_v, move.cruise_v, move.accel)
                for e_index, ea in enumerate(self.extra_axes):
                    if move.axes_d[e_index + 3]:
                        ea.process_move(next_move_time, move, e_index + 3)
                next_move_time = (next_move_time + move.accel_t
                                  + move.cruise_t + move.decel_t)
                for cb in move.timing_callbacks:
                    cb(next_move_time)
        # Generate steps for moves
        self._advance_move_time(next_move_time)
        self.motion_queuing.note_mcu_movequeue_activity(next_move_time)
    def _flush_lookahead(self, is_runout=False):
        # Transit from "NeedPrime"/"Priming"/main state to "NeedPrime"
        prev_print_time = self.print_time
        self._process_lookahead()
        self.special_queuing_state = "NeedPrime"
        self.need_check_pause = -1.
        self.lookahead.set_flush_time(BUFFER_TIME_HIGH)
        self.check_stall_time = 0.
        if is_runout and prev_print_time != self.print_time:
            self.check_stall_time = self.print_time
    def _handle_step_flush(self, flush_time, step_gen_time):
        if self.special_queuing_state:
            return
        # In "main" state - flush lookahead if buffer runs low
        kin_flush_delay = self.motion_queuing.get_kin_flush_delay()
        if step_gen_time >= self.print_time - kin_flush_delay - 0.001:
            self._flush_lookahead(is_runout=True)
    def flush_step_generation(self):
        self._flush_lookahead()
        self.motion_queuing.flush_all_steps()
    def get_last_move_time(self):
        if self.special_queuing_state:
            self._flush_lookahead()
            self._calc_print_time()
        else:
            self._process_lookahead()
        return self.print_time
    def _priming_handler(self, eventtime):
        self.reactor.unregister_timer(self.priming_timer)
        self.priming_timer = None
        try:
            if self.special_queuing_state == "Priming":
                self._flush_lookahead(is_runout=True)
        except:
            logging.exception("Exception in priming_handler")
            self.printer.invoke_shutdown("Exception in priming_handler")
        return self.reactor.NEVER
    def _check_priming_state(self, eventtime):
        if self.lookahead.is_empty():
            # In "NeedPrime" state and can remain there
            return
        est_print_time = self.mcu.estimated_print_time(eventtime)
        if self.check_stall_time:
            # Was in "NeedPrime" state and got there from idle input
            if est_print_time < self.check_stall_time:
                self.print_stall += 1
            self.check_stall_time = 0.
        # Transition from "NeedPrime"/"Priming" state to "Priming" state
        self.special_queuing_state = "Priming"
        self.need_check_pause = -1.
        if self.priming_timer is None:
            self.priming_timer = self.reactor.register_timer(
                self._priming_handler)
        will_pause_time = self.print_time - est_print_time - BUFFER_TIME_HIGH
        wtime = eventtime + max(0., will_pause_time) + PRIMING_CMD_TIME
        self.reactor.update_timer(self.priming_timer, wtime)
    def _check_pause(self):
        eventtime = self.reactor.monotonic()
        if self.special_queuing_state:
            # In "NeedPrime"/"Priming" state - update priming expiration timer
            self._check_priming_state(eventtime)
        # Check if there are lots of queued moves and pause if so
        did_pause = False
        while 1:
            est_print_time = self.mcu.estimated_print_time(eventtime)
            pause_time = self.print_time - est_print_time - BUFFER_TIME_HIGH
            if pause_time <= 0.:
                break
            if not self.can_pause:
                self.need_check_pause = self.reactor.NEVER
                return
            pause_time = max(.005, min(1., pause_time))
            eventtime = self.reactor.pause(eventtime + pause_time)
            did_pause = True
        if not self.special_queuing_state:
            # In main state - defer pause checking
            self.need_check_pause = self.print_time
            if not did_pause:
                # May be falling behind - yield to avoid starving other tasks
                self.reactor.pause(self.reactor.NOW)
    # Movement commands
    def get_position(self):
        return list(self.commanded_pos)
    def set_position(self, newpos, homing_axes=""):
        self.flush_step_generation()
        ffi_main, ffi_lib = chelper.get_ffi()
        ffi_lib.trapq_set_position(self.trapq, self.print_time,
                                   newpos[0], newpos[1], newpos[2])
        self.commanded_pos[:3] = newpos[:3]
        self.kin.set_position(newpos, homing_axes)
        self.printer.send_event("toolhead:set_position")
    def limit_next_junction_speed(self, speed):
        last_move = self.lookahead.get_last()
        if last_move is not None:
            last_move.limit_next_junction_speed(speed)
    def move(self, newpos, speed):
        move = Move(self, self.commanded_pos, newpos, speed)
        if not move.move_d:
            return
        if move.is_kinematic_move:
            self.kin.check_move(move)
        for e_index, ea in enumerate(self.extra_axes):
            if move.axes_d[e_index + 3]:
                ea.check_move(move, e_index + 3)
        self.commanded_pos[:] = move.end_pos
        want_flush = self.lookahead.add_move(move)
        if want_flush:
            self._process_lookahead(lazy=True)
        if self.print_time > self.need_check_pause:
            self._check_pause()
    def manual_move(self, coord, speed):
        curpos = list(self.commanded_pos)
        for i in range(len(coord)):
            if coord[i] is not None:
                curpos[i] = coord[i]
        self.move(curpos, speed)
        self.printer.send_event("toolhead:manual_move")
    def dwell(self, delay):
        self._flush_lookahead()
        next_print_time = self.get_last_move_time() + max(0., delay)
        self._advance_move_time(next_print_time)
        self._check_pause()
    def wait_moves(self):
        self._flush_lookahead()
        eventtime = self.reactor.monotonic()
        while (not self.special_queuing_state
               or self.print_time >= self.mcu.estimated_print_time(eventtime)):
            if not self.can_pause:
                break
            eventtime = self.reactor.pause(eventtime + 0.100)
    def _build_extra_axes_status(self):
        enames = [ea.get_name() for ea in self.extra_axes]
        self.extra_axes_status = {n: e_index + 3
                                  for e_index, n in enumerate(enames) if n}
    def set_extruder(self, extruder, extrude_pos):
        # XXX - should use add_extra_axis
        self.extra_axes[0] = extruder
        self.commanded_pos[3] = extrude_pos
        self._build_extra_axes_status()
    def get_extruder(self):
        return self.extra_axes[0]
    def add_extra_axis(self, ea, axis_pos):
        self._flush_lookahead()
        self.extra_axes.append(ea)
        self.commanded_pos.append(axis_pos)
        self._build_extra_axes_status()
        self.printer.send_event("toolhead:update_extra_axes")
    def remove_extra_axis(self, ea):
        self._flush_lookahead()
        if ea not in self.extra_axes:
            return
        ea_index = self.extra_axes.index(ea) + 3
        self.commanded_pos.pop(ea_index)
        self.extra_axes.pop(ea_index - 3)
        self._build_extra_axes_status()
        self.printer.send_event("toolhead:update_extra_axes")
    def get_extra_axes(self):
        return [None, None, None] + self.extra_axes
    # Homing "drip move" handling
    def _drip_load_trapq(self, submit_move):
        # Queue move into trapezoid motion queue (trapq)
        if submit_move.move_d:
            self.commanded_pos[:] = submit_move.end_pos
            self.lookahead.add_move(submit_move)
        moves = self.lookahead.flush()
        self._calc_print_time()
        start_time = end_time = self.print_time
        for move in moves:
            self.trapq_append(
                self.trapq, end_time,
                move.accel_t, move.cruise_t, move.decel_t,
                move.start_pos[0], move.start_pos[1], move.start_pos[2],
                move.axes_r[0], move.axes_r[1], move.axes_r[2],
                move.start_v, move.cruise_v, move.accel)
            end_time = end_time + move.accel_t + move.cruise_t + move.decel_t
        self.lookahead.reset()
        return start_time, end_time
    def drip_move(self, newpos, speed, drip_completion):
        # Create and verify move is valid
        newpos = newpos[:3] + self.commanded_pos[3:]
        move = Move(self, self.commanded_pos, newpos, speed)
        if move.move_d:
            self.kin.check_move(move)
        # Make sure stepper movement doesn't start before nominal start time
        kin_flush_delay = self.motion_queuing.get_kin_flush_delay()
        self.dwell(kin_flush_delay)
        # Transmit move in "drip" mode
        self._process_lookahead()
        start_time, end_time = self._drip_load_trapq(move)
        self.motion_queuing.drip_update_time(start_time, end_time,
                                             drip_completion)
        # Move finished; cleanup any remnants on trapq
        self.motion_queuing.wipe_trapq(self.trapq)
    # Misc commands
    def stats(self, eventtime):
        est_print_time = self.mcu.estimated_print_time(eventtime)
        buffer_time = self.print_time - est_print_time
        is_active = buffer_time > -60. or not self.special_queuing_state
        return is_active, "print_time=%.3f buffer_time=%.3f print_stall=%d" % (
            self.print_time, max(buffer_time, 0.), self.print_stall)
    def check_busy(self, eventtime):
        est_print_time = self.mcu.estimated_print_time(eventtime)
        return self.print_time, est_print_time, self.lookahead.is_empty()
    def get_status(self, eventtime):
        print_time = self.print_time
        estimated_print_time = self.mcu.estimated_print_time(eventtime)
        extruder = self.extra_axes[0]
        res = dict(self.kin.get_status(eventtime))
        res.update({ 'print_time': print_time,
                     'stalls': self.print_stall,
                     'estimated_print_time': estimated_print_time,
                     'extruder': extruder.get_name(),
                     'position': self.Coord(self.commanded_pos),
                     'max_velocity': self.max_velocity,
                     'max_accel': self.max_accel,
                     'minimum_cruise_ratio': self.min_cruise_ratio,
                     'square_corner_velocity': self.square_corner_velocity,
                     'extra_axes': self.extra_axes_status})
        return res
    def _handle_shutdown(self):
        self.can_pause = False
        self.lookahead.reset()
    def get_kinematics(self):
        return self.kin
    def get_trapq(self):
        return self.trapq
    def register_lookahead_callback(self, callback):
        last_move = self.lookahead.get_last()
        if last_move is None:
            callback(self.get_last_move_time())
            return
        last_move.timing_callbacks.append(callback)
    def get_max_velocity(self):
        return self.max_velocity, self.max_accel
    def _calc_junction_deviation(self):
        scv2 = self.square_corner_velocity**2
        self.junction_deviation = scv2 * (math.sqrt(2.) - 1.) / self.max_accel
        self.mcr_pseudo_accel = self.max_accel * (1. - self.min_cruise_ratio)
    def set_max_velocities(self, max_velocity, max_accel,
                           square_corner_velocity, min_cruise_ratio):
        if max_velocity is not None:
            self.max_velocity = max_velocity
        if max_accel is not None:
            self.max_accel = max_accel
        if square_corner_velocity is not None:
            self.square_corner_velocity = square_corner_velocity
        if min_cruise_ratio is not None:
            self.min_cruise_ratio = min_cruise_ratio
        self._calc_junction_deviation()
        return (self.max_velocity, self.max_accel,
                self.square_corner_velocity, self.min_cruise_ratio)

# Support common G-Code commands relative to the toolhead
class ToolHeadCommandHelper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.toolhead = self.printer.lookup_object("toolhead")
        # Register commands
        gcode = self.printer.lookup_object('gcode')
        gcode.register_command('G4', self.cmd_G4)
        gcode.register_command('M400', self.cmd_M400)
        gcode.register_command('SET_VELOCITY_LIMIT',
                               self.cmd_SET_VELOCITY_LIMIT,
                               desc=self.cmd_SET_VELOCITY_LIMIT_help)
        gcode.register_command('M204', self.cmd_M204)
    def cmd_G4(self, gcmd):
        # Dwell
        delay = gcmd.get_float('P', 0., minval=0.) / 1000.
        self.toolhead.dwell(delay)
    def cmd_M400(self, gcmd):
        # Wait for current moves to finish
        self.toolhead.wait_moves()
    cmd_SET_VELOCITY_LIMIT_help = "Set printer velocity limits"
    def cmd_SET_VELOCITY_LIMIT(self, gcmd):
        max_velocity = gcmd.get_float('VELOCITY', None, above=0.)
        max_accel = gcmd.get_float('ACCEL', None, above=0.)
        square_corner_velocity = gcmd.get_float(
            'SQUARE_CORNER_VELOCITY', None, minval=0.)
        min_cruise_ratio = gcmd.get_float(
            'MINIMUM_CRUISE_RATIO', None, minval=0., below=1.)
        mv, ma, scv, mcr = self.toolhead.set_max_velocities(
            max_velocity, max_accel, square_corner_velocity, min_cruise_ratio)
        msg = ("max_velocity: %.6f\n"
               "max_accel: %.6f\n"
               "minimum_cruise_ratio: %.6f\n"
               "square_corner_velocity: %.6f" % (mv, ma, mcr, scv))
        self.printer.set_rollover_info("toolhead", "toolhead: %s" % (msg,))
        if (max_velocity is None and max_accel is None
            and square_corner_velocity is None and min_cruise_ratio is None):
            gcmd.respond_info(msg, log=False)
    def cmd_M204(self, gcmd):
        # Use S for accel
        accel = gcmd.get_float('S', None, above=0.)
        if accel is None:
            # Use minimum of P and T for accel
            p = gcmd.get_float('P', None, above=0.)
            t = gcmd.get_float('T', None, above=0.)
            if p is None or t is None:
                gcmd.respond_info('Invalid M204 command "%s"'
                                  % (gcmd.get_commandline(),))
                return
            accel = min(p, t)
        self.toolhead.set_max_velocities(None, accel, None, None)

def add_printer_objects(config):
    printer = config.get_printer()
    printer.add_object('toolhead', ToolHead(config))
    ToolHeadCommandHelper(config)
    # Load default extruder objects
    kinematics.extruder.add_printer_objects(config)
    # Load some default modules
    modules = ["gcode_move", "homing", "idle_timeout", "statistics",
               "manual_probe", "tuning_tower", "garbage_collection"]
    for module_name in modules:
        printer.load_object(config, module_name)
