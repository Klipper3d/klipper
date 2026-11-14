"""Canonical parameter names and rejection of mixed v1/v2 commands."""

from functools import wraps


PARAMETERS = {
    'CONFIRMAR': 'CONFIRM',
    'CANAL': 'CHANNEL',
    'PURGAR_SE_CARREGADO': 'PURGE_IF_LOADED',
    'POSICAO': 'POSITION',
    'ANGULO': 'ANGLE',
    'LIMITE': 'LIMIT',
    'CAMINHO_LIVRE': 'CLEAR_PATH',
    'EXTRUSORA_PRIMEIRO': 'EXTRUDER_FIRST',
    'VELOCIDADE': 'SPEED',
    'APOS': 'EXTRA',
    'TEMPO': 'TIMEOUT',
    'TEMP_MESA': 'BED_TEMP',
    'TEMP_BICO': 'NOZZLE_TEMP',
    'FERRAMENTA': 'TOOL',
}

STATUS_FIELDS = {
    'cortador_incerto': 'cutter_uncertain',
    'ultimo_corte': 'last_cut',
}

DIAGNOSTIC_FIELDS = {
    'resultado': 'result', 'etapa': 'stage', 'segundos': 'seconds',
    'posicao_incerta': 'position_uncertain',
    'sincronismo_pre_corte': 'pre_cut_sync',
    'sincronismo_pos_corte': 'post_cut_sync',
    'diagnostico': 'diagnostic', 'inicio_s': 'started_at_seconds',
    'tempo_s': 'elapsed_seconds', 'tempo_movimentos_s': 'move_seconds',
    'recuo_extrusora_comandado_mm': 'commanded_extruder_retract_mm',
    'recuo_encoder_mm': 'encoder_retract_mm', 'movimento': 'motion',
    'antes_assistencia': 'before_assistance',
    'antes_primeiro_passo': 'before_first_step',
    'antes_passo': 'before_step', 'depois_passo': 'after_step',
}

DIAGNOSTIC_VALUES = {
    'comandos_concluidos': 'commands_completed', 'interrompido': 'interrupted',
    'validacao': 'validation', 'recolhimento_inicial': 'initial_retraction',
    'aproximacao': 'approach', 'recuo_pre_corte': 'pre_cut_retract',
    'acionamento_servo': 'servo_deployment', 'curso_da_lamina': 'blade_stroke',
    'retorno_e_recolhimento': 'return_and_retract',
    'retorno_a_origem': 'return_to_origin',
}


def public_diagnostics(value):
    """Copy telemetry into the public schema; never alter physical state."""
    if isinstance(value, dict):
        result = {}
        for key, item in value.items():
            key = DIAGNOSTIC_FIELDS.get(key, key)
            if key in ('result', 'stage') and isinstance(item, str):
                item = DIAGNOSTIC_VALUES.get(item, item)
            result[key] = public_diagnostics(item)
        return result
    if isinstance(value, (list, tuple)):
        return [public_diagnostics(item) for item in value]
    return value


def require_english_parameters(gcmd):
    legacy = set(gcmd.get_command_parameters()).intersection(PARAMETERS)
    if legacy:
        replacements = ', '.join(
            '%s -> %s' % (name, PARAMETERS[name]) for name in sorted(legacy)
        )
        raise gcmd.error(
            'Use the v2 English parameters: %s. Re-slice old print files.'
            % replacements
        )


def english_command(function):
    @wraps(function)
    def execute(gcmd):
        require_english_parameters(gcmd)
        return function(gcmd)
    return execute
