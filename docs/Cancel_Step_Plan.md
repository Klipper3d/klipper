# CANCEL_STEP implementation plan

This document is a step-by-step plan for adding a `CANCEL_STEP STEPPER=<name>`
G-code command that interrupts an in-progress `MANUAL_STEPPER ... MOVE=...`
motion.

## Scope and constraints

In scope:

- `CANCEL_STEP STEPPER=<config_name>` cancels the *currently running* move on
  the named `[manual_stepper ...]`.
- Only required to work for moves issued with `SYNC=0` (so the G-code channel
  is free to receive `CANCEL_STEP`).
- Only required to work for the `MOVE=` branch of `MANUAL_STEPPER` (plain
  point-to-point moves).

Out of scope (explicitly rejected for now):

- Canceling a `STOP_ON_ENDSTOP` (homing) move — that already has its own
  trsync owned by `HomingMove`; reusing it from a cancel path would conflict.
- Canceling moves while the stepper is registered as a `GCODE_AXIS` extra
  axis — those moves are scheduled through the toolhead lookahead and share
  print time with XYZ.
- Canceling a move that was issued with default `SYNC=1` — the G-code mutex
  is held by the blocked `MANUAL_STEPPER` line and the cancel command cannot
  reach a handler on the same channel.
- Multi-MCU manual steppers (single MCU only for v1).

The command must **reject with an error** in the out-of-scope cases (rather
than silently doing the wrong thing), so the contract is unambiguous.

## Background — why "just clear the host buffer" is not enough

By the time the motor is physically moving, steps for the move live in:

- the manual stepper's host `trapq`,
- the host `stepcompress` queue,
- the host `serialqueue` (C pthread),
- and the **MCU**'s per-stepper `struct stepper.mq` and currently executing
  `stepper_event` timer.

The host runs ~200–700 ms ahead of real time, so dropping the host-side
buffers only cancels the unsent tail — the MCU keeps stepping until its
internal queue drains. To actually stop motion we have to tell the MCU to
stop.

Klipper already has the right primitive on the MCU: `trsync` +
`stepper_stop_on_trigger` + `stepper_stop` (used by homing). We will reuse
it.

## High-level design

1. Each `[manual_stepper]` gets a **dedicated `MCU_trsync`** allocated at
   startup, plus a `stepper_stop_on_trigger` link from each of its steppers
   to that trsync. This is one-time setup.
2. Each `do_move()` **arms** the trsync (`trsync_start` +
   `trsync_set_timeout`) for the duration of the move plus a slack margin,
   and tracks the trigger completion.
3. After the move finishes normally, the host **releases** the trsync
   (`trsync_trigger` with `REASON_HOST_REQUEST` so it does not actually stop
   anything, since the move is already done — same wrap-up homing uses).
4. `CANCEL_STEP` fires `trsync_trigger` immediately. On the MCU,
   `trsync_do_trigger` → `stepper_stop` runs in IRQ context:
   - kills the stepper timer,
   - clears its move queue,
   - sets `SF_NEED_RESET`.
5. After the trigger, the host cleans up its state to match the MCU:
   - finalize the trapq (`motion_queuing.wipe_trapq`),
   - reset stepcompress and re-query the actual MCU step position
     (`stepper.note_homing_end`),
   - recompute `commanded_pos` from the queried MCU position,
   - reset `next_cmd_time` so the next move is scheduled fresh.

The flow is essentially a "homing move with no endstop, and the trigger
source is the operator typing `CANCEL_STEP`".

## Files to modify

