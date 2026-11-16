import argparse
import ast
import glob
import inspect
import json
import math
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

from host_transaction import HostTransaction, update_lock
import bmcu_runtime_lifecycle as runtime_lifecycle
from migrate_v2 import (
    CALLBACK_FIELDS, CALLBACKS, HEADER, OPTION, canonical_option,
    canonical_section,
    migrate_callback_source, migrate_config, migrate_state,
    restore_state_callbacks,
)

ROOT = Path(__file__).resolve().parents[2]
MODULE = (ROOT / 'integrations/bmcu/bmcu_ender_integration.py').read_text()
CONFIGURATION = (
    ROOT / ('config/ender3-v3-se-validada/bmcu-multicor-' 'ender.cfg')
).read_text()
MEASUREMENT = (ROOT / 'integrations/bmcu/bmcu_measurement.py').read_text()
CONTROLS = (
    ROOT / 'config/ender3-v3-se-validada/bmcu-controles.cfg'
).read_bytes()
PANEL_SETTINGS = (
    ROOT / 'config/ender3-v3-se-validada/bmcu-controles-ajustes.cfg'
).read_bytes()
TIP_SETTINGS = (
    ROOT / 'config/ender3-v3-se-validada/bmcu-formacao-ponta.cfg'
).read_bytes()
TRANSPORT_GUARD = (ROOT / 'integrations/bmcu/ender_transport.py').read_bytes()
CORTADOR = (ROOT / 'integrations/bmcu/bmcu_cortador_ender.py').read_bytes()
CUTTER_SCHEMA = (
    ROOT / 'integrations/bmcu/bmcu_cutter_settings.py'
).read_bytes()
ENGLISH_API = (ROOT / 'integrations/bmcu/bmcu_english_api.py').read_bytes()
PANEL_STATUS = (ROOT / 'integrations/bmcu/bmcu_ender_panel.py').read_bytes()
AJUSTES_CORTADOR = (
    ROOT / 'config/ender3-v3-se-validada/bmcu-cortador.cfg'
).read_bytes()


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
    if status['package_version'] != '1.1.2':
        raise RuntimeError(('This installer supports BMCU-Klipper 1.1.2.'))
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
        if device.get('firmware_compatible') is not True:
            raise RuntimeError(
                'Update BMCU firmware to 1.0.4 before installing.')
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
                        'bmcu_measurement.py before '
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
    text = migrate_callback_source(text)
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
            "        priority = command not in ('BMCU_PREPARE_ENDER', "
            "'BMCU_RELEASE_ENDER')\n"
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
    text = migrate_callback_source(text)
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
            "U_PREPARE_ENDER'):\n                device"
            '.stop_all()\n                endpoint._ende'
            'r_load_context = (device.name, int(channel)'
            ')\n            else:\n                devic'
            'e.set_motion(channel, protocol.MOTION_BEFOR'
            'E_ON_USE)\n\n            if not generic_con'
            "tract:\n                phase = 'BITE'"
        )
        previous_112 = (
            '                else:\n'
            '                    device.set_motion(channel, '
            'protocol.MOTION_BEFORE_ON_USE)\n\n'
            '            if not generic_contract:\n'
            "                phase = 'BITE'"
        )
        updated_112 = (
            '                elif (generic_contract and\n'
            "                      str(endpoint.get('toolhead_prepare_macro', "
            "'')).strip().upper() == 'BMCU_PREPARE_ENDER'):\n"
            '                    device.stop_all()\n'
            '                    endpoint._ender_load_context = '
            '(device.name, int(channel))\n'
            + previous_112
        )
        if updated_112 in fragment:
            return fragment
        if fragment.count(previous_112) == 1:
            return fragment.replace(previous_112, updated_112)
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


