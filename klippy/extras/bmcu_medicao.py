import logging
import math

from .bmcu_core import protocol
from .bmcu_core.release import PACKAGE_VERSION


class MedicaoChegada:
    def __init__(self, config):
        if PACKAGE_VERSION != '1.0.6':
            raise config.error('A medição exige BMCU-Klipper 1.0.6.')
        self.gerenciador = config.get_printer().load_object(config, 'bmcu')
        self.captura = None
        self.falha_captura = False
        self.gerenciador.gcode.register_command(
            'BMCU_MEDIR_CHEGADA', self.medir,
            desc='Avança até o sensor e mede a distância restante.')
        self.gerenciador.gcode.register_command(
            'BMCU_TESTAR_EXTRUSAO', self.testar_extrusao,
            desc='Testa o avanço da extrusora com alimentação assistida.')
        self.gerenciador.gcode.register_command(
            'BMCU_LIMITES_EXTRUSAO', self.limites_extrusao,
            desc='Mostra os limites configurados da extrusora.')

    def medir(self, gcmd):
        try:
            return self._medir(gcmd)
        except (RuntimeError, OSError) as erro:
            raise gcmd.error('Não foi possível medir a chegada: %s' % erro) from erro

    def _medir(self, gcmd):
        gerenciador = self.gerenciador
        gerenciador._require_standalone_operation('BMCU_MEDIR_CHEGADA', gcmd)
        canal = gcmd.get_int('CANAL', minval=1, maxval=4) - 1
        limite = gerenciador._finite_gcmd_float(
            gcmd, 'LIMITE', 1500.0, minval=10.0, maxval=1500.0)
        if not gcmd.get_int('CAMINHO_LIVRE', 0, minval=0, maxval=1):
            raise gcmd.error('Retire o filamento anterior da extrusora e confirme CAMINHO_LIVRE=1.')
        if (gerenciador._print_state() in ('printing', 'paused', 'pause')
                or gerenciador.print_plan_open or gerenciador.print_map_active
                or gerenciador.active_operations or gerenciador.prestaged):
            raise gcmd.error('A impressora e a BMCU precisam estar ociosas.')
        dispositivo = gerenciador._require_device(gcmd)
        destino = gerenciador._endpoint_for_channel(dispositivo, canal)
        if destino is None or destino.driver != 'generic_single_extruder':
            raise gcmd.error('O canal precisa estar associado à extrusora genérica.')
        gerenciador._check_automatic_ready(dispositivo, canal)
        dispositivo.refresh()
        gerenciador._check_automatic_ready(dispositivo, canal)
        gerenciador._require_exclusive_endpoint_route(dispositivo, destino, canal)
        if gerenciador._route_state(dispositivo, canal) != protocol.ROUTE_EMPTY:
            raise gcmd.error('O canal precisa estar EMPTY antes da medição.')
        if any(dispositivo.status.get('motion', [])):
            raise gcmd.error('Há movimento ativo na BMCU.')
        estado_sensor = destino.sensor_status('entry_sensor')
        if not estado_sensor or not estado_sensor.get('enabled', True):
            raise gcmd.error('O sensor de chegada não está disponível e habilitado.')
        if estado_sensor.get('filament_detected') is not False:
            raise gcmd.error('O sensor de chegada precisa estar livre antes da medição.')
        if dispositivo.status['buffer_pct'][canal] >= 70:
            raise gcmd.error('Solte o buffer do canal antes da medição.')
        gerenciador._lock(dispositivo, destino, 'MEDICAO_CHEGADA', channel=canal)
        velocidade_anterior = None
        configuracao_alterada = False
        try:
            velocidade_anterior = dispositivo.config_get(protocol.CONFIG_LOAD_SPEED_MMS)
            configuracao_alterada = True
            dispositivo.config_set(protocol.CONFIG_LOAD_SPEED_MMS, 10.0)
            prazo = limite / 10.0 + 10.0
            gcmd.respond_info('Canal %d: avanço a 10 mm/s até o switch; limite %.0f mm.'
                              % (canal + 1, limite))
            operacao = dispositivo.start_feed_distance(canal, limite, 70, int(prazo * 1000))
            resultado = gerenciador._wait_feed_operation(
                dispositivo, operacao, prazo + 2.0, destino, 'entry_sensor',
                poll_interval=0.050)
            medido = float(resultado.get('measured_mm', float('nan')))
            if (not resultado.get('ok') or not resultado.get('sensor_triggered')
                    or not math.isfinite(medido) or medido < 0):
                raise gcmd.error('A chegada não foi confirmada: %s; avanço medido: %s mm.'
                                 % (resultado.get('reason'), medido))
        except Exception as erro:
            raise gcmd.error('Medição interrompida: %s' % erro) from erro
        finally:
            try:
                if configuracao_alterada:
                    try:
                        dispositivo.stop_all()
                    except Exception:
                        logging.exception('Não foi possível confirmar a parada da BMCU.')
                        raise gcmd.error('Não foi possível confirmar a parada; desligue a alimentação da BMCU.')
                    dispositivo.config_set(protocol.CONFIG_LOAD_SPEED_MMS, velocidade_anterior)
            finally:
                gerenciador._unlock(dispositivo, destino)
        gerenciador.last_diagnostic = {
            'operation': 'medicao_chegada', 'device': dispositivo.name,
            'channel': canal, 'endpoint': destino.name,
            'measured_mm': medido, 'sensor_triggered': True,
            'origem': 'posicao_atual',
        }
        gcmd.respond_info('Switch acionado. Avanço restante medido no canal %d: %.2f mm.'
                          % (canal + 1, medido))
        self.captura = None
        self.falha_captura = False

    def testar_extrusao(self, gcmd):
        try:
            return self._testar_extrusao(gcmd)
        except (RuntimeError, OSError) as erro:
            raise gcmd.error('Não foi possível testar a extrusão: %s' % erro) from erro

    def _testar_extrusao(self, gcmd):
        gerenciador = self.gerenciador
        gerenciador._require_standalone_operation('BMCU_TESTAR_EXTRUSAO', gcmd)
        canal = gcmd.get_int('CANAL', minval=1, maxval=4) - 1
        avanco = gerenciador._finite_gcmd_float(
            gcmd, 'MM', 5.0, above=0.0)
        extrusora_primeiro = gcmd.get_int('EXTRUSORA_PRIMEIRO', 0, minval=0, maxval=1)
        velocidade = gerenciador._finite_gcmd_float(
            gcmd, 'VELOCIDADE', 120.0 if extrusora_primeiro else 180.0, above=0.0)
        if (gerenciador._print_state() in ('printing', 'paused', 'pause')
                or gerenciador.print_plan_open or gerenciador.print_map_active
                or gerenciador.active_operations or gerenciador.prestaged):
            raise gcmd.error('A impressora e a BMCU precisam estar ociosas.')
        dispositivo = gerenciador._require_device(gcmd)
        destino = gerenciador._endpoint_for_channel(dispositivo, canal)
        if destino is None or destino.driver != 'generic_single_extruder':
            raise gcmd.error('O canal precisa estar associado à extrusora genérica.')
        dispositivo.refresh()
        gerenciador._check_automatic_ready(dispositivo, canal, allow_uncertain=True)
        gerenciador._require_exclusive_endpoint_route(dispositivo, destino, canal)
        if gerenciador._route_state(dispositivo, canal) != protocol.ROUTE_UNCERTAIN:
            raise gcmd.error('Este teste exige a rota UNCERTAIN deixada pela chegada ao switch.')
        if any(dispositivo.status.get('motion', [])):
            raise gcmd.error('Há movimento ativo na BMCU.')
        sensor = destino.sensor_status('entry_sensor')
        if (not sensor or not sensor.get('enabled', True)
                or sensor.get('filament_detected') is not True):
            raise gcmd.error('A ponta precisa estar acionando o switch de chegada.')
        extrusora = gerenciador.printer.lookup_object(destino.get('extruder', 'extruder'))
        if not extrusora.get_status(gerenciador.reactor.monotonic()).get('can_extrude'):
            raise gcmd.error('Aqueça o bico à temperatura adequada ao material antes do teste.')
        identificacao = (dispositivo.name, dispositivo.status['session_id'], canal, destino.name)
        self.falha_captura = False
        gerenciador._lock(dispositivo, destino, 'TESTE_EXTRUSAO', channel=canal)
        velocidade_anterior = None
        configuracao_alterada = False
        try:
            velocidade_anterior = dispositivo.config_get(protocol.CONFIG_LOAD_SPEED_MMS)
            configuracao_alterada = True
            dispositivo.config_set(protocol.CONFIG_LOAD_SPEED_MMS, 10.0)
            if extrusora_primeiro:
                self._capturar_extrusora_primeiro(dispositivo, destino, canal, avanco, velocidade)
            else:
                dispositivo.set_motion(canal, protocol.MOTION_BEFORE_ON_USE)
                with gerenciador.printer_critical_section('teste_extrusao'):
                    gerenciador.gcode.run_script_from_command('SAVE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO')
                    try:
                        gerenciador.gcode.run_script_from_command(
                            'M83\nM220 S100\nM221 S100')
                        self._avancar_extrusora(avanco, velocidade, extrusora)
                        gerenciador.gcode.run_script_from_command('M400')
                    finally:
                        gerenciador.gcode.run_script_from_command('RESTORE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO')
        except Exception as erro:
            self.falha_captura = True
            raise gcmd.error('Teste interrompido; a contagem não é válida: %s' % erro) from erro
        finally:
            try:
                if configuracao_alterada:
                    try:
                        dispositivo.stop_all()
                        dispositivo.config_set(protocol.CONFIG_LOAD_SPEED_MMS, velocidade_anterior)
                    except Exception as erro:
                        self.falha_captura = True
                        raise gcmd.error('Falha ao encerrar a alimentação assistida: %s' % erro) from erro
            finally:
                gerenciador._unlock(dispositivo, destino)
        self.captura = {'identificacao': identificacao, 'total_mm': avanco}
        gerenciador.last_diagnostic = {
            'operation': 'teste_extrusao', 'device': dispositivo.name,
            'channel': canal, 'endpoint': destino.name,
            'commanded_mm': avanco,
            'requested_feedrate': velocidade,
            'extruder_first': bool(extrusora_primeiro),
        }
        gcmd.respond_info('Canal %d: tentativa de %.3f mm a F%.3f; limite configurado da extrusora F%.3f.'
                          % (canal + 1, avanco, velocidade, extrusora.max_e_velocity * 60.0))

    def limites_extrusao(self, gcmd):
        extrusora = self.gerenciador.printer.lookup_object('extruder')
        gcmd.respond_info('Limite configurado: %.3f mm/s, VELOCIDADE=%.3f mm/min; máximo por movimento interno: %.3f mm.'
                          % (extrusora.max_e_velocity, extrusora.max_e_velocity * 60.0,
                             extrusora.max_e_dist))

    def _avancar_extrusora(self, avanco, velocidade, extrusora):
        if extrusora.max_e_dist <= 0.0:
            raise RuntimeError('max_extrude_only_distance está configurado como zero.')
        quantidade = math.ceil(avanco / extrusora.max_e_dist)
        for _ in range(quantidade):
            self.gerenciador.gcode.run_script_from_command(
                'G1 E%.9f F%.9f' % (avanco / quantidade, velocidade))

    def _capturar_extrusora_primeiro(self, dispositivo, destino, canal, avanco, velocidade, sensor_gerenciado=False):
        gerenciador = self.gerenciador
        executar = gerenciador.gcode.run_script_from_command
        cabecote = gerenciador.printer.lookup_object('toolhead')
        extrusora = gerenciador.printer.lookup_object(destino.get('extruder', 'extruder'))
        if extrusora.max_e_dist <= 0.0:
            raise RuntimeError('max_extrude_only_distance está configurado como zero.')
        velocidade_real = min(velocidade / 60.0, extrusora.max_e_velocity)
        inicial = min(1.0, avanco / 4.0, velocidade_real * 0.5, extrusora.max_e_dist)
        seguinte = min(avanco - inicial, velocidade_real * 1.5, extrusora.max_e_dist)
        restante = avanco - inicial - seguinte
        executar('M400\nSAVE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO')
        try:
            tempos = []
            executar('M83\nM220 S100\nM221 S100\nG1 E%.9f F%.9f' % (inicial, velocidade))
            cabecote.register_lookahead_callback(tempos.append)
            executar('G1 E%.9f F%.9f' % (seguinte, velocidade))
            cabecote.register_lookahead_callback(tempos.append)
            cabecote.flush_step_generation()
            if len(tempos) != 2 or not all(math.isfinite(t) for t in tempos):
                raise RuntimeError('Não foi possível obter os tempos do avanço da extrusora.')
            prazo = gerenciador.reactor.monotonic() + inicial / velocidade_real + 5.0
            while True:
                agora = gerenciador.reactor.monotonic()
                if agora >= prazo:
                    raise RuntimeError('A extrusora não iniciou no prazo esperado.')
                if cabecote.mcu.estimated_print_time(agora) >= tempos[0]:
                    break
                gerenciador.reactor.pause(agora + 0.010)
            if cabecote.mcu.estimated_print_time(agora) >= tempos[1]:
                raise RuntimeError('A janela de giro terminou antes da assistência da BMCU.')
            sensor = destino.sensor_status('entry_sensor')
            if (not sensor or (not sensor_gerenciado and not sensor.get('enabled', True))
                    or sensor.get('filament_detected') is not True):
                raise RuntimeError('O switch deixou de detectar o filamento durante a captura.')
            dispositivo.set_motion(canal, protocol.MOTION_BEFORE_ON_USE)
            if cabecote.mcu.estimated_print_time(gerenciador.reactor.monotonic()) >= tempos[1]:
                raise RuntimeError('A BMCU respondeu após a janela inicial de giro da extrusora.')
            if restante > 0.0:
                self._avancar_extrusora(restante, velocidade, extrusora)
                cabecote.register_lookahead_callback(tempos.append)
                cabecote.flush_step_generation()
            fim = tempos[-1]
            if not math.isfinite(fim):
                raise RuntimeError('Não foi possível obter o término do avanço da extrusora.')
            momento_parada = fim - min(2.0, avanco / 2.0) / velocidade_real
            prazo = gerenciador.reactor.monotonic() + avanco / velocidade_real + 5.0
            while True:
                agora = gerenciador.reactor.monotonic()
                if agora >= prazo:
                    raise RuntimeError('Não foi possível acompanhar o avanço para parar a BMCU.')
                if cabecote.mcu.estimated_print_time(agora) >= momento_parada:
                    break
                gerenciador.reactor.pause(agora + 0.010)
            dispositivo.stop_all()
            if cabecote.mcu.estimated_print_time(gerenciador.reactor.monotonic()) >= fim:
                raise RuntimeError('Não foi possível confirmar a parada da BMCU antes do fim do avanço.')
            with gerenciador.printer_critical_section('teste_captura'):
                executar('M400')
        finally:
            executar('RESTORE_GCODE_STATE NAME=BMCU_MEDICAO_EXTRUSAO')


