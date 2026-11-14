"""Pure, conservative v1-to-v2 configuration and callback migration.

This module never writes files or executes G-code. The installer owns backup,
validation, activation and rollback. Unknown user data is retained verbatim.
"""

import ast
import copy
import configparser
import io
import importlib.util
from pathlib import Path
import re
import tokenize


COMMANDS = {
    'BMCU_ENDER_CARREGAR': 'BMCU_ENDER_LOAD',
    'BMCU_ENDER_LIBERAR': 'BMCU_ENDER_UNLOAD',
    'BMCU_TROCAR_ENDER': 'BMCU_CHANGE_TOOL_ENDER',
    'BMCU_CONFIRMAR_CAMINHO_LIVRE': 'BMCU_CONFIRM_CLEAR_PATH',
    'BMCU_ENDER_TESTAR_CORTADOR': 'BMCU_ENDER_TEST_CUTTER',
    'BMCU_ENDER_RESETAR': 'BMCU_ENDER_RESET',
    'BMCU_ENDER_POSICIONAR_PURGA': 'BMCU_ENDER_MOVE_TO_PURGE',
    'BMCU_MEDIR_CHEGADA': 'BMCU_MEASURE_ARRIVAL',
    'BMCU_TESTAR_EXTRUSAO': 'BMCU_TEST_EXTRUSION',
    'BMCU_LIMITES_EXTRUSAO': 'BMCU_EXTRUSION_LIMITS',
    'BMCU_RETIRAR_ATE_SWITCH': 'BMCU_UNLOAD_TO_SENSOR',
    'BMCU_PREPARAR_ENDER': 'BMCU_PREPARE_ENDER',
    'BMCU_LIBERAR_ENDER': 'BMCU_RELEASE_ENDER',
    'BMCU_CONFIGURAR_MULTICOR': 'BMCU_SETUP_MULTICOLOR',
    'ENDER_INICIAR_MULTICOR': 'ENDER_START_MULTICOLOR',
    'ENDER_FINALIZAR_MULTICOR': 'ENDER_END_MULTICOLOR',
    'ENDER_INICIAR_IMPRESSAO': 'ENDER_START_PRINT',
    'ENDER_FINALIZAR_IMPRESSAO': 'ENDER_END_PRINT',
    '_BMCU_ENDER_CORTADOR_AJUSTES': '_BMCU_ENDER_CUTTER_SETTINGS',
    'CORTADOR_SERVO': 'CUTTER_SERVO',
    'TESTAR_CORTADOR': 'TEST_CUTTER',
    'PRE_AQUECER_PLA': 'PREHEAT_PLA',
    'PRE_AQUECER_PETG': 'PREHEAT_PETG',
    'PRE_AQUECER_TPU': 'PREHEAT_TPU',
    'RETIRAR_FILAMENTO': 'UNLOAD_ACTIVE_CHANNEL',
    **{'CARREGAR_CANAL_%d' % n: 'LOAD_CHANNEL_%d' % n for n in range(1, 5)},
}

SECTIONS = {
    'bmcu_integracao_ender': 'bmcu_ender_integration',
    'bmcu_medicao': 'bmcu_measurement',
}

INTEGRATION_SETTINGS = {
    'dispositivo': 'device',
    'destino': 'destination',
    'captura_mm': 'capture_mm',
    'captura_f': 'capture_feedrate',
    'avanco_bico_mm': 'nozzle_feed_mm',
    'purga_mm': 'purge_mm',
    'avanco_f': 'feed_feedrate',
    'recuo_f': 'retract_feedrate',
    'limite_buffer_recuo': 'unload_buffer_limit',
    'retomar_buffer_recuo': 'unload_buffer_resume',
    'tempo_buffer_recuo': 'unload_buffer_timeout',
    'tempo_sem_recuo': 'unload_progress_timeout',
    'passo_recuo_mm': 'unload_step_mm',
    'folga_recuo_mm': 'unload_slack_mm',
    'recuo_apos_switch_mm': 'pullback_after_sensor_mm',
    'tempo_switch': 'sensor_timeout',
    'x_purga': 'purge_x',
    'x_limpeza': 'wipe_x',
    'y_purga': 'purge_y',
    'alivio_z': 'z_lift',
    'limpezas': 'wipes',
}

