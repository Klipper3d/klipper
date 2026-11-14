"""Install or roll back the Mainsail card; never issue motion G-code."""
import argparse
from contextlib import contextmanager
import fcntl
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import stat
import uuid
from urllib.request import urlopen
import zipfile

from build_mainsail_panel import build_provenance

ROOT = Path(__file__).resolve().parents[2]
MAX_UNPACKED = 128 * 1024 * 1024


def check_idle():
    url = ('http://127.0.0.1:7125/printer/objects/query?'
           'webhooks&print_stats&bmcu_ender_integration')
    with urlopen(url, timeout=15) as response:
        data = json.load(response)['result']['status']
    if (data['webhooks']['state'] != 'ready'
            or data['print_stats']['state'] not in (
                'standby', 'complete', 'cancelled')
            or data['bmcu_ender_integration']['manual_busy']
            or data['bmcu_ender_integration']['panel']['busy']):
        raise RuntimeError('The printer and BMCU must be ready and idle.')


@contextmanager
def panel_lock(parent):
    path = parent / '.mainsail-panel-update.lock'
    fd = os.open(path, os.O_CREAT | os.O_RDWR | os.O_NOFOLLOW, 0o600)
    try:
        if not stat.S_ISREG(os.fstat(fd).st_mode):
            raise ValueError('Invalid panel update lock.')
        fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
        yield
    finally:
        os.close(fd)


def inventory(root):
    result = {}
    for path in sorted(root.rglob('*')):
        name = path.relative_to(root).as_posix()
        if path.is_symlink():
            result[name] = 'link:' + os.readlink(path)
        elif path.is_dir():
            result[name] = 'directory'
        elif path.is_file():
            result[name] = hashlib.sha256(path.read_bytes()).hexdigest()
    return result


def validate_archive(archive, expected_sha256):
    expected_sha256 = expected_sha256.lower()
    if (not re.fullmatch(r'[0-9a-f]{64}', expected_sha256)
            or hashlib.sha256(Path(archive).read_bytes()).hexdigest()
            != expected_sha256):
        raise ValueError('Panel archive checksum mismatch.')
    with zipfile.ZipFile(archive) as zipped:
        entries = zipped.infolist()
        if sum(entry.file_size for entry in entries) > MAX_UNPACKED:
            raise ValueError('Panel archive is too large.')
        if len({entry.filename for entry in entries}) != len(entries):
            raise ValueError('Duplicate panel archive entries.')
        for entry in entries:
            name = PurePosixPath(entry.filename)
            if (name.is_absolute() or '..' in name.parts or not name.parts
                    or name.as_posix() != entry.filename
                    or '\\' in entry.filename or ':' in entry.filename
                    or entry.is_dir()
                    or stat.S_IFMT(entry.external_attr >> 16)
                    not in (0, stat.S_IFREG)):
                raise ValueError('Unsafe panel archive path.')
        files = {entry.filename: zipped.read(entry) for entry in entries}
    manifest = json.loads(files.get('bmcu-build.json', b'{}'))
    provenance = build_provenance((ROOT / 'VERSION').read_text().strip())
    if (not isinstance(manifest, dict) or any(
            manifest.get(key) != value for key, value in provenance.items())):
        raise ValueError('Unsupported panel build provenance.')
    license_data = (ROOT / 'COPYING').read_bytes().replace(b'\r\n', b'\n')
    for name in ('Mainsail-LICENSE', 'Ender-Integration-LICENSE'):
        if files.get(name, b'').replace(b'\r\n', b'\n') != license_data:
            raise ValueError('Missing or changed panel license: ' + name)
    expected = manifest.get('files')
    if (not isinstance(expected, dict)
            or set(expected) != set(files) - {'bmcu-build.json'}):
        raise ValueError('Incomplete panel file manifest.')
    for name, checksum in expected.items():
        if hashlib.sha256(files[name]).hexdigest() != checksum:
            raise ValueError('Panel file checksum mismatch: ' + name)
    if b'mainsail' not in files.get('index.html', b'').lower():
        raise ValueError('The archive is not a Mainsail web build.')
    return files


