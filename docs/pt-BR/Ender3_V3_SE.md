# Creality Ender 3 V3 SE (GD32F303RET6)

[English](../Ender3_V3_SE.md) | **Português brasileiro**

Este perfil é especificamente para uma Ender 3 V3 SE com:

- Placa principal marcada como `CR4NS200320C14`
- MCU `GD32F303RET6` verificada fisicamente
- Tela original de 3,2 polegadas TJC3224/DWIN
- Conexão serial USB entre a impressora e um host Linux

A revisão da placa, sozinha, não identifica a MCU. Não use uma compilação
STM32F401 neste alvo apenas porque outra placa C14 contém esse processador.

<a id="safety-status"></a>
## Situação de segurança

Este repositório pode validar o comportamento do software e compilar o
firmware, mas não pode validar fisicamente movimento, controle de saída dos
aquecedores, comportamento da sonda ou bootloader em uma impressora específica.
Mantenha uma mão na energia de emergência durante as primeiras verificações.
Não inicie uma impressão até conferir homing, aquecedores, CR-Touch, Z offset
e limites de deslocamento em baixa velocidade.

O código de Z offset automático PRTouch baseado em célula de carga foi
intencionalmente excluído. A implementação histórica é incompatível com as APIs
atuais de sonda e possui casos documentados de offsets incorretos e colisões
do bico com a mesa. Este perfil usa o CR-Touch original por meio da
implementação `bltouch` do Klipper oficial.

<a id="host-installation"></a>
## Instalação no host

Baixe o código-fonte e os arquivos de firmware da mesma
[release no GitHub](https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu/releases).
A tag da release identifica a versão de origem de seu `klipper.bin` e
`klipper.dict`; não execute um checkout de outro commit com esse binário.

No MainsailOS para a Orange Pi Zero 3, instale o Klippy deste repositório e
use o mesmo commit usado para compilar o firmware da MCU. Mantenha as
instalações padrão de Moonraker e Mainsail; nenhuma delas exige um fork.

**Somente para uma instalação nova**, copie a configuração de exemplo para
o diretório de configuração do Mainsail como ponto de partida. Não substitua
um `printer.cfg` já calibrado; instalações existentes usam o fluxo de atualização
abaixo:

```sh
cp config/printer-creality-ender3-v3-se-gd32f303-2023.cfg \
  ~/printer_data/config/printer.cfg
cp -r config/ender3-v3-se ~/printer_data/config/
```

Confirme o caminho `[mcu] serial` com:

```sh
ls /dev/serial/by-id/
```

A configuração de referência exige `[virtual_sdcard]`, `[pause_resume]` e
`[display_status]`. Essas seções já estão incluídas por
`config/ender3-v3-se/display.cfg`.

<a id="deterministic-firmware-build"></a>
## Compilação determinística do firmware

O alvo de compatibilidade comprovado para esta placa GD32F303 é o STM32F103
do Klipper. Ele usa deliberadamente o mapa conservador de flash e RAM do F103
oficial; este projeto não afirma que o Klipper suporte o GD32F303 nativamente.

As seleções exatas no `make menuconfig`, com os nomes da interface em inglês,
são:

- `Enable extra low-level configuration options: yes`
- `Micro-controller architecture: STMicroelectronics STM32`
- `Processor model: STM32F103`
- `Only 10KiB of RAM: no`
- `Disable SWD at startup: no`
- `Bootloader offset: 28KiB bootloader`
- `Clock reference: 8 MHz crystal`
- `Communication interface: Serial on USART1 PA10/PA9`
- `Baud rate: 250000`
- `Creality Ender 3 V3 SE stock display bridge: yes`

A configuração de compilação versionada é mais fácil e menos sujeita a erros:

```sh
make clean
make distclean
cp test/configs/e3v3se-gd32f303.config .config
make olddefconfig
make
```

Use um caminho de checkout sem espaços. O Makefile oficial do Klipper expande
o diretório de trabalho em comandos shell que não suportam espaços no caminho.

Saídas esperadas:

- `out/klipper.bin`: firmware copiado para o cartão SD da impressora
- `out/klipper.dict`: dicionário do protocolo da MCU, incluindo os comandos da tela
- `out/defconfig`: configuração de compilação normalizada

O firmware incorpora a versão Git do Klipper e o Kconfig normalizado. O Klippy
registra ambos durante a conexão. Os downloads da release também incluem
dicionário, configuração normalizada, checksums, SHA do commit, base upstream,
alvo de hardware, alvo de MCU e data da compilação em UTC.

<a id="flashing"></a>
## Gravação

1. Use um cartão microSD pequeno formatado como FAT32 com unidade de alocação
   de 4096 bytes.
2. Copie `out/klipper.bin` para o cartão.
3. Renomeie-o se esse nome foi usado na gravação anterior. Use um nome curto
   no formato 8.3 terminado em `.bin`, por exemplo `kli0916.bin`.
4. Desligue a impressora, insira o cartão e ligue-a.
5. Aguarde antes de remover a energia ou o cartão.
6. Confira a versão da MCU em `klippy.log` antes de habilitar a seção da tela.

