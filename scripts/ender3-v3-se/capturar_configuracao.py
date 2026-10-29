import argparse
import hashlib
import io
import json
import os
import pwd
import subprocess
import tarfile
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from urllib.parse import quote
from urllib.request import urlopen


def command(arguments):
    try:
        result = subprocess.run(arguments, capture_output=True, timeout=45)
        return {
            'codigo': result.returncode,
            'saida': result.stdout.decode(errors='replace'),
            'erro': result.stderr.decode(errors='replace'),
        }
    except (OSError, subprocess.TimeoutExpired) as error:
        return {'codigo': -1, 'erro': str(error)}


def query_object(name):
    url = 'http://127.0.0.1:7125/printer/objects/query?' + quote(name, safe='')
    with urlopen(url, timeout=15) as response:
        data = json.load(response)
    return data['result']['status'][name]


def check_idle():
    if query_object('print_stats')['state'] in ('printing', 'paused'):
        raise RuntimeError(
            ('Finish printing before capturing the configuration.')
        )
    bmcu = query_object('bmcu')
    if bmcu.get('active_operations') or bmcu.get('print_plan_open'):
        raise RuntimeError('Wait for BMCU operations to finish.')
    for device in bmcu.get('devices', []):
        if any(
            c.get('motion') or c.get('motor_pwm')
            for c in device.get('channels', [])
        ):
            raise RuntimeError('Stop BMCU motors before capture.')
    return bmcu


class BackupCapture:
    def __init__(self, file_path):
        self.tar = tarfile.open(file_path, 'w:gz')
        self.records = {}
        self.warnings = []
        self.sources = {}

    def add(self, name, data, source_path=None):
        if name in self.records:
            raise RuntimeError('Duplicate backup file: ' + name)
        item = tarfile.TarInfo(name)
        item.size = len(data)
        item.mode = 0o600
        self.tar.addfile(item, io.BytesIO(data))
        self.records[name] = {
            'bytes': len(data),
            'sha256': hashlib.sha256(data).hexdigest(),
        }
        if source_path is not None:
            self.sources[str(source_path)] = self.records[name]['sha256']

    def file_path(self, path, name, required=False):
        try:
            self.add(name, path.read_bytes(), path)
            info = path.stat()
            self.records[name].update(
                {
                    'modo': info.st_mode & 0o777,
                    'uid': info.st_uid,
                    'gid': info.st_gid,
                }
            )
        except OSError as error:
            if required:
                raise RuntimeError('Could not copy ' + str(path)) from error
            self.warnings.append(str(path) + ': ' + str(error))

    def directory(self, path, prefix, ignore=(), follow_symlinks=False):
        if not path.is_dir():
            self.warnings.append('Missing directory: ' + str(path))
            return
        visited = set()
        for root, directories, files in os.walk(
            path, followlinks=follow_symlinks
        ):
            real = Path(root).resolve()
            if real in visited:
                directories[:] = []
                self.warnings.append(
                    'Repeated symlinked directory: ' + str(root)
                )
                continue
            visited.add(real)
            for directory in list(directories):
                p = Path(root) / directory
                if directory in (
                    '.git',
                    '__pycache__',
                    '.cache',
                ) or directory.startswith('backup-'):
                    directories.remove(directory)
                elif p.is_symlink() and not follow_symlinks:
                    directories.remove(directory)
                    self.warnings.append(
                        'Copy this symlinked directory separately: ' + str(p)
                    )
            for file_path in sorted(files):
                p = Path(root) / file_path
                if any(file_path.endswith(s) for s in ignore):
                    continue
                if p.is_file():
                    self.file_path(
                        p, prefix + '/' + p.relative_to(path).as_posix()
                    )

    def finish(self, metadata):
        for source_path, expected in self.sources.items():
            if (
                hashlib.sha256(Path(source_path).read_bytes()).hexdigest()
                != expected
            ):
                raise RuntimeError(
                    'File changed during capture; retry: ' + source_path
                )
        manifest = {
            'formato': 1,
            'arquivos': dict(self.records),
            'avisos': self.warnings,
            'metadados': metadata,
        }
        self.add(
            'manifesto.json',
            (
                json.dumps(manifest, indent=2, ensure_ascii=False) + '\n'
            ).encode(),
        )


