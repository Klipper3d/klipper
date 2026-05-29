# `CANCEL_STEP` — Full lifecycle, host plumbing, and MCU firmware changes

This document describes the complete end-to-end behavior of the
asynchronous `CANCEL_STEP` feature for `MANUAL_STEPPER` moves on the
Monster8 (STM32F407) board, after all of the iterative fixes that were
made on top of `docs/Cancel_Step_Plan.md` and
`docs/Cancel_Step_StateMachine_Plan.md`.

It covers:

1. The user-visible lifecycle (`MOVE` → `CANCEL_STEP` → next `MOVE`).
2. How `MANUAL_STEPPER` was modified to participate in the cancel state
   machine.
3. How the move command path was changed (host-side step pipeline,
   stepcompress, iterative solver, syncemitter).
4. Every change made to the MCU firmware (`src/stepper.c`).
5. The set of failure modes the design protects against and *why* the
   chosen anchors / drains avoid them.

The host code lives in:

- `klippy/extras/manual_stepper.py`
- `klippy/stepper.py` (`MCU_stepper` cancel helpers)
- `klippy/mcu.py` (`MCU_stepper_cancel` low-level command wrapper)
- `klippy/chelper/itersolve.{c,h}`
- `klippy/chelper/stepcompress.c`
- `klippy/chelper/steppersync.c`
- `klippy/chelper/__init__.py` (FFI declarations)

The MCU firmware change is contained in `src/stepper.c`.

---

## 1. User-visible behavior

### 1.1 Supported scenarios

`CANCEL_STEP STEPPER=<name>` is supported when:

- The stepper is configured as a `[manual_stepper <name>]`.
- A `MANUAL_STEPPER … MOVE=… SYNC=0` is currently in flight (the move
  was submitted asynchronously and the gcode session has already
  returned `ok`).
- The stepper is **not** currently registered as a `GCODE_AXIS` and is
  **not** in a `STOP_ON_ENDSTOP` homing move (these are out of scope
  for v1).

### 1.2 Possible responses

`cmd_CANCEL_STEP` returns one of these `respond_info` strings:

| Response          | Meaning                                                                                    |
| ----------------- | ------------------------------------------------------------------------------------------ |
| `cancel_pending`  | A cancel was already in progress, OR a new cancel was just queued and is now in flight.    |
| `already_idle`    | No async (`SYNC=0`) move was active; nothing to cancel.                                    |
| `reject_gcode_axis`| The stepper is currently a `GCODE_AXIS`; cancel via this command is not supported.        |

The actual *result* of the cancel (success vs. timeout) is reported via
log lines (`manual_stepper_cancel_result …`) and via a follow-up
gcode response from the controller (e.g. `CANCEL_STEP <name> cancel_timeout`)
because the API is non-blocking. After `cancel_pending` the controller
runs the protocol asynchronously and transitions through the states
described in §2.

### 1.3 End-to-end timeline (happy path)

```
t=0.000  MANUAL_STEPPER MOVE=800 SPEED=30 ACCEL=200 SYNC=0
         → host returns "ok" immediately, motor begins spinning
t=0.500  CANCEL_STEP STEPPER=…
         → host returns "cancel_pending"
         → motor is decelerated (forced stop) within ~5–10ms
t=0.605  Internal RECONCILE completes:
           - MCU's next_step_time is anchored 100ms in the future
           - Host stepcompress / iterative solver re-anchored
           - Logical position is reconciled to physical step count
t=0.620  MANUAL_STEPPER MOVE=800 SPEED=30 ACCEL=200 SYNC=0
         → host returns "ok"; motor begins spinning again
         (with a ~100ms dwell from the cancel anchor's safety guard)
```

The 100ms guard is the *only* visible pause; everything else is host /
MCU bookkeeping.

---

## 2. The cancel state machine

`ManualStepper` maintains a per-stepper cancel state. All transitions
are logged with `manual_stepper_cancel_transition` so the cancel can be
fully reconstructed from `klippy.log`.

```
                            ┌──────┐
                            │ IDLE │
                            └──┬───┘
            do_move(SYNC=0) ───┘
                            ┌─────────────┐
                            │ MOVE_ACTIVE │
                            └──┬──────────┘
       cmd_CANCEL_STEP ────────┘
                            ┌──────────────────┐
                            │ CANCEL_REQUESTED │
                            └──┬───────────────┘
              tick (host)  ────┘  send stepper_cancel_start
                            ┌──────────────┐
                            │ CANCEL_ARMED │
                            └──┬───────────┘
              tick           ──┘  send stepper_cancel_trigger
                            ┌──────────────────┐
                            │ CANCEL_TRIGGERED │
                            └──┬───────────────┘
              tick           ──┘  poll stepper_cancel_status
                                   until active=0 && can_reset=1
                            ┌──────────┐
                            │ DRAINING │
                            └──┬───────┘
              tick           ──┘  wipe_trapq + drain host pipeline
                            ┌───────────┐
                            │ RECONCILE │
                            └──┬────────┘
              tick           ──┘  send stepper_cancel_finish
                                   stepcompress_reset / itersolve reset
                                   query MCU position
                            ┌──────┐                ┌──────────────┐
                            │ DONE │                │ ERROR_TIMEOUT│
                            └──┬───┘                └─────┬────────┘
              tick           ──┘                          │
                            ┌──────┐                      │
                            │ IDLE │ ◄────────────────────┘
                            └──────┘
```

