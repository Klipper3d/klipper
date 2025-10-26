import logging
import typing
from functools import partial


class UnloadFilamentError(Exception):
    """Raised when there is an error unloading filament"""

    def __init__(self, message, errors: typing.Optional[list] = None):
        super(UnloadFilamentError, self).__init__(message)
        self.errors = errors


class UnloadFilament:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object("gcode")
        self.custom_boundary_object = None
        self.min_event_systime = None
        self.bucket_object = None
        self.cutter_object = None
        self.filament_flow_sensor_object = self.filament_flow_sensor_name = (
            None
        )
        self.filament_switch_sensor_object = (
            self.filament_switch_sensor_name
        ) = None
        self.unload_started = False
        self.unextrude_count: int = 0
        self.travel_speed = None
        self.printer.register_event_handler(
            "klippy:connect", self.handle_connect
        )
        self.printer.register_event_handler("klippy:ready", self.handle_ready)
        self.printer.register_event_handler(
            "unload_filament:end",
            lambda: self.gcode.respond_info(
                "[UNLOAD FILAMENT] Unload Finished"
            ),
        )
        self.idex = config.getboolean("idex", False)
        self.has_custom_boundary = config.getboolean(
            "has_custom_boundary", False
        )
        self.travel_speed = config.getfloat(
            "travel_speed", 100.0, minval=50.0, maxval=500.0
        )
        self.bucket = config.getboolean("bucket", False)
        if not self.bucket:
            self.filament_flow_sensor_name = config.get(
                "filament_flow_sensor_name", None
            )
        self.filament_switch_sensor_name = config.get(
            "filament_switch_sensor_name", None
        )
        self.park = config.getfloatlist("park_xy", None, count=2)
        self.unload_speed = config.getfloat(
            "unload_speed", default=10.0, minval=2.0, maxval=200.0
        )
        self.cutter_name = config.get("cutter_sensor_name", None)
        self.timeout = config.getint(
            "timeout", default=None, minval=10, maxval=1000
        )
        self.unextrude_timer = self.reactor.register_timer(
            self.unextrude, self.reactor.NEVER
        )
        self.verify_flow_sensor_timer = self.reactor.register_timer(
            self.verify_flow_sensor_state, self.reactor.NEVER
        )
        self.verify_switch_sensor_timer = self.reactor.register_timer(
            self.verify_switch_sensor_state, self.reactor.NEVER
        )
        self.gcode.register_command(
            "UNLOAD_FILAMENT",
            self.cmd_UNLOAD_FILAMENT,
            "GCODE Macro to unload filament, takes into account if there is a belay and or a filament cutter with a sensor",
        )

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")

    def handle_ready(self):
        self.min_event_systime = self.reactor.monotonic() + 2.0

        if self.has_custom_boundary:
            self.custom_boundary_object = self.printer.lookup_object(
                "bed_custom_bound"
            )

        if self.bucket:
            self.bucket_object = self.printer.lookup_object("bucket")

        if self.cutter_name:
            if (
                self.printer.lookup_object(
                    f"cutter_sensor {self.cutter_name}", None
                )
                is not None
            ):
                self.cutter_object = self.printer.lookup_object(
                    f"cutter_sensor {self.cutter_name}", None
                )

                self.printer.register_event_handler(
                    "cutter_sensor:no_filament",
                    self.handle_cutter_fnp,
                )

        if self.idex:
            self.idex_object = self.printer.lookup_object("dual_carriage")

        if self.filament_flow_sensor_name:
            self.filament_flow_sensor_object = self.printer.lookup_object(
                f"filament_motion_sensor {self.filament_flow_sensor_name}",
                None,
            )

        if self.filament_switch_sensor_name:
            self.filament_switch_sensor_object = self.printer.lookup_object(
                f"filament_switch_sensor {self.filament_switch_sensor_name}",
                None,
            )

    def handle_cutter_fnp(self) -> None:
        """Handles cutter sensor **no filament** state"""
        if not self.unload_started:
            return
        self.gcode.respond_info("Pulling filament out of the printer wait....")
        logging.info(
            "Cutter reported not having filament, pulling filament out of the printer"
        )
        self.reactor.update_timer(self.unextrude_timer, self.reactor.NOW)

    def verify_switch_sensor_state(self, eventtime):
        """Verifies if the filament is actually unloaded or not
        The switch sensor here is assumed to be at the end of the filament pathway.
        """
        if not self.unload_started or not self.filament_switch_sensor_object:
            return self.reactor.NEVER

        if not self.filament_switch_sensor_object.get_status(eventtime).get(
            "filament_detected", True
        ):
            self.reactor.update_timer(self.unextrude_timer, self.reactor.NEVER)
            completion = self.reactor.register_callback(self.unload_end)
            return completion.wait()
        return eventtime + 1.50

    def verify_flow_sensor_state(self, eventtime):
        """TODO:

        Verifies the presence of filament on the **flow sensor** and reacts to this."""
        ...

    def unload_end(self, eventtime=None):
        if not self.unload_started or not self.toolhead:
            return self.reactor.NEVER

        self.gcode.run_script_from_command("G91")
        self.gcode.run_script_from_command("M83")
        self.gcode.run_script_from_command(
            "G92 E0.0"
        )  # Restore extruder position
        self.gcode.run_script_from_command("M82\nM400")
        self.restore_state()

        if self.custom_boundary_object:
            self.custom_boundary_object.set_custom_boundary()
            self.custom_boundary_object.move_to_park()

       

        self.gcode.respond_info("Cooling down extruder")
        self.heat_extruder(0, wait=False)

        if self.idex:
            self.gcode.respond_info("Parking toolhead 0")
            self.gcode.run_script_from_command("T0 PARK\nM400")

        self.gcode.respond_info("[UNLOAD FILAMENT] Finished")
        self.unload_started = False
        self.printer.send_event("unload_filament:end")
        return self.reactor.NEVER

    def unextrude(self, eventtime):
        """Retract filament A.K.A Unload"""
        if not self.unload_started:
            return self.reactor.NEVER
        try:
            if self.timeout:
                if self.unextrude_count >= self.timeout:
                    self.reactor.update_timer(
                        self.verify_switch_sensor_timer, self.reactor.NEVER
                    )
                    completion = self.reactor.register_callback(
                        self.unload_end
                    )
                    return completion.wait()
                self.unextrude_count += 1
            self.move_extruder_mm(
                distance=-10, speed=self.unload_speed, wait=False
            )
            return float(eventtime + float(10 / self.unload_speed))

        except Exception as e:
            raise UnloadFilamentError(
                f"[UNLOAD FILAMENT] Error while unloading: {e}"
            )

    def disable_sensors(self):
        if self.filament_flow_sensor_object:
            self.filament_flow_sensor_object.runout_helper.sensor_enabled = (
                False
            )

        if self.filament_switch_sensor_object:
            self.filament_switch_sensor_object.runout_helper.sensor_enabled = (
                False
            )
            self.gcode.respond_info("filament switch sensor is not enabled")

    def enable_sensors(self):
        if self.filament_flow_sensor_object:
            self.filament_flow_sensor_object.runout_helper.sensor_enabled = (
                True
            )

        if self.filament_switch_sensor_object:
            self.gcode.respond_info("filament switch sensor is now enabled")
            self.filament_switch_sensor_object.runout_helper.sensor_enabled = (
                True
            )

    def move_extruder_mm(self, distance=10.0, speed=30.0, wait=True) -> None:
        """Move the extruder

        Args:
            distance (float): The distance in mm to move the extruder.
        """
        if not self.toolhead:
            return
        try:
            eventtime = self.reactor.monotonic()
            gcode_move = self.printer.lookup_object("gcode_move")
            prev_pos = self.toolhead.get_position()
            gcode_move.absolute_coord = False  # G91
            v = distance * gcode_move.get_status(eventtime)["extrude_factor"]
            new_distance = v + prev_pos[3]
            self.toolhead.manual_move(
                [prev_pos[0], prev_pos[1], prev_pos[2], new_distance], speed
            )
            if wait:
                self.toolhead.wait_moves()
        except Exception as e:
            raise UnloadFilamentError(
                f"[UNLOAD FILAMENT] Error moving extruder {e}"
            )

    def home_needed(self) -> None:
        if not self.toolhead:
            return
        try:
            eventtime = self.reactor.monotonic()
            kin = self.toolhead.get_kinematics()
            _homed_axes = kin.get_status(eventtime)["homed_axes"]
            if "xyz" in _homed_axes.lower():
                return
            else:
                self.gcode.run_script_from_command("G28")
        except Exception as e:
            raise UnloadFilamentError(f"[UNLOAD FILAMENT] Error homing {e}")

    def heat_extruder(self, temp, wait: bool = False) -> None:
        """Heats the extruder and wait.

        Method returns when  temperature is [temp - 5 ; temp + 5].
        Args:
            temp (float):
                Target temperature in Celsius.
            wait (bool, optional):
                Whether to wait or not for the temperature to reach the interval . Defaults to True
        """
        # if not self.toolhead:
        #     return
        eventtime = self.reactor.monotonic()
        extruder = self.toolhead.get_extruder()
        pheaters = self.printer.lookup_object("heaters")
        extruder_heater = extruder.get_heater()
        pheaters.set_temperature(extruder_heater, temp, False)
        while not self.printer.is_shutdown() and wait:
            heater_temp, _ = extruder_heater.get_temp(eventtime)
            if heater_temp >= (temp - 5) and heater_temp <= (temp + 5):
                return
            eventtime = self.reactor.pause(eventtime + 1.0)
        return

    def save_state(self) -> None:
        """Save gcode state and dual carriage state if the system is in IDEX configuration"""
        if self.idex:
            self.gcode.run_script_from_command(
                "SAVE_DUAL_CARRIAGE_STATE NAME=unload_carriage_state\nM400"
            )
        self.gcode.run_script_from_command(
            "SAVE_GCODE_STATE NAME=_UNLOAD_STATE\nM400"
        )

    def restore_state(self) -> None:
        """Restore gcode state and dual carriage state if the system is in IDEX configuration"""
        self.gcode.run_script_from_command(
            "RESTORE_GCODE_STATE NAME=_UNLOAD_STATE MOVE=1 MOVE_SPEED=100\nM400"
        )
        if self.idex:
            self.gcode.run_script_from_command(
                "RESTORE_DUAL_CARRIAGE_STATE NAME=unload_carriage_state MOVE=0\nM400"
            )

    def cmd_UNLOAD_FILAMENT(self, gcmd):
        temp = gcmd.get(
            "TEMPERATURE", 250.0, parser=float, minval=220.0, maxval=500.0
        )
        if not self.toolhead:
            return
        try:
            if self.unload_started:
                self.gcode.respond_info("Printer already unloading filament")
                return
            self.home_needed()
            self.save_state()
            self.disable_sensors()  # So not to pause the filament switch sensor when filament is taken out

            if self.idex:
                if gcmd.get("TOOLHEAD") == "Load_T0":
                    self.gcode.run_script_from_command("T0 UNLOAD")
                else:
                    self.gcode.run_script_from_command("T1 UNLOAD")

            self.unload_started = True
            self.printer.send_event("unload_filament:start")
            self.gcode.respond_info("[UNLOAD FILAMENT] Start")

            self.gcode.run_script_from_command("G91\nM400")
            self.gcode.run_script_from_command("M83\nM400")

            if self.timeout:
                self.unextrude_count = 0

            self.heat_extruder(temp, wait=False)

            if self.bucket_object:
                self.bucket_object.move_to_bucket()

            self.heat_extruder(temp, wait=True)

            self.toolhead.wait_moves()

            if self.cutter_object:
                self.reactor.register_callback(
                    partial(
                        self.cutter_object.cut,
                        temp=temp,
                        return_last_pos=False,
                        off_heaters=False,
                    )
                )
            if not self.cutter_object and self.timeout:
                self.reactor.update_timer(
                    self.unextrude_timer, self.reactor.NOW
                )
                if self.filament_flow_sensor_object:
                    self.reactor.update_timer(
                        self.verify_flow_sensor_timer, self.reactor.NOW
                    )

            if self.filament_switch_sensor_object:
                self.gcode.respond_info(
                    "[UNLOAD] Starting filament switch sensor verification in 10 seconds"
                )
                self.reactor.update_timer(
                    self.verify_switch_sensor_timer, self.reactor.NOW + 5.0
                )

        except Exception as e:
            raise UnloadFilamentError(
                f"[UNLOAD] Unexpected error while trying to unload filament: {e}"
            )

    def get_status(self, eventtime):
        return {"state": bool(self.unload_started)}


def load_config(config):
    return UnloadFilament(config)
