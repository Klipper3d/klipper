### Lightweight USB Device Stack ###

+ Lightweight and fast
+ Event-driven process workflow
+ Completely separated USB hardware driver and usb core
+ Easy to use.

### Requirements ###

+ [CMSIS V4](https://github.com/ARM-software/CMSIS) or [CMSIS V5](https://github.com/ARM-software/CMSIS_5).
+ Device peripheral access layer header files for STM32. See [Vendor Template](https://github.com/ARM-software/CMSIS/tree/master/Device/_Template_Vendor) for details.
+ [stm32.h](https://github.com/dmitrystu/stm32h) STM32 universal header

### Supported hardware ###

<table>
    <tr><th>MCU Series</th><th>Features</th><th>Driver</th><th>File</th></tr>
    <tr>
        <td rowspan="2">STM32L0x2 STM32L0x3 STM32F0x2 STM32F0x8</td>
        <td nowrap rowspan="2">Doublebuffered<sup>[2]</sup><br />8<sup>[1]</sup> endpoints<br /> BC1.2</td>
        <td>usbd_devfs</td>
        <td>usbd_stm32l052_devfs.c</td>
    </tr>
    <tr>
        <td>usbd_devfs_asm</td>
        <td>usbd_stm32l052_devfs_asm.S</td>
    </tr>
    <tr>
        <td rowspan="2">STM32L4x2 STM32L4x3</td>
        <td nowrap rowspan="2">Doublebuffered<sup>[2]</sup><br />8<sup>[1]</sup> endpoints<br /> BC1.2</td>
        <td>usbd_devfs</td>
        <td>usbd_stm32l433_devfs.c</td>
    </tr>
    <tr>
        <td>usbd_devfs_asm</td>
        <td>usbd_stm32l052_devfs_asm.S</td>
    </tr>
    <tr>
        <td rowspan="2">STM32L1xx</td>
        <td nowrap rowspan="2">Doublebuffered<sup>[2]</sup><br />8<sup>[1]</sup> endpoints</td>
        <td>usbd_devfs</td>
        <td>usbd_stm32l100_devfs.c</td>
    </tr>
    <tr>
        <td>usbd_devfs_asm</td>
        <td>usbd_stm32l100_devfs_asm.S</td>
    </tr>
    <tr>
        <td rowspan="2">STM32F102 STM32F103 STM32F302 STM32F303 STM32F373</td>
        <td nowrap rowspan="2">Doublebuffered<sup>[2]</sup><br />External DP pullup<br />8<sup>[1]</sup> endpoints</td>
        <td>usbd_devfs</td>
        <td>usbd_stm32f103_devfs.c</td>
    </tr>
    <tr>
        <td>usbd_devfs_asm</td>
        <td>usbd_stm32f103_devfs_asm.S</td>
    </tr>
    <tr>
        <td>STM32L4x5 STM32L4x6</td>
        <td nowrap>Doublebuffered<br />6 endpoints<br /> BC1.2<br />VBUS detection</td>
        <td>usbd_otgfs</td>
        <td>usbd_stm32l476_otgfs.c</td>
    </tr>
    <tr>
        <td rowspan="2">STM32F4x5 STM32F4x7 STM32F4x9</td>
        <td nowrap>Doublebuffered<br/>4 endpoints<br/>VBUS detection<br/>SOF output</td>
        <td>usbd_otgfs</td>
        <td>usbd_stm32f429_otgfs.c</td>
    </tr>
    <tr>
        <td nowrap>Doublebuffered<br/>6 endpoints<br/>VBUS detection<br/>SOF output</td>
        <td>usbd_otghs</td>
        <td>usbd_stm32f429_otghs.c</td>
    </tr>
    <tr>
        <td>STM32F105 STM32F107</td>
        <td nowrap>Doublebuffered<br/>4 endpoints<br/>VBUS detection<br/>SOF output</td>
        <td>usbd_otgfs</td>
        <td>usbd_stm32f105_otgfs.c</td>
    </tr>
</table>

1. Single physical endpoint can be used to implement
  + one bi-directional/single-buffer logical endpoint (CONTROL)
  + one uni-directional/double-buffer logical endpoint (BULK OR ISOCHRONOUS)
  + two uni-directional/single-buffer logical endpoints (BULK OR INTERRUPT)

2. At this moment BULK IN endpoint can use both buffers, but it is not **real** doublebuffered.

3. Tested with STM32L052K8, STM32L100RC, STM32L476RG, STM32F072C8, STM32F103C8, STM32F103CB, STM32F303CC, STM32F303RE, STM32F429ZI, STM32F105RBT6, STM32F107VCT6, STM32L433CCT6

### Implemented definitions for classes ###
1. USB HID based on [Device Class Definition for Human Interface Devices (HID) Version 1.11](https://www.usb.org/sites/default/files/documents/hid1_11.pdf)
2. USB DFU based on [USB Device Firmware Upgrade Specification, Revision 1.1](https://www.usb.org/sites/default/files/DFU_1.1.pdf)
3. USB CDC based on [Class definitions for Communication Devices 1.2](https://www.usb.org/sites/default/files/CDC1.2_WMC1.1_012011.zip)

### Using makefile ###
+ to build library module
```
make module MODULE=path/module.a DEFINES="mcu spcified defines" CFLAGS="cpu cpecified compiler flags"
```
+ to build demo
```
make bluepill program
make stm32l052x8
```
+ to get a help
```
make help
```

### Default values: ###
| Variable | Default Value                       | Means                         |
|----------|-------------------------------------|-------------------------------|
| CMSIS    | ../../CMSIS                         | path to CMSIS root folder     |
| CMSISDEV | $(CMSIS)/Device                     | path to CMSIS device folder   |
| CMSISCORE| $(CMSIS)/CMSIS/Include $(CMSIS)/CMSIS/Core/Include | path to CMSIS core headers |
| MCU      | stm32l100xc                         | MCU selection for demo project|
| CFLAGS   | -mcpu=cortex-m3 -mfloat-abi=soft    | MCU specified compiler flags  |
| DEFINES  | STM32L1 STM32L100xC                 | MCU specified defines         |
| STPROG_CLI | ~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI | Path to the ST Cube Programmer CLI |
| OPTFLAFS | -Os | Code optimization flags |

### Useful Resources ###
1. [USB Implementers Forum official site](https://www.usb.org/)
2. [USB Made Simple](http://www.usbmadesimple.co.uk/)
3. [LUFA - the Lightweight USB Framework for AVRs.](https://github.com/abcminiuser/lufa)
4. [Open Source ARM cortex m microcontroller library](https://github.com/libopencm3/libopencm3)
