<a id="tmc-drivers"></a>
# Drivers TMC

[English](../TMC_Drivers.md) | **Português brasileiro**

Este documento fornece informações sobre como usar drivers de motor de passo Trinamic no modo SPI/UART no Klipper.

O Klipper também pode usar drivers Trinamic em seu "modo standalone". No entanto, quando os drivers estão neste modo, nenhuma configuração especial do Klipper é necessária e os recursos avançados do Klipper discutidos neste documento não estão disponíveis.

Além deste documento, certifique-se de revisar a [referência de configuração do driver TMC](Config_Reference.md#tmc-stepper-driver-configuration).

<a id="tuning-motor-current"></a>
## Ajustando a corrente do motor

Uma corrente maior do driver aumenta a precisão posicional e o torque. No entanto, uma corrente maior também aumenta o calor produzido pelo motor de passo e pelo driver do motor de passo. Se o driver do motor de passo ficar quente demais, ele se desabilitará e o Klipper relatará um erro. Se o motor de passo ficar quente demais, ele perde torque e precisão posicional. (Se ficar muito quente, também pode derreter peças plásticas acopladas a ele ou próximas a ele.)

Como dica geral de ajuste, prefira valores de corrente maiores desde que o motor de passo não fique quente demais e o driver do motor de passo não relate avisos ou erros. Em geral, é aceitável que o motor de passo esteja morno ao toque, mas ele não deve ficar tão quente a ponto de ser doloroso ao tocar.

<a id="prefer-to-not-specify-a-hold_current"></a>
## Prefira não especificar hold_current

Se você configurar um `hold_current`, o driver TMC pode reduzir a corrente para o motor de passo quando detectar que o motor não está se movendo. No entanto, alterar a corrente do motor pode por si só introduzir movimento do motor. Isso pode ocorrer devido a "forças de detente" dentro do motor de passo (o ímã permanente no rotor puxa em direção aos dentes de ferro no estator) ou devido a forças externas no carro do eixo.

A maioria dos motores de passo não obterá um benefício significativo da redução de corrente durante impressões normais, porque poucos movimentos de impressão deixarão um motor de passo parado por tempo suficiente para ativar o recurso `hold_current`. E é improvável que se queira introduzir artefatos sutis de impressão nos poucos movimentos de impressão que deixam um motor parado por tempo suficiente.

Se você deseja reduzir a corrente para os motores durante rotinas de início de impressão, considere emitir comandos [SET_TMC_CURRENT](G-Codes.md#set_tmc_current) em uma [macro START_PRINT](Slicers.md#klipper-gcode_macro) para ajustar a corrente antes e depois dos movimentos normais de impressão.

Algumas impressoras com motores Z dedicados que ficam parados durante movimentos normais de impressão (sem bed_mesh, sem bed_tilt, sem skew_correction de Z, sem impressões em modo "vase", etc.) podem achar que os motores Z operam mais frios com um `hold_current`. Se implementar isso, certifique-se de levar em conta esse tipo de movimento não comandado do eixo Z durante nivelamento da mesa, sondagem da mesa, calibração da sonda e similares. O `driver_TPOWERDOWN` e o `driver_IHOLDDELAY` também devem ser calibrados adequadamente. Em caso de dúvida, prefira não especificar um `hold_current`.

<a id="setting-spreadcycle-vs-stealthchop-mode"></a>
## Definindo o modo "spreadCycle" vs "stealthChop"

Por padrão, o Klipper coloca os drivers TMC no modo "spreadCycle". Se o driver suportar "stealthChop", ele pode ser habilitado adicionando `stealthchop_threshold: 999999` à seção de configuração TMC.

Em geral, o modo spreadCycle fornece maior torque e maior precisão posicional do que o modo stealthChop. No entanto, o modo stealthChop pode produzir ruído audível significativamente menor em algumas impressoras.

Testes comparando modos mostraram um "atraso posicional" aumentado de cerca de 75% de um passo completo durante movimentos em velocidade constante ao usar o modo stealthChop (por exemplo, em uma impressora com rotation_distance de 40mm e 200 steps_per_rotation, o desvio de posição de movimentos em velocidade constante aumentou em ~0.150mm). No entanto, este "atraso em obter a posição solicitada" pode não se manifestar como um defeito significativo de impressão e pode-se preferir o comportamento mais silencioso do modo stealthChop.

Recomenda-se sempre usar o modo "spreadCycle" (não especificando `stealthchop_threshold`) ou sempre usar o modo "stealthChop" (definindo `stealthchop_threshold` como 999999). Infelizmente, os drivers geralmente produzem resultados ruins e confusos se o modo mudar enquanto o motor estiver em velocidade diferente de zero.

Observe que a opção de configuração `stealthchop_threshold` não afeta o homing sem sensor, pois o Klipper alterna automaticamente o driver TMC para um modo apropriado durante operações de homing sem sensor.

<a id="tmc-interpolate-setting-introduces-small-position-deviation"></a>
## A configuração interpolate do TMC introduz pequeno desvio de posição

A configuração `interpolate` do driver TMC pode reduzir o ruído audível do movimento da impressora ao custo de introduzir um pequeno erro posicional sistêmico. Esse erro posicional sistêmico resulta do atraso do driver na execução dos "passos" que o Klipper envia a ele. Durante movimentos em velocidade constante, esse atraso resulta em um erro posicional de quase meio micropasso configurado (mais precisamente, o erro é metade da distância de um micropasso menos um quinhentos e doze avos da distância de um passo completo). Por exemplo, em um eixo com rotation_distance de 40mm, 200 steps_per_rotation e 16 micropassos, o erro sistêmico introduzido durante movimentos em velocidade constante é de ~0.006mm.

Para melhor precisão posicional, considere usar o modo spreadCycle e desabilitar a interpolação (defina `interpolate: False` na configuração do driver TMC). Quando configurado dessa forma, pode-se aumentar a configuração `microstep` para reduzir o ruído audível durante o movimento do motor de passo. Normalmente, uma configuração de micropasso de `64` ou `128` terá ruído audível semelhante ao da interpolação, e sem introduzir um erro posicional sistemático.

Se estiver usando o modo stealthChop, a imprecisão posicional da interpolação é pequena em relação à imprecisão posicional introduzida pelo modo stealthChop. Portanto, ajustar a interpolação não é considerado útil no modo stealthChop, e pode-se deixar a interpolação em seu estado padrão.

<a id="sensorless-homing"></a>
## Homing sem sensor

O homing sem sensor permite fazer o homing de um eixo sem a necessidade de uma chave fim de curso física. Em vez disso, o carro no eixo é movido até o limite mecânico, fazendo o motor de passo perder passos. O driver do motor detecta os passos perdidos e indica isso ao MCU controlador (Klipper) alternando um pino. Essa informação pode ser usada pelo Klipper como fim de curso para o eixo.

Este guia cobre a configuração do homing sem sensor para o eixo X da sua impressora (cartesiana). No entanto, funciona da mesma forma com todos os outros eixos (que exigem um fim de curso). Você deve configurá-lo e ajustá-lo para um eixo de cada vez.

<a id="limitations"></a>
### Limitações

Certifique-se de que seus componentes mecânicos sejam capazes de suportar a carga do carro colidindo com o limite do eixo repetidamente. Especialmente fusos de avanço podem gerar muita força. Fazer o homing de um eixo Z encostando o bico na superfície de impressão pode não ser uma boa ideia. Para melhores resultados, verifique se o carro do eixo fará um contato firme com o limite do eixo.

Além disso, o homing sem sensor pode não ser preciso o suficiente para sua impressora. Embora o homing dos eixos X e Y em uma máquina cartesiana possa funcionar bem, o homing do eixo Z geralmente não é preciso o suficiente e pode resultar em uma primeira camada inconsistente. Fazer o homing de uma impressora delta sem sensor não é aconselhável devido à falta de precisão.

Além disso, a detecção de travamento do driver do motor de passo depende da carga mecânica no motor, da corrente do motor e da temperatura do motor (resistência da bobina).

O homing sem sensor funciona melhor em velocidades médias do motor. Para velocidades muito lentas (menos de 10 RPM), o motor não gera força contraeletromotriz significativa e o TMC não consegue detectar de forma confiável travamentos do motor. Além disso, em velocidades muito altas, a força contraeletromotriz do motor se aproxima da tensão de alimentação do motor, de modo que o TMC não consegue mais detectar travamentos. Recomenda-se consultar o datasheet dos seus TMCs específicos. Lá você também pode encontrar mais detalhes sobre limitações dessa configuração.

<a id="prerequisites"></a>
### Pré-requisitos

Alguns pré-requisitos são necessários para usar o homing sem sensor:

1. Um driver de motor de passo TMC com capacidade stallGuard (tmc2130, tmc2209, tmc2660 ou tmc5160).
1. Interface SPI / UART do driver TMC ligada ao microcontrolador (modo autônomo não funciona).
1. O pino "DIAG" ou "SG_TST" apropriado do driver TMC conectado ao microcontrolador.
1. As etapas no documento [verificações de configuração](Config_checks.md) devem ser executadas para confirmar que os motores de passo estão configurados e funcionando corretamente.

<a id="tuning"></a>
### Ajuste

O procedimento descrito aqui tem seis etapas principais:

1. Escolha uma velocidade de referenciamento.
1. Configure o arquivo `printer.cfg` para ativar o referenciamento sem sensor.
1. Localize a configuração de stallguard com maior sensibilidade que realiza o referenciamento com sucesso.
1. Localize a configuração de stallguard com menor sensibilidade que realiza o referenciamento com sucesso com um único toque.
1. Atualize o `printer.cfg` com a configuração de stallguard desejada.
1. Crie ou atualize macros de `printer.cfg` para referenciar de forma consistente.

<a id="choose-homing-speed"></a>
#### Escolha a velocidade de homing

A velocidade de referenciamento (homing) é uma escolha importante ao realizar referenciamento sem sensor (sensorless homing). É desejável usar uma velocidade lenta de referenciamento para que o carro não exerça força excessiva na estrutura ao encostar no fim do trilho. No entanto, os drivers TMC não conseguem detectar de forma confiável uma parada (stall) em velocidades muito baixas.

Um bom ponto de partida para a velocidade de referenciamento é fazer o motor de passo dar uma volta completa a cada dois segundos. Para muitos eixos, isso será o `rotation_distance` dividido por dois. Por exemplo:

```
[stepper_x]
rotation_distance: 40
homing_speed: 20
...
```

<a id="configure-printercfg-for-sensorless-homing"></a>
#### Configure o printer.cfg para referenciamento sem sensor

O ajuste `homing_retract_dist` deve ser definido como zero na seção de configuração `stepper_x` para desativar o segundo movimento de referenciamento. A segunda tentativa de referenciamento não agrega valor ao usar referenciamento sem sensor, não funcionará de forma confiável e confundirá o processo de ajuste.

Certifique-se de que nenhum ajuste `hold_current` esteja especificado na seção do driver TMC da configuração. (Se um hold_current estiver definido, após o contato o motor para com o carro pressionado contra o fim do trilho, e reduzir a corrente nessa posição pode fazer o carro se mover - isso resulta em desempenho ruim e confundirá o processo de ajuste.)

É necessário configurar os pinos de referenciamento sem sensor e configurar os ajustes iniciais de "stallguard". Uma configuração de exemplo tmc2209 para um eixo X pode ser assim:

```
[tmc2209 stepper_x]
diag_pin: ^PA1      # Defina como o pino da MCU conectado ao pino DIAG do TMC
driver_SGTHRS: 255  # 255 é o valor mais sensível, 0 é o menos sensível
...

[stepper_x]
endstop_pin: tmc2209_stepper_x:virtual_endstop
homing_retract_dist: 0
...
```

Um exemplo de configuração tmc2130 ou tmc5160 pode ser assim:

```
[tmc2130 stepper_x]
diag1_pin: ^!PA1 # Pino conectado ao pino DIAG1 do TMC (ou use diag0_pin / pino DIAG0)
driver_SGT: -64  # -64 é o valor mais sensível, 63 é o menos sensível
...

[stepper_x]
endstop_pin: tmc2130_stepper_x:virtual_endstop
homing_retract_dist: 0
...
```

Um exemplo de configuração tmc2660 pode ser assim:

```
[tmc2660 stepper_x]
driver_SGT: -64     # -64 é o valor mais sensível, 63 é o menos sensível
...

[stepper_x]
endstop_pin: ^PA1   # Pino conectado ao pino SG_TST do TMC
homing_retract_dist: 0
...
```

Os exemplos acima mostram apenas ajustes específicos do referenciamento sem sensor. Consulte a [referência de configuração](Config_Reference.md#tmc-stepper-driver-configuration) para todas as opções disponíveis.

<a id="find-highest-sensitivity-that-successfully-homes"></a>
#### Encontre a maior sensibilidade que referencia com sucesso

Posicione o carro próximo ao centro do trilho. Use o comando SET_TMC_FIELD para definir a maior sensibilidade. Para tmc2209:

```
SET_TMC_FIELD STEPPER=stepper_x FIELD=SGTHRS VALUE=255
```
Para tmc2130, tmc5160 e tmc2660:

```
SET_TMC_FIELD STEPPER=stepper_x FIELD=sgt VALUE=-64
```

Em seguida, emita um comando `G28 X0` e verifique se o eixo não se move ou para de se mover rapidamente. Se o eixo não parar, emita um `M112` para interromper a impressora - algo não está correto na fiação ou configuração do pino diag/sg_tst e deve ser corrigido antes de continuar.

Em seguida, diminua continuamente a sensibilidade do ajuste `VALUE` e execute novamente os comandos `SET_TMC_FIELD` `G28 X0` para encontrar a maior sensibilidade que resulte no carro se movendo com sucesso até o fim de curso e parando. (Para drivers tmc2209, isso será diminuir SGTHRS; para outros drivers, será aumentar sgt.) Certifique-se de iniciar cada tentativa com o carro próximo ao centro do trilho (se necessário, emita `M84` e mova manualmente o carro para o centro). Deve ser possível encontrar a maior sensibilidade que referencia de forma confiável (ajustes com sensibilidade maior resultam em movimento pequeno ou nenhum movimento). Anote o valor encontrado como *maximum_sensitivity*. (Se a sensibilidade mínima possível (SGTHRS=0 ou sgt=63) for obtida sem nenhum movimento do carro, algo não está correto na fiação ou configuração do pino diag/sg_tst e deve ser corrigido antes de continuar.)

Ao procurar maximum_sensitivity, pode ser conveniente saltar para diferentes ajustes de VALUE (de modo a dividir o parâmetro VALUE por bisseção). Se fizer isso, esteja preparado para emitir um comando `M112` para interromper a impressora, pois um ajuste com sensibilidade muito baixa pode fazer o eixo "bater" repetidamente no fim do trilho.

Certifique-se de aguardar alguns segundos entre cada tentativa de referenciamento. Depois que o driver TMC detecta uma parada (stall), pode levar um pouco de tempo para limpar seu indicador interno e ser capaz de detectar outra parada.

Durante esses testes de ajuste, se um comando `G28 X0` não se mover até o limite do eixo, tome cuidado ao emitir quaisquer comandos normais de movimento (por ex., `G1`). O Klipper não terá um entendimento correto da posição do carro e um comando de movimento pode causar resultados indesejáveis e confusos.

<a id="find-lowest-sensitivity-that-homes-with-one-touch"></a>
#### Encontre a menor sensibilidade que referencia com um único toque

Ao referenciar com o valor de *maximum_sensitivity* encontrado, o eixo deve se mover até o fim do trilho e parar com um "único toque" - ou seja, não deve haver som de "clique" ou "batida". (Se houver som de batida ou clique em maximum_sensitivity, a homing_speed pode estar muito baixa, a corrente do driver pode estar muito baixa ou o referenciamento sem sensor pode não ser uma boa escolha para o eixo.)

O próximo passo é novamente mover continuamente o carro para uma posição próxima ao centro do trilho, diminuir a sensibilidade e executar os comandos `SET_TMC_FIELD` `G28 X0` - o objetivo agora é encontrar a menor sensibilidade que ainda resulte no carro referenciando com sucesso com um "único toque". Ou seja, ele não "bate" ou "clica" ao encostar no fim do trilho. Anote o valor encontrado como *minimum_sensitivity*.

<a id="update-printercfg-with-sensitivity-value"></a>
#### Atualize o printer.cfg com o valor de sensibilidade

Após encontrar *maximum_sensitivity* e *minimum_sensitivity*, use uma calculadora para obter a sensibilidade recomendada como *minimum_sensitivity + (maximum_sensitivity - minimum_sensitivity)/3*. A sensibilidade recomendada deve estar na faixa entre o mínimo e o máximo, mas ligeiramente mais próxima do mínimo. Arredonde o valor final para o inteiro mais próximo.

Para tmc2209, defina isso na configuração como `driver_SGTHRS`; para outros drivers TMC, defina na configuração como `driver_SGT`.

Se a faixa entre *maximum_sensitivity* e *minimum_sensitivity* for pequena (por ex., menor que 5), isso pode resultar em referenciamento instável. Uma velocidade de referenciamento maior pode aumentar a faixa e tornar a operação mais estável.

Observe que, se qualquer alteração for feita na corrente do driver, na velocidade de referenciamento ou uma alteração notável for feita no hardware da impressora, será necessário executar o processo de ajuste novamente.

<a id="using-macros-when-homing"></a>
#### Usando macros ao referenciar

Após a conclusão do referenciamento sem sensor, o carro ficará pressionado contra o fim do trilho e o motor de passo exercerá uma força sobre a estrutura até que o carro seja afastado. É uma boa ideia criar uma macro para referenciar o eixo e afastar imediatamente o carro do fim do trilho.

É uma boa ideia que a macro pause pelo menos 2 segundos antes de iniciar o referenciamento sem sensor (ou garanta de outra forma que não houve movimento no motor de passo por 2 segundos). Sem um atraso, é possível que o sinalizador interno de parada (stall) do driver ainda esteja definido a partir de um movimento anterior.

Também pode ser útil que essa macro defina a corrente do driver antes do referenciamento e defina uma nova corrente após o carro ter se afastado.

Uma macro de exemplo pode ser algo como:

```
[gcode_macro SENSORLESS_HOME_X]
gcode:
    {% set HOME_CUR = 0.700 %}
    {% set driver_config = printer.configfile.settings['tmc2209 stepper_x'] %}
    {% set RUN_CUR = driver_config.run_current %}
    # Define corrente para homing (referenciamento) sensorless
    SET_TMC_CURRENT STEPPER=stepper_x CURRENT={HOME_CUR}
    # Pausa para garantir que o flag de stall do driver está limpo
    G4 P2000
    # Home
    G28 X0
    # Afasta
    G90
    G1 X5 F1200
    # Define corrente durante impressão
    SET_TMC_CURRENT STEPPER=stepper_x CURRENT={RUN_CUR}
```

A macro resultante pode ser chamada a partir de uma [seção de configuração homing_override](Config_Reference.md#homing_override) ou de uma [macro START_PRINT](Slicers.md#klipper-gcode_macro).

Observe que, se a corrente do driver durante o referenciamento for alterada, o processo de ajuste deverá ser executado novamente.

<a id="tips-for-sensorless-homing-on-corexy"></a>
### Dicas para referenciamento sem sensor em CoreXY

É possível usar referenciamento sem sensor nos carros X e Y de uma impressora CoreXY. O Klipper usa o motor de passo `[stepper_x]` para detectar paradas ao referenciar o carro X e usa o motor de passo `[stepper_y]` para detectar paradas ao referenciar o carro Y.

Use o guia de ajuste descrito acima para localizar a "sensibilidade de stall" apropriada para cada carro, mas esteja ciente das seguintes restrições:

1. Ao usar referenciamento sem sensor em CoreXY, certifique-se de que não há `hold_current` configurado para nenhum dos motores de passo.
1. Durante o ajuste, certifique-se de que ambos os carros X e Y estejam próximos ao centro de seus trilhos antes de cada tentativa de referenciamento.
1. Após a conclusão do ajuste, ao referenciar X e Y, use macros para garantir que um eixo seja referenciado primeiro, depois mova esse carro para longe do limite do eixo, pause por pelo menos 2 segundos e então inicie o referenciamento do outro carro. O afastamento do eixo evita referenciar um eixo enquanto o outro está pressionado contra o limite do eixo (o que pode distorcer a detecção de stall). A pausa é necessária para garantir que o sinalizador de stall do driver seja limpo antes de referenciar novamente.

Uma macro de exemplo de referenciamento CoreXY pode ser algo como:

```
[gcode_macro HOME]
gcode:
    G90
    # Faz homing (referenciamento) de Z
    G28 Z0
    G1 Z10 F1200
    # Faz homing (referenciamento) de Y
    G28 Y0
    G1 Y5 F1200
    # Faz homing (referenciamento) de X
    G4 P2000
    G28 X0
    G1 X5 F1200
```

<a id="querying-and-diagnosing-driver-settings"></a>
## Consultando e diagnosticando ajustes do driver

O [comando DUMP_TMC](G-Codes.md#dump_tmc) é uma ferramenta útil ao configurar e diagnosticar os drivers. Ele relatará todos os campos configurados pelo Klipper, bem como todos os campos que podem ser consultados a partir do driver.

Todos os campos relatados são definidos na folha de dados (datasheet) da Trinamic para cada driver. Essas folhas de dados podem ser encontradas no [site da Trinamic](https://www.trinamic.com/). Obtenha e revise a folha de dados da Trinamic para o driver para interpretar os resultados do DUMP_TMC.

<a id="configuring-driver_xxx-settings"></a>
## Configurando ajustes driver_XXX

O Klipper suporta configurar muitos campos de baixo nível do driver usando ajustes `driver_XXX`. A [referência de configuração do driver TMC](Config_Reference.md#tmc-stepper-driver-configuration) tem a lista completa de campos disponíveis para cada tipo de driver.

Além disso, quase todos os campos podem ser modificados em tempo de execução usando o [comando SET_TMC_FIELD](G-Codes.md#set_tmc_field).

Cada um desses campos é definido na folha de dados da Trinamic para cada driver. Essas folhas de dados podem ser encontradas no [site da Trinamic](https://www.trinamic.com/).

Observe que as folhas de dados da Trinamic às vezes usam redação que pode confundir um ajuste de alto nível (como "hysteresis end") com um valor de campo de baixo nível (por ex., "HEND"). No Klipper, `driver_XXX` e SET_TMC_FIELD sempre definem o valor de campo de baixo nível que é efetivamente gravado no driver. Assim, por exemplo, se a folha de dados da Trinamic afirma que um valor de 3 deve ser gravado no campo HEND para obter um "hysteresis end" de 0, defina `driver_HEND=3` para obter o valor de alto nível de 0.

<a id="common-questions"></a>
## Perguntas comuns

<a id="can-i-use-stealthchop-mode-on-an-extruder-with-pressure-advance"></a>
### Posso usar o modo stealthChop em uma extrusora com pressure advance?

Muitas pessoas usam com sucesso o modo "stealthChop" com o pressure advance do Klipper. O Klipper implementa [pressure advance suave](Kinematics.md#pressure-advance), que não introduz nenhuma mudança instantânea de velocidade.

No entanto, o modo "stealthChop" pode produzir menor torque do motor e/ou produzir maior aquecimento do motor. Ele pode ou não ser um modo adequado para sua impressora específica.

<a id="i-keep-getting-unable-to-read-tmc-uart-stepper_x-register-ifcnt-errors"></a>
### Continuo recebendo erros "Unable to read tmc uart 'stepper_x' register IFCNT"?

Isso ocorre quando o Klipper não consegue se comunicar com um driver tmc2208 ou tmc2209.

Certifique-se de que a alimentação do motor esteja habilitada, pois o driver do motor de passo geralmente precisa da alimentação do motor antes de poder se comunicar com o microcontrolador.

Se esse erro ocorrer após atualizar (flash) o Klipper pela primeira vez, o driver de passo pode ter sido programado anteriormente em um estado que não é compatível com o Klipper. Para redefinir o estado, remova toda a alimentação da impressora por vários segundos (desconecte fisicamente tanto o USB quanto os plugues de alimentação).

Caso contrário, esse erro normalmente é resultado de fiação incorreta dos pinos UART ou de configuração incorreta no Klipper dos ajustes dos pinos UART.

<a id="i-keep-getting-unable-to-write-tmc-spi-stepper_x-register--errors"></a>
### Continuo recebendo erros "Unable to write tmc spi 'stepper_x' register ..."?

Isso ocorre quando o Klipper não consegue se comunicar com um driver tmc2130 ou tmc5160.

Certifique-se de que a alimentação do motor esteja habilitada, pois o driver do motor de passo geralmente precisa da alimentação do motor antes de poder se comunicar com o microcontrolador.

Caso contrário, esse erro normalmente é resultado de fiação SPI incorreta, configuração incorreta no Klipper dos ajustes SPI ou configuração incompleta de dispositivos no barramento SPI.

Observe que, se o driver estiver em um barramento SPI compartilhado com vários dispositivos, certifique-se de configurar completamente cada dispositivo nesse barramento SPI compartilhado no Klipper. Se um dispositivo em um barramento SPI compartilhado não estiver configurado, ele poderá responder incorretamente a comandos não destinados a ele e corromper a comunicação com o dispositivo pretendido. Se houver um dispositivo em um barramento SPI compartilhado que não possa ser configurado no Klipper, use uma [seção de configuração static_digital_output](Config_Reference.md#static_digital_output) para definir o pino CS do dispositivo não utilizado como alto (para que ele não tente usar o barramento SPI). O esquemático da placa costuma ser uma referência útil para descobrir quais dispositivos estão em um barramento SPI e seus pinos associados.

<a id="why-did-i-get-a-tmc-reports-error--error"></a>
### Por que recebi um erro "TMC reports error: ..."?

Esse tipo de erro indica que o driver TMC detectou um problema e se desativou. Ou seja, o driver parou de manter sua posição e ignorou comandos de movimento. Se o Klipper detectar que um driver ativo se desativou, ele colocará a impressora em estado de "shutdown".

Também é possível que um desligamento (shutdown) **TMC reports error** ocorra devido a erros de SPI que impedem a comunicação com o driver (em tmc2130, tmc5160 ou tmc2660). Se isso ocorrer, é comum que o status relatado do driver mostre `00000000` ou `ffffffff` - por exemplo: `TMC reports error: DRV_STATUS: ffffffff ...` OU `TMC reports error: READRSP@RDSEL2: 00000000 ...`. Tal falha pode ser devida a um problema de fiação SPI ou pode ser devida a uma reinicialização automática ou falha do driver TMC.

Alguns erros comuns e dicas para diagnosticá-los:

<a id="tmc-reports-error--ot1overtemperror"></a>
#### TMC reports error: `... ot=1(OvertempError!)`

Isso indica que o driver do motor se desativou porque ficou quente demais. Soluções típicas são diminuir a corrente do motor de passo, aumentar o resfriamento no driver do motor de passo e/ou aumentar o resfriamento no motor de passo.

<a id="tmc-reports-error--shorttognd-or-shorttosupply"></a>
#### TMC reports error: `... ShortToGND` OR `ShortToSupply`

Isso indica que o driver se desativou porque detectou corrente muito alta passando pelo driver. Isso pode indicar um fio solto ou em curto para o motor de passo ou dentro do próprio motor de passo.

Esse erro também pode ocorrer se estiver usando o modo stealthChop e o driver TMC não for capaz de prever com precisão a carga mecânica do motor. (Se o driver fizer uma previsão ruim, ele poderá enviar corrente demais pelo motor e acionar sua própria detecção de sobrecorrente.) Para testar isso, desative o modo stealthChop e verifique se os erros continuam ocorrendo.

<a id="tmc-reports-error--reset1reset-or-cs_actual0reset-or-se0reset"></a>
#### TMC reports error: `... reset=1(Reset)` OR `CS_ACTUAL=0(Reset?)` OR `SE=0(Reset?)`

Isso indica que o driver se reinicializou no meio da impressão. Isso pode ser devido a problemas de tensão ou fiação.

<a id="tmc-reports-error--uv_cp1undervoltage"></a>
#### TMC reports error: `... uv_cp=1(Undervoltage!)`

Isso indica que o driver detectou um evento de baixa tensão e se desativou. Isso pode ser devido a problemas de fiação ou da fonte de alimentação.

<a id="how-do-i-tune-spreadcyclecoolstepetc-mode-on-my-drivers"></a>
### Como ajusto o modo spreadCycle/coolStep/etc. nos meus drivers?

O [site da Trinamic](https://www.trinamic.com/) tem guias sobre como configurar os drivers. Esses guias costumam ser técnicos, de baixo nível e podem exigir hardware especializado. Ainda assim, são a melhor fonte de informação.
