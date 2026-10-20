import argparse
import hashlib
import io
import json
import os
import pwd
import subprocess
import tarfile
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from urllib.parse import quote
from urllib.request import urlopen


def comando(argumentos):
    try:
        resultado = subprocess.run(argumentos, capture_output=True, timeout=45)
        return {'codigo': resultado.returncode,
                'saida': resultado.stdout.decode(errors='replace'),
                'erro': resultado.stderr.decode(errors='replace')}
    except (OSError, subprocess.TimeoutExpired) as erro:
        return {'codigo': -1, 'erro': str(erro)}


def objeto(nome):
    url = 'http://127.0.0.1:7125/printer/objects/query?' + quote(nome, safe='')
    with urlopen(url, timeout=15) as resposta:
        dados = json.load(resposta)
    return dados['result']['status'][nome]


def conferir_ocioso():
    if objeto('print_stats')['state'] in ('printing', 'paused'):
        raise RuntimeError('Finalize a impressão antes de capturar a configuração.')
    bmcu = objeto('bmcu')
    if bmcu.get('active_operations') or bmcu.get('print_plan_open'):
        raise RuntimeError('Aguarde o término das operações da BMCU.')
    for dispositivo in bmcu.get('devices', []):
        if any(c.get('motion') or c.get('motor_pwm') for c in dispositivo.get('channels', [])):
            raise RuntimeError('Pare os motores da BMCU antes da captura.')
    return bmcu


class Captura:
    def __init__(self, arquivo):
        self.tar = tarfile.open(arquivo, 'w:gz')
        self.registros = {}
        self.avisos = []
        self.origens = {}

    def adicionar(self, nome, dados, origem=None):
        if nome in self.registros:
            raise RuntimeError('Arquivo duplicado no backup: ' + nome)
        item = tarfile.TarInfo(nome)
        item.size = len(dados)
        item.mode = 0o600
        self.tar.addfile(item, io.BytesIO(dados))
        self.registros[nome] = {'bytes': len(dados),
                                'sha256': hashlib.sha256(dados).hexdigest()}
        if origem is not None:
            self.origens[str(origem)] = self.registros[nome]['sha256']

    def arquivo(self, caminho, nome, obrigatorio=False):
        try:
            self.adicionar(nome, caminho.read_bytes(), caminho)
            info = caminho.stat()
            self.registros[nome].update(modo=info.st_mode & 0o777, uid=info.st_uid, gid=info.st_gid)
        except OSError as erro:
            if obrigatorio:
                raise RuntimeError('Não foi possível copiar ' + str(caminho)) from erro
            self.avisos.append(str(caminho) + ': ' + str(erro))

    def pasta(self, caminho, prefixo, ignorar=(), seguir_vinculos=False):
        if not caminho.is_dir():
            self.avisos.append('Diretório ausente: ' + str(caminho))
            return
        visitados = set()
        for raiz, pastas, arquivos in os.walk(caminho, followlinks=seguir_vinculos):
            real = Path(raiz).resolve()
            if real in visitados:
                pastas[:] = []
                self.avisos.append('Diretório vinculado repetido: ' + str(raiz))
                continue
            visitados.add(real)
            for pasta in list(pastas):
                p = Path(raiz) / pasta
                if pasta in ('.git', '__pycache__', '.cache') or pasta.startswith('backup-'):
                    pastas.remove(pasta)
                elif p.is_symlink() and not seguir_vinculos:
                    pastas.remove(pasta)
                    self.avisos.append('Copie separadamente o diretório vinculado: ' + str(p))
            for arquivo in sorted(arquivos):
                p = Path(raiz) / arquivo
                if any(arquivo.endswith(s) for s in ignorar):
                    continue
                if p.is_file():
                    self.arquivo(p, prefixo + '/' + p.relative_to(caminho).as_posix())

    def concluir(self, metadados):
        for origem, esperado in self.origens.items():
            if hashlib.sha256(Path(origem).read_bytes()).hexdigest() != esperado:
                raise RuntimeError('Arquivo mudou durante a captura; repita: ' + origem)
        manifesto = dict(formato=1, arquivos=dict(self.registros), avisos=self.avisos,
                         metadados=metadados)
        self.adicionar('manifesto.json',
                       (json.dumps(manifesto, indent=2, ensure_ascii=False) + '\n').encode())


