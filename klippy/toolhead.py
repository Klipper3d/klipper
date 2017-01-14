# Code for coordinating events on the printer toolhead
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, time
import cartesian, delta

EXTRUDE_DIFF_IGNORE = 1.02

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
        self.do_calc_junction = self.is_kinematic_move = True
        self.axes_d = axes_d = [end_pos[i] - start_pos[i] for i in (0, 1, 2, 3)]
        if axes_d[2]:
            # Move with Z
            move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
            self.do_calc_junction = False
        else:
            move_d = math.sqrt(axes_d[0]**2 + axes_d[1]**2)
            if not move_d:
                # Extrude only move
                move_d = abs(axes_d[3])
                if not move_d:
                    # No move
                    self.move_d = 0.
                    return
                self.do_calc_junction = self.is_kinematic_move = False
        self.move_d = move_d
        self.extrude_r = axes_d[3] / move_d
        # Junction speeds are tracked in velocity squared.  The
        # delta_v2 is the maximum amount of this squared-velocity that
        # can change in this move.
        self.max_start_v2 = 0.
        self.max_cruise_v2 = speed**2
        self.delta_v2 = 2.0 * move_d * self.accel
    def limit_speed(self, speed, accel):
        self.max_cruise_v2 = min(self.max_cruise_v2, speed**2)
        if accel < self.accel:
            self.accel = accel
            self.delta_v2 = 2.0 * self.move_d * self.accel
            self.do_calc_junction = False
    def calc_junction(self, prev_move):
        if not self.do_calc_junction or not prev_move.do_calc_junction:
            return
        if (self.extrude_r > prev_move.extrude_r * EXTRUDE_DIFF_IGNORE
            or prev_move.extrude_r > self.extrude_r * EXTRUDE_DIFF_IGNORE):
            # Extrude ratio between moves is too different
            return
        self.extrude_r = prev_move.extrude_r
        # Find max velocity using approximated centripetal velocity as
        # described at:
        # https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
        junction_cos_theta = -((self.axes_d[0] * prev_move.axes_d[0]
                                + self.axes_d[1] * prev_move.axes_d[1])
                               / (self.move_d * prev_move.move_d))
        if junction_cos_theta > 0.999999:
            return
        junction_cos_theta = max(junction_cos_theta, -0.999999)
        sin_theta_d2 = math.sqrt(0.5*(1.0-junction_cos_theta))
        R = self.toolhead.junction_deviation * sin_theta_d2 / (1. - sin_theta_d2)
        self.max_start_v2 = min(
            R * self.accel, self.max_cruise_v2, prev_move.max_cruise_v2
            , prev_move.max_start_v2 + prev_move.delta_v2)
    def process(self, start_v2, cruise_v2, end_v2, min_corner_v2, max_corner_v2):
        # Determine accel, cruise, and decel portions of the move distance
        inv_delta_v2 = 1. / self.delta_v2
        self.accel_r = accel_r = (cruise_v2 - start_v2) * inv_delta_v2
        self.decel_r = decel_r = (cruise_v2 - end_v2) * inv_delta_v2
        self.cruise_r = cruise_r = 1. - accel_r - decel_r
        # Determine move velocities
        self.start_v = start_v = math.sqrt(start_v2)
        self.cruise_v = cruise_v = math.sqrt(cruise_v2)
        self.end_v = end_v = math.sqrt(end_v2)
        self.min_corner_v = math.sqrt(min_corner_v2)
        self.max_corner_v = math.sqrt(max_corner_v2)
        # Determine time spent in each portion of move (time is the
        # distance divided by average velocity)
        accel_t = accel_r * self.move_d / ((start_v + cruise_v) * 0.5)
        cruise_t = cruise_r * self.move_d / cruise_v
        decel_t = decel_r * self.move_d / ((end_v + cruise_v) * 0.5)
        self.accel_t, self.cruise_t, self.decel_t = accel_t, cruise_t, decel_t
        # Generate step times for the move
        next_move_time = self.toolhead.get_next_move_time()
        if self.is_kinematic_move:
            self.toolhead.kin.move(next_move_time, self)
        if self.axes_d[3]:
            self.toolhead.extruder.move(next_move_time, self)
        self.toolhead.update_move_time(accel_t + cruise_t + decel_t)