def patch_toolchange_lock(text):
    """Keep native channel-filtered phases valid for same-device Ender swaps."""
    def change(fragment):
        previous_lock = (
            '        multi_device_switch = (\n'
            '            source_device is not None and '
            'source_device.name != device.name)\n'
            '        if multi_device_switch:\n'
            '            self._lock_refill(\n'
            "                source_device, device, endpoint, 'TOOL_SWITCH %s' "
            '% operation_label,\n'
            '                source_channel=source_channel, '
            'replacement_channel=channel)\n'
            '        else:\n'
            "            self._lock(device, endpoint, 'LOAD %s' "
            '% operation_label, channel=channel)'
        )
        updated_lock = previous_lock.replace(
            '        if multi_device_switch:\n',
            '        ender_channel_switch = (\n'
            '            source_device is not None and\n'
            '            source_device.name == device.name and\n'
            '            source_channel != channel and\n'
            "            endpoint.driver == 'generic_single_extruder' and\n"
            "            str(endpoint.get('before_pullback_macro', ''))."
            "strip().upper() == 'BMCU_RELEASE_ENDER')\n"
            '        if multi_device_switch or ender_channel_switch:\n',
        )
        previous_unlock = (
            '            if multi_device_switch:\n'
            '                self._unlock_refill('
            'source_device, device, endpoint)\n'
            '            else:\n'
            '                self._unlock(device, endpoint)'
        )
        updated_unlock = previous_unlock.replace(
            'if multi_device_switch:',
            'if multi_device_switch or ender_channel_switch:',
        )
        if (fragment.count(updated_lock) == 1
                and fragment.count(updated_unlock) == 1
                and previous_lock not in fragment
                and previous_unlock not in fragment):
            return fragment
        if (fragment.count(previous_lock) != 1
                or fragment.count(previous_unlock) != 1
                or updated_lock in fragment or updated_unlock in fragment):
            raise RuntimeError(
                'The native tool-change lock changed; review manager.py '
                'before installation.'
            )
        return fragment.replace(previous_lock, updated_lock, 1).replace(
            previous_unlock, updated_unlock, 1)

    return transform_method(text, 'BMCUManager', 'load_channel', change)


def patch_pre_unload(text):
    text = migrate_callback_source(text)
    def change(fragment):
        previous = (
            '            device.set_motion(channel, '
            'protocol.MOTION_BEFORE_PULL_BACK)\n'
            "            context['before_pullback_started'] = True\n"
            '            self.reactor.pause(self.reactor.monotonic() + 0.2)'
        )
        updated = (
            "            ender_release = (endpoint.driver == "
            "'generic_single_extruder' and\n"
            "                str(endpoint.get('before_pullback_macro', ''))."
            "strip().upper() == 'BMCU_RELEASE_ENDER')\n"
            '            if not ender_release:\n'
            + textwrap.indent(previous, '    ')
            + '\n            else:\n'
            '                endpoint._ender_unload_context = (\n'
            '                    device.name, int(channel), '
            "device.status['session_id'])\n"
            '                device.stop_all()'
        )
        if updated in fragment:
            return fragment
        if fragment.count(previous) != 1:
            raise RuntimeError(
                'The native unload phase changed; review manager.py '
                'before installation.'
            )
        return fragment.replace(previous, updated, 1)

    return transform_method(text, 'BMCUManager', '_begin_unload_locked', change)


def patch_transport(text):
    original_import = 'from bmcu_core import protocol, transport'
    guarded_import = (
        original_import
        + '\nfrom bmcu_core.ender_transport import EnderTransportGuard'
    )
    original_class = '\ndef parse_args():'
    guarded_class = (
        '\nclass EnderTransportDaemon(EnderTransportGuard, TransportDaemon):'
        '\n    pass\n\n\ndef parse_args():'
    )
    for original, updated in (
        (original_import, guarded_import),
        (original_class, guarded_class),
        (
            'daemon = TransportDaemon(args)',
            'daemon = EnderTransportDaemon(args)',
        ),
    ):
        if updated in text:
            continue
        if text.count(original) != 1:
            raise RuntimeError(
                'The BMCU transport changed; review before installing.'
            )
        text = text.replace(original, updated, 1)
    compile(text, 'bmcu_transportd.py', 'exec')
    return text


