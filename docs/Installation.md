These instructions assume the software will run on a Raspberry Pi
computer in conjunction with OctoPrint. It is recommended that a
Raspberry Pi 2 or Raspberry Pi 3 computer be used as the host
machine.

It should be possible to run the Klipper host software on any computer
running a recent Linux distribution, but doing so will require Linux
admin knowledge to translate these installation instructions to the
particulars of that machine.

Klipper currently supports Atmel ATmega based micro-controllers and
Arduino Due (Atmel SAM3x8e ARM micro-controller) printers.

Prepping an OS image
====================

Start by installing [OctoPi](https://github.com/guysoft/OctoPi) on the
Raspberry Pi computer. Use OctoPi v0.13.0 or later - see the
[octopi releases](https://github.com/guysoft/OctoPi/releases) for
release information. One should verify that OctoPi boots and that the
OctoPrint web server works. After connecting to the OctoPrint web
page, follow the prompt to upgrade OctoPrint to v1.3.2 or later.

After installing OctoPi and upgrading OctoPrint, ssh into the target
machine (ssh pi@octopi -- password is "raspberry") and run the
following commands:

```
git clone https://github.com/KevinOConnor/klipper
./klipper/scripts/install-octopi.sh
```

The above will download Klipper, install some system dependencies,
setup Klipper to run at system startup, and start the Klipper host
software. It will require an internet connection and it may take a few
minutes to complete.

Building and flashing the micro-controller
==========================================

To compile the micro-controller code, start by configuring it:

```
cd ~/klipper/
make menuconfig
```

Select the appropriate micro-controller and serial baud rate. Once
configured, run:

```
make
```

Finally, for common micro-controllers, the code can be flashed with:

```
make flash FLASH_DEVICE=/dev/ttyACM0
```

Configuring Klipper
===================

The Klipper configuration is stored in a text file on the Raspberry
Pi. Take a look at the example config files in the
[config directory](../config/). The
[example.cfg](../config/example.cfg) file contains documentation on
command parameters and it can also be used as an initial config file
template. However, for most printers, one of the other config files
may be a more concise starting point. The next step is to copy and
edit one of these config files - for example:

```
cp ~/klipper/config/example.cfg ~/printer.cfg
nano ~/printer.cfg
```

Make sure to review and update each setting that is appropriate for
the hardware.

Configuring OctoPrint to use Klipper
====================================

The OctoPrint web server needs to be configured to communicate with
the Klipper host software. Using a web browser, login to the OctoPrint
web page, and navigate to the Settings tab. Then configure the
following items:

Under "Serial Connection" in "Additional serial ports" add
"/tmp/printer". Then click "Save".

Enter the Settings tab again and under "Serial Connection" change the
"Serial Port" setting to "/tmp/printer". Unselect the "Not only cancel
ongoing prints but also disconnect..." checkbox. Click "Save".

From the main page, under the "Connection" section (at the top left of
the page) make sure the "Serial Port" is set to "/tmp/printer" and
click "Connect". (If "/tmp/printer" is not an available selection then
try reloading the page.)

Once connected, navigate to the "Terminal" tab and type "status"
(without the quotes) into the command entry box and click "Send". If
the Klipper config file was successfully read, and the
micro-controller was successfully found and configured, then this
command will report that the printer is ready. It is not unusual to
have a configuration error during the initial setup - one may modify
the printer config file and then issue a "restart" command in the
OctoPrint terminal to reload the config. Continue modifying the config
and restarting until "status" reports the printer is ready.

Klipper reports error messages via the OctoPrint terminal tab. The
"status" command can be used to re-report error messages. The default
Klipper startup script also places a log in **/tmp/klippy.log** which
provides more detailed information.

In addition to common g-code commands, Klipper supports a few extended
commands - "status" and "restart" are examples of these commands. Use
the "help" command to get a list of other extended commands.
