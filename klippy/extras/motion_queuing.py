# Helper code for low-level motion queuing and flushing
#
# Copyright (C) 2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class PrinterMotionQueuing:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = []
    def register_stepper(self, config, stepper):
        self.steppers.append(stepper)
    def flush_motion_queues(self, must_flush_time, max_step_gen_time):
        for stepper in self.steppers:
            stepper.generate_steps(max_step_gen_time)

def load_config(config):
    return PrinterMotionQueuing(config)
