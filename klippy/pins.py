# Pin name handling
#
# Copyright (C) 2016-2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import re

class error(Exception):
    pass


######################################################################
# Command translation
######################################################################

re_pin = re.compile(r'(?P<prefix>[ _]pin=)(?P<name>[^ ]*)')

class PinResolver:
    def __init__(self, validate_aliases=True):
        self.validate_aliases = validate_aliases
        self.reserved = {}
        self.aliases = {}
        self.active_pins = {}
    def reserve_pin(self, pin, reserve_name):
        if pin in self.reserved and self.reserved[pin] != reserve_name:
            raise error("Pin %s reserved for %s - can't reserve for %s" % (
                pin, self.reserved[pin], reserve_name))
        self.reserved[pin] = reserve_name
    def alias_pin(self, alias, pin):
        if alias in self.aliases and self.aliases[alias] != pin:
            raise error("Alias %s mapped to %s - can't alias to %s" % (
                alias, self.aliases[alias], pin))
        if pin in self.aliases:
            pin = self.aliases[pin]
        self.aliases[alias] = pin
        for existing_alias, existing_pin in self.aliases.items():
            if existing_pin == alias:
                self.aliases[existing_alias] = pin
    def update_command(self, cmd):
        def pin_fixup(m):
            name = m.group('name')
            pin_id = self.aliases.get(name, name)
            if (name != self.active_pins.setdefault(pin_id, name)
                and self.validate_aliases):
                raise error("pin %s is an alias for %s" % (
                    name, self.active_pins[pin_id]))
            if pin_id in self.reserved:
                raise error("pin %s is reserved for %s" % (
                    name, self.reserved[pin_id]))
            return m.group('prefix') + str(pin_id)
        return re_pin.sub(pin_fixup, cmd)


######################################################################
# Pin to chip mapping
######################################################################

class PrinterPins:
    error = error
    def __init__(self):
        self.chips = {}
        self.active_pins = {}
        self.pin_resolvers = {}
        self.allow_multi_use_pins = {}
    def parse_pin(self, pin_desc, can_invert=False, can_pullup=False):
        desc = pin_desc.strip()
        pullup = invert = 0
        if can_pullup and (desc.startswith('^') or desc.startswith('~')):
            pullup = 1
            if desc.startswith('~'):
                pullup = -1
            desc = desc[1:].strip()
        if can_invert and desc.startswith('!'):
            invert = 1
            desc = desc[1:].strip()
        if ':' not in desc:
            chip_name, pin = 'mcu', desc
        else:
            chip_name, pin = [s.strip() for s in desc.split(':', 1)]
        if chip_name not in self.chips:
            raise error("Unknown pin chip name '%s'" % (chip_name,))
        if [c for c in '^~!: ' if c in pin]:
            format = ""
            if can_pullup:
                format += "[^~] "
            if can_invert:
                format += "[!] "
            raise error("Invalid pin description '%s'\n"
                        "Format is: %s[chip_name:] pin_name" % (
                            pin_desc, format))
        pin_params = {'chip': self.chips[chip_name], 'chip_name': chip_name,
                      'pin': pin, 'invert': invert, 'pullup': pullup}
        return pin_params
    def lookup_pin(self, pin_desc, can_invert=False, can_pullup=False,
                   share_type=None):
        pin_params = self.parse_pin(pin_desc, can_invert, can_pullup)
        pin = pin_params['pin']
        share_name = "%s:%s" % (pin_params['chip_name'], pin)
        if share_name in self.active_pins:
            share_params = self.active_pins[share_name]
            if share_name in self.allow_multi_use_pins:
                pass
            elif share_type is None or share_type != share_params['share_type']:
                raise error("pin %s used multiple times in config" % (pin,))
            elif (pin_params['invert'] != share_params['invert']
                  or pin_params['pullup'] != share_params['pullup']):
                raise error("Shared pin %s must have same polarity" % (pin,))
            return share_params
        pin_params['share_type'] = share_type
        self.active_pins[share_name] = pin_params
        return pin_params
    def setup_pin(self, pin_type, pin_desc):
        can_invert = pin_type in ['endstop', 'digital_out', 'pwm']
        can_pullup = pin_type in ['endstop']
        pin_params = self.lookup_pin(pin_desc, can_invert, can_pullup)
        return pin_params['chip'].setup_pin(pin_type, pin_params)
    def reset_pin_sharing(self, pin_params):
        share_name = "%s:%s" % (pin_params['chip_name'], pin_params['pin'])
        del self.active_pins[share_name]
    def get_pin_resolver(self, chip_name):
        if chip_name not in self.pin_resolvers:
            raise error("Unknown chip name '%s'" % (chip_name,))
        return self.pin_resolvers[chip_name]
    def register_chip(self, chip_name, chip):
        chip_name = chip_name.strip()
        if chip_name in self.chips:
            raise error("Duplicate chip name '%s'" % (chip_name,))
        self.chips[chip_name] = chip
        self.pin_resolvers[chip_name] = PinResolver()
    def allow_multi_use_pin(self, pin_desc):
        pin_params = self.parse_pin(pin_desc)
        share_name = "%s:%s" % (pin_params['chip_name'], pin_params['pin'])
        self.allow_multi_use_pins[share_name] = True

def add_printer_objects(config):
    config.get_printer().add_object('pins', PrinterPins())
