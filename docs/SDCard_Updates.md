# SDCard updates

Many of today's popular controller boards ship with a bootloader capable of
updating firmware via SD Card.  While this is convenient in many
circumstances, these bootloaders typically provide no other way to update
firmware.  This can be a nuisance if your board is mounted in a location
that is difficult to access or if you need to update firmware often.
After Klipper has been initially flashed to a controller it is possible to
transfer new firmware to the SD Card and initiate the flashing procedure
via ssh.

## Typical Upgrade Procedure

The procedure for updating MCU firmware using the SD Card is similar to that
of other methods.  Instead of using `make flash` it is necessary to run a
helper script, `flash-sdcard.sh`.  Updating a BigTreeTech SKR 1.3 might look
like the following:
```
sudo service klipper stop
cd ~/klipper
git pull
make clean
make menuconfig
make
./scripts/flash-sdcard.sh /dev/ttyACM0 btt-skr-v1.3
sudo service klipper start
```

It is up to the user to determine the device location and board name.
If a user needs to flash multiple boards, `flash-sdcard.sh` (or
`make flash` if appropriate) should be run for each board prior to
restarting the Klipper service.

Supported boards can be listed with the following command:
```
./scripts/flash-sdcard.sh -l
```
If you do not see your board listed it may be necessary to add a new
board definition as [described below](#board-definitions).

## Advanced Usage

The above commands assume that your MCU connects at the default baud rate
of 250000 and the firmware is located at `~/klipper/out/klipper.bin`.  The
`flash-sdcard.sh` script provides options for changing these defaults.
All options can be viewed by the help screen:
```
./scripts/flash-sdcard.sh -h
SD Card upload utility for Klipper

usage: flash_sdcard.sh [-h] [-l] [-c] [-b <baud>] [-f <firmware>]
                       <device> <board>

positional arguments:
  <device>        device serial port
  <board>         board type

optional arguments:
  -h              show this message
  -l              list available boards
  -c              run flash check/verify only (skip upload)
  -b <baud>       serial baud rate (default is 250000)
  -f <firmware>   path to klipper.bin
```

If your board is flashed with firmware that connects at a custom baud
rate it is possible to upgrade by specifying the `-b` option:
```
./scripts/flash-sdcard.sh -b 115200 /dev/ttyAMA0 btt-skr-v1.3
```

If you wish to flash a build of Klipper located somewhere other than
the default location it can be done by specifying the `-f` option:
```
./scripts/flash-sdcard.sh -f ~/downloads/klipper.bin /dev/ttyAMA0 btt-skr-v1.3
```

Note that when upgrading a MKS Robin E3 it is not necessary to manually run
`update_mks_robin.py` and supply the resulting binary to `flash-sdcard.sh`.
This procedure is automated during the upload process.

The `-c` option is used to perform a check or verify-only operation
to test if the board is running the specified firmware correctly.  This
option is primarily intended for cases where a manual power-cycle is
necessary to complete the flashing procedure, such as with bootloaders that
use SDIO mode instead of SPI to access their SD Cards. (See Caveats below)
But, it can also be used anytime to verify if the code flashed into the board
matches the version in your build folder on any supported board.

## Caveats

- As mentioned in the introduction, this method only works for upgrading
  firmware.  The initial flashing procedure must be done manually per the
  instructions that apply to your controller board.
- While it is possible to flash a build that changes the Serial Baud or
  connection interface (ie: from USB to UART), verification will always
  fail as the script will be unable to reconnect to the MCU to verify
  the current version.
- Only boards that use SPI for SD Card communication are supported.
  Boards that use SDIO, such as the Flymaker Flyboard and MKS Robin Nano
  V1/V2, will not work in SDIO mode.  However, it's usually possible to
  flash such boards using Software SPI mode instead.  But if the board's
  bootloader only uses SDIO mode to access the SD Card, a power-cycle of
  the board and SD Card will be necessary so that the mode can switch from SPI
  back to SDIO to complete reflashing.  Such boards should be defined with
  `skip_verify` enabled to skip the verify step immediately after flashing.
  Then after the manual power-cycle, you can rerun the exact same
  `./scripts/flash-sdcard.sh` command, but add the `-c` option to complete
  the check/verify operation.  See [Flashing Boards that use SDIO](#flashing-boards-that-use-sdio)
  for examples.

## Board Definitions

Most common boards should be available, however it is possible to add a new
board definition if necessary.  Board definitions are located in
`~/klipper/scripts/spi_flash/board_defs.py`.  The definitions are stored
in dictionary, for example:
```python
BOARD_DEFS = {
    'generic-lpc1768': {
        'mcu': "lpc1768",
        'spi_bus': "ssp1",
        "cs_pin": "P0.6"
    },
    ...<further definitions>
}
```

The following fields may be specified:
- `mcu`: The mcu type.  This can be retrevied after configuring the build
  via `make menuconfig` by running `cat .config | grep CONFIG_MCU`.  This
  field is required.
- `spi_bus`:  The SPI bus connected to the SD Card.  This should be retreived
  from the board's schematic.  This field is required.
- `cs_pin`: The Chip Select Pin connected to the SD Card.  This should be
  retreived from the board schematic.  This field is required.
- `firmware_path`: The path on the SD Card where firmware should be
  transferred.  The default is `firmware.bin`.
- `current_firmware_path`: The path on the SD Card where the renamed firmware
  file is located after a successful flash.  The default is `firmware.cur`.
- `skip_verify`: This defines a boolean value which tells the scripts to skip
  the firmware verification step during the flashing process.  The default
  is `False`.  It can be set to `True` for boards that require a manual
  power-cycle to complete flashing.  To verify the firmware afterward, run
  the script again with the `-c` option to perform the verification step.
  [See caveats with SDIO cards](#caveats)

If software SPI is required, the `spi_bus` field should be set to `swspi`
and the following additional field should be specified:
- `spi_pins`:  This should be 3 comma separated pins that are connected to
  the SD Card in the format of `miso,mosi,sclk`.

It should be exceedingly rare that Software SPI is necessary, typically only
boards with design errors or boards that normally only support SDIO mode for
their SD Card will require it. The `btt-skr-pro` board definition provides an
example of the former, and the `btt-octopus-f446-v1` board definition
provides an example of the latter.

Prior to creating a new board definition one should check to see if an
existing board definition meets the criteria necessary for the new board.
If this is the case, a `BOARD_ALIAS` may be specified.  For example, the
following alias may be added to specify `my-new-board` as an alias for
`generic-lpc1768`:
```python
BOARD_ALIASES = {
    ...<previous aliases>,
    'my-new-board': BOARD_DEFS['generic-lpc1768'],
}
```

If you need a new board definition and you are uncomfortable with the
procedure outlined above it is recommended that you request one in
the [Klipper Community Discord](Contact.md#discord).

## Flashing Boards that use SDIO

[As mentioned in the Caveats](#caveats), boards whose bootloader uses
SDIO mode to access their SD Card require a power-cycle of the board,
and specifically the SD Card itself, in order to switch from the SPI Mode
used while writing the file to the SD Card back to SDIO mode for the
bootloader to flash it into the board.  These board definitions will
use the `skip_verify` flag, which tells the flashing tool to stop after
writing the firmware to the SD Card so that the board can be manually
power-cycled and the verification step deferred until that's complete.

There are two scenarios -- one with the RPi Host running on a separate
power supply and the other when the RPi Host is running on the same
power supply as the main board being flashed.  The difference is whether
or not it's necessary to also shutdown the RPi and then `ssh` again after
the flashing is complete in order to do the verification step, or if the
verification can be done immediately.  Here's examples of the two scenarios:

### SDIO Programming with RPi on Separate Power Supply

A typical session with the RPi on a Separate Power Supply looks like the
following.  You will, of course, need to use your proper device path and
board name:
```
sudo service klipper stop
cd ~/klipper
git pull
make clean
make menuconfig
make
./scripts/flash-sdcard.sh /dev/ttyACM0 btt-octopus-f446-v1
[[[manually power-cycle the printer board here when instructed]]]
./scripts/flash-sdcard.sh -c /dev/ttyACM0 btt-octopus-f446-v1
sudo service klipper start
```

### SDIO Programming with RPi on the Same Power Supply

A typical session with the RPi on the Same Power Supply looks like the
following.  You will, of course, need to use your proper device path and
board name:
```
sudo service klipper stop
cd ~/klipper
git pull
make clean
make menuconfig
make
./scripts/flash-sdcard.sh /dev/ttyACM0 btt-octopus-f446-v1
sudo shutdown -h now
[[[wait for the RPi to shutdown, then power-cycle and ssh again to the RPi when it restarts]]]
sudo service klipper stop
cd ~/klipper
./scripts/flash-sdcard.sh -c /dev/ttyACM0 btt-octopus-f446-v1
sudo service klipper start
```

In this case, since the RPi Host is being restarted, which will restart
the `klipper` service, it's necessary to stop `klipper` again before doing
the verification step and restart it after verification is complete.

### SDIO to SPI Pin Mapping

If your board's schematic uses SDIO for its SD Card, you can map the pins
as described in the chart below to determine the compatible Software SPI
pins to assign in the `board_defs.py` file:

| SD Card Pin | Micro SD Card Pin  |  SDIO Pin Name   |   SPI Pin Name   |
| :---------: | :----------------: | :--------------: | :--------------: |
|      9      |         1          |       DATA2      |     None (PU)*   |
|      1      |         2          |      CD/DATA3    |        CS        |
|      2      |         3          |        CMD       |       MOSI       |
|      4      |         4          |    +3.3V (VDD)   |    +3.3V (VDD)   |
|      5      |         5          |        CLK       |       SCLK       |
|      3      |         6          |     GND (VSS)    |     GND (VSS)    |
|      7      |         7          |       DATA0      |       MISO       |
|      8      |         8          |       DATA1      |     None (PU)*   |
|     N/A     |         9          | Card Detect (CD) | Card Detect (CD) |
|      6      |        10          |       GND        |        GND       |

\* None (PU) indicates an unused pin with a pull-up resistor
