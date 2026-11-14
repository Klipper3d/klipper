# Release 2.0.1: BMCU 1.1.2 and a native Mainsail card

**English** | [Português brasileiro](../../pt-BR/superpowers/specs/2026-11-14-v2-0-1-integration.md)

## Approved scope

The user selected one release, 2.0.1, containing the completed workflow audit,
BMCU-Klipper 1.1.2 compatibility, a native four-spool Mainsail dashboard card,
and confirmed manual replacement after filament runout. Power-loss recovery
is explicitly deferred. Do not implement automatic heating, homing or printing
after a restart. Updating physical BMCU firmware is a separate later operation.

## Integration and replacement

- Pin BMCU-Klipper 1.1.2 and require its firmware 1.0.4. An incompatible or
  unavailable device must not gain motion permission through a version rename.
- Inspect the actual pinned upstream methods and use native runout, detached
  tail, route reconciliation and operation-ID protection instead of duplicating
  them. Retain only Ender-specific adaptations that remain necessary.
- On runout, preserve the paused file, print map, position and extrusion state.
  The operator inserts replacement filament in the indicated channel and uses
  normal Resume on the stock screen or Mainsail. Complete native loading and
  the Ender capture/feed/purge/wipe before returning to the print. A failed or
  incomplete replacement remains paused; uncertain paths require physical review.
- Keep the 35 mm purge, six X=-6/-3 round trips plus final crossing, Y=123,
  calibrated rates/temperatures, encoder/buffer checks, X-first cutter homing,
  servo/PWM handling and existing guarded tool-change recovery.
- Preserve custom settings and full host/display backup. Do not imply that
  host rollback also undoes a separately updated external package or firmware.

## Native Mainsail card

Build a small, version-pinned overlay for Mainsail 2.19.0 rather than a second
web application or a counterfeit Happy Hare object. Reuse its Panel, spool icon,
dialog, Vuex/Moonraker subscription and command infrastructure. Do not add a
server, direct UART polling, HTML injection or new transport/motion implementation.

The card shows physical channels 1–4, colored spool graphics, material and
loaded/parked/empty/uncertain/offline states. Channel metadata is not evidence of
physical loading. Editing validates material/color and submits the existing
native metadata command, never raw user-authored G-code. Disable editing when
the package/device is incompatible, disconnected, busy or printing/paused.
Read-only state remains visible. Keep keyboard access, non-color state labels,
responsive layout, the existing Mainsail theme and English/PT-BR labels.

Provide a reproducible build, provenance and a separate, explicitly invoked
panel installer with validation, backup and rollback. It must not silently
overwrite an unrelated Mainsail installation or auto-update printer firmware.

## Release boundary

Use English commands and EN/PT-BR operator documentation. Preserve README
brevity, credits, software licenses and all six model links. Bump VERSION to
2.0.1 and prepare release notes, validated build artifacts and CI gates. No
deployment, printer movement, heating, firmware flashing or route reset is
authorized. Report software validation separately from pending hardware tests.
