# Arquitetura da tela original da Ender 3 V3 SE

[English](../E3V3SE_Display.md) | **Português brasileiro**

<a id="data-path"></a>
## Caminho dos dados

```text
Mainsail / Moonraker
        |
        v
Objetos Klippy e comandos G-code
        |
        v
Pacote Python e3v3se_display
        |
        v
Protocolo MCU do Klipper por USART1 PA10/PA9
        |
        v
Ponte da tela GD32F303RET6
        |
        v
USART2 PA3/PA2 a 115200 baud
        |
        v
Tela original TJC3224 / DWIN
```

O encoder rotativo não chega pela UART da tela. Está ligado à placa principal
em PA12, PA11 e PB1, usando `MenuKeys` e `buttons` do Klipper oficial.

<a id="mcu-module"></a>
## Módulo da MCU

`src/stm32/e3v3se_display.c` é intencionalmente específico do alvo. Evita portas
sem uso, configurações inválidas, instâncias globais mutáveis e quadros grandes
demais da ponte genérica histórica.

O módulo fornece:

- `config_e3v3se_display oid=%c baud=%u`
- `e3v3se_display_send oid=%c data=%*s`
- `e3v3se_display_response oid=%c data=%*s`
- `e3v3se_display_get_status oid=%c`
- `e3v3se_display_status ... rx_overflows ... tx_overflows`

Também incorpora `E3V3SE_DISPLAY_BRIDGE=1` e `E3V3SE_DISPLAY_MAX_CHUNK=40` no
dicionário da MCU. O Klippy confere a primeira constante antes de enviar a
configuração. Host combinado com firmware sem a ponte falha cedo com erro de
configuração que orienta a correção, em vez do loop histórico
`Unknown command: serial_bridge_send`.

USART2 usa buffers circulares RX/TX de 256 bytes. O estouro descarta dados da
tela em vez de parar o controle de movimento ou corromper o protocolo host-MCU.

<a id="message-limit"></a>
## Limite das mensagens

O Klipper atual usa:

- `MESSAGE_MAX = 64`
- Dois bytes de cabeçalho
- Três bytes finais de CRC/sincronização
- No máximo 59 bytes de conteúdo codificado

O comando ainda precisa codificar msgid, OID e comprimento da string dinâmica.
Por isso, cada fragmento do transporte da tela é limitado a 40 bytes.
O host fragmenta todos os quadros, inclusive desenho bruto; a MCU também informa
dados recebidos da tela em blocos de no máximo 40 bytes.

A fragmentação ocorre após codificar em UTF-8. O truncamento visual também
opera nos bytes codificados, removendo somente um ponto de código final
incompleto. Nomes longos selecionados rolam na tela; a fragmentação do transporte
independe do comprimento visível.

<a id="host-package"></a>
## Pacote do host

O pacote em `klippy/extras/e3v3se_display/` contém:

- `transport.py`: APIs atuais da MCU, fila limitada, fragmentação de 40 bytes
  e cadência UART.
- `tjc3224.py`: criação imutável de quadros e subconjunto mínimo de desenho DWIN.
- `model.py`: helpers puros de navegação e validação de nomes de arquivos.
- `ui.py`: status textual, arquivos, movimento, temperatura, ajustes, calibração,
  controle de impressão e menus de macros personalizadas.
- `__init__.py`: integração dos objetos Klipper e validação de dependências.

Não há buffers de quadros mutáveis no nível da classe. Cada `RESTART` cria novo
transporte, driver de protocolo, navegador e estado visual. Nenhum dado da
instância anterior pode ser anexado ao primeiro quadro da nova instância.

Todos os callbacks periódicos e de teclas capturam exceções da tela. Erros de
renderização são registrados, mas não podem provocar shutdown do Klipper.

Nomes selecionados são passados diretamente a `virtual_sdcard` por um
`GCodeCommand` construído; não são interpolados em script G-code. Isso preserva
espaços, aspas, texto não ASCII, `#`, `;` e barras invertidas literais sem criar
caminhos de injeção de comandos.

<a id="source-of-truth"></a>
## Fonte de verdade

A tela lê status novo a cada segundo de:

- `extruder`
- `heater_bed`
- `fan`, quando configurado
- `toolhead`
- `gcode_move`
- `print_stats`
- `virtual_sdcard`
- `pause_resume`
- `display_status`
- `manual_probe`

A tela não mantém um segundo estado de impressão. Ações do Mainsail/Moonraker
aparecem na atualização seguinte. Ações da tela invocam os mesmos comandos
G-code públicos usados na interface web.

<a id="screen-firmware"></a>
## Firmware da tela

A implementação histórica exigia firmware Creality 1.0.6 porque seus menus
usavam IDs fixos de imagens de idioma, ícones e recursos de tela inteira.
Esses IDs podem mudar entre versões.

Esta implementação desenha com primitivas do protocolo e texto, sem distribuir
o binário opaco `e3v3sedisplay.bin`. A versão 1.0.6 permanece a referência
validada, pois é a documentada pelo projeto de referência. Outras versões podem
implementar o mesmo protocolo, mas não são declaradas compatíveis sem teste
físico.

<a id="ui-behavior"></a>
## Comportamento da interface

