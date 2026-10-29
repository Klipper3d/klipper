import ast
import json
import os
import pwd
import re
import shutil
import subprocess
import tarfile
import tempfile
import textwrap
import time
from datetime import datetime
from pathlib import Path
from urllib.request import urlopen

ROOT = Path(__file__).resolve().parents[2]
MODULE = (ROOT / 'integrations/bmcu/bmcu_integracao_ender.py').read_text()
CONFIGURATION = (
    ROOT / ('config/ender3-v3-se-validada/bmcu-multicor-' 'ender.cfg')
).read_text()
MEASUREMENT = (ROOT / 'integrations/bmcu/bmcu_medicao.py').read_text()


def query_object(name):
    from urllib.parse import quote

    url = 'http://127.0.0.1:7125/printer/objects/query?' + quote(name, safe='')
    with urlopen(url, timeout=15) as response:
        data = json.load(response)
    if 'error' in data:
        raise RuntimeError(str(data['error']))
    return data['result']['status'][name]


def check_printer():
    status = query_object('webhooks')
    if status['state'] != 'ready':
        raise RuntimeError(
            ('Klipper must be ready before installation: ') + str(status)
        )
    if query_object('print_stats')['state'] in ('printing', 'paused'):
        raise RuntimeError('Finish printing before installing.')
    status = query_object('bmcu')
    if status['package_version'] != '1.0.6':
        raise RuntimeError(('This installer supports BMCU-Klipper 1.0.6.'))
    if (
        status['active_operations']
        or status['print_plan_open']
        or status['print_map_active']
        or status['prestaged']
    ):
        raise RuntimeError(
            ('Finish BMCU operations and the print plan before installing.')
        )
    devices = status['devices']
    if len(devices) != 1 or devices[0].get('name') != 'bmcu0':
        raise RuntimeError('This setup requires one BMCU named bmcu0.')
    for device in devices:
        if (
            not device['ready']
            or device['loaded_channels']
            or device['uncertain_channels']
        ):
            raise RuntimeError(
                ('Park all channels and make sure the BMCU is ready.')
            )
    if query_object('filament_switch_sensor bmcu_entry')['filament_detected']:
        raise RuntimeError(
            'The entry sensor must be clear before installation.'
        )


def transform_method(text, class_name, method, transform):
    tree = ast.parse(text)
    candidates = [
        n
        for c in tree.body
        if isinstance(c, ast.ClassDef) and c.name == class_name
        for n in c.body
        if isinstance(n, ast.FunctionDef) and n.name == method
    ]
    if len(candidates) != 1:
        raise RuntimeError('Incompatible method: %s.%s' % (class_name, method))
    node = candidates[0]
    lines = text.splitlines(keepends=True)
    fragment = ''.join(lines[node.lineno - 1 : node.end_lineno])
    updated = transform(fragment)
    lines[node.lineno - 1 : node.end_lineno] = [updated]
    result = ''.join(lines)
    compile(result, method, 'exec')
    return result


