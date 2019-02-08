This document provides information on common bootloaders found on
micro-controllers that Klipper supports.

The bootloader is 3rd-party software that runs on the micro-controller
when it is first powered on. It is typically used to flash a new
application (eg, Klipper) to the micro-controller without requiring
specialized hardware. Unfortunately, there is no industry wide
standard for flashing a micro-controller, nor is there a standard
bootloader that works across all micro-controllers. Worse, it is
common for each bootloader to require a different set of steps to
flash an application.

If one can flash a bootloader to a micro-controller then one can
generally also use that mechanism to flash an application, but care
should be taken when doing this as one may inadvertently remove the
bootloader. In contrast, a bootloader will generally only permit a
user to flash an application. It is therefore recommended to use a
bootloader to flash an application where possible.

This document attempts to describe common bootloaders, the steps
needed to flash a bootloader, and the steps needed to flash an
application.  This document is not an authoritative reference; it is
intended as a collection of useful information that the Klipper
developers have accumulated.

AVR micro-controllers
=====================

In general, the Arduino project is a good reference for bootloaders
and flashing procedures on the 8-bit Atmel Atmega micro-controllers.
In particular, the "boards.txt" file:
https://github.com/arduino/Arduino/blob/1.8.5/hardware/arduino/avr/boards.txt
is a useful reference.

To flash a bootloader itself, the AVR chips require an external
hardware flashing tool (which communicates with the chip using
SPI). This tool can be purchased (for example, do a web search for
"avr isp", "arduino isp", or "usb tiny isp"). It is also possible to
use another Arduino or Raspberry Pi to flash an AVR bootloader (for
example, do a web search for "program an avr using raspberry pi"). The
examples below are written assuming an "AVR ISP Mk2" type device is in
use.

The "avrdude" program is the most common tool used to flash atmega
chips (both bootloader flashing and application flashing).

## Atmega2560 ##

This chip is typically found in the "Arduino Mega" and is very common
in 3d printer boards.

To flash the bootloader itself use something like:
```
wget 'https://github.com/arduino/Arduino/raw/1.8.5/hardware/arduino/avr/bootloaders/stk500v2/stk500boot_v2_mega2560.hex'

avrdude -cavrispv2 -patmega2560 -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xD8:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega2560 -P/dev/ttyACM0 -b115200 -U flash:w:stk500boot_v2_mega2560.hex
avrdude -cavrispv2 -patmega2560 -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

To flash an application use something like:
```
avrdude -cwiring -patmega2560 -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
```

## Atmega1280 ##

This chip is typically found in earlier versions of the "Arduino
Mega".

To flash the bootloader itself use something like:
```
wget 'https://github.com/arduino/Arduino/raw/1.8.5/hardware/arduino/avr/bootloaders/atmega/ATmegaBOOT_168_atmega1280.hex'

avrdude -cavrispv2 -patmega1280 -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0xF5:m -U hfuse:w:0xDA:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega1280 -P/dev/ttyACM0 -b115200 -U flash:w:ATmegaBOOT_168_atmega1280.hex
avrdude -cavrispv2 -patmega1280 -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

To flash an application use something like:
```
avrdude -carduino -patmega1280 -P/dev/ttyACM0 -b57600 -D -Uflash:w:out/klipper.elf.hex:i
```

## Atmega1284p ##

This chip is commonly found in "Melzi" style 3d printer boards.

To flash the bootloader itself use something like:
```
wget 'https://github.com/Lauszus/Sanguino/raw/1.0.2/bootloaders/optiboot/optiboot_atmega1284p.hex'

avrdude -cavrispv2 -patmega1284p -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0xFD:m -U hfuse:w:0xDE:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega1284p -P/dev/ttyACM0 -b115200 -U flash:w:optiboot_atmega1284p.hex
avrdude -cavrispv2 -patmega1284p -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

To flash an application use something like:
```
avrdude -carduino -patmega1284p -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
```

Note that a number of "Melzi" style boards come preloaded with a
bootloader that uses a baud rate of 57600. In this case, to flash an
application use something like this instead:
```
avrdude -carduino -patmega1284p -P/dev/ttyACM0 -b57600 -D -Uflash:w:out/klipper.elf.hex:i
```

## At90usb1286 ##

This document does not cover the method to flash a bootloader to the
At90usb1286 nor does it cover general application flashing to this
device.

The Teensy++ device from pjrc.com comes with a proprietary bootloader.
It requires a custom flashing tool from
https://github.com/PaulStoffregen/teensy_loader_cli . One can flash an
application with it using something like:
```
teensy_loader_cli --mcu=at90usb1286 out/klipper.elf.hex -v
```

## Atmega168 ##

The atmega168 has limited flash space. If using a bootloader, it is
recommended to use the Optiboot bootloader. To flash that bootloader
use something like:
```
wget 'https://github.com/arduino/Arduino/raw/1.8.5/hardware/arduino/avr/bootloaders/optiboot/optiboot_atmega168.hex'