def preparar_servo_cortador(caminho, pino):
    if not re.fullmatch(r'host:gpiochip(?:[0-9]|1[0-5])/gpio[0-9]{1,3}', pino):
        raise ValueError(
            'Use an explicit host MCU GPIO: host:gpiochipN/gpioO.'
        )
    if caminho.is_file():
        conteudo = caminho.read_text()
        existente = re.search(r'(?m)^\s*pin\s*:\s*(\S+)\s*$', conteudo)
        if existente is None or existente.group(1) != pino:
            raise ValueError(
                'The servo file already exists with a different pin; '
                'review it manually.'
            )
        return caminho.read_bytes()
    return (
        '[servo bmcu_cortador]\n'
        'pin: %s\n'
        'maximum_servo_angle: 180\n'
        'minimum_pulse_width: 0.001\n'
        'maximum_pulse_width: 0.002\n' % pino
    ).encode()


def validar_pino_cortador(pino, configuracao, modelo=None):
    if modelo is None:
        try:
            modelo = Path('/proc/device-tree/model').read_text()
        except OSError:
            modelo = ''
    if (
        re.search(r'orange\s*pi\s*zero\s*3', modelo, re.I)
        and pino.endswith('/gpio73')
    ):
        raise ValueError(
            'PC9/physical pin 7 is used by the Zero 3 PMIC interrupt. '
            'Check PC8/physical pin 15 and its GPIO mapping before installing.'
        )
    if 'mcu host' not in configuracao:
        raise ValueError('Configure the host MCU before installing the servo.')
    for secao, valores in configuracao.items():
        if secao == 'servo bmcu_cortador':
            continue
        for chave, valor in valores.items():
            if 'pin' in chave and str(valor).lstrip('^!~') == pino:
                raise ValueError(
                    'The servo pin is already reserved in %s.' % secao
                )


def corrigir_aquecimento_inicio(texto, nome, monocor=False):
    secao = re.search(
        r'(?ms)^\[gcode_macro '
        + re.escape(nome)
        + r'\][^\S\n]*\n.*?(?=^\[|\Z)',
        texto,
    )
    if secao is None:
        return texto
    trecho = secao.group()
    if not re.search(r'(?im)^[ \t]*gcode[ \t]*[:=]', trecho):
        return texto
    comando = 'BMCU_ENDER_MOVE_TO_PURGE'
    if comando in trecho:
        return texto
    padrao = r'(?m)^( +)M109 +S\{(?:nozzle_temp|bico)\}[^\S\n]*\n'
    aquecimento = re.search(padrao, trecho)
    if aquecimento is None:
        raise RuntimeError(
            'Manually review the heating sequence in macro %s.' % nome
        )
    retorno = ''
    if monocor:
        retorno = aquecimento[1] + 'G1 X5 Y20 F6000\n'
        if trecho[: aquecimento.start()].count(retorno) != 1:
            raise RuntimeError(
                'Manually review the initial position in macro %s.' % nome
            )
        trecho = trecho.replace(retorno, '', 1)
    trecho = re.sub(
        padrao,
        lambda linha: linha[1] + comando + '\n' + linha[0] + retorno,
        trecho,
        count=1,
    )
    return texto[: secao.start()] + trecho + texto[secao.end() :]


