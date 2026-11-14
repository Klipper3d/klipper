import ast
import contextlib
import os
import struct
import sys
import threading
import types
import unittest
from unittest import mock


ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "klippy"))

from extras.e3v3se_display import E3V3SEDisplay
from extras.e3v3se_display.model import FileBrowser, validate_filename
from extras.e3v3se_display.tjc3224 import (FRAME_TAIL, TJC3224,
                                           safe_text_bytes, utf8_truncate)
from extras.e3v3se_display.transport import split_chunks
from extras.e3v3se_display.ui import DisplayUI
from extras.e3v3se_display.translations import translate
from extras.e3v3se_display import translations
from configfile import ConfigAutoSave, ConfigFileReader, ConfigWrapper
from extras.gcode_macro import GCodeMacro, PrinterGCodeMacro
from extras.gcode_move import GCodeMove
from gcode import CommandError, GCodeDispatch


class FakeTransport:
    def __init__(self):
        self.frames = []

    def send(self, frame):
        self.frames.append(bytes(frame))
        return True


class SourceCompatibilityTest(unittest.TestCase):
    def test_non_ascii_display_sources_declare_utf8_for_python2(self):
        directory = os.path.join(ROOT, "klippy", "extras", "e3v3se_display")
        for name in os.listdir(directory):
            if not name.endswith(".py"):
                continue
            with self.subTest(name=name):
                with open(os.path.join(directory, name), "rb") as source:
                    data = source.read()
                if any(value > 127 for value in data):
                    self.assertRegex(b"\n".join(data.splitlines()[:2]),
                                     br"coding[:=]\s*utf-?8")


class TJC3224Test(unittest.TestCase):
    def test_instances_never_share_frame_state(self):
        lengths = []
        for unused_index in range(100):
            transport = FakeTransport()
            display = TJC3224(transport)
            display.set_palette(0xffff, 0)
            lengths.append(len(transport.frames[0]))
        self.assertEqual([10] * 100, lengths)

    def test_negative_values_are_signed(self):
        transport = FakeTransport()
        display = TJC3224(transport)
        frame = display.value(10, 10, -123, signed=True)
        self.assertTrue(frame[2] & 0x40)
        self.assertEqual(-123, struct.unpack(">q", frame[-12:-4])[0])
        self.assertEqual(FRAME_TAIL, frame[-4:])

    def test_utf8_is_truncated_on_byte_boundary(self):
        text = u"arquivo-\u00e7\u00e3o"
        truncated = utf8_truncate(text, 9)
        encoded = truncated.encode("utf-8")
        self.assertEqual(u"arquivo-", truncated)
        self.assertLessEqual(len(encoded), 9)
        self.assertEqual(truncated, encoded.decode("utf-8"))

    def test_protocol_tail_is_not_embedded_in_text(self):
        encoded = safe_text_bytes(u"name\u00cc3\u00c3<.gcode", 100)
        self.assertNotIn(FRAME_TAIL, encoded)


class TransportTest(unittest.TestCase):
    def test_large_utf8_frame_is_chunked_and_reassembled(self):
        frame = (u"long-" + u"arquivo-" * 30).encode("utf-8")
        chunks = split_chunks(frame)
        self.assertTrue(all(0 < len(chunk) <= 40 for chunk in chunks))
        self.assertEqual(frame, b"".join(chunks))


class FileBrowserTest(unittest.TestCase):
    def test_empty_directory(self):
        browser = FileBrowser()
        self.assertEqual([], browser.update([]))

    def test_files_directories_utf8_and_special_characters(self):
        browser = FileBrowser()
        entries = browser.update([
            (u"cube.gcode", 1),
            (u"models/peca acentuada.gcode", 2),
            (u"models/deep/quoted'name.gcode", 3),
        ])
        self.assertEqual([u"models", u"cube.gcode"],
                         [entry.name for entry in entries])
        self.assertTrue(entries[0].is_dir)
        self.assertIsNone(browser.enter(entries[0]))
        entries = browser.update([
            (u"models/peca acentuada.gcode", 2),
            (u"models/deep/quoted'name.gcode", 3),
        ])
        self.assertEqual([u"deep", u"peca acentuada.gcode"],
                         [entry.name for entry in entries])
        self.assertEqual(u"models/peca acentuada.gcode",
                         browser.enter(entries[1]))
        self.assertTrue(browser.back())
        self.assertEqual("", browser.path)

    def test_special_filename_validation_and_control_rejection(self):
        filename = "dir/a file's #1;copy\\name.gcode"
        self.assertEqual(filename, validate_filename(filename))
        with self.assertRaises(ValueError):
            validate_filename("bad\nRESTART.gcode")

    def test_backslash_is_not_treated_as_a_directory_separator(self):
        browser = FileBrowser()
        entries = browser.update([("part\\name.gcode", 1)])
        self.assertEqual("part\\name.gcode", entries[0].name)


class DisplayActionTest(unittest.TestCase):
    def test_relative_adjustment_uses_only_effective_bounded_delta(self):
        ui = DisplayUI.__new__(DisplayUI)
        values = []
        ui.adjustment = {
            "value": 9., "step": 5., "minimum": 0., "maximum": 10.,
            "callback": lambda value: (values.append(value) or (True, "")),
            "relative": True,
        }
        ui._draw_adjustment = lambda: None
        ui.owner = type("Owner", (), {
            "is_operation_busy": lambda self: False})()
        ui._adjust(1)
        ui._adjust(1)
        self.assertEqual([1.], values)
        self.assertEqual(10., ui.adjustment["value"])

    def test_move_failure_still_restores_gcode_state(self):
        display = E3V3SEDisplay.__new__(E3V3SEDisplay)
        display.last_status = {"homed_axes": "xyz", "can_extrude": True}
        calls = []

        def execute(script):
            calls.append(script)
            if script.startswith("G1"):
                return False, "move failed"
            return True, ""

        display._execute = execute
        result = display.move_axis("X", 1.)
        self.assertFalse(result[0])
        self.assertTrue(calls[-1].startswith("RESTORE_GCODE_STATE"))

    def test_start_print_passes_special_filename_without_gcode_parsing(self):
        class CommandError(Exception):
            pass

        class FakeGCode:
            def __init__(self):
                self.params = None
                self.mutex = threading.Lock()

            def create_gcode_command(self, command, commandline, params):
                self.params = params
                return params

            def get_mutex(self):
                return self.mutex

        class FakeVirtualSD:
            def __init__(self):
                self.filename = None

            def cmd_SDCARD_PRINT_FILE(self, gcmd):
                self.filename = gcmd["FILENAME"]

        display = E3V3SEDisplay.__new__(E3V3SEDisplay)
        display.gcode = FakeGCode()
        display.virtual_sdcard = FakeVirtualSD()
        display.printer = type("Printer", (), {
            "command_error": CommandError})()
        filename = "models/a file's #1;copy\\name.gcode"
        result = display.start_print(filename)
        self.assertTrue(result[0])
        self.assertEqual(filename, display.virtual_sdcard.filename)


