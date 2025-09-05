# Helper code for low-level motion queuing and flushing
#
# Copyright (C) 2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper

BGFLUSH_LOW_TIME = 0.200
BGFLUSH_BATCH_TIME = 0.200
BGFLUSH_EXTRA_TIME = 0.250
MOVE_HISTORY_EXPIRE = 30.
MIN_KIN_TIME = 0.100
MOVE_BATCH_TIME = 0.500
STEPCOMPRESS_FLUSH_TIME = 0.050
SDS_CHECK_TIME = 0.001 # step+dir+step filter in stepcompress.c

DRIP_SEGMENT_TIME = 0.050
DRIP_TIME = 0.100

class PrinterMotionQueuing:
    def __init__(self, config):
        self.printer = printer = config.get_printer()
        self.reactor = printer.get_reactor()
        # Low level C allocations
        self.trapqs = []
        self.stepcompress = []
        self.steppersyncs = []
        # Low-level C flushing calls
        ffi_main, ffi_lib = chelper.get_ffi()
        self.trapq_finalize_moves = ffi_lib.trapq_finalize_moves
        self.steppersync_start_gen_steps = ffi_lib.steppersync_start_gen_steps
        self.steppersync_finalize_gen_steps = \
            ffi_lib.steppersync_finalize_gen_steps
        self.steppersync_history_expire = ffi_lib.steppersync_history_expire
        # Flush notification callbacks
        self.flush_callbacks = []
        # History expiration
        self.clear_history_time = 0.
        # Flush tracking
        self.flush_timer = self.reactor.register_timer(self._flush_handler)
        self.do_kick_flush_timer = True
        self.last_flush_time = self.last_step_gen_time = 0.
        self.need_flush_time = self.need_step_gen_time = 0.
        self.check_flush_lookahead_cb = (lambda e: None)
        # MCU tracking
        self.all_mcus = [m for n, m in printer.lookup_objects(module='mcu')]
        self.mcu = self.all_mcus[0]
        self.can_pause = True
        if self.mcu.is_fileoutput():
            self.can_pause = False
        # Kinematic step generation scan window time tracking
        self.need_calc_kin_flush_delay = True
        self.kin_flush_delay = SDS_CHECK_TIME
        # Register handlers
        printer.register_event_handler("klippy:shutdown", self._handle_shutdown)
    def allocate_trapq(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        trapq = ffi_main.gc(ffi_lib.trapq_alloc(), ffi_lib.trapq_free)
        self.trapqs.append(trapq)
        return trapq
    def allocate_stepcompress(self, mcu, oid, name):
        name = name.encode("utf-8")[:15]
        ffi_main, ffi_lib = chelper.get_ffi()
        sc = ffi_main.gc(ffi_lib.stepcompress_alloc(oid, name),
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
    def _flush_motion_queues(self, must_flush_time, max_step_gen_time):
        # Invoke flush callbacks (if any)
        for cb in self.flush_callbacks:
            cb(must_flush_time, max_step_gen_time)
        # Generate stepper movement and transmit
        for mcu, ss in self.steppersyncs:
            clock = max(0, mcu.print_time_to_clock(must_flush_time))
            self.steppersync_start_gen_steps(ss, max_step_gen_time, clock)
        for mcu, ss in self.steppersyncs:
            clock = max(0, mcu.print_time_to_clock(must_flush_time))
            ret = self.steppersync_finalize_gen_steps(ss, clock)
            if ret:
                raise mcu.error("Internal error in MCU '%s' stepcompress"
                                % (mcu.get_name(),))
        # Determine maximum history to keep
        trapq_free_time = max_step_gen_time - self.kin_flush_delay
        clear_history_time = self.clear_history_time
        if not self.can_pause:
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
        self.trapq_finalize_moves(trapq, self.reactor.NEVER, 0.)
    def lookup_trapq_append(self):
        ffi_main, ffi_lib = chelper.get_ffi()
        return ffi_lib.trapq_append
    def stats(self, eventtime):
        # Hack to globally invoke mcu check_active()
        for m in self.all_mcus:
            m.check_active(self.last_step_gen_time, eventtime)
        # Calculate history expiration
        est_print_time = self.mcu.estimated_print_time(eventtime)
        self.clear_history_time = est_print_time - MOVE_HISTORY_EXPIRE
        return False, ""
    # Kinematic step generation scan window time tracking
    def get_kin_flush_delay(self):
        return self.kin_flush_delay
    def _calc_kin_flush_delay(self):
        self.need_calc_kin_flush_delay = False
        ffi_main, ffi_lib = chelper.get_ffi()
        kin_flush_delay = SDS_CHECK_TIME
        for mcu, sc in self.stepcompress:
            sk = ffi_lib.stepcompress_get_stepper_kinematics(sc)
            if sk == ffi_main.NULL:
                continue
            trapq = ffi_lib.itersolve_get_trapq(sk)
            if trapq == ffi_main.NULL:
                continue
            pre_active = ffi_lib.itersolve_get_gen_steps_pre_active(sk)
            post_active = ffi_lib.itersolve_get_gen_steps_post_active(sk)
            kin_flush_delay = max(kin_flush_delay, pre_active, post_active)
        self.kin_flush_delay = kin_flush_delay
    # Flush tracking
    def _handle_shutdown(self):
        self.can_pause = False
    def setup_lookahead_flush_callback(self, check_flush_lookahead_cb):
        self.check_flush_lookahead_cb = check_flush_lookahead_cb
    def advance_flush_time(self, target_time, lazy_target=False):
        want_flush_time = want_step_gen_time = target_time
        if lazy_target:
            # Account for step gen scan windows and optimize step compression
            want_step_gen_time -= self.kin_flush_delay
            want_flush_time = want_step_gen_time - STEPCOMPRESS_FLUSH_TIME
        want_flush_time = max(want_flush_time, self.last_flush_time)
        flush_time = self.last_flush_time
        if want_flush_time > flush_time + 10. * MOVE_BATCH_TIME:
            # Use closer startup time when coming out of idle state
            curtime = self.reactor.monotonic()
            est_print_time = self.mcu.estimated_print_time(curtime)
            flush_time = max(flush_time, est_print_time)
        while 1:
            flush_time = min(flush_time + MOVE_BATCH_TIME, want_flush_time)
            # Generate steps via itersolve
            want_sg_wave = min(flush_time + STEPCOMPRESS_FLUSH_TIME,
                               want_step_gen_time)
            step_gen_time = max(self.last_step_gen_time, want_sg_wave,
                                flush_time)
            self._flush_motion_queues(flush_time, step_gen_time)
            self.last_flush_time = flush_time
            self.last_step_gen_time = step_gen_time
            if flush_time >= want_flush_time:
                break
    def flush_all_steps(self):
        self.need_calc_kin_flush_delay = True
        self.advance_flush_time(self.need_step_gen_time)
    def calc_step_gen_restart(self, est_print_time):
        if self.need_calc_kin_flush_delay:
            self._calc_kin_flush_delay()
        kin_time = max(est_print_time + MIN_KIN_TIME, self.last_step_gen_time)
        return kin_time + self.kin_flush_delay
    def _flush_handler(self, eventtime):
        try:
            # Check if flushing is done via lookahead queue
            ret = self.check_flush_lookahead_cb(eventtime)
            if ret is not None:
                return ret
            # Flush motion queues
            est_print_time = self.mcu.estimated_print_time(eventtime)
            while 1:
                end_flush = self.need_flush_time + BGFLUSH_EXTRA_TIME
                if self.last_flush_time >= end_flush:
                    self.do_kick_flush_timer = True
                    return self.reactor.NEVER
                buffer_time = self.last_flush_time - est_print_time
                if buffer_time > BGFLUSH_LOW_TIME:
                    return eventtime + buffer_time - BGFLUSH_LOW_TIME
                ftime = est_print_time + BGFLUSH_LOW_TIME + BGFLUSH_BATCH_TIME
                self.advance_flush_time(min(end_flush, ftime))
        except:
            logging.exception("Exception in flush_handler")
            self.printer.invoke_shutdown("Exception in flush_handler")
        return self.reactor.NEVER
    def note_mcu_movequeue_activity(self, mq_time, is_step_gen=True):
        if is_step_gen:
            mq_time += self.kin_flush_delay
            self.need_step_gen_time = max(self.need_step_gen_time, mq_time)
        self.need_flush_time = max(self.need_flush_time, mq_time)
        if self.do_kick_flush_timer:
            self.do_kick_flush_timer = False
            self.reactor.update_timer(self.flush_timer, self.reactor.NOW)
    def drip_update_time(self, start_time, end_time, drip_completion):
        # Disable background flushing from timer
        self.reactor.update_timer(self.flush_timer, self.reactor.NEVER)
        self.do_kick_flush_timer = False
        # Flush in segments until drip_completion signal
        flush_delay = DRIP_TIME + STEPCOMPRESS_FLUSH_TIME + self.kin_flush_delay
        flush_time = start_time
        while flush_time < end_time:
            if drip_completion.test():
                break
            curtime = self.reactor.monotonic()
            est_print_time = self.mcu.estimated_print_time(curtime)
            wait_time = flush_time - est_print_time - flush_delay
            if wait_time > 0. and self.can_pause:
                # Pause before sending more steps
                drip_completion.wait(curtime + wait_time)
                continue
            flush_time = min(flush_time + DRIP_SEGMENT_TIME, end_time)
            self.note_mcu_movequeue_activity(flush_time)
            self.advance_flush_time(flush_time, lazy_target=True)
        # Restore background flushing
        self.reactor.update_timer(self.flush_timer, self.reactor.NOW)
        self.advance_flush_time(self.need_step_gen_time)

def load_config(config):
    return PrinterMotionQueuing(config)
