import argparse
import hashlib
import json
import os
import pwd
import re
import secrets
import shutil
import subprocess
from datetime import datetime
from pathlib import Path, PurePosixPath

import instalar_multicor_bmcu as installer

ROOT = Path(__file__).resolve().parents[2]
TOKEN_PLACEHOLDER = '__GERAR_TOKEN_LOCAL__'


def verify(snapshot):
    records = json.loads((snapshot / 'integridade.json').read_text())
    present_files = {
        p.relative_to(snapshot).as_posix()
        for p in snapshot.rglob('*')
        if p.is_file()
    }
    if present_files != set(records) | {'integridade.json'}:
        raise ValueError('The snapshot contains missing or extra files.')
    for name, expected in records.items():
        relative = PurePosixPath(name)
        if relative.is_absolute() or '..' in relative.parts:
            raise ValueError('Invalid snapshot path: ' + name)
        path = snapshot / name
        if not path.resolve().is_relative_to(snapshot.resolve()):
            raise ValueError('Symlink outside the snapshot: ' + name)
        if hashlib.sha256(path.read_bytes()).hexdigest() != expected:
            raise ValueError('Snapshot changed: ' + name)
    return json.loads((snapshot / 'metadados.json').read_text())


def panel_token(configurations):
    path = configurations / 'bmcu/bmcu_panel.cfg'
    if path.is_file():
        result = re.search(
            r'(?m)^access_token\s*:\s*(\S+)\s*$', path.read_text()
        )
        if result and result[1] != TOKEN_PLACEHOLDER:
            if len(result[1]) >= 32:
                return result[1]
    return secrets.token_hex(32)


def prepare(home, snapshot):
    verify(snapshot)
    configurations = home / 'printer_data/config'
    extras = home / 'klipper/klippy/extras'
    runtime = configurations / 'bmcu/runtime'
    version = runtime / 'version'
    if not version.is_file() or 'package=1.0.6' not in version.read_text():
        raise RuntimeError('Install BMCU-Klipper 1.0.6 before restoring.')
    contracts = extras / 'bmcu_core/endpoints.py'
    manager = extras / 'bmcu_core/manager.py'
    if not contracts.is_file() or not manager.is_file():
        raise RuntimeError('Native BMCU modules were not found.')
    changes = {}
    token = panel_token(configurations)
    for source_path in sorted((snapshot / 'config').rglob('*')):
        if not source_path.is_file():
            continue
        relative = source_path.relative_to(snapshot / 'config')
        data = source_path.read_bytes()
        if relative.as_posix() == 'bmcu/bmcu_panel.cfg':
            data = data.replace(TOKEN_PLACEHOLDER.encode(), token.encode())
        changes[configurations / relative] = data
    for source_path in sorted((snapshot / 'ambiente-servicos').glob('*.env')):
        destination = home / 'printer_data/systemd' / source_path.name
        changes[destination] = source_path.read_bytes()
    installation = runtime / 'INSTALLATION.json'
    if installation.is_file():
        installation_data = json.loads(installation.read_text())
        installation_data['panel_token_sha256'] = hashlib.sha256(
            token.encode()
        ).hexdigest()
        changes[installation] = (
            json.dumps(installation_data, indent=2) + '\n'
        ).encode()
    changes[extras / 'bmcu_integracao_ender.py'] = (
        ROOT / 'integrations/bmcu/bmcu_integracao_ender.py'
    ).read_bytes()
    changes[extras / 'bmcu_medicao.py'] = (
        ROOT / 'integrations/bmcu/bmcu_medicao.py'
    ).read_bytes()
    changes[contracts.resolve()] = installer.patch_contract(
        contracts.read_text()
    ).encode()
    changes[manager.resolve()] = installer.patch_pre_capture(
        manager.read_text()
    ).encode()
    if any(not p.resolve().is_relative_to(home.resolve()) for p in changes):
        raise RuntimeError(
            'Some destination paths are outside the user home directory.'
        )
    return changes


