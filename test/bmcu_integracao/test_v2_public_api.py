import ast
import configparser
import re
import types
import unittest

from test_integracao import Gcmd, ROOT, TestBench, import_module, module


MIGRATION = import_module(
    'v2_api_migration', ROOT / 'scripts/ender3-v3-se/migrate_v2.py'
)


class V2PublicApiTests(unittest.TestCase):
    def test_integration_diagnostics_use_english_and_current_parameters(self):
        legacy = re.compile(
            r'^(?:Posicao de|O posicionamento|A referencia|'
            r'A extrusora esfriou|A ponta precisa)|\bVELOCIDADE='
        )
        messages = []
        for name in ('bmcu_ender_integration.py', 'bmcu_measurement.py'):
            tree = ast.parse((ROOT / 'integrations/bmcu' / name).read_text())
            messages.extend(node.value for node in ast.walk(tree)
                            if isinstance(node, ast.Constant)
                            and isinstance(node.value, str)
                            and legacy.search(node.value))
        self.assertEqual([], messages)

    def test_installer_diagnostics_do_not_keep_legacy_portuguese_messages(self):
        path = ROOT / 'scripts/ender3-v3-se/instalar_multicor_bmcu.py'
        tree = ast.parse(path.read_text())
        legacy = re.compile(
            r'^(?:Revise manualmente|Configure a MCU|O pino do servo|'
            r'O arquivo do servo|PC9/pino|Use um GPIO)'
        )
        messages = [node.value for node in ast.walk(tree)
                    if isinstance(node, ast.Constant)
                    and isinstance(node.value, str)
                    and legacy.match(node.value)]
        self.assertEqual([], messages)

    def test_reset_diagnostic_is_english_with_the_current_command(self):
        b = TestBench()
        with self.assertRaisesRegex(
            RuntimeError, 'Manually remove.*RESET_BMCU CONFIRM=1',
        ):
            b.obj.resetar_bmcu(Gcmd())

    def test_public_status_is_english_without_mutating_motion_state(self):
        b = TestBench()
        b.obj.manual_operation = None
        original = {'resultado': 'interrompido', 'etapa': 'curso_da_lamina',
                    'posicao_incerta': True, 'segundos': 2.5}
        b.obj.cortador.ultimo_corte = dict(original)
        b.obj.last_unload_timing = {'sincronismo_pre_corte': {
            'antes_passo': {'recuo_encoder_mm': 0.5, 'movimento': 0},
        }}
        status = b.obj.get_status(0.)
        self.assertNotIn('ultimo_corte', status)
        self.assertIn('last_cut', status)
        self.assertEqual({
            'result': 'interrupted', 'stage': 'blade_stroke',
            'position_uncertain': True, 'seconds': 2.5,
        }, status['last_cut'])
        self.assertEqual(0.5, status['unload_timing']['pre_cut_sync']
                         ['before_step']['encoder_retract_mm'])
        self.assertEqual(original, b.obj.cortador.ultimo_corte)

    def test_integration_registers_only_canonical_english_commands(self):
        b = TestBench()
        b.printer.get_reactor = lambda: b.reactor
        b.bmcu._wait_pullback_safe = lambda *args: None
        loaded = []

        def load(config, name):
            loaded.append(name)
            return b.bmcu if name == 'bmcu' else b.obj.measurement

        b.printer.load_object = load
        lookup = b.printer.lookup_object
        b.printer.lookup_object = lambda name, default=None: (
            b.gcode if name == 'gcode' else lookup(name, default)
        )
        b.printer.register_event_handler = lambda *args: None
        config = types.SimpleNamespace(
            error=RuntimeError,
            get_printer=lambda: b.printer,
            get=lambda name, default: default,
            getint=lambda name, default, **limits: default,
            getfloat=lambda name, default, **limits: default,
        )
        module.EnderIntegration(config)
        self.assertEqual(
            {
                'BMCU_ENDER_LOAD', 'BMCU_ENDER_UNLOAD',
                'BMCU_CHANGE_TOOL_ENDER', 'BMCU_ENDER_MANUAL',
                'BMCU_CONFIRM_CLEAR_PATH', 'BMCU_ENDER_SERVO',
                'BMCU_ENDER_TEST_CUTTER', 'BMCU_ENDER_RESET',
                'BMCU_ENDER_MOVE_TO_PURGE',
                'BMCU_ENDER_SET_FILAMENT',
            }, set(b.handlers),
        )
        self.assertIn('bmcu_measurement', loaded)

    def test_canonical_english_configuration_modules_exist(self):
        for name in ('bmcu_ender_integration.py', 'bmcu_measurement.py'):
            with self.subTest(name=name):
                self.assertTrue((ROOT / 'integrations/bmcu' / name).is_file())

    def test_macro_names_and_cutter_fields_are_english(self):
        parser = configparser.RawConfigParser()
        paths = list((ROOT / 'config/ender3-v3-se-validada').glob('*.cfg'))
        for path in paths:
            if path.name != 'bmcu-cortador-servo.cfg':
                parser.read(path)
        macros = {
            section.split(None, 1)[1] for section in parser.sections()
            if section.startswith('gcode_macro ')
        }
        self.assertFalse(macros.intersection(MIGRATION.COMMANDS))
        self.assertIn('CUTTER_SERVO', macros)
        self.assertIn('LOAD_CHANNEL_1', macros)
        self.assertIn('UNLOAD_ACTIVE_CHANNEL', macros)
        section = 'gcode_macro _BMCU_ENDER_CUTTER_SETTINGS'
        self.assertIn(section, parser.sections())
        settings = parser[section]
        for key in MIGRATION.CUTTER_SETTINGS.values():
            self.assertIn('variable_' + key, settings)

    def test_public_handlers_do_not_read_portuguese_parameters(self):
        paths = (
            'integrations/bmcu/bmcu_ender_integration.py',
            'integrations/bmcu/bmcu_measurement.py',
            'integrations/bmcu/bmcu_cortador_ender.py',
        )
        for name in paths:
            with self.subTest(name=name):
                path = ROOT / name
                if not path.exists():
                    path = ROOT / name.replace(
                        'bmcu_ender_integration', 'bmcu_integracao_ender'
                    ).replace('bmcu_measurement', 'bmcu_medicao')
                tree = ast.parse(path.read_text())
                parameters = {
                    node.args[0].value for node in ast.walk(tree)
                    if isinstance(node, ast.Call)
                    and isinstance(node.func, ast.Attribute)
                    and isinstance(node.func.value, ast.Name)
                    and node.func.value.id == 'gcmd'
                    and node.func.attr in ('get', 'get_int', 'get_float')
                    and node.args and isinstance(node.args[0], ast.Constant)
                }
                self.assertFalse(parameters.intersection(MIGRATION.PARAMETERS))

    def test_display_never_emits_portuguese_commands(self):
        path = ROOT / 'klippy/extras/e3v3se_display/__init__.py'
        tree = ast.parse(path.read_text())
        emitted = set()
        for node in ast.walk(tree):
            if isinstance(node, ast.Constant) and isinstance(node.value, str):
                emitted.update(re.findall(r'\b[A-Z][A-Z_0-9]+\b', node.value))
        self.assertFalse(emitted.intersection(MIGRATION.COMMANDS))

    def test_legacy_parameters_are_rejected_before_the_handler_runs(self):
        b = TestBench()
        executed = []
        handler = b.obj.protect(lambda command: executed.append(True))
        for name in MIGRATION.PARAMETERS:
            with self.subTest(name=name):
                with self.assertRaisesRegex(RuntimeError, 'English|v2'):
                    handler(Gcmd(**{name: 1}))
        self.assertEqual([], executed)
        handler(Gcmd(CONFIRM=1, CHANNEL=1))
        self.assertEqual([True], executed)


if __name__ == '__main__':
    unittest.main()