def patch_sensor(text):
    def change(fragment):
        method = ast.parse(textwrap.dedent(fragment)).body[0]
        parameters = [argument.arg for argument in method.args.args]
        expected_values = [
            'self',
            'dispositivo',
            'destino',
            'canal',
            'avanco',
            'velocidade',
        ]
        english_parameters = [
            'self',
            'device',
            'destination',
            'channel',
            'advance',
            'speed',
        ]
        new_condition = (
            "not sensor_gerenciado and not sensor.get('enabled', True)"
        )
        expected = ast.dump(ast.parse(new_condition, mode='eval').body)
        if parameters in (
            expected_values + ['sensor_gerenciado'],
            english_parameters + ['sensor_gerenciado'],
        ) and any(ast.dump(node) == expected for node in ast.walk(method)):
            return fragment
        signature = (
            r'def _capturar_extrusora_primeiro\(\s*self,'
            r'\s*dispositivo,\s*destino,\s*canal,\s*avanco,'
            r'\s*velocidade,?\s*\):'
        )
        condition = r"not\s+sensor\.get\(\s*['\"]enabled['\"]," r"\s*True\s*\)"
        if (
            parameters != expected_values
            or len(re.findall(signature, fragment)) != 1
            or len(re.findall(condition, fragment)) != 1
        ):
            raise RuntimeError(
                (
                    (
                        'The capture implementation changed; review '
                        'bmcu_medicao.py before '
                        'installation.'
                    )
                )
            )
        new_signature = (
            'def _capturar_extrusora_primeiro(\n'
            '            self, dispositivo, destino, canal, avanco,\n'
            '            velocidade, sensor_gerenciado=False):'
        )
        fragment = re.sub(
            signature, lambda _: new_signature, fragment, count=1
        )
        return re.sub(
            condition, lambda _: '(' + new_condition + ')', fragment, count=1
        )

    classes = [
        node.name
        for node in ast.parse(text).body
        if isinstance(node, ast.ClassDef)
        and node.name in ('ArrivalMeasurement', 'MedicaoChegada')
    ]
    if len(classes) != 1:
        raise RuntimeError('Incompatible arrival measurement module.')
    return transform_method(
        text, classes[0], '_capturar_extrusora_primeiro', change
    )


def patch_contract(text):
    tree = ast.parse(text)
    classes = [
        c.name
        for c in tree.body
        if isinstance(c, ast.ClassDef)
        and any(
            isinstance(n, ast.FunctionDef)
            and n.name == '_run_generic_contract_macro'
            for n in c.body
        )
    ]
    if len(classes) != 1:
        raise RuntimeError('Incompatible generic extruder contract.')

    def change(fragment):
        previous = '        self._run(script, printer_priority=True)'
        updated = (
            "        priority = command not in ('BMCU_PREPARAR_ENDER', "
            "'BMCU_LIBERAR_ENDER')\n"
            '        self._run(script, printer_priority=priority)'
        )
        if updated in fragment:
            return fragment
        legacy = updated.replace('priority =', 'prioridade =').replace(
            'printer_priority=priority)', 'printer_priority=prioridade)'
        )
        if legacy in fragment:
            return fragment.replace(legacy, updated)
        if fragment.count(previous) != 1:
            raise RuntimeError(
                (
                    (
                        'The BMCU contract changed; review endpoints.py '
                        'before installation.'
                    )
                )
            )
        return fragment.replace(previous, updated)

    return transform_method(
        text, classes[0], '_run_generic_contract_macro', change
    )


def patch_pre_capture(text):
    def change(fragment):
        previous = (
            '            else:\n                device.s'
            'et_motion(channel, protocol.MOTION_BEFORE_O'
            'N_USE)\n\n            if not generic_contra'
            "ct:\n                phase = 'BITE'"
        )
        updated = (
            '            elif (generic_contract and\n   '
            "               str(endpoint.get('toolhead_p"
            "repare_macro', '')).strip().upper() == 'BMC"
            "U_PREPARAR_ENDER'):\n                device"
            '.stop_all()\n                endpoint._ende'
            'r_load_context = (device.name, int(channel)'
            ')\n            else:\n                devic'
            'e.set_motion(channel, protocol.MOTION_BEFOR'
            'E_ON_USE)\n\n            if not generic_con'
            "tract:\n                phase = 'BITE'"
        )
        if updated in fragment:
            return fragment
        if fragment.count(previous) != 1:
            raise RuntimeError(
                (
                    (
                        'The native capture phase changed; review manager.py '
                        'before installation.'
                    )
                )
            )
        return fragment.replace(previous, updated)

    return transform_method(text, 'BMCUManager', 'load_channel', change)