_SCHEMA_PATH = (
    Path(__file__).resolve().parents[2]
    / 'integrations/bmcu/bmcu_cutter_settings.py'
)
_SCHEMA_SPEC = importlib.util.spec_from_file_location(
    'cutter_schema', _SCHEMA_PATH,
)
_SCHEMA = importlib.util.module_from_spec(_SCHEMA_SPEC)
_SCHEMA_SPEC.loader.exec_module(_SCHEMA)
CUTTER_SETTINGS = _SCHEMA.LEGACY_TO_PUBLIC
_API_SPEC = importlib.util.spec_from_file_location(
    'english_api_schema', _SCHEMA_PATH.with_name('bmcu_english_api.py')
)
_API_SCHEMA = importlib.util.module_from_spec(_API_SPEC)
_API_SPEC.loader.exec_module(_API_SCHEMA)
PARAMETERS = _API_SCHEMA.PARAMETERS
STATUS_FIELDS = {
    **_API_SCHEMA.STATUS_FIELDS, **_API_SCHEMA.DIAGNOSTIC_FIELDS,
}
LEGACY_DIAGNOSTIC_ENUM = re.compile(
    r'([\x22\x27])(?:'
    + '|'.join(re.escape(value) for value in _API_SCHEMA.DIAGNOSTIC_VALUES)
    + r')\1'
)

CALLBACK_FIELDS = ('toolhead_prepare_macro', 'before_pullback_macro')
CALLBACKS = {
    name: COMMANDS[name]
    for name in ('BMCU_PREPARAR_ENDER', 'BMCU_LIBERAR_ENDER')
}
HEADER = re.compile(r'^(\s*)\[([^]\r\n]+)\]([^\r\n]*)(\r?\n)?$')
OPTION = re.compile(r'^(\s*)([A-Za-z_][A-Za-z_0-9]*)(\s*[:=])(.*)$')
TEMPLATE_STRING = re.compile(r'"(?:\\.|[^"\\])*"|\x27(?:\\.|[^\x27\\])*\x27')


def canonical_section(name):
    if name.lower().startswith('gcode_macro '):
        _, command = name.split(None, 1)
        return 'gcode_macro ' + COMMANDS.get(command.upper(), command)
    return SECTIONS.get(name.lower(), name)


def canonical_option(section, key):
    if section.lower() == 'bmcu_ender_integration':
        return INTEGRATION_SETTINGS.get(key.lower(), key)
    if section.upper() == 'GCODE_MACRO _BMCU_ENDER_CUTTER_SETTINGS':
        if key.lower().startswith('variable_'):
            field = key.lower().removeprefix('variable_')
            return 'variable_' + CUTTER_SETTINGS.get(field, field)
    return key


def _own_parameters(section):
    return section.upper() in {
        'GCODE_MACRO ' + name for name in COMMANDS.values()
    } | {'GCODE_MACRO RESET_BMCU'}


def _status_fields(section):
    if section.upper() == 'GCODE_MACRO _BMCU_ENDER_CUTTER_SETTINGS':
        return CUTTER_SETTINGS
    if section.lower() == 'bmcu_ender_integration':
        return STATUS_FIELDS
    return None


