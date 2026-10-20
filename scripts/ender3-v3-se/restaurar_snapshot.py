import argparse
import hashlib
import json
import os
import pwd
import re
import secrets
import shutil
import subprocess
from datetime import datetime
from pathlib import Path, PurePosixPath

import instalar_multicor_bmcu as instalador

RAIZ = Path(__file__).resolve().parents[2]
SNAPSHOT = RAIZ / 'machines/ender3-v3-se-zero3/2026-10-20'
MARCADOR = '__GERAR_TOKEN_LOCAL__'


def verificar(snapshot):
    registros = json.loads((snapshot / 'integridade.json').read_text())
    presentes = {
        p.relative_to(snapshot).as_posix()
        for p in snapshot.rglob('*')
        if p.is_file()
    }
    if presentes != set(registros) | {'integridade.json'}:
        raise ValueError('Há arquivos ausentes ou extras no snapshot.')
    for nome, esperado in registros.items():
        relativo = PurePosixPath(nome)
        if relativo.is_absolute() or '..' in relativo.parts:
            raise ValueError('Caminho inválido no snapshot: ' + nome)
        caminho = snapshot / nome
        if not caminho.resolve().is_relative_to(snapshot.resolve()):
            raise ValueError('Vínculo fora do snapshot: ' + nome)
        if hashlib.sha256(caminho.read_bytes()).hexdigest() != esperado:
            raise ValueError('Snapshot alterado: ' + nome)
    return json.loads((snapshot / 'metadados.json').read_text())


def token_painel(configuracoes):
    caminho = configuracoes / 'bmcu/bmcu_panel.cfg'
    if caminho.is_file():
        resultado = re.search(
            r'(?m)^access_token\s*:\s*(\S+)\s*$', caminho.read_text()
        )
        if resultado and resultado[1] != MARCADOR:
            if len(resultado[1]) >= 32:
                return resultado[1]
    return secrets.token_hex(32)


def preparar(casa, snapshot=SNAPSHOT):
    verificar(snapshot)
    configuracoes = casa / 'printer_data/config'
    extras = casa / 'klipper/klippy/extras'
    runtime = configuracoes / 'bmcu/runtime'
    versao = runtime / 'version'
    if not versao.is_file() or 'package=1.0.6' not in versao.read_text():
        raise RuntimeError('Instale BMCU-Klipper 1.0.6 antes de restaurar.')
    contratos = extras / 'bmcu_core/endpoints.py'
    gerenciador = extras / 'bmcu_core/manager.py'
    if not contratos.is_file() or not gerenciador.is_file():
        raise RuntimeError('Os módulos nativos da BMCU não foram encontrados.')
    alteracoes = {}
    token = token_painel(configuracoes)
    for origem in sorted((snapshot / 'config').rglob('*')):
        if not origem.is_file():
            continue
        relativo = origem.relative_to(snapshot / 'config')
        dados = origem.read_bytes()
        if relativo.as_posix() == 'bmcu/bmcu_panel.cfg':
            dados = dados.replace(MARCADOR.encode(), token.encode())
        alteracoes[configuracoes / relativo] = dados
    instalacao = runtime / 'INSTALLATION.json'
    if instalacao.is_file():
        dados_instalacao = json.loads(instalacao.read_text())
        dados_instalacao['panel_token_sha256'] = hashlib.sha256(
            token.encode()
        ).hexdigest()
        alteracoes[instalacao] = (
            json.dumps(dados_instalacao, indent=2) + '\n'
        ).encode()
    alteracoes[extras / 'bmcu_integracao_ender.py'] = (
        RAIZ / 'integrations/bmcu/bmcu_integracao_ender.py'
    ).read_bytes()
    alteracoes[extras / 'bmcu_medicao.py'] = (
        RAIZ / 'integrations/bmcu/bmcu_medicao.py'
    ).read_bytes()
    alteracoes[contratos.resolve()] = instalador.corrigir_contrato(
        contratos.read_text()
    ).encode()
    alteracoes[gerenciador.resolve()] = instalador.corrigir_pre_captura(
        gerenciador.read_text()
    ).encode()
    if any(not p.resolve().is_relative_to(casa.resolve()) for p in alteracoes):
        raise RuntimeError(
            'Há arquivos de destino fora do diretório do usuário.'
        )
    return alteracoes


