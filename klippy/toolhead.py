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
        self.max_smoothed_v2 = 0.
        self.smooth_delta_v2 = 2.0 * move_d * toolhead.max_accel_to_decel
        self.next_junction_v2 = 999999999.9
    def limit_speed(self, speed, accel):
        speed2 = speed**2
        if speed2 < self.max_cruise_v2:
            self.max_cruise_v2 = speed2
            self.min_move_t = self.move_d / speed
        self.accel = min(self.accel, accel)
        self.delta_v2 = 2.0 * self.move_d * self.accel
        self.smooth_delta_v2 = min(self.smooth_delta_v2, self.delta_v2)
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
        self.max_smoothed_v2 = min(
            max_start_v2, prev_move.max_smoothed_v2 + prev_move.smooth_delta_v2)
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

LOOKAHEAD_FLUSH_TIME = 0.250

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
        delayed = []
        next_end_v2 = next_smoothed_v2 = peak_cruise_v2 = 0.
        for i in range(flush_count-1, -1, -1):
            move = queue[i]
            reachable_start_v2 = next_end_v2 + move.delta_v2
            start_v2 = min(move.max_start_v2, reachable_start_v2)
            reachable_smoothed_v2 = next_smoothed_v2 + move.smooth_delta_v2
            smoothed_v2 = min(move.max_smoothed_v2, reachable_smoothed_v2)
            if smoothed_v2 < reachable_smoothed_v2:
                # It's possible for this move to accelerate
                if (smoothed_v2 + move.smooth_delta_v2 > next_smoothed_v2
                    or delayed):
                    # This move can decelerate or this is a full accel
                    # move after a full decel move
                    if update_flush_count and peak_cruise_v2:
                        flush_count = i
                        update_flush_count = False
                    peak_cruise_v2 = min(move.max_cruise_v2, (
                        smoothed_v2 + reachable_smoothed_v2) * .5)
                    if delayed:
                        # Propagate peak_cruise_v2 to any delayed moves
                        if not update_flush_count and i < flush_count:
                            mc_v2 = peak_cruise_v2
                            for m, ms_v2, me_v2 in reversed(delayed):
                                mc_v2 = min(mc_v2, ms_v2)
                                m.set_junction(min(ms_v2, mc_v2), mc_v2
                                               , min(me_v2, mc_v2))
                        del delayed[:]
                if not update_flush_count and i < flush_count:
                    cruise_v2 = min((start_v2 + reachable_start_v2) * .5
                                    , move.max_cruise_v2, peak_cruise_v2)
                    move.set_junction(min(start_v2, cruise_v2), cruise_v2
                                      , min(next_end_v2, cruise_v2))
            else:
                # Delay calculating this move until peak_cruise_v2 is known
                delayed.append((move, start_v2, next_end_v2))
            next_end_v2 = start_v2
            next_smoothed_v2 = smoothed_v2
        if update_flush_count or not flush_count:
            return []
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

BUFFER_TIME_LOW = 1.0
BUFFER_TIME_HIGH = 2.0
BUFFER_TIME_START = 0.250
BGFLUSH_LOW_TIME = 0.200
BGFLUSH_BATCH_TIME = 0.200
BGFLUSH_EXTRA_TIME = 0.250
MIN_KIN_TIME = 0.100
MOVE_BATCH_TIME = 0.500
STEPCOMPRESS_FLUSH_TIME = 0.050
SDS_CHECK_TIME = 0.001 # step+dir+step filter in stepcompress.c
MOVE_HISTORY_EXPIRE = 30.

DRIP_SEGMENT_TIME = 0.050
DRIP_TIME = 0.100

