# Code for coordinating events on the printer toolhead
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import mcu, homing, cartesian, corexy, delta, extruder

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
        self.is_kinematic_move = True
        self.axes_d = axes_d = [end_pos[i] - start_pos[i] for i in (0, 1, 2, 3)]
        self.move_d = move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
        if not move_d:
            # Extrude only move
            self.move_d = move_d = abs(axes_d[3])
            self.is_kinematic_move = False
        self.min_move_t = move_d / speed
        # Junction speeds are tracked in velocity squared.  The
        # delta_v2 is the maximum amount of this squared-velocity that
        # can change in this move.
        self.max_start_v2 = 0.
        self.max_cruise_v2 = speed**2
        self.delta_v2 = 2.0 * move_d * self.accel
        self.max_smoothed_v2 = 0.
        self.smooth_delta_v2 = 2.0 * move_d * toolhead.max_accel_to_decel
    def limit_speed(self, speed, accel):
        speed2 = speed**2
        if speed2 < self.max_cruise_v2:
            self.max_cruise_v2 = speed2
            self.min_move_t = self.move_d / speed
        self.accel = min(self.accel, accel)
        self.delta_v2 = 2.0 * self.move_d * self.accel
        self.smooth_delta_v2 = min(self.smooth_delta_v2, self.delta_v2)
    def calc_junction(self, prev_move):
        if not self.is_kinematic_move or not prev_move.is_kinematic_move:
            return
        # Allow extruder to calculate its maximum junction
        extruder_v2 = self.toolhead.extruder.calc_junction(prev_move, self)
        # Find max velocity using approximated centripetal velocity as
        # described at:
        # https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
        axes_d = self.axes_d
        prev_axes_d = prev_move.axes_d
        junction_cos_theta = -((axes_d[0] * prev_axes_d[0]
                                + axes_d[1] * prev_axes_d[1]
                                + axes_d[2] * prev_axes_d[2])
                               / (self.move_d * prev_move.move_d))
        if junction_cos_theta > 0.999999:
            return
        junction_cos_theta = max(junction_cos_theta, -0.999999)
        sin_theta_d2 = math.sqrt(0.5*(1.0-junction_cos_theta))
        R = self.toolhead.junction_deviation * sin_theta_d2 / (1. - sin_theta_d2)
        self.max_start_v2 = min(
            R * self.accel, R * prev_move.accel, extruder_v2
            , self.max_cruise_v2, prev_move.max_cruise_v2
            , prev_move.max_start_v2 + prev_move.delta_v2)
        self.max_smoothed_v2 = min(
            self.max_start_v2
            , prev_move.max_smoothed_v2 + prev_move.smooth_delta_v2)
    def set_junction(self, start_v2, cruise_v2, end_v2):
        # Determine accel, cruise, and decel portions of the move distance
        inv_delta_v2 = 1. / self.delta_v2
        self.accel_r = accel_r = (cruise_v2 - start_v2) * inv_delta_v2
        self.decel_r = decel_r = (cruise_v2 - end_v2) * inv_delta_v2
        self.cruise_r = cruise_r = 1. - accel_r - decel_r
        # Determine move velocities
        self.start_v = start_v = math.sqrt(start_v2)
        self.cruise_v = cruise_v = math.sqrt(cruise_v2)
        self.end_v = end_v = math.sqrt(end_v2)
        # Determine time spent in each portion of move (time is the
        # distance divided by average velocity)
        self.accel_t = accel_r * self.move_d / ((start_v + cruise_v) * 0.5)
        self.cruise_t = cruise_r * self.move_d / cruise_v
        self.decel_t = decel_r * self.move_d / ((end_v + cruise_v) * 0.5)
    def move(self):
        # Generate step times for the move
        next_move_time = self.toolhead.get_next_move_time()
        if self.is_kinematic_move:
            self.toolhead.kin.move(next_move_time, self)
        if self.axes_d[3]:
            self.toolhead.extruder.move(next_move_time, self)
        self.toolhead.update_move_time(
            self.accel_t + self.cruise_t + self.decel_t)

LOOKAHEAD_FLUSH_TIME = 0.250

