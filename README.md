# Mightyboard Rev G and H support - Fork of Klipper3d/klipper

This fork adds support for Makerbot Replicator2/2X
(Mightyboard rev G and H) and some clones.

**STATUS** - I would consider master to be at a late beta stage.
Everything that can be supported is at this point and all
features are working.  This has been tested on a small number
of printers and more wide spread testing would be appreciated.
The example config file will be updated to incorporate
setting hold currents while pre-heating.

**Supported clones**
Currently, no clones have example config files or instructions
for flashing Klipper.  If you have a Replicator clone that
uses k type thermocouples and/or an ADS1118 adc chip and
want to get Klipper running on it let me know and submit
a sample config file and flashing instructions and I will
include them in this repo.

Printers that may use ADS1118 ADCs and may work with this
repo include:
- FlashForge DreamerNX
- FlashForge Dreamer
- FlashForge Inventor
- Dremel 3D20
- Dremel 3D40
- PowerSpec Ultra 3D
- Monoprice Inventor 1

**Changes include:**
* Documents what is necessary to flash the MCU (57600 baud and correct 
    avrdude protocol)
* Adds support for MCU reset (toggle connection baud rate to 57600)
* Adds support for ADS1118 and k-type thermocouples (one or two extruders)
* Adds support for hardware blinking of the LEDs
* Adds support for multiple buttons with same action in display
* Removes the need for specifying dummy pins for software spi devices
* Adds example g code macros to emulate some original Makerbot behaviors

[/config/printer-makerbot-replicator2x-2012.cfg](/config/printer-makerbot-replicator2x-2012.cfg) can be used as a starting point for the Replicator 2X.
[/config/printer-makerbot-replicator2-2012.cfg](/config/printer-makerbot-replicator2-2012.cfg) can be used as a starting point for the Replicator 2.

# Installation

<details><summary>KIAUH</summary>
<p>

