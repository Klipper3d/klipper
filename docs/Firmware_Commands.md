This document provides high-level information on common firmware
commands. It is not an authoritative reference for these commands, nor
is it an exclusive list of all available firmware commands.

This document may be useful for users needing to configure a set of
hardware actions that their printer may require at startup (via the
"custom" field in the printer config file), and it may be useful for
developers wishing to obtain a high-level feel for available firmware
commands.

See the [protocol](Protocol.md) document for more information on the
format of commands and their low-level transmission. The commands here
are described using their "printf" style syntax - for those unfamiliar
with that format, just note that where a '%...' sequence is seen it
should be replaced with an actual integer.

Startup Commands
================

It may be necessary to take certain one-time actions to configure the
micro-controller and its peripherals. This section lists common
commands available for that purpose. Unlike other firmware commands,
these commands run as soon as they are received by the firmware and
they do not require any particular setup.

These commands are most useful in the "custom" block of the "mcu"
section of the printer configuration file. This feature is typically
used to configure the initial settings of LEDs, to configure
micro-stepping pins, to configure a digipot, etc.

Several of these commands will take a "pin=%u" parameter. The
low-level firmware uses integer encodings of the hardware pin numbers,
but to make things more readable the host will translate human
readable pin names (eg, "PA3") to their equivalent integer
encodings. By convention, any parameter named "pin" or that has a
"_pin" suffix will use pin name translation by the host.

Common startup commands:

* set_digital_out pin=%u value=%c : This command immediately
  configures the given pin as a digital out GPIO and it sets it to
  either a low level (value=0) or a high level (value=1). This command
  may be useful for configuring the initial value of LEDs and for
  configuring the initial value of stepper driver micro-stepping pins.

* set_pwm_out pin=%u cycle_ticks=%u value=%c : This command will
  immediately configure the given pin to use hardware based
  pulse-width-modulation (PWM) with the given number of
  cycle_ticks. The "cycle_ticks" is the number of MCU clock ticks each
  power on and power off cycle should last. A cycle_ticks value of 1
  can be used to request the fastest possible cycle time. The "value"
  parameter is between 0 and 255 with 0 indicating a full off state
  and 255 indicating a full on state. This command may be useful for
  enabling CPU and nozzle cooling fans.

* send_spi_message pin=%u msg=%*s : This command can be used to
  transmit messages to a serial-peripheral-interface (SPI) component
  connected to the micro-controller. It has been used to configure the
  startup settings of AD5206 digipots. The 'pin' parameter specifies
  the chip select line to use during the transmission. The 'msg'
  indicates the binary message to transmit to the given chip.

Firmware configuration
======================

Most commands in the firmware require an initial setup before they can
be successfully invoked. This section provides a high-level overview
of the micro-controller configuration process. This section and the
following sections are likely only of interest to developers
interested in the internal details of Klipper.

When the host first connects to the firmware it always starts by
obtaining the firmware's data dictionary (see [protocol](Protocol.md)
for more information). After the data dictionary is obtained the host
will check if the firmware is in a "configured" state and configure it
if not. Configuration involves the following phases:

* get_config : The host starts by checking if the firmware is already
  configured. The firmware responds to this command with a "config"
  response message. At micro-controller power-on the firmware always
  starts in an unconfigured state. It remains in this state until the
  host completes the configuration processes (by issuing a
  finalize_config command). If the firmware is already configured (and
  is configured with the desired settings) from a previous
  host/firmware session then no further action is needed by the host
  and the configuration process ends successfully.

* allocate_oids count=%c : This command is issued to inform the
  firmware the maximum number of object-ids (oid) that the host
  requires. It is only valid to issue this command once. An oid is an
  integer identifier allocated to each stepper, each endstop, and each
  schedulable gpio pin. The host determines in advance the number of
  oids it will require to operate the hardware and passes this to the
  firmware so that the firmware may allocate sufficient memory to
  store a mapping from oid to internal firmware object.

