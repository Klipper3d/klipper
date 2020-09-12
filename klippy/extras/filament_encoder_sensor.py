# filament Encoder Sensor Module by Mike Stromberg <Abom Klipper discord>
# Based off of FIlament_Switch_Sensor by Eric Callahan <arksine.code@gmail.com>
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging

class RunoutHelper:
    def __init__(self, config):
        self.name = config.get_name().split()[-1]
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object('gcode')
        self.last_button_event = self.reactor.monotonic()
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
        self.event_delay = config.getfloat('event_delay', 3., above=0.)
        self.sensor_enabled = 1
        self.runout = 0
        # Internal state
        self.min_event_systime = self.reactor.NEVER
        # Register commands and event handlers
        self.printer.register_event_handler("klippy:ready", self._handle_ready)
        self.gcode.register_mux_command(
            "QUERY_FILAMENT_ENCODER", "ENCODER", self.name,
            self.cmd_QUERY_FILAMENT_ENCODER,
            desc=self.cmd_QUERY_FILAMENT_ENCODER_help)
        self.gcode.register_mux_command(
            "SET_FILAMENT_ENCODER", "ENCODER", self.name,
            self.cmd_SET_FILAMENT_ENCODER,
            desc=self.cmd_SET_FILAMENT_ENCODER_help)
        self.gcode.register_mux_command(
            "SET_FILAMENT_ENCODER_DELAY", "ENCODER", self.name,
            self.cmd_SET_FILAMENT_ENCODER_DELAY,
            desc=self.cmd_SET_FILAMENT_ENCODER_DELAY_help)
        # kick start the sensor with a delay to allow everything to initialize
        self.reactor.register_callback(self.note_filament_encoder, 3)
    def _handle_ready(self):
        self.min_event_systime = self.reactor.monotonic() + 2.
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
            self.gcode.run_script(prefix + template.render())  # + "\nM400"
        except Exception:
            logging.exception("Script running error")
        self.min_event_systime = self.reactor.monotonic() + self.event_delay
    def note_filament_encoder(self, bogus):
        eventtime = self.reactor.monotonic()
        idle_timeout = self.printer.lookup_object("idle_timeout")
        is_printing = idle_timeout.get_status(eventtime)["state"] == "Printing"
        # do not process during the initialization time, duplicates,
        # during the event delay time, while an event is running, or
        # when the sensor is disabled
        if eventtime < self.min_event_systime or not self.sensor_enabled\
                or not is_printing:
            # while the printer is heating up or otherwise idle, the encoder
            # may be sitting at an "on" state, making the last_button_event
            # move farther and farther into the past, and causeing a false
            # runout when printing begins. set it to the current eventtime to
            # prevent this. when printing actually starts the button handler
            # will take over.
            self.last_button_event = eventtime
        else:
            # insert gcode should re-enable the filement sensor. This way user
            # can disable on reload if needed. Determine "printing" status
            # Perform filament action associated with status change (if any)
            if is_printing and self.last_button_event + \
                self.event_delay < eventtime:
                # runout detected
                if self.runout_gcode is not None:
                    self.runout = 1
                    self.min_event_systime = self.reactor.NEVER
                    logging.info(
                        "Filament Encoder %s: runout event detected" %
                        (self.name))
                    self.reactor.register_callback(self._runout_event_handler)
        self.reactor.register_callback(
            self.note_filament_encoder, eventtime + self.event_delay)
    def filament_inserted(self):
        eventtime = self.reactor.monotonic()
        if self.runout and self.insert_gcode is not None:
            self.runout = 0
            # insert detected
            self.min_event_systime = self.reactor.NEVER
            logging.info(
                "Filament Encoder %s: insert event detected, Time %.2f" %
                (self.name, eventtime))
            # not much to do with this for an encoder except to re-enable the
            # sensor in the macro.
            self.reactor.register_callback(self._insert_event_handler)
    cmd_QUERY_FILAMENT_ENCODER_help = "Query status of the Filament encoder"
    def cmd_QUERY_FILAMENT_ENCODER(self, gcmd):
        msg = "encoder enabled = %x " % (self.sensor_enabled)
        gcmd.respond_info(msg)
    cmd_SET_FILAMENT_ENCODER_help = "Sets the filament encoder on/off"
    def cmd_SET_FILAMENT_ENCODER(self, gcmd):
        self.sensor_enabled = gcmd.get_int("ENABLE", 1)
        # self.note_filament_encoder(1)
    cmd_SET_FILAMENT_ENCODER_DELAY_help = "Set how many seconds between checks"
    def cmd_SET_FILAMENT_ENCODER_DELAY(self, gcmd):
        self.event_delay = gcmd.get_float("DELAY", 9., above=0.)

class SwitchSensor:
    def __init__(self, config):
        printer = config.get_printer()
        buttons = printer.load_object(config, 'buttons')
        switch_pin = config.get('switch_pin')
        buttons.register_buttons([switch_pin], self._button_handler)
        self.runout_helper = RunoutHelper(config)
    def _button_handler(self, eventtime, state):
        self.runout_helper.last_button_event = eventtime
        # if runout is true, the printer was paused by the runout event handler
        # filament has been inserted thru the encoder now.
        if self.runout_helper.runout:
            self.runout_helper.filament_inserted()

def load_config_prefix(config):
    return SwitchSensor(config)
    