def _template_references(line, migrate_parameters, aliases):
    def expression(match):
        text = match[0]
        dotted_sections = '|'.join(re.escape(name) for name in
                                   (*SECTIONS, *SECTIONS.values()))
        if re.search(r'\bprinter\s*\.\s*(?:' + dotted_sections + r')\b',
                     TEMPLATE_STRING.sub('', text)):
            raise ValueError('Dotted integration status requires manual review '
                             'before v2 migration.')
        if migrate_parameters:
            for old, new in PARAMETERS.items():
                text = re.sub(
                    r'\bparams\.' + old + r'\b', 'params.' + new,
                    text, flags=re.I,
                )
                text = re.sub(
                    r'(\bparams(?:\s*\[|\.get\()\s*)([\x22\x27])'
                    + old + r'\2',
                    lambda m: m[1] + m[2] + new + m[2], text, flags=re.I,
                )
                text = re.sub(
                    r'([\x22\x27])' + old
                    + r'\1(?=\s+(?:not\s+)?in\s+params\b)',
                    lambda m: m[1] + new + m[1], text, flags=re.I,
                )
        reference = r'\bprinter\s*\[\s*([\x22\x27])([^\x22\x27]+)\1\s*\]'
        text = re.sub(
            reference,
            lambda m: 'printer[' + m[1] + canonical_section(m[2]) + m[1] + ']',
            text,
        )
        assignment = re.search(r'\bset\s+(\w+)\s*=\s*', text)
        rhs = text[assignment.end():] if assignment else text
        receiver = re.match(reference, rhs) if assignment else None
        fields = _status_fields(receiver[2]) if receiver else None
        simple_status = (fields is STATUS_FIELDS
                         and rhs[receiver.end():].strip() in ('%}', '-%}'))
        status_names = {name for name, schema in aliases.items()
                        if schema is STATUS_FIELDS}
        names = set(re.findall(r'\b\w+\b', TEMPLATE_STRING.sub('', rhs)))
        status_object = any(_status_fields(item[2]) is STATUS_FIELDS
                            for item in re.finditer(reference, rhs))
        if re.match(r'\{%-?\s*set\b', text) and not simple_status and (
                status_object or names.intersection(status_names)
                or (assignment and assignment[1] in status_names)):
            # Do not infer the type or lifetime of derived, copied or rebound
            # aliases. A later condition could otherwise silently stop firing.
            raise ValueError('Status alias expression requires manual review '
                             'before v2 migration.')
        if assignment:
            if fields:
                aliases[assignment[1]] = fields
            else:
                aliases.pop(assignment[1], None)
        receivers = dict(aliases)
        for item in re.finditer(reference, text):
            fields = _status_fields(item[2])
            if fields:
                receivers[item[0]] = fields
        for receiver, fields in receivers.items():
            if (fields is STATUS_FIELDS and LEGACY_DIAGNOSTIC_ENUM.search(text)
                    and re.search(r'(?<!\w)' + re.escape(receiver)
                                  + r'(?!\w)', text)):
                # Do not silently change the meaning of custom conditions or
                # rewrite unrelated literals. Require an explicit human review.
                raise ValueError(
                    'Legacy diagnostic enum expression requires manual '
                    'review before v2 migration.')
            def attributes(match):
                value = match[0]
                for old, new in fields.items():
                    value = re.sub(r'\.' + old + r'\b', '.' + new, value)
                    value = re.sub(
                        r'\[([\x22\x27])' + old + r'\1\]',
                        lambda m: '[' + m[1] + new + m[1] + ']', value,
                    )
                return value
            text = re.sub(
                r'(?<!\w)' + re.escape(receiver)
                + r'(?:\.\w+|\[[\x22\x27][^\x22\x27]+[\x22\x27]\])*',
                attributes, text,
            )
        return text

    return re.sub(r'\{%.*?%\}|\{[^{}]*\}', expression, line)


def _argument_spans(body):
    start = None
    quote = None
    escaped = False
    depth = 0
    for index, char in enumerate(body):
        if start is None:
            if char.isspace():
                continue
            start = index
        if escaped:
            escaped = False
        elif char == '\\':
            escaped = True
        elif quote:
            if char == quote:
                quote = None
        elif char in '\x22\x27':
            quote = char
        elif char == '{':
            depth += 1
        elif char == '}':
            depth -= 1
        elif char.isspace() and not depth:
            yield start, index
            start = None
    if quote or depth:
        raise ValueError('Review incomplete G-code arguments before migration.')
    if start is not None:
        yield start, len(body)


