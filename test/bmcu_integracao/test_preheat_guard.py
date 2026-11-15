import types
import unittest

from test_integracao import Gcmd
from test_manual_controls import ManualBench
from test_cortador import BancadaCortador


class TemperatureCommand(Gcmd):
    def get_float(self, name, default=None, **limits):
        value = float(self.get(name, default))
        if 'minval' in limits and value < limits['minval']:
            raise self.error('Temperature below minimum')
        return value


class HeatingBench(ManualBench):
    def __init__(self):
        super().__init__()
        self.heater = types.SimpleNamespace(min_temp=0.0, max_temp=260.0)
        self.extruder.get_heater = lambda: self.heater
        self.toolhead.get_extruder = lambda: self.extruder
        self.heating_positions = []

    def execute(self, text):
        if text.startswith('SET_HEATER_TEMPERATURE'):
            self.heating_positions.append(list(self.position))
        return super().execute(text)

    def guarded(self, name, **params):
        applied = []
        previous = lambda command: applied.append(list(self.position)) or 'native'
        handler = self.obj.guard_hotend_temperature(name, previous)
        result = handler(TemperatureCommand(**params))
        return result, applied


class PreheatGuardTests(unittest.TestCase):
    def test_manual_load_parks_before_setting_230_including_initial_homing(self):
        for homed in ('xyz', ''):
            with self.subTest(homed=homed):
                b = HeatingBench()
                b.homed_axes = homed
                b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
                self.assertTrue(b.heating_positions)
                self.assertTrue(all(pos[:2] == [-6.0, 123.0]
                                    for pos in b.heating_positions))

    def test_all_native_hotend_commands_park_before_delegating(self):
        for name, params in (
            ('M104', {'S': 230}), ('M109', {'S': 230}),
            ('SET_HEATER_TEMPERATURE', {'HEATER': 'extruder', 'TARGET': 230}),
        ):
            with self.subTest(command=name):
                b = HeatingBench()
                result, applied = b.guarded(name, **params)
                self.assertEqual('native', result)
                self.assertEqual([[-6.0, 123.0, 7.0, 0.0]], applied)
                self.assertEqual(0.0, b.e)

    def test_ready_installs_guards_on_existing_handlers(self):
        b = HeatingBench()
        for name in ('G28', 'RESUME', 'CANCEL_PRINT', 'SDCARD_RESET_FILE',
                     'BMCU_LOAD', 'BMCU_UNLOAD', 'BMCU_TOOL_CHANGE',
                     'BMCU_PRINT_END', 'M104', 'M109',
                     'SET_HEATER_TEMPERATURE'):
            b.handlers[name] = lambda command: 'native'
        b.obj.ready()
        self.assertEqual('native', b.handlers['M104'](TemperatureCommand(S=230)))
        self.assertEqual([-6.0, 123.0], b.position[:2])

    def test_low_targets_cooldown_bed_and_other_tools_never_park(self):
        for name, params in (
            ('M104', {'S': 0}), ('M109', {'S': 150}),
            ('M104', {'S': 230, 'T': 1}),
            ('SET_HEATER_TEMPERATURE', {'HEATER': 'heater_bed', 'TARGET': 80}),
        ):
            with self.subTest(command=name, params=params):
                b = HeatingBench()
                b.guarded(name, **params)
                self.assertFalse(b.head_moves)

    def test_temperature_adjustments_during_print_or_pause_never_move(self):
        for state in ('printing', 'paused', 'pause'):
            with self.subTest(state=state):
                b = HeatingBench()
                b.print_state = state
                result, applied = b.guarded('M104', S=230)
                self.assertEqual('native', result)
                self.assertEqual([b.position], applied)
                self.assertFalse(b.head_moves)

    def test_unspecified_tool_preserves_another_active_hotend(self):
        b = HeatingBench()
        b.toolhead.get_extruder = lambda: object()
        result, applied = b.guarded('M104', S=230)
        self.assertEqual('native', result)
        self.assertEqual([b.position], applied)
        self.assertFalse(b.head_moves)

    def test_print_starting_during_parking_blocks_stale_heater_request(self):
        b = HeatingBench()
        move = b.toolhead.manual_move
        def start_print(*args):
            move(*args)
            b.print_state = 'printing'
        b.toolhead.manual_move = start_print
        applied = []
        guard = b.obj.guard_hotend_temperature(
            'M104', lambda g: applied.append(True),
        )
        with self.assertRaisesRegex(RuntimeError, 'Print state changed'):
            guard(TemperatureCommand(S=230))
        self.assertFalse(applied)

    def test_repeated_preheat_at_purge_does_not_accumulate_z_lift(self):
        b = HeatingBench()
        b.guarded('M104', S=230)
        count = len(b.head_moves)
        b.guarded('M109', S=230)
        self.assertEqual(count, len(b.head_moves))
        self.assertEqual(7.0, b.position[2])

    def test_unhomed_or_invalid_target_rejects_before_motion_and_heating(self):
        for condition in ('unhomed', 'hot', 'nan', 'inf'):
            with self.subTest(condition=condition):
                b = HeatingBench()
                if condition == 'unhomed':
                    b.homed_axes = ''
                target = {'hot': 300, 'nan': float('nan'),
                          'inf': float('inf')}.get(condition, 230)
                applied = []
                guard = b.obj.guard_hotend_temperature(
                    'M104', lambda g: applied.append(True),
                )
                with self.assertRaises(RuntimeError):
                    guard(TemperatureCommand(S=target))
                self.assertFalse(applied)
                self.assertFalse(b.head_moves)

    def test_parking_failure_does_not_apply_the_temperature(self):
        b = HeatingBench()
        applied = []
        def fail(*args):
            raise RuntimeError('parking failed')
        b.toolhead.manual_move = fail
        guard = b.obj.guard_hotend_temperature(
            'M104', lambda g: applied.append(True),
        )
        with self.assertRaisesRegex(RuntimeError, 'parking failed'):
            guard(TemperatureCommand(S=230))
        self.assertFalse(applied)

    def test_other_target_while_parking_is_not_overwritten(self):
        b = HeatingBench()
        move = b.toolhead.manual_move
        def change_target(*args):
            move(*args)
            b.target = 215
        b.toolhead.manual_move = change_target
        applied = []
        guard = b.obj.guard_hotend_temperature(
            'M104', lambda g: applied.append(True),
        )
        with self.assertRaisesRegex(RuntimeError, 'target changed'):
            guard(TemperatureCommand(S=230))
        self.assertFalse(applied)
        self.assertEqual(215, b.target)

    def test_uncertain_or_deployed_cutter_blocks_idle_preheat(self):
        for uncertain in (False, True):
            b = BancadaCortador()
            b.obj.cortador.incerto = uncertain
            b.obj.cortador.recolhido = False
            with self.assertRaises(RuntimeError):
                b.obj.prepare_preheat(230)
            self.assertFalse(b.head_moves)
            self.assertFalse(b.commands)

    def test_threshold_is_configurable(self):
        b = HeatingBench()
        b.obj.preheat_park_threshold = 160.0
        b.guarded('M104', S=155)
        self.assertFalse(b.head_moves)


if __name__ == '__main__':
    unittest.main()
