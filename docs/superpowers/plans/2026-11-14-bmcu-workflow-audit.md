# BMCU workflow audit implementation plan

**English** | [Português brasileiro](../../pt-BR/superpowers/plans/2026-11-14-bmcu-workflow-audit.md)

> Execute task by task. The main agent owns edits; final review is read-only.

**Goal:** improve the maintained change workflow and implement the confirmed
six-cycle X=-6/-3 nozzle wipe without weakening quality or safety checks.

**Architecture:** retain the integration/cutter/measurement separation, native
BMCU callbacks and existing installation preservation. Use the current unittest
benches and timing evidence; do not introduce another transport or motion path.

**Tech stack:** Python 3, Klipper motion/G-code APIs, Jinja configuration, unittest.

**Spec:** `../specs/2026-11-14-bmcu-workflow-audit.md`.

## Global constraints

- X wiping: -6 to -3, six complete round trips and the final crossing; Y=123.
- Preserve 35 mm purge and calibrated temperature/extrusion settings.
- Preserve sensor/encoder/session checks, X-first homing, servo/PWM guards,
  cancellation, recovery and machine-coordinate semantics.
- No printer deployment, heating, movement or route-state reconciliation.
- English public API; EN/PT-BR documents; preserve user settings on upgrade.

## Review focus

- Session, sensor or temperature changes at the end of a purge must not become
  a falsely successful load.
- Removing a wait must not remove a physical inspection boundary or permit
  concurrent UART traffic inside a printer critical section.
- Wider wiping must not alter Y/Z or bypass G-code-offset-independent limits.
- Existing custom wipe coordinates/pass counts must survive installation.
- Timing/material estimates must not be presented as measured speed or quality.

### Task 1: Read the maintained workflow and establish evidence

**Files:** `integrations/bmcu/bmcu_ender_integration.py`,
`integrations/bmcu/bmcu_cortador_ender.py`,
`integrations/bmcu/bmcu_measurement.py`,
`config/ender3-v3-se-validada/`, `profiles/orca/ender3-v3-se-bmcu/`,
`scripts/ender3-v3-se/instalar_multicor_bmcu.py` and their existing tests.

- [x] Run the complete integration/display/documentation baseline suites.
- [x] Read the integration, cutter and measurement modules in full; trace native
  callbacks, relevant configuration and installer preservation call sites.
- [x] Record inspected ranges, concrete findings and rejected unsafe shortcuts
  in this plan. Consult the checked-in Klipper implementation for synchronization.
- [x] For each verified defect or removable repetition, add a focused failing
  regression in the existing owning test module before changing implementation.

### Task 2: Confirmed wipe defaults and preservation

**Files:** integration constructor and `wipe()`,
`config/ender3-v3-se-validada/bmcu-multicor-ender.cfg`,
`test/bmcu_integracao/test_cutter_safety.py`,
`test/bmcu_integracao/test_integracao.py`.

- [x] Add a failing constructor/config test asserting `wipe_x=-3`, `wipes=6`
  while `purge_x=-6`, `purge_y=123` and `purge_mm=35` remain unchanged.
- [x] Add a machine-coordinate movement test asserting exactly six complete
  round trips plus final crossing, unchanged Y/Z and preserved print overrides.
- [x] Add a preservation test with custom coordinates/pass count and verify
  that preparation does not overwrite it with the new defaults.
- [x] Observe the expected failures, update only the relevant defaults and
  make the focused tests pass. Do not rewrite preserved installation settings.

### Task 3: Reproduce and correct workflow findings

**Files:** existing integration/cutter/measurement functions implicated by Task 1;
their existing `test/bmcu_integracao/` regression modules.

- [x] Exercise a session/sensor failure during the last purge segment and during
  wiping; assert no subsequent movement or success is reported after detection.
- [x] Exercise each proposed reduced synchronization against command counts,
  motion order, max extrusion distance and the existing UART critical-section
  assertions; reject a proposal if inspection/cancellation boundaries change.
- [x] Implement only corrections with reproduced failures or demonstrable
  redundant work. Keep physical feedrates, purge volume and calibrated delays.
- [x] Re-run the complete integration suite after each grouped correction.
- [x] Report actual command/synchronization reductions separately from estimated
  mechanical travel time; make no unmeasured physical performance claim.

### Task 4: Documentation, full validation and final review

**Files:** affected operator guides and their PT-BR companions, this plan/spec
and their companions. Keep process reports out of project/profile READMEs.

- [x] Document wider wiping, the added travel/time and existing-setting
  preservation, with a manual commissioning checklist in the operator guide.
- [x] Run all integration, display and documentation tests, strict document
  checks, native Git diff checks and original full whitespace checking on a
  public LF export.
- [x] Run the target build and configuration validation if changes affect their
  inputs; do not substitute these for regression tests or physical acceptance.
- [x] Obtain mandatory external GPT final review of the entire task, fix concrete
  findings and re-run affected validations before delivery.

## Audit evidence

### Inspected boundary

