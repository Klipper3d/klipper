# Ender 3 V3 SE Port Analysis

**English** | [Português brasileiro](pt-BR/E3V3SE_Port_Analysis.md)

This document records the basis for the implementation. It is not a claim that
the historical forks should be merged wholesale.

## Compared revisions

- `Klipper3d/klipper`: `72b3cdb4`, current upstream at analysis time
- `jpcurti/ender3-v3-se-klipper-with-display`: `d74d36bb`, tag `1.0.1`
- `0xD34D/klipper_ender3_v3_se`: `7e61c1cc`
- `0xD34D/ender3-v3-se-klipper-config`: `deb2cfd`
- `huhen/Ender3_V3_SE_for_klipper`: `6649c4b0`, GPLv3

At analysis time the working repository was byte-for-byte at the official
Klipper head and had no local source changes. The `upstream` remote was then
added without replacing `origin`.

## Display fork delta

The display fork had 43 changed or added files relative to upstream, about
8,777 inserted lines. The principal additions were a 4,533-line display
module, a 530-line printer abstraction, the TJC driver, a generic serial
bridge, PRTouch, and documentation/assets.

The port retains behavior, not that structure. It does not retain changes to
`gcode_move.py`, `stepper.py`, generic G-code documentation, PRTouch, `hx711s`,
`dirzctl`, `filter`, old CI scripts, or a checked-in display binary.

Important display commits reviewed include:

- `cd1e82a7`, `2e08a2c`, `a34fd9a5`, `09ec47c7`: bridge and initial UI
- `c0cb2aa9`: large strings
- `bf50bf6c`, `4567e70a`, `fea6b700`: state and position synchronization
- `434275fe`, `bd9fbb67`, `b3f9bd3a`, `fc94f2aa`: folders and empty media
- `11e6dd26`, `459ee204`, `773e6073`, `78eb46f1`: manual probe and macros
- `b65d4028`: signed coordinates
- `6b75461c`, `2ce3670b`, `aacdcd6e`: status message handling
- `0f6b57c9`, `f31055ee`, `d716359b`: temperature controls
- `0c84c5ad`: migration to `register_serial_response()`

## Machine fork delta

The machine fork had ten changed or added files relative to upstream, about
1,383 inserted lines. All substantive changes implemented load-cell PRTouch:
`prtouch.py`, `hx711s.py`, `dirzctl.py`, their MCU sources, and supporting
patches to `stepper.py` and `src/Makefile`.

The separate 0xD34D configuration repository was useful for community hardware
cross-checking, but it has no declared license and is not the licensing basis
for the files in this repository. The electrical mapping is traceable to the
GPLv3 pinout and configuration published by huhen. It was checked against the
physically identified target board and then split into board, motion,
calibration, display, and macro files.

Travel and probe geometry were independently recalculated from the configured
machine envelope and probe offsets. In particular, the mesh maxima are the
230 mm nozzle maxima plus offsets of -23 mm and -14.5 mm. Machine-specific PID,
mesh, input-shaper, pressure-advance, and Z-offset results were removed.

The 0xD34D history credits bootuz-dinamon for TMC UART findings and FinalX1992
for correcting the screw thread direction. They are retained in the public
credits as hardware cross-check contributors; their unlicensed repositories
are not represented as GPL sources.

## Issue conclusions

Every open and closed issue was triaged. Technically relevant duplicates were
grouped by root cause:

- Bridge/build/version mismatch: display issues #4, #14, #15, #18, #20, #63,
  #70, #73, #74, #76, #77, #81, #90, #92, #93, #101, #119, #127, #134,
  #148, #152; machine issues #3, #6, #10, #14, #16, #25.
- Long names, UTF-8, and files: display #11, #28, #32, #35, #52, #98, #124,
  #133, #139, #152.
- Missing objects and stale state: display #3, #10, #12, #50, #57, #116;
  machine #24.
- Signed movement and unsafe macros: display #39, #41, #42, #50, #108, #136;
  machine #2, #5, #12, #20.
- Pause/resume/cancel and temperature: display #33, #55, #57, #79, #82, #121,
  #131.
- Z offset, mesh, and PRTouch: display #47, #49, #59, #85, #104; machine #1,
  #4, #5, #12, #20, #28, #29, #32 and PR #33.

The current PRTouch failure is not a one-line compatibility issue. Current
Klipper returns immutable `ProbeResult` values and changed the meaning of the Z
coordinate returned by `create_probe_result()`. Historical code also persists
a value different from the live `z_adjust`, has a speed- and temperature-
dependent trigger bias, and can interact incorrectly with an active bed mesh.
Community reports include physical bed damage. PRTouch is therefore excluded
from the initial safe profile.

## Upstream facilities used

- `MenuKeys` and `buttons`
- `register_serial_response()`
- `virtual_sdcard` and `print_stats`
- `display_status`
- `pause_resume`
- `gcode.run_script()`
- `bltouch`, `probe`, `manual_probe`, and `bed_mesh`
- Existing build version, Kconfig, and MCU dictionary reporting
- Official Input Shaping without modification

## Remaining physical uncertainties

- Creality has not supplied a public schematic tying every C14 revision to one
  MCU, bootloader, or oscillator.
- The target machine's GD32F303RET6 is physically confirmed, but upstream
  Klipper builds it through the STM32F103 compatibility path.
- The 28KiB bootloader and 8 MHz crystal are supported by working community
  builds, not a Creality hardware specification.
- Display firmware 1.0.6 is the validated asset/protocol baseline; other screen
  firmware versions remain unverified.

No software-only validation can remove those hardware uncertainties.
