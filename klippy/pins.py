# Pin name to pin number definitions
#
# Copyright (C) 2016-2018  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import re


######################################################################
# Hardware pin names
######################################################################

def port_pins(port_count, bit_count=8):
    pins = {}
    for port in range(port_count):
        portchr = chr(65 + port)
        if portchr == 'I':
            continue
        for portbit in range(bit_count):
            pins['P%c%d' % (portchr, portbit)] = port * bit_count + portbit
    return pins

def named_pins(fmt, port_count, bit_count=32):
    return { fmt % (port, portbit) : port * bit_count + portbit
             for port in range(port_count)
             for portbit in range(bit_count) }

def beaglebone_pins():
    gpios = named_pins("gpio%d_%d", 4)
    gpios.update({"AIN%d" % i: i+4*32 for i in range(8)})
    return gpios

MCU_PINS = {
    "atmega168": port_pins(5), "atmega328": port_pins(5),
    "atmega644p": port_pins(4), "atmega1284p": port_pins(4),
    "at90usb1286": port_pins(6),
    "atmega1280": port_pins(12), "atmega2560": port_pins(12),
    "sam3x8e": port_pins(4, 32),
    "pru": beaglebone_pins(),
    "linux": {"analog%d" % i: i for i in range(8)}, # XXX
}


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

Arduino_Due = [
    "PA8", "PA9", "PB25", "PC28", "PA29", "PC25", "PC24", "PC23", "PC22", "PC21",
    "PA28", "PD7", "PD8", "PB27", "PD4", "PD5", "PA13", "PA12", "PA11", "PA10",
    "PB12", "PB13", "PB26", "PA14", "PA15", "PD0", "PD1", "PD2", "PD3", "PD6",
    "PD9", "PA7", "PD10", "PC1", "PC2", "PC3", "PC4", "PC5", "PC6", "PC7",
    "PC8", "PC9", "PA19", "PA20", "PC19", "PC18", "PC17", "PC16", "PC15", "PC14",
    "PC13", "PC12", "PB21", "PB14", "PA16", "PA24", "PA23", "PA22", "PA6", "PA4",
    "PA3", "PA2", "PB17", "PB18", "PB19", "PB20", "PB15", "PB16", "PA1", "PA0",
    "PA17", "PA18", "PC30", "PA21", "PA25", "PA26", "PA27", "PA28", "PB23"
]
Arduino_Due_analog = [
    "PA16", "PA24", "PA23", "PA22", "PA6", "PA4", "PA3", "PA2", "PB17", "PB18",
    "PB19", "PB20"
]


Arduino_from_mcu = {
    "atmega168": (Arduino_standard, Arduino_analog_standard),
    "atmega328": (Arduino_standard, Arduino_analog_standard),
    "atmega644p": (Sanguino, Sanguino_analog),
    "atmega1280": (Arduino_mega, Arduino_analog_mega),
    "atmega2560": (Arduino_mega, Arduino_analog_mega),
    "sam3x8e": (Arduino_Due, Arduino_Due_analog),
}

def update_map_arduino(pins, mcu):
    dpins, apins = Arduino_from_mcu.get(mcu, ([], []))
    for i in range(len(dpins)):
        pins['ar' + str(i)] = pins[dpins[i]]
    for i in range(len(apins)):
        pins['analog%d' % (i,)] = pins[apins[i]]


######################################################################
# Beaglebone mappings
######################################################################

beagleboneblack_mappings = {
    'P8_3': 'gpio1_6', 'P8_4': 'gpio1_7', 'P8_5': 'gpio1_2',
    'P8_6': 'gpio1_3', 'P8_7': 'gpio2_2', 'P8_8': 'gpio2_3',
    'P8_9': 'gpio2_5', 'P8_10': 'gpio2_4', 'P8_11': 'gpio1_13',
    'P8_12': 'gpio1_12', 'P8_13': 'gpio0_23', 'P8_14': 'gpio0_26',
    'P8_15': 'gpio1_15', 'P8_16': 'gpio1_14', 'P8_17': 'gpio0_27',
    'P8_18': 'gpio2_1', 'P8_19': 'gpio0_22', 'P8_20': 'gpio1_31',
    'P8_21': 'gpio1_30', 'P8_22': 'gpio1_5', 'P8_23': 'gpio1_4',
    'P8_24': 'gpio1_1', 'P8_25': 'gpio1_0', 'P8_26': 'gpio1_29',
    'P8_27': 'gpio2_22', 'P8_28': 'gpio2_24', 'P8_29': 'gpio2_23',
    'P8_30': 'gpio2_25', 'P8_31': 'gpio0_10', 'P8_32': 'gpio0_11',
    'P8_33': 'gpio0_9', 'P8_34': 'gpio2_17', 'P8_35': 'gpio0_8',
    'P8_36': 'gpio2_16', 'P8_37': 'gpio2_14', 'P8_38': 'gpio2_15',
    'P8_39': 'gpio2_12', 'P8_40': 'gpio2_13', 'P8_41': 'gpio2_10',
    'P8_42': 'gpio2_11', 'P8_43': 'gpio2_8', 'P8_44': 'gpio2_9',
    'P8_45': 'gpio2_6', 'P8_46': 'gpio2_7', 'P9_11': 'gpio0_30',
    'P9_12': 'gpio1_28', 'P9_13': 'gpio0_31', 'P9_14': 'gpio1_18',
    'P9_15': 'gpio1_16', 'P9_16': 'gpio1_19', 'P9_17': 'gpio0_5',
    'P9_18': 'gpio0_4', 'P9_19': 'gpio0_13', 'P9_20': 'gpio0_12',
    'P9_21': 'gpio0_3', 'P9_22': 'gpio0_2', 'P9_23': 'gpio1_17',
    'P9_24': 'gpio0_15', 'P9_25': 'gpio3_21', 'P9_26': 'gpio0_14',
    'P9_27': 'gpio3_19', 'P9_28': 'gpio3_17', 'P9_29': 'gpio3_15',
    'P9_30': 'gpio3_16', 'P9_31': 'gpio3_14', 'P9_41': 'gpio0_20',
    'P9_42': 'gpio3_20', 'P9_43': 'gpio0_7', 'P9_44': 'gpio3_18',

    'P9_33': 'AIN4', 'P9_35': 'AIN6', 'P9_36': 'AIN5', 'P9_37': 'AIN2',
    'P9_38': 'AIN3', 'P9_39': 'AIN0', 'P9_40': 'AIN1',
}

