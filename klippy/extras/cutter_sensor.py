# Generic Filament Sensor Module
#
# Copyright (C) 2019  Eric Callahan <arksine.code@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
#
#########################################################################
# Cutter Sensor
#
# Copyright (C) 2025 Hugo Costa <https://github.com/HugoCLSC>
#
# This file may be distributed under the terms of the GNU GPLv3 license.

import logging
import typing
from functools import partial


class CutterSensorError(Exception):
    """Raised when an error occurs when the cutter sensor is used"""

    def __init__(self, message, errors=None):
        super(CutterSensorError, self).__init__(message)
        self.errors = errors
        logging.error(msg=message)


class CutterSensor:
    def __init__(self, config):
        self.name = config.get_name().split()[-1]
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object("gcode")
        self.toolhead = None
        self.printer.register_event_handler(
            "klippy:connect", self._handle_connect
        )
        self.printer.register_event_handler("klippy:ready", self._handle_ready)

        self.min_event_systime = self.reactor.NEVER
        self.filament_present = False
        self.sensor_enabled = True
        self.is_cutting: bool = False
        self.prev_pos = None
        self._extrude_timeout = 30
        self._timeout = 0

        self.pause_delay = config.getfloat("pause_delay", 0.5, above=0.0)
        self.event_delay = config.getfloat("event_delay", 3.0, above=0.0)
        self.runout_pause = config.getboolean("pause_on_runout", False)

        self.extrude_length_mm = config.getfloat(
            "extrude_length_mm", 5.0, minval=1.0, maxval=100.0
        )
        self.retract_length_mm = config.getfloat(
            "retract_length_mm", -5.0, minval=-30.0, maxval=-0.5
        )
        self.retract_to_cutter_sensor = config.getfloat(
            "retract_to_sensor_mm", -10.0, minval=-50.0, maxval=-0.5
        )
        self.extrude_speed = config.getfloat(
            "extrude_speed", 2.0, above=0.0, minval=1.0, maxval=50.0
        )
        self.travel_speed = config.getfloat(
            "travel_speed", 100.0, above=0.0, minval=30.0, maxval=600.0
        )
        self.cut_speed = config.getfloat(
            "cut_speed", 100.0, above=50.0, minval=50.0, maxval=300.0
        )
        self.cutter_position = config.getfloatlist(
            "cutter_position_xy", count=2
        )
        self.pre_cutter_position = config.getfloatlist(
            "pre_cutter_position_xy", count=2
        )
        self.bucked_position_xy = config.getfloatlist(
            "bucket_position_xy", default=None, count=2
        )

        self.runout_gcode = self.insert_gcode = None
        gcode_macro = self.printer.load_object(config, "gcode_macro")

        if config.get("runout_gcode", None) is not None:
            self.runout_gcode = gcode_macro.load_template(
                config, "runout_gcode", ""
            )
        if config.get("insert_gcode", None) is not None:
            self.insert_gcode = gcode_macro.load_template(
                config, "insert_gcode"
            )

        if self.bucked_position_xy:
            self.bucked_position_x, self.bucked_position_y = (
                self.bucked_position_xy
            )

        self.cutter_position_x, self.cutter_position_y = self.cutter_position
        self.pre_cutter_position_x, self.pre_cutter_position_y = (
            self.pre_cutter_position
        )

        cutter_sensor_pin = config.get("cutter_sensor_pin")
        buttons = self.printer.load_object(config, "buttons")
        buttons.register_debounce_button(
            cutter_sensor_pin, self._cutter_sensor_callback, config
        )

        self.unextrude_to_sensor_timer = self.reactor.register_timer(
            self.unextrude, self.reactor.NEVER
        )
        self.gcode.register_mux_command(
            "CUT", "SENSOR", self.name, self.cmd_CUT, self.cmd_CUT_helper
        )
        self.gcode.register_mux_command(
            "QUERY_FILAMENT_SENSOR",
            "SENSOR",
            self.name,
            self.cmd_QUERY_FILAMENT_SENSOR,
            self.cmd_QUERY_FILAMENT_SENSOR_helper,
        )
        self.gcode.register_mux_command(
            "SET_FILAMENT_SENSOR",
            "SENSOR",
            self.name,
            self.cmd_SET_FILAMENT_SENSOR,
            self.cmd_SET_FILAMENT_SENSOR_helper,
        )

    def _handle_connect(self) -> None:
        self.toolhead = self.printer.lookup_object("toolhead")

    def _handle_ready(self) -> None:
        self.min_event_systime = self.reactor.monotonic() + 2.0
        self.custom_boundary_object = self.printer.lookup_object(
            "bed_custom_bound", default=None
        )
        self.load_filament_object = self.printer.lookup_object(
            "load_filament load_toolhead", default=None
        )
        self.unload_filament_object = self.printer.lookup_object(
            "unload_filament", default=None
        )

    cmd_QUERY_FILAMENT_SENSOR_helper = "Query cutter sensor status"

    def cmd_QUERY_FILAMENT_SENSOR(self, gcmd) -> None:
        if self.filament_present:
            msg = f"[CUTTER {self.name} sensor] Filament Detected"
        else:
            msg = f"[CUTTER {self.name} sensor] Filament not detected"
        gcmd.respond_info(msg)

    cmd_SET_FILAMENT_SENSOR_helper = "Query the status of the cutter sensor"

    def cmd_SET_FILAMENT_SENSOR(self, gcmd) -> None:
        self.sensor_enabled = gcmd.get_int("ENABLE", 1)

    cmd_CUT_helper = "Routine that handles a cutter on the printer toolhead"

    def cmd_CUT(self, gcmd) -> None:
        """Gcode command for the Cutter module

        Call CUT gcode command to perform the filament cutting
        """
        return_last_position = gcmd.get(
            "MOVE_TO_LAST_POS", default=False, parser=bool
        )
        turn_off_heaters = gcmd.get(
            "TURN_OFF_HEATER", default=False, parser=bool
        )
        temperature = gcmd.get(
            "TEMPERATURE", default=220.0, parser=float, minval=200, maxval=250
        )

        self.reactor.register_callback(
            partial(
                self.cut,
                return_last_pos=return_last_position,
                off_heaters=turn_off_heaters,
                temp=temperature,
            )
        )

    def cut(
        self,
        eventtime=None,
        return_last_pos: bool = False,
        off_heaters: bool = False,
        temp: int = 220,
    ) -> None:
        """Perform cut"""
        if not self.toolhead:
            return

        if self.is_cutting:
            self.gcode.respond_info(
                f"[CUTTER {self.name} sensor] Already cutting filament"
            )
            return

        self.is_cutting = True
        self.home_needed()
        self.toolhead.wait_moves()
        eventtime = self.reactor.monotonic()
        kin_status = self.toolhead.get_kinematics().get_status(eventtime)

        if "xyz" not in kin_status["homed_axes"]:
            self.gcode.respond_info(
                f"[CUTTER {self.name} sensor] Printer needs to be homed for filament cutting.",
                log=True,
            )
            return
        self.prev_pos = self.toolhead.get_position()
        self.gcode.run_script_from_command("G90\nM400")
        self.gcode.run_script_from_command("M83\nM400")
        self.gcode.run_script_from_command("G92 E0.0\nM400")
        self.gcode.respond_info(
            f"[CUTTER {self.name} sensor ] Heating extruder."
        )
        self.heat_extruder(temp, wait=True)
        if self.bucked_position_xy:
            self.move_to_bucket()
        self.move_extruder_mm(distance=10, speed=self.extrude_speed)
        self.move_extruder_mm(
            distance=self.retract_length_mm, speed=self.extrude_speed
        )
        if self.custom_boundary_object:
            self.custom_boundary_object.restore_default_boundary()
        self.move_to_cutter_pos()
        self.cut_move()
        if self.bucked_position_xy:
            self.move_to_bucket()
        self.move_extruder_mm(
            distance=-2.0, speed=self.extrude_speed
        )  # Relief pressure on the blade
        self.move_extruder_mm(
            distance=self.extrude_length_mm + 10, speed=self.extrude_speed
        )
        self.toolhead.wait_moves()
        self.reactor.update_timer(
            self.unextrude_to_sensor_timer, self.reactor.NOW
        )

        if self.prev_pos and return_last_pos:
            self.move_back()
            self.toolhead.wait_moves()
            if self.custom_boundary_object:
                self.custom_boundary_object.set_custom_boundary()
        if off_heaters:
            self.heat_extruder(0, wait=False)
        return

    def unextrude(self, eventtime):
        """Unextrude"""
        if not self.is_cutting:
            return self.reactor.NEVER
        try:
            self.move_extruder_mm(
                distance=-10, speed=self.extrude_speed, wait=False
            )
            logging.info("[CUTTER] unextruding")
            return float(eventtime + float(10 / self.extrude_speed))
        except Exception as e:
            raise CutterSensorError(
                f"[CUTTER {self.name} sensor] Error Unextruding: {e}"
            )

    def move_extruder_mm(self, distance=10.0, speed=10, wait=True) -> None:
        """Move the extruder.
        Args:
            dist (float in mm): The distance in a certain amount.
        """
        if not self.toolhead:
            return
        try:
            eventtime = self.reactor.monotonic()
            gcode_move = self.printer.lookup_object("gcode_move")
            prev_pos = self.toolhead.get_position()
            v = distance * gcode_move.get_status(eventtime)["extrude_factor"]
            new_distance = v + prev_pos[3]
            self.toolhead.move(
                [prev_pos[0], prev_pos[1], prev_pos[2], new_distance], speed
            )
            if wait:
                self.toolhead.wait_moves()
        except Exception as e:
            raise CutterSensorError(
                f"[CUTTER {self.name} sensor] Unable to move extruder error: {e}."
            )

    def home_needed(self) -> None:
        if not self.toolhead:
            raise CutterSensorError(
                "Toolhead object is missing, called on home_needed."
            )
        try:
            eventtime = self.reactor.monotonic()

            kin = self.toolhead.get_kinematics()
            _homed_axes = kin.get_status(eventtime)["homed_axes"]

            if "xyz" in _homed_axes.lower():
                self.gcode.respond_info("Printer already homed.")
                return
            else:
                self.gcode.respond_info("Homing.")
                self.gcode.run_script_from_command("G28\nM400")

        except Exception as e:
            raise CutterSensorError(f"Unable to home: {e}")

    def heat_extruder(
        self, temp, wait: typing.Optional["bool"] = True
    ) -> None:
        """Heats the extruder and wait.

        Method returns when  temperature is [temp - 5 ; temp + 5].
        Args:
            temp (float):
                Target temperature in Celsius.
            wait (bool, optional):
                Weather to wait or not for the temperature to reach the interval . Defaults to True
        """
        try:
            if self.toolhead is None:
                return
            eventtime = self.reactor.monotonic()
            extruder = self.toolhead.get_extruder()
            pheaters = self.printer.lookup_object("heaters")
            pheaters.set_temperature(extruder.get_heater(), temp, False)
            extruder_heater = extruder.get_heater()

            while not (self.printer.is_shutdown() and wait):
                self.gcode.respond_info(
                    "Waiting for temperature to stabilize."
                )
                heater_temp, target = extruder_heater.get_temp(eventtime)
                if heater_temp >= (temp - 5) and heater_temp <= (temp + 5):
                    return
                eventtime = self.reactor.pause(eventtime + 1.0)
        except Exception as e:
            raise CutterSensorError(f"Error heating extruder: {e}")

    def cut_move(self) -> None:
        """Performs the cut movement"""
        try:
            if not self.toolhead:
                return
            self.toolhead.manual_move(
                [self.cutter_position_x, self.cutter_position_y],
                self.travel_speed,
            )
            self.toolhead.manual_move(
                [self.pre_cutter_position_x, self.pre_cutter_position_y],
                self.cut_speed,
            )
            self.toolhead.wait_moves()
        except Exception as e:
            raise CutterSensorError(
                f"Error performing performing cut move: {e}."
            )

    def move_to_cutter_pos(self) -> None:
        """Moves the toolhead to the pre cutting position"""
        try:
            if not self.toolhead:
                return
            curtime = self.reactor.monotonic()
            kin_status = self.toolhead.get_kinematics().get_status(curtime)

            if "xyz" not in kin_status["homed_axes"]:
                return
            self.toolhead.manual_move(
                [self.pre_cutter_position_x, self.pre_cutter_position_y],
                self.travel_speed,
            )
            self.toolhead.wait_moves()
        except Exception as e:
            raise CutterSensorError(f"Error moving to cutter position: {e}.")

    def move_home(self) -> None:
        try:
            if not self.toolhead:
                return
            """Moves to the homing position"""
            gcode_move = self.printer.lookup_object("gcode_move")
            homing_origin = gcode_move.get_status()["homing_origin"]
            self.toolhead.manual_move(homing_origin, self.travel_speed)
        except Exception as e:
            raise CutterSensorError(f"Error moving to home position: {e}.")

    def move_to_bucket(self, split=False) -> None:
        """Moves to the bucket position"""
        try:
            if not self.toolhead:
                return
            if not self.custom_boundary_object:
                self.gcode.respond_info(
                    "Restoring original printer Boundaries."
                )
                self.custom_boundary_object.restore_default_boundary()

            if not split:
                self.toolhead.manual_move(
                    [self.bucked_position_x, self.bucked_position_y],
                    self.travel_speed,
                )
            else:
                self.toolhead.manual_move(
                    [self.bucked_position_x], self.travel_speed
                )
                self.toolhead.wait_moves()
                self.toolhead.manual_move(
                    [self.bucked_position_y], self.travel_speed
                )

            self.toolhead.wait_moves()
        except Exception as e:
            raise CutterSensorError(f"Error moving to bucket position: {e}.")

    def move_back(self):
        """Moves back to the original position where the CUT gcode command was called"""
        try:
            if not self.prev_pos or not self.toolhead:
                return

            self.toolhead.manual_move(
                [self.prev_pos[0], self.prev_pos[1], self.prev_pos[2]],
                self.travel_speed,
            )
            self.toolhead.wait_moves()
        except Exception as e:
            raise CutterSensorError(
                f"Error moving to the original position: {e}."
            )

    def _cutter_sensor_callback(self, eventtime, state):
        """Callback for the sensor state change"""
        if self.filament_present == state:
            return
        self.filament_present = state

        logging.info(
            f"[CUTTER {self.name} Sensor] state changed to {str(state)}"
        )

        eventtime = self.reactor.monotonic()
        if (
            eventtime < self.min_event_systime
        ):  # Can also add self.enabled, if not enabled does nothing
            return

        idle_timeout = self.printer.lookup_object("idle_timeout")
        is_printing = (
            idle_timeout.get_status(eventtime)["state"] == "Printing"
        )  # state can be either ["Ready", "Printing", "Idle"]

        if (
            self.load_filament_object
            and self.load_filament_object.get_status(eventtime).get(
                "state", False
            )
        ) or (
            self.unload_filament_object
            and self.unload_filament_object.get_status(eventtime).get(
                "state", False
            )
        ):
            if state:
                self.printer.send_event("cutter_sensor:filament_present")
                self.gcode.respond_info(
                    f"[CUTTER {self.name}] detected filament while loading filament"
                )
                logging.info(
                    f"[CUTTER {self.name}] detected filament while loading filament"
                )
            else:
                if self.is_cutting:
                    self.reactor.update_timer(
                        self.unextrude_to_sensor_timer, self.reactor.NEVER
                    )
                    self.is_cutting = False
                    self.gcode.run_script_from_command("G90\nM400")
                    self.gcode.run_script_from_command("M83\nM400")
                    self.gcode.run_script_from_command("G92 E0.0\nM400")
                    self.gcode.respond_info(
                        f"[CUTTER {self.name} sensor] Cut done."
                    )
                self.printer.send_event("cutter_sensor:no_filament")
            self.min_event_systime = self.reactor.monotonic() + 2.0

        elif state:
            if not is_printing and self.insert_gcode is not None:
                self.printer.send_event("cutter_sensor:filament_present")
                # self.min_event_systime = self.reactor.NEVER
                self.min_event_systime = self.reactor.monotonic() + 2.0
                logging.info(
                    f"[CUTTER {self.name} sensor] Filament Detected | Time: {eventtime}"
                )
                # self.reactor.register_callback(self._insert_event_handler)

        elif not is_printing and self.runout_gcode is not None:
            # runout detected

            # self.min_event_systime = self.reactor.NEVER
            self.min_event_systime = self.reactor.monotonic() + 2.0
            # if self.is_cutting:
            #     self.reactor.update_timer(
            #         self.unextrude_to_sensor_timer, self.reactor.NEVER
            #     )
            #     self.is_cutting = False
            self.printer.send_event("cutter_sensor:no_filament")
            logging.info(
                f"[CUTTER {self.name} sensor] Filament Not Detected | Time: {eventtime}"
            )
            self.gcode.run_script_from_command("G90\nM400")
            self.gcode.run_script_from_command("M83\nM400")
            self.gcode.run_script_from_command("G92 E0.0\nM400")
        elif (
            is_printing and self.runout_gcode is not None
        ):  # Runout detected while printing
            # self.min_event_systime = self.reactor.NEVER
            self.min_event_systime = self.reactor.monotonic() + 2.0

            self.printer.send_event("cutter_sensor:no_filament")
            logging.info(
                f"[CUTTER {self.name} sensor] Filament Not Detected | Time: {eventtime}"
            )
            # self.reactor.register_callback(self._runout_event_handler)

        self.gcode.run_script_from_command("G90\nM400")
        self.gcode.run_script_from_command("M83\nM400")
        self.gcode.run_script_from_command("G92 E0.0\nM400")
        return

    def _insert_event_handler(self, eventtime):
        self._exec_gcode("", self.insert_gcode)

    def _runout_event_handler(self, eventtime):
        pause_prefix = ""
        if self.runout_pause:
            pause_resume = self.printer.lookup_object("pause_resume")
            pause_resume.send_pause_command()
            pause_prefix = "PAUSE\n"
            self.printer.get_reactor().pause(eventtime + self.pause_delay)
        self._exec_gcode(pause_prefix, self.runout_gcode)

    def _exec_gcode(self, prefix, template):
        """Internal Executes a gcode just like what's in the klipper filament_switch_sensor.py"""

        try:
            self.gcode.run_script(prefix + template.render() + "\nM400")
        except Exception:
            logging.exception("Script running error")
        self.min_event_systime = self.reactor.monotonic() + self.event_delay

    def get_status(self, eventtime):
        """Gets the status of the sensor of the cutter"""
        return {
            "filament_detected": bool(self.filament_present),
            "enabled": bool(self.sensor_enabled),
        }


def load_config_prefix(config):
    return CutterSensor(config)
