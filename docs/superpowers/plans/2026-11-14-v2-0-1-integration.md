# Release 2.0.1 implementation plan

**English** | [Português brasileiro](../../pt-BR/superpowers/plans/2026-11-14-v2-0-1-integration.md)

> Execute task by task. The main agent owns implementation; final review is read-only.

**Goal:** ship the approved 2.0.1 scope without printer deployment or power-loss recovery.

**Architecture:** pin and test real BMCU 1.1.2 source contracts, reuse native refill
and transport guards, and build a minimal Mainsail 2.19.0 source overlay using
the existing Moonraker/Vuex connection. Keep the panel installer separate.

**Tech stack:** Python 3, Klipper, unittest, Vue 2.7, TypeScript, Vuetify 2,
Mainsail 2.19.0, Vitest and browser verification.

**Spec:** `../specs/2026-11-14-v2-0-1-integration.md`.

## Global constraints

- Release 2.0.1; BMCU-Klipper 1.1.2; firmware 1.0.4; Mainsail 2.19.0.
- Preserve the completed wipe/safety audit and installation-specific calibration.
- Reuse native runout and guarded Resume; do not simulate physical route states.
- No printer access/deployment/heating/movement/flash; no power-loss recovery.
- English command API and complete EN/PT-BR documentation/UI labels.

## Review focus

- Old firmware, missing native methods and changed source contracts fail closed.
- Runout replacement cannot skip loading, move to the part early or lose the file/map.
- UI metadata input cannot inject G-code or race an active operation silently.
- Offline/stale/uncertain channel data cannot be presented as confirmed loaded.
- Failed panel installation/rollback preserves unrelated files and user configuration.

### Task 1: BMCU 1.1.2 and confirmed runout replacement

**Files:** integration/measurement modules, display adapter, existing installer,
validated macros, owning tests and pinned native-method fixtures under
`test/bmcu_integracao/fixtures/`.

- [x] Compare actual 1.1.2 contracts and transport protection with the installed patches.
- [x] Add failing tests for version/firmware guards and real patched native methods.
- [x] Adapt necessary Ender callbacks. Retain explicit history verification: upstream
  counter randomization is not an equivalent check; restart via native process helpers.
- [x] Exercise native manual refill through actual pause/resume and the Ender callback;
  assert file/map/position preservation, completed wipe before resume and failures paused.
- [x] Extend display read-only availability/refill state without UART from rendering.
- [x] Run all Python suites and preserve rollback/custom-setting regressions.

### Task 2: Native dashboard card and controlled installation

**Files:** `integrations/mainsail/` source overlay, build/apply scripts under
`scripts/ender3-v3-se/`, panel unit/browser tests and EN/PT-BR operator guide.

- [x] Inspect pinned Mainsail dashboard layouts, subscriptions, dialogs and spool components.
- [x] Write failing tests for channel normalization, safe metadata commands, offline/busy
  guards, layout registration and installer validation/rollback.
- [x] Implement the four-spool card, theme-aware layout, metadata dialog and EN/PT-BR labels.
- [x] Build the overlay reproducibly from the pinned upstream revision, preserving licenses.
- [x] Implement a separately invoked panel installer with explicit destination, backup,
  artifact validation and rollback; no automatic remote deployment.
- [x] Run unit/lint/type/build checks and inspect desktop/mobile browser states.

### Task 3: Release assembly and final validation

**Files:** `VERSION`, release-note pair, current operator guides/READMEs/profile
documentation, CI workflow, both release plan/spec pairs.

- [x] Update supported-version references without rewriting historical release notes.
- [x] Document the separate package/firmware maintenance boundary and preserved wipe settings.
- [x] Prepare 2.0.1 notes, panel artifact/checksums/provenance and release CI checks.
- [x] Run complete Python/UI suites, strict public-document checker, full LF whitespace,
  target build/configuration and review the complete native Git diff.
- [x] Obtain external GPT final review of all prior and new changes; correct concrete
  findings and rerun affected checks before any release publication.

## Validation checkpoint

- Python: 299 integration, 74 display and 14 documentation tests passed.
- Pinned upstream refill fixture comparison, full-source patch compilation/idempotence
  and native verified process stop passed without UART or a printer.
- Mainsail: 61 unit tests, lint, formatting and production/TypeScript/PWA build passed.
- Local browser: 20 language/theme/state cases, keyboard editing, saved metadata,
  stale/failing saves and mobile width checks passed against synthetic state.
- Strict documentation: 81 English pages, no missing PT-BR pairs or structural errors.
- Full public LF whitespace, GD32F303 build/bridge dictionary and Klippy configuration
  passed. The local firmware export is a validation build, not a tagged release binary.
- Candidate card: `ci_build/ender-201-verified/mainsail-bmcu.zip`; complete manifest,
  licenses and canonical build fingerprints verified; browser fixture excluded.
- At this checkpoint, changes were local and uncommitted; GitHub jobs/publication had
  not run. Package/firmware maintenance and physical commissioning remain separate.
  Power-loss recovery is deferred.
- External review was rerun successfully. Its P2 finding at the nozzle-feed/purge
  boundary was reproduced with a failing native-refill regression: a stopped feeder
  could be rearmed before the purge. The first purge readback now requires assistance
  already active; the regression confirms no additional extrusion/wiping and retains
  the paused file, map and refill request. The affected 299-test suite passed.
  The same reviewer approved the software scope after confirming the fix; remote CI
  and physical acceptance remain separate validation boundaries.