# Main code to track events (and their timing) on the printer toolhead
class ToolHead:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.all_mcus = [
            m for n, m in self.printer.lookup_objects(module='mcu')]
        self.mcu = self.all_mcus[0]
        self.lookahead = LookAheadQueue()
        self.lookahead.set_flush_time(BUFFER_TIME_HIGH)
        self.commanded_pos = [0., 0., 0., 0.]
        # Velocity and acceleration control
        self.max_velocity = config.getfloat('max_velocity', above=0.)
        self.max_accel = config.getfloat('max_accel', above=0.)
        min_cruise_ratio = 0.5
        if config.getfloat('minimum_cruise_ratio', None) is None:
            req_accel_to_decel = config.getfloat('max_accel_to_decel', None,
                                                 above=0.)
            if req_accel_to_decel is not None:
                config.deprecate('max_accel_to_decel')
                min_cruise_ratio = 1. - min(1., (req_accel_to_decel
                                                 / self.max_accel))
        self.min_cruise_ratio = config.getfloat('minimum_cruise_ratio',
                                                min_cruise_ratio,
                                                below=1., minval=0.)
        self.square_corner_velocity = config.getfloat(
            'square_corner_velocity', 5., minval=0.)
        self.junction_deviation = self.max_accel_to_decel = 0.
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
        # Flush tracking
        self.flush_timer = self.reactor.register_timer(self._flush_handler)
        self.do_kick_flush_timer = True
        self.last_flush_time = self.min_restart_time = 0.
        self.need_flush_time = self.step_gen_time = self.clear_history_time = 0.
        # Kinematic step generation scan window time tracking
        self.kin_flush_delay = SDS_CHECK_TIME
        self.kin_flush_times = []
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapq_append = ffi_lib.trapq_append
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
        # Motion flushing
        self.step_generators = []
        self.flush_trapqs = [self.trapq]
        # Create kinematics class
        gcode = self.printer.lookup_object('gcode')
        self.Coord = gcode.Coord
        extruder = kinematics.extruder.DummyExtruder(self.printer)
        self.extra_axes = [extruder]
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
        # Register commands
        gcode.register_command('G4', self.cmd_G4)
        gcode.register_command('M400', self.cmd_M400)
        gcode.register_command('SET_VELOCITY_LIMIT',
                               self.cmd_SET_VELOCITY_LIMIT,
                               desc=self.cmd_SET_VELOCITY_LIMIT_help)
        gcode.register_command('M204', self.cmd_M204)
        self.printer.register_event_handler("klippy:shutdown",
                                            self._handle_shutdown)
        # Load some default modules
        modules = ["gcode_move", "homing", "idle_timeout", "statistics",
                   "manual_probe", "tuning_tower", "garbage_collection"]
        for module_name in modules:
            self.printer.load_object(config, module_name)
    # Print time and flush tracking
    def _advance_flush_time(self, flush_time):
        flush_time = max(flush_time, self.last_flush_time)
        # Generate steps via itersolve
        sg_flush_want = min(flush_time + STEPCOMPRESS_FLUSH_TIME,
                            self.print_time - self.kin_flush_delay)
        sg_flush_time = max(sg_flush_want, flush_time)
        for sg in self.step_generators:
            sg(sg_flush_time)
        self.min_restart_time = max(self.min_restart_time, sg_flush_time)
        # Free trapq entries that are no longer needed
        clear_history_time = self.clear_history_time
        if not self.can_pause:
            clear_history_time = flush_time - MOVE_HISTORY_EXPIRE
        free_time = sg_flush_time - self.kin_flush_delay
        for trapq in self.flush_trapqs:
            self.trapq_finalize_moves(trapq, free_time, clear_history_time)
        # Flush stepcompress and mcu steppersync
        for m in self.all_mcus:
            m.flush_moves(flush_time, clear_history_time)
        self.last_flush_time = flush_time
    def _advance_move_time(self, next_print_time):
        pt_delay = self.kin_flush_delay + STEPCOMPRESS_FLUSH_TIME
        flush_time = max(self.last_flush_time, self.print_time - pt_delay)
        self.print_time = max(self.print_time, next_print_time)
        want_flush_time = max(flush_time, self.print_time - pt_delay)
        while 1:
            flush_time = min(flush_time + MOVE_BATCH_TIME, want_flush_time)
            self._advance_flush_time(flush_time)
            if flush_time >= want_flush_time:
                break
    def _calc_print_time(self):
        curtime = self.reactor.monotonic()
        est_print_time = self.mcu.estimated_print_time(curtime)
        kin_time = max(est_print_time + MIN_KIN_TIME, self.min_restart_time)
        kin_time += self.kin_flush_delay
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
        self.note_mcu_movequeue_activity(next_move_time + self.kin_flush_delay,
                                         set_step_gen_time=True)
        self._advance_move_time(next_move_time)
    def _flush_lookahead(self):
        # Transit from "NeedPrime"/"Priming"/"Drip"/main state to "NeedPrime"
        self._process_lookahead()
        self.special_queuing_state = "NeedPrime"
        self.need_check_pause = -1.
        self.lookahead.set_flush_time(BUFFER_TIME_HIGH)
        self.check_stall_time = 0.
    def flush_step_generation(self):
        self._flush_lookahead()
        self._advance_flush_time(self.step_gen_time)
        self.min_restart_time = max(self.min_restart_time, self.print_time)
    def get_last_move_time(self):
        if self.special_queuing_state:
            self._flush_lookahead()
            self._calc_print_time()
        else:
            self._process_lookahead()
        return self.print_time
    def _check_pause(self):
        eventtime = self.reactor.monotonic()
        est_print_time = self.mcu.estimated_print_time(eventtime)
        buffer_time = self.print_time - est_print_time
        if self.special_queuing_state:
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
            wtime = eventtime + max(0.100, buffer_time - BUFFER_TIME_LOW)
            self.reactor.update_timer(self.priming_timer, wtime)
        # Check if there are lots of queued moves and pause if so
        while 1:
            pause_time = buffer_time - BUFFER_TIME_HIGH
            if pause_time <= 0.:
                break
            if not self.can_pause:
                self.need_check_pause = self.reactor.NEVER
                return
            eventtime = self.reactor.pause(eventtime + min(1., pause_time))
            est_print_time = self.mcu.estimated_print_time(eventtime)
            buffer_time = self.print_time - est_print_time
        if not self.special_queuing_state:
            # In main state - defer pause checking until needed
            self.need_check_pause = est_print_time + BUFFER_TIME_HIGH + 0.100
    def _priming_handler(self, eventtime):
        self.reactor.unregister_timer(self.priming_timer)
        self.priming_timer = None
        try:
            if self.special_queuing_state == "Priming":
                self._flush_lookahead()
                self.check_stall_time = self.print_time
        except:
            logging.exception("Exception in priming_handler")
            self.printer.invoke_shutdown("Exception in priming_handler")
        return self.reactor.NEVER
    def _flush_handler(self, eventtime):
        try:
            est_print_time = self.mcu.estimated_print_time(eventtime)
            if not self.special_queuing_state:
                # In "main" state - flush lookahead if buffer runs low
                print_time = self.print_time
                buffer_time = print_time - est_print_time
                if buffer_time > BUFFER_TIME_LOW:
                    # Running normally - reschedule check
                    return eventtime + buffer_time - BUFFER_TIME_LOW
                # Under ran low buffer mark - flush lookahead queue
                self._flush_lookahead()
                if print_time != self.print_time:
                    self.check_stall_time = self.print_time
            # In "NeedPrime"/"Priming" state - flush queues if needed
            while 1:
                end_flush = self.need_flush_time + BGFLUSH_EXTRA_TIME
                if self.last_flush_time >= end_flush:
                    self.do_kick_flush_timer = True
                    return self.reactor.NEVER
                buffer_time = self.last_flush_time - est_print_time
                if buffer_time > BGFLUSH_LOW_TIME:
                    return eventtime + buffer_time - BGFLUSH_LOW_TIME
                ftime = est_print_time + BGFLUSH_LOW_TIME + BGFLUSH_BATCH_TIME
                self._advance_flush_time(min(end_flush, ftime))
        except:
            logging.exception("Exception in flush_handler")
            self.printer.invoke_shutdown("Exception in flush_handler")
        return self.reactor.NEVER
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
    def set_extruder(self, extruder, extrude_pos):
        # XXX - should use add_extra_axis
        prev_ea_trapq = self.extra_axes[0].get_trapq()
        if prev_ea_trapq in self.flush_trapqs:
            self.flush_trapqs.remove(prev_ea_trapq)
        self.extra_axes[0] = extruder
        self.commanded_pos[3] = extrude_pos
        ea_trapq = extruder.get_trapq()
        if ea_trapq is not None:
            self.flush_trapqs.append(ea_trapq)
    def get_extruder(self):
        return self.extra_axes[0]
    def add_extra_axis(self, ea, axis_pos):
        self._flush_lookahead()
        self.extra_axes.append(ea)
        self.commanded_pos.append(axis_pos)
        ea_trapq = ea.get_trapq()
        if ea_trapq is not None:
            self.flush_trapqs.append(ea_trapq)
        self.printer.send_event("toolhead:update_extra_axes")
    def remove_extra_axis(self, ea):
        self._flush_lookahead()
        if ea not in self.extra_axes:
            return
        ea_index = self.extra_axes.index(ea) + 3
        ea_trapq = ea.get_trapq()
        if ea_trapq in self.flush_trapqs:
            self.flush_trapqs.remove(ea_trapq)
        self.commanded_pos.pop(ea_index)
        self.extra_axes.pop(ea_index - 3)
        self.printer.send_event("toolhead:update_extra_axes")
    def get_extra_axes(self):
        return [None, None, None] + self.extra_axes
    # Homing "drip move" handling
    def drip_update_time(self, next_print_time, drip_completion, addstepper=()):
        # Transition from "NeedPrime"/"Priming"/main state to "Drip" state
        self.special_queuing_state = "Drip"
        self.need_check_pause = self.reactor.NEVER
        self.reactor.update_timer(self.flush_timer, self.reactor.NEVER)
        self.do_kick_flush_timer = False
        self.lookahead.set_flush_time(BUFFER_TIME_HIGH)
        self.check_stall_time = 0.
        # Update print_time in segments until drip_completion signal
        flush_delay = DRIP_TIME + STEPCOMPRESS_FLUSH_TIME + self.kin_flush_delay
        while self.print_time < next_print_time:
            if drip_completion.test():
                break
            curtime = self.reactor.monotonic()
            est_print_time = self.mcu.estimated_print_time(curtime)
            wait_time = self.print_time - est_print_time - flush_delay
            if wait_time > 0. and self.can_pause:
                # Pause before sending more steps
                drip_completion.wait(curtime + wait_time)
                continue
            npt = min(self.print_time + DRIP_SEGMENT_TIME, next_print_time)
            self.note_mcu_movequeue_activity(npt + self.kin_flush_delay,
                                             set_step_gen_time=True)
            for stepper in addstepper:
                stepper.generate_steps(npt)
            self._advance_move_time(npt)
        # Exit "Drip" state
        self.reactor.update_timer(self.flush_timer, self.reactor.NOW)
        self.flush_step_generation()
    def _drip_load_trapq(self, submit_move):
        # Queue move into trapezoid motion queue (trapq)
        if submit_move.move_d:
            self.commanded_pos[:] = submit_move.end_pos
            self.lookahead.add_move(submit_move)
        moves = self.lookahead.flush()
        self._calc_print_time()
        next_move_time = self.print_time
        for move in moves:
            self.trapq_append(
                self.trapq, next_move_time,
                move.accel_t, move.cruise_t, move.decel_t,
                move.start_pos[0], move.start_pos[1], move.start_pos[2],
                move.axes_r[0], move.axes_r[1], move.axes_r[2],
                move.start_v, move.cruise_v, move.accel)
            next_move_time = (next_move_time + move.accel_t
                              + move.cruise_t + move.decel_t)
        self.lookahead.reset()
        return next_move_time
    def drip_move(self, newpos, speed, drip_completion):
        # Create and verify move is valid
        newpos = newpos[:3] + self.commanded_pos[3:]
        move = Move(self, self.commanded_pos, newpos, speed)
        if move.move_d:
            self.kin.check_move(move)
        # Make sure stepper movement doesn't start before nominal start time
        self.dwell(self.kin_flush_delay)
        # Transmit move in "drip" mode
        self._process_lookahead()
        next_move_time = self._drip_load_trapq(move)
        self.drip_update_time(next_move_time, drip_completion)
        # Move finished; cleanup any remnants on trapq
        self.trapq_finalize_moves(self.trapq, self.reactor.NEVER, 0)
    # Misc commands
    def stats(self, eventtime):
        max_queue_time = max(self.print_time, self.last_flush_time)
        for m in self.all_mcus:
            m.check_active(max_queue_time, eventtime)
        est_print_time = self.mcu.estimated_print_time(eventtime)
        self.clear_history_time = est_print_time - MOVE_HISTORY_EXPIRE
        buffer_time = self.print_time - est_print_time
        is_active = buffer_time > -60. or not self.special_queuing_state
        if self.special_queuing_state == "Drip":
            buffer_time = 0.
        return is_active, "print_time=%.3f buffer_time=%.3f print_stall=%d" % (
            self.print_time, max(buffer_time, 0.), self.print_stall)
    def check_busy(self, eventtime):
        est_print_time = self.mcu.estimated_print_time(eventtime)
        lookahead_empty = not self.lookahead.queue
        return self.print_time, est_print_time, lookahead_empty
    def get_status(self, eventtime):
        print_time = self.print_time
        estimated_print_time = self.mcu.estimated_print_time(eventtime)
        extruder = self.extra_axes[0]
        res = dict(self.kin.get_status(eventtime))
        res.update({ 'print_time': print_time,
                     'stalls': self.print_stall,
                     'estimated_print_time': estimated_print_time,
                     'extruder': extruder.get_name(),
                     'position': self.Coord(*self.commanded_pos[:4]),
                     'max_velocity': self.max_velocity,
                     'max_accel': self.max_accel,
                     'minimum_cruise_ratio': self.min_cruise_ratio,
                     'square_corner_velocity': self.square_corner_velocity})
        return res
    def _handle_shutdown(self):
        self.can_pause = False
        self.lookahead.reset()
    def get_kinematics(self):
        return self.kin
    def get_trapq(self):
        return self.trapq
    def register_step_generator(self, handler):
        self.step_generators.append(handler)
    def unregister_step_generator(self, handler):
        if handler in self.step_generators:
            self.step_generators.remove(handler)
    def note_step_generation_scan_time(self, delay, old_delay=0.):
        self.flush_step_generation()
        if old_delay:
            self.kin_flush_times.pop(self.kin_flush_times.index(old_delay))
        if delay:
            self.kin_flush_times.append(delay)
        new_delay = max(self.kin_flush_times + [SDS_CHECK_TIME])
        self.kin_flush_delay = new_delay
    def register_lookahead_callback(self, callback):
        last_move = self.lookahead.get_last()
        if last_move is None:
            callback(self.get_last_move_time())
            return
        last_move.timing_callbacks.append(callback)
    def note_mcu_movequeue_activity(self, mq_time, set_step_gen_time=False):
        self.need_flush_time = max(self.need_flush_time, mq_time)
        if set_step_gen_time:
            self.step_gen_time = max(self.step_gen_time, mq_time)
        if self.do_kick_flush_timer:
            self.do_kick_flush_timer = False
            self.reactor.update_timer(self.flush_timer, self.reactor.NOW)
    def get_max_velocity(self):
        return self.max_velocity, self.max_accel
    def _calc_junction_deviation(self):
        scv2 = self.square_corner_velocity**2
        self.junction_deviation = scv2 * (math.sqrt(2.) - 1.) / self.max_accel
        self.max_accel_to_decel = self.max_accel * (1. - self.min_cruise_ratio)
    def cmd_G4(self, gcmd):
        # Dwell
        delay = gcmd.get_float('P', 0., minval=0.) / 1000.
        self.dwell(delay)
    def cmd_M400(self, gcmd):
        # Wait for current moves to finish
        self.wait_moves()
    cmd_SET_VELOCITY_LIMIT_help = "Set printer velocity limits"
    def cmd_SET_VELOCITY_LIMIT(self, gcmd):
        max_velocity = gcmd.get_float('VELOCITY', None, above=0.)
        max_accel = gcmd.get_float('ACCEL', None, above=0.)
        square_corner_velocity = gcmd.get_float(
            'SQUARE_CORNER_VELOCITY', None, minval=0.)
        min_cruise_ratio = gcmd.get_float(
            'MINIMUM_CRUISE_RATIO', None, minval=0., below=1.)
        if min_cruise_ratio is None:
            req_accel_to_decel = gcmd.get_float('ACCEL_TO_DECEL',
                                                None, above=0.)
            if req_accel_to_decel is not None and max_accel is not None:
                min_cruise_ratio = 1. - min(1., req_accel_to_decel / max_accel)
            elif req_accel_to_decel is not None and max_accel is None:
                min_cruise_ratio = 1. - min(1., (req_accel_to_decel
                                                 / self.max_accel))
        if max_velocity is not None:
            self.max_velocity = max_velocity
        if max_accel is not None:
            self.max_accel = max_accel
        if square_corner_velocity is not None:
            self.square_corner_velocity = square_corner_velocity
        if min_cruise_ratio is not None:
            self.min_cruise_ratio = min_cruise_ratio
        self._calc_junction_deviation()
        msg = ("max_velocity: %.6f\n"
               "max_accel: %.6f\n"
               "minimum_cruise_ratio: %.6f\n"
               "square_corner_velocity: %.6f" % (
                   self.max_velocity, self.max_accel,
                   self.min_cruise_ratio, self.square_corner_velocity))
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
        self.max_accel = accel
        self._calc_junction_deviation()

def add_printer_objects(config):
    config.get_printer().add_object('toolhead', ToolHead(config))
    kinematics.extruder.add_printer_objects(config)
