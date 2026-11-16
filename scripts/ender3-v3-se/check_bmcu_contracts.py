"""Check pinned upstream sources and process ownership without a printer."""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from bmcu_runtime_lifecycle import HELPER_HASHES, RuntimeTransport
import instalar_multicor_bmcu as installer

ROOT = Path(__file__).resolve().parents[2]
BMCU_COMMIT = 'a1215523c51551cf11cf4c4aca39aa56b6565ea8'


def methods(source):
    return {(cls.name, node.name): ast.dump(node)
            for cls in ast.parse(source).body if isinstance(cls, ast.ClassDef)
            for node in cls.body if isinstance(node, ast.FunctionDef)}


def check_refill_contract(refill, manager):
    native = {key: body for text, owner in (
        (refill, 'AutoRefillController'), (manager, 'BMCUManager'),
    ) for key, body in methods(text).items() if key[0] == owner}
    fixture = ROOT / 'test/bmcu_integracao/fixtures/refill_112_original.txt'
    for key, expected in methods(fixture.read_text()).items():
        if native.get(key) != expected:
            raise ValueError('BMCU native refill method changed: %s.%s' % key)


def toolchange_boundaries(source):
    method = next(node for cls in ast.parse(source).body
                  if isinstance(cls, ast.ClassDef)
                  and cls.name == 'BMCUManager'
                  for node in cls.body if isinstance(node, ast.FunctionDef)
                  and node.name == 'load_channel')

    def assigns(node, name):
        return (isinstance(node, ast.Assign) and len(node.targets) == 1
                and isinstance(node.targets[0], ast.Name)
                and node.targets[0].id == name)

    start = next(i for i, node in enumerate(method.body)
                 if assigns(node, 'multi_device_switch'))
    stop = next(i for i in range(start + 1, len(method.body))
                if assigns(method.body[i], 'phase'))
    unlock = next(node for node in ast.walk(method)
                  if isinstance(node, ast.If) and node.body
                  and isinstance(node.body[0], ast.Expr)
                  and isinstance(node.body[0].value, ast.Call)
                  and isinstance(node.body[0].value.func, ast.Attribute)
                  and node.body[0].value.func.attr == '_unlock_refill')
    return method.body[start:stop], unlock


def check_toolchange_contract(manager):
    fixture_dir = ROOT / 'test/bmcu_integracao/fixtures'
    fixture = (fixture_dir / 'operation_lock_112_original.txt').read_text()
    native = methods(manager)
    for key, expected in methods(fixture).items():
        if native.get(key) != expected:
            raise ValueError(
                'BMCU native lock/phase method changed: %s.%s' % key)
    fixture = (fixture_dir / 'manager_original.txt').read_text()
    expected_lock, expected_unlock = toolchange_boundaries(fixture)
    lock, unlock = toolchange_boundaries(manager)
    if ([ast.dump(node) for node in lock] !=
            [ast.dump(node) for node in expected_lock]
            or ast.dump(unlock) != ast.dump(expected_unlock)):
        raise ValueError('BMCU native tool-change lock boundary changed')


def check_process_stop(source):
    # Only a sleeping synthetic process is started. No serial library is
    # imported and the native bootstrap START command is never executed.
    with tempfile.TemporaryDirectory() as temporary:
        home = Path(temporary)
        directory = home / 'printer_data/config/bmcu'
        runtime = directory / 'runtime'
        extras = home / 'klipper/klippy/extras'
        extras.mkdir(parents=True)
        for name in (*HELPER_HASHES, 'klippy/extras/bmcu.py',
                     'klippy/extras/bmcu_panel.py'):
            target = runtime / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source / name, target)
        for name in ('bmcu.py', 'bmcu_core', 'bmcu_panel.py'):
            (extras / name).symlink_to(runtime / 'klippy/extras' / name)
        metadata = dict(
            schema=1, product='BMCU-Klipper', version='1.1.2',
            config_dir=str(directory.parent), klipper_dir=str(home / 'klipper'),
            module_strategy='persistent-runtime-links',
            service=dict(backend='systemd', name='klipper'),
        )
        (runtime / 'INSTALLATION.json').write_text(json.dumps(metadata))
        sockets = directory / 'sockets'
        sockets.mkdir()
        (directory / 'bmcu.cfg').write_text(
            '[bmcu]\ntransport_socket_dir: %s\n' % sockets)
        daemon = runtime / 'scripts/bmcu_transportd.py'
        daemon.write_text('import time\ntime.sleep(60)\n')
        socket_path = sockets / 'bmcu0.sock'
        status_file = sockets / 'bmcu0.status.json'
        command = [sys.executable, '-B', str(daemon), '--name', 'bmcu0',
                   '--port', '/dev/null', '--socket', str(socket_path),
                   '--status-file', str(status_file)]
        sleeper = subprocess.Popen(command)
        try:
            record = dict(pid=sleeper.pid, daemon=str(daemon), name='bmcu0',
                          port='/dev/null', socket=str(socket_path),
                          status_file=str(status_file))
            (directory / 'transport-processes.json').write_text(
                json.dumps({'devices': {'bmcu0': record}}))
            adapter = RuntimeTransport(home)
            adapter.check_active()
            adapter.stop()
            sleeper.wait(timeout=5)
            adapter.stop()
        finally:
            if sleeper.poll() is None:
                sleeper.terminate()
                sleeper.wait(timeout=5)


def check_sources(source):
    source = Path(source).resolve()
    git = os.environ.get('GIT', 'git')
    commit = subprocess.check_output(
        [git, 'rev-parse', 'HEAD'], cwd=source).decode().strip()
    if commit != BMCU_COMMIT:
        raise ValueError('Select the pinned BMCU 1.1.2 commit.')
    dirty = subprocess.check_output(
        [git, 'diff', '--name-only', 'HEAD'], cwd=source)
    if dirty:
        raise ValueError('Use unchanged upstream BMCU sources.')
    for name, expected in HELPER_HASHES.items():
        actual = hashlib.sha256((source / name).read_bytes().replace(
            b'\r\n', b'\n')).hexdigest()
        if actual != expected:
            raise ValueError('Pinned BMCU lifecycle helper changed: ' + name)
    core = source / 'klippy/extras/bmcu_core'
    manager = (core / 'manager.py').read_text()
    check_refill_contract((core / 'refill.py').read_text(), manager)
    check_toolchange_contract(manager)
    cases = (
        (manager, lambda text: installer.patch_pre_unload(
            installer.patch_toolchange_lock(
                installer.patch_pre_capture(text)))),
        ((core / 'endpoints.py').read_text(), installer.patch_contract),
        ((source / 'scripts/bmcu_transportd.py').read_text(),
         installer.patch_transport),
    )
    for text, transform in cases:
        result = transform(text)
        compile(result, '<patched BMCU 1.1.2>', 'exec')
        if transform(result) != result:
            raise ValueError('BMCU callback/transport patch is not idempotent.')
    check_process_stop(source)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    args = parser.parse_args()
    check_sources(args.source)
    print('Pinned BMCU 1.1.2: refill, tool-change locks, patches and process '
          'stop verified. '
          'No printer or serial device was accessed.')


if __name__ == '__main__':
    main()
