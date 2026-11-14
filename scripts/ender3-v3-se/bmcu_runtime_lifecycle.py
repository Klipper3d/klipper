"""Delegate transport lifecycle to pinned BMCU 1.1.2 process helpers.

No UART access, PID guessing, module removal or route reconciliation here.
Call stop only after Klipper has exited; call start before starting Klipper.
"""
import hashlib
import json
from pathlib import Path
import subprocess
import sys


# LF-normalized upstream files, v1.1.2:
# a1215523c51551cf11cf4c4aca39aa56b6565ea8.
HELPER_HASHES = {
    'scripts/bmcu_host_bootstrap.py':
        'fd07e52048f27511e8d98f0636a160b4f29c5d17277b0384e61e598f2028c102',
    'scripts/bmcu_transport_process.py':
        'ae4a15aa3fba294b892d405ce0354c747d7f9e5f3e39f27f302143be88fdbb0f',
    'scripts/bmcu_planner_process.py':
        'e0fb6d1c6756f3d8c3647cb347a995b2e56a9371640d4d7c6702ee777d209a63',
    'klippy/extras/bmcu_core/release.py':
        'd15745c0e40127a4377d4790221d8ed0e27f6b4fa548278b03ca51ac47ea79a9',
    'klippy/extras/bmcu_core/__init__.py':
        'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855',
}


class RuntimeTransport:
    def __init__(self, home):
        self.home = Path(home).resolve()
        self.directory = self.home / 'printer_data/config/bmcu'
        self.runtime = self.directory / 'runtime'
        self.metadata = self.runtime / 'INSTALLATION.json'
        self.validate()

    def validate(self):
        if (self.directory.is_symlink() or self.runtime.is_symlink()
                or self.metadata.is_symlink() or not self.metadata.is_file()):
            raise ValueError('BMCU 1.1.2 managed runtime metadata is required.')
        data = json.loads(self.metadata.read_text())
        service = data.get('service', {})
        if (data.get('schema') != 1 or data.get('product') != 'BMCU-Klipper'
                or data.get('version') != '1.1.2'
                or data.get('module_strategy') != 'persistent-runtime-links'
                or Path(data.get('config_dir', '')).resolve()
                != self.directory.parent
                or Path(data.get('klipper_dir', '')).resolve()
                != self.home / 'klipper'
                or service.get('backend') != 'systemd'
                or service.get('name') not in ('klipper', 'klipper.service')):
            raise ValueError('Unsupported BMCU runtime metadata or service.')
        extras = self.home / 'klipper/klippy/extras'
        for name in ('bmcu.py', 'bmcu_core', 'bmcu_panel.py'):
            link = extras / name
            expected = self.runtime / 'klippy/extras' / name
            if (not link.is_symlink() or not expected.exists()
                    or expected.is_symlink() or link.resolve() != expected):
                raise ValueError('BMCU managed module link changed: ' + name)
        for name, digest in HELPER_HASHES.items():
            path = self.runtime / name
            if (path.is_symlink() or not path.is_file()
                    or not path.resolve().is_relative_to(self.runtime)
                    or hashlib.sha256(path.read_bytes().replace(b'\r\n', b'\n'))
                    .hexdigest() != digest):
                raise ValueError('BMCU 1.1.2 process helper changed: ' + name)

    def _process_action(self, action):
        self.validate()
        script = '''\
import sys
sys.path.insert(0, sys.argv[1])
import bmcu_transport_process as transport
directory, action = sys.argv[2:]
if action == 'stop':
    transport.stop(directory)
records = transport.processes(directory)
if action == 'stop' and records:
    raise RuntimeError('BMCU transports are still running')
if action == 'check' and [r['name'] for r in records] != ['bmcu0']:
    raise RuntimeError('One verified BMCU transport named bmcu0 is required')
'''
        subprocess.run([
            sys.executable, '-B', '-I', '-S', '-c', script,
            str(self.runtime / 'scripts'), str(self.directory), action,
        ], check=True, timeout=30)

    def check_active(self):
        self._process_action('check')

    def stop(self):
        self._process_action('stop')

    def start(self):
        self.validate()
        subprocess.run([
            sys.executable, '-B', '-I', '-S',
            str(self.runtime / 'scripts/bmcu_host_bootstrap.py'),
            '--sync-transports', '--metadata', str(self.metadata),
        ], check=True, timeout=60)
        self.check_active()
