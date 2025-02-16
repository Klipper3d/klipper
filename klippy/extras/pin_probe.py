# Z-Probe support
#
# Copyright (C) 2017-2024  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
from . import manual_probe, probe

######################################################################
# Pin Probe device implementation helpers
######################################################################


# Helper to implement common probing commands
class PinProbeCommandHelper(probe.ProbeCommandHelper):
    def __init__(self, config, probe, query_endstop=None):
        self.printer = config.get_printer()
        self.probe = probe
        self.query_endstop = query_endstop
        self.name = config.get_name()
        self.probe_name = self.name.split()[1]

        gcode = self.printer.lookup_object("gcode")
        # QUERY_PROBE command
        self.last_state = False
        gcode.register_mux_command(
            "QUERY_PIN_PROBE",
            "PROBE",
            self.probe_name,
            self.cmd_QUERY_PROBE,
            desc=self.cmd_QUERY_PROBE_help,
        )
        # PROBE command
        self.last_z_result = 0.0
        gcode.register_mux_command(
            "PROBE_PIN",
            "PROBE",
            self.probe_name,
            self.cmd_PROBE,
            desc=self.cmd_PROBE_help,
        )
        # PROBE_CALIBRATE command
        self.probe_calibrate_z = 0.0
        gcode.register_mux_command(
            "PROBE_PIN_CALIBRATE",
            "PROBE",
            self.probe_name,
            self.cmd_PROBE_CALIBRATE,
            desc=self.cmd_PROBE_CALIBRATE_help,
        )
        # Other commands
        gcode.register_mux_command(
            "PROBE_PIN_ACCURACY",
            "PROBE",
            self.probe_name,
            self.cmd_PROBE_ACCURACY,
            desc=self.cmd_PROBE_ACCURACY_help,
        )
        gcode.register_mux_command(
            "Z_OFFSET_APPLY_PROBE_PIN",
            "PROBE",
            self.probe_name,
            self.cmd_Z_OFFSET_APPLY_PROBE,
            desc=self.cmd_Z_OFFSET_APPLY_PROBE_help,
        )


# Homing via probe:z_virtual_endstop
class PinHomingViaProbeHelper(probe.HomingViaProbeHelper):
    def __init__(self, config, mcu_probe):
        self.printer = config.get_printer()
        self.mcu_probe = mcu_probe
        self.multi_probe_pending = False
        self.probe_name = config.get_name().split()[1]
        # Register z_virtual_endstop pin
        self.printer.lookup_object("pins").register_chip(
            "probe_{probe_name}".format(probe_name=self.probe_name), self
        )
        # Register event handlers
        self.printer.register_event_handler(
            "klippy:mcu_identify", self._handle_mcu_identify
        )
        self.printer.register_event_handler(
            "homing:homing_move_begin", self._handle_homing_move_begin
        )
        self.printer.register_event_handler(
            "homing:homing_move_end", self._handle_homing_move_end
        )
        self.printer.register_event_handler(
            "homing:home_rails_begin", self._handle_home_rails_begin
        )
        self.printer.register_event_handler(
            "homing:home_rails_end", self._handle_home_rails_end
        )
        self.printer.register_event_handler(
            "gcode:command_error", self._handle_command_error
        )


# Helper to track multiple probe attempts in a single command
class PinProbeSessionHelper(probe.ProbeSessionHelper):
    def __init__(self, config, mcu_probe):
        self.printer = config.get_printer()
        self.mcu_probe = mcu_probe
        gcode = self.printer.lookup_object("gcode")
        self.dummy_gcode_cmd = gcode.create_gcode_command("", "", {})
        # Infer Z position to move to during a probe
        if config.has_section("stepper_z"):
            zconfig = config.getsection("stepper_z")
            self.z_position = zconfig.getfloat("position_min", 0.0,
                                               note_valid=False)
        else:
            pconfig = config.getsection("printer")
            self.z_position = pconfig.getfloat(
                "minimum_z_position", 0.0, note_valid=False
            )
        self.homing_helper = PinHomingViaProbeHelper(config, mcu_probe)
        # Configurable probing speeds
        self.speed = config.getfloat("speed", 5.0, above=0.0)
        self.lift_speed = config.getfloat("lift_speed", self.speed, above=0.0)
        # Multi-sample support (for improved accuracy)
        self.sample_count = config.getint("samples", 1, minval=1)
        self.sample_retract_dist = config.getfloat(
            "sample_retract_dist", 2.0, above=0.0
        )
        atypes = ["median", "average"]
        self.samples_result = config.getchoice("samples_result", atypes,
                                                "average")
        self.samples_tolerance = config.getfloat("samples_tolerance", 0.100,
                                                  minval=0.0)
        self.samples_retries = config.getint("samples_tolerance_retries", 0,
                                              minval=0)
        # Session state
        self.multi_probe_pending = False
        self.results = []
        # Register event handlers
        self.printer.register_event_handler(
            "gcode:command_error", self._handle_command_error
        )