* config_XXX oid=%c ... : By convention any command starting with the
  "config_" prefix creates a new firmware object and assigns the given
  oid to it. For example, the config_digital_out command will
  configure the specified pin as a digital output GPIO and create an
  internal object that the host can use to schedule changes to the
  given GPIO. The oid parameter passed into the config command is
  selected by the host and must be between zero and the maximum count
  supplied in the allocate_oids command. The config commands may only
  be run when the firmware is not in a configured state (ie, prior to
  the host sending finalize_config) and after the allocate_oids
  command has been sent.

* finalize_config crc=%u : The finalize_config command transitions the
  firmware from an unconfigured state to a configured state. The crc
  parameter passed to the firmware is stored in the firmware and
  provided back to the host in "config" response messages. By
  convention, the host takes a 32bit CRC of the firmware configuration
  it will request and at the start of subsequent host/firmware
  communication sessions it checks that the CRC stored in the firmware
  exactly matches its desired CRC. If the CRC does not match then the
  host knows the firmware has not been configured in the state desired
  by the host.

Common firmware objects
-----------------------

This section lists some commonly used config commands.

* config_digital_out oid=%c pin=%u default_value=%c max_duration=%u :
  This command creates an internal firmware object for the given GPIO
  'pin'. The pin will be configured in digital output mode and set to
  an initial value as specified by 'default_value' (0 for low, 1 for
  high). Creating a digital_out object allows the host to schedule
  GPIO updates for the given pin at specified times (see the
  schedule_digital_out command described below). Should the firmware
  go into shutdown mode then all configured digital_out objects will
  be set back to their default values. The 'max_duration' parameter is
  used to implement a safety check - if it is non-zero then it is the
  maximum number of clock ticks that the host may set the given GPIO
  to a non-default value without further updates. For example, if the
  default_value is zero and the max_duration is 16000 then if the host
  sets the gpio to a value of one then it must schedule another update
  to the gpio pin (to either zero or one) within 16000 clock
  ticks. This safety feature can be used with heater pins to ensure
  the host does not set the heater to a value of one and then go
  off-line.

* config_pwm_out oid=%c pin=%u cycle_ticks=%u default_value=%c
  max_duration=%u : This command creates an internal object for
  hardware based PWM pins that the host may schedule updates for. Its
  usage is analogous to config_digital_out - see the description of
  the 'set_pwm_out' and 'config_digital_out' commands for parameter
  description.

* config_soft_pwm_out oid=%c pin=%u cycle_ticks=%u default_value=%c
  max_duration=%u : This command creates an internal firmware object
  for software implemented PWM. Unlike hardware pwm pins, a software
  pwm object does not require any special hardware support (other than
  the ability to configure the pin as a digital output GPIO). Because
  the output switching is implemented in the software of the firmware,
  it is recommended that the cycle_ticks parameter correspond to a
  time of 10ms or greater. See the description of the 'set_pwm_out'
  and 'config_digital_out' commands for parameter description.

* config_analog_in oid=%c pin=%u : This command is used to configure a
  pin in analog input sampling mode. Once configured, the pin can be
  sampled at regular interval using the query_analog_in command (see
  below).

* config_stepper oid=%c step_pin=%c dir_pin=%c min_stop_interval=%u
  invert_step=%c : This command creates an internal stepper
  object. The 'step_pin' and 'dir_pin' parameters specify the step and
  direction pins respectively; this command will configure them in
  digital output mode. The 'invert_step' parameter specifies whether a
  step occurs on a rising edge (invert_step=0) or falling edge
  (invert_step=1). The 'min_stop_interval' implements a safety
  feature - it is checked when the firmware finishes all moves for a
  stepper - if it is non-zero it specifies the minimum number of clock
  ticks since the last step. It is used as a check on the maximum
  stepper velocity that a stepper may have before stopping.

* config_end_stop oid=%c pin=%c pull_up=%c stepper_oid=%c : This
  command creates an internal "endstop" object. It is used to specify
  the endstop pins and to enable "homing" operations (see the
  end_stop_home command below). The command will configure the
  specified pin in digital input mode. The 'pull_up' parameter
  determines whether hardware provided pullup resistors for the pin
  (if available) will be enabled. The 'stepper_oid' parameter
  specifies the oid of an associated stepper for the given endstop -
  it is used during homing operations.

Common commands
===============