class FakeReactor:
    NOW = 0.
    NEVER = float("inf")

    def __init__(self):
        self.now = 0.
        self.on_pause = None
        self.callbacks = []
        self.timer_waketimes = {}

    def monotonic(self):
        return self.now

    def mutex(self):
        return threading.Lock()

    def assert_no_pause(self):
        return contextlib.nullcontext()

    def update_timer(self, timer, waketime):
        self.timer_waketimes[timer] = waketime

    def register_timer(self, callback):
        return object()

    def register_callback(self, callback):
        self.callbacks.append(callback)

    def pause(self, waketime):
        self.now = waketime
        if self.on_pause:
            self.on_pause()
        return self.now


class FakeExtruder:
    min_extrude_temp = 170.
    max_temp = 260.

    def __init__(self):
        self.temperature = 25.
        self.target = 0.

    def get_heater(self):
        return self

    def get_status(self, eventtime):
        return {"temperature": self.temperature, "target": self.target,
                "can_extrude": self.temperature >= self.min_extrude_temp}


class FakePrinter:
    command_error = config_error = CommandError

    def __init__(self):
        self.objects = {}
        self.events = {}
        self.reactor = FakeReactor()
        self.shutdown = False

    def lookup_object(self, name, default=None):
        return self.objects.get(name, default)

    def lookup_objects(self):
        return self.objects.items()

    def load_object(self, config, name):
        return self.objects[name]

    def get_reactor(self):
        return self.reactor

    def get_start_args(self):
        return {}

    def register_event_handler(self, event, callback):
        self.events.setdefault(event, []).append(callback)

    def send_event(self, event, *args):
        for callback in self.events.get(event, []):
            callback(*args)

    def is_shutdown(self):
        return self.shutdown

    def invoke_shutdown(self, message):
        self.shutdown = True


class FakeUI:
    SUCCESS = 1
    ERROR = 2

    def __init__(self):
        self.messages = []

    def show_operation(self):
        pass

    def show_prepare_menu(self):
        pass

    def show_message(self, title, message, *args):
        self.messages.append((title, message))


class FilamentHarness:
    # Execute the real Klipper dispatcher, macros and G-code state machinery.
    # Only heaters, the reactor and physical motion are fakes.
    def __init__(self, calibrated=True):
        self.printer = printer = FakePrinter()
        self.extruder = FakeExtruder()
        printer.objects["extruder"] = self.extruder
        printer.objects["heaters"] = type("Heaters", (), {
            "set_temperature": lambda obj, heater, temp:
                setattr(heater, "target", temp)})()
        self.state = "standby"
        printer.objects["print_stats"] = type("Stats", (), {
            "get_status": lambda obj, time: {"state": self.state}})()
        self.moves = []
        self.position = [0., 0., 0., 0.]
        self.fail_move = False
        self.on_wait = None
        toolhead = type("Toolhead", (), {
            "move": lambda obj, pos, speed: self.move(pos, speed),
            "get_position": lambda obj: list(self.position)})()
        printer.objects["toolhead"] = toolhead
        self.gcode = GCodeDispatch(printer)
        printer.objects["gcode"] = self.gcode
        reader = ConfigFileReader()
        path = os.path.join(ROOT, "config", "ender3-v3-se", "macros.cfg")
        bmcu_path = os.path.join(ROOT, "config", "ender3-v3-se", "bmcu.cfg")
        cfg = reader.build_fileconfig_with_includes(
            reader.read_config_file(path) + "\n" +
            reader.read_config_file(bmcu_path), path)
        wrap = lambda name: ConfigWrapper(printer, cfg, {}, name)
        printer.objects["gcode_macro"] = PrinterGCodeMacro(wrap("printer"))
        self.gmove = GCodeMove(wrap("printer"))
        printer.objects["gcode_move"] = self.gmove
        for section in cfg.sections():
            if section.startswith("gcode_macro "):
                # Pause/resume integration needs the full real printer in CI.
                if section.split()[1] in ("PAUSE", "RESUME", "CANCEL_PRINT"):
                    continue
                printer.objects[section] = GCodeMacro(wrap(section))
        settings = printer.objects["gcode_macro _ENDER_FILAMENT_SETTINGS"]
        if calibrated:
            settings.variables.update({
                "calibrated": 1, "sensor_to_gears_mm": 80.,
                "capture_mm": 5., "melt_mm": 45., "purge_mm": 15.,
                "tip_push_mm": 3., "tip_retract_mm": 20.,
                "cooling_move_mm": 3., "cooling_moves": 2,
                "tip_dwell_ms": 250, "unload_mm": 60.,
            })
        self.display = display = E3V3SEDisplay.__new__(E3V3SEDisplay)
        display.printer = printer
        display.reactor = printer.reactor
        display.gcode = self.gcode
        display.extruder = self.extruder
        display.print_stats = printer.objects["print_stats"]
        display.ui = FakeUI()
        display.operation = display.bmcu_session = None
        display._resume_handler = None
        display.operation_timer = object()
        display.operation_message = ""
        display.operation_result = (True, "")
        display.heat_timeout = 300.
        display.material = "PLA"
        display.language = "en"
        display.macros = []
        display.max_hotend_temp = 260.
        display.preheat_pla_temps = (200, 60)
        display.preheat_petg_temps = (230, 70)
        display.preheat_tpu_temps = (230, 50)
        display.last_status = {"homed_axes": "xyz"}
        printer.objects["e3v3se_display"] = display
        for command in ("FILAMENT", "FILAMENT_CHECK", "FILAMENT_CANCEL"):
            name = "ENDER_" + command
            self.gcode.register_command(name, getattr(display, "cmd_" + name))
        self.gcode.register_command("M400", lambda cmd: self.wait())
        self.gcode.register_command("G4", lambda cmd: self.wait())
        printer.send_event("klippy:ready")

    def move(self, position, speed):
        if self.fail_move:
            raise CommandError("motion failure")
        if not self.extruder.get_status(0)["can_extrude"]:
            raise CommandError("cold extrusion")
        self.moves.append((position[3] - self.position[3], speed))
        self.position = list(position)

    def wait(self):
        if self.on_wait:
            self.on_wait()

    def heat(self):
        self.extruder.temperature = self.extruder.target

    def tick(self, time=1.):
        self.printer.reactor.now = time
        return self.display._operation_event(time)

    def add_bmcu(self, ready=True, loaded=None, uncertain=None):
        self.bmcu = {
            "package_version": "1.1.2", "active_operations": {},
            "loaded_tools": {"bmcu0:%d" % i: i for i in loaded or []},
            "last_error": "",
            "devices": [{"name": "bmcu0", "connected": ready,
                         "ready": ready, "runtime_configured": ready,
                         "now_channel": 255,
                         "loaded_channels": loaded or [],
                         "uncertain_channels": uncertain or []}],
        }
        self.bmcu_device = type("Device", (), {
            "connected": ready, "ready": ready,
            "runtime_configured": ready, "suspended": False,
            "firmware_compatible": True, "status_reconciled": True,
            "status": {"route_state": [2 if i in (uncertain or []) else
                                       1 if i in (loaded or []) else 0
                                       for i in range(4)]}})()
        self.printer.objects["bmcu"] = type("BMCU", (), {
            "get_status": lambda obj, time: self.bmcu,
            "devices_by_name": {"bmcu0": self.bmcu_device}})()
        settings = self.printer.objects["gcode_macro _ENDER_BMCU_SETTINGS"]
        settings.variables.update({"arrival_mode": "contact",
                                   "arrival_validated": 1,
                                   "max_route_mm": 1000.})