def capture(home, output):
    status = check_idle()
    configurations = home / 'printer_data/config'
    if not (configurations / 'printer.cfg').is_file():
        raise RuntimeError(
            'printer.cfg was not found in ' + str(configurations)
        )
    active_config = query_object('configfile')
    klipper_root = home / 'klipper'
    git = [
        'git',
        '-c',
        'safe.directory=' + str(klipper_root),
        '-C',
        str(klipper_root),
    ]
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        raise RuntimeError('The destination already exists: ' + str(output))
    with tempfile.NamedTemporaryFile(
        dir=output.parent, suffix='.parcial', delete=False
    ) as temp:
        temporary = Path(temp.name)
    capture_job = BackupCapture(temporary)
    try:
        capture_job.directory(
            configurations,
            'config',
            ('.tar.gz', '.tar', '.zip', '.3mf', '.gcode', '.pyc', '.parcial'),
        )
        capture_job.directory(
            home / 'printer_data/systemd', 'ambiente-servicos'
        )
        capture_job.directory(
            klipper_root / 'klippy',
            'klippy-instalado',
            ('.pyc', '.so'),
            follow_symlinks=True,
        )
        for name in (
            '.config',
            'out/klipper.bin',
            'out/klipper.dict',
            'out/defconfig',
        ):
            path = klipper_root / name
            if path.is_file():
                capture_job.file_path(path, 'compilacao/' + name.lstrip('.'))
        capture_job.file_path(
            Path('/usr/local/bin/klipper_mcu'), 'host-mcu/klipper_mcu'
        )
        for path in (
            Path('/boot/armbianEnv.txt'),
            Path('/etc/armbian-release'),
            Path('/etc/os-release'),
        ):
            capture_job.file_path(
                path, 'sistema/' + path.as_posix().lstrip('/')
            )
        capture_job.directory(
            Path('/boot/overlay-user'), 'sistema/boot/overlay-user'
        )
        capture_job.directory(
            Path('/etc/udev/rules.d'), 'sistema/etc/udev/rules.d'
        )
        for path in sorted(Path('/etc/systemd/system').glob('*')):
            if not any(
                n in path.name
                for n in ('klipper', 'moonraker', 'bmcu', 'crowsnest')
            ):
                continue
            if path.is_dir() and not path.is_symlink():
                capture_job.directory(
                    path, 'sistema/etc/systemd/system/' + path.name
                )
            elif path.is_file() and not path.is_symlink():
                capture_job.file_path(
                    path, 'sistema/etc/systemd/system/' + path.name
                )
        for path in sorted(Path('/etc/default').glob('*')):
            if path.is_file() and any(
                n in path.name for n in ('klipper', 'moonraker', 'bmcu')
            ):
                capture_job.file_path(path, 'sistema/etc/default/' + path.name)
        data = {
            'uname': command(['uname', '-a']),
            'pacotes': command(['dpkg-query', '-W']),
            'python': command(
                [str(home / 'klippy-env/bin/python'), '-m', 'pip', 'freeze']
            ),
            'gpio': command(['gpiodetect']),
            'gpio_linhas': command(['gpioinfo']),
            'usb': command(['lsusb']),
            'klipper_commit': command(git + ['rev-parse', 'HEAD']),
            'klipper_versao': command(
                git + ['describe', '--tags', '--always', '--dirty']
            ),
            'klipper_status': command(git + ['status', '--short']),
        }
        diff = subprocess.run(
            git + ['diff', '--binary', 'HEAD'],
            capture_output=True,
            timeout=45,
            check=True,
        )
        capture_job.add('fontes/klipper-alteracoes.patch', diff.stdout)
        with tempfile.TemporaryFile() as source:
            subprocess.run(
                git + ['archive', '--format=tar', 'HEAD'],
                stdout=source,
                check=True,
                timeout=45,
            )
            source.seek(0)
            capture_job.add('fontes/klipper-commit.tar', source.read())
        for name in (
            'klipper',
            'klipper-mcu',
            'moonraker',
            'bmcu-transport',
            'crowsnest',
        ):
            data[name] = command(['systemctl', 'cat', name])
        data['servicos_bmcu'] = command(
            ['systemctl', 'list-unit-files', '*bmcu*', '--no-pager']
        )
        for name, content in [('bmcu', status), ('configfile', active_config)]:
            capture_job.add(
                'diagnostico/' + name + '.json',
                json.dumps(content, indent=2, ensure_ascii=False).encode(),
            )
        bmcu_status = (
            active_config.get('config', {}).get('bmcu', {}).get('state_file')
        )
        if bmcu_status:
            path = Path(bmcu_status.replace('~/', str(home) + '/', 1))
            if not path.is_absolute():
                path = configurations / path
            if path.resolve().is_relative_to(configurations.resolve()):
                pass
            else:
                capture_job.file_path(
                    path, 'estado-externo/bmcu_state.json', required=True
                )
        check_idle()
        capture_job.finish(
            {
                'criado_utc': datetime.now(timezone.utc).isoformat(),
                'home_original': str(home),
                'ambiente': data,
            }
        )
        capture_job.tar.close()
        os.chmod(temporary, 0o600)
        os.replace(temporary, output)
    finally:
        capture_job.tar.close()
        temporary.unlink(missing_ok=True)
    return capture_job.warnings


