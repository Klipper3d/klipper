import math
import re
import types
import unittest

from test_cortador import BancadaCortador
from test_integracao import (
    Gcmd, NATIVE_MANAGER, TestBench, installer, module, protocol,
)
from test_print_recovery import RecoveryCommand, gcode_move


class CutterSafetyTests(unittest.TestCase):
    def test_unload_sequence_is_valid_for_every_physical_channel(self):
        for channel in range(4):
            for cutter in (0, 1):
                with self.subTest(channel=channel, cutter=cutter):
                    b = BancadaCortador()
                    b.device.status['present'] = [True] * 4
                    b.ajustes['habilitado'] = cutter
                    b.ajustes['recuo_antes_corte_mm'] = 2.0
                    b.motion(channel, protocol.MOTION_BEFORE_PULL_BACK)
                    b.obj.release(Gcmd())
                    self.assertAlmostEqual(-66.0, b.e)
                    self.assertFalse(b.obj.sensor(b.endpoint))
                    self.assertEqual([(channel, 55.0)], b.policies)
                    self.assertEqual([0] * 4, b.device.status['motion'])

    def assert_no_blade(self, bench):
        self.assertFalse(any(
            event[0] == 'gcode' and 'ANGLE=90.' in event[1]
            for event in bench.eventos
        ))
        self.assertFalse(bench.policies)

    def test_pre_cut_retract_never_starts_feeder_assistance(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        b.unload()
        self.assertEqual([(-1.0, protocol.MOTION_IDLE)] * 2, b.moves[:2])
        self.assertEqual(protocol.MOTION_BEFORE_PULL_BACK, b.moves[2][1])
        timing = b.obj.last_unload_timing
        self.assertEqual(2.0, timing['pre_cut_retract_mm'])
        self.assertIn('sincronismo_pre_corte', timing)
        self.assertIn('sincronismo_pos_corte', timing)

    def test_post_cut_encoder_reference_does_not_include_pre_cut_retract(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        b.unload()
        before = b.obj.last_unload_timing['sincronismo_pos_corte'][
            'antes_assistencia'
        ]
        self.assertEqual(0.0, before['recuo_encoder_mm'])
        self.assertEqual(0.0, before['recuo_extrusora_comandado_mm'])
        self.assertEqual(protocol.MOTION_IDLE, before['movimento'])

    def test_feeder_starting_during_blade_stroke_aborts_before_withdrawal(self):
        b = BancadaCortador()
        move = b.toolhead.manual_move

        def started(coords, speed):
            move(coords, speed)
            if coords[0] == -7.0:
                b.device.status['motor_pwm'][0] = 850

        b.toolhead.manual_move = started
        with self.assertRaisesRegex(RuntimeError, 'BMCU.*stopped'):
            b.unload()
        self.assertTrue(b.obj.cortador.incerto)
        self.assertEqual('curso_da_lamina',
                         b.obj.cortador.ultimo_corte['etapa'])
        self.assertFalse(b.moves)
        self.assertFalse(b.policies)

    def test_return_to_origin_failure_does_not_report_successful_dry_cycle(
        self,
    ):
        b = BancadaCortador()
        origin = list(b.position)
        move = b.toolhead.manual_move

        def failed_return(coords, speed):
            if coords == [origin[0], origin[1], None]:
                raise RuntimeError('return failed')
            return move(coords, speed)

        b.toolhead.manual_move = failed_return
        with self.assertRaisesRegex(RuntimeError, 'return failed'):
            b.obj.cortador.cortar(b.obj.cortador.preparar(), lambda: None)
        self.assertEqual('retorno_a_origem',
                         b.obj.cortador.ultimo_corte['etapa'])
        self.assertEqual('interrompido',
                         b.obj.cortador.ultimo_corte['resultado'])
        self.assertTrue(b.obj.cortador.recolhido)
        self.assertFalse(b.obj.cortador.incerto)

    def test_retract_exceeding_buffer_allowance_fails_before_motion(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.1
        with self.assertRaisesRegex(RuntimeError, 'allowance'):
            b.unload()
        self.assertEqual(0.0, b.e)
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))
        self.assert_no_blade(b)

    def test_pre_cut_sensor_release_records_last_completed_step(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        b.switch_distance = 1.0
        with self.assertRaisesRegex(RuntimeError, 'sensor'):
            b.unload()
        self.assertEqual(-1.0, b.e)
        self.assert_no_blade(b)
        timing = b.obj.last_unload_timing
        self.assertEqual('failed', timing['result'])
        self.assertNotIn('sincronismo_pos_corte', timing)
        snapshot = timing['sincronismo_pre_corte']['depois_passo']
        self.assertEqual(1.0, snapshot['recuo_extrusora_comandado_mm'])
        self.assertEqual('recuo_pre_corte',
                         b.obj.cortador.ultimo_corte['etapa'])

    def test_pre_cut_buffer_limit_aborts_before_another_step(self):
        for initial in (False, True):
            with self.subTest(initial=initial):
                b = BancadaCortador()
                b.ajustes['recuo_antes_corte_mm'] = 2.0
                refresh = b.device.refresh

                def compressed():
                    refresh()
                    if initial or b.e < 0:
                        b.device.status['buffer_pct'][3] = 70

                b.device.refresh = compressed
                with self.assertRaisesRegex(RuntimeError, 'buffer'):
                    b.unload()
                self.assertEqual(0.0 if initial else -1.0, b.e)
                self.assert_no_blade(b)

    def test_cut_requires_all_feeder_motors_idle_even_without_retract(self):
        for retract in (0.0, 2.0):
            for field, value in (('motion', protocol.MOTION_ON_USE),
                                 ('motor_pwm', -850)):
                with self.subTest(retract=retract, field=field):
                    b = BancadaCortador()
                    b.ajustes['recuo_antes_corte_mm'] = retract
                    refresh = b.device.refresh

                    def moving():
                        refresh()
                        b.device.status[field][0] = value

                    b.device.refresh = moving
                    with self.assertRaisesRegex(RuntimeError, 'BMCU.*stopped'):
                        b.unload()
                    self.assertEqual(0.0, b.e)
                    self.assert_no_blade(b)

    def test_restart_during_pre_cut_retract_blocks_blade_and_parking(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        refresh = b.device.refresh

        def restarted():
            refresh()
            if b.e < 0:
                b.device.status['session_id'] += 1

        b.device.refresh = restarted
        with self.assertRaisesRegex(RuntimeError, 'restarted'):
            b.unload()
        self.assertEqual(-1.0, b.e)
        self.assert_no_blade(b)

    def test_pre_cut_retract_respects_extruder_step_limit(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        b.extruder.max_e_dist = 0.4
        b.unload()
        self.assertTrue(all(abs(mm) <= 0.4 + 1e-8 for mm, _ in b.moves))
        before_cut = [mm for mm, mode in b.moves
                      if mode == protocol.MOTION_IDLE]
        self.assertAlmostEqual(-2.0, sum(before_cut))

    def test_failed_validation_replaces_previous_cut_success(self):
        b = BancadaCortador()
        profile = b.obj.cortador.preparar()
        b.obj.cortador.cortar(profile, lambda: None)
        b.position[2] = 250
        with self.assertRaisesRegex(RuntimeError, 'Z clearance'):
            b.obj.cortador.cortar(profile, lambda: None)
        self.assertEqual('interrompido',
                         b.obj.cortador.ultimo_corte['resultado'])
        self.assertEqual('validacao', b.obj.cortador.ultimo_corte['etapa'])

    def test_failed_initial_retraction_has_current_cut_diagnostic(self):
        b = BancadaCortador()
        b.position[0] = -5.0
        with self.assertRaisesRegex(RuntimeError, 'Move the head clear'):
            b.obj.cortador.cortar(b.obj.cortador.preparar(), lambda: None)
        self.assertEqual('recolhimento_inicial',
                         b.obj.cortador.ultimo_corte['etapa'])
        self.assertFalse(b.obj.cortador.ultimo_corte['posicao_incerta'])

    def test_every_cut_stage_reports_failure_without_false_success(self):
        cases = (
            ('aproximacao', 'move', -2.0, False),
            ('acionamento_servo', 'servo', 90.0, True),
            ('curso_da_lamina', 'move', -7.0, True),
            ('retorno_e_recolhimento', 'servo', 0.0, True),
        )
        for stage, kind, value, uncertain in cases:
            with self.subTest(stage=stage):
                b = BancadaCortador()
                b.obj.cortador.recolhido = True
                move = b.toolhead.manual_move
                execute = b.gcode.run_script_from_command

                def moved(coords, speed):
                    if kind == 'move' and coords[0] == value:
                        raise RuntimeError('injected motion failure')
                    return move(coords, speed)

                def commanded(text):
                    if kind == 'servo' and 'ANGLE=%.6f' % value in text:
                        raise RuntimeError('injected servo failure')
                    return execute(text)

                b.toolhead.manual_move = moved
                b.gcode.run_script_from_command = commanded
                with self.assertRaisesRegex(RuntimeError, 'injected'):
                    b.unload()
                self.assertEqual(stage, b.obj.cortador.ultimo_corte['etapa'])
                self.assertEqual(uncertain, b.obj.cortador.incerto)
                self.assertEqual(0.0, b.e)
                self.assertFalse(b.policies)
                self.assertEqual('failed', b.obj.last_unload_timing['result'])

    def test_parking_cannot_complete_with_motors_still_running(self):
        for field, value in (('motion', protocol.MOTION_PULL_BACK),
                             ('motor_pwm', -850)):
            with self.subTest(field=field):
                b = BancadaCortador()
                b.unload()

                def parked(device, channel, *args, **kwargs):
                    device.status['meters'][channel] -= 0.055
                    device.status[field][channel] = value
                    return {'ok': True}

                b.obj.original_wait_pullback = parked
                with self.assertRaisesRegex(RuntimeError, 'stop|idle'):
                    b.obj.wait_pullback(b.device, 3)
                self.assertEqual('parking_failed',
                                 b.obj.last_unload_timing['result'])
                self.assertEqual('parar', b.actuators[-1])


class NativeUnloadTests(unittest.TestCase):
    def native_begin(self):
        source = installer.patch_pre_unload(NATIVE_MANAGER)
        self.assertEqual(source, installer.patch_pre_unload(source))
        namespace = {'protocol': protocol}
        exec(compile(source, '<native_unload>', 'exec'), namespace)
        return namespace['BMCUManager']._begin_unload_locked

    def test_native_boundary_stops_print_follow_before_ender_callback(self):
        for retract in (0.0, 2.0):
            with self.subTest(retract=retract):
                b = BancadaCortador()
                b.ajustes['recuo_antes_corte_mm'] = retract
                b.bmcu.active_operations['bmcu0']['phase'] = 'CUT_OR_FORM'
                b.motion(3, protocol.MOTION_ON_USE)
                b.endpoint.driver = 'generic_single_extruder'
                b.endpoint.get = lambda *args: 'BMCU_RELEASE_ENDER'
                b.endpoint.prepare_toolhead_for_pullback = (
                    lambda *args, **kwargs: b.obj.release(Gcmd())
                )
                manager = types.SimpleNamespace(reactor=b.reactor)
                context = self.native_begin()(manager, b.device, b.endpoint, 3)
                self.assertFalse(context['before_pullback_started'])
                self.assertIsNone(b.endpoint._ender_unload_context)
                self.assertFalse(b.obj.sensor(b.endpoint))
                self.assertEqual([(3, 55.0)], b.policies)
                if retract:
                    self.assertEqual(protocol.MOTION_IDLE, b.moves[0][1])

    def test_other_native_callbacks_keep_existing_assistance(self):
        for driver, macro in (
            ('generic_single_extruder', 'OTHER_MACRO'),
            ('other', 'BMCU_RELEASE_ENDER'),
        ):
            with self.subTest(driver=driver):
                b = BancadaCortador()
                observed = []
                b.endpoint.driver = driver
                b.endpoint.get = lambda *args: macro
                b.endpoint.prepare_toolhead_for_pullback = (
                    lambda *args, **kwargs: observed.append(
                        b.device.status['motion'][3]
                    )
                )
                context = self.native_begin()(
                    types.SimpleNamespace(reactor=b.reactor),
                    b.device, b.endpoint, 3,
                )
                self.assertTrue(context['before_pullback_started'])
                self.assertEqual(0.2, b.timeout)
                if driver == 'generic_single_extruder':
                    self.assertEqual([protocol.MOTION_BEFORE_PULL_BACK],
                                     observed)

    def test_restart_while_native_stop_is_in_flight_rejects_old_context(self):
        b = BancadaCortador()
        b.bmcu.active_operations['bmcu0']['phase'] = 'CUT_OR_FORM'
        b.endpoint.driver = 'generic_single_extruder'
        b.endpoint.get = lambda *args: 'BMCU_RELEASE_ENDER'
        b.endpoint.prepare_toolhead_for_pullback = (
            lambda *args, **kwargs: b.obj.release(Gcmd())
        )
        stop = b.device.stop_all

        def restarted():
            stop()
            b.device.status['session_id'] += 1

        b.device.stop_all = restarted
        with self.assertRaisesRegex(RuntimeError, 'context changed'):
            self.native_begin()(
                types.SimpleNamespace(reactor=b.reactor), b.device,
                b.endpoint, 3,
            )
        self.assertFalse(b.moves)
        self.assertFalse(b.policies)

    def test_stale_or_wrong_unload_context_cannot_move_filament(self):
        for context in (('other', 3, 3), ('bmcu0', 3, 2)):
            with self.subTest(context=context):
                b = BancadaCortador()
                b.bmcu.active_operations['bmcu0']['phase'] = 'CUT_OR_FORM'
                b.endpoint._ender_unload_context = context
                with self.assertRaisesRegex(RuntimeError, 'context changed'):
                    b.obj.release(Gcmd())
                self.assertIsNone(b.endpoint._ender_unload_context)
                self.assertFalse(b.moves)
                self.assertFalse(b.policies)

    def test_idle_context_outside_native_unload_phase_is_rejected(self):
        b = BancadaCortador()
        b.endpoint._ender_unload_context = ('bmcu0', 3, 3)
        with self.assertRaisesRegex(RuntimeError, 'unloading phase'):
            b.obj.release(Gcmd())
        self.assertFalse(b.moves)
        self.assertFalse(b.policies)

    def test_incompatible_native_source_is_rejected_not_silently_patched(self):
        source = NATIVE_MANAGER.replace(
            'self.reactor.monotonic() + 0.2', 'self.reactor.monotonic() + 0.3'
        )
        with self.assertRaisesRegex(RuntimeError, 'native unload phase'):
            installer.patch_pre_unload(source)


class FiniteConfigurationTests(unittest.TestCase):
    def test_nonfinite_distances_speeds_positions_and_timeouts_are_rejected(
        self,
    ):
        keys = (
            'capture_mm', 'capture_feedrate', 'nozzle_feed_mm', 'purge_mm',
            'feed_feedrate', 'retract_feedrate', 'unload_buffer_limit',
            'unload_buffer_resume', 'unload_buffer_timeout',
            'unload_progress_timeout', 'unload_step_mm', 'unload_slack_mm',
            'pullback_after_sensor_mm', 'sensor_timeout',
            'purge_x', 'wipe_x', 'purge_y', 'z_lift',
        )
        for key in keys:
            for value in (math.nan, math.inf, -math.inf):
                with self.subTest(key=key, value=value):
                    b = TestBench()
                    b.printer.get_reactor = lambda: b.reactor
                    b.bmcu._wait_pullback_safe = lambda *args: None
                    b.printer.load_object = lambda config, name: (
                        b.bmcu if name == 'bmcu' else b.obj.measurement
                    )
                    b.printer.register_event_handler = lambda *args: None
                    config = types.SimpleNamespace(
                        error=RuntimeError,
                        get_printer=lambda: b.printer,
                        get=lambda name, default: default,
                        getint=lambda name, default, **limits: default,
                        getfloat=lambda name, default, **limits: (
                            value if name == key else default
                        ),
                    )
                    with self.assertRaisesRegex(RuntimeError, 'finite'):
                        module.EnderIntegration(config)
                    self.assertFalse(b.commands)
                    self.assertFalse(b.actuators)


class MotionCommand(RecoveryCommand):
    def get_float(self, name, default=None, **limits):
        value = self.params.get(name, default)
        return None if value is None else float(value)


class MotionBench(BancadaCortador):
    # Real Klipper G-code state; only the toolhead and controller are simulated.
    def __init__(self):
        super().__init__()
        move = gcode_move.GCodeMove.__new__(gcode_move.GCodeMove)
        self.gmove = move
        move.printer = self.printer
        move.is_printer_ready = True
        move.Coord = tuple
        move.axis_map = dict(zip('XYZE', range(4)))
        move.absolute_coord = move.allow_absolute_extrude = True
        move.base_position = [0.0] * 4
        move.last_position = list(self.position)
        move.homing_position = [0.0] * 4
        move.speed = 25.0
        move.speed_factor = 1.0 / 60.0
        move.extrude_factor = 1.0
        move.saved_states = {}
        move.position_with_transform = lambda: list(self.position)
        move.move_with_transform = self.move_gcode
        manual_move = self.toolhead.manual_move

        def manual(coords, speed):
            manual_move(coords, speed)
            move.reset_last_position()
            self.physical_moves.append((list(self.position), speed))

        self.toolhead.manual_move = manual
        self.physical_moves = []

    def move_gcode(self, position, speed):
        self.physical_moves.append((list(position), speed))
        self.position[:] = position

    def execute(self, text):
        if not hasattr(self, 'gmove'):
            return super().execute(text)
        self.commands.append(text)
        for line in text.splitlines():
            command, _, rest = line.partition(' ')
            handler = getattr(self.gmove, 'cmd_' + command, None)
            if handler is None:
                continue
            if command in ('G1', 'G92', 'M220', 'M221'):
                params = dict(re.findall(r'([XYZEFS])([-+0-9.]+)', rest))
            else:
                params = dict(re.findall(r'(\w+)=(\S+)', rest))
            handler(MotionCommand(**params))


class PurgeMotionTests(unittest.TestCase):
    def test_purge_uses_machine_coordinates_not_gcode_offsets(self):
        for offset in (-3.0, 3.0):
            with self.subTest(offset=offset):
                b = MotionBench()
                b.execute('SET_GCODE_OFFSET X=%s Y=%s Z=%s' % (
                    offset, offset, offset,
                ))
                b.obj.position_head()
                self.assertEqual([-6.0, 123.0, 7.0], b.position[:3])
                self.assertEqual([offset] * 3, b.gmove.homing_position[:3])
                self.assertEqual(0.0, b.position[3])

    def test_purge_wipe_uses_machine_coordinates_not_gcode_offsets(self):
        b = MotionBench()
        b.obj.purge_distance = 1.0
        b.execute('SET_GCODE_OFFSET X=3 Y=-3 Z=1')
        b.obj.position_head()
        b.obj.wipe(b.device, b.endpoint, 3, 3)
        self.assertEqual([-4.0, 123.0, 7.0], b.position[:3])

    def test_purge_travel_speed_is_independent_of_print_override(self):
        for factor in (50, 200):
            with self.subTest(factor=factor):
                b = MotionBench()
                b.execute('M220 S%d' % factor)
                b.obj.position_head()
                speeds = [speed for _, speed in b.physical_moves]
                self.assertEqual([5.0, 20.0, 20.0], speeds)
                self.assertAlmostEqual(factor / 6000,
                                       b.gmove.speed_factor)

    def test_invalid_purge_or_wipe_coordinates_reject_before_any_motion(self):
        for attribute, value in (
            ('purge_x', -9), ('purge_y', 231), ('wipe_x', 231),
            ('purge_x', math.nan), ('purge_y', math.inf),
        ):
            with self.subTest(attribute=attribute, value=value):
                b = MotionBench()
                setattr(b.obj, attribute, value)
                with self.assertRaisesRegex(RuntimeError, 'limits'):
                    b.obj.position_head()
                self.assertEqual([], b.physical_moves)
                self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_insufficient_z_clearance_blocks_xy_travel(self):
        b = MotionBench()
        b.position[2] = 249.0
        with self.assertRaisesRegex(RuntimeError, 'limits'):
            b.obj.position_head()
        self.assertEqual([], b.physical_moves)

    def test_cold_purge_positioning_rejects_before_motion_except_preheat(self):
        b = MotionBench()
        b.temperature = 25.0
        with self.assertRaisesRegex(RuntimeError, 'Heat'):
            b.obj.position_head()
        self.assertEqual([], b.physical_moves)
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))
        b.obj.position_head(exigir_aquecimento=False)
        self.assertEqual([-6.0, 123.0, 7.0], b.position[:3])


if __name__ == '__main__':
    unittest.main()
