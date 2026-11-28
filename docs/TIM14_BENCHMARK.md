# STM32G0B1 TIM14 scheduler and TIM2 hardware PWM: measured use case

Supporting test results for [PR #7433](https://github.com/Klipper3d/klipper/pull/7433).
Original plots, interval CSV, plotting script, and reproducibility information:
[benchmark evidence branch](https://github.com/Doncuppjr/klipper/tree/tim14-benchmark-evidence).

## Board-specific motivation and why TIM14 is opt-in

On the Nighthawk SB v2 STM32G0B1 toolhead used with a Voron 2.4R2,
the original PA15/PF- board fan output can be driven by TIM2_CH1 hardware PWM.
The default Klipper 32-bit TIM2 scheduler owns that timer, requiring
25 kHz fan PWM on this output to be scheduled in software. PD0/TIM16
runs another hardware fan PWM independently. EBB boards may assign PA15
to a different function; the motivation is a *specific pin-mux tradeoff*
on boards that really need high-frequency PWM.

Selecting the optional 16-bit TIM14 scheduler frees TIM2 for hardware PWM
on PA0, PA1, PA5 and PA15; timer-ownership guards reject hardware PWM on
the timer used for scheduling. **The existing 32-bit TIM2 scheduler remains
the default.** Other PWM frequencies and GPIO allocations may prefer it.

## Instrumented CPU comparison at 20k STEP/s

Tested on NUCLEO-G0B1RE at nominal 64 MHz using a simulated PB8 STEP
load and TIM16/PD0 hardware 25 kHz PWM in both cases. The reported
percentage is ISR handler-core time from independent TIM7 measurement,
not total CPU utilization.

![Matched 200 Hz and 25 kHz ISR comparison](https://raw.githubusercontent.com/Doncuppjr/klipper/tim14-benchmark-evidence/graphs/tim14_isr_cpu_200hz_25khz.png)

| PA15 output | Stock TIM2 + software PWM | TIM14 + TIM2 hardware PWM |
|---|---:|---:|
| 200 Hz | 8.79% | 10.18% |
| 25 kHz | 21.79% | 10.18% |

At **25 kHz**, measured handler-core time fell ~53% (11.61 percentage
points). Interrupt rate was ~49,993 to ~15,785/s, ~68% fewer.
At **200 Hz**, the new 16-bit scheduler was ~1.39 points *worse*.

## Under high simulated STEP loads

![STEP rate versus IRQ time](https://raw.githubusercontent.com/Doncuppjr/klipper/tim14-benchmark-evidence/graphs/tim14_isr_cpu_step_sweep.png)

| Requested STEP/s | Stock ISR core CPU | Proposed ISR core CPU |
|---|---:|---:|
| 40,000 | 31.62% | 20.61% |
| 80,000 | 46.11% | 40.50% |
| 120,000 | 65.53% | 59.88% |

~4 second measurement windows did not span every requested pulse
at all rates; results are not a verified maximum step rate.

## Physical scope measurements

Rigol DS1202Z-E USBTMC on the NUCLEO board (CH1 PB8 STEP, CH2 PA15 PWM,
10x probes). Software and hardware 25 kHz, 50% PWM both measured
~40.0 us period and ~20.0 us high on physical **MCU pin PA15**.
This is not an electrical measurement of the Nighthawk PF- connector
after its output conditioning stage.

At 80k STEP/s, six short captures per firmware produced 54 measured
rising-edge intervals each (200 ns sample spacing). Stock TIM2/software
PWM: mean 12.585 us, std. dev. 0.899 us, range 10.8..15.2 us.
TIM14/TIM2 hardware PWM: mean 12.489 us, std. dev. 0.166 us,
range 12.4..12.8 us.

![Rigol 80k step samples](https://raw.githubusercontent.com/Doncuppjr/klipper/tim14-benchmark-evidence/graphs/tim14_step_80k_rigol.png)

![STEP timing including 120k limit](https://raw.githubusercontent.com/Doncuppjr/klipper/tim14-benchmark-evidence/graphs/tim14_step_jitter_sweep.png)

At 120k STEP/s both firmware variants show alternating short and
long intervals and neither has uniform timing. These very short
captures do **not** prove freedom from missed steps, a universal timing
bound, or more reliable operation at 120k.

## Real toolhead validation

On a Nighthawk SB v2 STM32G0B1, TIM14 firmware was flashed via
Katapult and the original PA15/PF- board fan ran with TIM2 hardware
PWM at 25 kHz. Tachometer speeds at commanded duty:

| Duty | RPM |
|---|---:|
| 0% | 0 |
| 35% | 3,628 |
| 50% | 5,757 |
| 75% | 8,727 |
| 100% | 9,655 |

Firmware reported Ready and sensor, accelerometer and extruder TMC
UART queries passed. A real ABS print endurance test was running when
these observations were logged; it should not be counted as a completed
print result until verified.

## Builds, safety, limitations

- Builds: STM32G0B1 default TIM2 and optional TIM14, USB/12 MHz crystal,
  Katapult 8 KiB bootloader; STM32F070 and STM32F042 regression builds;
  whitespace checks.
- Scheduler/PWM coexistence and ownership guard tested on development
  board; requesting PWM on the occupied scheduler timer was rejected.
- Valid bench test runs had no MCU shutdown. Initial 9 bytes of serial
  retransmits appeared in both variants and were not attributed to PWM.
- Hardware/software PWM alone did **not** show a compelling large
  PWM-edge jitter difference at 25 kHz to scope resolution.
- Fan noise reduction has not been measured; fan acoustics and power
  efficiency cannot be inferred from IRQ time or STEP jitter.
- Timer performance is workload, pin-mux, and board dependent.
  This is an optional mechanism to avoid high-frequency software PWM,
  not a default scheduler replacement.

Signed-off-by: Donald A. Cupp Jr <doncuppjr@yahoo.com>
