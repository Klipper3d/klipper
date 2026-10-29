import logging
import math

from .bmcu_core import protocol
from .bmcu_core.release import PACKAGE_VERSION


class IntegracaoEnder:
    def __init__(self, config):
        if PACKAGE_VERSION != '1.0.6':
            raise config.error('Esta integração exige BMCU-Klipper 1.0.6.')
        self.printer = config.get_printer()
        self.bmcu = self.printer.load_object(config, 'bmcu')
        self.medicao = self.printer.load_object(config, 'bmcu_medicao')
        self.gcode = self.printer.lookup_object('gcode')
        self.reactor = self.printer.get_reactor()
        self.dispositivo = config.get('dispositivo', 'bmcu0')
        self.destino = config.get('destino', 'extruder')
        self.captura = config.getfloat('captura_mm', 20.0, above=0.0)
        self.vel_captura = config.getfloat('captura_f', 180.0, above=0.0)
        self.avanco = config.getfloat('avanco_bico_mm', 80.0, above=0.0)
        self.purga = config.getfloat('purga_mm', 70.0, above=0.0)
        self.vel_avanco = config.getfloat('avanco_f', 120.0, above=0.0)
        self.vel_recuo = config.getfloat('recuo_f', 600.0, above=0.0)
        self.recuo = config.getfloat('recuo_apos_switch_mm', 55.0, above=0.0)
        self.tempo = config.getfloat('tempo_switch', 60.0, above=0.0)
        self.x_purga = config.getfloat('x_purga', -6.0)
        self.x_limpeza = config.getfloat('x_limpeza', -4.0)
        self.y_purga = config.getfloat('y_purga', 123.0)
        self.alivio = config.getfloat('alivio_z', 2.0, above=0.0)
        self.limpezas = config.getint('limpezas', 3, minval=1)
        self.recuos_temporarios = {}
        self.aguardar_recuo_original = self.bmcu._wait_pullback_safe
        self.bmcu._wait_pullback_safe = self.aguardar_recuo
        self.gcode.register_command(
            'BMCU_ENDER_CARREGAR', self.proteger(self.carregar)
        )
        self.gcode.register_command(
            'BMCU_ENDER_LIBERAR', self.proteger(self.liberar)
        )
        self.gcode.register_command(
            'BMCU_TROCAR_ENDER', self.proteger(self.trocar)
        )
        self.printer.register_event_handler('klippy:ready', self.pronto)

    def pronto(self):
        for nome in (
            'BMCU_LOAD',
            'BMCU_UNLOAD',
            'BMCU_TOOL_CHANGE',
            'BMCU_PRINT_END',
        ):
            anterior = self.gcode.register_command(nome, None)
            if anterior is None:
                raise self.printer.config_error(
                    'Comando %s não encontrado.' % nome
                )
            self.gcode.register_command(
                nome, self.proteger(self.adaptar_operacao(anterior))
            )
        for indice in range(4):
            nome = 'T%d' % indice
            anterior = self.gcode.register_command(nome, None)
            if anterior is None:
                anterior = self.tool_nativo(indice)
            self.gcode.register_command(
                nome, self.proteger(self.adaptar_tool(indice, anterior))
            )

    def proteger(self, funcao):
        def executar(gcmd):
            try:
                return funcao(gcmd)
            except Exception as erro:
                raise gcmd.error(str(erro)) from erro

        return executar

    def tool_nativo(self, indice):
        def executar(gcmd):
            return self.gcode.run_script_from_command(
                'BMCU_TOOL_CHANGE TOOL=%d' % indice
            )

        return executar

    def restaurar_recuos(self):
        for nome in tuple(self.recuos_temporarios):
            dispositivo = self.bmcu.devices_by_name[nome]
            dispositivo.stop_all()
            self.bmcu._send_device_runtime_config(dispositivo)
            self.recuos_temporarios.pop(nome)

    def aguardar_recuo(self, dispositivo, canal, *args, **kwargs):
        registro = self.recuos_temporarios.get(dispositivo.name)
        if registro is None or registro['canal'] != canal:
            return self.aguardar_recuo_original(
                dispositivo, canal, *args, **kwargs
            )
        kwargs['start_m'] = registro['inicio']
        resultado = self.aguardar_recuo_original(
            dispositivo, canal, *args, **kwargs
        )
        self.reactor.pause(self.reactor.monotonic() + 0.150)
        dispositivo.refresh()
        medido = (
            registro['inicio'] - float(dispositivo.status['meters'][canal])
        ) * 1000.0
        try:
            if dispositivo.status['session_id'] != registro['sessao']:
                raise RuntimeError('A BMCU reiniciou durante o recolhimento.')
            if not math.isfinite(medido) or medido + 0.1 < self.recuo:
                raise RuntimeError(
                    (
                        'O encoder confirmou apenas %.2f mm após o '
                        'switch; o outro canal não será '
                        'carregado.'
                    )
                    % medido
                )
            if self.sensor(self.bmcu.endpoints[self.destino]):
                raise RuntimeError(
                    (
                        'O switch voltou a detectar filamento '
                        'durante o '
                        'recolhimento.'
                    )
                )
        except Exception:
            dispositivo.stop_all()
            raise
        resultado = dict(resultado)
        resultado['encoder_mm'] = medido
        resultado['after_switch_encoder_mm'] = medido
        self.gcode.respond_info(
            (
                'Canal %d: recolhimento após o switch '
                'confirmado pelo encoder: %.2f '
                'mm.'
            )
            % (canal + 1, medido)
        )
        return resultado

    def adaptar_operacao(self, anterior):
        def executar(gcmd):
            try:
                return anterior(gcmd)
            finally:
                self.restaurar_recuos()

        return executar

    def adaptar_tool(self, indice, anterior):
        def executar(gcmd):
            if 'B' not in gcmd.get_command_parameters():
                if not self.bmcu.print_map_active:
                    return anterior(gcmd)
                ferramenta = indice + 1
                if not self.bmcu.print_tools.get(str(ferramenta)):
                    raise gcmd.error(
                        'A ferramenta %d do Orca não consta no mapa de '
                        'impressão BMCU confirmado.' % ferramenta
                    )
            else:
                ferramenta = gcmd.get_int('B', minval=1, maxval=4)
            if ferramenta != indice + 1:
                raise gcmd.error(
                    ('O índice do Orca e o canal BMCU não ' 'correspondem.')
                )
            return self.gcode.run_script_from_command(
                'BMCU_TROCAR_ENDER TOOL=%d' % ferramenta
            )

        return executar

    def executar(self, texto):
        with self.bmcu.printer_critical_section('integracao_ender'):
            self.gcode.run_script_from_command(texto)

    def sensor(self, destino):
        estado = destino.sensor_status('entry_sensor')
        if not estado or estado.get('filament_detected') not in (True, False):
            raise RuntimeError(
                ('O switch de chegada não informou uma ' 'leitura ' 'válida.')
            )
        return bool(estado['filament_detected'])

    def contexto(self, gcmd, carregar):
        dispositivo = self.bmcu.devices_by_name.get(self.dispositivo)
        if dispositivo is None or not dispositivo.ready:
            raise RuntimeError('A BMCU não está pronta.')
        operacao = self.bmcu.active_operations.get(dispositivo.name)
        if not operacao:
            raise RuntimeError(
                (
                    'Este comando só pode ser chamado pela '
                    'operação nativa da '
                    'BMCU.'
                )
            )
        destino = self.bmcu.endpoints.get(self.destino)
        if destino is None or destino.name != self.destino:
            raise RuntimeError('Destino da extrusora incompatível.')
        if gcmd.get('ENDPOINT', self.destino) != self.destino:
            raise RuntimeError('A operação pertence a outra extrusora.')
        dispositivo.refresh()
        if carregar:
            contexto = getattr(destino, '_ender_load_context', None)
            if (
                not contexto
                or contexto[0] != dispositivo.name
                or operacao.get('phase') != 'TOOLHEAD_PREPARATION'
            ):
                raise RuntimeError(
                    (
                        'A etapa de captura com a extrusora '
                        'primeiro não está '
                        'instalada.'
                    )
                )
            canal = int(contexto[1])
        else:
            canal = int(dispositivo.status.get('now_channel', -1))
        if not 0 <= canal < 4:
            raise RuntimeError(
                'A BMCU não informou qual canal está em operação.'
            )
        esperado = (
            protocol.MOTION_IDLE
            if carregar
            else protocol.MOTION_BEFORE_PULL_BACK
        )
        if int(dispositivo.status['motion'][canal]) != esperado:
            raise RuntimeError(
                (
                    'A etapa nativa de alimentação não '
                    'corresponde ao '
                    'comando.'
                )
            )
        if (
            self.bmcu._endpoint_for_channel(dispositivo, canal).name
            != destino.name
        ):
            raise RuntimeError('Canal associado a outra extrusora.')
        return dispositivo, destino, canal

    def verificar_sessao(self, dispositivo, sessao, canal):
        dispositivo.refresh()
        if dispositivo.status['session_id'] != sessao:
            raise RuntimeError('A BMCU reiniciou durante a troca.')
        if not dispositivo.status['present'][canal]:
            raise RuntimeError('O canal deixou de detectar filamento.')

    def posicionar(self):
        toolhead = self.printer.lookup_object('toolhead')
        estado = toolhead.get_status(self.reactor.monotonic())
        if not all(eixo in estado['homed_axes'] for eixo in 'xyz'):
            raise RuntimeError(
                ('Execute G28 antes de carregar ou retirar ' 'filamento.')
            )
        z = min(
            toolhead.get_position()[2] + self.alivio, estado['axis_maximum'][2]
        )
        self.executar(
            (
                'M400\nG90\nG1 Z%.6f F300\nG1 X%.6f F1200\n'
                'G1 Y%.6f F1200\n'
                'M400'
            )
            % (z, self.x_purga, self.y_purga)
        )
        extrusora = self.printer.lookup_object('extruder')
        estado = extrusora.get_status(self.reactor.monotonic())
        if not estado.get('can_extrude'):
            raise RuntimeError('Aqueça o bico antes da troca de filamento.')

    def mover_filamento(self, mm, velocidade, bloco=10.0):
        extrusora = self.printer.lookup_object('extruder')
        limite = min(bloco, extrusora.max_e_dist)
        if limite <= 0:
            raise RuntimeError(
                'O limite de extrusão está configurado como zero.'
            )
        quantidade = int(math.ceil(abs(mm) / limite))
        if quantidade:
            for _ in range(quantidade):
                self.executar(
                    'G1 E%.9f F%.9f\nM400' % (mm / quantidade, velocidade)
                )

    def limpar(self, dispositivo, destino, canal, sessao):
        dispositivo.set_motion(canal, protocol.MOTION_ON_USE)
        restante = self.purga
        while restante > 0.000001:
            self.verificar_sessao(dispositivo, sessao, canal)
            if not self.sensor(destino):
                raise RuntimeError('O switch liberou durante a purga.')
            passo = min(10.0, restante)
            self.mover_filamento(passo, self.vel_avanco)
            restante -= passo
        self.executar('G4 P1000')
        for _ in range(self.limpezas):
            self.executar(
                'G1 X%.6f F600\nG1 X%.6f F600\nM400'
                % (self.x_limpeza, self.x_purga)
            )
        self.executar('G1 X%.6f F600\nM400' % self.x_limpeza)

    def carregar(self, gcmd):
        dispositivo = None
        salvo = False
        concluido = False
        try:
            dispositivo, destino, canal = self.contexto(gcmd, True)
            sessao = dispositivo.status['session_id']
            dispositivo.stop_all()
            self.restaurar_recuos()
            self.executar('SAVE_GCODE_STATE NAME=BMCU_ENDER_CARGA')
            salvo = True
            self.posicionar()
            self.executar('M83\nM220 S100\nM221 S100')
            if not self.sensor(destino):
                raise RuntimeError(
                    'O filamento não acionou o switch de chegada.'
                )
            self.medicao._capturar_extrusora_primeiro(
                dispositivo,
                destino,
                canal,
                self.captura,
                self.vel_captura,
                sensor_gerenciado=True,
            )
            restante = self.avanco
            while restante > 0.000001:
                self.verificar_sessao(dispositivo, sessao, canal)
                if not self.sensor(destino):
                    raise RuntimeError('O switch liberou durante a carga.')
                dispositivo.set_motion(canal, protocol.MOTION_BEFORE_ON_USE)
                self.mover_filamento(min(10.0, restante), self.vel_avanco)
                dispositivo.stop_all()
                restante -= min(10.0, restante)
            self.limpar(dispositivo, destino, canal, sessao)
            concluido = True
            gcmd.respond_info(
                (
                    'Canal %d: captura %.1f mm, avanço %.1f mm '
                    'e purga %.1f mm '
                    'concluídos.'
                )
                % (canal + 1, self.captura, self.avanco, self.purga)
            )
        except Exception as erro:
            raise gcmd.error('Carga interrompida: %s' % erro) from erro
        finally:
            try:
                if dispositivo is not None and not concluido:
                    dispositivo.stop_all()
            finally:
                if salvo:
                    self.executar('RESTORE_GCODE_STATE NAME=BMCU_ENDER_CARGA')

    def liberar(self, gcmd):
        dispositivo = None
        salvo = False
        try:
            dispositivo, destino, canal = self.contexto(gcmd, False)
            sessao = dispositivo.status['session_id']
            self.executar('SAVE_GCODE_STATE NAME=BMCU_ENDER_RETIRADA')
            salvo = True
            self.posicionar()
            self.executar('M83\nM220 S100\nM221 S100')
            if not self.sensor(destino):
                raise RuntimeError(
                    'O switch já estava livre antes da retirada.'
                )
            marco = float(dispositivo.status['meters'][canal])
            prazo = self.reactor.monotonic() + self.tempo
            total = 0.0
            while self.sensor(destino):
                if self.reactor.monotonic() >= prazo:
                    raise RuntimeError(
                        'O switch não liberou no prazo configurado.'
                    )
                self.mover_filamento(-2.0, self.vel_recuo, bloco=2.0)
                total += 2.0
                self.verificar_sessao(dispositivo, sessao, canal)
                atual = float(dispositivo.status['meters'][canal])
                if (marco - atual) * 1000.0 >= 25.0 and self.sensor(destino):
                    dispositivo.stop_all()
                    self.reactor.pause(self.reactor.monotonic() + 0.050)
                    dispositivo.set_motion(
                        canal, protocol.MOTION_BEFORE_PULL_BACK
                    )
                    marco = atual
            gcmd.respond_info(
                (
                    'Canal %d: switch livre após %.1f mm de '
                    'extrusora; a BMCU recolherá mais %.1f '
                    'mm.'
                )
                % (canal + 1, total, self.recuo)
            )
            dispositivo.stop_all()
            self.reactor.pause(self.reactor.monotonic() + 0.050)
            dispositivo.refresh()
            self.recuos_temporarios[dispositivo.name] = {
                'canal': canal,
                'inicio': float(dispositivo.status['meters'][canal]),
                'sessao': sessao,
            }
            self.bmcu._send_device_runtime_config(
                dispositivo, channel_retract_override=(canal, self.recuo)
            )
        except Exception as erro:
            raise gcmd.error('Retirada interrompida: %s' % erro) from erro
        finally:
            try:
                if dispositivo is not None:
                    dispositivo.stop_all()
                    self.reactor.pause(self.reactor.monotonic() + 0.050)
            finally:
                if salvo:
                    self.executar(
                        'RESTORE_GCODE_STATE NAME=BMCU_ENDER_RETIRADA'
                    )

    def trocar(self, gcmd):
        ferramenta = gcmd.get_int('TOOL', minval=1, maxval=4)
        purgar = gcmd.get_int('PURGAR_SE_CARREGADO', 0, minval=0, maxval=1)
        cabecote = self.printer.lookup_object('toolhead').get_status(
            self.reactor.monotonic()
        )
        if not all(eixo in cabecote['homed_axes'] for eixo in 'xyz'):
            raise gcmd.error('Execute G28 antes de trocar o filamento.')
        if (
            not self.printer.lookup_object('extruder')
            .get_status(self.reactor.monotonic())
            .get('can_extrude')
        ):
            raise gcmd.error('Aqueça o bico antes de trocar o filamento.')
        estado = self.printer.lookup_object('gcode_move').get_status(
            self.reactor.monotonic()
        )
        posicao = list(estado['gcode_position'])
        atual = self.bmcu.get_status(self.reactor.monotonic()).get(
            'loaded_tool'
        )
        self.executar('M400\nSAVE_GCODE_STATE NAME=BMCU_ENDER_TROCA')
        sucesso = False
        try:
            self.gcode.run_script_from_command(
                'BMCU_TOOL_CHANGE TOOL=%d' % ferramenta
            )
            if purgar and atual == ferramenta:
                dispositivo = self.bmcu.devices_by_name[self.dispositivo]
                canal = ferramenta - 1
                destino = self.bmcu._endpoint_for_channel(dispositivo, canal)
                dispositivo.refresh()
                self.posicionar()
                self.executar('M83\nM220 S100\nM221 S100')
                self.limpar(
                    dispositivo,
                    destino,
                    canal,
                    dispositivo.status['session_id'],
                )
            self.executar(
                'G90\nG1 X%.9f Y%.9f F6000\nG1 Z%.9f F300\nM400'
                % tuple(posicao[:3])
            )
            sucesso = True
        finally:
            if not sucesso:
                try:
                    self.bmcu.devices_by_name[self.dispositivo].stop_all()
                except Exception:
                    logging.exception(
                        'Não foi possível confirmar a parada da BMCU.'
                    )
            self.executar('RESTORE_GCODE_STATE NAME=BMCU_ENDER_TROCA')


def load_config(config):
    return IntegracaoEnder(config)
