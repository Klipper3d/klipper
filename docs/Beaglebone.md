# Beaglebone

This document describes the process of running Klipper on a Beaglebone
PRU.

## Building an OS image

Start by installing the
[Debian 11.7 2023-09-02 4GB microSD IoT](https://beagleboard.org/latest-images)
image. One may run the image from either a micro-SD card or from
builtin eMMC. If using the eMMC, install it to eMMC now by following
the instructions from the above link.

Then ssh into the Beaglebone machine (`ssh debian@beaglebone` --
password is `temppwd`) and check that required devices are present

```
sudo beagle-version
```
You should check that output contains successful "remoteproc" drivers loading and presence of PRU cores,
in Kernel 5.10 they should be "remoteproc1" and "remoteproc2" (4a334000.pru, 4a338000.pru)
Also check that many GPIOs are loaded they will look like "Allocated GPIO id=0 name='P8_03'" 
Usually everything is fine and no hardware configuration is required. 
If something is missing - try to play with "uboot overlays" options or with cape-overlays
Just for reference some output of working BeagleBone Black configuration with CRAMPS board:
```
model:[TI_AM335x_BeagleBone_Black]
UBOOT: Booted Device-Tree:[am335x-boneblack-uboot-univ.dts]
UBOOT: Loaded Overlay:[BB-ADC-00A0.bb.org-overlays]
UBOOT: Loaded Overlay:[BB-BONE-eMMC1-01-00A0.bb.org-overlays]
kernel:[5.10.168-ti-r71]
/boot/uEnv.txt Settings:
uboot_overlay_options:[enable_uboot_overlays=1]
uboot_overlay_options:[disable_uboot_overlay_video=1]
uboot_overlay_options:[disable_uboot_overlay_audio=1]
uboot_overlay_options:[disable_uboot_overlay_wireless=1]
uboot_overlay_options:[enable_uboot_cape_universal=1]
pkg:[bb-cape-overlays]:[4.14.20210821.0-0~bullseye+20210821]
pkg:[bb-customizations]:[1.20230720.1-0~bullseye+20230720]
pkg:[bb-usb-gadgets]:[1.20230414.0-0~bullseye+20230414]
pkg:[bb-wl18xx-firmware]:[1.20230414.0-0~bullseye+20230414]
```

Before start installing Klipper you need to free-up additional space.
there are 3 options to do that:
1. remove some BeagleBone "Demo" resources
2. if you did boot from SD-Card, and it's bigger than 4Gb - you can expand 
current filesystem to take whole card space
3. do option #1 and #2 together.

1. To remove some BeagleBone "Demo" resources execute these commands
```
sudo rm -R /opt/bb-code-server
sudo rm -R /opt/node-red
```

2. To expand filesystem to full size of your SD-Card execute this command, reboot is not required.
```
sudo growpart /dev/mmcblk0 1
sudo resize2fs /dev/mmcblk0p1
```


Install Klipper by running the following
commands:

```
git clone https://github.com/Ga-Ol-St/klipper.git
./klipper/scripts/install-beaglebone.sh
```

## Install Octoprint

One may then install Octoprint:
```
git clone https://github.com/foosel/OctoPrint.git
cd OctoPrint/
virtualenv venv
./venv/bin/python setup.py install
```

And setup OctoPrint to start at bootup:
```
sudo cp ~/OctoPrint/scripts/octoprint.init /etc/init.d/octoprint
sudo chmod +x /etc/init.d/octoprint
sudo cp ~/OctoPrint/scripts/octoprint.default /etc/default/octoprint
sudo update-rc.d octoprint defaults
```

It is necessary to modify OctoPrint's **/etc/default/octoprint**
configuration file. One must change the `OCTOPRINT_USER` user to
`debian`, change `NICELEVEL` to `0`, uncomment the `BASEDIR`, `CONFIGFILE`,
and `DAEMON` settings and change the references from `/home/pi/` to
`/home/debian/`:
```
sudo nano /etc/default/octoprint
```

Then start the Octoprint service:
```
sudo systemctl start octoprint
```

Make sure the OctoPrint web server is accessible - it should be at:
[http://beaglebone:5000/](http://beaglebone:5000/)

## Building the micro-controller code

To compile the Klipper micro-controller code, start by configuring it
for the "Beaglebone PRU", for "BeagleBone Black" additionally disable options "Support GPIO Bit-banging devices" 
and disable "Support LCD devices" because they will not fit in 8Kb PRU, then exit and save config:
```
cd ~/klipper/
make menuconfig
```

To build and install the new micro-controller code, run:
```
sudo service klipper stop
make clean flash
sudo service klipper start
```

It is also necessary to compile and install the micro-controller code
for a Linux host process. Configure it a second time for a "Linux process":
```
make menuconfig
```

Then install this micro-controller code as well:
```
sudo service klipper stop
make clean flash
sudo service klipper start
```

## Remaining configuration

Take a note about following: when you will define printer configuration you should always
use temperature sensors from "mcu host" because ADCs not present in default "mcu" (PRU cores)
sample configuration of "sensor_pin" for extruder and heated bed are available in "generic-cramps.cfg"
You can use any other GPIO directly from "mcu host" by referencing them this way "host:gpiochip1/gpio17"
but that should be avoided because it will be creating additional load on main CPU

Complete the installation by configuring Klipper and Octoprint
following the instructions in
the main [Installation](Installation.md#configuring-klipper) document.

## Printing on the Beaglebone

Unfortunately, the Beaglebone processor can sometimes struggle to run
OctoPrint well. Print stalls have been known to occur on complex
prints (the printer may move faster than OctoPrint can send movement
commands). If this occurs, consider using the "virtual_sdcard" feature
(see [Config Reference](Config_Reference.md#virtual_sdcard) for
details) to print directly from Klipper 
and disable any DEBUG or VERBOSE logging options if you did enable them.


## AVR micro-controller code build 
This environment have everything to build necessary micro-controller code except AVR,
AVR packages was removed because of conflict with PRU packages.
if you still want to build AVR micro-controller code in this environment you need to remove
PRU packages and install AVR packages by executing following commands

```
sudo apt-get remove gcc-pru
sudo apt-get install avrdude gcc-avr binutils-avr avr-libc
```
