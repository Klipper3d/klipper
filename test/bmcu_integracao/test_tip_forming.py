import ast
import configparser
import types
import unittest

from test_integracao import Gcmd, ROOT, TestBench, protocol


class TipBench(TestBench):
    def __init__(self):
        super().__init__()
        parser = configparser.RawConfigParser()
        parser.read(
            ROOT / 'config/ender3-v3-se-validada/bmcu-formacao-ponta.cfg'
        )
        self.settings = {
            key.removeprefix('variable_'): ast.literal_eval(value)
            for key, value in parser.items(
                'gcode_macro _BMCU_ENDER_TIP_SETTINGS'
            )
            if key.startswith('variable_')
        }
        self.temperature = self.target = 230.0
        self.pressure = 0.04
        self.fail_cooling = False
        self.extruder.max_e_dist = 50.0
        self.extruder.max_e_velocity = 60.0
        self.extruder.get_heater = lambda: types.SimpleNamespace(
            min_extrude_temp=170.0, get_temp_range=lambda: (0.0, 300.0)
        )
        self.extruder.get_status = lambda t: {
            'can_extrude': self.temperature >= 170,
            'temperature': self.temperature,
            'target': self.target,
            'pressure_advance': self.pressure,
            'smooth_time': 0.04,
        }

    def lookup(self, name, default=None):
        if name == 'gcode_macro _BMCU_ENDER_TIP_SETTINGS':
            return types.SimpleNamespace(get_status=lambda t: self.settings)
        return super().lookup(name, default)

    def pause(self, timeout):
        super().pause(timeout)
        self.temperature = self.target

    def execute(self, text):
        if self.fail_cooling and text.startswith('G1 E10.'):
            raise RuntimeError('cooling interrupted')
        super().execute(text)
        if text.startswith('SET_PRESSURE_ADVANCE'):
            self.pressure = float(text.split('ADVANCE=')[1].split()[0])
        if text.startswith('SET_HEATER_TEMPERATURE'):
            self.target = float(text.split('TARGET=')[1])

    def unload(self):
        self.motion(3, protocol.MOTION_BEFORE_PULL_BACK)
        return self.obj.release(Gcmd(ENDPOINT='extruder'))


class TipFormingTests(unittest.TestCase):
    def test_compressed_buffer_aborts_without_further_extrusion(self):
        b = TipBench()
        b.device.status['buffer_pct'][3] = 80
        with self.assertRaisesRegex(RuntimeError, 'buffer did not recover'):
            b.unload()
        self.assertEqual(b.e, 0)
        self.assertLess(b.timeout, 4)
        self.assertEqual([], b.policies)
        self.assertEqual(b.device.status['motion'], [0, 0, 0, 0])
        self.assertAlmostEqual(b.pressure, 0.04)

    def test_buffer_recovery_keeps_feeder_active_and_resumes_unload(self):
        b = TipBench()
        b.device.status['buffer_pct'][3] = 80
        pause = b.pause
        observations = []

        def recover(timeout):
            observations.append((b.e, b.device.status['motion'][3]))
            pause(timeout)
            if timeout >= 0.2:
                b.device.status['buffer_pct'][3] = 55

        b.reactor.pause = recover
        b.unload()
        self.assertEqual(observations[0], (0, protocol.MOTION_ON_USE))
        self.assertEqual(observations[1], (0, protocol.MOTION_ON_USE))
        self.assertEqual([(3, 55.0)], b.policies)

    def test_encoder_stall_stops_before_full_switch_timeout(self):
        b = TipBench()
        b.obj.pullback_speed = 300
        execute = b.execute

        def stall_after_forming(text):
            position = b.device.status['meters'][3]
            execute(text)
            if text.startswith('G1 E-2.'):
                b.device.status['meters'][3] = position

        b.gcode.run_script_from_command = stall_after_forming
        with self.assertRaisesRegex(
            RuntimeError, 'No encoder pullback progress.*encoder pullback'
        ):
            b.unload()
        self.assertLess(b.reverse, b.switch_distance)
        self.assertLess(b.timeout, 25)
        self.assertEqual([], b.policies)
        self.assertEqual(b.device.status['motion'], [0, 0, 0, 0])

    def test_half_purge_preserves_capture_and_nozzle_feed(self):
        b = TipBench()
        b.obj.purge_distance = 35
        b.obj.load(Gcmd(ENDPOINT='extruder'))
        self.assertEqual(b.e, 135)
        self.assertEqual(b.position[0], -4)

    def test_cooling_keeps_filament_gripped_until_final_unload(self):
        b = TipBench()
        b.unload()
        self.assertAlmostEqual(b.e, -67.0)
        self.assertFalse(b.obj.sensor(b.endpoint))
        self.assertEqual([(3, 55.0)], b.policies)
        self.assertAlmostEqual(b.pressure, 0.04)
        self.assertEqual(b.target, 230)
        self.assertEqual(b.device.status['motion'], [0, 0, 0, 0])
        self.assertEqual(b.critical_depth, 0)
        self.assertEqual(
            len([c for c in b.commands if c.startswith('G1 E10.')]), 4
        )

    def test_cooling_failure_stops_and_preserves_route_evidence(self):
        b = TipBench()
        b.fail_cooling = True
        with self.assertRaisesRegex(RuntimeError, 'cooling interrupted'):
            b.unload()
        self.assertEqual(b.device.status['motion'], [0, 0, 0, 0])
        self.assertEqual([], b.policies)
        self.assertAlmostEqual(b.pressure, 0.04)
        self.assertTrue(b.obj.sensor(b.endpoint))
        self.assertIn(
            'RESTORE_GCODE_STATE NAME=BMCU_ENDER_RETIRADA', b.commands
        )

    def test_temperature_is_restored_after_failed_forming(self):
        b = TipBench()
        b.settings['temperature'] = 220
        b.fail_cooling = True
        with self.assertRaisesRegex(RuntimeError, 'cooling interrupted'):
            b.unload()
        self.assertEqual(b.target, 230)
        self.assertGreater(b.timeout, 0)
        self.assertEqual([], b.policies)

    def test_geometry_that_releases_sensor_aborts_before_parking(self):
        b = TipBench()
        b.switch_distance = 40
        with self.assertRaisesRegex(RuntimeError, 'entry sensor'):
            b.unload()
        self.assertEqual([], b.policies)
        self.assertEqual(b.device.status['motion'], [0, 0, 0, 0])

    def test_invalid_parameters_do_not_start_forming(self):
        for key, value in (
            ('cooling_moves', 1.5),
            ('separation_f', float('nan')),
            ('cooling_position_mm', 5),
            ('temperature', 160),
            ('separation_f', 4000),
            ('cooling_length_mm', 60),
        ):
            with self.subTest(key=key):
                b = TipBench()
                b.settings[key] = value
                with self.assertRaises(RuntimeError):
                    b.unload()
                self.assertEqual(b.e, 0)
                self.assertEqual([], b.policies)

    def test_disabled_profile_keeps_existing_unload(self):
        b = TipBench()
        b.settings['enabled'] = 0
        b.unload()
        self.assertEqual(b.e, -66)
        self.assertFalse(any('PRESSURE_ADVANCE' in c for c in b.commands))

    def test_load_uses_follow_after_capture(self):
        b = TipBench()
        b.obj.load(Gcmd(ENDPOINT='extruder'))
        self.assertEqual(b.e, 170)
        self.assertNotIn(protocol.MOTION_BEFORE_ON_USE, b.actuators)
        self.assertEqual(b.device.status['motion'][3], protocol.MOTION_ON_USE)


if __name__ == '__main__':
    unittest.main()
