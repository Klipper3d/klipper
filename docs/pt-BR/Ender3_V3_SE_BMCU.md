# Ender 3 V3 SE: BMCU 1.1.2 e tela original

[English](../Ender3_V3_SE_BMCU.md) | **Português brasileiro**

Para integração por switch, instalação e ferramentas privadas de recuperação,
consulte a [integração por switch](Ender3_V3_SE_Configuracao_Validada.md).
O restante desta página descreve o modelo original não comissionado da v0.2.0.
Não execute `BMCU_SETUP_ENDER` sobre o endpoint validado `extruder`.

Esta integração usa **uma extrusora física** (`extruder`) e quatro canais
direcionados ao endpoint `main`, com driver `generic_single_extruder`.
Use o [instalador oficial v1.1.2](https://github.com/jarczakpawel/BMCU-Klipper/tree/v1.1.2)
para módulos BMCU, `[bmcu]`, transporte e serviços. Não copie seus módulos para
este fork nem adicione outra seção `[bmcu]`. O firmware correspondente é
**1.0.4**, conforme o
[arquivo de versão](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/version).
Instalações existentes precisam de [manutenção separada](releases/v2.0.1.md#upgrade-and-rollback),
não de atualizar o pacote às cegas mantendo módulos Ender antigos ativos.
O arquivo fornecido tem 56.492 bytes e SHA-256:
`b85a21a14e61754db021302e0d54e4c6f9a309d0276d4fb5842b45e3e9a27327`.
Isso identifica o arquivo recebido; confira a versão gravada no status BMCU.

<a id="configuration-and-wiring"></a>
## Configuração e ligação

- `macros.cfg`: sequências da extrusora física e parâmetros de calibração.
- `bmcu.cfg`: callbacks genéricos e configuração manual dos canais.
- `bmcu-sensor.cfg`: modelo de configuração do switch na Orange Pi.
- `display.cfg`: idioma, perfis de material e atalhos.

Esses arquivos ficam em `config/ender3-v3-se/`.

A UART1 da Orange Pi Zero 3 usa TX **PG06** e RX **PG07**. Conecte TX do host ao
RX da BMCU e RX do host ao TX da BMCU, com GND comum. Habilite UART1 na imagem
Linux instalada, identifique seu TTY e confira que nenhum console ou outro
processo o ocupa. Não presuma `/dev/ttyS1`. Informe a porta verificada ao
instalador oficial. O formato do pacote é `devices: bmcu0,<UART1 port>`, com
baud padrão 115200. Preserve configuração, serviços e estado persistente do
instalador. Referência da multiplexação de pinos:
[DTS da Orange Pi Zero 3](https://github.com/orangepi-xunlong/orangepi-build/blob/next/external/packages/pack-uboot/sun50iw9/bin/dts/orangepizero3-u-boot-current.dts).

O switch fica **após a BMCU**: COM conecta ao pino físico **6/GND**, NO ao
pino físico **16/PC15**. Configure a MCU Linux conforme
[RPi_microcontroller.md](RPi_microcontroller.md). No host, execute `gpiodetect`
e `gpioinfo`, identifique chip/offset do PC15 e confira que a linha está livre.
Descomente `bmcu-sensor.cfg`, substituindo `N` e `O` em
`^!host:gpiochipN/gpioO` pelos índices reais. Reutilize um `[mcu host]` existente.
Pull-up com inversão detecta NO fechando para GND. Verifique os dois estados com
`QUERY_FILAMENT_SENSOR SENSOR=bmcu_entry`.

<a id="display-language"></a>
## Idioma da tela

O padrão é inglês. Escolha **Configurações → Idioma → English / Português**
na tela para mudar imediatamente. Isso prepara a preferência pela API de
configuração do Klipper, sem reiniciar. **Configurações → Salvar ajustes**
persiste a escolha e reinicia o Klipper após confirmação, somente fora de
impressão ou operação ocupada. Para definir manualmente, coloque isto no
`printer.cfg` principal, não em um include que conflitaria com `SAVE_CONFIG`:

```ini
[e3v3se_display]
language: pt_BR
```

Use `language: en` para inglês. Edições manuais exigem reiniciar o Klipper;
a escolha na tela não. Código, comentários e descrições de macros usam inglês;
a documentação pública tem pares EN/PT-BR. Rótulos nativos, estados, mensagens
de operação e erros conhecidos são traduzidos. Rótulos personalizados e
diagnósticos externos desconhecidos mantêm o texto configurado/original.

O menu parado é **Imprimir / Filamento / Temperatura / Mover / Calibrar /
Configurações**, com **Atalhos** apenas para atalhos personalizados.
**Temperatura** concentra pré-aquecimento/resfriamento; **Mover** concentra
homing, movimentos manuais, motores e batente opcional do cortador.
**Filamento / BMCU** concentra carga/retirada dos canais. A integração validada
também adiciona ali o **Reset BMCU** protegido após remoção manual.
Executa `BMCU_ENDER_RESET CONFIRM=1`, não é reset de fábrica nem apaga calibração.
Impressões ativas/pausadas e operações ocupadas o bloqueiam. Uma troca pendente
pausada usa **Recuperar filamento** e
`BMCU_CONFIRM_CLEAR_PATH CHANNEL=<1-4> CONFIRM=1`, nunca um reset global.

<a id="arrival-strategy-and-the-unmeasured-distance"></a>
## Estratégia de chegada e distância não medida

Meça o percurso do filamento do switch às engrenagens e registre
`sensor_to_gears_mm`; `-1` significa desconhecido. Isso **não é convertido em
movimento da extrusora**: ela não pode capturar uma ponta que não chegou às
engrenagens.

No [BMCU 1.1.2](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/klippy/extras/bmcu_core/manager.py),
a detecção por `ENTRY_SENSOR` para a alimentação BMCU imediatamente.
`FINAL_SEARCH_MM` limita a busca, não um avanço adicional após a detecção.
`BMCU_ROUTE_FEED` exige operação isolada, não pode rodar no callback de carga
e deixa a rota incerta. Não o aninhe em `BMCU_TOOLHEAD_PREPARE`.

Escolha e valide fisicamente `_ENDER_BMCU_SETTINGS.arrival_mode`:

- `"contact"`: a BMCU continua alimentando até o contato nas engrenagens, com
  sua lógica de transporte/buffer. Um switch PC15 distante é diagnóstico, não
  `ENTRY_SENSOR`. Confira se o contato ocorre nas engrenagens sem dobrar o
  filamento nem encontrar obstrução intermediária no PTFE.
- `"sensor"`: somente se acionar o switch garantir que a ponta já pode ser
  capturada pela extrusora. Reposicione o switch se necessário.

Defina `max_route_mm` como limite medido do percurso total e ajuste
`contact_timeout`. Execute `BMCU_SETUP_ENDER` **manualmente**, com impressora
parada e rotas vazias. Ele configura `main`, atribui quatro canais e limpa
sensores incompatíveis do endpoint. Não roda na partida nem apaga o estado do
pacote. Defina `arrival_validated: 1` após verificar fisicamente. O modo inicial
é `"unconfigured"`; a tela não inicia transporte antes da validação.
O modo por sensor exige um objeto de sensor PC15 configurado e verificado.

<a id="complete-load-and-cutter-free-unload"></a>
## Carga completa e retirada sem cortador

Edite `_ENDER_FILAMENT_SETTINGS`. As distâncias começam em zero, e
`calibrated: 0` bloqueia macros antes de aquecer/mover. Distâncias dos testes
unitários são sintéticas, não valores recomendados para a impressora.

| Parâmetro | Medição/efeito |
| --- | --- |
| `capture_mm` | Captura da ponta entregue nas engrenagens |
| `melt_mm` | Percurso adicional à zona de fusão/bico |
| `purge_mm` | Purga após alcançar o bico |
| `tip_push_mm` | Extrusão curta antes da formação da ponta |
| `tip_retract_mm` | Retração inicial rápida |
| `tip_dwell_ms` | Espera para resfriar a ponta |
| `cooling_move_mm`, `cooling_moves` | Ciclos de avanço/recuo de resfriamento, percurso líquido zero |
| `unload_mm` | Retirada líquida total do bico até liberar as engrenagens |
| `*_speed` | Velocidades das etapas em mm/s |
| `chunk_mm` | Blocos de até 20 mm, com cancelamento verificado entre blocos |

Teste captura, avanço ao bico e purga separadamente. Inspecione a ponta retirada,
ajuste resfriamento, ciclos, distâncias e velocidades para o material e confira
passagem livre por PTFE/combinador. Confirme a liberação completa das engrenagens
antes de a BMCU puxar, depois defina `calibrated: 1`. Nenhuma calibração física
foi feita aqui.

`LOAD_FILAMENT`/`UNLOAD_FILAMENT` operam a extrusora. Para transporte BMCU
completo, use **Filamento**, ou `BMCU_LOAD DEVICE=bmcu0 CHANNEL=0` /
`BMCU_UNLOAD DEVICE=bmcu0 CHANNEL=0`. Canais 1–4 da tela correspondem a comandos
0–3. Com a BMCU pronta, carregar na tela abre os canais e retirar usa a rota
carregada confirmada. Origens lógicas `T1+` não criam extrusoras físicas.

Callbacks genéricos `BMCU_TOOLHEAD_PREPARE`/`BMCU_BEFORE_PULLBACK` usam `WAIT=1`
e retornam somente após purga/liberação e `M400`, conforme o
[contrato genérico](https://github.com/jarczakpawel/BMCU-Klipper/blob/v1.1.2/printers/Generic/README.md).
Sem `WAIT=1`, macros manuais aquecem de forma assíncrona; não use esse modo nas
trocas do fatiador nem imediatamente antes de imprimir.

<a id="temperature-state-and-cancellation"></a>
## Temperatura, estado e cancelamento

Tela e macros compartilham um controlador: reutilize um alvo válido ou selecione
PLA/PETG. O material do callback escolhe o perfil alternativo; outros materiais
precisam de alvo adequado existente ou `TEMP` explícito, por exemplo
`LOAD_FILAMENT TEMP=215 WAIT=1`. A proteção de extrusão a frio continua ativa.
O movimento aguarda temperatura atual no máximo 1 grau abaixo do alvo e
`can_extrude` verdadeiro. Mudanças de alvo, timeout ou falhas abortam de forma
visível. Cancelamento/falha restaura o alvo anterior, salvo se outro cliente o
alterou. O sucesso mantém o alvo.

Durante **Aquecendo bico**, temperaturas atual/alvo continuam atualizando.
Clicar cancela; inícios repetidos não enfileiram movimentos. Em movimento,
o cancelamento vale após o bloco enviado e antes do próximo. O estado G-code
é restaurado no sucesso/falha, incluindo modo de extrusão, coordenadas E,
velocidade e fatores de velocidade/fluxo. As sequências usam 100% de velocidade
e fluxo para a calibração independer dos ajustes da impressão.

`PAUSE` mantém rota e hooks do pacote. `RESUME` é bloqueado durante operações
de filamento antes do hook de reposição do pacote, preservando-o quando livre.
A proteção é instalada depois de pronto e verificada antes das operações para
cobrir hooks adicionados por configuração manual do endpoint. `CANCEL_PRINT`
cancela operações locais, solicita `BMCU_STOP`, desliga aquecedores/ventoinha
e cancela SD virtual. O cancelamento na tela funciona no aquecimento síncrono
do callback. G-code externo é serializado pelo mutex do Klipper e pode esperar
por esse callback. Na 1.1.2, `BMCU_STOP` para motores, mas não é API completa
de cancelamento do fluxo do host. A tela impede o próximo callback da extrusora
de mover e pede parada BMCU quando a fase atual do transporte retorna.
Aguarde o movimento físico parar e inspecione a rota antes de recuperar;
nunca confirme `LOADED`/`EMPTY` sem inspeção.

O canal ativo vem do mapa `loaded_tools` em execução, não do último clique
ou de `now_channel`. A 1.1.2 congela snapshots públicos do dispositivo durante
impressões e pausas. Um adaptador somente leitura usa conexão e rotas em memória
sem I/O UART; erros de execução e rotas incertas bloqueiam ações. BMCU ausente,
offline ou de versão divergente não impede navegar. Canais indisponíveis ficam
desabilitados, rotas incertas bloqueiam movimento. Operações de painel/fatiador
continuam sob a BMCU; callbacks também exigem calibração. Sem sensor de endpoint,
a reposição genérica pausa para troca manual. Habilite reposição automática
somente após validar fisicamente.

<a id="updating-an-existing-installation"></a>
## Atualizar uma instalação existente

Para a integração v2 por switch, siga as instruções de atualização com fonte
separada e rollback completo do host/display no
[guia de instalação](Ender3_V3_SE_Configuracao_Validada.md#install-or-update-an-existing-integration).
Não atualize o `~/klipper` ativo com `git pull` antes de preservá-lo.
O instalador precisa rodar de uma fonte limpa separada com `--host-source`
apontando para ela; mudanças em entradas MCU/compilação ou requisitos Python
exigem revisão separada. Não grava firmware nem instala dependências. Valores
calibrados existentes são preservados. **Refatie trabalhos antigos com o perfil
v2**; não há aliases de comandos em português nem conversão automática de
G-code armazenado. O modelo original acima é separado: não substitua sua
configuração de endpoint pelo comando de comissionamento da integração validada.

<a id="pending-physical-acceptance"></a>
## Aceitação física pendente

Teste canais 1–4, captura/purga, formação da ponta sem cortador, pausa/retomada,
cancelamento no aquecimento/movimento, timeout/desconexão e recuperação de rota
incerta. Confira os dois idiomas, acentos portugueses, todas as linhas, rolagem
de nomes longos e legibilidade do estado/canal em 240×320. Os testes verificam
limites UTF-8 e do protocolo, não os glifos da fonte interna da tela.