A token (`cancel_token`) is bumped each time `_cancel_request` runs so
that asynchronous responses from a stale cancel cannot affect a newer
cancel attempt.

### 2.1 IDLE → MOVE_ACTIVE

Triggered by `do_move(…, sync=False)` (i.e. a `SYNC=0` move). The
`active_move_id` is allocated from a monotonically increasing counter,
and `cancel_deadline` is set to the move's planned end print_time
(`self.next_cmd_time` after `_submit_move` returned).

`_mark_sync0_move_active` is **suppressed** if a cancel is currently
busy (`_is_cancel_busy()` returns true). This used to be a race source:
a stale `MOVE_ACTIVE` set during a cancel would confuse the next
`CANCEL_STEP`.

### 2.2 MOVE_ACTIVE → CANCEL_REQUESTED

Triggered by `cmd_CANCEL_STEP`. The G-code handler:

1. Rejects the request if a `GCODE_AXIS` is active.
2. Returns `cancel_pending` immediately if a cancel is already busy.
3. Calls `_refresh_cancel_activity()` which clears the move tracking if
   the planned deadline has already elapsed (`already_idle`).
4. Verifies MCU firmware support via `_do_protocol_cancel()`; if the
   firmware lacks the `stepper_cancel_*` commands it raises an error.
5. Bumps `cancel_token`, sets `_cancel_timeout_at = now + 2.000s`, and
   transitions to `CANCEL_REQUESTED`.
6. Schedules the reactor timer `_cancel_step_tick` to run immediately.

The G-code session now returns `cancel_pending` — the rest of the
protocol runs in the background via `_cancel_step_tick`.

### 2.3 CANCEL_REQUESTED → CANCEL_ARMED → CANCEL_TRIGGERED

`_cancel_step_tick` (a non-blocking reactor callback) sequences the
MCU commands:

- `CANCEL_REQUESTED`: send `stepper_cancel_start oid=N`. This sets
  `s->cancel_flags |= SCF_ARMED` and clears `cancel_reason` on the MCU
  but does **not** stop motion.
- `CANCEL_ARMED`: send `stepper_cancel_trigger oid=N`. The MCU then
  performs the actual stop (see §4).
- `CANCEL_TRIGGERED`: poll `stepper_cancel_status oid=N` every
  `_cancel_poll_interval = 5ms`. The MCU returns
  `active`, `can_reset`, `last_cancel_clock`, `reason`. The host
  proceeds only when `active == 0 && can_reset == 1`.

If the cancel does not reach `can_reset` within the 2-second timeout
window, the controller fires `_cancel_abort_timeout()` which transitions
to `ERROR_TIMEOUT`, logs `manual_stepper_cancel_result … cancel_timeout`,
and emits `CANCEL_STEP <name> cancel_timeout` (this is recoverable; the
printer is **not** shut down).

### 2.4 CANCEL_TRIGGERED → DRAINING

When the MCU says `active=0 && can_reset=1`, the host knows:

- The MCU stepper timer has been removed from the scheduler.
- The MCU's internal stepper move queue has been freed.
- `s->next_step_time = 0` and `SF_NEED_RESET` is set on the MCU.

The host now needs to clean up its **own** pipeline (everything from
the trapq down to the stepcompress / syncemitter / serialqueue) before
issuing the final `stepper_cancel_finish`. This is the `DRAINING` phase.

### 2.5 DRAINING → RECONCILE

Three operations happen here, in order:

1. `motion_queuing.wipe_trapq(self.trapq)` — moves every trapq entry
   for this manual stepper into the history list (so iterative solver
   walks of the live "moves" list become no-ops).
2. `mstepper.cancel_drain_host_pipeline()` — drains the host-side step
   pipeline. This is the critical fix that prevents stale steps from
   the canceled move from being emitted to the MCU after the cancel
   anchor has been moved (see §3.2).
3. Transition to `RECONCILE`.

### 2.6 RECONCILE → DONE

`_cancel_reconcile()` performs the host/MCU re-anchoring:

```python
est_print_time = mcu.estimated_print_time(reactor.monotonic())
safe_print_time = est_print_time + CANCEL_RESYNC_GUARD   # 100ms
safe_clock      = mcu.print_time_to_clock(safe_print_time)
finish, commanded_pos = mstepper.cancel_reconcile(safe_clock)
```

`MCU_stepper.cancel_reconcile(safe_clock)` then:

1. Sends `stepper_cancel_finish oid=N clock=safe_clock` (synchronous
   query; waits for `stepper_cancel_finish_state` response).