class FilamentOperationTest(unittest.TestCase):
    def setUp(self):
        self.h = FilamentHarness()
        self.display = self.h.display

    def test_manual_bmcu_controls_use_calibrated_integration(self):
        for channel in range(4):
            with self.subTest(channel=channel):
                h = FilamentHarness(calibrated=False)
                h.add_bmcu()
                calls = []
                h.gcode.register_command('BMCU_ENDER_MANUAL', lambda cmd:
                    calls.append(cmd.get_command_parameters()))
                self.assertTrue(h.display.bmcu_load(channel)[0])
                h.printer.reactor.callbacks.pop()(0.)
                self.assertEqual([{'ACTION': 'LOAD',
                                   'CHANNEL': str(channel + 1)}], calls)
                self.assertFalse(h.display.is_operation_busy())

    def test_prior_integration_still_reports_bmcu_available(self):
        self.h.add_bmcu()
        integration = types.SimpleNamespace()
        self.h.printer.objects['bmcu_ender_integration'] = integration
        self.assertTrue(self.display._bmcu_status(0.)['bmcu_available'])
        integration.manual_operation = 'LOAD'
        self.assertTrue(self.display._bmcu_status(0.)['bmcu_busy'])

    def test_manual_unload_without_loaded_channel_reaches_noop_handler(self):
        self.h.add_bmcu()
        calls = []
        self.h.gcode.register_command('BMCU_ENDER_MANUAL', lambda cmd:
            calls.append(cmd.get_command_parameters()))
        self.assertTrue(self.display.bmcu_unload()[0])
        self.h.printer.reactor.callbacks.pop()(0.)
        self.assertEqual([{'ACTION': 'UNLOAD'}], calls)
        self.assertEqual(0., self.h.extruder.target)

    def test_preheat_buttons_share_the_dashboard_macros(self):
        calls = []
        for material in ('PLA', 'PETG', 'TPU'):
            command = 'PREHEAT_' + material
            self.h.gcode.register_command(command, lambda cmd:
                calls.append(cmd.get_command()))
            preheat = getattr(self.display, 'preheat_' + material.lower())
            self.assertTrue(preheat()[0])
            self.assertEqual(material, self.display.material)
        self.assertEqual(['PREHEAT_PLA', 'PREHEAT_PETG', 'PREHEAT_TPU'], calls)

    def test_tpu_profile_is_available_without_bmcu_integration(self):
        calls = []
        self.display._execute = lambda text: (calls.append(text) or True, '')
        self.assertTrue(self.display.preheat_tpu()[0])
        self.assertIn('HEATER=extruder TARGET=230', calls[0])
        self.assertIn('HEATER=heater_bed TARGET=50', calls[0])
        self.assertEqual(230, self.display._filament_temperature())

    def test_cold_extrusion_waits_for_selected_material(self):
        self.display.material = "PETG"
        self.assertTrue(self.display.move_axis("E", 5)[0])
        self.assertEqual(230, self.h.extruder.target)
        self.h.tick()
        self.assertEqual([], self.h.moves)
        self.h.extruder.temperature = 180.
        self.h.tick(2.)
        self.assertEqual([], self.h.moves)
        self.h.heat()
        self.h.tick(3.)
        self.assertEqual([(5., 5.)], self.h.moves)
        self.assertIsNone(self.display.operation)

    def test_existing_adequate_target_is_kept_even_when_currently_cold(self):
        self.h.extruder.target = 245.
        self.display.load_filament()
        self.assertEqual(245., self.h.extruder.target)
        self.h.tick()
        self.assertEqual([], self.h.moves)
        self.h.heat()
        self.h.tick(2.)
        self.assertAlmostEqual(65., sum(mm for mm, speed in self.h.moves))
        self.assertTrue(all(abs(mm) <= 10 for mm, speed in self.h.moves))

    def test_hot_nozzle_without_target_still_selects_profile(self):
        self.h.extruder.temperature = 210.
        self.display.material = "PETG"
        self.display.unload_filament()
        self.h.tick()
        self.assertEqual(230., self.h.extruder.target)
        self.assertEqual([], self.h.moves)

    def test_calibrated_speeds_ignore_and_restore_print_speed_factor(self):
        for action in ("LOAD", "UNLOAD", "EXTRUDE"):
            for factor in (50, 200):
                with self.subTest(action=action, factor=factor):
                    h = FilamentHarness()
                    h.gcode.run_script("M220 S%d" % factor)
                    before = h.gmove.speed_factor
                    self.assertTrue(h.display._begin_operation(
                        action, delta=5)[0])
                    h.heat()
                    h.tick()
                    self.assertAlmostEqual(before, h.gmove.speed_factor)
                    expected = {"LOAD": {2., 3.},
                                "UNLOAD": {2., 3., 5., 10.},
                                "EXTRUDE": {5.}}[action]
                    self.assertEqual(expected, {speed for mm, speed in h.moves})

    def test_late_timer_does_not_execute_blocking_callback(self):
        self.display.load_filament()
        self.display.cancel_operation()
        reactor = self.h.printer.reactor
        self.assertEqual(reactor.NEVER,
                         reactor.timer_waketimes[self.display.operation_timer])
        seen = []
        def paused():
            self.h.heat()
            seen.append(self.display._operation_event(reactor.now))
        reactor.on_pause = paused
        self.h.gcode.run_script("ENDER_FILAMENT ACTION=LOAD WAIT=1")
        self.assertEqual([reactor.NEVER], seen)
        self.assertAlmostEqual(65., sum(mm for mm, speed in self.h.moves))
        self.assertIsNone(self.display.operation)

    def test_reentrant_timer_cannot_duplicate_motion(self):
        self.display.load_filament()
        self.h.on_wait = lambda: self.h.tick(2.)
        self.h.heat()
        self.h.tick()
        self.assertAlmostEqual(65., sum(mm for mm, speed in self.h.moves))

    def test_stale_mutex_waiter_cannot_move_or_finish_new_operation(self):
        self.display.load_filament()
        self.h.heat()
        old = self.display.operation
        @contextlib.contextmanager
        def replaced_mutex():
            self.display._finish_operation(False, "Operation cancelled", old)
            self.assertTrue(self.display.move_axis("E", 5)[0])
            yield
        with mock.patch.object(self.h.gcode, "get_mutex", replaced_mutex):
            self.assertEqual(1.25, self.h.tick())
        self.assertEqual([], self.h.moves)
        self.assertIsNotNone(self.display.operation)
        self.assertEqual("EXTRUDE", self.display.operation["action"])
        self.display._finish_operation(True, "", old)
        self.assertIsNotNone(self.display.operation)
        self.h.heat()
        self.h.tick(2.)
        self.assertEqual([(5., 5.)], self.h.moves)

    def test_resume_guard_runs_before_official_manual_refill_hook(self):
        calls = []
        def official_wrapper(gcmd):
            calls.append(("REFILL", gcmd.get("VELOCITY")))
            calls.append(("RESUME", gcmd.get("VELOCITY")))
        self.h.gcode.register_command("RESUME", official_wrapper)
        self.display._install_resume_guard(0.)
        self.display._install_resume_guard(0.)
        self.display.move_axis("E", 5)
        with self.assertRaises(CommandError):
            self.h.gcode.run_script("RESUME VELOCITY=25")
        self.assertEqual([], calls)
        self.display.cancel_operation()
        self.h.gcode.run_script("RESUME VELOCITY=25")
        self.assertEqual([("REFILL", "25"), ("RESUME", "25")], calls)

    def test_resume_guard_handles_refill_hook_installed_after_ready(self):
        calls = []
        self.h.gcode.register_command("RESUME", lambda cmd:
                                      calls.append("RESUME"))
        self.display._install_resume_guard(0.)
        previous = self.h.gcode.register_command("RESUME", None)
        def late_hook(gcmd):
            calls.append("REFILL")
            previous(gcmd)
        self.h.gcode.register_command("RESUME", late_hook)
        self.display.move_axis("E", 5)
        with self.assertRaises(CommandError):
            self.h.gcode.run_script("RESUME")
        self.assertEqual([], calls)
        self.display.cancel_operation()
        self.h.gcode.run_script("RESUME")
        self.assertEqual(["REFILL", "RESUME"], calls)

    def test_resume_guard_is_idempotent_with_real_dispatch(self):
        calls = []
        self.h.gcode.register_command("RESUME", lambda cmd:
                                      calls.append("RESUME"))
        self.display._install_resume_guard(0.)
        registered = self.h.gcode.ready_gcode_handlers["RESUME"]
        for _ in range(1000):
            self.display._install_resume_guard(0.)
        self.assertIs(registered, self.h.gcode.ready_gcode_handlers["RESUME"])
        self.h.gcode.run_script("RESUME")
        self.assertEqual(["RESUME"], calls)
        self.assertFalse(self.h.printer.shutdown)

    def test_repeated_clicks_do_not_enqueue_additional_motion(self):
        self.assertTrue(self.display.move_axis("E", 5)[0])
        self.assertFalse(self.display.move_axis("E", 5)[0])
        self.assertFalse(self.display.load_filament()[0])
        self.assertFalse(self.display.unload_filament()[0])
        self.h.heat()
        self.h.tick()
        self.assertEqual(1, len(self.h.moves))

    def test_cancel_heating_restores_previous_target_without_motion(self):
        self.h.extruder.target = 100.
        self.display.load_filament()
        self.display.cancel_operation()
        self.h.heat()
        self.h.tick()
        self.assertEqual(100., self.h.extruder.target)
        self.assertEqual([], self.h.moves)
        self.assertFalse(self.display.operation_result[0])

    def test_external_target_change_aborts_without_overwriting_it(self):
        self.display.load_filament()
        self.h.extruder.target = 0.
        self.h.tick()
        self.assertEqual([], self.h.moves)
        self.assertEqual(0., self.h.extruder.target)
        self.assertIn("changed", self.display.operation_result[1])

    def test_heating_timeout_aborts_and_clears_busy(self):
        self.display.load_filament()
        self.h.tick(301.)
        self.assertEqual([], self.h.moves)
        self.assertIsNone(self.display.operation)
        self.assertIn("timed out", self.display.operation_result[1])
        self.assertEqual(0., self.h.extruder.target)

    def test_load_unload_restore_real_klipper_extrusion_modes_and_flow(self):
        for command in ("LOAD_FILAMENT", "UNLOAD_FILAMENT"):
            for mode in ("M82", "M83"):
                with self.subTest(command=command, mode=mode):
                    self.h.gcode.run_script("G90\n%s\nG92 E123\nM221 S150"
                                            % mode)
                    self.h.gcode.run_script(command)
                    self.h.heat()
                    self.h.tick()
                    self.assertTrue(self.h.gmove.absolute_coord)
                    self.assertEqual(mode == "M82",
                                     self.h.gmove.allow_absolute_extrude)
                    self.assertEqual(1.5, self.h.gmove.extrude_factor)
                    position = self.h.gmove._get_gcode_position()
                    self.assertAlmostEqual(123., position[3])

    def test_unload_forms_tip_then_releases_full_calibrated_distance(self):
        self.display.unload_filament()
        self.h.heat()
        self.h.tick()
        moves = [mm for mm, speed in self.h.moves]
        self.assertGreater(moves[0], 0.)
        self.assertLess(moves[1], 0.)
        self.assertIn(3., moves[2:])
        self.assertAlmostEqual(-60., sum(moves))

    def test_motion_failure_restores_state_and_reports_error(self):
        self.h.gcode.run_script("M82\nG92 E123\nM221 S150")
        self.display.load_filament()
        self.h.heat()
        self.h.fail_move = True
        self.h.tick()
        self.assertTrue(self.h.gmove.allow_absolute_extrude)
        self.assertEqual(1.5, self.h.gmove.extrude_factor)
        self.assertFalse(self.display.operation_result[0])
        self.assertIn("failure", self.display.operation_result[1])

    def test_cancel_during_motion_stops_at_next_chunk_and_restores_state(self):
        self.display.load_filament()
        self.h.on_wait = self.display.cancel_operation
        self.h.heat()
        self.h.tick()
        self.assertEqual(1, len(self.h.moves))
        self.assertFalse(self.display.operation_result[0])
        self.assertTrue(self.h.gmove.allow_absolute_extrude)

    def test_heater_shutdown_during_motion_aborts_next_chunk(self):
        self.display.load_filament()
        self.h.on_wait = lambda: setattr(self.h.extruder, "target", 0.)
        self.h.heat()
        self.h.tick()
        self.assertEqual(1, len(self.h.moves))
        self.assertEqual(0., self.h.extruder.target)
        self.assertFalse(self.display.operation_result[0])

    def test_blocking_generic_callback_waits_and_finishes_before_return(self):
        self.h.add_bmcu()
        self.h.printer.reactor.on_pause = self.h.heat
        self.h.gcode.run_script("BMCU_TOOLHEAD_PREPARE MATERIAL=PETG")
        self.assertEqual(230., self.h.extruder.target)
        self.assertAlmostEqual(65., sum(mm for mm, speed in self.h.moves))
        self.assertIsNone(self.display.operation)

    def test_blocking_generic_callback_propagates_cancel(self):
        self.h.add_bmcu()
        self.h.printer.reactor.on_pause = self.display.cancel_operation
        with self.assertRaises(CommandError):
            self.h.gcode.run_script("BMCU_BEFORE_PULLBACK MATERIAL=PLA")
        self.assertEqual([], self.h.moves)
        self.assertIsNone(self.display.operation)

    def test_uncalibrated_configuration_blocks_before_heating(self):
        h = FilamentHarness(calibrated=False)
        self.assertFalse(h.display.load_filament()[0])
        self.assertEqual(0., h.extruder.target)
        self.assertEqual([], h.moves)

    def test_invalid_temperature_never_heats_or_extrudes(self):
        for value in (0, 100, 270, float("nan"), float("inf")):
            with self.subTest(value=value):
                self.assertFalse(self.display._begin_operation(
                    "EXTRUDE", temperature=value, delta=5)[0])
                self.assertEqual(0., self.h.extruder.target)
                self.assertEqual([], self.h.moves)

    def test_manual_filament_motion_requires_pause_while_printing(self):
        self.h.state = "printing"
        self.assertFalse(self.display.move_axis("E", 5)[0])
        self.assertFalse(self.display.load_filament()[0])
        self.assertEqual(0., self.h.extruder.target)

    def test_print_start_while_heating_cannot_trigger_late_extrusion(self):
        self.assertTrue(self.display.move_axis("E", 5)[0])
        self.assertFalse(self.display.start_print("part.gcode")[0])
        self.h.state = "printing"
        self.h.heat()
        self.h.tick()
        self.assertEqual([], self.h.moves)
        self.assertFalse(self.display.operation_result[0])

    def test_display_works_without_bmcu_and_blocks_offline_channels(self):
        self.assertFalse(self.display.bmcu_available())
        self.assertTrue(self.display.move_axis("E", 5)[0])
        self.display.cancel_operation()
        self.h.add_bmcu(ready=False)
        self.assertFalse(self.display.bmcu_load(0)[0])
        self.assertFalse(self.display.bmcu_unload()[0])
        self.assertTrue(self.display.move_axis("E", 5)[0])

    def test_active_channel_comes_from_confirmed_loaded_route(self):
        self.h.add_bmcu(loaded=[2])
        status = self.display._bmcu_status(0.)
        self.assertEqual(2, status["bmcu_channel"])
        self.assertEqual("Channel 3", status["bmcu_label"])
        self.h.bmcu_device.status["route_state"][2] = 2
        self.assertIsNone(self.display._bmcu_status(0.)["bmcu_channel"])
        self.assertFalse(self.display.bmcu_load(0)[0])

    def test_paused_bmcu_uses_runtime_routes_and_connection(self):
        self.h.add_bmcu(loaded=[0])
        self.h.state = "paused"
        self.h.bmcu["loaded_tools"] = {"bmcu0:3": 3}
        self.assertEqual(3, self.display._bmcu_status(0.)["bmcu_channel"])
        self.h.bmcu["loaded_tools"] = {}
        self.assertIsNone(self.display._bmcu_status(0.)["bmcu_channel"])
        self.h.bmcu_device.connected = False
        self.assertFalse(self.display.bmcu_available())
        self.assertFalse(self.display.bmcu_load(0)[0])
        self.h.bmcu_device.connected = True
        self.h.bmcu["last_error"] = "route failure"
        self.assertTrue(self.display._bmcu_status(0.)["bmcu_uncertain"])
        self.assertIsNone(self.display._bmcu_status(0.)["bmcu_channel"])
        self.assertFalse(self.display.bmcu_load(0)[0])

    def test_live_bmcu_state_overrides_frozen_public_snapshot(self):
        self.h.add_bmcu(loaded=[0])
        manager = self.h.printer.lookup_object("bmcu")
        manager.loaded_tools = {"bmcu0:3": 3}
        manager.active_operations = {"bmcu0": {"phase": "LOAD"}}
        manager.last_error = None
        self.h.bmcu_device.status["route_state"] = [0, 0, 0, 1]
        status = self.display._bmcu_status(0.)
        self.assertEqual(3, status["bmcu_channel"])
        self.assertTrue(status["bmcu_busy"])

    def test_incompatible_firmware_cannot_enable_display_actions(self):
        self.h.add_bmcu(loaded=[0])
        self.h.bmcu_device.firmware_compatible = False
        self.assertFalse(self.display.bmcu_available())
        self.assertFalse(self.display.bmcu_load(1)[0])

    def test_manual_runout_channel_is_visible_without_a_new_load_path(self):
        self.h.add_bmcu(loaded=[3])
        manager = self.h.printer.lookup_object("bmcu")
        pending = dict(device="bmcu0", channel=3, endpoint="extruder", tool=4)
        manager.refill = types.SimpleNamespace(manual_pending=pending)
        self.h.state = "paused"
        status = self.display._bmcu_status(0.)
        self.assertTrue(status.get("refill_pending"))
        self.assertEqual(3, status["refill_channel"])
        self.assertEqual("Refill channel 4", status["bmcu_label"])
        self.assertIs(pending, manager.refill.manual_pending)

    def test_bmcu_repeated_click_and_cancel_before_transport(self):
        self.h.add_bmcu()
        calls = []
        self.h.gcode.register_command("BMCU_LOAD", lambda cmd:
                                          calls.append("LOAD"))
        self.h.gcode.register_command("BMCU_STOP", lambda cmd:
                                          calls.append("STOP"))
        self.h.gcode._build_status_commands()
        self.assertTrue(self.display.bmcu_load(0)[0])
        self.assertFalse(self.display.bmcu_load(1)[0])
        self.display.cancel_operation()
        self.h.printer.reactor.callbacks.pop()(0.)
        self.assertEqual(["STOP"], calls)
        self.assertFalse(self.display.is_operation_busy())

    def test_bmcu_error_does_not_mark_requested_channel_as_loaded(self):
        self.h.add_bmcu()
        def fail(cmd):
            raise CommandError("BMCU disconnected")
        self.h.gcode.register_command("BMCU_LOAD", fail)
        self.assertTrue(self.display.bmcu_load(3)[0])
        self.h.printer.reactor.callbacks.pop()(0.)
        self.assertIsNone(self.display._bmcu_status(0.)["bmcu_channel"])
        self.assertIn("disconnected", self.display.ui.messages[-1][1])

    def test_bmcu_transport_and_callback_finish_before_unlocking_screen(self):
        self.h.add_bmcu()
        self.h.printer.reactor.on_pause = self.h.heat
        seen = []
        def load(cmd):
            seen.append(self.display.is_operation_busy())
            self.h.gcode.run_script_from_command(
                "BMCU_TOOLHEAD_PREPARE MATERIAL=PLA")
            self.assertTrue(self.display.is_operation_busy())
            self.h.bmcu["devices"][0]["loaded_channels"] = [1]
            self.h.bmcu["loaded_tools"] = {"bmcu0:1": 1}
        self.h.gcode.register_command("BMCU_LOAD", load)
        self.assertTrue(self.display.bmcu_load(1)[0])
        self.h.printer.reactor.callbacks.pop()(0.)
        self.assertEqual([True], seen)
        self.assertEqual(1, self.display._bmcu_status(0.)["bmcu_channel"])
        self.assertFalse(self.display.is_operation_busy())

    def test_cancelled_transport_cannot_start_the_toolhead_callback(self):
        self.h.add_bmcu()
        def load(cmd):
            self.display.cancel_operation()
            self.h.gcode.run_script_from_command(
                "BMCU_TOOLHEAD_PREPARE MATERIAL=PLA")
        self.h.gcode.register_command("BMCU_LOAD", load)
        self.h.gcode.register_command("BMCU_STOP", lambda cmd: None)
        self.display.bmcu_load(0)
        self.h.printer.reactor.callbacks.pop()(0.)
        self.assertEqual([], self.h.moves)
        self.assertEqual(0., self.h.extruder.target)
        self.assertFalse(self.display.is_operation_busy())


