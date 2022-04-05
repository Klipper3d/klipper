# Benchmarks

This document describes Klipper benchmarks.

## Micro-controller Benchmarks

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

### Step rate benchmark test

The test is performed using the console.py tool (described in
[Debugging.md](Debugging.md)). The micro-controller is configured for
the particular hardware platform (see below) and then the following is
cut-and-paste into the console.py terminal window:
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

To obtain the single stepper benchmarks, the same configuration
sequence is used, but only the first block of the above test is
cut-and-paste into the console.py window.

To produce the benchmarks found in the [Features](Features.md) document, the total
number of steps per second is calculated by multiplying the number of
active steppers with the nominal mcu frequency and dividing by the
final ticks parameter. The results are rounded to the nearest K. For
example, with three active steppers:
```
ECHO Test result is: {"%.0fK" % (3. * freq / ticks / 1000.)}
```

The benchmarks are run with parameters suitable for TMC Drivers. For
micro-controllers that support `STEPPER_BOTH_EDGE=1` (as reported in
the `MCU config` line when console.py first starts) use
`step_pulse_duration=0` and `invert_step=-1` to enable optimized
stepping on both edges of the step pulse. For other micro-controllers
use a `step_pulse_duration` corresponding to 100ns.

### AVR step rate benchmark

The following configuration sequence is used on AVR chips:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA5 dir_pin=PA4 invert_step=0 step_pulse_ticks=32
config_stepper oid=1 step_pin=PA3 dir_pin=PA2 invert_step=0 step_pulse_ticks=32
config_stepper oid=2 step_pin=PC7 dir_pin=PC6 invert_step=0 step_pulse_ticks=32
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version `avr-gcc
(GCC) 5.4.0`. Both the 16Mhz and 20Mhz tests were run using simulavr
configured for an atmega644p (previous tests have confirmed simulavr
results match tests on both a 16Mhz at90usb and a 16Mhz atmega2560).

| avr              | ticks |
| ---------------- | ----- |
| 1 stepper        | 102   |
| 3 stepper        | 486   |

### Arduino Due step rate benchmark

The following configuration sequence is used on the Due:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PB27 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB26 dir_pin=PC30 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA21 dir_pin=PC30 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| sam3x8e              | ticks |
| -------------------- | ----- |
| 1 stepper            | 66    |
| 3 stepper            | 257   |

### Duet Maestro step rate benchmark

The following configuration sequence is used on the Duet Maestro:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PC26 dir_pin=PC18 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PC26 dir_pin=PA8 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PC26 dir_pin=PB4 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| sam4s8c              | ticks |
| -------------------- | ----- |
| 1 stepper            | 71    |
| 3 stepper            | 260   |

### Duet Wifi step rate benchmark

The following configuration sequence is used on the Duet Wifi:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PD6 dir_pin=PD11 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PD7 dir_pin=PD12 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PD8 dir_pin=PD13 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`gcc version 10.3.1 20210621 (release) (GNU Arm Embedded Toolchain 10.3-2021.07)`.

| sam4e8e          | ticks |
| ---------------- | ----- |
| 1 stepper        | 48    |
| 3 stepper        | 215   |

### Beaglebone PRU step rate benchmark

The following configuration sequence is used on the PRU:
```
allocate_oids count=3
config_stepper oid=0 step_pin=gpio0_23 dir_pin=gpio1_12 invert_step=0 step_pulse_ticks=20
config_stepper oid=1 step_pin=gpio1_15 dir_pin=gpio0_26 invert_step=0 step_pulse_ticks=20
config_stepper oid=2 step_pin=gpio0_22 dir_pin=gpio2_1 invert_step=0 step_pulse_ticks=20
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version `pru-gcc
(GCC) 8.0.0 20170530 (experimental)`.

| pru              | ticks |
| ---------------- | ----- |
| 1 stepper        | 231   |
| 3 stepper        | 847   |

### STM32F042 step rate benchmark

The following configuration sequence is used on the STM32F042:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA1 dir_pin=PA2 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PA3 dir_pin=PA2 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PB8 dir_pin=PA2 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| stm32f042        | ticks |
| ---------------- | ----- |
| 1 stepper        | 59    |
| 3 stepper        | 249   |

### STM32F103 step rate benchmark

The following configuration sequence is used on the STM32F103:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PC13 dir_pin=PB5 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB3 dir_pin=PB6 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA4 dir_pin=PB7 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| stm32f103            | ticks |
| -------------------- | ----- |
| 1 stepper            | 61    |
| 3 stepper            | 264   |

### STM32F4 step rate benchmark

The following configuration sequence is used on the STM32F4:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA5 dir_pin=PB5 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB2 dir_pin=PB6 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PB3 dir_pin=PB7 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`. The STM32F407
results were obtained by running an STM32F407 binary on an STM32F446
(and thus using a 168Mhz clock).

| stm32f446            | ticks |
| -------------------- | ----- |
| 1 stepper            | 46    |
| 3 stepper            | 205   |

| stm32f407            | ticks |
| -------------------- | ----- |
| 1 stepper            | 46    |
| 3 stepper            | 205   |

### STM32G0B1 step rate benchmark

The following configuration sequence is used on the STM32G0B1:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PB13 dir_pin=PB12 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB10 dir_pin=PB2 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PB0 dir_pin=PC5 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `247cd753` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| stm32f042        | ticks |
| ---------------- | ----- |
| 1 stepper        | 58    |
| 3 stepper        | 243   |

