This document describes the overall code layout and major code flow of
Klipper.

Directory Layout
================

The **src/** directory contains the C source for the micro-controller
code. The **src/avr/** directory contains specific code for Atmel
ATmega micro-controllers. The **src/simulator/** contains code stubs
that allow the micro-controller to be test compiled on other
architectures.

The **klippy/** directory contains the C and Python source for the
host part of the firmware.

The **config/** directory contains example printer configuration
files.

The **scripts/** directory contains build-time scripts useful for
compiling the micro-controller code.

During compilation, the build may create an **out/** directory. This
contains temporary build time objects. The final micro-controller
object that is built is in **out/klipper.elf.hex**

Micro-controller code flow
==========================

Execution of the micro-controller code starts in **src/avr/main.c**
which calls sched_main() located in **src/sched.c**. The sched_main()
code starts by running all functions that have been tagged with the
DECL_INIT() macro. It then goes on to repeatedly run all functions
tagged with the DECL_TASK() macro.

One of the main task functions is command_task() located in
**src/command.c**. This function processes incoming serial commands
and runs the associated command function for them. Command functions
are declared using the DECL_COMMAND() macro.

Task, init, and command functions always run with interrupts enabled
(however, they can temporarily disable interrupts if needed). These
functions should never pause, delay, or do any work that lasts more
than a few micro-seconds. These functions schedule work at specific
times by scheduling timers.

Timer functions are scheduled by calling sched_timer() (located in
**src/sched.c**). The scheduler code will arrange for the given
function to be called at the requested clock time. Timer interrupts
are initially handled in an interrupt handler in **src/avr/timer.c**,
but this just calls sched_timer_kick() located in **src/sched.c**. The
timer interrupt leads to execution of schedule timer functions.  Timer
functions always run with interrupts disabled. The timer functions
should always complete within a few micro-seconds. At completion of
the timer event, the function may choose to reschedule itself.

In the event an error is detected the code can invoke shutdown() (a
macro which calls sched_shutdown() located in **src/sched.c**).
Invoking shutdown() causes all functions tagged with the
DECL_SHUTDOWN() macro to be run. Shutdown functions always run with
interrupts disabled.

Much of the functionality of the micro-controller involves working
with General-Purpose Input/Output pins (GPIO). In order to abstract
the low-level architecture specific code from the high-level task
code, all GPIO events are implemented via wrappers. These wrappers are
located in **src/avr/gpio.c**. The code is compiled with gcc's "-flto
-fwhole-program" optimization which does an excellent job of inlining
functions across compilation units, so most of these tiny gpio
functions are inlined into their callers, and there is no run-time
cost to using them.

Klippy code overview
====================

The host code (Klippy) is intended to run on a low-cost computer (such
as a Raspberry Pi) paired with the micro-controller. The code is
primarily written in Python, however it does use CFFI to implement
some functionality in C code.

Initial execution starts in **klippy/klippy.py**. This reads the
command-line arguments, opens the printer config file, instantiates
the main printer objects, and starts the serial connection. The main
execution of gcode commands is in the process_commands() method in
**klippy/gcode.py**. This code translates the gcode commands into
printer object calls, which frequently translate the actions to
commands to be executed on the micro-controller (as declared via the
DECL_COMMAND macro in the micro-controller code).

There are three threads in the Klippy host code. The main thread
handles incoming gcode commands. A second thread (which resides
entirely in the **klippy/serialqueue.c** C code) handles low-level IO
with the serial port. The third thread is used to process response
messages from the micro-controller in the Python code.