O painel mostra temperaturas, posições X/Y/Z com sinal, estado da impressão,
progresso, nome do arquivo e mensagens `M117`/`SET_DISPLAY_TEXT`. O menu oferece:

- Seleção recursiva do SD virtual com indicação explícita de diretório vazio
- Homing e movimentos relativos limitados
- Ajuste de bico, mesa, ventoinha e fatores de velocidade/fluxo
- Pausar, retomar e cancelar com confirmação
- Ajuste de Z offset ao vivo após referenciar Z
- `PROBE_CALIBRATE` explícito para CR-Touch
- `BED_MESH_CALIBRATE` explícito
- Sonda manual com `TESTZ`, `ACCEPT` e `ABORT`
- Macros de carga/retirada que rejeitam extrusão a frio
- Entradas de macros personalizadas

Movimento é recusado se o eixo não está referenciado. Extrusão é recusada se
o Klipper informa `can_extrude=false`. A tela não executa limpeza do bico,
PRTouch ou procedimento automático de Z offset.

<a id="menus-and-language"></a>
## Menus e idioma

O menu parado é **Imprimir / Filamento / Temperatura / Mover / Calibrar /
Configurações**, mais **Atalhos** somente quando há macros personalizadas.

- **Imprimir:** navegador recursivo de SD virtual e início confirmado do arquivo.
- **Filamento:** carga/retirada comum sem BMCU; com ela, **Filamento / BMCU**
  com carga dos quatro canais físicos e retirada do canal ativo.
- **Temperatura:** alvos do bico/mesa, ventoinha, aquecer PLA/PETG/TPU e esfriar.
- **Mover:** homing, movimentos X/Y/Z/E limitados, motores e batente opcional.
- **Calibrar:** CR-Touch, mesh e ajuste dos parafusos quando configurado.
  A sondagem manual usa os comandos existentes `TESTZ`, `ACCEPT` e `ABORT`.
- **Configurações:** idioma e salvamento separado e confirmado dos ajustes.

**Configurações → Idioma → English / Português** aplica a escolha imediatamente
e a prepara por `configfile.set`; não reinicia o Klipper.
**Configurações → Salvar ajustes** executa `SAVE_CONFIG` após confirmação para
persistir e reiniciar. Salvar é recusado durante impressão, pausa ou operação
ocupada. Os valores são `en` e `pt_BR`. O instalador move preferências de idioma
de includes para o `printer.cfg` principal, evitando conflitos do autosave
nativo. Comandos e parâmetros permanecem em inglês nos dois idiomas da tela.
Rótulos personalizados e diagnósticos externos desconhecidos mantêm o original.

<a id="bmcu-reset-and-paused-recovery"></a>
## Reset BMCU e recuperação pausada

O **Reset BMCU** da integração validada exige confirmar que hotend e caminho
compartilhado foram desobstruídos manualmente, depois invoca
`BMCU_ENDER_RESET CONFIRM=1`. Reconcilia rotas, não ajustes de fábrica;
não move filamento, apaga calibração nem recupera impressão pausada.
Impressão, pausa, recuperação pendente e operações ocupadas bloqueiam a ação.
Carga em rota incerta fica desabilitada, mas o reset protegido continua
disponível fora de impressão. O backend revalida ao executar a confirmação.

Durante a impressão, pausa/ajuste/cancelamento continuam contextuais.
Uma troca pausada com falha oferece **Recuperar filamento**, mostrando canal
físico e detalhes da falha. **Confirmar caminho livre** confirma a inspeção
física do operador e chama `BMCU_CONFIRM_CLEAR_PATH CHANNEL=<1-4> CONFIRM=1`
para aquele canal. Retomar/cancelar reutilizam a recuperação existente, nunca
outro estado de impressão ou reset global. Veja o
[guia de recuperação](Ender3_V3_SE_Print_Recovery.md).

Com cortador calibrado instalado em estado incerto, a recuperação oferece
somente **Fechar batente**. Após confirmar folga do cabeçote, envia
`CUTTER_SERVO POSITION=RETRACTED CONFIRM=1` e retorna à recuperação, sem
homing, movimentos manuais ou acionamento do batente. **Mover**, parado,
oferece as duas posições com a mesma confirmação e backend. Comanda apenas
o servo; a integração mantém os intertravamentos e o homing X primeiro.

<a id="custom-shortcuts"></a>
## Atalhos personalizados

O modelo não duplica mais carga/retirada em seções padrão de macros da tela.
As macros G-code dessas operações continuam disponíveis. Entradas
`[e3v3se_display macro <name>]` com `label` e `gcode` continuam suportadas;
nomes duplicados causam erro de configuração. O instalador remove somente
os padrões exatos redundantes, preservando rótulos e scripts personalizados.

<a id="diagnostics"></a>
## Diagnóstico

Use:

```text
ENDER3V3SE_DISPLAY_STATUS
ENDER3V3SE_DISPLAY_REFRESH
```

`ENDER3V3SE_DISPLAY_STATUS` informa conexão, bytes na fila, quadros descartados
pela fila limitada do host e os dois contadores de estouro UART da MCU.
O Klippy consulta os contadores e registra cada aumento. Versão do firmware,
Kconfig, constantes e dicionário de comandos já são registrados pelo Klipper
oficial atual.
