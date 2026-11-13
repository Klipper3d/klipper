import ast
import configparser
import math
import types
import unittest

from test_integracao import Gcmd, ROOT, protocol
from test_manual_controls import ManualBench
from test_tip_forming import TipBench


class BancadaCortador(TipBench):
    def __init__(self):
        super().__init__()
        parser = configparser.RawConfigParser()
        parser.read(ROOT / 'config/ender3-v3-se-validada/bmcu-cortador.cfg')
        self.ajustes = {
            chave.removeprefix('variable_'): ast.literal_eval(valor)
            for chave, valor in parser.items(
                'gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES'
            )
            if chave.startswith('variable_')
        }
        self.ajustes.update(
            habilitado=1,
            calibrado=1,
            x_aproximacao=-2.0,
            x_corte=-7.0,
            y_corte=123.0,
        )
        self.eventos = []
        self.falha_servo = False
        self.falha_corte = False
        self.referencias = 'xyz'
        self.print_state = 'standby'
        self.obj.manual_operation = None
        self.printer.is_shutdown = lambda: False
        self.device.status['route_state'] = [protocol.ROUTE_EMPTY] * 4
        self.bmcu._route_state = lambda dispositivo, canal: dispositivo.status[
            'route_state'
        ][canal]
        self.bmcu.print_plan_open = False
        self.obj.chamada_temperatura_original = (
            lambda *args, **kwargs: 'original'
        )
        self.obj.posicionamento_pendente = None
        self.toolhead.get_status = lambda t: {
            'homed_axes': self.referencias,
            'axis_minimum': [-8, 0, 0],
            'axis_maximum': [230, 230, 250],
        }
        self.toolhead.manual_move = self.mover
        self.toolhead.wait_moves = lambda: self.eventos.append(
            ('esperar', list(self.position))
        )
        self.toolhead.get_last_move_time = lambda: self.eventos.append(
            ('iniciar', list(self.position))
        )
        self.toolhead.dwell = lambda segundos: self.pause(
            self.timeout + segundos
        )

    def lookup(self, nome, default=None):
        if nome == 'gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES':
            return types.SimpleNamespace(get_status=lambda t: self.ajustes)
        if nome == 'servo bmcu_cortador':
            return types.SimpleNamespace(max_angle=180.0)
        if nome == 'print_stats':
            return types.SimpleNamespace(
                get_status=lambda t: {'state': self.print_state}
            )
        return super().lookup(nome, default)

    def mover(self, coordenadas, velocidade):
        self.eventos.append(('mover', list(coordenadas), velocidade))
        if self.falha_corte and coordenadas[0] == -7:
            raise RuntimeError('movimento interrompido')
        for eixo, valor in enumerate(coordenadas):
            if valor is not None:
                self.position[eixo] = valor

    def execute(self, texto):
        self.eventos.append(('gcode', texto))
        if self.falha_servo and 'ANGLE=90.' in texto:
            raise RuntimeError('servo interrompido')
        return super().execute(texto)


