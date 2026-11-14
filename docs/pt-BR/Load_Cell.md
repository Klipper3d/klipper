<a id="load-cells"></a>
# Células de carga

[English](../Load_Cell.md) | **Português brasileiro**

Este documento descreve o suporte do Klipper a células de carga. A funcionalidade básica de célula de carga pode ser usada para ler dados de força e para pesar itens como filamento. Um sensor de força calibrado é uma parte importante de uma sonda baseada em célula de carga.

<a id="related-documentation"></a>
## Documentação relacionada

* [Referência de configuração de load_cell](Config_Reference.md#load_cell)
* [Comandos G-Code de load_cell](G-Codes.md#load_cell)
* [Referência de estado de load_cell](Status_Reference.md#load_cell)

<a id="using-load_cell_diagnostic"></a>
## Usando `LOAD_CELL_DIAGNOSTIC`

Ao conectar uma célula de carga pela primeira vez, é uma boa prática verificar problemas executando `LOAD_CELL_DIAGNOSTIC`. Essa ferramenta coleta 10 segundos de dados da célula de carga e informa estatísticas:

```
$ LOAD_CELL_DIAGNOSTIC
// Collecting load cell data for 10 seconds...
// Samples Collected: 3211
// Measured samples per second: 332.0
// Good samples: 3211, Saturated samples: 0, Unique values: 900
// Sample range: [4.01% to 4.02%]
// Sample range / sensor capacity: 0.00524%
```

Itens que você pode verificar com esses dados:

* A taxa de amostragem configurada do sensor deve estar próxima do valor de 'Measured samples per second'. Se não estiver, você pode ter um problema de configuração ou de fiação.
* 'Saturated samples' deve ser 0. Se houver amostras saturadas, significa que a célula de carga está sofrendo mais força do que consegue medir.
* 'Unique values' deve ser uma grande porcentagem do valor de 'Samples Collected'. Se 'Unique values' for 1, é muito provável que seja um problema de fiação.
* Toque ou empurre o sensor enquanto `LOAD_CELL_DIAGNOSTIC` é executado. Se tudo estiver funcionando corretamente, isso deve aumentar o 'Sample range'.

<a id="calibrating-a-load-cell"></a>
## Calibrando uma célula de carga

As células de carga são calibradas usando o comando `LOAD_CELL_CALIBRATE`. Este é um utilitário interativo de calibração que orienta você em um processo de 3 etapas:

1. Primeiro use o comando `TARE` para estabelecer o valor de força zero. Este é o valor de configuração `reference_tare_counts`.
1. Em seguida, aplique uma carga ou força conhecida à célula de carga e execute o comando `CALIBRATE GRAMS=nnn`. A partir disso, o valor `counts_per_gram` é calculado. Veja [a próxima seção](#applying-a-known-force-or-load) para algumas sugestões sobre como fazer isso.
1. Por fim, use o comando `ACCEPT` para salvar os resultados.

Você pode cancelar o processo de calibração a qualquer momento com `ABORT`.

<a id="applying-a-known-force-or-load"></a>
### Aplicando uma força ou carga conhecida

A etapa `CALIBRATE GRAMS=nnn` pode ser realizada de várias maneiras. Se sua célula de carga estiver sob uma plataforma como uma mesa ou um suporte de filamento, pode ser mais fácil colocar uma massa conhecida sobre a plataforma. Por exemplo, você poderia usar alguns rolos de filamento de 1KG.

Se sua célula de carga estiver no cabeçote da impressora, uma abordagem diferente é mais fácil. Coloque uma balança digital sobre a mesa da impressora e abaixe suavemente o cabeçote sobre a balança (ou eleve a mesa até o cabeçote, se sua mesa se mover). Você pode conseguir fazer isso usando o comando `FORCE_MOVE`. Mas é mais provável que você tenha que mover manualmente o eixo z com os motores desligados até o cabeçote pressionar a balança.

Uma boa força de calibração seria idealmente uma grande porcentagem da capacidade nominal da célula de carga. Por exemplo, se você tem uma célula de carga de 5Kg, o ideal seria calibrá-la com uma massa de 5kg. Isso pode funcionar bem com sensores sob a mesa, que precisam suportar muito peso. Para sondas no cabeçote, essa pode não ser uma carga que a mesa ou o cabeçote da sua impressora tolerem sem danos. Tente usar pelo menos 1Kg de força; a maioria das impressoras deve tolerar isso sem problemas.

Ao calibrar, anote cuidadosamente os valores informados:

```
$ CALIBRATE GRAMS=555
// Calibration value: -2.78% (-59803108), Counts/gram: 73039.78739,
Total capacity: +/- 29.14Kg
```

A `Total capacity` deve estar próxima da capacidade teórica da célula de carga com base na capacidade da sonda. Se for muito maior, você poderia ter usado uma configuração de ganho maior na sonda ou uma célula de carga mais sensível. Isso não é tão crítico para sondas de 32 bits e 24 bits, mas é muito mais crítico para sondas de baixa largura de bits.

<a id="reading-force-data"></a>
## Leitura de dados de força

Dados de força podem ser lidos com um comando GCode:

```
LOAD_CELL_READ
// 10.6g (1.94%)
```

Os dados também são lidos continuamente e podem ser consumidos a partir do objeto de impressora load_cell em uma macro:

```
{% set grams = printer.load_cell.force_g %}
```

Isso fornece uma força média ao longo do último 1 segundo, de forma semelhante ao funcionamento dos sensores de temperatura.

<a id="taring-a-load-cell"></a>
## Tara de uma célula de carga

A tara, às vezes chamada de zeragem, define o peso atual informado pela load_cell como 0. Isso é útil para medir em relação a um peso conhecido. p. ex. ao medir um rolo de filamento, usar `LOAD_CELL_TARE` define o peso como 0. Então, à medida que o filamento é impresso, a load_cell informará o peso do filamento usado.

```
LOAD_CELL_TARE
// Load cell tare value: 5.32% (445903)
```

O valor atual de tara é informado no estado da impressora e pode ser lido em uma macro:

```
{% set tare_counts = printer.load_cell.tare_counts %}
```

<a id="load-cell-probes"></a>
# Sondas com célula de carga

<a id="related-documentation-1"></a>
## Documentação relacionada

* [Referência de configuração de load_cell_probe](Config_Reference.md#load_cell_probe)
* [Comandos G-Code de load_cell_probe](G-Codes.md#load_cell_probe)
* [Referência de estado de load_cell_probe](Status_Reference.md#load_cell_probe)

<a id="load-cell-probe-safety"></a>
## Segurança da sonda com célula de carga

Como as células de carga são uma sonda de contato direto do bico, há risco de danos à sua impressora se for usada força excessiva. O sistema de sondagem com célula de carga inclui várias verificações de segurança que tentam manter sua máquina protegida contra força excessiva no cabeçote. É importante entender o que elas são e como funcionam, pois você pode anular a maioria delas com valores de configuração mal escolhidos.

<a id="calibration-check"></a>
#### Verificação de calibração

Toda vez que um movimento de origem inicia, a load_cell_probe verifica se a load_cell está calibrada. Caso contrário, interromperá o movimento com um erro: `!! Load Cell not calibrated`.

<a id="counts_per_gram"></a>
#### `counts_per_gram`

Esta configuração é usada para converter contagens brutas da sonda em gramas. Todos os limites de segurança estão em unidades de gramas para sua conveniência. Se a configuração `counts_per_gram` não estiver precisa, você pode facilmente exceder a força segura no cabeçote. Você nunca deve adivinhar este valor. Use `LOAD_CELL_CALIBRATE` para descobrir o `counts_per_gram` real da sua célula de carga.

<a id="trigger_force"></a>
#### `trigger_force`

Esta é a força em gramas que dispara o fim de curso para interromper o movimento de origem. Quando um movimento de origem inicia, o fim de curso faz tara de si mesmo com a leitura atual da célula de carga. `trigger_force` é medida a partir desse valor de tara. Há sempre alguma ultrapassagem deste valor quando a sonda colide com a mesa, portanto seja conservador. p. ex. uma configuração de 100g pode resultar em 350g de força de pico antes de o cabeçote parar. Essa ultrapassagem aumentará com `speed` de sondagem mais rápida, baixa taxa de amostragem do ADC ou [origem com múltiplos MCUs](Multi_MCU_Homing.md).

<a id="reference_tare_counts"></a>
#### `reference_tare_counts`

Este é o valor de tara de referência definido por `LOAD_CELL_CALIBRATE`. Este valor funciona com `force_safety_limit` para limitar a força máxima no cabeçote.

<a id="force_safety_limit"></a>
#### `force_safety_limit`

Esta é a força absoluta máxima, relativa a `reference_tare_counts`, que a sonda permitirá durante origem ou sondagem. Se o MCU vir essa força excedida, ele desligará a impressora com o erro `!! Load cell endstop: too much force!`. Há várias maneiras de isso ser disparado:

O primeiro risco contra o qual isso protege é escolher um valor muito grande para `drift_filter_cutoff_frequency`. Isso pode fazer com que o filtro de deriva filtre um evento de sondagem e continue o movimento de referenciamento. Se isso acontecer, o `force_safety_limit` atua como proteção reserva.

O segundo problema é sondar repetidamente em um mesmo ponto. O Klipper não retrai a sonda ao executar um único comando `PROBE`. Isso pode resultar em força aplicada ao cabeçote ao fim de um ciclo de sondagem. Como as forças externas podem variar muito entre pontos de sondagem, o `load_cell_probe` realiza uma tara antes de iniciar cada sondagem. Se você repetir o comando `PROBE`, o load_cell_probe tarará o fim de curso com a força atual. Vários ciclos disso resultarão em força cada vez maior sobre o cabeçote. O `force_safety_limit` impede que esse ciclo saia de controle.

Outra forma de essa fuga ocorrer é dano ao extensômetro. Se a peça metálica for permanentemente deformada, isso alterará o `reference_tare_counts` do dispositivo. Isso coloca o valor inicial de tara muito mais próximo do limite, tornando mais provável que ele seja violado. Você deve ser notificado se isso estiver acontecendo, pois seu hardware foi permanentemente danificado.

A última forma de isso ser acionado é devido a mudanças de temperatura. Se seus extensômetros forem aquecidos, seus `reference_tare_counts` podem ser muito diferentes na temperatura ambiente em comparação com a temperatura de operação. Nesse caso, pode ser necessário aumentar o `force_safety_limit` para permitir mudanças térmicas.

<a id="load-cell-endstop-watchdog-task"></a>
#### Tarefa de vigilância (watchdog) do fim de curso da célula de carga

Durante a origem, o load_cell_endstop inicia uma tarefa no MCU para acompanhar as medidas que chegam da sonda. Se a sonda não enviar medidas por 2 períodos de amostragem, a vigilância desligará a impressora com um erro `!! LoadCell Endstop timed out waiting on ADC data`.

Se isso acontecer, a causa mais provável é uma falha do ADC. Aterramento inadequado da sua impressora pode ser a causa raiz. A estrutura, a carcaça da fonte de alimentação e a mesa de impressão devem estar todas conectadas ao terra. Pode ser necessário aterrar a estrutura em vários pontos. Perfis de alumínio anodizado não conduzem bem eletricidade. Pode ser necessário lixar a área onde o fio de aterramento é fixado para obter um bom contato elétrico.

<a id="interpolation"></a>
#### Interpolação

Para aumentar a precisão do resultado de sondagem, a posição do primeiro contato entre o bico e a mesa pode ser estimada ajustando uma função por partes aos dados medidos. Os dados consistirão em duas regiões: enquanto o bico estiver acima da mesa, a força será constante (no valor de tara). Quando o bico estiver em contato com a mesa, a força aumentará linearmente com a diminuição da posição z. Uma função por partes é ajustada aos dados e a posição z ideal do ponto de divisão é encontrada minimizando o erro quadrático. Isso permite uma resolução mais fina que a distância entre os pontos de amostragem e também será menos sensível a ruído.

Por razões físicas e técnicas, a interpolação usa dados coletados durante um movimento adicional de subida após o movimento inicial de descida. Os primeiros 300ms de dados coletados durante o movimento de subida serão usados para o ajuste (isso minimiza a influência de derivas de tara). Os dados coletados devem conter amostras suficientes para o ajuste: são necessárias pelo menos 3 amostras abaixo e acima do ponto de contato.

Recomenda-se usar uma força de acionamento relativamente alta para que a sonda tenha um sinal suficientemente forte. Se você tiver poucas amostras abaixo do ponto de contato, tente aumentar o `trigger_force` ou reduzir a `lift_speed`. No entanto, se a `lift_speed` for muito pequena, haverá poucas amostras acima do ponto de contato devido à janela de 300ms.

A distância do movimento de subida pode ser configurada pelo parâmetro `sample_retract_dist`.

<a id="load-cell-probe-setup"></a>
## Configuração da sonda com célula de carga

Esta seção aborda o processo de comissionamento de uma sonda com célula de carga.

<a id="verify-the-load-cell-first"></a>
### Verifique primeiro a célula de carga

Um `[load_cell_probe]` também é um `[load_cell]`, e os comandos de código-g relacionados a `[load_cell]` funcionam com `[load_cell_probe]`. Antes de tentar usar uma sonda com célula de carga, siga as instruções de [calibração da célula de carga](Load_Cell.md#calibrating-a-load-cell) com `LOAD_CELL_CALIBRATE` e verifique sua operação com `LOAD_CELL_DIAGNOSTIC`.

<a id="verify-probe-operation-with-load_cell_test_tap"></a>
### Verifique a operação da sonda com LOAD_CELL_TEST_TAP

Use o comando `LOAD_CELL_TEST_TAP` para testar a operação da sonda com célula de carga antes de tentar sondar com ela. Esse comando detecta toques, assim como o comando PROBE, mas não move o eixo z. Por padrão, ele aguarda 3 toques antes de encerrar o teste. Você tem 30 segundos para cada toque; se nenhum toque for detectado, o comando atingirá o tempo limite.

Se esse teste falhar, verifique cuidadosamente sua configuração e o `LOAD_CELL_DIAGNOSTIC` para procurar problemas.

Sondas com célula de carga não suportam os comandos `QUERY_ENDSTOPS` ou `QUERY_PROBE`. Use `LOAD_CELL_TEST_TAP` para testar a funcionalidade antes de sondar.

<a id="suggested-probing-temperature"></a>
### Temperatura de sondagem sugerida

Atualmente, sugerimos manter a temperatura do bico abaixo do nível que faz o filamento escorrer durante referenciamento e sondagem. 140°C é um bom ponto de partida. Essa temperatura também é baixa o suficiente para não marcar superfícies de impressão de PEI.

Sujeira no bico e na mesa de impressão devido ao escorrimento de filamento é a fonte nº 1 de erro de sondagem com a sonda com célula de carga. O Klipper ainda não tem uma maneira universal de detectar toques de má qualidade devido ao escorrimento de filamento. O código existente pode decidir que um toque é válido quando ele é de má qualidade. Classificar esses toques de má qualidade é uma área de pesquisa ativa.

O Klipper também não tem suporte para realocar um ponto de sondagem se o local ficou sujo por escorrimento de filamento. Módulos como `quad_gantry_level` sondarão repetidamente as mesmas coordenadas mesmo se uma sondagem anterior tiver falhado ali.

Diante do exposto, é fortemente sugerido não sondar em temperaturas de impressão.

<a id="hot-nozzle-protection"></a>
### Proteção contra bico quente

O projeto Voron tem uma ótima macro para proteger sua superfície de impressão contra o bico quente. Consulte [`activate_gcode` do Voron Tap](https://github.com/VoronDesign/Voron-Tap/blob/main/config/tap_klipper_instructions.md)

É altamente sugerido adicionar algo assim à sua configuração.

<a id="nozzle-cleaning"></a>
### Limpeza do bico

Antes de sondar, o bico deve estar limpo. Você pode fazer isso manualmente antes de cada impressão. Você também pode implementar um limpador de bico e automatizar o processo. Aqui está uma sequência sugerida:

1. Aguarde o bico aquecer até a temperatura de sondagem (p. ex. `M109 S140`)
1. Referencie a máquina (`G28`)
1. Esfregue o bico em uma escova
1. Aguarde a estabilização térmica da mesa de impressão
1. Execute as tarefas de sondagem: QGL, malha da mesa etc.

<a id="temperature-compensation-for-nozzle-growth"></a>
### Compensação de temperatura para crescimento do bico

Se você estiver sondando em uma temperatura segura, o bico expandirá após aquecer até as temperaturas de impressão. Isso fará com que o bico fique mais longo e mais próximo da superfície de impressão. Você pode compensar isso com [[z_thermal_adjust]](Config_Reference.md#z_thermal_adjust). Esse ajuste funcionará em uma faixa de temperaturas de impressão de PLA a PC.

<a id="calculating-the-temp_coeff-for-z_thermal_adjust"></a>
#### Calculando o `temp_coeff` para `[z_thermal_adjust]`

A maneira mais fácil de fazer isso é medir em 2 temperaturas diferentes. Idealmente, estes devem ser os limites superior e inferior da faixa de temperatura de impressão. Por exemplo, 180°C e 290°C. Você pode executar um `PROBE_ACCURACY` em ambas as temperaturas e então calcular a diferença do `average z` em ambas.

O valor de ajuste é a mudança no comprimento do bico dividida pela mudança na temperatura. p. ex.

```
temp_coeff = -0.05 / (290 - 180) = -0.00045455
```

O resultado esperado é um número negativo. Valores positivos para `temp_coeff` movem o bico para mais perto da mesa e valores negativos o movem para mais longe. Espere ter que mover o bico para mais longe à medida que ele fica mais longo quando quente.

<a id="configure-z_thermal_adjust"></a>
#### Configure `[z_thermal_adjust]`

Configure o z_thermal_adjust para referenciar a `extruder` como fonte de dados de temperatura. P. ex.:

```
[z_thermal_adjust]
temp_coeff=-0.00045455
sensor_type: temperature_combined
sensor_list: extruder
combination_method: max
maximum_deviation: 999
min_temp: 0
max_temp: 400
max_z_adjustment: 0.1
```

<a id="continuous-tare-filters-for-toolhead-load-cells"></a>
## Filtros contínuos de tara para células de carga no cabeçote

O Klipper implementa um filtro IIR configurável no MCU para fornecer taragem contínua da célula de carga durante a sondagem. Taragem contínua significa que o valor 0 acompanha a deriva causada por fatores externos como tubos Bowden e mudanças térmicas. Isso é voltado a sensores no cabeçote e mesas móveis que sofrem muitas forças externas que mudam durante a sondagem.

<a id="installing-scipy"></a>
### Instalando SciPy

O código de filtragem usa a excelente biblioteca [SciPy](https://scipy.org/) para calcular os coeficientes do filtro com base nos valores inseridos na configuração.

Compilações pré-compiladas de SciPy estão disponíveis para Python 3 em sistemas Raspberry Pi de 32 bits e 64 bits. 32 bits/64 bits + Python 3 é fortemente recomendado porque simplificará sua experiência de instalação. Funciona com Python 2, mas a instalação pode levar mais de 30 minutos e exige a instalação de ferramentas adicionais.

```bash
~/klippy-env/bin/pip install scipy
```

<a id="filter-workbench"></a>
### Bancada de filtros

Os parâmetros do filtro devem ser selecionados com base na deriva observada na impressora durante a operação normal. Um notebook Jupyter é fornecido em scripts, [filter_workbench.ipynb](../../scripts/filter_workbench.ipynb), para realizar uma investigação detalhada com dados reais capturados e FFTs.

<a id="filtering-suggestions"></a>
### Sugestões de filtragem

Para quem está apenas tentando fazer um filtro funcionar, siga estas sugestões:

* A única opção essencial é `drift_filter_cutoff_frequency`. Um valor inicial conservador é `0.5`Hz. A Prusa forneceu o MK4 com uma configuração de `0.8`Hz e o XL com `11.2`Hz. Esta é provavelmente uma faixa segura para experimentar. Esse valor deve ser aumentado somente até que a deriva normal devida à força do tubo Bowden seja eliminada. Definir esse valor muito alto resultará em acionamento lento e força excessiva passando pelo cabeçote.
* Mantenha `trigger_force` baixo. O padrão é `75`g. O filtro de deriva mantém o valor interno de gramas muito próximo de 0, portanto não é necessária uma grande força de acionamento.
* Mantenha `force_safety_limit` em um valor conservador. O valor padrão é 2Kg e deve manter seu cabeçote seguro durante experimentos. Se você atingir esse limite, o valor de `drift_filter_cutoff_frequency` pode estar muito alto.

<a id="suggestions-for-load-cell-tool-boards"></a>
## Sugestões para placas de ferramenta com célula de carga

Esta seção aborda sugestões para desenvolvedores de placas de cabeçote que desejam suportar [load_cell_probe]

<a id="adc-sensor-selection--board-development-hints"></a>
### Seleção de sensor ADC e dicas de desenvolvimento de placa

Idealmente, um sensor atenderia a estes critérios:

* Pelo menos 24 bits de largura
* Usar comunicação SPI
* Ter um pino que possa ser usado para indicar amostra pronta sem comunicação SPI. Isso é frequentemente chamado de pino "data ready" ou "DRDY". Verificar um pino é muito mais rápido do que executar uma consulta SPI.
* Ter uma configuração de ganho de amplificador de ganho programável de 128. Isso deve eliminar a necessidade de um amplificador separado.
* Indicar via SPI se o sensor foi reiniciado. Detectar reinicializações evita erros de temporização no referenciamento e o uso de dados ruidosos na inicialização. Também pode ajudar os usuários a rastrear problemas de fiação e aterramento.
* Uma taxa de amostragem selecionável entre 350Hz e 2Khz. Taxas de amostragem muito altas não se mostram benéficas em nossas impressoras 3D porque produzem muito ruído ao se mover rápido. Taxas de amostragem abaixo de 250Hz exigirão velocidades de sondagem mais lentas. Elas também aumentam a força sobre o cabeçote devido a atrasos maiores entre medições. Por exemplo, um sensor de 500Hz movendo-se a 5mm/s tem o mesmo fator de segurança que um sensor de 100Hz movendo-se a apenas 1mm/s.
* Se projetar para aplicações sob a mesa e quiser detectar várias células de carga, use um chip que possa amostrar todas as suas entradas simultaneamente. ADCs multiplexados que exigem alternância de canais têm acomodação de várias amostras após cada troca de canal, tornando-os inadequados para aplicações de sondagem.

Implementar suporte para um novo chip sensor não é particularmente difícil com a infraestrutura `bulk_sensor` e `load_cell_endstop` do Klipper.

<a id="5v-power-filtering"></a>
### Filtragem de alimentação de 5V

É fortemente sugerido usar capacitores maiores do que os especificados pelo fabricante do chip ADC. Chips ADC geralmente são voltados a ambientes de baixo ruído, como dispositivos alimentados por bateria. As notas de aplicação sugeridas pelos fabricantes de sensores geralmente pressupõem uma fonte de alimentação silenciosa. Trate seus valores sugeridos de capacitores como mínimos.

Impressoras 3D colocam enormes quantidades de ruído no barramento de 5V e isso pode arruinar a precisão do sensor. Teste o sensor na placa com uma fonte de alimentação típica de impressora 3D e drivers de motor de passo ativos antes de decidir os tamanhos dos capacitores de suavização.

<a id="grounding--ground-planes"></a>
### Aterramento e planos de terra

Chips ADC analógicos contêm componentes muito vulneráveis a ruído e ESD. Um grande plano de terra na primeira camada da placa sob o chip pode ajudar com ruído. Mantenha o chip longe de seções de potência e conversores CC para CC. A placa deve ter aterramento adequado de volta à fonte CC.

<a id="hx711-and-hx717-notes"></a>
### Notas sobre HX711 e HX717

Esse sensor é popular por causa de seu baixo custo e disponibilidade na cadeia de suprimentos. No entanto, este é um sensor com algumas desvantagens:

* Os sensores HX71x usam comunicação bit-bang, que tem alta sobrecarga no MCU. Usar um sensor que se comunica via SPI economizaria recursos na CPU da placa de ferramenta.
* O HX71x não tem como comunicar eventos de reinicialização ao MCU. O Klipper detecta reinicializações com uma heurística de temporização, mas isso não é ideal. Reinicializações indicam um problema de fiação ou aterramento.
* Para aplicações de sondagem, a versão HX717 é fortemente preferida por causa de sua maior taxa de amostragem (320 vs 80). A velocidade de sondagem no HX711 deve ser limitada a menos de 2mm/s.
* A taxa de amostragem no HX71x não pode ser definida pela configuração do Klipper. Se você tem a versão de 10SPS do sensor (amplamente distribuída), ela precisa ser religada fisicamente para operar a 80SPS.
