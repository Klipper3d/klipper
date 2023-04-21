# Per-tool Z-Probe support
#
# Copyright (C) 2023 Viesturs Zarins <viesturz@gmail.com>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import logging
import pins


# Virtual endstop, using a tool attached Z probe in a toolchanger setup.
# Tool endstop change may be done either via SET_ACTIVE_TOOL_PROBE TOOL=99
# Or via auto-detection of single open tool probe via DETECT_ACTIVE_TOOL_PROBE
class ToolProbeEndstop:
    def __init__(self, config, mcu_probe):
        self.printer = config.get_printer()
        self.name = config.get_name()
        self.mcu_probe = mcu_probe
        self.tool_probes = {}
        self.last_query = {}  # map from tool number to endstop state
        self.active_probe = None
        self.gcode_move = self.printer.load_object(config, "gcode_move")

        # Emulate the probe object, since others rely on this.
        if self.printer.lookup_object('probe', default=None):
            raise self.printer.config_error(
                'Cannot have both [probe] and [tool_probe_endstop].')
        self.printer.add_object('probe', self)
        # Register z_virtual_endstop pin
        self.printer.lookup_object('pins').register_chip('probe', self)
        # Register homing event handlers
        self.printer.register_event_handler("klippy:connect",
                                            self._handle_connect)
        self.printer.register_event_handler("homing:homing_move_begin",
                                            self._handle_homing_move_begin)
        self.printer.register_event_handler("homing:homing_move_end",
                                            self._handle_homing_move_end)
        self.printer.register_event_handler("homing:home_rails_begin",
                                            self._handle_home_rails_begin)
        self.printer.register_event_handler("homing:home_rails_end",
                                            self._handle_home_rails_end)
        # Register PROBE/QUERY_PROBE commands
        self.gcode = self.printer.lookup_object('gcode')
        self.gcode.register_command('SET_ACTIVE_TOOL_PROBE',
                                    self.cmd_SET_ACTIVE_TOOL_PROBE,
                                    desc=self.cmd_SET_ACTIVE_TOOL_PROBE_help)
        self.gcode.register_command('DETECT_ACTIVE_TOOL_PROBE',
                                    self.cmd_DETECT_ACTIVE_TOOL_PROBE,
                                    desc=self.cmd_DETECT_ACTIVE_TOOL_PROBE_help)
        self.gcode.register_command('PROBE', self.cmd_PROBE,
                                    desc=self.cmd_PROBE_help)
        self.gcode.register_command('QUERY_PROBE', self.cmd_QUERY_PROBE,
                                    desc=self.cmd_QUERY_PROBE_help)
        self.gcode.register_command('PROBE_CALIBRATE', self.cmd_PROBE_CALIBRATE,
                                    desc=self.cmd_PROBE_CALIBRATE_help)
        self.gcode.register_command('PROBE_ACCURACY', self.cmd_PROBE_ACCURACY,
                                    desc=self.cmd_PROBE_ACCURACY_help)
        self.gcode.register_command('Z_OFFSET_APPLY_PROBE',
                                    self.cmd_Z_OFFSET_APPLY_PROBE,
                                    desc=self.cmd_Z_OFFSET_APPLY_PROBE_help)

    def _handle_connect(self):
        self._detect_active_tool()

    def _handle_homing_move_begin(self, hmove):
        if self.mcu_probe in hmove.get_mcu_endstops():
            if self.active_probe:
                self.active_probe.mcu_probe.probe_prepare(hmove)
            else:
                raise self.printer.command_error("No active tool probe")

    def _handle_homing_move_end(self, hmove):
        if self.mcu_probe in hmove.get_mcu_endstops():
            if self.active_probe:
                self.active_probe.mcu_probe.probe_finish(hmove)

    def _handle_home_rails_begin(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if self.mcu_probe in endstops:
            self.multi_probe_begin()

    def _handle_home_rails_end(self, homing_state, rails):
        endstops = [es for rail in rails for es, name in rail.get_endstops()]
        if self.mcu_probe in endstops:
            self.multi_probe_end()

    def get_offsets(self):
        if self.active_probe:
            return self.active_probe.get_offsets()
        return 0.0, 0.0, 0.0

    def get_lift_speed(self, gcmd=None):
        if self.active_probe:
            return self.active_probe.get_lift_speed(gcmd)
        raise self.printer.command_error("No active tool probe")

    def run_probe(self, gcmd):
        if self.active_probe:
            return self.active_probe.run_probe(gcmd)
        raise gcmd.error("No active tool probe")

    def multi_probe_begin(self):
        if self.active_probe:
            return self.active_probe.multi_probe_begin()
        raise self.printer.command_error("No active tool probe")

    def multi_probe_end(self):
        if self.active_probe:
            return self.active_probe.multi_probe_end()
        raise self.printer.command_error("No active tool probe")

    def setup_pin(self, pin_type, pin_params):
        if pin_type != 'endstop' or pin_params['pin'] != 'z_virtual_endstop':
            raise pins.error("Probe virtual endstop only useful as endstop pin")
        if pin_params['invert'] or pin_params['pullup']:
            raise pins.error("Can not pullup/invert probe virtual endstop")
        return self.mcu_probe

    def add_probe(self, config, tool_probe):
        if (tool_probe.tool in self.tool_probes):
            raise config.error("Duplicate tool probe nr: " + tool_probe.tool)
        self.tool_probes[tool_probe.tool] = tool_probe

    def set_active_probe(self, tool_probe):
        if self.active_probe == tool_probe:
            return
        if self.active_probe:
            self.active_probe.multi_probe_end()
        self.active_probe = tool_probe
        if self.active_probe:
            self.mcu_probe.set_active_probe(tool_probe.mcu_probe)
        else:
            self.mcu_probe.set_active_probe(None)

    def _query_open_tools(self):
        toolhead = self.printer.lookup_object('toolhead')
        print_time = toolhead.get_last_move_time()
        self.last_query.clear()
        candidates = []
        for tool_probe in self.tool_probes.values():
            triggered = tool_probe.mcu_probe.query_endstop(print_time)
            self.last_query[tool_probe.tool] = triggered
            if not triggered:
                candidates.append(tool_probe)
        return candidates

    def _describe_tool_detection_issue(self, candidates):
        if len(candidates) == 1:
            return 'OK'
        elif len(candidates) == 0:
            return "All probes triggered"
        else:
            return "Multiple probes not triggered: %s" % map(lambda p: p.name,
                                                             candidates)

    def _ensure_active_tool_or_fail(self, gcode):
        if self.active_probe:
            return
        active_tools = self._query_open_tools()
        if len(active_tools) != 1:
            raise gcode.error(self._describe_tool_detection_issue(active_tools))
        self.set_active_probe(active_tools[0])

    def _detect_active_tool(self):
        active_tools = self._query_open_tools()
        if len(active_tools) == 1:
            self.set_active_probe(active_tools[0])

    cmd_SET_ACTIVE_TOOL_PROBE_help = ("Set the tool probe that will "
                                      "act as the Z endstop.")

    def cmd_SET_ACTIVE_TOOL_PROBE(self, gcmd):
        probe_nr = gcmd.get_int("T")
        if (probe_nr not in self.tool_probes):
            raise gcmd.error(
                "SET_ACTIVE_TOOL_PROBE no tool probe for tool %d" % (probe_nr))
        self.set_active_probe(self.tool_probes[probe_nr])

    cmd_DETECT_ACTIVE_TOOL_PROBE_help = (
        "Detect which tool is active by "
        "identifying a probe that is NOT triggered")

    def cmd_DETECT_ACTIVE_TOOL_PROBE(self, gcmd):
        active_tools = self._query_open_tools()
        if len(active_tools) == 1:
            active = active_tools[0]
            gcmd.respond_info("Found active tool probe: %s" % (active.name))
            self.set_active_probe(active)
        else:
            self.set_active_probe(None)
            gcmd.respond_info(self._describe_tool_detection_issue(active_tools))

    cmd_PROBE_help = "Probe Z-height at current XY position"

    def cmd_PROBE(self, gcmd):
        self._ensure_active_tool_or_fail(gcmd)
        self.active_probe.cmd_PROBE(gcmd)

    cmd_QUERY_PROBE_help = "Return the status of the z-probe"

    def cmd_QUERY_PROBE(self, gcmd):
        if not self.active_probe:
            raise gcmd.respond_info("no active tool probe")
        self.active_probe.cmd_QUERY_PROBE(gcmd)

    def get_status(self, eventtime):
        if not self.active_probe:
            return {'name': self.name,
                    'active_tool_probe': None,
                    'active_tool_number': -1,
                    'last_query': False,
                    'last_tools_query': self.last_query,
                    'last_z_result': 0.}
        status = self.active_probe.get_status(eventtime)
        status['name'] = self.name
        status['active_tool_probe'] = self.active_probe.name
        status['active_tool'] = self.active_probe.tool
        status['last_tools_query'] = self.last_query
        return status

    cmd_PROBE_ACCURACY_help = "Probe Z-height accuracy at current XY position"

    def cmd_PROBE_ACCURACY(self, gcmd):
        self._ensure_active_tool_or_fail(gcmd)
        self.active_probe.cmd_PROBE_ACCURACY(gcmd)

    cmd_PROBE_CALIBRATE_help = "Calibrate the probe's z_offset"

    def cmd_PROBE_CALIBRATE(self, gcmd):
        self._ensure_active_tool_or_fail(gcmd)
        self.active_probe.cmd_PROBE_CALIBRATE(gcmd)

    def cmd_Z_OFFSET_APPLY_PROBE(self, gcmd):
        if not self.active_probe:
            raise gcmd.respond_info("no active tool probe")
        self.active_probe.cmd_Z_OFFSET_APPLY_PROBE(gcmd)

    cmd_Z_OFFSET_APPLY_PROBE_help = "Adjust the probe's z_offset"


# Endstop wrapper that routes commands to the selected tool probe.
class ToolProbeEndstopWrapper:
    def __init__(self, config):
        self.active_probe = None
        self._steppers = []
        self.printer = config.get_printer()
        # Create an "endstop" object to handle the probe pin
        self.printer.register_event_handler('klippy:mcu_identify',
                                            self._handle_mcu_identify)

    def _handle_mcu_identify(self):
        kin = self.printer.lookup_object('toolhead').get_kinematics()
        for stepper in kin.get_steppers():
            if stepper.is_active_axis('z'):
                self.add_stepper(stepper)

    # Each tool probe will manage the steppers internally, this is just to have
    # a tool independant get_steppers
    def add_stepper(self, stepper):
        if stepper in self._steppers:
            return
        self._steppers.append(stepper)

    def get_steppers(self):
        return list(self._steppers)

    def set_active_probe(self, tool_probe_wrapper):
        self.active_probe = tool_probe_wrapper
        # Update Wrappers
        if self.active_probe:
            self.get_mcu = self.active_probe.get_mcu
            self.home_start = self.active_probe.home_start
        else:
            self.get_mcu = None
            self.home_start = None

    def query_endstop(self, print_time):
        if not self.active_probe:
            raise self.printer.command_error(
                "Cannot query endstop - no active tool probe.")
        return self.active_probe.query_endstop(print_time)

    def home_wait(self, home_end_time):
        if not self.active_probe:
            raise self.printer.command_error(
                "Cannot home wait - no active tool probe.")
        return self.active_probe.home_wait(home_end_time)

    def get_position_endstop(self):
        if not self.active_probe:
            logging.warning(
                "Getting tool probe endstop position before active "
                "tool is determined.")
            return 0.0
        return self.active_probe.get_position_endstop()


def load_config(config):
    return ToolProbeEndstop(config, ToolProbeEndstopWrapper(config))
