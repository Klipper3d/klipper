# Análise da adaptação para Ender 3 V3 SE

[English](../E3V3SE_Port_Analysis.md) | **Português brasileiro**

Este documento registra a base da implementação. Não defende incorporar
integralmente os forks históricos.

<a id="compared-revisions"></a>
## Revisões comparadas

- `Klipper3d/klipper`: `72b3cdb4`, upstream atual no momento da análise
- `jpcurti/ender3-v3-se-klipper-with-display`: `d74d36bb`, tag `1.0.1`
- `0xD34D/klipper_ender3_v3_se`: `7e61c1cc`
- `0xD34D/ender3-v3-se-klipper-config`: `deb2cfd`
- `huhen/Ender3_V3_SE_for_klipper`: `6649c4b0`, GPLv3

Na análise, o repositório de trabalho era idêntico, byte a byte, ao HEAD oficial
do Klipper e não tinha mudanças locais nas fontes. O remoto `upstream` foi
então adicionado sem substituir `origin`.

<a id="display-fork-delta"></a>
## Diferenças do fork da tela

O fork da tela tinha 43 arquivos alterados/adicionados em relação ao upstream,
cerca de 8.777 linhas inseridas. As principais adições eram um módulo de tela
de 4.533 linhas, uma abstração de impressora de 530 linhas, driver TJC, ponte
serial genérica, PRTouch e documentação/recursos.

A adaptação preserva comportamento, não aquela estrutura. Não mantém mudanças
em `gcode_move.py`, `stepper.py`, documentação genérica G-code, PRTouch,
`hx711s`, `dirzctl`, `filter`, scripts antigos de CI ou binário de tela no Git.

Commits importantes revisados da tela incluem:

- `cd1e82a7`, `2e08a2c`, `a34fd9a5`, `09ec47c7`: ponte e interface inicial
- `c0cb2aa9`: strings longas
- `bf50bf6c`, `4567e70a`, `fea6b700`: sincronização de estado e posição
- `434275fe`, `bd9fbb67`, `b3f9bd3a`, `fc94f2aa`: pastas e mídia vazia
- `11e6dd26`, `459ee204`, `773e6073`, `78eb46f1`: sonda manual e macros
- `b65d4028`: coordenadas com sinal
- `6b75461c`, `2ce3670b`, `aacdcd6e`: tratamento das mensagens de status
- `0f6b57c9`, `f31055ee`, `d716359b`: controle de temperatura
- `0c84c5ad`: migração para `register_serial_response()`

<a id="machine-fork-delta"></a>
## Diferenças do fork da máquina

O fork da máquina tinha dez arquivos alterados/adicionados em relação ao
upstream, cerca de 1.383 linhas inseridas. Todas as mudanças substanciais
implementavam PRTouch com célula de carga: `prtouch.py`, `hx711s.py`,
`dirzctl.py`, suas fontes MCU e patches de apoio em `stepper.py` e
`src/Makefile`.

O repositório separado de configuração 0xD34D ajudou a conferir hardware
comunitário, mas não declara licença e não é a base de licenciamento deste
repositório. O mapeamento elétrico é rastreável ao pinout e à configuração
GPLv3 publicados por huhen. Foi conferido com a placa-alvo identificada
fisicamente e depois dividido em arquivos de placa, movimento, calibração,
tela e macros.

Curso e geometria da sonda foram recalculados de forma independente a partir
do envelope configurado e offsets da sonda. Em particular, os máximos da mesh
são os máximos do bico de 230 mm mais offsets de -23 mm e -14,5 mm.
Resultados particulares de PID, mesh, input shaper, pressure advance e
Z offset foram removidos.

O histórico 0xD34D credita bootuz-dinamon pelas descobertas da UART TMC e
FinalX1992 pela correção do sentido da rosca dos parafusos. Continuam nos
créditos públicos como contribuidores da conferência de hardware; seus
repositórios sem licença não são apresentados como fontes GPL.

<a id="issue-conclusions"></a>
## Conclusões das issues

Todas as issues abertas/fechadas foram triadas. Duplicatas tecnicamente
relevantes foram agrupadas pela causa raiz:

- Incompatibilidade de ponte/compilação/versão: tela #4, #14, #15, #18, #20,
  #63, #70, #73, #74, #76, #77, #81, #90, #92, #93, #101, #119, #127, #134,
  #148, #152; máquina #3, #6, #10, #14, #16, #25.
- Nomes longos, UTF-8 e arquivos: tela #11, #28, #32, #35, #52, #98, #124,
  #133, #139, #152.
- Objetos ausentes e estado desatualizado: tela #3, #10, #12, #50, #57, #116;
  máquina #24.
- Movimento com sinal e macros inseguras: tela #39, #41, #42, #50, #108, #136;
  máquina #2, #5, #12, #20.
- Pausa/retomada/cancelamento e temperatura: tela #33, #55, #57, #79, #82,
  #121, #131.
- Z offset, mesh e PRTouch: tela #47, #49, #59, #85, #104; máquina #1, #4,
  #5, #12, #20, #28, #29, #32 e PR #33.

A falha atual do PRTouch não é uma questão de compatibilidade de uma linha.
O Klipper atual retorna valores `ProbeResult` imutáveis e mudou o significado
da coordenada Z retornada por `create_probe_result()`. O código histórico
também persiste valor diferente do `z_adjust` ao vivo, tem viés de disparo
dependente de velocidade/temperatura e pode interagir incorretamente com mesh
ativa. Relatos comunitários incluem dano físico à mesa. Portanto, PRTouch
é excluído do perfil inicial seguro.

<a id="upstream-facilities-used"></a>
## Recursos upstream utilizados

- `MenuKeys` e `buttons`
- `register_serial_response()`
- `virtual_sdcard` e `print_stats`
- `display_status`
- `pause_resume`
- `gcode.run_script()`
- `bltouch`, `probe`, `manual_probe` e `bed_mesh`
- Relato existente de versão da compilação, Kconfig e dicionário MCU
- Input Shaping oficial sem modificação

<a id="remaining-physical-uncertainties"></a>
## Incertezas físicas restantes

- A Creality não forneceu esquema público que vincule cada revisão C14 a uma
  MCU, bootloader ou oscilador.
- A GD32F303RET6 da máquina-alvo foi confirmada fisicamente, mas o Klipper
  upstream a compila pelo caminho compatível STM32F103.
- Bootloader de 28KiB e cristal de 8 MHz são sustentados por compilações
  comunitárias funcionais, não por especificação de hardware da Creality.
- Firmware 1.0.6 da tela é a referência validada de recursos/protocolo;
  outras versões continuam não verificadas.

Nenhuma validação somente por software elimina essas incertezas de hardware.