### LPC176x step rate benchmark

The following configuration sequence is used on the LPC176x:
```
allocate_oids count=3
config_stepper oid=0 step_pin=P1.20 dir_pin=P1.18 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=P1.21 dir_pin=P1.18 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=P1.23 dir_pin=P1.18 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`. The 120Mhz LPC1769
results were obtained by overclocking an LPC1768 to 120Mhz.

| lpc1768              | ticks |
| -------------------- | ----- |
| 1 stepper            | 52    |
| 3 stepper            | 222   |

| lpc1769              | ticks |
| -------------------- | ----- |
| 1 stepper            | 51    |
| 3 stepper            | 222   |

### SAMD21 step rate benchmark

The following configuration sequence is used on the SAMD21:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA27 dir_pin=PA20 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB3 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA17 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0` on a SAMD21G18
micro-controller.

| samd21               | ticks |
| -------------------- | ----- |
| 1 stepper            | 70    |
| 3 stepper            | 306   |

### SAMD51 step rate benchmark

The following configuration sequence is used on the SAMD51:
```
allocate_oids count=3
config_stepper oid=0 step_pin=PA22 dir_pin=PA20 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PA22 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA22 dir_pin=PA19 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0` on a SAMD51J19A
micro-controller.

| samd51               | ticks |
| -------------------- | ----- |
| 1 stepper            | 39    |
| 3 stepper            | 191   |
| 1 stepper (200Mhz)   | 39    |
| 3 stepper (200Mhz)   | 181   |

### RP2040 step rate benchmark

The following configuration sequence is used on the RP2040:

```
allocate_oids count=3
config_stepper oid=0 step_pin=gpio25 dir_pin=gpio3 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=gpio26 dir_pin=gpio4 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=gpio27 dir_pin=gpio5 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0` on a Raspberry Pi
Pico board.

| rp2040               | ticks |
| -------------------- | ----- |
| 1 stepper            | 5     |
| 3 stepper            | 22    |

### Linux MCU step rate benchmark

The following configuration sequence is used on a Raspberry Pi:
```
allocate_oids count=3
config_stepper oid=0 step_pin=gpio2 dir_pin=gpio3 invert_step=0 step_pulse_ticks=5
config_stepper oid=1 step_pin=gpio4 dir_pin=gpio5 invert_step=0 step_pulse_ticks=5
config_stepper oid=2 step_pin=gpio6 dir_pin=gpio17 invert_step=0 step_pulse_ticks=5
finalize_config crc=0
```

The test was last run on commit `59314d99` with gcc version
`gcc (Raspbian 8.3.0-6+rpi1) 8.3.0` on a Raspberry Pi 3 (revision
a02082). It was difficult to get stable results in this benchmark.

| Linux (RPi3)         | ticks |
| -------------------- | ----- |
| 1 stepper            | 160   |
| 3 stepper            | 380   |

## Command dispatch benchmark

The command dispatch benchmark tests how many "dummy" commands the
micro-controller can process. It is primarily a test of the hardware
communication mechanism. The test is run using the console.py tool
(described in [Debugging.md](Debugging.md)). The following is
cut-and-paste into the console.py terminal window:
```
DELAY {clock + 2*freq} get_uptime
FLOOD 100000 0.0 debug_nop
get_uptime
```

When the test completes, determine the difference between the clocks
reported in the two "uptime" response messages. The total number of
commands per second is then `100000 * mcu_frequency / clock_diff`.

Note that this test may saturate the USB/CPU capacity of a Raspberry
Pi. If running on a Raspberry Pi, Beaglebone, or similar host computer
then increase the delay (eg, `DELAY {clock + 20*freq} get_uptime`).
Where applicable, the benchmarks below are with console.py running on
a desktop class machine with the device connected via a high-speed
hub.

| MCU                 | Rate | Build    | Build compiler      |
| ------------------- | ---- | -------- | ------------------- |
| stm32f042 (CAN)     |  18K | c105adc8 | arm-none-eabi-gcc (GNU Tools 7-2018-q3-update) 7.3.1 |
| atmega2560 (serial) |  23K | b161a69e | avr-gcc (GCC) 4.8.1 |
| sam3x8e (serial)    |  23K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| at90usb1286 (USB)   |  75K | 01d2183f | avr-gcc (GCC) 5.4.0 |
| samd21 (USB)        | 223K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| pru (shared memory) | 260K | c5968a08 | pru-gcc (GCC) 8.0.0 20170530 (experimental) |
| stm32f103 (USB)     | 355K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| sam3x8e (USB)       | 418K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| lpc1768 (USB)       | 534K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| lpc1769 (USB)       | 628K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| sam4s8c (USB)       | 650K | 8d4a5c16 | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| samd51 (USB)        | 864K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| stm32f446 (USB)     | 870K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| rp2040 (USB)        | 873K | c5667193 | arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0 |

## Host Benchmarks

It is possible to run timing tests on the host software using the
"batch mode" processing mechanism (described in
[Debugging.md](Debugging.md)). This is typically done by choosing a
large and complex G-Code file and timing how long it takes for the
host software to process it. For example:
```
time ~/klippy-env/bin/python ./klippy/klippy.py config/example-cartesian.cfg -i something_complex.gcode -o /dev/null -d out/klipper.dict
```