**NOTE:** KIAUH takes a little bit more work to set up, but is ideal for running
multiple instances of klipper, and also makes it possible to install any and
all frontends including Mainsail, Fluidd, and Octoprint! A huge thanks to
th33xitus for making [KIAUH](https://github.com/th33xitus/kiauh)!

**Step 1:**

Follow the instruction on [KIAUH's github page](https://github.com/th33xitus/kiauh#--%EF%B8%8F-instructions-) to install both the recommended operating
system for KIAUH and the script itself.

**Step 2:**

KIAUH is set by default to install klipper from the klipper3d repo, however, we
need to install it from the dockterj branch. This can be accomplished by editing
the file `klipper_repos.txt.example`.

SSH into your raspberry pi, and run the following commands:
```
cd kiauh
sudo nano klipper_repos.txt.example
```
Now, you should be presented with a screen that looks like this:

![](https://i.imgur.com/kWNYJNQ.png)

Delete the four uncommented lines, and add the line `https://github.com/dockterj/klipper` in their place.
Your screen should now look like this:

![](https://i.imgur.com/UTat7JL.png)

Now, hit Control+x to exit, then hit `y` to save the modified buffer.

**You must save the file as `klipper_repos.txt`, otherwise KIAUH will not recognize the file as valid.**

Hit `y` to save the file under a different name.

If you want to check to be sure the edit was successful, you can type `ls` into the command line. If the
file `klipper_repos.txt` is there, then the edit was successful.

**Step 3:**

Now you're ready to start KIAUH! Enter `./kiauh/kiauh.sh` to start the script.

Once in KIAUH, you should see a screen like this:

![](https://i.imgur.com/mJKOZqq.png)

KIAUH, by default, uses the official master branch of klipper. To tell KIAUH to
use this branch instead, we need to add the custom repo you created to KIAUH.

Input the number `6` to enter KIAUH's settings, then hit enter. 
You should now see a screen like this:

![](https://i.imgur.com/AMtFrQm.png)

Input `1` to set the custom repository. You'll see a screen like this:

![](https://i.imgur.com/w1uRnS1.png)

Input `0` to set the custom repository, you'll be presented with a screen like this:

![](https://i.imgur.com/0CWf11R.png)

Then input `b` and `b` again to get back to the main menu.

**Step 4:**

Now you can install Klipper! Input `1` to get to the installation menu. Once there,
input `1` again to enter the klipper installation procedure.

Input `1` to install python 3.x, then set the number of Klipper instances your would
like to install.

![](https://i.imgur.com/lEuWk4T.png)

You may be presented with a prompt like this:

![](https://i.imgur.com/mHdaWpC.png)

Input `y` for this prompt to avoid potential problems down the road.

Let the process complete, and now Klipper is fully installed! From here you can
install Moonraker, your interface of choice, and whatever other programs you may want 
that KIAUH offers.

**Step 5:**

Depending on the printer that you are running, you will either want to use the
`/config/printer-makerbot-replicator2-2012.cfg` config file or the
`/config/printer-makerbot-replicator2x-2012.cfg` config file.

Mainsail and Fluidd both allow you to copy over example configs from
within the web interface, however, Octoprint does not. These following steps
will detail the process of doing this through the ssh terminal. If this does
not apply to you, you can skip to the next step.
    
If you are still in KIAUH, input `Q` to close it, then type `cd` to navigate
back to the pi directory, which is represented by a `~`. The text preceding
your cursor will look similar to `pi@replicator2x:~ $`
    
Type `cd klipper/config` into the command line. You are now in Klipper's example
config folder. Type either `cp printer-makerbot-replicator2-2012.cfg ~/printer_data/config/`
or `cp printer-makerbot-replicator2x-2012.cfg ~/printer_data/config/` to copy the example
config for your printer into your printer's configuration directory.
    
Now type `cd ~/printer_data/config` to move over to your printer's configuration directory.
Type `rm printer.cfg` to remove the placeholder printer config that comes with klipper. Now
type `mv printer-makerbot-replicator2-2012.cfg printer.cfg` or 
`mv printer-makerbot-replicator2x-2012.cfg printer.cfg`, depening on your printer, to rename
the example config to the config name recognized by Klipper.

**Step 6:**

Now we're going to flash Klipper to the printer's mainboard.
    
Plug the printer into one of the Pi's usb ports. Verify that the device
appears in /dev/serial/by-id by executing the command `ls /dev/serial/by-id`.
If the printer is connected, this command will return a dialogue that looks like
this:

![](https://i.imgur.com/zxS6FMs.png)

Copy this by highlighting it, then right clicking it. Open up a temporary notepad document
and paste it into that, you'll need this to update your printer.cfg.

Find the section of your config that looks like this:
```
[mcu]
serial: dev/serial/by-id/[your-serial-id-here]
restart_method: mightyboard
baud: 250000
```

Update the `serial: [your-serial-id-here]` line with the line you saved earlier.
When completed the line should look similar to:
```
serial: /dev/serial/by-id/usb-MakerBot_Industries_The_Replicator_5533034353435160A141-if00
```
    
Now, go back to the command line and run:
```
cd ~/klipper/
make menuconfig
```
   
In this menu, choose the atmega1280, 16mhz, and uart0.

**Note:** See below for the note about atmega2560.
    
Once you enter those settings, hit `Q` to exit, then `Y` to save.
    
Now run the command `make`. This will build the firmware for your
particular machine.
    
Now, run the commands:
```
sudo service klipper stop
make flash FLASH_DEVICE=/dev/serial/by-id/dev/serial/by-id/[your-serial-id-here]
sudo service klipper start
```
Where an example of FLASH_DEVICE will look like:
```
FLASH_DEVICE=/dev/serial/by-id/dev/serial/by-id/usb-MakerBot_Industries_The_Replicator_5533034353435160A141-if00
```
This will flash your printer. If it fails, you may need to power off and then power on your printer,
or attempt to connect and disconnect with Klipper. It should flash after you try one or both of these things.

Congratulations! You now have Klipper installed on your Replicator 2 or 2X!
Head over to https://www.klipper3d.org/ for documentation on futher tuning
of your printer and to learn the specifics of what Klipper is capable of.

</p>
</details>

<details><summary>Mainsail OS</summary>
<p>

**NOTE:** I am using Mainsail OS for these install instructions but you can
use any frontend and install method you want.  The only changes needed from
this repo are in Klipper.

Create an SD card with Mainsail OS (using Raspberry Pi Imager).  Follow the
instructions from Mainsail OS https://docs.mainsail.xyz/setup/mainsail-os
Note that there is a bug in Mainsail OS v1.0.1 that will cause the wifi
connection to bounce up and down until Sonar is updated.  To work around
this bug, once the Pi has booted IMMEDIATELY SSH into the Pi and execute
"systemctl stop sonar".  After that navigate to the Machine page and update
all components.

SSH into the Pi (user pi, password is what you set up when you created
the SD card).  Execute the following commands:
mv klipper klipper-orig
git clone https://github.com/dockterj/klipper
systemctl restart klipper

At this point if you refresh update manager on the machine page it will
say that klipper is invalid.  Ignore this and don't click on "hard recovery"
or "soft recovery."  Klipper should also report ERROR.  This is normal as
there is no printer configured yet.

Copy /config/printer-makerbot-replicator2x-2012.cfg to printer.cfg.  An
easy way to do this is to change the root directory in Config File
(on the Machine tab) to config_examples.  Find the
printer-makerbot-replicator2x-2012.cfg file, right click and "download"
it.  Change the root directory back to config, upload that file, then
rename it to printer.cfg.

Plug the printer in to the Pi's usb port and verify that the device
appears in /dev/serial/by-id by executing the following:
ls /dev/serial/by-id
Make a note of the name of the printer so you can update
printer.cfg in the next steps.

Edit this file to add/remove features specific to your printer
(e.g. remove HBP, change the HBP sensor to match what you have,
change x,y, and z limits).  Update the "serial:" line under [mcu]
to match the filename found in the previous step.  Add the following
two lines at the top of you printer.cfg file to enable Mainsail
support.
[include mainsail.cfg]
Click save and restart.

Following the normal installation steps, run make menuconfig.  Choose 
an atmega1280, 16mhz, and uart0.  (see below for note about atmega2560).

Run make flash.  This should flash your mightyboard.  If not, I have
found times where I needed to power the mightboard off and back on
or attempt to connect and disconnect with Klipper (i.e. connect at a
speed other than 57600 first) before it would flash.

At this point you should have Klipper running on your Replicator.
Follow the normal Klipper documentation for further tuning.

</p>
</details>

# Notes
* The Sailfish firmware has profiles for mightyboard rev g and rev
h printers that have atmega2560 instead of atmega1280.  If you have
one of these please let me know so I can get a known working config.
I believe you should be able to get this working by changing the MCU
processor in make menuconfig and editing src/arv/Makefile and changing
the last line from "-C stk500v1" to "-C stk500v2".  Software reset
should work but needs to be tested to confirm.
* The generic-mightyboard.cfg in the main repo should NOT be use
for these printers.  That config file is for the original
Makerbot Replicator and clones and does not work with printers
that have ADS1118 adc and thermocouples.  Make sure to use the
config file specific for the rep2x as a basis for these printers.
* There is no specific error for a disconnected thermocouple however
the printer will shut down if a thermocouple is not attached (this
triggers a temperature out of range error).
* The included
printer.cfg changes the origin (0,0) to be in the left front of the build
plate to be consistent with other cartesian printers.  Keeping the original
Replicator origin (which resembles a delta printer) requires updating
position_min, position_max, position_endstop, and bed_screws values for the
x and y axix
* Please watch or star this repo if you are interested.  The more
people that use this the better the chances of getting it included
upstream.  Feel free to file issues in this repo for questions or
problems (DON'T USE ISSUES IN UPSTREAM FOR THIS)

*************************************************************************
# Welcome to the Klipper project!

[![Klipper](docs/img/klipper-logo-small.png)](https://www.klipper3d.org/)

https://www.klipper3d.org/

Klipper is a 3d-Printer firmware. It combines the power of a general
purpose computer with one or more micro-controllers. See the
[features document](https://www.klipper3d.org/Features.html) for more
information on why you should use Klipper.

To begin using Klipper start by
[installing](https://www.klipper3d.org/Installation.html) it.

Klipper is Free Software. See the [license](COPYING) or read the
[documentation](https://www.klipper3d.org/Overview.html). We depend on
the generous support from our
[sponsors](https://www.klipper3d.org/Sponsors.html).
