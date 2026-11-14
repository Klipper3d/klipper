# X-first cutter homing

**English** | [Português brasileiro](../../pt-BR/superpowers/specs/2026-11-13-cutter-homing.md)

## Approved behavior

The operator confirmed that the entire X homing path is clear with the cutter
stop in its existing position. On this machine X homes at the configured
endstop, -6 mm. Do not send any servo command before or during X homing.
After X homing completes successfully, retract the stop, wait for the servo,
then disable PWM. A complete G28 continues with Y (bed) and Z (vertical).
Keep servo torque during deployment, blade travel and carriage retreat; release
PWM only after the stop has retracted. This does not switch off the 5 V supply.

## Implementation decisions

- Reuse the integration's existing G28 wrapper, native homing handler and
  cutter retraction routine. Never use an unreferenced G1 or fake X position.
- Repeat X-first preparation on every G28 with the calibrated cutter enabled.
  Axis-specific requests retain their requested remaining axes: G28 X stops
  after retraction, G28 Y adds X preparation, G28 Z adds X preparation but
  retains the native requirement for an already referenced Y axis.
- Preserve native safe_z_home, including one initial protective Z lift, probing
  location and post-probing lift. The initial lift may precede X; no servo
  command precedes X. Reuse its extracted home_axes continuation for Y/Z so
  splitting a logical G28 cannot duplicate the physical initial lift.
- Abort before subsequent stages on X failure, missing X reference, invalid
  profile, busy BMCU, or unsafe retraction position. Retain the manual recovery
  requirement for an interrupted cut with uncertain stop position.
- The display's home action sends only G28. Explicit manual open/close actions
  still require confirmation; no display shortcut retracts before X.
- Preserve manual load/unload auto-homing by performing necessary cutter G28
  preparation before claiming the manual operation, not by bypassing its lock.
- Reuse soltar_servo_recolhido, defaulting to 1 in new and missing settings;
  retain explicit 0 as an opt-out for mechanisms requiring holding torque.
  Existing preserved profiles with 0 need that setting changed to 1 on install.
- recolher_sem_referencia no longer bypasses X homing and is obsolete.

## Verification and limits

Automated tests cover full and partial homing, repeated calls, no servo output
during X, failure isolation, display behavior, PWM timing and opt-out. Run both
project unittest suites on Linux/WSL and an external final review. Tests do not
prove physical clearance, stop retention without torque, or successful cutting.
No printer service, remote configuration or firmware is changed by this task.