# Class to track a list of pending move requests and to facilitate
# "look-ahead" across moves to reduce acceleration between moves.
class MoveQueue:
    def __init__(self):
        self.queue = []
        self.junction_flush = 0.
    def reset(self):
        del self.queue[:]
    def flush(self, lazy=False):
        flush_count = len(self.queue)
        move_info = [None] * flush_count
        # Traverse queue from last to first move and determine maximum
        # junction speed assuming the robot comes to a complete stop
        # after the last move.
        next_end_v2 = min_corner_v2 = max_corner_v2 = 0.
        for i in range(flush_count-1, -1, -1):
            move = self.queue[i]
            reachable_start_v2 = next_end_v2 + move.delta_v2
            start_v2 = min(move.max_start_v2, reachable_start_v2)
            cruise_v2 = min((start_v2 + reachable_start_v2) * .5
                            , move.max_cruise_v2)
            move_info[i] = (start_v2, cruise_v2, next_end_v2
                            , min_corner_v2, max_corner_v2)
            if reachable_start_v2 > start_v2:
                min_corner_v2 = start_v2
                if start_v2 + move.delta_v2 > next_end_v2:
                    max_corner_v2 = cruise_v2
                    if lazy:
                        flush_count = i
                        lazy = False
            next_end_v2 = start_v2
        if lazy:
            flush_count = 0
        # Generate step times for all moves ready to be flushed
        for i in range(flush_count):
            self.queue[i].process(*move_info[i])
        # Remove processed moves from the queue
        del self.queue[:flush_count]
        if self.queue:
            self.junction_flush = 2. * self.queue[-1].max_cruise_v2
    def add_move(self, move):
        self.queue.append(move)
        if len(self.queue) == 1:
            self.junction_flush = 2. * move.max_cruise_v2
            return
        move.calc_junction(self.queue[-2])
        self.junction_flush -= move.delta_v2
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
        self.reactor = printer.reactor
        self.extruder = printer.objects.get('extruder')
        kintypes = {'cartesian': cartesian.CartKinematics,
                    'delta': delta.DeltaKinematics}
        self.kin = config.getchoice('kinematics', kintypes)(printer, config)
        self.max_speed = config.getfloat('max_velocity')
        self.max_accel = config.getfloat('max_accel')
        self.junction_deviation = config.getfloat('junction_deviation', 0.02)
        self.move_queue = MoveQueue()
        self.commanded_pos = [0., 0., 0., 0.]
        # Print time tracking
        self.buffer_time_high = config.getfloat('buffer_time_high', 5.000)
        self.buffer_time_low = config.getfloat('buffer_time_low', 0.150)
        self.move_flush_time = config.getfloat('move_flush_time', 0.050)
        self.motor_off_delay = config.getfloat('motor_off_time', 60.000)
        self.print_time = 0.
        self.need_check_stall = -1.
        self.print_time_stall = 0
        self.motor_off_time = self.reactor.NEVER
        self.flush_timer = self.reactor.register_timer(self._flush_handler)
    def build_config(self):
        xy_halt = 0.005 * self.max_accel # XXX
        self.kin.set_max_jerk(xy_halt, self.max_speed, self.max_accel)
        if self.extruder is not None:
            self.extruder.set_max_jerk(xy_halt, self.max_speed, self.max_accel)
        self.kin.build_config()
    # Print time tracking
    def update_move_time(self, movetime):
        self.print_time += movetime
        flush_to_time = self.print_time - self.move_flush_time
        self.printer.mcu.flush_moves(flush_to_time)
    def get_next_move_time(self):
        if not self.print_time:
            self.print_time = self.buffer_time_low + STALL_TIME
            curtime = time.time()
            self.printer.mcu.set_print_start_time(curtime)
            self.reactor.update_timer(self.flush_timer, self.reactor.NOW)
        return self.print_time
    def get_last_move_time(self):
        self.move_queue.flush()
        return self.get_next_move_time()
    def reset_motor_off_time(self, eventtime):
        self.motor_off_time = eventtime + self.motor_off_delay
    def reset_print_time(self):
        self.move_queue.flush()
        self.printer.mcu.flush_moves(self.print_time)
        self.print_time = 0.
        self.need_check_stall = -1.
        self.reset_motor_off_time(time.time())
        self.reactor.update_timer(self.flush_timer, self.motor_off_time)
    def _check_stall(self):
        if not self.print_time:
            # XXX - find better way to flush initial move_queue items
            if self.move_queue.queue:
                self.reactor.update_timer(self.flush_timer, time.time() + 0.100)
            return
        eventtime = time.time()
        while 1:
            buffer_time = self.printer.mcu.get_print_buffer_time(
                eventtime, self.print_time)
            stall_time = buffer_time - self.buffer_time_high
            if stall_time <= 0.:
                break
            eventtime = self.reactor.pause(eventtime + stall_time)
            if not self.print_time:
                return
        self.need_check_stall = self.print_time - stall_time + 0.100
    def _flush_handler(self, eventtime):
        try:
            if not self.print_time:
                self.move_queue.flush()
                if not self.print_time:
                    if eventtime >= self.motor_off_time:
                        self.motor_off()
                        self.reset_print_time()
                        self.motor_off_time = self.reactor.NEVER
                    return self.motor_off_time
            print_time = self.print_time
            buffer_time = self.printer.mcu.get_print_buffer_time(
                eventtime, print_time)
            if buffer_time > self.buffer_time_low:
                return eventtime + buffer_time - self.buffer_time_low
            self.move_queue.flush()
            if print_time != self.print_time:
                self.print_time_stall += 1
                self.dwell(self.buffer_time_low + STALL_TIME)
                return self.reactor.NOW
            self.reset_print_time()
            return self.motor_off_time
        except:
            logging.exception("Exception in flush_handler")
            self.force_shutdown()
    def stats(self, eventtime):
        buffer_time = 0.
        if self.print_time:
            buffer_time = self.printer.mcu.get_print_buffer_time(
                eventtime, self.print_time)
        return "print_time=%.3f buffer_time=%.3f print_time_stall=%d" % (
            self.print_time, buffer_time, self.print_time_stall)
    # Movement commands
    def get_position(self):
        return list(self.commanded_pos)
    def set_position(self, newpos):
        self.move_queue.flush()
        self.commanded_pos[:] = newpos
        self.kin.set_position(newpos)
    def move(self, newpos, speed):
        speed = min(speed, self.max_speed)
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
    def home(self, homing_state):
        self.kin.home(homing_state)
    def dwell(self, delay):
        self.get_last_move_time()
        self.update_move_time(delay)
        self._check_stall()
    def motor_off(self):
        self.dwell(STALL_TIME)
        last_move_time = self.get_last_move_time()
        self.kin.motor_off(last_move_time)
        self.extruder.motor_off(last_move_time)
        self.dwell(STALL_TIME)
        logging.debug('; Max time of %f' % (last_move_time,))
    def wait_moves(self):
        self.move_queue.flush()
        eventtime = time.time()
        while self.print_time:
            eventtime = self.reactor.pause(eventtime + 0.100)
    def query_endstops(self):
        last_move_time = self.get_last_move_time()
        return self.kin.query_endstops(last_move_time)
    def force_shutdown(self):
        self.printer.mcu.force_shutdown()
        self.move_queue.reset()
        self.reset_print_time()