def preserve_user_files(active, staged):
    for name in ('config.json', '.theme'):
        source, target = active / name, staged / name
        if not source.exists() and not source.is_symlink():
            continue
        if target.exists() or target.is_symlink():
            if not target.is_file() or target.is_symlink():
                raise ValueError('Unexpected bundled user-settings path.')
            target.unlink()
        if source.is_symlink():
            target.symlink_to(os.readlink(source),
                              target_is_directory=source.is_dir())
        elif source.is_dir():
            shutil.copytree(source, target, symlinks=True)
        else:
            shutil.copy2(source, target)


def write_record(path, data):
    path.write_bytes((json.dumps(data, indent=2) + '\n').encode())
    path.chmod(0o600)


def install(archive, destination, expected_sha256):
    files = validate_archive(archive, expected_sha256)
    active = Path(destination).absolute()
    if (active.is_symlink() or not active.is_dir()
            or (active / '.git').exists()
            or b'mainsail' not in (active / 'index.html').read_bytes().lower()):
        raise ValueError('Select an existing non-Git Mainsail web directory.')
    with panel_lock(active.parent):
        check_idle()
        before = inventory(active)
        owner = active.stat()
        backup = active.parent / ('mainsail-panel-backup-' + uuid.uuid4().hex)
        backup.mkdir(mode=0o700)
        staged, previous = backup / 'staged', backup / 'previous'
        staged.mkdir()
        for name, data in files.items():
            path = staged / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        preserve_user_files(active, staged)
        for path in [staged, *staged.rglob('*')]:
            if not path.is_symlink():
                path.chmod(0o755 if path.is_dir() else 0o644)
            if os.geteuid() == 0:
                os.chown(path, owner.st_uid, owner.st_gid,
                         follow_symlinks=False)
        check_idle()
        if inventory(active) != before:
            raise ValueError('Mainsail files changed during preparation.')
        record = {
            'schema': 1, 'active': str(active), 'previous': str(previous),
            'archive_sha256': expected_sha256,
            'previous_files': before, 'installed_files': inventory(staged),
        }
        try:
            active.rename(previous)
            staged.rename(active)
            record_path = backup / 'rollback.json'
            write_record(record_path, record)
        except Exception:
            if previous.exists():
                if active.exists():
                    active.rename(backup / 'failed')
                previous.rename(active)
            raise
        return record_path


def rollback(record_path):
    record_path = Path(record_path).absolute()
    data = json.loads(record_path.read_text())
    active, previous = Path(data['active']), Path(data['previous'])
    backup = record_path.parent
    if (data.get('schema') != 1 or previous != backup / 'previous'
            or active.parent != backup.parent or active.is_symlink()
            or previous.is_symlink()):
        raise ValueError('Invalid panel rollback record.')
    with panel_lock(active.parent):
        check_idle()
        if (inventory(active) != data['installed_files']
                or inventory(previous) != data['previous_files']):
            raise ValueError(
                'Panel or user files changed; review before rollback.')
        replaced = backup / 'replaced'
        if replaced.exists():
            raise ValueError('This panel rollback was already used.')
        active.rename(replaced)
        try:
            previous.rename(active)
        except Exception:
            replaced.rename(active)
            raise
        return active


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--archive', type=Path)
    parser.add_argument('--destination', type=Path)
    parser.add_argument('--sha256')
    parser.add_argument('--rollback', type=Path)
    args = parser.parse_args()
    if args.rollback:
        if args.archive or args.destination or args.sha256:
            parser.error('Use --rollback alone.')
        print('Previous Mainsail restored:', rollback(args.rollback))
    else:
        if not all((args.archive, args.destination, args.sha256)):
            parser.error('--archive, --destination and --sha256 are required.')
        record = install(args.archive, args.destination, args.sha256)
        print('Mainsail card installed. Refresh the browser. Rollback record:',
              record)


if __name__ == '__main__':
    main()
