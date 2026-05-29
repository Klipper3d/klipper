# CANCEL_STEP state-machine plan (host + MCU)

This document replaces the previous ad-hoc cancel approach with a strict
control-path design intended to avoid:

- MCU shutdowns such as `Timer too close`
- MCU shutdowns such as `Can't reset time when stepper active`
- request/response hangs such as `Unable to obtain 'trsync_state' response`

The key change is to treat cancel as a coordinated state machine across host
and MCU, instead of issuing immediate stop/reset commands inline in a single
G-code handler.

## Goals

- Implement `CANCEL_STEP STEPPER=<name>` for manual steppers running:
  - `MANUAL_STEPPER ... MOVE=... SYNC=0`
  - manual mode only (not `GCODE_AXIS`)
- Guarantee that cancel does **not** put Klipper into shutdown state.
- Guarantee deterministic command outcomes:
  - `canceled`
  - `already_idle`
  - `cancel_pending`
  - `cancel_timeout` (recoverable, not shutdown)

## Non-goals (v1)

- Canceling `STOP_ON_ENDSTOP` homing moves.
- Canceling extra-axis (`GCODE_AXIS`) motion.
- Canceling `SYNC=1` moves from the same gcode stream.
- Multi-MCU shared-axis semantics.

## Root cause summary from failed prototypes

The failed prototypes attempted to:

- trigger trsync inline while step generation traffic was active,
- query/stop trsync synchronously in a single request path,
- and/or reset step clock while stepper activity might still exist.

This produced unsafe ordering:

- `trsync_start/trsync_trigger` could be scheduled too close to current MCU
  time under real serial jitter/load.
- `reset_step_clock` could be sent while `s->count != 0`, causing firmware
  shutdown (`Can't reset time when stepper active`).

## New architecture

### 1) Host side (`klippy`) adds a per-stepper cancel controller

Each `ManualStepper` gets a `CancelController` object with explicit states:

- `IDLE`
- `MOVE_ACTIVE`
- `CANCEL_REQUESTED`
- `CANCEL_ARMED`
- `CANCEL_TRIGGERED`
- `DRAINING`
- `RECONCILE`
- `DONE`
- `ERROR_TIMEOUT`

The controller runs via reactor callbacks (non-blocking) and never performs
unsafe MCU operations in the direct request handler.

### 2) MCU firmware adds explicit cancel-safe primitives

Add dedicated MCU commands for safe stop/reconcile, instead of overloading
homing-oriented trsync paths:

- `stepper_cancel_start oid=%c`
  - arms cancel mode for that stepper
  - links stop signal and records cancellation epoch
- `stepper_cancel_trigger oid=%c`
  - stops active stepping immediately in IRQ-safe way
  - clears pending move queue
  - records canceled-at-clock and reason
- `stepper_cancel_status oid=%c` -> response:
  - `active=%c` (whether timer is still active)
  - `can_reset=%c` (safe to issue reset clock)
  - `last_cancel_clock=%u`
  - `last_reason=%c`
- `stepper_cancel_finish oid=%c clock=%u`
  - safe, explicit finalize/reset point; rejects if not `can_reset`

This separates:

- stop signal path (real-time),
- status polling path,
- reset/finalize path.

No `lookup_query_command(trsync_trigger -> trsync_state)` coupling is needed
for cancel in this design.

## Host control-path sequence

### A) Move path (`MANUAL_STEPPER ... MOVE ... SYNC=0`)

1. Host schedules move as today (`trapq` -> `stepcompress` -> serialqueue).
2. Controller sets state `MOVE_ACTIVE`.
3. Controller stores:
   - `move_id`
   - `expected_end_time`
   - `stepper_oid`
   - `cancel_token`

### B) Cancel request path (`CANCEL_STEP`)

Request handler does only lightweight checks and state transition:

1. Reject if:
   - `axis_gcode_id is not None`
   - not in `MOVE_ACTIVE`
2. If active:
   - set `CANCEL_REQUESTED`
   - enqueue controller callback now
   - return immediate `respond_info("cancel_pending")`

No synchronous wait for MCU query in request handler.

### C) Controller background sequence

1. `CANCEL_REQUESTED`:
   - send `stepper_cancel_start oid`
   - advance -> `CANCEL_ARMED`
2. `CANCEL_ARMED`:
   - send `stepper_cancel_trigger oid`
   - advance -> `CANCEL_TRIGGERED`
3. `CANCEL_TRIGGERED`:
   - periodically query `stepper_cancel_status`
   - once `active=0` and `can_reset=1` -> `DRAINING`
4. `DRAINING`:
   - call `motion_queuing.wipe_trapq(self.trapq)`
   - flush step generation
   - advance -> `RECONCILE`
5. `RECONCILE`:
   - send `stepper_cancel_finish oid clock=<safe_clock>`
   - query stepper position
   - update commanded position and local timing fields
   - clear pending token
   - `DONE` -> `IDLE`

### D) Timeout policy

If status does not reach `can_reset=1` within timeout window:

- set `ERROR_TIMEOUT`
- leave printer operational
- report `cancel_timeout` for operator action
- do **not** invoke shutdown automatically

## Required host changes

### `klippy/extras/manual_stepper.py`

- Add cancel controller class or internal state-machine methods:
  - `_cancel_request()`
  - `_cancel_step_tick()`
  - `_cancel_reconcile()`
  - `_cancel_abort_timeout()`
- Register `CANCEL_STEP` mux command.
- Track per-move metadata (`move_id`, `expected_end_time`).
- Remove inline stop/reset logic from command handler.
- Ensure `do_set_position`, `do_enable(0)`, `do_homing_move`,
  `GCODE_AXIS` transitions clear/deny cancel state appropriately.

### `klippy/mcu.py`

- Add wrappers for new MCU commands:
  - `lookup_command("stepper_cancel_start ...")`
  - `lookup_command("stepper_cancel_trigger ...")`
  - `lookup_query_command("stepper_cancel_status ...")`
  - `lookup_command("stepper_cancel_finish ...")`
- Add lightweight helper class `MCU_stepper_cancel` (analogous to
  existing wrappers) to hide raw command details.

### `klippy/stepper.py`

- Add helper to perform post-cancel reconciliation once status says safe:
  - safe reset clock path (guarded by `can_reset`)
  - or consume `stepper_cancel_finish` semantics directly.
- Keep existing `note_homing_end()` unchanged for homing path.

### `klippy/extras/motion_queuing.py`

- No core redesign required, but add optional debug hooks for cancel trace:
  - move queue activity marker tagged with stepper name and move_id.

## Required MCU firmware changes

### `src/stepper.c`

Add cancel state to `struct stepper`:

- `cancel_flags`
- `cancel_clock`
- `cancel_reason`

Add commands:

- `command_stepper_cancel_start(uint32_t *args)`
- `command_stepper_cancel_trigger(uint32_t *args)`
- `command_stepper_cancel_status(uint32_t *args)`
- `command_stepper_cancel_finish(uint32_t *args)`

Rules:

- `cancel_trigger` may run while active and must be IRQ-safe.
- `cancel_finish` must reject if active (`s->count != 0`), but reject by
  response/status code (not immediate global shutdown for this path).

### `src/trsync.c` (optional)

If cancel reuses trsync internals, add dedicated cancel reason code and
non-fatal status responses for cancel timeout. Alternatively keep cancel
fully in `stepper.c` and skip trsync dependency.

### `src/command.c` / protocol docs

- Add command schema entries and response formats.
- Ensure unknown/legacy firmware behavior is graceful on host side.

## Protocol / compatibility strategy

- Host feature-detect new cancel commands from MCU dictionary.
- If unsupported:
  - `CANCEL_STEP` returns clear error:
    `MCU firmware does not support stepper cancel protocol`.
- Do not silently fallback to unsafe legacy behavior.

## Documentation updates

### `docs/G-Codes.md`

Update `CANCEL_STEP` section:

- asynchronous behavior (`cancel_pending` then eventual state)
- supported scope
- possible responses (`already_idle`, `cancel_timeout`)

### `docs/Protocol.md` and `docs/MCU_Commands.md`

- document new commands and response fields.

### `docs/Manual_Stepper_Pipeline.md`

- add cancel state-machine flowchart and host/MCU handshake timeline.

## Testing plan

### Unit/integration (klippy tests)

Add tests in `test/klippy/manual_stepper.test`:

1. cancel request on idle stepper -> no-op status
2. active SYNC=0 move -> cancel_pending then completion
3. repeated cancel loops (10+) without shutdown
4. reject in `GCODE_AXIS`
5. reject/unsupported firmware path

### Live stress (Moonraker HTTP)

Use `scripts/test_cancel_step_moonraker.py` with:

- `--enable-before-move`
- varying cancel delays (0.03, 0.05, 0.10, 0.20)
- 100+ iterations each

Acceptance criteria:

- no MCU shutdown
- no `trsync_state` timeout dependency
- deterministic per-iteration outcome

## Migration / rollout strategy

1. Implement MCU commands behind compile-time feature constant.
2. Implement host wrapper with feature detection.
3. Land docs + tests.
4. Stage rollout:
   - v1: strict scope (`MOVE + SYNC=0`)
   - v2: evaluate homing/extra-axis support only after stability data.

## Open design decisions

1. Should cancel completion be reported via status object (`manual_stepper`)
   in addition to immediate gcode response?
2. Should `CANCEL_STEP` optionally auto-disable motor after completion?
3. Should timeout be user-configurable per manual stepper section?

## Recommended immediate next implementation steps

1. Revert current experimental trsync-based cancel internals.
2. Introduce host-side state machine skeleton and no-op command plumbing.
3. Add MCU command stubs returning explicit `unsupported` status.
4. Wire one end-to-end happy path (`cancel_start/trigger/status/finish`).
5. Re-run Moonraker stress loops as gating criterion before any broader scope.

## Phased implementation checklist

This section breaks the work into explicit phases with concrete tasks and
phase-gate criteria.