def prepare(home):
    configurations = home / 'printer_data/config'
    extras = home / 'klipper/klippy/extras'
    root = configurations / 'printer.cfg'
    sensor = extras / 'bmcu_medicao.py'
    contracts = extras / 'bmcu_core/endpoints.py'
    manager = extras / 'bmcu_core/manager.py'
    module = extras / 'bmcu_integracao_ender.py'
    destination = configurations / 'bmcu-multicor-ender.cfg'
    for path in (root, contracts, manager):
        if not path.is_file():
            raise RuntimeError('File not found: ' + str(path))
    content = root.read_text()
    include_line = '[include bmcu-multicor-ender.cfg]'
    if not re.search(
        r'^\s*\[include\s+bmcu-multicor-ender\.cfg\]\s*$',
        content,
        re.M | re.I,
    ):
        marker = content.find('#*# <')
        if marker < 0:
            content = content.rstrip('\n') + '\n\n' + include_line + '\n'
        else:
            content = (
                content[:marker].rstrip('\n')
                + '\n\n'
                + include_line
                + '\n\n'
                + content[marker:]
            )
    compile(MODULE, 'bmcu_integracao_ender.py', 'exec')
    return {
        root.resolve(): content.encode(),
        sensor.resolve(): patch_sensor(
            sensor.read_text() if sensor.is_file() else MEASUREMENT
        ).encode(),
        contracts.resolve(): patch_contract(contracts.read_text()).encode(),
        manager.resolve(): patch_pre_capture(manager.read_text()).encode(),
        module.resolve(): MODULE.encode(),
        destination.resolve(): (
            destination.read_bytes()
            if destination.is_file()
            else CONFIGURATION.encode()
        ),
    }


def write_file(path, data, user, mode=0o644):
    path.parent.mkdir(parents=True, exist_ok=True)
    previous = path.stat() if path.exists() else None
    with tempfile.NamedTemporaryFile(
        dir=path.parent, delete=False
    ) as temporary:
        name = temporary.name
        temporary.write(data)
        temporary.flush()
        os.fsync(temporary.fileno())
    try:
        os.chmod(name, previous.st_mode & 0o777 if previous else mode)
        if os.geteuid() == 0:
            os.chown(
                name,
                previous.st_uid if previous else user.pw_uid,
                previous.st_gid if previous else user.pw_gid,
            )
        os.replace(name, path)
    finally:
        if os.path.exists(name):
            os.unlink(name)


def service(action):
    command = ['systemctl', action, 'klipper']
    if os.geteuid() != 0:
        command.insert(0, 'sudo')
    subprocess.run(command, check=True)


def wait_ready():
    deadline = time.monotonic() + 90
    last = None
    while time.monotonic() < deadline:
        try:
            status = query_object('webhooks')
            if status['state'] == 'ready':
                configuration = query_object('configfile').get('config', {})
                if 'bmcu_integracao_ender' in configuration:
                    return
            last = status.get('state_message', status['state'])
            if status['state'] in ('error', 'shutdown'):
                raise RuntimeError(last)
        except (OSError, KeyError, ValueError) as error:
            last = str(error)
        time.sleep(1)
    raise RuntimeError('Klipper did not become ready: %s' % last)


