import math
import re


class CortadorEnder:
    def __init__(self, integracao):
        self.integracao = integracao
        self.recolhido = False
        self.incerto = False
        self.ultimo_corte = {}

    def perfil(self):
        macro = self.integracao.printer.lookup_object(
            'gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES', None
        )
        if macro is None:
            return None
        return dict(macro.get_status(self.integracao.reactor.monotonic()))

    def preparar(self, calibracao=False):
        perfil = self.perfil()
        if perfil is None or not perfil.get('habilitado') and not calibracao:
            return None
        perfil.setdefault('soltar_servo_recolhido', 1)
        perfil.setdefault('manter_y_corte', 0)
        perfil.setdefault('recuo_antes_corte_mm', 0.0)
        if perfil.get('x_contato') is None:
            perfil['x_contato'] = perfil.get('x_aproximacao')
        if any(
            perfil.get(chave) not in (0, 1)
            for chave in (
                'habilitado', 'calibrado', 'posicionamento_paralelo',
                'soltar_servo_recolhido', 'manter_y_corte',
            )
        ):
            raise RuntimeError('As opcoes do cortador devem ser 0 ou 1.')
        if not calibracao and not perfil.get('calibrado'):
            raise RuntimeError(
                'Calibre o cortador antes de habilitar as trocas.'
            )
        if self.incerto:
            raise RuntimeError(
                'Cortador em posicao incerta; afaste o cabecote e confirme '
                'o recolhimento pelo comando CORTADOR_SERVO.'
            )
        for chave in (
            'angulo_recolhido',
            'angulo_acionado',
            'x_aproximacao',
            'x_contato',
            'x_corte',
            'z_minimo',
            'alivio_z',
            'curso_maximo_mm',
            'deslocamento_f',
            'corte_f',
            'espera_servo_ms',
            'recuo_antes_corte_mm',
            'velocidade_recuo_f',
            'velocidade_avanco_f',
            'velocidade_purga_f',
        ) + (() if perfil['manter_y_corte'] else ('y_corte',)):
            try:
                valor = float(perfil[chave])
            except (KeyError, TypeError, ValueError) as erro:
                raise RuntimeError(
                    'Ajuste do cortador invalido: %s.' % chave
                ) from erro
            if not math.isfinite(valor):
                raise RuntimeError('Ajuste do cortador invalido: %s.' % chave)
            perfil[chave] = valor
        for chave in (
            'alivio_z',
            'curso_maximo_mm',
            'deslocamento_f',
            'corte_f',
            'espera_servo_ms',
            'velocidade_recuo_f',
            'velocidade_avanco_f',
            'velocidade_purga_f',
        ):
            if perfil[chave] <= 0:
                raise RuntimeError(
                    'Ajuste do cortador deve ser positivo: %s.' % chave
                )
        if not 0 <= perfil['recuo_antes_corte_mm'] <= 5.0:
            raise RuntimeError('Configure o recuo antes do corte de 0 a 5 mm.')
        if perfil['recuo_antes_corte_mm'] > self.integracao.unload_slack:
            raise RuntimeError(
                'O recuo antes do corte excede a folga configurada do buffer.'
            )
        sentido = math.copysign(
            1, perfil['x_corte'] - perfil['x_aproximacao']
        )
        folga = (perfil['x_contato'] - perfil['x_aproximacao']) * sentido
        curso = (perfil['x_corte'] - perfil['x_contato']) * sentido
        if folga < 0:
            raise RuntimeError('Contato anterior ao ponto de aproximacao.')
        if not 0 < curso <= perfil['curso_maximo_mm']:
            raise RuntimeError('Configure um curso de corte dentro do limite.')
        servo = self.servo(perfil)
        for chave in ('angulo_recolhido', 'angulo_acionado'):
            if not 0 <= perfil[chave] <= servo.max_angle:
                raise RuntimeError(
                    'Angulo do cortador fora dos limites do servo.'
                )
        if perfil['angulo_recolhido'] == perfil['angulo_acionado']:
            raise RuntimeError('Os angulos do batente devem ser diferentes.')
        extrusora = self.integracao.printer.lookup_object('extruder')
        if (
            max(
                perfil[chave]
                for chave in (
                    'velocidade_recuo_f',
                    'velocidade_avanco_f',
                    'velocidade_purga_f',
                )
            )
            > extrusora.max_e_velocity * 60
        ):
            raise RuntimeError(
                'Velocidade do perfil excede o limite da extrusora.'
            )
        return perfil

    def servo(self, perfil):
        nome = str(perfil.get('servo', ''))
        if not re.fullmatch(r'[A-Za-z0-9_]+', nome):
            raise RuntimeError('Nome do servo do cortador invalido.')
        servo = self.integracao.printer.lookup_object('servo ' + nome, None)
        if servo is None:
            raise RuntimeError(
                'Instale o servo com --cutter-servo-pin antes de testar.'
            )
        return servo

    def validar_movimento(self, perfil):
        cabecote = self.integracao.printer.lookup_object('toolhead')
        estado = cabecote.get_status(self.integracao.reactor.monotonic())
        if not all(eixo in estado['homed_axes'] for eixo in 'xyz'):
            raise RuntimeError(
                'Execute G28 antes de testar ou acionar o cortador.'
            )
        self.validar_limites(perfil, estado)
        minimo, maximo = estado['axis_minimum'], estado['axis_maximum']
        posicao = list(cabecote.get_position())
        altura = max(posicao[2] + perfil['alivio_z'], perfil['z_minimo'])
        if not minimo[2] <= altura <= maximo[2]:
            raise RuntimeError('Nao ha altura livre para acionar o cortador.')
        return cabecote, posicao, altura

    def validar_limites(self, perfil, estado):
        minimo, maximo = estado['axis_minimum'], estado['axis_maximum']
        limites = [(0, ('x_aproximacao', 'x_contato', 'x_corte'))]
        if not perfil['manter_y_corte']:
            limites.append((1, ('y_corte',)))
        for eixo, chaves in limites:
            if any(
                not minimo[eixo] <= perfil[chave] <= maximo[eixo]
                for chave in chaves
            ):
                raise RuntimeError(
                    'Posicao de corte fora dos limites da impressora.'
                )

    def x_livre_para_recolher(self, perfil, x):
        sentido = math.copysign(1, perfil['x_corte'] - perfil['x_aproximacao'])
        return (x - perfil['x_aproximacao']) * sentido <= 0.05

    def validar_homing(self, perfil):
        if self.integracao.bmcu.active_operations or (
            self.integracao.manual_operation is not None
        ):
            raise RuntimeError('Aguarde a operacao da BMCU antes de G28.')
        cabecote = self.integracao.printer.lookup_object('toolhead')
        cabecote.wait_moves()
        estado = cabecote.get_status(self.integracao.reactor.monotonic())
        self.validar_limites(perfil, estado)
        return cabecote, estado

    def recolher_para_homing(self, perfil):
        cabecote, estado = self.validar_homing(perfil)
        if 'x' not in estado['homed_axes']:
            raise RuntimeError(
                'O eixo X deve terminar a referencia antes de recolher '
                'o batente no G28.'
            )
        if not self.x_livre_para_recolher(
            perfil, cabecote.get_position()[0]
        ):
            raise RuntimeError(
                'Afaste o cabecote do batente antes de G28.'
            )
        self.posicionar_recolhido(perfil)

    def posicionar_servo(self, perfil, angulo):
        self.integracao.execute(
            'SET_SERVO SERVO=%s ANGLE=%.6f' % (perfil['servo'], angulo)
        )
        cabecote = self.integracao.printer.lookup_object('toolhead')
        cabecote.dwell(perfil['espera_servo_ms'] / 1000)
        cabecote.wait_moves()

    def posicionar_recolhido(self, perfil):
        self.recolhido = False
        self.incerto = True
        self.posicionar_servo(perfil, float(perfil['angulo_recolhido']))
        if perfil.get('soltar_servo_recolhido', 1) == 1:
            self.integracao.execute(
                'SET_SERVO SERVO=%s WIDTH=0' % perfil['servo']
            )
            self.integracao.printer.lookup_object('toolhead').wait_moves()
        self.recolhido = True
        self.incerto = False

    def recolher(self, perfil):
        if self.recolhido:
            return
        cabecote, posicao, altura = self.validar_movimento(perfil)
        if not self.x_livre_para_recolher(perfil, posicao[0]):
            raise RuntimeError(
                'Afaste o cabecote do batente antes de recolher o servo.'
            )
        self.posicionar_recolhido(perfil)

    def mover(self, cabecote, coordenadas, velocidade):
        with self.integracao.bmcu.printer_critical_section('cortador_ender'):
            cabecote.manual_move(coordenadas, velocidade / 60)
            cabecote.wait_moves()

    def cortar(self, perfil, verificar, retornar=True, antes_do_corte=None):
        inicio = self.integracao.reactor.monotonic()
        etapa = 'validacao'
        try:
            cabecote, origem, altura = self.validar_movimento(perfil)
            etapa = 'recolhimento_inicial'
            verificar()
            self.recolher(perfil)
            verificar()
            etapa = 'aproximacao'
            self.mover(cabecote, [None, None, altura], 300)
            y = None if perfil['manter_y_corte'] else perfil['y_corte']
            self.mover(
                cabecote,
                [perfil['x_aproximacao'], y, None],
                perfil['deslocamento_f'],
            )
            verificar()
            if antes_do_corte is not None:
                etapa = 'recuo_pre_corte'
                antes_do_corte()
                verificar()
            etapa = 'acionamento_servo'
            self.incerto = True
            self.recolhido = False
            self.posicionar_servo(perfil, perfil['angulo_acionado'])
            verificar()
            etapa = 'curso_da_lamina'
            self.mover(
                cabecote, [perfil['x_corte'], None, None], perfil['corte_f']
            )
            verificar()
            etapa = 'retorno_e_recolhimento'
            self.mover(
                cabecote,
                [perfil['x_aproximacao'], None, None],
                perfil['corte_f'],
            )
            self.posicionar_recolhido(perfil)
            verificar()
            if retornar:
                etapa = 'retorno_a_origem'
                self.mover(
                    cabecote,
                    [origem[0], origem[1], None],
                    perfil['deslocamento_f'],
                )
            self.ultimo_corte = {
                'resultado': 'comandos_concluidos',
                'segundos': self.integracao.reactor.monotonic() - inicio,
            }
        except Exception:
            self.ultimo_corte = {
                'resultado': 'interrompido',
                'etapa': etapa,
                'segundos': self.integracao.reactor.monotonic() - inicio,
                'posicao_incerta': self.incerto,
            }
            raise

    def servo_manual(self, gcmd):
        if gcmd.get_int('CONFIRMAR', 0, minval=0, maxval=1) != 1:
            raise gcmd.error('Confira a folga do batente e use CONFIRMAR=1.')
        estado = (
            self.integracao.printer.lookup_object('print_stats')
            .get_status(self.integracao.reactor.monotonic())
            .get('state')
        )
        recuperacao = bool(
            self.incerto
            and getattr(self.integracao, 'pending_tool_change', None)
        )
        if (
            estado == 'printing'
            or estado in ('paused', 'pause')
            and not recuperacao
            or (
                self.integracao.bmcu.active_operations
                or self.integracao.manual_operation is not None
            )
        ):
            raise gcmd.error('Finalize a operacao antes de testar o servo.')
        perfil = self.perfil()
        if perfil is None:
            raise gcmd.error('Instale bmcu-cortador.cfg antes de testar.')
        servo = self.servo(perfil)
        posicao = gcmd.get('POSICAO', '').upper()
        if posicao:
            if posicao not in ('RECOLHIDO', 'ACIONADO'):
                raise gcmd.error('Use POSICAO=RECOLHIDO ou POSICAO=ACIONADO.')
            if 'ANGULO' in gcmd.get_command_parameters():
                raise gcmd.error('Use apenas POSICAO ou ANGULO.')
            angulo = float(perfil['angulo_' + posicao.lower()])
        else:
            angulo = float(gcmd.get('ANGULO'))
        if not math.isfinite(angulo) or not 0 <= angulo <= servo.max_angle:
            raise gcmd.error('Angulo fora dos limites do servo.')
        espera = float(perfil.get('espera_servo_ms', 500))
        if not math.isfinite(espera) or espera <= 0:
            raise gcmd.error('Espera do servo invalida.')
        perfil['espera_servo_ms'] = espera
        recolher = math.isclose(
            angulo, float(perfil['angulo_recolhido']), abs_tol=0.01
        )
        if recuperacao and estado in ('paused', 'pause') and not recolher:
            raise gcmd.error('Recolha o batente para recuperar a troca.')
        if recolher:
            self.posicionar_recolhido(perfil)
        else:
            incerto = self.incerto
            self.recolhido = False
            self.incerto = True
            self.posicionar_servo(perfil, angulo)
            self.incerto = incerto
        gcmd.respond_info(
            'Servo comandado a %.1f graus; confira a posicao fisica.' % angulo
        )

    def testar(self, gcmd):
        if gcmd.get_int('CONFIRMAR', 0, minval=0, maxval=1) != 1:
            raise gcmd.error('Confira o percurso livre e use CONFIRMAR=1.')
        dispositivo, destino, carregado = self.integracao.manual_context()
        if carregado is not None:
            raise gcmd.error('Retire o filamento antes do teste mecanico.')
        perfil = self.preparar(calibracao=True)
        if perfil is None:
            raise gcmd.error('Instale o perfil do cortador antes de testar.')
        sessao = dispositivo.status['session_id']

        def verificar():
            self.integracao.check_manual_operation()
            dispositivo.refresh()
            if dispositivo.status[
                'session_id'
            ] != sessao or self.integracao.sensor(destino):
                raise RuntimeError(
                    'A BMCU reiniciou ou o caminho deixou de estar livre.'
                )

        self.integracao.manual_operation = 'TESTE_CORTADOR'
        try:
            origem = list(
                self.integracao.printer.lookup_object(
                    'toolhead'
                ).get_position()
            )
            self.cortar(perfil, verificar)
            self.mover(
                self.integracao.printer.lookup_object('toolhead'),
                [None, None, origem[2]],
                300,
            )
        finally:
            self.integracao.manual_operation = None
        gcmd.respond_info(
            'Teste concluido; confira o corte e o recolhimento '
            'antes de calibrado=1.'
        )
