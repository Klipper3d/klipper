import configparser
import math
import unittest

from test_cortador import BancadaCortador
from test_integracao import Gcmd, ROOT, protocol


class CutterPushbackTests(unittest.TestCase):
    def bench(self):
        b = BancadaCortador()
        b.ajustes.update(pushback_mm=10.0, pushback_feedrate=120.0)
        return b

    def test_new_profile_ships_ten_mm_but_cutter_disabled(self):
        parser = configparser.RawConfigParser()
        parser.read(ROOT / 'config/ender3-v3-se-validada/bmcu-cortador.cfg')
        cfg = parser['gcode_macro _BMCU_ENDER_CUTTER_SETTINGS']
        self.assertEqual('0', cfg['variable_enabled'])
        self.assertEqual('0', cfg['variable_calibrated'])
        self.assertEqual(10.0, cfg.getfloat('variable_pushback_mm'))
        self.assertEqual(120.0, cfg.getfloat('variable_pushback_feedrate'))

    def test_round_trip_reuses_assistance_and_preserves_net_withdrawal(self):
        b = self.bench()
        b.unload()
        positive = [mm for mm, _ in b.moves if mm > 0]
        self.assertAlmostEqual(10.0, sum(positive))
        self.assertAlmostEqual(-66.0, b.e)
        self.assertTrue(all(abs(mm) <= 1.0 for mm, _ in b.moves))
        self.assertTrue(all(
            mode == protocol.MOTION_BEFORE_PULL_BACK for _, mode in b.moves
        ))
        self.assertNotIn(protocol.MOTION_ON_USE, b.actuators)
        self.assertNotIn(protocol.MOTION_BEFORE_ON_USE, b.actuators)
        self.assertEqual([(3, 55.0)], b.policies)
        self.assertEqual(230.0, b.target)
        self.assertEqual(0.04, b.pressure)
        self.assertEqual(
            {
                'requested_mm': 10.0, 'advance_mm': 10.0,
                'return_mm': 10.0, 'stage': 'complete',
                'result': 'commands_completed',
            },
            b.obj.last_unload_timing['pushback'],
        )

    def test_pushback_starts_only_after_blade_release_and_purge_position(self):
        b = self.bench()
        b.unload()
        first = next(
            i for i, e in enumerate(b.eventos)
            if e[0] == 'gcode' and e[1].startswith('G1 E1.')
        )
        before = b.eventos[:first]
        self.assertIn(
            ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=0.000000'),
            before,
        )
        self.assertTrue(any(
            e[:2] == ('mover', [b.obj.purge_x, None, None])
            for e in before
        ))
        self.assertIn('F120.', b.eventos[first][1])
        self.assertEqual('complete',
                         b.obj.last_unload_timing['pushback']['stage'])

    def test_absent_or_zero_distance_keeps_historical_unload(self):
        for setting in (None, 0.0):
            with self.subTest(setting=setting):
                b = self.bench()
                if setting is None:
                    del b.ajustes['pushback_mm']
                    del b.ajustes['pushback_feedrate']
                else:
                    b.ajustes['pushback_mm'] = setting
                b.unload()
                self.assertFalse(any(mm > 0 for mm, _ in b.moves))
                self.assertAlmostEqual(-66.0, b.e)
                self.assertNotIn('pushback', b.obj.last_unload_timing)

    def test_disabled_pushback_preserves_slow_existing_profiles(self):
        for setting in (None, 0.0):
            with self.subTest(setting=setting):
                b = self.bench()
                if setting is None:
                    del b.ajustes['pushback_mm']
                    del b.ajustes['pushback_feedrate']
                else:
                    b.ajustes['pushback_mm'] = setting
                b.extruder.max_e_velocity = 1.0
                b.obj.timeout = 120.0
                for key in (
                    'velocidade_recuo_f', 'velocidade_avanco_f',
                    'velocidade_purga_f',
                ):
                    b.ajustes[key] = 60.0
                b.unload()
                self.assertAlmostEqual(-66.0, b.e)
                self.assertEqual([(3, 55.0)], b.policies)
                self.assertFalse(any(mm > 0 for mm, _ in b.moves))
                self.assertNotIn('pushback', b.obj.last_unload_timing)

    def test_invalid_pushback_rejected_before_cut_or_extrusion(self):
        for key, value in (
            ('pushback_mm', -1.0), ('pushback_mm', 10.1),
            ('pushback_mm', math.nan), ('pushback_mm', math.inf),
            ('pushback_feedrate', 0.0), ('pushback_feedrate', math.nan),
            ('pushback_feedrate', 601.0),
        ):
            with self.subTest(key=key, value=value):
                b = self.bench()
                b.ajustes[key] = value
                with self.assertRaisesRegex(RuntimeError, 'pushback'):
                    b.unload()
                self.assertFalse(b.moves)
                self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_stalled_feeder_cannot_consume_the_forward_stroke(self):
        b = self.bench()
        execute = b.execute

        def stall(text):
            before = b.device.status['meters'][3]
            execute(text)
            if text.startswith('G1 E'):
                b.device.status['meters'][3] = before

        b.gcode.run_script_from_command = stall
        with self.assertRaisesRegex(RuntimeError, 'pushback.*follow'):
            b.unload()
        self.assertAlmostEqual(2.0, b.e)
        self.assertEqual([0] * 4, b.device.status['motion'])
        self.assertFalse(b.policies)
        self.assertFalse(b.obj.temporary_pullbacks)
        self.assertEqual('advance',
                         b.obj.last_unload_timing['pushback']['stage'])

    def test_forward_catchup_waits_with_extruder_stationary(self):
        b = self.bench()
        execute, pause = b.execute, b.pause
        waiting = []
        delayed = True

        def lag(text):
            before = b.device.status['meters'][3]
            execute(text)
            if delayed and text.startswith('G1 E'):
                b.device.status['meters'][3] = before

        def catchup(timeout):
            nonlocal delayed
            if b.e == 2.0 and delayed:
                waiting.append(b.e)
                b.device.status['meters'][3] = 0.170 + b.e / 1000
                delayed = False
            pause(timeout)

        b.gcode.run_script_from_command = lag
        b.reactor.pause = catchup
        b.unload()
        self.assertEqual([2.0], waiting)
        self.assertAlmostEqual(-66.0, b.e)

    def test_stalled_return_cannot_be_hidden_by_resetting_the_encoder(self):
        b = self.bench()
        execute = b.execute

        def stall_return(text):
            before = b.device.status['meters'][3]
            execute(text)
            if text.startswith('G1 E-'):
                b.device.status['meters'][3] = before

        b.gcode.run_script_from_command = stall_return
        with self.assertRaisesRegex(RuntimeError, 'follow'):
            b.unload()
        self.assertAlmostEqual(8.0, b.e)
        self.assertEqual('return',
                         b.obj.last_unload_timing['pushback']['stage'])
        self.assertFalse(b.policies)

    def test_wrong_direction_or_oversupply_aborts_before_another_step(self):
        for delta in (-0.002, 0.004):
            with self.subTest(delta=delta):
                b = self.bench()
                execute = b.execute

                def wrong(text):
                    before = b.device.status['meters'][3]
                    execute(text)
                    if text.startswith('G1 E'):
                        b.device.status['meters'][3] = before + delta

                b.gcode.run_script_from_command = wrong
                with self.assertRaisesRegex(RuntimeError, 'mismatch'):
                    b.unload()
                self.assertAlmostEqual(1.0, b.e)
                self.assertEqual([0] * 4, b.device.status['motion'])
                self.assertFalse(b.policies)

    def test_low_buffer_pauses_extruder_then_resumes_only_after_recovery(self):
        b = self.bench()
        execute, pause = b.execute, b.pause
        observations = []

        def drain(text):
            execute(text)
            if b.e == 1.0 and text.startswith('G1 E1.'):
                b.device.status['buffer_pct'][3] = 25.0

        def recover(timeout):
            if b.device.status['buffer_pct'][3] == 25.0:
                observations.append(b.e)
                b.device.status['buffer_pct'][3] = 40.0
            pause(timeout)

        b.gcode.run_script_from_command = drain
        b.reactor.pause = recover
        b.unload()
        self.assertEqual([1.0], observations)
        self.assertAlmostEqual(-66.0, b.e)

    def test_unrecoverable_or_invalid_buffer_cannot_start_pushback(self):
        for pct in (20.0, 80.0, math.nan, math.inf, -1.0, 101.0):
            with self.subTest(pct=pct):
                b = self.bench()
                b.device.status['buffer_pct'][3] = pct
                with self.assertRaises(RuntimeError):
                    b.unload()
                self.assertFalse(b.moves)
                self.assertFalse(b.policies)

    def test_unsafe_symmetric_buffer_band_rejected_before_cut(self):
        b = self.bench()
        b.obj.unload_buffer_limit = 40.0
        with self.assertRaisesRegex(RuntimeError, 'pushback'):
            b.unload()
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_zero_extrusion_limit_rejected_before_cut(self):
        b = self.bench()
        b.extruder.max_e_dist = 0.0
        with self.assertRaisesRegex(RuntimeError, 'extrusion limit'):
            b.unload()
        self.assertFalse(b.moves)
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_final_unload_does_not_erase_the_round_trip_encoder_gap(self):
        b = self.bench()
        execute = b.execute

        def residual(text):
            execute(text)
            if text.startswith('G1 E-') and b.e == 0.0:
                b.device.status['meters'][3] += 0.0015

        b.gcode.run_script_from_command = residual
        with self.assertRaisesRegex(RuntimeError, 'follow'):
            b.unload()
        self.assertAlmostEqual(0.0, b.e)
        self.assertEqual('commands_completed',
                         b.obj.last_unload_timing['pushback']['result'])
        self.assertFalse(b.policies)
        self.assertFalse(b.obj.temporary_pullbacks)

    def test_existing_encoder_credit_cannot_hide_forward_stall(self):
        b = self.bench()
        execute = b.execute

        def lag(text):
            before = b.device.status['meters'][3]
            execute(text)
            if text.startswith('G1 E1.'):
                b.device.status['meters'][3] = before

        b.gcode.run_script_from_command = lag
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        with self.assertRaisesRegex(RuntimeError, 'follow'):
            b.unload()
        self.assertAlmostEqual(0.0, b.e)
        self.assertAlmostEqual(2.0, sum(
            mm for mm, _ in b.moves if mm > 0
        ))
        self.assertFalse(b.policies)

    def test_invalid_encoder_and_other_active_channels_abort(self):
        for failure in ('encoder', 'other_motion', 'other_pwm'):
            with self.subTest(failure=failure):
                b = self.bench()
                execute = b.execute

                def corrupt(text):
                    execute(text)
                    if not text.startswith('G1 E1.'):
                        return
                    if failure == 'encoder':
                        b.device.status['meters'][3] = math.nan
                    elif failure == 'other_motion':
                        b.device.status['motion'][0] = protocol.MOTION_ON_USE
                    else:
                        b.device.status['motor_pwm'][0] = 100

                b.gcode.run_script_from_command = corrupt
                with self.assertRaises(RuntimeError):
                    b.unload()
                self.assertAlmostEqual(1.0, b.e)
                self.assertEqual([0] * 4, b.device.status['motion'])
                self.assertFalse(b.policies)

    def test_partial_pushback_failures_do_not_retry_or_park(self):
        for failure in ('session', 'sensor', 'assistance', 'cold', 'command'):
            with self.subTest(failure=failure):
                b = self.bench()
                execute = b.execute

                def interrupt(text):
                    execute(text)
                    if not text.startswith('G1 E1.'):
                        return
                    if failure == 'session':
                        b.device.status['session_id'] += 1
                    elif failure == 'sensor':
                        b.present = False
                    elif failure == 'assistance':
                        b.device.status['motion'][3] = protocol.MOTION_IDLE
                    elif failure == 'cold':
                        b.temperature = 25.0
                    else:
                        raise RuntimeError('pushback command interrupted')

                b.gcode.run_script_from_command = interrupt
                with self.assertRaises(RuntimeError):
                    b.unload()
                self.assertAlmostEqual(1.0, b.e)
                self.assertFalse(b.policies)
                self.assertFalse(b.obj.temporary_pullbacks)
                self.assertEqual([0] * 4, b.device.status['motion'])
                self.assertEqual(
                    'failed', b.obj.last_unload_timing['pushback']['result']
                )

    def test_actual_channel_and_pre_cut_retract_are_preserved(self):
        for channel in range(4):
            with self.subTest(channel=channel):
                b = self.bench()
                b.device.status['present'] = [True] * 4
                b.device.status['meters'] = [0.170] * 4
                b.ajustes['recuo_antes_corte_mm'] = 2.0
                b.motion(channel, protocol.MOTION_BEFORE_PULL_BACK)
                b.obj.release(Gcmd(ENDPOINT='extruder'))
                self.assertAlmostEqual(-66.0, b.e)
                self.assertAlmostEqual(10.0, sum(
                    mm for mm, _ in b.moves if mm > 0
                ))
                self.assertEqual([(channel, 55.0)], b.policies)
                self.assertEqual(
                    2.0, b.obj.last_unload_timing['pre_cut_retract_mm']
                )


if __name__ == '__main__':
    unittest.main()