class TestesCortador(unittest.TestCase):
    def test_buffer_no_alvo_nao_substitui_recuo_da_extrusora(self):
        for recuo_antes in (0.0, 2.0):
            with self.subTest(recuo_antes=recuo_antes):
                b = BancadaCortador()
                b.ajustes['recuo_antes_corte_mm'] = recuo_antes
                b.switch_distance = recuo_antes + 40.0
                b.device.status['buffer_pct'][3] = 40
                b.device.config_get = lambda chave: 40.0
                b.device.config_set = lambda *args: self.fail(
                    'O alvo do buffer nao deve substituir a retirada'
                )
                b.unload()
                self.assertAlmostEqual(-(recuo_antes + 40.0), b.e)
                self.assertFalse(b.obj.sensor(b.endpoint))
                self.assertEqual([(3, 55.0)], b.policies)
                self.assertNotIn('recovery_feeder_mm', b.obj.last_unload_timing)

    def test_sensor_travado_no_alvo_para_sem_confirmar_estacionamento(self):
        b = BancadaCortador()
        b.stuck = True
        b.device.status['buffer_pct'][3] = 40
        b.device.config_get = lambda chave: 40.0
        b.device.config_set = lambda *args: self.fail(
            'O sensor acionado nao autoriza mudar o alvo'
        )
        with self.assertRaisesRegex(RuntimeError, 'configured timeout'):
            b.unload()
        self.assertEqual(protocol.MOTION_IDLE, b.device.status['motion'][3])
        self.assertFalse(b.policies)
        self.assertFalse(b.obj.temporary_pullbacks)

    def test_perfil_entregue_fica_desabilitado_e_sem_coordenadas_inventadas(
        self,
    ):
        b = BancadaCortador()
        parser = configparser.RawConfigParser()
        parser.read(ROOT / 'config/ender3-v3-se-validada/bmcu-cortador.cfg')
        cfg = parser['gcode_macro _BMCU_ENDER_CORTADOR_AJUSTES']
        self.assertEqual('0', cfg['variable_habilitado'])
        self.assertEqual('0', cfg['variable_calibrado'])
        self.assertEqual('1', cfg['variable_soltar_servo_recolhido'])
        b.ajustes['habilitado'] = 0
        self.assertIsNone(b.obj.cortador.preparar())

    def test_corte_substitui_formacao_e_recolhe_batente_antes_de_recuar(self):
        b = BancadaCortador()
        b.obj.form_tip = lambda *args: self.fail(
            'Nao deve formar ponta com cortador'
        )
        b.unload()
        corte = next(
            i
            for i, e in enumerate(b.eventos)
            if e[:2] == ('mover', [-7.0, None, None])
        )
        retirada = next(
            i
            for i, e in enumerate(b.eventos)
            if e[0] == 'gcode' and e[1].startswith('G1 E')
        )
        recolher = max(
            i
            for i, e in enumerate(b.eventos[:retirada])
            if e[0] == 'gcode' and 'ANGLE=0.' in e[1]
        )
        self.assertLess(corte, recolher)
        self.assertLess(recolher, retirada)
        self.assertFalse(b.tip_formed)
        self.assertAlmostEqual(-66.0, b.e)
        self.assertIn('cut_seconds', b.obj.last_unload_timing)
        self.assertFalse(b.obj.cortador.incerto)

    def test_recuo_antes_do_corte_mantem_alimentador_parado(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        executar = b.gcode.run_script_from_command
        estado_ao_acionar = []

        def observar(comando):
            if 'ANGLE=90.' in comando:
                estado_ao_acionar.append((b.e, b.device.status['motion'][3]))
            return executar(comando)

        b.gcode.run_script_from_command = observar
        b.unload()
        self.assertEqual(
            [(-2.0, protocol.MOTION_IDLE)], estado_ao_acionar
        )
        self.assertEqual(
            [(-1.0, protocol.MOTION_IDLE)] * 2, b.moves[:2]
        )
        self.assertEqual(2.0, b.obj.last_unload_timing['pre_cut_retract_mm'])
        self.assertEqual([(3, 55.0)], b.policies)

    def test_recuo_que_libera_o_sensor_impede_acionamento_da_lamina(self):
        b = BancadaCortador()
        b.ajustes['recuo_antes_corte_mm'] = 2.0
        b.switch_distance = 1.0
        with self.assertRaisesRegex(RuntimeError, 'sensor'):
            b.unload()
        self.assertNotIn(
            ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=90.000000'),
            b.eventos,
        )
        self.assertEqual('parar', b.actuators[-1])

    def test_pwm_fica_ativo_ate_recuar_e_recolher_o_batente(self):
        b = BancadaCortador()
        b.ajustes['soltar_servo_recolhido'] = 1
        b.obj.cortador.recolhido = True
        perfil = b.obj.cortador.preparar()
        b.obj.cortador.cortar(perfil, lambda: None, retornar=False)
        eventos = [
            evento[:2] for evento in b.eventos
            if evento[0] in ('mover', 'gcode')
        ]
        self.assertEqual(
            [
                ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=90.000000'),
                ('mover', [-7.0, None, None]),
                ('mover', [-2.0, None, None]),
                ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=0.000000'),
                ('gcode', 'SET_SERVO SERVO=bmcu_cortador WIDTH=0'),
            ],
            eventos[2:],
        )

    def test_corte_interrompido_nao_soltar_batente_acionado(self):
        b = BancadaCortador()
        b.ajustes['soltar_servo_recolhido'] = 1
        b.obj.cortador.recolhido = True
        b.falha_corte = True
        perfil = b.obj.cortador.preparar()
        with self.assertRaisesRegex(RuntimeError, 'interrompido'):
            b.obj.cortador.cortar(perfil, lambda: None)
        self.assertTrue(b.obj.cortador.incerto)
        self.assertNotIn(
            ('gcode', 'SET_SERVO SERVO=bmcu_cortador WIDTH=0'), b.eventos
        )

    def test_recolhimento_manual_soltar_pwm_por_padrao_com_opcao_de_manter(self):
        for ajuste in (None, 0, 1):
            with self.subTest(ajuste=ajuste):
                b = BancadaCortador()
                b.bmcu.active_operations = {}
                if ajuste is None:
                    del b.ajustes['soltar_servo_recolhido']
                else:
                    b.ajustes['soltar_servo_recolhido'] = ajuste
                b.obj.cortador.servo_manual(Gcmd(ANGULO=90, CONFIRMAR=1))
                self.assertEqual(1, len([
                    evento for evento in b.eventos if evento[0] == 'gcode'
                ]))
                b.obj.cortador.servo_manual(Gcmd(ANGULO=0, CONFIRMAR=1))
                self.assertTrue(b.obj.cortador.recolhido)
                self.assertEqual(
                    ajuste != 0,
                    ('gcode', 'SET_SERVO SERVO=bmcu_cortador WIDTH=0')
                    in b.eventos,
                )

    def test_cortador_nao_calibrado_nao_move_cabecote_ou_extrusora(self):
        b = BancadaCortador()
        b.ajustes['calibrado'] = 0
        with self.assertRaisesRegex(RuntimeError, 'Calibre'):
            b.unload()
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))
        self.assertEqual(0, b.e)
        self.assertEqual('parar', b.actuators[-1])

    def test_validacoes_rejeitam_antes_de_mover(self):
        for chave, valor in [
            ('x_corte', math.nan),
            ('corte_f', 0),
            ('recuo_antes_corte_mm', -1),
            ('recuo_antes_corte_mm', 5.1),
            ('angulo_acionado', 181),
            ('x_aproximacao', -20),
            ('velocidade_recuo_f', 10000),
        ]:
            with self.subTest(chave=chave):
                b = BancadaCortador()
                b.ajustes[chave] = valor
                with self.assertRaises(RuntimeError):
                    perfil = b.obj.cortador.preparar()
                    b.obj.cortador.cortar(perfil, lambda: None)
                self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_corte_mantem_y_e_limita_apenas_curso_depois_do_contato(self):
        b = BancadaCortador()
        b.position[:3] = [110.0, 80.0, 20.0]
        b.ajustes.update(
            manter_y_corte=1,
            x_aproximacao=180.0,
            x_contato=190.0,
            x_corte=230.0,
            curso_maximo_mm=40.0,
            y_corte=-9999.0,
        )
        perfil = b.obj.cortador.preparar()
        b.obj.cortador.cortar(perfil, lambda: None, retornar=False)
        self.assertEqual(
            [
                [None, None, 22.0],
                [180.0, None, None],
                [230.0, None, None],
                [180.0, None, None],
            ],
            [evento[1] for evento in b.eventos if evento[0] == 'mover'],
        )
        self.assertEqual(80.0, b.position[1])

    def test_contato_invalido_ou_curso_excessivo_impede_movimento(self):
        for contato in (179.0, 189.0, 230.0, 231.0):
            with self.subTest(contato=contato):
                b = BancadaCortador()
                b.ajustes.update(
                    x_aproximacao=180.0,
                    x_contato=contato,
                    x_corte=230.0,
                    curso_maximo_mm=40.0,
                )
                with self.assertRaises(RuntimeError):
                    b.obj.cortador.preparar()
                self.assertEqual([], b.eventos)

    def test_sem_referencia_sem_altura_e_bico_frio_nao_iniciam_corte(self):
        for condicao in ('referencia', 'altura', 'frio'):
            b = BancadaCortador()
            if condicao == 'referencia':
                b.referencias = ''
            elif condicao == 'altura':
                b.position[2] = 250
            else:
                b.temperature = 25
            with self.assertRaises(RuntimeError):
                b.unload()
            self.assertFalse(any(e[0] == 'mover' for e in b.eventos))
            self.assertEqual(0, b.e)

    def test_falha_servo_ou_curso_preserva_posicao_incerta_e_nao_extruda(self):
        for atributo in ('falha_servo', 'falha_corte'):
            b = BancadaCortador()
            setattr(b, atributo, True)
            with self.assertRaises(RuntimeError):
                b.unload()
            self.assertTrue(b.obj.cortador.incerto)
            self.assertEqual(0, b.e)
            with self.assertRaisesRegex(RuntimeError, 'incerta'):
                b.obj.cortador.preparar()

    def test_sensor_liberado_durante_corte_interrompe_antes_da_retirada(self):
        b = BancadaCortador()
        anterior = b.mover

        def mover(coordenadas, velocidade):
            anterior(coordenadas, velocidade)
            if coordenadas[0] == -7:
                b.present = False

        b.toolhead.manual_move = mover
        with self.assertRaisesRegex(RuntimeError, 'sensor'):
            b.unload()
        self.assertEqual(0, b.e)
        self.assertTrue(b.obj.cortador.incerto)

    def test_carga_usa_velocidades_separadas_sem_acelerar_a_captura(self):
        b = BancadaCortador()
        b.obj.load(Gcmd(ENDPOINT='extruder'))
        avances = [c for c in b.commands if c.startswith('G1 E')]
        self.assertTrue(any('F240.' in c for c in avances))
        self.assertTrue(any('F180.' in c for c in avances))
        self.assertEqual(180.0, b.obj.capture_speed)

    def test_paralelo_so_posiciona_com_todos_os_caminhos_livres(self):
        for condicao in ('livre', 'sensor', 'carregado', 'incerto'):
            b = BancadaCortador()
            b.present = condicao == 'sensor'
            if condicao in ('carregado', 'incerto'):
                b.device.status['route_state'][0] = (
                    protocol.ROUTE_LOADED
                    if condicao == 'carregado'
                    else protocol.ROUTE_UNCERTAIN
                )
            resultado = b.obj.antecipar_carga(
                b.endpoint, 'prepare_load', 'PETG'
            )
            self.assertEqual('original', resultado)
            movimentos = [e for e in b.eventos if e[0] == 'mover']
            self.assertEqual(condicao == 'livre', bool(movimentos))
            self.assertFalse(any(c.startswith('G1 E') for c in b.commands))
            if condicao == 'livre':
                inicio = max(
                    i for i, e in enumerate(b.eventos) if e[0] == 'iniciar'
                )
                self.assertFalse(
                    any(e[0] == 'esperar' for e in b.eventos[inicio:])
                )
                b.obj.position_head()
                self.assertEqual('esperar', b.eventos[-1][0])
                self.assertIsNone(b.obj.posicionamento_pendente)

    def test_paralelo_ignora_outro_destino_e_outros_eventos(self):
        b = BancadaCortador()
        b.present = False
        for destino, comando in [
            (object(), 'prepare_load'),
            (b.endpoint, 'prepare_unload'),
        ]:
            self.assertEqual(
                'original', b.obj.antecipar_carga(destino, comando)
            )
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_sem_referencia_ou_calor_nao_antecipa_movimento(self):
        for condicao in ('referencia', 'frio'):
            b = BancadaCortador()
            b.present = False
            if condicao == 'referencia':
                b.referencias = ''
            else:
                b.temperature = 25
            b.obj.antecipar_carga(b.endpoint, 'prepare_load')
            self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_homing_nao_recolhe_automaticamente_apos_corte_interrompido(self):
        b = BancadaCortador()
        b.bmcu.active_operations = {}
        b.ajustes['recolher_sem_referencia'] = 1
        b.obj.cortador.incerto = True
        comando = b.obj.validar_homing_cortador(
            lambda g: self.fail('Homing nao deve iniciar')
        )
        with self.assertRaisesRegex(RuntimeError, 'incerta'):
            comando(Gcmd())
        self.assertEqual([], b.eventos)

    def test_posicao_nomeada_usa_angulos_configurados_sem_mover_eixos(self):
        b = BancadaCortador()
        b.bmcu.active_operations = {}
        b.referencias = ''
        b.ajustes.update(angulo_recolhido=180.0, angulo_acionado=20.0)
        b.obj.cortador.servo_manual(Gcmd(POSICAO='ACIONADO', CONFIRMAR=1))
        self.assertFalse(b.obj.cortador.recolhido)
        b.obj.cortador.servo_manual(Gcmd(POSICAO='RECOLHIDO', CONFIRMAR=1))
        self.assertTrue(b.obj.cortador.recolhido)
        self.assertEqual(
            [
                ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=20.000000'),
                ('gcode', 'SET_SERVO SERVO=bmcu_cortador ANGLE=180.000000'),
                ('gcode', 'SET_SERVO SERVO=bmcu_cortador WIDTH=0'),
            ],
            [evento for evento in b.eventos if evento[0] != 'esperar'],
        )

    def test_servo_pode_recolher_corte_interrompido_com_impressao_pausada(
        self,
    ):
        b = BancadaCortador()
        b.bmcu.active_operations = {}
        b.obj.cortador.incerto = True
        b.obj.pending_tool_change = {'tool': 4}
        b.print_state = 'paused'
        b.obj.cortador.servo_manual(Gcmd(ANGULO=0, CONFIRMAR=1))
        self.assertFalse(b.obj.cortador.incerto)
        self.assertTrue(b.obj.cortador.recolhido)
        self.assertEqual({'tool': 4}, b.obj.pending_tool_change)

    def test_cauda_persistida_impede_posicionamento_paralelo(self):
        b = BancadaCortador()
        b.present = False
        b.bmcu._durable_tail_route = lambda d, c: (
            {'tail': True} if c == 0 else None
        )
        b.obj.antecipar_carga(b.endpoint, 'prepare_load')
        self.assertFalse(any(e[0] == 'mover' for e in b.eventos))

    def test_teste_mecanico_nao_aquece_nem_extruda_e_restaura_posicao(self):
        b = BancadaCortador()
        b.present = False
        b.bmcu.active_operations = {}
        b.temperature = b.target = 25
        b.ajustes.update(habilitado=0, calibrado=0)
        origem = list(b.position)
        b.obj.cortador.testar(Gcmd(CONFIRMAR=1))
        self.assertEqual(origem, b.position)
        self.assertEqual(0, b.e)
        self.assertEqual(25, b.target)
        self.assertTrue(b.obj.cortador.recolhido)
        self.assertIsNone(b.obj.manual_operation)
        self.assertFalse(
            any('HEATER' in c or c.startswith('G1 E') for c in b.commands)
        )

    def test_servo_bloqueado_durante_impressao_e_operacao_bmcu(self):
        for estado, operacoes in [
            ('printing', {}),
            ('paused', {}),
            ('standby', {'bmcu0': {}}),
        ]:
            b = BancadaCortador()
            b.print_state = estado
            b.bmcu.active_operations = operacoes
            with self.assertRaisesRegex(RuntimeError, 'Finalize'):
                b.obj.cortador.servo_manual(Gcmd(ANGULO=0, CONFIRMAR=1))
            self.assertEqual([], b.eventos)

    def test_retirada_nao_retorna_a_peca_antes_de_ir_para_purga(self):
        b = BancadaCortador()
        origem = list(b.position)
        b.unload()
        self.assertFalse(
            any(
                e[:2] == ('mover', [origem[0], origem[1], None])
                for e in b.eventos
            )
        )


