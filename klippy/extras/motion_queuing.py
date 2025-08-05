# Helper code for low-level motion queuing and flushing
#
# Copyright (C) 2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper

class PrinterMotionQueuing:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.steppers = []
        self.trapqs = []
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
    def allocate_trapq(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapqs.append(trapq)
        return trapq
    def register_stepper(self, config, stepper):
        self.steppers.append(stepper)
    def flush_motion_queues(self, must_flush_time, max_step_gen_time):
        for stepper in self.steppers:
            stepper.generate_steps(max_step_gen_time)
    def clean_motion_queues(self, trapq_free_time, clear_history_time):
        for trapq in self.trapqs:
            self.trapq_finalize_moves(trapq, trapq_free_time,
                                      clear_history_time)
    def wipe_trapq(self, trapq):
        # Expire any remaining movement in the trapq (force to history list)
        NEVER = 9999999999999999.
        self.trapq_finalize_moves(trapq, NEVER, 0.)
    def lookup_trapq_append(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_lib.trapq_append

def load_config(config):
    return PrinterMotionQueuing(config)
