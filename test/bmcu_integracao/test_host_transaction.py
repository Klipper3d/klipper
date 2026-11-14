import importlib.util
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from test_integracao import ROOT


MODULE = ROOT / 'scripts/ender3-v3-se/host_transaction.py'


class HostTransactionTests(unittest.TestCase):
    def module(self):
        self.assertTrue(MODULE.is_file(),
                        'Full host rollback is not implemented')
        spec = importlib.util.spec_from_file_location(
            'host_transaction', MODULE)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module

    def git(self, directory, *arguments):
        return subprocess.check_output(
            ['git', '-C', str(directory), *arguments], stderr=subprocess.PIPE,
            text=True,
        ).strip()

    def commit(self, directory):
        self.git(directory, 'add', '.')
        self.git(directory, '-c', 'user.name=Synthetic Test',
                 '-c', 'user.email=test@example.invalid',
                 'commit', '-qm', 'test')
        return self.git(directory, 'rev-parse', 'HEAD')

    def bench(self, root):
        active, source, work = (
            root / 'klipper', root / 'source', root / 'backup')
        active.mkdir()
        self.git(active, 'init', '-q')
        display = Path('klippy/extras/e3v3se_display/__init__.py')
        (active / display).parent.mkdir(parents=True)
        (active / display).write_text('VERSION = 1\n')
        (active / 'src').mkdir()
        (active / 'src/command.c').write_text('/* unchanged MCU protocol */\n')
        before = self.commit(active)
        self.git(root, 'clone', '-q', str(active), str(source))
        (source / display).write_text('VERSION = 2\n')
        after = self.commit(source)
        addon = active / 'klippy/extras/bmcu_core/manager.py'
        addon.parent.mkdir()
        addon.write_text('NATIVE_CALIBRATION = 31.75\n')
        (active / 'personal-note.txt').write_text('preserve my work\n')
        return active, source, work, display, before, after

    def test_full_rollback_preserves_checkout_and_native_modules(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, display, before, after = self.bench(
                Path(temporary))
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            self.assertEqual('VERSION = 1\n', (active / display).read_text())
            transaction.activate()
            self.assertEqual(after, self.git(active, 'rev-parse', 'HEAD'))
            self.assertEqual('VERSION = 2\n', (active / display).read_text())
            addon = active / 'klippy/extras/bmcu_core/manager.py'
            self.assertIn('31.75', addon.read_text())
            record = transaction.record()
            self.assertEqual(before, record['previous_commit'])
            module.restore_checkout(record)
            self.assertEqual(before, self.git(active, 'rev-parse', 'HEAD'))
            self.assertEqual('VERSION = 1\n', (active / display).read_text())
            self.assertEqual('preserve my work\n',
                             (active / 'personal-note.txt').read_text())
            self.assertTrue(Path(record['failed']).is_dir())
            module.restore_checkout(record)
            self.assertEqual(before, self.git(active, 'rev-parse', 'HEAD'))

    def test_dirty_host_or_target_is_rejected_without_discarding_work(self):
        module = self.module()
        for dirty_target in (False, True):
            with self.subTest(target=dirty_target), \
                    tempfile.TemporaryDirectory() as tmp:
                active, source, work, display, _, _ = self.bench(Path(tmp))
                dirty = source if dirty_target else active
                (dirty / display).write_text('USER_WORK = 1\n')
                with self.assertRaisesRegex(ValueError, 'local changes'):
                    module.HostTransaction(source, active, work).prepare()
                self.assertEqual('USER_WORK = 1\n',
                                 (dirty / display).read_text())

    def test_mcu_source_changes_require_a_separate_firmware_update(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, display, _, _ = self.bench(Path(temporary))
            (source / 'src/command.c').write_text('/* different protocol */\n')
            self.commit(source)
            with self.assertRaisesRegex(ValueError, 'firmware'):
                module.HostTransaction(source, active, work).prepare()
            self.assertEqual('VERSION = 1\n', (active / display).read_text())

    def test_failed_second_rename_restores_the_original_active_path(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, display, before, _ = self.bench(
                Path(temporary))
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            rename = Path.rename

            def fail_staging(path, destination):
                if path == transaction.staged:
                    raise OSError('simulated activation failure')
                return rename(path, destination)

            with patch.object(Path, 'rename', fail_staging):
                with self.assertRaisesRegex(OSError, 'activation failure'):
                    transaction.activate()
            self.assertEqual(before, self.git(active, 'rev-parse', 'HEAD'))
            self.assertEqual('VERSION = 1\n', (active / display).read_text())

    def test_changed_native_module_after_staging_is_not_silently_lost(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, _, before, _ = self.bench(Path(temporary))
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            addon = active / 'klippy/extras/bmcu_core/manager.py'
            addon.write_text('USER_CALIBRATION = 42\n')
            with self.assertRaisesRegex(ValueError, 'changed.*staging'):
                transaction.activate()
            self.assertEqual(before, self.git(active, 'rev-parse', 'HEAD'))
            self.assertIn('42', addon.read_text())

    def test_staged_code_tampering_is_rejected_before_replacing_the_host(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, display, before, _ = self.bench(
                Path(temporary))
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            (transaction.staged / display).write_text('UNREVIEWED = True\n')
            with self.assertRaisesRegex(ValueError, 'local changes'):
                transaction.activate()
            self.assertEqual(before, self.git(active, 'rev-parse', 'HEAD'))

    @unittest.skipUnless(os.geteuid() == 0, 'Ownership test requires root')
    def test_sudo_staging_preserves_the_active_host_owner(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, display, _, _ = self.bench(Path(temporary))
            os.chown(active, 1000, 1000)
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            self.assertEqual(1000, (transaction.staged / display).stat().st_uid)
            self.assertEqual(1000, (transaction.staged / '.git').stat().st_uid)
            self.assertEqual(1000, work.stat().st_uid)

    def test_build_makefile_changes_are_not_treated_as_host_only(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, _, _, _ = self.bench(Path(temporary))
            (source / 'Makefile').write_text('MCU_PROTOCOL = changed\n')
            self.commit(source)
            with self.assertRaisesRegex(ValueError, 'firmware'):
                module.HostTransaction(source, active, work).prepare()

    def test_recovery_continues_after_a_crash_between_rollback_renames(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, display, before, _ = self.bench(
                Path(temporary))
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            transaction.activate()
            active.rename(transaction.failed)
            module.restore_checkout(transaction.record())
            self.assertEqual(before, self.git(active, 'rev-parse', 'HEAD'))
            self.assertEqual('VERSION = 1\n', (active / display).read_text())
            self.assertTrue(transaction.failed.is_dir())

    def test_preserved_user_files_and_remotes_are_recorded(self):
        module = self.module()
        with tempfile.TemporaryDirectory() as temporary:
            active, source, work, _, _, _ = self.bench(Path(temporary))
            self.git(active, 'remote', 'add', 'upstream',
                     'https://example.invalid/upstream.git')
            transaction = module.HostTransaction(source, active, work)
            transaction.prepare()
            self.assertIn('personal-note.txt', transaction.record().get(
                'other_untracked_preserved', []))
            self.assertEqual('https://example.invalid/upstream.git',
                             self.git(transaction.staged, 'remote',
                                      'get-url', 'upstream'))

    def test_update_lock_rejects_a_second_installer_until_released(self):
        module = self.module()
        self.assertTrue(hasattr(module, 'update_lock'))
        with tempfile.TemporaryDirectory() as temporary:
            lock = Path(temporary) / 'update.lock'
            with module.update_lock(lock):
                with self.assertRaisesRegex(ValueError, 'another update'):
                    with module.update_lock(lock):
                        self.fail('The second installer acquired the lock')
            with module.update_lock(lock):
                self.assertTrue(lock.is_file())

    def test_update_lock_does_not_follow_a_symlink(self):
        module = self.module()
        self.assertTrue(hasattr(module, 'update_lock'))
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            target = root / 'user-file'
            target.write_text('preserve my contents')
            lock = root / 'update.lock'
            lock.symlink_to(target)
            with self.assertRaises(OSError):
                with module.update_lock(lock):
                    self.fail('Symlink lock was accepted')
            self.assertEqual('preserve my contents', target.read_text())


if __name__ == '__main__':
    unittest.main()
