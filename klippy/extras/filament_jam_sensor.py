# Generic Filament Jam Sensor Module
#
# Copyright (C) 2019   Antonio Cheong <windo.ac@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging


class JamSensor:
    def __init__(self, config):
        self.mname = config.get_name().split()[0]
        self.name = config.get_name().split()[1]
        self.printer = config.get_printer()
        self.gcode = self.printer.lookup_object("gcode")
        self.reactor = self.printer.get_reactor()
        self.buttons = self.printer.try_load_module(
            config, "buttons"
        )
        self.buttons.register_buttons(
            [config.get("pin")], self._signal_handler
        )
        self.timer = config.getfloat(
            "timer", 1.0, above=0.0
        )
        self.extruder_index = int(
            config.getint("extruder_index", 0)
        )
        self.base_usage = config.getfloat(
            "base_usage", None, above=0.0
        )
        self.slow_usage = (
            config.getfloat("slow_usage", 0.0) / 100.0
        )
        self.slow_speed = config.getfloat(
            "slow_speed", 10.0, above=0.0
        ) / (60.0 * 100.0)
        self.slow_gcode = config.get("slow_gcode", None)
        self.jam_usage = (
            config.getfloat("jam_usage", 400.0, above=100.0)
            / 100.0
        )
        self.jam_gcode = config.get("jam_gcode", None)
        self.slow_speed_resume = config.getfloat(
            "slow_speed_resume", 1.0, above=0.0
        ) / (
            60.0 * 100.0
        )  # Zore mean not resume
        self.filament_usage_last = 0.0
        self.timer_usage_last = 0.0
        self.toolhead = None
        self.pause_resume = None
        self.enable = False
        self.debug = False
        self.action = True
        self.slow_triggered = False
        self.jam_triggered = False
        self.slowest_speed = 0.5 / 60.0
        self.full_speed = 1.0 / 60.0
        self.cmd_SET_FILAMENT_JAM_SENSOR_help = (
            "Set param of FILAMENT_JAM_SENSOR"
        )
        self.gcode.register_mux_command(
            "SET_FILAMENT_JAM_SENSOR",
            "SENSOR",
            self.name,
            self.cmd_SET_FILAMENT_JAM_SENSOR,
            desc=self.cmd_SET_FILAMENT_JAM_SENSOR_help,
        )
        self.printer.register_event_handler(
            "klippy:ready", self._handle_ready
        )

    def _handle_ready(self):
        self.toolhead = self.printer.lookup_object(
            "toolhead"
        )
        if self.base_usage is None:
            logging.exception("base_usage must defined")
            self.enable = False
        elif (
            self.slow_usage > 0.0
            and self.slow_usage >= self.jam_usage
        ):
            logging.exception("must slow_usage < jam_usage")
            self.enable = False
        else:
            self.filament_usage_last = (
                self.get_filament_usage()
            )
            self.pause_resume = self.printer.lookup_object(
                "pause_resume", None
            )
            if self.pause_resume is None:
                logging.exception(
                    "pause_resume must define"
                )
                self.enable = False
            else:
                self.enable = True
                self.reactor.register_timer(
                    self._timer_handler, self.reactor.NOW
                )
                logging.info(
                    "%s(%s): ready to use",
                    self.mname,
                    self.name,
                )

    def _signal_handler(self, eventtime, state):
        if self.enable and state == 1:
            new_usage = self.get_filament_usage()
            delta_usage = (
                new_usage - self.filament_usage_last
            )
            self.filament_usage_last = new_usage
            if self.debug:
                self.gcode.respond_info(
                    "%s(%s): %.2f %% ( %.2f / %.2f )"
                    % (
                        self.mname,
                        self.name,
                        delta_usage
                        / self.base_usage
                        * 100.0,
                        delta_usage,
                        self.base_usage,
                    )
                )
                logging.debug(
                    "%s(%s): _signal_handler triggered | delta_usage = %s",
                    self.mname,
                    self.name,
                    delta_usage,
                )
            if (
                delta_usage > 0
                and self.check_jam(eventtime, delta_usage)
                and self.slow_usage > 0.0
                and self.gcode.speed_factor
                < self.full_speed
            ):  # won't run check_jam if delta_usage<0
                temp_speed = (
                    self.gcode.speed_factor
                    + self.slow_speed_resume
                )
                self.gcode.speed_factor = (
                    temp_speed
                    if temp_speed < self.full_speed
                    else self.full_speed
                )
                self.gcode.respond_info(
                    "speed_factor:%d"
                    % (
                        self.gcode.speed_factor
                        * 60.0
                        * 100.0
                    )
                )
            self.slow_triggered = False
            self.jam_triggered = False

    def _timer_handler(self, eventtime):
        if self.enable:
            new_usage = self.get_filament_usage()
            delta_usage = (
                new_usage - self.filament_usage_last
            )
            if self.debug:
                logging.debug(
                    "%s(%s): _timer_handler triggered | delta_usage = %s",
                    self.mname,
                    self.name,
                    delta_usage,
                )
            if (
                delta_usage > 0
                and self.timer_usage_last != new_usage
            ):  # ignore when it's not move
                self.check_jam(eventtime, delta_usage)
            self.timer_usage_last = new_usage
        return eventtime + self.timer

    def check_jam(self, eventtime, delta_usage):
        theresult = False
        temp_rate = delta_usage / self.base_usage
        if temp_rate > self.jam_usage:
            # detect jam
            if not self.jam_triggered:
                if self.debug:
                    self.gcode.respond_info(
                        "%s(%s): detect jam | %.2f %% ( %.2f / %.2f )"
                        % (
                            self.mname,
                            self.name,
                            delta_usage
                            / self.base_usage
                            * 100.0,
                            delta_usage,
                            self.base_usage,
                        )
                    )
                self.gcode.respond_info(
                    "%s(%s): detect jam"
                    % (self.mname, self.name)
                )
                if self.action:
                    # self.pause_resume.send_pause_command()
                    self._exec_gcode("PAUSE")
                    if self.jam_gcode:
                        self._exec_gcode(self.jam_gcode)
            self.jam_triggered = True
        elif (
            self.slow_usage > 0.0
            and temp_rate > self.slow_usage
        ):
            # detect slow
            if not self.slow_triggered:
                if self.debug:
                    self.gcode.respond_info(
                        "%s(%s): detect slow | %.2f %% ( %.2f / %.2f )"
                        % (
                            self.mname,
                            self.name,
                            delta_usage
                            / self.base_usage
                            * 100.0,
                            delta_usage,
                            self.base_usage,
                        )
                    )
                self.gcode.respond_info(
                    "%s(%s): detect slow"
                    % (self.mname, self.name)
                )
                if self.action:
                    if (
                        self.gcode.speed_factor
                        > self.slowest_speed
                    ):
                        temp_speed = (
                            self.gcode.speed_factor
                            - self.slow_speed
                        )
                        self.gcode.speed_factor = (
                            temp_speed
                            if temp_speed
                            > self.slowest_speed
                            else self.slowest_speed
                        )
                        self.gcode.respond_info(
                            "speed_factor:%d"
                            % (
                                self.gcode.speed_factor
                                * 60.0
                                * 100.0
                            )
                        )
                    if self.slow_gcode:
                        self._exec_gcode(self.slow_gcode)
            self.slow_triggered = True
        elif temp_rate > 0.25 and temp_rate < (
            0.5 + self.slow_usage / 2.0
        ):
            # normal
            theresult = True
        return theresult

    def get_filament_usage(self):
        theout = 0.0
        if self.toolhead:
            theout = self.toolhead.get_position()[
                3 + self.extruder_index
            ]
            # theout = self.gcode.base_position[3+self.extruder_index]
        return float(theout)

    def _exec_gcode(self, script):
        try:
            self.gcode.run_script(script)
        except Exception:
            logging.exception("Script running error")

    def cmd_SET_FILAMENT_JAM_SENSOR(self, params):
        self.timer = self.gcode.get_float(
            "TIMER", params, self.timer, minval=0.0
        )
        self.base_usage = self.gcode.get_float(
            "BASE_USAGE",
            params,
            self.base_usage,
            minval=0.0,
        )
        self.slow_usage = (
            self.gcode.get_float(
                "SLOW_USAGE",
                params,
                self.slow_usage * 100.0,
            )
            / 100.0
        )
        temp_jam_usage = (
            self.gcode.get_float(
                "JAM_USAGE",
                params,
                self.jam_usage * 100.0,
                minval=200.0,
            )
            / 100.0
        )
        if (
            temp_jam_usage is not None
            and temp_jam_usage > self.slow_usage
        ):
            self.jam_usage = temp_jam_usage
        self.slow_speed_resume = self.gcode.get_float(
            "SLOW_SPEED_RESUME",
            params,
            self.slow_speed_resume * 100.0 * 60.0,
            minval=0.0,
        ) / (60.0 * 100.0)
        self.slow_speed = self.gcode.get_float(
            "SLOW_SPEED",
            params,
            self.slow_speed * 100.0 * 60.0,
            minval=0.0,
        ) / (60.0 * 100.0)
        self.enable = (
            True
            if self.gcode.get_int(
                "ENABLE", params, 1 if self.enable else 0
            )
            == 1
            else False
        )
        self.debug = (
            True
            if self.gcode.get_int(
                "DEBUG", params, 1 if self.debug else 0
            )
            == 1
            else False
        )
        self.action = (
            True
            if self.gcode.get_int(
                "ACTION", params, 1 if self.action else 0
            )
            == 1
            else False
        )
        self.gcode.respond_info(
            "%s(%s): timer = %.2f | base_usage = %.2f | slow_usage = %.2f | "
            "jam_usage = %.2f | slow_speed = %.2f | slow_speed_resume = %.2f "
            "| enable = %s | debug = %s | action = %s"
            % (
                self.mname,
                self.name,
                self.timer,
                self.base_usage,
                self.slow_usage * 100.0,
                self.jam_usage * 100.0,
                self.slow_speed * 100.0 * 60.0,
                self.slow_speed_resume * 100.0 * 60.0,
                self.enable,
                self.debug,
                self.action,
            )
        )
        # reset some counter
        self.timer_usage_last = (
            self.filament_usage_last
        ) = self.get_filament_usage()


def load_config_prefix(config):
    return JamSensor(config)
