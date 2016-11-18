# Printer stepper support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import homing

class PrinterStepper:
    def __init__(self, printer, config, name):
        self.printer = printer
        self.config = config
        self.name = name
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

        self.need_motor_enable = True
    def set_max_jerk(self, max_jerk):
        self.max_jerk = max_jerk
    def build_config(self):
        max_error = self.config.getfloat('max_error', 0.000050)
        step_pin = self.config.get('step_pin')
        dir_pin = self.config.get('dir_pin')
        jc = self.max_jerk / self.max_accel
        inv_max_step_accel = self.step_dist / self.max_accel
        min_stop_interval = (math.sqrt(3.*inv_max_step_accel + jc**2)
                             - math.sqrt(inv_max_step_accel + jc**2)) - max_error
        min_stop_interval = max(0., min_stop_interval)
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
        if enable and self.need_motor_enable:
            mcu_time = self.mcu_stepper.print_to_mcu_time(move_time)
            self.mcu_stepper.reset_step_clock(mcu_time)
        if (self.mcu_enable is not None
            and self.mcu_enable.get_last_setting() != enable):
            mcu_time = self.mcu_enable.print_to_mcu_time(move_time)
            self.mcu_enable.set_digital(mcu_time, enable)
        self.need_motor_enable = not enable
    def enable_endstop_checking(self, move_time, step_time):
        mcu_time = self.mcu_endstop.print_to_mcu_time(move_time)
        self.mcu_endstop.home(mcu_time, step_time)
        return self.mcu_endstop
    def query_endstop(self, print_time):
        if self.mcu_endstop is None:
            return None
        mcu_time = self.mcu_endstop.print_to_mcu_time(print_time)
        self.mcu_endstop.query_endstop(mcu_time)
        return self.mcu_endstop
    def get_homed_offset(self):
        if not self.homing_stepper_phases or self.need_motor_enable:
            return 0
        pos = self.mcu_endstop.get_last_position()
        pos %= self.homing_stepper_phases
        if self.homing_endstop_phase is None:
            logging.info("Setting %s endstop phase to %d" % (
                self.config.section, pos))
            self.homing_endstop_phase = pos
            return 0
        delta = (pos - self.homing_endstop_phase) % self.homing_stepper_phases
        if delta >= self.homing_stepper_phases - self.homing_endstop_accuracy:
            delta -= self.homing_stepper_phases
        elif delta > self.homing_endstop_accuracy:
            raise homing.EndstopError(
                "Endstop %s incorrect phase (got %d vs %d)" % (
                    self.name, pos, self.homing_endstop_phase))
        return delta