def migrate_active_configuration(configurations, changes):
    """Migrate the active include graph, leaving external shared files alone."""
    base = configurations.resolve()
    planned = {
        path.resolve(): data for path, data in changes.items()
        if path.resolve().is_relative_to(base)
    }
    visiting = set()
    visited = set()
    section_origins = {}
    option_origins = {}

    def visit(path):
        path = path.resolve()
        if path in visiting:
            raise ValueError('Recursive configuration include: ' + str(path))
        if path in visited:
            return
        visiting.add(path)
        original = planned.get(path)
        if original is None:
            if not path.is_file():
                raise ValueError('Missing configuration include: ' + str(path))
            original = path.read_bytes()
        text = original.decode('utf-8')
        # Inspect original spellings too: planned files may already have been
        # migrated. Preserve legitimate same-name overrides, but never merge an
        # independent v2 definition into a renamed legacy section.
        source = path.read_text() if path.is_file() else text
        section = None
        in_gcode = False
        for line in source.splitlines(keepends=True):
            header = HEADER.match(line)
            if header is None:
                if section is None or (in_gcode and line[:1].isspace()):
                    continue
                option = OPTION.match(line.rstrip('\r\n'))
                if option is None:
                    continue
                raw = option[2].lower()
                key = canonical_option(section, option[2]).lower()
                identity = (section.lower(), key)
                previous = option_origins.get(identity)
                if previous and previous[0] != raw:
                    raise ValueError(
                        'Configuration option collision: %s in %s and %s'
                        % (key, previous[1], path)
                    )
                option_origins[identity] = (raw, path)
                in_gcode = raw == 'gcode'
                continue
            section = canonical_section(header[2])
            in_gcode = False
            if header[2].startswith('include '):
                section = None
                continue
            raw = header[2].lower()
            identity = section.lower()
            previous = section_origins.get(identity)
            if previous and previous[0] != raw:
                raise ValueError(
                    'Configuration section collision: %s in %s and %s'
                    % (identity, previous[1], path)
                )
            section_origins[identity] = (raw, path)
        migrated = migrate_config(text).encode('utf-8')
        if path.is_relative_to(base):
            changes[path] = migrated
        elif migrated != original:
            raise ValueError(
                'An external include requires manual migration: ' + str(path)
            )
        for match in re.finditer(r'(?m)^\s*\[include\s+([^]\r\n]+)\]', text):
            pattern = match[1].strip()
            if pattern.startswith('~/'):
                pattern = str(configurations.parent.parent / pattern[2:])
            full_pattern = str(path.parent / pattern)
            candidates = {
                Path(name).resolve() for name in glob.glob(full_pattern)
            }
            candidates.update(
                name for name in planned if name.match(full_pattern)
            )
            if not candidates and not glob.has_magic(pattern):
                raise ValueError('Missing configuration include: ' + pattern)
            for candidate in sorted(candidates):
                visit(candidate)
        visiting.remove(path)
        visited.add(path)

    visit(configurations / 'printer.cfg')
    return visited


def migrate_display_language(root, changes, visited, active_language=None):
    """Move included preferences to the autosave-capable top-level config."""
    preferences = []
    for path in visited:
        text = changes.get(path, path.read_bytes() if path.is_file() else b'')
        section = None
        for index, line in enumerate(text.decode().splitlines(keepends=True)):
            header = HEADER.match(line)
            if header:
                section = header[2]
                continue
            option = OPTION.match(line.rstrip('\r\n'))
            if (section == 'e3v3se_display' and option
                    and option[2].lower() == 'language'):
                value = re.split('[#;]', option[4], maxsplit=1)[0].strip()
                preferences.append((path, index, value))
    included = [item for item in preferences if item[0] != root]
    if not included:
        return
    if any(path not in changes for path, _, _ in included):
        raise ValueError(
            'Move external display language into printer.cfg first.')
    values = {value for _, _, value in preferences}
    if active_language is None and len(values) != 1:
        raise ValueError(
            'Conflicting display language preferences need review.')
    language = active_language or values.pop()
    if language not in ('en', 'pt_BR'):
        raise ValueError('Unsupported display language: ' + language)
    for path in {item[0] for item in preferences}:
        lines = changes[path].decode().splitlines(keepends=True)
        for _, index, _ in (item for item in preferences if item[0] == path):
            lines[index] = '# Moved to printer.cfg: ' + lines[index]
        changes[path] = ''.join(lines).encode()
    content = changes[root].decode()
    marker = content.find('#*# <')
    marker = len(content) if marker < 0 else marker
    preference = '\n\n[e3v3se_display]\nlanguage: %s\n\n' % language
    changes[root] = (
        content[:marker].rstrip('\r\n') + preference + content[marker:]
    ).encode()


