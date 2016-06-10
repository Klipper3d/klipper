The Klippy host code has some tools to help in debugging the firmware.

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

and compile the firmware for an AVR atmega644p, disable the AVR
watchdog timer, and set the MCU frequency to 20000000. Then one can
compile Klipper (run `make`) and then start the simulation with:

```
PYTHONPATH=/path/to/simulavr/src/python/ ./scripts/avrsim.py -m atmega644 -s 20000000 -b 250000 out/klipper.elf
```

It may be necessary to create a python virtual environment to run
Klippy on the target machine. To do so, run:

```
virtualenv ~/klippy-env
~/klippy-env/bin/pip install cffi==1.6.0 pyserial==2.7
```

Then, with simulavr running in another window, one can run the
following to read gcode from a file (eg, "test.gcode"), process it
with Klippy, and send it to Klipper running in simulavr:

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
-------------------------------------------------

Normally, Klippy would be used to translate gcode commands to Klipper
commands. However, it's also possible to manually send Klipper
commands (functions marked with the DECL_COMMAND() macro in the
Klipper source code). To do so, run:

```
~/klippy-env/bin/python ./klippy/console.py /tmp/pseudoserial 250000
```
