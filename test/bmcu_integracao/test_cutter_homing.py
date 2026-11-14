import sys
import types
import unittest

from test_cortador import BancadaCortador
from test_integracao import Gcmd, ROOT, protocol

sys.path.insert(0, str(ROOT / 'klippy'))
from extras.safe_z_home import SafeZHoming
from gcode import GCodeDispatch


class HomingBench(BancadaCortador):
    def __init__(self):
        super().__init__()
        self.bmcu.active_operations = {}
        self.referencias = ''
        self.ajustes.update(
            x_aproximacao=180.0, x_contato=190.0, x_corte=230.0,
            curso_maximo_mm=40.0, soltar_servo_recolhido=1,
        )
        self.fail_axis = None
        self.reference_x = True
        self.gcode.create_gcode_command = (
            lambda command, line, params: Gcmd(**params)
        )
        self.toolhead.get_status = lambda t: {
            'homed_axes': self.referencias,
            'axis_minimum': [-6, -14, -3],
            'axis_maximum': [230, 230, 250],
        }
        self.toolhead.dwell = self.dwell
        self.command = self.obj.validar_homing_cortador(self.native_home)

    def dwell(self, seconds):
        self.eventos.append(('dwell', seconds))
        self.pause(self.timeout + seconds)

    def native_home(self, gcmd):
        params = dict(gcmd.get_command_parameters())
        axes = ''.join(axis for axis in 'XYZ' if axis in params) or 'XYZ'
        self.eventos.append(('native', params))
        for axis in axes:
            self.eventos.append(('home', axis))
            if axis == self.fail_axis:
                raise RuntimeError('endstop failure')
            if axis != 'X' or self.reference_x:
                self.referencias += axis.lower()
            self.position['XYZ'.index(axis)] = {'X': -6, 'Y': -14, 'Z': 0}[axis]

    def run(self, **params):
        return self.command(Gcmd(**params))

    def sequence(self):
        return [event for event in self.eventos
                if event[0] in ('home', 'gcode')]

    def use_safe_z_home(self):
        native = SafeZHoming.__new__(SafeZHoming)
        native.printer, native.gcode = self.printer, self.gcode
        native.prev_G28 = self.native_home
        native.z_hop, native.z_hop_speed = 10.0, 10.0
        native.home_x_pos, native.home_y_pos = 134.0, 123.0
        native.speed, native.move_to_previous = 100.0, False
        kin = types.SimpleNamespace(
            get_status=self.toolhead.get_status,
            clear_homing_state=lambda axes: setattr(
                self, 'referencias', ''.join(
                    axis for axis in self.referencias if axis not in axes
                )
            ),
        )
        self.printer.get_reactor = lambda: self.reactor
        self.toolhead.get_kinematics = lambda: kin
        self.toolhead.get_position = lambda: list(self.position)

        def set_position(position, homing_axes=''):
            self.position[:] = position
            self.referencias += homing_axes

        self.toolhead.set_position = set_position
        lookup = self.printer.lookup_object
        self.printer.lookup_object = lambda name, default=None: (
            native if name == 'safe_z_home' else lookup(name, default)
        )
        self.command = self.obj.validar_homing_cortador(native.cmd_G28)
        return native


class PhysicalHomingBench(HomingBench):
    def __init__(self):
        super().__init__()
        self.physical_z = 235.0
        self.z_positions = []

    def mover(self, coordinates, speed):
        if len(coordinates) > 2 and coordinates[2] is not None:
            self.physical_z += coordinates[2] - self.position[2]
            self.z_positions.append(self.physical_z)
        return super().mover(coordinates, speed)

    def native_home(self, gcmd):
        result = super().native_home(gcmd)
        if 'Z' in gcmd.get_command_parameters():
            self.physical_z = 0.0
        return result


RETRACT = ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=0.000000')
RELEASE = ('gcode', 'SET_SERVO SERVO=bmcu_cortador WIDTH=0')