2. Calls `stepcompress_reset(stepqueue, safe_clock)` so the host's
   `last_step_clock` matches the MCU's freshly assigned
   `next_step_time`.
3. Sends `stepper_get_position oid=N` (synchronous) to read the actual
   physical step count after the MCU stop.
4. Calls `stepcompress_set_last_position(stepqueue, pos_clock, last_pos)`
   to mark the history.
5. Recomputes the *physical* logical position
   (`physical_pos = last_pos × step_dist − mcu_position_offset`) and
   calls `itersolve_set_position(sk, physical_pos, …)` to re-anchor the
   iterative solver to where the motor *actually* stopped (not where
   the canceled trapq entry would have ended).
6. Calls `itersolve_reset_flush_time(sk, clock_to_print_time(clock))`
   so the iterative solver's internal "already-flushed-up-to" pointer
   is rewound to the cancel anchor; otherwise the next move's early
   trapq slice would be skipped (see §3.4).
7. Emits the `stepper:sync_mcu_position` printer event so any other
   subscribers see the new position.

Back in `manual_stepper._cancel_reconcile`:

- `self.commanded_pos` is updated to the physical position.
- `self.rail.set_position([commanded_pos, 0., 0.])` is called to keep
  `mcu_position_offset` consistent.
- `self.next_cmd_time = safe_print_time` — this is what makes the next
  G-code MOVE start at the freshly anchored time, with a 100ms guard
  ahead of "now".
- A `manual_stepper_cancel_position` log line records the planning vs.
  physical position for diagnostics.
- State transitions: `RECONCILE → DONE → IDLE`.
- `manual_stepper_cancel_result … status=cancel_pending` is logged.

### 2.7 ERROR_TIMEOUT → IDLE

If the protocol does not advance within `_cancel_query_timeout`
(2.000s), `_cancel_abort_timeout` fires:

- Resets `active_move_id`, `cancel_deadline`, `_cancel_timeout_at`.
- Sets state to `ERROR_TIMEOUT`.
- Logs `manual_stepper_cancel_result … status=cancel_timeout`.
- Emits `CANCEL_STEP <name> cancel_timeout` over G-code.
- Transitions back to `IDLE` so future `CANCEL_STEP` requests can run.

The printer is **not** put into shutdown — the cancel just gives up,
and the user can SET_POSITION + retry.

---

## 3. Move command path changes (host)

### 3.1 `do_move` participates in the state machine

`ManualStepper.do_move(movepos, speed, accel, sync=True)` was extended
so that `SYNC=0` moves register themselves with the cancel controller:

```python
def do_move(self, movepos, speed, accel, sync=True):
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
```

Key details:

- `_mark_sync0_move_active()` is a no-op when a cancel is busy, so a
  stray late-arriving `MOVE_ACTIVE` cannot ever override a
  `CANCEL_REQUESTED` / `CANCEL_TRIGGERED` state.
- `cancel_deadline` is captured from `self.next_cmd_time` (the move's
  planned print_time end) so the next `CANCEL_STEP` knows when the
  move would naturally finish.
- A `SYNC=1` move always runs `_clear_cancel_tracking("sync_move")` —
  by the time `sync_print_time()` returns, the move has been flushed
  and is irrelevant to cancel tracking.

`MANUAL_STEPPER` (and the `GCODE_AXIS` registration path) implicitly
**wait** for any in-flight cancel to drain back to `IDLE` before
proceeding:

```python
def cmd_MANUAL_STEPPER(self, gcmd):
    self._await_cancel_complete(gcmd, "MANUAL_STEPPER")
    …
```

This is both a UX and a correctness fix. UX-wise it lets users issue

```
CANCEL_STEP STEPPER=back_left_drive
MANUAL_STEPPER STEPPER=back_left_drive MOVE=800 SYNC=0
```

back-to-back without inserting a polling step. (`M400` does **not**
help here — it waits for the toolhead's lookahead/movequeue to drain,
which is independent of the cancel state machine.)
Correctness-wise, allowing a fresh `MANUAL_STEPPER` (especially
`SET_POSITION=…`, which calls `_clear_cancel_tracking`) to race a
mid-protocol cancel would silently abort the cancel before
`stepper_cancel_finish` was sent, leaving the MCU stuck in
`NEED_RESET`.

`_await_cancel_complete` polls `_is_cancel_busy()` while yielding to
the reactor (`reactor.pause`) so the cancel timer continues to run.
Two constants govern its behavior:

| Constant                     | Value | Meaning                                                     |
| ---------------------------- | ----- | ----------------------------------------------------------- |
| `CANCEL_DRAIN_WAIT_TIMEOUT`  | 4.0s  | Hard upper bound; raises `<action> timed out waiting for cancel to complete`. |
| `CANCEL_DRAIN_POLL`          | 0.010s| Reactor sleep increment between busy checks.                |

