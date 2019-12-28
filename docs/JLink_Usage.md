Install J-Link on Raspberry Pi
===============================

Some tiny chips has no bootloader capability. You should connect J-Link adapter for chip flashing.

## Installing the J-Link utilities

Connect your J-Link via USB to the Pi and to your target board via the appropriate header.

In the pi user home directory, download and un-tar the Segger utilities for the Pi.
Then configure the udev rules as per README.txt file in the JLink_Linux_arm directory.

```
$ wget --post-data 'accept_license_agreement=accepted&non_emb_ctr=confirmed&submit=Download+software' https://www.segger.com/downloads/jlink/JLink_Linux_arm.tgz
$ tar xvf JLink_Linux_arm.tgz
$ cd JLink_Linux_arm/
$ more README.txt
$ sudo cp 99-jlink.rules /etc/udev/rules.d/
$ sudo reboot
```

Re-login to the RPi.

Under the directory JLink_Linux_arm there are a number of utilities, better to include J-Link folder to path:

    nano .bashrc

Add last line:

     export PATH="/home/pi/JLink_Linux_arm:$PATH"

Check if J-Link connected to target:

     $ JLinkExe

```
SEGGER J-Link Commander V6.56a (Compiled Nov 25 2019 17:01:23)
DLL version V6.56a, compiled Nov 25 2019 17:01:03

Connecting to J-Link via USB...O.K.
Firmware: J-Link V9 compiled May 17 2019 09:50:41
Hardware version: V9.30
S/N: 59302778
License(s): GDB
VTref=3.275V


Type "connect" to establish a target connection, '?' for help
J-Link>connect
Please specify device / core. <Default>: STM32F042C6
Type '?' for selection dialog
Device>
Please specify target interface:
  J) JTAG (Default)
  S) SWD
  T) cJTAG
TIF>s
Specify target interface speed [kHz]. <Default>: 4000 kHz
Speed>
Device "STM32F042C6" selected.


Connecting to target via SWD
Found SW-DP with ID 0x0BB11477
Found SW-DP with ID 0x0BB11477
Scanning AP map to find all available APs
AP[1]: Stopped AP scan as end of AP map has been reached
AP[0]: AHB-AP (IDR: 0x04770021)
Iterating through AP map to find AHB-AP to use
AP[0]: Core found
AP[0]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410CC200. Implementer code: 0x41 (ARM)
Found Cortex-M0 r0p0, Little endian.
FPUnit: 4 code (BP) slots and 0 literal slots
CoreSight components:
ROMTbl[0] @ E00FF000
ROMTbl[0][0]: E000E000, CID: B105E00D, PID: 000BB008 SCS
ROMTbl[0][1]: E0001000, CID: B105E00D, PID: 000BB00A DWT
ROMTbl[0][2]: E0002000, CID: B105E00D, PID: 000BB00B FPB
Cortex-M0 identified.
J-Link>
```

Now "make jflash" command can upload HEX into connected board

## Remote debugging

Command "make gdb" runs remote GDB server, you can connect from debugger using  RPi's IP-address and port 2331
