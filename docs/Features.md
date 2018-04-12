Klipper has several compelling features:

* High precision stepper movement. Klipper utilizes an application
  processor (such as a low-cost Raspberry Pi) when calculating printer
  movements. The application processor determines when to step each
  stepper motor, it compresses those events, transmits them to the
  micro-controller, and then the micro-controller executes each event
  at the requested time. Each stepper event is scheduled with a
  precision of 25 micro-seconds or better. The software does not use
  kinematic estimations (such as the Bresenham algorithm) - instead it
  calculates precise step times based on the physics of acceleration
  and the physics of the machine kinematics. More precise stepper
  movement translates to quieter and more stable printer operation.

* Best in class performance. Klipper is able to achieve high stepping
  rates on both new and old micro-controllers. Even an old 8bit AVR
  micro-controller can obtain rates over 175K steps per second. On
  more recent micro-controllers, rates over 500K steps per second are
  possible. Higher stepper rates enable higher print velocities. The
  stepper event timing remains precise even at high speeds which
  improves overall stability.

* Configuration via simple config file. There's no need to reflash the
  micro-controller to change a setting. All of Klipper's configuration
  is stored in a standard config file which can be easily edited. This
  makes it easier to setup and maintain the hardware.

* Portable code. Klipper works on both ARM and AVR
  micro-controllers. Existing "reprap" style printers can run Klipper
  without hardware modification - just add a Raspberry Pi. Klipper's
  internal code layout makes it easier to support other
  micro-controller architectures as well.

* Simpler code. Klipper uses a very high level language (Python) for
  most code. The kinematics algorithms, the G-code parsing, the
  heating and thermistor algorithms, etc. are all written in
  Python. This makes it easier to develop new functionality.

* Advanced features:
  * Klipper implements the "pressure advance" algorithm for
    extruders. When properly tuned, pressure advance reduces extruder
    ooze.
  * Klipper supports printers with multiple micro-controllers. For
    example, one micro-controller could be used to control an
    extruder, while another could control the printer's heaters, while
    a third controls the rest of the printer. The Klipper host
    software implements clock synchronization to account for clock
    drift between micro-controllers. No special code is needed to
    enable multiple micro-controllers - it just requires a few extra
    lines in the config file.
  * Klipper also implements a novel "stepper phase endstop" algorithm
    that can dramatically improve the accuracy of typical endstop
    switches. When properly tuned it can improve a print's first layer
    bed adhesion.
  * Support for limiting the top speed of short "zigzag" moves to
    reduce printer vibration and noise. See the
    [kinematics](Kinematics.md) document for more information.

To get started with Klipper, read the [installation](Installation.md)
guide.

Common features supported by Klipper
====================================

Klipper supports many standard 3d printer features:

* Works with Octoprint. This allows the printer to be controlled using
  a regular web-browser. The same Raspberry Pi that runs Klipper can
  also run Octoprint.

* Standard G-Code support. Common g-code commands that are produced by
  typical "slicers" are supported. One may continue to use Slic3r,
  Cura, etc. with Klipper.

* Constant speed acceleration support. All printer moves will
  gradually accelerate from standstill to cruising speed and then
  decelerate back to a standstill.

* "Look-ahead" support. The incoming stream of G-Code movement
  commands are queued and analyzed - the acceleration between
  movements in a similar direction will be optimized to reduce print
  stalls and improve overall print time.

* Support for cartesian, delta, and corexy style printers.

Step Benchmarks
===============

Below are the results of stepper performance tests. The numbers shown
represent total number of steps per second on the micro-controller.

| Micro-controller  | Fastest step rate | 3 steppers active |
| ----------------- | ----------------- | ----------------- |
| 16Mhz AVR         | 151K              | 100K              |
| 20Mhz AVR         | 189K              | 125K              |
| STM32F103         | 340K              | 300K              |
| Arduino Due (ARM) | 382K              | 337K              |
| Beaglebone PRU    | 689K              | 689K              |

On AVR platforms, the highest achievable step rate is with just one
stepper stepping. On the Due amd STM32F103, the highest step rate is
with two simultaneous steppers stepping. On the PRU, the highest step
rate is with three simultaneous steppers.
