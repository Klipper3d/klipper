import ast
import configparser
import importlib.util
import json
import math
import re
import sys
import tempfile
import types
import unittest
from contextlib import contextmanager
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
BASE = Path(__file__).parent / 'fixtures'


def import_module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


for name in ('integration_test', 'integration_test.bmcu_core'):
    package = types.ModuleType(name)
    package.__path__ = []
    sys.modules[name] = package
release = types.ModuleType('integration_test.bmcu_core.release')
release.PACKAGE_VERSION = '1.0.6'
release.REQUIRED_FIRMWARE = (1, 0, 1)
release.REQUIRED_FIRMWARE_TEXT = '1.0.1'
sys.modules[release.__name__] = release
protocol = import_module(
    'integration_test.bmcu_core.protocol', BASE / 'protocolo_original.py'
)
protocol.ROUTE_EMPTY = 0
protocol.ROUTE_LOADED = 1
protocol.ROUTE_UNCERTAIN = 2
module = import_module(
    'integration_test.bmcu_integracao_ender',
    ROOT / 'integrations/bmcu/bmcu_integracao_ender.py',
)
installer = import_module(
    'instalador', ROOT / 'scripts/ender3-v3-se/instalar_multicor_bmcu.py'
)


class Gcmd:
    def __init__(self, **params):
        self.params = params
        self.messages = []

    def get(self, name, default=None):
        return self.params.get(name, default)

    def get_int(self, name, default=None, **limits):
        value = int(self.params.get(name, default))
        if (
            'minval' in limits
            and value < limits['minval']
            or 'maxval' in limits
            and value > limits['maxval']
        ):
            raise RuntimeError('Value outside the allowed range.')
        return value

    def get_command_parameters(self):
        return self.params

    def error(self, message):
        return RuntimeError(message)

    def respond_info(self, message):
        self.messages.append(message)


