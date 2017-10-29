# Printer stepper support
#
# Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging
import homing, pins

class PrinterStepper:
    def __init__(self, printer, config):
        self.name = config.section
        if self.name.startswith('stepper_'):
            self.name = self.name[8:]

        self.step_dist = config.getfloat('step_distance', above=0.)
        self.inv_step_dist = 1. / self.step_dist
        self.min_stop_interval = 0.
        self.mcu_stepper = pins.setup_pin(
            printer, 'stepper', config.get('step_pin'))
        dir_pin_params = pins.get_printer_pins(printer).parse_pin_desc(
            config.get('dir_pin'), can_invert=True)
        self.mcu_stepper.setup_dir_pin(dir_pin_params)
        self.mcu_stepper.setup_step_distance(self.step_dist)

        enable_pin = config.get('enable_pin', None)
        self.mcu_enable = None
        if enable_pin is not None:
            self.mcu_enable = pins.setup_pin(printer, 'digital_out', enable_pin)
            self.mcu_enable.setup_max_duration(0.)
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
        self.mcu_stepper.setup_min_stop_interval(min_stop_interval)
    def motor_enable(self, print_time, enable=0):
        if enable and self.need_motor_enable:
            self.mcu_stepper.reset_step_clock(print_time)
        if (self.mcu_enable is not None
            and self.mcu_enable.get_last_setting() != enable):
            self.mcu_enable.set_digital(print_time, enable)
        self.need_motor_enable = not enable

class PrinterHomingStepper(PrinterStepper):
    def __init__(self, printer, config):
        PrinterStepper.__init__(self, printer, config)

        self.mcu_endstop = pins.setup_pin(
            printer, 'endstop', config.get('endstop_pin'))
        self.mcu_endstop.add_stepper(self.mcu_stepper)
        self.position_min = config.getfloat('position_min', 0.)
        self.position_max = config.getfloat(
            'position_max', 0., above=self.position_min)
        self.position_endstop = config.getfloat('position_endstop')

        self.homing_speed = config.getfloat('homing_speed', 5.0, above=0.)
        self.homing_positive_dir = config.getboolean('homing_positive_dir', None)
        if self.homing_positive_dir is None:
            axis_len = self.position_max - self.position_min
            if self.position_endstop <= self.position_min + axis_len / 4.:
                self.homing_positive_dir = False
            elif self.position_endstop >= self.position_max - axis_len / 4.:
                self.homing_positive_dir = True
            else:
                raise config.error(
                    "Unable to infer homing_positive_dir in section '%s'" % (
                        config.section,))
        self.homing_retract_dist = config.getfloat(
            'homing_retract_dist', 5., above=0.)
        self.homing_stepper_phases = config.getint(
            'homing_stepper_phases', None, minval=0)
        endstop_accuracy = config.getfloat(
            'homing_endstop_accuracy', None, above=0.)
        self.homing_endstop_accuracy = self.homing_endstop_phase = None
        if self.homing_stepper_phases:
            self.homing_endstop_phase = config.getint(
                'homing_endstop_phase', None, minval=0
                , maxval=self.homing_stepper_phases-1)
            if self.homing_endstop_phase is not None:
                # Adjust the endstop position so 0.0 is always at a full step
                micro_steps = self.homing_stepper_phases // 4
                phase_offset = (
                    ((self.homing_endstop_phase + micro_steps // 2) % micro_steps)
                    - micro_steps // 2) * self.step_dist
                full_step = micro_steps * self.step_dist
                es_pos = (int(self.position_endstop / full_step + .5) * full_step
                          + phase_offset)
                if es_pos != self.position_endstop:
                    logging.info("Changing %s endstop position to %.3f"
                                 " (from %.3f)", self.name, es_pos,
                                 self.position_endstop)
                    self.position_endstop = es_pos
            if endstop_accuracy is None:
                self.homing_endstop_accuracy = self.homing_stepper_phases//2 - 1
            elif self.homing_endstop_phase is not None:
                self.homing_endstop_accuracy = int(math.ceil(
                    endstop_accuracy * self.inv_step_dist / 2.))
            else:
                self.homing_endstop_accuracy = int(math.ceil(
                    endstop_accuracy * self.inv_step_dist))
            if self.homing_endstop_accuracy >= self.homing_stepper_phases // 2:
                logging.info("Endstop for %s is not accurate enough for stepper"
                             " phase adjustment", name)
                self.homing_stepper_phases = None
            if self.mcu_endstop.get_mcu().is_fileoutput():
                self.homing_endstop_accuracy = self.homing_stepper_phases
    def get_homing_speed(self):
        # Round the configured homing speed so that it is an even
        # number of ticks per step.
        adjusted_freq = self.mcu_stepper.get_mcu().get_adjusted_freq()
        dist_ticks = adjusted_freq * self.step_dist
        ticks_per_step = round(dist_ticks / self.homing_speed)
        return dist_ticks / ticks_per_step
    def get_homed_offset(self):
        if not self.homing_stepper_phases or self.need_motor_enable:
            return 0
        pos = self.mcu_stepper.get_mcu_position()
        pos %= self.homing_stepper_phases
        if self.homing_endstop_phase is None:
            logging.info("Setting %s endstop phase to %d", self.name, pos)
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
