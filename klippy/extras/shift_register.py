# Support for 74HC595 shift register based GPIO expanders
#
# Copyright (C) 2026
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
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
        # Send initial state (all zeros) as MCU init command.
        # At this point spi_send_cmd is None, so this queues an init
        # command that runs after MCU config is complete.
        self._spi.spi_send(self._state)
        # After connect, send the actual configured state (with
        # start values set by setup_start_value calls from other modules)
        self._printer.register_event_handler(
            "klippy:connect", self._handle_connect)
    def _handle_connect(self):
        reactor = self._mcu.get_printer().get_reactor()
        curtime = reactor.monotonic()
        print_time = self._mcu.estimated_print_time(curtime)
        self.send_state(print_time)
    def get_mcu(self):
        return self._mcu
    def get_name(self):
        return self._name
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
    def _parse_pin_number(self, pin):
        if not pin.startswith('PIN_'):
            raise pins.error(
                "Shift register pin must be named PIN_N (e.g. PIN_0), "
                "got '%s'" % (pin,))
        try:
            pin_number = int(pin[4:])
        except ValueError:
            raise pins.error("Invalid shift register pin '%s'" % (pin,))
        total_pins = self._chain_count * 8
        if pin_number < 0 or pin_number >= total_pins:
            raise pins.error(
                "Shift register pin %d out of range (0-%d for %d chained "
                "chips)" % (pin_number, total_pins - 1, self._chain_count))
        return pin_number
    def get_pin_indices(self, pin_number):
        # PIN_0 is bit 0 of the first chip (furthest from MCU in chain).
        # SPI sends bytes MSB-first and the first byte sent ends up in the
        # last chip of the chain, so we reverse the byte index.
        chip_index = pin_number // 8
        bit_index = pin_number % 8
        byte_index = self._chain_count - 1 - chip_index
        return byte_index, bit_index
    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'digital_out':
            raise pins.error("Shift register only supports digital_out pins")
        pin_number = self._parse_pin_number(pin_params['pin'])
        byte_index, bit_index = self.get_pin_indices(pin_number)
        return ShiftRegisterPin(self, byte_index, bit_index, pin_params)


class ShiftRegisterStepperDir:
    """Hooks into a manual_stepper to manage direction via a shift register.

    This allows a manual_stepper's physical direction pin to be on a 74HC595
    shift register while the firmware uses a dummy dir_pin. Before each move,
    this module updates the shift register direction bit via SPI, timed to
    complete before stepping begins.
    """
    def __init__(self, config):
        self._printer = config.get_printer()
        self._name = config.get_name().split(None, 1)[1]
        # Locate the shift register and pin
        ppins = self._printer.lookup_object('pins')
        dir_pin_desc = config.get('dir_pin')
        dir_pin_params = ppins.parse_pin(dir_pin_desc, can_invert=True)
        self._sr = dir_pin_params['chip']
        if not isinstance(self._sr, ShiftRegister74HC595):
            raise config.error(
                "%s: dir_pin must be on a shift_register chip" % (
                    config.get_name(),))
        pin_number = self._sr._parse_pin_number(dir_pin_params['pin'])
        byte_index, bit_index = self._sr.get_pin_indices(pin_number)
        self._byte_index = byte_index
        self._bitmask = 1 << bit_index
        self._invert = dir_pin_params['invert']
        self._last_dir = -1
        # Locate the manual_stepper to hook into
        self._stepper_name = config.get('manual_stepper')
        self._printer.register_event_handler(
            "klippy:ready", self._handle_ready)
    def _handle_ready(self):
        stepper_obj = self._printer.lookup_object(
            'manual_stepper ' + self._stepper_name)
        # Wrap do_move to update direction before each move
        orig_do_move = stepper_obj.do_move
        def wrapped_do_move(movepos, speed, accel, sync=True):
            self._update_dir(stepper_obj, movepos)
            return orig_do_move(movepos, speed, accel, sync)
        stepper_obj.do_move = wrapped_do_move
        # Wrap do_homing_move to update direction before homing
        orig_do_homing_move = stepper_obj.do_homing_move
        def wrapped_do_homing_move(movepos, speed, accel,
                                   probe_pos, triggered, check_trigger):
            self._update_dir(stepper_obj, movepos)
            return orig_do_homing_move(movepos, speed, accel,
                                       probe_pos, triggered, check_trigger)
        stepper_obj.do_homing_move = wrapped_do_homing_move
        # Wrap process_move for gcode axis mode
        orig_process_move = stepper_obj.process_move
        def wrapped_process_move(print_time, move, ea_index):
            axis_r = move.axes_r[ea_index]
            start_pos = move.start_pos[ea_index]
            end_pos = move.end_pos[ea_index]
            if end_pos != start_pos:
                new_dir = 1 if end_pos > start_pos else 0
                self._set_dir(print_time, new_dir)
            return orig_process_move(print_time, move, ea_index)
        stepper_obj.process_move = wrapped_process_move
        logging.info("shift_register_stepper_dir: hooked into '%s' "
                     "for direction control via shift register '%s'",
                     self._stepper_name, self._sr.get_name())
    def _update_dir(self, stepper_obj, movepos):
        cp = stepper_obj.commanded_pos
        if movepos == cp:
            return
        new_dir = 1 if movepos > cp else 0
        stepper_obj.sync_print_time()
        self._set_dir(stepper_obj.next_cmd_time, new_dir)
    def _set_dir(self, print_time, new_dir):
        if new_dir == self._last_dir:
            return
        self._last_dir = new_dir
        value = new_dir ^ self._invert
        if value:
            self._sr.set_bit(self._byte_index, self._bitmask)
        else:
            self._sr.clear_bit(self._byte_index, self._bitmask)
        self._sr.send_state(print_time)


def load_config_prefix(config):
    return ShiftRegister74HC595(config)
