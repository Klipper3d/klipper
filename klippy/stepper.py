# Printer stepper support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging

class PrinterStepper:
    def __init__(self, printer, config):
        self.printer = printer
        self.config = config
        self.mcu_stepper = self.mcu_enable = self.mcu_endstop = None

        self.step_dist = config.getfloat('step_distance')
        self.inv_step_dist = 1. / self.step_dist
        self.max_velocity = config.getfloat('max_velocity')
        self.max_accel = config.getfloat('max_accel')
        self.max_jerk = 0.

        self.homing_speed = config.getfloat('homing_speed', 5.0)
        self.homing_positive_dir = config.getboolean(
            'homing_positive_dir', False)
        self.homing_retract_dist = config.getfloat('homing_retract_dist', 5.)
        self.homing_stepper_phases = config.getint('homing_stepper_phases')
        self.homing_endstop_phase = config.getint('homing_endstop_phase')
        endstop_accuracy = config.getfloat('homing_endstop_accuracy')
        self.homing_endstop_accuracy = None
        if self.homing_stepper_phases:
            if endstop_accuracy is None:
                self.homing_endstop_accuracy = self.homing_stepper_phases//2 - 1
            elif self.homing_endstop_phase is not None:
                self.homing_endstop_accuracy = int(math.ceil(
                    endstop_accuracy * self.inv_step_dist / 2.))
            else:
                self.homing_endstop_accuracy = int(math.ceil(
                    endstop_accuracy * self.inv_step_dist))
            if self.homing_endstop_accuracy >= self.homing_stepper_phases/2:
                logging.info("Endstop for %s is not accurate enough for stepper"
                             " phase adjustment" % (self.config.section,))
                self.homing_stepper_phases = None
        self.position_min = config.getfloat('position_min', 0.)
        self.position_endstop = config.getfloat('position_endstop')
        self.position_max = config.getfloat('position_max')

        self.clock_ticks = None
        self.need_motor_enable = True
    def set_max_jerk(self, max_jerk):
        self.max_jerk = max_jerk
    def build_config(self):
        self.clock_ticks = self.printer.mcu.get_mcu_freq()
        max_error = self.config.getfloat('max_error', 0.000050)
        max_error = int(max_error * self.clock_ticks)

        step_pin = self.config.get('step_pin')
        dir_pin = self.config.get('dir_pin')
        jc = self.max_jerk / self.max_accel
        inv_max_step_accel = self.step_dist / self.max_accel
        min_stop_interval = int((math.sqrt(3.*inv_max_step_accel + jc**2)
                                 - math.sqrt(inv_max_step_accel + jc**2))
                                * self.clock_ticks) - max_error
        min_stop_interval = max(0, min_stop_interval)
        mcu = self.printer.mcu
        self.mcu_stepper = mcu.create_stepper(
            step_pin, dir_pin, min_stop_interval, max_error)
        enable_pin = self.config.get('enable_pin')
        if enable_pin is not None:
            self.mcu_enable = mcu.create_digital_out(enable_pin, 0)
        endstop_pin = self.config.get('endstop_pin')
        if endstop_pin is not None:
            self.mcu_endstop = mcu.create_endstop(endstop_pin, self.mcu_stepper)
    def motor_enable(self, move_time, enable=0):
        if (self.mcu_enable is not None
            and self.mcu_enable.get_last_setting() != enable):
            mc = int(self.mcu_enable.get_print_clock(move_time))
            self.mcu_enable.set_digital(mc + 1, enable)
        self.need_motor_enable = True
    def prep_move(self, sdir, move_time):
        move_clock = self.mcu_stepper.get_print_clock(move_time)
        self.mcu_stepper.set_next_step_dir(sdir, int(move_clock))
        if self.need_motor_enable:
            self.motor_enable(move_time, 1)
            self.need_motor_enable = False
        return (move_clock, self.clock_ticks, self.mcu_stepper)
    def enable_endstop_checking(self, move_time, hz):
        move_clock = int(self.mcu_endstop.get_print_clock(move_time))
        self.mcu_endstop.home(move_clock, int(self.clock_ticks / hz))
        return self.mcu_endstop
    def get_homed_position(self):
        if not self.homing_stepper_phases:
            return self.position_endstop
        pos = self.mcu_endstop.get_last_position()
        pos %= self.homing_stepper_phases
        if self.homing_endstop_phase is None:
            logging.info("Setting %s endstop phase to %d" % (
                self.config.section, pos))
            self.homing_endstop_phase = pos
            return self.position_endstop
        delta = (pos - self.homing_endstop_phase) % self.homing_stepper_phases
        if delta >= self.homing_stepper_phases - self.homing_endstop_accuracy:
            delta -= self.homing_stepper_phases
        elif delta > self.homing_endstop_accuracy:
            logging.error("Endstop %s incorrect phase (got %d vs %d)" % (
                self.config.section, pos, self.homing_endstop_phase))
            return self.position_endstop
        return self.position_endstop + delta * self.step_dist
