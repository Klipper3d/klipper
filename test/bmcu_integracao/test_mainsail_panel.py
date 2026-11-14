import hashlib
import json
from pathlib import Path
import shutil
import tempfile
import unittest
from unittest.mock import patch
import zipfile

from test_integracao import ROOT, import_module


BUILD_PATH = ROOT / 'scripts/ender3-v3-se/build_mainsail_panel.py'
BUILD = (import_module('panel_build', BUILD_PATH)
         if BUILD_PATH.exists() else None)
INSTALL_PATH = ROOT / 'scripts/ender3-v3-se/install_mainsail_panel.py'
INSTALL = (import_module('panel_install', INSTALL_PATH)
           if INSTALL_PATH.exists() else None)


class MainsailOverlayTests(unittest.TestCase):
    def test_release_pipeline_requires_and_verifies_the_native_panel(self):
        workflow = (ROOT / '.github/workflows/ender3-v3-se.yml').read_text()
        self.assertIn('  mainsail-panel:', workflow)
        self.assertIn('needs: [display-and-firmware, mainsail-panel]', workflow)
        self.assertIn('check_bmcu_contracts.py', workflow)
        self.assertIn('sha256sum --check PANEL-SHA256SUMS', workflow)
        self.assertIn('release-artifacts/mainsail-bmcu.zip', workflow)

    def test_provenance_covers_translations_and_the_build_recipe(self):
        self.assertTrue(hasattr(BUILD, 'build_provenance'))
        before = BUILD.build_provenance('2.0.1')
        with tempfile.TemporaryDirectory() as temporary:
            overlay = Path(temporary) / 'overlay'
            shutil.copytree(BUILD.OVERLAY, overlay)
            with patch.object(BUILD, 'OVERLAY', overlay):
                self.assertEqual(before, BUILD.build_provenance('2.0.1'))
                with (overlay / 'locales/pt.json').open('a') as stream:
                    stream.write('\n')
                after = BUILD.build_provenance('2.0.1')
                self.assertNotEqual(before['overlay_sha256'],
                                    after['overlay_sha256'])
        self.assertEqual(64, len(before['recipe_sha256']))

    def source(self, root):
        files = {
            'package.json': json.dumps({'version': '2.19.0'}),
            'src/pages/Dashboard.vue': (
                "import AfcPanel from '@/components/panels/AfcPanel.vue'\n"
                '    components: {\n        AfcPanel,\n    },\n'
            ),
            'src/store/variables.ts': "const panels = [\n    'afc',\n]\n",
            'src/store/gui/getters.ts': (
                '        // remove afc panel, '
                'if no AFC module exists in Klipper\n'
            ),
            'src/components/mixins/dashboard.ts': (
                "            case 'mmu':\n                return mdiMulticast\n"
            ),
            'src/locales/en.json': '{"Panels": {}}',
            'src/locales/pt.json': '{"Panels": {}}',
        }
        for name, data in files.items():
            path = root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(data)
        return files

    def test_overlay_registers_real_card_and_translations(self):
        self.assertIsNotNone(BUILD, 'Missing reproducible Mainsail build')
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.source(root)
            BUILD.apply_overlay(root, '2.0.1')
            panel_path = root / 'src/components/panels/BmcuPanel.vue'
            self.assertTrue(panel_path.is_file())
            self.assertIn('BmcuPanel',
                          (root / 'src/pages/Dashboard.vue').read_text())
            self.assertIn("'bmcu'",
                          (root / 'src/store/variables.ts').read_text())
            self.assertIn('bmcu_ender_integration',
                          (root / 'src/store/gui/getters.ts').read_text())
            for language in ('en', 'pt'):
                path = root / ('src/locales/' + language + '.json')
                data = json.loads(path.read_text())
                self.assertIn('Refill', data['Panels']['BmcuPanel'])
            data = json.loads((root / 'public/bmcu-build.json').read_text())
            self.assertEqual('2.0.1', data['release'])
            self.assertEqual('2.19.0', data['mainsail_version'])

    def test_incompatible_source_fails_without_partial_changes(self):
        self.assertIsNotNone(BUILD, 'Missing reproducible Mainsail build')
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.source(root)
            target = root / 'src/store/variables.ts'
            target.write_text('different upstream source\n')
            before = {p.relative_to(root): p.read_bytes()
                      for p in root.rglob('*') if p.is_file()}
            with self.assertRaises(ValueError):
                BUILD.apply_overlay(root, '2.0.1')
            after = {p.relative_to(root): p.read_bytes()
                     for p in root.rglob('*') if p.is_file()}
            self.assertEqual(before, after)

    def test_archive_is_deterministic_and_contains_verified_manifest(self):
        self.assertIsNotNone(BUILD, 'Missing reproducible Mainsail build')
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            dist = root / 'dist'
            dist.mkdir()
            (dist / 'index.html').write_text('<title>Mainsail</title>')
            (dist / 'app.js').write_text('const card = "BMCU";\n')
            (dist / 'bmcu-build.json').write_text(json.dumps(
                BUILD.build_provenance('2.0.1')))
            for name in ('Mainsail-LICENSE', 'Ender-Integration-LICENSE'):
                (dist / name).write_bytes((ROOT / 'COPYING').read_bytes())
            first = BUILD.package_panel(dist, root / 'first.zip')
            second = BUILD.package_panel(dist, root / 'second.zip')
            self.assertEqual(first, second)
            actual = hashlib.sha256(
                (root / 'first.zip').read_bytes()).hexdigest()
            self.assertEqual(actual, first)


