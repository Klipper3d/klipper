import copy
import io
import re
import types
import unittest

from test_integracao import Gcmd, ROOT, import_module, protocol
from test_manual_controls import ManualBench


virtual_sd = import_module(
    'recovery_virtual_sd', ROOT / 'klippy/extras/virtual_sdcard.py'
)
pause_resume = import_module(
    'recovery_pause_resume', ROOT / 'klippy/extras/pause_resume.py'
)
gcode_move = import_module(
    'recovery_gcode_move', ROOT / 'klippy/extras/gcode_move.py'
)


class RecoveryCommand(Gcmd):
    def get_float(self, name, default=None, **limits):
        return float(self.params.get(name, default))

    def respond_raw(self, message):
        self.messages.append(message)


class PrintBench(ManualBench):
    def __init__(self):
        super().__init__(hot=True)
        self.fail_change = True
        self.print_state = 'printing'
        self.target = 230.0
        self.obj.pending_tool_change = None
        self.obj.recovering_tool_change = False
        self.reactor.NOW = 0.0
        self.reactor.NEVER = 1e30
        self.reactor.unregister_timer = lambda timer: None
        self.reactor.register_timer = lambda *args: object()
        self.move = gcode_move.GCodeMove.__new__(gcode_move.GCodeMove)
        self.move.saved_states = {}
        self.move.Coord = tuple
        self.move.axis_map = {'X': 0, 'Y': 1, 'Z': 2, 'E': 3}
        self.move.absolute_coord = True
        self.move.allow_absolute_extrude = True
        self.move.base_position = [0.0] * 4
        self.move.last_position = self.position
        self.move.homing_position = [0.0] * 4
        self.move.speed = 10.0
        self.move.speed_factor = self.move.extrude_factor = 1.0
        self.move.extra_axes = []
        self.move.move_with_transform = self.restore_position
        self.stats = types.SimpleNamespace(
            state='printing',
            get_status=lambda t: {'state': self.stats.state},
            note_start=lambda: self.set_state('printing'),
            note_pause=lambda: self.set_state('paused'),
            note_error=lambda error: self.set_state('error'),
            note_complete=lambda: self.set_state('complete'),
            note_cancel=lambda: self.set_state('cancelled'),
        )
        self.gcode.run_script = self.execute
        self.gcode.get_mutex = lambda: types.SimpleNamespace(
            test=lambda: False
        )
        self.gcode.error = RuntimeError
        self.gcode.respond_raw = lambda message: None
        self.sd = virtual_sd.VirtualSD.__new__(virtual_sd.VirtualSD)
        self.sd.printer = self.printer
        self.sd.reactor = self.reactor
        self.sd.gcode = self.gcode
        self.sd.print_stats = self.stats
        self.sd.current_file = io.StringIO(
            'BMCU_CHANGE_TOOL_ENDER TOOL=1\nG1 X101 E1 F600\n'
        )
        self.sd.file_size = len(self.sd.current_file.getvalue())
        self.sd.file_position = self.sd.next_file_position = 0
        self.sd.must_pause_work = self.sd.cmd_from_sd = False
        self.sd.work_timer = object()
        self.sd.on_error_gcode = types.SimpleNamespace(
            render=lambda: 'CANCEL_PRINT'
        )
        self.pause_control = pause_resume.PauseResume.__new__(
            pause_resume.PauseResume
        )
        self.pause_control.printer = self.printer
        self.pause_control.gcode = self.gcode
        self.pause_control.v_sd = self.sd
        self.pause_control.is_paused = False
        self.pause_control.sd_paused = False
        self.pause_control.pause_command_sent = False
        self.pause_control.recover_velocity = 50.0
        self.bmcu._resolve_tool = lambda tool: (
            self.device,
            tool - 1,
            self.endpoint,
        )
        self.bmcu._uncertain_routes = set()
        self.bmcu.loaded_tools = {}
        self.bmcu.print_tools = {'1': {'device': 'bmcu0', 'channel': 0}}
        self.bmcu._route_key = lambda device, channel: str(channel)
        self.bmcu._route_states_from_status = lambda status: status[
            'route_state'
        ]
        self.bmcu._durable_tail_route = lambda *args: None
        self.bmcu._lock = lambda *args, **kwargs: None
        self.bmcu._unlock = lambda *args: None
        self.bmcu._unmark_print_route = lambda *args: None
        self.bmcu._save_runtime = lambda: None
        self.bmcu.device_status_changed = lambda *args: None
        refresh = self.device.refresh
        self.device.refresh = lambda: (refresh(), self.device.status)[1]
        self.device.mark_unloaded = lambda channel: self.device.status[
            'route_state'
        ].__setitem__(channel, protocol.ROUTE_EMPTY)
        self.bmcu.get_status = lambda t: {
            'loaded_tool': next(
                (
                    ch + 1
                    for ch in range(4)
                    if self.device.status['route_state'][ch]
                    == protocol.ROUTE_LOADED
                ),
                -1,
            )
        }
        self.obj.wipe = lambda *args: self.commands.append('PURGA_CONFIRMADA')

    def lookup(self, name, default=None):
        if name == 'print_stats' and hasattr(self, 'stats'):
            return self.stats
        if name == 'virtual_sdcard':
            return getattr(self, 'sd', None)
        if name == 'pause_resume':
            return getattr(self, 'pause_control', None)
        if name == 'gcode_move' and hasattr(self, 'move'):
            return self.move
        return super().lookup(name, default)

    def set_state(self, state):
        self.stats.state = state
        self.print_state = state

    def restore_position(self, position, speed):
        self.position[:] = position

    def execute(self, text):
        if text.startswith('BMCU_CHANGE_TOOL_ENDER '):
            params = dict(re.findall(r'(\w+)=(\S+)', text))
            return self.obj.change_tool(RecoveryCommand(**params))
        if text.startswith('BMCU_TOOL_CHANGE '):
            self.commands.append(text)
            if self.fail_change:
                self.position[:3] = [-6.0, 123.0, 7.0]
                raise RuntimeError('arrival interrupted')
            tool = int(text.split('TOOL=')[1])
            self.device.status['route_state'] = [protocol.ROUTE_EMPTY] * 4
            self.device.status['route_state'][tool - 1] = protocol.ROUTE_LOADED
            self.present = True
            self.position[3] += 80.0
            return
        if text.startswith('M109 '):
            self.target = self.temperature = float(text.split('S')[1])
        if text == 'CANCEL_PRINT':
            self.obj.cancel_recovery(self.pause_control.cmd_CANCEL_PRINT)(
                RecoveryCommand()
            )
            return
        super().execute(text)
        for line in text.splitlines():
            if line.startswith('SAVE_GCODE_STATE '):
                self.move.cmd_SAVE_GCODE_STATE(
                    RecoveryCommand(NAME=line.split('NAME=')[1])
                )
            if line.startswith('RESTORE_GCODE_STATE '):
                params = dict(re.findall(r'(\w+)=(\S+)', line))
                self.move.cmd_RESTORE_GCODE_STATE(RecoveryCommand(**params))

    def start_print(self):
        self.sd.work_handler(0.0)

    def resume(self):
        return self.obj.resume_print(self.pause_control.cmd_RESUME)(
            RecoveryCommand()
        )


