import argparse
import hashlib
import json
import tarfile
from pathlib import Path, PurePosixPath


def verify(file_path, destination=None):
    with tarfile.open(file_path, 'r:gz') as tar:
        members = tar.getmembers()
        names = [m.name for m in members]
        if len(names) != len(set(names)):
            raise ValueError('The archive contains duplicate names.')
        for member in members:
            path = PurePosixPath(member.name)
            if (
                not member.isfile()
                or path.is_absolute()
                or '..' in path.parts
                or '\\' in member.name
            ):
                raise ValueError('Invalid entry: ' + member.name)
        manifest = json.load(tar.extractfile('manifesto.json'))
        if manifest['formato'] != 1 or set(names) != set(
            manifest['arquivos']
        ) | {'manifesto.json'}:
            raise ValueError(
                'The manifest does not match the archive contents.'
            )
        for name, metadata in manifest['arquivos'].items():
            data = tar.extractfile(name).read()
            if (
                len(data) != metadata['bytes']
                or hashlib.sha256(data).hexdigest() != metadata['sha256']
            ):
                raise ValueError('File changed or incomplete: ' + name)
        if destination is not None:
            destination.mkdir(mode=0o700, parents=True, exist_ok=False)
            for member in members:
                path = destination / member.name
                path.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
                path.write_bytes(tar.extractfile(member).read())
                path.chmod(0o600)
    return manifest


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('archive', type=Path)
    parser.add_argument('--extract', '--extrair', dest='extract', type=Path)
    args = parser.parse_args()
    manifest = verify(args.archive, args.extract)
    print('Backup verified: %d files.' % len(manifest['arquivos']))
    print('Source:', manifest['metadados']['home_original'])
    print('Created:', manifest['metadados']['criado_utc'])
    for warning in manifest['avisos']:
        print('Pending:', warning)
    if args.extract:
        print('Files extracted for review to:', args.extract)
        print(('No services, firmware or active configuration were changed.'))


if __name__ == '__main__':
    main()
