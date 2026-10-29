import importlib.util
import io
import json
import sys
import tarfile
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]


def import_module(name):
    file_path = ROOT / 'scripts/ender3-v3-se' / (name + '.py')
    spec = importlib.util.spec_from_file_location(name, file_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


capture = import_module('capturar_configuracao')
verifier = import_module('verificar_backup')


class BackupTest(unittest.TestCase):
    def test_verifier_cli_accepts_english_and_legacy_flags(self):
        for flag in ('--extract', '--extrair'):
            with self.subTest(
                flag=flag
            ), tempfile.TemporaryDirectory() as temp:
                directory = Path(temp)
                archive = self.create(directory)
                output = directory / 'extracted'
                with patch.object(
                    sys,
                    'argv',
                    ['verify_backup', str(archive), flag, str(output)],
                ):
                    verifier.main()
                self.assertTrue((output / 'config/printer.cfg').is_file())

    def create(self, directory):
        source_path = directory / 'config'
        source_path.mkdir()
        (source_path / 'printer.cfg').write_text(
            '[bltouch]\n#*# z_offset = 1.887\n'
        )
        (source_path / '.moonraker.secrets').write_text('senha=privada\n')
        (source_path / 'bmcu_state.json').write_text('{"loaded_tool": 4}')
        (source_path / 'backup-antigo.tar.gz').write_bytes(b'backup antigo')
        file_path = directory / 'backup.tar.gz'
        capture_job = capture.BackupCapture(file_path)
        capture_job.directory(source_path, 'config', ('.tar.gz',))
        capture_job.finish(
            {'home_original': '/home/pi', 'criado_utc': '2026-10-20'}
        )
        capture_job.tar.close()
        return file_path

    def test_private_backup_preserves_calibration_state_and_secrets(
        self,
    ):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            file_path = self.create(directory)
            destination = directory / 'revisao'
            manifest = verifier.verify(file_path, destination)
            self.assertEqual(
                (destination / 'config/printer.cfg').read_bytes(),
                (directory / 'config/printer.cfg').read_bytes(),
            )
            self.assertEqual(
                (destination / 'config/.moonraker.secrets').read_text(),
                'senha=privada\n',
            )
            self.assertFalse(
                (destination / 'config/backup-antigo.tar.gz').exists()
            )
            self.assertEqual(len(manifest['arquivos']), 3)
            with self.assertRaises(FileExistsError):
                verifier.verify(file_path, destination)

    def test_archive_rejects_corruption_before_extraction(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            file_path = self.create(directory)
            with tarfile.open(file_path) as tar:
                content = {
                    m.name: tar.extractfile(m).read() for m in tar.getmembers()
                }
            content['config/printer.cfg'] += b'changed'
            with tarfile.open(file_path, 'w:gz') as tar:
                for name, data in content.items():
                    item = tarfile.TarInfo(name)
                    item.size = len(data)
                    tar.addfile(item, io.BytesIO(data))
            with self.assertRaisesRegex(ValueError, 'changed'):
                verifier.verify(file_path, directory / 'revisao')
            self.assertFalse((directory / 'revisao').exists())

    def test_archive_rejects_external_paths_and_links(self):
        with tempfile.TemporaryDirectory() as temp:
            file_path = Path(temp) / 'malicioso.tar.gz'
            for name, kind in [
                ('../fora', tarfile.REGTYPE),
                ('/absoluto', tarfile.REGTYPE),
                ('link', tarfile.SYMTYPE),
                ('link', tarfile.LNKTYPE),
            ]:
                with tarfile.open(file_path, 'w:gz') as tar:
                    item = tarfile.TarInfo(name)
                    item.type = kind
                    item.linkname = (
                        '/etc/passwd' if kind != tarfile.REGTYPE else ''
                    )
                    tar.addfile(item, io.BytesIO(b''))
                with self.assertRaisesRegex(ValueError, 'Invalid entry'):
                    verifier.verify(file_path)

    def test_capture_rejects_changed_source_files(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            source_path = directory / 'printer.cfg'
            source_path.write_text('antes')
            capture_job = capture.BackupCapture(directory / 'backup.tar.gz')
            try:
                capture_job.file_path(
                    source_path, 'config/printer.cfg', required=True
                )
                source_path.write_text('depois')
                with self.assertRaisesRegex(RuntimeError, 'changed'):
                    capture_job.finish({})
            finally:
                capture_job.tar.close()

    def test_linked_modules_and_permissions_are_captured(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            runtime = directory / 'runtime/bmcu_core'
            runtime.mkdir(parents=True)
            (runtime / 'manager.py').write_text('conteudo em uso')
            (runtime / 'manager.py').chmod(0o750)
            extras = directory / 'klippy/extras'
            extras.mkdir(parents=True)
            (extras / 'bmcu_core').symlink_to(
                runtime, target_is_directory=True
            )
            file_path = directory / 'backup.tar.gz'
            capture_job = capture.BackupCapture(file_path)
            try:
                capture_job.directory(
                    directory / 'klippy',
                    'klippy-instalado',
                    follow_symlinks=True,
                )
                capture_job.finish(
                    {'home_original': '/home/pi', 'criado_utc': '2026-10-20'}
                )
            finally:
                capture_job.tar.close()
            manifest = verifier.verify(file_path, directory / 'revisao')
            name = 'klippy-instalado/extras/bmcu_core/manager.py'
            self.assertEqual(
                (directory / 'revisao' / name).read_text(), 'conteudo em uso'
            )
            self.assertEqual(manifest['arquivos'][name]['modo'], 0o750)

    def test_capture_rejects_printing_and_motion(self):
        for status, bmcu in [
            ('printing', {}),
            ('paused', {}),
            ('standby', {'active_operations': {'bmcu0': {}}}),
            ('standby', {'devices': [{'channels': [{'motion': 3}]}]}),
        ]:
            with patch.object(
                capture,
                'query_object',
                side_effect=lambda n: (
                    {'state': status} if n == 'print_stats' else bmcu
                ),
            ):
                with self.assertRaises(RuntimeError):
                    capture.check_idle()

    def test_backup_keeps_loaded_state(self):
        bmcu = {
            'devices': [{'channels': [{'motion': 0}], 'loaded_channels': [3]}]
        }
        with patch.object(
            capture,
            'query_object',
            side_effect=lambda n: (
                {'state': 'standby'} if n == 'print_stats' else bmcu
            ),
        ):
            self.assertEqual(capture.check_idle(), bmcu)


if __name__ == '__main__':
    unittest.main()
