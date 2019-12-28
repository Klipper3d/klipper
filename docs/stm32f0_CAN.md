This document describes how the STM32F0 port operates and how to work with
tiny CAN-enabled boards.

Required components
===================

#### MCP2515 module

![MCP2515](img/mcp2515.jpg)
or this
![CAN/RS485 hat](img/canhat.jpg)

#### St-link dongle

![st-link v2](img/stlinkv2-700x700.jpg)


Adding CAN bus to Raspberry Pi
==============================
(Based on Quick Guide https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=141052)

First of all, (only if you have small CAN module) it’s necessary to modify the CAN-module from ebay, because it has
only one VCC pin, but the MCP2515 needs to be powered from 3V3 and the TJA1050
CAN-transceiver needs to be powered from 5V. Powering both chips from 5V would
work, but then a level-shifter for the SPI would be needed. The Pi's GPIO pins
are NOT 5V tolerant. Cut a trace on the PCB and soldered a pin onto the trace
to deliver 5V only to the TJA1050. Be sure to cut the trace before the capacitor:

![VCC cut](img/mcp2515_vcc_cut.png)

Next connect the module:

| MCP2515 | Raspberry Pi |
| --- | --- |
| VCC | 1 (3V3) |
| TJA 1050 VCC | 2 (5V) |
| GND | 6 (GND) |
| CS | 24 (CE0) |
| MISO | 21 (MISO) |
| MOSI | 19 (MOSI) |
| SCK | 23 (SCK) |
| INT | 22 (GPIO25) |

Install can-utils:
> sudo apt-get install can-utils

To activate the driver for the MCP2515 you have to add a kernel overlay, to do
so edit the /boot/config.txt
> sudo nano /boot/config.txt

And add the following lines (set oscillator value according to crystal on your board):

```
dtparam=spi=on
dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25
dtoverlay=spi1-1cs
```

Now reboot, after the reboot try to setup the the can interface:
> sudo ip link set can0 up type can bitrate 500000

If no errors occurred, the can interface should be ready now.
To make the CAN-interface permanent, add the following lines to /etc/network/interfaces

```
auto can0
iface can0 can static
    bitrate 500000
```

Communicating over CAN
======================

Use "Serial over CAN" emulator software to establish connection:
https://github.com/Delsian/CanSerial