class PrintRecoveryTests(unittest.TestCase):
    def test_virtual_sd_pauses_without_cancelling_or_executing_next_line(self):
        b = PrintBench()
        original = list(b.position)
        b.start_print()
        self.assertEqual('paused', b.stats.state)
        self.assertIsNotNone(b.sd.current_file)
        self.assertTrue(b.pause_control.is_paused)
        self.assertNotIn('G1 X101 E1 F600', b.commands)
        self.assertEqual(
            original, b.move.saved_states['PAUSE_STATE']['last_position']
        )
        self.assertEqual(1, b.obj.pending_tool_change['tool'])
        self.assertEqual(230.0, b.target)
        self.assertNotIn('CANCEL_PRINT', b.commands)

    def test_resume_finishes_pending_change_before_next_print_move(self):
        b = PrintBench()
        b.start_print()
        b.fail_change = False
        b.resume()
        self.assertFalse(b.pause_control.is_paused)
        self.assertIsNone(b.obj.pending_tool_change)
        self.assertEqual([100, 110, 5], b.position[:3])
        self.assertNotIn('G1 X101 E1 F600', b.commands)
        b.sd.work_handler(0.0)
        self.assertEqual('complete', b.stats.state)
        self.assertIn('G1 X101 E1 F600', b.commands)
        self.assertEqual(2, b.commands.count('BMCU_TOOL_CHANGE TOOL=1'))

    def test_failed_recovery_keeps_print_paused_and_pending(self):
        b = PrintBench()
        b.start_print()
        position = b.sd.file_position
        with self.assertRaisesRegex(RuntimeError, 'arrival interrupted'):
            b.resume()
        self.assertEqual(position, b.sd.file_position)
        self.assertTrue(b.pause_control.is_paused)
        self.assertFalse(b.sd.is_active())
        self.assertIsNotNone(b.obj.pending_tool_change)
        self.assertNotIn('G1 X101 E1 F600', b.commands)

    def test_uncertain_or_incoherent_route_blocks_resume_before_any_motion(
        self,
    ):
        for condition in ('uncertain', 'sensor'):
            b = PrintBench()
            b.start_print()
            if condition == 'uncertain':
                b.device.status['route_state'][0] = protocol.ROUTE_UNCERTAIN
            else:
                b.present = True
            before = list(b.commands)
            with self.assertRaises(RuntimeError):
                b.resume()
            self.assertEqual(before, b.commands)
            self.assertTrue(b.pause_control.is_paused)

    def test_file_homing_or_offset_change_cannot_resume_wrong_context(self):
        for condition in ('file', 'homing', 'offset', 'session'):
            b = PrintBench()
            b.start_print()
            if condition == 'file':
                b.sd.current_file = io.StringIO('another print')
            elif condition == 'homing':
                b.homed_axes = ''
            elif condition == 'session':
                b.device.status['session_id'] += 1
            else:
                b.move.homing_position[2] = 0.1
            with self.assertRaises(RuntimeError):
                b.resume()
            self.assertTrue(b.pause_control.is_paused)

    def test_cancel_discards_pending_change_and_closes_print_file(self):
        b = PrintBench()
        b.start_print()
        b.execute('CANCEL_PRINT')
        self.assertIsNone(b.obj.pending_tool_change)
        self.assertIsNone(b.sd.current_file)
        self.assertEqual('cancelled', b.stats.state)

    def test_loaded_target_repeats_cleaning_before_resuming(self):
        b = PrintBench()
        b.start_print()
        b.device.status['route_state'][0] = protocol.ROUTE_LOADED
        b.present = True
        b.fail_change = False
        b.resume()
        self.assertIn('PURGA_CONFIRMADA', b.commands)

    def test_different_print_temperature_is_restored_after_recovery(self):
        b = PrintBench()
        b.target = b.temperature = 210.0
        b.start_print()
        b.fail_change = False
        b.resume()
        self.assertEqual(210.0, b.target)

    def test_manual_load_error_still_propagates(self):
        b = PrintBench()
        b.sd.cmd_from_sd = False
        with self.assertRaisesRegex(RuntimeError, 'arrival interrupted'):
            b.obj.change_tool(RecoveryCommand(TOOL=1))
        self.assertIsNone(b.obj.pending_tool_change)

    def test_recovery_preserves_absolute_and_relative_extrusion_coordinates(
        self,
    ):
        for absolute in (True, False):
            b = PrintBench()
            b.move.allow_absolute_extrude = absolute
            b.position[3] = 125.0
            b.start_print()
            b.fail_change = False
            b.resume()
            self.assertAlmostEqual(125.0, b.move._get_gcode_position()[3])
            physical = b.position[3]
            b.move.cmd_G1(RecoveryCommand(E=126 if absolute else 1, F=600))
            self.assertAlmostEqual(physical + 1.0, b.position[3])

    def test_motor_disable_or_rehome_invalidates_recovery(self):
        b = PrintBench()
        b.start_print()
        b.obj.invalidate_recovery()
        with self.assertRaisesRegex(RuntimeError, 'paused print changed'):
            b.resume()
        self.assertTrue(b.pause_control.is_paused)

    def test_explicit_clear_path_confirmation_preserves_print_mapping(self):
        b = PrintBench()
        b.start_print()
        b.obj.pending_tool_change['failed_channel'] = 0
        b.device.status['route_state'][0] = protocol.ROUTE_UNCERTAIN
        b.bmcu._uncertain_routes.add('0')
        original = copy.deepcopy(b.bmcu.print_tools)
        b.obj.confirm_recovery_path(RecoveryCommand(CHANNEL=1, CONFIRM=1))
        self.assertEqual(original, b.bmcu.print_tools)
        self.assertFalse(b.bmcu._uncertain_routes)
        self.assertTrue(b.pause_control.is_paused)
        b.fail_change = False
        b.resume()
        self.assertFalse(b.pause_control.is_paused)

    def test_clear_path_requires_sensor_clear_exclusivity_and_confirmation(
        self,
    ):
        for condition in ('sensor', 'other', 'confirmation'):
            b = PrintBench()
            b.start_print()
            b.obj.pending_tool_change['failed_channel'] = 0
            b.device.status['route_state'][0] = protocol.ROUTE_UNCERTAIN
            if condition == 'sensor':
                b.present = True
            if condition == 'other':
                b.device.status['route_state'][2] = protocol.ROUTE_LOADED
            with self.assertRaises(RuntimeError):
                b.obj.confirm_recovery_path(
                    RecoveryCommand(
                        CHANNEL=1,
                        CONFIRM=0 if condition == 'confirmation' else 1,
                    )
                )
            self.assertEqual(
                protocol.ROUTE_UNCERTAIN, b.device.status['route_state'][0]
            )
