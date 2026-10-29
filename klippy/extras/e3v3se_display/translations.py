# -*- coding: utf-8 -*-
# Display translations for the Ender 3 V3 SE stock display
#
# Copyright (C) 2026 Bernardo Costa
#
# This file may be distributed under the terms of the GNU GPLv3 license.

from __future__ import unicode_literals

import re

from .tjc3224 import _as_text


PORTUGUESE = {
    'COMMAND FAILED': 'FALHA NO COMANDO',
    'COMMAND STARTED': 'COMANDO INICIADO',
    'Home all': 'Referenciar eixos',
    'Homing all axes': 'Referenciando eixos',
    'Preheat PLA': 'Aquecer PLA',
    'PLA preheat selected': 'Perfil PLA selecionado',
    'Preheat PETG': 'Aquecer PETG',
    'PETG preheat selected': 'Perfil PETG selecionado',
    'Preheat TPU': 'Aquecer TPU',
    'TPU preheat selected': 'Perfil TPU selecionado',
    'Cooldown': 'Esfriar',
    'Heaters and fan disabled': 'Aquecedores e ventoinha desligados',
    'Load filament': 'Carregar filamento',
    'Load macro started': 'Carga iniciada',
    'Unload filament': 'Retirar filamento',
    'Unload macro started': 'Descarga iniciada',
    'Disable motors': 'Desligar motores',
    'Motors disabled': 'Motores desligados',
    'PREPARE': 'PREPARAR',
    'MOVE %s': 'MOVER %s',
    'Move X (1 mm)': 'Mover X (1 mm)',
    'Move Y (1 mm)': 'Mover Y (1 mm)',
    'Move Z (0.1 mm)': 'Mover Z (0,1 mm)',
    'Extrude (5 mm)': 'Extrudar (5 mm)',
    'MOVE': 'MOVER',
    'Nozzle target': 'Alvo do bico',
    'NOZZLE': 'BICO',
    'Bed target': 'Alvo da mesa',
    'BED': 'MESA',
    'Fan': 'Ventoinha',
    'FAN': 'VENTOINHA',
    'TEMPERATURE': 'TEMPERATURA',
    'Speed factor': 'Velocidade',
    'SPEED': 'VELOCIDADE',
    'Flow factor': 'Fluxo',
    'FLOW': 'FLUXO',
    'Z offset': 'Offset Z',
    'Z OFFSET': 'OFFSET Z',
    'TUNE': 'AJUSTAR',
    'CR-Touch Z offset': 'Offset Z CR-Touch',
    'Bed mesh': 'Malha da mesa',
    'Screws tilt': 'Ajustar parafusos',
    'Screw tilt calculation started': 'Ajuste de parafusos iniciado',
    'Save config': 'Salvar ajustes',
    'CALIBRATION': 'CALIBRAR',
    '%s started': '%s iniciado',
    'No display macros': 'Sem atalhos',
    'FILES UNAVAILABLE': 'ARQUIVOS INDISPONÍVEIS',
    '< Parent': '< Pasta anterior',
    'No G-code files': 'Sem arquivos G-code',
    'PRINT FILES': 'ARQUIVOS',
    'START PRINT?': 'INICIAR IMPRESSÃO?',
    'Print started': 'Impressão iniciada',
    'CANCEL PRINT?': 'CANCELAR IMPRESSÃO?',
    'This cannot be undone': 'A impressão será cancelada',
    'Print cancelled': 'Impressão cancelada',
    'CALIBRATE Z?': 'CALIBRAR Z?',
    'Clear the bed first': 'Esvazie a mesa primeiro',
    'Homing, then manual probe': 'Referenciar e calibrar Z',
    'BUILD MESH?': 'GERAR MALHA?',
    'Homing, then bed mesh': 'Referenciar e gerar malha',
    'SAVE CONFIG?': 'SALVAR AJUSTES?',
    'Klipper will restart': 'Klipper será reiniciado',
    'Confirm': 'Confirmar',
    'Cancel': 'Cancelar',
    'Rotate to adjust': 'Gire para ajustar',
    'Click to return': 'Clique para voltar',
    'Z CALIBRATION': 'CALIBRAÇÃO Z',
    'Turn: move 0.05 mm': 'Gire: mover 0,05 mm',
    'Click: ACCEPT': 'Clique: aceitar',
    'Hold: ABORT': 'Segure: abortar',
    '< Back': '< Voltar',
    'Resume print': 'Retomar impressão',
    'Print resumed': 'Impressão retomada',
    'Pause print': 'Pausar impressão',
    'Print paused': 'Impressão pausada',
    'Tune': 'Ajustar',
    'Cancel print': 'Cancelar impressão',
    'Print': 'Imprimir',
    'Prepare': 'Preparar',
    'Move': 'Mover',
    'Temperature': 'Temperatura',
    'Calibration': 'Calibrar',
    'Macros': 'Atalhos',
    'MAIN MENU': 'MENU PRINCIPAL',
    'STATE': 'ESTADO',
    'NOZZLE %d / %d C': 'BICO   %d / %d C',
    'BED    %d / %d C': 'MESA   %d / %d C',
    'Ready': 'Pronta',
    'Click for menu': 'Clique para o menu',
    'Printing': 'Imprimindo',
    'Paused': 'Pausada',
    'Complete': 'Concluída',
    'Cancelled': 'Cancelada',
    'Error': 'Erro',
    'BMCU absent': 'BMCU ausente',
    'BMCU unavailable': 'BMCU indisponível',
    'BMCU offline': 'BMCU offline',
    'BMCU: version': 'BMCU: versão',
    'BMCU: check route': 'BMCU: verificar',
    'BMCU: conflict': 'BMCU: conflito',
    'BMCU ready': 'BMCU pronta',
    'BMCU: failure': 'BMCU: falha',
    'Channel %d': 'Canal %d',
    'Channel %d: load': 'Canal %d: carregar',
    'Unload active channel': 'Descarregar canal ativo',
    'BMCU channels': 'Canais BMCU',
    'BMCU CHANNELS': 'CANAIS BMCU',
    'FILAMENT': 'FILAMENTO',
    'FILAMENT FAILED': 'FALHA NO FILAMENTO',
    'BMCU FAILED': 'FALHA NA BMCU',
    'Filament': 'Filamento',
    'Heating nozzle': 'Aquecendo bico',
    'Loading filament': 'Carregando filamento',
    'Unloading filament': 'Descarregando filamento',
    'Moving extruder': 'Movendo extrusor',
    'Click to cancel': 'Clique para cancelar',
    'Wait for motion to stop': 'Aguarde o movimento parar',
    'Finished': 'Concluído',
    'Cancelling operation': 'Cancelando operação',
    'Cancelling BMCU': 'Cancelando BMCU',
    'BMCU: finishing': 'BMCU: finalizando',
    'BMCU: channel %d': 'BMCU: canal %d',
    'KLIPPER SHUTDOWN': 'KLIPPER DESLIGADO',
    'MCU ERROR': 'ERRO DA MCU',
    'Calibration finished or aborted': 'Calibração concluída ou abortada',
    'Complete or cancel the filament operation':
        'Conclua ou cancele a operação de filamento',
    'Home Z before changing the live Z offset':
        'Referencie Z antes de ajustar o offset',
    'Home %s before moving it': 'Referencie %s antes de mover',
    'Configure the LOAD_FILAMENT macro': 'Configure a macro LOAD_FILAMENT',
    'Configure the UNLOAD_FILAMENT macro': 'Configure a macro UNLOAD_FILAMENT',
    'Filament operation in progress': 'Operação de filamento em andamento',
    'BMCU operation in progress': 'Operação BMCU em andamento',
    'BMCU operation cancelled': 'Operação BMCU cancelada',
    'BMCU is busy': 'A BMCU está ocupada',
    'Use the BMCU channels and check the route':
        'Use os canais BMCU e verifique a rota',
    'Pause the print before moving filament':
        'Pause a impressão antes de mover o filamento',
    'Pause the print before using channels':
        'Pause a impressão antes de usar os canais',
    'Filament temperature outside the safe range':
        'Temperatura do filamento fora do intervalo seguro',
    'Set an appropriate target for material %s':
        'Defina um alvo adequado para o material %s',
    'Operation cancelled': 'Operação cancelada',
    'Filament operation cancelled': 'Operação de filamento cancelada',
    'Nozzle target changed; operation aborted':
        'Alvo do bico alterado; operação abortada',
    'Heating timed out': 'Tempo de aquecimento excedido',
    'Invalid filament action': 'Ação de filamento inválida',
    'Invalid BMCU channel': 'Canal BMCU inválido',
    'No confirmed loaded BMCU channel':
        'Nenhum canal carregado confirmado pela BMCU',
    'BMCU unavailable or route needs verification':
        'BMCU indisponível ou rota precisa de verificação',
    'Operation cancelled; check the BMCU route':
        'Operação cancelada; verifique a rota BMCU',
    'Measure distances and calibrate filament sequences':
        'Meça as distâncias e calibre as sequências de filamento',
    'chunk_mm must be greater than 0 and at most 20 mm':
        'chunk_mm deve estar entre 0 e 20 mm',
    'Calibrate capture, nozzle feed and purge':
        'Calibre captura, avanço ao bico e purga',
    'Calibrate tip forming and total retraction':
        'Calibre a formação de ponta e o recuo total',
    'Filament speed must be positive':
        'Velocidade de filamento deve ser positiva',
    'Physically validate BMCU arrival at the gears':
        'Valide fisicamente a chegada BMCU até as engrenagens',
    'This integration requires BMCU-Klipper 1.0.6':
        'Esta integração requer BMCU-Klipper 1.0.6',
    'Set arrival mode and measured route limit':
        'Defina modo de chegada e limite de percurso medido',
    'Install BMCU-Klipper 1.0.6 before setup':
        'Instale BMCU-Klipper 1.0.6 antes de configurar',
    'Configure and verify the PC9 switch': 'Configure e verifique o switch PC9',
    'Set an appropriate material temperature':
        'Defina a temperatura apropriada ao material',
    'N %d/%dC': 'Bico %d/%d',
    'B %d/%dC': 'Mesa %d/%d',
    'Print started; operation aborted':
        'Impressão iniciada; operação abortada',
    'Extruder below minimum temperature':
        'Extrusor abaixo da temperatura mínima',
    'Finish or cancel the print before using channels.':
        'Conclua ou cancele a impressão antes de usar os canais.',
    'A BMCU operation or print plan is in progress.':
        'Há uma operação ou um plano de impressão BMCU em andamento.',
    'Finish the BMCU print context before using channels.':
        'Finalize o contexto de impressão BMCU antes de usar os canais.',
    'The BMCU is not ready.': 'A BMCU não está pronta.',
    'Manual controls require one BMCU.':
        'Os controles manuais exigem uma BMCU.',
    'Check the uncertain BMCU route before continuing.':
        'Confira o caminho incerto da BMCU antes de continuar.',
    'More than one BMCU channel is marked loaded.':
        'Mais de um canal BMCU está marcado como carregado.',
    'The entry sensor and loaded BMCU route disagree.':
        'O switch e o canal carregado da BMCU não correspondem.',
    'The printer is shut down.': 'A impressora está em parada de emergência.',
    'Filament operation cancelled.': 'Operação de filamento cancelada.',
    'A print started during the filament operation.':
        'Uma impressão iniciou durante a operação de filamento.',
    'Nozzle target changed; operation aborted.':
        'O alvo do bico mudou; operação abortada.',
    'Heating timed out.': 'O aquecimento excedeu o tempo limite.',
    'Invalid filament action.': 'Ação de filamento inválida.',
    'Filament operation in progress.': 'Operação de filamento em andamento.',
    'No filament is loaded; nothing to unload.':
        'Nenhum filamento carregado; não há o que retirar.',
    'Channel assigned to another extruder.':
        'Canal associado a outra extrusora.',
    'Insert filament into channel %d first.':
        'Insira filamento no canal %d primeiro.',
    'Channel %d is already loaded.': 'O canal %d já está carregado.',
    'The previous filament did not unload.':
        'O filamento anterior não foi retirado.',
    'The requested channel was not loaded.':
        'O canal solicitado não foi carregado.',
    'Finish the current operation before preheating.':
        'Conclua a operação atual antes de pré-aquecer.',
}