# Class to track a list of pending move requests and to facilitate
# "look-ahead" across moves to reduce acceleration between moves.
class MoveQueue:
    def __init__(self):
        self.extruder_lookahead = None
        self.queue = []
        self.leftover = 0
        self.junction_flush = LOOKAHEAD_FLUSH_TIME
    def reset(self):
        del self.queue[:]
        self.leftover = 0
        self.junction_flush = LOOKAHEAD_FLUSH_TIME
    def set_flush_time(self, flush_time):
        self.junction_flush = flush_time
    def set_extruder(self, extruder):
        self.extruder_lookahead = extruder.lookahead
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
        for i in range(flush_count-1, self.leftover-1, -1):
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
                            for m, ms_v2, me_v2 in delayed:
                                mc_v2 = min(peak_cruise_v2, ms_v2)
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
        if update_flush_count:
            return
        # Allow extruder to do its lookahead
        move_count = self.extruder_lookahead(queue, flush_count, lazy)
        # Generate step times for all moves ready to be flushed
        for move in queue[:move_count]:
            move.move()
        # Remove processed moves from the queue
        self.leftover = flush_count - move_count
        del queue[:move_count]
    def add_move(self, move):
        self.queue.append(move)
        if len(self.queue) == 1:
            return
        move.calc_junction(self.queue[-2])
        self.junction_flush -= move.min_move_t
        if self.junction_flush <= 0.:
            # There are enough queued moves to return to zero velocity
            # from the first move's maximum possible velocity, so at
            # least one move can be flushed.
            self.flush(lazy=True)

STALL_TIME = 0.100

