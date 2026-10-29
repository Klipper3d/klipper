import configparser
import re
import types
import unittest

from jinja2 import Environment

from test_integracao import Gcmd, ROOT, TestBench, protocol


class ManualBench(TestBench):
    def __init__(self, loaded=None, hot=False):
        super().__init__()
        self.temperature = 230.0 if hot else 25.0
        self.target = 0.0
        self.heat = True
        self.print_state = 'standby'
        self.homed_axes = 'xyz'
        self.fail_unload = False
        self.fail_load = False
        self.display = None
        self.present = loaded is not None
        self.device.status['present'] = [True] * 4
        self.device.status['route_state'] = [protocol.ROUTE_EMPTY] * 4
        if loaded is not None:
            self.device.status['route_state'][loaded] = protocol.ROUTE_LOADED
        self.bmcu.active_operations = {}
        self.bmcu.print_plan_open = False
        self.bmcu._route_state = lambda device, channel: device.status[
            'route_state'
        ][channel]
        self.printer.is_shutdown = lambda: False
        self.obj.manual_operation = None
        self.extruder.get_status = lambda t: {
            'temperature': self.temperature,
            'target': self.target,
            'can_extrude': self.temperature >= 170.0,
        }
        self.toolhead.get_status = lambda t: {
            'homed_axes': self.homed_axes,
            'axis_maximum': [230, 230, 250],
        }

    def lookup(self, name, default=None):
        if name == 'print_stats':
            return types.SimpleNamespace(
                get_status=lambda t: {'state': self.print_state}
            )
        if name == 'e3v3se_display':
            return self.display
        return super().lookup(name)

    def pause(self, timeout):
        super().pause(timeout)
        if self.heat:
            self.temperature = min(230.0, self.temperature + 40.0)

    def execute(self, text):
        super().execute(text)
        if text.startswith('SET_HEATER_TEMPERATURE'):
            self.target = float(text.split('TARGET=')[1])
        if text == 'G28':
            self.homed_axes = 'xyz'
        if text.startswith('BMCU_UNLOAD') and not self.fail_unload:
            self.device.status['route_state'] = [protocol.ROUTE_EMPTY] * 4
            self.present = False
        if text.startswith('BMCU_LOAD'):
            if self.fail_load:
                raise RuntimeError('arrival failed')
            channel = int(re.search(r'CHANNEL=(\d+)', text).group(1))
            self.device.status['route_state'][channel] = protocol.ROUTE_LOADED
            self.present = True