def conferir_aplicacao(casa, metadados):
    git = [
        'git',
        '-c',
        'safe.directory=' + str(casa / 'klipper'),
        '-C',
        str(casa / 'klipper'),
        'rev-parse',
        'HEAD',
    ]
    atual = subprocess.check_output(git, text=True).strip()
    if atual != metadados['klipper_commit']:
        raise RuntimeError(
            'Use o checkout Klipper indicado em metadados.json.'
        )
    ativo = (
        subprocess.run(
            ['systemctl', 'is-active', '--quiet', 'klipper']
        ).returncode
        == 0
    )
    if ativo:
        estado = instalador.objeto('print_stats')
        if estado['state'] in ('printing', 'paused'):
            raise RuntimeError('Finalize a impressão antes de restaurar.')
        bmcu = instalador.objeto('bmcu')
        if bmcu['active_operations'] or bmcu['print_plan_open']:
            raise RuntimeError('Aguarde o fim das operações da BMCU.')
        for dispositivo in bmcu.get('devices', []):
            if any(
                c.get('motion') or c.get('motor_pwm')
                for c in dispositivo.get('channels', [])
            ):
                raise RuntimeError('Pare a alimentação antes de restaurar.')
    return ativo


def aplicar(casa, alteracoes, usuario):
    backup = casa / (
        'backup-antes-restauracao-'
        + datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    )
    backup.mkdir(mode=0o700)
    if os.geteuid() == 0:
        os.chown(backup, usuario.pw_uid, usuario.pw_gid)
    registros = []
    for indice, caminho in enumerate(alteracoes):
        existe = caminho.is_file()
        copia = backup / str(indice)
        if existe:
            shutil.copy2(caminho, copia)
            copia.chmod(0o600)
        registros.append(
            {
                'destino': str(caminho),
                'copia': str(copia) if existe else None,
                'vinculo': (
                    os.readlink(caminho) if caminho.is_symlink() else None
                ),
                'modo': caminho.stat().st_mode & 0o777 if existe else None,
                'uid': caminho.stat().st_uid if existe else None,
                'gid': caminho.stat().st_gid if existe else None,
            }
        )
    manifesto = backup / 'arquivos.json'
    manifesto.write_text(json.dumps(registros, indent=2))
    manifesto.chmod(0o600)
    instalador.servico('stop')
    try:
        for caminho, dados in alteracoes.items():
            modo = 0o600 if caminho.name == 'bmcu_panel.cfg' else 0o644
            instalador.gravar(caminho, dados, usuario, modo)
    except Exception:
        for item in registros:
            caminho = Path(item['destino'])
            if item['vinculo'] is not None:
                caminho.unlink(missing_ok=True)
                caminho.symlink_to(item['vinculo'])
            elif item['copia'] is None:
                caminho.unlink(missing_ok=True)
            else:
                instalador.gravar(
                    caminho, Path(item['copia']).read_bytes(), usuario
                )
        print('Falha: os arquivos anteriores foram restaurados.')
        print('O serviço Klipper permanece parado.')
        raise
    print('Configuração restaurada. Backup anterior:', backup)
    print('O serviço Klipper permanece parado para conferência.')
    print('Confira o filamento físico antes de iniciar o Klipper.')
    print('Para iniciar: sudo systemctl start klipper')


def principal():
    parser = argparse.ArgumentParser()
    parser.add_argument('--home', type=Path)
    parser.add_argument('--snapshot', type=Path, default=SNAPSHOT)
    parser.add_argument('--aplicar', action='store_true')
    args = parser.parse_args()
    usuario = (
        pwd.getpwnam(os.environ['SUDO_USER'])
        if os.environ.get('SUDO_USER')
        else pwd.getpwuid(os.getuid())
    )
    casa = args.home or Path(usuario.pw_dir)
    metadados = verificar(args.snapshot)
    alteracoes = preparar(casa, args.snapshot)
    print('Arquivos de configuração e módulos preparados:', len(alteracoes))
    print('Z-offset 1,940; malha 9×9; extrusora 7,589 mm/volta.')
    if not args.aplicar:
        print('Simulação: nenhum arquivo ou serviço foi alterado.')
        return
    if os.geteuid() != 0:
        raise RuntimeError('Use sudo para aplicar a restauração.')
    conferir_aplicacao(casa, metadados)
    aplicar(casa, alteracoes, usuario)


if __name__ == '__main__':
    principal()
