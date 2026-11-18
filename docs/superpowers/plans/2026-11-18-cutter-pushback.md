# Cutter Pushback Implementation Plan

> Execute locally, task by task, using test-driven development. No printer
> installation, motion, commits or firmware changes are authorized by this plan.

**Goal:** Reposition the cut fragment with a bounded 10 mm advance and return,
while the BMCU follows the direct drive without accumulating tube slack.

**Architecture:** Add the operation to the existing cutter unload path.
Reuse the native `MOTION_BEFORE_PULL_BACK` pressure controller, which follows
both filament directions, and the existing incremental withdrawal/session/
sensor/buffer checks. Add symmetric encoder checks only for pushback.

**Tech Stack:** Python, Klipper G-code, BMCU-Klipper 1.1.2 / firmware 1.0.4.

**Spec:** Design below, reflecting the user's clarification and proposed 1 cm.

## Design

- Only a successful cutter cycle can lead to pushback, after commanding blade
  release and moving to the existing purge location.
- New cutter profiles request 10 mm at 120 mm/min in both directions. Existing
  profiles without `pushback_mm` keep their behavior until explicitly updated.
  Zero disables pushback; positive distances cannot exceed 10 mm.
- Do not change pre-cut retraction, temperature, motor current, native buffer
  targets, capture, purge or parking distances.
- Use the same native pressure-assistance mode in both directions: its firmware
  controller feeds or retracts according to the buffer, without normal printing
  target refills or an artificial motor-direction switch.
- Move in existing bounded increments, maintain one encoder/command origin
  across advance, return and final withdrawal, and wait before an increment
  would exceed the existing 2 mm mismatch budget.
- Also wait for the buffer to be inside the symmetric safety band derived
  from `unload_buffer_limit` (30–70% with the existing 70% setting).
- Stop on excessive mismatch, invalid telemetry, lost session/sensor,
  stopped assistance, cooling or timeout. Never retry or compensate a failed
  pushback automatically, and never mark it as parked.
- Tests establish software behavior, not physical fragment movement, absence of
  heat creep or protection against every mechanical PTFE failure.

## Review Focus

- A stalled feeder in either direction cannot consume a whole 10 mm stroke.
- Oversupply and opposite-direction encoder travel abort without another move.
- Residual encoder discrepancy is not erased at reversal or final withdrawal.
- Old calibrated profiles do not silently acquire new physical movements.
- Cold/dry tests, failed cuts, all channels and sensor/session failures remain
  safe; normal unload and parking retain their existing contracts.

## Task 1: Profile and synchronized pushback

**Files:** `integrations/bmcu/bmcu_cortador_ender.py`,
`integrations/bmcu/bmcu_ender_integration.py`,
`config/ender3-v3-se-validada/bmcu-cortador.cfg`,
`test/bmcu_integracao/test_cutter_pushback.py`,
`test/bmcu_integracao/test_cutter_safety.py`.

- [x] Write tests for the 10 mm round trip, native mode reuse, idle cutting,
  unchanged net withdrawal and preservation of old/disabled profiles.
- [x] Run the tests and verify missing-feature failures.
- [x] Add validated `pushback_mm` and `pushback_feedrate` settings; extend the
  existing movement helper with an opt-in symmetric following guard.
- [x] Add pushback to cutter unloading, with per-direction progress diagnostics
  and no reset of the encoder origin before the subsequent withdrawal.
- [x] Exercise stalled/late/wrong-direction encoders, buffer limits, interrupted
  strokes, firmware restart, sensor loss, cooling and failed blade commands.
- [x] Run targeted tests, then both repository Python suites.

## Task 2: Instructions and final verification

**Files:** `docs/Ender3_V3_SE_Cutter.md`,
`docs/pt-BR/Ender3_V3_SE_Cutter.md`, this plan.

- [x] Document the new-profile values, explicit opt-in for retained profiles,
  native bidirectional following, fail-stop behavior and hardware limitations.
- [x] Run diff/style checks and review the complete change.
- [x] Request the mandatory external Sol review; address concrete findings.
- [x] Report software validation separately from unperformed hardware testing.

## Verification evidence

- Initial integration baseline: 341 tests passed.
- New pushback tests failed for the missing feature before implementation.
- Pre-review integration suite: 359 tests passed under WSL Ubuntu 22.04.
- Final display suite: 82 tests passed in the same environment.
- Targeted cutter suites: 82 tests passed.
- `git diff --check` passed. Direct Linux whitespace checking of this Windows
  checkout reports existing CRLF characters. The full
  `bash scripts/check_whitespace.sh` passed on a public 2,540-file snapshot
  with LF endings, including both new files.
- No printer connection, installation, service restart or physical test.
- External Sol review reported one P2: the inactive pushback default rate was
  incorrectly checked against slow extruder limits in retained profiles.
  The limit comparison now includes this rate only when pushback is enabled.
  A regression reproduced the failure for both absent and zero distance before
  the fix, then passed. Targeted pushback suite: 19 tests passed.
  This bounded compatibility fix introduces no new physical behavior.
- Post-fix full verification: 360 integration tests and 82 display tests passed.
  The whitespace checker passed on LF copies of all eight changed files;
  `git diff --check` passed as well.
