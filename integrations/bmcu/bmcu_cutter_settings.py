"""English public cutter settings; the tested motion state remains private."""

LEGACY_TO_PUBLIC = {
    'habilitado': 'enabled',
    'calibrado': 'calibrated',
    'angulo_recolhido': 'retracted_angle',
    'angulo_acionado': 'deployed_angle',
    'x_aproximacao': 'approach_x',
    'x_contato': 'contact_x',
    'x_corte': 'cut_x',
    'y_corte': 'cut_y',
    'manter_y_corte': 'keep_cut_y',
    'z_minimo': 'minimum_z',
    'alivio_z': 'z_lift',
    'curso_maximo_mm': 'maximum_stroke_mm',
    'deslocamento_f': 'travel_feedrate',
    'corte_f': 'cut_feedrate',
    'espera_servo_ms': 'servo_wait_ms',
    'soltar_servo_recolhido': 'release_retracted_servo',
    'recuo_antes_corte_mm': 'pre_cut_retract_mm',
    'velocidade_recuo_f': 'retract_feedrate',
    'velocidade_avanco_f': 'feed_feedrate',
    'velocidade_purga_f': 'purge_feedrate',
    'posicionamento_paralelo': 'parallel_positioning',
}


def private_settings(public):
    """Decode canonical macro state without changing the motion logic."""
    if set(public).intersection(LEGACY_TO_PUBLIC):
        raise ValueError(
            'Migrate legacy cutter settings with the v2 installer.'
        )
    reverse = {value: key for key, value in LEGACY_TO_PUBLIC.items()}
    return {reverse.get(key, key): value for key, value in public.items()}


def public_settings(private):
    """Encode the private motion profile for status/configuration consumers."""
    return {
        LEGACY_TO_PUBLIC.get(key, key): value for key, value in private.items()
    }
