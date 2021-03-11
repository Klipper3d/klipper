# Filament Motion Sensor Module
#
# Copyright (C) 2021 Joshua Wherrett <thejoshw.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import filament_switch_sensor

CHECK_RUNOUT_TIMEOUT = .250

class MotionHelper:
    def __init__(self, config):
        # Read config
        self.printer = config.get_printer()
        self.extruder_name = config.get("extruder", None)
        self.detection_length = config.getfloat(
                'detection_length', 7., above=0.)
        # Get printer objects
        self.reactor = self.printer.get_reactor()
        self.runout_helper = filament_switch_sensor.RunoutHelper(config)
        self.extruder = None
        self.mcu = None
        # Initialise internal state
        self.filament_runout_pos = None
        # Register commands and event handlers
        self.printer.register_event_handler("klippy:ready",
                self._handle_ready)
        self.printer.register_event_handler("idle_timeout:printing",
                self._handle_printing)
        self.printer.register_event_handler("idle_timeout:ready",
                self._handle_not_printing)
        self.printer.register_event_handler("idle_timeout:idle",
                self._handle_not_printing)
    def _update_filament_runout_pos(self, eventtime=None):
        if eventtime == None:
            eventtime = self.reactor.monotonic()
        self.filament_runout_pos = (
                self._get_extruder_pos(eventtime) +
                self.detection_length)
    def _handle_ready(self):
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.mcu = self.extruder.stepper.get_mcu()
        self._update_filament_runout_pos()
        self._extruder_pos_update_timer = self.reactor.register_timer(
                self._extruder_pos_update_event)
    def _handle_printing(self, print_time):
        self.reactor.update_timer(self._extruder_pos_update_timer,
                self.reactor.NOW)
    def _handle_not_printing(self, print_time):
        self.reactor.update_timer(self._extruder_pos_update_timer,
                self.reactor.NEVER)
    def _get_extruder_pos(self, eventtime=None):
        if eventtime == None:
            eventtime = self.reactor.monotonic()
        print_time = self.mcu.estimated_print_time(eventtime)
        return self.extruder.find_past_position(print_time)
    def _extruder_pos_update_event(self, eventtime):
        extruder_pos = self._get_extruder_pos(eventtime)
        # Check for filament runout
        self.runout_helper.note_filament_present(
                extruder_pos < self.filament_runout_pos)
        return eventtime + CHECK_RUNOUT_TIMEOUT
    def encoder_event(self, eventtime, state):
        if self.mcu != None:
            self._update_filament_runout_pos(eventtime)
            # Check for filament insertion
            # Filament is always assumed to be present on an encoder event
            self.runout_helper.note_filament_present(True)

class EncoderSensor:
    def __init__(self, config):
        printer = config.get_printer()
        # This sensor behaves like a button
        buttons = printer.load_object(config, 'buttons')
        switch_pin = config.get('switch_pin')
        buttons.register_buttons([switch_pin], self._button_handler)
        self.motion_helper = MotionHelper(config)
    def _button_handler(self, eventtime, state):
        self.motion_helper.encoder_event(eventtime, state)

def load_config_prefix(config):
    return EncoderSensor(config)
