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
    def __init__(self, kin, relsteps, speed):
        self.kin = kin
        self.relsteps = relsteps
        self.junction_max = self.junction_start_max = self.junction_delta = 0.
        # Calculate requested distance to travel (in mm)
        steppers = self.kin.steppers
        absrelsteps = [abs(relsteps[i]) for i in StepList]
        stepper_d = [absrelsteps[i] * steppers[i].step_dist
                     for i in StepList]
        self.move_d = math.sqrt(sum([d*d for d in stepper_d[:3]]))
        if not self.move_d:
            self.move_d = stepper_d[3]
            if not self.move_d:
                return
        # Limit velocity to max for each stepper
        velocity_factor = min([steppers[i].max_step_velocity / absrelsteps[i]
                               for i in StepList if absrelsteps[i]])
        move_v = min(speed, velocity_factor * self.move_d)
        self.junction_max = move_v**2
        # Find max acceleration factor
        accel_factor = min([steppers[i].max_step_accel / absrelsteps[i]
                            for i in StepList if absrelsteps[i]])
        accel = min(self.kin.max_accel, accel_factor * self.move_d)
        self.junction_delta = 2.0 * self.move_d * accel
    def calc_junction(self, prev_move):
        # Find max start junction velocity using approximated
        # centripetal velocity as described at:
        # https://onehossshay.wordpress.com/2011/09/24/improving_grbl_cornering_algorithm/
        if not prev_move.move_d or self.relsteps[2] or prev_move.relsteps[2]:
            return
        steppers = self.kin.steppers
        junction_cos_theta = -sum([
            self.relsteps[i] * prev_move.relsteps[i] * steppers[i].step_dist**2
            for i in range(2)]) / (self.move_d * prev_move.move_d)
        if junction_cos_theta > 0.999999:
            return
        junction_cos_theta = max(junction_cos_theta, -0.999999)
        sin_theta_d2 = math.sqrt(0.5*(1.0-junction_cos_theta));
        R = self.kin.junction_deviation * sin_theta_d2 / (1.0 - sin_theta_d2)
        accel = self.junction_delta / (2.0 * self.move_d)
        self.junction_start_max = min(
            accel * R, self.junction_max, prev_move.junction_max)
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
            steps = self.relsteps[i]
            if not steps:
                continue
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
        self.max_accel = min(s.max_step_accel*s.step_dist
                             for s in self.steppers[:2]) # XXX
        dummy_move = Move(self, [0]*len(self.steppers), 0.)
        dummy_move.junction_max = 0.
        self.junction_deviation = config.getfloat('junction_deviation', 0.02)
        self.move_queue = lookahead.MoveQueue(dummy_move)
        self.pos = [0, 0, 0, 0]
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
        return [self.pos[i] * self.steppers[i].step_dist
                for i in StepList]
    def set_position(self, newpos):
        self.pos = [int(newpos[i]*self.steppers[i].inv_step_dist + 0.5)
                    for i in StepList]
    def move(self, newpos, speed, sloppy=False):
        # Round to closest step position
        newpos = [int(newpos[i]*self.steppers[i].inv_step_dist + 0.5)
                  for i in StepList]
        relsteps = [newpos[i] - self.pos[i] for i in StepList]
        self.pos = newpos
        if relsteps == [0]*len(newpos):
            # no move
            return
        #logging.debug("; dist %s @ %d\n" % (
        #    [newpos[i]*self.steppers[i].step_dist for i in StepList], speed))
        # Create move and queue it
        move = Move(self, relsteps, speed)
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