class TestBench:
    def __init__(self):
        self.timeout = 0.0
        self.critical_depth = 0
        self.commands = []
        self.policies = []
        self.present = True
        self.reverse = 0
        self.switch_distance = 66
        self.stuck = False
        self.restart = False
        self.position = [100.0, 110.0, 5.0, 0.0]
        self.e = 0.0
        self.actuators = []
        self.handlers = {}
        self.reactor = types.SimpleNamespace(
            monotonic=lambda: self.timeout, pause=self.pause
        )
        self.device = types.SimpleNamespace(
            name='bmcu0',
            ready=True,
            status={
                'now_channel': 255,
                'motion': [0, 0, 0, 0],
                'present': [False, True, False, True],
                'session_id': 3,
                'meters': [0, 0, 0, 0.170],
            },
            refresh=self.refresh,
            stop_all=self.stop,
            set_motion=self.motion,
        )
        self.endpoint = types.SimpleNamespace(
            name='extruder',
            sensor_status=self.sensor,
            _ender_load_context=('bmcu0', 3),
        )
        self.bmcu = types.SimpleNamespace(
            devices=[self.device],
            devices_by_name={'bmcu0': self.device},
            endpoints={'extruder': self.endpoint},
            active_operations={'bmcu0': {'phase': 'TOOLHEAD_PREPARATION'}},
            print_map_active=False,
            print_tools={},
            _endpoint_for_channel=lambda *args: self.endpoint,
            printer_critical_section=self.critical,
            _send_device_runtime_config=self.policy,
            get_status=lambda t: {'loaded_tool': 2},
        )
        self.toolhead = types.SimpleNamespace(
            get_position=lambda: self.position,
            get_status=lambda t: {
                'homed_axes': 'xyz',
                'axis_maximum': [230, 230, 250],
            },
        )
        self.extruder = types.SimpleNamespace(
            max_e_dist=5.0, get_status=lambda t: {'can_extrude': True}
        )
        self.gcode = types.SimpleNamespace(
            run_script_from_command=self.execute,
            register_command=self.register,
            respond_info=lambda msg: None,
        )
        self.gcode_move = types.SimpleNamespace(
            get_status=lambda t: {'gcode_position': list(self.position)}
        )
        self.printer = types.SimpleNamespace(
            lookup_object=self.lookup, config_error=RuntimeError
        )
        self.obj = module.EnderIntegration.__new__(module.EnderIntegration)
        values = dict(
            printer=self.printer,
            bmcu=self.bmcu,
            gcode=self.gcode,
            reactor=self.reactor,
            device='bmcu0',
            destination='extruder',
            capture=20.0,
            capture_speed=180.0,
            advance=80.0,
            purge_distance=70.0,
            pullback=55.0,
            advance_speed=120.0,
            pullback_speed=600.0,
            timeout=60,
            purge_x=-6.0,
            wipe_x=-4.0,
            purge_y=123.0,
            z_lift=2.0,
            wipe_passes=3,
            temporary_pullbacks={},
        )
        self.obj.__dict__.update(values)
        self.obj.measurement = types.SimpleNamespace(
            _capturar_extrusora_primeiro=self.capture
        )

    def lookup(self, name):
        return {
            'toolhead': self.toolhead,
            'extruder': self.extruder,
            'gcode_move': self.gcode_move,
        }[name]

    def pause(self, timeout):
        self.timeout = timeout

    @contextmanager
    def critical(self, name):
        self.critical_depth += 1
        try:
            yield
        finally:
            self.critical_depth -= 1

    def uart(self):
        assert self.critical_depth == 0, (
            'UART em seção crítica de ' 'movimento'
        )

    def refresh(self):
        self.uart()
        if self.restart and self.reverse >= 4:
            self.device.status['session_id'] += 1

    def stop(self):
        self.uart()
        self.actuators.append('parar')
        self.device.status['motion'] = [0] * 4
        self.device.status['now_channel'] = 255

    def motion(self, channel, mode):
        self.uart()
        self.actuators.append(mode)
        self.device.status['motion'][channel] = mode
        self.device.status['now_channel'] = channel

    def policy(self, device, channel_retract_override=None):
        self.uart()
        self.policies.append(channel_retract_override)

    def sensor(self, role):
        return {
            'enabled': False,
            'filament_detected': self.present
            and (self.stuck or self.reverse < self.switch_distance),
        }

    def capture(
        self, device, destination, channel, mm, f, sensor_gerenciado=False
    ):
        assert sensor_gerenciado is True
        assert destination.sensor_status('entry_sensor')['enabled'] is False
        self.e += mm
        device.status['meters'][channel] += mm / 1000

    def execute(self, text):
        self.commands.append(text)
        for line in text.splitlines():
            if not line.startswith('G1 '):
                continue
            params = dict(
                (k, float(v))
                for k, v in re.findall(r'([XYZEF])([-+0-9.]+)', line)
            )
            for axis, index in zip('XYZ', range(3)):
                if axis in params:
                    self.position[index] = params[axis]
            if 'E' in params:
                mm = params['E']
                assert abs(mm) <= self.extruder.max_e_dist + 1e-8
                self.e += mm
                self.timeout += abs(mm) / params['F'] * 60
                self.reverse += max(0, -mm)
                self.device.status['meters'][3] += mm / 1000

    def register(self, name, handler):
        previous = self.handlers.pop(name, None)
        if handler is not None:
            self.handlers[name] = handler
        return previous


