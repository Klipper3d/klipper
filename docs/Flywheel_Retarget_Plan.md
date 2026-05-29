# Flywheel velocity control: `MANUAL_STEPPER RETARGET=`

Implementation plan for continuous trapq-based velocity/direction changes on
flywheel drives.

**Full reference (timing, delays, diagnostics):**
[Manual_Stepper_Retarget.md](Manual_Stepper_Retarget.md)

Trapq mechanics: [Manual_Stepper_Trapq.md](Manual_Stepper_Trapq.md)

## Requirements summary

| Requirement | Plan |
|-------------|------|
| Motor must **not stop** between flywheel velocity/direction changes | Trapq splice only; **no** `stepper_stop` |
| ~200ms command → new motion OK | `splice_time = est + kin_flush_delay` |
| **Keep `CANCEL_STEP` intact** | No changes to `cmd_CANCEL_STEP` or cancel state machine |
| **Keep `MANUAL_STEPPER` intact for steer** | Existing `MOVE`, `ENABLE`, `SET_POSITION`, `SYNC`, homing, `GCODE_AXIS` unchanged |
| New flywheel API | **`MANUAL_STEPPER RETARGET=<pos> SPEED=…`** on same mux command |
| **Bidirectional** | Direction from `RETARGET` position vs splice position (signed distance) |

## Command surface (additive only)

### Unchanged: steer and hard cancel

```gcode
MANUAL_STEPPER STEPPER=front_steer MOVE=45 SPEED=10 SYNC=1
CANCEL_STEP STEPPER=flywheel
```

### New: flywheel streaming

```gcode
MANUAL_STEPPER STEPPER=flywheel MOVE=99999 SPEED=30 ACCEL=0 SYNC=0
MANUAL_STEPPER STEPPER=flywheel RETARGET=99999 SPEED=42 ACCEL=0
MANUAL_STEPPER STEPPER=flywheel RETARGET=-99999 SPEED=42 ACCEL=0
```

`RETARGET` is evaluated **before** `MOVE` (mutually exclusive).

| Parameter | Required | Notes |
|-----------|----------|-------|
| `RETARGET` | Yes | New endpoint (mm); direction = sign(`RETARGET - pos_at_splice`) |
| `SPEED` | No | Magnitude only (`above=0`) |
| `ACCEL` | No | Use `0` for flywheel |

**Rejected:** `RETARGET`+`MOVE`, `RETARGET` during `GCODE_AXIS`, `RETARGET` with no active `SYNC=0` move, `RETARGET`+`STOP_ON_ENDSTOP`/`ENABLE`/`SET_POSITION`.

## `do_retarget()` (new)

Separate from `do_move()` — no cancel protocol.

1. `_refresh_cancel_activity()`
2. `splice_time = est_print_time + splice_delay`
3. `trapq_truncate_after(trapq, splice_time)` → `splice_pos`
4. Update `commanded_pos` / `rail.set_position` (not `trapq_set_position`)
5. `_submit_move(splice_time, movepos, speed, accel)`
6. `cancel_drain_host_pipeline()` (host only)
7. `itersolve_reset_flush_time(sk, splice_time)`
8. Update `commanded_pos`, `next_cmd_time`, `cancel_deadline`
9. `note_mcu_movequeue_activity(next_cmd_time)`

## `trapq_truncate_after` (implemented)

C API in [`trapq.c`](../klippy/chelper/trapq.c):

```c
int trapq_truncate_after(struct trapq *tq, double print_time
                         , double *pos_x, double *pos_y, double *pos_z);
```

Returns `0` and sets position at `print_time`; returns `-1` if trapq is empty.
Truncates the containing move or deletes moves starting at/after the splice.

## Files to change

| File | Change |
|------|--------|
| `klippy/chelper/trapq.c`, `trapq.h`, `__init__.py` | `trapq_truncate_after` |
| `klippy/extras/manual_stepper.py` | `do_retarget`, `RETARGET` branch, `splice_delay` config |
| `docs/G-Codes.md`, `docs/Manual_Stepper_Pipeline.md` | Docs |
| `test/klippy/manual_stepper*.test` | RETARGET tests; cancel tests unchanged |

**Non-goals:** change `src/stepper.c` cancel firmware, remove `CANCEL_STEP`, or change `do_move` / `SYNC=1` behavior.

## Validation

1. Existing cancel + steer tests pass
2. No STEP gap on speed/direction retargets
3. ~200ms latency acceptable
4. No `Timer too close` in logs

## Fallback

Velocity-mode hardware for flywheel; Klipper for steer + `CANCEL_STEP`.
