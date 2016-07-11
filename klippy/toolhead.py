# Code for coordinating events on the printer toolhead
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, time
import cartesian

# Common suffixes: _d is distance (in mm), _v is velocity (in
#   mm/second), _t is time (in seconds), _r is ratio (scalar between
#   0.0 and 1.0)

# Class to track each move request
class Move:
    def __init__(self, toolhead, pos, move_d, axes_d, speed, accel):
        self.toolhead = toolhead
        self.pos = tuple(pos)
        self.move_d = move_d
        self.axes_d = axes_d
        self.accel = accel
        # Junction speeds are velocities squared.  The junction_delta
        # is the maximum amount of this squared-velocity that can
        # change in this move.
        self.junction_max = speed**2
        self.junction_delta = 2.0 * move_d * accel
        self.junction_start_max = 0.
    def calc_junction(self, prev_move):
        # Find max start junction velocity using approximated
        # centripetal velocity as described at:
        # https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
        junction_cos_theta = -((self.axes_d[0] * prev_move.axes_d[0]
                                + self.axes_d[1] * prev_move.axes_d[1])
                               / (self.move_d * prev_move.move_d))
        if junction_cos_theta > 0.999999:
            return
        junction_cos_theta = max(junction_cos_theta, -0.999999)
        sin_theta_d2 = math.sqrt(0.5*(1.0-junction_cos_theta));
        R = self.toolhead.junction_deviation * sin_theta_d2 / (1. - sin_theta_d2)
        self.junction_start_max = min(
            R * self.accel, self.junction_max, prev_move.junction_max)
    def process(self, junction_start, junction_end):
        # Determine accel, cruise, and decel portions of the move distance
        junction_cruise = self.junction_max
        inv_junction_delta = 1. / self.junction_delta
        accel_r = (junction_cruise-junction_start) * inv_junction_delta
        decel_r = (junction_cruise-junction_end) * inv_junction_delta
        cruise_r = 1. - accel_r - decel_r
        if cruise_r < 0.:
            accel_r += 0.5 * cruise_r
            decel_r = 1.0 - accel_r
            cruise_r = 0.
            junction_cruise = junction_start + accel_r*self.junction_delta
        self.accel_r, self.cruise_r, self.decel_r = accel_r, cruise_r, decel_r
        # Determine move velocities
        start_v = math.sqrt(junction_start)
        cruise_v = math.sqrt(junction_cruise)
        end_v = math.sqrt(junction_end)
        self.start_v, self.cruise_v, self.end_v = start_v, cruise_v, end_v
        # Determine time spent in each portion of move (time is the
        # distance divided by average velocity)
        accel_t = accel_r * self.move_d / ((start_v + cruise_v) * 0.5)
        cruise_t = cruise_r * self.move_d / cruise_v
        decel_t = decel_r * self.move_d / ((end_v + cruise_v) * 0.5)
        self.accel_t, self.cruise_t, self.decel_t = accel_t, cruise_t, decel_t
        # Generate step times for the move
        next_move_time = self.toolhead.get_next_move_time()
        self.toolhead.kin.move(next_move_time, self)
        if self.axes_d[3]:
            self.toolhead.extruder.move(next_move_time, self)
        self.toolhead.update_move_time(accel_t + cruise_t + decel_t)

