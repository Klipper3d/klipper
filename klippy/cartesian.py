# Code for handling cartesian (standard x, y, z planes) moves
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, time
import lookahead, stepper, homing

# Common suffixes: _d is distance (in mm), _v is velocity (in
#   mm/second), _t is time (in seconds), _r is ratio (scalar between
#   0.0 and 1.0)

StepList = (0, 1, 2, 3)

class Move:
    def __init__(self, kin, pos, move_d, axes_d, speed, accel):
        self.kin = kin
        self.pos = tuple(pos)
        self.axes_d = axes_d
        self.move_d = move_d
        self.junction_max = speed**2
        self.junction_delta = 2.0 * move_d * accel
        self.junction_start_max = 0.
    def calc_junction(self, prev_move):
        # Find max start junction velocity using approximated
        # centripetal velocity as described at:
        # https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
        if not prev_move.move_d:
            return
        junction_cos_theta = -((self.axes_d[0] * prev_move.axes_d[0]
                                + self.axes_d[1] * prev_move.axes_d[1])
                               / (self.move_d * prev_move.move_d))
        if junction_cos_theta > 0.999999:
            return
        junction_cos_theta = max(junction_cos_theta, -0.999999)
        sin_theta_d2 = math.sqrt(0.5*(1.0-junction_cos_theta));
        R = self.kin.junction_deviation * sin_theta_d2 / (1.0 - sin_theta_d2)
        self.junction_start_max = min(
            R * self.kin.max_xy_accel, self.junction_max, prev_move.junction_max)
    def process(self, junction_start, junction_end):
        # Determine accel, cruise, and decel portions of the move
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
        # Determine the move velocities and time spent in each portion
        start_v = math.sqrt(junction_start)
        cruise_v = math.sqrt(junction_cruise)
        end_v = math.sqrt(junction_end)
        inv_cruise_v = 1. / cruise_v
        inv_accel = 2.0 * self.move_d * inv_junction_delta
        accel_t = 2.0 * self.move_d * accel_r / (start_v+cruise_v)
        cruise_t = self.move_d * cruise_r * inv_cruise_v
        decel_t = 2.0 * self.move_d * decel_r / (end_v+cruise_v)

        #logging.debug("Move: %s v=%.2f/%.2f/%.2f mt=%.3f st=%.3f %.3f %.3f" % (
        #    self.relsteps, start_v, cruise_v, end_v, move_t
        #    , next_move_time, accel_r, cruise_r))

        # Calculate step times for the move
        next_move_time = self.kin.get_next_move_time()
        for i in StepList:
            new_step_pos = int(self.pos[i]*self.kin.steppers[i].inv_step_dist
                               + 0.5)
            steps = new_step_pos - self.kin.stepper_pos[i]
            if not steps:
                continue
            self.kin.stepper_pos[i] = new_step_pos
            sdir = 0
            if steps < 0:
                sdir = 1
                steps = -steps
            clock_offset, clock_freq, so = self.kin.steppers[i].prep_move(
                sdir, next_move_time)

            step_dist = self.move_d / steps
            step_offset = 0.5

            # Acceleration steps
            #t = sqrt(2*pos/accel + (start_v/accel)**2) - start_v/accel
            accel_clock_offset = start_v * inv_accel * clock_freq
            accel_sqrt_offset = accel_clock_offset**2
            accel_multiplier = 2.0 * step_dist * inv_accel * clock_freq**2
            accel_steps = accel_r * steps
            step_offset = so.step_sqrt(
                accel_steps, step_offset, clock_offset - accel_clock_offset
                , accel_sqrt_offset, accel_multiplier)
            clock_offset += accel_t * clock_freq
            # Cruising steps
            #t = pos/cruise_v
            cruise_multiplier = step_dist * inv_cruise_v * clock_freq
            cruise_steps = cruise_r * steps
            step_offset = so.step_factor(
                cruise_steps, step_offset, clock_offset, cruise_multiplier)
            clock_offset += cruise_t * clock_freq
            # Deceleration steps
            #t = cruise_v/accel - sqrt((cruise_v/accel)**2 - 2*pos/accel)
            decel_clock_offset = cruise_v * inv_accel * clock_freq
            decel_sqrt_offset = decel_clock_offset**2
            decel_steps = decel_r * steps
            so.step_sqrt(
                decel_steps, step_offset, clock_offset + decel_clock_offset
                , decel_sqrt_offset, -accel_multiplier)
        self.kin.update_move_time(accel_t + cruise_t + decel_t)

STALL_TIME = 0.100

class CartKinematics:
    def __init__(self, printer, config):
        self.printer = printer
        self.reactor = printer.reactor
        steppers = ['stepper_x', 'stepper_y', 'stepper_z', 'stepper_e']
        self.steppers = [stepper.PrinterStepper(printer, config.getsection(n))
                         for n in steppers]
        self.max_xy_speed = min(s.max_step_velocity*s.step_dist
                                for s in self.steppers[:2])
        self.max_xy_accel = min(s.max_step_accel*s.step_dist
                                for s in self.steppers[:2])
        self.junction_deviation = config.getfloat('junction_deviation', 0.02)
        dummy_move = Move(self, [0.]*4, 0., [0.]*4, 0., 0.)
        self.move_queue = lookahead.MoveQueue(dummy_move)
        self.commanded_pos = [0., 0., 0., 0.]
        self.stepper_pos = [0, 0, 0, 0]
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
        for stepper in self.steppers:
            stepper.build_config()
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
        self.stepper_pos = [int(newpos[i]*self.steppers[i].inv_step_dist + 0.5)
                            for i in StepList]
    def _move_with_z(self, newpos, axes_d, speed):
        self.move_queue.flush()
        # Limit velocity to max for each stepper
        move_d = math.sqrt(sum([d*d for d in axes_d[:3]]))
        velocity_factor = min(
            [self.steppers[i].max_step_velocity
             * self.steppers[i].step_dist / abs(axes_d[i])
             for i in StepList if axes_d[i]])
        speed = min(speed, self.max_xy_speed, velocity_factor * move_d)
        # Find max acceleration factor
        accel_factor = min(
            [self.steppers[i].max_step_accel
             * self.steppers[i].step_dist / abs(axes_d[i])
             for i in StepList if axes_d[i]])
        accel = min(self.max_xy_accel, accel_factor * move_d)
        move = Move(self, newpos, move_d, axes_d, speed, accel)
        move.process(0., 0.)
    def _move_only_e(self, newpos, axes_d, speed):
        self.move_queue.flush()
        s = self.steppers[3]
        speed = min(speed, self.max_xy_speed, s.max_step_velocity * s.step_dist)
        accel = min(self.max_xy_accel, s.max_step_accel * s.step_dist)
        move = Move(self, newpos, abs(axes_d[3]), axes_d, speed, accel)
        move.process(0., 0.)
    def move(self, newpos, speed, sloppy=False):
        axes_d = [newpos[i] - self.commanded_pos[i] for i in StepList]
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
        move.calc_junction(self.move_queue.prev_move())
        self.move_queue.add_move(move)
    def home(self, axis):
        # Each axis is homed independently and in order
        homing_state = homing.Homing(self, self.steppers)
        for a in axis:
            homing_state.plan_home(a)
        return homing_state
    def dwell(self, delay):
        self.get_last_move_time()
        self.update_move_time(delay)
    def motor_off(self):
        self.dwell(STALL_TIME)
        last_move_time = self.get_last_move_time()
        for stepper in self.steppers:
            stepper.motor_enable(last_move_time, 0)
        self.dwell(STALL_TIME)
        logging.debug('; Max time of %f' % (last_move_time,))
