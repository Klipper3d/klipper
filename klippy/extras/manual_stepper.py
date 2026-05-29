# Support for a manual controlled stepper
#
# Copyright (C) 2019-2025  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import chelper
import stepper
from . import force_move

# Forward guard added between "now" (the moment we re-anchor host/MCU clocks
# during cancel reconcile) and the print_time we hand to the MCU as the new
# `next_step_time`. The next move's first queue_step must arrive at the MCU
# with clock >= this anchor; without a guard, message latency causes the MCU
# to see step intervals <= 0 and shut down with "Timer too close". 250ms
# matches Klipper's BUFFER_TIME_START and gives enough margin for host
# serial latency plus in-flight steps from the canceled move.
CANCEL_RESYNC_GUARD = 0.250

# Maximum time MANUAL_STEPPER (and related commands) will block waiting
# for an in-flight cancel to drain before failing the new request. The
# normal cancel protocol completes in ~30-50ms; this is a generous
# upper bound that should never trip under healthy operation.
CANCEL_DRAIN_WAIT_TIMEOUT = 4.0
CANCEL_DRAIN_POLL = 0.010

class ManualStepper:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name()
        if config.get('endstop_pin', None) is not None:
            self.can_home = True
            self.rail = stepper.LookupRail(
                config, need_position_minmax=False, default_position_endstop=0.)
            self.steppers = self.rail.get_steppers()
        else:
            self.can_home = False
            self.rail = stepper.PrinterStepper(config)
            self.steppers = [self.rail]
        self.velocity = config.getfloat('velocity', 5., above=0.)
        self.accel = self.homing_accel = config.getfloat('accel', 0., minval=0.)
        self.next_cmd_time = 0.
        self.commanded_pos = 0.
        self.pos_min = config.getfloat('position_min', None)
        self.pos_max = config.getfloat('position_max', None)
        # Setup iterative solver
        self.motion_queuing = self.printer.load_object(config, 'motion_queuing')
        self.trapq = self.motion_queuing.allocate_trapq()
        self.trapq_append = self.motion_queuing.lookup_trapq_append()
        ffi_main, ffi_lib = chelper.get_ffi()
        self._ffi_main = ffi_main
        self._trapq_truncate_after = ffi_lib.trapq_truncate_after
        self.splice_delay = config.getfloat(
            'splice_delay', None, minval=0., above=0.)
        self.rail.setup_itersolve('cartesian_stepper_alloc', b'x')
        self.rail.set_trapq(self.trapq)
        # Registered with toolhead as an axtra axis
        self.axis_gcode_id = None
        self.instant_corner_v = 0.
        self.gaxis_limit_velocity = self.gaxis_limit_accel = 0.
        # Phase-1 cancel controller scaffolding
        self.cancel_state = "IDLE"
        self.cancel_token = 0
        self.active_move_id = None
        self.cancel_deadline = 0.
        self._next_move_id = 1
        self._cancel_query_timeout = 2.000
        self._cancel_poll_interval = 0.005
        self._cancel_timeout_at = 0.
        # Set after a cancel reconcile to force the next motion-emitting
        # command to re-issue reset_step_clock with a FRESH safe_clock
        # (chosen at do_move time, not at cancel time). Without this, an
        # arbitrarily long gap between CANCEL_STEP and the next MOVE lets
        # the original safe_clock fall into the past on the MCU and the
        # first new queue_step shuts the MCU down with "Timer too close".
        self._needs_step_clock_reanchor = False
        # Set by _finalize_cancel_timing; the next sync_print_time() must not
        # dwell toward the canceled move's old planned end.
        self._post_cancel_skip_dwell = False
        self.reactor = self.printer.get_reactor()
        self._cancel_timer = self.reactor.register_timer(
            self._cancel_step_tick, self.reactor.NEVER)
        # Register commands
        stepper_name = self.name.split()[1]
        gcode = self.printer.lookup_object('gcode')
        gcode.register_mux_command('MANUAL_STEPPER', "STEPPER",
                                   stepper_name, self.cmd_MANUAL_STEPPER,
                                   desc=self.cmd_MANUAL_STEPPER_help)
        gcode.register_mux_command('CANCEL_STEP', "STEPPER",
                                   stepper_name, self.cmd_CANCEL_STEP,
                                   desc=self.cmd_CANCEL_STEP_help)
    def get_name(self):
        return self.name
    def sync_print_time(self):
        toolhead = self.printer.lookup_object('toolhead')
        mcu = self.steppers[0].get_mcu()
        est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
        max_time = est_print_time + CANCEL_RESYNC_GUARD
        print_time = toolhead.get_last_move_time()
        # Drop a stale far-future next_cmd_time left by a canceled SYNC=0 move.
        # Do not pull print_time down - it may legitimately lead est while
        # prior move steps are still in the MCU pipeline.
        if (self.next_cmd_time > max_time
            and self.next_cmd_time > print_time + 0.500):
            self.next_cmd_time = max_time
        skip_dwell = self._post_cancel_skip_dwell
        if skip_dwell:
            self._post_cancel_skip_dwell = False
            if self.next_cmd_time < print_time:
                self.next_cmd_time = print_time
        elif self.next_cmd_time > print_time:
            toolhead.dwell(self.next_cmd_time - print_time)
        elif self.next_cmd_time < print_time:
            self.next_cmd_time = print_time
    def do_enable(self, enable):
        stepper_names = [s.get_name() for s in self.steppers]
        stepper_enable = self.printer.lookup_object('stepper_enable')
        stepper_enable.set_motors_enable(stepper_names, enable)
        if not enable:
            self._clear_cancel_tracking("motor_disable")
    def do_set_position(self, setpos):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
        # Re-anchor local timing so a prior canceled async move does not
        # force subsequent commands to wait for its old end time.
        self.next_cmd_time = toolhead.get_last_move_time()
        self._clear_cancel_tracking("set_position")
        self.commanded_pos = setpos
        self.rail.set_position([self.commanded_pos, 0., 0.])
    def _set_cancel_state(self, new_state, reason):
        old_state = self.cancel_state
        if old_state == new_state:
            return
        self.cancel_state = new_state
        logging.info(
            "manual_stepper_cancel_transition stepper=%s old=%s new=%s "
            "token=%d move_id=%s deadline=%.6f next_cmd_time=%.6f reason=%s",
            self.name, old_state, new_state, self.cancel_token,
            self.active_move_id, self.cancel_deadline, self.next_cmd_time,
            reason)
    def _clear_cancel_tracking(self, reason):
        old_deadline = self.cancel_deadline
        self.active_move_id = None
        self.cancel_deadline = 0.
        self._cancel_timeout_at = 0.
        if reason == "cancel_complete":
            self._finalize_cancel_timing(old_deadline)
        self._set_cancel_state("IDLE", reason)
    def _mark_sync0_move_active(self):
        if self._is_cancel_busy():
            return
        self.active_move_id = self._next_move_id
        self._next_move_id += 1
        self.cancel_deadline = self.next_cmd_time
        self._set_cancel_state("MOVE_ACTIVE", "sync0_move")
    def _refresh_cancel_activity(self):
        if self.active_move_id is None:
            return
        reactor = self.printer.get_reactor()
        curtime = reactor.monotonic()
        est_print_time = self.steppers[0].get_mcu().estimated_print_time(curtime)
        if est_print_time >= self.cancel_deadline:
            self._clear_cancel_tracking("move_completed")
    def _get_cancel_stepper(self):
        if len(self.steppers) != 1:
            return None
        return self.steppers[0]
    def _do_protocol_cancel(self):
        mstepper = self._get_cancel_stepper()
        if mstepper is None or not mstepper.is_cancel_protocol_supported():
            return "unsupported_firmware"
        return None
    def _cancel_request(self):
        self.cancel_token += 1
        self._cancel_timeout_at = (self.reactor.monotonic()
                                   + self._cancel_query_timeout)
        self._set_cancel_state("CANCEL_REQUESTED", "host_request")
        self.reactor.update_timer(self._cancel_timer, self.reactor.NOW)
    def _is_cancel_busy(self):
        return self.cancel_state in ("CANCEL_REQUESTED", "CANCEL_ARMED",
                                     "CANCEL_TRIGGERED", "DRAINING",
                                     "RECONCILE")
    def _calc_post_cancel_time(self, mcu, print_time=None, old_deadline=0.):
        est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
        if print_time is None:
            toolhead = self.printer.lookup_object('toolhead')
            print_time = toolhead.print_time
        base = max(est_print_time, print_time)
        if (old_deadline and base > est_print_time + 1.0
            and base >= old_deadline - 1.0):
            base = est_print_time
        return base + CANCEL_RESYNC_GUARD
    def _calc_mcu_anchor_time(self, mcu, print_time=None):
        return self._calc_post_cancel_time(mcu, print_time)
    def _reset_stale_movequeue_horizon(self):
        # note_mcu_movequeue_activity() only ever increases
        # need_step_gen_time / need_flush_time. After cancel those horizons
        # still point at the canceled move's planned end, so flush_all_steps()
        # and background step generation keep targeting that far-future time.
        mstepper = self._get_cancel_stepper()
        if mstepper is None:
            return
        mcu = mstepper.get_mcu()
        toolhead = self.printer.lookup_object('toolhead')
        est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
        anchor = self._calc_post_cancel_time(
            mcu, toolhead.print_time, self.cancel_deadline)
        mq = self.motion_queuing
        mq.need_step_gen_time = anchor + mq.kin_flush_delay
        mq.need_flush_time = anchor
    def _reset_retarget_movequeue_horizon(self, end_time):
        # need_step_gen_time may still point at a far-future SYNC=0 move;
        # replace it so flushing targets the retargeted trapq horizon.
        mcu = self.steppers[0].get_mcu()
        est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
        mq = self.motion_queuing
        gen_time = max(end_time, est_print_time) + mq.kin_flush_delay
        mq.need_step_gen_time = gen_time
        mq.need_flush_time = max(end_time, est_print_time)
    def _get_splice_delay(self):
        if self.splice_delay is not None:
            return self.splice_delay
        return self.motion_queuing.get_kin_flush_delay()
    def _finalize_cancel_timing(self, old_deadline=0.):
        # Drop the canceled SYNC=0 move's far-future planned end time and
        # re-anchor to MCU "now" so the next MANUAL_STEPPER command does not
        # dwell until the old move deadline in sync_print_time().
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        mstepper = self._get_cancel_stepper()
        if mstepper is None:
            self.next_cmd_time = print_time + CANCEL_RESYNC_GUARD
            self._post_cancel_skip_dwell = True
            return
        mcu = mstepper.get_mcu()
        est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
        self.next_cmd_time = self._calc_post_cancel_time(
            mcu, print_time, old_deadline)
        self._reset_stale_movequeue_horizon()
        self._post_cancel_skip_dwell = True
        logging.info(
            "manual_stepper_cancel_next_cmd_time stepper=%s "
            "next_cmd_time=%.6f print_time=%.6f est_print_time=%.6f "
            "old_deadline=%.6f",
            self.name, self.next_cmd_time, print_time, est_print_time,
            old_deadline)
    def _await_cancel_complete(self, gcmd, action,
                               timeout=CANCEL_DRAIN_WAIT_TIMEOUT):
        # Block (yielding to the reactor so the cancel timer can run)
        # until any in-flight cancel for this stepper has fully drained
        # back to IDLE. This lets users chain
        #   CANCEL_STEP STEPPER=...
        #   MANUAL_STEPPER STEPPER=... MOVE=... SYNC=0
        # without an explicit polling step (M400 does NOT help here -
        # the cancel protocol runs on a reactor timer outside the
        # toolhead lookahead/movequeue that M400 waits on).
        # Waiting is also a CORRECTNESS requirement: silently letting a
        # new MANUAL_STEPPER (e.g. SET_POSITION) run while cancel is
        # mid-flight would call _clear_cancel_tracking() and abort the
        # protocol before stepper_cancel_finish was sent, leaving the
        # MCU stepper stuck in NEED_RESET.
        if not self._is_cancel_busy():
            return
        deadline = self.reactor.monotonic() + timeout
        while self._is_cancel_busy():
            now = self.reactor.monotonic()
            if now >= deadline:
                raise gcmd.error(
                    "%s timed out waiting for cancel to complete" % (action,))
            wake = min(deadline, now + CANCEL_DRAIN_POLL)
            self.reactor.pause(wake)
    def _cancel_reconcile(self):
        mstepper = self._get_cancel_stepper()
        if mstepper is None:
            return False
        move_id = self.active_move_id
        deadline = self.cancel_deadline
        mcu = mstepper.get_mcu()
        # Re-anchor host stepcompress and MCU step-clock reset slightly in
        # the FUTURE. The MCU's stepper_cancel_finish sets next_step_time to
        # `safe_clock`; the very next move's first queue_step must arrive
        # with clock >= safe_clock or the MCU shuts down with "Timer too
        # close". Anchoring at "now" loses this race (host->MCU latency,
        # serial queue, etc.). CANCEL_RESYNC_GUARD is the margin used when
        # re-syncing kinematic/MCU clocks after cancel.
        mcu_anchor = self._calc_mcu_anchor_time(mcu)
        safe_clock = mcu.print_time_to_clock(mcu_anchor)
        finish, commanded_pos = mstepper.cancel_reconcile(safe_clock)
        if not finish or not finish.get('ok', 0):
            if finish is not None and finish.get('active', 0):
                # MCU still reports active cancel operation - keep polling.
                return False
            if finish is not None and not finish.get('can_reset', 1):
                # Cancel trigger has not fully settled yet - keep polling.
                return False
            return False
        if commanded_pos is not None:
            old_pos = self.commanded_pos
            self.commanded_pos = commanded_pos
            self.rail.set_position([self.commanded_pos, 0., 0.])
            logging.info(
                "manual_stepper_cancel_position stepper=%s "
                "planning_pos=%.6f physical_pos=%.6f token=%d",
                self.name, old_pos, commanded_pos, self.cancel_token)
        # Force the next motion-generating command to re-issue
        # reset_step_clock with a freshly chosen safe_clock. The anchor
        # picked HERE goes stale as soon as the user delays the next
        # MOVE (e.g. by chaining ENABLE=0 / ENABLE=1 / a steer move
        # before the new drive MOVE). See _reanchor_step_clock_if_needed.
        self._needs_step_clock_reanchor = True
        self._set_cancel_state("DONE", "reconcile_complete")
        self._clear_cancel_tracking("cancel_complete")
        logging.info(
            "manual_stepper_cancel_result stepper=%s status=cancel_pending "
            "token=%d move_id=%s deadline=%.6f",
            self.name, self.cancel_token, move_id, deadline)
        return True
    def _cancel_abort_timeout(self):
        move_id = self.active_move_id
        deadline = self.cancel_deadline
        self.active_move_id = None
        self.cancel_deadline = 0.
        self._cancel_timeout_at = 0.
        self._finalize_cancel_timing(deadline)
        self._needs_step_clock_reanchor = False
        self._set_cancel_state("ERROR_TIMEOUT", "cancel_timeout")
        logging.info(
            "manual_stepper_cancel_result stepper=%s status=cancel_timeout "
            "token=%d move_id=%s deadline=%.6f",
            self.name, self.cancel_token, move_id, deadline)
        self.printer.lookup_object('gcode').respond_info(
            "CANCEL_STEP %s cancel_timeout" % (self.name.split()[1],))
    def _cancel_step_tick(self, eventtime):
        while 1:
            if self.active_move_id is None:
                return self.reactor.NEVER
            if (self._cancel_timeout_at and eventtime >= self._cancel_timeout_at
                and self.cancel_state in ("CANCEL_REQUESTED", "CANCEL_ARMED",
                                          "CANCEL_TRIGGERED")):
                self._cancel_abort_timeout()
                return self.reactor.NEVER
            mstepper = self._get_cancel_stepper()
            if mstepper is None or not mstepper.is_cancel_protocol_supported():
                self._cancel_abort_timeout()
                return self.reactor.NEVER
            if self.cancel_state == "CANCEL_REQUESTED":
                mstepper.cancel_drain_host_pipeline()
                self._reset_stale_movequeue_horizon()
                mstepper.cancel_start()
                self._set_cancel_state("CANCEL_ARMED", "protocol_start")
                continue
            if self.cancel_state == "CANCEL_ARMED":
                mstepper.cancel_trigger()
                self._set_cancel_state("CANCEL_TRIGGERED", "protocol_trigger")
                return eventtime + self._cancel_poll_interval
            if self.cancel_state == "CANCEL_TRIGGERED":
                status = mstepper.cancel_status()
                if status['active'] or not status['can_reset']:
                    return eventtime + self._cancel_poll_interval
                self._set_cancel_state("DRAINING", "status_safe")
                continue
            if self.cancel_state == "DRAINING":
                mstepper = self._get_cancel_stepper()
                if mstepper is not None:
                    mstepper.cancel_drain_host_pipeline()
                self.motion_queuing.wipe_trapq(self.trapq)
                self._reset_stale_movequeue_horizon()
                self._set_cancel_state("RECONCILE", "trapq_drained")
                continue
            if self.cancel_state == "RECONCILE":
                if self._cancel_reconcile():
                    return self.reactor.NEVER
                if (self._cancel_timeout_at
                    and eventtime < self._cancel_timeout_at):
                    self._cancel_timeout_at = eventtime
                return eventtime + self._cancel_poll_interval
            return self.reactor.NEVER
    def _reanchor_step_clock_if_needed(self):
        # Re-issue reset_step_clock + matching host stepcompress reset
        # with a FRESH safe_clock chosen "now" (i.e. just before the
        # next move's first queue_step is generated). The cancel
        # reconcile that ran earlier picked a safe_clock that was only
        # CANCEL_RESYNC_GUARD ahead of est_print_time AT THAT MOMENT;
        # any wall-clock delay before the user's next MOVE (chained
        # ENABLE=0/1, an unrelated stepper command, plain user latency,
        # etc.) eats that guard and the MCU shuts down with "Timer too
        # close" on the very first new queue_step. Re-anchoring here
        # bounds the host->MCU latency window to a single BG flush +
        # serial transmit, comfortably within CANCEL_RESYNC_GUARD.
        if not self._needs_step_clock_reanchor:
            return
        mstepper = self._get_cancel_stepper()
        if mstepper is None:
            self._needs_step_clock_reanchor = False
            return
        mcu = mstepper.get_mcu()
        mcu_anchor = self._calc_mcu_anchor_time(mcu)
        safe_clock = mcu.print_time_to_clock(mcu_anchor)
        mstepper.reanchor_step_clock(safe_clock, mcu_anchor)
        self.next_cmd_time = mcu_anchor
        self._needs_step_clock_reanchor = False
    def _submit_move(self, movetime, movepos, speed, accel):
        cp = self.commanded_pos
        dist = movepos - cp
        axis_r, accel_t, cruise_t, cruise_v = force_move.calc_move_time(
            dist, speed, accel)
        self.trapq_append(self.trapq, movetime,
                          accel_t, cruise_t, accel_t,
                          cp, 0., 0., axis_r, 0., 0.,
                          0., cruise_v, accel)
        self.commanded_pos = movepos
        return movetime + accel_t + cruise_t + accel_t
    def do_move(self, movepos, speed, accel, sync=True):
        # Re-anchor before sync_print_time so a stale next_cmd_time from a
        # canceled SYNC=0 move cannot trigger a multi-second toolhead dwell.
        self._reanchor_step_clock_if_needed()
        self.sync_print_time()
        self.next_cmd_time = self._submit_move(self.next_cmd_time, movepos,
                                               speed, accel)
        if sync:
            self._clear_cancel_tracking("sync_move")
        else:
            self._mark_sync0_move_active()
        self.motion_queuing.note_mcu_movequeue_activity(self.next_cmd_time)
        if sync:
            self.sync_print_time()
    def _check_move_range(self, movepos, gcmd):
        if ((self.pos_min is not None and movepos < self.pos_min)
            or (self.pos_max is not None and movepos > self.pos_max)):
            raise gcmd.error("Move out of range")
    def do_retarget(self, movepos, speed, accel, gcmd):
        self._refresh_cancel_activity()
        if self.active_move_id is None:
            raise gcmd.error("RETARGET requires active MOVE SYNC=0")
        mstepper = self.steppers[0]
        mcu = mstepper.get_mcu()
        splice_delay = self._get_splice_delay()
        est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
        splice_time = est_print_time + splice_delay
        pos_x = self._ffi_main.new("double *")
        pos_y = self._ffi_main.new("double *")
        pos_z = self._ffi_main.new("double *")
        ret = self._trapq_truncate_after(
            self.trapq, splice_time, pos_x, pos_y, pos_z)
        if ret < 0:
            raise gcmd.error("RETARGET failed to truncate trapq")
        splice_pos = pos_x[0]
        mstepper.cancel_drain_host_pipeline()
        physical_pos = mstepper.retarget_reanchor_host(splice_time)
        self.commanded_pos = physical_pos
        self.rail.set_position([physical_pos, 0., 0.])
        self.next_cmd_time = self._submit_move(
            splice_time, movepos, speed, accel)
        self.cancel_deadline = self.next_cmd_time
        self._reset_retarget_movequeue_horizon(self.next_cmd_time)
        self.motion_queuing.note_mcu_movequeue_activity(self.next_cmd_time)
        logging.info(
            "manual_stepper_retarget stepper=%s splice_time=%.6f "
            "splice_pos=%.6f physical_pos=%.6f retarget_pos=%.6f speed=%.3f "
            "move_id=%s",
            self.name, splice_time, splice_pos, physical_pos, movepos, speed,
            self.active_move_id)
    def do_homing_move(self, movepos, speed, accel,
                       probe_pos, triggered, check_trigger):
        if not self.can_home:
            raise self.printer.command_error(
                "No endstop for this manual stepper")
        self.homing_accel = accel
        pos = [movepos, 0., 0., 0.]
        endstops = self.rail.get_endstops()
        phoming = self.printer.lookup_object('homing')
        phoming.manual_home(self, endstops, pos, speed,
                            probe_pos, triggered, check_trigger)
        self._clear_cancel_tracking("homing_move")
        self.sync_print_time()
    cmd_MANUAL_STEPPER_help = (
        "Command a manually configured stepper; RETARGET splices trapq motion")
    def cmd_MANUAL_STEPPER(self, gcmd):
        self._await_cancel_complete(gcmd, "MANUAL_STEPPER")
        if gcmd.get('GCODE_AXIS', None) is not None:
            return self.command_with_gcode_axis(gcmd)
        if self.axis_gcode_id is not None:
            raise gcmd.error("Must unregister from gcode axis first")
        enable = gcmd.get_int('ENABLE', None)
        if enable is not None:
            self.do_enable(enable)
        setpos = gcmd.get_float('SET_POSITION', None)
        if setpos is not None:
            self.do_set_position(setpos)
        speed = gcmd.get_float('SPEED', self.velocity, above=0.)
        accel = gcmd.get_float('ACCEL', self.accel, minval=0.)
        if gcmd.get_float('RETARGET', None) is not None:
            if (gcmd.get_float('MOVE', None) is not None
                or gcmd.get('STOP_ON_ENDSTOP', None) is not None
                or gcmd.get_int('ENABLE', None) is not None
                or gcmd.get_float('SET_POSITION', None) is not None):
                raise gcmd.error(
                    "RETARGET cannot be combined with MOVE, ENABLE, "
                    "SET_POSITION, or STOP_ON_ENDSTOP")
            movepos = gcmd.get_float('RETARGET')
            self._check_move_range(movepos, gcmd)
            self.do_retarget(movepos, speed, accel, gcmd)
            return
        homing_move = gcmd.get('STOP_ON_ENDSTOP', None)
        if homing_move is not None:
            old_map = {'-2': 'try_inverted_home', '-1': 'inverted_home',
                       '1': 'home', '2': 'try_home'}.get(homing_move)
            if old_map is not None:
                pconfig = self.printer.lookup_object('configfile')
                pconfig.deprecate_gcode("MANUAL_STEPPER", "STOP_ON_ENDSTOP",
                                        homing_move)
                homing_move = old_map
            is_try = homing_move.startswith('try_')
            homing_move = homing_move[is_try*4:]
            is_inverted = homing_move.startswith('inverted_')
            homing_move = homing_move[is_inverted*9:]
            if homing_move not in ["probe", "home"]:
                raise gcmd.error("Unknown STOP_ON_ENDSTOP request")
            is_probe = (homing_move == "probe")
            movepos = gcmd.get_float('MOVE')
            self._check_move_range(movepos, gcmd)
            self.do_homing_move(movepos, speed, accel,
                                is_probe, not is_inverted, not is_try)
        elif gcmd.get_float('MOVE', None) is not None:
            movepos = gcmd.get_float('MOVE')
            self._check_move_range(movepos, gcmd)
            sync = gcmd.get_int('SYNC', 1)
            self.do_move(movepos, speed, accel, sync)
        elif gcmd.get_int('SYNC', 0):
            self.sync_print_time()
    cmd_CANCEL_STEP_help = "Cancel active MANUAL_STEPPER MOVE SYNC=0 motion"
    def cmd_CANCEL_STEP(self, gcmd):
        if self.axis_gcode_id is not None:
            gcmd.respond_info("reject_gcode_axis")
            return
        if self._is_cancel_busy():
            gcmd.respond_info("cancel_pending")
            return
        self._refresh_cancel_activity()
        if self.active_move_id is None:
            gcmd.respond_info("already_idle")
            return
        result = self._do_protocol_cancel()
        if result == "unsupported_firmware":
            raise gcmd.error("MCU firmware does not support stepper cancel protocol")
        self._cancel_request()
        gcmd.respond_info("cancel_pending")
    # Register as a gcode axis
    def command_with_gcode_axis(self, gcmd):
        self._await_cancel_complete(gcmd, "GCODE_AXIS")
        gcode_move = self.printer.lookup_object("gcode_move")
        toolhead = self.printer.lookup_object('toolhead')
        gcode_axis = gcmd.get('GCODE_AXIS').upper()
        instant_corner_v = gcmd.get_float('INSTANTANEOUS_CORNER_VELOCITY', 1.,
                                          minval=0.)
        limit_velocity = gcmd.get_float('LIMIT_VELOCITY', 999999.9, above=0.)
        limit_accel = gcmd.get_float('LIMIT_ACCEL', 999999.9, above=0.)
        if self.axis_gcode_id is not None:
            if gcode_axis:
                raise gcmd.error("Must unregister axis first")
            # Unregister
            toolhead.remove_extra_axis(self)
            self.axis_gcode_id = None
            self._clear_cancel_tracking("gcode_axis_unregistered")
            return
        if (len(gcode_axis) != 1 or not gcode_axis.isupper()
            or gcode_axis in "XYZEFN"):
            if not gcode_axis:
                # Request to unregister already unregistered axis
                return
            raise gcmd.error("Not a valid GCODE_AXIS")
        for ea in toolhead.get_extra_axes():
            if ea is not None and ea.get_axis_gcode_id() == gcode_axis:
                raise gcmd.error("Axis '%s' already registered" % (gcode_axis,))
        self.axis_gcode_id = gcode_axis
        self.instant_corner_v = instant_corner_v
        self.gaxis_limit_velocity = limit_velocity
        self.gaxis_limit_accel = limit_accel
        self._clear_cancel_tracking("gcode_axis_registered")
        toolhead.add_extra_axis(self, self.commanded_pos)
    def process_move(self, print_time, move, ea_index):
        axis_r = move.axes_r[ea_index]
        start_pos = move.start_pos[ea_index]
        accel = move.accel * axis_r
        start_v = move.start_v * axis_r
        cruise_v = move.cruise_v * axis_r
        self.trapq_append(self.trapq, print_time,
                          move.accel_t, move.cruise_t, move.decel_t,
                          start_pos, 0., 0.,
                          1., 0., 0.,
                          start_v, cruise_v, accel)
        self.commanded_pos = move.end_pos[ea_index]
    def check_move(self, move, ea_index):
        # Check move is in bounds
        movepos = move.end_pos[ea_index]
        if ((self.pos_min is not None and movepos < self.pos_min)
            or (self.pos_max is not None and movepos > self.pos_max)):
            raise move.move_error()
        # Check if need to limit maximum velocity and acceleration
        axis_ratio = move.move_d / abs(move.axes_d[ea_index])
        limit_velocity = self.gaxis_limit_velocity * axis_ratio
        limit_accel = self.gaxis_limit_accel * axis_ratio
        if not move.is_kinematic_move and self.accel:
            limit_accel = min(limit_accel, self.accel * axis_ratio)
        move.limit_speed(limit_velocity, limit_accel)
    def calc_junction(self, prev_move, move, ea_index):
        diff_r = move.axes_r[ea_index] - prev_move.axes_r[ea_index]
        if diff_r:
            return (self.instant_corner_v / abs(diff_r))**2
        return move.max_cruise_v2
    def get_axis_gcode_id(self):
        return self.axis_gcode_id
    def get_trapq(self):
        return self.trapq
    # Toolhead wrappers to support homing
    def flush_step_generation(self):
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.flush_step_generation()
    def get_position(self):
        return [self.commanded_pos, 0., 0., 0.]
    def set_position(self, newpos, homing_axes=""):
        self.do_set_position(newpos[0])
    def get_last_move_time(self):
        self.sync_print_time()
        return self.next_cmd_time
    def dwell(self, delay):
        self.next_cmd_time += max(0., delay)
    def drip_move(self, newpos, speed, drip_completion):
        # Submit move to trapq
        self.sync_print_time()
        start_time = self.next_cmd_time
        end_time = self._submit_move(start_time, newpos[0],
                                     speed, self.homing_accel)
        # Drip updates to motors
        self.motion_queuing.drip_update_time(start_time, end_time,
                                             drip_completion)
        # Clear trapq of any remaining parts of movement
        self.motion_queuing.wipe_trapq(self.trapq)
    def get_kinematics(self):
        return self
    def get_steppers(self):
        return self.steppers
    def calc_position(self, stepper_positions):
        return [stepper_positions[self.rail.get_name()], 0., 0.]

def load_config_prefix(config):
    return ManualStepper(config)
