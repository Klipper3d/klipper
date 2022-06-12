# Installation

These instructions assume the software will run on a Raspberry Pi
computer in conjunction with OctoPrint. It is recommended that a
Raspberry Pi 2, 3, or 4 computer be used as the host machine (see the
[FAQ](FAQ.md#can-i-run-klipper-on-something-other-than-a-raspberry-pi-3)
for other machines).

## Obtain a Klipper Configuration File

Most Klipper settings are determined by a "printer configuration file"
that will be stored on the Raspberry Pi. An appropriate configuration
file can often be found by looking in the Klipper
[config directory](../config/) for a file starting with a "printer-"
prefix that corresponds to the target printer. The Klipper
configuration file contains technical information about the printer
that will be needed during the installation.

If there isn't an appropriate printer configuration file in the
Klipper config directory then try searching the printer manufacturer's
website to see if they have an appropriate Klipper configuration file.

If no configuration file for the printer can be found, but the type of
printer control board is known, then look for an appropriate
[config file](../config/) starting with a "generic-" prefix. These
example printer board files should allow one to successfully complete
the initial installation, but will require some customization to
obtain full printer functionality.

It is also possible to define a new printer configuration from
scratch. However, this requires significant technical knowledge about
the printer and its electronics. It is recommended that most users
start with an appropriate configuration file. If creating a new custom
printer configuration file, then start with the closest example
[config file](../config/) and use the Klipper
[config reference](Config_Reference.md) for further information.

## Prepping an OS image

Start by installing [OctoPi](https://github.com/guysoft/OctoPi) on the
Raspberry Pi computer. Use OctoPi v0.17.0 or later - see the
[OctoPi releases](https://github.com/guysoft/OctoPi/releases) for
release information. One should verify that OctoPi boots and that the
OctoPrint web server works. After connecting to the OctoPrint web
page, follow the prompt to upgrade OctoPrint to v1.4.2 or later.

After installing OctoPi and upgrading OctoPrint, it will be necessary
to ssh into the target machine to run a handful of system commands. If
using a Linux or MacOS desktop, then the "ssh" software should already
be installed on the desktop. There are free ssh clients available for
other desktops (eg,
[PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/)). Use the
ssh utility to connect to the Raspberry Pi (ssh pi@octopi -- password
is "raspberry") and run the following commands:

```
git clone https://github.com/Klipper3d/klipper
./klipper/scripts/install-octopi.sh
```

The above will download Klipper, install some system dependencies,
setup Klipper to run at system startup, and start the Klipper host
software. It will require an internet connection and it may take a few
minutes to complete.

## Building and flashing the micro-controller

To compile the micro-controller code, start by running these commands
on the Raspberry Pi:

```
cd ~/klipper/
make menuconfig
```

The comments at the top of the
[printer configuration file](#obtain-a-klipper-configuration-file)
should describe the settings that need to be set during "make
menuconfig". Open the file in a web browser or text editor and look
for these instructions near the top of the file. Once the appropriate
"menuconfig" settings have been configured, press "Q" to exit, and
then "Y" to save. Then run:

```
make
```

If the comments at the top of the
[printer configuration file](#obtain-a-klipper-configuration-file)
describe custom steps for "flashing" the final image to the printer
control board then follow those steps and then proceed to
[configuring OctoPrint](#configuring-octoprint-to-use-klipper).

Otherwise, the following steps are often used to "flash" the printer
control board. First, it is necessary to determine the serial port
connected to the micro-controller. Run the following:

```
ls /dev/serial/by-id/*
```

It should report something similar to the following:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

It's common for each printer to have its own unique serial port name.
This unique name will be used when flashing the micro-controller. It's
possible there may be multiple lines in the above output - if so,
choose the line corresponding to the micro-controller (see the
[FAQ](FAQ.md#wheres-my-serial-port) for more information).

For common micro-controllers, the code can be flashed with something
similar to:

```
sudo service klipper stop
make flash FLASH_DEVICE=/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
sudo service klipper start
```

Be sure to update the FLASH_DEVICE with the printer's unique serial
port name.

When flashing for the first time, make sure that OctoPrint is not
connected directly to the printer (from the OctoPrint web page, under
the "Connection" section, click "Disconnect").

## Configuring OctoPrint to use Klipper

The OctoPrint web server needs to be configured to communicate with
the Klipper host software. Using a web browser, login to the OctoPrint
web page and then configure the following items:

Navigate to the Settings tab (the wrench icon at the top of the
page). Under "Serial Connection" in "Additional serial ports" add
"/tmp/printer". Then click "Save".

Enter the Settings tab again and under "Serial Connection" change the
"Serial Port" setting to "/tmp/printer".

In the Settings tab, navigate to the "Behavior" sub-tab and select the
"Cancel any ongoing prints but stay connected to the printer"
option. Click "Save".

From the main page, under the "Connection" section (at the top left of
the page) make sure the "Serial Port" is set to "/tmp/printer" and
click "Connect". (If "/tmp/printer" is not an available selection then
try reloading the page.)

Once connected, navigate to the "Terminal" tab and type "status"
(without the quotes) into the command entry box and click "Send". The
terminal window will likely report there is an error opening the
config file - that means OctoPrint is successfully communicating with
Klipper. Proceed to the next section.

## Configuring Klipper

The next step is to copy the
[printer configuration file](#obtain-a-klipper-configuration-file) to
the Raspberry Pi.

Arguably the easiest way to set the Klipper configuration file is to
use a desktop editor that supports editing files over the "scp" and/or
"sftp" protocols. There are freely available tools that support this
(eg, Notepad++, WinSCP, and Cyberduck). Load the printer config file
in the editor and then save it as a file named "printer.cfg" in the
home directory of the pi user (ie, /home/pi/printer.cfg).

Alternatively, one can also copy and edit the file directly on the
Raspberry Pi via ssh. That may look something like the following (be
sure to update the command to use the appropriate printer config
filename):

```
cp ~/klipper/config/example-cartesian.cfg ~/printer.cfg
nano ~/printer.cfg
```

It's common for each printer to have its own unique name for the
micro-controller. The name may change after flashing Klipper, so rerun
these steps again even if they were already done when flashing. Run:

```
ls /dev/serial/by-id/*
```

It should report something similar to the following:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

Then update the config file with the unique name. For example, update
the `[mcu]` section to look something similar to:

```
[mcu]
serial: /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

After creating and editing the file it will be necessary to issue a
"restart" command in the OctoPrint web terminal to load the config. A
"status" command will report the printer is ready if the Klipper
config file is successfully read and the micro-controller is
successfully found and configured.

When customizing the printer config file, it is not uncommon for
Klipper to report a configuration error. If an error occurs, make any
necessary corrections to the printer config file and issue "restart"
until "status" reports the printer is ready.

Klipper reports error messages via the OctoPrint terminal tab. The
"status" command can be used to re-report error messages. The default
Klipper startup script also places a log in **/tmp/klippy.log** which
provides more detailed information.

After Klipper reports that the printer is ready, proceed to the
[config check document](Config_checks.md) to perform some basic checks
on the definitions in the config file. See the main
[documentation reference](Overview.md) for other information.
