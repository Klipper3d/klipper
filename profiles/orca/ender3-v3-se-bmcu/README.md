# OrcaSlicer 2.4.2 — Ender 3 V3 SE with BMCU

**English** | [Português brasileiro](README.pt-BR.md)

## v2 profile: re-slice existing jobs

This branch prepares v2.0.0; it is not a published or hardware-validated v2
release. Its commands are English only. **Re-slice old jobs using this profile**
after completing the installation migration. Previously sliced jobs containing
Portuguese commands are incompatible. There are no Portuguese command aliases
and no automatic conversion of stored print jobs. Do not mix snippets from v1
and v2 or start a queued v1 job after upgrading.

Use these snippets with the [switch-based integration](../../../docs/Ender3_V3_SE_Configuracao_Validada.md).
They are reusable examples, not a personal slicer export or a native importable
preset bundle. Replace `YOUR_BMCU_UID` in `start.gcode` with the UID reported by
your own BMCU. Never use another machine's UID.

| Printer setting | Value |
| --- | --- |
| Start G-code | Contents of `start.gcode` |
| End G-code | Contents of `end.gcode` |
| Filament change G-code | Contents of `change.gcode` |
| Pause | `PAUSE` |
| Firmware | Klipper |
| Physical extruders | One, with single-extruder multimaterial enabled |
| Manual changes and prime tower | Disabled in the tested chute/wiper setup |
| Tool-change retraction, cooling tube and extra loading | Zero; integration owns these phases |
| Ramming and cooling moves | Disabled |

Save this as a user printer profile. Do not append another home, temperature
sequence or prime line after `ENDER_START_MULTICOLOR`. Do not use `M600` for
BMCU multi-color tool changes.

The start macro uses `BED_TEMP`, `NOZZLE_TEMP` and `TOOL`; the end macro is
`ENDER_END_MULTICOLOR`. Keep these names and their values in English in both
documentation languages and both display languages.

| Orca slot | Explicit command | Default physical channel | BMCU API index |
| --- | --- | --- | --- |
| 1 | `T0 B1` | 1 | 0 |
| 2 | `T1 B2` | 2 | 1 |
| 3 | `T2 B3` | 3 | 2 |
| 4 | `T3 B4` | 4 | 3 |

With a committed print map, the integration also accepts automatic `T0` through
`T3` without `B` as logical tools 1 through 4. BMCU resolves their physical
routes. Outside that context, native tools retain their original meaning;
`T0` selects external feeding. An absent tool in an active map produces an
error rather than selecting a different source. Re-selecting a loaded tool
does not add another purge.

Keep `T{next_extruder} B{next_extruder+1}` in the custom change field. No
computer-side post-processing script is required; remove
`corrigir_canais_bmcu.ps1` from the process profile if previously configured.

`example-settings.json` contains reference settings, not a complete preset or
proven hotend limit. The tested PETG example used 230 °C, bed 80/75 °C, PA 0.04,
flow 1.00 and a slicer volumetric limit of 20 mm³/s. Calibrate material and flow
for your own hotend; input shaping and maximum-speed tuning are separate tasks.

## Four-channel reload project

[`bmcu_four_channel_reload_test.3mf`](bmcu_four_channel_reload_test.3mf) contains
four cycles of two layers per channel: 16 loads, 15 changes and three reloads
per channel. Open it as a project, review every setting, replace the placeholder
UID and slice it with the v2 profile. It does not contain ready-to-print G-code
and is not evidence of successful four-color physical testing.
