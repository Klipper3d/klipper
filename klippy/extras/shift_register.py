# Support for 74HC595 shift register based GPIO expanders
#
# Copyright (C) 2026
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import pins
from . import bus

PIN_MIN_TIME = 0.100

class ShiftRegisterPin:
    def __init__(self, shift_register, byte_index, bit_index, pin_params):
        self._shift_register = shift_register
        self._mcu = shift_register.get_mcu()
        self._byte_index = byte_index
        self._bit_index = bit_index
        self._bitmask = 1 << bit_index
        self._invert = pin_params['invert']
        self._start_value = self._shutdown_value = self._invert
        self._max_duration = 2.
        self._mcu.register_config_callback(self._build_config)
    def _build_config(self):
        if self._max_duration:
            raise pins.error(
                "Shift register pins are not suitable for heaters")
    def get_mcu(self):
        return self._mcu
    def setup_max_duration(self, max_duration):
        self._max_duration = max_duration
    def setup_cycle_time(self, cycle_time, hardware_pwm=False):
        if hardware_pwm:
            raise pins.error(
                "Shift register pins do not support hardware PWM")
    def setup_start_value(self, start_value, shutdown_value):
        self._start_value = (not not start_value) ^ self._invert
        self._shutdown_value = (not not shutdown_value) ^ self._invert
        if self._start_value:
            self._shift_register.set_bit(self._byte_index, self._bitmask)
        else:
            self._shift_register.clear_bit(self._byte_index, self._bitmask)
        self._shift_register.set_shutdown_bit(
            self._byte_index, self._bitmask, self._shutdown_value)
    def set_digital(self, print_time, value):
        if int(value) ^ self._invert:
            self._shift_register.set_bit(self._byte_index, self._bitmask)
        else:
            self._shift_register.clear_bit(self._byte_index, self._bitmask)
        self._shift_register.send_state(print_time)
    def set_pwm(self, print_time, value, cycle_time=None):
        self.set_digital(print_time, value >= 0.5)

class ShiftRegister74HC595:
    def __init__(self, config):
        self._printer = config.get_printer()
        self._name = config.get_name().split()[1]
        self._chain_count = config.getint('chain_count', 1, minval=1, maxval=8)
        total_pins = self._chain_count * 8
        # State tracking: one byte per chip in the chain
        self._state = bytearray(self._chain_count)
        self._shutdown_state = bytearray(self._chain_count)
        self._last_clock = 0
        # Setup SPI - latch_pin acts as CS (active-low: latches on rising edge
        # after transfer completes, which is standard SPI CS behavior)
        self._spi = bus.MCU_SPI_from_config(
            config, 0, pin_option="latch_pin",
            default_speed=4000000)
        self._mcu = self._spi.get_mcu()
        # Register chip with pin system
        ppins = self._printer.lookup_object('pins')
        ppins.register_chip(self._name, self)
        # Optional output enable pin (active-low on 74HC595)
        oe_pin = config.get('output_enable_pin', None)
        if oe_pin is not None:
            self._oe = ppins.setup_pin('digital_out', oe_pin)
            self._oe.setup_max_duration(0.)
            self._oe.setup_start_value(0, 0)
        # Finalize shutdown and initial state after all pins are configured
        self._printer.register_event_handler(
            "klippy:connect", self._handle_connect)
    def _handle_connect(self):
        self._spi.setup_shutdown_msg(self._shutdown_state)
        self._spi.spi_send(self._state)
    def get_mcu(self):
        return self._mcu
    def set_bit(self, byte_index, bitmask):
        self._state[byte_index] |= bitmask
    def clear_bit(self, byte_index, bitmask):
        self._state[byte_index] &= ~bitmask
    def set_shutdown_bit(self, byte_index, bitmask, value):
        if value:
            self._shutdown_state[byte_index] |= bitmask
        else:
            self._shutdown_state[byte_index] &= ~bitmask
    def send_state(self, print_time):
        clock = self._mcu.print_time_to_clock(print_time)
        self._spi.spi_send(self._state, minclock=self._last_clock,
                           reqclock=clock)
        self._last_clock = clock
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'digital_out':
            raise pins.error("Shift register only supports digital_out pins")
        pin = pin_params['pin']
        if pin.startswith('PIN_'):
            try:
                pin_number = int(pin[4:])
            except ValueError:
                raise pins.error("Invalid shift register pin '%s'" % (pin,))
        else:
            raise pins.error(
                "Shift register pin must be named PIN_N (e.g. PIN_0), "
                "got '%s'" % (pin,))
        total_pins = self._chain_count * 8
        if pin_number < 0 or pin_number >= total_pins:
            raise pins.error(
                "Shift register pin %d out of range (0-%d for %d chained "
                "chips)" % (pin_number, total_pins - 1, self._chain_count))
        # PIN_0 is bit 0 of the first chip (furthest from MCU in chain).
        # SPI sends bytes MSB-first and the first byte sent ends up in the
        # last chip of the chain, so we reverse the byte index.
        chip_index = pin_number // 8
        bit_index = pin_number % 8
        byte_index = self._chain_count - 1 - chip_index
        return ShiftRegisterPin(self, byte_index, bit_index, pin_params)

def load_config_prefix(config):
    return ShiftRegister74HC595(config)