def check_application(home, metadata):
    git = [
        'git',
        '-c',
        'safe.directory=' + str(home / 'klipper'),
        '-C',
        str(home / 'klipper'),
        'rev-parse',
        'HEAD',
    ]
    current = subprocess.check_output(git, text=True).strip()
    if current != metadata['klipper_commit']:
        raise RuntimeError(
            'Use the Klipper checkout specified in metadados.json.'
        )
    active = (
        subprocess.run(
            ['systemctl', 'is-active', '--quiet', 'klipper']
        ).returncode
        == 0
    )
    if active:
        status = installer.query_object('print_stats')
        if status['state'] in ('printing', 'paused'):
            raise RuntimeError('Finish printing before restoring.')
        bmcu = installer.query_object('bmcu')
        if bmcu['active_operations'] or bmcu['print_plan_open']:
            raise RuntimeError('Wait for BMCU operations to finish.')
        for device in bmcu.get('devices', []):
            if any(
                c.get('motion') or c.get('motor_pwm')
                for c in device.get('channels', [])
            ):
                raise RuntimeError('Stop filament feeding before restoring.')
    return active


def apply(home, changes, user):
    backup = home / (
        'backup-antes-restauracao-'
        + datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    )
    backup.mkdir(mode=0o700)
    if os.geteuid() == 0:
        os.chown(backup, user.pw_uid, user.pw_gid)
    records = []
    for index, path in enumerate(changes):
        exists = path.is_file()
        copy_path = backup / str(index)
        if exists:
            shutil.copy2(path, copy_path)
            copy_path.chmod(0o600)
        records.append(
            {
                'destino': str(path),
                'copia': str(copy_path) if exists else None,
                'vinculo': (os.readlink(path) if path.is_symlink() else None),
                'modo': path.stat().st_mode & 0o777 if exists else None,
                'uid': path.stat().st_uid if exists else None,
                'gid': path.stat().st_gid if exists else None,
            }
        )
    manifest = backup / 'arquivos.json'
    manifest.write_text(json.dumps(records, indent=2))
    manifest.chmod(0o600)
    installer.service('stop')
    try:
        for path, data in changes.items():
            private = path.name == 'bmcu_panel.cfg' or path.suffix == '.env'
            mode = 0o600 if private else 0o644
            installer.write_file(path, data, user, mode)
    except Exception:
        for item in records:
            path = Path(item['destino'])
            if item['vinculo'] is not None:
                path.unlink(missing_ok=True)
                path.symlink_to(item['vinculo'])
            elif item['copia'] is None:
                path.unlink(missing_ok=True)
            else:
                installer.write_file(
                    path, Path(item['copia']).read_bytes(), user
                )
        print('Failure: previous files were restored.')
        print('Klipper remains stopped.')
        raise
    print('Configuration restored. Previous backup:', backup)
    print('Klipper remains stopped for review.')
    print('Inspect the physical filament path before starting Klipper.')
    print('To start: sudo systemctl start klipper')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--home', type=Path)
    parser.add_argument('--snapshot', type=Path, required=True)
    parser.add_argument(
        '--apply', '--aplicar', dest='apply', action='store_true'
    )
    args = parser.parse_args()
    user = (
        pwd.getpwnam(os.environ['SUDO_USER'])
        if os.environ.get('SUDO_USER')
        else pwd.getpwuid(os.getuid())
    )
    home = args.home or Path(user.pw_dir)
    metadata = verify(args.snapshot)
    changes = prepare(home, args.snapshot)
    print('Configuration, environment and modules prepared:', len(changes))
    if not args.apply:
        print('Dry run: no files or services were changed.')
        return
    if os.geteuid() != 0:
        raise RuntimeError('Use sudo to apply the restoration.')
    check_application(home, metadata)
    apply(home, changes, user)


if __name__ == '__main__':
    main()