def capturar(casa, saida):
    estado = conferir_ocioso()
    configuracoes = casa / 'printer_data/config'
    if not (configuracoes / 'printer.cfg').is_file():
        raise RuntimeError('printer.cfg não encontrado em ' + str(configuracoes))
    config_ativa = objeto('configfile')
    raiz_klipper = casa / 'klipper'
    git = ['git', '-c', 'safe.directory=' + str(raiz_klipper), '-C', str(raiz_klipper)]
    saida.parent.mkdir(parents=True, exist_ok=True)
    if saida.exists():
        raise RuntimeError('O destino já existe: ' + str(saida))
    with tempfile.NamedTemporaryFile(dir=saida.parent, suffix='.parcial', delete=False) as temp:
        temporario = Path(temp.name)
    coleta = Captura(temporario)
    try:
        coleta.pasta(configuracoes, 'config',
                     ('.tar.gz', '.tar', '.zip', '.3mf', '.gcode', '.pyc', '.parcial'))
        coleta.pasta(raiz_klipper / 'klippy', 'klippy-instalado', ('.pyc', '.so'), seguir_vinculos=True)
        for nome in ('.config', 'out/klipper.bin', 'out/klipper.dict', 'out/defconfig'):
            caminho = raiz_klipper / nome
            if caminho.is_file():
                coleta.arquivo(caminho, 'compilacao/' + nome.lstrip('.'))
        coleta.arquivo(Path('/usr/local/bin/klipper_mcu'), 'host-mcu/klipper_mcu')
        for caminho in (Path('/boot/armbianEnv.txt'), Path('/etc/armbian-release'),
                        Path('/etc/os-release')):
            coleta.arquivo(caminho, 'sistema/' + caminho.as_posix().lstrip('/'))
        coleta.pasta(Path('/boot/overlay-user'), 'sistema/boot/overlay-user')
        coleta.pasta(Path('/etc/udev/rules.d'), 'sistema/etc/udev/rules.d')
        for caminho in sorted(Path('/etc/systemd/system').glob('*')):
            if not any(n in caminho.name for n in ('klipper', 'moonraker', 'bmcu', 'crowsnest')):
                continue
            if caminho.is_dir() and not caminho.is_symlink():
                coleta.pasta(caminho, 'sistema/etc/systemd/system/' + caminho.name)
            elif caminho.is_file() and not caminho.is_symlink():
                coleta.arquivo(caminho, 'sistema/etc/systemd/system/' + caminho.name)
        for caminho in sorted(Path('/etc/default').glob('*')):
            if caminho.is_file() and any(n in caminho.name for n in ('klipper', 'moonraker', 'bmcu')):
                coleta.arquivo(caminho, 'sistema/etc/default/' + caminho.name)
        dados = {'uname': comando(['uname', '-a']),
                 'pacotes': comando(['dpkg-query', '-W']),
                 'python': comando([str(casa / 'klippy-env/bin/python'), '-m', 'pip', 'freeze']),
                 'gpio': comando(['gpiodetect']),
                 'gpio_linhas': comando(['gpioinfo']),
                 'usb': comando(['lsusb']),
                 'klipper_commit': comando(git + ['rev-parse', 'HEAD']),
                 'klipper_versao': comando(git + ['describe', '--tags', '--always', '--dirty']),
                 'klipper_status': comando(git + ['status', '--short'])}
        diff = subprocess.run(git + ['diff', '--binary', 'HEAD'],
                              capture_output=True, timeout=45, check=True)
        coleta.adicionar('fontes/klipper-alteracoes.patch', diff.stdout)
        with tempfile.TemporaryFile() as fonte:
            subprocess.run(git + ['archive', '--format=tar', 'HEAD'],
                           stdout=fonte, check=True, timeout=45)
            fonte.seek(0)
            coleta.adicionar('fontes/klipper-commit.tar', fonte.read())
        for nome in ('klipper', 'klipper-mcu', 'moonraker', 'bmcu-transport', 'crowsnest'):
            dados[nome] = comando(['systemctl', 'cat', nome])
        dados['servicos_bmcu'] = comando(['systemctl', 'list-unit-files', '*bmcu*', '--no-pager'])
        for nome, conteudo in [('bmcu', estado), ('configfile', config_ativa)]:
            coleta.adicionar('diagnostico/' + nome + '.json',
                             json.dumps(conteudo, indent=2, ensure_ascii=False).encode())
        estado_bmcu = config_ativa.get('config', {}).get('bmcu', {}).get('state_file')
        if estado_bmcu:
            caminho = Path(estado_bmcu.replace('~/', str(casa) + '/', 1))
            if not caminho.is_absolute():
                caminho = configuracoes / caminho
            if caminho.resolve().is_relative_to(configuracoes.resolve()):
                pass
            else:
                coleta.arquivo(caminho, 'estado-externo/bmcu_state.json', obrigatorio=True)
        conferir_ocioso()
        coleta.concluir({'criado_utc': datetime.now(timezone.utc).isoformat(),
                         'home_original': str(casa), 'ambiente': dados})
        coleta.tar.close()
        os.chmod(temporario, 0o600)
        os.replace(temporario, saida)
    finally:
        coleta.tar.close()
        temporario.unlink(missing_ok=True)
    return coleta.avisos


def principal():
    parser = argparse.ArgumentParser()
    parser.add_argument('--home', type=Path)
    parser.add_argument('--saida', type=Path)
    args = parser.parse_args()
    usuario = pwd.getpwnam(os.environ['SUDO_USER']) if os.environ.get('SUDO_USER') else pwd.getpwuid(os.getuid())
    casa = args.home or Path(usuario.pw_dir)
    nome = 'backup-restauracao-' + datetime.now().strftime('%Y%m%d-%H%M%S') + '.tar.gz'
    saida = args.saida or casa / 'printer_data/config' / nome
    avisos = capturar(casa, saida)
    if os.geteuid() == 0:
        os.chown(saida, usuario.pw_uid, usuario.pw_gid)
    print('Backup criado:', saida)
    print('SHA256:', hashlib.sha256(saida.read_bytes()).hexdigest())
    print('Baixe pelo Mainsail → Máquina → arquivos de configuração.')
    print('O arquivo é privado: pode conter senhas e tokens. Não publique o arquivo inteiro no GitHub.')
    for aviso in avisos:
        print('Pendente:', aviso)


if __name__ == '__main__':
    principal()
