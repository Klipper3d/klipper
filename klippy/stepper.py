# Printer stepper support
#
# Copyright (C) 2016-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import math, logging, collections
import homing


######################################################################
# Stepper enable pins
######################################################################

# Tracking of shared stepper enable pins
class StepperEnablePin:
    def __init__(self, mcu_enable, enable_count=0):
        self.mcu_enable = mcu_enable
        self.enable_count = enable_count
    def set_enable(self, print_time, enable):
        if enable:
            if not self.enable_count:
                self.mcu_enable.set_digital(print_time, 1)
            self.enable_count += 1
        else:
            self.enable_count -= 1
            if not self.enable_count:
                self.mcu_enable.set_digital(print_time, 0)

class StepperMultiEnablePin:
    def __init__(self, enable_list):
        self.enable_list = enable_list
    def set_enable(self, print_time, enable):
        for en in self.enable_list:
            en.set_enable(print_time, enable)

def lookup_enable_pin(ppins, pin_list):
    if pin_list is None:
        return StepperEnablePin(None, 9999)
    enable_list = []
    for pin in pin_list.split(','):
        pin_params = ppins.lookup_pin(pin, can_invert=True,
                                      share_type='stepper_enable')
        enable = pin_params.get('class')
        if enable is None:
            mcu_enable = pin_params['chip'].setup_pin('digital_out', pin_params)
            mcu_enable.setup_max_duration(0.)
            pin_params['class'] = enable = StepperEnablePin(mcu_enable)
        enable_list.append(enable)
    if len(enable_list) == 1:
        return enable_list[0]
    return StepperMultiEnablePin(enable_list)


######################################################################
# Steppers
######################################################################

# Code storing the definitions for a stepper motor
class PrinterStepper:
    def __init__(self, config):
        printer = config.get_printer()
        self.name = config.get_name()
        self.need_motor_enable = True
        # Stepper definition
        ppins = printer.lookup_object('pins')
        step_pin = config.get('step_pin')
        self.mcu_stepper = mcu_stepper = ppins.setup_pin('stepper', step_pin)
        dir_pin = config.get('dir_pin')
        dir_pin_params = ppins.lookup_pin(dir_pin, can_invert=True)
        mcu_stepper.setup_dir_pin(dir_pin_params)
        step_dist = config.getfloat('step_distance', above=0.)
        mcu_stepper.setup_step_distance(step_dist)
        mcu_stepper.add_active_callback(self._stepper_active)
        self.enable = lookup_enable_pin(ppins, config.get('enable_pin', None))
        # Register STEPPER_BUZZ command
        force_move = printer.try_load_module(config, 'force_move')
        force_move.register_stepper(self)
        # Wrappers
        self.setup_itersolve = mcu_stepper.setup_itersolve
        self.generate_steps = mcu_stepper.generate_steps
        self.set_trapq = mcu_stepper.set_trapq
        self.set_stepper_kinematics = mcu_stepper.set_stepper_kinematics
        self.calc_position_from_coord = mcu_stepper.calc_position_from_coord
        self.set_position = mcu_stepper.set_position
        self.get_commanded_position = mcu_stepper.get_commanded_position
        self.set_commanded_position = mcu_stepper.set_commanded_position
        self.get_mcu_position = mcu_stepper.get_mcu_position
        self.get_step_dist = mcu_stepper.get_step_dist
        self.is_dir_inverted = mcu_stepper.is_dir_inverted
    def get_name(self, short=False):
        if short and self.name.startswith('stepper_'):
            return self.name[8:]
        return self.name
    def add_to_endstop(self, mcu_endstop):
        mcu_endstop.add_stepper(self.mcu_stepper)
    def _dist_to_time(self, dist, start_velocity, accel):
        # Calculate the time it takes to travel a distance with constant accel
        time_offset = start_velocity / accel
        return math.sqrt(2. * dist / accel + time_offset**2) - time_offset
    def set_max_jerk(self, max_halt_velocity, max_accel):
        # Calculate the firmware's maximum halt interval time
        step_dist = self.get_step_dist()
        last_step_time = self._dist_to_time(
            step_dist, max_halt_velocity, max_accel)
        second_last_step_time = self._dist_to_time(
            2. * step_dist, max_halt_velocity, max_accel)
        min_stop_interval = second_last_step_time - last_step_time
        self.mcu_stepper.setup_min_stop_interval(min_stop_interval)
    def _stepper_active(self, active_time):
        self.motor_enable(active_time, 1)
    def motor_enable(self, print_time, enable=0):
        if self.need_motor_enable != (not enable):
            self.enable.set_enable(print_time, enable)
        self.need_motor_enable = not enable
        if not enable:
            # Enable stepper on future stepper movement
            self.mcu_stepper.add_active_callback(self._stepper_active)
    def is_motor_enabled(self):
        return not self.need_motor_enable


######################################################################
# Stepper controlled rails
######################################################################