def main():
    name = os.environ.get('SUDO_USER') if os.geteuid() == 0 else None
    user = pwd.getpwnam(name) if name else pwd.getpwuid(os.getuid())
    home = Path(user.pw_dir)
    check_printer()
    changes = prepare(home)
    originals = {p: p.read_bytes() if p.exists() else None for p in changes}
    changes = {p: d for p, d in changes.items() if originals[p] != d}
    if not changes:
        print(('The integration is already installed.'))
        return
    backup = home / (
        'backup-multicor-' + datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    )
    backup.mkdir(mode=0o700)
    if os.geteuid() == 0:
        os.chown(backup, user.pw_uid, user.pw_gid)
    with tarfile.open(backup / 'configuracoes.tar.gz', 'w:gz') as tar:
        tar.add(home / 'printer_data/config', arcname='config')
    manifest = []
    for index, path in enumerate(changes):
        original = originals[path]
        copy_path = backup / ('arquivo-%d' % index)
        metadata = path.stat() if path.exists() else None
        if original is not None:
            shutil.copy2(path, copy_path)
        manifest.append(
            {
                'destino': str(path),
                'copia': str(copy_path) if original is not None else None,
                'uid': metadata.st_uid if metadata else user.pw_uid,
                'gid': metadata.st_gid if metadata else user.pw_gid,
                'modo': metadata.st_mode & 0o777 if metadata else 0o644,
            }
        )
    (backup / 'arquivos.json').write_text(json.dumps(manifest, indent=2))
    bmcu_config = query_object('configfile').get('config', {}).get('bmcu', {})
    state_path = bmcu_config.get(
        'state_file', str(home / 'printer_data/config/bmcu_state.json')
    )
    if state_path.startswith('~/'):
        state_path = str(home / state_path[2:])
    state_path = Path(state_path)
    if state_path.is_file():
        state_data = json.loads(state_path.read_text())
        endpoint = state_data.get('endpoints', {}).get('extruder')
        (backup / 'endpoint.json').write_text(
            json.dumps({'arquivo': str(state_path), 'endpoint': endpoint})
        )
    restorer = (
        'import json, os, subprocess\nfrom pathlib import Path\nbase = '
        "Path(__file__).resolve().parent\nsubprocess.run(['systemctl', "
        "'stop', 'klipper'], check=True)\nfor item in json.loads((base / "
        "'arquivos.json').read_text()):\n    destination = "
        "Path(item['destino'])\n    if item['copia'] is None:\n        "
        'destination.unlink(missing_ok=True)\n    else:\n        '
        "destination.write_bytes(Path(item['copia']).read_bytes())\n        "
        "os.chmod(destination, item['modo'])\n        os.chown(destination, "
        "item['uid'], item['gid'])\nif (base / 'endpoint.json').is_file():\n  "
        "  previous = json.loads((base / 'endpoint.json').read_text())\n    "
        "status = Path(previous['arquivo'])\n    if status.is_file() and "
        "previous['endpoint'] is not None:\n        data = "
        'json.loads(status.read_text())\n        current = '
        "data.get('endpoints', {}).get('extruder', {})\n        if "
        "current.get('toolhead_prepare_macro') == 'BMCU_PREPARAR_ENDER':\n    "
        "        data['endpoints']['extruder'] = previous['endpoint']\n       "
        '     metadata = status.stat()\n            temporary = '
        "status.with_name(status.name + '.restauracao')\n            "
        "temporary.write_text(json.dumps(data, separators=(',', ':'), "
        'allow_nan=False))\n            os.chmod(temporary, metadata.st_mode '
        '& 0o777)\n            os.chown(temporary, metadata.st_uid, '
        'metadata.st_gid)\n            os.replace(temporary, '
        "status)\nsubprocess.run(['systemctl', 'start', 'klipper'], "
        "check=True)\nprint('Previous files "
        "restored.')\n"
    )
    (backup / 'restaurar.py').write_text(restorer)
    print('Backup:', backup)
    service('stop')
    try:
        for path, data in changes.items():
            write_file(path, data, user)
        service('start')
        wait_ready()
    except Exception:
        service('stop')
        for path in changes:
            if originals[path] is None:
                path.unlink(missing_ok=True)
            else:
                write_file(path, originals[path], user)
        service('start')
        print(('Installation failed; previous files were restored.'))
        raise
    print('Integration installed; Klipper is ready.')
    print('For initial endpoint setup, review BMCU_CONFIGURAR_MULTICOR.')
    print(('PID, Z offset, mesh and extrusion calibration were preserved.'))
    print('To restore files: sudo python3 ' + str(backup / 'restaurar.py'))


if __name__ == '__main__':
    main()
