# Code for handling the kinematics of corexy robots
#
# Copyright (C) 2017-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, math
import stepper, homing, chelper

StepList = (0, 1, 2)

class CoreXYKinematics:
    def __init__(self, toolhead, printer, config):
        self.steppers = [
            stepper.PrinterHomingStepper(
                printer, config.getsection('stepper_x')),
            stepper.PrinterHomingStepper(
                printer, config.getsection('stepper_y')),
            stepper.LookupMultiHomingStepper(
                printer, config.getsection('stepper_z'))]
        self.steppers[0].mcu_endstop.add_stepper(self.steppers[1].mcu_stepper)
        self.steppers[1].mcu_endstop.add_stepper(self.steppers[0].mcu_stepper)
        max_velocity, max_accel = toolhead.get_max_velocity()
        self.max_z_velocity = config.getfloat(
            'max_z_velocity', max_velocity, above=0., maxval=max_velocity)
        self.max_z_accel = config.getfloat(
            'max_z_accel', max_accel, above=0., maxval=max_accel)
        self.need_motor_enable = True
        self.limits = [(1.0, -1.0)] * 3
        # Setup iterative solver
        ffi_main, ffi_lib = chelper.get_ffi()
        self.cmove = ffi_main.gc(ffi_lib.move_alloc(), ffi_lib.free)
        self.move_fill = ffi_lib.move_fill
        self.steppers[0].setup_itersolve(ffi_main.gc(
            ffi_lib.corexy_stepper_alloc('+'), ffi_lib.free))
        self.steppers[1].setup_itersolve(ffi_main.gc(
            ffi_lib.corexy_stepper_alloc('-'), ffi_lib.free))
        self.steppers[2].setup_cartesian_itersolve('z')
        # Setup stepper max halt velocity
        max_halt_velocity = toolhead.get_max_axis_halt()
        max_xy_halt_velocity = max_halt_velocity * math.sqrt(2.)
        self.steppers[0].set_max_jerk(max_xy_halt_velocity, max_accel)
        self.steppers[1].set_max_jerk(max_xy_halt_velocity, max_accel)
        self.steppers[2].set_max_jerk(
            min(max_halt_velocity, self.max_z_velocity), self.max_z_accel)
    def get_steppers(self, flags=""):
        if flags == "Z":
            return [self.steppers[2]]
        return list(self.steppers)
    def get_position(self):
        pos = [s.mcu_stepper.get_commanded_position() for s in self.steppers]
        return [0.5 * (pos[0] + pos[1]), 0.5 * (pos[0] - pos[1]), pos[2]]
    def set_position(self, newpos, homing_axes):
        pos = (newpos[0] + newpos[1], newpos[0] - newpos[1], newpos[2])
        for i in StepList:
            s = self.steppers[i]
            s.set_position(pos[i])
            if i in homing_axes:
                self.limits[i] = (s.position_min, s.position_max)
    def home(self, homing_state):
        # Each axis is homed independently and in order
        for axis in homing_state.get_axes():
            s = self.steppers[axis]
            # Determine moves
            if s.homing_positive_dir:
                pos = s.position_endstop - 1.5*(
                    s.position_endstop - s.position_min)
                rpos = s.position_endstop - s.homing_retract_dist
                r2pos = rpos - s.homing_retract_dist
            else:
                pos = s.position_endstop + 1.5*(
                    s.position_max - s.position_endstop)
                rpos = s.position_endstop + s.homing_retract_dist
                r2pos = rpos + s.homing_retract_dist
            # Initial homing
            homing_speed = s.homing_speed
            if axis == 2:
                homing_speed = min(homing_speed, self.max_z_velocity)
            homepos = [None, None, None, None]
            homepos[axis] = s.position_endstop
            coord = [None, None, None, None]
            coord[axis] = pos
            homing_state.home(coord, homepos, s.get_endstops(), homing_speed)
            # Retract
            coord[axis] = rpos
            homing_state.retract(coord, homing_speed)
            # Home again
            coord[axis] = r2pos
            homing_state.home(coord, homepos, s.get_endstops(),
                              homing_speed/2.0, second_home=True)
            if axis == 2:
                # Support endstop phase detection on Z axis
                coord[axis] = s.position_endstop + s.get_homed_offset()
                homing_state.set_homed_position(coord)
    def motor_off(self, print_time):
        self.limits = [(1.0, -1.0)] * 3
        for stepper in self.steppers:
            stepper.motor_enable(print_time, 0)
        self.need_motor_enable = True
    def _check_motor_enable(self, print_time, move):
        if move.axes_d[0] or move.axes_d[1]:
            self.steppers[0].motor_enable(print_time, 1)
            self.steppers[1].motor_enable(print_time, 1)
        if move.axes_d[2]:
            self.steppers[2].motor_enable(print_time, 1)
        need_motor_enable = False
        for i in StepList:
            need_motor_enable |= self.steppers[i].need_motor_enable
        self.need_motor_enable = need_motor_enable
    def _check_endstops(self, move):
        end_pos = move.end_pos
        for i in StepList:
            if (move.axes_d[i]
                and (end_pos[i] < self.limits[i][0]
                     or end_pos[i] > self.limits[i][1])):
                if self.limits[i][0] > self.limits[i][1]:
                    raise homing.EndstopMoveError(
                        end_pos, "Must home axis first")
                raise homing.EndstopMoveError(end_pos)
    def check_move(self, move):
        limits = self.limits
        xpos, ypos = move.end_pos[:2]
        if (xpos < limits[0][0] or xpos > limits[0][1]
            or ypos < limits[1][0] or ypos > limits[1][1]):
            self._check_endstops(move)
        if not move.axes_d[2]:
            # Normal XY move - use defaults
            return
        # Move with Z - update velocity and accel for slower Z axis
        self._check_endstops(move)
        z_ratio = move.move_d / abs(move.axes_d[2])
        move.limit_speed(
            self.max_z_velocity * z_ratio, self.max_z_accel * z_ratio)
    def move(self, print_time, move):
        if self.need_motor_enable:
            self._check_motor_enable(print_time, move)
        axes_d = move.axes_d
        cmove = self.cmove
        self.move_fill(
            cmove, print_time,
            move.accel_t, move.cruise_t, move.decel_t,
            move.start_pos[0], move.start_pos[1], move.start_pos[2],
            axes_d[0], axes_d[1], axes_d[2],
            move.start_v, move.cruise_v, move.accel)
        stepper_a, stepper_b, stepper_z = self.steppers
        if axes_d[0] or axes_d[1]:
            stepper_a.step_itersolve(cmove)
            stepper_b.step_itersolve(cmove)
        if axes_d[2]:
            stepper_z.step_itersolve(cmove)
