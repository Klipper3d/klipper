import ast
import json
import os
import pwd
import re
import shutil
import subprocess
import tarfile
import tempfile
import time
from datetime import datetime
from pathlib import Path
from urllib.request import urlopen


RAIZ = Path(__file__).resolve().parents[2]
MODULO = (RAIZ / 'klippy/extras/bmcu_integracao_ender.py').read_text()
CONFIGURACAO = (RAIZ / 'config/ender3-v3-se-validada/bmcu-multicor-ender.cfg').read_text()
MEDICAO = (RAIZ / 'klippy/extras/bmcu_medicao.py').read_text()


def objeto(nome):
    from urllib.parse import quote
    url = 'http://127.0.0.1:7125/printer/objects/query?' + quote(nome, safe='')
    with urlopen(url, timeout=15) as resposta:
        dados = json.load(resposta)
    if 'error' in dados:
        raise RuntimeError(str(dados['error']))
    return dados['result']['status'][nome]


def conferir_maquina():
    estado = objeto('webhooks')
    if estado['state'] != 'ready':
        raise RuntimeError('O Klipper precisa estar pronto antes da instalação: ' + str(estado))
    if objeto('print_stats')['state'] in ('printing', 'paused'):
        raise RuntimeError('Finalize a impressão antes de instalar.')
    estado = objeto('bmcu')
    if estado['package_version'] != '1.0.6':
        raise RuntimeError('Este instalador corresponde ao BMCU-Klipper 1.0.6.')
    if estado['active_operations'] or estado['print_plan_open'] or estado['print_map_active'] or estado['prestaged']:
        raise RuntimeError('Finalize as operações e o plano de impressão da BMCU antes de instalar.')
    dispositivos = estado['devices']
    if len(dispositivos) != 1 or dispositivos[0]['uid'] != 'CDAB42A1D0BEDF0B39E339E3':
        raise RuntimeError('A BMCU detectada não corresponde à configuração desta impressora.')
    for dispositivo in dispositivos:
        if not dispositivo['ready'] or dispositivo['loaded_channels'] or dispositivo['uncertain_channels']:
            raise RuntimeError('Deixe todos os canais estacionados e a BMCU pronta.')
    if objeto('filament_switch_sensor bmcu_entry')['filament_detected']:
        raise RuntimeError('O switch deve estar livre antes de instalar.')


def transformar_metodo(texto, classe, metodo, transformar):
    arvore = ast.parse(texto)
    candidatos = [n for c in arvore.body if isinstance(c, ast.ClassDef) and c.name == classe
                  for n in c.body if isinstance(n, ast.FunctionDef) and n.name == metodo]
    if len(candidatos) != 1:
        raise RuntimeError('Método incompatível: %s.%s' % (classe, metodo))
    no = candidatos[0]
    linhas = texto.splitlines(keepends=True)
    trecho = ''.join(linhas[no.lineno - 1:no.end_lineno])
    novo = transformar(trecho)
    linhas[no.lineno - 1:no.end_lineno] = [novo]
    resultado = ''.join(linhas)
    compile(resultado, metodo, 'exec')
    return resultado


def corrigir_sensor(texto):
    def mudar(trecho):
        assinatura = 'def _capturar_extrusora_primeiro(self, dispositivo, destino, canal, avanco, velocidade):'
        condicao = "not sensor.get('enabled', True)"
        nova_assinatura = assinatura.replace('velocidade):', 'velocidade, sensor_gerenciado=False):')
        nova_condicao = "(not sensor_gerenciado and not sensor.get('enabled', True))"
        if nova_assinatura in trecho and nova_condicao in trecho:
            return trecho
        if trecho.count(assinatura) != 1 or trecho.count(condicao) != 1:
            raise RuntimeError('A captura foi modificada; envie o bmcu_medicao.py atual antes de instalar.')
        return trecho.replace(assinatura, nova_assinatura).replace(condicao, nova_condicao)
    return transformar_metodo(texto, 'MedicaoChegada', '_capturar_extrusora_primeiro', mudar)


def corrigir_contrato(texto):
    arvore = ast.parse(texto)
    classes = [c.name for c in arvore.body if isinstance(c, ast.ClassDef)
               and any(isinstance(n, ast.FunctionDef) and n.name == '_run_generic_contract_macro' for n in c.body)]
    if len(classes) != 1:
        raise RuntimeError('Contrato de extrusora genérica incompatível.')
    def mudar(trecho):
        anterior = '        self._run(script, printer_priority=True)'
        novo = ("        prioridade = command not in ('BMCU_PREPARAR_ENDER', 'BMCU_LIBERAR_ENDER')\n"
                '        self._run(script, printer_priority=prioridade)')
        if novo in trecho:
            return trecho
        if trecho.count(anterior) != 1:
            raise RuntimeError('O contrato da BMCU mudou; envie endpoints.py antes de instalar.')
        return trecho.replace(anterior, novo)
    return transformar_metodo(texto, classes[0], '_run_generic_contract_macro', mudar)


