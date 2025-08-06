# Helper code for low-level motion queuing and flushing
#
# Copyright (C) 2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper

MOVE_HISTORY_EXPIRE = 30.

class PrinterMotionQueuing:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.trapqs = []
        self.stepcompress = []
        self.steppersyncs = []
        self.flush_callbacks = []
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
        self.steppersync_generate_steps = ffi_lib.steppersync_generate_steps
        self.steppersync_flush = ffi_lib.steppersync_flush
        self.steppersync_history_expire = ffi_lib.steppersync_history_expire
        self.clear_history_time = 0.
        is_debug = self.printer.get_start_args().get('debugoutput') is not None
        self.is_debugoutput = is_debug
    def allocate_trapq(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapqs.append(trapq)
        return trapq
    def allocate_stepcompress(self, mcu, oid):
        ffi_main, ffi_lib = chelper.get_ffi()
        sc = ffi_main.gc(ffi_lib.stepcompress_alloc(oid),
                         ffi_lib.stepcompress_free)
        self.stepcompress.append((mcu, sc))
        return sc
    def allocate_steppersync(self, mcu, serialqueue, move_count):
        stepqueues = []
        for sc_mcu, sc in self.stepcompress:
            if sc_mcu is mcu:
                stepqueues.append(sc)
        ffi_main, ffi_lib = chelper.get_ffi()
        ss = ffi_main.gc(
            ffi_lib.steppersync_alloc(serialqueue, stepqueues, len(stepqueues),
                                      move_count),
            ffi_lib.steppersync_free)
        self.steppersyncs.append((mcu, ss))
        return ss
    def register_flush_callback(self, callback):
        self.flush_callbacks.append(callback)
    def unregister_flush_callback(self, callback):
        if callback in self.flush_callbacks:
            fcbs = list(self.flush_callbacks)
            fcbs.remove(callback)
            self.flush_callbacks = fcbs
    def flush_motion_queues(self, must_flush_time, max_step_gen_time,
                            trapq_free_time):
        # Invoke flush callbacks (if any)
        for cb in self.flush_callbacks:
            cb(must_flush_time, max_step_gen_time)
        # Generate stepper movement and transmit
        for mcu, ss in self.steppersyncs:
            clock = max(0, mcu.print_time_to_clock(must_flush_time))
            # Generate steps
            ret = self.steppersync_generate_steps(ss, max_step_gen_time, clock)
            if ret:
                raise mcu.error("Internal error in MCU '%s' stepcompress"
                                % (mcu.get_name(),))
            # Flush steps from steppersync
            ret = self.steppersync_flush(ss, clock)
            if ret:
                raise mcu.error("Internal error in MCU '%s' stepcompress"
                                % (mcu.get_name(),))
        # Determine maximum history to keep
        clear_history_time = self.clear_history_time
        if self.is_debugoutput:
            clear_history_time = trapq_free_time - MOVE_HISTORY_EXPIRE
        # Move processed trapq moves to history list, and expire old history
        for trapq in self.trapqs:
            self.trapq_finalize_moves(trapq, trapq_free_time,
                                      clear_history_time)
        # Clean up old history entries in stepcompress objects
        for mcu, ss in self.steppersyncs:
            clock = max(0, mcu.print_time_to_clock(clear_history_time))
            self.steppersync_history_expire(ss, clock)
    def wipe_trapq(self, trapq):
        # Expire any remaining movement in the trapq (force to history list)
        NEVER = 9999999999999999.
        self.trapq_finalize_moves(trapq, NEVER, 0.)
    def lookup_trapq_append(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_lib.trapq_append
    def stats(self, eventtime):
        mcu = self.printer.lookup_object('mcu')
        est_print_time = mcu.estimated_print_time(eventtime)
        self.clear_history_time = est_print_time - MOVE_HISTORY_EXPIRE
        return False, ""

def load_config(config):
    return PrinterMotionQueuing(config)