def prepare(
    home, purge_mm=None, pullback_speed=None, pino_servo_cortador=None,
    state_path=None, display_language=None,
):
    configurations = home / 'printer_data/config'
    extras = home / 'klipper/klippy/extras'
    root = configurations / 'printer.cfg'
    sensor = extras / 'bmcu_measurement.py'
    contracts = extras / 'bmcu_core/endpoints.py'
    manager = extras / 'bmcu_core/manager.py'
    module = extras / 'bmcu_ender_integration.py'
    daemon = configurations / 'bmcu/runtime/scripts/bmcu_transportd.py'
    destination = configurations / 'bmcu-multicor-ender.cfg'
    arquivo_servo = configurations / 'bmcu-cortador-servo.cfg'
    dados_servo = (
        preparar_servo_cortador(arquivo_servo, pino_servo_cortador)
        if pino_servo_cortador is not None
        else None
    )
    for path in (root, contracts, manager, daemon):
        if not path.is_file():
            raise RuntimeError('File not found: ' + str(path))
    content = migrate_config(root.read_text())
    includes = [
        'bmcu-multicor-ender.cfg',
        'bmcu-controles.cfg',
        'bmcu-formacao-ponta.cfg',
        'bmcu-cortador.cfg',
    ]
    if dados_servo is not None or arquivo_servo.is_file():
        includes.append('bmcu-cortador-servo.cfg')
    for filename in includes:
        include_line = '[include %s]' % filename
        if not re.search(
            r'^\s*\[include\s+' + re.escape(filename) + r'\]\s*$',
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
    settings = configurations / 'bmcu-controles-ajustes.cfg'
    tip_settings = configurations / 'bmcu-formacao-ponta.cfg'
    ajustes_cortador = configurations / 'bmcu-cortador.cfg'
    sequence = migrate_config(
        destination.read_text() if destination.is_file() else CONFIGURATION
    )
    sequence = corrigir_aquecimento_inicio(sequence, 'ENDER_START_MULTICOLOR')
    for key, value in (
        ('purge_mm', purge_mm), ('retract_feedrate', pullback_speed),
    ):
        if value is None:
            continue
        if not math.isfinite(value) or value <= 0:
            raise ValueError('%s must be a finite positive number.' % key)
        section = re.search(
            r'(?ms)^\[bmcu_ender_integration\][^\S\n]*\n.*?(?=^\[|\Z)',
            sequence,
        )
        if section is None:
            raise RuntimeError('The Ender integration section was not found.')
        text = section.group()
        pattern = r'(?m)^\s*' + key + r'\s*:[^\n]*$'
        replacement = '%s: %g' % (key, value)
        if re.search(pattern, text):
            text = re.sub(pattern, replacement, text)
        else:
            text = text.rstrip() + '\n' + replacement + '\n\n'
        sequence = (
            sequence[: section.start()] + text + sequence[section.end() :]
        )
    compile(MODULE, 'bmcu_ender_integration.py', 'exec')
    compile(CORTADOR, 'bmcu_cortador_ender.py', 'exec')
    changes = {
        root.resolve(): content.encode(),
        sensor.resolve(): patch_sensor(MEASUREMENT).encode(),
        contracts.resolve(): patch_contract(contracts.read_text()).encode(),
        manager.resolve(): patch_pre_unload(
            patch_toolchange_lock(patch_pre_capture(manager.read_text()))
        ).encode(),
        module.resolve(): MODULE.encode(),
        (extras / 'bmcu_cortador_ender.py').resolve(): CORTADOR,
        (extras / 'bmcu_cutter_settings.py').resolve(): CUTTER_SCHEMA,
        (extras / 'bmcu_english_api.py').resolve(): ENGLISH_API,
        (extras / 'bmcu_ender_panel.py').resolve(): PANEL_STATUS,
        (extras / 'bmcu_core/ender_transport.py').resolve(): TRANSPORT_GUARD,
        daemon.resolve(): patch_transport(daemon.read_text()).encode(),
        (configurations / 'bmcu-controles.cfg').resolve(): CONTROLS,
        settings.resolve(): (
            settings.read_bytes() if settings.is_file() else PANEL_SETTINGS
        ),
        tip_settings.resolve(): (
            tip_settings.read_bytes()
            if tip_settings.is_file()
            else TIP_SETTINGS
        ),
        destination.resolve(): sequence.encode(),
        ajustes_cortador.resolve(): (
            ajustes_cortador.read_bytes()
            if ajustes_cortador.is_file()
            else AJUSTES_CORTADOR
        ),
    }
    if dados_servo is not None:
        changes[arquivo_servo.resolve()] = dados_servo
    impressao = configurations / 'impressao.cfg'
    if impressao.is_file():
        changes[impressao.resolve()] = corrigir_aquecimento_inicio(
            migrate_config(impressao.read_text()),
            'ENDER_START_PRINT', monocor=True,
        ).encode()
    visited = migrate_active_configuration(configurations, changes)
    for path in sorted(visited.intersection(changes)):
        changes[path] = corrigir_aquecimento_inicio(
            changes[path].decode(), 'ENDER_START_PRINT', monocor=True,
        ).encode()
    migrate_display_language(root.resolve(), changes, visited, display_language)
    state_path = Path(
        state_path or configurations / 'bmcu_state.json'
    ).resolve()
    if state_path.is_file():
        original_state = state_path.read_bytes()
        state = json.loads(original_state)
        migrated_state = migrate_state(state)
        changes[state_path] = (
            original_state if state == migrated_state
            else (json.dumps(migrated_state, indent=2, allow_nan=False) + '\n')
            .encode('utf-8')
        )
    return changes


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


def service(action, unit='klipper'):
    command = ['systemctl', action, unit]
    if os.geteuid() != 0:
        command.insert(0, 'sudo')
    subprocess.run(command, check=True)


def wait_ready(required_section='bmcu_ender_integration'):
    deadline = time.monotonic() + 90
    last = None
    while time.monotonic() < deadline:
        try:
            status = query_object('webhooks')
            if status['state'] == 'ready':
                if required_section is None:
                    return
                configuration = query_object('configfile').get('config', {})
                if required_section in configuration:
                    return
            last = status.get('state_message', status['state'])
            if status['state'] in ('error', 'shutdown'):
                raise RuntimeError(last)
        except (OSError, KeyError, ValueError) as error:
            last = str(error)
        time.sleep(1)
    raise RuntimeError('Klipper did not become ready: %s' % last)


RESTORE_HELPERS = '\n'.join(inspect.getsource(function) for function in (
    restore_state_callbacks, write_file, query_object, check_printer,
    wait_ready,
))


def create_backup(
    home, changes, originals, user, state_path, host=None,
    previous_section=None,
):
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
                'callback_state': path.resolve() == state_path,
            }
        )
    (backup / 'arquivos.json').write_text(json.dumps(manifest, indent=2))
    if state_path.is_file():
        state_data = json.loads(state_path.read_text())
        endpoint = state_data.get('endpoints', {}).get('extruder')
        (backup / 'endpoint.json').write_text(
            json.dumps({
                'arquivo': str(state_path), 'endpoint': endpoint,
                'endpoints': state_data.get('endpoints', {}),
            })
        )
    if host is not None:
        record = dict(host.record(), previous_section=previous_section)
        (backup / 'host.json').write_text(json.dumps(record, indent=2))
    shutil.copy2(Path(__file__).with_name('host_transaction.py'),
                 backup / 'host_transaction.py')
    shutil.copy2(Path(__file__).with_name('bmcu_runtime_lifecycle.py'),
                 backup / 'bmcu_runtime_lifecycle.py')
    restorer = (
        'import copy, importlib.util, json, os, subprocess, tempfile, time\n'
        'import bmcu_runtime_lifecycle as runtime_lifecycle\n'
        'from pathlib import Path\n'
        'from urllib.request import urlopen\n'
        + 'from types import SimpleNamespace\n'
        + 'CALLBACK_FIELDS = ' + repr(CALLBACK_FIELDS) + '\n'
        + 'CALLBACKS = ' + repr(CALLBACKS) + '\n'
        + 'UPDATE_LOCK = ' + repr(str(
            home / 'printer_data/ender3-v2-update.lock')) + '\n'
        + 'UPDATE_OWNER = ' + repr((user.pw_uid, user.pw_gid)) + '\n'
        + 'HOME = ' + repr(str(home)) + '\n'
        + RESTORE_HELPERS + '\n'
    )
    body = textwrap.dedent('''\
            base = Path(__file__).resolve().parent
            if subprocess.run(['systemctl', 'is-active', '--quiet',
                               'klipper']).returncode == 0:
                check_printer()
            transport = runtime_lifecycle.RuntimeTransport(Path(HOME))
            subprocess.run(['systemctl', 'stop', 'klipper'], check=True)
            transport.stop()
            host_path = base / 'host.json'
            host = (json.loads(host_path.read_text())
                    if host_path.is_file() else None)
            if host:
                helper.restore_checkout(host)
            for item in json.loads((base / 'arquivos.json').read_text()):
                if item.get('callback_state'):
                    continue
                destination = Path(item['destino'])
                if host and destination.is_relative_to(Path(host['active'])):
                    continue
                if item['copia'] is None:
                    destination.unlink(missing_ok=True)
                else:
                    user = SimpleNamespace(
                        pw_uid=item['uid'], pw_gid=item['gid'])
                    write_file(destination, Path(item['copia']).read_bytes(),
                               user, item['modo'])
                    os.chmod(destination, item['modo'])
                    os.chown(destination, item['uid'], item['gid'])
            if (base / 'endpoint.json').is_file():
                previous = json.loads((base / 'endpoint.json').read_text())
                status = Path(previous['arquivo'])
                if status.is_file():
                    data = json.loads(status.read_text())
                    restored = restore_state_callbacks(data, {
                        'endpoints': previous['endpoints'],
                    })
                    if restored != data:
                        encoded = json.dumps(restored, allow_nan=False).encode()
                        metadata = status.stat()
                        user = SimpleNamespace(pw_uid=metadata.st_uid,
                                               pw_gid=metadata.st_gid)
                        write_file(status, encoded, user)
            if host:
                try:
                    transport.start()
                    subprocess.run(['systemctl', 'start', 'klipper'],
                                   check=True)
                    wait_ready(required_section=host['previous_section'])
                except Exception:
                    subprocess.run(['systemctl', 'stop', 'klipper'], check=True)
                    transport.stop()
                    raise
                print('Previous host, display and files restored; '
                      'Klipper is ready.')
            else:
                print('Previous files restored. Services remain stopped.')
                print('The host checkout was not preserved '
                      'by this installation.')
                print('Restore a matching host/display '
                      'before starting services.')
            ''')
    restorer += textwrap.dedent('''\
        base = Path(__file__).resolve().parent
        spec = importlib.util.spec_from_file_location(
            'saved_host_transaction', base / 'host_transaction.py')
        helper = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(helper)
        with helper.update_lock(Path(UPDATE_LOCK), UPDATE_OWNER):
        ''') + textwrap.indent(body, '    ')
    compile(restorer, '<generated rollback>', 'exec')
    (backup / 'restaurar.py').write_text(restorer)
    return backup


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--purge-mm', type=float)
    parser.add_argument('--pullback-speed', type=float)
    parser.add_argument('--cutter-servo-pin', dest='pino_servo_cortador')
    parser.add_argument('--host-source', type=Path)
    args = parser.parse_args()
    name = os.environ.get('SUDO_USER') if os.geteuid() == 0 else None
    user = pwd.getpwnam(name) if name else pwd.getpwuid(os.getuid())
    home = Path(user.pw_dir)
    with update_lock(home / 'printer_data/ender3-v2-update.lock',
                     (user.pw_uid, user.pw_gid)):
        return install(args, user)


