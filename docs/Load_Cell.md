# Load Cells 7 Load Cell Probes

## Load Cells

This document describes Klipper's support for load cells. Basic load cell functionality can be used to read force data and to weigh things like filament. A calibrated force sensor is an important part of a load cell based probe.

### Related Documentation

* [load_cell Config Reference](Config_Reference.md#load_cell)
* [load_cell G-Code Commands](G-Codes.md#load_cell)
* [load_cell Status Reference](Status_Reference.md#load_cell)

### Using `LOAD_CELL_DIAGNOSTIC`

When you first connect a load cell its good practice to check for issues by running `LOAD_CELL_DIAGNOSTIC`. This tool collects 10 seconds of data from the load cell and reports statistics:

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

* The configured sample rate of the sensor should be close to the 'Measured samples per second' value. If it is not you may have a configuration or wiring issue.
* 'Saturated samples' should be 0. If you have saturated samples it means the load sell is seeing more force than it can measure.
* 'Unique values' should be a large percentage of the 'Samples Collected' value. If 'Unique values' is 1 it is very likely a wiring issue.
* Tap or push on the sensor while `LOAD_CELL_DIAGNOSTIC` runs. If things are working correctly this should increase the 'Sample range'.

### Calibrating a Load Cell

Load cells are calibrated using the
`LOAD_CELL_CALIBRATE` command. This is an interactive calibration utility that walks you though a 3 step process:

1. First use the `TARE` command to establish the zero force value. This is the `reference_tare_counts` config value.
2. Next you apply a known load or force to the load cell and run the `CALIBRATE GRAMS=nnn` command. From this the `counts_per_gram` value is calculated. See [the next section](#applying-a-known-force-or-load) for some suggestions on how to do this.
3. Finally, use the `ACCEPT` command to save the results.

You can cancel the calibration process at any time with `ABORT`.

#### Applying a Known Force or Load

The `CALIBRATE GRAMS=nnn` step can be accomplished in a number of ways. If your load cell is under a platform like a bed or filament holder it might be easiest to put a known mass on the platform. E.g. you could use a couple of 1KG filament spools.

If your load cell is in the printer's toolhead a different approach is easier. Put a digital scale on the printers bed and gently lower the toolhead onto the scale (or raise the bed into the toolhead if your bed moves). You may be able to do this using the `FORCE_MOVE` command. But more likely you will have to manually moving the z axis with the motors off until the toolhead presses on the scale.

A good calibration force would ideally be a large percentage of the load cell's rated capacity. E.g. if you have a 5Kg load cell you would ideally calibrate it with a 5kg mass. This might work well with under-bed sensors that have to support a lot of weight. For toolhead probes this may not be a load that your printer bed or toolhead can tolerate without damage. Do try to use at least 1Kg of force, most printers should tolerate this without issue.

When calibrating make careful note of the values reported:

```gcode
$ CALIBRATE GRAMS=555
// Calibration value: -2.78% (-59803108), Counts/gram: 73039.78739,
Total capacity: +/- 29.14Kg
```

The `Total capacity` should be close to the theoretical rating of the load cell based on the sensor's capacity. If it is much larger you could have used a higher gain setting in the sensor or a more sensitive load cell. This isn't as critical for 32bit and 24bit sensors but is much more critical for low bit width sensors.

### Reading Force Data

Force data can be read with a GCode command:

```gcode
LOAD_CELL_READ
// 10.6g (1.94%)
```

Data is also continuously read and can be consumed from the load_cell printer object in a macro:

```gcode
{% set grams = printer.load_cell.force_g %}
```

This provides an average force over the last 1 second, similar to how temperature sensors work.

### Taring a Load Cell

Taring, sometimes called zeroing, sets the current weight reported by the load_cell to 0. This is useful for measuring relative to a known weight. e.g. when measuring a filament spool, using `LOAD_CELL_TARE` sets the weight to 0. Then as filament is printed the load_cell will report the weight of the filament used.

```gcode
LOAD_CELL_TARE
// Load cell tare value: 5.32% (445903)
```

The current tare value is reported in the printers status and can be read in a macro:

```gcode
{% set tare_counts = printer.load_cell.tare_counts %}
```

## Load Cell Probes

#### Related Documentation

* [[load_cell_probe] Config Reference](Config_Reference.md#load_cell_probe)
* [[load_cell_probe] G-Code Commands](G-Codes.md#load_cell_probe)
* [[load_cell_probe] Status Reference](Status_Reference.md#load_cell_probe)

### Load Cell Probe Safety

Because load cells are a direct nozzle contact probe there is a risk of damage to your printer if too much force is used. The load cell probing system includes a number of safety checks that try to keep your machine safe from excessive force to the toolhead. It's important to understand what they are and how they work as you can defeat most of them with poorly chosen config values.

#### Calibration Check

Every time a probe starts, `[load_cell_probe]` checks that the load cell is calibrated. If not it will stop the move with an error: `!! Load Cell not calibrated`.

#### `counts_per_gram`

This setting is used to convert raw sensor counts into grams. All the safety limits are in gram units for your convenience. If the `counts_per_gram` setting is not accurate you can easily exceed the safe force on the toolhead. You should never guess this value. Use [`LOAD_CELL_CALIBRATE`](#calibrating-a-load-cell) to find your load cell's actual `counts_per_gram`.

#### `trigger_force`

This is the force in grams that triggers the probe to halt the homing move. When a probe starts the load cell is tared with the current reading. `trigger_force` is measured from that tare value. There is always some overshoot of this value when the probe collides with the bed, so be conservative. e.g. a setting of 100g could result in 350g of peak force before the toolhead stops. This overshoot will increase with faster probing speed, a low ADC sample rate. [multi MCU homing](Multi_MCU_Homing.md) also increases the time it takes to stop and thus the overshoot.

#### `reference_tare_counts`

This is the baseline tare value that is set by `LOAD_CELL_CALIBRATE`. This value works with `force_safety_limit` to limit the maximum force on the toolhead.

#### `force_safety_limit`

This is the maximum absolute force, relative to `reference_tare_counts`, that the probe will allow while homing or probing. If the MCU sees this force exceeded it will shut down the printer with the error `!! Load Cell Probe Error: load exceeds safety limit`. There are a number of ways this could be triggered:

The first risk this protects against is picking too large of a value for `drift_filter_cutoff_frequency`. This can cause the drift filter to filter out a probe collision event and continue the probing move. If this happens the `force_safety_limit` acts as a backup protection.

The second problem is probing repeatedly in one place. Klipper does not retract the probe when doing a single `PROBE` command. This can result in force applied to the toolhead at the end of a probing cycle. Because external forces can vary greatly between probing locations, `load_cell_probe` performs a tare before beginning each probe. If you repeat the `PROBE` command, load_cell_probe will tare at the current force. Multiple cycles of this will result in ever increasing force on the toolhead. `force_safety_limit` stops this cycle from running out of control.

Another way this run-away can happen is damage to a strain gauge. If the metal part is permanently bent it will change the `reference_tare_counts` of the device. This puts the starting tare value much closer to the limit making it more likely to be violated. You want to be notified if this is happening because your hardware has been permanently damaged.

The final way this can be triggered is due to temperature changes. If your strain gauges are heated their `reference_tare_counts` may be very different at ambient temperature vs operating temperature. In this case you may need to increase the `force_safety_limit` to allow for thermal changes.

#### Load Cell Probe Watchdog Task

When probing the `[load_cell_probe]` starts a task on the MCU to track measurements arriving from the sensor. If the sensor fails to send measurements for 2 sample periods the watchdog will shut down the printer with an error `!! Load Cell Probe Error: timed out waiting for sensor data`.

If this happens, the most likely cause is a fault from the ADC. Inadequate grounding of your printer can be the root cause. The frame, power supply case and pint bed should all be connected to ground. You may need to ground the frame in multiple places. Anodized aluminum extrusions do not conduct electricity well. You might need to sand the area where the grounding wire is attached to make good electrical contact.

### Setting up a Load Cell Probe

This section covers the process for commissioning a load cell probe.

### Verify the Load Cell First

A `[load_cell_probe]` is also a `[load_cell]` and G-code commands related to `[load_cell]` work with `[load_cell_probe]`. Before attempting to use a load cell probe, follow the directions for [calibrating the load cell](Load_Cell.md#calibrating-a-load-cell) with `LOAD_CELL_CALIBRATE` and checking its operation with `LOAD_CELL_DIAGNOSTIC`.

### Verify Probe Operation With LOAD_CELL_TEST_TAP

Use the command [`LOAD_CELL_TEST_TAP`](./G-Codes.md#load_cell_test_tap) to test the operation of the load cell probe before actually trying to probe with it. This command detects taps, just like the `PROBE` command will, but it does not move the z axis. By default, it listens for 3 taps before ending the test. You have 30 seconds to do each tap, if no taps are detected the command will time out.

If this test fails, check your configuration and `LOAD_CELL_DIAGNOSTIC` carefully to look for issues.

Load cell probes don't support the `QUERY_ENDSTOPS` or `QUERY_PROBE` commands. Use `LOAD_CELL_TEST_TAP` for testing functionality before probing.

### Homing Macros

Load cell probe is not an endstop and doesn't support `endstop: probe:z_virtual_endstop`. For the time being you'll need to configure your z axis with an MCU pin as its endstop. The pin will not actually be used but for the time being you have to configure something.

To home the axis with just the probe you need to set up a custom homing macro. This requires setting up a [homing_override](Config_Reference.md#homing_override).

Here is a pair of macros that can accomplish this. Note that the `_HOME_Z_FROM_LAST_PROBE` macro has to be separate because of the way macros work. The sub-call is needed so that the `_HOME_Z_FROM_LAST_PROBE` macro can see the result of the probe in `printer.probe.last_z_result`.

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

    # Fast approach and tap for the inital homing
    # BAD_TAP_STRATEGY=IGNORE forces the tap to succeed, even if its inaccurate
    PROBE PROBE_SPEED={5 * 60} BAD_TAP_STRATEGY=IGNORE
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

### Hot Nozzle Protection

The Voron project has a great macro for protecting your print surface from the hot nozzle. See [Voron Tap's `activate_gcode`](https://github.com/VoronDesign/Voron-Tap/blob/main/config/tap_klipper_instructions.md)

It is highly suggested to add something like this to your config.

### Temperature Compensation for Nozzle Growth

If you are probing at a safe temperature, the nozzle will expand after heating to printing temperatures. This will cause the nozzle to get longer and closer to the print surface. You can compensate for this with
[[z_thermal_adjust]](Config_Reference.md#z_thermal_adjust). This adjustment will work across a range of printing temperatures from PLA to PC.

#### Calculating the `temp_coeff` for `[z_thermal_adjust]`

The easiest way to do this is to measure at 2 different temperatures. Ideally these should be the upper and lower limits of the printing temperature range. E.g. 180C and 290C. You can perform a `PROBE_ACCURACY` at both temperatures and then calculate the difference of the `average z` at both.

The adjustment value is the change in nozzle length divided by the change in temperature. e.g.

```python
temp_coeff = -0.05 / (290 - 180) = -0.00045455
```

The expected result is a negative number. Positive values for `temp_coeff` move the nozzle closer to the bed and negative values move it further away. Expect to have to move the nozzle further away as it gets longer when hot.

Alternately there is a [macro](https://klipper.discourse.group/t/z-thermal-adjust-for-nozzles/22457#p-72934-testing-macro-4) that you can run which will determine the temperature coefficient with linear regression across multiple checks ad different temperatures.

#### Configure `[z_thermal_adjust]`

> WARNING: This requires this PR get merged to support multiple `[z_thermal_adjust]` sections: https://github.com/Klipper3d/klipper/pull/6855

Set up [`z_thermal_adjust`](Config_Reference.md#z_thermal_adjust) to reference the [`extruder`](Config_Reference.md#extruder) as the source of temperature data. E.g.:

```ini
[z_thermal_adjust nozzle]
temp_coeff: -0.00045455
sensor_type: temperature_combined
sensor_list: extruder
combination_method: max
min_temp: 0
max_temp: 290
max_z_adjustment: 0.1
```

## Continuous Tare Filters for Toolhead Load Cells

Klipper implements a configurable IIR filter on the MCU to provide continuous taring of the load cell while probing. The 0 value moves or drifts while probing. This can be caused by external factors like bowden tubes and thermal changes. Continuous Taring filters out these drifting signals and keep the tare value stable throughout the probe. This is primarily beneficial for toolhead sensors and moving beds that experience lots of external forces that change while probing.

### Installing SciPy

The filtering code uses the excellent [SciPy](https://scipy.org/) library to compute the filter coefficients based on the values your enter into the config.

Pre-compiled SciPy builds are available for Python 3 on Raspberry Pi images. Python 3 is strongly recommended because it will streamline your installation experience. It does work with Python 2 but installation can take 30+ minutes and require installing additional tools.

```bash
~/klippy-env/bin/pip install scipy
```

### Filter Workbench

The filter parameters should be selected based on drift seen on the printer during normal operation. A Jupyter notebook is provided in scripts, [filter_workbench.ipynb](../scripts/filter_workbench.ipynb), to perform a detailed investigation with real captured data and FFTs. That notebook contains the same math thats used by [load_cell_probe] and it can simulate the effect of filter configurations on your actual force data.

### Filtering Suggestions

For those just trying to get a filter working follow these suggestions:

* The only essential option is `drift_filter_cutoff_frequency`. A conservative starting value is `0.5`Hz. Prusa shipped the MK4 with a setting of `0.8`Hz and the XL with `11.2`Hz. This is probably a safe range to experiment with. This value should be increased only until normal drift due to bowden tube force is eliminated. Setting this value too high will result in slow triggering and excess force going through the toolhead.
* Keep `trigger_force` low. The default is `75`g. The drift filter keeps the internal grams value very close to 0 so a large trigger force is not needed.
* Keep `force_safety_limit` to a conservative value. The default value is 2Kg and should keep your toolhead safe while experimenting. If you hit this limit the `drift_filter_cutoff_frequency` value may be too high.

### Viewing Live Force & Tap Graphs

It can be extremely helpful to have a live view of the load cell force data and tap graphs while tuning your printer. You can see a live view using a web tool available here:

[Klipper Load Cell Debugging Tool](https://observablehq.com/@garethky/klipper-load-cell-debugging-tool)

The page loads from the secure [observablehq.com](http://observablehq.com)
domain. It doesn't send anything back to observablehq.com and communicates with Moonraker's websocket over your local network. You will need to set up HTTPS in Moonraker to get the websocket connection working. There are directions on that page for setting up SSL and CORS to make the connection.

#### Configuring the Page

There are 2 boxes on the page:

**Moonraker hostname**: enter the hostname of your printer without the `https://` or `:7130`. e.g. `voron24.local`

**`[load_cell]`, `[load_cell name], [load_cell_probe]`**: Enter the name of the config section that you want to monitor just as it appears in your config.

## Probing with a Load Cell

There are some real practical issues with using a 3D printer's nozzle as its probe tip. Mainly, it tends to be covered in plastic and this is the enemy of accuracy. There are 2 strategies for handling this: Warm Probing and Hot Probing. Each one has pros and cons and what you decide to use might depend on your printer, how you use it and how much automation you want.

### Warm Probing

In this strategy the nozzle is heated to 140C-150C. This is hot enough to make most plastics pliable but not hot enough for plastic to transfer to the print surface.

* Heat the nozzle to 140
* Manually clean the nozzle at the beginning of every print
* Use `bad_tap_strategy: RETRY`, the default
* Set up nozzle cleaner gcode

This is a good place to start, but it does require manual attention for every print.

### Nozzle cleaning and retrying bad taps for warm probes

When a tap fails, the default `bad_tap_strategy` is to retry that tap in the same location. For Warm probing the location on the bed is probably not fouled because the nozzle isn't hot enough to fully melt and transfer plastic. But its also not hot enough to resolve the issue on its own.

[load_cell_probe] support a `nozzle_cleaner_gcode` option. This is run when an invalid tap is detected during a probe using `bad_tap_strategy: RETRY`. You can do whatever you like in this macro but the toolhead must return to the same location after the macro completes.

If you have a nozzle brush you can automate scrubbing the nozzle on that. If you don't have one you can present the nozzle to the user at the front of the machine to be manually cleaned. Obviously this requires attention during print start.

### Nozzle Cleaner GCode

[load_cell_probe] support a `nozzle_cleaner_gcode` option. This is run when an invalid tap is detected during a probe using `bad_tap_strategy: RETRY`.

### Hot Probing

In this strategy the nozzle is heated to a temperature that is hot enough to successfully clean the nozzle but not hot enough to ooze continuously. This approach is the way to get a printer that doesn't require user attention before the print. While nozzle brushing and parking can be beneficial, they are not required to get this to work. E.g. the Prusa MK4, XL and Core One use this strategy to print without a nozzle brush.

* Set up [`[simple_tap_classifier]`](Config_Reference.md#simple_tap_classifier)
* Use `bad_tap_strategy: CIRCLE`
* Use `LOAD_CELL_CLEANUP` to verify nozzle is clean before meshing
* Set a filament specific probing temperature
* No manual intervention expected or required

This table has a good starting point for probing temperatures for common plastics:

| Plastic | Temperature C |
|---|---|
| PLA | 170 |
| PETG | 170 |
| ABS | 180 |
| ASA | 180 |
| PC | 240 |
| PVB | 140 |
| HIPS | 160 |
| PP | 160 |
| FLEX | 140 |

#### Retracting

You should consider adding some retractions to two spots in a print. The first is after the nozzle reaches probing temp but before probing.

```gcode
G1 E-2 F2400; retraction
```

The second is when the print end or is canceled. At that point filament tends to ooze out of the nozzle while it cools down. You can perform a retract and enable the print fan while the nozzle cools off:

```gcode
TURN_OFF_HEATERS # stop all heating
M106 S255 # print fan to maximum
G1 E-2 F2400 # retraction
TEMPERATURE_WAIT SENSOR=extruder MAXIMUM=140  # wait for temperature to drop
M106 S0 # print fan off
```

### Mixing Probing Strategies

In printers that require QGL, Trident / delta homing, you need to probe warm for homing.

## Hardware

## Suggestions for Load Cell Tool Boards

This section covers suggestions for those developing toolhead boards that want to support [load_cell_probe]

### ADC Sensor Selection & Board Development Hints

Ideally a sensor would meet these criteria:

* At least 24 bits wide
* Use SPI communications
* Has a pin that can be used to indicate sample ready without SPI communications. This is often called the "data ready" or "DRDY" pin. Checking a pin is much faster than running an SPI query.
* Has a programmable gain amplifier gain setting of 128. This should eliminate the need for a separate amplifier.
* Indicates via SPI if the sensor has been reset. Detecting resets avoids timing errors in probing and using noisy data at startup. It can also help users track down wiring and grounding issues.
* A selectable sample rate between 350Hz and 2kHz. Very high sample rates don't turn out to be beneficial in our 3D printers because they produce so much noise. Sample rates below 250Hz will require slower probing speeds. They also increase the force on the toolhead due to longer delays between measurements. E.g. a 500Hz sensor moving at 5mm/s has the same safety factor as a 100Hz sensor moving at only 1mm/s.
* If designing for under-bed applications, and you want to sense multiple load cells, use a chip that can sample all of its inputs simultaneously. Multiplex ADCs that require switching channels have a settling of several samples after each channel switch making them unsuitable for probing applications.

Implementing support for a new sensor chip is not particularly difficult with Klipper's
`bulk_sensor` and `load_cell_probe` infrastructure.

### 5V Power Filtering

It is strongly suggested to use larger capacitors than specified by the ADC chip manufacturer. ADC chips are usually targeted at low noise environments, like battery powered devices. Sensor manufacturers suggested application notes generally assume a quiet power supply. Treat their suggested capacitor values as minimums.

3D printers put huge amounts of noise onto the 5V bus and this can ruin the sensor's accuracy. Test the sensor on the board with a typical 3D printer power supply and active stepper drivers before deciding on smoothing capacitor sizes.

### Grounding & Ground Planes

Analog ADC chips contain components that are very vulnerable to noise and ESD. A large ground plane on the first board layer under the chip can help with noise. Keep the chip away from power sections and DC to DC converters. The board should have proper grounding back to the DC supply.

### HX711 and HX717 Notes

This sensor is popular because of its low cost and availability in the supply chain. However, this is a sensor with some drawbacks:

* The HX71x sensors use bit-bang communication which has a high overhead on the MCU. Using a sensor that communicates via SPI would save resources on the tool board's CPU.
* The HX71x lacks a way to communicate reset events to the MCU. Klipper detects resets with a timing heuristic but this is not ideal. Resets indicate a problem with wiring or grounding.
* For probing applications the HX717 version is strongly preferred because of its higher sample rate (320 vs 80). Probing speed on the HX711 should be limited to less than 2mm/s.
* The sample rate on the HX71x cannot be set from klipper's config. If you have the 10SPS version of the sensor (which is widely distributed) it needs to be physically rewired to run at 80SPS.

## Tap Analysis Explained

Load cell probe captures load cell data for a "tap". It's called a tap because the nozzle touches the print surface and then lifts away. This process has 3 phases: probe, dwell and pullback. The probe phase is similar to other probes; the nozzle moves towards the bed and then stops when the required amount of force is detected by the MCU. The dwell phase occurs while klipper is reconciling the position of the toolhead with the state on the host, during this time the toolhead is motionless. Last the pullback move slowly lifts the nozzle off the build surface, usually at a rate of 1 micron (0.001mm) per sensor sample. It's this pullback move that is used to deliver the high accuracy of the load cell probe.

<diagram: tap phases>

The entire tap is captured and broken down into a series of lines and points using elbow finding, linear regression and line intersections. Each line in the decomposition has a name, they are: Approach, Compression, Dwell, Decompression and Departure. The point where the approach and compression lines meet is where the nozzle first makes contact with the build surface. The point where the decompression and departure lines meet is the point where the nozzle stops touching the build surface.

<diagram: ideal tap with labeled lines>

Tap Analysis does a few simple checks on these lines and points:

* The points must be in order in time, from left to right on the graph. If they are out of chronological order the data is distorted and not usable.
* The set of lines must form a "tap shape", meaning the path of the graph follows a sequence of turns: left/right/right/left. Again, if this isn't true the data doesn't represent a tap.
* Finally, it checks that the pullback elbow, the last point, isn't too close to the end of the graph. If it is too close, this indicates that the pullback move was too short.

These checks identify taps that are unusable, meaning the data is so unrecognizable that it is very clearly not a tap. If you always tapped on a solid surface with a perfectly clean nozzle, these checks would always pass.

However, if you have some molten plastic on the nozzle there will be taps that this process will accept as good, which are in fact bad. This is where Tap Classifiers come in. They take the tap data and apply a set of rules to further weed out bad taps. Because they apply more stringent checks they may not work on all machines in all situations. Prusa's [classifier](https://github.com/prusa3d/Prusa-Firmware-Buddy/blob/818d812f954802903ea0ff39bf44376fb0b35dd2/src/common/probe_analysis.cpp#L444) uses machine learning trained on their printers and a [decision tree](https://scikit-learn.org/stable/modules/tree.html) to make its classifications. Because klipper is not a specific printer, we cant directly offer a machine learned solution. Instead, we offer [simple_tap_classifier](./Config_Reference.md#simple_tap_classifier) which applies a small set of user configurable rules which can detect most bad taps. Depending on the printer's configuration the parameters may need some tuning to get good results.
