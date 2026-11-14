# X-first Cutter Homing Implementation Plan

**English** | [Português brasileiro](../../pt-BR/superpowers/plans/2026-11-13-cutter-homing.md)

> **For agentic workers:** Execute this plan task-by-task in the main session;
> delegate only the mandatory external final review. Steps use checkboxes.

**Goal:** Home X without servo output, retract and release the stop, then home
the remaining requested axes without a startup confirmation.

**Architecture:** Split calls to the already registered native G28 handler in
the existing integration wrapper. Reuse cutter validation/retraction and its
PWM-release option; the display delegates homing to the same G28 entry point.

**Tech Stack:** Python, Klipper G-code dispatcher/safe_z_home, unittest, WSL.

**Spec:** `docs/superpowers/specs/2026-11-13-cutter-homing.md`

## Global Constraints

- No servo command before or during X homing, including WIDTH=0.
- Use the configured X endstop (-6 mm on the target), never fake a reference.
- Preserve safe_z_home and existing busy/uncertain-cut safeguards.
- Preserve explicit PWM opt-out and existing mechanical calibration files.
- Do not connect to or move the physical printer.

## Review Focus

- X homing error or a handler returning without reference must prevent servo/Y/Z.
- Partial G28 and repeated G28 must not bypass X-first preparation.
- Disabled/unconfigured cutters must retain the original G28 unchanged.
- A retraction/PWM failure must leave uncertain state and prevent Y/Z.
- Manual opening and interrupted cuts must not release holding torque early.

### Task 1: X-first preparation and PWM release

**Files:**
- Modify: `integrations/bmcu/bmcu_integracao_ender.py`
- Modify: `integrations/bmcu/bmcu_cortador_ender.py`
- Modify: `klippy/extras/safe_z_home.py`
- Modify: `config/ender3-v3-se-validada/bmcu-cortador.cfg`
- Test: `test/bmcu_integracao/test_cortador.py`
- Create test: `test/bmcu_integracao/test_cutter_homing.py`

**Interfaces:** `validar_homing_cortador(anterior)` wraps native G28;
`recolher_para_homing(perfil)` verifies actual X reference and retracts;
`posicionar_recolhido(perfil)` waits before optional WIDTH=0.

- [x] Add failing tests for full/partial/repeated G28, X failure/missing
  reference, busy/uncertain/disabled states, invalid profile, unsafe X,
  retraction/PWM failure and defaults/explicit opt-out.
- [x] Run the targeted unittest files in WSL and confirm expected failures.
- [x] Implement X-only native G28 followed by guarded retraction, then only
  remaining requested Y/Z, preserving non-axis parameters.
- [x] Default PWM release to 1, retain explicit 0, retire unreferenced bypass.
- [x] Preserve manual load/unload by calling the same G28 before claiming its
  operation when references/retraction are needed; test cold-start loading.
- [x] Update obsolete tests and run the integration suite.
- [x] Address final review P1: first reproduce cumulative Z lift in full and
  partial homing with physical position tracked separately, then extract native
  `SafeZHoming.home_axes(gcmd)` so only the first segment performs the initial
  protective lift. Preserve normal native calls and the post-probing lift.

### Task 2: Display and operator documentation

**Files:**
- Modify: `klippy/extras/e3v3se_display/__init__.py`
- Modify: `klippy/extras/e3v3se_display/ui.py`
- Test: `test/e3v3se_display/test_display.py`
- Modify: `docs/Ender3_V3_SE_Cortador.pt-BR.md`
- Modify: `docs/Ender3_V3_SE_Cutter.md`

**Interfaces:** `home_all()` sends only G28; `solicitar_homing()` invokes it
directly. `mover_batente(recolher)` retains manual confirmation.

- [x] Change the display test to expect direct G28 while retaining manual
  open/close confirmation; observe failure before implementation.
- [x] Remove the display's pre-homing servo shortcut and confirmation prompt.
- [x] Document X-first order, physical clearance, PWM defaults and the existing
  preserved configuration's required soltar_servo_recolhido=1 setting.
- [x] Run both complete suites with `/opt/ender3-tests/bin/python -B -m unittest
  discover -s test/<suite> -p 'test_*.py'` in WSL and `git diff --check`.
- [x] Request `review_final_gpt56` for all changed/new files, address concrete
  findings, rerun affected validation and report hardware validation limits.

## Validation outcome

- Integration: 189 tests passed; display: 57 tests passed (246 total).
- `git diff --check` passed. Negative-case logs were also present at baseline.
- External review approved all 13 files after the P1 correction; the same
  reviewer additionally confirmed 384 native G28 equivalence scenarios and
  eight dispatcher + safe_z_home axis combinations.
- No deployment or physical printer validation was performed.

No commit is planned without an explicit user request.
