# OctoPrint for Klipper

Klipper has a few options for its front ends, Octoprint was the first
and original front end for Klipper. This document will give
a brief overview of installing with this option.

## Install with OctoPi

Start by installing [OctoPi](https://github.com/guysoft/OctoPi) on the
Raspberry Pi computer. Use OctoPi v0.17.0 or later - see the
[OctoPi releases](https://github.com/guysoft/OctoPi/releases) for
release information.

One should verify that OctoPi boots and that the
OctoPrint web server works. After connecting to the OctoPrint web
page, follow the prompt to upgrade OctoPrint if needed.

After installing OctoPi and upgrading OctoPrint, it will be necessary
to ssh into the target machine to run a handful of system commands.

Start by running these commands on your host device:

__If you do not have git installed, please do so with:__
```
sudo apt install git
```
then proceed:
```
cd ~
git clone https://github.com/Klipper3d/klipper
./klipper/scripts/install-octopi.sh
```

The above will download Klipper, install the needed system dependencies,
setup Klipper to run at system startup, and start the Klipper host
software. It will require an internet connection and it may take a few
minutes to complete.

## Installing with KIAUH

KIAUH can be used to install OctoPrint on a variety of Linux based systems
that run a form of Debian. More information can be found
at https://github.com/dw-0/kiauh

## Configuring OctoPrint to use Klipper

The OctoPrint web server needs to be configured to communicate with the Klipper
host software. Using a web browser, login to the OctoPrint web page and then
configure the following items:

Navigate to the Settings tab (the wrench icon at the top of the page).
Under "Serial Connection" in "Additional serial ports" add:

```
~/printer_data/comms/klippy.sock
```
Then click "Save".

_In some older setups this address may be `/tmp/printer`_


Enter the Settings tab again and under "Serial Connection" change the "Serial Port"
setting to the one added above.

In the Settings tab, navigate to the "Behavior" sub-tab and select the
"Cancel any ongoing prints but stay connected to the printer" option. Click "Save".

From the main page, under the "Connection" section (at the top left of the page)
make sure the "Serial Port" is set to the new additional one added
and click "Connect". (If it is not in the available selection then
try reloading the page.)

Once connected, navigate to the "Terminal" tab and type "status" (without the quotes)
into the command entry box and click "Send". The terminal window will likely report
there is an error opening the config file - that means OctoPrint is successfully
communicating with Klipper.

Please proceed to [Installation.md](Installation.md) and the
_Building and flashing the micro-controller_ section