def corrigir_pre_captura(texto):
    def mudar(trecho):
        anterior = ("            else:\n"
                    "                device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)\n\n"
                    "            if not generic_contract:\n"
                    "                phase = 'BITE'")
        novo = ("            elif (generic_contract and\n"
                "                  str(endpoint.get('toolhead_prepare_macro', '')).strip().upper() == 'BMCU_PREPARAR_ENDER'):\n"
                "                device.stop_all()\n"
                "                endpoint._ender_load_context = (device.name, int(channel))\n"
                "            else:\n"
                "                device.set_motion(channel, protocol.MOTION_BEFORE_ON_USE)\n\n"
                "            if not generic_contract:\n"
                "                phase = 'BITE'")
        if novo in trecho:
            return trecho
        if trecho.count(anterior) != 1:
            raise RuntimeError('A etapa nativa de captura mudou; envie manager.py antes de instalar.')
        return trecho.replace(anterior, novo)
    return transformar_metodo(texto, 'BMCUManager', 'load_channel', mudar)


def preparar(casa):
    configuracoes = casa / 'printer_data/config'
    extras = casa / 'klipper/klippy/extras'
    raiz = configuracoes / 'printer.cfg'
    sensor = extras / 'bmcu_medicao.py'
    contratos = extras / 'bmcu_core/endpoints.py'
    gerenciador = extras / 'bmcu_core/manager.py'
    modulo = extras / 'bmcu_integracao_ender.py'
    destino = configuracoes / 'bmcu-multicor-ender.cfg'
    for caminho in (raiz, contratos, gerenciador):
        if not caminho.is_file():
            raise RuntimeError('Arquivo não encontrado: ' + str(caminho))
    conteudo = raiz.read_text()
    incluir = '[include bmcu-multicor-ender.cfg]'
    if not re.search(r'^\s*\[include\s+bmcu-multicor-ender\.cfg\]\s*$', conteudo, re.M | re.I):
        marcador = conteudo.find('#*# <')
        if marcador < 0:
            conteudo = conteudo.rstrip('\n') + '\n\n' + incluir + '\n'
        else:
            conteudo = conteudo[:marcador].rstrip('\n') + '\n\n' + incluir + '\n\n' + conteudo[marcador:]
    compile(MODULO, 'bmcu_integracao_ender.py', 'exec')
    return {
        raiz.resolve(): conteudo.encode(),
        sensor.resolve(): corrigir_sensor(sensor.read_text() if sensor.is_file() else MEDICAO).encode(),
        contratos.resolve(): corrigir_contrato(contratos.read_text()).encode(),
        gerenciador.resolve(): corrigir_pre_captura(gerenciador.read_text()).encode(),
        modulo.resolve(): MODULO.encode(),
        destino.resolve(): CONFIGURACAO.encode(),
    }


def gravar(caminho, dados, usuario, modo=0o644):
    caminho.parent.mkdir(parents=True, exist_ok=True)
    anterior = caminho.stat() if caminho.exists() else None
    with tempfile.NamedTemporaryFile(dir=caminho.parent, delete=False) as temporario:
        nome = temporario.name
        temporario.write(dados)
        temporario.flush()
        os.fsync(temporario.fileno())
    try:
        os.chmod(nome, anterior.st_mode & 0o777 if anterior else modo)
        if os.geteuid() == 0:
            os.chown(nome, anterior.st_uid if anterior else usuario.pw_uid,
                     anterior.st_gid if anterior else usuario.pw_gid)
        os.replace(nome, caminho)
    finally:
        if os.path.exists(nome):
            os.unlink(nome)


def servico(acao):
    comando = ['systemctl', acao, 'klipper']
    if os.geteuid() != 0:
        comando.insert(0, 'sudo')
    subprocess.run(comando, check=True)


def aguardar():
    prazo = time.monotonic() + 90
    ultimo = None
    while time.monotonic() < prazo:
        try:
            estado = objeto('webhooks')
            if estado['state'] == 'ready':
                configuracao = objeto('configfile').get('config', {})
                if 'bmcu_integracao_ender' in configuracao:
                    return
            ultimo = estado.get('state_message', estado['state'])
            if estado['state'] in ('error', 'shutdown'):
                raise RuntimeError(ultimo)
        except (OSError, KeyError, ValueError) as erro:
            ultimo = str(erro)
        time.sleep(1)
    raise RuntimeError('O Klipper não ficou pronto: %s' % ultimo)


