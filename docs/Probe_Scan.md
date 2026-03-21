# Probe Scan

This document describes the probe_scan module for continuous-motion bed
mesh scanning with a binary (on/off) inductive probe.

## Overview

Standard bed mesh probing stops at each grid point, lowers Z until the
probe triggers, records the height, retracts, and moves to the next
point. probe_scan eliminates this stop-and-go pattern by keeping the
toolhead in continuous motion while oscillating Z, collecting probe
events in real time.

The toolhead traverses the bed in a serpentine (back-and-forth) pattern
while Z oscillates up and down. The MCU firmware polls the probe GPIO at
a configurable interval (default 50 microseconds) and timestamps each
state transition. The host correlates these timestamps with toolhead
position via Klipper's stepper history (trapq lookup), producing
scattered (x, y, z) measurements across the bed. These scattered points
are interpolated onto the bed_mesh grid using Inverse Distance Weighting
(IDW).

Trigger events (probe descending into detection range) give the bed
height directly. Untrigger events (probe ascending out of detection
range) are corrected by subtracting the measured hysteresis.

### Hysteresis measurement

Inductive probes have hysteresis: the Z height where the probe triggers
(descending) differs from the height where it untriggers (ascending).
probe_scan measures this offset before each scanning session.

The measurement procedure:
1. A standard probing_move finds the approximate trigger height.
2. The toolhead oscillates Z continuously for 8 cycles at `z_speed`.
3. The MCU collects trigger/untrigger events during the oscillation.
4. Events are paired sequentially (each trigger with its following
   untrigger). The first pair is discarded (contaminated by the initial
   approach).
5. The median of 7 hysteresis samples is used.

The hysteresis value is cached for the Klipper session. Subsequent scans
reuse the cached value without re-measuring.

### Scan procedure

1. **Approach**: A standard probing_move at the first mesh point
   establishes the bed contact height.
2. **Z range computation**: `z_bottom = z_offset - margin`,
   `z_top = z_offset + hysteresis + margin`.
3. **Path queuing**: The entire serpentine path is queued to the motion
   planner before motion starts. This ensures continuous planner motion
   with no gaps or stutters.
4. **Event collection**: The MCU collects probe events during motion.
   The host periodically correlates events with toolhead positions
   before Klipper's stepper history expires (approximately 30 seconds of
   retention).
5. **Mesh generation**: After motion completes, the scattered probe
   points are interpolated onto the bed_mesh grid using IDW.

## Prerequisites

- A binary (on/off) inductive probe (NPN NC recommended, e.g., Super
  Pinda, Vinda, SupCR, Decoprobe)
- Working `[probe]` and `[bed_mesh]` configuration sections

## Configuration

Add a `[probe_scan]` section to `printer.cfg`:

```
[probe_scan]
pin: ^probe_pin
#   GPIO pin for the probe. Supports ^ (pullup) and ! (invert) prefixes.
#   Use the same pin as [probe]. This parameter must be provided.
x_offset: -27.8
#   Probe X offset from nozzle in mm. Must match your physical setup.
#   The default is 0.
y_offset: -12
#   Probe Y offset from nozzle in mm. Must match your physical setup.
#   The default is 0.
#speed: 50
#   XY movement speed during scanning in mm/s. The default is 50.
#z_speed: 5
#   Z oscillation speed during scanning and probing speed during
#   hysteresis measurement in mm/s. The default is 5.
#z_margin: 0.1
#   Z margin above z_top and below z_bottom of the oscillation range
#   in mm. Larger values increase the oscillation amplitude and produce
#   more trigger events, at the cost of slower scanning. The default
#   is 0.1.
#segment_length: 2.0
#   XY distance per motion segment in mm. Smaller values produce smoother
#   Z oscillation but increase motion planner load. Minimum 0.5.
#   The default is 2.0.
#poll_us: 50
#   MCU GPIO polling interval in microseconds. Lower values increase
#   timing resolution but increase MCU load. The default is 50.
#max_hysteresis: 0.5
#   Maximum acceptable hysteresis value in mm. The module raises an
#   error if the measured hysteresis exceeds this value. The default
#   is 0.5.
#probe_z_min: 0
#   Lowest Z position for probing moves in mm. The default is 0.
#calibration_point:
#   Optional X,Y coordinates for hysteresis measurement (comma-separated).
#   If not specified, uses the center of the bed_mesh area.
#search_radius: 35
#   IDW interpolation search radius in mm. Probe data points beyond this
#   distance from a grid point are ignored during interpolation. For
#   dense meshes (11x11+), 35 mm works well. For sparse meshes, increase
#   to ensure coverage. The default is 35.
#cross_hatch: False
#   When enabled, scans the bed in both X and Y directions (cross-hatch
#   pattern) instead of X-only serpentine. Produces more uniform spatial
#   coverage at the cost of slightly longer scan time. Recommended for
#   large beds or when single-axis scans leave coverage gaps. When using
#   cross-hatch, increase search_radius to 50 or more. The default is
#   False.
#cross_hatch_rows: 7
#   Number of X-direction scan rows for cross-hatch mode. Rows are
#   evenly spaced across the bed, independent of the mesh grid. The
#   default is 7.
#cross_hatch_cols: 7
#   Number of Y-direction scan columns for cross-hatch mode. Columns
#   are evenly spaced across the bed, independent of the mesh grid. The
#   default is 7.
```

