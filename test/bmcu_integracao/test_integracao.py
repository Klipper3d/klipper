import ast
import configparser
import importlib.util
import json
import math
import re
import sys
import tempfile
import types
import unittest
from contextlib import contextmanager
from pathlib import Path

RAIZ = Path(__file__).resolve().parents[2]
BASE = Path(__file__).parent / 'fixtures'


def importar(nome, caminho):
    spec = importlib.util.spec_from_file_location(nome, caminho)
    modulo = importlib.util.module_from_spec(spec)
    sys.modules[nome] = modulo
    spec.loader.exec_module(modulo)
    return modulo


for nome in ('ensaio', 'ensaio.bmcu_core'):
    pacote = types.ModuleType(nome)
    pacote.__path__ = []
    sys.modules[nome] = pacote
release = types.ModuleType('ensaio.bmcu_core.release')
release.PACKAGE_VERSION = '1.0.6'
release.REQUIRED_FIRMWARE = (1, 0, 1)
release.REQUIRED_FIRMWARE_TEXT = '1.0.1'
sys.modules[release.__name__] = release
protocolo = importar('ensaio.bmcu_core.protocol', BASE / 'protocolo_original.py')
modulo = importar('ensaio.bmcu_integracao_ender', RAIZ / 'klippy/extras/bmcu_integracao_ender.py')
instalador = importar('instalador', RAIZ / 'scripts/ender3-v3-se/instalar_multicor_bmcu.py')


class Gcmd:
    def __init__(self, **params):
        self.params = params
        self.mensagens = []

    def get(self, nome, padrao=None):
        return self.params.get(nome, padrao)

    def get_int(self, nome, padrao=None, **limites):
        valor = int(self.params.get(nome, padrao))
        if 'minval' in limites and valor < limites['minval'] or 'maxval' in limites and valor > limites['maxval']:
            raise RuntimeError('Valor fora do intervalo.')
        return valor

    def get_command_parameters(self):
        return self.params

    def error(self, mensagem):
        return RuntimeError(mensagem)

    def respond_info(self, mensagem):
        self.mensagens.append(mensagem)


class Bancada:
    def __init__(self):
        self.tempo = 0.0
        self.critico = 0
        self.comandos = []
        self.politicas = []
        self.presenca = True
        self.reverse = 0
        self.distancia_switch = 66
        self.travado = False
        self.reiniciar = False
        self.posicao = [100.0, 110.0, 5.0, 0.0]
        self.e = 0.0
        self.atuadores = []
        self.handlers = {}
        self.reactor = types.SimpleNamespace(monotonic=lambda: self.tempo, pause=self.pausar)
        self.device = types.SimpleNamespace(
            name='bmcu0', ready=True,
            status={'now_channel': 255, 'motion': [0, 0, 0, 0],
                    'present': [False, True, False, True], 'session_id': 3, 'meters': [0, 0, 0, 0.170]},
            refresh=self.refresh, stop_all=self.parar, set_motion=self.motion)
        self.endpoint = types.SimpleNamespace(name='extruder', sensor_status=self.sensor, _ender_load_context=('bmcu0', 3))
        self.bmcu = types.SimpleNamespace(
            devices=[self.device], devices_by_name={'bmcu0': self.device}, endpoints={'extruder': self.endpoint},
            active_operations={'bmcu0': {'phase': 'TOOLHEAD_PREPARATION'}},
            _endpoint_for_channel=lambda *args: self.endpoint,
            printer_critical_section=self.critical,
            _send_device_runtime_config=self.politica,
            get_status=lambda t: {'loaded_tool': 2})
        self.toolhead = types.SimpleNamespace(
            get_position=lambda: self.posicao,
            get_status=lambda t: {'homed_axes': 'xyz', 'axis_maximum': [230, 230, 250]})
        self.extrusora = types.SimpleNamespace(max_e_dist=5.0, get_status=lambda t: {'can_extrude': True})
        self.gcode = types.SimpleNamespace(run_script_from_command=self.executar,
                                          register_command=self.registrar, respond_info=lambda msg: None)
        self.gcode_move = types.SimpleNamespace(get_status=lambda t: {'gcode_position': list(self.posicao)})
        self.printer = types.SimpleNamespace(lookup_object=self.lookup, config_error=RuntimeError)
        self.obj = modulo.IntegracaoEnder.__new__(modulo.IntegracaoEnder)
        valores = dict(printer=self.printer, bmcu=self.bmcu, gcode=self.gcode, reactor=self.reactor,
                       dispositivo='bmcu0', destino='extruder', captura=20.0, vel_captura=180.0,
                       avanco=80.0, purga=70.0, recuo=55.0, vel_avanco=120.0, vel_recuo=600.0,
                       tempo=60, x_purga=-6.0, x_limpeza=-4.0, y_purga=123.0,
                       alivio=2.0, limpezas=3, recuos_temporarios={})
        self.obj.__dict__.update(valores)
        self.obj.medicao = types.SimpleNamespace(_capturar_extrusora_primeiro=self.capturar)

    def lookup(self, nome):
        return {'toolhead': self.toolhead, 'extruder': self.extrusora, 'gcode_move': self.gcode_move}[nome]

    def pausar(self, tempo):
        self.tempo = tempo

    @contextmanager
    def critical(self, nome):
        self.critico += 1
        try:
            yield
        finally:
            self.critico -= 1

    def uart(self):
        assert self.critico == 0, 'UART em seção crítica de movimento'

    def refresh(self):
        self.uart()
        if self.reiniciar and self.reverse >= 4:
            self.device.status['session_id'] += 1

    def parar(self):
        self.uart()
        self.atuadores.append('parar')
        self.device.status['motion'] = [0] * 4
        self.device.status['now_channel'] = 255

    def motion(self, canal, modo):
        self.uart()
        self.atuadores.append(modo)
        self.device.status['motion'][canal] = modo
        self.device.status['now_channel'] = canal

    def politica(self, dispositivo, channel_retract_override=None):
        self.uart()
        self.politicas.append(channel_retract_override)

    def sensor(self, papel):
        return {'enabled': False,
                'filament_detected': self.presenca and (self.travado or self.reverse < self.distancia_switch)}

    def capturar(self, dispositivo, destino, canal, mm, f, sensor_gerenciado=False):
        assert sensor_gerenciado is True
        assert destino.sensor_status('entry_sensor')['enabled'] is False
        self.e += mm
        dispositivo.status['meters'][canal] += mm / 1000

    def executar(self, texto):
        self.comandos.append(texto)
        for linha in texto.splitlines():
            if not linha.startswith('G1 '):
                continue
            params = dict((k, float(v)) for k, v in re.findall(r'([XYZEF])([-+0-9.]+)', linha))
            for eixo, indice in zip('XYZ', range(3)):
                if eixo in params:
                    self.posicao[indice] = params[eixo]
            if 'E' in params:
                mm = params['E']
                assert abs(mm) <= self.extrusora.max_e_dist + 1e-8
                self.e += mm
                self.tempo += abs(mm) / params['F'] * 60
                self.reverse += max(0, -mm)
                self.device.status['meters'][3] += mm / 1000

    def registrar(self, nome, handler):
        anterior = self.handlers.pop(nome, None)
        if handler is not None:
            self.handlers[nome] = handler
        return anterior


