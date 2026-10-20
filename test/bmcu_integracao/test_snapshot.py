import hashlib
import json
import re
import shutil
import sys
import tempfile
import types
import unittest
from pathlib import Path
from unittest.mock import patch

RAIZ = Path(__file__).resolve().parents[2]
SCRIPTS = RAIZ / 'scripts/ender3-v3-se'
sys.path.insert(0, str(SCRIPTS))
sys.path.insert(0, str(RAIZ / 'klippy'))
from configfile import ConfigFileReader
import restaurar_snapshot as restaurador
import capturar_configuracao as coletor
import verificar_backup as verificador


class SnapshotTest(unittest.TestCase):
    def bancada(self, pasta):
        configuracoes = pasta / 'printer_data/config'
        runtime = configuracoes / 'bmcu/runtime'
        runtime.mkdir(parents=True)
        (runtime / 'version').write_text('package=1.0.6\nfirmware=1.0.1\n')
        (runtime / 'INSTALLATION.json').write_text('{}')
        extras = pasta / 'klipper/klippy/extras/bmcu_core'
        extras.mkdir(parents=True)
        fixtures = Path(__file__).parent / 'fixtures'
        for nome in ('endpoints', 'manager'):
            (extras / (nome + '.py')).write_text(
                (fixtures / (nome + '_original.txt')).read_text()
            )
        return configuracoes

    def test_integridade_calibracoes_e_includes_reais(self):
        snapshot = restaurador.SNAPSHOT
        metadados = restaurador.verificar(snapshot)
        self.assertEqual(metadados['calibracoes']['z_offset'], 1.940)
        cfg = snapshot / 'config'
        printer = (cfg / 'printer.cfg').read_text()
        self.assertIn('#*# z_offset = 1.940', printer)
        self.assertIn('#*# x_count = 9', printer)
        self.assertIn('#*# y_count = 9', printer)
        linhas = printer.split('#*# points =\n', 1)[1].splitlines()[:9]
        self.assertEqual(len(linhas), 9)
        self.assertTrue(all(len(linha.split(',')) == 9 for linha in linhas))
        for caminho in cfg.rglob('*.cfg'):
            for include in re.findall(
                r'^\[include\s+([^\]]+)\]', caminho.read_text(), re.M
            ):
                self.assertTrue((caminho.parent / include).is_file(), include)
        motion = (cfg / 'ender3-v3-se/motion.cfg').read_text()
        self.assertIn('rotation_distance: 7.589', motion)
        estado = json.loads((cfg / 'bmcu_state.json').read_text())
        dispositivo = next(iter(estado['devices'].values()))
        self.assertEqual(dispositivo['motion_config']['load_speed_mms'], 10)
        self.assertFalse(estado['print_session']['active'])
        self.assertEqual(
            estado['endpoints']['extruder']['toolhead_prepare_macro'],
            'BMCU_PREPARAR_ENDER',
        )

    def test_configuracao_macros_compila_com_sintaxe_klipper(self):
        from jinja2 import Environment

        ambiente = Environment(
            block_start_string='{%',
            block_end_string='%}',
            variable_start_string='{',
            variable_end_string='}',
        )
        for caminho in (restaurador.SNAPSHOT / 'config').rglob('*.cfg'):
            cfg = ConfigFileReader().build_fileconfig(
                caminho.read_text(), str(caminho)
            )
            for secao in cfg.sections():
                if cfg.has_option(secao, 'gcode'):
                    ambiente.parse(cfg.get(secao, 'gcode'))

    def test_preparacao_preserva_save_config_e_token_existente(self):
        with tempfile.TemporaryDirectory() as temp:
            casa = Path(temp)
            cfg = self.bancada(casa)
            token = 'a' * 64
            (cfg / 'bmcu/bmcu_panel.cfg').write_text('access_token: ' + token)
            plano = restaurador.preparar(casa)
            self.assertEqual(
                plano[cfg / 'printer.cfg'],
                (restaurador.SNAPSHOT / 'config/printer.cfg').read_bytes(),
            )
            self.assertIn(token.encode(), plano[cfg / 'bmcu/bmcu_panel.cfg'])
            self.assertFalse((cfg / 'printer.cfg').exists())
            metadados = json.loads(
                plano[cfg / 'bmcu/runtime/INSTALLATION.json']
            )
            self.assertEqual(
                metadados['panel_token_sha256'],
                hashlib.sha256(token.encode()).hexdigest(),
            )

    def test_gera_token_local_e_nao_publica_credencial_real(self):
        with tempfile.TemporaryDirectory() as temp:
            casa = Path(temp)
            cfg = self.bancada(casa)
            plano = restaurador.preparar(casa)
            texto = plano[cfg / 'bmcu/bmcu_panel.cfg'].decode()
            self.assertNotIn(restaurador.MARCADOR, texto)
            token = re.search(r'access_token:\s*(\w+)', texto)[1]
            self.assertEqual(len(token), 64)
            self.assertNotEqual(token, restaurador.token_painel(cfg))
            referencia = (
                restaurador.SNAPSHOT / 'config/bmcu/bmcu_panel.cfg'
            ).read_text()
            self.assertIn(restaurador.MARCADOR, referencia)

    def test_rejeita_alteracao_ou_arquivo_adicional(self):
        with tempfile.TemporaryDirectory() as temp:
            snapshot = Path(temp) / 'snapshot'
            shutil.copytree(restaurador.SNAPSHOT, snapshot)
            extra = snapshot / 'config/extra.cfg'
            extra.write_text('extra')
            with self.assertRaisesRegex(ValueError, 'extras'):
                restaurador.verificar(snapshot)
            extra.unlink()
            with (snapshot / 'config/printer.cfg').open('a') as arquivo:
                arquivo.write('alterado')
            with self.assertRaisesRegex(ValueError, 'alterado'):
                restaurador.verificar(snapshot)

    def test_falha_na_aplicacao_restaura_arquivos_anteriores(self):
        with tempfile.TemporaryDirectory() as temp:
            casa = Path(temp)
            cfg = casa / 'printer.cfg'
            cfg.write_text('calibracao anterior')
            segundo = casa / 'state.json'
            segundo.write_text('estado anterior')
            usuario = types.SimpleNamespace(pw_uid=0, pw_gid=0)
            gravar = restaurador.instalador.gravar

            def simular_falha(caminho, dados, usuario, modo=0o644):
                if dados == b'novo':
                    raise OSError('falha de disco')
                return gravar(caminho, dados, usuario, modo)

            with patch.object(restaurador.instalador, 'servico'), patch.object(
                restaurador.instalador, 'gravar', side_effect=simular_falha
            ):
                with self.assertRaisesRegex(OSError, 'falha de disco'):
                    restaurador.aplicar(
                        casa,
                        {cfg: b'primeira escrita', segundo: b'novo'},
                        usuario,
                    )
            self.assertEqual(cfg.read_text(), 'calibracao anterior')
            self.assertEqual(segundo.read_text(), 'estado anterior')

    def test_complemento_captura_env_sem_consultar_ou_parar_impressora(self):
        with tempfile.TemporaryDirectory() as temp:
            casa = Path(temp)
            env = casa / 'printer_data/systemd'
            env.mkdir(parents=True)
            conteudo = 'KLIPPER_ARGS="klippy.py printer.cfg"\n'
            (env / 'klipper.env').write_text(conteudo)
            saida = casa / 'complemento.tar.gz'
            with patch.object(
                coletor, 'conferir_ocioso'
            ) as conferir, patch.object(
                coletor,
                'objeto',
                return_value={
                    'mcu_version': 'versao-de-teste',
                },
            ):
                coletor.capturar_complemento(casa, saida)
                conferir.assert_not_called()
            destino = casa / 'revisao'
            verificador.verificar(saida, destino)
            self.assertEqual(
                (destino / 'ambiente-servicos/klipper.env').read_text(),
                conteudo,
            )


if __name__ == '__main__':
    unittest.main()
