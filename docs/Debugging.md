The Klippy host code has some tools to help in debugging.

Translating gcode files to micro-controller commands
====================================================

The Klippy host code can run in a batch mode to produce the low-level
micro-controller commands associated with a gcode file. Inspecting
these low-level commands is useful when trying to understand the
actions of the low-level hardware. It can also be useful to compare
the difference in micro-controller commands after a code change.

To run Klippy in this batch mode, there is a one time step necessary
to generate the micro-controller "data dictionary". This is done by
compiling the micro-controller code to obtain the **out/klipper.dict**
file:

```
make menuconfig
make
```

Once the above is done it is possible to run Klipper in batch mode
(see [installation](Installation.md) for the steps necessary to build
the python virtual environment and a printer.cfg file):

```
~/klippy-env/bin/python ./klippy/klippy.py ~/printer.cfg -i test.gcode -o test.serial -v -d out/klipper.dict
```

The above will produce a file **test.serial** with the binary serial
output. This output can be translated to readable text with:

```
~/klippy-env/bin/python ./klippy/parsedump.py out/klipper.dict test.serial > test.txt
```

The resulting file **test.txt** contains a human readable list of
micro-controller commands.

The batch mode disables certain response / request commands in order
to function. As a result, there will be some differences between
actual commands and the above output. The generated data is useful for
testing and inspection; it is not useful for sending to a real
micro-controller.

Testing with simulavr
=====================

The [simulavr](http://www.nongnu.org/simulavr/) tool enables one to
simulate an Atmel ATmega micro-controller. This section describes how
one can run test gcode files through simulavr. It is recommended to
run this on a desktop class machine (not a Raspberry Pi) as it does
require significant cpu to run efficiently.

To use simulavr, download the simulavr package and compile with python
support:

```
git clone git://git.savannah.nongnu.org/simulavr.git
cd simulavr
./bootstrap
./configure --enable-python
make
```

Note that the build system may need to have some packages (such as
swig) installed in order to build the python module. Make sure the
file **src/python/_pysimulavr.so** is present after the above
compilation.

To compile Klipper for use in simulavr, run:

```
cd /patch/to/klipper
make menuconfig
```

and compile the micro-controller software for an AVR atmega644p, set
the MCU frequency to 20Mhz, and select SIMULAVR software emulation
support. Then one can compile Klipper (run `make`) and then start the
simulation with:

```
PYTHONPATH=/path/to/simulavr/src/python/ ./scripts/avrsim.py -m atmega644 -s 20000000 -b 250000 out/klipper.elf
```

Then, with simulavr running in another window, one can run the
following to read gcode from a file (eg, "test.gcode"), process it
with Klippy, and send it to Klipper running in simulavr (see
[installation](Installation.md) for the steps necessary to build the
python virtual environment):

```
~/klippy-env/bin/python ./klippy/klippy.py config/avrsim.cfg -i test.gcode -v
```

Using simulavr with gtkwave
---------------------------

One useful feature of simulavr is its ability to create signal wave
generation files with the exact timing of events. To do this, follow
the directions above, but run avrsim.py with a command-line like the
following:

```
PYTHONPATH=/path/to/simulavr/src/python/ ./scripts/avrsim.py -m atmega644 -s 20000000 -b 250000 out/klipper.elf -t PORTA.PORT,PORTC.PORT
```

The above would create a file **avrsim.vcd** with information on each
change to the GPIOs on PORTA and PORTB. This could then be viewed
using gtkwave with:

```
gtkwave avrsim.vcd
```

Manually sending commands to the micro-controller
=================================================

Normally, the host klippy.py process would be used to translate gcode
commands to Klipper micro-controller commands. However, it's also
possible to manually send these MCU commands (functions marked with
the DECL_COMMAND() macro in the Klipper source code). To do so, run:

```
~/klippy-env/bin/python ./klippy/console.py /tmp/pseudoserial 250000
```

See the "HELP" command within the tool for more information on its
functionality.

Generating load graphs
======================

The Klippy log file (/tmp/klippy.log) stores statistics on bandwidth,
micro-controller load, and host buffer load. It can be useful to graph
these statistics after a print.

To generate a graph, a one time step is necessary to install the
"matplotlib" package:

```
sudo apt-get update
sudo apt-get install python-matplotlib
```

Then graphs can be produced with:

```
~/klipper/scripts/graphstats.py /tmp/klippy.log loadgraph.png
```

One can then view the resulting **loadgraph.png** file.

Extracting information from the klippy.log file
===============================================

The Klippy log file (/tmp/klippy.log) also contains debugging
information. There is a logextract.py script that may be useful when
analyzing a micro-controller shutdown or similar problem. It is
typically run with something like:

```
mkdir work_directory
cd work_directory
cp /tmp/klippy.log .
~/klipper/scripts/logextract.py ./klippy.log
```

The script will extract the printer config file and will extract MCU
shutdown information. The information dumps from an MCU shutdown (if
present) will be reordered by timestamp to assist in diagnosing cause
and effect scenarios.

Micro-controller Benchmarks
===========================

This section describes the mechanism used to generate the Klipper
micro-controller step rate benchmarks.

The primary goal of the benchmarks is to provide a consistent
mechanism for measuring the impact of coding changes within the
software. A secondary goal is to provide high-level metrics for
comparing the performance between chips and between software
platforms.

The step rate benchmark is designed to find the maximum stepping rate
that the hardware and software can reach. This benchmark stepping rate
is not achievable in day-to-day use as Klipper needs to perform other
tasks (eg, mcu/host communication, temperature reading, endstop
checking) in any real-world usage.

In general, the pins for the benchmark tests are chosen to flash LEDs
or other innocuous pins. **Always verify that it is safe to drive the
configured pins prior to running a benchmark.** It is not recommended
to drive an actual stepper during a benchmark.

## Step rate benchmark test ##

The test is performed using the console.py tool (described above). The
micro-controller is configured for the particular hardware platform
(see below) and then the following is cut-and-paste into the
console.py terminal window:
```
SET start_clock {clock+freq}
SET ticks 1000

reset_step_clock oid=0 clock={start_clock}
set_next_step_dir oid=0 dir=0
queue_step oid=0 interval={ticks} count=60000 add=0
set_next_step_dir oid=0 dir=1
queue_step oid=0 interval=3000 count=1 add=0

reset_step_clock oid=1 clock={start_clock}
set_next_step_dir oid=1 dir=0
queue_step oid=1 interval={ticks} count=60000 add=0
set_next_step_dir oid=1 dir=1
queue_step oid=1 interval=3000 count=1 add=0

reset_step_clock oid=2 clock={start_clock}
set_next_step_dir oid=2 dir=0
queue_step oid=2 interval={ticks} count=60000 add=0
set_next_step_dir oid=2 dir=1
queue_step oid=2 interval=3000 count=1 add=0
```

The above tests three steppers simultaneously stepping. If running the
above results in a "Rescheduled timer in the past" or "Stepper too far
in past" error then it indicates the `ticks` parameter is too low (it
results in a stepping rate that is too fast). The goal is to find the
lowest setting of the ticks parameter that reliably results in a
successful completion of the test. It should be possible to bisect the
ticks parameter until a stable value is found.

