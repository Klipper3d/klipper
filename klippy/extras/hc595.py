# Host module for SNx4HC595 shift register with firmware-level GPIO support
#
# Copyright (C) 2026
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins


class HC595:
    """Configures a 74HC595 shift register chain with firmware-level support.

    The firmware exposes shift register outputs as virtual GPIO pins
    (PG0-PG7 for the first chip, PH0-PH7 for the second). Stepper
    dir_pin and enable_pin can reference these directly, allowing the
    firmware to toggle them natively in the stepper ISR without any
    host involvement.

    This module sends the config_hc595 command to set up the bitbang
    pins, and provides hc595_update for host-driven state changes
    (e.g. LED control on the second chip).
    """
    def __init__(self, config):
        self.printer = config.get_printer()
        self.name = config.get_name().split()[1]
        ppins = self.printer.lookup_object('pins')
        # Resolve MCU from data_pin (any pin on the target MCU)
        data_pin_name = config.get('data_pin')
        data_params = ppins.lookup_pin(data_pin_name)
        self.mcu = data_params['chip']
        self.data_pin = data_params['pin']
        latch_params = ppins.lookup_pin(config.get('latch_pin'))
        self.latch_pin = latch_params['pin']
        clock_params = ppins.lookup_pin(config.get('clock_pin'))
        self.clock_pin = clock_params['pin']
        # Validate all pins are on the same MCU
        if latch_params['chip'] is not self.mcu:
            raise config.error("hc595: latch_pin must be on same MCU as "
                               "data_pin")
        if clock_params['chip'] is not self.mcu:
            raise config.error("hc595: clock_pin must be on same MCU as "
                               "data_pin")
        self.chain_count = config.getint('chain_count', 2, minval=1, maxval=4)
        self.oid = self.mcu.create_oid()
        self.mcu.register_config_callback(self._build_config)
        self.update_cmd = None
        self.printer.register_event_handler(
            "klippy:connect", self._handle_connect)
    def _build_config(self):
        self.mcu.add_config_cmd(
            "config_hc595 oid=%d data_pin=%s latch_pin=%s clock_pin=%s"
            % (self.oid, self.data_pin, self.latch_pin, self.clock_pin))
        cmd_queue = self.mcu.alloc_command_queue()
        self.update_cmd = self.mcu.lookup_command(
            "hc595_update oid=%c data=%*s", cq=cmd_queue)
    def _handle_connect(self):
        logging.info("hc595 '%s': configured on %s (oid=%d, chain=%d)",
                     self.name, self.mcu.get_name(), self.oid,
                     self.chain_count)
    def get_mcu(self):
        return self.mcu
    def update_state(self, data):
        """Send full state update to shift register (e.g. for LED control).
        data should be a bytes/bytearray of length chain_count."""
        if self.update_cmd is not None:
            self.update_cmd.send([self.oid, data])


def load_config_prefix(config):
    return HC595(config)