def _migrate_arguments(body, command):
    names = command in COMMANDS or command in COMMANDS.values() or command in (
        'BMCU_ENDER_SERVO', 'BMCU_ENDER_MANUAL', 'RESET_BMCU',
    )
    if not names and command not in ('BMCU_SET_ENDPOINT', 'SET_GCODE_VARIABLE'):
        return body
    parts, seen = [], set()
    cursor = 0
    arguments = []
    for start, end in _argument_spans(body):
        token = body[start:end]
        key, separator, value = token.partition('=')
        if separator and key.isidentifier():
            key = PARAMETERS.get(key.upper(), key) if names else key
            if key.upper() in seen:
                raise ValueError('G-code parameter collision: ' + key)
            seen.add(key.upper())
            arguments.append((start, end, key, value))
    macro = next((value.strip('\x22\x27').upper()
                  for _, _, key, value in arguments
                  if key.upper() == 'MACRO'), '')
    for start, end, key, value in arguments:
        quote = value[:1] if value[:1] in ('\x22', '\x27') else ''
        literal = value[1:-1] if quote and value.endswith(quote) else value
        if names and key.upper() == 'POSITION':
            literal = {
                'RECOLHIDO': 'RETRACTED', 'ACIONADO': 'DEPLOYED',
            }.get(literal.upper(), literal)
        elif command == 'BMCU_SET_ENDPOINT' and key.lower() in CALLBACK_FIELDS:
            literal = CALLBACKS.get(literal.upper(), literal)
        elif command == 'SET_GCODE_VARIABLE':
            if key.upper() == 'MACRO':
                literal = COMMANDS.get(literal.upper(), literal)
            elif key.upper() == 'VARIABLE' and COMMANDS.get(macro, macro) == (
                '_BMCU_ENDER_CUTTER_SETTINGS'
            ):
                literal = CUTTER_SETTINGS.get(literal.lower(), literal)
        parts.append(body[cursor:start] + key + '=' + quote + literal + quote)
        cursor = end
    parts.append(body[cursor:])
    return ''.join(parts)


def _gcode_line(line, migrate_parameters=False, aliases=None):
    if line.lstrip().startswith(('#', ';')):
        return line
    if aliases is None:
        aliases = {}
    ending = line[len(line.rstrip('\r\n')):]
    content = line.rstrip('\r\n')
    comment = ''
    # Config and G-code comments are not executable identifiers. Ignore markers
    # inside Jinja expressions, which may contain strings used by the template.
    depth = 0
    for index, char in enumerate(content):
        if char == '{':
            depth += 1
        elif char == '}':
            depth = max(0, depth - 1)
        elif char in '#;' and not depth:
            content, comment = content[:index], content[index:]
            break
    content = _template_references(content, migrate_parameters, aliases)
    match = re.match(r'^(\s*)([A-Za-z_][A-Za-z_0-9]*)(.*)$', content)
    if not match:
        return content + comment + ending
    command = match[2].upper()
    if command in ('M117', 'M118', 'RESPOND'):
        return content + comment + ending
    body = _migrate_arguments(match[3], command)
    return match[1] + COMMANDS.get(command, match[2]) + body + comment + ending


def _remove_default_display_shortcuts(text):
    def shortcut(match):
        parser = configparser.RawConfigParser()
        parser.read_string(match[0])
        section = parser.sections()[0]
        operation = match[1].upper()
        expected = {'label': operation.capitalize().replace('_', ' '),
                    'gcode': operation}
        values = {key: value.strip() for key, value in parser.items(section)}
        if values != expected:
            return match[0]
        # Retain any explanatory comments, but remove the redundant shortcut.
        return ''.join(line for line in match[0].splitlines(keepends=True)
                       if line.lstrip().startswith(('#', ';')))

    return re.sub(
        r'(?ms)^[ \t]*\[e3v3se_display macro '
        r'(load_filament|unload_filament)\][^\r\n]*\r?\n'
        r'.*?(?=^[ \t]*\[|\Z)', shortcut, text,
    )