Se o host informar ausência de `e3v3se_display_send` ou incompatibilidade de
protocolo, não continue reiniciando. Recompile e regrave a MCU a partir do mesmo
commit do host. O módulo do host rejeita intencionalmente um firmware sem a
opção da tela, com erro de configuração.

<a id="commissioning-order"></a>
## Ordem de comissionamento

1. Verifique se todos os aquecedores permanecem desligados e se ambos os sensores
   apresentam temperaturas ambientes plausíveis depois da conexão.
2. Aqueça somente o hotend a 50 C. Confirme que apenas o sensor correspondente
   sobe, desligue-o e confira o resfriamento. Corte a energia imediatamente se
   houver comportamento inesperado.
3. Repita somente com a mesa a 40 C, desligue-a e confira o resfriamento.
4. Confira os fins de curso X e Y com `QUERY_ENDSTOPS`, acionando-os manualmente.
5. Confira o avanço/recolhimento do CR-Touch e o estado acionado sem referenciar Z.
6. Referencie X e Y em baixa velocidade, pronto para cortar a energia.
7. Referencie Z sobre a mesa com o bico em altura segura acima dela.
8. Confira direções de deslocamento e limites configurados.
9. Execute `PID_CALIBRATE` separadamente para hotend e mesa, depois `SAVE_CONFIG`.
10. Execute `PROBE_CALIBRATE`, use `TESTZ` manual, depois `ACCEPT` e `SAVE_CONFIG`.
11. Execute `BED_MESH_CALIBRATE`, inspecione a malha e salve-a se apropriado.
12. Calibre rotation distance, pressure advance e input shaping na máquina
    física. Nenhum valor dessas calibrações é fornecido aqui.

O controle inicial dos aquecedores é `watermark` para não distribuir valores
PID medidos em outra impressora. O `z_offset: 0.0` inicial do BLTouch é um valor
provisório que precisa de calibração antes de imprimir. Esses três valores
provisórios, gerenciados pelo autosave, ficam intencionalmente na configuração
principal da impressora; movê-los para um include impede o Klipper atual de
salvar resultados de calibração de PID e da sonda.

<a id="updating"></a>
## Atualização

Mantenha `origin` apontando para este fork e `upstream` apontando para
`https://github.com/Klipper3d/klipper.git`.

Para a atualização v2 da integração somente no host, use uma fonte separada
em um checkout limpo. **Não execute `git pull` no checkout ativo antes do
snapshot.** O atualizador preserva a árvore anterior inteira de `~/klipper`,
incluindo a tela e os metadados Git, antes de ativar um clone independente.
Compara fontes da MCU, bibliotecas, configurações-alvo, Makefile/comandos de
compilação e requisitos Python; diferenças são recusadas para revisão separada
do firmware e do ambiente. Não grava firmware nem descarta alterações locais
em arquivos rastreados.

Depois que a v2.0.1 for publicada:

```sh
git clone --branch v2.0.1 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source"
```

Se a inicialização falhar, host/display e configurações anteriores são
restaurados e conferidos antes de retomar o uso normal. As evidências atuais
das rotas são mantidas, não substituídas por um snapshot antigo do estado de
execução. Se não for possível validar a recuperação, os serviços ficam parados.
Os dois checkouts são mantidos para diagnóstico.

Em futuras atualizações que alterem entradas da MCU/compilação, revise o par
host/MCU e continue usando fontes e firmware correspondentes da mesma release:

1. Integre o commit upstream desejado neste repositório.
2. Execute os testes unitários da tela e a suíte de testes do Klipper.
3. Compile o alvo GD32F303.
4. Atualize o Klippy na Orange Pi para exatamente esse commit.
5. Grave o artefato da MCU produzido a partir desse mesmo commit.
6. Mantenha a configuração da máquina em controle de versão separado ou backup.

Não atualize somente o Klippy ou somente a MCU quando um comando do protocolo
da tela tiver mudado.

<a id="physical-acceptance-tests"></a>
## Testes físicos de aceitação

Os testes abaixo exigem a impressora e não são feitos pelo CI:

- 50 ciclos consecutivos de `RESTART`
- 50 ciclos consecutivos de `FIRMWARE_RESTART`
- Navegação repetida por todos os menus
- Diretórios SD virtuais vazios, com arquivo único, múltiplos arquivos, pastas
  aninhadas, nomes longos, UTF-8 e nomes especiais
- Posições X, Y e Z negativas
- Pausa, retomada e cancelamento pelo Mainsail e pela tela original
- Sincronização de temperatura, fatores de velocidade/fluxo, posição, Z offset,
  início e conclusão de impressão entre o Mainsail e a tela
- Impressão longa monitorando `bytes_retransmit` da MCU, timeouts e status de
  quadros descartados da ponte

Testes de estresse de software cobrem estado dos quadros, limites de bytes,
limites UTF-8, valores com sinal e navegação de arquivos. Não substituem a
validação física.

<a id="bmcu-and-display-languages"></a>
## BMCU e idiomas da tela

Consulte a [integração BMCU 1.1.2](Ender3_V3_SE_BMCU.md) para UART1, switch PC15,
sequências de filamento, seleção de idioma e testes físicos de aceitação da tela.
