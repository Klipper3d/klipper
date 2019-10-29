# Code for handling the kinematics of cable winch robots
#
# Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import stepper, mathutil

class WinchKinematics:
    def __init__(self, toolhead, config):
        # Setup steppers at each anchor
        self.steppers = []
        self.anchors = []
        for i in range(26):
            name = 'stepper_' + chr(ord('a') + i)
            if i >= 3 and not config.has_section(name):
                break
            stepper_config = config.getsection(name)
            s = stepper.PrinterStepper(stepper_config)
            self.steppers.append(s)
            a = tuple([stepper_config.getfloat('anchor_' + n) for n in 'xyz'])
            self.anchors.append(a)
            s.setup_itersolve('winch_stepper_alloc', *a)
            s.set_trapq(toolhead.get_trapq())
            toolhead.register_move_handler(s.generate_steps)
        # Setup stepper max halt velocity
        max_velocity, max_accel = toolhead.get_max_velocity()
        max_halt_velocity = toolhead.get_max_axis_halt()
        for s in self.steppers:
            s.set_max_jerk(max_halt_velocity, max_accel)
        # Setup boundary checks
        self.set_position([0., 0., 0.], ())
    def get_steppers(self, flags=""):
        return list(self.steppers)
    def calc_position(self):
        # Use only first three steppers to calculate cartesian position
        spos = [s.get_commanded_position() for s in self.steppers[:3]]
        return mathutil.trilateration(self.anchors[:3], [sp*sp for sp in spos])
    def set_position(self, newpos, homing_axes):
        for s in self.steppers:
            s.set_position(newpos)
    def home(self, homing_state):
        # XXX - homing not implemented
        homing_state.set_axes([0, 1, 2])
        homing_state.set_homed_position([0., 0., 0.])
    def motor_off(self, print_time):
        for s in self.steppers:
            s.motor_enable(print_time, 0)
    def check_move(self, move):
        # XXX - boundary checks and speed limits not implemented
        pass
    def get_status(self):
        # XXX - homed_checks and rail limits not implemented
        return {'homed_axes': 'XYZ'}

def load_kinematics(toolhead, config):
    return WinchKinematics(toolhead, config)
