import logging
import typing
from functools import partial


class LoadFilamentError(Exception):
    """Raised when there is an error loading filament"""

    def __init__(self, message, errors):
        super(LoadFilamentError, self).__init__(message)
        self.errors = errors


class LoadFilament:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.reactor = self.printer.get_reactor()
        self.gcode = self.printer.lookup_object("gcode")
        self.name = config.get_name().split()[-1]

        self.idex_object = None
        self.cutter_object = self.cutter_name = None
        self.bucket_object = None
        self.custom_boundary_object = None
        self.filament_switch_sensor_name = (
            self.filament_switch_sensor_object
        ) = None
        self.filament_flow_sensor_name = self.filament_flow_sensor_object = (
            None
        )
        self.load_started: bool = False
        self.current_purge_index: int = 0
        self.travel_speed = None
        self._old_extrude_distance: float | None = None
        self.extrude_count: int = 0

        self.printer.register_event_handler(
            "klippy:connect", self.handle_connect
        )
        self.printer.register_event_handler("klippy:ready", self.handle_ready)

        self.idex = config.getboolean("idex", False)
        self.has_custom_boundary = config.getboolean(
            "has_custom_boundary", False
        )

        if config.get("filament_flow_sensor_name", None):
            self.filament_flow_sensor_name = config.get(
                "filament_flow_sensor_name"
            )
        if (
            config.get("filament_switch_sensor_name", None)
            and self.filament_flow_sensor_object is None
        ):
            self.filament_switch_sensor_name = config.get(
                "filament_switch_sensor_name"
            )
        if (
            config.get("cutter_sensor_name", None)
            and not self.filament_flow_sensor_object
            and not self.filament_switch_sensor_object
        ):
            self.cutter_name = config.get("cutter_sensor_name")

        self.park = config.getfloatlist("park_xy", None, count=2)
        self.bucket = config.getboolean("bucket", False)
        self.extruder_to_nozzle_dist = config.getfloat(
            "extruder_to_nozzle_distance",
            default=30.0,
            minval=5.0,
            maxval=1000.0,
        )
        self.travel_speed = config.getfloat(
            "travel_speed", default=50.0, minval=20.0, maxval=500.0
        )
        self.load_speed = config.getfloat(
            "load_speed", default=10.0, minval=2.0, maxval=60.0
        )
        self.purge_speed = config.getfloat(
            "purge_speed", default=5.0, minval=2.0, maxval=50.0
        )
        self.purge_distance = config.getfloat(
            "purge_distance", default=1.5, minval=0.5, maxval=20.0
        )
        self.purge_max_retries = config.getint(
            "purge_max_count", default=10, minval=2, maxval=30
        )
        self.purge_interval = config.getfloat(
            "purge_interval", default=3.0, minval=0.5, maxval=10.0
        )
        self.extrude_timeout = config.getint(
            "extrude_max_count", default=None, minval=2, maxval=1000
        )

        # * Callback Timers
        self.extrude_purge_timer = self.reactor.register_timer(
            self.purge_extrude, self.reactor.NEVER
        )
        self.extrude_to_sensor_timer = self.reactor.register_timer(
            self.load, self.reactor.NEVER
        )
        self.verify_flow_sensor_timer = self.reactor.register_timer(
            self.verify_flow_sensor_state, self.reactor.NEVER
        )
        self.verify_switch_sensor_timer = self.reactor.register_timer(
            self.verify_switch_sensor_state, self.reactor.NEVER
        )

        # * Register new gcode commands
        self.gcode.register_mux_command(
            "LOAD_FILAMENT",
            "TOOLHEAD",
            self.name,
            self.cmd_LOAD_FILAMENT,
            "GCODE MACRO to load filament, takes into account if there is a belay and or a filament cutter with a sensor.",
        )
        self.gcode.register_mux_command(
            "PURGE_STOP",
            "TOOLHEAD",
            self.name,
            self.cmd_PURGE_STOP,
            "Helper gcode command that stop filament purging",
        )

    def handle_connect(self):
        self.toolhead = self.printer.lookup_object("toolhead")

    def handle_ready(self):
        self.min_event_systime = self.reactor.monotonic() + 2.0
        if self.bucket:
            self.bucket_object = self.printer.lookup_object("bucket")
            if self.bucket_object:
                logging.info("[LOADING FILAMENT EXTRA] Bucket Recognized")
        if self.cutter_name:
            if self.printer.lookup_object(
                f"cutter_sensor {str(self.cutter_name)}", None
            ):
                self.cutter_object = self.printer.lookup_object(
                    f"cutter_sensor {str(self.cutter_name)}", None
                )
                self.printer.register_event_handler(
                    "cutter_sensor:filament_present",
                    self.handle_cutter_filament_present,
                )
                logging.info(
                    "[LOAD FILAMENT EXTRA] Cutter Sensor recognized, cutter sensor state event handlers set"
                )
        # if self.printer.lookup_object("unload_filament", None) is not None:
        #     self.printer.register_event_handler(
        #         "unload_filament", self.handle_filament_unload
        #     )
        #     logging.info("[LOAD FILAMENT EXTRA] Unload Extra recognized")

        if self.idex:
            self.idex_object = self.printer.lookup_object("dual_carriage")
            logging.info("[LOAD FILAMENT EXTRA] Idex machine recognized")
        if self.filament_flow_sensor_name:
            self.filament_flow_sensor_object = self.printer.lookup_object(
                f"filament_motion_sensor {self.filament_flow_sensor_name}",
                default=None,
            )
            logging.info(
                f"[LOAD FILAMENT EXTRA] Filament flow sensor {self.filament_flow_sensor_name} recognized"
            )
        if self.filament_switch_sensor_name:
            self.filament_switch_sensor_object = self.printer.lookup_object(
                f"filament_switch_sensor {self.filament_switch_sensor_name}",
                default=None,
            )
            logging.info(
                f"[LOAD FILAMENT EXTRA] Filament switch sensor {self.filament_switch_sensor_name} recognized"
            )
        if self.has_custom_boundary:
            self.custom_boundary_object = self.printer.lookup_object(
                "bed_custom_bound", None
            )
            logging.info(
                "[LOAD FILAMENT EXTRA] Bed custom boundary extra recognized"
            )

    def handle_cutter_filament_present(self, eventtime=None) -> None:
        if not self.load_started or not self.cutter_object:
            return

        self.gcode.respond_info("Filament reached toolhead")
        logging.info(
            "[LOAD FILAMENT EXTRA] 'filament_present' event received from cutter sensor"
        )
        self.reactor.update_timer(
            self.extrude_to_sensor_timer, self.reactor.NEVER
        )
        self.reactor.update_timer(self.extrude_purge_timer, self.reactor.NOW)
        return

    def verify_switch_sensor_state(self, eventtime):
        if not self.load_started or not self.filament_switch_sensor_object:
            return self.reactor.NEVER

        if self.filament_switch_sensor_object.get_status(eventtime)[
            "filament_detected"
        ]:
            self.reactor.update_timer(
                self.extrude_to_sensor_timer, self.reactor.NEVER
            )
            self.move_extruder_mm(
                self.extruder_to_nozzle_dist, speed=30, wait=True
            )
            self.reactor.update_timer(
                self.extrude_purge_timer, self.reactor.NOW
            )
            return self.reactor.NEVER
        else:
            return eventtime + 1.50

    def verify_flow_sensor_state(self, eventtime):
        if self.load_started and self.filament_flow_sensor_object:
            if self.filament_flow_sensor_object.runout_helper.get_status(
                eventtime
            )["filament_detected"]:
                self.reactor.update_timer(
                    self.extrude_to_sensor_timer, self.reactor.NEVER
                )
                self.move_extruder_mm(
                    self.extruder_to_nozzle_dist, speed=30, wait=True
                )
                self.reactor.update_timer(
                    self.extrude_purge_timer, self.reactor.NOW
                )
                return self.reactor.NEVER
            return eventtime + 0.775

    def load(self, eventtime):
        if not self.load_started:
            return self.reactor.NEVER

        if self.extrude_timeout:
            if self.extrude_count >= self.extrude_timeout:
                self.move_extruder_mm(
                    self.extruder_to_nozzle_dist, speed=30, wait=True
                )  # Extrude to the nozzle
                self.reactor.update_timer(
                    self.extrude_purge_timer, self.reactor.NOW
                )
                return self.reactor.NEVER
            self.extrude_count += 1

        self.move_extruder_mm(distance=10, speed=self.load_speed, wait=False)
        return eventtime + float(10 / self.load_speed)

    def purge_extrude(self, eventtime):
        if not self.load_started:
            return self.reactor.NEVER

        if self.current_purge_index >= self.purge_max_retries:
            self.gcode.respond_info("[LOAD FILAMENT] End.")
            completion = self.reactor.register_callback(self._purge_end)
            completion.wait()
            return self.reactor.NEVER
        self.gcode.respond_info(
            f"[LOAD FILAMENT] Purging....{self.current_purge_index + 1}"
        )
        self.move_extruder_mm(
            distance=self.purge_distance, speed=self.purge_speed
        )
        self.current_purge_index += 1
        return eventtime + float(self.purge_interval)

    def _purge_end(self, eventtime):
        self.reactor.update_timer(self.extrude_purge_timer, self.reactor.NEVER)

        if self.park is not None:
            self.toolhead.manual_move(
                [self.park[0], self.park[1]], self.travel_speed
            )
            self.toolhead.wait_moves()

        if (
            self.has_custom_boundary
            and self.custom_boundary_object is not None
        ):
            if self.custom_boundary_object.get_status()["status"] == "default":
                self.custom_boundary_object.set_custom_boundary()

        self._old_extrude_distance = None
        self.current_purge_index = 0
        self.load_started = False
        self.printer.send_event("load_filament:end")
        self.gcode.run_script_from_command("G91\nM400")
        self.gcode.run_script_from_command("M83\nM400")
        self.gcode.run_script_from_command(
            "G92 E0.0\nM400"
        )  # Restore extruder position
        self.gcode.run_script_from_command("M82\nM400")
        self.toolhead.wait_moves()
        self.gcode.respond_info("Restoring gcode positions.")
        self.restore_state()
        self.toolhead.wait_moves()
        self.heat_and_wait(0, wait=False)

        if self.idex:
            self.gcode.run_script_from_command("T0 PARK\nM400")

    def move_extruder_mm(self, distance=10.0, speed=30.0, wait=True) -> None:
        """Move the extruder

        Args:
            distance (float): The distance in mm to move the extruder.
        """
        try:
            eventtime = self.reactor.monotonic()
            gcode_move = self.printer.lookup_object("gcode_move")
            prev_pos = self.toolhead.get_position()
            v = distance * gcode_move.get_status(eventtime)["extrude_factor"]
            new_distance = v + prev_pos[3]
            self.toolhead.manual_move(
                [prev_pos[0], prev_pos[1], prev_pos[2], new_distance], speed
            )
            if wait:
                self.toolhead.wait_moves()

            return
        except Exception:
            logging.error("Unexpected error while trying to move extruder.")
            return

    def move_home(self):
        """Move the toolhead to the home position (To the park position)"""
        if self.toolhead is None:
            return
        self.toolhead.manual_move(
            [self.park[0], self.park[1]], self.travel_speed
        )
        self.toolhead.wait_moves()

    def home_needed(self):
        """Perform home if needed"""
        if self.toolhead is None:
            return
        try:
            eventtime = self.reactor.monotonic()
            kin = self.toolhead.get_kinematics()
            _homed_axes = kin.get_status(eventtime)["homed_axes"]
            if "xyz" in _homed_axes.lower():
                return
            else:
                self.gcode.respond_info("Homing")
                self.gcode.run_script_from_command("G28")
        except Exception as e:
            logging.error(
                f"Unable to perform home on load filament, error: {e}"
            )

    def heat_and_wait(self, temp, wait: typing.Optional["bool"] = False):
        """Heats the extruder and wait.

        Method returns when  temperature is [temp - 5 ; temp + 5].
        Args:
            temp (float):
                Target temperature in Celsius.
            wait (bool, optional):
                Weather to wait or not for the temperature to reach the interval . Defaults to True
        """
        eventtime = self.reactor.monotonic()
        extruder = self.toolhead.get_extruder()
        pheaters = self.printer.lookup_object("heaters")
        extruder_heater = extruder.get_heater()
        pheaters.set_temperature(extruder.get_heater(), temp, False)
        while not self.printer.is_shutdown() and wait:
            heater_temp, _ = extruder_heater.get_temp(eventtime)
            if heater_temp >= (temp - 5) and heater_temp <= (temp + 5):
                return
            eventtime = self.reactor.pause(eventtime + 1.0)

    def disable_sensors(self):
        if not self.load_started:
            return
        if self.filament_flow_sensor_object is not None:
            self.filament_flow_sensor_object.runout_helper.sensor_enabled = 0

        if self.filament_switch_sensor_object is not None:
            self.filament_switch_sensor_object.runout_helper.sensor_enabled = 0

        return

    def enable_sensors(self):
        if self.filament_flow_sensor_object is not None:
            self.filament_flow_sensor_object.runout_helper.sensor_enabled = 1

        if self.filament_switch_sensor_object is not None:
            self.filament_switch_sensor_object.runout_helper.sensor_enabled = 1

    def save_state(self):
        """Save gcode state and dual carriage state if the system is in IDEX configuration"""
        if self.idex:
            self.gcode.run_script_from_command(
                f"SAVE_DUAL_CARRIAGE_STATE NAME=load_carriage_state_{self.name}\nM400"
            )
        self.gcode.run_script_from_command(
            "SAVE_GCODE_STATE NAME=_LOAD_STATE\nM400"
        )
        return True

    def restore_state(self):
        """Restore gcode state and dual carriage state if the system is in IDEX configuration"""
        self.gcode.run_script_from_command(
            "RESTORE_GCODE_STATE NAME=_LOAD_STATE MOVE=0\nM400"
        )
        if self.idex:
            self.gcode.run_script_from_command(
                f"RESTORE_DUAL_CARRIAGE_STATE NAME=load_carriage_state_{self.name}\nM400"
            )

        return True

    cmd_PURGE_STOP_HELPER = """Helper gcode command to stop filament purging when in the Load filament routine"""

    def cmd_PURGE_STOP(self, gcmd):
        if self.load_started:
            self.reactor.register_callback(self._purge_end)
        else:
            self.gcode.respond_info("Not in loading routine skipping command.")

    cmd_LOAD_FILAMENT_HELPER = """Load Filament to the toolhead routine"""

    def cmd_LOAD_FILAMENT(self, gcmd):
        temp = gcmd.get(
            "TEMPERATURE", 220.0, parser=float, minval=210, maxval=250
        )
        try:
            if self.load_started:
                self.gcode.respond_info("Already loading filament")
                return

            self.home_needed()
            self.save_state()
            self.disable_sensors()

            self.load_started = True
            self.printer.send_event("load_filament:start")
            self.gcode.respond_info("[LOAD FILAMENT] Start")

            # * Select the head -> Meant for Idex systems
            if self.idex:
                if gcmd.get("TOOLHEAD") == "Load_T0":
                    self.gcode.run_script_from_command("T0 LOAD")
                else:
                    self.gcode.run_script_from_command("T1 LOAD")

            self.gcode.run_script_from_command("G91\nM400")
            self.gcode.run_script_from_command("M83\nM400")

            self.extrude_count = 0

            if self.custom_boundary_object is not None:
                self.custom_boundary_object.restore_default_boundary()

            self.heat_and_wait(temp, wait=False)

            if self.bucket_object is not None:
                self.bucket_object.move_to_bucket()

            self.heat_and_wait(temp, wait=True)
            self.toolhead.wait_moves()

            # * Force the motion sensor to "No Filament" state
            if self.filament_flow_sensor_object is not None:
                self.reactor.register_callback(
                    partial(
                        self.filament_flow_sensor_object.encoder_event,
                        state=False,
                    )
                )
                self.move_extruder_mm(distance=30, speed=40, wait=True)

            # * Actually start loading the machine
            self.reactor.update_timer(
                self.extrude_to_sensor_timer, self.reactor.NOW
            )

            if self.filament_flow_sensor_object is not None:
                self.reactor.update_timer(
                    self.verify_flow_sensor_timer, self.reactor.NOW
                )

            if self.filament_switch_sensor_object is not None:
                self.reactor.update_timer(
                    self.verify_switch_sensor_timer, self.reactor.NOW
                )

        except LoadFilamentError as e:
            logging.error(f"[LOADING FILAMENT ERROR]: {e}")

    def get_status(self, eventtime):
        return {"state": bool(self.load_started)}


def load_config_prefix(config):
    return LoadFilament(config)
