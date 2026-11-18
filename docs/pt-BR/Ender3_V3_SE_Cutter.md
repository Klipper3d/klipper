# Cortador opcional de filamento e servo na Orange Pi

[English](../Ender3_V3_SE_Cutter.md) | **Português brasileiro**

A referência mecânica é o [cortador de matarsaed para Ender 3 V3 SE/KE](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke),
incluindo sua [discussão de montagem](https://www.printables.com/model/1243521-filament-cutter-for-ender-3-v3-seke/comments).
O suporte do servo no pórtico é adaptado do [suporte Filametrix de jmillerfo](https://www.printables.com/model/1097275-voron-24r2-filametrix-filament-cutter-gantry-servo).
Baixe o CAD nas páginas dos autores; as licenças dos modelos permanecem
separadas da licença do software.

O servo posiciona um batente no pórtico. O carro X pressiona a alavanca do
cortador contra esse batente; o servo não empurra a lâmina diretamente.
Esta integração exige um corte **abaixo das engrenagens da extrusora**, deixando
o filamento cortado do lado do bico para ser expulso na próxima carga e purga.
Um cortador a montante que deixe uma ponta solta sem tração exige outra
sequência de transporte e não é suportado por este perfil.

<a id="wiring-the-sg90-to-orange-pi-zero-3"></a>
## Ligação do SG90 na Orange Pi Zero 3

Use um SG90 posicional, não um modelo de rotação contínua. O suporte original
do pórtico foi projetado para um MG90S; instalar um SG90 não comprova sua
durabilidade sob cargas repetidas de contato.

| Ligação | Destino |
| --- | --- |
| Sinal do servo, normalmente laranja/amarelo | Pino físico proposto **15, PC8** |
| GND do servo, normalmente marrom/preto | GND da fonte regulada e **GND** da Orange Pi, por exemplo pino físico **6** |
| Alimentação do servo, normalmente vermelho | Fonte regulada **5 V** separada, com capacidade para os picos de corrente do servo |

O GPIO transporta o sinal de controle de 3,3 V, não a potência do motor.
Mantenha a alimentação da Orange Pi inalterada, una os GNDs e não injete a fonte
externa no cabeçalho de 5 V da Pi. Nunca ligue o servo em 24 V. A Tower Pro
relata picos substanciais de corrente; uma fonte regulada de pelo menos 2 A é
um ponto de partida conservador para um SG90.

O switch de entrada validado ocupa PC15, pino físico 16. Mantenha a UART
verificada da BMCU inalterada; não habilite overlay de PWM nos pinos RX/TX dela.
O PC8 usa o PWM por software da MCU Linux existente e não exige alteração de
UART ou overlays de boot. Verifique a estabilidade dos pulsos com a câmera e a
carga normal do host; use PWM por hardware ou uma MCU dedicada se houver
jitter. Testes de software não verificam temporização elétrica nem curso real
da lâmina.

Antes de ligar, execute na Orange Pi:

```sh
sudo gpiodetect
sudo gpioinfo
```

Confirme qual gpiochip contém o PC8 e se a linha está livre. No layout observado,
o endereço proposto do PC8 é `host:gpiochip1/gpio72`, mas a numeração de gpiochip
depende do kernel. O número do pino físico não é o offset do GPIO. Confira
[o pinout da placa](http://www.orangepi.org/orangepiwiki/index.php/Orange_Pi_Zero_3#26_Pin_Interface_Pin_Description),
o [mapeamento da biblioteca GPIO](https://github.com/rm-hull/OPi.GPIO/issues/79)
e a [orientação de MCU Linux do Klipper](https://www.klipper3d.org/RPi_microcontroller.html).

O PC9 (pino físico 7) está ligado à interrupção de gerenciamento de energia
AXP313A no [device tree da Zero 3 no Linux](https://github.com/torvalds/linux/blob/v6.18/arch/arm64/boot/dts/allwinner/sun50i-h618-orangepi-zero3.dts).
Não o use como saída do servo nem desative a interrupção do PMIC para liberá-lo.
Uma linha sem consumidor no `gpioinfo` não prova que ela esteja disponível como
saída: também é preciso considerar IRQ e propriedade do pin-mux. O instalador
rejeita o offset 73 do GPIO quando o modelo da placa identifica uma Orange Pi
Zero 3. A recomendação anterior de PC9 estava incorreta.

Se a instalação falhou no PC9, mantenha a configuração restaurada. Desligue o
sistema antes de mover somente o fio de sinal do servo para o PC8; mantenha
alimentação e GND comum conforme documentado. Após inicializar em estado pronto,
execute novamente o instalador com o mapeamento conferido do PC8. Se a MCU
Linux permanecer em shutdown após o rollback, use `FIRMWARE_RESTART` com a
configuração restaurada antes de tentar de novo. Não envie comandos ao servo
até a instalação concluir.

<a id="installation-and-calibration"></a>
## Instalação e calibração

Cancele qualquer impressão, recupere manualmente um atolamento e confirme todas
as rotas vazias ou estacionadas antes. Faça backup da configuração. Em uma
instalação existente, prepare a fonte publicada da v2 separadamente para que o
instalador preserve o checkout anterior inteiro do host/display. **Não atualize
primeiro o checkout ativo com `git pull`.** Conclua antes a
[manutenção de pacote/firmware](releases/v2.0.1.md#upgrade-and-rollback).
Depois que a v2.0.1 for publicada e
após confirmar que o GPIO proposto está livre:

```sh
git clone --branch v2.0.1 --single-branch \
  https://github.com/bernardokcosta/ender3-v3-se-klipper-bmcu.git \
  "$HOME/klipper-v2-source"
sudo python3 "$HOME/klipper-v2-source/scripts/ender3-v3-se/install_multicolor.py" \
  --host-source "$HOME/klipper-v2-source" \
  --cutter-servo-pin host:gpiochip1/gpio72
```

Substitua pelo mapeamento real de GPIO se for diferente. O atualizador preparado
aceita somente mudanças no host: fontes da MCU, bibliotecas, configurações-alvo
e requisitos Python precisam coincidir com o checkout atual. Caso contrário,
recusa a atualização e exige revisar separadamente firmware e ambiente.
Ele nunca grava um `.bin` na impressora. Preserva o checkout anterior, módulos
opcionais instalados, ângulos, posições, larguras de pulso, comprimento de purga,
PID, mesh e Z offset. Uma falha de inicialização restaura host/display e
configurações anteriores, mantendo as evidências atuais de rotas. Um pino de
servo existente diferente precisa de revisão manual em vez de sobrescrita.
Sem `--cutter-servo-pin`, instala o perfil opcional e a macro de reset sem
reservar um pino. O instalador só da integração, sem `--host-source`, não pode
restaurar um checkout alterado antes de sua execução.

O novo `bmcu-cortador.cfg` vem com `variable_enabled: 0`,
`variable_calibrated: 0` e sentinelas de coordenadas inválidas. Nenhum corte ou
perfil mais rápido do cortador inicia por padrão. O `bmcu-cortador-servo.cfg`
não tem ângulo inicial, portanto instalar o arquivo não move o servo
automaticamente.

1. Com o carro fisicamente livre do batente, use
   `CUTTER_SERVO ANGLE=<safe_angle> CONFIRM=1` para encontrar os ângulos
   recolhido e acionado seguros. Comece com a alavanca sem carga se a direção
   for desconhecida. Os limites padrão de pulso são apenas um ponto de partida,
   não uma calibração de clone de SG90.
2. Configure `variable_retracted_angle`, `variable_deployed_angle`,
   `variable_approach_x`, `variable_cut_x` e `variable_cut_y` em
   `bmcu-cortador.cfg`. Meça a aproximação real e o curso completo de corte
   nesta montagem. Coordenadas de purga não são coordenadas do cortador.
   Mantenha todas as coordenadas dentro do curso configurado; não amplie os
   limites da máquina para esconder uma colisão. Configure a folga Z para o
   balde, câmera, ventoinha e hub instalados.
3. Reinicie o Klipper, comande o ângulo recolhido calibrado e faça homing.
   Com hotend/caminho compartilhado vazios e canais estacionados, execute
   `TEST_CUTTER CONFIRM=1`. Isso realiza um ciclo mecânico a seco sem aquecer
   ou extrudar; inspecione o curso completo da alavanca, o retorno da mola e
   a folga. Uma falha após acionar o batente deixa sua posição incerta e bloqueia
   novo uso automático.
4. Somente após o ciclo a seco funcionar, configure `variable_calibrated: 1`
   e `variable_enabled: 1` e reinicie.
5. Teste uma carga/descarga aquecida, inspecione a ponta cortada e o filamento
   restante no hotend e repita um teste curto de quatro cores. Verifique o
   fluxo no bico em cada purga. Não há sensor de posição da lâmina, de sucesso
   de corte ou de captura após as engrenagens nesta montagem; a conclusão do
   comando não prova corte físico bem-sucedido nem confiabilidade autônoma
   em quatro cores.

Na tela original, **Mover → Abrir batente / Fechar batente** move somente
o servo após o operador confirmar a folga. **Mover → Referenciar eixos**
envia somente `G28`, sem recolhimento antecipado nem confirmação na partida
normal. **Filamento / BMCU** concentra carga, retirada e **Reset BMCU** com
proteções; **Temperatura** concentra perfis de pré-aquecimento e resfriamento.
Escolha **Configurações → Idioma → English / Português** para aplicar o idioma
imediatamente; **Configurações → Salvar ajustes** persiste a escolha e reinicia
o Klipper após confirmação. Console e tela usam a mesma sequência abaixo, com
comandos em inglês nos dois idiomas. O comando manual
`CUTTER_SERVO POSITION=RETRACTED CONFIRM=1` permanece disponível para calibração
e recuperação com o carro livre.

Com o cortador habilitado e calibrado, todo `G28` primeiro referencia X pelo
fim de curso real configurado (−6 mm nesta máquina), **sem nenhum comando ao
servo, nem mesmo `WIDTH=0`, antes ou durante o homing de X**. Somente após X
concluir e confirmar uma posição livre ele recolhe o batente, aguarda
`variable_servo_wait_ms` e libera o PWM conforme o ajuste abaixo. Um `G28`
completo então referencia Y (mesa) e Z (vertical). O `safe_z_home` nativo mantém
uma única elevação inicial de proteção, que pode preceder X, seu ponto de
sondagem Z e sua elevação pós-sondagem.

**Todo o caminho até o fim de curso X precisa estar livre com o batente na
posição em que ficou**, inclusive após falta de energia. Não use esta sequência
em um mecanismo que exija mover o servo para liberar o próprio homing de X.
Nenhum movimento X sem referência ou referência artificial é usado.
`G28 X` termina após o recolhimento; `G28 Y` faz X → recolher → Y;
`G28 Z` faz X → recolher → Z e mantém a exigência nativa de Y já referenciado.
A antiga opção `recolher_sem_referencia` está obsoleta e não dispensa o homing
de X. Uma falha de X, servo ou validação impede as etapas seguintes. Um corte
interrompido que deixe o batente incerto na sessão atual ainda exige recuperação
manual. Carga/descarga manual de filamento usa esse mesmo `G28` antes de
assumir sua operação quando homing ou recolhimento for necessário.

Se o cortador estiver habilitado mas não calibrado, sem servo, fora dos limites
de curso ou com posição incerta, as operações param em vez de voltar à formação
de ponta quente. Com o perfil opcional desabilitado, o fluxo anterior de
formação de ponta é preservado.

`variable_release_retracted_servo: 1` é o padrão em `bmcu-cortador.cfg` e quando
a variável está ausente. Após comandar o ângulo recolhido e aguardar a alavanca
assentar, a integração envia `SET_SERVO SERVO=bmcu_cortador WIDTH=0` para que o
servo pare de corrigir continuamente o ângulo e vibrar. **O instalador preserva
perfis existentes:** se seu arquivo ainda contiver
`variable_release_retracted_servo: 0`, altere para `1` em
`[gcode_macro _BMCU_ENDER_CUTTER_SETTINGS]` e reinicie o Klipper para adotar esse
comportamento.

Verifique no ciclo a seco que o batente recolhido permanece fora de todo o
caminho do carro sem torque, inclusive sob vibração. Se seu mecanismo exigir
torque de retenção recolhido, configure explicitamente `0` para manter o PWM
ativo. Quando acionado, o servo mantém torque durante todo o curso da lâmina
e o recuo do carro; libera somente após o recolhimento. Um corte interrompido
não libera automaticamente um batente acionado. Desabilitar o PWM não desliga
a alimentação de 5 V.

Se houver curso livre entre acionar o batente e tocar a alavanca, configure
`variable_contact_x` na primeira posição de contato.
`variable_maximum_stroke_mm` então limita a distância do contato ao corte
completo. Com `None` ou sem ajuste, o limite continua valendo desde o ponto de
aproximação. Avanço e retorno com o batente acionado usam `variable_cut_feedrate`
em todo o trecho.

Configure `variable_keep_cut_y: 1` somente se todo o caminho de corte estiver
livre em todas as posições de mesa usadas nas trocas. O ciclo de corte ignora
`variable_cut_y` nesse modo. A purga continua usando a posição configurada da
calha.

<a id="retracting-before-cutting"></a>
### Recuo antes de cortar

`variable_pre_cut_retract_mm` tem padrão 0 e aceita até 5 mm, mas não pode
exceder `unload_slack_mm`. Um valor excessivo rejeita o perfil antes do movimento
do carro. No ponto de aproximação, a extrusora recua em incrementos monitorados
com a lâmina livre e o alimentador parado, usando o buffer para absorver a
diferença. Um sensor liberado, buffer no limite, sessão alterada ou motor do
alimentador ativo bloqueia a lâmina. O teste a seco não move filamento.

Se o filamento prender na entrada do dissipador, verifique o
`Cutter_Base with guide v1` do autor do modelo. Confira o alinhamento e garanta
que o recuo total não leve material amolecido até a lâmina. Qualquer retração
do fatiador ainda em vigor soma-se ao recuo da integração; não há compensação
automática. O recuo pré-corte usa `retract_feedrate` da integração; distâncias
de purga e posições da lâmina permanecem iguais.

O instalador também impede que o gerenciador nativo inicie a assistência de
retirada por 200 ms antes de `BMCU_RELEASE_ENDER`. Ele passa canal e sessão
explicitamente porque parar os motores pode limpar `now_channel`. A assistência
inicia somente após o curso da lâmina e o recolhimento do batente, no pushback
quando habilitado e depois na retirada. Execute o instalador completo em vez
de copiar apenas os módulos
opcionais: o ajuste do gerenciador nativo pertence à mesma atualização.
Outros callbacks mantêm o comportamento nativo.

### Pushback após o corte

Perfis novos incluem estes ajustes em
`[gcode_macro _BMCU_ENDER_CUTTER_SETTINGS]`:

```ini
variable_pushback_mm: 10.0
variable_pushback_feedrate: 120.0
```

Após concluir os comandos do cortador e posicionar sobre a calha de purga,
toda retirada com cortador avança 10 mm e retorna 10 mm a 120 mm/min (2 mm/s),
depois continua a retirada normal. É um curso inicial proposto para o hotend
original da SE, não uma medida interna nem uma correção de entupimento comprovada.
Não altera recuo pré-corte, temperatura, corrente dos motores nem distâncias
de captura, purga ou estacionamento. O teste mecânico a seco não faz pushback.

**Perfis preservados não mudam silenciosamente:** sem `pushback_mm`, o curso
fica desabilitado. Para adotá-lo, adicione as duas linhas acima ao perfil
existente e reinicie o Klipper. Zero desabilita; a distância é limitada a 10 mm.
A taxa precisa ser positiva e não pode superar 600 mm/min nem o máximo
configurado da extrusora.

O controlador de pressão `MOTION_BEFORE_PULL_BACK` do firmware 1.0.4 acompanha
avanço e retorno no alvo de buffer de retirada. A integração não muda para o
alvo de reposição de impressão nem altera os alvos nativos. Cada incremento
respeita `unload_step_mm`, metade de `unload_slack_mm` e o limite da extrusora
(normalmente 1 mm). O encoder é conferido nas duas direções antes de permitir
um passo que excederia a tolerância existente. A referência é mantida no
avanço, retorno e retirada final; inverter não apaga uma diferença acumulada.

Com pushback ativo, a extrusora também aguarda o buffer entre
`100 - unload_buffer_limit` e `unload_buffer_limit` (30–70% por padrão).
Isso exige um limite maior que 50 e menor que 100. Alimentador que não acompanha,
diferença excessiva, telemetria inválida, perda de sensor/sessão, assistência
parada ou resfriamento interrompem a retirada e param a BMCU. Não há repetição
automática, movimento compensatório nem confirmação de estacionamento.

`unload_timing.pushback` registra curso solicitado, avanço/retorno comandados
concluídos, etapa e resultado; `pushback_sync` guarda buffer e encoder.
Esses sensores a montante não comprovam o movimento do fragmento abaixo das
engrenagens nem medem força no hotend. Ainda é necessário desobstruir um
entupimento existente e realizar uma troca física supervisionada antes de
voltar a impressões repetidas.

<a id="faster-exchanges"></a>
## Trocas mais rápidas

A limpeza do bico é separada do curso da lâmina. Consulte os
[padrões de limpeza e checklist de aceitação](Ender3_V3_SE_Configuracao_Validada.md#nozzle-wiping-defaults)
para o padrão de seis ciclos X=-6/-3 e a preservação dos ajustes existentes.

Com o cortador calibrado e habilitado, a integração substitui os movimentos de
separação/resfriamento pelo ciclo de corte mecânico. Mantém folga do switch,
feedback do encoder, limites de compressão e a retirada de estacionamento
confirmada de 55 mm; não permite entrada de filamento novo em rota ocupada
ou incerta.

O perfil inicial do cortador usa taxas separadas, todas em mm/min:

| Fase | Valor inicial |
| --- | --- |
| Recuo da extrusora após o corte | 600 |
| Avanço ao bico após captura | 240 |
| Purga | 180 |

Distância/velocidade de captura e distâncias de purga/estacionamento permanecem
as já configuradas. Verifique se a BMCU sustenta a taxa de retirada; a velocidade
validada de puxada da BMCU é 10 mm/s. Reduza as taxas se a extrusora patinar ou
o fluxo no bico for insuficiente. Nunca aumente a tolerância de atraso do
encoder para esconder um alimentador que não acompanha.

`variable_parallel_positioning: 1` permite posicionar o carro durante a busca
nativa de chegada **somente após todas as rotas estarem confirmadas vazias e
o sensor de entrada livre**. A BMCU nativa continua responsável por parar no
sensor de entrada. A captura da extrusora aguarda a conclusão dos movimentos
do carro enfileirados. Não há carga em segundo plano de um segundo filamento
no caminho compartilhado, e nenhuma verificação nativa de rota/ponta é ignorada.

Compare `unload_timing` e `last_cut` no status de `bmcu_ender_integration`.
Cortes com falha registram `last_cut.stage`, incluindo falhas de validação e
de recuo inicial, em vez de manter um sucesso anterior. `pre_cut_sync` e
`post_cut_sync` registram recuo comandado, encoder, buffer, PWM, modo de movimento
e tempo decorrido ao redor dos incrementos. O tempo de movimento inclui esperas
de conclusão, não apenas deslocamento do motor. Testes validam sequência e
intertravamentos; temporizações físicas e confiabilidade exigem medições na
impressora.

O posicionamento de purga e a limpeza usam coordenadas físicas da máquina,
como o corte, independentemente de `G92`, offsets de G-code e `M220`. Limites
da calha e do limpador e espaço para a elevação Z completa são verificados antes
do posicionamento; sem folga Z suficiente, o deslocamento XY é bloqueado.
Distâncias, taxas, coordenadas e timeouts de configuração precisam ser finitos:
NaN e infinito são rejeitados. A verificação final de estacionamento exige
deslocamento do encoder, sensor livre, mesma sessão e motores parados; uma
resposta antiga de conclusão sozinha é insuficiente.

O sensor de entrada acionado mantém o recuo da extrusora com assistência da
BMCU, sujeito aos limites de tempo, buffer e progresso do encoder. O motor da
BMCU estar momentaneamente parado no alvo do buffer não comprova que a ponta
saiu das engrenagens. A tentativa introduzida no PR #22, que substituía o recuo
da extrusora aos 30 mm por uma redução do alvo do buffer, foi removida após
falhar no teste físico. O recuo pré-corte de 2 mm continua disponível. Essa
reversão remove a interrupção antecipada, mas não explica por si só os timeouts
anteriores próximos aos 62 mm.

<a id="reset-after-manual-withdrawal"></a>
## Reset após retirada manual

Após cancelar a impressão e deixar hotend/caminho compartilhado fisicamente
livres:

```gcode
RESET_BMCU CONFIRM=1
```

O comando para o movimento da BMCU e usa a confirmação nativa de rota para os
quatro canais: `PARKED` se sua entrada ainda detectar filamento, `EMPTY` caso
contrário. Exige sensor de entrada livre e nenhuma impressão ativa/pausada ou
operação BMCU em andamento. Somente após todas as confirmações terem sucesso
ele limpa a sessão de impressão e a troca pendente. Uma falha parcial é
relatada, não apresentada como reset concluído.

Na tela, **Filamento / BMCU → Reset BMCU** confirma a mesma ação por
`BMCU_ENDER_RESET CONFIRM=1`. Isso reconcilia o estado das rotas; não é reset
de fábrica nem reinício do firmware.

Isso não descarrega fisicamente, não aquece, não extruda, não desentope, não
apaga calibração, não redefine a posição incerta do servo nem substitui a
recuperação de impressão pausada. Para uma falha de troca com impressão
pausada, use o [procedimento de recuperação existente](Ender3_V3_SE_Print_Recovery.md):
a opção contextual **Recuperar filamento → Confirmar caminho livre** executa
`BMCU_CONFIRM_CLEAR_PATH CHANNEL=<1-4> CONFIRM=1`, sem apagar o mapa de impressão.
Um cortador incerto precisa primeiro ser liberado fisicamente; `CUTTER_SERVO`
permite o recolhimento durante essa recuperação pausada pendente, com confirmação
explícita e sem operação BMCU ativa.

Fontes: [referência de servo do Klipper](https://www.klipper3d.org/Config_Reference.html#servo),
[Tower Pro SG90](https://towerpro.com.tw/product/sg90-7/) e as referências
mecânicas/GPIO acima.