# Class to track a list of pending move requests and to facilitate
# "look-ahead" across moves to reduce acceleration between moves.
class MoveQueue:
    def __init__(self):
        self.queue = []
        self.prev_junction_max = 0.
        self.junction_flush = 0.
    def flush(self, lazy=False):
        can_flush = not lazy
        flush_count = len(self.queue)
        junction_end = [None] * flush_count
        # Traverse queue from last to first move and determine maximum
        # junction speed assuming the robot comes to a complete stop
        # after the last move.
        next_junction_max = 0.
        for i in range(len(self.queue)-1, -1, -1):
            move = self.queue[i]
            junction_end[i] = next_junction_max
            if not can_flush:
                flush_count -= 1
            next_junction_max = next_junction_max + move.junction_delta
            if next_junction_max >= move.junction_start_max:
                next_junction_max = move.junction_start_max
                can_flush = True
        # Generate step times for all moves ready to be flushed
        prev_junction_max = self.prev_junction_max
        for i in range(flush_count):
            move = self.queue[i]
            next_junction_max = min(prev_junction_max + move.junction_delta
                                    , junction_end[i])
            move.process(prev_junction_max, next_junction_max)
            prev_junction_max = next_junction_max
        # Remove processed moves from the queue
        del self.queue[:flush_count]
        self.prev_junction_max = prev_junction_max
        self.junction_flush = 0.
        if self.queue:
            self.junction_flush = self.queue[-1].junction_max
    def add_move(self, move):
        self.queue.append(move)
        if len(self.queue) == 1:
            self.junction_flush = move.junction_max
            return
        move.calc_junction(self.queue[-2])
        self.junction_flush -= move.junction_delta
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
        self.kin = cartesian.CartKinematics(printer, config)
        self.max_xy_speed, self.max_xy_accel = self.kin.get_max_xy_speed()
        self.junction_deviation = config.getfloat('junction_deviation', 0.02)
        self.move_queue = MoveQueue()
        self.commanded_pos = [0., 0., 0., 0.]
        # Print time tracking
        self.buffer_time_high = config.getfloat('buffer_time_high', 5.000)
        self.buffer_time_low = config.getfloat('buffer_time_low', 0.150)
        self.move_flush_time = config.getfloat('move_flush_time', 0.050)
        self.motor_off_delay = config.getfloat('motor_off_time', 60.000)
        self.print_time = 0.
        self.print_time_stall = 0
        self.motor_off_time = self.reactor.NEVER
        self.flush_timer = self.reactor.register_timer(self.flush_handler)
    def build_config(self):
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
        self.reset_motor_off_time(time.time())
        self.reactor.update_timer(self.flush_timer, self.motor_off_time)
    def check_busy(self, eventtime):
        if not self.print_time:
            # XXX - find better way to flush initial move_queue items
            if self.move_queue.queue:
                self.reactor.update_timer(self.flush_timer, eventtime + 0.100)
            return False
        buffer_time = self.printer.mcu.get_print_buffer_time(
            eventtime, self.print_time)
        return buffer_time > self.buffer_time_high
    def flush_handler(self, eventtime):
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
    def _move_with_z(self, newpos, axes_d, speed):
        self.move_queue.flush()
        move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
        # Limit velocity and accel to max for each stepper
        kin_speed, kin_accel = self.kin.get_max_speed(axes_d, move_d)
        speed = min(speed, self.max_xy_speed, kin_speed)
        accel = min(self.max_xy_accel, kin_accel)
        # Generate and execute move
        move = Move(self, newpos, move_d, axes_d, speed, accel)
        move.process(0., 0.)
    def _move_only_e(self, newpos, axes_d, speed):
        self.move_queue.flush()
        kin_speed, kin_accel = self.extruder.get_max_speed()
        speed = min(speed, self.max_xy_speed, kin_speed)
        accel = min(self.max_xy_accel, kin_accel)
        move = Move(self, newpos, abs(axes_d[3]), axes_d, speed, accel)
        move.process(0., 0.)
    def move(self, newpos, speed, sloppy=False):
        axes_d = [newpos[i] - self.commanded_pos[i]
                  for i in (0, 1, 2, 3)]
        self.commanded_pos[:] = newpos
        if axes_d[2]:
            self._move_with_z(newpos, axes_d, speed)
            return
        move_d = math.sqrt(axes_d[0]**2 + axes_d[1]**2)
        if not move_d:
            if axes_d[3]:
                self._move_only_e(newpos, axes_d, speed)
            return
        # Common xy move - create move and queue it
        speed = min(speed, self.max_xy_speed)
        move = Move(self, newpos, move_d, axes_d, speed, self.max_xy_accel)
        self.move_queue.add_move(move)
    def home(self, axis):
        return self.kin.home(self, axis)
    def dwell(self, delay):
        self.get_last_move_time()
        self.update_move_time(delay)
    def motor_off(self):
        self.dwell(STALL_TIME)
        last_move_time = self.get_last_move_time()
        self.kin.motor_off(last_move_time)
        self.extruder.motor_off(last_move_time)
        self.dwell(STALL_TIME)
        logging.debug('; Max time of %f' % (last_move_time,))