On a failure, one can copy-and-paste the following to clear the error
in preparation for the next test:
```
clear_shutdown
```

To obtain the single stepper and dual stepper benchmarks, the same
configuration sequence is used, but only the first block (for the
single stepper case) or first two blocks (for the dual stepper case)
of the above test is cut-and-paste into the console.py window.

To produce the benchmarks found in the Features.md document, the total
number of steps per second is calculated by multiplying the number of
active steppers with the nominal mcu frequency and dividing by the
final ticks parameter. The results are rounded to the nearest K. For
example, with three active steppers:
```
ECHO Test result is: {"%.0fK" % (3. * freq / ticks / 1000.)}
```

### AVR step rate benchmark ###

The following configuration sequence is used on AVR chips:
```
PINS arduino
allocate_oids count=3
config_stepper oid=0 step_pin=ar29 dir_pin=ar28 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=ar27 dir_pin=ar26 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=ar23 dir_pin=ar22 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `b161a69e` with gcc version `avr-gcc
(GCC) 4.8.1`. Both the 16Mhz and 20Mhz tests were run using simulavr
configured for an atmega644p (previous tests have confirmed simulavr
results match tests on both a 16Mhz at90usb and a 16Mhz atmega2560).
On both 16Mhz and 20Mhz the best single stepper result is `SET ticks
106`, the best dual stepper result is `SET ticks 276`, and the best
three stepper result is `SET ticks 481`.

### Arduino Due step rate benchmark ###

The following configuration sequence is used on the Due:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PB27 dir_pin=PA21 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=PB26 dir_pin=PC30 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=PA21 dir_pin=PC30 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `74c21654` with gcc version
`arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0`. The best single
stepper result is `SET ticks 388`, the best dual stepper result is
`SET ticks 405`, and the best three stepper result is `SET ticks 576`.

### Duet Maestro step rate benchmark ###

The following configuration sequence is used on the Duet Maestro:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PC26 dir_pin=PC18 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=PC26 dir_pin=PA8 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=PC26 dir_pin=PB4 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `74c21654` with gcc version
`arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0`. The best single
stepper result is `SET ticks 553`, the best dual stepper result is
`SET ticks 563`, and the best three stepper result is `SET ticks 623`.

### Duet Wifi step rate benchmark ###

The following configuration sequence is used on the Duet Wifi:
```
allocate_oids count=4
config_stepper oid=0 step_pin=PD6 dir_pin=PD11 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=PD7 dir_pin=PD12 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=PD8 dir_pin=PD13 min_stop_interval=0 invert_step=0
config_stepper oid=3 step_pin=PD5 dir_pin=PA1 min_stop_interval=0 invert_step=0
finalize_config crc=0

