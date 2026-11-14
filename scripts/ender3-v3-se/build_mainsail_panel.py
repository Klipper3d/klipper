"""Build the native card from an exact, separate upstream Mainsail checkout."""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import shutil
import subprocess
import tarfile
import zipfile

ROOT = Path(__file__).resolve().parents[2]
OVERLAY = ROOT / 'integrations/mainsail'
MAINSAIL_COMMIT = '5fb9e77fb9f4e60cf0725d9dc7f57cf7b84bbd70'
MAINSAIL_VERSION = '2.19.0'
INSTRUCTION_ALIASES = {
    '.github/agent_docs': '../agent_docs',
    '.github/copilot-instructions.md': '../AGENTS.md',
    'CLAUDE.md': 'AGENTS.md',
}


def replace_once(text, before, after):
    if text.count(before) != 1:
        raise ValueError('Unsupported Mainsail source; do not patch blindly.')
    return text.replace(before, after, 1)


def sorted_locale(value):
    if isinstance(value, dict):
        return {key: sorted_locale(value[key])
                for key in sorted(value, key=str.casefold)}
    if isinstance(value, list):
        return [sorted_locale(item) for item in value]
    return value


def build_provenance(release):
    fingerprint = hashlib.sha256()
    for source in sorted(OVERLAY.rglob('*')):
        if source.is_file():
            name = source.relative_to(OVERLAY).as_posix()
            data = source.read_bytes().replace(b'\r\n', b'\n')
            fingerprint.update(name.encode() + b'\0' + data)
    recipe = Path(__file__).read_bytes().replace(b'\r\n', b'\n')
    return {
        'schema': 1, 'release': release,
        'mainsail_version': MAINSAIL_VERSION,
        'mainsail_commit': MAINSAIL_COMMIT,
        'bmcu_version': '1.1.2', 'required_firmware': '1.0.4',
        'overlay_sha256': fingerprint.hexdigest(),
        'recipe_sha256': hashlib.sha256(recipe).hexdigest(),
    }


def apply_overlay(root, release):
    root = Path(root)
    package = json.loads((root / 'package.json').read_text())
    if package['version'] != MAINSAIL_VERSION:
        raise ValueError('This overlay requires Mainsail ' + MAINSAIL_VERSION)
    patches = {
        'src/pages/Dashboard.vue': [
            ("import AfcPanel from '@/components/panels/AfcPanel.vue'",
             "import BmcuPanel from '@/components/panels/BmcuPanel.vue'\n"
             "import AfcPanel from '@/components/panels/AfcPanel.vue'"),
            ('        AfcPanel,', '        BmcuPanel,\n        AfcPanel,'),
        ],
        'src/store/variables.ts': [
            ("    'afc',", "    'bmcu',\n    'afc',"),
        ],
        'src/store/gui/getters.ts': [
            ('        // remove afc panel, if no AFC module exists in Klipper',
             '        if (!rootState.printer?.bmcu_ender_integration) {\n'
             '            allPanels = allPanels.filter('
             "(name) => name !== 'bmcu')\n"
             '        }\n\n'
             '        // remove afc panel, if no AFC module exists in Klipper'),
        ],
        'src/components/mixins/dashboard.ts': [
            ("            case 'mmu':",
             "            case 'bmcu':\n            case 'mmu':"),
        ],
    }
    changes = {}
    for name, replacements in patches.items():
        text = (root / name).read_text(encoding='utf-8')
        for before, after in replacements:
            text = replace_once(text, before, after)
        changes[name] = text.encode('utf-8')
    for language in ('en', 'pt'):
        name = 'src/locales/' + language + '.json'
        locale = json.loads((root / name).read_text(encoding='utf-8'))
        if 'BmcuPanel' in locale['Panels']:
            raise ValueError('BMCU panel translations already exist.')
        locale['Panels']['BmcuPanel'] = json.loads(
            (OVERLAY / 'locales' / (language + '.json')).read_text('utf-8'))
        changes[name] = (json.dumps(sorted_locale(locale), ensure_ascii=False,
                                   indent=4) + '\n').encode('utf-8')
    sources = sorted((OVERLAY / 'overlay').rglob('*'))
    for source in sources:
        if not source.is_file():
            continue
        name = source.relative_to(OVERLAY / 'overlay').as_posix()
        data = source.read_bytes().replace(b'\r\n', b'\n')
        changes[name] = data
    changes['public/bmcu-build.json'] = (
        json.dumps(build_provenance(release), indent=2) + '\n').encode()
    # Validate every source marker before writing any file.
    for name, data in changes.items():
        target = root / name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)


