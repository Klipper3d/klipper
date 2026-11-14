<a id="packaging-klipper"></a>
# Empacotamento do Klipper

[English](../Packaging.md) | **Português brasileiro**

O Klipper é uma espécie de anomalia de empacotamento entre os programas python, pois não usa setuptools para compilar e instalar. Algumas notas sobre a melhor forma de embalá-lo são as seguintes:

<a id="c-modules"></a>
## módulos C

O Klipper usa um módulo C para lidar com alguns cálculos cinemáticos mais rapidamente. Este módulo precisa ser compilado no momento do empacotamento para evitar a introdução de uma dependência de tempo de execução em um compilador. Para compilar o módulo C, execute `python2 klippy/chelper/__init__.py`.

<a id="compiling-python-code"></a>
## Compilando código python

Muitas distribuições têm uma política de compilar todo o código python antes do empacotamento para melhorar o tempo de inicialização. Você pode fazer isso executando `python2 -m compileall klippy`.

<a id="versioning"></a>
## Controle de versão

Se você estiver compilando um pacote do Klipper a partir do git, é prática usual não distribuir um diretório .git, portanto o versionamento deve ser tratado sem o git. Para isso, use o script distribuído em `scripts/make_version.py`, que deve ser executado da seguinte forma: `python2 scripts/make_version.py YOURDISTRONAME > klippy/.version`.

<a id="sample-packaging-script"></a>
## Exemplo de script de empacotamento

klipper-git é empacotado para Arch Linux, e possui um PKGBUILD (script de construção do pacote) disponível em [Arch User Repository](https://aur.archlinux.org/cgit/aur.git/tree/PKGBUILD?h=klipper-git).