def install(args, user):
    home = Path(user.pw_dir)
    check_printer()
    if args.pino_servo_cortador is not None:
        validar_pino_cortador(
            args.pino_servo_cortador,
            query_object('configfile').get('config', {}),
        )
    active_config = query_object('configfile').get('config', {})
    bmcu_config = active_config.get('bmcu', {})
    display_language = active_config.get('e3v3se_display', {}).get('language')
    previous_section = next((name for name in (
        'bmcu_ender_integration', 'bmcu_integracao_ender',
    ) if name in active_config), None)
    state_path = bmcu_config.get(
        'state_file', str(home / 'printer_data/config/bmcu_state.json')
    )
    if state_path.startswith('~/'):
        state_path = str(home / state_path[2:])
    state_path = Path(state_path)
    if not state_path.is_absolute():
        raise ValueError('Use an absolute BMCU state_file path for migration.')
    state_path = state_path.resolve()

    def preparation():
        changes = prepare(
            home, args.purge_mm, args.pullback_speed, args.pino_servo_cortador,
            state_path=state_path, display_language=display_language,
        )
        originals = {p: p.read_bytes() if p.exists() else None for p in changes}
        changed = {p: d for p, d in changes.items() if originals[p] != d}
        return changed, originals

    changes, _ = preparation()
    host = None
    if args.host_source is not None:
        if args.host_source.resolve() != ROOT.resolve():
            raise ValueError('Run the installer from the selected host source.')
        work = home / ('backup-host-update-'
                       + datetime.now().strftime('%Y%m%d-%H%M%S-%f'))
        host = HostTransaction(args.host_source, home / 'klipper', work)
        try:
            host.prepare()
        except Exception:
            print('Any staged files were retained for review under:', work)
            raise
    if not changes and host is None:
        print('The integration is already installed.')
        return
    transport = runtime_lifecycle.RuntimeTransport(home)
    transport.check_active()
    check_printer()
    service('stop')
    # The live process may save state while stopping. Take the authoritative
    # snapshot only after it has exited, before activating any migrated file.
    try:
        transport.stop()
        changes, originals = preparation()
        backup = create_backup(
            home, changes, originals, user, state_path, host, previous_section,
        )
    except Exception:
        print('No migrated files activated; '
              'services remain stopped for review.')
        raise
    print('Backup:', backup)
    try:
        if host is not None:
            host.activate()
        for path, data in changes.items():
            write_file(path, data, user)
        transport.start()
        service('start')
        wait_ready()
    except Exception:
        service('stop')
        transport.stop()
        if host is not None:
            host.rollback()
        for path in changes:
            if host is not None and path.is_relative_to(home / 'klipper'):
                continue
            if originals[path] is None:
                path.unlink(missing_ok=True)
            elif path == state_path:
                current = json.loads(path.read_bytes())
                previous = json.loads(originals[path])
                restored = restore_state_callbacks(current, previous)
                data = (
                    originals[path] if restored == previous
                    else json.dumps(restored, allow_nan=False).encode()
                )
                write_file(path, data, user)
            else:
                write_file(path, originals[path], user)
        if host is not None:
            try:
                transport.start()
                service('start')
                wait_ready(required_section=previous_section)
            except Exception:
                service('stop')
                transport.stop()
                print('Previous host/files restored; services remain stopped.')
                raise
            print('Upgrade failed; previous host/display restored and ready.')
        else:
            print('Installation failed; integration files were restored.')
            print('Services remain stopped. '
                  'The host checkout was not preserved.')
            print('Restore a matching host/display before starting services.')
        raise
    print('Integration installed; Klipper is ready.')
    print('For initial endpoint setup, review BMCU_SETUP_MULTICOLOR.')
    print(('PID, Z offset, mesh and extrusion calibration were preserved.'))
    print('To restore files: sudo python3 ' + str(backup / 'restaurar.py'))


if __name__ == '__main__':
    main()
