# Filament Motion Sensor Module
#
# Copyright (C) 2021 Joshua Wherrett <thejoshw.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging, collections
from . import filament_switch_sensor

DEFAULT_CHECK_RUNOUT_TIMEOUT = .250

class EncoderSensor:
    def __init__(self, config):
        # Read config
        self.printer = config.get_printer()
        switch_pin = config.get('switch_pin')
        self.extruder_name = config.get('extruder')
        self.detection_length = config.getfloat(
                'detection_length', 7., above=0.)
        self.check_runout_timeout = config.getfloat('check_runout_timeout',
                            DEFAULT_CHECK_RUNOUT_TIMEOUT, minval=0.050)
        self.consecutive_count = config.getint('consecutive_count', 1,
                                               minval=1)
        self.current_count = 0
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
        self.filament_runout_pos = None
        self.verbose = False
        self.verbose_gcmd = None
        # Register commands and event handlers
        self.printer.register_event_handler('klippy:ready',
                self._handle_ready)
        self.printer.register_event_handler('idle_timeout:printing',
                self._handle_printing)
        self.printer.register_event_handler('idle_timeout:ready',
                self._handle_not_printing)
        self.printer.register_event_handler('idle_timeout:idle',
                self._handle_not_printing)
        # Register GCODE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_mux_command(
            'SET_FILAMENT_MOTION_SENSOR', 'SENSOR',
            self.runout_helper.name, self.cmd_SET_FILAMENT_MOTION_SENSOR,
            desc=self.cmd_SET_FILAMENT_MOTION_SENSOR_help)
    cmd_SET_FILAMENT_MOTION_SENSOR_help = \
        "Enable debug logging for the encoder event"
    def cmd_SET_FILAMENT_MOTION_SENSOR(self, gcmd):
            self.verbose = True if gcmd.getInt('Verbose', 0) == 1 else False
            self.verbose_gcmd = gcmd if self.verbose else None
            gcmd.respond_info("Filament Motion Verbose is %s" %
                              ("On" if self.verbose else "Off"))
    def _update_filament_runout_pos(self, eventtime=None):
        if eventtime is None:
            eventtime = self.reactor.monotonic()
        self.filament_runout_pos = self._get_extruder_pos(eventtime)
    def _handle_ready(self):
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.estimated_print_time = (
                self.printer.lookup_object('mcu').estimated_print_time)
        self._update_filament_runout_pos()
        self._extruder_pos_update_timer = self.reactor.register_timer(
                self._extruder_pos_update_event)
    def _handle_printing(self, print_time):
        self.current_count = 0
        self.reactor.update_timer(self._extruder_pos_update_timer,
                self.reactor.NOW)
    def _handle_not_printing(self, print_time):
        self.reactor.update_timer(self._extruder_pos_update_timer,
                self.reactor.NEVER)
    def _get_extruder_pos(self, eventtime=None):
        if eventtime is None:
            eventtime = self.reactor.monotonic()
        print_time = self.estimated_print_time(eventtime)
        return self.extruder.find_past_position(print_time)
    def _extruder_pos_update_event(self, eventtime):
        extruder_pos = self._get_extruder_pos(eventtime)
        # Check for filament runout
        diff_pos = abs(extruder_pos - self.filament_runout_pos)
        it_fail = diff_pos > self.detection_length
        is_filament_present = True
        if it_fail:
            self.current_count += 1
            if self.current_count >= self.consecutive_count:
                is_filament_present = False
        self.runout_helper.note_filament_present(is_filament_present)
        if self.verbose:
            self.verbose_gcmd.respond_info(
                "Filament Motion: Update Event pos %s present %s" %
                diff_pos, "true" if is_filament_present else "false")
        return eventtime + self.check_runout_timeout
    def encoder_event(self, eventtime, state):
        if self.extruder is not None:
            self._update_filament_runout_pos(eventtime)
            # Check for filament insertion
            # Filament is always assumed to be present on an encoder event
            self.runout_helper.note_filament_present(True)
            self.current_count = 0

def load_config_prefix(config):
    return EncoderSensor(config)
