# Desenvolvimento e testes locais

[English](../Development.md) | **Português brasileiro**

Os testes não conectam à impressora: usam aquecedores, movimentos e BMCU
simulados. Parte da suíte executa o dispatcher, macros, estado G-code e
pausa/retomada reais do Klipper. Os testes do cortador incluem os quatro canais,
recuo com alimentador parado, buffer, sensor, reinício, falhas por etapa,
estacionamento e posicionamento com offsets e overrides de impressão.

<a id="windows-with-wsl"></a>
## Windows com WSL

Use Ubuntu 22.04 no WSL para reproduzir o ambiente do CI. Os utilitários Linux
usam `pwd`, permissões e links simbólicos; não substitua esses recursos por mocks
apenas para obter um resultado verde no Python do Windows.

Na raiz do clone, dentro do Ubuntu, prepare uma vez:

```sh
sudo apt-get update &&
sudo apt-get install -y python3-venv python3-dev libffi-dev build-essential &&
python3 -m venv "$HOME/.venvs/ender3-tests" &&
"$HOME/.venvs/ender3-tests/bin/pip" install -r scripts/klippy-requirements.txt
```

Rode as duas suítes, ainda na raiz do clone:

```sh
"$HOME/.venvs/ender3-tests/bin/python" -B -m unittest discover -s test/bmcu_integracao -p 'test_*.py' &&
"$HOME/.venvs/ender3-tests/bin/python" -B -m unittest discover -s test/e3v3se_display -p 'test_*.py' &&
git diff --check
```

Um ambiente já preparado pode usar outro caminho de virtualenv; substitua o
prefixo dos comandos pelo caminho correspondente. Não instale nem reinicie
serviços da impressora para rodar testes locais.

O workflow **Build test** também executa o verificador de estilo do Klipper.
`git diff --check` não substitui essa etapa: ele não detecta o limite de 80
caracteres dos arquivos Python/C. Em um checkout limpo no sistema de arquivos
Linux, com finais de linha LF, caminho sem espaços e sem arquivos privados,
rode também:

```sh
bash scripts/check_whitespace.sh
```

Não publique nem remova seus arquivos privados para obter esse checkout; use
uma cópia pública separada. O verificador percorre os diretórios diretamente,
inclusive arquivos ignorados que estejam presentes neles.

<a id="builds-and-limitations"></a>
## Compilação e limites

O [guia do cartão Mainsail nativo](Ender3_V3_SE_Mainsail.md#rebuild-and-verify)
explica o build web fixado e o simulador somente local. Confira separadamente
o contrato da fonte BMCU 1.1.2 real; isso inicia/para apenas um processo
inofensivo aguardando, sem abrir dispositivo serial:

```sh
git clone --depth 1 --branch v1.1.2 \
  https://github.com/jarczakpawel/BMCU-Klipper.git /path/to/bmcu-upstream
python3 scripts/ender3-v3-se/check_bmcu_contracts.py --source /path/to/bmcu-upstream
```

Para compilar o firmware, também são necessários `gcc-arm-none-eabi` e
`binutils-arm-none-eabi`. Siga o [guia de compilação](Ender3_V3_SE.md).
O Makefile exige um caminho sem espaços; se o clone estiver em uma pasta com
espaços no Windows, use uma cópia pública de trabalho no sistema de arquivos
Linux. Não copie arquivos privados para artefatos públicos de teste/compilação.

O CI descobre automaticamente os arquivos `test_*.py` das duas suítes.
Testes, build e cobertura não comprovam corte físico, captura nas engrenagens
nem fluxo pelo bico. A aceitação de hardware continua exigindo uma troca avulsa
supervisionada antes de testes multicor repetidos.