def migrate_config(text):
    """Migrate text; reject ambiguous sections/options before writing."""
    text = _remove_default_display_shortcuts(text)
    result = []
    sections = {}
    section_options = {}
    options = {}
    section = None
    in_gcode = False
    aliases = {}
    for line in text.splitlines(keepends=True):
        header = HEADER.match(line)
        if header:
            section = canonical_section(header[2])
            identity = section.lower()
            raw = header[2].lower()
            if identity in sections and sections[identity] != raw:
                raise ValueError('Configuration section collision: ' + section)
            sections[identity] = raw
            options = section_options.setdefault(identity, {})
            in_gcode = False
            aliases = {}
            result.append(
                header[1] + '[' + section + ']' + header[3]
                + (header[4] or '')
            )
            continue
        option_after_body = (
            OPTION.match(line) if not line[:1].isspace() else None
        )
        if in_gcode and not option_after_body:
            result.append(_gcode_line(line, _own_parameters(section), aliases))
            continue
        option = OPTION.match(line.rstrip('\r\n'))
        if option and section:
            key = canonical_option(section, option[2])
            raw = option[2].lower()
            if key.lower() in options and options[key.lower()] != raw:
                raise ValueError('Configuration option collision: ' + key)
            options[key.lower()] = raw
            ending = line[len(line.rstrip('\r\n')):]
            in_gcode = key.lower() == 'gcode'
            value = option[4]
            if in_gcode:
                value = _gcode_line(value, _own_parameters(section), aliases)
            result.append(option[1] + key + option[3] + value + ending)
        else:
            result.append(line)
    return ''.join(result)


def migrate_state(data):
    """Copy state, changing only recognized native endpoint callback values."""
    if not isinstance(data, dict):
        raise ValueError('The BMCU state must be an object.')
    result = copy.deepcopy(data)
    endpoints = result.get('endpoints', {})
    if not isinstance(endpoints, dict):
        raise ValueError('The BMCU endpoint state must be an object.')
    for endpoint in endpoints.values():
        if not isinstance(endpoint, dict):
            raise ValueError('A BMCU endpoint state must be an object.')
        for field in CALLBACK_FIELDS:
            value = endpoint.get(field)
            if isinstance(value, str) and value.strip().upper() in CALLBACKS:
                endpoint[field] = CALLBACKS[value.strip().upper()]
    return result


def migrate_callback_source(text):
    """Update exact callback string literals without editing comments/code."""
    tokens = []
    for token in tokenize.generate_tokens(io.StringIO(text).readline):
        if token.type == tokenize.STRING:
            try:
                value = ast.literal_eval(token.string)
            except (ValueError, SyntaxError):
                value = None
            if isinstance(value, str) and value.upper() in CALLBACKS:
                token = token._replace(string=repr(CALLBACKS[value.upper()]))
        tokens.append(token)
    result = tokenize.untokenize(tokens)
    compile(result, '<migrated callback source>', 'exec')
    return result


def restore_state_callbacks(current, original):
    """Undo only our callback rename, never restore stale routes/calibration."""
    if not isinstance(current, dict) or not isinstance(original, dict):
        raise ValueError('The BMCU state must be an object.')
    result = copy.deepcopy(current)
    endpoints = result.get('endpoints', {})
    previous = original.get('endpoints', {})
    if not isinstance(endpoints, dict) or not isinstance(previous, dict):
        raise ValueError('The BMCU endpoint state must be an object.')
    for name, old in previous.items():
        active = endpoints.get(name)
        if not isinstance(old, dict) or not isinstance(active, dict):
            continue
        for field in CALLBACK_FIELDS:
            value = old.get(field)
            if not isinstance(value, str):
                continue
            expected = CALLBACKS.get(value.strip().upper())
            if expected and active.get(field) == expected:
                active[field] = value
    return result