The actual wait is short (~30–50 ms protocol + `CANCEL_RESYNC_GUARD`
of 100 ms is paid once across both the cancel reconcile and the next
move's first step).

### 3.2 New helper: `MCU_stepper.cancel_drain_host_pipeline()`

Prior to RECONCILE, the host-side pipeline still contains state
generated for the canceled move. There are **two** distinct buffers
that must be cleared:

1. **Stepcompress internal pending state**:
   - `sc->next_step_clock` — the single rollback-capable pending step
     used by the step+dir+step filter.
   - `sc->queue[]` — committed but un-flushed compressed steps.
2. **Syncemitter `msg_queue`** — encoded `queue_step` /
   `set_next_step_dir` messages that have been emitted by stepcompress
   but not yet handed to the serialqueue's send path.

`cancel_drain_host_pipeline()` (added on `MCU_stepper`) calls two new
C helpers (described in §3.5–3.6):

```python
def cancel_drain_host_pipeline(self):
    ffi_main, ffi_lib = chelper.get_ffi()
    ffi_lib.stepcompress_discard_pending(self._stepqueue)
    ffi_lib.syncemitter_drop_pending_msgs(self._syncemitter)
```

Without this drain, the failure mode is:

> A background flush runs *during* `stepper_cancel_finish`'s synchronous
> wait. With trapq empty, no new steps are generated, but stepcompress
> still has buffered output from before `wipe_trapq`. Those buffered
> messages get dispatched to the MCU after `cancel_finish` set
> `next_step_time = safe_clock`, so MCU computes
> `step_clock = safe_clock + interval` instead of
> `step_clock = OLD_C + interval`. Because `safe_clock ≪ OLD_C`, the
> resulting step is hundreds of milliseconds in the past →
> `MCU 'mcu' shutdown: Timer too close`.

Calling the drain *before* sending `cancel_finish` removes both the
pending stepcompress state and the queued msgs, so the BG flush during
the `cancel_finish` wait is a no-op for this stepper.

### 3.3 `MCU_stepper.cancel_reconcile(clock)` extended

`cancel_reconcile` now does much more than it originally did. The full
sequence is:

1. `cancel_finish(clock)` → MCU sets `s->next_step_time = clock`.
2. `stepcompress_reset(stepqueue, clock)` → host `last_step_clock` is
   re-anchored to the same value the MCU received. This makes the next
   `queue_step` interval a positive number relative to the new anchor.
3. Synchronous `stepper_get_position` query.
4. `stepcompress_set_last_position(stepqueue, pos_clock, last_pos)` to
   register the position in stepcompress history.
5. Compute `physical_pos = last_pos * step_dist - mcu_position_offset`
   and call `itersolve_set_position(sk, physical_pos, 0., 0.)`. This is
   the fix for "next move silently produces no motion": before this
   change the iterative solver kept `commanded_pos` at the *planned*
   end of the canceled move (e.g. 800mm), so the next `MOVE=800` saw
   `dist = 800 - 800 = 0` and produced no steps.
6. `itersolve_reset_flush_time(sk, clock_to_print_time(clock))` (see
   §3.4).
7. Send a `stepper:sync_mcu_position` event.

### 3.4 New C helper: `itersolve_reset_flush_time()`

The iterative solver caches an "already flushed up to" print_time
pointer (`sk->last_flush_time`). During normal motion this advances
monotonically as the background flush handler walks the trapq. When a
move is canceled we need to *rewind* that pointer to the cancel
anchor; otherwise:

> The first `MOVE=800` made the BG flush handler advance
> `sk->last_flush_time` to ~68.7s (end of the planned 26.6s move).
> `wipe_trapq` removes the trapq entry but `sk->last_flush_time` is
> untouched. The next `MOVE=800` appends a trapq entry at
> `[43.0, 69.6]`. Inside `itersolve_generate_steps`,
> `gen_steps_range(m, last_flush_time=68.67, flush_time=69.6)` only
> emits steps for the **last 0.93s** of the 26.6s move — about 28mm of
> motion, then nothing. The print_time bumps from 43.0 → 43.057, the
> buffer drains, and the host believes the move is done.

`itersolve_reset_flush_time(sk, flush_print_time)` resets both
`sk->last_flush_time` and `sk->last_move_time` so the new move is in
the future of the solver's pointer and step generation walks the full
trapq entry from its start.

```c
void __visible
itersolve_reset_flush_time(struct stepper_kinematics *sk, double flush_time)
{
    sk->last_flush_time = flush_time;
    sk->last_move_time  = flush_time;
}
```

### 3.5 New C helper: `stepcompress_discard_pending()`

```c
void __visible
stepcompress_discard_pending(struct stepcompress *sc)
{
    sc->next_step_clock = 0;
    sc->next_step_dir   = sc->sdir < 0 ? 0 : sc->sdir;
    sc->queue_pos = sc->queue_next = sc->queue;
}
```

Drops the pending rollback-capable step **and** clears the compressed
queue without flushing anything to the message queue. Unlike
`stepcompress_reset` (which calls `stepcompress_flush(sc, UINT64_MAX)`
first and *does* emit pending content), this is purely a discard
operation. `stepcompress_reset(sc, safe_clock)` is still called
afterward (during reconcile) to set `last_step_clock = safe_clock` and
`sdir = -1`, but at that point the internal flush is a no-op.

### 3.6 New C helper: `syncemitter_drop_pending_msgs()`

```c
void __visible
syncemitter_drop_pending_msgs(struct syncemitter *se)
{
    message_queue_free(&se->msg_queue);
}
```

Drains the syncemitter's pending message list. Any encoded
`queue_step` / `set_next_step_dir` commands sitting in
`se->msg_queue` (that have not yet been moved to a `command_queue`'s
upcoming list by `steppersync_flush`) are freed.

### 3.7 Why a 100 ms guard? (`CANCEL_RESYNC_GUARD`)

The MCU's `stepper_cancel_finish` sets `s->next_step_time = clock`,
where `clock` is the value supplied by the host. The very next
`queue_step` that the MCU receives must have an interval strictly
greater than the time elapsed since `clock` — otherwise the MCU
schedules a step in the past and shuts down with `Timer too close`.

If the host anchors at exactly `est_print_time` (i.e. "now"), there is
no margin for:

- Host → MCU serial latency.
- The reactor returning from the synchronous `cancel_finish` query.
- The next G-code's processing (HTTP roundtrip, gcode parser,
  `_submit_move`, `note_mcu_movequeue_activity`).