class Ensaios(unittest.TestCase):
    def test_carga_com_sensor_gerenciado_e_blocos_respeitados(self):
        b = Bancada()
        b.obj.carregar(Gcmd(ENDPOINT='extruder'))
        self.assertAlmostEqual(b.e, 170)
        self.assertEqual(b.device.status['motion'][3], protocolo.MOTION_ON_USE)
        self.assertEqual(b.posicao[0], -4)
        self.assertEqual(b.critico, 0)

    def test_retirada_ate_switch_e_override_apenas_depois(self):
        b = Bancada()
        b.motion(3, protocolo.MOTION_BEFORE_PULL_BACK)
        b.obj.liberar(Gcmd(ENDPOINT='extruder'))
        self.assertAlmostEqual(b.reverse, 66)
        self.assertFalse(b.obj.sensor(b.endpoint))
        self.assertEqual(b.politicas, [(3, 55.0)])
        self.assertEqual(b.device.status['motion'][3], 0)
        self.assertIn('bmcu0', b.obj.recuos_temporarios)
        b.obj.adaptar_operacao(lambda g: None)(Gcmd())
        self.assertEqual(b.politicas, [(3, 55.0), None])
        self.assertFalse(b.obj.recuos_temporarios)

    def test_switch_travado_interrompe_e_para(self):
        b = Bancada()
        b.motion(3, protocolo.MOTION_BEFORE_PULL_BACK)
        b.travado = True
        with self.assertRaisesRegex(RuntimeError, 'não liberou'):
            b.obj.liberar(Gcmd())
        self.assertEqual(b.device.status['motion'][3], 0)
        self.assertFalse(b.politicas)
        self.assertEqual(b.critico, 0)

    def test_reset_uart_interrompe_e_para(self):
        b = Bancada()
        b.motion(3, protocolo.MOTION_BEFORE_PULL_BACK)
        b.reiniciar = True
        with self.assertRaisesRegex(RuntimeError, 'reiniciou'):
            b.obj.liberar(Gcmd())
        self.assertEqual(b.device.status['motion'][3], 0)

    def test_override_restaurado_mesmo_com_falha_nativa(self):
        b = Bancada()
        b.obj.recuos_temporarios['bmcu0'] = {}
        def falhar(gcmd):
            raise RuntimeError('timeout')
        with self.assertRaisesRegex(RuntimeError, 'timeout'):
            b.obj.adaptar_operacao(falhar)(Gcmd())
        self.assertEqual(b.politicas, [None])
        self.assertFalse(b.obj.recuos_temporarios)

    def test_carga_restabelece_politica_sem_parar_follow_no_fim(self):
        b = Bancada()
        b.obj.recuos_temporarios['bmcu0'] = {}
        b.obj.adaptar_operacao(b.obj.carregar)(Gcmd())
        self.assertEqual(b.politicas, [None])
        self.assertEqual(b.device.status['motion'][3], protocolo.MOTION_ON_USE)

    def test_adapter_orca_e_tools_existentes(self):
        b = Bancada()
        antigos = []
        for indice in range(4):
            adaptador = b.obj.adaptar_tool(indice, lambda g: antigos.append(g))
            adaptador(Gcmd(B=str(indice + 1)))
            self.assertEqual(b.comandos[-1], 'BMCU_TROCAR_ENDER TOOL=%d' % (indice + 1))
            adaptador(Gcmd())
        self.assertEqual(len(antigos), 4)
        with self.assertRaisesRegex(RuntimeError, 'não correspondem'):
            b.obj.adaptar_tool(1, lambda g: None)(Gcmd(B='1'))

    def test_troca_sem_homing_nao_inicia_alimentacao(self):
        b = Bancada()
        b.toolhead.get_status = lambda t: {'homed_axes': ''}
        with self.assertRaisesRegex(RuntimeError, 'G28'):
            b.obj.trocar(Gcmd(TOOL=2))
        self.assertFalse(b.comandos)
        self.assertFalse(b.atuadores)

    def test_confere_encoder_antes_de_liberar_o_proximo_canal(self):
        for distancia, valido in [(55.58, True), (20.0, False), (-55.0, False)]:
            b = Bancada()
            b.presenca = False
            inicio = b.device.status['meters'][3]
            b.obj.recuos_temporarios['bmcu0'] = {'canal': 3, 'inicio': inicio, 'sessao': 3}
            def esperar(d, c, *args, **kwargs):
                self.assertEqual(kwargs['start_m'], inicio)
                d.status['meters'][c] = inicio - distancia / 1000
                return {'encoder_mm': distancia}
            b.obj.aguardar_recuo_original = esperar
            if valido:
                retorno = b.obj.aguardar_recuo(b.device, 3, 60.0, start_m=999)
                self.assertAlmostEqual(retorno['after_switch_encoder_mm'], distancia)
            else:
                with self.assertRaisesRegex(RuntimeError, 'outro canal'):
                    b.obj.aguardar_recuo(b.device, 3, 60.0)
                self.assertEqual(b.atuadores[-1], 'parar')

    def test_patches_idempotentes_e_helper_antigo_preservado(self):
        original = (RAIZ / 'klippy/extras/bmcu_medicao.py').read_text()
        original = original.replace(', sensor_gerenciado=False):', '):', 1)
        original = original.replace("(not sensor_gerenciado and not sensor.get('enabled', True))",
                                    "not sensor.get('enabled', True)", 1)
        novo = instalador.corrigir_sensor(original)
        self.assertEqual(instalador.corrigir_sensor(novo), novo)
        self.assertEqual(original[original.index('class RetiradaAssistida:'):],
                         novo[novo.index('class RetiradaAssistida:'):])
        endpoint = (BASE / 'endpoints_original.py').read_text()
        novo_endpoint = instalador.corrigir_contrato(endpoint)
        self.assertEqual(instalador.corrigir_contrato(novo_endpoint), novo_endpoint)
        self.assertEqual(novo_endpoint.count('prioridade = command not in'), 1)

    def test_include_preserva_save_config_e_instalacao_repetida(self):
        with tempfile.TemporaryDirectory() as temp:
            casa = Path(temp)
            config = casa / 'printer_data/config'
            extras = casa / 'klipper/klippy/extras'
            config.mkdir(parents=True)
            (extras / 'bmcu_core').mkdir(parents=True)
            salvo = '#*# <---------------------- SAVE_CONFIG ---------------------->\n#*# [bltouch]\n#*# z_offset = 1.887\n#*# [bed_mesh default]\n#*# x_count = 9\n'
            (config / 'printer.cfg').write_text('[include mainsail.cfg]\n' + salvo)
            (extras / 'bmcu_medicao.py').write_text((RAIZ / 'klippy/extras/bmcu_medicao.py').read_text())
            (extras / 'bmcu_core/endpoints.py').write_text((BASE / 'endpoints_original.py').read_text())
            (extras / 'bmcu_core/manager.py').write_text((BASE / 'manager_original.py').read_text())
            alteracoes = instalador.preparar(casa)
            raiz = alteracoes[(config / 'printer.cfg').resolve()].decode()
            self.assertEqual(raiz[raiz.index('#*# <'):], salvo)
            for p, dados in alteracoes.items():
                p.write_bytes(dados)
            self.assertEqual(instalador.preparar(casa), alteracoes)

    def test_captura_nativa_nao_empurra_antes_da_extrusora(self):
        original = (BASE / 'manager_original.py').read_text()
        novo = instalador.corrigir_pre_captura(original)
        self.assertEqual(instalador.corrigir_pre_captura(novo), novo)
        raiz = ast.parse(novo)
        condicional = next(n for n in ast.walk(raiz)
                           if isinstance(n, ast.If)
                           and 'BMCU_PREPARAR_ENDER' in ast.dump(n.test))
        for macro, esperado in [('BMCU_PREPARAR_ENDER', 'parar'), ('OUTRO_CALLBACK', 'avancar')]:
            acoes = []
            dispositivo = types.SimpleNamespace(name='bmcu0', stop_all=lambda: acoes.append('parar'),
                                                set_motion=lambda c, m: acoes.append('avancar'))
            destino = types.SimpleNamespace(get=lambda *args: macro)
            ctx = dict(generic_contract=True, endpoint=destino, device=dispositivo,
                       channel=3, protocol=protocolo)
            exec(compile(ast.Module(body=[condicional], type_ignores=[]), '<pre_captura>', 'exec'), ctx)
            self.assertEqual(acoes, [esperado])
            if esperado == 'parar':
                self.assertEqual(destino._ender_load_context, ('bmcu0', 3))

    def test_prioridade_de_transporte_restrita_aos_dois_callbacks(self):
        fonte = instalador.corrigir_contrato((BASE / 'endpoints_original.py').read_text())
        arvore = ast.parse(fonte)
        no = next(n for c in arvore.body if isinstance(c, ast.ClassDef)
                  for n in c.body if isinstance(n, ast.FunctionDef)
                  and n.name == '_run_generic_contract_macro')
        espaco = {}
        exec(compile(ast.Module(body=[no], type_ignores=[]), '<contrato>', 'exec'), espaco)
        prioridades = []
        for comando, esperado in [('BMCU_PREPARAR_ENDER', False), ('BMCU_LIBERAR_ENDER', False), ('OUTRO_CALLBACK', True)]:
            destino = types.SimpleNamespace(name='extruder', _required_generic_macro=lambda k, l: comando,
                                           _gcode_param=lambda v: v,
                                           _run=lambda script, printer_priority: prioridades.append(printer_priority))
            espaco['_run_generic_contract_macro'](destino, 'k', 'l')
            self.assertEqual(prioridades[-1], esperado)

    def test_templates_da_configuracao_com_a_sintaxe_do_klipper(self):
        from jinja2 import Environment
        parser = configparser.RawConfigParser()
        parser.read_string(instalador.CONFIGURACAO)
        env = Environment(block_start_string='{%', block_end_string='%}',
                          variable_start_string='{', variable_end_string='}')
        def falhar(texto):
            raise RuntimeError(texto)
        contexto = dict(params={'TEMP_MESA': '80', 'TEMP_BICO': '230', 'FERRAMENTA': '2'},
                        printer={'bmcu': {'active_operations': {}, 'print_map_active': False,
                                          'print_plan_open': False, 'devices': [dict(loaded_channels=[], uncertain_channels=[])]},
                                 'print_stats': {'state': 'standby'},
                                 'filament_switch_sensor bmcu_entry': {'filament_detected': False}},
                        rawparams='ENDPOINT=extruder MATERIAL=PETG REASON=load', action_raise_error=falhar)
        for secao in parser.sections():
            if not secao.startswith('gcode_macro '):
                continue
            resultado = env.from_string(parser[secao]['gcode']).render(**contexto)
            if secao == 'gcode_macro ENDER_INICIAR_MULTICOR':
                self.assertIn('M140 S80.0', resultado)
                self.assertIn('M109 S230.0', resultado)
                self.assertNotIn('G1 E', resultado)
            if secao == 'gcode_macro BMCU_CONFIGURAR_MULTICOR':
                self.assertIn('CONTACT_TIMEOUT=180', resultado)
                self.assertNotIn('UNLOAD_RETRACT_MM', resultado)
        contexto['printer']['filament_switch_sensor bmcu_entry']['filament_detected'] = True
        with self.assertRaisesRegex(RuntimeError, 'switch'):
            env.from_string(parser['gcode_macro BMCU_CONFIGURAR_MULTICOR']['gcode']).render(**contexto)


if __name__ == '__main__':
    unittest.main(verbosity=2)