| File | Purpose of change |
|------|-------------------|
| `klippy/extras/manual_stepper.py` | Add cancel trsync, arm/disarm logic in `do_move`, register `CANCEL_STEP` mux command, cleanup helper |
| `docs/G-Codes.md` | Document `CANCEL_STEP` user-facing behavior |
| `docs/Manual_Stepper_Pipeline.md` | Add a section describing the cancel path |
| `test/klippy/manual_stepper.cfg` + `test/klippy/manual_stepper.test` | Add coverage for the new command (sanity only; cancel timing isn't reproducible in file-output mode, so most assertions are command-acceptance + state-after-cancel) |

No MCU C changes required — `trsync`, `stepper_stop_on_trigger`, and
`trsync_trigger` already exist in `src/trsync.c` and `src/stepper.c`.

## Detailed change list

### 1. `klippy/extras/manual_stepper.py`

#### 1a. Imports

- Already imports `stepper` and `force_move`. Add:
  ```python
  from . import mcu  # for MCU_trsync, TRSYNC_TIMEOUT
  ```
  (or `from mcu import MCU_trsync, TRSYNC_SINGLE_MCU_TIMEOUT` matching style
  of `homing.py` — pick whichever fits the file's existing import style).

#### 1b. `__init__` — allocate trsync and stop link

After the rail / steppers / trapq are set up and before `register_mux_command`:

- Determine the MCU of the stepper(s):
  ```python
  mcu_obj = self.steppers[0].get_mcu()
  ```
- Reject multi-MCU for v1:
  ```python
  if any(s.get_mcu() is not mcu_obj for s in self.steppers):
      raise config.error("CANCEL_STEP unsupported on multi-mcu manual_stepper")
  ```
  (Or downgrade to: still allow the manual stepper, but disable cancel by
  setting `self._cancel_trsync = None`. Simpler is to require single MCU.)
- Allocate the trsync:
  ```python
  ffi_main, ffi_lib = chelper.get_ffi()
  self._cancel_trdispatch = ffi_main.gc(
      ffi_lib.trdispatch_alloc(), ffi_lib.free)
  self._cancel_trsync = mcu.MCU_trsync(mcu_obj, self._cancel_trdispatch)
  for s in self.steppers:
      self._cancel_trsync.add_stepper(s)
  ```
  The `trdispatch`/`trsync` pair is the same shape `TriggerDispatch` builds
  for endstops; we replicate it here without the endstop part.
- Hook a config callback so `stepper_stop_on_trigger oid=... trsync_oid=...`
  gets sent at MCU connect for each stepper. The cleanest way is to piggy-back
  on `MCU_trsync`'s `_build_config` — that already happens through the MCU's
  `register_config_callback`. We then need to send `stepper_stop_on_trigger`
  once after MCU connect; do that in a `klippy:mcu_identify` or
  `klippy:connect` handler:
  ```python
  self.printer.register_event_handler("klippy:mcu_identify",
                                      self._build_cancel_links)
  ```
  with:
  ```python
  def _build_cancel_links(self):
      for s in self.steppers:
          self._cancel_trsync._stepper_stop_cmd.send(
              [s.get_oid(), self._cancel_trsync.get_oid()])
  ```
  Note: `MCU_trsync._stepper_stop_cmd` is currently private; either expose
  a small public method on `MCU_trsync` (preferred — call it
  `attach_stepper_stop(stepper_oid)`) or accept the private access and add a
  TODO.

#### 1c. Tracking variables

Add to `__init__`:

```python
self._cancel_completion = None
self._cancel_armed = False
```

#### 1d. `do_move` — arm trsync around the move

Modify `do_move(self, movepos, speed, accel, sync=True)`:

- Before submitting:
  ```python
  self.sync_print_time()
  start_time = self.next_cmd_time
  ```
- Submit the move (existing code) and get `end_time`:
  ```python
  end_time = self._submit_move(start_time, movepos, speed, accel)
  self.next_cmd_time = end_time
  ```
- **Arm the cancel trsync** for this move's duration:
  ```python
  reactor = self.printer.get_reactor()
  self._cancel_completion = reactor.completion()
  expire_timeout = (end_time - start_time) + mcu.TRSYNC_SINGLE_MCU_TIMEOUT
  self._cancel_trsync.start(start_time, 0., self._cancel_completion,
                            expire_timeout)
  self._cancel_armed = True
  ```
  `report_offset=0.` is fine because we only have one trsync.
- `note_mcu_movequeue_activity(end_time)` (unchanged).
- `if sync: self.sync_print_time()` (unchanged, but irrelevant for our use
  case where SYNC=0).
- Schedule a release. With SYNC=0 we don't naturally wait, so we have to
  arrange for the trsync to be disarmed after the move completes. Two
  options:
  - **Option A (preferred):** register a reactor callback for `end_time + slack`
    that calls `_release_cancel_trsync()`. The reactor is alive even when the
    G-code handler has returned.
  - **Option B:** rely on trsync's `expire_clock` to auto-trigger past
    `end_time`. This stops the motor at expire-time, which is fine because
    the move would already have finished — but it consumes a step queue
    reset (`SF_NEED_RESET`), which means the next move on this stepper has
    to go through the same re-query path. That's safe but wasteful.

  Use Option A. Pseudocode:
  ```python
  reactor = self.printer.get_reactor()
  waketime = reactor.monotonic() + (end_time
                                    - self._mcu_obj.estimated_print_time(
                                        reactor.monotonic()))
  reactor.register_callback(self._cancel_autorelease, waketime + 0.050)
  ```

#### 1e. `_release_cancel_trsync` and `_cancel_autorelease`

```python
def _release_cancel_trsync(self):
    if not self._cancel_armed:
        return
    res = self._cancel_trsync.stop()
    self._cancel_armed = False
    self._cancel_completion = None
    return res

def _cancel_autorelease(self, eventtime):
    # End-of-move auto release (move finished without cancel)
    try:
        if self._cancel_armed:
            # send a host-request trigger to release without stopping
            self._cancel_trsync._trsync_trigger_cmd.send(
                [self._cancel_trsync.get_oid(),
                 mcu.MCU_trsync.REASON_HOST_REQUEST])
            self._release_cancel_trsync()
    except Exception:
        logging.exception("manual_stepper cancel autorelease")
    return self.printer.get_reactor().NEVER
```

(The trigger reason is `HOST_REQUEST` so `stepper_stop` fires but the move
queue is already empty; this is the same finalize pattern homing uses.)

#### 1f. `cmd_CANCEL_STEP`

Register in `__init__`:

```python
gcode.register_mux_command('CANCEL_STEP', "STEPPER",
                           stepper_name, self.cmd_CANCEL_STEP,
                           desc=self.cmd_CANCEL_STEP_help)
```

Handler:

```python
cmd_CANCEL_STEP_help = "Cancel the current MANUAL_STEPPER move"
def cmd_CANCEL_STEP(self, gcmd):
    if self.axis_gcode_id is not None:
        raise gcmd.error("CANCEL_STEP not allowed while GCODE_AXIS active")
    if not self._cancel_armed:
        # Nothing to cancel; treat as no-op for idempotency
        gcmd.respond_info("No active MANUAL_STEPPER move to cancel")
        return
    # 1. Fire trigger -> MCU stepper_stop in IRQ context
    self._cancel_trsync._trsync_trigger_cmd.send(
        [self._cancel_trsync.get_oid(),
         mcu.MCU_trsync.REASON_HOST_REQUEST])
    # 2. Tear down trsync state on host
    self._release_cancel_trsync()
    # 3. Flush host motion state to reality
    self._cleanup_after_cancel()
    gcmd.respond_info("MANUAL_STEPPER %s move canceled"
                      % (self.name.split()[1],))
```

#### 1g. `_cleanup_after_cancel`

```python
def _cleanup_after_cancel(self):
    toolhead = self.printer.lookup_object('toolhead')
    toolhead.flush_step_generation()
    # Drop any remaining trapq segments to history
    self.motion_queuing.wipe_trapq(self.trapq)
    # Reset stepcompress + re-query MCU for actual stopped position
    for s in self.steppers:
        s.note_homing_end()
    # Re-derive commanded_pos from MCU position
    new_pos = self.steppers[0].get_commanded_position()
    self.commanded_pos = new_pos
    self.rail.set_position([self.commanded_pos, 0., 0.])
    # Future moves should schedule from "now", not from canceled future
    self.next_cmd_time = 0.
    self.sync_print_time()
```

`stepper.note_homing_end()` (in `klippy/stepper.py`) already does exactly the
right combination: `stepcompress_reset`, send `reset_step_clock oid=... clock=0`,
and `_query_mcu_position` to update host-side `_mcu_position_offset` from the
actual MCU position.

#### 1h. Guardrails

- In `cmd_MANUAL_STEPPER`, when `STOP_ON_ENDSTOP` is being handled, ensure we
  do **not** also arm `self._cancel_trsync` — homing arms a different trsync
  on the same `stop_signal`, and `trsync_add_signal` on the MCU shuts down
  if the signal is already active. The simplest fix is to only arm
  `_cancel_trsync` in `do_move`, never in `do_homing_move`.
- In `command_with_gcode_axis` (the GCODE_AXIS registration path), there are
  no `do_move` calls — extra-axis moves go through `process_move` invoked by
  the toolhead. We do not arm the cancel trsync there. `cmd_CANCEL_STEP`
  already rejects when `axis_gcode_id is not None`.
- In `do_homing_move`: after `phoming.manual_home` finishes, the homing
  trsync detaches; our cancel trsync was never armed. Nothing to do.

### 2. Documentation

#### 2a. `docs/G-Codes.md`

Add a subsection right after `MANUAL_STEPPER`:

```
#### CANCEL_STEP
`CANCEL_STEP STEPPER=config_name`: Cancel an in-progress
`MANUAL_STEPPER ... MOVE=...` motion on the named stepper. The move must
have been issued with `SYNC=0`. The stepper will halt as soon as the MCU
processes the cancel (typically within a few ms over USB). After the
cancel, the stepper's commanded position is updated to match where the
motor actually stopped, so subsequent moves are relative to the stop
location.

Restrictions:
* Has no effect if no move is in progress.
* Returns an error if the stepper is currently registered as a
  `GCODE_AXIS`.
* Does not interrupt `STOP_ON_ENDSTOP` (homing) moves.
* Does not interrupt moves issued with default `SYNC=1`, because the
  G-code channel is blocked until that move finishes.
```

#### 2b. `docs/Manual_Stepper_Pipeline.md`

Add a new section "11. CANCEL_STEP path" describing:

- Host flow: register mux command → arm trsync per do_move → cancel sends
  `trsync_trigger` → cleanup with `wipe_trapq` + `note_homing_end`.
- MCU flow: `trsync_do_trigger` → `stepper_stop` (IRQ context) → step timer
  killed, queue cleared, `SF_NEED_RESET`.
- Diagram showing the trsync as a parallel control channel that does not go
  through the move queue.
- Limitations matching G-Codes.md.

### 3. Tests

#### 3a. `test/klippy/manual_stepper.cfg`

No config-time change strictly required (the trsync is auto-created). If we
decide to make cancel opt-in via a config flag (see "Open questions"), add
that flag to the test config.

#### 3b. `test/klippy/manual_stepper.test`

Append a small smoke-test sequence — note that file-output mode runs without
real timing, so the goal is to confirm command-acceptance and host-state
consistency, not real-time stopping:

```
MANUAL_STEPPER STEPPER=basic_stepper ENABLE=1
MANUAL_STEPPER STEPPER=basic_stepper SET_POSITION=0
MANUAL_STEPPER STEPPER=basic_stepper MOVE=100 SPEED=10 ACCEL=200 SYNC=0
CANCEL_STEP STEPPER=basic_stepper
MANUAL_STEPPER STEPPER=basic_stepper MOVE=5 SPEED=10
MANUAL_STEPPER STEPPER=basic_stepper ENABLE=0
```

Expectations: no exceptions; second `MOVE=5` is from the post-cancel
position (whatever the test-mode MCU reports); `CANCEL_STEP` on an idle
stepper produces the no-op info message rather than an error.

Also add a negative test asserting that `CANCEL_STEP STEPPER=basic_stepper`
errors when the stepper has been registered as a `GCODE_AXIS`:

```
MANUAL_STEPPER STEPPER=basic_stepper GCODE_AXIS=A
CANCEL_STEP STEPPER=basic_stepper     ; expect error
MANUAL_STEPPER STEPPER=basic_stepper GCODE_AXIS=
```

(Use the existing test harness for expected-error patterns.)

## Edge cases and how they are handled

| Case | Behavior |
|------|----------|
| `CANCEL_STEP` while idle | `respond_info("No active ... move to cancel")`, no error |
| `CANCEL_STEP` mid-move (SYNC=0) | Triggers `stepper_stop`, cleans up host state, motor halts within a few ms |
| Cancel during `STOP_ON_ENDSTOP` move | We never armed our trsync; cancel command is a no-op (idle path) |
| Cancel while `GCODE_AXIS` is set | Hard error |
| Cancel called twice quickly | Second call sees `_cancel_armed == False`; idle path |
| Move completes naturally without cancel | `_cancel_autorelease` callback fires `trsync_trigger` with `HOST_REQUEST` and stops the trsync; next move can re-arm cleanly |
| Klippy disconnect / shutdown during armed move | `MCU_trsync._shutdown` already completes the trigger completion with False; our `_release_cancel_trsync` becomes safe to call from cleanup path |
| Multi-MCU manual stepper | Rejected at config time |
| Two manual steppers, cancel one | Each has its own trsync; independent |

## Risks and validations

- **Trsync slot collision with future homing**: a `stop_signal` slot per
  stepper is a single linked-list entry on the MCU. `stepper_stop_on_trigger`
  is called once at startup pointing at the cancel trsync. When homing
  starts, `HomingMove` calls `stepper_stop_on_trigger` again pointing at the
  homing trsync — and `trsync_add_signal` will `shutdown` if the signal is
  already active. Need to verify whether the cancel trsync's
  `trsync_clear()` (run at homing time via `trsync_start`) clears the
  per-stepper `tss->func` field. Looking at `trsync_clear`:
  ```text
  trsync_clear iterates ts->signals and sets each tss->func = NULL
  ```
  So when a trsync is re-`start`ed, it nulls out signal entries on the
  steppers it owns. But homing has a *different* trsync, so the cancel
  trsync's stale link on the stepper is what matters. The fix is: on the
  host, **do not** persistently call `stepper_stop_on_trigger` once at
  startup — do it inside `do_move`'s arm sequence, the same way
  `MCU_trsync.start()` does:
  ```text
  for s in self._steppers:
      self._stepper_stop_cmd.send([s.get_oid(), self._oid])
  ```
  In fact, `MCU_trsync.start()` already does that. So skip step 1b's
  `_build_cancel_links` event handler and let `MCU_trsync.start()` re-link
  on every arm. After homing finishes, homing's trsync has called `stop()`
  and its `trsync_clear()` cleared its stop signals; the next `do_move`
  call re-links our cancel trsync cleanly. This is the safer pattern and
  follows the homing precedent exactly.

  **Revision to 1b/1c**: drop the `klippy:mcu_identify` handler; the
  `stepper_stop_on_trigger` send happens automatically inside
  `self._cancel_trsync.start(...)`.

- **Cancel during the "release window" of a previous move**: there is a
  small interval where `_cancel_autorelease` is scheduled but hasn't run,
  yet the move has actually finished. If `CANCEL_STEP` is issued in that
  window, the `trsync_trigger` is harmless (no steps left) and the cleanup
  is also harmless (`wipe_trapq` on empty trapq, `note_homing_end` re-queries
  position — same as the autorelease path). Net: idempotent and safe.

- **TRSYNC_TIMEOUT vs long moves**: `TRSYNC_SINGLE_MCU_TIMEOUT = 0.250` is
  the *initial* expire window. The plan computes `expire_timeout = move_t +
  TRSYNC_SINGLE_MCU_TIMEOUT`. This must be passed as `expire_timeout` to
  `MCU_trsync.start`, which internally uses it for `expire_ticks` and the
  initial `trsync_set_timeout`. The `report_ticks` is set to 30% of that.
  For very long moves (e.g. 60s), report_ticks gets large — that's fine.
  Verify there is no host-side TRSYNC `expire_timeout` upper bound. (Spot
  check: `trdispatch_mcu_setup` accepts arbitrary uint32 ticks → up to
  ~71 s at 60 MHz. For longer moves, the autorelease callback runs first
  and re-arms or releases.)

- **trsync `report_ticks` extension**: `MCU_trsync._handle_trsync_state`
  extends the timeout based on `_home_end_clock`. For cancel we never set
  `_home_end_clock`, so the timeout will not auto-extend. If the move's
  duration is close to `expire_timeout`, the trsync could auto-fire near
  the end, stopping the motor a few ms early. Mitigation: pad
  `expire_timeout` more generously, e.g. `move_t + 1.0`. Alternatively, set
  `_home_end_clock` via the existing `set_home_end_time(end_time)` so the
  trsync extends itself — that is actually the cleaner option and matches
  homing's pattern.

  **Revision to 1d**: after `_cancel_trsync.start(...)`, call
  `self._cancel_trsync.set_home_end_time(end_time)`. Then the autorelease
  callback can be simpler: after `end_time` it sends
  `trsync_trigger(HOST_REQUEST)` and calls `stop()`.

- **Multiple steppers per rail**: `MCU_trsync.add_stepper` accepts multiple,
  and its `start()` issues `stepper_stop_on_trigger` for each. Already
  handled.

- **No active enable callback interaction**: stepper enable lines are
  tracked separately and don't need touching. The motor remains enabled
  after cancel (caller can `MANUAL_STEPPER ENABLE=0` if desired).

## Open questions (decide before implementation)

1. **Opt-in or always-on?** Always-on is simpler. The trsync overhead is
   one extra OID per manual stepper and one trsync_start per move. Likely
   negligible. Recommend always-on.
2. **Should `CANCEL_STEP` accept an `EMERGENCY=1` flag?** No — keep it
   simple. M112 already exists for hard stop.
3. **Naming**: the user proposed `CANCEL_STEP`. Slight alternative:
   `MANUAL_STEPPER_CANCEL` for naming consistency. Stick with the user's
   name unless they prefer the longer one.
4. **Multi-MCU**: reject at config-time vs. silently disable cancel?
   Rejecting is louder; silently disabling means `CANCEL_STEP` errors with
   a clearer reason later. Recommend silent disable + clear error message
   on `CANCEL_STEP` invocation.

## Implementation order (suggested commits)

1. Add `MCU_trsync` allocation + `add_stepper` in `ManualStepper.__init__`,
   no behavior change yet.
2. Add `_release_cancel_trsync`, `_cancel_autorelease`, and trsync arming
   in `do_move`. Verify normal moves still pass `test/klippy/manual_stepper.test`.
3. Add `CANCEL_STEP` mux command and `_cleanup_after_cancel`.
4. Update `docs/G-Codes.md` and `docs/Manual_Stepper_Pipeline.md`.
5. Add test cases.

Each step is independently reviewable and can be reverted without breaking
the others.

## Out-of-band: what *not* to change

- `src/stepper.c`, `src/trsync.c`, `src/endstop.c` — no MCU changes needed.
- `klippy/extras/homing.py` — independent path; do not entangle.
- `klippy/toolhead.py` — manual steppers run on their own trapq; cancel
  does not need to touch toolhead lookahead state.
- `klippy/stepper.py` — `note_homing_end` is reused as-is. Renaming for
  clarity (e.g. to `note_step_stop`) is tempting but would be a separate
  refactor.
