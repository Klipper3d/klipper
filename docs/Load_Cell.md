# Load Cells

This document describes Klipper's support for load cells. Basic load cell
functionality can be used to read force data and to weigh things like filament.
A calibrated force sensor is an important part of a load cell based probe.

## Related Documentation

* [load_cell Config Reference](Config_Reference.md#load_cell)
* [load_cell G-Code Commands](G-Codes.md#load_cell)
* [load_cell Status Reference](Status_Reference.md#load_cell)

## Using `LOAD_CELL_DIAGNOSTIC`

When you first connect a load cell its good practice to check for issues by
running `LOAD_CELL_DIAGNOSTIC`. This tool collects 10 seconds of data from the
load cell and resport statistics:

```
$ LOAD_CELL_DIAGNOSTIC
// Collecting load cell data for 10 seconds...
// Samples Collected: 3211
// Measured samples per second: 332.0
// Good samples: 3211, Saturated samples: 0, Unique values: 900
// Sample range: [4.01% to 4.02%]
// Sample range / sensor capacity: 0.00524%
```

Things you can check with this data:
* The configured sample rate of the sensor should be close to the 'Measured
samples per second' value. If it is not you may have a configuration or wiring
issue.
* 'Saturated samples' should be 0. If you have saturated samples it means the
load sell is seeing more force than it can measure.
* 'Unique values' should be a large percentage of the 'Samples
Collected' value. If 'Unique values' is 1 it is very likely a wiring issue.
* Tap or push on the sensor while `LOAD_CELL_DIAGNOSTIC` runs. If
things are working correctly ths should increase the 'Sample range'.

## Calibrating a Load Cell

Load cells are calibrated using the `LOAD_CELL_CALIBRATE` command. This is an
interactive calibration utility that walks you though a 3 step process:
1. First use the `TARE` command to establish the zero force value. This is the
`reference_tare_counts` config value.
2. Next you apply a known load or force to the load cell and run the
`CALIBRATE GRAMS=nnn` command. From this the `counts_per_gram` value is
calculated. See [the next section](#applying-a-known-force-or-load) for some
suggestions on how to do this.
3. Finally, use the `ACCEPT` command to save the results.

You can cancel the calibration process at any time with `ABORT`.

### Applying a Known Force or Load

The `CALIBRATE GRAMS=nnn` step can be accomplished in a number of ways. If your
load cell is under a platform like a bed or filament holder it might be easiest
to put a known mass on the platform. E.g. you could use a couple of 1KG filament
spools.

If your load cell is in the printer's toolhead a different approach is easier.
Put a digital scale on the printers bed and gently lower the toolhead onto the
scale (or raise the bed into the toolhead if your bed moves). You may be able to
do this using the `FORCE_MOVE` command. But more likely you will have to
manually moving the z axis with the motors off until the toolhead presses on the
scale.

A good calibration force would ideally be a large percentage of the load cell's
rated capacity. E.g. if you have a 5Kg load cell you would ideally calibrate it
with a 5kg mass. This might work well with under-bed sensors that have to
support a lot of weight. For toolhead probes this may not be a load that your
printer bed or toolhead can tolerate without damage. Do try to use at least 1Kg
of force, most printers should tolerate this without issue.

When calibrating make careful note of the values reported:
```
$ CALIBRATE GRAMS=555
// Calibration value: -2.78% (-59803108), Counts/gram: 73039.78739,
Total capacity: +/- 29.14Kg
```
The `Total capacity` should be close to the theoretical rating of the load cell
based on the sensor's capacity. If it is much larger you could have used a
higher gain setting in the sensor or a more sensitive load cell. This isn't as
critical for 32bit and 24bit sensors but is much more critical for low bit width
sensors.

## Reading Force Data
Force data can be read with a GCode command:

```
LOAD_CELL_READ
// 10.6g (1.94%)
```

Data is also continuously read and can be consumed from the load_cell printer
object in a macro:

```
{% set grams = printer.load_cell.force_g %}
```

This provides an average force over the last 1 second, similar to how
temperature sensors work.

## Taring a Load Cell
Taring, sometimes called zeroing, sets the current weight reported by the
load_cell to 0. This is useful for measuring relative to a known weight. e.g.
when measuring a filament spool, using `LOAD_CELL_TARE` sets the weight to 0.
Then as filament is printed the load_cell will report the weight of the
filament used.

```
LOAD_CELL_TARE
// Load cell tare value: 5.32% (445903)
```

The current tare value is reported in the printers status and can be read in
a macro:

```
{% set tare_counts = printer.load_cell.tare_counts %}
```
