# Printer stepper support
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import homing

class PrinterStepper:
    def __init__(self, printer, config, name):
        self.name = name

        self.step_dist = config.getfloat('step_distance')
        self.inv_step_dist = 1. / self.step_dist
        self.min_stop_interval = 0.

        self.homing_speed = config.getfloat('homing_speed', 5.0)
        self.homing_positive_dir = config.getboolean(
            'homing_positive_dir', False)
        self.homing_retract_dist = config.getfloat('homing_retract_dist', 5.)
        self.homing_stepper_phases = config.getint('homing_stepper_phases', None)
        self.homing_endstop_phase = config.getint('homing_endstop_phase', None)
        endstop_accuracy = config.getfloat('homing_endstop_accuracy', None)
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
                             " phase adjustment" % (name,))
                self.homing_stepper_phases = None
            if printer.mcu.is_fileoutput():
                self.homing_endstop_accuracy = self.homing_stepper_phases
        self.position_min = self.position_endstop = self.position_max = None
        endstop_pin = config.get('endstop_pin', None)
        step_pin = config.get('step_pin')
        dir_pin = config.get('dir_pin')
        mcu = printer.mcu
        self.mcu_stepper = mcu.create_stepper(step_pin, dir_pin)
        self.mcu_stepper.set_step_distance(self.step_dist)
        enable_pin = config.get('enable_pin', None)
        if enable_pin is not None:
            self.mcu_enable = mcu.create_digital_out(enable_pin, 0)
        if endstop_pin is not None:
            self.mcu_endstop = mcu.create_endstop(endstop_pin)
            self.mcu_endstop.add_stepper(self.mcu_stepper)
            self.position_min = config.getfloat('position_min', 0.)
            self.position_endstop = config.getfloat('position_endstop')
            self.position_max = config.getfloat('position_max', 0.)
        self.need_motor_enable = True
    def _dist_to_time(self, dist, start_velocity, accel):
        # Calculate the time it takes to travel a distance with constant accel
        time_offset = start_velocity / accel
        return math.sqrt(2. * dist / accel + time_offset**2) - time_offset
    def set_max_jerk(self, max_halt_velocity, max_accel):
        # Calculate the firmware's maximum halt interval time
        last_step_time = self._dist_to_time(
            self.step_dist, max_halt_velocity, max_accel)
        second_last_step_time = self._dist_to_time(
            2. * self.step_dist, max_halt_velocity, max_accel)
        min_stop_interval = second_last_step_time - last_step_time
        self.mcu_stepper.set_min_stop_interval(min_stop_interval)
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
        self.mcu_endstop.home_start(mcu_time, step_time)
        return self.mcu_endstop
    def query_endstop(self, print_time):
        mcu_time = self.mcu_endstop.print_to_mcu_time(print_time)
        self.mcu_endstop.query_endstop(mcu_time)
        return self.mcu_endstop
    def get_homed_offset(self):
        if not self.homing_stepper_phases or self.need_motor_enable:
            return 0
        pos = self.mcu_stepper.get_mcu_position()
        pos %= self.homing_stepper_phases
        if self.homing_endstop_phase is None:
            logging.info("Setting %s endstop phase to %d" % (self.name, pos))
            self.homing_endstop_phase = pos
            return 0
        delta = (pos - self.homing_endstop_phase) % self.homing_stepper_phases
        if delta >= self.homing_stepper_phases - self.homing_endstop_accuracy:
            delta -= self.homing_stepper_phases
        elif delta > self.homing_endstop_accuracy:
            raise homing.EndstopError(
                "Endstop %s incorrect phase (got %d vs %d)" % (
                    self.name, pos, self.homing_endstop_phase))
        return delta * self.step_dist