class RetiradaAssistida:
    def __init__(self, medicao):
        self.gerenciador = medicao.gerenciador
        self.gerenciador.gcode.register_command(
            'BMCU_RETIRAR_ATE_SWITCH', self.executar,
            desc='Retira com assistência até liberar o switch e recolhe o restante.')

    def sensor(self, destino):
        estado = destino.sensor_status('entry_sensor')
        if not estado or not estado.get('enabled', True):
            raise RuntimeError('O sensor de chegada precisa estar disponível e habilitado.')
        valor = estado.get('filament_detected')
        if valor is not True and valor is not False:
            raise RuntimeError('O sensor não informou um estado válido.')
        return valor

    def executar(self, gcmd):
        gerenciador = self.gerenciador
        gerenciador._require_standalone_operation('BMCU_RETIRAR_ATE_SWITCH', gcmd)
        canal = gcmd.get_int('CANAL', 2, minval=1, maxval=4) - 1
        velocidade = gerenciador._finite_gcmd_float(gcmd, 'VELOCIDADE', 600.0, above=0.0)
        depois = gerenciador._finite_gcmd_float(gcmd, 'APOS', 55.0, above=0.0)
        tempo = gerenciador._finite_gcmd_float(gcmd, 'TEMPO', 60.0, above=0.0)
        if (gerenciador._print_state() in ('printing', 'paused', 'pause')
                or gerenciador.print_plan_open or gerenciador.print_map_active
                or gerenciador.active_operations or gerenciador.prestaged):
            raise gcmd.error('Este teste exige a impressora e a BMCU ociosas.')
        dispositivo = gerenciador._require_device(gcmd)
        destino = gerenciador._endpoint_for_channel(dispositivo, canal)
        if destino is None or destino.driver != 'generic_single_extruder':
            raise gcmd.error('O canal precisa estar associado à extrusora genérica.')
        dispositivo.refresh()
        gerenciador._check_automatic_ready(dispositivo, canal, allow_uncertain=True)
        gerenciador._require_exclusive_endpoint_route(dispositivo, destino, canal)
        if gerenciador._route_state(dispositivo, canal) not in (protocol.ROUTE_LOADED, protocol.ROUTE_UNCERTAIN):
            raise gcmd.error('O canal precisa estar carregado ou com retirada incompleta.')
        extrusora = gerenciador.printer.lookup_object(destino.get('extruder', 'extruder'))
        if not extrusora.get_status(gerenciador.reactor.monotonic()).get('can_extrude'):
            raise gcmd.error('Aqueça o bico à temperatura do material antes da retirada.')
        if extrusora.max_e_dist <= 0:
            raise gcmd.error('O limite de movimento da extrusora está configurado como zero.')
        try:
            if not self.sensor(destino):
                raise gcmd.error('O switch já está livre. Não há uma nova liberação para medir.')
        except RuntimeError as erro:
            raise gcmd.error(str(erro)) from erro

        executar = gerenciador.gcode.run_script_from_command
        configuracao = {}
        estado_salvo = False
        erro_principal = None
        total_extrusora = 0.0
        medido = 0.0
        sessao = dispositivo.status['session_id']
        gerenciador._lock(dispositivo, destino, 'RETIRADA_ATE_SWITCH', channel=canal)
        try:
            executar('M400')
            executar('SAVE_GCODE_STATE NAME=BMCU_RETIRADA_ASSISTIDA')
            estado_salvo = True
            executar('M83\nM220 S100\nM221 S100')
            configuracao[protocol.CONFIG_PULL_SPEED_MMS] = dispositivo.config_get(protocol.CONFIG_PULL_SPEED_MMS)
            configuracao[protocol.CONFIG_PULL_SPEED_END_MMS] = dispositivo.config_get(protocol.CONFIG_PULL_SPEED_END_MMS)
            dispositivo.set_motion(canal, protocol.MOTION_BEFORE_PULL_BACK)
            prazo = gerenciador.reactor.monotonic() + tempo
            passo = min(2.0, extrusora.max_e_dist)
            gcmd.respond_info('Canal %d: extrusora recuando até liberar o switch, com assistência da BMCU.' % (canal + 1))
            while self.sensor(destino):
                if gerenciador.reactor.monotonic() >= prazo:
                    raise RuntimeError('O switch não liberou em %.1f segundos.' % tempo)
                with gerenciador.printer_critical_section('retirada_ate_switch'):
                    executar('G1 E-%.9f F%.9f\nM400' % (passo, velocidade))
                total_extrusora += passo
                dispositivo.refresh()
                if dispositivo.status['session_id'] != sessao:
                    raise RuntimeError('A BMCU reiniciou durante a retirada.')

            dispositivo.stop_all()
            executar('M400')
            dispositivo.refresh()
            inicio = float(dispositivo.status['meters'][canal])
            gcmd.respond_info('Switch livre. Extrusora parada; recolhendo mais %.1f mm pela BMCU.' % depois)
            dispositivo.config_set(protocol.CONFIG_PULL_SPEED_END_MMS, 4.0)
            dispositivo.config_set(protocol.CONFIG_PULL_SPEED_MMS, 10.0)
            dispositivo.set_motion(canal, protocol.MOTION_PULL_BACK)
            prazo = gerenciador.reactor.monotonic() + tempo
            while True:
                dispositivo.refresh()
                if dispositivo.status['session_id'] != sessao:
                    raise RuntimeError('A BMCU reiniciou durante o recolhimento.')
                medido = (inicio - float(dispositivo.status['meters'][canal])) * 1000.0
                if not math.isfinite(medido) or medido < -2.0:
                    raise RuntimeError('O encoder não confirmou o sentido da retirada.')
                if self.sensor(destino):
                    raise RuntimeError('O switch voltou a detectar filamento durante o recolhimento.')
                if medido >= depois:
                    break
                if gerenciador.reactor.monotonic() >= prazo:
                    raise RuntimeError('O encoder mediu apenas %.2f mm após liberar o switch.' % medido)
                gerenciador.reactor.pause(gerenciador.reactor.monotonic() + 0.050)
        except Exception as erro:
            erro_principal = erro
        finally:
            try:
                try:
                    dispositivo.stop_all()
                except Exception as erro:
                    logging.exception('Não foi possível confirmar a parada da BMCU.')
                    erro_principal = RuntimeError('Parada da BMCU não confirmada: %s' % erro)
                for chave, valor in configuracao.items():
                    try:
                        dispositivo.config_set(chave, valor)
                    except Exception as erro:
                        logging.exception('Não foi possível restaurar a velocidade da BMCU.')
                        if erro_principal is None:
                            erro_principal = erro
                if estado_salvo:
                    try:
                        executar('M400\nRESTORE_GCODE_STATE NAME=BMCU_RETIRADA_ASSISTIDA')
                    except Exception as erro:
                        if erro_principal is None:
                            erro_principal = erro
            finally:
                gerenciador._unlock(dispositivo, destino)
        if erro_principal is not None:
            raise gcmd.error('Retirada interrompida; não carregue outro canal ainda: %s' % erro_principal) from erro_principal
        executar('BMCU_ROUTE_CONFIRM DEVICE=%s CHANNEL=%d STATE=PARKED' % (dispositivo.name, canal))
        gerenciador.last_diagnostic = {
            'operation': 'retirada_ate_switch', 'device': dispositivo.name,
            'channel': canal, 'commanded_extruder_mm': total_extrusora,
            'retracted_after_switch_mm': medido, 'sensor_triggered': False,
        }
        gcmd.respond_info('Canal %d estacionado. Recuo comandado nesta execução: %.2f mm; recolhimento após o switch: %.2f mm.' % (canal + 1, total_extrusora, medido))


def load_config(config):
    medicao = MedicaoChegada(config)
    medicao.retirada_assistida = RetiradaAssistida(medicao)
    return medicao