- Step compression and serialqueue scheduling.

`CANCEL_RESYNC_GUARD = 0.100` (100ms) matches Klipper's existing
`MIN_KIN_TIME` constant used by toolhead/homing re-sync paths and
provides a comfortable margin for all of the above. The user-visible
effect is a ~100ms dwell between `CANCEL_STEP` and the start of the
next motion.

```python
CANCEL_RESYNC_GUARD = 0.100

est_print_time = mcu.estimated_print_time(self.reactor.monotonic())
safe_print_time = est_print_time + CANCEL_RESYNC_GUARD
safe_clock      = mcu.print_time_to_clock(safe_print_time)
```

Both the MCU's `next_step_time` and the host's `last_step_clock` and
`next_cmd_time` are anchored to this single `safe_print_time` value,
keeping host and MCU clock domains coherent.

### 3.8 `do_set_position` re-anchoring

`do_set_position` was extended to also re-anchor `next_cmd_time` to
`toolhead.get_last_move_time()` and to clear cancel tracking. Without
this, a `SET_POSITION` issued after a stale (already-completed) async
move would force the next `MOVE` to dwell until the *previous* move's
planned end time:

```python
def do_set_position(self, setpos):
    toolhead = self.printer.lookup_object('toolhead')
    toolhead.flush_step_generation()
    self.next_cmd_time = toolhead.get_last_move_time()
    self._clear_cancel_tracking("set_position")
    self.commanded_pos = setpos
    self.rail.set_position([self.commanded_pos, 0., 0.])
```

---

## 4. MCU firmware (`src/stepper.c`) changes

The firmware additions are limited to one source file and two new
fields in `struct stepper`. They form a small, IRQ-safe state machine
that pairs 1:1 with the host controller.

### 4.1 New per-stepper fields

```c
struct stepper {
    …
    uint32_t cancel_clock;        // MCU clock at cancel_trigger time
    …
    uint8_t  cancel_flags : 8;    // SCF_ARMED
    uint8_t  cancel_reason : 8;   // SCR_NONE / SCR_HOST_REQUEST
};

enum { SCF_ARMED = 1<<0 };
enum { SCR_NONE = 0, SCR_HOST_REQUEST = 1 };
```

`cancel_clock` records the MCU timestamp at the moment the cancel
trigger fired (used by the host for diagnostics).

### 4.2 New commands

Four new `DECL_COMMAND` entries were added:

```c
DECL_COMMAND(command_stepper_cancel_start,
             "stepper_cancel_start oid=%c");
DECL_COMMAND(command_stepper_cancel_trigger,
             "stepper_cancel_trigger oid=%c");
DECL_COMMAND(command_stepper_cancel_status,
             "stepper_cancel_status oid=%c");
DECL_COMMAND(command_stepper_cancel_finish,
             "stepper_cancel_finish oid=%c clock=%u");
```

And two response messages:

```
stepper_cancel_state oid=%c active=%c can_reset=%c \
    last_cancel_clock=%u reason=%c
stepper_cancel_finish_state oid=%c ok=%c active=%c can_reset=%c \
    last_cancel_clock=%u reason=%c
```

#### `command_stepper_cancel_start`

Arms the stepper for cancellation but does **not** stop motion.
Crucially, it clears any prior `cancel_reason` so the host can
distinguish a fresh cancel from a stale one.

```c
void
command_stepper_cancel_start(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    irq_disable();
    s->cancel_flags |= SCF_ARMED;
    s->cancel_reason = SCR_NONE;
    irq_enable();
}
```

#### `command_stepper_cancel_trigger`