class IntegrationTests(unittest.TestCase):
    def test_installer_accepts_another_device_uid(self):
        status = {
            'webhooks': {'state': 'ready'},
            'print_stats': {'state': 'standby'},
            'filament_switch_sensor bmcu_entry': {'filament_detected': False},
            'bmcu': {
                'package_version': '1.0.6',
                'active_operations': {},
                'print_plan_open': False,
                'print_map_active': False,
                'prestaged': {},
                'devices': [
                    {
                        'name': 'bmcu0',
                        'uid': 'SYNTHETIC_DEVICE',
                        'ready': True,
                        'loaded_channels': [],
                        'uncertain_channels': [],
                    }
                ],
            },
        }
        with patch.object(
            installer, 'query_object', side_effect=status.__getitem__
        ):
            installer.check_printer()

    def test_measurement_module_keeps_existing_commands(self):
        measurement_module = import_module(
            'integration_test.bmcu_medicao',
            ROOT / 'integrations/bmcu/bmcu_medicao.py',
        )
        commands = {}
        gcode = types.SimpleNamespace(
            register_command=lambda name, handler, **kwargs: commands.update(
                {name: handler}
            )
        )
        manager = types.SimpleNamespace(gcode=gcode)
        printer = types.SimpleNamespace(
            load_object=lambda config, name: manager
        )
        config = types.SimpleNamespace(get_printer=lambda: printer)
        measurement = measurement_module.load_config(config)
        self.assertEqual(
            set(commands),
            {
                'BMCU_MEDIR_CHEGADA',
                'BMCU_TESTAR_EXTRUSAO',
                'BMCU_LIMITES_EXTRUSAO',
                'BMCU_RETIRAR_ATE_SWITCH',
            },
        )
        self.assertIsInstance(
            measurement.assisted_unload, measurement_module.AssistedUnload
        )

    def test_public_templates_do_not_bind_a_personal_device(self):
        start = (
            ROOT / 'profiles/orca/ender3-v3-se-bmcu/inicio.gcode'
        ).read_text()
        self.assertIn('UID=YOUR_BMCU_UID', start)
        self.assertNotRegex(start, r'UID=[A-Fa-f0-9]{16,}')
        self.assertNotIn('BMCU_SET_FILAMENT ', installer.CONFIGURATION)
        self.assertFalse(list((ROOT / 'machines').rglob('*')))

    def test_load_with_managed_sensor_and_move_limits(self):
        b = TestBench()
        b.obj.load(Gcmd(ENDPOINT='extruder'))
        self.assertAlmostEqual(b.e, 170)
        self.assertEqual(b.device.status['motion'][3], protocol.MOTION_ON_USE)
        self.assertEqual(b.position[0], -4)
        self.assertEqual(b.critical_depth, 0)

    def test_unload_applies_override_only_after_sensor_clears(self):
        b = TestBench()
        b.motion(3, protocol.MOTION_BEFORE_PULL_BACK)
        b.obj.release(Gcmd(ENDPOINT='extruder'))
        self.assertAlmostEqual(b.reverse, 66)
        self.assertFalse(b.obj.sensor(b.endpoint))
        self.assertEqual(b.policies, [(3, 55.0)])
        self.assertEqual(b.device.status['motion'][3], 0)
        self.assertIn('bmcu0', b.obj.temporary_pullbacks)
        b.obj.adapt_operation(lambda g: None)(Gcmd())
        self.assertEqual(b.policies, [(3, 55.0), None])
        self.assertFalse(b.obj.temporary_pullbacks)

    def test_stuck_sensor_aborts_and_stops(self):
        b = TestBench()
        b.motion(3, protocol.MOTION_BEFORE_PULL_BACK)
        b.stuck = True
        with self.assertRaisesRegex(RuntimeError, 'did not clear'):
            b.obj.release(Gcmd())
        self.assertEqual(b.device.status['motion'][3], 0)
        self.assertFalse(b.policies)
        self.assertEqual(b.critical_depth, 0)

    def test_uart_reset_aborts_and_stops(self):
        b = TestBench()
        b.motion(3, protocol.MOTION_BEFORE_PULL_BACK)
        b.restart = True
        with self.assertRaisesRegex(RuntimeError, 'restarted'):
            b.obj.release(Gcmd())
        self.assertEqual(b.device.status['motion'][3], 0)

    def test_native_failure_restores_override(self):
        b = TestBench()
        b.obj.temporary_pullbacks['bmcu0'] = {}

        def fail(gcmd):
            raise RuntimeError('timeout')

        with self.assertRaisesRegex(RuntimeError, 'timeout'):
            b.obj.adapt_operation(fail)(Gcmd())
        self.assertEqual(b.policies, [None])
        self.assertFalse(b.obj.temporary_pullbacks)

    def test_load_restores_policy_without_stopping_follow(self):
        b = TestBench()
        b.obj.temporary_pullbacks['bmcu0'] = {}
        b.obj.adapt_operation(b.obj.load)(Gcmd())
        self.assertEqual(b.policies, [None])
        self.assertEqual(b.device.status['motion'][3], protocol.MOTION_ON_USE)

    def test_orca_adapter_and_existing_tools(self):
        b = TestBench()
        previous_calls = []
        for index in range(4):
            adapter = b.obj.adapt_tool(
                index, lambda g: previous_calls.append(g)
            )
            adapter(Gcmd(B=str(index + 1)))
            self.assertEqual(
                b.commands[-1], 'BMCU_TROCAR_ENDER TOOL=%d' % (index + 1)
            )
            adapter(Gcmd())
        self.assertEqual(len(previous_calls), 4)
        with self.assertRaisesRegex(RuntimeError, 'do not match'):
            b.obj.adapt_tool(1, lambda g: None)(Gcmd(B='1'))

    def test_automatic_orca_tools_use_committed_map(self):
        for index in range(4):
            with self.subTest(index=index):
                b = TestBench()
                b.bmcu.print_map_active = True
                b.bmcu.print_tools = {
                    str(index + 1): {'device': 'bmcu0', 'channel': index}
                }
                previous_calls = []
                adapter = b.obj.adapt_tool(
                    index, lambda g: previous_calls.append(g)
                )
                adapter(Gcmd())
                self.assertEqual(
                    b.commands,
                    ['BMCU_TROCAR_ENDER TOOL=%d' % (index + 1)],
                )
                self.assertFalse(previous_calls)

    def test_missing_map_tool_does_not_select_external_input(self):
        b = TestBench()
        b.bmcu.print_map_active = True
        b.bmcu.print_tools = {'4': {'device': 'bmcu0', 'channel': 3}}
        previous_calls = []
        with self.assertRaisesRegex(RuntimeError, 'print map'):
            b.obj.adapt_tool(0, lambda g: previous_calls.append(g))(Gcmd())
        self.assertFalse(b.commands)
        self.assertFalse(previous_calls)

    def test_inactive_map_preserves_native_tools(self):
        b = TestBench()
        b.bmcu.print_tools = {'1': {'device': 'bmcu0', 'channel': 0}}
        previous_calls = []
        b.obj.adapt_tool(0, lambda g: previous_calls.append(g))(Gcmd())
        self.assertEqual(len(previous_calls), 1)
        self.assertFalse(b.commands)

    def test_active_map_preserves_explicit_b_validation(self):
        b = TestBench()
        b.bmcu.print_map_active = True
        b.obj.adapt_tool(0, lambda g: self.fail('Tool nativo'))(Gcmd(B='1'))
        self.assertEqual(b.commands, ['BMCU_TROCAR_ENDER TOOL=1'])
        with self.assertRaisesRegex(RuntimeError, 'do not match'):
            b.obj.adapt_tool(0, lambda g: None)(Gcmd(B='2'))
        self.assertEqual(len(b.commands), 1)

    def test_repeated_t0_uses_logical_tool_without_extra_purge(self):
        b = TestBench()
        b.bmcu.print_map_active = True
        b.bmcu.print_tools = {'1': {'device': 'bmcu0', 'channel': 3}}
        b.bmcu.get_status = lambda t: {'loaded_tool': 1}
        execute = b.gcode.run_script_from_command

        def dispatch(text):
            if text == 'BMCU_TROCAR_ENDER TOOL=1':
                return b.obj.change_tool(Gcmd(TOOL='1'))
            return execute(text)

        b.gcode.run_script_from_command = dispatch
        adapter = b.obj.adapt_tool(0, lambda g: self.fail('Tool nativo'))
        adapter(Gcmd(B='1'))
        adapter(Gcmd())
        self.assertEqual(b.commands.count('BMCU_TOOL_CHANGE TOOL=1'), 2)
        self.assertNotIn('BMCU_TOOL_CHANGE TOOL=0', b.commands)
        self.assertFalse(b.actuators)
        self.assertEqual(b.e, 0)

    def test_tool_change_requires_homing(self):
        b = TestBench()
        b.toolhead.get_status = lambda t: {'homed_axes': ''}
        with self.assertRaisesRegex(RuntimeError, 'G28'):
            b.obj.change_tool(Gcmd(TOOL=2))
        self.assertFalse(b.commands)
        self.assertFalse(b.actuators)

    def test_encoder_check_precedes_next_channel(self):
        for distance, valid in [
            (55.58, True),
            (20.0, False),
            (-55.0, False),
        ]:
            b = TestBench()
            b.present = False
            start = b.device.status['meters'][3]
            b.obj.temporary_pullbacks['bmcu0'] = {
                'channel': 3,
                'start': start,
                'session': 3,
            }

            def wait(d, c, *args, **kwargs):
                self.assertEqual(kwargs['start_m'], start)
                d.status['meters'][c] = start - distance / 1000
                return {'encoder_mm': distance}

            b.obj.original_wait_pullback = wait
            if valid:
                return_value = b.obj.wait_pullback(
                    b.device, 3, 60.0, start_m=999
                )
                self.assertAlmostEqual(
                    return_value['after_switch_encoder_mm'], distance
                )
            else:
                with self.assertRaisesRegex(RuntimeError, 'next channel'):
                    b.obj.wait_pullback(b.device, 3, 60.0)
                self.assertEqual(b.actuators[-1], 'parar')

    def test_patches_are_idempotent_and_preserve_legacy_helper(self):
        original = (BASE / 'bmcu_medicao_original.txt').read_text()
        updated = installer.patch_sensor(original)
        self.assertEqual(installer.patch_sensor(updated), updated)
        self.assertEqual(
            original[original.index('class RetiradaAssistida:') :],
            updated[updated.index('class RetiradaAssistida:') :],
        )
        endpoint = (BASE / 'endpoints_original.txt').read_text()
        updated_endpoint = installer.patch_contract(endpoint)
        self.assertEqual(
            installer.patch_contract(updated_endpoint), updated_endpoint
        )
        self.assertEqual(
            updated_endpoint.count('priority = command not in'), 1
        )

    def test_include_preserves_save_config_and_repeated_install(self):
        with tempfile.TemporaryDirectory() as temp:
            home = Path(temp)
            config = home / 'printer_data/config'
            extras = home / 'klipper/klippy/extras'
            config.mkdir(parents=True)
            (extras / 'bmcu_core').mkdir(parents=True)
            saved = (
                '#*# <---------------------- SAVE_CONFIG ---'
                '------------------->\n#*# [bltouch]\n#*# z_'
                'offset = 1.887\n#*# [bed_mesh default]\n#*#'
                ' x_count = 9\n'
            )
            (config / 'printer.cfg').write_text(
                '[include mainsail.cfg]\n' + saved
            )
            (extras / 'bmcu_medicao.py').write_text(
                (ROOT / 'integrations/bmcu/bmcu_medicao.py').read_text()
            )
            (extras / 'bmcu_core/endpoints.py').write_text(
                (BASE / 'endpoints_original.txt').read_text()
            )
            (extras / 'bmcu_core/manager.py').write_text(
                (BASE / 'manager_original.txt').read_text()
            )
            changes = installer.prepare(home)
            root = changes[(config / 'printer.cfg').resolve()].decode()
            self.assertEqual(root[root.index('#*# <') :], saved)
            for p, data in changes.items():
                p.write_bytes(data)
            self.assertEqual(installer.prepare(home), changes)
            sequence = config / 'bmcu-multicor-ender.cfg'
            custom = sequence.read_text().replace(
                'purga_mm: 70', 'purga_mm: 91'
            )
            sequence.write_text(custom)
            self.assertEqual(
                installer.prepare(home)[sequence], custom.encode()
            )
            settings = config / 'bmcu-controles-ajustes.cfg'
            custom_settings = settings.read_text().replace(
                'variable_tpu_nozzle: 230', 'variable_tpu_nozzle: 240'
            )
            settings.write_text(custom_settings)
            self.assertEqual(
                installer.prepare(home)[settings], custom_settings.encode()
            )
            self.assertEqual(root.count('[include bmcu-controles.cfg]'), 1)

    def test_native_capture_starts_extruder_before_feeder(self):
        original = (BASE / 'manager_original.txt').read_text()
        updated = installer.patch_pre_capture(original)
        self.assertEqual(installer.patch_pre_capture(updated), updated)
        root = ast.parse(updated)
        conditional = next(
            n
            for n in ast.walk(root)
            if isinstance(n, ast.If)
            and 'BMCU_PREPARAR_ENDER' in ast.dump(n.test)
        )
        for macro, expected in [
            ('BMCU_PREPARAR_ENDER', 'parar'),
            ('OUTRO_CALLBACK', 'avancar'),
        ]:
            actions = []
            device = types.SimpleNamespace(
                name='bmcu0',
                stop_all=lambda: actions.append('parar'),
                set_motion=lambda c, m: actions.append('avancar'),
            )
            destination = types.SimpleNamespace(get=lambda *args: macro)
            ctx = dict(
                generic_contract=True,
                endpoint=destination,
                device=device,
                channel=3,
                protocol=protocol,
            )
            exec(
                compile(
                    ast.Module(body=[conditional], type_ignores=[]),
                    '<pre_captura>',
                    'exec',
                ),
                ctx,
            )
            self.assertEqual(actions, [expected])
            if expected == 'parar':
                self.assertEqual(destination._ender_load_context, ('bmcu0', 3))

    def test_transport_priority_is_limited_to_contract_callbacks(self):
        source = installer.patch_contract(
            (BASE / 'endpoints_original.txt').read_text()
        )
        tree = ast.parse(source)
        node = next(
            n
            for c in tree.body
            if isinstance(c, ast.ClassDef)
            for n in c.body
            if isinstance(n, ast.FunctionDef)
            and n.name == '_run_generic_contract_macro'
        )
        namespace = {}
        exec(
            compile(
                ast.Module(body=[node], type_ignores=[]), '<contrato>', 'exec'
            ),
            namespace,
        )
        priorities = []
        for command, expected in [
            ('BMCU_PREPARAR_ENDER', False),
            ('BMCU_LIBERAR_ENDER', False),
            ('OUTRO_CALLBACK', True),
        ]:
            destination = types.SimpleNamespace(
                name='extruder',
                _required_generic_macro=lambda k, l: command,
                _gcode_param=lambda v: v,
                _run=lambda script, printer_priority: priorities.append(
                    printer_priority
                ),
            )
            namespace['_run_generic_contract_macro'](destination, 'k', 'l')
            self.assertEqual(priorities[-1], expected)

    def test_configuration_templates_use_klipper_syntax(self):
        from jinja2 import Environment

        parser = configparser.RawConfigParser()
        parser.read_string(installer.CONFIGURATION)
        env = Environment(
            block_start_string='{%',
            block_end_string='%}',
            variable_start_string='{',
            variable_end_string='}',
        )

        def fail(text):
            raise RuntimeError(text)

        context = dict(
            params={'TEMP_MESA': '80', 'TEMP_BICO': '230', 'FERRAMENTA': '2'},
            printer={
                'bmcu': {
                    'active_operations': {},
                    'print_map_active': False,
                    'print_plan_open': False,
                    'devices': [
                        dict(loaded_channels=[], uncertain_channels=[])
                    ],
                },
                'print_stats': {'state': 'standby'},
                'filament_switch_sensor bmcu_entry': {
                    'filament_detected': False
                },
            },
            rawparams='ENDPOINT=extruder MATERIAL=PETG REASON=load',
            action_raise_error=fail,
        )
        for section in parser.sections():
            if not section.startswith('gcode_macro '):
                continue
            result = env.from_string(parser[section]['gcode']).render(
                **context
            )
            if section == 'gcode_macro ENDER_INICIAR_MULTICOR':
                self.assertIn('M140 S80.0', result)
                self.assertIn('M109 S230.0', result)
                self.assertNotIn('G1 E', result)
            if section == 'gcode_macro BMCU_CONFIGURAR_MULTICOR':
                self.assertIn('CONTACT_TIMEOUT=180', result)
                self.assertNotIn('UNLOAD_RETRACT_MM', result)
        context['printer']['filament_switch_sensor bmcu_entry'][
            'filament_detected'
        ] = True
        with self.assertRaisesRegex(RuntimeError, 'entry sensor'):
            env.from_string(
                parser['gcode_macro BMCU_CONFIGURAR_MULTICOR']['gcode']
            ).render(**context)


if __name__ == '__main__':
    unittest.main(verbosity=2)
