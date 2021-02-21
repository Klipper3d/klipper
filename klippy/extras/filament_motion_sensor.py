# Filament Motion Sensor Module
#
# Copyright (C) 2021 Joshua Wherrett <thejoshw.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class RunoutHelper:
    def __init__(self, config):
        (self.mname, self.name) = config.get_name().split(None, 1)
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.mutex = self.printer.reactor.mutex()
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
        self._mcu = None
        self.idle_timeout = None
        self.pause_resume = None
        self._extruder_move_list = list()
        self._insert_processed = False
        self._runout_eventtime = None
        # Internal state
        self.sensor_enabled = True
        self.sensor_state = None
        self._filament_runout_timer = self.reactor.register_timer(
                self._runout_event_handler)
        self._filament_runout_pos = None
        # Register commands and event handlers
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        self.printer.register_event_handler(
                "extruder:move", self._handle_commanded_extruder_move)
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
    def _handle_ready(self):
        self.extruder = self.printer.lookup_object(self.extruder_name)
        self.toolhead = self.printer.lookup_object("toolhead")
        self._mcu = self.extruder.stepper.get_mcu()
        self.idle_timeout = self.printer.lookup_object("idle_timeout")
        self.sensor_enabled = self._can_be_enabled()
        self._filament_runout_pos = (
                self.extruder.stepper.get_commanded_position() +
                self.detection_length)
    def eventtime_to_print_time(self, eventtime):
        return self._mcu.estimated_print_time(eventtime)
    def print_time_to_eventtime(self, print_time):
        #TODO: Replace call to private mcu member _clocksync
        return self._mcu._clocksync.estimate_clock_systime(
                self._mcu.print_time_to_clock(print_time))
    def _handle_commanded_extruder_move(self, move_params):
        # Need a mutex because this call is not handled by a reactor greenlet
        # (or is it...)
        with self.mutex:
            self._extruder_move_list.append(move_params)
        # A new move could move the extruder to a point where a runout occurs
        # so update the runout timer
        self.reactor.register_callback(self._update_filament_runout_timer)
    def _update_filament_runout_timer(self, eventtime):
        if self._filament_runout_pos:
            if self._filament_runout_timer.waketime == self.reactor.NEVER:
                runout_pos = self._filament_runout_pos
                with self.mutex:
                    for (start_time,
                         end_time,
                         start_pos,
                         end_pos,
                         cruise_vel) in self._extruder_move_list:
                        if ((end_pos > start_pos) and
                            (start_pos <= runout_pos) and
                            (end_pos >= runout_pos)):
                            runout_print_time = (
                                    ((runout_pos - start_pos) / cruise_vel) +
                                    start_time)
                            self.reactor.update_timer(
                                    self._filament_runout_timer,
                                    self.print_time_to_eventtime(
                                            runout_print_time))
                            break
        else:
            self.reactor.update_timer(
                    self._filament_runout_timer, self.reactor.NEVER)
    def _update_filament_runout_pos(self, eventtime=None):
        if eventtime == None:
            eventtime = self.reactor.monotonic()
        runout_pos = self._get_extruder_pos(eventtime) + self.detection_length
        if runout_pos != self._filament_runout_pos:
            self._filament_runout_pos = runout_pos
            self.reactor.update_timer(
                    self._filament_runout_timer, self.reactor.NEVER)
            self._update_filament_runout_timer(eventtime)
    def _get_extruder_pos(self, eventtime=None):
        if eventtime == None:
            eventtime = self.reactor.monotonic()
        print_time_now = self.eventtime_to_print_time(eventtime)
        extruder_pos = None
        list_idx = 0
        with self.mutex:
            # First check queued moves to extrapolate extruder position now
            for (start_time,
                 end_time,
                 start_pos,
                 end_pos,
                 cruise_vel) in self._extruder_move_list:
                if ((start_time <= print_time_now) and
                    (end_time >= print_time_now)):
                    extruder_pos = (
                            ((print_time_now - start_time) * cruise_vel) +
                            start_pos)
                    break
                list_idx += 1
            else:
                # Extruder may have completed moving so just use the commanded
                # position
                extruder_pos = self.extruder.stepper.get_commanded_position()
            if list_idx:
                # Remove out of date entries from list
                del self._extruder_move_list[:list_idx]
        return extruder_pos
    def _process_runout(self, eventtime):
        # TL;DR, set pause_on_runout to True.
        # If a runout occurs, the printer will continue printing so long as
        # moves are still queued. If pause_on_runout is True then it is
        # assumed the runout_gcode includes moves so will only run it once.
        # However if pause_on_runout is not set then runout_gcode can still
        # be used however it will be executed repeatedly if a long move is
        # queued, so in this case do not put moves in the runout_gcode.
        pause_prefix = ""
        if self._runout_eventtime is None:
            self._runout_eventtime = eventtime
        if self.runout_pause:
            if self.pause_resume.pause_command_sent:
                self._update_filament_runout_pos(self._runout_eventtime)
                self._runout_eventtime = None
                return
            # Pausing from inside an event requires that the pause portion
            # of pause_resume execute immediately.
            self.pause_resume.send_pause_command()
            pause_prefix = "PAUSE\n"
            self.toolhead.wait_moves()
            self.reactor.pause(eventtime + self.pause_delay)
        self._exec_gcode(pause_prefix, self.runout_gcode)
        self._update_filament_runout_pos(self._runout_eventtime)
        self._runout_eventtime = None
    def _runout_event_handler(self, eventtime):
        # Return from this handler as quickly as possible to return NEVER
        # And prevent duplicate runouts being triggered
        self._runout_eventtime = eventtime
        self.reactor.register_callback(self._process_runout)
        return self.reactor.NEVER
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
            # do not process during the initialization time, duplicates,
            # during the event delay time, while an event is running, or
            # when the sensor is disabled
            return
        # Perform filament action associated with status change (if any)
        is_printing = (
                self.idle_timeout.get_status(eventtime)["state"] == "Printing")
        if is_printing:
            self._insert_processed = False
            self._update_filament_runout_pos(eventtime)
        else:
            # Not printing so filament must have been inserted
            if self.insert_gcode is not None:
                # Due to this sensor constantly triggering during insert
                # only trigger the insert event once
                # TODO: May remove insert event processing if it becomes too
                # troublesome or adds no value
                if not self._insert_processed:
                    # insert detected
                    #self.min_event_systime = self.reactor.NEVER
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
