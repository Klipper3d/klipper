# CANBUS

This document describes Klipper's CAN bus support.

## Device Hardware

Klipper currently supports CAN on stm32, SAME5x, and rp2040 chips. In
addition, the micro-controller chip must be on a board that has a CAN
transceiver.

To compile for CAN, run `make menuconfig` and select "CAN bus" as the
communication interface. Finally, compile the micro-controller code
and flash it to the target board.

## Host Hardware

In order to use a CAN bus, it is necessary to have a host adapter. It
is recommended to use a "USB to CAN adapter". There are many different
USB to CAN adapters available from different manufacturers. When
choosing one, we recommend verifying that the firmware can be updated
on it. (Unfortunately, we've found some USB adapters run defective
firmware and are locked down, so verify before purchasing.) Look for
adapters that can run Klipper directly (in its "USB to CAN bridge
mode") or that run the
[candlelight firmware](https://github.com/candle-usb/candleLight_fw).

It is also necessary to configure the host operating system to use the
adapter. This is typically done by creating a new file named
`/etc/network/interfaces.d/can0` with the following contents:
```
allow-hotplug can0
iface can0 can static
    bitrate 1000000
    up ip link set $IFACE txqueuelen 128
```

## Terminating Resistors

A CAN bus should have two 120 ohm resistors between the CANH and CANL
wires. Ideally, one resistor located at each the end of the bus.

Note that some devices have a builtin 120 ohm resistor that can not be
easily removed. Some devices do not include a resistor at all. Other
devices have a mechanism to select the resistor (typically by
connecting a "pin jumper"). Be sure to check the schematics of all
devices on the CAN bus to verify that there are two and only two 120
Ohm resistors on the bus.

To test that the resistors are correct, one can remove power to the
printer and use a multi-meter to check the resistance between the CANH
and CANL wires - it should report ~60 ohms on a correctly wired CAN
bus.

## Finding the canbus_uuid for new micro-controllers

Each micro-controller on the CAN bus is assigned a unique id based on
the factory chip identifier encoded into each micro-controller. To
find each micro-controller device id, make sure the hardware is
powered and wired correctly, and then run:
```
~/klippy-env/bin/python ~/klipper/scripts/canbus_query.py can0
```

If uninitialized CAN devices are detected the above command will
report lines like the following:
```
Found canbus_uuid=11aa22bb33cc, Application: Klipper
```

Each device will have a unique identifier. In the above example,
`11aa22bb33cc` is the micro-controller's "canbus_uuid".

Note that the `canbus_query.py` tool will only report uninitialized
devices - if Klipper (or a similar tool) configures the device then it
will no longer appear in the list.

## Configuring Klipper

Update the Klipper [mcu configuration](Config_Reference.md#mcu) to use
the CAN bus to communicate with the device - for example:
```
[mcu my_can_mcu]
canbus_uuid: 11aa22bb33cc
```

## USB to CAN bus bridge mode

Some micro-controllers support selecting "USB to CAN bus bridge" mode
during Klipper's "make menuconfig". This mode may allow one to use a
micro-controller as both a "USB to CAN bus adapter" and as a Klipper
node.

When Klipper uses this mode the micro-controller appears as a "USB CAN
bus adapter" under Linux. The "Klipper bridge mcu" itself will appear
as if it was on this CAN bus - it can be identified via
`canbus_query.py` and it must be configured like other CAN bus Klipper
nodes.

Some important notes when using this mode:

* It is necessary to configure the `can0` (or similar) interface in
  Linux in order to communicate with the bus. However, Linux CAN bus
  speed and CAN bus bit-timing options are ignored by Klipper.
  Currently, the CAN bus frequency is specified during "make
  menuconfig" and the bus speed specified in Linux is ignored.

* Whenever the "bridge mcu" is reset, Linux will disable the
  corresponding `can0` interface. To ensure proper handling of
  FIRMWARE_RESTART and RESTART commands, it is recommended to use
  `allow-hotplug` in the `/etc/network/interfaces.d/can0` file. For
  example:
```
allow-hotplug can0
iface can0 can static
    bitrate 1000000
    up ip link set $IFACE txqueuelen 128
```

* The "bridge mcu" is not actually on the CAN bus. Messages to and
  from the bridge mcu will not be seen by other adapters that may be
  on the CAN bus.

* The available bandwidth to both the "bridge mcu" itself and all
  devices on the CAN bus is effectively limited by the CAN bus
  frequency. As a result, it is recommended to use a CAN bus frequency
  of 1000000 when using "USB to CAN bus bridge mode".

  Even at a CAN bus frequency of 1000000, there may not be sufficient
  bandwidth to run a `SHAPER_CALIBRATE` test if both the XY steppers
  and the accelerometer all communicate via a single "USB to CAN bus"
  interface.

* A USB to CAN bridge board will not appear as a USB serial device, it
  will not show up when running `ls /dev/serial/by-id`, and it can not
  be configured in Klipper's printer.cfg file with a `serial:`
  parameter. The bridge board appears as a "USB CAN adapter" and it is
  configured in the printer.cfg as a [CAN node](#configuring-klipper).

## Tips for troubleshooting

See the [CAN bus troubleshooting](CANBUS_Troubleshooting.md) document.