```

The test was last run on commit `59a60d68` with gcc version
`arm-none-eabi-gcc 7.3.1 20180622 (release)
[ARM/embedded-7-branch revision 261907]`. The best single stepper
result is `SET ticks 519`, the best dual stepper result is `SET ticks
520`, and the best three stepper result is `SET ticks 525`, and the
best four stepper result is `SET ticks 703`.

### Beaglebone PRU step rate benchmark ###

The following configuration sequence is used on the PRU:
```
PINS beaglebone
allocate_oids count=3
config_stepper oid=0 step_pin=P8_13 dir_pin=P8_12 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=P8_15 dir_pin=P8_14 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=P8_19 dir_pin=P8_18 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `b161a69e` with gcc version `pru-gcc
(GCC) 8.0.0 20170530 (experimental)`. The best single stepper result
is `SET ticks 861`, the best dual stepper result is `SET ticks 853`,
and the best three stepper result is `SET ticks 883`.

### STM32F103 step rate benchmark ###

The following configuration sequence is used on the STM32F103:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PC13 dir_pin=PB5 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=PB3 dir_pin=PB6 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=PA4 dir_pin=PB7 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `9f3517fd` with gcc version
`arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0`. The best single
stepper result is `SET ticks 345`, the best dual stepper result is
`SET ticks 365`, and the best three stepper result is `SET ticks 606`.

### LPC176x step rate benchmark ###

The following configuration sequence is used on the LPC176x:
```
allocate_oids count=3
config_stepper oid=0 step_pin=P1.20 dir_pin=P1.18 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=P1.21 dir_pin=P1.18 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=P1.23 dir_pin=P1.18 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `9f3517fd` with gcc version
`arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0`. For the 100Mhz
LPC1768, the best single stepper result is `SET ticks 448`, the best
dual stepper result is `SET ticks 450`, and the best three stepper
result is `SET ticks 523`. The 120Mhz LPC1769 results were obtained by
overclocking an LPC1768 to 120Mhz - the best single stepper result is
`SET ticks 525`, the best dual stepper result is `SET ticks 526`, and
the best three stepper result is `SET ticks 545`.

### SAMD21 step rate benchmark ###

The following configuration sequence is used on the SAMD21:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA27 dir_pin=PA20 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=PB3 dir_pin=PA21 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=PA17 dir_pin=PA21 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `9f3517fd` with gcc version
`arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0`. The best single
stepper result is `SET ticks 277`, the best dual stepper result is
`SET ticks 410`, and the best three stepper result is `SET ticks 664`.

### SAMD51 step rate benchmark ###

The following configuration sequence is used on the SAMD51:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA22 dir_pin=PA20 min_stop_interval=0 invert_step=0
config_stepper oid=1 step_pin=PA22 dir_pin=PA21 min_stop_interval=0 invert_step=0
config_stepper oid=2 step_pin=PA22 dir_pin=PA19 min_stop_interval=0 invert_step=0
config_stepper oid=3 step_pin=PA22 dir_pin=PA18 min_stop_interval=0 invert_step=0
finalize_config crc=0
```

The test was last run on commit `9f3517fd` with gcc version
`arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0` on a SAMD51G19A
micro-controller. The best single stepper result is `SET ticks 516`,
the best dual stepper result is `SET ticks 520`, the best three
stepper result is `SET ticks 519`, and the best four stepper result is
`SET ticks 655`.

## Command dispatch benchmark ##

The command dispatch benchmark tests how many "dummy" commands the
micro-controller can process. It is primarily a test of the hardware
communication mechanism. The test is run using the console.py tool
(described above). The following is cut-and-paste into the console.py
terminal window:
```
DELAY {clock + 2*freq} get_uptime
FLOOD 100000 0.0 end_group
get_uptime
```

When the test completes, determine the difference between the clocks
reported in the two "uptime" response messages. The total number of
commands per second is then `100000 * mcu_frequency / clock_diff`.

Note that this test may saturate the USB/CPU capacity of a Raspberry
Pi. The benchmarks below are with console.py running on a desktop
class machine.

| MCU                 | Rate | Build    | Build compiler      |
| ------------------- | ---- | -------- | ------------------- |
| pru (shared memory) |   5K | b161a69e | pru-gcc (GCC) 8.0.0 20170530 (experimental) |
| atmega2560 (serial) |  23K | b161a69e | avr-gcc (GCC) 4.8.1 |
| sam3x8e (serial)    |  23K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| at90usb1286 (USB)   |  75K | b161a69e | avr-gcc (GCC) 4.8.1 |
| samd21 (USB)        | 238K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| stm32f103 (USB)     | 335K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| sam3x8e (USB)       | 413K | efd1bf5e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| lpc1768 (USB)       | 546K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| sam4s8c (USB)       | 579K | 8e0eb0d5 | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| lpc1769 (USB)       | 619K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| samd51 (USB)        | 620K | 8cd83b4c | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |

Host Benchmarks
===============

It is possible to run timing tests on the host software using the
"batch mode" processing mechanism described above. This is typically
done by choosing a large and complex G-Code file and timing how long
it takes for the host software to process it. For example:
```
time ~/klippy-env/bin/python ./klippy/klippy.py config/example.cfg -i something_complex.gcode -o /dev/null -d out/klipper.dict
```