class DisplayLanguageTest(unittest.TestCase):
    def make_ui(self, language):
        h = FilamentHarness()
        h.display.language = language
        transport = FakeTransport()
        ui = DisplayUI(TJC3224(transport), h.display)
        h.display.ui = ui
        return h, ui, transport

    def text_frames(self, transport):
        return [frame[13:-4].decode("utf-8") for frame in transport.frames
                if frame[1] == TJC3224.CMD_DRAW_TEXT]

    def test_menu_actions_have_one_owner_in_both_languages(self):
        for language in ('en', 'pt_BR'):
            h, ui, transport = self.make_ui(language)
            h.display.macros = []
            ui.show_main_menu()
            self.assertIn(translate('Filament', language),
                          [item.label for item in ui.items])
            self.assertNotIn(translate('Prepare', language),
                             [item.label for item in ui.items])
            self.assertIn(translate('Settings', language),
                          [item.label for item in ui.items])
            ui.show_filament_menu()
            labels = [item.label for item in ui.items]
            for material in ('PLA', 'PETG', 'TPU'):
                self.assertNotIn(translate('Preheat ' + material, language),
                                 labels)
            self.assertIn(translate('Unload filament', language), labels)
            self.assertNotIn(translate('Cooldown', language), labels)
            ui.show_temperature_menu()
            labels = [item.label for item in ui.items]
            for material in ('PLA', 'PETG', 'TPU'):
                self.assertIn(translate('Preheat ' + material, language),
                              labels)
            self.assertIn(translate('Cooldown', language), labels)
            ui.show_move_menu()
            labels = [item.label for item in ui.items]
            self.assertIn(translate('Home all', language), labels)
            self.assertIn(translate('Disable motors', language), labels)
            self.assertNotIn(translate('Load filament', language), labels)

    def test_reset_requires_confirmation_and_is_not_offered_during_print(self):
        for language in ('en', 'pt_BR'):
            h, ui, _ = self.make_ui(language)
            h.add_bmcu(uncertain=[0])
            calls = []
            h.gcode.register_command('BMCU_ENDER_RESET', lambda cmd:
                calls.append(cmd.get_command_parameters()))
            ui.show_filament_menu()
            label = translate('Reset BMCU', language)
            reset = next((item for item in ui.items if item.label == label),
                         None)
            self.assertIsNotNone(reset)
            self.assertTrue(reset.enabled)
            reset.callback()
            self.assertEqual([], calls)
            ui.items[1].callback()
            self.assertEqual([], calls)
            ui.show_filament_menu()
            next(item for item in ui.items if item.label == label).callback()
            ui.items[0].callback()
            self.assertEqual([{'CONFIRM': '1'}], calls)
            h.state = 'paused'
            self.assertFalse(h.display.bmcu_reset()[0])
            self.assertEqual(1, len(calls))
            ui.show_filament_menu()
            reset = next((item for item in ui.items if item.label == label),
                         None)
            self.assertTrue(reset is None or not reset.enabled)

    def test_pending_recovery_uses_its_failed_channel_not_a_global_reset(self):
        h, ui, _ = self.make_ui('en')
        h.add_bmcu(uncertain=[2])
        h.state = 'paused'
        integration = {'recovery_pending': True, 'recovery_channel': 2,
                       'recovery_tool': 1, 'recovery_error': 'arrival failed'}
        h.printer.objects['bmcu_ender_integration'] = types.SimpleNamespace(
            get_status=lambda time: integration)
        calls = []
        h.gcode.register_command('BMCU_CONFIRM_CLEAR_PATH', lambda cmd:
            calls.append(cmd.get_command_parameters()))
        ui.status = {'print_state': 'paused', 'paused': True}
        ui.show_main_menu()
        recovery = next((item for item in ui.items
                         if item.label == 'Recover filament'), None)
        self.assertIsNotNone(recovery)
        recovery.callback()
        self.assertNotIn('Reset BMCU', [item.label for item in ui.items])
        confirmation = next(item for item in ui.items
                            if item.label == 'Confirm clear path')
        confirmation.callback()
        self.assertEqual([], calls)
        ui.items[0].callback()
        self.assertEqual([{'CHANNEL': '3', 'CONFIRM': '1'}], calls)
        self.assertFalse(h.display.bmcu_reset()[0])
        integration['recovery_channel'] = -1
        self.assertFalse(h.display.confirm_bmcu_path()[0])
        self.assertEqual(1, len(calls))

    def test_language_applies_now_and_stages_existing_config_save(self):
        h, ui, _ = self.make_ui('en')
        configfile = mock.Mock()
        h.printer.objects['configfile'] = configfile
        self.assertTrue(hasattr(ui, 'show_settings_menu'))
        ui.show_settings_menu()
        language = next(item for item in ui.items if item.label == 'Language')
        language.callback()
        next(item for item in ui.items if item.label == 'Português').callback()
        self.assertEqual('pt_BR', h.display.language)
        self.assertEqual('pt_BR', ui.language)
        self.assertIn('Idioma', [item.label for item in ui.items])
        configfile.set.assert_called_once_with(
            'e3v3se_display', 'language', 'pt_BR')
        self.assertEqual([], h.moves)
        self.assertFalse(h.printer.shutdown)
        self.assertFalse(h.display.set_language('invalid')[0])
        self.assertEqual('pt_BR', h.display.language)

    def test_default_display_macros_do_not_duplicate_builtin_controls(self):
        path = os.path.join(ROOT, 'config/ender3-v3-se/display.cfg')
        cfg = ConfigFileReader().build_fileconfig(
            ConfigFileReader().read_config_file(path), path)
        self.assertFalse(any(name.startswith('e3v3se_display macro ')
                             for name in cfg.sections()))

    def test_new_public_cutter_diagnostics_are_localized_including_floats(self):
        self.assertEqual(
            'O sensor de entrada ainda detecta filamento.',
            translate('The entry sensor still detects filament.', 'pt_BR'))
        self.assertEqual(
            'Servo comandado a 12.5 graus; confira a posição física.',
            translate('Servo commanded to 12.5 degrees; '
                      'check its physical position.', 'pt_BR'))
        for source in ('Reset BMCU', 'RECOVERY', 'Settings', 'Language',
                       'Confirm clear path', 'Failure details'):
            self.assertIn(source, translations.PORTUGUESE)

    def test_recovery_translation_preserves_the_sensor_side(self):
        source = ('Physically clear the shared path and park this filament '
                  'behind the entry sensor before CONFIRM=1.')
        self.assertEqual(
            'Desobstrua fisicamente o caminho compartilhado e estacione este '
            'filamento antes do sensor de entrada; só então use CONFIRM=1.',
            translate(source, 'pt_BR'))
        self.assertEqual('O canal deve estar EMPTY antes da medição.',
                         translate('The channel must be EMPTY before '
                                   'measurement.', 'pt_BR'))
        self.assertEqual('A posição foi perdida; a recuperação está bloqueada.',
                         translate('Position was lost; print recovery '
                                   'is blocked.', 'pt_BR'))

    def test_unload_error_localizes_nested_cause_and_numeric_evidence(self):
        self.assertEqual(
            'Retirada interrompida: A assistência de retirada do alimentador '
            'parou. Canal 4; buffer 50%; PWM 0; movimento 0. '
            'Total comandado líquido 12.50 mm.',
            translate('Unload interrupted: Feeder withdrawal assistance '
                      'stopped. Channel 4; buffer 50%; PWM 0; motion 0. '
                      'Total commanded net 12.50 mm.', 'pt_BR'))

    def test_catalogue_templates_keep_numeric_types_and_unknown_details(self):
        token = r'%(?:\.[0-9]+)?[sdf]|%%'
        for source, target in translations.PORTUGUESE.items():
            kinds = translations.re.findall(token, source)
            if not kinds:
                continue
            with self.subTest(source=source):
                self.assertEqual(kinds, translations.re.findall(token, target))
                values = tuple('EXTERNAL_DETAIL' if kind == '%s'
                               else 4 if kind.endswith('d') else 12.5
                               for kind in kinds if kind != '%%')
                rendered = source % values
                self.assertEqual(target % values,
                                 translate(rendered, 'pt_BR'))
                self.assertEqual(rendered, translate(rendered, 'en'))

    def test_catalogue_has_no_silently_overwritten_duplicate_keys(self):
        path = os.path.join(
            ROOT, 'klippy/extras/e3v3se_display/translations.py')
        with open(path, encoding='utf-8') as stream:
            tree = ast.parse(stream.read())
        catalogue = next(node.value for node in tree.body
                         if isinstance(node, ast.Assign)
                         and any(isinstance(target, ast.Name)
                                 and target.id == 'PORTUGUESE'
                                 for target in node.targets))
        keys = [ast.literal_eval(key) for key in catalogue.keys]
        self.assertEqual(len(keys), len(set(keys)))

    def test_paused_cutter_recovery_can_only_retract_after_confirmation(self):
        h, ui, _ = self.make_ui('en')
        h.add_bmcu(uncertain=[0])
        h.state = 'paused'
        h.printer.objects['bmcu_ender_integration'] = types.SimpleNamespace(
            get_status=lambda time: {'recovery_pending': True,
                                    'recovery_channel': 0,
                                    'cutter_uncertain': True})
        h.display.cortador_configurado = lambda: object()
        calls = []
        h.gcode.register_command('CUTTER_SERVO', lambda cmd:
            calls.append(cmd.get_command_parameters()))
        ui.show_recovery_menu()
        retract = next((item for item in ui.items
                        if item.label == 'Close cutter stop'), None)
        self.assertIsNotNone(retract)
        self.assertNotIn('Open cutter stop', [item.label for item in ui.items])
        retract.callback()
        self.assertEqual([], calls)
        ui.items[0].callback()
        self.assertEqual([{'POSITION': 'RETRACTED', 'CONFIRM': '1'}], calls)
        self.assertEqual('RECOVERY', ui.title)
        self.assertNotIn('Home all', [item.label for item in ui.items])

    def test_reset_and_save_recheck_busy_state_after_confirmation(self):
        h, ui, _ = self.make_ui('en')
        h.add_bmcu()
        calls = []
        h.gcode.register_command('BMCU_ENDER_RESET', lambda cmd:
                                calls.append('RESET'))
        h.gcode.register_command('SAVE_CONFIG', lambda cmd:
                                calls.append('SAVE'))
        for action in (ui.confirm_bmcu_reset, ui.confirm_save_config):
            h.state = 'standby'
            action()
            h.state = 'printing'
            ui.items[0].callback()
            self.assertEqual([], calls)
        h.state = 'standby'
        h.display.bmcu_session = {'cancelled': False}
        self.assertFalse(h.display.bmcu_reset()[0])
        self.assertFalse(h.display.save_config()[0])
        self.assertEqual([], calls)

    def test_save_rechecks_bmcu_work_started_outside_the_screen(self):
        for operation in ('native', 'manual'):
            with self.subTest(operation=operation):
                h, ui, _ = self.make_ui('en')
                h.add_bmcu()
                calls = []
                h.gcode.register_command('SAVE_CONFIG', lambda cmd:
                                        calls.append('SAVE'))
                integration = types.SimpleNamespace(manual_operation=None)
                h.printer.objects['bmcu_ender_integration'] = integration
                ui.confirm_save_config()
                if operation == 'native':
                    h.bmcu['active_operations'] = {'extruder': 'LOAD'}
                else:
                    integration.manual_operation = 'LOAD'
                self.assertFalse(h.display.is_operation_busy())
                self.assertTrue(h.display._bmcu_status(0.)['bmcu_busy'])
                ui.items[0].callback()
                self.assertEqual([], calls)
                h.bmcu['active_operations'] = {}
                integration.manual_operation = None
                ui.confirm_save_config()
                ui.items[0].callback()
                self.assertEqual(['SAVE'], calls)

    def test_batente_manual_exige_confirmacao_mas_homing_envia_so_g28(self):
        h, ui, _ = self.make_ui("pt_BR")
        ui.show_move_menu()
        self.assertNotIn("Fechar batente", [item.label for item in ui.items])
        h.display.cortador_configurado = lambda: types.SimpleNamespace(
            recolhido=False)
        comandos = []
        h.display._execute = lambda texto: (comandos.append(texto) or True, "")
        for rotulo, posicao in (("Abrir batente", "DEPLOYED"),
                                ("Fechar batente", "RETRACTED")):
            ui.show_move_menu()
            menu = next(item for item in ui.items if item.label == rotulo)
            total = len(comandos)
            menu.callback()
            self.assertEqual(total, len(comandos))
            ui.items[0].callback()
            self.assertEqual(
                "CUTTER_SERVO POSITION=%s CONFIRM=1" % posicao, comandos[-1])
        ui.solicitar_homing()
        self.assertEqual(3, len(comandos))
        self.assertEqual("G28", comandos[-1])

    def test_default_language_and_configured_portuguese(self):
        path = os.path.join(
            ROOT, "config", "printer-creality-ender3-v3-se-gd32f303-2023.cfg")
        reader = ConfigFileReader()
        cfg = reader.build_fileconfig_with_includes(
            reader.read_config_file(path), path)
        for language in ("en", "pt_BR"):
            printer = FakePrinter()
            printer.objects["gcode"] = GCodeDispatch(printer)
            for name in ("virtual_sdcard", "pause_resume", "display_status",
                         "manual_probe"):
                printer.objects[name] = mock.Mock()
            if language == "pt_BR":
                cfg.set("e3v3se_display", "language", language)
            config = ConfigWrapper(printer, cfg, {}, "e3v3se_display")
            keys = types.ModuleType("extras.display.menu_keys")
            keys.MenuKeys = mock.Mock()
            with mock.patch("extras.e3v3se_display.DisplayTransport"), \
                    mock.patch.dict(sys.modules,
                                    {"extras.display.menu_keys": keys}):
                display = E3V3SEDisplay(config)
            self.assertEqual(language, display.language)
            self.assertEqual(language, display.ui.language)

    def test_english_and_portuguese_menus_and_operation_text(self):
        for language, menu, heating in (
                ("en", "MOVE", "Heating nozzle"),
                ("pt_BR", "MOVER", "Aquecendo bico")):
            with self.subTest(language=language):
                h, ui, transport = self.make_ui(language)
                ui.show_move_menu()
                self.assertIn(menu, self.text_frames(transport))
                self.assertTrue(h.display.move_axis("E", 5)[0])
                self.assertIn(heating, self.text_frames(transport))
                ui.handle_key("click")
                self.assertIsNone(h.display.operation)
                self.assertEqual([], h.moves)

    def test_dynamic_state_channel_error_and_utf8_limits(self):
        h, ui, transport = self.make_ui("pt_BR")
        ui.status = {"print_state": "paused", "bmcu_label": "Channel 4",
                     "hotend_temp": 200, "hotend_target": 230}
        ui.show_dashboard()
        texts = self.text_frames(transport)
        self.assertIn("Pausada", texts)
        self.assertIn("Canal 4", texts)
        ui.show_message("FILAMENT FAILED", "Heating timed out")
        self.assertIn("FALHA NO FILAMENTO", self.text_frames(transport))
        for frame in transport.frames:
            if frame[1] == TJC3224.CMD_DRAW_TEXT:
                self.assertLessEqual(len(frame[13:-4]), 29)
                frame[13:-4].decode("utf-8", "strict")
        self.assertEqual("Referencie X antes de mover",
                         translate("Home X before moving it", "pt_BR"))
        self.assertEqual("Custom diagnostic",
                         translate("Custom diagnostic", "pt_BR"))

    def test_offline_bmcu_menu_does_not_execute_channel_actions(self):
        h, ui, transport = self.make_ui("en")
        h.add_bmcu(ready=False)
        ui.show_bmcu_menu()
        self.assertFalse(any(item.enabled for item in ui.items[:5]))
        ui.handle_key("click")
        self.assertFalse(h.display.is_operation_busy())
        self.assertIn("BMCU unavailable", self.text_frames(transport))

    def test_legacy_escaped_placeholders_and_unicode_translation(self):
        original_escape = translations.re.escape
        def legacy_escape(value):
            return original_escape(value).replace("%", "\\%")
        with mock.patch.object(translations.re, "escape", legacy_escape):
            patterns = translations._build_patterns()
        with mock.patch.object(translations, "_PATTERNS", patterns):
            self.assertEqual("Canal 4", translate(b"Channel 4", "pt_BR"))
            self.assertEqual("Referencie X antes de mover",
                             translate("Home X before moving it", "pt_BR"))
            accented = "Impress\u00e3o iniciada; opera\u00e7\u00e3o abortada"
            self.assertEqual(accented, translate(
                "Print started; operation aborted", "pt_BR"))
            self.assertEqual(accented, translate(accented, "pt_BR"))
            self.assertEqual(accented, translate(
                accented.encode("utf-8"), "pt_BR"))