### Phase 1 - Host scaffolding (no MCU protocol yet)

Goal: land non-risky host structure first, with zero chance of MCU shutdown
from cancel command.

Checklist:

- [ ] Remove legacy/experimental `CANCEL_STEP` internals that directly issue
      trsync/reset flows from `manual_stepper.py`.
- [ ] Add `CancelController` state fields to `ManualStepper`:
      - `cancel_state`
      - `cancel_token`
      - `active_move_id`
      - `cancel_deadline`
- [ ] Implement `CANCEL_STEP` command contract in host-only mode:
      - `already_idle` on idle
      - `reject_gcode_axis` in extra-axis mode
      - `unsupported_firmware` until Phase 2
- [ ] Add structured logging for cancel transitions (single-line, grep-friendly).
- [ ] Keep all existing move behavior unchanged for non-cancel workflows.

Exit criteria:

- [ ] No behavior regression in existing `manual_stepper.test`.
- [ ] `CANCEL_STEP` never triggers shutdown when firmware lacks cancel protocol.
- [ ] Stress script reports stable `unsupported_firmware`/`already_idle`
      outcomes (no crashes).

### Phase 2 - MCU protocol plumbing (commands exist, minimal behavior)

Goal: define and wire new protocol safely before optimizing behavior.

Checklist:

- [ ] Add MCU command declarations and handlers in `src/stepper.c`:
      - `stepper_cancel_start`
      - `stepper_cancel_trigger`
      - `stepper_cancel_status`
      - `stepper_cancel_finish`
- [ ] Add response format for `stepper_cancel_status`.
- [ ] Add dictionary/command documentation updates.
- [ ] Add host wrappers in `klippy/mcu.py`.
- [ ] Add host feature detection and hard fail with clear error if missing.

Exit criteria:

- [ ] Host can issue protocol round-trip without move active.
- [ ] `stepper_cancel_finish` never hard-shutdowns on invalid state; returns
      explicit failure status instead.
- [ ] No `trsync_state` dependency in `CANCEL_STEP` path.

### Phase 3 - End-to-end cancel happy path

Goal: implement robust cancel flow for `MOVE + SYNC=0`.

Checklist:

- [ ] Implement state-machine reactor callback chain:
      `REQUESTED -> ARMED -> TRIGGERED -> DRAINING -> RECONCILE -> DONE`.
- [ ] Ensure request handler is non-blocking and returns deterministic status.
- [ ] Implement timeout handling to `ERROR_TIMEOUT` without printer shutdown.
- [ ] Reconcile commanded position only after `status.active=0` and
      `status.can_reset=1`.
- [ ] Ensure `SET_POSITION`, `ENABLE=0`, and mode changes clear pending cancel.

Exit criteria:

- [ ] 100-iteration stress pass at cancel delay `0.10` with no shutdown.
- [ ] No `Unable to obtain 'trsync_state' response` in logs.
- [ ] No `Can't reset time when stepper active` in logs.

### Phase 4 - Robustness matrix and soak tests

Goal: prove stability across timing windows and normal operational variance.

Checklist:

- [ ] Stress matrix:
      - `cancel_delay`: 0.03 / 0.05 / 0.10 / 0.20
      - `speed`: low/nominal/high
      - `accel`: low/nominal/high
- [ ] Add reconnect/restart scenarios:
      - RESTART before loop
      - FIRMWARE_RESTART before loop
      - printer-not-ready retry handling in test script
- [ ] Add 1000-iteration soak test at nominal settings.
- [ ] Capture and archive log excerpts for each run.

Exit criteria:

- [ ] Zero MCU shutdowns in soak run.
- [ ] Deterministic terminal state after each cancel loop.
- [ ] No drift/lockup requiring manual intervention.

### Phase 5 - Docs and rollout completion

Goal: ship with clear operator semantics and maintenance guidance.

Checklist:

- [ ] Update `docs/G-Codes.md` with final user-visible behavior.
- [ ] Update `docs/Protocol.md` and `docs/MCU_Commands.md`.
- [ ] Add troubleshooting section:
      - timeout handling
      - unsupported firmware message
      - recommended restart path
- [ ] Add changelog entry in `docs/Config_Changes.md` if command is new.

Exit criteria:

- [ ] Docs match actual runtime behavior and responses.
- [ ] Test script usage examples validated against released code.

## Per-phase go/no-go gates

Do not advance a phase unless all gates are green:

- **Safety gate**: zero MCU shutdowns introduced by new code in that phase.
- **Determinism gate**: command responses map to documented states only.
- **Recovery gate**: after handled errors/timeouts, printer remains operable.
- **Observability gate**: logs provide enough data to diagnose failures quickly.

## Suggested execution order (single branch)

1. Phase 1 + tests
2. Phase 2 protocol + host wrappers
3. Phase 3 happy path
4. Phase 4 soak/matrix
5. Phase 5 docs finalization

If review bandwidth is tight, split into two PRs:

- PR-A: Phase 1 scaffolding + script hardening
- PR-B: Phases 2-5 protocol and end-to-end behavior