def _build_patterns():
    patterns = []
    for source, target in PORTUGUESE.items():
        if "%" in source:
            pattern = re.escape(source)
            # Python 2 escapes '%' too; match the escaped placeholder token.
            pattern = pattern.replace(re.escape("%s"), "(.+?)")
            pattern = pattern.replace(re.escape("%d"), "(-?[0-9]+)")
            patterns.append((re.compile("^" + pattern + "$"), target))
    return patterns


_PATTERNS = _build_patterns()


def translate(value, language):
    text = _as_text(value)
    if language != "pt_BR":
        return text
    if text in PORTUGUESE:
        return PORTUGUESE[text]
    for pattern, target in _PATTERNS:
        match = pattern.match(text)
        if match:
            args = match.groups()
            # Translate dynamic labels too, such as a macro's display label.
            args = tuple(translate(arg, language) for arg in args)
            # Numeric templates keep their original numeric value.
            placeholders = re.findall(r"%[sd]", target)
            values = tuple(int(arg) if kind == "%d" else arg
                           for arg, kind in zip(args, placeholders))
            return target % values
    # Klipper template errors wrap the original macro error. Localize its
    # known message while retaining the diagnostic context and source name.
    for source, target in PORTUGUESE.items():
        if "%" not in source and len(source) > 20 and source in text:
            text = text.replace(source, target)
    return text