avrdude -cavrispv2 -patmega168 -P/dev/ttyACM0 -b115200 -e -u -U lock:w:0x3F:m -U efuse:w:0x04:m -U hfuse:w:0xDD:m -U lfuse:w:0xFF:m
avrdude -cavrispv2 -patmega168 -P/dev/ttyACM0 -b115200 -U flash:w:optiboot_atmega168.hex
avrdude -cavrispv2 -patmega168 -P/dev/ttyACM0 -b115200 -U lock:w:0x0F:m
```

To flash an application via the Optiboot bootloader use something
like:
```
avrdude -carduino -patmega168 -P/dev/ttyACM0 -b115200 -D -Uflash:w:out/klipper.elf.hex:i
```

SAM3 micro-controllers (Arduino Due)
====================================

It is not common to use a bootloader with the SAM3 mcu. The chip
itself has a ROM that allows the flash to be programmed from 3.3V
serial port or from USB.

To enable the ROM, the "erase" pin is held high during a reset, which
erases the flash contents, and causes the ROM to run. On an Arduino
Due, this sequence can be accomplished by setting a baud rate of 1200
on the "programming usb port" (the USB port closest to the power
supply).

The code at https://github.com/shumatech/BOSSA can be used to program
the SAM3. It is recommended to use version 1.9 or later.

To flash an application use something like:
```
bossac -U -p /dev/ttyACM0 -a -e -w out/klipper.bin -v -b
bossac -U -p /dev/ttyACM0 -R
```

SAM4 micro-controllers (Duet Wifi)
====================================

It is not common to use a bootloader with the SAM4 mcu. The chip
itself has a ROM that allows the flash to be programmed from 3.3V
serial port or from USB.

To enable the ROM, the "erase" pin is held high during a reset, which
erases the flash contents, and causes the ROM to run.

The code at https://github.com/shumatech/BOSSA can be used to program
the SAM4. It is necessary to use version `1.8.0` or higher.

To flash an application use something like:
```
bossac --port=/dev/ttyACM0 -b -U -e -w -v -R out/klipper.bin
```

SAMD21 micro-controllers (Arduino Zero)
=======================================

The SAMD21 bootloader is flashed via the ARM Serial Wire Debug (SWD)
interface. This is commonly done with a dedicated SWD hardware dongle.
Alternatively, it appears one can use a Raspberry Pi with OpenOCD as a
programmer (see:
https://learn.adafruit.com/programming-microcontrollers-using-openocd-on-raspberry-pi
).

Unfortunately, there are two common bootloaders deployed on the
SAMD21. One comes standard with the "Arduino Zero" and the other comes
standard with the "Arduino M0".

The Arduino Zero uses an 8KiB bootloader (the application must be
compiled with a start address of 8KiB). One can enter the bootloader
by double clicking the reset button. To flash an application use
something like:
```
bossac -U -p "$(FLASH_DEVICE)" --offset=0x2000 -w out/klipper.bin -v -b -R
```

The Arduino M0 uses a 16KiB bootloader (the application must be
compiled with a start address of 16KiB). To flash an application,
reset the micro-controller and run the flash command within the first
few seconds of boot - something like:
```
avrdude -c stk500v2 -p atmega2560 -P /dev/ttyACM0 -u -Uflash:w:out/klipper.elf.hex:i
```

STM32F103 micro-controllers (Blue Pill devices)
===============================================

The STM32F103 devices have a ROM that can flash a bootloader or
application via 3.3V serial. To access this ROM, one should connect
the "boot 0" pin to high and "boot 1" pin to low, and then reset the
device. The "stm32flash" package can then be used to flash the device
using something like:
```
stm32flash -w out/klipper.bin -v -g 0 /dev/ttyAMA0
```

Note that if one is using a Raspberry Pi for the 3.3V serial, the
stm32flash protocol uses a serial parity mode which the Raspberry Pi's
"miniuart" does not support. See
https://www.raspberrypi.org/documentation/configuration/uart.md for
details on enabling the full uart on the Raspberry Pi GPIO pins.

After flashing, set both "boot 0" and "boot 1" back to low so that
future resets boot from flash.

## STM32F103 with stm32duino bootloader ##

The "stm32duino" project has a USB capable bootloader - see:
https://github.com/rogerclarkmelbourne/STM32duino-bootloader

This bootloader can be flashed via 3.3V serial with something like:
```
wget 'https://github.com/rogerclarkmelbourne/STM32duino-bootloader/raw/master/binaries/generic_boot20_pc13.bin'

stm32flash -w generic_boot20_pc13.bin -v -g 0 /dev/ttyAMA0
```

This bootloader uses 8KiB of flash space (the application must be
compiled with a start address of 8KiB). Flash an application with
something like:
```
dfu-util -d 1eaf:0003 -a 2 -R -D out/klipper.bin
```

The bootloader typically runs for only a short period after boot. It
may be necessary to time the above command so that it runs while the
bootloader is still active (the bootloader will flash a board led
while it is running). Alternatively, set the "boot 0" pin to low and
"boot 1" pin to high to stay in the bootloader after a reset.

LPC176x micro-controllers (Smoothieboards)
==========================================

This document does not describe the method to flash a bootloader
itself - see: http://smoothieware.org/flashing-the-bootloader for
further information on that topic.

It is common for Smoothieboards to come with a bootloader from:
https://github.com/triffid/LPC17xx-DFU-Bootloader . When using this
bootloader the application must be compiled with a start address of
16KiB. The easiest way to flash an application with this bootloader is
to copy the application file (eg, `out/klipper.bin`) to a file named
`firmware.bin` on an SD card, and then to reboot the micro-controller
with that SD card.
