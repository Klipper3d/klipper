"""Stage a host-only checkout update and retain the complete previous tree.

The caller owns printer preflight, service control and the configuration/state
transaction. This module never talks to a printer, flashes firmware or resets
Git. Renamed checkout trees retain their Git history and untracked user files.
"""

import hashlib
import json
import os
import fcntl
from contextlib import contextmanager
from pathlib import Path
import shutil
import subprocess


@contextmanager
def update_lock(path, owner=None):
    """Serialize installation and rollback without replacing the lock inode."""
    descriptor = os.open(path, os.O_CREAT | os.O_RDWR | os.O_NOFOLLOW, 0o600)
    try:
        try:
            fcntl.flock(descriptor, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError as error:
            raise ValueError(
                'Wait for another update or rollback to finish.') from error
        if owner is not None and os.geteuid() == 0:
            os.fchown(descriptor, *owner)
        yield
    finally:
        os.close(descriptor)


def _git(root, *arguments):
    return subprocess.check_output(
        ['git', '-c', 'safe.directory=' + str(root), '-C', str(root),
         *arguments], stderr=subprocess.PIPE,
    )


def _commit(root):
    return _git(root, 'rev-parse', 'HEAD').decode().strip()


def _clean_checkout(root):
    if root.is_symlink() or not (root / '.git').is_dir():
        raise ValueError('Use a normal, independent Git checkout: ' + str(root))
    if _git(root, 'status', '--porcelain', '--untracked-files=no').strip():
        raise ValueError('Preserve or commit local changes before updating: '
                         + str(root))


def _optional_files(root):
    tracked = set(_git(root, 'ls-files', '-z').decode().split('\0'))
    extras = root / 'klippy/extras'
    files = {}
    for path in sorted(extras.rglob('*')):
        if '__pycache__' in path.parts or path.suffix == '.pyc':
            continue
        name = path.relative_to(root).as_posix()
        if name in tracked or path.is_dir() and not path.is_symlink():
            continue
        if path.is_symlink():
            files[name] = ('link', os.readlink(path), path.lstat().st_mode)
        elif path.is_file():
            checksum = hashlib.sha256(path.read_bytes()).hexdigest()
            files[name] = ('file', checksum, path.stat().st_mode)
    return files


def restore_checkout(record):
    """Restore the previous tree without overwriting checkout snapshots."""
    active = Path(record['active'])
    previous = Path(record['previous'])
    failed = Path(record['failed'])
    if not previous.exists():
        if active.is_dir() and _commit(active) == record['previous_commit']:
            return
        raise ValueError(
            'The previous host checkout is missing; remain stopped.')
    if (previous.is_symlink() or active.is_symlink()
            or _commit(previous) != record['previous_commit']):
        raise ValueError('The preserved host checkout changed; remain stopped.')
    if active.exists() and _commit(active) != record['target_commit']:
        raise ValueError(
            'The active host revision changed; review before rollback.')
    if active.exists() and failed.exists():
        raise ValueError(
            'The failed checkout snapshot already exists: ' + str(failed))
    if active.exists():
        active.rename(failed)
    previous.rename(active)


class HostTransaction:
    def __init__(self, source, active, work):
        self.source = Path(source).absolute()
        self.active = Path(active).absolute()
        self.work = Path(work).absolute()
        self.staged = self.work / 'host-staged'
        self.previous = self.work / 'host-before'
        self.failed = self.work / 'host-failed'
        self.before = self.after = None
        self.optional = None
        self.other_untracked = []

    def prepare(self):
        _clean_checkout(self.active)
        _clean_checkout(self.source)
        active, source, work = (
            path.resolve() for path in (self.active, self.source, self.work)
        )
        if (active == source or work.is_relative_to(active)
                or work.is_relative_to(source) or active.is_relative_to(work)
                or source.is_relative_to(work)):
            raise ValueError(
                'Use separate source, active and backup directories.')
        # This transaction deliberately cannot roll back a flashed MCU or a
        # changed Python environment. Require identical inputs for both.
        paths = ('src', 'lib', 'test/configs', 'Makefile',
                 'scripts/buildcommands.py', 'scripts/klippy-requirements.txt')
        if _git(active, 'ls-tree', '-r', 'HEAD', '--', *paths) != _git(
            source, 'ls-tree', '-r', 'HEAD', '--', *paths
        ):
            raise ValueError('MCU firmware or dependencies changed; perform a '
                             'separately reviewed firmware/environment update.')
        self.before, self.after = _commit(active), _commit(source)
        self.optional = _optional_files(active)
        self.other_untracked = [name for name in _git(
            active, 'ls-files', '--others', '--exclude-standard', '-z',
        ).decode().split('\0')
            if name and not name.startswith('klippy/extras/')]
        if self.work.parent.stat().st_dev != self.active.stat().st_dev:
            raise ValueError('Host activation requires one filesystem.')
        self.work.mkdir(mode=0o700)
        owner = self.active.stat()
        if os.geteuid() == 0:
            os.chown(self.work, owner.st_uid, owner.st_gid)
        subprocess.run(
            ['git', 'clone', '--quiet', '--no-hardlinks', str(source),
             str(self.staged)], check=True, capture_output=True,
        )
        if _commit(self.staged) != self.after:
            raise ValueError('The target revision changed during staging.')
        staged_remotes = _git(self.staged, 'remote').decode().split()
        for name in _git(active, 'remote').decode().split():
            remote = _git(active, 'remote', 'get-url', name).decode().strip()
            action = 'set-url' if name in staged_remotes else 'add'
            _git(self.staged, 'remote', action, name, remote)
        for name, (kind, _, _) in self.optional.items():
            original, target = active / name, self.staged / name
            if target.exists() or target.is_symlink():
                raise ValueError(
                    'Optional module conflicts with target source: ' + name)
            target.parent.mkdir(parents=True, exist_ok=True)
            if kind == 'link':
                target.symlink_to(os.readlink(original),
                                  target_is_directory=original.is_dir())
            else:
                shutil.copy2(original, target)
        _clean_checkout(self.source)
        if _commit(self.source) != self.after:
            raise ValueError('The source revision changed during staging.')
        if os.geteuid() == 0:
            for path in [self.staged, *self.staged.rglob('*')]:
                os.chown(path, owner.st_uid, owner.st_gid,
                         follow_symlinks=False)
        journal = self.work / 'host-record.json'
        journal.write_text(json.dumps(self.record(), indent=2))
        journal.chmod(0o600)
        if os.geteuid() == 0:
            os.chown(journal, owner.st_uid, owner.st_gid)

    def record(self):
        if self.before is None or self.after is None:
            raise ValueError(
                'Prepare the host transaction before recording it.')
        return {
            'active': str(self.active), 'previous': str(self.previous),
            'failed': str(self.failed), 'previous_commit': self.before,
            'target_commit': self.after,
            'optional_modules_copied': sorted(self.optional),
            'other_untracked_preserved': list(self.other_untracked),
        }

    def activate(self):
        _clean_checkout(self.active)
        _clean_checkout(self.staged)
        if (_commit(self.active) != self.before
                or _optional_files(self.active) != self.optional):
            raise ValueError('The active host changed after staging.')
        if self.previous.exists() or _commit(self.staged) != self.after:
            raise ValueError(
                'The staged/preserved host changed before activation.')
        if _optional_files(self.staged) != self.optional:
            raise ValueError('Optional modules changed in the staged checkout.')
        self.active.rename(self.previous)
        try:
            self.staged.rename(self.active)
        except Exception:
            self.previous.rename(self.active)
            raise

    def rollback(self):
        restore_checkout(self.record())
