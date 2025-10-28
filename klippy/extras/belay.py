# Belay extruder-syncing sensor support
#
# Copyright (C) 2023-2024 Ryan Ghosh <rghosh776@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

DIRECTION_UPDATE_INTERVAL = 0.1
POSITION_TIME_DIFF = 0.3


class Belay:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()

        # initial type-specific setup
        type_options = ["trad_rack", "extruder_stepper"]
        self.type = config.getchoice(
            "extruder_type", {t: t for t in type_options}
        )

        self.stuck_timeout = config.getfloat("stuck_timeout", 5.0, above=1)
        self.curr_stuck_timer = 0.0
        self.printer.register_event_handler(
            "idle_timeout:printing", self.handle_printing
        )

        if self.type == "trad_rack":
            enable_events = ["trad_rack:synced_to_extruder"]
            disable_events = ["trad_rack:unsyncing_from_extruder"]
            self.enable_initial = False
        elif self.type == "extruder_stepper":
            self.extruder_stepper_name = config.get("extruder_stepper_name")
            enable_events = []
            disable_events = []
            self.enable_initial = True

        # register event handlers
        self.printer.register_event_handler(
            "klippy:connect", self.handle_connect
        )
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        for event in enable_events:
            self.printer.register_event_handler(event, self.handle_enable)
        for event in disable_events:
            self.printer.register_event_handler(event, self.handle_disable)

        # register button
        sensor_pin = config.get("sensor_pin")
        buttons = self.printer.load_object(config, "buttons")
        buttons.register_buttons([sensor_pin], self.sensor_callback)

        # read other values
        self.multiplier_high = config.getfloat(
            "multiplier_high", default=1.05, minval=1.0
        )
        self.multiplier_low = config.getfloat(
            "multiplier_low", default=0.95, minval=0.0, maxval=1.0
        )
        self.debug_level = config.getint(
            "debug_level", default=0.0, minval=0.0, maxval=2.0
        )

        # other variables
        self.name = config.get_name().split()[1]
        self.enabled = False
        self.last_state = False
        self.last_direction = True
        self.set_multiplier = None
        self.enable_conditions = [lambda: not self.enabled]
        self.disable_conditions = [lambda: self.enabled]
        self.gcode = self.printer.lookup_object("gcode")
        self.toolhead = None
        self.update_direction_timer = self.reactor.register_timer(
            self.update_direction
        )

        self.update_stuck_timer = self.reactor.register_timer(
            self.update_verify_stuck
        )
        # register commands
        self.gcode.register_mux_command(
            "QUERY_BELAY",
            "BELAY",
            self.name,
            self.cmd_QUERY_BELAY,
            desc=self.cmd_QUERY_BELAY_help,
        )
        self.gcode.register_mux_command(
            "BELAY_SET_MULTIPLIER",
            "BELAY",
            self.name,
            self.cmd_BELAY_SET_MULTIPLIER,
            desc=self.cmd_BELAY_SET_MULTIPLIER_help,
        )

        # self.gcode.register_mux_command(
        #     "QUERY_FILAMENT_TRACK_STATUS",
        #     "BELAY",
        #     self.name,
        #     self.cmd_QUERY_FILAMENT_TRACK_STATUS,
        #     desc=self.cmd_QUERY_FILAMENT_TRACK_STATUS_help,
        # )

        # self.gcode.register_mux_command(
        #     "SET_BELAY_FILAMENT_TRACK",
        #     "BELAY",
        #     self.name,
        #     self.cmd_SET_BELAY_FILAMENT_TRACK,
        #     desc=self.cmd_SET_BELAY_FILAMENT_TRACK_help,
        # )

        # register extruder_stepper-only commands
        if self.type == "extruder_stepper":
            self.gcode.register_mux_command(
                "BELAY_SET_STEPPER",
                "BELAY",
                self.name,
                self.cmd_BELAY_SET_STEPPER,
                desc=self.cmd_BELAY_SET_STEPPER_help,
            )

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")

        # finish type-specific setup
        if self.type == "trad_rack":
            trad_rack = self.printer.lookup_object("trad_rack")
            self.set_multiplier = trad_rack.set_fil_driver_multiplier
            self.enable_conditions.append(trad_rack.is_fil_driver_synced)
            self.disable_conditions.append(trad_rack.is_fil_driver_synced)
        elif self.type == "extruder_stepper":
            self._set_extruder_stepper(self.extruder_stepper_name)

    def _set_extruder_stepper(self, extruder_stepper_name):
        printer_extruder_stepper = self.printer.lookup_object(
            "extruder_stepper {}".format(extruder_stepper_name)
        )
        stepper = printer_extruder_stepper.extruder_stepper.stepper
        base_rotation_dist = stepper.get_rotation_distance()[0]
        self.set_multiplier = lambda m: stepper.set_rotation_distance(
            base_rotation_dist / m
        )

    def handle_ready(self):
        if self.enable_initial:
            self.handle_enable()

    def handle_enable(self):
        for condition in self.enable_conditions:
            if not condition():
                return
        self.enabled = True
        self.reactor.update_timer(
            self.update_direction_timer, self.reactor.NOW
        )
        self.update_multiplier()

    def handle_disable(self):
        for condition in self.disable_conditions:
            if not condition():
                return
        self.reset_multiplier()
        self.reactor.update_timer(
            self.update_direction_timer, self.reactor.NEVER
        )
        self.enabled = False

    def sensor_callback(self, eventtime, state):
        self.last_state = state
        if self.enabled:
            virtual_sdcard = self.printer.lookup_object("virtual_sdcard")
            file_pos = virtual_sdcard.get_status(eventtime).get(
                "file_position"
            )
            if file_pos > 1:
                self.timeout = self.reactor.monotonic() + self.stuck_timeout
                if self.debug_level >= 1:
                    self.gcode.respond_info(
                        "Belay Pressure changed, increasing stuck timeout"
                    )
            self.update_multiplier()

    def update_multiplier(self, print_msg=True):
        if self.last_state == self.last_direction:
            # compressed/forward or expanded/backward
            multiplier = self.multiplier_high
        else:
            # compressed/backward or expanded/forward
            multiplier = self.multiplier_low

        self.set_multiplier(multiplier)

        if (print_msg and self.debug_level >= 1) or self.debug_level >= 2:
            self.gcode.respond_info(
                "Set secondary extruder multiplier: %f" % multiplier
            )

    def reset_multiplier(self):
        self.set_multiplier(1.0)
        if self.debug_level >= 1:
            self.gcode.respond_info("Reset secondary extruder multiplier")

    def update_direction(self, eventtime):
        mcu = self.printer.lookup_object("mcu")
        print_time = mcu.estimated_print_time(eventtime)
        extruder = self.toolhead.get_extruder()
        curr_pos = extruder.find_past_position(print_time)
        past_pos = extruder.find_past_position(
            max(0.0, print_time - POSITION_TIME_DIFF)
        )
        prev_direction = self.last_direction
        self.last_direction = curr_pos >= past_pos
        if self.last_direction != prev_direction:
            if self.debug_level >= 1:
                self.gcode.respond_info(
                    "New Belay sensor direction: %s" % self.last_direction
                )

            self.update_multiplier(False)

        return eventtime + DIRECTION_UPDATE_INTERVAL

    def handle_printing(self, eventtime) -> None:
        self.timeout = self.reactor.monotonic() + self.stuck_timeout
        self.reactor.update_timer(self.update_stuck_timer, self.reactor.NOW)

    def update_verify_stuck(self, eventtime) -> float:
        idle_timeout = self.printer.lookup_object("idle_timeout")
        state = idle_timeout.get_status(eventtime).get("state")
        virtual_sdcard = self.printer.lookup_object("virtual_sdcard")

        if state.lower() == "printing":
            if self.debug_level >= 1:
                self.gcode.respond_info(
                    f"Belay stuck with timeout {self.timeout} timer: {self.reactor.monotonic()}"
                )

            if self.timeout > self.reactor.monotonic():
                return self.reactor.monotonic() + 1.0

            self.gcode.respond_info("Belay reports filament Stuck.")
            self.gcode.run_script_from_command("PAUSE")
            return self.reactor.NEVER
        else:
            self.curr_stuck_timer = 0
            return self.reactor.NEVER

    cmd_QUERY_BELAY_help = "Report Belay sensor state"

    def cmd_QUERY_BELAY(self, gcmd):
        if self.last_state:
            state_info = "compressed"
        else:
            state_info = "expanded"
        self.gcode.respond_info("belay {}: {}".format(self.name, state_info))

    cmd_BELAY_SET_MULTIPLIER_help = (
        "Sets multiplier_high and/or multiplier_low. Does not persist across"
        " restarts."
    )

    def cmd_BELAY_SET_MULTIPLIER(self, gcmd):
        self.multiplier_high = gcmd.get_float(
            "HIGH", self.multiplier_high, minval=1.0
        )
        self.multiplier_low = gcmd.get_float(
            "LOW", self.multiplier_low, minval=0.0, maxval=1.0
        )

    cmd_BELAY_SET_STEPPER_help = (
        "Select the extruder_stepper object to be controlled by the Belay"
    )

    def cmd_BELAY_SET_STEPPER(self, gcmd):
        self.handle_disable()
        self._set_extruder_stepper(gcmd.get("STEPPER"))
        self.handle_enable()

    def cmd_QUERY_FILAMENT_TRACK_STATUS(self, gcmd) -> None: ...
    def cmd_SET_BELAY_FILAMENT_TRACK(self, gcmd) -> None: ...

    def get_status(self, eventtime):
        return {"last_state": self.last_state, "enabled": self.enabled}


def load_config_prefix(config):
    return Belay(config)
