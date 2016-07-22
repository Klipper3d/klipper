# Code for handling printer nozzle extruders
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import stepper, heater

class PrinterExtruder:
    def __init__(self, printer, config):
        cfg = config.getsection('extruder')
        self.heater = heater.PrinterHeater(printer, cfg)
        self.stepper = stepper.PrinterStepper(printer, cfg)
        self.stepper_pos = 0
    def build_config(self):
        self.heater.build_config()
        self.stepper.set_max_jerk(9999999.9)
        self.stepper.build_config()
    def get_max_speed(self):
        return self.stepper.max_velocity, self.stepper.max_accel
    def motor_off(self, move_time):
        self.stepper.motor_enable(move_time, 0)
    def move(self, move_time, move):
        inv_accel = 1. / move.accel
        new_step_pos = int(move.pos[3]*self.stepper.inv_step_dist + 0.5)
        steps = new_step_pos - self.stepper_pos
        if not steps:
            return
        self.stepper_pos = new_step_pos
        sdir = 0
        if steps < 0:
            sdir = 1
            steps = -steps
        clock_offset, clock_freq, so = self.stepper.prep_move(sdir, move_time)

        step_dist = move.move_d / steps
        step_offset = 0.5

        # Acceleration steps
        #t = sqrt(2*pos/accel + (start_v/accel)**2) - start_v/accel
        accel_clock_offset = move.start_v * inv_accel * clock_freq
        accel_sqrt_offset = accel_clock_offset**2
        accel_multiplier = 2.0 * step_dist * inv_accel * clock_freq**2
        accel_steps = move.accel_r * steps
        step_offset = so.step_sqrt(
            accel_steps, step_offset, clock_offset - accel_clock_offset
            , accel_sqrt_offset, accel_multiplier)
        clock_offset += move.accel_t * clock_freq
        # Cruising steps
        #t = pos/cruise_v
        cruise_multiplier = step_dist * clock_freq / move.cruise_v
        cruise_steps = move.cruise_r * steps
        step_offset = so.step_factor(
            cruise_steps, step_offset, clock_offset, cruise_multiplier)
        clock_offset += move.cruise_t * clock_freq
        # Deceleration steps
        #t = cruise_v/accel - sqrt((cruise_v/accel)**2 - 2*pos/accel)
        decel_clock_offset = move.cruise_v * inv_accel * clock_freq
        decel_sqrt_offset = decel_clock_offset**2
        decel_steps = move.decel_r * steps
        so.step_sqrt(
            decel_steps, step_offset, clock_offset + decel_clock_offset
            , decel_sqrt_offset, -accel_multiplier)