class MainsailInstallTests(unittest.TestCase):
    def bench(self, root):
        self.assertIsNotNone(INSTALL, 'Missing controlled panel installer')
        active = root / 'mainsail'
        active.mkdir()
        (active / 'index.html').write_text('<title>Mainsail</title>old')
        (active / 'config.json').write_text('{"printer": "preserved"}')
        (active / '.theme').mkdir()
        (active / '.theme/custom.css').write_text('/* preserved theme */')
        dist = root / 'dist'
        dist.mkdir()
        (dist / 'index.html').write_text('<title>Mainsail</title>new')
        (dist / 'app.js').write_text('const panel = "BMCU";')
        (dist / 'bmcu-build.json').write_text(json.dumps(
            BUILD.build_provenance('2.0.1')))
        for name in ('Mainsail-LICENSE', 'Ender-Integration-LICENSE'):
            (dist / name).write_bytes((ROOT / 'COPYING').read_bytes())
        archive = root / 'panel.zip'
        digest = BUILD.package_panel(dist, archive)
        return active, archive, digest

    def test_packager_rejects_stale_metadata_symlinks_and_nested_output(self):
        for failure in ('stale', 'symlink', 'nested'):
            with self.subTest(failure=failure), \
                    tempfile.TemporaryDirectory() as t:
                root = Path(t)
                self.bench(root)
                dist = root / 'dist'
                output = root / 'new.zip'
                if failure == 'stale':
                    manifest_path = dist / 'bmcu-build.json'
                    manifest = json.loads(manifest_path.read_text())
                    manifest['recipe_sha256'] = '0' * 64
                    (dist / 'bmcu-build.json').write_text(json.dumps(manifest))
                elif failure == 'symlink':
                    (dist / 'private-link').symlink_to(root / 'panel.zip')
                else:
                    output = dist / 'nested.zip'
                with self.assertRaises(ValueError):
                    BUILD.package_panel(dist, output)

    def test_install_and_rollback_preserve_config_theme_and_old_tree(self):
        with tempfile.TemporaryDirectory() as temporary:
            active, archive, digest = self.bench(Path(temporary))
            with patch.object(INSTALL, 'check_idle'):
                record = INSTALL.install(archive, active, digest)
                self.assertIn('new', (active / 'index.html').read_text())
                self.assertIn('preserved', (active / 'config.json').read_text())
                self.assertTrue((active / '.theme/custom.css').is_file())
                INSTALL.rollback(record)
            self.assertIn('old', (active / 'index.html').read_text())
            self.assertIn('preserved', (active / 'config.json').read_text())

    def test_modified_user_files_block_rollback_without_discarding_work(self):
        with tempfile.TemporaryDirectory() as temporary:
            active, archive, digest = self.bench(Path(temporary))
            with patch.object(INSTALL, 'check_idle'):
                record = INSTALL.install(archive, active, digest)
                (active / '.theme/custom.css').write_text('new user work')
                with self.assertRaisesRegex(ValueError, 'changed'):
                    INSTALL.rollback(record)
            self.assertEqual('new user work',
                             (active / '.theme/custom.css').read_text())

    def test_new_empty_user_directory_also_blocks_rollback(self):
        with tempfile.TemporaryDirectory() as temporary:
            active, archive, digest = self.bench(Path(temporary))
            with patch.object(INSTALL, 'check_idle'):
                record = INSTALL.install(archive, active, digest)
                (active / 'new-user-directory').mkdir()
                with self.assertRaisesRegex(ValueError, 'changed'):
                    INSTALL.rollback(record)

    def test_forged_provenance_alias_paths_and_missing_licenses_fail(self):
        for failure in ('overlay', 'recipe', 'alias', 'license', 'special'):
            with self.subTest(failure=failure), \
                    tempfile.TemporaryDirectory() as t:
                active, archive, _ = self.bench(Path(t))
                with zipfile.ZipFile(archive) as zipped:
                    files = {name: zipped.read(name)
                             for name in zipped.namelist()}
                manifest = json.loads(files['bmcu-build.json'])
                if failure in ('overlay', 'recipe'):
                    manifest[failure + '_sha256'] = '0' * 64
                elif failure == 'alias':
                    files['assets//alias.js'] = b'alias'
                elif failure == 'license':
                    del files['Mainsail-LICENSE']
                manifest['files'] = {
                    name: hashlib.sha256(data).hexdigest()
                    for name, data in files.items() if name != 'bmcu-build.json'
                }
                files['bmcu-build.json'] = json.dumps(manifest).encode()
                with zipfile.ZipFile(archive, 'w') as zipped:
                    for name, data in files.items():
                        info = zipfile.ZipInfo(name)
                        if failure == 'special' and name == 'app.js':
                            info.external_attr = 0o010644 << 16
                        zipped.writestr(info, data)
                digest = hashlib.sha256(archive.read_bytes()).hexdigest()
                with patch.object(INSTALL, 'check_idle'):
                    with self.assertRaises(ValueError):
                        INSTALL.install(archive, active, digest)
                self.assertIn('old', (active / 'index.html').read_text())

    def test_bad_checksum_busy_printer_and_unsafe_zip_never_activate(self):
        for failure in ('checksum', 'busy', 'zip'):
            with self.subTest(failure=failure):
                with tempfile.TemporaryDirectory() as temporary:
                    root = Path(temporary)
                    active, archive, digest = self.bench(root)
                    before = (active / 'index.html').read_bytes()
                    if failure == 'checksum':
                        digest = '0' * 64
                    if failure == 'zip':
                        with zipfile.ZipFile(archive, 'a') as z:
                            z.writestr('../escape.txt', 'escape')
                        digest = hashlib.sha256(
                            archive.read_bytes()).hexdigest()
                    error = (RuntimeError('printing')
                             if failure == 'busy' else None)
                    with patch.object(INSTALL, 'check_idle', side_effect=error):
                        with self.assertRaises((ValueError, RuntimeError)):
                            INSTALL.install(archive, active, digest)
                    self.assertEqual(before,
                                     (active / 'index.html').read_bytes())
                    self.assertFalse((root / 'escape.txt').exists())

    def test_failed_activation_restores_previous_panel_and_keeps_evidence(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            active, archive, digest = self.bench(root)
            rename = Path.rename

            def fail_staged(path, target):
                if path.name == 'staged':
                    raise OSError('activation failed')
                return rename(path, target)

            with patch.object(INSTALL, 'check_idle'):
                with patch.object(Path, 'rename', fail_staged):
                    with self.assertRaisesRegex(OSError, 'activation failed'):
                        INSTALL.install(archive, active, digest)
            self.assertIn('old', (active / 'index.html').read_text())
            self.assertTrue(list(root.glob('mainsail-panel-backup-*')))


if __name__ == '__main__':
    unittest.main()