This section lists some commonly used run-time commands. It is likely
only of interest to developers looking to gain insight into Klippy.

* schedule_digital_out oid=%c clock=%u value=%c : This command will
  schedule a change to a digital output GPIO pin at the given clock
  time. To use this command a 'config_digital_out' command with the
  same 'oid' parameter must have been issued during firmware
  configuration.

* schedule_pwm_out oid=%c clock=%u value=%c : Schedules a change to a
  hardware PWM output pin. See the 'schedule_digital_out' and
  'config_pwm_out' commands for more info.

* schedule_soft_pwm_out oid=%c clock=%u value=%c : Schedules a change
  to a software PWM output pin. See the 'schedule_digital_out' and
  'config_soft_pwm_out' commands for more info.

* query_analog_in oid=%c clock=%u sample_ticks=%u sample_count=%c
  rest_ticks=%u min_value=%hu max_value=%hu : This command sets up a
  recurring schedule of analog input samples. To use this command a
  'config_analog_in' command with the same 'oid' parameter must have
  been issued during firmware configuration. The samples will start as
  of 'clock' time, it will report on the obtained value every
  'rest_ticks' clock ticks, it will over-sample 'sample_count' number
  of times, and it will pause 'sample_ticks' number of clock ticks
  between over-sample samples. The 'min_value' and 'max_value'
  parameters implement a safety feature - the firmware will verify the
  sampled value (after any oversampling) is always between the
  supplied range. This is intended for use with pins attached to
  thermistors controlling heaters - it can be used to check that a
  heater is within a temperature range.

* get_status : This command causes the firmware to generate a "status"
  response message. The host sends this command once a second to
  obtain the value of the micro-controller clock and to estimate the
  drift between host and micro-controller clocks. It enables the host
  to accurately estimate the micro-controller clock.

Stepper commands
----------------

* queue_step oid=%c interval=%u count=%hu add=%hi : This command
  schedules 'count' number of steps for the given stepper, with
  'interval' number of clock ticks between each step. The first step
  will be 'interval' number of clock ticks since the last scheduled
  step for the given stepper. If 'add' is non-zero then the interval
  will be adjusted by 'add' amount after each step. This command
  appends the given interval/count/add sequence to a per-stepper
  queue. There may be hundreds of these sequences queued during normal
  operation. New sequence are appended to the end of the queue and as
  each sequence completes its 'count' number of steps it is popped
  from the front of the queue. This system allows the firmware to
  queue potentially hundreds of thousands of steps - all with reliable
  and predictable schedule times.

* set_next_step_dir oid=%c dir=%c : This command specifies the value
  of the dir_pin that the next queue_step command will use.

* reset_step_clock oid=%c clock=%u : Normally, step timing is relative
  to the last step for a given stepper. This command resets the clock
  so that the next step is relative to the supplied 'clock' time. The
  host usually only sends this command at the start of a print.

* stepper_get_position oid=%c : This command causes the firmware to
  generate a "stepper_position" response message with the stepper's
  current position.  The position is the total number of steps
  generated with dir=1 minus the total number of steps generated with
  dir=0.

* end_stop_home oid=%c clock=%u rest_ticks=%u pin_value=%c : This
  command is used during stepper "homing" operations. To use this
  command a 'config_end_stop' command with the same 'oid' parameter
  must have been issued during firmware configuration. When invoked,
  the firmware will sample the endstop pin every 'rest_ticks' clock
  ticks and check if it has a value equal to 'pin_value'. If the value
  matches then the movement queue for the associated stepper will be
  cleared and the stepper will come to an immediate halt. The host
  uses this command to implement homing - the host instructs the
  endstop to sample for the endstop trigger and then it issues a
  series of queue_step commands to the stepper to move it towards the
  endstop. Once the stepper hits the endstop, the trigger will be
  detected, the movement halted, and the host notified.

### Move queue

Each queue_step command utilizes an entry in the firmware "move
queue". The firmware allocates this queue when it receives the
"finalize_config" command, and it reports the number of available
queue entries in "config" response messages.

It is the responsibility of the host to ensure that there is available
space in the queue before sending a queue_step command. The host does
this by calculating when each queue_step command completes and
scheduling new queue_step commands accordingly.
