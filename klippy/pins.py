# Pin name to pin number definitions
#
# Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import re

def avr_pins(port_count):
    pins = {}
    for port in range(port_count):
        portchr = chr(65 + port)
        if portchr == 'I':
            continue
        for portbit in range(8):
            pins['P%c%d' % (portchr, portbit)] = port * 8 + portbit
    return pins

PINS_atmega164 = avr_pins(4)
PINS_atmega1280 = avr_pins(12)

MCU_PINS = {
    "atmega168": PINS_atmega164, "atmega644p": PINS_atmega164,
    "at90usb1286": avr_pins(5),
    "atmega1280": PINS_atmega1280, "atmega2560": PINS_atmega1280,
}

def mcu_to_pins(mcu):
    return MCU_PINS.get(mcu, {})

re_pin = re.compile(r'(?P<prefix>[ _]pin=)(?P<name>[^ ]*)')
def update_command(cmd, pmap):
    def fixup(m):
        return m.group('prefix') + str(pmap[m.group('name')])
    return re_pin.sub(fixup, cmd)


######################################################################
# Arduino mappings
######################################################################

Arduino_standard = [
    "PD0", "PD1", "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PB0", "PB1",
    "PB2", "PB3", "PB4", "PB5", "PC0", "PC1", "PC2", "PC3", "PC4", "PC5",
]
Arduino_analog_standard = [
    "PC0", "PC1", "PC2", "PC3", "PC4", "PC5", "PE0", "PE1",
]

Arduino_mega = [
    "PE0", "PE1", "PE4", "PE5", "PG5", "PE3", "PH3", "PH4", "PH5", "PH6",
    "PB4", "PB5", "PB6", "PB7", "PJ1", "PJ0", "PH1", "PH0", "PD3", "PD2",
    "PD1", "PD0", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7",
    "PC7", "PC6", "PC5", "PC4", "PC3", "PC2", "PC1", "PC0", "PD7", "PG2",
    "PG1", "PG0", "PL7", "PL6", "PL5", "PL4", "PL3", "PL2", "PL1", "PL0",
    "PB3", "PB2", "PB1", "PB0", "PF0", "PF1", "PF2", "PF3", "PF4", "PF5",
    "PF6", "PF7", "PK0", "PK1", "PK2", "PK3", "PK4", "PK5", "PK6", "PK7",
]
Arduino_analog_mega = [
    "PF0", "PF1", "PF2", "PF3", "PF4", "PF5",
    "PF6", "PF7", "PK0", "PK1", "PK2", "PK3", "PK4", "PK5", "PK6", "PK7",
]

Sanguino = [
    "PB0", "PB1", "PB2", "PB3", "PB4", "PB5", "PB6", "PB7", "PD0", "PD1",
    "PD2", "PD3", "PD4", "PD5", "PD6", "PD7", "PC0", "PC1", "PC2", "PC3",
    "PC4", "PC5", "PC6", "PC7", "PA0", "PA1", "PA2", "PA3", "PA4", "PA5",
    "PA6", "PA7"
]
Sanguino_analog = [
    "PA0", "PA1", "PA2", "PA3", "PA4", "PA5", "PA6", "PA7"
]

Arduino_from_mcu = {
    "atmega168": (Arduino_standard, Arduino_analog_standard),
    "atmega644p": (Sanguino, Sanguino_analog),
    "atmega1280": (Arduino_mega, Arduino_analog_mega),
    "atmega2560": (Arduino_mega, Arduino_analog_mega),
}

def map_pins(name, mcu):
    pins = MCU_PINS.get(mcu, {})
    if name == 'arduino':
        dpins, apins = Arduino_from_mcu.get(mcu, [])
        for i in range(len(dpins)):
            pins['ar' + str(i)] = pins[dpins[i]]
        for i in range(len(apins)):
            pins['analog%d' % (i,)] = pins[apins[i]]
    return pins
