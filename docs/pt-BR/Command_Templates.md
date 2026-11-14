<a id="commands-templates"></a>
# Modelos de comandos

[English](../Command_Templates.md) | **Português brasileiro**

Este documento fornece informações sobre a implementação de sequências de comando G-Code nas seções de configuração gcode_macro (e similares).

<a id="g-code-macro-naming"></a>
## Nomenclatura de macro de código G

As letras maiúsculas e minúsculas não são importantes para o nome da macro G-Code - MY_MACRO e my_macro avaliarão o mesmo e podem ser chamadas em letras maiúsculas ou minúsculas. Se algum número for usado no nome da macro, todos devem estar no final do nome (por exemplo, TEST_MACRO25 é válido, mas MACRO25_TEST3 não é).

<a id="formatting-of-g-code-in-the-config"></a>
## Formatação do G-Code na configuração

O recuo é importante ao definir uma macro no arquivo de configuração. Para especificar uma sequência de código G de várias linhas, é importante que cada linha tenha a indentação adequada. Por exemplo:

```
[gcode_macro blink_led]
gcode:
  SET_PIN PIN=my_led VALUE=1
  G4 P2000
  SET_PIN PIN=my_led VALUE=0
```

Observe como a opção de configuração `gcode:` sempre começa no início da linha e as linhas subsequentes na macro G-Code nunca começam no início.

<a id="add-a-description-to-your-macro"></a>
## Adicione uma descrição à sua macro

Para ajudar a identificar a funcionalidade, uma breve descrição pode ser adicionada. Adicione `description:` com um texto curto para descrever a funcionalidade. O padrão é "G-Code macro" se não for especificado. Por exemplo:

```
[gcode_macro blink_led]
description: Blink my_led one time
gcode:
  SET_PIN PIN=my_led VALUE=1
  G4 P2000
  SET_PIN PIN=my_led VALUE=0
```

O terminal exibirá a descrição quando você usar o comando `HELP` ou a função de preenchimento automático.

<a id="saverestore-state-for-g-code-moves"></a>
## Salvar/Restaurar estado para movimentos G-Code

Infelizmente, a linguagem de comando G-Code pode ser difícil de usar. O mecanismo padrão para mover o cabeçote é através do comando `G1` (o comando `G0` é um alias para `G1` e pode ser usado de forma intercambiável com ele). No entanto, este comando depende da configuração do "estado de análise de código G" pelos comandos `M82`, `M83`, `G90`, `G91`, `G92` e `G1` anteriores. Ao criar uma macro G-Code, é uma boa ideia sempre definir explicitamente o estado de análise do G-Code antes de emitir um comando `G1`. (Caso contrário, existe o risco de o comando `G1` fazer uma solicitação indesejável.)

Uma maneira comum de conseguir isso é agrupar os movimentos `G1` em `SAVE_GCODE_STATE`, `G91` e `RESTORE_GCODE_STATE`. Por exemplo:

```
[gcode_macro MOVE_UP]
gcode:
  SAVE_GCODE_STATE NAME=my_move_up_state
  G91
  G1 Z10 F300
  RESTORE_GCODE_STATE NAME=my_move_up_state
```

O comando `G91` coloca o estado de análise G-Code em "modo de movimento relativo" e o comando `RESTORE_GCODE_STATE` restaura o estado ao que era antes de entrar na macro. Certifique-se de especificar uma velocidade explícita (através do parâmetro `F`) no primeiro comando `G1`.

<a id="template-expansion"></a>
## Expansão do modelo

