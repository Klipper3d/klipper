# BMCU workflow audit and wider nozzle wiping

**English** | [Português brasileiro](../../pt-BR/superpowers/specs/2026-11-14-bmcu-workflow-audit.md)

## Approved goal

Review the maintained filament-change workflow line by line, correct verified
defects and remove demonstrably unnecessary work. Prefer shorter changes and
less waste without trading away print quality, calibration or safety.

The user confirmed six complete X round trips between -6 and -3, followed by
the existing final crossing to the wipe side. Y remains 123. This is nozzle
wiping, not a change to the blade's cutting stroke. The previous default was
three round trips between -6 and -4 plus the final crossing.

## Constraints

- Keep the current 35 mm purge, temperatures, extrusion feedrates and physical
  calibration unless evidence supports a separate, explicit adjustment.
- Preserve homing order, servo retraction/PWM release, motion limits, sensor and
  encoder checks, BMCU session identity, cancellation and paused-print recovery.
- Reuse existing helpers and tests. Do not add a parallel filament-change path.
- Do not batch away a synchronization point needed to inspect the physical
  state. Do not replace sensor-controlled unloading with a fixed short pullback.
- Respect installation-specific values. Document how new defaults differ from
  preserved existing settings instead of silently overwriting calibration.
- Public API and diagnostics remain English; operator documentation stays EN/PT-BR.
- No printer deployment, movement, heating or route reset is authorized by this
  audit. Physical acceptance and measured speed/quality remain separate.

## Review boundary and evidence

Inspect the fork-maintained integration, cutter, measurement, configuration,
slicer entry points and installation preservation paths involved in this
workflow. Record which files were read and the evidence for each finding.
Do not imply that this audit proves the entire upstream Klipper codebase
perfect or establishes a universal maximum speed.

For each change, use failing regression tests before implementation, preserve
machine-coordinate semantics and report any timing or material estimate as an
estimate, not a measurement. Wider and more frequent wiping increases that
phase's travel; do not disguise this as a speed improvement.