- Read the full integration (original lines 1–1815), cutter (1–400) and
  measurement (1–736), including capture timing, assisted diagnostics, native
  callback contexts, tip forming, encoder/buffer withdrawal, parking, manual
  cancellation, paused-print recovery and reset. Measurement/cutter code did
  not need changes for the reproduced findings.
- Read all eight validated configuration files and the Orca start/change/end
  snippets and settings. The existing reload-project test inspects the 3MF's
  embedded settings and all 16 parts; geometry and slicer profiles are unchanged.
- Traced installer preparation, native capture/unload/contract patches, active
  includes, preserved sequence/tip/cutter/panel settings, activation and rollback.
  Read `integrations/bmcu/ender_transport.py`; transport guards are unchanged.
- Consulted `klippy/toolhead.py` motion/wait semantics and the official Klipper
  code overview. Context7 was unavailable due to its quota. The pinned
  [BMCU device implementation](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.0.6/klippy/extras/bmcu_core/device.py)
  confirms each motion-mode resend is an acknowledged request, not a local
  no-op. Native callback excerpts and existing recovery/homing tests were used
  to trace the maintained boundary; this is not an upstream-wide certification.

### Reproduced findings and corrections

- Old completion checks occurred only before purge increments. Injecting a
  restart, missing channel, clear entry switch or cold extruder during the last
  increment or wiping produced false success. Regression cases now check all
  13 wipe boundaries, including the final crossing, and the settling dwell.
  Manual display cancellation during that dwell also blocks further movement.
- Reissuing assistance at every chunk could restart a feeder that stopped
  unexpectedly. Loading now enters follow once, checks each fresh snapshot and
  checks the final nozzle-feed boundary. Withdrawal starts assistance in its
  owning phase; its existing helper still checks mode, session, sensor, encoder
  and buffer. The intentional periodic withdrawal stop/restart remains intact.
- New constructor/template defaults match the requested six round trips and
  final crossing. Real Klipper G-code-state tests cover offsets, speed/flow
  overrides and stationary Y/Z; explicit calibration and installer-preserved
  coordinates/pass counts remain unchanged.
- Each changed behavior was first exercised with failing regressions. The
  baseline was 253 integration + 71 display + 14 documentation tests. Integration
  grew to 267 passing tests, including parameterized fault-injection cases and
  checks before arming nozzle assistance after capture and after its reply.

### Optimization accounting and rejected shortcuts

- With 80 mm nozzle feed in 10 mm chunks, mode requests fall from nine to two
  across nozzle feed plus purge: seven redundant acknowledged requests removed.
- In the synthetic cutter withdrawal clearing the switch at 66 mm, 33 redundant
  mode requests are removed. The bench count falls from 37 to four, including
  its one initial setup request; the phase start and two intentional restarts
  remain. Extruder travel is still 66 monitored 1 mm increments.
- The cutter-free tip/withdrawal bench falls from 25 to three mode requests,
  again including its one setup request: 22 redundant requests removed. Its
  146 extrusion increments and net 66 mm withdrawal are unchanged. These are
  synthetic test counts, not measured printer timings or calibration advice.
- No motion-completion wait, one-second purge dwell, servo-settling delay,
  sensor transition, capture window or encoder inspection was removed. Batching
  extrusion submoves would alter the timing of Klipper's temperature checks;
  longer unload chunks would weaken slack/progress boundaries. Both were rejected.
- Purge volume stays 35 mm. There is no demonstrated material saving yet;
  avoiding retries or retained purge remains a physical acceptance goal.
- Wiping grows from 14 to 39 mm: ideal travel at F600 grows from 1.4 to 3.9 s.
  Additional safety readbacks also take time. Neither the removed requests nor
  these estimates establish a faster total exchange, detachment or print quality.

### Validation results

- 267 integration + 71 display + 14 documentation tests passed: 352 total.
- Strict public-document checks: 77 English pages, no missing PT-BR pairs,
  no structural errors. Native Git diff checks passed.
- Original complete whitespace checker passed on a fresh public LF export.
  Its first run caught one overlong new test line, which was corrected.
- The GD32F303 target built; the dictionary's display bridge, chunk size and
  commands/responses were verified. The stock-display Klippy configuration test
  passed against that dictionary. Its first in-place attempt encountered the
  upstream C-helper build's space-in-path limitation; running from the public
  export without spaces passed. No upstream workaround was added.
- Python 3 host import validation passed. MCU sources, dependencies and target
  configuration are unchanged; the complete multi-target Python 2/3 CI matrix
  was not rerun locally. No printer access, deployment or physical test occurred.
- External final review covered all 12 changed/new files and reproduced one
  additional P2 finding: unexpected assistance loss at the periodic restart
  boundaries could be hidden by that restart. Regressions first failed at the
  synthetic 26 mm and 52 mm boundaries. The helper now checks the fresh mode
  after every completed increment while the entry switch remains occupied,
  before any periodic restart. Healthy restart counts and all 352 tests still
  pass. The main agent verified this focused correction without another review
  call; it adds no new motion, API or scope. Physical acceptance remains pending.