class BancadaReset(ManualBench):
    def __init__(self):
        super().__init__()
        self.device.status['present'] = [True, False, True, False]
        self.device.status['route_state'] = [protocol.ROUTE_UNCERTAIN] * 4
        self.bmcu.print_map_active = True
        self.bmcu.print_plan_open = False
        self.falhar_canal = None
        self.acionar_sensor = False

    def execute(self, texto):
        if texto.startswith('BMCU_ROUTE_CONFIRM'):
            if self.critical_depth:
                raise RuntimeError('Confirmacao dentro de secao critica')
            canal = int(texto.split('CHANNEL=')[1].split()[0])
            if canal == self.falhar_canal:
                raise RuntimeError('confirmacao indisponivel')
            self.device.status['route_state'][canal] = protocol.ROUTE_EMPTY
            if self.acionar_sensor:
                self.present = True
        if texto == 'BMCU_PRINT_END MODE=KEEP CLEAR=1':
            self.bmcu.print_map_active = False
            self.bmcu.print_plan_open = False
        return super().execute(texto)


class TestesReset(unittest.TestCase):
    def test_reset_distingue_estacionado_de_entrada_vazia_sem_mover_filamento(
        self,
    ):
        b = BancadaReset()
        b.obj.pending_tool_change = {'tool': 4}
        comando = Gcmd(CONFIRMAR=1)
        b.obj.resetar_bmcu(comando)
        for canal, estado in enumerate(('PARKED', 'EMPTY', 'PARKED', 'EMPTY')):
            self.assertIn(
                'BMCU_ROUTE_CONFIRM DEVICE=bmcu0 CHANNEL=%d STATE=%s'
                % (canal, estado),
                b.commands,
            )
        self.assertIsNone(b.obj.pending_tool_change)
        self.assertIn('BMCU_PRINT_END MODE=KEEP CLEAR=1', b.commands)
        self.assertFalse(
            any(
                c.startswith(
                    ('G1 ', 'G28', 'M109', 'BMCU_UNLOAD', 'BMCU_LOAD')
                )
                for c in b.commands
            )
        )
        self.assertIn('nenhum filamento foi movido', comando.messages[-1])

    def test_reset_bloqueia_sem_confirmacao_sensor_ocupado_e_operacoes_ativas(
        self,
    ):
        for condicao in (
            'confirmacao',
            'sensor',
            'printing',
            'paused',
            'operacao',
        ):
            b = BancadaReset()
            if condicao == 'sensor':
                b.present = True
            elif condicao in ('printing', 'paused'):
                b.print_state = condicao
            elif condicao == 'operacao':
                b.bmcu.active_operations = {'bmcu0': {}}
            with self.assertRaises(RuntimeError):
                b.obj.resetar_bmcu(
                    Gcmd(CONFIRMAR=0 if condicao == 'confirmacao' else 1)
                )
            self.assertEqual([], b.commands)

    def test_falha_parcial_ou_sensor_acionado_nao_encerram_a_sessao(self):
        for condicao in ('comando', 'sensor'):
            b = BancadaReset()
            if condicao == 'comando':
                b.falhar_canal = 2
            else:
                b.acionar_sensor = True
            with self.assertRaises(RuntimeError):
                b.obj.resetar_bmcu(Gcmd(CONFIRMAR=1))
            self.assertTrue(b.bmcu.print_map_active)
            self.assertNotIn('BMCU_PRINT_END MODE=KEEP CLEAR=1', b.commands)