# Main code to track events (and their timing) on the printer toolhead
class ToolHead:
    def __init__(self, printer, config):
        self.printer = printer
        self.reactor = printer.get_reactor()
        self.all_mcus = printer.lookup_module_objects('mcu')
        self.mcu = self.all_mcus[0]
        self.max_velocity = config.getfloat('max_velocity', above=0.)
        self.max_accel = config.getfloat('max_accel', above=0.)
        self.max_accel_to_decel = config.getfloat(
            'max_accel_to_decel', self.max_accel * 0.5
            , above=0., maxval=self.max_accel)
        self.junction_deviation = config.getfloat(
            'junction_deviation', 0.02, minval=0.)
        self.move_queue = MoveQueue()
        self.commanded_pos = [0., 0., 0., 0.]
        # Print time tracking
        self.buffer_time_low = config.getfloat(
            'buffer_time_low', 1.000, above=0.)
        self.buffer_time_high = config.getfloat(
            'buffer_time_high', 2.000, above=self.buffer_time_low)
        self.buffer_time_start = config.getfloat(
            'buffer_time_start', 0.250, above=0.)
        self.move_flush_time = config.getfloat(
            'move_flush_time', 0.050, above=0.)
        self.print_time = 0.
        self.need_check_stall = -1.
        self.print_stall = 0
        self.sync_print_time = True
        self.idle_flush_print_time = 0.
        self.flush_timer = self.reactor.register_timer(self._flush_handler)
        self.move_queue.set_flush_time(self.buffer_time_high)
        # Motor off tracking
        self.need_motor_off = False
        self.motor_off_time = config.getfloat('motor_off_time', 600., above=0.)
        self.motor_off_timer = self.reactor.register_timer(
            self._motor_off_handler, self.reactor.NOW)
        # Create kinematics class
        self.extruder = extruder.DummyExtruder()
        self.move_queue.set_extruder(self.extruder)
        kintypes = {'cartesian': cartesian.CartKinematics,
                    'corexy': corexy.CoreXYKinematics,
                    'delta': delta.DeltaKinematics}
        self.kin = config.getchoice('kinematics', kintypes)(
            self, printer, config)
    # Print time tracking
    def update_move_time(self, movetime):
        self.print_time += movetime
        flush_to_time = self.print_time - self.move_flush_time
        for m in self.all_mcus:
            m.flush_moves(flush_to_time)
    def get_next_move_time(self):
        if not self.sync_print_time:
            return self.print_time
        self.sync_print_time = False
        self.need_motor_off = True
        est_print_time = self.mcu.estimated_print_time(self.reactor.monotonic())
        self.print_time = max(
            self.print_time, est_print_time + self.buffer_time_start)
        self.reactor.update_timer(self.flush_timer, self.reactor.NOW)
        return self.print_time
    def _flush_lookahead(self, must_sync=False):
        sync_print_time = self.sync_print_time
        self.move_queue.flush()
        self.idle_flush_print_time = 0.
        if sync_print_time or must_sync:
            self.sync_print_time = True
            self.move_queue.set_flush_time(self.buffer_time_high)
            self.need_check_stall = -1.
            self.reactor.update_timer(self.flush_timer, self.reactor.NEVER)
            for m in self.all_mcus:
                m.flush_moves(self.print_time)
    def get_last_move_time(self):
        self._flush_lookahead()
        return self.get_next_move_time()
    def reset_print_time(self, min_print_time=0.):
        self._flush_lookahead(must_sync=True)
        self.print_time = max(min_print_time, self.mcu.estimated_print_time(
            self.reactor.monotonic()))
    def _check_stall(self):
        eventtime = self.reactor.monotonic()
        if self.sync_print_time:
            # Building initial queue - make sure to flush on idle input
            if self.idle_flush_print_time:
                est_print_time = self.mcu.estimated_print_time(eventtime)
                if est_print_time < self.idle_flush_print_time:
                    self.print_stall += 1
                self.idle_flush_print_time = 0.
            self.reactor.update_timer(self.flush_timer, eventtime + 0.100)
            return
        # Check if there are lots of queued moves and stall if so
        while 1:
            est_print_time = self.mcu.estimated_print_time(eventtime)
            buffer_time = self.print_time - est_print_time
            stall_time = buffer_time - self.buffer_time_high
            if stall_time <= 0.:
                break
            if self.mcu.is_fileoutput():
                self.need_check_stall = self.reactor.NEVER
                return
            eventtime = self.reactor.pause(eventtime + min(1., stall_time))
        self.need_check_stall = est_print_time + self.buffer_time_high + 0.100
    def _flush_handler(self, eventtime):
        try:
            print_time = self.print_time
            buffer_time = print_time - self.mcu.estimated_print_time(eventtime)
            if buffer_time > self.buffer_time_low:
                # Running normally - reschedule check
                return eventtime + buffer_time - self.buffer_time_low
            # Under ran low buffer mark - flush lookahead queue
            self._flush_lookahead(must_sync=True)
            if print_time != self.print_time:
                self.idle_flush_print_time = self.print_time
        except:
            logging.exception("Exception in flush_handler")
            self.printer.invoke_shutdown("Exception in flush_handler")
        return self.reactor.NEVER
    # Motor off timer
    def _motor_off_handler(self, eventtime):
        if not self.need_motor_off or not self.sync_print_time:
            return eventtime + self.motor_off_time
        elapsed_time = self.mcu.estimated_print_time(eventtime) - self.print_time
        if elapsed_time < self.motor_off_time:
            return eventtime + self.motor_off_time - elapsed_time
        try:
            self.motor_off()
        except:
            logging.exception("Exception in motor_off_handler")
            self.printer.invoke_shutdown("Exception in motor_off_handler")
        return eventtime + self.motor_off_time
    # Movement commands
    def get_position(self):
        return list(self.commanded_pos)
    def set_position(self, newpos):
        self._flush_lookahead()
        self.commanded_pos[:] = newpos
        self.kin.set_position(newpos)
    def move(self, newpos, speed):
        speed = min(speed, self.max_velocity)
        move = Move(self, self.commanded_pos, newpos, speed)
        if not move.move_d:
            return
        if move.is_kinematic_move:
            self.kin.check_move(move)
        if move.axes_d[3]:
            self.extruder.check_move(move)
        self.commanded_pos[:] = newpos
        self.move_queue.add_move(move)
        if self.print_time > self.need_check_stall:
            self._check_stall()
    def dwell(self, delay, check_stall=True):
        self.get_last_move_time()
        self.update_move_time(delay)
        if check_stall:
            self._check_stall()
    def motor_off(self):
        self.dwell(STALL_TIME)
        last_move_time = self.get_last_move_time()
        self.kin.motor_off(last_move_time)
        self.extruder.motor_off(last_move_time)
        self.dwell(STALL_TIME)
        self.need_motor_off = False
        logging.debug('; Max time of %f', last_move_time)
    def wait_moves(self):
        self._flush_lookahead()
        if self.mcu.is_fileoutput():
            return
        eventtime = self.reactor.monotonic()
        while (not self.sync_print_time
               or self.print_time >= self.mcu.estimated_print_time(eventtime)):
            eventtime = self.reactor.pause(eventtime + 0.100)
    def set_extruder(self, extruder):
        last_move_time = self.get_last_move_time()
        self.extruder.set_active(last_move_time, False)
        extrude_pos = extruder.set_active(last_move_time, True)
        self.extruder = extruder
        self.move_queue.set_extruder(extruder)
        self.commanded_pos[3] = extrude_pos
    # Misc commands
    def check_active(self, eventtime):
        for m in self.all_mcus:
            m.check_active(self.print_time, eventtime)
        if not self.sync_print_time:
            return True
        return self.print_time + 60. > self.mcu.estimated_print_time(eventtime)
    def stats(self, eventtime):
        est_print_time = self.mcu.estimated_print_time(eventtime)
        buffer_time = max(0., self.print_time - est_print_time)
        return "print_time=%.3f buffer_time=%.3f print_stall=%d" % (
            self.print_time, buffer_time, self.print_stall)
    def printer_state(self, state):
        if state == 'shutdown':
            try:
                self.move_queue.reset()
                self.reset_print_time()
            except:
                logging.exception("Exception in toolhead shutdown")
    def get_kinematics(self):
        return self.kin
    def get_max_velocity(self):
        return self.max_velocity, self.max_accel
    def get_max_axis_halt(self):
        # Determine the maximum velocity a cartesian axis could halt
        # at due to the junction_deviation setting.  The 8.0 was
        # determined experimentally.
        return min(self.max_velocity,
                   math.sqrt(8. * self.junction_deviation * self.max_accel))

def add_printer_objects(printer, config):
    printer.add_object('toolhead', ToolHead(printer, config))
