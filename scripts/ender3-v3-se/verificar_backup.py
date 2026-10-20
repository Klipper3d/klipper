import argparse
import hashlib
import json
import tarfile
from pathlib import Path, PurePosixPath


def verificar(arquivo, destino=None):
    with tarfile.open(arquivo, 'r:gz') as tar:
        membros = tar.getmembers()
        nomes = [m.name for m in membros]
        if len(nomes) != len(set(nomes)):
            raise ValueError('O arquivo contém nomes duplicados.')
        for membro in membros:
            caminho = PurePosixPath(membro.name)
            if not membro.isfile() or caminho.is_absolute() or '..' in caminho.parts or '\\' in membro.name:
                raise ValueError('Entrada inválida: ' + membro.name)
        manifesto = json.load(tar.extractfile('manifesto.json'))
        if manifesto['formato'] != 1 or set(nomes) != set(manifesto['arquivos']) | {'manifesto.json'}:
            raise ValueError('O manifesto não corresponde aos arquivos.')
        for nome, metadados in manifesto['arquivos'].items():
            dados = tar.extractfile(nome).read()
            if len(dados) != metadados['bytes'] or hashlib.sha256(dados).hexdigest() != metadados['sha256']:
                raise ValueError('Arquivo alterado ou incompleto: ' + nome)
        if destino is not None:
            destino.mkdir(mode=0o700, parents=True, exist_ok=False)
            for membro in membros:
                caminho = destino / membro.name
                caminho.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
                caminho.write_bytes(tar.extractfile(membro).read())
                caminho.chmod(0o600)
    return manifesto


def principal():
    parser = argparse.ArgumentParser()
    parser.add_argument('arquivo', type=Path)
    parser.add_argument('--extrair', type=Path)
    args = parser.parse_args()
    manifesto = verificar(args.arquivo, args.extrair)
    print('Backup íntegro: %d arquivos.' % len(manifesto['arquivos']))
    print('Origem:', manifesto['metadados']['home_original'])
    print('Criado:', manifesto['metadados']['criado_utc'])
    for aviso in manifesto['avisos']:
        print('Pendente:', aviso)
    if args.extrair:
        print('Arquivos extraídos para revisão em:', args.extrair)
        print('Nenhum serviço, firmware ou configuração ativa foi alterado.')


if __name__ == '__main__':
    principal()