def capture_service_environment(home, output):
    source_path = home / 'printer_data/systemd'
    if not (source_path / 'klipper.env').is_file():
        raise RuntimeError('klipper.env was not found in ' + str(source_path))
    if output.exists():
        raise RuntimeError('The destination already exists: ' + str(output))
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile(
        dir=output.parent, suffix='.parcial', delete=False
    ) as temp:
        temporary = Path(temp.name)
    capture_job = BackupCapture(temporary)
    try:
        capture_job.directory(source_path, 'ambiente-servicos')
        for name in ('mcu', 'mcu host'):
            try:
                status = query_object(name)
                capture_job.add(
                    'diagnostico/' + name.replace(' ', '-') + '.json',
                    json.dumps(status, indent=2).encode(),
                )
            except (OSError, KeyError, ValueError) as error:
                capture_job.warnings.append(name + ': ' + str(error))
        capture_job.finish(
            {
                'criado_utc': datetime.now(timezone.utc).isoformat(),
                'home_original': str(home),
                'tipo': 'complemento-ambiente-servicos',
            }
        )
        capture_job.tar.close()
        temporary.chmod(0o600)
        os.replace(temporary, output)
    finally:
        capture_job.tar.close()
        temporary.unlink(missing_ok=True)
    return capture_job.warnings


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--home', type=Path)
    parser.add_argument('--output', '--saida', dest='output', type=Path)
    parser.add_argument(
        '--services-only',
        '--complemento',
        dest='services_only',
        action='store_true',
    )
    args = parser.parse_args()
    user = (
        pwd.getpwnam(os.environ['SUDO_USER'])
        if os.environ.get('SUDO_USER')
        else pwd.getpwuid(os.getuid())
    )
    home = args.home or Path(user.pw_dir)
    name = (
        (
            'complemento-restauracao-'
            if args.services_only
            else 'backup-restauracao-'
        )
        + datetime.now().strftime('%Y%m%d-%H%M%S')
        + '.tar.gz'
    )
    output = args.output or home / 'printer_data/config' / name
    function = capture_service_environment if args.services_only else capture
    warnings = function(home, output)
    if os.geteuid() == 0:
        os.chown(output, user.pw_uid, user.pw_gid)
    print('Backup created:', output)
    print('SHA256:', hashlib.sha256(output.read_bytes()).hexdigest())
    print(('Download through Mainsail > Machine > Configuration Files.'))
    print(
        (
            (
                'This backup is private and may contain passwords and tokens. '
                'Do not publish it on '
                'GitHub.'
            )
        )
    )
    for warning in warnings:
        print('Pending:', warning)


if __name__ == '__main__':
    main()