A seção de configuração gcode_macro `gcode:` é avaliada usando a linguagem de modelo Jinja2. Pode-se avaliar expressões em tempo de execução agrupando-as em caracteres `{ }` ou usando instruções condicionais agrupadas em `{% %}`. Consulte a [documentação do Jinja2](http://jinja.pocoo.org/docs/2.10/templates/) para obter mais informações sobre a sintaxe.

Um exemplo de uma macro complexa:

```
[gcode_macro clean_nozzle]
gcode:
  {% set wipe_count = 8 %}
  SAVE_GCODE_STATE NAME=clean_nozzle_state
  G90
  G0 Z15 F300
  {% for wipe in range(wipe_count) %}
    {% for coordinate in [(275, 4),(235, 4)] %}
      G0 X{coordinate[0]} Y{coordinate[1] + 0.25 * wipe} Z9.7 F12000
    {% endfor %}
  {% endfor %}
  RESTORE_GCODE_STATE NAME=clean_nozzle_state
```

<a id="macro-parameters"></a>
### Parâmetros de macro

Geralmente é útil inspecionar os parâmetros passados para a macro quando ela é chamada. Esses parâmetros estão disponíveis através da pseudo-variável `params`. Por exemplo, se a macro:

```
[gcode_macro SET_PERCENT]
gcode:
  M117 Now at { params.VALUE|float * 100 }%
```

fossem invocados como `SET_PERCENT VALUE=.2` seria avaliado como `M117 Now at 20%`. Observe que os nomes dos parâmetros estão sempre em letras maiúsculas quando avaliados na macro e são sempre passados como strings. Se estiver executando matemática, eles devem ser explicitamente convertidos em números inteiros ou flutuantes.

É comum usar a diretiva `set` Jinja2 para usar um parâmetro padrão e atribuir o resultado a um nome local. Por exemplo:

```
[gcode_macro SET_BED_TEMPERATURE]
gcode:
  {% set bed_temp = params.TEMPERATURE|default(40)|float %}
  M140 S{bed_temp}
```

<a id="the-rawparams-variable"></a>
### A variável "rawparams"

Os parâmetros completos não analisados para a macro em execução podem ser acessados por meio da pseudovariável `rawparams`.

Observe que isso incluirá todos os comentários que faziam parte do comando original.

Consulte o arquivo [sample-macros.cfg](../../config/sample-macros.cfg) para obter um exemplo que mostra como substituir o comando `M117` usando `rawparams`.

<a id="the-printer-variable"></a>
### A variável "printer"

É possível inspecionar (e alterar) o estado atual da impressora através da pseudo-variável `printer`. Por exemplo:

```
[gcode_macro slow_fan]
gcode:
  M106 S{ printer.fan.speed * 0.9 * 255}
```

Os campos disponíveis são definidos no documento [Referência de estado](Status_Reference.md).

Importante! As macros são primeiro avaliadas em sua totalidade e somente então os comandos resultantes são executados. Se uma macro emitir um comando que altere o estado da impressora, os resultados dessa mudança de estado não serão visíveis durante a avaliação da macro. Isso também pode resultar em um comportamento sutil quando uma macro gera comandos que chamam outras macros, pois a macro chamada é avaliada quando é invocada (que ocorre após toda a avaliação da macro de chamada).

Por convenção, o nome imediatamente após `printer` é o nome de uma seção de configuração. Então, por exemplo, `printer.fan` refere-se ao objeto fan criado pela seção de configuração `[fan]`. Existem algumas exceções a esta regra - principalmente os objetos `gcode_move` e `toolhead`. Se a seção de configuração contiver espaços, então é possível acessá-la por meio do acessador `[ ]` - por exemplo: `printer["generic_heater my_chamber_heater"].temperature`.

Observe que a diretiva Jinja2 `set` pode atribuir um nome local a um objeto na hierarquia `printer`. Isso pode tornar as macros mais legíveis e reduzir a digitação. Por exemplo:

```
[gcode_macro QUERY_HTU21D]
gcode:
    {% set sensor = printer["htu21d my_sensor"] %}
    M117 Temp:{sensor.temperature} Humidity:{sensor.humidity}
```

<a id="actions"></a>
## Ações

Existem alguns comandos disponíveis que podem alterar o estado da impressora. Por exemplo, `{ action_emergency_stop() }` faria com que a impressora entrasse em estado de desligamento. Observe que essas ações são executadas no momento em que a macro é avaliada, o que pode levar um tempo significativo antes que os comandos do g-code gerados sejam executados.

Comandos de "ação" disponíveis:

- `action_respond_info(msg)`: Escreva a `msg` fornecida no pseudo-terminal /tmp/printer. Cada linha de `msg` será enviada com um prefixo "//".
- `action_raise_error(msg)`: Aborte a macro atual (e quaisquer macros de chamada) e escreva a `msg` fornecida no pseudo-terminal /tmp/printer. A primeira linha de `msg` será enviada com um prefixo "!! " e as linhas subseqüentes terão um prefixo "//".
- `action_emergency_stop(msg)`: Faz a transição da impressora para um estado de desligamento. O parâmetro `msg` é opcional, pode ser útil para descrever o motivo do desligamento.
- `action_call_remote_method(method_name)`: Chama um método registrado por um cliente remoto. Se o método receber parâmetros, eles devem ser fornecidos por meio de argumentos de palavra-chave, por exemplo: `action_call_remote_method("print_stuff", my_arg="hello_world")`

<a id="variables"></a>
## Variáveis

O comando SET_GCODE_VARIABLE pode ser útil para salvar o estado entre chamadas de macro. Os nomes das variáveis não podem conter caracteres maiúsculos. Por exemplo:

```
[gcode_macro start_probe]
variable_bed_temp: 0
gcode:
  # Salvar temperatura alvo na variável bed_temp
  SET_GCODE_VARIABLE MACRO=start_probe VARIABLE=bed_temp VALUE={printer.heater_bed.target}
  # Desligar aquecedor da mesa
  M140
  # Realizar sondagem
  PROBE
  # Chamar macro finish_probe na conclusão da sondagem
  finish_probe

[gcode_macro finish_probe]
gcode:
  # Restaurar temperatura
  M140 S{printer["gcode_macro start_probe"].bed_temp}
```

Certifique-se de levar em consideração o tempo de avaliação de macro e execução de comando ao usar SET_GCODE_VARIABLE.

<a id="delayed-gcodes"></a>
## Códigos G atrasados

A opção de configuração [delayed_gcode] pode ser usada para executar uma sequência de gcode atrasada:

```
[delayed_gcode clear_display]
gcode:
  M117

[gcode_macro load_filament]
gcode:
 G91
 G1 E50
 G90
 M400
 M117 Load Complete!
 UPDATE_DELAYED_GCODE ID=clear_display DURATION=10
```

Quando a macro `load_filament` acima for executada, ela exibirá uma mensagem "Load Complete!" mensagem após a conclusão da extrusão. A última linha do gcode habilita o delayed_gcode "clear_display", definido para ser executado em 10 segundos.

A opção de configuração `initial_duration` pode ser definida para executar o delayed_gcode na inicialização da impressora. A contagem regressiva começa quando a impressora entra no estado "pronta". Por exemplo, o delayed_gcode abaixo será executado 5 segundos depois que a impressora estiver pronta, inicializando a exibição com uma mensagem "Welcome!":

```
[delayed_gcode welcome]
initial_duration: 5.
gcode:
  M117 Welcome!
```

É possível que um gcode atrasado se repita atualizando-se na opção gcode:

```
[delayed_gcode report_temp]
initial_duration: 2.
gcode:
  {action_respond_info("Extruder Temp: %.1f" % (printer.extruder0.temperature))}
  UPDATE_DELAYED_GCODE ID=report_temp DURATION=2
```

O delayed_gcode acima enviará "// Extruder Temp: [ex0_temp]" para Octoprint a cada 2 segundos. Isso pode ser cancelado com o seguinte gcode:

```
UPDATE_DELAYED_GCODE ID=report_temp DURATION=0
```

<a id="menu-templates"></a>
## Modelos de menu

Se uma [seção de configuração de exibição](Config_Reference.md#display) estiver ativada, é possível personalizar o menu com as seções de configuração [menu](Config_Reference.md#menu).

Os seguintes atributos somente leitura estão disponíveis em modelos de menu:

* `menu.width` - largura do elemento (número de colunas de exibição)
* `menu.ns` - namespace do elemento
* `menu.event` - nome do evento que acionou o script
* `menu.input` - valor de entrada, disponível apenas no contexto do script de entrada

As seguintes ações estão disponíveis nos modelos de menu:

* `menu.back(force, update)`: executará o comando de voltar ao menu, parâmetros booleanos opcionais `<force>` e `<update>`.
   * Quando `<force>` for definido como True, ele também interromperá a edição. O valor padrão é Falso.
   * Quando `<update>` está definido como False, os itens do contêiner pai não são atualizados. O valor padrão é True.
* `menu.exit(force)` - executará o comando de saída do menu, parâmetro booleano opcional `<force>` valor padrão False.
   * Quando `<force>` for definido como True, ele também interromperá a edição. O valor padrão é Falso.

<a id="save-variables-to-disk"></a>
## Salvar Variáveis no disco

Se uma [seção de configuração save_variables](Config_Reference.md#save_variables) tiver sido habilitada, `SAVE_VARIABLE VARIABLE=<name> VALUE=<value>` pode ser usado para salvar a variável em disco para que possa ser usada entre reinicializações. Todas as variáveis armazenadas são carregadas no dicionário `printer.save_variables.variables` na inicialização e podem ser usadas em macros gcode. para evitar linhas excessivamente longas você pode adicionar o seguinte no topo da macro:

```
{% set svv = printer.save_variables.variables %}
```

Como exemplo, poderia ser usado para salvar o estado de hotend 2-in-1-out e ao iniciar uma impressão garantir que o extrusor ativo seja usado, em vez de T0:

```
[gcode_macro T1]
gcode:
  ACTIVATE_EXTRUDER extruder=extruder1
  SAVE_VARIABLE VARIABLE=currentextruder VALUE='"extruder1"'

[gcode_macro T0]
gcode:
  ACTIVATE_EXTRUDER extruder=extruder
  SAVE_VARIABLE VARIABLE=currentextruder VALUE='"extruder"'

[gcode_macro START_GCODE]
gcode:
  {% set svv = printer.save_variables.variables %}
  ACTIVATE_EXTRUDER extruder={svv.currentextruder}
```
