# SX1509 Extra
#
# Copyright (C) 2018  Florian Heilmann <Florian.Heilmann@gmx.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins
import bus

# Word registers
REG_RESET = 0x7D
REG_CLOCK = 0x1E
REG_MISC = 0x1F
REG_DIR = 0x0E
REG_DATA = 0x10
REG_PULLUP = 0x06
REG_PULLDOWN = 0x08
REG_INPUT_DISABLE = 0x00
REG_ANALOG_DRIVER_ENABLE = 0x20


# Byte registers
REG_I_ON = [0x2A, 0x2D, 0x30, 0x33, 0x36, 0x3B, 0x40, 0x45,
            0x4A, 0x4D, 0x50, 0x53, 0x56, 0x5B, 0x5F, 0x65]
class SX1509(object):
    def __init__(self, config):
        self._printer = config.get_printer()
        self._name = config.get_name().split()[1]
        self._i2c = bus.MCU_I2C_from_config(config, default_speed=400000)
        self._ppins = self._printer.lookup_object("pins")
        self._ppins.register_chip("sx1509_" + self._name, self)
        self._mcu = self._i2c.get_mcu()
        self._mcu.register_config_callback(self._build_config)
        self._oid = self._i2c.get_oid()
        self._last_clock = 0
        # Set up registers default values
        self.reg_dict = {REG_DIR : 0xFFFF, REG_DATA : 0,
                         REG_PULLUP : 0, REG_PULLDOWN : 0,
                         REG_INPUT_DISABLE : 0, REG_ANALOG_DRIVER_ENABLE : 0}
        self.reg_i_on_dict = {reg : 0 for reg in REG_I_ON}
    def _build_config(self):
        # Reset the chip
        self._mcu.add_config_cmd("i2c_write oid=%d data=%02x%02x" % (
            self._oid, REG_RESET, 0x12))
        self._mcu.add_config_cmd("i2c_write oid=%d data=%02x%02x" % (
            self._oid, REG_RESET, 0x34))
        # Enable Oscillator
        self._mcu.add_config_cmd("i2c_modify_bits oid=%d reg=%02x"
                                 " clear_set_bits=%02x%02x" % (
                                     self._oid, REG_CLOCK, 0, (1 << 6)))
        # Setup Clock Divider
        self._mcu.add_config_cmd("i2c_modify_bits oid=%d reg=%02x"
                                 " clear_set_bits=%02x%02x" % (
                                     self._oid, REG_MISC, 0, (1 << 4)))
        # Transfer all regs with their initial cached state
        for _reg, _data in self.reg_dict.iteritems():
            self._mcu.add_config_cmd("i2c_write oid=%d data=%02x%04x" % (
                self._oid, _reg, _data), is_init=True)
    def setup_pin(self, pin_type, pin_params):
        if pin_type == 'digital_out' and pin_params['pin'][0:4] == "PIN_":
            return SX1509_digital_out(self, pin_params)
        elif pin_type == 'pwm' and pin_params['pin'][0:4] == "PIN_":
            return SX1509_pwm(self, pin_params)
        raise pins.error("Wrong pin or incompatible type: %s with type %s! " % (
            pin_params['pin'][0:4], pin_type))
    def get_mcu(self):
        return self._mcu
    def get_oid(self):
        return self._oid
    def clear_bits_in_register(self, reg, bitmask):
        if reg in self.reg_dict:
            self.reg_dict[reg] &= ~(bitmask)
        elif reg in self.reg_i_on_dict:
            self.reg_i_on_dict[reg] &= ~(bitmask)
    def set_bits_in_register(self, reg, bitmask):
        if reg in self.reg_dict:
            self.reg_dict[reg] |= bitmask
        elif reg in self.reg_i_on_dict:
            self.reg_i_on_dict[reg] |= bitmask
    def set_register(self, reg, value):
        if reg in self.reg_dict:
            self.reg_dict[reg] = value
        elif reg in self.reg_i_on_dict:
            self.reg_i_on_dict[reg] = value
    def send_register(self, reg, print_time):
        data = [reg & 0xFF]
        if reg in self.reg_dict:
            # Word
            data += [(self.reg_dict[reg] >> 8) & 0xFF,
                     self.reg_dict[reg] & 0xFF]
        elif reg in self.reg_i_on_dict:
            # Byte
            data += [self.reg_i_on_dict[reg] & 0xFF]
        clock = self._mcu.print_time_to_clock(print_time)
        self._i2c.i2c_write(data, minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock

class SX1509_digital_out(object):
    def __init__(self, sx1509, pin_params):
        self._sx1509 = sx1509
        self._mcu = sx1509.get_mcu()
        self._sxpin = int(pin_params['pin'].split('_')[1])
        self._bitmask = 1 << self._sxpin
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        self._mcu.register_config_callback(self._build_config)
        self._start_value = self._shutdown_value = self._invert
        self._is_static = False
        self._max_duration = 2.
        self._set_cmd = self._clear_cmd = None
        # Set direction to output
        self._sx1509.clear_bits_in_register(REG_DIR, self._bitmask)
    def _build_config(self):
        if self._max_duration:
            raise pins.error("SX1509 pins are not suitable for heaters")
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_start_value(self, start_value, shutdown_value, is_static=False):
        if is_static and start_value != shutdown_value:
            raise pins.error("Static pin can not have shutdown value")
        self._start_value = (not not start_value) ^ self._invert
        self._shutdown_value = self._invert
        self._is_static = is_static
        # We need to set the start value here so the register is
        # updated before the SX1509 class writes it.
        if self._start_value:
            self._sx1509.set_bits_in_register(REG_DATA, self._bitmask)
        else:
            self._sx1509.clear_bits_in_register(REG_DATA, self._bitmask)
    def set_digital(self, print_time, value):
        if int(value) ^ self._invert:
            self._sx1509.set_bits_in_register(REG_DATA, self._bitmask)
        else:
            self._sx1509.clear_bits_in_register(REG_DATA, self._bitmask)
        self._sx1509.send_register(REG_DATA, print_time)
    def set_pwm(self, print_time, value):
        self.set_digital(print_time, value >= 0.5)

class SX1509_pwm(object):
    def __init__(self, sx1509, pin_params):
        self._sx1509 = sx1509
        self._mcu = sx1509.get_mcu()
        self._sxpin = int(pin_params['pin'].split('_')[1])
        self._bitmask = 1 << self._sxpin
        self._i_on_reg = REG_I_ON[self._sxpin]
        self._pin = pin_params['pin']
        self._invert = pin_params['invert']
        self._mcu.register_config_callback(self._build_config)
        self._start_value = self._shutdown_value = float(self._invert)
        self._is_static = False
        self._max_duration = 2.
        self._hardware_pwm = False
        self._pwm_max = 0.
        self._set_cmd = None
        self._cycle_time = 0.
        # Set required registers
        self._sx1509.set_bits_in_register(REG_INPUT_DISABLE, self._bitmask)
        self._sx1509.clear_bits_in_register(REG_PULLUP, self._bitmask)
        self._sx1509.clear_bits_in_register(REG_DIR, self._bitmask)
        self._sx1509.set_bits_in_register(REG_ANALOG_DRIVER_ENABLE,
                                          self._bitmask)
        self._sx1509.clear_bits_in_register(REG_DATA, self._bitmask)
    def _build_config(self):
        if not self._hardware_pwm:
            raise pins.error("SX1509_pwm must have hardware_pwm enabled")
        if self._max_duration:
            raise pins.error("SX1509 pins are not suitable for heaters")
        # Send initial value
        self._sx1509.set_register(self._i_on_reg,
                                  ~int(255 * self._start_value) & 0xFF)
        self._mcu.add_config_cmd("i2c_write oid=%d data=%02x%02x" % (
            self._sx1509.get_oid(),
            self._i_on_reg,
            self._sx1509.reg_i_on_dict[self._i_on_reg]
            ),
                                 is_init=True)
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        self._cycle_time = cycle_time
        self._hardware_pwm = hardware_pwm
    def setup_start_value(self, start_value, shutdown_value, is_static=False):
        if is_static and start_value != shutdown_value:
            raise pins.error("Static pin can not have shutdown value")
        if self._invert:
            start_value = 1. - start_value
            shutdown_value = 1. - shutdown_value
        self._start_value = max(0., min(1., start_value))
        self._shutdown_value = max(0., min(1., shutdown_value))
        self._is_static = is_static
    def set_pwm(self, print_time, value):
        self._sx1509.set_register(self._i_on_reg, ~int(255 * value)
                                  if not self._invert
                                  else int(255 * value) & 0xFF)
        self._sx1509.send_register(self._i_on_reg, print_time)

def load_config_prefix(config):
    return SX1509(config)