def principal():
    nome = os.environ.get('SUDO_USER') if os.geteuid() == 0 else None
    usuario = pwd.getpwnam(nome) if nome else pwd.getpwuid(os.getuid())
    casa = Path(usuario.pw_dir)
    conferir_maquina()
    alteracoes = preparar(casa)
    originais = {p: p.read_bytes() if p.exists() else None for p in alteracoes}
    alteracoes = {p: d for p, d in alteracoes.items() if originais[p] != d}
    if not alteracoes:
        print('A integração já está instalada. No Mainsail, execute BMCU_CONFIGURAR_MULTICOR.')
        return
    backup = casa / ('backup-multicor-' + datetime.now().strftime('%Y%m%d-%H%M%S-%f'))
    backup.mkdir(mode=0o700)
    if os.geteuid() == 0:
        os.chown(backup, usuario.pw_uid, usuario.pw_gid)
    with tarfile.open(backup / 'configuracoes.tar.gz', 'w:gz') as tar:
        tar.add(casa / 'printer_data/config', arcname='config')
    manifest = []
    for indice, caminho in enumerate(alteracoes):
        original = originais[caminho]
        copia = backup / ('arquivo-%d' % indice)
        metadados = caminho.stat() if caminho.exists() else None
        if original is not None:
            shutil.copy2(caminho, copia)
        manifest.append({'destino': str(caminho), 'copia': str(copia) if original is not None else None,
                         'uid': metadados.st_uid if metadados else usuario.pw_uid,
                         'gid': metadados.st_gid if metadados else usuario.pw_gid,
                         'modo': metadados.st_mode & 0o777 if metadados else 0o644})
    (backup / 'arquivos.json').write_text(json.dumps(manifest, indent=2))
    config_bmcu = objeto('configfile').get('config', {}).get('bmcu', {})
    caminho_estado = config_bmcu.get('state_file', str(casa / 'printer_data/config/bmcu_state.json'))
    if caminho_estado.startswith('~/'):
        caminho_estado = str(casa / caminho_estado[2:])
    caminho_estado = Path(caminho_estado)
    if caminho_estado.is_file():
        dados_estado = json.loads(caminho_estado.read_text())
        endpoint = dados_estado.get('endpoints', {}).get('extruder')
        (backup / 'endpoint.json').write_text(json.dumps({'arquivo': str(caminho_estado), 'endpoint': endpoint}))
    restaurador = '''import json, os, subprocess
from pathlib import Path
base = Path(__file__).resolve().parent
subprocess.run(['systemctl', 'stop', 'klipper'], check=True)
for item in json.loads((base / 'arquivos.json').read_text()):
    destino = Path(item['destino'])
    if item['copia'] is None:
        destino.unlink(missing_ok=True)
    else:
        destino.write_bytes(Path(item['copia']).read_bytes())
        os.chmod(destino, item['modo'])
        os.chown(destino, item['uid'], item['gid'])
if (base / 'endpoint.json').is_file():
    anterior = json.loads((base / 'endpoint.json').read_text())
    estado = Path(anterior['arquivo'])
    if estado.is_file() and anterior['endpoint'] is not None:
        dados = json.loads(estado.read_text())
        atual = dados.get('endpoints', {}).get('extruder', {})
        if atual.get('toolhead_prepare_macro') == 'BMCU_PREPARAR_ENDER':
            dados['endpoints']['extruder'] = anterior['endpoint']
            metadados = estado.stat()
            temporario = estado.with_name(estado.name + '.restauracao')
            temporario.write_text(json.dumps(dados, separators=(',', ':'), allow_nan=False))
            os.chmod(temporario, metadados.st_mode & 0o777)
            os.chown(temporario, metadados.st_uid, metadados.st_gid)
            os.replace(temporario, estado)
subprocess.run(['systemctl', 'start', 'klipper'], check=True)
print('Arquivos anteriores restaurados.')
'''
    (backup / 'restaurar.py').write_text(restaurador)
    print('Backup:', backup)
    servico('stop')
    try:
        for caminho, dados in alteracoes.items():
            gravar(caminho, dados, usuario)
        servico('start')
        aguardar()
    except Exception:
        servico('stop')
        for caminho in alteracoes:
            if originais[caminho] is None:
                caminho.unlink(missing_ok=True)
            else:
                gravar(caminho, originais[caminho], usuario)
        servico('start')
        print('A instalação falhou e os arquivos anteriores foram restaurados.')
        raise
    print('Integração instalada; Klipper pronto.')
    print('No console do Mainsail, execute: BMCU_CONFIGURAR_MULTICOR')
    print('PID, Z-offset, malha e calibração de extrusão foram preservados.')
    print('Para restaurar os arquivos: sudo python3 ' + str(backup / 'restaurar.py'))


if __name__ == '__main__':
    principal()