class ConfigLayoutTest(unittest.TestCase):
    def test_language_can_be_saved_without_an_include_conflict(self):
        config_path = os.path.join(
            ROOT, 'config', 'printer-creality-ender3-v3-se-gd32f303-2023.cfg')
        reader = ConfigFileReader()
        data = reader.read_config_file(config_path)
        autosave = reader.build_fileconfig(
            '[e3v3se_display]\nlanguage: pt_BR\n', '*AUTOSAVE*')
        helper = ConfigAutoSave.__new__(ConfigAutoSave)
        helper.fileconfig = autosave
        helper.printer = FakePrinter()
        regular = helper._strip_duplicates(data, autosave)
        cfg = reader.build_fileconfig_with_includes(regular, config_path)
        helper._disallow_include_conflicts(cfg)
        self.assertFalse(cfg.has_option('e3v3se_display', 'language'))

    def test_autosave_values_do_not_conflict_with_includes(self):
        config_path = os.path.join(
            ROOT, "config",
            "printer-creality-ender3-v3-se-gd32f303-2023.cfg")
        reader = ConfigFileReader()
        regular_data = reader.read_config_file(config_path)
        autosave = reader.build_fileconfig(
            "[extruder]\ncontrol: pid\n"
            "[heater_bed]\ncontrol: pid\n"
            "[bltouch]\nz_offset: 1.25\n", "*AUTOSAVE*")
        helper = ConfigAutoSave.__new__(ConfigAutoSave)
        stripped = helper._strip_duplicates(regular_data, autosave)
        merged = reader.build_fileconfig_with_includes(stripped, config_path)
        self.assertFalse(merged.has_option("extruder", "control"))
        self.assertFalse(merged.has_option("heater_bed", "control"))
        self.assertFalse(merged.has_option("bltouch", "z_offset"))


if __name__ == "__main__":
    unittest.main()
