import hashlib
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from test_integracao import ROOT, import_module


PATH = ROOT / 'scripts/ender3-v3-se/bmcu_runtime_lifecycle.py'
LIFECYCLE = (import_module('test_runtime_lifecycle_impl', PATH)
             if PATH.exists() else None)


class RuntimeLifecycleTests(unittest.TestCase):
    def bench(self, home):
        self.assertIsNotNone(LIFECYCLE, 'Missing native process adapter')
        bmcu = home / 'printer_data/config/bmcu'
        runtime = bmcu / 'runtime'
        scripts = runtime / 'scripts'
        scripts.mkdir(parents=True)
        extras = home / 'klipper/klippy/extras'
        extras.mkdir(parents=True)
        source = runtime / 'klippy/extras'
        (source / 'bmcu_core').mkdir(parents=True)
        (source / 'bmcu.py').write_text('# native manager entry\n')
        (source / 'bmcu_panel.py').write_text('class BMCUPanel: pass\n')
        for name in ('bmcu.py', 'bmcu_core', 'bmcu_panel.py'):
            (extras / name).symlink_to(source / name)
        metadata = dict(
            schema=1, product='BMCU-Klipper', version='1.1.2',
            config_dir=str(bmcu.parent), klipper_dir=str(home / 'klipper'),
            module_strategy='persistent-runtime-links',
            service=dict(backend='systemd', name='klipper'),
        )
        (runtime / 'INSTALLATION.json').write_text(json.dumps(metadata))
        (bmcu / 'active.json').write_text('[{"name": "bmcu0"}]')
        scripts_data = {
            'scripts/bmcu_transport_process.py': (
                'import json\nfrom pathlib import Path\n'
                'def processes(directory):\n'
                '    path = Path(directory)/"active.json"\n'
                '    return json.loads(path.read_text())\n'
                'def stop(directory):\n'
                '    (Path(directory)/"active.json").write_text("[]")\n'
            ),
            'scripts/bmcu_host_bootstrap.py': (
                'import json, sys\nfrom pathlib import Path\n'
                'assert sys.argv[1:3] == ["--sync-transports", "--metadata"]\n'
                'p = Path(sys.argv[3]).parent.parent/"active.json"\n'
                'p.write_text(json.dumps([dict(name="bmcu0")]))\n'
            ),
        }
        hashes = {}
        for name, data in scripts_data.items():
            (runtime / name).write_text(data)
            hashes[name] = hashlib.sha256(data.encode()).hexdigest()
        return bmcu, hashes

    def test_native_helpers_stop_and_start_without_a_transport_unit(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            bmcu, hashes = self.bench(home)
            with patch.object(LIFECYCLE, 'HELPER_HASHES', hashes):
                transport = LIFECYCLE.RuntimeTransport(home)
                transport.check_active()
                transport.stop()
                self.assertEqual([], json.loads(
                    (bmcu / 'active.json').read_text()))
                transport.stop()
                transport.start()
                self.assertEqual([dict(name='bmcu0')],
                                 json.loads((bmcu / 'active.json').read_text()))

    def test_foreign_helpers_or_metadata_fail_before_running_code(self):
        for invalid in ('hash', 'version', 'path', 'service', 'module'):
            with self.subTest(invalid=invalid), \
                    tempfile.TemporaryDirectory() as t:
                home = Path(t)
                bmcu, hashes = self.bench(home)
                runtime = bmcu / 'runtime'
                meta = runtime / 'INSTALLATION.json'
                data = json.loads(meta.read_text())
                if invalid == 'hash':
                    script = runtime / 'scripts/bmcu_host_bootstrap.py'
                    script.write_text('raise Exception()')
                elif invalid == 'version':
                    data['version'] = '1.0.6'
                elif invalid == 'path':
                    data['klipper_dir'] = str(home / 'another-klipper')
                elif invalid == 'service':
                    data['service']['name'] = 'unrelated'
                else:
                    path = home / 'klipper/klippy/extras/bmcu_panel.py'
                    path.unlink()
                    path.write_text('# not the native link')
                meta.write_text(json.dumps(data))
                with patch.object(LIFECYCLE, 'HELPER_HASHES', hashes):
                    with self.assertRaisesRegex(ValueError, 'BMCU'):
                        LIFECYCLE.RuntimeTransport(home)

    def test_missing_active_transport_blocks_preflight(self):
        with tempfile.TemporaryDirectory() as temporary:
            home = Path(temporary)
            bmcu, hashes = self.bench(home)
            (bmcu / 'active.json').write_text('[]')
            with patch.object(LIFECYCLE, 'HELPER_HASHES', hashes):
                with self.assertRaises(Exception):
                    LIFECYCLE.RuntimeTransport(home).check_active()


if __name__ == '__main__':
    unittest.main()
