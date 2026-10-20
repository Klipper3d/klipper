import importlib.util
import io
import json
import sys
import tarfile
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

RAIZ = Path(__file__).resolve().parents[2]


def importar(nome):
    arquivo = RAIZ / 'scripts/ender3-v3-se' / (nome + '.py')
    spec = importlib.util.spec_from_file_location(nome, arquivo)
    modulo = importlib.util.module_from_spec(spec)
    sys.modules[nome] = modulo
    spec.loader.exec_module(modulo)
    return modulo


captura = importar('capturar_configuracao')
verificador = importar('verificar_backup')


class BackupTest(unittest.TestCase):
    def criar(self, pasta):
        origem = pasta / 'config'
        origem.mkdir()
        (origem / 'printer.cfg').write_text(
            '[bltouch]\n#*# z_offset = 1.887\n'
        )
        (origem / '.moonraker.secrets').write_text('senha=privada\n')
        (origem / 'bmcu_state.json').write_text('{"loaded_tool": 4}')
        (origem / 'backup-antigo.tar.gz').write_bytes(b'backup antigo')
        arquivo = pasta / 'backup.tar.gz'
        coleta = captura.Captura(arquivo)
        coleta.pasta(origem, 'config', ('.tar.gz',))
        coleta.concluir(
            {'home_original': '/home/pi', 'criado_utc': '2026-10-20'}
        )
        coleta.tar.close()
        return arquivo

    def test_conserva_calibracao_estado_e_segredos_apenas_no_backup_privado(
        self,
    ):
        with tempfile.TemporaryDirectory() as temp:
            pasta = Path(temp)
            arquivo = self.criar(pasta)
            destino = pasta / 'revisao'
            manifesto = verificador.verificar(arquivo, destino)
            self.assertEqual(
                (destino / 'config/printer.cfg').read_bytes(),
                (pasta / 'config/printer.cfg').read_bytes(),
            )
            self.assertEqual(
                (destino / 'config/.moonraker.secrets').read_text(),
                'senha=privada\n',
            )
            self.assertFalse(
                (destino / 'config/backup-antigo.tar.gz').exists()
            )
            self.assertEqual(len(manifesto['arquivos']), 3)
            with self.assertRaises(FileExistsError):
                verificador.verificar(arquivo, destino)

    def test_rejeita_corrupcao_antes_de_extrair(self):
        with tempfile.TemporaryDirectory() as temp:
            pasta = Path(temp)
            arquivo = self.criar(pasta)
            with tarfile.open(arquivo) as tar:
                conteudo = {
                    m.name: tar.extractfile(m).read() for m in tar.getmembers()
                }
            conteudo['config/printer.cfg'] += b'alterado'
            with tarfile.open(arquivo, 'w:gz') as tar:
                for nome, dados in conteudo.items():
                    item = tarfile.TarInfo(nome)
                    item.size = len(dados)
                    tar.addfile(item, io.BytesIO(dados))
            with self.assertRaisesRegex(ValueError, 'alterado'):
                verificador.verificar(arquivo, pasta / 'revisao')
            self.assertFalse((pasta / 'revisao').exists())

    def test_rejeita_caminhos_externos_e_links(self):
        with tempfile.TemporaryDirectory() as temp:
            arquivo = Path(temp) / 'malicioso.tar.gz'
            for nome, tipo in [
                ('../fora', tarfile.REGTYPE),
                ('/absoluto', tarfile.REGTYPE),
                ('link', tarfile.SYMTYPE),
                ('link', tarfile.LNKTYPE),
            ]:
                with tarfile.open(arquivo, 'w:gz') as tar:
                    item = tarfile.TarInfo(nome)
                    item.type = tipo
                    item.linkname = (
                        '/etc/passwd' if tipo != tarfile.REGTYPE else ''
                    )
                    tar.addfile(item, io.BytesIO(b''))
                with self.assertRaisesRegex(ValueError, 'inválida'):
                    verificador.verificar(arquivo)

    def test_rejeita_arquivo_que_mudou_durante_a_captura(self):
        with tempfile.TemporaryDirectory() as temp:
            pasta = Path(temp)
            origem = pasta / 'printer.cfg'
            origem.write_text('antes')
            coleta = captura.Captura(pasta / 'backup.tar.gz')
            try:
                coleta.arquivo(origem, 'config/printer.cfg', obrigatorio=True)
                origem.write_text('depois')
                with self.assertRaisesRegex(RuntimeError, 'mudou'):
                    coleta.concluir({})
            finally:
                coleta.tar.close()

    def test_copia_modulos_bmcu_vinculados_e_registra_permissoes(self):
        with tempfile.TemporaryDirectory() as temp:
            pasta = Path(temp)
            runtime = pasta / 'runtime/bmcu_core'
            runtime.mkdir(parents=True)
            (runtime / 'manager.py').write_text('conteudo em uso')
            (runtime / 'manager.py').chmod(0o750)
            extras = pasta / 'klippy/extras'
            extras.mkdir(parents=True)
            (extras / 'bmcu_core').symlink_to(
                runtime, target_is_directory=True
            )
            arquivo = pasta / 'backup.tar.gz'
            coleta = captura.Captura(arquivo)
            try:
                coleta.pasta(
                    pasta / 'klippy', 'klippy-instalado', seguir_vinculos=True
                )
                coleta.concluir(
                    {'home_original': '/home/pi', 'criado_utc': '2026-10-20'}
                )
            finally:
                coleta.tar.close()
            manifesto = verificador.verificar(arquivo, pasta / 'revisao')
            nome = 'klippy-instalado/extras/bmcu_core/manager.py'
            self.assertEqual(
                (pasta / 'revisao' / nome).read_text(), 'conteudo em uso'
            )
            self.assertEqual(manifesto['arquivos'][nome]['modo'], 0o750)

    def test_nao_captura_durante_impressao_ou_movimento(self):
        for estado, bmcu in [
            ('printing', {}),
            ('paused', {}),
            ('standby', {'active_operations': {'bmcu0': {}}}),
            ('standby', {'devices': [{'channels': [{'motion': 3}]}]}),
        ]:
            with patch.object(
                captura,
                'objeto',
                side_effect=lambda n: (
                    {'state': estado} if n == 'print_stats' else bmcu
                ),
            ):
                with self.assertRaises(RuntimeError):
                    captura.conferir_ocioso()

    def test_backup_nao_e_simulado_como_maquina_estacionada(self):
        bmcu = {
            'devices': [{'channels': [{'motion': 0}], 'loaded_channels': [3]}]
        }
        with patch.object(
            captura,
            'objeto',
            side_effect=lambda n: (
                {'state': 'standby'} if n == 'print_stats' else bmcu
            ),
        ):
            self.assertEqual(captura.conferir_ocioso(), bmcu)


if __name__ == '__main__':
    unittest.main()