class ManualControlsTests(unittest.TestCase):
    def test_empty_unload_does_not_heat_home_or_move(self):
        b = ManualBench()
        command = Gcmd(ACTION='UNLOAD')
        b.obj.manual_filament(command)
        self.assertEqual([], b.commands)
        self.assertEqual(0, b.target)
        self.assertIn('nothing to unload', command.messages[0])

    def test_all_four_buttons_select_physical_channels(self):
        for physical in range(1, 5):
            with self.subTest(channel=physical):
                b = ManualBench()
                b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=physical))
                self.assertIn(
                    'BMCU_LOAD DEVICE=bmcu0 CHANNEL=%d' % (physical - 1),
                    b.commands,
                )
                self.assertEqual(230, b.target)
                self.assertIsNone(b.obj.manual_operation)

    def test_different_channel_unloads_before_loading(self):
        b = ManualBench(loaded=3)
        b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
        unload = b.commands.index('BMCU_UNLOAD DEVICE=bmcu0 CHANNEL=3')
        load = b.commands.index('BMCU_LOAD DEVICE=bmcu0 CHANNEL=0')
        self.assertLess(unload, load)
        self.assertTrue(any('X-6.000000' in c for c in b.commands[:unload]))
        self.assertIn('RESTORE_GCODE_STATE NAME=BMCU_ENDER_MANUAL', b.commands)

    def test_unload_waits_for_230_and_homes_before_purging(self):
        b = ManualBench(loaded=1)
        b.homed_axes = ''
        b.obj.manual_filament(Gcmd(ACTION='UNLOAD'))
        self.assertGreaterEqual(b.timeout, 1.0)
        home = b.commands.index('G28')
        purge = next(i for i, c in enumerate(b.commands) if 'X-6.000000' in c)
        unload = b.commands.index('BMCU_UNLOAD DEVICE=bmcu0 CHANNEL=1')
        self.assertLess(home, purge)
        self.assertLess(purge, unload)

    def test_hot_nozzle_does_not_wait_for_heating(self):
        b = ManualBench(hot=True)
        b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=2))
        self.assertEqual(0, b.timeout)

    def test_same_channel_is_a_noop(self):
        b = ManualBench(loaded=0)
        b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
        self.assertEqual([], b.commands)

    def test_missing_target_does_not_unload_existing_filament(self):
        b = ManualBench(loaded=0)
        b.device.status['present'][3] = False
        with self.assertRaisesRegex(RuntimeError, 'Insert filament'):
            b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=4))
        self.assertEqual([], b.commands)

    def test_failed_unload_never_starts_next_channel(self):
        b = ManualBench(loaded=0)
        b.fail_unload = True
        with self.assertRaisesRegex(RuntimeError, 'did not unload'):
            b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=4))
        self.assertFalse(any(c.startswith('BMCU_LOAD') for c in b.commands))
        self.assertIsNone(b.obj.manual_operation)

    def test_arrival_failure_restores_modal_state_and_stops_feeder(self):
        b = ManualBench()
        b.fail_load = True
        with self.assertRaisesRegex(RuntimeError, 'arrival failed'):
            b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=4))
        self.assertEqual('parar', b.actuators[-1])
        self.assertIn('RESTORE_GCODE_STATE NAME=BMCU_ENDER_MANUAL', b.commands)
        self.assertIsNone(b.obj.manual_operation)

    def test_route_sensor_disagreement_rejects_before_heating(self):
        for loaded, detected in ((None, True), (0, False)):
            b = ManualBench(loaded=loaded)
            b.present = detected
            with self.assertRaisesRegex(RuntimeError, 'disagree'):
                b.obj.manual_filament(Gcmd(ACTION='UNLOAD'))
            self.assertEqual([], b.commands)

    def test_printing_paused_busy_mapped_and_uncertain_reject_before_heat(
        self,
    ):
        for condition in ('printing', 'paused', 'busy', 'map', 'uncertain'):
            with self.subTest(condition=condition):
                b = ManualBench()
                if condition in ('printing', 'paused'):
                    b.print_state = condition
                elif condition == 'busy':
                    b.bmcu.active_operations = {'bmcu0': {}}
                elif condition == 'map':
                    b.bmcu.print_map_active = True
                else:
                    b.device.status['route_state'][
                        0
                    ] = protocol.ROUTE_UNCERTAIN
                with self.assertRaises(RuntimeError):
                    b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
                self.assertEqual([], b.commands)

    def test_heating_timeout_does_not_move_filament(self):
        b = ManualBench()
        b.heat = False
        with self.assertRaisesRegex(RuntimeError, 'Heating timed out'):
            b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
        self.assertFalse(any(c.startswith('BMCU_LOAD') for c in b.commands))
        self.assertIsNone(b.obj.manual_operation)

    def test_screen_cancellation_during_heating_does_not_start_load(self):
        b = ManualBench()
        b.display = types.SimpleNamespace(bmcu_session={'cancelled': True})
        with self.assertRaisesRegex(RuntimeError, 'cancelled'):
            b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
        self.assertFalse(any(c.startswith('BMCU_LOAD') for c in b.commands))

    def test_preheat_uses_shared_settings_and_rejects_during_print(self):
        parser = configparser.RawConfigParser()
        folder = ROOT / 'config/ender3-v3-se-validada'
        parser.read(folder / 'bmcu-controles.cfg')
        env = Environment(variable_start_string='{', variable_end_string='}')
        template = env.from_string(
            parser['gcode_macro _ENDER_PANEL_PREHEAT']['gcode']
        )
        settings = {
            'pla_nozzle': 205,
            'pla_bed': 60,
            'petg_nozzle': 230,
            'petg_bed': 80,
            'tpu_nozzle': 240,
            'tpu_bed': 50,
        }
        printer = {
            'gcode_macro _ENDER_PANEL_SETTINGS': settings,
            'print_stats': {'state': 'standby'},
            'bmcu': {'active_operations': {}},
            'bmcu_integracao_ender': {'manual_busy': False},
        }

        def fail(message):
            raise RuntimeError(message)

        for material in ('PLA', 'PETG', 'TPU'):
            result = template.render(
                printer=printer,
                params={'MATERIAL': material},
                action_raise_error=fail,
            )
            self.assertIn(
                'TARGET=%.1f' % settings[material.lower() + '_nozzle'], result
            )
            self.assertIn(
                'TARGET=%.1f' % settings[material.lower() + '_bed'], result
            )
        printer['print_stats']['state'] = 'printing'
        with self.assertRaisesRegex(RuntimeError, 'current operation'):
            template.render(
                printer=printer,
                params={'MATERIAL': 'PLA'},
                action_raise_error=fail,
            )


if __name__ == '__main__':
    unittest.main()
