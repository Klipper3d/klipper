# Filament Motion Sensor Module
#
# Copyright (C) 2021 Joshua Wherrett <thejoshw.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

CHECK_RUNOUT_TIMEOUT = .250

class RunoutHelper:
    def __init__(self, config):
        (self.mname, self.name) = config.get_name().split(None, 1)
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object('gcode')
        # Read config
        self.runout_pause = config.getboolean('pause_on_runout', True)
        self.runout_gcode = self.insert_gcode = None
        gcode_macro = self.printer.load_object(config, 'gcode_macro')
        if self.runout_pause or config.get('runout_gcode', None) is not None:
            self.runout_gcode = gcode_macro.load_template(
                config, 'runout_gcode', '')
        if config.get('insert_gcode', None) is not None:
            self.insert_gcode = gcode_macro.load_template(
                config, 'insert_gcode')
        self.pause_delay = config.getfloat('pause_delay', .5, above=.0)
        self.detection_length = config.getfloat(
                'detection_length', 7., above=0.)
        self.extruder_name = config.get("extruder", None)
        self.extruder = None
        self.toolhead = None
        self.mcu = None
        self.idle_timeout = None
        self.pause_resume = None
        self._insert_processed = False
        # Internal state
        self.sensor_enabled = True
        self.sensor_state = None
        self.filament_runout_pos = None
        self.is_printing = False
        # Register commands and event handlers
        self.printer.register_event_handler("klippy:ready",
                self._handle_ready)
        self.printer.register_event_handler("idle_timeout:printing",
                self._handle_printing)
        self.printer.register_event_handler("idle_timeout:ready",
                self._handle_not_printing)
        self.printer.register_event_handler("idle_timeout:idle",
                self._handle_not_printing)
        self.gcode.register_mux_command(
            "QUERY_FILAMENT_MOTION_SENSOR", "SENSOR", self.name,
            self.cmd_QUERY_FILAMENT_MOTION_SENSOR,
            desc=self.cmd_QUERY_FILAMENT_MOTION_SENSOR_help)
        self.gcode.register_mux_command(
            "SET_FILAMENT_MOTION_SENSOR", "SENSOR", self.name,
            self.cmd_SET_FILAMENT_MOTION_SENSOR,
            desc=self.cmd_SET_FILAMENT_MOTION_SENSOR_help)
    def _can_be_enabled(self):
        if self.detection_length is None:
            logging.error(
                    "%s(%s): ERROR: detection_length must defined",
                    self.mname, self.name)
            return False
        elif self.runout_pause:
            self.pause_resume = self.printer.lookup_object("pause_resume", None)
            if self.pause_resume is None:
                logging.error(
                        "%s(%s): ERROR: [pause_resume] section must be defined "
                        "if using pause_on_runout", self.mname, self.name)
                return False
        logging.info("%s(%s): ready to use", self.mname, self.name)
        return True
    def _update_filament_runout_pos(self, eventtime=None):
        if eventtime == None:
            eventtime = self.reactor.monotonic()
        self.filament_runout_pos = (
                self._get_extruder_pos(eventtime) +
                self.detection_length)
    def _handle_ready(self):
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.toolhead = self.printer.lookup_object("toolhead")
        self.mcu = self.extruder.stepper.get_mcu()
        self.idle_timeout = self.printer.lookup_object("idle_timeout")
        self.sensor_enabled = self._can_be_enabled()
        self._update_filament_runout_pos()
        self._check_runout_timer = self.reactor.register_timer(
                self._check_if_runout)
    def _handle_printing(self, print_time):
        self.is_printing = True
        self.reactor.update_timer(self._check_runout_timer, self.reactor.NOW)
    def _handle_not_printing(self, print_time):
        self.is_printing = False
        self.reactor.update_timer(self._check_runout_timer, self.reactor.NEVER)
    def _get_extruder_pos(self, eventtime=None):
        if eventtime == None:
            eventtime = self.reactor.monotonic()
        print_time = self.mcu.estimated_print_time(eventtime)
        return self.extruder.find_past_position(print_time)
    def _check_if_runout(self, eventtime):
        if self.is_printing:
            timeout = eventtime + CHECK_RUNOUT_TIMEOUT
        else:
            timeout = self.reactor.NEVER
        if self.filament_runout_pos != None:
            extruder_pos = self._get_extruder_pos(eventtime)
            if extruder_pos >= self.filament_runout_pos:
                # Stop further runouts from triggering until this one
                # is processed
                timeout = self.reactor.NEVER
                self.reactor.register_callback(self._process_runout)
        return timeout
    def _process_runout(self, eventtime):
        pause_prefix = ""
        if self.runout_pause:
            # Pausing from inside an event requires that the pause portion
            # of pause_resume execute immediately.
            self.pause_resume.send_pause_command()
            pause_prefix = "PAUSE\n"
            self.toolhead.wait_moves()
            self.reactor.pause(eventtime + self.pause_delay)
        self._exec_gcode(pause_prefix, self.runout_gcode)
        self._update_filament_runout_pos()
        if self.is_printing:
            self.reactor.update_timer(
                    self._check_runout_timer, self.reactor.NOW)
    def _insert_event_handler(self, eventtime):
        self._exec_gcode("", self.insert_gcode)
    def _exec_gcode(self, prefix, template):
        try:
            self.gcode.run_script(prefix + template.render() + "\nM400")
        except Exception:
            logging.exception("Script running error")
    def encoder_event(self, eventtime, state):
        self.sensor_state = state
        if not self.sensor_enabled or self.idle_timeout is None:
            # do not process during the initialisation time or
            # when the sensor is disabled
            return
        # Perform filament action associated with status change (if any)
        if self.is_printing:
            self._insert_processed = False
            self._update_filament_runout_pos(eventtime)
        else:
            # Not printing so filament must have been inserted
            if self.insert_gcode is not None:
                # Due to this sensor constantly triggering during insert
                # only trigger the insert event once
                if not self._insert_processed:
                    # insert detected
                    logging.info(
                        "Filament Sensor %s: insert event detected, Time %.2f" %
                        (self.name, eventtime))
                    self.reactor.register_callback(self._insert_event_handler)
            self._insert_processed = True
    def get_status(self, eventtime):
        return {
            "enabled": bool(self.sensor_enabled)}
    cmd_QUERY_FILAMENT_MOTION_SENSOR_help = (
            "Query the state of the filament motion sensor "
            "(toggles 0/1 during motion)")
    def cmd_QUERY_FILAMENT_MOTION_SENSOR(self, gcmd):
        msg = "Filament Motion Sensor %s: State %s" % (
                self.name, self.sensor_state)
        gcmd.respond_info(msg)
    cmd_SET_FILAMENT_MOTION_SENSOR_help = ("Sets the filament motion sensor "
            "enable and detection_length")
    def cmd_SET_FILAMENT_MOTION_SENSOR(self, gcmd):
        self.detection_length = gcmd.get_float(
                "DETECTION_LENGTH", self.detection_length, minval=0.0)
        self.sensor_enabled = (True if gcmd.get_int(
                "ENABLE", 1 if self.sensor_enabled else 0 ) == 1 and
                self._can_be_enabled() else False)
        gcmd.respond_info("%s(%s): enable = %s, detection_length = %.2f" % (
                self.mname, self.name, self.sensor_enabled,
                self.detection_length))
        self.reactor.register_callback(self._update_filament_runout_pos)

class EncoderSensor:
    def __init__(self, config):
        printer = config.get_printer()
        # This sensor behaves like a button
        buttons = printer.load_object(config, 'buttons')
        switch_pin = config.get('switch_pin')
        buttons.register_buttons([switch_pin], self._button_handler)
        self.runout_helper = RunoutHelper(config)
        self.get_status = self.runout_helper.get_status
    def _button_handler(self, eventtime, state):
        self.runout_helper.encoder_event(eventtime, state)

def load_config_prefix(config):
    return EncoderSensor(config)
