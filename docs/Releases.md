# Releases

History of Klipper releases. Please see
[installation](Installation.md) for information on installing Klipper.

## Klipper 0.13.0

Available on 20250411. Major changes in this release:
* New "sweeping vibrations" resonance testing mechanism for input
  shaper.
* Fans and GPIO pins can now be assigned a formula (via Jinja2
  "templates").
* The bed_mesh code now supports "adaptive bed mesh". The area probed
  can be adjusted for the size of the print.
* A new `minimum_cruise_ratio` kinematic parameter has been added (it
  replaces the previous `max_accel_to_decel` parameter).
* Several new sensors added:
  * Support for ldc1612 "eddy" current sensors. This includes probing
    support, fast "scan" probing, and temperature calibration.
  * New support for "load cell" measurements. Support for connecting
    these load cells to hx71x and ads1220 ADC sensors.
  * Support for BMP180, BMP388, and SHT3x temperature sensors. Support
    for measuring temperature with ADS1x1x ADC chips.
  * New lis3dh and icm20948 accelerometer support.
  * Support for mt6816 and mt6826s "hall angle" sensors.
* New micro-controller improvements:
  * New support for rp2350 micro-controllers.
  * Existing rp2040 chips now run at 200MHz (up from 125Mhz).
  * The micro-controller code can now define many more commands (up to
    16384 from 128).
* Other modules added: aip31068_spi, canbus_stats, error_mcu,
  garbage_collection, pwm_cycle_time, pwm_tool, garbage_collection.
* Several bug fixes and code cleanups.

## Klipper 0.12.0

Available on 20231110. Major changes in this release:
* Support for COPY and MIRROR modes on IDEX printers.
* Several micro-controller improvements:
  * Support for new ar100 and hc32f460 architectures.
  * Support for stm32f7, stm32g0b0, stm32g07x, stm32g4, stm32h723,
    n32g45x, samc21, and samd21j18 chip variants.
  * Improved DFU and Katapult reboot handling.
  * Improved performance on USB to CANbus bridge mode.
  * Improved performance on "linux mcu".
  * New support for software based i2c.
* New hardware support for tmc2240 stepper motor drivers, lis2dw12
  accelerometers, and aht10 temperature sensors.
* New axis_twist_compensation and temperature_combined modules added.
* New support for gcode arcs in XY, XZ, and YZ planes.
* Several bug fixes and code cleanups.

## Klipper 0.11.0

Available on 20221128. Major changes in this release:
* Trinamic stepper motor driver "step on both edges" optimization.
* Support for Python3. The Klipper host code will run with either
  Python2 or Python3.
* Enhanced CAN bus support. Support for CAN bus on rp2040, stm32g0,
  stm32h7, same51, and same54 chips. Support for "USB to CAN bus
  bridge" mode.
* Support for CanBoot bootloader.
* Support for mpu9250 and mpu6050 accelerometers.
* Improved error handling for max31856, max31855, max31865, and
  max6675 temperature sensors.
* It is now possible to configure LEDs to update during long running
  G-Code commands using LED "template" support.
* Several micro-controller improvements. New support for stm32h743,
  stm32h750, stm32l412, stm32g0b1, same70, same51, and same54 chips.
  Support for i2c reads on atsamd and stm32f0. Hardware pwm support on
  stm32. Linux mcu signal based event dispatch. New rp2040 support for
  "make flash", i2c, and rp2040-e5 USB errata.
* New modules added: angle, dac084S085, exclude_object, led, mpu9250,
  pca9632, smart_effector, z_thermal_adjust. New deltesian kinematics
  added. New dump_mcu tool added.
* Several bug fixes and code cleanups.

## Klipper 0.10.0

Available on 20210929. Major changes in this release:
* Support for "Multi-MCU Homing". It is now possible for a stepper
  motor and its endstop to be wired to separate micro-controllers.
  This simplifies wiring of Z probes on "toolhead boards".
