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

# Load Cell Probes

## Related Documentation

* [load_cell_probe Config Reference](Config_Reference.md#load_cell_probe)
* [load_cell_probe G-Code Commands](G-Codes.md#load_cell_probe)
* [load_cell_probe Statuc Reference](Status_Reference.md#load_cell_probe)

## Load Cell Probe Safety

Because load cells are a direct nozzle contact probe there is a risk of
damage to your printer if too much force is used. The load cell probing system
includes a number of safety checks that try to keep your machine safe from
excessive force to the toolhead. It's important to understand what they are
and how they work as you can defeat most of them with poorly chosen config
values.

#### Calibration Check
Every time a homing move starts, load_cell_probe checks
that the load_cell is calibrated. If not it will stop the move with an error:
`!! Load Cell not calibrated`.

#### `counts_per_gram`
This setting is used to convert raw sensor counts into grams. All the safety
limits are in gram units for your convenience. If the `counts_per_gram`
setting is not accurate you can easily exceed the safe force on the toolhead.
You should never guess this value. Use `LOAD_CELL_CALIBRATE` to find your load
cells actual `counts_per_gram`.

#### `trigger_force`
This is the force in grams that triggers the endstop to halt the homing move.
When a homing move starts the endstop tares itself with the current reading
from the load cell. `trigger_force` is measured from that tare value. There is
always some overshoot of this value when the probe collides with the bed,
so be conservative. e.g. a setting of 100g could result in 350g of peak force
before the toolhead stops. This overshoot will increase with faster probing
`speed`, a low ADC sample rate or [multi MCU homing](Multi_MCU_Homing.md).

#### `reference_tare_counts`
This is the baseline tare value that is set by `LOAD_CELL_CALIBRATE`.
This value works with `force_safety_limit` to limit the maximum force on the
toolhead.

#### `force_safety_limit`
This is the maximum absolute force, relative to `reference_tare_counts`,
that the probe will allow while homing or probing. If the MCU sees this
force exceeded it will shut down the printer with the error `!! Load cell
endstop: too much force!`. There are a number of ways this can be triggered:

The first risk this protects against is picking too large of a value for
`drift_filter_cutoff_frequency`. This can cause the drift filter to filter out
a probe event and continue the homing move. If this happens the
`force_safety_limit` acts as a backup protection.

The second problem is probing repeatedly in one place. Klipper does not retract
the probe when doing a single `PROBE` command. This can result
in force applied to the toolhead at the end of a probing cycle. Because
external forces can vary greatly between probing locations,
`load_cell_probe` performs a tare before beginning each probe. If you repeat
the `PROBE` command, load_cell_probe will tare the endstop at the current force.
Multiple cycles of this will result in ever-increasing force on the toolhead.
`force_safety_limit` stops this cycle from running out of control.

Another way this run-away can happen is damage to a strain gauge. If the metal
part is permanently bent it wil change the `reference_tare_counts` of the
device. This puts the starting tare value much closer to the limit making it
more likely to be violated. You want to be notified if this is happening
because your hardware has been permanently damaged.

The final way this can be triggered is due to temperature changes. If your
strain gauges are heated their `reference_tare_counts` may be very different
at ambient temperature vs operating temperature. In this case you may need
to increase the `force_safety_limit` to allow for thermal changes.

#### Load Cell Endstop Watchdog Task
When homing the load_cell_endstop starts a task on the MCU to trac
measurements arriving from the sensor. If the sensor fails to send
measurements for 2 sample periods the watchdog will shut down the printer
with an error `!! LoadCell Endstop timed out waiting on ADC data`.

If this happens, the most likely cause is a fault from the ADC. Inadequate
grounding of your printer can be the root cause. The frame, power supply
case and pint bed should all be connected to ground. You may need to ground
the frame in multiple places. Anodized aluminum extrusions do not conduct
electricity well. You might need to sand the area where the grounding wire
is attached to make good electrical contact.

## Load Cell Probe Setup

This section covers the process for commissioning a load cell probe.

### Verify the Load Cell First

A `[load_cell_probe]` is also a `[load_cell]` and G-code commands related to
`[load_cell]` work with `[load_cell_probe]`. Before attempting to use a load
cell probe, follow the directions for
[calibrating the load cell](Load_Cell.md#calibrating-a-load-cell) with
`CALIBRATE_LOAD_CELL` and checking its operation with `LOAD_CELL_DIAGNOSTIC`.

### Verify Probe Operation With LOAD_CELL_TEST_TAP

Use the command `LOAD_CELL_TEST_TAP` to test the operation of the load cell
probe before actually trying to probe with it. This command detects taps,
just like the PROBE command, but it does not move the z axis. By default, it
listens for 3 taps before ending the test. You have 30 seconds to do each
tap, if no taps are detected the command will time out.

If this test fails, check your configuration and `LOAD_CELL_DIAGNOSTIC`
carefully to look for issues.

Load cell probes don't support the `QUERY_ENDSTOPS` or `QUERY_PROBE`
commands. Use `LOAD_CELL_TEST_TAP` for testing functionality before probing.

### Homing Macros

Load cell probe is not an endstop and doesn't support `endstop:
prove:z_virtual_endstop`. For the time being you'll need to configure your z
axis with an MCU pin as its endstop. You won't actually be using the pin but
for the time being you have to configure something.

To home the axis with just the probe you need to set up a custom homing
macro. This requires setting up
[homing_override](Config_Reference.md#homing_override).

Here is a simple macro that can accomplish this. Note that the
`_HOME_Z_FROM_LAST_PROBE` macro has to be separate because of the way macros
work. The sub-call is needed so that the `_HOME_Z_FROM_LAST_PROBE` macro can
see the result of the probe in `printer.probe.last_z_result`.

```gcode
[gcode_macro _HOME_Z_FROM_LAST_PROBE]
gcode:
    {% set z_probed = printer.probe.last_z_result %}
    {% set z_position = printer.toolhead.position[2] %}
    {% set z_actual = z_position - z_probed %}
    SET_KINEMATIC_POSITION Z={z_actual}

[gcode_macro _HOME_Z]
gcode:
    SET_GCODE_OFFSET Z=0  # load cell probes dont need a Z offset
    # position toolhead for homing Z, edit for your printers size
    #G90  # absolute move
    #G1 Y50 X50 F{5 * 60}  # move to X/Y position for homing

    # soft home the z axis to its limit so it can be moved:
    SET_KINEMATIC_POSITION Z={printer.toolhead.axis_maximum[2]}

    # Fast approach and tap
    PROBE PROBE_SPEED={5 * 60}  # override the speed for faster homing
    _HOME_Z_FROM_LAST_PROBE

    # lift z to 2mm
    G91  # relative move
    G1 Z2 F{5 * 60}

    # probe at standard speed
    PROBE
    _HOME_Z_FROM_LAST_PROBE

    # lift z to 10mm for clearance
    G91  # relative move
    G1 Z10 F{5 * 60}
```

### Suggested Probing Temperature

Currently, we suggest keeping the nozzle temperature below the level that causes
the filament to ooze while homing and probing. 140C is a good starting
point. This temperature is also low enough not to scar PEI build surfaces.

Fouling of the nozzle and the print bed due to oozing filament is the #1 source
of probing error with the load cell probe. Klipper does not yet have a universal
way to detect poor quality taps due to filament ooze. The existing code may
decide that a tap is valid when it is of poor quality. Classifying these poor
quality taps is an area of active research.

Klipper also lacks support for re-locating a probe point if the
location has become fouled by filament ooze. Modules like `quad_gantry_level`
will repeatedly probe the same coordinates even if a probe previously failed
there.

Give the above it is strongly suggested not to probe at printing temperatures.

### Hot Nozzle Protection

The Voron project has a great macro for protecting your print surface from the
hot nozzle. See [Voron Tap's
`activate_gcode`](https://github.com/VoronDesign/Voron-Tap/blob/main/config/tap_klipper_instructions.md)

It is highly suggested to add something like this to your config.

### Nozzle Cleaning

Before probing the nozzle should be clean. You could do this manually before
every print. You can also implement a nozzle scrubber and automate the process.
Here is a suggested sequence:

1. Wait for the nozzle to heat up to probing temp (e.g. `M109 S140`)
1. Home the machine (`G28`)
1. Scrub the nozzle on a brush
1. Heat soak the print bed
1. Perform probing tasks: QGL, bed mesh etc.

### Temperature Compensation for Nozzle Growth

If you are probing at a safe temperature, the nozzle will expand after
heating to printing temperatures. This will cause the nozzle to get longer
and closer to the print surface. You can compensate for this with
[[z_thermal_adjust]](Config_Reference.md#z_thermal_adjust). This adjustment will
work across a range of printing
temperatures from PLA to PC.

#### Calculating the `temp_coeff` for `[z_thermal_adjust]`

The easiest way to do this is to measure at 2 different temperatures.
Ideally these should be the upper and lower limits of the printing
temperature range. E.g. 180C and 290C. You can perform a `PROBE_ACCURACY` at
both temperatures and then calculate the difference of the `average z` at both.

The adjustment value is the change in nozzle length divided by the change in
temperature. e.g.

```
temp_coeff = -0.05 / (290 - 180) = -0.00045455
```

The expected result is a negative number. Positive values for `temp_coeff` move
the nozzle closer to the bed and negative values move it further away.
Expect to have to move the nozzle further away as it gets longer when hot.

#### Configure `[z_thermal_adjust]`
Set up z_thermal_adjust to reference the `extruder` as the source of temperature
data. E.g.:

```
[z_thermal_adjust nozzle]
temp_coeff=-0.00045455
sensor_type: temperature_combined
sensor_list: extruder
combination_method: max
min_temp: 0
max_temp: 400
max_z_adjustment: 0.1
```

## Continuous Tare Filters for Toolhead Load Cells

Klipper implements a configurable IIR filter on the MCU to provide continuous
tareing of the load cell while probing. Continuous taring means the 0 value
moves with drift caused by external factors like bowden tubes and thermal
changes. This is aimed at toolhead sensors and moving beds that experience lots
of external forces that change while probing.

### Installing SciPy

The filtering code uses the excellent [SciPy](https://scipy.org/) library to
compute the filter coefficients based on the values your enter into the config.

Pre-compiled SciPi builds are available for Python 3 on 32 bit Raspberry Pi
systems. 32 bit + Python 3 is strongly recommended because it will streamline
your installation experience. It does work with Python 2 but installation can
take 30+ minutes and require installing additional tools.

```bash
~/klippy-env/bin/pip install scipy
```

### Filter Workbench

The filter parameters should be selected based on drift seen on the printer
during normal operation. A Jupyter notebook is provided in scripts,
[filter_workbench.ipynb](../scripts/filter_workbench.ipynb), to perform a
detailed investigation with real captured data and FFTs.

### Filtering Suggestions

For those just trying to get a filter working follow these suggestions:

* The only essential option is `drift_filter_cutoff_frequency`. A conservative
  starting value is `0.5`Hz. Prusa shipped the MK4 with a setting of `0.8`Hz and
  the XL with `11.2`Hz. This is probably a safe range to experiment with. This
  value should be increased only until normal drift due to bowden tube force is
  eliminated. Setting this value too high will result in slow triggering and
  excess force going through the toolhead.
* Keep `trigger_force` low. The default is `75`g. The drift filter keeps the
  internal grams value very close to 0 so a large trigger force is not needed.
* Keep `force_safety_limit` to a conservative value. The default value is 2Kg
  and should keep your toolhead safe while experimenting. If you hit this limit
  the `drift_filter_cutoff_frequency` value may be too high.

## Suggestions for Load Cell Tool Boards

This section covers suggestions for those developing toolhead boards that want
to support [load_cell_probe]

### ADC Sensor Selection & Board Development Hints

Ideally a sensor would meet these criteria:

* At least 24 bits wide
* Use SPI communications
* Has a pin can be used to indicate sample ready without SPI communications.
  This is often called the "data ready" or "DRDY" pin. Checking a pin is much
  faster than running an SPI query.
* Has a programmable gain amplifier gain setting of 128. This should eliminate
  the need for a separate amplifier.
* Indicates via SPI if the sensor has been reset. Detecting resets avoids
  timing errors in homing and using noisy data at startup. It can also help
  users
  track down wiring and grounding issues.
* A selectable sample rate between 350Hz and 2Khz. Very high sample rates don't
  turn out to be beneficial in our 3D printers because they produce so much
  noise
  when moving fast. Sample rates below 250Hz will require slower probing speeds.
  They also increase the force on the toolhead due to longer delays between
  measurements. E.g. a 500Hz sensor moving at 5mm/s has the same safety factor
  as
  a 100Hz sensor moving at only 1mm/s.
* If designing for under-bed applications, and you want to sense multiple load
  cells, use a chip that can sample all of its inputs simultaneously. Multiplex
  ADCs that require switching channels have a settling of several samples after
  each channel switch making them unsuitable for probing applications.

Implementing support for a new sensor chip is not particularly difficult with
Klipper's `bulk_sensor` and `load_cell_endstop` infrastructure.

### 5V Power Filtering

It is strongly suggested to use larger capacitors than specified by the ADC chip
manufacturer. ADC chips are usually targeted at low noise environments, like
battery powered devices. Sensor manufacturers suggested application notes
generally assume a quiet power supply. Treat their suggested capacitor values as
minimums.

3D printers put huge amounts of noise onto the 5V bus and this can ruin the
sensor's accuracy. Test the sensor on the board with a typical 3D printer power
supply and active stepper drivers before deciding on smoothing capacitor sizes.

### Grounding & Ground Planes

Analog ADC chips contain components that are very vulnerable to noise and
ESD. A large ground plane on the first board layer under the chip can help with
noise. Keep the chip away from power sections and DC to DC converters. The board
should have proper grounding back to the DC supply.

### HX711 and HX717 Notes

This sensor is popular because of its low cost and availability in the
supply chain. However, this is a sensor with some drawbacks:

* The HX71x sensors use bit-bang communication which has a high overhead on the
  MCU. Using a sensor that communicates via SPI would save resources on the tool
  board's CPU.
* The HX71x lacks a way to communicate reset events to the MCU. Klipper detects
  resets with a timing heuristic but this is not ideal. Resets indicate a
  problem with wiring or grounding.
* For probing applications the HX717 version is strongly preferred because
  of its higher sample rate (320 vs 80). Probing speed on the HX711 should be
  limited to less than 2mm/s.
* The sample rate on the HX71x cannot be set from klipper's config. If you have
  the 10SPS version of the sensor (which is widely distributed) it needs to
  be physically re-wired to run at 80SPS.
