import hashlib
import json
import re
import shutil
import sys
import tempfile
import types
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
SCRIPTS = ROOT / 'scripts/ender3-v3-se'
sys.path.insert(0, str(SCRIPTS))
sys.path.insert(0, str(ROOT / 'klippy'))
from configfile import ConfigFileReader
import restaurar_snapshot as restorer
import capturar_configuracao as collector
import verificar_backup as verifier


class SnapshotTest(unittest.TestCase):
    def test_restore_cli_accepts_apply_aliases(self):
        for flag in ('--apply', '--aplicar'):
            with self.subTest(
                flag=flag
            ), tempfile.TemporaryDirectory() as temp:
                home = Path(temp)
                self.bench(home)
                arguments = [
                    'restore_snapshot',
                    '--home',
                    str(home),
                    '--snapshot',
                    str(self.snapshot),
                    flag,
                ]
                with patch.object(sys, 'argv', arguments), patch.object(
                    restorer, 'check_application'
                ), patch.object(restorer, 'apply') as apply, patch.object(
                    restorer.os, 'geteuid', return_value=0
                ):
                    restorer.main()
                    apply.assert_called_once()

    def test_capture_cli_accepts_output_aliases(self):
        for output_flag, services_flag in (
            ('--output', '--services-only'),
            ('--saida', '--complemento'),
        ):
            with self.subTest(
                flag=output_flag
            ), tempfile.TemporaryDirectory() as temp:
                home = Path(temp)
                output = home / 'private-backup.tar.gz'

                def capture_environment(home, output):
                    output.write_bytes(b'synthetic-archive')
                    return []

                arguments = [
                    'capture_configuration',
                    '--home',
                    str(home),
                    output_flag,
                    str(output),
                    services_flag,
                ]
                with patch.object(sys, 'argv', arguments), patch.object(
                    collector,
                    'capture_service_environment',
                    side_effect=capture_environment,
                ) as capture, patch.object(
                    collector.os, 'geteuid', return_value=1000
                ):
                    collector.main()
                    capture.assert_called_once_with(home, output)

    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.snapshot = Path(temporary.name) / 'snapshot'
        self.snapshot.mkdir()
        rows = ''.join(
            '#*# ' + ', '.join(['0.000'] * 9) + '\n' for _ in range(9)
        )
        files = {
            'config/printer.cfg': '[include ender3-v3-se/motion.cfg]\n'
            '#*# <---------------------- SAVE_CONFIG ---------------------->\n'
            '#*# [bltouch]\n#*# z_offset = 1.234\n'
            '#*# [bed_mesh default]\n#*# x_count = 9\n#*# y_count = 9\n'
            '#*# points =\n' + rows,
            ('config/ender3-v3-se/motion.cfg'): (
                '[extruder]\nrotation_distance: ' '7.000\n'
            ),
            'config/bmcu/bmcu_panel.cfg': '[bmcu_panel]\naccess_token: '
            + restorer.TOKEN_PLACEHOLDER
            + '\n',
            'config/bmcu_state.json': json.dumps(
                {
                    'devices': {
                        'TEST_DEVICE': {
                            'motion_config': {'load_speed_mms': 10}
                        }
                    },
                    'print_session': {'active': False},
                    'endpoints': {
                        'extruder': {
                            'toolhead_prepare_macro': 'BMCU_PREPARAR_ENDER'
                        }
                    },
                }
            ),
            'metadados.json': json.dumps(
                {
                    'klipper_commit': '0' * 40,
                    'calibracoes': {'z_offset': 1.234},
                    'firmware_bin_capturado': {
                        'gravado_na_placa': 'hash_nao_verificavel_por_status'
                    },
                }
            ),
            'versoes-mcus.json': json.dumps(
                {
                    'mcu': {
                        'mcu_version': 'test-printer-version',
                        'mcu_constants': {'E3V3SE_DISPLAY_BRIDGE': 1},
                    },
                    'mcu host': {'mcu_version': 'test-host-version'},
                }
            ),
        }
        for name in ('klipper', 'moonraker', 'crowsnest', 'sonar'):
            files['ambiente-servicos/' + name + '.env'] = (
                'TEST_SETTING=example\n'
            )
        for name, content in files.items():
            path = self.snapshot / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content)
        hashes = {
            name: hashlib.sha256(
                (self.snapshot / name).read_bytes()
            ).hexdigest()
            for name in files
        }
        (self.snapshot / 'integridade.json').write_text(json.dumps(hashes))

    def bench(self, directory):
        configurations = directory / 'printer_data/config'
        runtime = configurations / 'bmcu/runtime'
        runtime.mkdir(parents=True)
        (runtime / 'version').write_text('package=1.0.6\nfirmware=1.0.1\n')
        (runtime / 'INSTALLATION.json').write_text('{}')
        extras = directory / 'klipper/klippy/extras/bmcu_core'
        extras.mkdir(parents=True)
        fixtures = Path(__file__).parent / 'fixtures'
        for name in ('endpoints', 'manager'):
            (extras / (name + '.py')).write_text(
                (fixtures / (name + '_original.txt')).read_text()
            )
        return configurations

    def test_synthetic_snapshot_integrity_calibration_and_includes(self):
        snapshot = self.snapshot
        metadata = restorer.verify(snapshot)
        self.assertEqual(metadata['calibracoes']['z_offset'], 1.234)
        cfg = snapshot / 'config'
        printer = (cfg / 'printer.cfg').read_text()
        self.assertIn('#*# z_offset = 1.234', printer)
        self.assertIn('#*# x_count = 9', printer)
        self.assertIn('#*# y_count = 9', printer)
        lines = printer.split('#*# points =\n', 1)[1].splitlines()[:9]
        self.assertEqual(len(lines), 9)
        self.assertTrue(all(len(line.split(',')) == 9 for line in lines))
        for path in cfg.rglob('*.cfg'):
            for include in re.findall(
                r'^\[include\s+([^\]]+)\]', path.read_text(), re.M
            ):
                self.assertTrue((path.parent / include).is_file(), include)
        motion = (cfg / 'ender3-v3-se/motion.cfg').read_text()
        self.assertIn('rotation_distance: 7.000', motion)
        status = json.loads((cfg / 'bmcu_state.json').read_text())
        device = next(iter(status['devices'].values()))
        self.assertEqual(device['motion_config']['load_speed_mms'], 10)
        self.assertFalse(status['print_session']['active'])
        self.assertEqual(
            status['endpoints']['extruder']['toolhead_prepare_macro'],
            'BMCU_PREPARAR_ENDER',
        )

    def test_snapshot_macros_use_klipper_syntax(self):
        from jinja2 import Environment

        environment = Environment(
            block_start_string='{%',
            block_end_string='%}',
            variable_start_string='{',
            variable_end_string='}',
        )
        for path in (self.snapshot / 'config').rglob('*.cfg'):
            cfg = ConfigFileReader().build_fileconfig(
                path.read_text(), str(path)
            )
            for section in cfg.sections():
                if cfg.has_option(section, 'gcode'):
                    environment.parse(cfg.get(section, 'gcode'))

    def test_prepare_preserves_save_config_and_existing_token(self):
        with tempfile.TemporaryDirectory() as temp:
            home = Path(temp)
            cfg = self.bench(home)
            token = 'a' * 64
            (cfg / 'bmcu/bmcu_panel.cfg').write_text('access_token: ' + token)
            plan = restorer.prepare(home, self.snapshot)
            self.assertEqual(
                plan[cfg / 'printer.cfg'],
                (self.snapshot / 'config/printer.cfg').read_bytes(),
            )
            self.assertIn(token.encode(), plan[cfg / 'bmcu/bmcu_panel.cfg'])
            self.assertFalse((cfg / 'printer.cfg').exists())
            metadata = json.loads(plan[cfg / 'bmcu/runtime/INSTALLATION.json'])
            self.assertEqual(
                metadata['panel_token_sha256'],
                hashlib.sha256(token.encode()).hexdigest(),
            )

    def test_local_panel_token_is_generated(self):
        with tempfile.TemporaryDirectory() as temp:
            home = Path(temp)
            cfg = self.bench(home)
            plan = restorer.prepare(home, self.snapshot)
            text = plan[cfg / 'bmcu/bmcu_panel.cfg'].decode()
            self.assertNotIn(restorer.TOKEN_PLACEHOLDER, text)
            token = re.search(r'access_token:\s*(\w+)', text)[1]
            self.assertEqual(len(token), 64)
            self.assertNotEqual(token, restorer.panel_token(cfg))
            reference = (
                self.snapshot / 'config/bmcu/bmcu_panel.cfg'
            ).read_text()
            self.assertIn(restorer.TOKEN_PLACEHOLDER, reference)

    def test_service_environment_prepare_does_not_modify_destination(self):
        with tempfile.TemporaryDirectory() as temp:
            home = Path(temp)
            self.bench(home)
            destination = home / 'printer_data/systemd'
            destination.mkdir()
            previous = destination / 'klipper.env'
            previous.write_text('configuracao anterior')
            plan = restorer.prepare(home, self.snapshot)
            source_path = self.snapshot / 'ambiente-servicos'
            self.assertEqual(len(list(source_path.glob('*.env'))), 4)
            for file_path in source_path.glob('*.env'):
                self.assertEqual(
                    plan[destination / file_path.name], file_path.read_bytes()
                )
            self.assertEqual(previous.read_text(), 'configuracao anterior')
            self.assertEqual(list(destination.iterdir()), [previous])

    def test_reported_versions_do_not_claim_flash_identity(self):
        metadata = restorer.verify(self.snapshot)
        file_path = self.snapshot / 'versoes-mcus.json'
        versions = json.loads(file_path.read_text())
        self.assertEqual(
            versions['mcu']['mcu_version'], 'test-printer-version'
        )
        self.assertEqual(
            versions['mcu host']['mcu_version'], 'test-host-version'
        )
        self.assertEqual(
            versions['mcu']['mcu_constants']['E3V3SE_DISPLAY_BRIDGE'], 1
        )
        self.assertEqual(
            metadata['firmware_bin_capturado']['gravado_na_placa'],
            'hash_nao_verificavel_por_status',
        )

    def test_snapshot_rejects_changes_and_extra_files(self):
        with tempfile.TemporaryDirectory() as temp:
            snapshot = Path(temp) / 'snapshot'
            shutil.copytree(self.snapshot, snapshot)
            extra = snapshot / 'config/extra.cfg'
            extra.write_text('extra')
            with self.assertRaisesRegex(ValueError, 'extra'):
                restorer.verify(snapshot)
            extra.unlink()
            with (snapshot / 'config/printer.cfg').open('a') as file_path:
                file_path.write('changed')
            with self.assertRaisesRegex(ValueError, 'changed'):
                restorer.verify(snapshot)

    def test_failed_apply_restores_previous_files(self):
        with tempfile.TemporaryDirectory() as temp:
            home = Path(temp)
            cfg = home / 'printer.cfg'
            cfg.write_text('calibracao anterior')
            second = home / 'state.json'
            second.write_text('estado anterior')
            user = types.SimpleNamespace(pw_uid=0, pw_gid=0)
            write_file = restorer.installer.write_file

            def simulate_failure(path, data, user, mode=0o644):
                if data == b'novo':
                    raise OSError('falha de disco')
                return write_file(path, data, user, mode)

            with patch.object(restorer.installer, 'service'), patch.object(
                restorer.installer, 'write_file', side_effect=simulate_failure
            ):
                with self.assertRaisesRegex(OSError, 'falha de disco'):
                    restorer.apply(
                        home,
                        {cfg: b'primeira escrita', second: b'novo'},
                        user,
                    )
            self.assertEqual(cfg.read_text(), 'calibracao anterior')
            self.assertEqual(second.read_text(), 'estado anterior')

    def test_service_capture_does_not_stop_printer(self):
        with tempfile.TemporaryDirectory() as temp:
            home = Path(temp)
            env = home / 'printer_data/systemd'
            env.mkdir(parents=True)
            content = 'KLIPPER_ARGS="klippy.py printer.cfg"\n'
            (env / 'klipper.env').write_text(content)
            output = home / 'complemento.tar.gz'
            with patch.object(collector, 'check_idle') as check, patch.object(
                collector,
                'query_object',
                return_value={
                    'mcu_version': 'versao-de-teste',
                },
            ):
                collector.capture_service_environment(home, output)
                check.assert_not_called()
            destination = home / 'revisao'
            verifier.verify(output, destination)
            self.assertEqual(
                (destination / 'ambiente-servicos/klipper.env').read_text(),
                content,
            )


if __name__ == '__main__':
    unittest.main()