* Klipper now has a
  [Community Discord Server](https://discord.klipper3d.org)
  and a [Community Discourse Server](https://community.klipper3d.org).
* The [Klipper website](https://www.klipper3d.org) now uses the
  "mkdocs" infrastructure. There is also a
  [Klipper Translations](https://github.com/Klipper3d/klipper-translations)
  project.
* Automated support for flashing firmware via sdcard on many boards.
* New kinematic support for "Hybrid CoreXY" and "Hybrid CoreXZ"
  printers.
* Klipper now uses `rotation_distance` to configure stepper motor
  travel distances.
* The main Klipper host code can now directly communicate with
  micro-controllers using CAN bus.
* New "motion analysis" system. Klipper's internal motion updates and
  sensor results can be tracked and logged for analysis.
* Trinamic stepper motor drivers are now continuously monitored for
  error conditions.
* Support for the rp2040 micro-controller (Raspberry Pi Pico boards).
* The "make menuconfig" system now utilizes kconfiglib.
* Many additional modules added: ds18b20, duplicate_pin_override,
  filament_motion_sensor, palette2, motion_report, pca9533,
  pulse_counter, save_variables, sdcard_loop, temperature_host,
  temperature_mcu
* Several bug fixes and code cleanups.

## Klipper 0.9.0

Available on 20201020. Major changes in this release:
* Support for "Input Shaping" - a mechanism to counteract printer
  resonance. It can reduce or eliminate "ringing" in prints.
* New "Smooth Pressure Advance" system. This implements "Pressure
  Advance" without introducing instantaneous velocity changes. It is
  also now possible to tune pressure advance using a "Tuning Tower"
  method.
* New "webhooks" API server. This provides a programmable JSON
  interface to Klipper.
* The LCD display and menu are now configurable using the Jinja2
  template language.
* The TMC2208 stepper motor drivers can now be used in "standalone"
  mode with Klipper.
* Improved BL-Touch v3 support.
* Improved USB identification. Klipper now has its own USB
  identification code and micro-controllers can now report their
  unique serial numbers during USB identification.
* New kinematic support for "Rotary Delta" and "CoreXZ" printers.
* Micro-controller improvements: support for stm32f070, support for
  stm32f207, support for GPIO pins on "Linux MCU", stm32 "HID
  bootloader" support, Chitu bootloader support, MKS Robin bootloader
  support.
* Improved handling of Python "garbage collection" events.
* Many additional modules added: adc_scaled, adxl345, bme280,
  display_status, extruder_stepper, fan_generic,
  hall_filament_width_sensor, htu21d, homing_heaters, input_shaper,
  lm75, print_stats, resonance_tester, shaper_calibrate, query_adc,
  graph_accelerometer, graph_extruder, graph_motion, graph_shaper,
  graph_temp_sensor, whconsole
* Several bug fixes and code cleanups.

### Klipper 0.9.1

Available on 20201028. Release containing only bug fixes.

## Klipper 0.8.0

Available on 20191021. Major changes in this release:
* New G-Code command template support. G-Code in the config file is
  now evaluated with the Jinja2 template language.
* Improvements to Trinamic stepper drivers:
  * New support for TMC2209 and TMC5160 drivers.
  * Improved DUMP_TMC, SET_TMC_CURRENT, and INIT_TMC G-Code commands.
  * Improved support for TMC UART handling with an analog mux.
* Improved homing, probing, and bed leveling support:
  * New manual_probe, bed_screws, screws_tilt_adjust, skew_correction,
    safe_z_home modules added.
  * Enhanced multi-sample probing with median, average, and retry
    logic.
  * Improved documentation for BL-Touch, probe calibration, endstop
    calibration, delta calibration, sensorless homing, and endstop
    phase calibration.
  * Improved homing support on a large Z axis.
* Many Klipper micro-controller improvements:
  * Klipper ported to: SAM3X8C, SAM4S8C, SAMD51, STM32F042, STM32F4
  * New USB CDC driver implementations on SAM3X, SAM4, STM32F4.
  * Enhanced support for flashing Klipper over USB.
  * Software SPI support.
  * Greatly improved temperature filtering on the LPC176x.
  * Early output pin settings can be configured in the
    micro-controller.
* New website with the Klipper documentation: http://klipper3d.org/
  * Klipper now has a logo.
* Experimental support for polar and "cable winch" kinematics.
* The config file can now include other config files.
* Many additional modules added: board_pins, controller_fan,
  delayed_gcode, dotstar, filament_switch_sensor, firmware_retraction,
  gcode_arcs, gcode_button, heater_generic, manual_stepper, mcp4018,
  mcp4728, neopixel, pause_resume, respond, temperature_sensor
  tsl1401cl_filament_width_sensor, tuning_tower
* Many additional commands added: RESTORE_GCODE_STATE,
  SAVE_GCODE_STATE, SET_GCODE_VARIABLE, SET_HEATER_TEMPERATURE,
  SET_IDLE_TIMEOUT, SET_TEMPERATURE_FAN_TARGET
* Several bug fixes and code cleanups.

## Klipper 0.7.0

Available on 20181220. Major changes in this release:
* Klipper now supports "mesh" bed leveling
* New support for "enhanced" delta calibration (calibrates print x/y
  dimensions on delta printers)
* Support for run-time configuration of Trinamic stepper motor drivers
  (tmc2130, tmc2208, tmc2660)
* Improved temperature sensor support: MAX6675, MAX31855, MAX31856,
  MAX31865, custom thermistors, common pt100 style sensors
* Several new modules: temperature_fan, sx1509, force_move, mcp4451,
  z_tilt, quad_gantry_level, endstop_phase, bltouch
* Several new commands added: SAVE_CONFIG, SET_PRESSURE_ADVANCE,
  SET_GCODE_OFFSET, SET_VELOCITY_LIMIT, STEPPER_BUZZ, TURN_OFF_HEATERS,
  M204, custom g-code macros
* Expanded LCD display support:
  * Support for run-time menus
  * New display icons
  * Support for "uc1701" and "ssd1306" displays
* Additional micro-controller support:
  * Klipper ported to: LPC176x (Smoothieboards), SAM4E8E (Duet2),
    SAMD21 (Arduino Zero), STM32F103 ("Blue pill" devices), atmega32u4
  * New Generic USB CDC driver implemented on AVR, LPC176x, SAMD21, and
    STM32F103
  * Performance improvements on ARM processors
* The kinematics code was rewritten to use an "iterative solver"
* New automatic test cases for the Klipper host software
* Many new example config files for common off-the-shelf printers
* Documentation updates for bootloaders, benchmarking,
    micro-controller porting, config checks, pin mapping, slicer
    settings, packaging, and more
* Several bug fixes and code cleanups

## Klipper 0.6.0

Available on 20180331. Major changes in this release:
* Enhanced heater and thermistor hardware failure checks
* Support for Z probes
* Initial support for automatic parameter calibration on deltas (via a
  new delta_calibrate command)
* Initial support for bed tilt compensation (via bed_tilt_calibrate
  command)
* Initial support for "safe homing" and homing overrides
* Initial support for displaying status on RepRapDiscount style 2004
  and 12864 displays
* New multi-extruder improvements:
  * Support for shared heaters
  * Initial support for dual carriages
* Support for configuring multiple steppers per axis (eg, dual Z)
* Support for custom digital and pwm output pins (with a new SET_PIN command)
* Initial support for a "virtual sdcard" that allows printing directly
  from Klipper (helps on machines too slow to run OctoPrint well)
* Support for setting different arm lengths on each tower of a delta
* Support for G-Code M220/M221 commands (speed factor override /
  extrude factor override)
* Several documentation updates:
  * Many new example config files for common off-the-shelf printers
  * New multiple MCU config example
  * New bltouch sensor config example
  * New FAQ, config check, and G-Code documents
* Initial support for continuous integration testing on all github commits
* Several bug fixes and code cleanups

## Klipper 0.5.0

Available on 20171025. Major changes in this release:

* Support for printers with multiple extruders.
* Initial support for running on the Beaglebone PRU. Initial support
  for the Replicape board.
* Initial support for running the micro-controller code in a real-time
  Linux process.
* Support for multiple micro-controllers. (For example, one could
  control an extruder with one micro-controller and the rest of the
  printer with another.) Software clock synchronization is implemented
  to coordinate actions between micro-controllers.
* Stepper performance improvements (20Mhz AVRs up to 189K steps per
  second).
* Support for controlling servos and support for defining nozzle
  cooling fans.
* Several bug fixes and code cleanups

## Klipper 0.4.0

Available on 20170503. Major changes in this release:

* Improved installation on Raspberry Pi machines. Most of the install
  is now scripted.
* Support for corexy kinematics
* Documentation updates: New Kinematics document, new Pressure Advance
  tuning guide, new example config files, and more
* Stepper performance improvements (20Mhz AVRs over 175K steps per
  second, Arduino Due over 460K)
* Support for automatic micro-controller resets. Support for resets
  via toggling USB power on Raspberry Pi.
* The pressure advance algorithm now works with look-ahead to reduce
  pressure changes during cornering.
* Support for limiting the top speed of short zigzag moves
* Support for AD595 sensors
* Several bug fixes and code cleanups

## Klipper 0.3.0

Available on 20161223. Major changes in this release:

* Improved documentation
* Support for robots with delta kinematics
* Support for Arduino Due micro-controller (ARM cortex-M3)
* Support for USB based AVR micro-controllers
* Support for "pressure advance" algorithm - it reduces ooze during
  prints.
* New "stepper phased based endstop" feature - enables higher
  precision on endstop homing.
* Support for "extended g-code" commands such as "help", "restart",
  and "status".
* Support for reloading the Klipper config and restarting the host
  software by issuing a "restart" command from the terminal.
* Stepper performance improvements (20Mhz AVRs up to 158K steps per
  second).
* Improved error reporting. Most errors now shown via the terminal
  along with help on how to resolve.
* Several bug fixes and code cleanups

## Klipper 0.2.0

Initial release of Klipper. Available on 20160525. Major features
available in the initial release include:

* Basic support for cartesian printers (steppers, extruder, heated
  bed, cooling fan).
* Support for common g-code commands. Support for interfacing with
  OctoPrint.
* Acceleration and lookahead handling
* Support for AVR micro-controllers via standard serial ports
