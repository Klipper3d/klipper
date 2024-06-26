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
password is `temppwd`).

Before start installing Klipper you need to free-up additional space.
there are 3 options to do that:
1. remove some BeagleBone "Demo" resources
2. if you did boot from SD-Card, and it's bigger than 4Gb - you can expand
current filesystem to take whole card space
3. do option #1 and #2 together.

To remove some BeagleBone "Demo" resources execute these commands
```
sudo apt remove bb-node-red-installer
sudo apt remove bb-code-server
```

To expand filesystem to full size of your SD-Card execute this command, reboot is not required.
```
sudo growpart /dev/mmcblk0 1
sudo resize2fs /dev/mmcblk0p1
```


Install Klipper by running the following
commands:

```
git clone https://github.com/Klipper3d/klipper.git
./klipper/scripts/install-beaglebone.sh
```

After installing Klipper you need to decide what kind of deployment do you need,
but take a note that BeagleBone is 3.3v based hardware and in most cases you can't
directly connect pins to 5v or 12v based hardware without conversion boards.

As Klipper have multimodule architecture on BeagleBone you can achieve many different use cases,
but general ones are following:

Use case 1: Use BeagleBone only as a host system to run Klipper and additional software
like OctoPrint/Fluidd + Moonraker/...  and this configuration will be driving
external micro-controllers via serial/usb/canbus connections.

Use case 2: Use BeagleBone with extension board (cape) like CRAMPS board.
in this configuration BeagleBone will host Klipper + additional software, and
it will drive extension board with BeagleBone PRU cores (2 additional cores 200Mh, 32Bit).

Use case 3: It's same as "Use case 1" but additionally you want to drive
BeagleBone GPIOs with high speed by utilizing PRU cores to offload main CPU.


## Installing Octoprint

