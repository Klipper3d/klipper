# Filament Motion Sensor Module
#
# Copyright (C) 2021 Joshua Wherrett <thejoshw.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
from . import filament_switch_sensor

CHECK_RUNOUT_TIMEOUT = .250

class EncoderSensor:
    def __init__(self, config):
        # Read config
        self.printer = config.get_printer()
        switch_pin = config.get('switch_pin')
        self.extruder_name = config.get('extruder')
        self.detection_length = config.getfloat(
                'detection_length', 7., above=0.)
        self.time_based = config.getboolean('time_based', False)
        self.timeout_sec = config.getfloat(
                'timeout_seconds', 30., above=0.)
        self.arm_sec = config.getfloat(
                'arm_seconds', 120., above=0.)
        # Configure pins
        buttons = self.printer.load_object(config, 'buttons')
        buttons.register_buttons([switch_pin], self.encoder_event)
        # Get printer objects
        self.reactor = self.printer.get_reactor()
        self.runout_helper = filament_switch_sensor.RunoutHelper(config)
        self.get_status = self.runout_helper.get_status
        self.extruder = None
        self.estimated_print_time = None
        # Initialise internal state
        self.start_time = None
        self.filament_runout_pos = None
        self.last_extruder_pos = None
        self.last_extruder_time = None
        self.last_filament_time = None
        # Register commands and event handlers
        self.printer.register_event_handler('klippy:ready',
                self._handle_ready)
        self.printer.register_event_handler('idle_timeout:printing',
                self._handle_printing)
        self.printer.register_event_handler('idle_timeout:ready',
                self._handle_not_printing)
        self.printer.register_event_handler('idle_timeout:idle',
                self._handle_not_printing)

        if self.time_based:
            logging.info(
                    "Filament Sensor %s: using time-based detection, "
                    "%.2f seconds" %
                    (self.runout_helper.name, self.timeout_sec))
    def _update_filament_runout_pos(self, eventtime=None):
        logging.info(
                "Filament Sensor %s: position triggered" %
                (self.runout_helper.name))

        if eventtime is None:
            eventtime = self.reactor.monotonic()
        if self.time_based:
            self.last_filament_time = eventtime
        self.filament_runout_pos = (
                self._get_extruder_pos(eventtime) +
                self.detection_length)
    def _handle_ready(self):
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.estimated_print_time = (
                self.printer.lookup_object('mcu').estimated_print_time)
        self._update_filament_runout_pos()
        self._extruder_pos_update_timer = self.reactor.register_timer(
                self._extruder_pos_update_event)
    def _handle_printing(self, print_time):
        # Force update the sensor time to the latest if we just started
        # printing since we might not tick for another 7mm
        if self.time_based:
            self.last_filament_time = None
            self.last_extruder_time = None
        self.start_time = self.reactor.monotonic()
        self.reactor.update_timer(self._extruder_pos_update_timer,
                self.reactor.NOW)
    def _handle_not_printing(self, print_time):
        if self.time_based:
            self.last_filament_time = None
            self.last_extruder_time = None
        self.reactor.update_timer(self._extruder_pos_update_timer,
                self.reactor.NEVER)
    def _get_extruder_pos(self, eventtime=None):
        if eventtime is None:
            eventtime = self.reactor.monotonic()
        print_time = self.estimated_print_time(eventtime)
        return self.extruder.find_past_position(print_time)
    def _extruder_pos_update_event(self, eventtime):
        extruder_pos = self._get_extruder_pos(eventtime)

        if self.time_based:
            # Check for filament timeout

            # If the extruder hasn't moved we don't care
            if self.last_extruder_pos == extruder_pos:
                return eventtime + CHECK_RUNOUT_TIMEOUT

            # If this is the first time the extruder has moved
            # set the location but don't start the timer
            if self.last_extruder_pos == None:
                self.last_extruder_pos = extruder_pos
                return eventtime + CHECK_RUNOUT_TIMEOUT

            self.last_extruder_pos = extruder_pos

            if self.last_extruder_time is None:
                self.last_extruder_time = self.reactor.monotonic()
                self.last_filament_time = self.last_extruder_time
            else:
                self.last_extruder_time = self.reactor.monotonic()

            if self.last_filament_time is None:
                self.last_filament_time = self.last_extruder_time

            if self.reactor.monotonic() < self.start_time - self.arm_sec:
                logging.info(
                        "Filament Sensor %s: not armed (%f remaining)" %
                        (self.runout_helper.name,
                            self.start_time - self.arm_sec))
                return eventtime + CHECK_RUNOUT_TIMEOUT

            logging.info(
                    "Filament Sensor %s: last %f diff %f" %
                    (self.runout_helper.name, self.last_filament_time,
                        self.last_extruder_time - self.last_filament_time))
            self.runout_helper.note_filament_present(
                    (self.last_extruder_time - self.last_filament_time) <
                    self.timeout_sec)
        else:
            # Check for filament runout
            if self.reactor.monotonic() < self.start_time - self.arm_sec:
                logging.info(
                        "Filament Sensor %s: not armed (%f remaining)" %
                        (self.runout_helper.name,
                            self.start_time - self.arm_sec))
                return eventtime + CHECK_RUNOUT_TIMEOUT

            self.runout_helper.note_filament_present(
                extruder_pos < self.filament_runout_pos)

        return eventtime + CHECK_RUNOUT_TIMEOUT
    def encoder_event(self, eventtime, state):
        if self.extruder is not None:
            self._update_filament_runout_pos(eventtime)
            # Check for filament insertion
            # Filament is always assumed to be present on an encoder event
            self.runout_helper.note_filament_present(True)

def load_config_prefix(config):
    return EncoderSensor(config)
