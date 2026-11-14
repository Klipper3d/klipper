import inspect
import io
import json
import os
from pathlib import Path
import sys
import subprocess
import tempfile
import types
import unittest
from unittest.mock import call, patch

from test_integracao import BASE, NATIVE_MANAGER, ROOT, import_module, installer

CONFIG_READER = import_module('v2_config_reader', ROOT / 'klippy/configfile.py')

class V2InstallationTests(unittest.TestCase):
    def bench(self, home):
        config = home / 'printer_data/config'
        extras = home / 'klipper/klippy/extras'
        (config / 'custom').mkdir(parents=True)
        (extras / 'bmcu_core').mkdir(parents=True)
        (extras / 'bmcu_core/endpoints.py').write_text(
            (BASE / 'endpoints_original.txt').read_text()
        )
        (extras / 'bmcu_core/manager.py').write_text(NATIVE_MANAGER)
        daemon = config / 'bmcu/runtime/scripts/bmcu_transportd.py'
        daemon.parent.mkdir(parents=True)
        daemon.write_text((BASE / 'transport_original.txt').read_text())
        saved = '#*# [bltouch]\n#*# z_offset = 1.950\n'
        (config / 'printer.cfg').write_text(
            '[include custom/*.cfg]\n' + saved
        )
        (config / 'custom/manual.cfg').write_text(
            '[gcode_macro MY_CUSTOM_PRINT]\ngcode:\n'
            '  CORTADOR_SERVO POSICAO=RECOLHIDO CONFIRMAR=1\n'
        )
        (config / 'bmcu-cortador.cfg').write_text(
            '[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]\n'
            'variable_habilitado: 1\nvariable_calibrado: 1\n'
            'variable_angulo_recolhido: 170.0\n'
            'variable_angulo_acionado: 10.0\ngcode:\n  M117 profile\n'
        )
        state = home / 'private/state.json'
        state.parent.mkdir()
        state.write_text(json.dumps({
            'endpoints': {'extruder': {
                'toolhead_prepare_macro': 'BMCU_PREPARAR_ENDER',
                'before_pullback_macro': 'BMCU_LIBERAR_ENDER',
                'custom_value': 31.75,
            }},
            'devices': {'TEST_DEVICE': {'calibration': [3, 5, 7]}},
            'print_session': {'active': False},
        }))
        return config, extras, state, saved

    def prepare(self, home, state):
        parameters = inspect.signature(installer.prepare).parameters
        self.assertIn('state_path', parameters)
        return installer.prepare(home, state_path=state)

    def test_existing_config_and_external_state_migrate_together(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, extras, state, saved = self.bench(home)
            original_state = state.read_bytes()
            original_profile = (config / 'bmcu-cortador.cfg').read_bytes()
            changes = self.prepare(home, state)
            self.assertEqual(original_state, state.read_bytes())
            self.assertEqual(
                original_profile, (config / 'bmcu-cortador.cfg').read_bytes()
            )
            profile = changes[config / 'bmcu-cortador.cfg'].decode()
            self.assertIn('variable_retracted_angle: 170.0', profile)
            self.assertIn('variable_deployed_angle: 10.0', profile)
            custom = changes[config / 'custom/manual.cfg'].decode()
            self.assertIn('[gcode_macro MY_CUSTOM_PRINT]', custom)
            self.assertIn('CUTTER_SERVO POSITION=RETRACTED CONFIRM=1', custom)
            self.assertIn(saved, changes[config / 'printer.cfg'].decode())
            migrated = json.loads(changes[state])
            self.assertEqual(
                'BMCU_PREPARE_ENDER',
                migrated['endpoints']['extruder']['toolhead_prepare_macro'],
            )
            self.assertEqual(
                'BMCU_RELEASE_ENDER',
                migrated['endpoints']['extruder']['before_pullback_macro'],
            )
            self.assertEqual(
                json.loads(original_state)['devices'], migrated['devices']
            )
            self.assertIn(extras / 'bmcu_ender_integration.py', changes)
            self.assertIn(extras / 'bmcu_measurement.py', changes)
            self.assertIn(extras / 'bmcu_cutter_settings.py', changes)
            for path, data in changes.items():
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(data)
            self.assertEqual(changes, self.prepare(home, state))

    def test_native_bmcu_panel_module_is_never_replaced(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, extras, state, _ = self.bench(home)
            native = extras / 'bmcu_panel.py'
            native.write_text('class BMCUPanel:\n    pass\n')
            changes = self.prepare(home, state)
            self.assertNotIn(native, changes)
            self.assertIn(extras / 'bmcu_ender_panel.py', changes)

    def test_monochrome_heating_is_migrated_in_active_nested_includes(self):
        for legacy in (False, True):
            with self.subTest(legacy=legacy), \
                    tempfile.TemporaryDirectory() as temporary:
                home = Path(temporary)
                config, _, state, _ = self.bench(home)
                name = ('ENDER_INICIAR_IMPRESSAO' if legacy
                        else 'ENDER_START_PRINT')
                parameter = 'TEMP_BICO' if legacy else 'NOZZLE_TEMP'
                original = (
                    '[gcode_macro %s]\ngcode:\n'
                    '  {%% set bico = params.%s|float %%}\n'
                    '  G1 X5 Y20 F6000\n  M109 S{bico}\n'
                    '\n[gcode_macro KEEP_CUSTOM]\ngcode:\n  M117 custom\n'
                ) % (name, parameter)
                path = config / 'custom/print.cfg'
                path.write_text(original)
                changes = self.prepare(home, state)
                expected = installer.corrigir_aquecimento_inicio(
                    installer.migrate_config(original),
                    'ENDER_START_PRINT', monocor=True,
                ).encode()
                self.assertEqual(expected, changes[path])
                self.assertEqual(original, path.read_text())
                for target, data in changes.items():
                    target.parent.mkdir(parents=True, exist_ok=True)
                    target.write_bytes(data)
                self.assertEqual(changes, self.prepare(home, state))

    def test_unknown_nested_monochrome_heating_fails_without_writes(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            (config / 'custom/print.cfg').write_text(
                '[gcode_macro ENDER_START_PRINT]\ngcode:\n'
                '  G1 X5 Y20 F6000\n  M109 S{custom_temperature}\n'
            )
            before = {p: p.read_bytes() for p in home.rglob('*')
                      if p.is_file()}
            with self.assertRaisesRegex(RuntimeError, 'Manually review'):
                self.prepare(home, state)
            self.assertEqual(before, {p: p.read_bytes() for p in home.rglob('*')
                                      if p.is_file()})

    def test_monochrome_variable_override_keeps_the_included_gcode_body(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, saved = self.bench(home)
            root = config / 'printer.cfg'
            root.write_text('[include impressao.cfg]\n'
                            '[include custom/*.cfg]\n' + saved)
            body = config / 'impressao.cfg'
            original = ('[gcode_macro ENDER_START_PRINT]\ngcode:\n'
                        '  G1 X5 Y20 F6000\n  M109 S{nozzle_temp}\n')
            body.write_text(original)
            override = config / 'custom/override.cfg'
            variables = ('[gcode_macro ENDER_START_PRINT]\n'
                         'variable_some_flag: 1\n')
            override.write_text(variables)
            reader = CONFIG_READER.ConfigFileReader()
            merged = reader.build_fileconfig_with_includes(
                root.read_text(), str(root))
            self.assertIn('M109', merged.get(
                'gcode_macro ENDER_START_PRINT', 'gcode'))
            self.assertEqual('1', merged.get(
                'gcode_macro ENDER_START_PRINT', 'variable_some_flag'))
            changes = self.prepare(home, state)
            self.assertEqual(variables.encode(), changes[override])
            self.assertIn(b'BMCU_ENDER_MOVE_TO_PURGE', changes[body])
            for target, data in changes.items():
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(data)
            self.assertEqual(changes, self.prepare(home, state))

    def test_inactive_nested_print_configuration_is_not_migrated(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            path = config / 'inactive/print.cfg'
            path.parent.mkdir()
            original = (
                '[gcode_macro ENDER_INICIAR_IMPRESSAO]\ngcode:\n'
                '  G1 X5 Y20 F6000\n  M109 S{bico}\n'
            )
            path.write_text(original)
            self.assertNotIn(path, self.prepare(home, state))
            self.assertEqual(original, path.read_text())

    def test_collision_fails_before_touching_active_files(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            custom = config / 'custom/manual.cfg'
            custom.write_text(
                '[gcode_macro CORTADOR_SERVO]\ngcode:\n  M117 old\n'
                '[gcode_macro CUTTER_SERVO]\ngcode:\n  M117 custom\n'
            )
            before = {
                path: path.read_bytes() for path in home.rglob('*')
                if path.is_file()
            }
            with self.assertRaisesRegex(ValueError, 'collision|conflict'):
                self.prepare(home, state)
            self.assertEqual(before, {
                path: path.read_bytes() for path in home.rglob('*')
                if path.is_file()
            })

    def test_unsupported_status_templates_fail_before_active_file_changes(self):
        templates = (
            "{% set state = printer['bmcu_integracao_ender'] %}\n"
            "  {% set cut = state.ultimo_corte %}\n"
            "  {% if cut.resultado == 'interrompido' %}\n"
            '    M117 CUT_FAILED\n  {% endif %}\n',
            '{% if printer.bmcu_integracao_ender.manual_busy %}\n'
            '    M117 BUSY\n  {% endif %}\n',
        )
        for template in templates:
            with self.subTest(template=template), \
                    tempfile.TemporaryDirectory() as temporary:
                home = Path(temporary)
                config, _, state, _ = self.bench(home)
                (config / 'custom/manual.cfg').write_text(
                    '[gcode_macro STATUS]\ngcode:\n  ' + template,
                )
                before = {p: p.read_bytes() for p in home.rglob('*')
                          if p.is_file()}
                with self.assertRaisesRegex(ValueError, 'manual review'):
                    self.prepare(home, state)
                self.assertEqual(before, {p: p.read_bytes()
                                         for p in home.rglob('*')
                                         if p.is_file()})

    def test_old_and_new_sections_in_separate_includes_are_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            (config / 'custom/old.cfg').write_text(
                '[gcode_macro CORTADOR_SERVO]\ngcode:\n  M117 old\n'
            )
            (config / 'custom/new.cfg').write_text(
                '[gcode_macro CUTTER_SERVO]\ngcode:\n  M117 custom\n'
            )
            with self.assertRaisesRegex(ValueError, 'collision|conflict'):
                self.prepare(home, state)

    def test_old_and_new_cutter_fields_across_includes_are_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            root = config / 'printer.cfg'
            root.write_text(
                '[include bmcu-cortador.cfg]\n[include custom/*.cfg]\n'
            )
            (config / 'custom/override.cfg').write_text(
                '[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]\n'
                'variable_retracted_angle: 20.0\n'
            )
            reader = CONFIG_READER.ConfigFileReader()
            parsed = reader.build_fileconfig_with_includes(
                root.read_text(), str(root),
            )
            self.assertEqual(170.0, parsed.getfloat(
                'gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES',
                'variable_angulo_recolhido',
            ))
            original = root.read_bytes()
            with self.assertRaisesRegex(ValueError, 'option collision'):
                self.prepare(home, state)
            self.assertEqual(original, root.read_bytes())

    def test_include_cycles_and_external_migrations_fail_without_writes(self):
        for cycle in (True, False):
            with self.subTest(cycle=cycle), \
                    tempfile.TemporaryDirectory() as tmp:
                home = Path(tmp)
                config, _, state, _ = self.bench(home)
                custom = config / 'custom/manual.cfg'
                if cycle:
                    custom.write_text('[include ../printer.cfg]\n')
                    expected = 'Recursive'
                else:
                    external = home / 'shared.cfg'
                    external.write_text(
                        '[gcode_macro MY_EXTERNAL]\ngcode:\n'
                        '  CORTADOR_SERVO POSICAO=RECOLHIDO CONFIRMAR=1\n'
                    )
                    custom.write_text('[include ../../../shared.cfg]\n')
                    expected = 'external include'
                before = {
                    p: p.read_bytes() for p in home.rglob('*') if p.is_file()
                }
                with self.assertRaisesRegex(ValueError, expected):
                    self.prepare(home, state)
                self.assertEqual(before, {
                    p: p.read_bytes() for p in home.rglob('*') if p.is_file()
                })

    def test_same_legacy_section_overrides_keep_include_order(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            (config / 'custom/override.cfg').write_text(
                '[gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES]\n'
                'variable_angulo_recolhido: 165.5\n'
            )
            changes = self.prepare(home, state)
            self.assertIn(
                b'variable_retracted_angle: 165.5',
                changes[config / 'custom/override.cfg'],
            )

    def test_cli_passes_the_live_custom_state_path_to_preparation(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, _, state, _ = self.bench(home)
            user = types.SimpleNamespace(
                pw_dir=str(home), pw_uid=os.getuid(), pw_gid=os.getgid(),
            )

            def prepare(*args, **kwargs):
                self.assertEqual(state, kwargs.get('state_path'))
                raise RuntimeError('Preparation inspected; no writes')

            status = {'config': {'bmcu': {'state_file': str(state)}}}
            with patch.object(sys, 'argv', ['install_multicolor']), \
                    patch.object(installer, 'check_printer'), \
                    patch.object(installer, 'query_object',
                                 return_value=status), \
                    patch.object(installer.pwd, 'getpwuid',
                                 return_value=user), \
                    patch.object(installer.pwd, 'getpwnam',
                                 return_value=user), \
                    patch.object(installer, 'prepare', side_effect=prepare):
                with self.assertRaisesRegex(RuntimeError, 'no writes'):
                    installer.main()

    def run_cli(
        self, home, state, fail_ready=False, before_failure=None,
        on_service=None, host_source=None, events=None,
    ):
        user = installer.pwd.struct_passwd((
            'pi', 'x', os.getuid(), os.getgid(), 'Test', str(home), '/bin/sh',
        ))
        status = {'config': {'bmcu': {'state_file': str(state)},
                             'bmcu_integracao_ender': {}}}
        arguments = ['install_multicolor']
        if host_source:
            arguments += ['--host-source', str(host_source)]
        with patch.object(sys, 'argv', arguments), \
                patch.object(installer, 'ROOT',
                             host_source or installer.ROOT), \
                patch.object(installer, 'check_printer'), \
                patch.object(installer, 'query_object', return_value=status), \
                patch.object(installer.pwd, 'getpwuid', return_value=user), \
                patch.object(installer.pwd, 'getpwnam', return_value=user), \
                patch.object(installer, 'service') as service, \
                patch.object(installer.runtime_lifecycle,
                             'RuntimeTransport') as runtime, \
                patch.object(installer, 'wait_ready') as ready:
            def service_action(*args):
                if events is not None:
                    events.append(('service', *args))
                if on_service:
                    on_service(*args)
            service.side_effect = service_action
            for action in ('check_active', 'stop', 'start'):
                if events is not None:
                    getattr(runtime.return_value, action).side_effect = (
                        lambda a=action: events.append(('transport', a)))
            if fail_ready:
                def fail(required_section='bmcu_ender_integration'):
                    if host_source and ready.call_count > 1:
                        return
                    if before_failure:
                        before_failure()
                    raise RuntimeError('simulated readiness failure')
                ready.side_effect = fail
            installer.main()
            return service.call_args_list

    def test_generated_rollback_restores_callbacks_not_stale_routes(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, extras, state, _ = self.bench(home)
            original = (config / 'bmcu-cortador.cfg').read_bytes()
            self.run_cli(home, state)
            backup = next(home.glob('backup-multicor-*'))
            source = (backup / 'restaurar.py').read_text()
            code = compile(source, str(backup / 'restaurar.py'), 'exec')
            current = json.loads(state.read_text())
            current['devices']['TEST_DEVICE']['route_state'] = 'PARKED'
            current['endpoints']['extruder']['custom_value'] = 70.25
            state.write_text(json.dumps(current))
            with patch.object(installer.subprocess, 'run') as service, \
                    patch.object(installer.runtime_lifecycle,
                                 'RuntimeTransport') as runtime:
                exec(code, {'__file__': str(backup / 'restaurar.py')})
            self.assertEqual([
                call(['systemctl', 'is-active', '--quiet', 'klipper']),
                call(['systemctl', 'stop', 'klipper'], check=True),
            ], service.call_args_list)
            runtime.return_value.stop.assert_called_once_with()
            runtime.return_value.start.assert_not_called()
            result = json.loads(state.read_text())
            self.assertEqual(current['devices'], result['devices'])
            self.assertEqual(
                70.25, result['endpoints']['extruder']['custom_value']
            )
            self.assertEqual(
                'BMCU_PREPARAR_ENDER',
                result['endpoints']['extruder']['toolhead_prepare_macro'],
            )
            self.assertEqual(
                original, (config / 'bmcu-cortador.cfg').read_bytes(),
            )
            self.assertFalse((extras / 'bmcu_ender_integration.py').exists())

    def test_failed_startup_restores_all_original_active_files(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, extras, state, _ = self.bench(home)
            before = {
                path: path.read_bytes() for path in home.rglob('*')
                if path.is_file()
            }
            with self.assertRaisesRegex(RuntimeError, 'readiness failure'):
                self.run_cli(home, state, fail_ready=True)
            for path, data in before.items():
                self.assertEqual(data, path.read_bytes(), str(path))
            self.assertFalse((extras / 'bmcu_ender_integration.py').exists())
            self.assertFalse((extras / 'bmcu_measurement.py').exists())

    def test_failed_startup_does_not_restore_stale_runtime_routes(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, _, state, _ = self.bench(home)

            def changed_runtime():
                current = json.loads(state.read_text())
                current['devices']['TEST_DEVICE']['route_state'] = 'UNCERTAIN'
                state.write_text(json.dumps(current))

            with self.assertRaisesRegex(RuntimeError, 'readiness failure'):
                self.run_cli(home, state, True, changed_runtime)
            result = json.loads(state.read_text())
            self.assertEqual(
                'UNCERTAIN',
                result['devices']['TEST_DEVICE'].get('route_state'),
            )
            self.assertEqual(
                'BMCU_PREPARAR_ENDER',
                result['endpoints']['extruder']['toolhead_prepare_macro'],
            )

    def test_state_is_reread_after_stopping_klipper(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, _, state, _ = self.bench(home)

            def service(action, unit='klipper'):
                if action == 'stop' and unit == 'klipper':
                    current = json.loads(state.read_text())
                    current['last_shutdown'] = 'saved-by-klipper'
                    state.write_text(json.dumps(current))

            self.run_cli(home, state, on_service=service)
            result = json.loads(state.read_text())
            self.assertEqual('saved-by-klipper', result.get('last_shutdown'))

    def test_transport_restarts_before_klipper_uses_the_new_daemon(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, _, state, _ = self.bench(home)
            events = []
            calls = self.run_cli(home, state, events=events)
            self.assertEqual([
                call('stop'), call('start'),
            ], calls)
            self.assertEqual([
                ('transport', 'check_active'), ('service', 'stop'),
                ('transport', 'stop'), ('transport', 'start'),
                ('service', 'start'),
            ], events)

    def test_included_language_is_preserved_and_moved_to_autosave_root(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            display = config / 'custom/display.cfg'
            display.write_text('[e3v3se_display]\nlanguage: pt_BR\n')
            changes = self.prepare(home, state)
            self.assertNotRegex(
                changes[display].decode(), r'(?m)^language\s*:',
            )
            self.assertIn(b'language: pt_BR', changes[config / 'printer.cfg'])
            for path, data in changes.items():
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(data)
            self.assertEqual(changes, self.prepare(home, state))

    def test_failed_upgrade_does_not_restart_a_mismatched_host_checkout(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, _, state, _ = self.bench(home)
            calls = []
            with self.assertRaisesRegex(RuntimeError, 'readiness failure'):
                self.run_cli(
                    home, state, True,
                    on_service=lambda *args: calls.append(args),
                )
            self.assertEqual([('stop',), ('start',), ('stop',)], calls)

    def staged_source(self, home):
        active = home / 'klipper'
        display = Path('klippy/extras/e3v3se_display/__init__.py')
        (active / display).parent.mkdir(parents=True)
        (active / display).write_text('OLD_DISPLAY = True\n')

        def git(path, *arguments):
            return subprocess.check_output(
                ['git', '-C', str(path), *arguments], text=True,
                stderr=subprocess.PIPE,
            ).strip()

        def commit(path):
            git(path, '-c', 'user.name=Synthetic Test',
                '-c', 'user.email=test@example.invalid',
                'commit', '-qm', 'test')

        git(active, 'init', '-q')
        git(active, 'add', str(display))
        commit(active)
        before = git(active, 'rev-parse', 'HEAD')
        source = home / 'staged-source'
        git(home, 'clone', '-q', str(active), str(source))
        (source / display).write_text('NEW_DISPLAY = True\n')
        git(source, 'add', str(display))
        commit(source)
        return source, display, before

    def test_failure_restores_host_display_config_and_current_routes(self):
        self.assertIn('--host-source', inspect.getsource(installer.main))
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            config, _, state, _ = self.bench(home)
            source, display, before = self.staged_source(home)
            original = (config / 'bmcu-cortador.cfg').read_bytes()

            def changed_runtime():
                current = json.loads(state.read_text())
                current['devices']['TEST_DEVICE']['route_state'] = 'UNCERTAIN'
                state.write_text(json.dumps(current))

            with self.assertRaisesRegex(RuntimeError, 'readiness failure'):
                self.run_cli(home, state, True, changed_runtime,
                             host_source=source)
            self.assertEqual('OLD_DISPLAY = True\n',
                             (home / 'klipper' / display).read_text())
            restored = subprocess.check_output(
                ['git', '-C', str(home / 'klipper'), 'rev-parse', 'HEAD'],
                text=True,
            ).strip()
            self.assertEqual(before, restored)
            self.assertEqual(original,
                             (config / 'bmcu-cortador.cfg').read_bytes())
            current = json.loads(state.read_text())
            self.assertEqual('UNCERTAIN',
                             current['devices']['TEST_DEVICE']['route_state'])
            self.assertEqual('BMCU_PREPARAR_ENDER', current['endpoints']
                             ['extruder']['toolhead_prepare_macro'])

    def test_generated_staged_restorer_recovers_the_complete_host_tree(self):
        self.assertIn('--host-source', inspect.getsource(installer.main))
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            _, _, state, _ = self.bench(home)
            source, display, _ = self.staged_source(home)
            self.run_cli(home, state, host_source=source)
            self.assertEqual('NEW_DISPLAY = True\n',
                             (home / 'klipper' / display).read_text())
            backup = next(home.glob('backup-multicor-*'))
            code = compile((backup / 'restaurar.py').read_text(),
                           str(backup / 'restaurar.py'), 'exec')

            def response(url, **kwargs):
                key = 'configfile' if 'configfile' in url else 'webhooks'
                value = {'state': 'ready'} if key == 'webhooks' else {
                    'config': {'bmcu_integracao_ender': {}}}
                return io.StringIO(json.dumps({
                    'result': {'status': {key: value}},
                }))

            run = subprocess.run

            def services_only(command, **kwargs):
                if command[0] == 'systemctl':
                    return subprocess.CompletedProcess(command, 3)
                return run(command, **kwargs)

            with patch.object(installer.subprocess, 'run',
                              side_effect=services_only), \
                    patch.object(installer.runtime_lifecycle,
                                 'RuntimeTransport'), \
                    patch('urllib.request.urlopen', side_effect=response):
                exec(code, {'__file__': str(backup / 'restaurar.py')})
            self.assertEqual('OLD_DISPLAY = True\n',
                             (home / 'klipper' / display).read_text())


if __name__ == '__main__':
    unittest.main()