Performs the actual stop, but only if `SCF_ARMED` was set. This
two-step (`start` → `trigger`) handshake exists because the host's
`CANCEL_REQUESTED → CANCEL_ARMED → CANCEL_TRIGGERED` transitions are
separated by reactor ticks; `start` alone is harmless if for some
reason `trigger` never arrives (the cancel just times out from the
host side).

```c
void
command_stepper_cancel_trigger(uint32_t *args)
{
    struct stepper *s = stepper_oid_lookup(args[0]);
    irq_disable();
    if (s->cancel_flags & SCF_ARMED) {
        s->cancel_clock  = timer_read_time();
        s->cancel_reason = SCR_HOST_REQUEST;
        s->cancel_flags &= ~SCF_ARMED;
        stepper_stop(&s->stop_signal, s->cancel_reason);
    }
    irq_enable();
}
```

`stepper_stop` (§4.3) is the existing function used for homing
trsync stops. By reusing it, cancel inherits all of its IRQ-safety
guarantees and queue-cleanup logic.

#### `command_stepper_cancel_status`

A pure query. Reports:

- `active = !!s->count` — true if the timer is still scheduled.
- `can_reset = !active && !!(s->flags & SF_NEED_RESET)` — true only
  when the stop has fully settled and a clock reset is safe.
- `last_cancel_clock` — the MCU clock recorded at trigger time.
- `reason` — `SCR_NONE` (no cancel happened) or `SCR_HOST_REQUEST`.

```c
void
command_stepper_cancel_status(uint32_t *args)
{
    uint8_t oid = args[0];
    struct stepper *s = stepper_oid_lookup(oid);
    irq_disable();
    uint8_t  active           = !!s->count;
    uint8_t  can_reset        = !active && !!(s->flags & SF_NEED_RESET);
    uint32_t cancel_clock     = s->cancel_clock;
    uint8_t  cancel_reason    = s->cancel_reason;
    irq_enable();
    sendf("stepper_cancel_state oid=%c active=%c can_reset=%c"
          " last_cancel_clock=%u reason=%c",
          oid, active, can_reset, cancel_clock, cancel_reason);
}
```

The host polls this every 5ms while in `CANCEL_TRIGGERED` and
advances to `DRAINING` only when `active=0 && can_reset=1`. This
explicit handshake avoids the prior race where the host would race
ahead and issue a `reset_step_clock` while the MCU stepper was still
mid-stop, triggering `Can't reset time when stepper active`.

#### `command_stepper_cancel_finish`

Performs the actual `next_step_time` reset (replacing the old
`reset_step_clock` path during cancel) **only if** `can_reset` is
still true. This double-check protects against the host issuing
`finish` without first observing a clean status.

```c
void
command_stepper_cancel_finish(uint32_t *args)
{
    uint8_t  oid      = args[0];
    uint32_t waketime = args[1];
    struct stepper *s = stepper_oid_lookup(oid);
    irq_disable();
    uint8_t active    = !!s->count;
    uint8_t can_reset = !active && !!(s->flags & SF_NEED_RESET);
    uint8_t ok = 0;
    if (can_reset) {
        s->next_step_time = s->time.waketime = waketime;
        s->flags &= ~SF_NEED_RESET;
        ok = 1;
    }
    uint32_t cancel_clock  = s->cancel_clock;
    uint8_t  cancel_reason = s->cancel_reason;
    irq_enable();
    sendf("stepper_cancel_finish_state oid=%c ok=%c active=%c can_reset=%c"
          " last_cancel_clock=%u reason=%c",
          oid, ok, active, can_reset, cancel_clock, cancel_reason);
}
```

If `ok == 0`, the host treats the finish as having failed and either
keeps polling (when `active=1` or `can_reset=0`) or surfaces a timeout
to the user.

### 4.3 No structural changes to `stepper_stop`

`stepper_stop` was **not** modified — cancel uses it as-is via the
existing `trsync_signal` infrastructure. Inside `stepper_stop`:

- The stepper timer is removed from the scheduler:
  `sched_del_timer(&s->time);`
- `s->next_step_time = s->time.waketime = 0;`
- `s->position = -stepper_get_position(s);` — accurately captures the
  *physical* step count (handles the mid-move case using `s->count`).
- `s->count = 0;` and `s->flags |= SF_NEED_RESET`.
- The direction pin is brought low and the step pin returned to its
  unstep state.
- The MCU's pending `stepper_move` queue (`s->mq`) is fully drained:

  ```c
  while (!move_queue_empty(&s->mq)) {
      struct move_node *mn = move_queue_pop(&s->mq);
      struct stepper_move *m = container_of(mn, struct stepper_move, node);
      move_free(m);
  }
  ```