######################################################################
# Tools for utilizing the probe
######################################################################


# Helper code that can probe a series of points and report the
# position at each point.
class PinProbePointsHelper(probe.ProbePointsHelper):
    def __init__(self, config, finalize_callback, default_points=None):
        self.printer = config.get_printer()
        self.finalize_callback = finalize_callback
        self.probe_points = default_points
        self.name = config.get_name()
        self.probe_name = self.name.split()[1]
        self.gcode = self.printer.lookup_object("gcode")
        # Read config settings
        if default_points is None or config.get("points", None) is not None:
            self.probe_points = config.getlists(
                "points", seps=(",", "\n"), parser=float, count=2
            )
        def_move_z = config.getfloat("horizontal_move_z", 5.0)
        self.default_horizontal_move_z = def_move_z
        self.speed = config.getfloat("speed", 50.0, above=0.0)
        self.use_offsets = False
        # Internal probing state
        self.lift_speed = self.speed
        self.probe_offsets = (0.0, 0.0, 0.0)
        self.manual_results = []

    def start_probe(self, gcmd):
        manual_probe.verify_no_manual_probe(self.printer)
        # Lookup objects
        probe = self.printer.lookup_object(
            "probe_{probe_name}".format(probe_name=self.probe_name), None
        )
        method = gcmd.get("METHOD", "automatic").lower()
        def_move_z = self.default_horizontal_move_z
        self.horizontal_move_z = gcmd.get_float("HORIZONTAL_MOVE_Z",
                                                 def_move_z)
        if probe is None or method == "manual":
            # Manual probe
            self.lift_speed = self.speed
            self.probe_offsets = (0.0, 0.0, 0.0)
            self.manual_results = []
            self._manual_probe_start()
            return
        # Perform automatic probing
        self.lift_speed = probe.get_probe_params(gcmd)["lift_speed"]
        self.probe_offsets = probe.get_offsets()
        if self.horizontal_move_z < self.probe_offsets[2]:
            raise gcmd.error("horizontal_move_z can't be less than"
                             " probe's z_offset")
        probe_session = probe.start_probe_session(gcmd)
        probe_num = 0
        while 1:
            self._raise_tool(not probe_num)
            if probe_num >= len(self.probe_points):
                results = probe_session.pull_probed_results()
                done = self._invoke_callback(results)
                if done:
                    break
                # Caller wants a "retry" - restart probing
                probe_num = 0
            self._move_next(probe_num)
            probe_session.run_probe(gcmd)
            probe_num += 1
        probe_session.end_probe_session()


# Main external probe interface
class PinProbe:
    def __init__(self, config):
        self.printer = config.get_printer()
        self.mcu_probe = probe.ProbeEndstopWrapper(config)
        self.cmd_helper = PinProbeCommandHelper(
            config, self, self.mcu_probe.query_endstop
        )
        self.probe_offsets = probe.ProbeOffsetsHelper(config)
        self.probe_session = PinProbeSessionHelper(config, self.mcu_probe)

    def get_probe_params(self, gcmd=None):
        return self.probe_session.get_probe_params(gcmd)

    def get_offsets(self):
        return self.probe_offsets.get_offsets()

    def get_status(self, eventtime):
        return self.cmd_helper.get_status(eventtime)

    def start_probe_session(self, gcmd):
        return self.probe_session.start_probe_session(gcmd)


def load_config_prefix(config):
    pinProbe = PinProbe(config)
    probe_name = config.get_name().split()[1]
    config.get_printer().add_object(
        "probe_{probe_name}".format(probe_name=probe_name), pinProbe
    )
    return pinProbe