class CutterHomingTests(unittest.TestCase):
    def test_restart_homes_x_without_any_servo_command_then_retracts_y_z(self):
        b = HomingBench()
        b.run()
        self.assertEqual(
            [('home', 'X'), RETRACT, RELEASE, ('home', 'Y'), ('home', 'Z')],
            b.sequence(),
        )
        self.assertTrue(b.obj.cortador.recolhido)
        self.assertFalse(b.obj.cortador.incerto)

    def test_repeated_g28_still_homes_x_before_any_servo_command(self):
        b = HomingBench()
        for _ in range(2):
            b.eventos.clear()
            b.run()
            self.assertEqual(
                [('home', 'X'), RETRACT, RELEASE,
                 ('home', 'Y'), ('home', 'Z')], b.sequence(),
            )

    def test_partial_requests_only_add_x_preparation(self):
        for axes in ('X', 'Y', 'Z', 'XY', 'XZ', 'YZ', 'XYZ'):
            with self.subTest(axes=axes):
                b = HomingBench()
                b.run(**dict.fromkeys(axes, '0'))
                self.assertEqual(
                    [('home', 'X'), RETRACT, RELEASE]
                    + [('home', axis) for axis in 'YZ' if axis in axes],
                    b.sequence(),
                )

    def test_non_axis_parameters_survive_without_mutating_original(self):
        b = HomingBench()
        command = Gcmd(X='0', Y='0', EXTRA='value')
        b.command(command)
        self.assertEqual({'X': '0', 'Y': '0', 'EXTRA': 'value'}, command.params)
        self.assertEqual(
            [{'X': '0', 'EXTRA': 'value'}, {'Y': '0', 'EXTRA': 'value'}],
            [event[1] for event in b.eventos if event[0] == 'native'],
        )

    def test_x_failure_prevents_servo_and_remaining_axes(self):
        b = HomingBench()
        b.fail_axis = 'X'
        with self.assertRaisesRegex(RuntimeError, 'endstop failure'):
            b.run()
        self.assertEqual([('home', 'X')], b.sequence())
        self.assertFalse(b.obj.cortador.recolhido)

    def test_missing_reference_cannot_use_old_unreferenced_bypass(self):
        b = HomingBench()
        b.reference_x = False
        b.ajustes['recolher_sem_referencia'] = 1
        with self.assertRaisesRegex(RuntimeError, 'homing'):
            b.run()
        self.assertEqual([('home', 'X')], b.sequence())

    def test_busy_bmcu_blocks_x_even_when_stop_is_marked_retracted(self):
        for operation in ('bmcu', 'manual'):
            with self.subTest(operation=operation):
                b = HomingBench()
                b.obj.cortador.recolhido = True
                if operation == 'bmcu':
                    b.bmcu.active_operations = {'bmcu0': {}}
                else:
                    b.obj.manual_operation = 'TESTE_CORTADOR'
                with self.assertRaisesRegex(RuntimeError, 'Wait'):
                    b.run()
                self.assertEqual([], b.sequence())

    def test_uncertain_cut_requires_manual_recovery_before_any_motion(self):
        b = HomingBench()
        b.obj.cortador.incerto = True
        with self.assertRaisesRegex(RuntimeError, 'uncertain'):
            b.run()
        self.assertEqual([], b.sequence())

    def test_invalid_profile_rejects_before_x(self):
        for name, value in (('calibrado', 0), ('x_corte', 231.0),
                            ('espera_servo_ms', float('nan'))):
            with self.subTest(name=name):
                b = HomingBench()
                b.ajustes[name] = value
                with self.assertRaises(RuntimeError):
                    b.run()
                self.assertEqual([], b.sequence())

    def test_unsafe_x_position_never_commands_servo_or_y_z(self):
        b = HomingBench()
        native = b.native_home

        def wrong_position(gcmd):
            native(gcmd)
            b.position[0] = 220

        b.command = b.obj.validar_homing_cortador(wrong_position)
        with self.assertRaisesRegex(RuntimeError, 'Move the head clear'):
            b.run()
        self.assertEqual([('home', 'X')], b.sequence())

    def test_retraction_or_pwm_failure_prevents_remaining_axes(self):
        for fragment in ('ANGLE=0.', 'WIDTH=0'):
            with self.subTest(fragment=fragment):
                b = HomingBench()
                execute = b.gcode.run_script_from_command

                def fail(text):
                    if fragment in text:
                        raise RuntimeError('servo failure')
                    return execute(text)

                b.gcode.run_script_from_command = fail
                with self.assertRaisesRegex(RuntimeError, 'servo failure'):
                    b.run()
                self.assertTrue(b.obj.cortador.incerto)
                self.assertFalse(b.obj.cortador.recolhido)
                self.assertEqual([('home', 'X')], [
                    event for event in b.eventos if event[0] == 'home'
                ])

    def test_retraction_waits_before_disabling_pwm(self):
        b = HomingBench()
        b.run(X='0')
        retract = b.eventos.index(RETRACT)
        release = b.eventos.index(RELEASE)
        self.assertEqual(('dwell', 0.5), b.eventos[retract + 1])
        self.assertEqual('esperar', b.eventos[retract + 2][0])
        self.assertGreater(release, retract + 2)

    def test_disabled_cutter_passes_original_g28_unchanged(self):
        b = HomingBench()
        b.ajustes['habilitado'] = 0
        b.run(Y='0')
        self.assertEqual([('home', 'Y')], b.sequence())
        self.assertEqual(('native', {'Y': '0'}), b.eventos[0])

    def test_missing_cutter_profile_passes_g28_unchanged(self):
        b = HomingBench()
        lookup = b.printer.lookup_object
        b.printer.lookup_object = lambda name, default=None: (
            default if name == 'gcode_macro _BMCU_ENDER_CUTTER_SETTINGS'
            else lookup(name, default)
        )
        b.run(Z='0')
        self.assertEqual([('home', 'Z')], b.sequence())

    def test_homing_releases_pwm_by_default_but_respects_explicit_opt_out(self):
        for option in (None, 0, 1):
            with self.subTest(option=option):
                b = HomingBench()
                if option is None:
                    del b.ajustes['soltar_servo_recolhido']
                else:
                    b.ajustes['soltar_servo_recolhido'] = option
                b.run(X='0')
                self.assertEqual(
                    [('home', 'X'), RETRACT]
                    + ([RELEASE] if option != 0 else []), b.sequence(),
                )

    def test_real_gcode_dispatcher_does_not_reenter_the_g28_wrapper(self):
        for script in ('G28', 'G28 X', 'G28 Y0 Z0'):
            with self.subTest(script=script):
                b = HomingBench()
                b.printer.get_start_args = lambda: {}
                b.printer.get_reactor = lambda: b.reactor
                b.printer.register_event_handler = lambda *args: None
                b.reactor.mutex = lambda: None
                dispatch = GCodeDispatch(b.printer)
                b.gcode = b.obj.gcode = dispatch
                dispatch.register_command('G28', b.obj.protect(
                    b.obj.validar_homing_cortador(b.native_home)
                ))
                dispatch.register_mux_command(
                    'SET_SERVO', 'SERVO', 'bmcu_cortador',
                    lambda g: b.eventos.append(('gcode', g.get_commandline())),
                )
                dispatch._handle_ready()
                dispatch.run_script_from_command(script)
                remaining = [] if script == 'G28 X' else [
                    ('home', 'Y'), ('home', 'Z'),
                ]
                self.assertEqual(
                    [('home', 'X'), RETRACT, RELEASE] + remaining,
                    b.sequence(),
                )

    def test_manual_loading_can_home_before_claiming_the_filament_operation(
        self,
    ):
        for retracted, axes in ((False, ''), (False, 'xyz'), (True, '')):
            with self.subTest(retracted=retracted, axes=axes):
                b = HomingBench()
                b.obj.cortador.recolhido = retracted
                b.referencias = axes
                b.present = False
                b.device.status['present'] = [True] * 4
                execute = b.gcode.run_script_from_command

                def run(text):
                    if text == 'G28':
                        self.assertIsNone(b.obj.manual_operation)
                        return b.run()
                    if text.startswith('BMCU_LOAD '):
                        self.assertTrue(b.obj.cortador.recolhido)
                        self.assertTrue(all(a in b.referencias for a in 'xyz'))
                        b.device.status['route_state'][0] = (
                            protocol.ROUTE_LOADED
                        )
                        b.present = True
                    return execute(text)

                b.gcode.run_script_from_command = run
                b.obj.manual_filament(Gcmd(ACTION='LOAD', CHANNEL=1))
                self.assertEqual(
                    [('home', 'X'), RETRACT, RELEASE,
                     ('home', 'Y'), ('home', 'Z')], b.sequence()[:5],
                )
                self.assertIsNone(b.obj.manual_operation)

    def test_safe_z_home_retains_lift_probe_location_and_y_requirement(self):
        b = HomingBench()
        b.use_safe_z_home()
        b.run()
        self.assertEqual(
            [('home', 'X'), RETRACT, RELEASE, ('home', 'Y'), ('home', 'Z')],
            b.sequence(),
        )
        self.assertIn(('mover', [None, None, 10.0], 10.0), b.eventos)
        self.assertIn(('mover', [134.0, 123.0], 100.0), b.eventos)
        b.referencias = ''
        b.eventos.clear()
        with self.assertRaisesRegex(RuntimeError, 'Must home X and Y'):
            b.run(Z='0')
        self.assertEqual([('home', 'X'), RETRACT, RELEASE], b.sequence())

    def test_unreferenced_z_has_only_one_physical_initial_lift(self):
        for axes in ('', 'X', 'Y', 'Z', 'XY', 'XZ', 'YZ', 'XYZ'):
            with self.subTest(axes=axes):
                b = PhysicalHomingBench()
                b.use_safe_z_home()
                if axes in ('Z', 'XZ'):
                    b.referencias = 'y'
                b.run(**dict.fromkeys(axes, '0'))
                probes_z = not axes or 'Z' in axes
                self.assertEqual(
                    [245.0, 10.0] if probes_z else [245.0], b.z_positions,
                )
                self.assertLessEqual(max(b.z_positions), 250.0)
                first_move = next(event for event in b.eventos
                                  if event[0] in ('mover', 'home', 'gcode'))
                self.assertEqual(
                    ('mover', [None, None, 10.0], 10.0), first_move,
                )

    def test_already_referenced_z_retains_native_initial_and_final_clearance(
        self,
    ):
        b = PhysicalHomingBench()
        b.position[2] = b.physical_z = 5.0
        b.referencias = 'z'
        b.use_safe_z_home()
        b.run()
        self.assertEqual([10.0, 10.0], b.z_positions)

    def test_native_safe_z_home_still_runs_its_own_initial_lift(self):
        b = PhysicalHomingBench()
        native = b.use_safe_z_home()
        native.cmd_G28(Gcmd(X='0', Y='0', Z='0'))
        self.assertEqual([245.0, 10.0], b.z_positions)


if __name__ == '__main__':
    unittest.main()