This last point is what makes the MCU side "atomic" from the host's
perspective: by the time `cancel_status` reports `active=0`, every
single move that had been queued for that stepper before
`cancel_trigger` has been freed. Anything that arrives after
`cancel_trigger` is treated as fresh input that the host is fully
responsible for (which is why §3.2's host-side drain matters).

### 4.4 No new shutdown reasons

The cancel commands intentionally cannot trigger `shutdown(...)` on
their own — `stepper_cancel_finish` returns `ok=0` rather than
shutting down on a bad state. The pre-existing
`Can't reset time when stepper active` shutdown in the standard
`command_reset_step_clock` path still applies if some other code path
misuses it, but cancel does not go through `reset_step_clock`.

---

## 5. Failure modes the design protects against

| Symptom (pre-fix)                                          | Root cause                                                                                   | Fix                                                                                                       |
| ---------------------------------------------------------- | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| `MCU 'mcu' shutdown: Can't reset time when stepper active` | Host issued `reset_step_clock` before stepper queue had drained on the MCU.                  | Two-phase `cancel_status` poll until `active=0 && can_reset=1`; finish only allowed when `can_reset==1`.  |
| `MCU 'mcu' shutdown: Timer too close` after cancel         | Stale `queue_step` messages from before `wipe_trapq` reached the MCU after `cancel_finish` reset `next_step_time`. | `cancel_drain_host_pipeline()` (`stepcompress_discard_pending` + `syncemitter_drop_pending_msgs`) before reconcile. |
| Same `Timer too close` even with the drain                 | Anchor was at `est_print_time` (i.e. "now"); message latency made the next move's first step land in the past. | `CANCEL_RESYNC_GUARD = 0.100s` future-bias on `safe_print_time`.                                          |
| `Timer too close` when the next `MOVE` is delayed (chained `ENABLE=0` / `ENABLE=1` / unrelated commands between `CANCEL_STEP` and the new `MOVE`) | `safe_clock` was committed at *cancel time*; arbitrary wall-clock latency until the user's next `MOVE` consumes the 100 ms guard, so the first new `queue_step`'s absolute step time (`safe_clock + interval`) is in the past on the MCU. | After reconcile, set `_needs_step_clock_reanchor`; in `do_move`, call `MCU_stepper.reanchor_step_clock(safe_clock_new, safe_print_time_new)` to (a) `stepcompress_reset` host last_step_clock, (b) queue `reset_step_clock oid=… clock=…` via `syncemitter_queue_msg(req_clock=0)` so it transmits ahead of the new `queue_step`s, and (c) `itersolve_reset_flush_time` to the new anchor. |
| Next `MANUAL_STEPPER MOVE` returns ok but motor doesn't move | `commanded_pos` and the iterative solver still pointed at the planned end of the canceled move; `dist = 0`. | Read MCU's actual step count, recompute physical position, call `itersolve_set_position` with that value. |
| Next move only steps for ~1s of a 26s motion              | `sk->last_flush_time` still pointed past the new move's start print_time, so itersolve only emitted the tail slice. | New `itersolve_reset_flush_time()` rewinds the solver pointer to the cancel anchor.                       |
| `stepcompress … Invalid sequence`                         | `next_cmd_time` was held at the canceled move's planned end (in the future), so the next move's first step had a clock < `last_step_clock`. | `next_cmd_time = safe_print_time` after reconcile, anchored to the same epoch as the MCU reset.           |
| `cancel_pending` returned but motor never stopped         | Cancel state machine was bypassed by a concurrent `MOVE_ACTIVE` set during `do_move`.        | `_mark_sync0_move_active` is suppressed when `_is_cancel_busy()` is true; `_await_cancel_complete` makes new `MANUAL_STEPPER` commands wait for any in-flight cancel before proceeding. |
| `MANUAL_STEPPER not allowed while cancel is in progress` returned for `CANCEL_STEP` + `MANUAL_STEPPER MOVE` chain | Initial implementation rejected new `MANUAL_STEPPER` commands during cancel. `M400` does not help — the cancel state machine runs on a reactor timer, not the toolhead lookahead queue. | `_await_cancel_complete` blocks (yielding to the reactor) up to 4s for cancel to finish, so chained commands "just work". |
| Mid-protocol cancel silently aborted by racing `SET_POSITION` | `do_set_position` calls `_clear_cancel_tracking` which forced state to `IDLE`, leaving the MCU in `NEED_RESET` because `stepper_cancel_finish` was never sent. | Same wait — `MANUAL_STEPPER SET_POSITION=…` now waits for cancel to fully complete before running, so `_clear_cancel_tracking` is a no-op by the time it fires. |
| Hang (`Unable to obtain 'trsync_state' response`)         | Direct trsync usage during high-traffic step generation.                                     | Cancel uses dedicated `stepper_cancel_*` query commands on a separate command queue; no trsync involvement. |

---

## 6. Diagnostics

Every transition is logged so a `klippy.log` capture is enough to
reconstruct any cancel:

- `manual_stepper_cancel_transition stepper=… old=X new=Y token=… move_id=… deadline=… reason=…`
- `manual_stepper_cancel_position stepper=… planning_pos=… physical_pos=… token=…`
- `manual_stepper_cancel_result stepper=… status={cancel_pending|cancel_timeout} token=… move_id=… deadline=…`

Useful greps:

```bash
grep manual_stepper_cancel_ /home/swervepi/printer_data/logs/klippy.log
grep "Timer too close\|Invalid sequence\|Can't reset time" \
     /home/swervepi/printer_data/logs/klippy.log
```

The stress harness `scripts/test_cancel_step_moonraker.py` exercises
the full lifecycle over Moonraker and is the canonical regression
test.

---

## 7. Files changed (summary)

### Host (Python)

- `klippy/extras/manual_stepper.py`
  - Added `CANCEL_RESYNC_GUARD` constant.
  - Added cancel state machine: `cancel_state`, `cancel_token`,
    `active_move_id`, `cancel_deadline`, `_cancel_timeout_at`,
    `_cancel_timer`.
  - Added `_set_cancel_state`, `_clear_cancel_tracking`,
    `_mark_sync0_move_active`, `_refresh_cancel_activity`,
    `_get_cancel_stepper`, `_do_protocol_cancel`, `_cancel_request`,
    `_is_cancel_busy`, `_reject_if_cancel_busy`, `_cancel_reconcile`,
    `_cancel_abort_timeout`, `_cancel_step_tick`.
  - Added the `CANCEL_STEP STEPPER=<name>` G-code (`cmd_CANCEL_STEP`).
  - `do_move` now calls `_mark_sync0_move_active` for `SYNC=0` moves
    and clears tracking for `SYNC=1` moves.
  - `do_set_position` re-anchors `next_cmd_time` and clears cancel
    tracking.
  - `do_enable(False)` clears cancel tracking.
  - Added `_await_cancel_complete` — `cmd_MANUAL_STEPPER` and
    `command_with_gcode_axis` now block (yielding to the reactor)
    until any in-flight cancel drains, instead of rejecting. This
    enables back-to-back `CANCEL_STEP` + `MANUAL_STEPPER` chains and
    prevents a mid-protocol abort from a racing `SET_POSITION`.
  - Drain of host pipeline (`cancel_drain_host_pipeline`) is invoked
    in `DRAINING`.
  - Added `_needs_step_clock_reanchor` flag, set in `_cancel_reconcile`
    and consumed by `_reanchor_step_clock_if_needed()`. `do_move` calls
    the helper before `_submit_move` so the MCU's `s->next_step_time`
    is refreshed *just before* the next move's first `queue_step` is
    generated, regardless of how long the user delayed the next motion.

- `klippy/stepper.py` (`MCU_stepper`)
  - Added `_cancel_ctl` (`MCU_stepper_cancel`) plumbing.
  - Added `is_cancel_protocol_supported`, `cancel_start`,
    `cancel_trigger`, `cancel_status`, `cancel_finish`.
  - Added `cancel_reconcile(clock)` — the host-side "tie everything
    together" routine.
  - Added `cancel_drain_host_pipeline()` — discards pending
    stepcompress + syncemitter state.
  - Added `reanchor_step_clock(clock, print_time)` — pairs a
    `stepcompress_reset` with a `reset_step_clock` MCU command queued
    at `req_clock=0` (mirrors `note_homing_end()`), and rewinds
    `itersolve` to `print_time`. Used to refresh the cancel anchor
    immediately before the next move emits its first `queue_step`.

- `klippy/mcu.py`
  - Added `MCU_stepper_cancel` class wrapping the four MCU commands
    and the two response message templates; checks for firmware
    support during `_build_config`.

### Host (C)

- `klippy/chelper/itersolve.c`, `klippy/chelper/itersolve.h`
  - Added `itersolve_reset_flush_time(sk, flush_time)` — rewinds
    `sk->last_flush_time` and `sk->last_move_time`.

- `klippy/chelper/stepcompress.c`
  - Added `stepcompress_discard_pending(sc)` — clears
    `sc->next_step_clock`, `sc->next_step_dir`, and the internal
    `sc->queue[]` *without* flushing to the message queue.

- `klippy/chelper/steppersync.c`
  - Added `syncemitter_drop_pending_msgs(se)` — frees every encoded
    message currently queued in `se->msg_queue`.

- `klippy/chelper/__init__.py`
  - FFI declarations for the three new C helpers.

### MCU firmware

- `src/stepper.c`
  - Extended `struct stepper` with `cancel_clock`, `cancel_flags`,
    `cancel_reason`.
  - Added `command_stepper_cancel_start`,
    `command_stepper_cancel_trigger`,
    `command_stepper_cancel_status`,
    `command_stepper_cancel_finish`.
  - No changes to the pre-existing `command_reset_step_clock` or
    `stepper_stop` paths — cancel reuses `stepper_stop` and
    introduces its own `cancel_finish`-based reset path with
    `ok=0` failure semantics.

### Documentation

- `docs/Cancel_Step_Lifecycle.md` (this document) — full lifecycle
  reference.
- `docs/Cancel_Step_StateMachine_Plan.md` — original design doc that
  this implementation follows.
- `docs/Cancel_Step_Plan.md` — earlier exploratory design (kept for
  history).