def update_map_beaglebone(pins, mcu):
    for pin, gpio in beagleboneblack_mappings.items():
        pins[pin] = pins[gpio]


######################################################################
# Command translation
######################################################################

re_pin = re.compile(r'(?P<prefix>[ _]pin=)(?P<name>[^ ]*)')

class PinResolver:
    def __init__(self, mcu_type, validate_aliases=True):
        self.mcu_type = mcu_type
        self.validate_aliases = validate_aliases
        self.pins = dict(MCU_PINS.get(mcu_type, {}))
        self.active_pins = {}
    def update_aliases(self, mapping_name):
        self.pins = dict(MCU_PINS.get(self.mcu_type, {}))
        if mapping_name == 'arduino':
            update_map_arduino(self.pins, self.mcu_type)
        elif mapping_name == 'beaglebone':
            update_map_beaglebone(self.pins, self.mcu_type)
    def update_command(self, cmd):
        def pin_fixup(m):
            name = m.group('name')
            if name not in self.pins:
                raise error("Unable to translate pin name: %s" % (cmd,))
            pin_id = self.pins[name]
            if (name != self.active_pins.setdefault(pin_id, name)
                and self.validate_aliases):
                raise error("pin %s is an alias for %s" % (
                    name, self.active_pins[pin_id]))
            return m.group('prefix') + str(pin_id)
        return re_pin.sub(pin_fixup, cmd)


######################################################################
# Pin to chip mapping
######################################################################

class error(Exception):
    pass

class PrinterPins:
    error = error
    def __init__(self):
        self.chips = {}
        self.active_pins = {}
    def lookup_pin(self, pin_type, pin_desc, share_type=None):
        can_invert = pin_type in ['stepper', 'endstop', 'digital_out', 'pwm']
        can_pullup = pin_type == 'endstop'
        pullup = invert = 0
        if can_pullup and pin_desc.startswith('^'):
            pullup = 1
            pin_desc = pin_desc[1:].strip()
        if can_invert and pin_desc.startswith('!'):
            invert = 1
            pin_desc = pin_desc[1:].strip()
        if ':' not in pin_desc:
            chip_name, pin = 'mcu', pin_desc
        else:
            chip_name, pin = [s.strip() for s in pin_desc.split(':', 1)]
        if chip_name not in self.chips:
            raise error("Unknown pin chip name '%s'" % (chip_name,))
        share_name = "%s:%s" % (chip_name, pin)
        if share_name in self.active_pins:
            pin_params = self.active_pins[share_name]
            if share_type is None or share_type != pin_params['share_type']:
                raise error("pin %s used multiple times in config" % (pin,))
            if invert != pin_params['invert'] or pullup != pin_params['pullup']:
                raise error("Shared pin %s must have same polarity" % (pin,))
            return pin_params
        pin_params = {'chip': self.chips[chip_name], 'chip_name': chip_name,
                      'type': pin_type, 'share_type': share_type,
                      'pin': pin, 'invert': invert, 'pullup': pullup}
        self.active_pins[share_name] = pin_params
        return pin_params
    def setup_pin(self, pin_type, pin_desc):
        pin_params = self.lookup_pin(pin_type, pin_desc)
        return pin_params['chip'].setup_pin(pin_params)
    def register_chip(self, chip_name, chip):
        chip_name = chip_name.strip()
        if chip_name in self.chips:
            raise error("Duplicate chip name '%s'" % (chip_name,))
        self.chips[chip_name] = chip

def add_printer_objects(printer, config):
    printer.add_object('pins', PrinterPins())

def get_printer_pins(printer):
    return printer.lookup_object('pins')

def setup_pin(printer, pin_type, pin_desc):
    return get_printer_pins(printer).setup_pin(pin_type, pin_desc)