def prepare_source(source, output, release):
    source, output = Path(source).resolve(), Path(output).absolute()
    if output.exists() or output.is_symlink():
        raise ValueError('Use a new output directory; existing files are kept.')
    git = os.environ.get('GIT', 'git')
    commit = subprocess.check_output(
        [git, 'rev-parse', 'HEAD'], cwd=source).decode().strip()
    if commit != MAINSAIL_COMMIT:
        raise ValueError('Select the pinned Mainsail 2.19.0 commit.')
    archive = subprocess.check_output(
        [git, 'archive', '--format=tar', commit], cwd=source)
    with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
        members = tar.getmembers()
        for member in members:
            if member.issym() and INSTRUCTION_ALIASES.get(
                member.name
            ) == member.linkname:
                continue
            name = Path(member.name)
            if (name.is_absolute() or '..' in name.parts
                    or not (member.isfile() or member.isdir())):
                raise ValueError('Unsupported upstream archive entry.')
        output.mkdir(parents=True)
        for member in members:
            if member.issym():
                # Only redundant instruction aliases were allowed above.
                continue
            target = output / member.name
            if member.isdir():
                target.mkdir(parents=True, exist_ok=True)
            else:
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(tar.extractfile(member).read())
    apply_overlay(output, release)


def package_panel(dist, output):
    dist, output = Path(dist), Path(output)
    if (dist.is_symlink() or output.resolve().is_relative_to(dist.resolve())
            or any(path.is_symlink() for path in dist.rglob('*'))):
        raise ValueError('Use a regular build tree and an external ZIP path.')
    manifest_path = dist / 'bmcu-build.json'
    manifest = json.loads(manifest_path.read_text())
    expected = build_provenance((ROOT / 'VERSION').read_text().strip())
    if (not isinstance(manifest, dict) or any(
            manifest.get(key) != value for key, value in expected.items())):
        raise ValueError(
            'Stale build provenance; rebuild from canonical sources.')
    manifest['files'] = {
        path.relative_to(dist).as_posix(): hashlib.sha256(
            path.read_bytes()).hexdigest()
        for path in sorted(dist.rglob('*'))
        if path.is_file() and path != manifest_path
    }
    manifest_path.write_bytes((json.dumps(manifest, sort_keys=True, indent=2)
                               + '\n').encode('utf-8'))
    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open('xb') as stream:
        with zipfile.ZipFile(stream, 'w', zipfile.ZIP_DEFLATED) as archive:
            for path in sorted(dist.rglob('*')):
                if path.is_file():
                    entry = zipfile.ZipInfo(path.relative_to(dist).as_posix(),
                                            date_time=(2000, 1, 1, 0, 0, 0))
                    entry.compress_type = zipfile.ZIP_DEFLATED
                    entry.external_attr = 0o100644 << 16
                    archive.writestr(entry, path.read_bytes())
    return hashlib.sha256(output.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--artifact', type=Path)
    parser.add_argument('--prepare-only', action='store_true')
    args = parser.parse_args()
    release = (ROOT / 'VERSION').read_text().strip()
    prepare_source(args.source, args.output, release)
    if args.prepare_only:
        print('Prepared pinned Mainsail source:', args.output)
        return
    npm = shutil.which('npm')
    if not npm:
        raise RuntimeError('Install Node.js and npm before building the panel.')
    env = dict(os.environ, CYPRESS_INSTALL_BINARY='0')
    for command in ([npm, 'ci', '--no-audit', '--no-fund'],
                    [npm, 'run', 'test:unit'], [npm, 'run', 'lint'],
                    [npm, 'exec', '--', 'prettier',
                     'src/components/panels/BmcuPanel.vue',
                     'src/components/panels/Bmcu/bmcu.ts',
                     'tests/bmcu.spec.ts', 'tests/bmcu-preview.ts',
                     'tests/bmcu-preview.html', '--check'],
                    [npm, 'exec', '--', 'vite', 'build']):
        subprocess.run(command, cwd=args.output, env=env, check=True)
    dist = args.output / 'dist'
    shutil.copyfile(args.output / 'LICENSE', dist / 'Mainsail-LICENSE')
    shutil.copyfile(ROOT / 'COPYING', dist / 'Ender-Integration-LICENSE')
    artifact = args.artifact or (ROOT / 'ci_build/mainsail-bmcu.zip')
    print(package_panel(dist, artifact), artifact)


if __name__ == '__main__':
    main()
