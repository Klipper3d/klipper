# Handle shift register pins
#
# Copyright (C) 2025  Hans-Albert Maritz <maritz.hans@gmail.com>
# Copyright (C) 2026  Michael Atzmueller <michael.atzmueller98@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from klippy import mcu

class ShiftRegisterPin:
    def __init__(self, pin_params):
        self._mcu = pin_params['chip'].get_mcu()
        self._sr_oid = pin_params['chip'].get_oid()
        self._pin_num = pin_params['pin']
        self._invert = pin_params['invert']
        self._mcu.register_config_callback(self._build_config)
        self._last_clock = 0
        self._set_cmd = None
        self._start_value = 0
        self._shutdown_value = 0

    def setup_max_duration(self, duration):
        pass

    def set_digital(self, print_time, value):
        clock = self._mcu.print_time_to_clock(print_time)
        self._set_cmd.send([self._oid, clock, (not not value) ^ self._invert],
                           minclock=self._last_clock, reqclock=clock)
        self._last_clock = clock
        pass

    def setup_start_value(self, start_value, shutdown_value):
        self._start_value = (not not start_value) ^ self._invert
        self._shutdown_value = (not not shutdown_value) ^ self._invert

    def get_mcu(self):
        return self._mcu

    def _build_config(self):
        self._oid = self._mcu.create_oid()
        pin = str((self._sr_oid << 8) | int(self._pin_num))
        self._mcu.add_config_cmd(
            "config_digital_out oid=%d pin=%s value=%d default_value=%d "
            "max_duration=%d"
            % (self._oid, pin, self._start_value, self._shutdown_value, 0))
        cmd_queue = self._mcu.alloc_command_queue()
        self._set_cmd = self._mcu.lookup_command(
            "queue_digital_out oid=%c clock=%u on_ticks=%u", cq=cmd_queue)
        self._update_cmd = self._mcu.lookup_command(
            "update_digital_out oid=%c value=%c", cq=cmd_queue)

class ShiftRegister:
    _oid = None
    _set_cmd = None
    def __init__(self, config):
        logging.info(config.get_name() + " : Generic Shift Register Setup")

        printer = config.get_printer()
        self._mcu = mcu.get_printer_mcu(printer, config.get('mcu', 'mcu'))
        logging.info("Using MCU: " + self._mcu.get_name())
        self._num_registers = config.getint('num_registers', 1)

        ppins = printer.lookup_object('pins')
        ppins.register_chip(config.get_name().split()[1], self)
        self._data_pin_params = ppins.lookup_pin(config.get('data_pin'))
        self._latch_pin_params = ppins.lookup_pin(config.get('latch_pin'))
        self._clock_pin_params = ppins.lookup_pin(config.get('clock_pin'))

        self._oid = self._mcu.create_oid()
        if self._oid == 0:
            # oid of 0 indicates no shift register, normal pins should be used
            self._oid = self._mcu.create_oid()

        # Register shift register pin enums early so they are available for
        # config command encoding (pin names like "259" need to be valid).
        self._add_sr_pin_enums()

        self._mcu.register_config_callback(self._build_config)

    def _add_sr_pin_enums(self):
        logging.info("Adding shift register pin enums")
        shift_register_enums = {}
        # Add regular pin numbers for compatibility
        for i in range(self._num_registers * 8):
            shift_register_enums['%d' % i] = i
        # Add encoded pins (sr_oid<<8 | pin_index) so the host can send them
        # without needing a separate shift_register_oid field.
        for i in range(self._num_registers * 8):
            encoded = (self._oid << 8) | i
            shift_register_enums['%d' % encoded] = encoded
        self._mcu._serial.get_msgparser().fill_enumerations({
            'pin': shift_register_enums
        })

    def _build_config(self):
        self._mcu.add_config_cmd(
            "config_shift_register oid=%d data_pin=%s clock_pin=%s "
            "latch_pin=%s num_registers=%d"
            % (self._oid, self._data_pin_params['pin'],
                    self._clock_pin_params['pin'],
                    self._latch_pin_params['pin'], self._num_registers))
        self._add_sr_pin_enums()

    def setup_pin(self, pin_type, pin_params):
        return ShiftRegisterPin(pin_params)

    @staticmethod
    def is_sreg():
        return True

    def get_oid(self):
        return self._oid

    def get_mcu(self):
        return self._mcu

def load_config_prefix(config):
    return ShiftRegister(config)