One may then install Octoprint or fully skip this section if desired other software:
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
Wait 1-2 minutes and make sure the OctoPrint web server is accessible - it should be at:
[http://beaglebone:5000/](http://beaglebone:5000/)


## Building the BeagleBone PRU micro-controller code (PRU firmware)
This section is required for "Use case 2" and "Use case 3" mentioned above,
you should skip it for "Use case 1".

Check that required devices are present

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
uboot_overlay_options:[disable_uboot_overlay_video=0]
uboot_overlay_options:[disable_uboot_overlay_audio=1]
uboot_overlay_options:[disable_uboot_overlay_wireless=1]
uboot_overlay_options:[enable_uboot_cape_universal=1]
pkg:[bb-cape-overlays]:[4.14.20210821.0-0~bullseye+20210821]
pkg:[bb-customizations]:[1.20230720.1-0~bullseye+20230720]
pkg:[bb-usb-gadgets]:[1.20230414.0-0~bullseye+20230414]
pkg:[bb-wl18xx-firmware]:[1.20230414.0-0~bullseye+20230414]
.............
.............

```

To compile the Klipper micro-controller code, start by configuring it for the "Beaglebone PRU",
for "BeagleBone Black" additionally disable options "Support GPIO Bit-banging devices" and disable "Support LCD devices"
inside the "Optional features" because they will not fit in 8Kb PRU firmware memory,
then exit and save config:
```
cd ~/klipper/
make menuconfig
```

To build and install the new PRU micro-controller code, run:
```
sudo service klipper stop
make flash
sudo service klipper start
```
After previous commands was executed your PRU firmware should be ready and started
to check if everything was fine you can execute following command
```
dmesg
```
and compare last messages with sample one which indicate that everything started properly:
```
[   71.105499] remoteproc remoteproc1: 4a334000.pru is available
[   71.157155] remoteproc remoteproc2: 4a338000.pru is available
[   73.256287] remoteproc remoteproc1: powering up 4a334000.pru
[   73.279246] remoteproc remoteproc1: Booting fw image am335x-pru0-fw, size 97112
[   73.285807]  remoteproc1#vdev0buffer: registered virtio0 (type 7)
[   73.285836] remoteproc remoteproc1: remote processor 4a334000.pru is now up
[   73.286322] remoteproc remoteproc2: powering up 4a338000.pru
[   73.313717] remoteproc remoteproc2: Booting fw image am335x-pru1-fw, size 188560
[   73.313753] remoteproc remoteproc2: header-less resource table
[   73.329964] remoteproc remoteproc2: header-less resource table
[   73.348321] remoteproc remoteproc2: remote processor 4a338000.pru is now up
[   73.443355] virtio_rpmsg_bus virtio0: creating channel rpmsg-pru addr 0x1e
[   73.443727] virtio_rpmsg_bus virtio0: msg received with no recipient
[   73.444352] virtio_rpmsg_bus virtio0: rpmsg host is online
[   73.540993] rpmsg_pru virtio0.rpmsg-pru.-1.30: new rpmsg_pru device: /dev/rpmsg_pru30
```
take a note about "/dev/rpmsg_pru30" - it's your future serial device for main mcu configuration
this device is required to be present, if it's absent - your PRU cores did not start properly.

## Building and installing Linux host micro-controller code
This section is required for "Use case 2" and optional for "Use case 3" mentioned above

It is also necessary to compile and install the micro-controller code
for a Linux host process. Configure it a second time for a "Linux process":
```
make menuconfig
```

Then install this micro-controller code as well:
```
sudo service klipper stop
make flash
sudo service klipper start
```
take a note about "/tmp/klipper_host_mcu" - it will be your future serial device for "mcu host"
if that file don't exist - refer to "scripts/klipper-mcu.service" file, it was installed by
previous commands, and it's responsible for it.


Take a note for "Use case 2" about following: when you will define printer configuration you should always
use temperature sensors from "mcu host" because ADCs not present in default "mcu" (PRU cores).
Sample configuration of "sensor_pin" for extruder and heated bed are available in "generic-cramps.cfg"
You can use any other GPIO directly from "mcu host" by referencing them this way "host:gpiochip1/gpio17"
but that should be avoided because it will be creating additional load on main CPU and most probably
you can't use them for stepper control.


## Remaining configuration

Complete the installation by configuring Klipper
following the instructions in
the main [Installation](Installation.md#configuring-octoprint-to-use-klipper) document.

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
if you need to restore PRU packages - then remove ARV packages before that
```
sudo apt-get remove avrdude gcc-avr binutils-avr avr-libc
sudo apt-get install gcc-pru
```


## Hardware Pin designation
BeagleBone is very flexible in terms of pin designation, same pin can be configured for different function
but always single function for single pin, same function can be present on different pins.
So you can't have multiple functions on single pin or have same function on multiple pins.
Example:
P9_20 - i2c2_sda/can0_tx/spi1_cs0/gpio0_12/uart1_ctsn
P9_19 - i2c2_scl/can0_rx/spi1_cs1/gpio0_13/uart1_rtsn
P9_24 - i2c1_scl/can1_rx/gpio0_15/uart1_tx
P9_26 - i2c1_sda/can1_tx/gpio0_14/uart1_rx

Pin designation is defined by using special "overlays" which will be loaded during linux boot
they are configured by editing file /boot/uEnv.txt with elevated permissions
```
sudo editor /boot/uEnv.txt
```
and defining which functionality to load, for example to enable CAN1 you need to define overlay for it
```
uboot_overlay_addr4=/lib/firmware/BB-CAN1-00A0.dtbo
```
This overlay BB-CAN1-00A0.dtbo will reconfigure all required pins for CAN1 and create CAN device in Linux.
Any change in overlays will require system reboot to be applied.
If you need to understand which pins are involved in some overlay - you can analyze source files in
this location: /opt/sources/bb.org-overlays/src/arm/
or search info in BeagleBone forums.


## Enabling hardware SPI
BeagleBone usually have multiple hardware SPI buses, for example BeagleBone Black can have 2 of them,
they can work up to 48Mhz, but usually they are limited to 16Mhz by Kernel Device-tree.
By default, in BeagleBone Black some of SPI1 pins are configured for HDMI-Audio output,
to fully enable 4-wire SPI1 you need to disable HDMI Audio and enable SPI1
To do that edit file /boot/uEnv.txt with elevated permissions
```
sudo editor /boot/uEnv.txt
```
uncomment variable
```
disable_uboot_overlay_audio=1
```

next uncomment variable and define it this way
```
uboot_overlay_addr4=/lib/firmware/BB-SPIDEV1-00A0.dtbo
```
Save changes in /boot/uEnv.txt and reboot the board.
Now you have SPI1 Enabled, to verify its presence execute command
```
ls /dev/spidev1.*
```
Take a note that BeagleBone usually is 3.3v based hardware and to use 5V SPI devices
you need to add Level-Shifting chip, for example SN74CBTD3861, SN74LVC1G34 or similar.
If you are using CRAMPS board - it already contains Level-Shifting chip and SPI1 pins
will become available on P503 port, and they can accept 5v hardware,
check CRAMPS board Schematics for pin references.

## Enabling hardware I2C
BeagleBone usually have multiple hardware I2C buses, for example BeagleBone Black can have 3 of them,
they support speed up-to 400Kbit Fast mode.
By default, in BeagleBone Black there are two of them (i2c-1 and i2c-2) usually both are already configured and
present on P9, third ic2-0 usually reserved for internal use.
If you are using CRAMPS board then i2c-2 is present on P303 port with 3.3v level,
If you want to obtain I2c-1 in CRAMPS board - you can get them on Extruder1.Step, Extruder1.Dir pins,
they also are 3.3v based, check CRAMPS board Schematics for pin references.
Related overlays, for [Hardware Pin designation](#hardware-pin-designation):
I2C1(100Kbit): BB-I2C1-00A0.dtbo
I2C1(400Kbit): BB-I2C1-FAST-00A0.dtbo
I2C2(100Kbit): BB-I2C2-00A0.dtbo
I2C2(400Kbit): BB-I2C2-FAST-00A0.dtbo

## Enabling hardware UART(Serial)/CAN
BeagleBone have up to 6 hardware UART(Serial) buses (up to 3Mbit)
and up to 2 hardware CAN(1Mbit) buses.
UART1(RX,TX) and CAN1(TX,RX) and I2C2(SDA,SCL) are using same pins - so you need to chose what to use
UART1(CTSN,RTSN) and CAN0(TX,RX) and I2C1(SDA,SCL) are using same pins - so you need to chose what to use
All UART/CAN related pins are 3.3v based, so you will need to use Transceiver chips/boards like SN74LVC2G241DCUR (for UART),
SN65HVD230 (for CAN), TTL-RS485 (for RS-485) or something similar which can convert 3.3v signals to appropriate levels.

Related overlays, for [Hardware Pin designation](#hardware-pin-designation)
CAN0: BB-CAN0-00A0.dtbo
CAN1: BB-CAN1-00A0.dtbo
UART0: - used for Console
UART1(RX,TX):  BB-UART1-00A0.dtbo
UART1(RTS,CTS): BB-UART1-RTSCTS-00A0.dtbo
UART2(RX,TX): BB-UART2-00A0.dtbo
UART3(RX,TX): BB-UART3-00A0.dtbo
UART4(RS-485): BB-UART4-RS485-00A0.dtbo
UART5(RX,TX): BB-UART5-00A0.dtbo
