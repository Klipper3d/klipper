This document describes the overall code layout and major code flow of
Klipper.

Directory Layout
================

The **src/** directory contains the C source for the micro-controller
code. The **src/avr/** directory contains specific code for Atmel
ATmega micro-controllers. The **src/sam3x8e/** directory contains code
specific to the Arduino Due style ARM micro-controllers. The
**src/simulator/** contains code stubs that allow the micro-controller
to be test compiled on other architectures. The **src/generic/**
directory contains helper code that may be useful across different
host architectures. The build arranges for includes of
"board/somefile.h" to first look in the current architecture directory
(eg, src/avr/somefile.h) and then in the generic directory (eg,
src/generic/somefile.h).

The **klippy/** directory contains the C and Python source for the
host part of the software.

The **lib/** directory contains external 3rd-party library code that
is necessary to build some targets.

The **config/** directory contains example printer configuration
files.

The **scripts/** directory contains build-time scripts useful for
compiling the micro-controller code.

During compilation, the build may create an **out/** directory. This
contains temporary build time objects. The final micro-controller
object that is built is **out/klipper.elf.hex** on AVR and
**out/klipper.bin** on ARM.

Micro-controller code flow
==========================

Execution of the micro-controller code starts in architecture specific
code (eg, **src/avr/main.c**) which ultimately calls sched_main()
located in **src/sched.c**. The sched_main() code starts by running
all functions that have been tagged with the DECL_INIT() macro. It
then goes on to repeatedly run all functions tagged with the
DECL_TASK() macro.

One of the main task functions is command_task() located in
**src/command.c**. This function processes incoming serial commands
and runs the associated command function for them. Command functions
are declared using the DECL_COMMAND() macro.

Task, init, and command functions always run with interrupts enabled
(however, they can temporarily disable interrupts if needed). These
functions should never pause, delay, or do any work that lasts more
than a few micro-seconds. These functions schedule work at specific
times by scheduling timers.

Timer functions are scheduled by calling sched_add_timer() (located in
**src/sched.c**). The scheduler code will arrange for the given
function to be called at the requested clock time. Timer interrupts
are initially handled in an architecture specific interrupt handler
(eg, **src/avr/timer.c**) which calls sched_timer_dispatch() located
in **src/sched.c**. The timer interrupt leads to execution of schedule
timer functions. Timer functions always run with interrupts
disabled. The timer functions should always complete within a few
micro-seconds. At completion of the timer event, the function may
choose to reschedule itself.

In the event an error is detected the code can invoke shutdown() (a
macro which calls sched_shutdown() located in **src/sched.c**).
Invoking shutdown() causes all functions tagged with the
DECL_SHUTDOWN() macro to be run. Shutdown functions always run with
interrupts disabled.

Much of the functionality of the micro-controller involves working
with General-Purpose Input/Output pins (GPIO). In order to abstract
the low-level architecture specific code from the high-level task
code, all GPIO events are implemented in architectures specific
wrappers (eg, **src/avr/gpio.c**). The code is compiled with gcc's
"-flto -fwhole-program" optimization which does an excellent job of
inlining functions across compilation units, so most of these tiny
gpio functions are inlined into their callers, and there is no
run-time cost to using them.

Klippy code overview
====================

The host code (Klippy) is intended to run on a low-cost computer (such
as a Raspberry Pi) paired with the micro-controller. The code is
primarily written in Python, however it does use CFFI to implement
some functionality in C code.

Initial execution starts in **klippy/klippy.py**. This reads the
command-line arguments, opens the printer config file, instantiates
the main printer objects, and starts the serial connection. The main
execution of G-code commands is in the process_commands() method in
**klippy/gcode.py**. This code translates the G-code commands into
printer object calls, which frequently translate the actions to
commands to be executed on the micro-controller (as declared via the
DECL_COMMAND macro in the micro-controller code).

There are four threads in the Klippy host code. The main thread
handles incoming gcode commands. A second thread (which resides
entirely in the **klippy/serialqueue.c** C code) handles low-level IO
with the serial port. The third thread is used to process response
messages from the micro-controller in the Python code (see
**klippy/serialhdl.py**). The fourth thread writes debug messages to
the log (see **klippy/queuelogger.py**) so that the other threads
never block on log writes.

Code flow of a move command
===========================

A typical printer movement starts when a "G1" command is sent to the
Klippy host and it completes when the corresponding step pulses are
produced on the micro-controller. This section outlines the code flow
of a typical move command. The [kinematics](Kinematics.md) document
provides further information on the mechanics of moves.

* Processing for a move command starts in gcode.py. The goal of
  gcode.py is to translate G-code into internal calls. Changes in
  origin (eg, G92), changes in relative vs absolute positions (eg,
  G90), and unit changes (eg, F6000=100mm/s) are handled here. The
  code path for a move is: `process_data() -> process_commands() ->
  cmd_G1()`. Ultimately the ToolHead class is invoked to execute the
  actual request: `cmd_G1() -> ToolHead.move()`

* The ToolHead class (in toolhead.py) handles "look-ahead" and tracks
  the timing of printing actions. The codepath for a move is:
  `ToolHead.move() -> MoveQueue.add_move() -> MoveQueue.flush() ->
  Move.set_junction() -> Move.move()`.
  * ToolHead.move() creates a Move() object with the parameters of the
  move (in cartesian space and in units of seconds and millimeters).
  * MoveQueue.add_move() places the move object on the "look-ahead"
  queue.
  * MoveQueue.flush() determines the start and end velocities of each
  move.
  * Move.set_junction() implements the "trapezoid generator" on a
  move. The "trapezoid generator" breaks every move into three parts:
  a constant acceleration phase, followed by a constant velocity
  phase, followed by a constant deceleration phase. Every move
  contains these three phases in this order, but some phases may be of
  zero duration.
  * When Move.move() is called, everything about the move is known -
  its start location, its end location, its acceleration, its
  start/crusing/end velocity, and distance traveled during
  acceleration/cruising/deceleration. All the information is stored in
  the Move() class and is in cartesian space in units of millimeters
  and seconds. Times are stored relative to the start of the print.

  The move is then handed off to the kinematics classes: `Move.move()
  -> kin.move()`

* The goal of the kinematics classes is to translate the movement in
  cartesian space to movement on each stepper. The kinematics classes
  are in cartesian.py, corexy.py, delta.py, and extruder.py. The
  kinematic class is given a chance to audit the move
  (`ToolHead.move() -> kin.check_move()`) before it goes on the
  look-ahead queue, but once the move arrives in *kin*.move() the
  kinematic class is required to handle the move as specified. The
  kinematic classes translate the three parts of each move
  (acceleration, constant "cruising" velocity, and deceleration) to
  the associated movement on each stepper. Note that the extruder is
  handled in its own kinematic class. Since the Move() class specifies
  the exact movement time and since step pulses are sent to the
  micro-controller with specific timing, stepper movements produced by
  the extruder class will be in sync with head movement even though
  the code is kept separate.

* For efficiency reasons, the stepper pulse times are generated in C
  code. The code flow is: `kin.move() -> MCU_Stepper.step_const() ->
  stepcompress_push_const()`, or for delta kinematics:
  `DeltaKinematics.move() -> MCU_Stepper.step_delta() ->
  stepcompress_push_delta()`. The MCU_Stepper code just performs unit
  and axis transformation (seconds to clock ticks and millimeters to
  step distances), and calls the C code. The C code calculates the
  stepper step times for each movement and fills an array (struct
  stepcompress.queue) with the corresponding micro-controller clock
  counter times (in 64bit integers) for every step. Here the
  "micro-controller clock counter" value directly corresponds to the
  micro-controller's hardware counter - it is relative to when the
  micro-controller was last powered up.

* The next major step is to compress the steps: `stepcompress_flush()
  -> compress_bisect_add()` (in stepcompress.c). This code generates
  and encodes a series of micro-controller "queue_step" commands that
  correspond to the list of stepper step times built in the previous
  stage. These "queue_step" commands are then queued, prioritized, and
  sent to the micro-controller (via stepcompress.c:steppersync and
  serialqueue.c:serialqueue).

* Processing of the queue_step commands on the micro-controller starts
  in command.c which parses the command and calls
  `command_queue_step()`. The command_queue_step() code (in stepper.c)
  just appends the parameters of each queue_step command to a per
  stepper queue. Under normal operation the queue_step command is
  parsed and queued at least 100ms before the time of its first
  step. Finally, the generation of stepper events is done in
  `stepper_event()`. It's called from the hardware timer interrupt at
  the scheduled time of the first step. The stepper_event() code
  generates a step pulse and then reschedules itself to run at the
  time of the next step pulse for the given queue_step parameters. The
  parameters for each queue_step command are "interval", "count", and
  "add". At a high-level, stepper_event() runs the following, 'count'
  times: `do_step(); next_wake_time = last_wake_time + interval;
  interval += add;`

The above may seem like a lot of complexity to execute a
movement. However, the only really interesting parts are in the
ToolHead and kinematic classes. It's this part of the code which
specifies the movements and their timings. The remaining parts of the
processing is mostly just communication and plumbing.
