# Generic Filament Sensor Module
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2026  Hwang Younsang <famtory@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging


class RunoutHelper:
    def __init__(self, config):
        self.name = config.get_name().split()[-1]
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object('gcode')
        # Read config
        self.runout_pause = config.getboolean('pause_on_runout', True)
        if self.runout_pause:
            self.printer.load_object(config, 'pause_resume')
        self.runout_gcode = self.insert_gcode = None
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        if self.runout_pause or config.get('runout_gcode', None) is not None:
            self.runout_gcode = gcode_macro.load_template(
                config, 'runout_gcode', '')
        if config.get('insert_gcode', None) is not None:
            self.insert_gcode = gcode_macro.load_template(
                config, 'insert_gcode')
        self.pause_delay = config.getfloat('pause_delay', .5, above=.0)
        self.event_delay = config.getfloat('event_delay', 3., minval=.0)
        # Distance configuration
        self.runout_distance = config.getfloat('runout_distance', 0., minval=0.)
        self.extruder_name = config.get('extruder', 'extruder')
        self.feature_history = [(0., 0)]
        self.deferred_runout = False
        self.runout_check_timer = None
        # Internal state
        self.min_event_systime = self.reactor.NEVER
        self.filament_present = False
        self.sensor_enabled = True
        # Register commands and event handlers
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        self.printer.register_event_handler("filament_sensor:set_feature",
                                            self._handle_set_feature)
        try:
            self.gcode.register_command(
                "SET_PRINT_FEATURE", self.cmd_SET_PRINT_FEATURE,
                desc="Set current print feature for runout sensors")
        except self.printer.config_error:
            pass
        self.gcode.register_mux_command(
            "QUERY_FILAMENT_SENSOR", "SENSOR", self.name,
            self.cmd_QUERY_FILAMENT_SENSOR,
            desc=self.cmd_QUERY_FILAMENT_SENSOR_help)
        self.gcode.register_mux_command(
            "SET_FILAMENT_SENSOR", "SENSOR", self.name,
            self.cmd_SET_FILAMENT_SENSOR,
            desc=self.cmd_SET_FILAMENT_SENSOR_help)
    def _handle_ready(self):
        self.min_event_systime = self.reactor.monotonic() + 2.
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.estimated_print_time = (
            self.printer.lookup_object('mcu').estimated_print_time)
        self.feature_history = [(0., 0)]
        self.deferred_runout = False
    def cmd_SET_PRINT_FEATURE(self, gcmd):
        feature = gcmd.get_int('FEATURE', 0)
        toolhead = self.printer.lookup_object('toolhead')
        toolhead.register_lookahead_callback(
            lambda pt: self.printer.send_event("filament_sensor:set_feature", pt, feature))
    def _handle_set_feature(self, pt, feature):
        self.feature_history.append((pt, feature))
        if len(self.feature_history) > 100:
            self.feature_history.pop(0)
    def _get_feature_at_time(self, print_time):
        current_feature = 0
        for pt, feature in self.feature_history:
            if pt <= print_time:
                current_feature = feature
            else:
                break
        return current_feature
    def _runout_distance_check_event(self, eventtime):
        if not self.deferred_runout:
            return self.reactor.NEVER
        print_time = self.estimated_print_time(eventtime)
        current_feature = self._get_feature_at_time(print_time)
        if current_feature == 2:
            logging.info("Filament Sensor %s: Deferred runout pausing now on infill" % (self.name,))
            self.deferred_runout = False
            self.reactor.register_callback(self._runout_event_handler)
            return self.reactor.NEVER
        current_pos = self.extruder.find_past_position(print_time)
        diff = current_pos - self.runout_trigger_pos
        if diff >= self.runout_distance:
            logging.info(
                "Filament Sensor %s: runout distance limit reached (%.2f / %.2f). Pausing now." %
                (self.name, diff, self.runout_distance))
            self.deferred_runout = False
            self.reactor.register_callback(self._runout_event_handler)
            return self.reactor.NEVER
        return eventtime + 0.100
    def _runout_event_handler(self, eventtime):
        # Pausing from inside an event requires that the pause portion
        # of pause_resume execute immediately.
        pause_prefix = ""
        if self.runout_pause:
            pause_resume = self.printer.lookup_object('pause_resume')
            pause_resume.send_pause_command()
            pause_prefix = "PAUSE\n"
            self.printer.get_reactor().pause(eventtime + self.pause_delay)
        self._exec_gcode(pause_prefix, self.runout_gcode)
    def _insert_event_handler(self, eventtime):
        self._exec_gcode("", self.insert_gcode)
    def _exec_gcode(self, prefix, template):
        try:
            self.gcode.run_script(prefix + template.render() + "\nM400")
        except Exception:
            logging.exception("Script running error")
        self.min_event_systime = self.reactor.monotonic() + self.event_delay
    def note_filament_present(self, eventtime, is_filament_present):
        if is_filament_present == self.filament_present:
            return
        self.filament_present = is_filament_present

        if eventtime < self.min_event_systime or not self.sensor_enabled:
            # do not process during the initialization time, duplicates,
            # during the event delay time, while an event is running, or
            # when the sensor is disabled
            return
        # Determine "printing" status
        now = self.reactor.monotonic()
        idle_timeout = self.printer.lookup_object("idle_timeout")
        is_printing = idle_timeout.get_status(now)["state"] == "Printing"
        # Perform filament action associated with status change (if any)
        if is_filament_present:
            if self.deferred_runout:
                self.deferred_runout = False
                logging.info(
                    "Filament Sensor %s: filament reinserted, clearing deferred runout" % (self.name,))
            if not is_printing and self.insert_gcode is not None:
                # insert detected
                self.min_event_systime = self.reactor.NEVER
                logging.info(
                    "Filament Sensor %s: insert event detected, Time %.2f" %
                    (self.name, now))
                self.reactor.register_callback(self._insert_event_handler)
        elif is_printing and self.runout_gcode is not None:
            # runout detected
            self.min_event_systime = self.reactor.NEVER
            logging.info(
                "Filament Sensor %s: runout event detected, Time %.2f" %
                (self.name, now))
            # Check if we should defer (current feature is 1 - Outer Wall)
            print_time = self.estimated_print_time(now)
            current_feature = self._get_feature_at_time(print_time)
            if current_feature == 1 and self.runout_distance > 0.:
                self.deferred_runout = True
                self.runout_trigger_pos = self.extruder.find_past_position(print_time)
                logging.info(
                    "Filament Sensor %s: outer wall detected, deferring runout. Start position: %.2f" %
                    (self.name, self.runout_trigger_pos))
                if self.runout_check_timer is None:
                    self.runout_check_timer = self.reactor.register_timer(
                        self._runout_distance_check_event)
                self.reactor.update_timer(self.runout_check_timer, self.reactor.NOW)
            else:
                self.reactor.register_callback(self._runout_event_handler)
    def get_status(self, eventtime):
        return {
            "filament_detected": bool(self.filament_present),
            "enabled": bool(self.sensor_enabled)}
    cmd_QUERY_FILAMENT_SENSOR_help = "Query the status of the Filament Sensor"
    def cmd_QUERY_FILAMENT_SENSOR(self, gcmd):
        if self.filament_present:
            msg = "Filament Sensor %s: filament detected" % (self.name)
        else:
            msg = "Filament Sensor %s: filament not detected" % (self.name)
        gcmd.respond_info(msg)
    cmd_SET_FILAMENT_SENSOR_help = "Sets the filament sensor on/off"
    def cmd_SET_FILAMENT_SENSOR(self, gcmd):
        self.sensor_enabled = gcmd.get_int("ENABLE", 1)

class SwitchSensor:
    def __init__(self, config):
        printer = config.get_printer()
        buttons = printer.load_object(config, 'buttons')
        switch_pin = config.get('switch_pin')
        buttons.register_debounce_button(switch_pin, self._button_handler
                                         , config)
        self.runout_helper = RunoutHelper(config)
        self.get_status = self.runout_helper.get_status
    def _button_handler(self, eventtime, state):
        self.runout_helper.note_filament_present(eventtime, state)

def load_config_prefix(config):
    return SwitchSensor(config)