# A motor control "rail" with one (or more) steppers and one (or more)
# endstops.
class PrinterRail:
    def __init__(self, config, need_position_minmax=True,
                 default_position_endstop=None):
        # Primary stepper
        stepper = PrinterStepper(config)
        self.steppers = [stepper]
        self.name = stepper.get_name(short=True)
        self.get_commanded_position = stepper.get_commanded_position
        self.is_motor_enabled = stepper.is_motor_enabled
        # Primary endstop and its position
        printer = config.get_printer()
        ppins = printer.lookup_object('pins')
        mcu_endstop = ppins.setup_pin('endstop', config.get('endstop_pin'))
        self.endstops = [(mcu_endstop, self.name)]
        stepper.add_to_endstop(mcu_endstop)
        if hasattr(mcu_endstop, "get_position_endstop"):
            self.position_endstop = mcu_endstop.get_position_endstop()
        elif default_position_endstop is None:
            self.position_endstop = config.getfloat('position_endstop')
        else:
            self.position_endstop = config.getfloat(
                'position_endstop', default_position_endstop)
        query_endstops = printer.try_load_module(config, 'query_endstops')
        query_endstops.register_endstop(mcu_endstop, self.name)
        # Axis range
        if need_position_minmax:
            self.position_min = config.getfloat('position_min', 0.)
            self.position_max = config.getfloat(
                'position_max', above=self.position_min)
        else:
            self.position_min = 0.
            self.position_max = self.position_endstop
        if (self.position_endstop < self.position_min
            or self.position_endstop > self.position_max):
            raise config.error(
                "position_endstop in section '%s' must be between"
                " position_min and position_max" % config.get_name())
        # Homing mechanics
        self.homing_speed = config.getfloat('homing_speed', 5.0, above=0.)
        self.second_homing_speed = config.getfloat(
            'second_homing_speed', self.homing_speed/2., above=0.)
        self.homing_retract_dist = config.getfloat(
            'homing_retract_dist', 5., minval=0.)
        self.homing_positive_dir = config.getboolean(
            'homing_positive_dir', None)
        if self.homing_positive_dir is None:
            axis_len = self.position_max - self.position_min
            if self.position_endstop <= self.position_min + axis_len / 4.:
                self.homing_positive_dir = False
            elif self.position_endstop >= self.position_max - axis_len / 4.:
                self.homing_positive_dir = True
            else:
                raise config.error(
                    "Unable to infer homing_positive_dir in section '%s'" % (
                        config.get_name(),))
    def get_range(self):
        return self.position_min, self.position_max
    def get_homing_info(self):
        homing_info = collections.namedtuple('homing_info', [
            'speed', 'position_endstop', 'retract_dist', 'positive_dir',
            'second_homing_speed'])(
                self.homing_speed, self.position_endstop,
                self.homing_retract_dist, self.homing_positive_dir,
                self.second_homing_speed)
        return homing_info
    def get_steppers(self):
        return list(self.steppers)
    def get_endstops(self):
        return list(self.endstops)
    def add_extra_stepper(self, config):
        stepper = PrinterStepper(config)
        self.steppers.append(stepper)
        mcu_endstop = self.endstops[0][0]
        endstop_pin = config.get('endstop_pin', None)
        if endstop_pin is not None:
            printer = config.get_printer()
            ppins = printer.lookup_object('pins')
            mcu_endstop = ppins.setup_pin('endstop', endstop_pin)
            name = stepper.get_name(short=True)
            self.endstops.append((mcu_endstop, name))
            query_endstops = printer.try_load_module(config, 'query_endstops')
            query_endstops.register_endstop(mcu_endstop, name)
        stepper.add_to_endstop(mcu_endstop)
    def add_to_endstop(self, mcu_endstop):
        for stepper in self.steppers:
            stepper.add_to_endstop(mcu_endstop)
    def setup_itersolve(self, alloc_func, *params):
        for stepper in self.steppers:
            stepper.setup_itersolve(alloc_func, *params)
    def generate_steps(self, flush_time):
        for stepper in self.steppers:
            stepper.generate_steps(flush_time)
    def set_trapq(self, trapq):
        for stepper in self.steppers:
            stepper.set_trapq(trapq)
    def set_max_jerk(self, max_halt_velocity, max_accel):
        for stepper in self.steppers:
            stepper.set_max_jerk(max_halt_velocity, max_accel)
    def set_commanded_position(self, pos):
        for stepper in self.steppers:
            stepper.set_commanded_position(pos)
    def set_position(self, coord):
        for stepper in self.steppers:
            stepper.set_position(coord)
    def motor_enable(self, print_time, enable=0):
        for stepper in self.steppers:
            stepper.motor_enable(print_time, enable)

# Wrapper for dual stepper motor support
def LookupMultiRail(config):
    rail = PrinterRail(config)
    for i in range(1, 99):
        if not config.has_section(config.get_name() + str(i)):
            break
        rail.add_extra_stepper(config.getsection(config.get_name() + str(i)))
    return rail
