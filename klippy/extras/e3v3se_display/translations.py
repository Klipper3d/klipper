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
    'Nozzle temperature must be a finite number.':
        'A temperatura do bico deve ser um número finito.',
    'Nozzle temperature is outside the heater limits.':
        'A temperatura do bico está fora dos limites do aquecedor.',
    'Run G28 before preheating above the parking threshold.':
        'Execute G28 antes de pré-aquecer acima do limite de estacionamento.',
    'Print state changed during preheat positioning.':
        'O estado da impressão mudou durante o posicionamento '
        'para pré-aquecer.',
    'Preheat positioning did not reach the purge position.':
        'O posicionamento para pré-aquecer não alcançou a posição de purga.',
    'There is no pending print recovery.':
        'Não há recuperação de impressão pendente.',
    'The paused print changed; recovery is blocked.':
        'A impressão pausada mudou; a recuperação está bloqueada.',
    'A BMCU operation is still active.': 'Ainda há operação da BMCU ativa.',
    'Position was lost; print recovery is blocked.':
        'A posição foi perdida; a recuperação está bloqueada.',
    'The coordinate offset changed during recovery.':
        'O offset de coordenadas mudou durante a recuperação.',
    'The BMCU is not ready for recovery.':
        'A BMCU não está pronta para recuperação.',
    'The BMCU session changed; print recovery is blocked.':
        'A sessão da BMCU mudou; a recuperação está bloqueada.',
    'An uncertain filament route needs physical recovery; '
    'the print remains paused.':
        'Uma rota incerta exige recuperação física; a impressão segue pausada.',
    'The entry sensor and filament routes disagree.':
        'O sensor de entrada e as rotas de filamento não correspondem.',
    'Physically clear the shared path and park this filament '
    'behind the entry sensor before CONFIRM=1.':
        'Desobstrua fisicamente o caminho compartilhado e estacione este '
        'filamento antes do sensor de entrada; só então use CONFIRM=1.',
    'This channel does not match the paused failure.':
        'Este canal não corresponde à falha pausada.',
    'A detached filament tail requires separate recovery.':
        'Uma ponta de filamento solta exige recuperação separada.',
    'The shared path is not confirmed clear and idle.':
        'O caminho compartilhado não foi confirmado livre e parado.',
    'The BMCU did not confirm an empty route.':
        'A BMCU não confirmou uma rota vazia.',
    'Channel %d route confirmed clear. Print mapping preserved; '
    'RESUME will finish the pending tool change.':
        'Rota do canal %d confirmada livre. Mapa preservado; '
        'RESUME conclui a troca pendente.',
    'The pending filament was not confirmed.':
        'O filamento pendente não foi confirmado.',
    'A tool change is pending; use RESUME to recover it.':
        'Há troca pendente; use RESUME para recuperá-la.',
    'Print paused with tool T%d pending: %s. Check the filament path, '
    'then RESUME will finish this tool change before printing.':
        'Impressão pausada com ferramenta T%d pendente: %s. Confira o caminho; '
        'RESUME conclui a troca antes de imprimir.',
    'Channel %d unload timing: positioning %.2f s; tip %.2f s; '
    'sensor release %.2f s; feeder parking %.2f s; total %.2f s.':
        'Canal %d tempo de retirada: posicionamento %.2f s; ponta %.2f s; '
        'liberação do sensor %.2f s; estacionamento %.2f s; total %.2f s.',
    'Channel %d: pausing extruder retraction while the feeder '
    'relieves buffer compression.':
        'Canal %d: pausando a retração da extrusora enquanto o alimentador '
        'alivia a compressão do buffer.',
    'Channel %d: extruder waiting for feeder pullback; encoder lag %.2f mm.':
        'Canal %d: extrusora aguardando recuo; atraso do encoder %.2f mm.',
    'This test requires the UNCERTAIN route left by entry sensor arrival.':
        'Este teste exige a rota UNCERTAIN deixada pela chegada ao sensor.',
    'The channel must be EMPTY before measurement.':
        'O canal deve estar EMPTY antes da medição.',
    'The channel must be loaded or partially unloaded.':
        'O canal deve estar carregado ou parcialmente retirado.',
    'Channel %d: encoder-confirmed pullback after the entry sensor: %.2f mm.':
        'Canal %d: recuo confirmado pelo encoder após o sensor: %.2f mm.',
    'Channel %d: capture %.1f mm, nozzle feed %.1f mm '
    'and purge %.1f mm completed.':
        'Canal %d: captura %.1f mm, avanço ao bico %.1f mm '
        'e purga %.1f mm concluídos.',
    'Channel %d: entry sensor cleared after %.1f mm of extrusion; '
    'BMCU will retract another %.1f mm.':
        'Canal %d: sensor de entrada liberado após %.1f mm de extrusão; '
        'a BMCU vai retirar mais %.1f mm.',
    'Channel %d parked. Extruder retraction commanded: %.2f mm; '
    'pullback after the entry sensor: %.2f mm.':
        'Canal %d estacionado. Retração comandada da extrusora: %.2f mm; '
        'recuo após o sensor de entrada: %.2f mm.',
    'Channel %d: feed at 10 mm/s to the entry sensor; limit %.0f mm.':
        'Canal %d: avanço a 10 mm/s até o sensor de entrada; limite %.0f mm.',
    'Entry sensor triggered. Channel %d measured remaining travel: %.2f mm.':
        'Sensor de entrada acionado. Canal %d percurso restante: %.2f mm.',
    'Channel %d: retracting the extruder until the entry sensor clears, '
    'with BMCU assistance.':
        'Canal %d: retraindo a extrusora até liberar o sensor de entrada, '
        'com assistência BMCU.',
    'Entry sensor clear. Extruder stopped; '
    'retracting another %.1f mm through BMCU.':
        'Sensor de entrada livre. Extrusora parada; '
        'retirando mais %.1f mm pela BMCU.',
    'Channel %d: attempting %.3f mm at F%.3f; '
    'configured extruder limit F%.3f.':
        'Canal %d: tentando %.3f mm a F%.3f; '
        'limite configurado da extrusora F%.3f.',
    'Channel %d: tip-forming moves completed; '
    'inspect the tip after unloading.':
        'Canal %d: formação de ponta concluída; '
        'inspecione a ponta após a retirada.',
    '%s must be a finite number.': '%s deve ser um número finito.',
    'Unload buffer resume must be below its limit.':
        'A retomada do buffer de retirada deve ficar abaixo do limite.',
    'Command G28 was not found.': 'Comando G28 não encontrado.',
    'Command %s was not found.': 'Comando %s não encontrado.',
    'The BMCU restarted during pullback.': 'A BMCU reiniciou durante o recuo.',
    'The BMCU did not remain idle after parking.':
        'A BMCU não permaneceu parada após estacionar.',
    'The encoder confirmed only %.2f mm after the entry sensor; '
    'the next channel will not be loaded.':
        'O encoder confirmou só %.2f mm após o sensor de entrada; '
        'o próximo canal não será carregado.',
    'The entry sensor detected filament again during pullback.':
        'O sensor de entrada voltou a detectar filamento durante o recuo.',
    'Orca tool %d is missing from the committed BMCU print map.':
        'A ferramenta %d do Orca não consta no mapa BMCU confirmado.',
    'The Orca index and BMCU channel do not match.':
        'O índice do Orca e o canal BMCU não correspondem.',
    'The entry sensor did not return a valid reading.':
        'O sensor de entrada não retornou leitura válida.',
    'The sensor did not return a valid state.':
        'O sensor não retornou estado válido.',
    'The entry sensor must be available and enabled.':
        'O sensor de entrada deve estar disponível e habilitado.',
    'The entry sensor must be clear before measurement.':
        'O sensor de entrada deve estar livre antes da medição.',
    'The entry sensor is already clear; no new transition can be measured.':
        'O sensor de entrada já está livre; não há nova transição para medir.',
    'The entry sensor did not clear within %.1f seconds.':
        'O sensor de entrada não liberou em %.1f segundos.',
    'The entry sensor did not clear within the configured timeout.':
        'O sensor de entrada não liberou no tempo configurado.',
    'The entry sensor was already clear before unloading.':
        'O sensor de entrada já estava livre antes da retirada.',
    'The entry sensor cleared during loading.':
        'O sensor de entrada liberou durante a carga.',
    'The entry sensor cleared during purging.':
        'O sensor de entrada liberou durante a purga.',
    'The filament did not trigger the entry sensor.':
        'O filamento não acionou o sensor de entrada.',
    'The entry sensor lost filament detection during capture.':
        'O sensor de entrada perdeu a detecção durante a captura.',
    'No encoder pullback progress while the entry sensor remains triggered.':
        'Sem progresso de recuo no encoder com o sensor de entrada acionado.',
    'The encoder measured only %.2f mm after the entry sensor cleared.':
        'O encoder mediu só %.2f mm após o sensor de entrada liberar.',
    'The encoder did not confirm the retraction direction.':
        'O encoder não confirmou o sentido da retirada.',
    'Feeder/extruder mismatch exceeded the withdrawal allowance; '
    'encoder lag %.2f mm.':
        'Divergência alimentador/extrusora excedeu a folga; '
        'atraso do encoder %.2f mm.',
    'Feeder withdrawal assistance stopped.':
        'A assistência de retirada do alimentador parou.',
    'Feeder did not follow extruder withdrawal; encoder lag %.2f mm, '
    'commanded net %.2f mm, measured net %.2f mm.':
        'O alimentador não acompanhou a retirada; atraso %.2f mm, '
        'comandado líquido %.2f mm, medido líquido %.2f mm.',
    'Feeder assistance changed during withdrawal.':
        'A assistência do alimentador mudou durante a retirada.',
    'Invalid withdrawal encoder position.':
        'Posição inválida do encoder na retirada.',
    'The unload buffer did not recover.': 'O buffer de retirada não recuperou.',
    'Release the channel buffer before measurement.':
        'Libere o buffer do canal antes da medição.',
    'The BMCU restarted during unloading.':
        'A BMCU reiniciou durante a retirada.',
    'The BMCU restarted during the tool change.':
        'A BMCU reiniciou durante a troca.',
    'The channel lost filament detection.':
        'O canal perdeu a detecção de filamento.',
    'This command must be called by a native BMCU operation.':
        'Este comando deve ser chamado por operação nativa BMCU.',
    'Incompatible extruder endpoint.': 'Endpoint de extrusora incompatível.',
    'This operation belongs to another extruder.':
        'Esta operação pertence a outra extrusora.',
    'Extruder-first capture is not installed.':
        'Captura iniciada pela extrusora não instalada.',
    'The native unload context changed.':
        'O contexto nativo de retirada mudou.',
    'The BMCU did not identify the active channel.':
        'A BMCU não identificou o canal ativo.',
    'The native unloading phase does not match.':
        'A fase nativa de retirada não corresponde.',
    'The native feeding phase does not match this command.':
        'A fase nativa de avanço não corresponde a este comando.',
    'The channel must be assigned to the generic extruder.':
        'O canal deve estar associado à extrusora genérica.',
    'The printer and BMCU must be idle.':
        'A impressora e a BMCU devem estar paradas.',
    'This test requires an idle printer and BMCU.':
        'Este teste exige impressora e BMCU paradas.',
    'Remove the previous filament from the extruder and confirm CLEAR_PATH=1.':
        'Retire o filamento anterior da extrusora e confirme CLEAR_PATH=1.',
    'Heat the nozzle to the material temperature before testing.':
        'Aqueça o bico à temperatura do material antes de testar.',
    'Heat the nozzle to the material temperature before unloading.':
        'Aqueça o bico à temperatura do material antes de retirar.',
    'Heat the nozzle before changing filament.':
        'Aqueça o bico antes de trocar filamento.',
    'Run G28 before loading or unloading filament.':
        'Execute G28 antes de carregar ou retirar filamento.',
    'Run G28 before changing filament.':
        'Execute G28 antes de trocar filamento.',
    'The extrusion limit is set to zero.': 'O limite de extrusão está zerado.',
    'The extruder move limit is set to zero.':
        'O limite de movimento da extrusora está zerado.',
    'max_extrude_only_distance is set to zero.':
        'max_extrude_only_distance está zerado.',
    'Invalid tip-forming setting: %s.':
        'Ajuste de formação de ponta inválido: %s.',
    'Invalid tip-forming distances or speeds.':
        'Distâncias ou velocidades de formação de ponta inválidas.',
    'Tip-forming speed exceeds the extruder limit.':
        'A velocidade de formação de ponta excede o limite da extrusora.',
    'Tip-forming temperature is outside limits.':
        'A temperatura de formação de ponta está fora dos limites.',
    'Tip-forming moves exceed max_extrude_only_distance.':
        'Movimentos de formação de ponta excedem max_extrude_only_distance.',
    'Tip-forming target changed.': 'O alvo de formação de ponta mudou.',
    'Tip-forming temperature timed out.':
        'A temperatura de formação de ponta excedeu o tempo limite.',
    'Filament left before tip forming.':
        'O filamento saiu antes da formação de ponta.',
    'Filament left the entry sensor during tip forming.':
        'O filamento saiu do sensor de entrada durante a formação de ponta.',
    'Filament left during tip cooling.':
        'O filamento saiu durante o resfriamento da ponta.',
    'Cooling position passed the entry sensor; '
    'reduce the tip-forming distances.':
        'A posição de resfriamento passou o sensor; '
        'reduza as distâncias da ponta.',
    'Could not obtain extruder move timing.':
        'Não foi possível obter o tempo de movimento da extrusora.',
    'The extruder did not start within the expected time.':
        'A extrusora não iniciou no tempo esperado.',
    'The extruder motion window ended before BMCU assistance.':
        'A janela de movimento da extrusora terminou '
        'antes da assistência BMCU.',
    'The BMCU responded after the initial extruder motion window.':
        'A BMCU respondeu após a janela inicial de movimento da extrusora.',
    'Could not obtain the extruder move end time.':
        'Não foi possível obter o fim do movimento da extrusora.',
    'Could not track the move to stop the BMCU.':
        'Não foi possível acompanhar o movimento para parar a BMCU.',
    'Could not confirm BMCU stop before the extruder move ended.':
        'Não foi possível confirmar parada BMCU antes do fim da extrusora.',
    'Could not confirm motor stop; disconnect BMCU power.':
        'Não foi possível confirmar a parada; desligue a alimentação da BMCU.',
    'Could not confirm that the BMCU stopped.':
        'Não foi possível confirmar que a BMCU parou.',
    'Could not confirm the BMCU stopped before pausing.':
        'Não foi possível confirmar a parada BMCU antes de pausar.',
    'Could not restore the BMCU speed.':
        'Não foi possível restaurar a velocidade BMCU.',
    'Could not measure arrival: %s': 'Não foi possível medir a chegada: %s',
    'Measurement interrupted: %s': 'Medição interrompida: %s',
    'Arrival was not confirmed: %s; measured travel: %s mm.':
        'Chegada não confirmada: %s; percurso medido: %s mm.',
    'Could not test extrusion: %s': 'Não foi possível testar a extrusão: %s',
    'Test interrupted; the accumulated distance is invalid: %s':
        'Teste interrompido; distância acumulada inválida: %s',
    'Failed to stop assisted feeding: %s':
        'Falha ao parar o avanço assistido: %s',
    'Unload interrupted; do not load another channel yet: %s':
        'Retirada interrompida; não carregue outro canal ainda: %s',
    'Load interrupted: %s': 'Carga interrompida: %s',
    'Unload interrupted: %s': 'Retirada interrompida: %s',
    'BMCU stop was not confirmed: %s': 'Parada BMCU não confirmada: %s',
    '%s Channel %d; buffer %s%%; PWM %s; motion %s.%s':
        '%s Canal %d; buffer %s%%; PWM %s; movimento %s.%s',
    '%s Channel %d; buffer %s%%; PWM %s; motion %s.':
        '%s Canal %d; buffer %s%%; PWM %s; movimento %s.',
    ' Commanded net retract %.1f mm; '
    'encoder pullback after forming %.2f mm.':
        ' Retração líquida comandada %.1f mm; '
        'recuo do encoder após formação %.2f mm.',
    ' Total commanded net %.2f mm.':
        ' Total comandado líquido %.2f mm.',
    'Hotend confirmed unloaded; no filament was moved. %s':
        'Hotend confirmado descarregado; nenhum filamento foi movido. %s',
    'Purge/wipe position is outside the travel limits.':
        'A posição de purga/limpeza está fora dos limites de curso.',
    'Parallel positioning was interrupted.':
        'O posicionamento paralelo foi interrompido.',
    'Axis homing reference was lost.': 'A referência dos eixos foi perdida.',
    'The extruder cooled during arrival.':
        'A extrusora esfriou durante a chegada.',
    'The filament tip must be triggering the entry sensor.':
        'A ponta do filamento deve estar acionando o sensor de entrada.',
    'Configured limit: %.3f mm/s, SPEED=%.3f mm/min; '
    'maximum internal move: %.3f mm.':
        'Limite configurado: %.3f mm/s, SPEED=%.3f mm/min; '
        'movimento interno máximo: %.3f mm.',
    'Cutter flags must be 0 or 1.': 'As opções do cortador devem ser 0 ou 1.',
    'Calibrate the cutter before enabling tool changes.':
        'Calibre o cortador antes de habilitar as trocas.',
    'Cutter position is uncertain; move the head clear and '
    'confirm retraction with CUTTER_SERVO.':
        'Posição do cortador incerta; afaste o cabeçote e '
        'confirme o recolhimento com CUTTER_SERVO.',
    'Invalid cutter setting: %s.': 'Ajuste do cortador inválido: %s.',
    'Cutter setting must be positive: %s.':
        'O ajuste do cortador deve ser positivo: %s.',
    'Set pre-cut retraction between 0 and 5 mm.':
        'Configure o recuo pré-corte entre 0 e 5 mm.',
    'Pre-cut retraction exceeds the configured buffer allowance.':
        'O recuo pré-corte excede a folga configurada do buffer.',
    'Contact is before the approach position.':
        'O contato está antes da posição de aproximação.',
    'Set the cutting stroke within its limit.':
        'Configure o curso de corte dentro do limite.',
    'Cutter angle is outside the servo limits.':
        'O ângulo do cortador está fora dos limites do servo.',
    'The cutter stop angles must be different.':
        'Os ângulos do batente devem ser diferentes.',
    'Profile feedrate exceeds the extruder limit.':
        'A velocidade do perfil excede o limite da extrusora.',
    'Invalid cutter servo name.': 'Nome do servo do cortador inválido.',
    'Install the servo with --cutter-servo-pin before testing.':
        'Instale o servo com --cutter-servo-pin antes de testar.',
    'Run G28 before testing or operating the cutter.':
        'Execute G28 antes de testar ou acionar o cortador.',
    'Insufficient Z clearance to operate the cutter.':
        'Não há folga Z suficiente para acionar o cortador.',
    'Cutting position is outside the printer travel limits.':
        'A posição de corte está fora dos limites da impressora.',
    'Wait for the BMCU operation before G28.':
        'Aguarde a operação da BMCU antes de G28.',
    'X must finish homing before retracting the stop during G28.':
        'X deve concluir a referência antes de recolher o batente no G28.',
    'Move the head clear of the stop before G28.':
        'Afaste o cabeçote do batente antes de G28.',
    'Move the head clear of the stop before retracting the servo.':
        'Afaste o cabeçote do batente antes de recolher o servo.',
    'Finish the operation before testing the servo.':
        'Finalize a operação antes de testar o servo.',
    'Install bmcu-cortador.cfg before testing.':
        'Instale bmcu-cortador.cfg antes de testar.',
    'Angle is outside the servo limits.':
        'Ângulo fora dos limites do servo.',
    'Invalid servo wait time.': 'Espera do servo inválida.',
    'Retract the stop to recover the tool change.':
        'Recolha o batente para recuperar a troca.',
    'Servo commanded to %.1f degrees; check its physical position.':
        'Servo comandado a %.1f graus; confira a posição física.',
    'Check stop clearance and use CONFIRM=1.':
        'Confira a folga do batente e use CONFIRM=1.',
    'Check the clear path and use CONFIRM=1.':
        'Confira o caminho livre e use CONFIRM=1.',
    'Remove filament before the mechanical test.':
        'Retire o filamento antes do teste mecânico.',
    'Install the cutter profile before testing.':
        'Instale o perfil do cortador antes de testar.',
    'The BMCU restarted or the filament path is no longer clear.':
        'A BMCU reiniciou ou o caminho do filamento deixou de estar livre.',
    'Test completed; inspect cutting and retraction '
    'before setting calibrated=1.':
        'Teste concluído; confira o corte e o recolhimento '
        'antes de definir calibrated=1.',
    'Manually remove filament from the hotend and shared path; '
    'then use RESET_BMCU CONFIRM=1.':
        'Retire manualmente o filamento do hotend e do caminho '
        'compartilhado; depois use RESET_BMCU CONFIRM=1.',
    'Cancel the print before resetting BMCU.':
        'Cancele a impressão antes de resetar a BMCU.',
    'Wait for operations to finish before resetting.':
        'Aguarde o fim das operações antes do reset.',
    'This reset requires exactly one BMCU.':
        'Este reset exige exatamente uma BMCU.',
    'The BMCU or endpoint is not ready.':
        'A BMCU ou o destino não está pronto.',
    'The entry sensor still detects filament.':
        'O sensor de entrada ainda detecta filamento.',
    'A channel is assigned to another endpoint.':
        'Um canal está associado a outro destino.',
    'Physical state changed; reset interrupted.':
        'O estado físico mudou; reset interrompido.',
    'The BMCU is still moving.': 'A BMCU ainda está em movimento.',
    'The four channel sensor readings are missing.':
        'Leitura dos sensores dos quatro canais indisponível.',
    'Invalid BMCU channel sensor readings.':
        'Leitura inválida dos sensores dos canais BMCU.',
    'Could not confirm that all filament paths are clear.':
        'Não foi possível confirmar todos os caminhos livres.',
    'The previous print session still needs recovery.':
        'A sessão anterior ainda precisa de recuperação.',
    'Hotend confirmed unloaded; no filament was moved.':
        'Hotend confirmado descarregado; nenhum filamento foi movido.',
    'The BMCU is not stopped for cutting.':
        'A BMCU não está parada para o corte.',
    'The entry sensor lost filament before cutting.':
        'O filamento saiu do sensor de entrada antes do corte.',
    'The buffer compressed during pre-cut retract.':
        'O buffer comprimiu durante o recuo pré-corte.',
    'Wait for the BMCU operation to finish.':
        'Aguarde o fim da operação da BMCU.',
    'Retract and check the stop before positioning.':
        'Recolha e confira o batente antes de posicionar.',
    'Heat the nozzle before cutting and unloading filament.':
        'Aqueça o bico antes de cortar e retirar o filamento.',
    'The entry sensor detected filament again.':
        'O sensor de entrada voltou a detectar filamento.',
    'Settings': 'Configurações',
    'SETTINGS': 'CONFIGURAÇÕES',
    'Language': 'Idioma',
    'LANGUAGE': 'IDIOMA',
    'FILAMENT / BMCU': 'FILAMENTO / BMCU',
    'Reset BMCU': 'Reset BMCU',
    'RESET BMCU?': 'RESET BMCU?',
    'Manually clear hotend/path': 'Desobstrua hotend/caminho',
    'BMCU path state reconciled.': 'Estado do caminho BMCU reconciliado.',
    'Recover filament': 'Recuperar filamento',
    'RECOVERY': 'RECUPERAÇÃO',
    'Channel unknown': 'Canal desconhecido',
    'Failure details': 'Detalhes da falha',
    'Confirm clear path': 'Confirmar caminho livre',
    'CLEAR FILAMENT PATH?': 'CAMINHO LIVRE?',
    'Park failed channel first': 'Estacione o canal da falha',
    'Filament path confirmed.': 'Caminho do filamento confirmado.',
    'Finish or cancel the print before resetting BMCU.':
        'Conclua ou cancele a impressão antes de resetar a BMCU.',
    'No eligible pending filament recovery.':
        'Nenhuma recuperação de filamento elegível pendente.',
    'Finish or cancel the print before saving configuration.':
        'Conclua ou cancele a impressão antes de salvar os ajustes.',
    'Choose English or Brazilian Portuguese.':
        'Escolha inglês ou português brasileiro.',
    'Configuration storage is unavailable.':
        'Armazenamento da configuração indisponível.',
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
    'Open cutter stop': 'Abrir batente',
    'Close cutter stop': 'Fechar batente',
    'Close stop and home?': 'Fechar e referenciar?',
    'Keep head clear of stop': 'Confira a folga do cabeçote',
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
    'Refill channel %d': 'Repor canal %d',
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
            parts, cursor = [], 0
            for placeholder in re.finditer(r"%(?:\.[0-9]+)?[sdf]|%%", source):
                parts.append(re.escape(source[cursor:placeholder.start()]))
                token = placeholder.group()
                parts.append("%" if token == "%%" else "(.+?)" if token == "%s"
                             else "(-?[0-9]+)" if token.endswith("d")
                             else r"(-?[0-9]+(?:\.[0-9]+)?)")
                cursor = placeholder.end()
            pattern = "".join(parts) + re.escape(source[cursor:])
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
            placeholders = re.findall(r"%(?:\.[0-9]+)?[sdf]", target)
            values = tuple(int(arg) if kind.endswith("d")
                           else float(arg) if kind.endswith("f") else arg
                           for arg, kind in zip(args, placeholders))
            return target % values
    # Klipper template errors wrap the original macro error. Localize its
    # known message while retaining the diagnostic context and source name.
    for source, target in PORTUGUESE.items():
        if "%" not in source and len(source) > 20 and source in text:
            text = text.replace(source, target)
    return text