The `z_offset` is read from the `[probe]` section so it stays in sync
with `PROBE_CALIBRATE` and `SAVE_CONFIG`.

## G-Code commands

### BED_MESH_CALIBRATE METHOD=probe_scan

`BED_MESH_CALIBRATE METHOD=probe_scan [SKIP_APPROACH=<0|1>]
[MESH=<0|1>] [PROFILE=<name>]`: Performs a full continuous-motion bed
mesh scan using the standard `BED_MESH_CALIBRATE` entry point. All
standard `BED_MESH_CALIBRATE` parameters (e.g., `PROFILE`,
`MESH_MIN`, `MESH_MAX`) are supported.

Measures hysteresis if not already cached, approaches the bed at the
first mesh point, scans the entire bed in a serpentine pattern, and
generates a bed mesh.

`SKIP_APPROACH=1` skips the initial probe approach (use with caution;
requires the toolhead to already be at an appropriate Z height).
`MESH=0` skips mesh interpolation and outputs raw scan data only.

Scan data is written to `~/printer_data/logs/probe_scan_data.txt` for
offline analysis.

### PROBE_SCAN_CALIBRATE

`PROBE_SCAN_CALIBRATE`: Calibrates the probe by measuring hysteresis,
without performing a scan. Useful for diagnostics or pre-caching the
hysteresis value before a scan.

### PROBE_SCAN_TEST

`PROBE_SCAN_TEST [X=<pos>] [Y=<pos>] [LENGTH=<mm>] [Z=<height>]
[SKIP_APPROACH=<0|1>]`: Runs a short single-line scan for verifying
probe operation and tuning parameters.

Default parameters: `X=30`, `Y=0`, `LENGTH=30`, `Z=<z_offset>`.

### PROBE_SCAN_DIAG

`PROBE_SCAN_DIAG`: Diagnostic command that performs a single
trigger/untrigger cycle using probing moves and reports the raw MCU
events. Useful for verifying that the MCU firmware is collecting events
correctly.

## Usage

### Basic usage

After homing, run:
```
BED_MESH_CALIBRATE METHOD=probe_scan
```

This measures hysteresis (first run only per session), approaches the
bed, scans the entire mesh area, and saves the resulting mesh.

### Integration with print start macros

To use probe_scan as the default mesh method, wrap
`BED_MESH_CALIBRATE` with a macro that injects `METHOD=probe_scan`:
```
[gcode_macro BED_MESH_CALIBRATE]
rename_existing: _BED_MESH_CALIBRATE
gcode:
    {% set ns = namespace(filtered="") %}
    {% for item in rawparams.split() %}
        {% if not item.upper().startswith("METHOD=") %}
            {% set ns.filtered = ns.filtered ~ " " ~ item %}
        {% endif %}
    {% endfor %}
    _BED_MESH_CALIBRATE METHOD=probe_scan{ns.filtered}
```

### Tuning

- **speed**: Higher speeds complete the scan faster but produce fewer
  data points per unit area. 100-150 mm/s works well for most setups.
- **z_speed**: Controls Z oscillation frequency. Too slow produces few
  trigger events per scan row. 10-15 mm/s is a typical range.
- **z_margin**: Increase if scans produce "too few trigger events"
  errors. Decrease for tighter oscillation (faster scans, but requires
  a flat bed and accurate z_offset).
- **segment_length**: Smaller values produce smoother Z oscillation but
  increase motion planner load. 2.0 mm is a good starting point.
- **search_radius**: For dense meshes (11x11 or larger), 35 mm provides
  good coverage. For sparse meshes (e.g., 3x3), increase to ensure
  every grid point has nearby probe data.

## Troubleshooting

- **"Too few trigger events"**: The probe triggered fewer than 3 times
  during the scan. Increase `z_margin` or decrease `speed` to
  produce more oscillation cycles per scan row.

- **"Measured hysteresis exceeds max_hysteresis"**: The measured
  trigger/untrigger gap exceeds the `max_hysteresis` safety limit. Check
  probe mounting height, wiring, and the `max_hysteresis` setting.

- **"Negative hysteresis"**: The untrigger height is below the trigger
  height, which indicates a probe malfunction. Check wiring and pin
  inversion settings (`!` prefix).

- **"Not enough hysteresis event pairs"**: Z oscillation during
  hysteresis measurement did not produce enough trigger/untrigger cycles.
  Verify that `z_speed` is reasonable and the probe triggers reliably.

- **"Timed out waiting for scan motion to complete"**: Motion took
  longer than 120 seconds. Check for mechanical binding, MCU
  communication issues, or an unusually large mesh area.

- **"No probe data near grid point (X, Y)"**: IDW interpolation found
  no probe data points within `search_radius` of a mesh grid point.
  Increase `search_radius` or increase `z_margin` to generate more
  probe events.
