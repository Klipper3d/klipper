<a id="status-reference"></a>
# Referência de status

[English](../Status_Reference.md) | **Português brasileiro**

Este documento é uma referência das informações de status da impressora disponíveis no Klipper [macros](Command_Templates.md), [campos de exibição](Config_Reference.md#display) e por meio do [Servidor de API](API_Server.md).

Os campos neste documento estão sujeitos a alterações - se estiver usando um atributo, certifique-se de revisar o [documento de alterações de configuração](Config_Changes.md) ao atualizar o software Klipper.

<a id="angle"></a>
## angle

As seguintes informações estão disponíveis em objetos [angle some_name](Config_Reference.md#angle):

- `temperature`: A última leitura de temperatura (em Celsius) de um sensor hall magnético tle5012b. Este valor só está disponível se o sensor de ângulo for um chip tle5012b e se as medições estiverem em andamento (caso contrário, informa `None`).

<a id="bed_mesh"></a>
## bed_mesh

As seguintes informações estão disponíveis no objeto [bed_mesh](Config_Reference.md#bed_mesh):

- `profile_name`, `mesh_min`, `mesh_max`, `probed_matrix`, `mesh_matrix`: Informações sobre o bed_mesh atualmente ativo.
- `profiles`: O conjunto de perfis definidos atualmente conforme configurado usando BED_MESH_PROFILE.

<a id="bed_screws"></a>
## bed_screws

As seguintes informações estão disponíveis no objeto [bed_screws](Config_Reference.md#bed_screws):

- `is_active`: Retorna True se a ferramenta de ajuste de parafusos da mesa estiver ativa no momento.
- `state`: O estado da ferramenta de ajuste de parafusos da mesa. É uma das seguintes strings: "adjust", "fine".
- `current_screw`: O índice do parafuso atual sendo ajustado.
- `accepted_screws`: O número de parafusos aceitos.

<a id="canbus_stats"></a>
## canbus_stats

As seguintes informações estão disponíveis no objeto `canbus_stats some_mcu_name` (este objeto está automaticamente disponível se um mcu estiver configurado para usar canbus):

- `rx_error`: O número de erros de recepção detectados pelo hardware canbus do microcontrolador.
- `tx_error`: O número de erros de transmissão detectados pelo hardware canbus do microcontrolador.
- `tx_retries`: O número de tentativas de transmissão repetidas devido a contenção do barramento ou erros.
- `bus_state`: O estado da interface (normalmente “active” para um barramento em operação normal, “warn” para um barramento com erros recentes, “passive” para um barramento que não transmitirá mais quadros de erro canbus, ou “off” para um barramento que não transmitirá nem receberá mais mensagens).

Observe que apenas os microcontroladores rp2XXX relatam um campo `tx_retries` diferente de zero e os microcontroladores rp2XXX sempre relatam `tx_error` como zero e `bus_state` como "active".

<a id="configfile"></a>
## configfile

As seguintes informações estão disponíveis no objeto `configfile` (este objeto está sempre disponível):

- `settings.<section>.<option>`: Retorna a dada configuração do arquivo de configuração (ou valor padrão) durante a última inicialização ou reinicialização do software. (Quaisquer configurações alteradas em tempo de execução não serão refletidas aqui.)
- `config.<section>.<option>`: Retorna a dada configuração bruta do arquivo de configuração conforme lida pelo Klipper durante a última inicialização ou reinicialização do software. (Quaisquer configurações alteradas em tempo de execução não serão refletidas aqui.) Todos os valores são retornados como strings.
- `save_config_pending`: Retorna true se houver atualizações que um comando `SAVE_CONFIG` possa persistir no disco.
- `save_config_pending_items`: Contém as seções e opções que foram alteradas e seriam persistidas por um `SAVE_CONFIG`.
- `warnings`: Uma lista de avisos sobre opções de configuração. Cada entrada na lista será um dicionário contendo os campos `type` e `message` (ambos strings). Campos adicionais podem estar disponíveis dependendo do tipo de aviso.

<a id="display_status"></a>
## display_status

As seguintes informações estão disponíveis no objeto `display_status` (este objeto está automaticamente disponível se uma seção de configuração [display](Config_Reference.md#display) estiver definida):

- `progress`: O valor de progresso do último comando G-Code `M73` (ou `virtual_sdcard.progress` se nenhum `M73` recente recebido).
- `message`: A mensagem contida no último comando G-Code `M117`.

<a id="endstop_phase"></a>
## endstop_phase

As seguintes informações estão disponíveis no objeto [endstop_phase](Config_Reference.md#endstop_phase):

- `last_home.<stepper name>.phase`: A fase do motor de passo ao final da última tentativa de referenciamento.
- `last_home.<stepper name>.phases`: O número total de fases disponíveis no motor de passo.
- `last_home.<stepper name>.mcu_position`: A posição (conforme rastreada pelo microcontrolador) do motor de passo ao final da última tentativa de referenciamento. A posição é o número total de passos dados na direção avante menos o número total de passos dados na direção reversa desde a última reinicialização do microcontrolador.

<a id="exclude_object"></a>
## exclude_object

As seguintes informações estão disponíveis no objeto [exclude_object](Exclude_Object.md):

- `objects`: Uma matriz dos objetos conhecidos conforme fornecido pelo comando `EXCLUDE_OBJECT_DEFINE`. Esta é a mesma informação fornecida pelo comando `EXCLUDE_OBJECT VERBOSE=1`. Os campos `center` e `polygon` só estarão presentes se fornecidos no `EXCLUDE_OBJECT_DEFINE` original

   Aqui está um exemplo JSON:

```
[
  {
    "polygon": [
      [ 156.25, 146.2511675 ],
      [ 156.25, 153.7488325 ],
      [ 163.75, 153.7488325 ],
      [ 163.75, 146.2511675 ]
    ],
    "name": "CYLINDER_2_STL_ID_2_COPY_0",
    "center": [ 160, 150 ]
  },
  {
    "polygon": [
      [ 146.25, 146.2511675 ],
      [ 146.25, 153.7488325 ],
      [ 153.75, 153.7488325 ],
      [ 153.75, 146.2511675 ]
    ],
    "name": "CYLINDER_2_STL_ID_1_COPY_0",
    "center": [ 150, 150 ]
  }
]
```
- `excluded_objects`: Uma matriz de strings listando os nomes dos objetos excluídos.
- `current_object`: O nome do objeto atualmente sendo impresso.

<a id="extruder_stepper"></a>
## extruder_stepper

As seguintes informações estão disponíveis para objetos extruder_stepper (bem como objetos [extruder](Config_Reference.md#extruder)):

- `pressure_advance`: O valor atual de [avanço de pressão](Pressure_Advance.md).
- `smooth_time`: O tempo de suavização atual do pressure advance.
- `motion_queue`: O nome da extrusora à qual este extruder stepper está sincronizado no momento. Isso é informado como `None` se o extruder stepper não estiver associado no momento a uma extrusora.

<a id="fan"></a>
## fan

As seguintes informações estão disponíveis em objetos [fan](Config_Reference.md#fan), [heater_fan some_name](Config_Reference.md#heater_fan) e [controller_fan some_name](Config_Reference.md#controller_fan):

- `speed`: A velocidade do ventilador como float entre 0.0 e 1.0.
- `rpm`: A velocidade medida do ventilador em rotações por minuto se o ventilador tiver um tachometer_pin definido.

<a id="filament_switch_sensor"></a>
## filament_switch_sensor

As seguintes informações estão disponíveis em objetos [filament_switch_sensor some_name](Config_Reference.md#filament_switch_sensor):

- `enabled`: Retorna True se o sensor de interruptor estiver ativado no momento.
- `filament_detected`: Retorna True se o sensor estiver em estado acionado.

<a id="filament_motion_sensor"></a>
## filament_motion_sensor

As seguintes informações estão disponíveis em objetos [filament_motion_sensor some_name](Config_Reference.md#filament_motion_sensor):

- `enabled`: Retorna True se o sensor de movimento estiver ativado no momento.
- `filament_detected`: Retorna True se o sensor estiver em estado acionado.

<a id="firmware_retraction"></a>
## firmware_retraction

As seguintes informações estão disponíveis no objeto [firmware_retraction](Config_Reference.md#firmware_retraction):

- `retract_length`, `retract_speed`, `unretract_extra_length`, `unretract_speed`: As configurações atuais do módulo firmware_retraction. Essas configurações podem diferir do arquivo de configuração se um comando `SET_RETRACTION` alterá-las.

<a id="gcode"></a>
## gcode

As seguintes informações estão disponíveis no objeto `gcode`:

- `commands`: Retorna uma lista de todos os comandos disponíveis no momento. Para cada comando, se uma string de ajuda estiver definida, ela também será fornecida.

<a id="gcode_button"></a>
## gcode_button

As seguintes informações estão disponíveis em objetos [gcode_button some_name](Config_Reference.md#gcode_button):

- `state`: O estado atual do botão retornado como "PRESSED" ou "RELEASED"

<a id="gcode_macro"></a>
## gcode_macro

As seguintes informações estão disponíveis em objetos [gcode_macro some_name](Config_Reference.md#gcode_macro):

- `<variable>`: O valor atual de uma [variável de gcode_macro](Command_Templates.md#variables).

<a id="gcode_move"></a>
## gcode_move

As seguintes informações estão disponíveis no objeto `gcode_move` (este objeto está sempre disponível):

- `gcode_position`: A posição atual do cabeçote em relação à origem atual do G-Code. Ou seja, posições que se poderia enviar diretamente para um comando `G1`. Este valor é codificado como uma [coordenada](#accessing-coordinates).
- `position`: A última posição comandada do cabeçote usando o sistema de coordenadas especificado no arquivo de configuração. Este valor é codificado como uma [coordenada](#accessing-coordinates).
- `homing_origin`: A origem do sistema de coordenadas de G-code (em relação ao sistema de coordenadas especificado no arquivo de configuração) a usar após um comando `G28`. O comando `SET_GCODE_OFFSET` pode alterar esta posição. Este valor é codificado como uma [coordenada](#accessing-coordinates).
- `speed`: A última velocidade definida em um comando `G1` (em mm/s).
- `speed_factor`: A “sobrescrita de fator de velocidade” conforme definida por um comando `M220`. Este é um valor de ponto flutuante tal que 1.0 significa sem sobrescrita e, por exemplo, 2.0 dobraria a velocidade solicitada.
- `extrude_factor`: a "sobreposição do fator de extrusão" definida por um comando `M221`. É um valor de ponto flutuante em que 1.0 significa sem sobreposição e, por exemplo, 2.0 dobraria as extrusões solicitadas.
- `absolute_coordinates`: retorna True se estiver no modo de coordenadas absolutas `G90` ou False se estiver no modo relativo `G91`.
- `absolute_extrude`: retorna True se extrusões absolutas forem permitidas. Um comando `M83` de forçar modo de extrusão relativa faz este valor retornar False. Um comando `M82` retorna este valor para True.
- `axis_map`: fornece um mecanismo para localizar o componente de coordenada para um determinado id de G-Code usado em comandos `G1`. Consulte a seção [Acesso às coordenadas](#accessing-coordinates) para detalhes.

<a id="hall_filament_width_sensor"></a>
## hall_filament_width_sensor

As seguintes informações estão disponíveis no objeto [hall_filament_width_sensor](Config_Reference.md#hall_filament_width_sensor):

- todos os itens de [filament_switch_sensor](Status_Reference.md#filament_switch_sensor)
- `is_active`: retorna True se o sensor estiver ativo no momento.
- `flow_compensation_enabled`: retorna True se a compensação de vazão estiver ativada.
- `Diameter`: retorna a última leitura de largura em mm se o sensor estiver ativo ou o diâmetro nominal do filamento se não estiver.
- `Raw`: a última leitura ADC bruta do sensor.

<a id="heater"></a>
## heater

As seguintes informações estão disponíveis para objetos de aquecedor como [extruder](Config_Reference.md#extruder), [heater_bed](Config_Reference.md#heater_bed) e [heater_generic](Config_Reference.md#heater_generic):

- `temperature`: a última temperatura relatada (em Celsius como float) para o aquecedor informado.
- `target`: a temperatura-alvo atual (em Celsius como float) para o aquecedor informado.
- `power`: a última configuração do pino PWM (um valor entre 0.0 e 1.0) associado ao aquecedor.
- `can_extrude`: se a extrusora pode extrudar (definido por `min_extrude_temp`), disponível somente para [extruder](Config_Reference.md#extruder)

<a id="heaters"></a>
## heaters

As seguintes informações estão disponíveis no objeto `heaters` (este objeto está disponível se algum aquecedor estiver definido):

- `available_heaters`: retorna uma lista de todos os aquecedores disponíveis no momento pelos nomes completos das seções de configuração, ex. `["extruder", "heater_bed", "heater_generic my_custom_heater"]`.
- `available_sensors`: retorna uma lista de todos os sensores de temperatura disponíveis no momento pelos nomes completos das seções de configuração, ex. `["extruder", "heater_bed", "heater_generic my_custom_heater", "temperature_sensor electronics_temp"]`.
- `available_monitors`: retorna uma lista de todos os monitores de temperatura disponíveis no momento pelos nomes completos das seções de configuração, ex. `["tmc2240 stepper_x"]`. Embora um sensor de temperatura esteja sempre disponível para leitura, um monitor de temperatura pode não estar disponível e retornará null nesse caso.

<a id="idle_timeout"></a>
## idle_timeout

As seguintes informações estão disponíveis no objeto [idle_timeout](Config_Reference.md#idle_timeout) (este objeto está sempre disponível):

- `state`: o estado atual da impressora conforme acompanhado pelo módulo idle_timeout. É uma das seguintes strings: "Idle", "Printing", "Ready".
- `printing_time`: a quantidade de tempo (em segundos) em que a impressora esteve no estado "Printing" (conforme acompanhado pelo módulo idle_timeout).
- `idle_timeout`: o 'tempo-limite' atual (em segundos) de espera para o gcode ser acionado. (conforme definido por [SET_IDLE_TIMEOUT](G-Codes.md#set_idle_timeout))

<a id="led"></a>
## led

As seguintes informações estão disponíveis para cada seção de configuração `[led led_name]`, `[neopixel led_name]`, `[dotstar led_name]`, `[pca9533 led_name]` e `[pca9632 led_name]` definida em printer.cfg:

- `color_data`: uma lista de listas de cores contendo os valores RGBW para um led na cadeia. Cada valor é representado como float de 0.0 a 1.0. Cada lista de cores contém 4 itens (vermelho, verde, azul, branco) mesmo que o LED subjacente suporte menos canais de cor. Por exemplo, o valor de azul (3º item na lista de cores) do segundo neopixel em uma cadeia poderia ser acessado em `printer["neopixel <config_name>"].color_data[1][2]`.

<a id="load_cell"></a>
## load_cell

As seguintes informações estão disponíveis para cada `[load_cell name]`:

- `is_calibrated`: True/False indicando se a célula de carga está calibrada.
- `counts_per_gram`: o número de contagens brutas do sensor que equivale a 1 grama de força.
- `reference_tare_counts`: o número de referência de contagens brutas do sensor para força 0.
- `tare_counts`: o número atual de contagens brutas do sensor para força 0.
- `force_g`: a força em gramas, com média sobre o último período de amostragem.
- `min_force_g`: a força mínima em gramas, sobre o último período de amostragem.
- `max_force_g`: a força máxima em gramas, sobre o último período de amostragem.
- `errors`: o número de erros do sensor detectados desde o último início das medições.
- `overflows`: o número de estouros do buffer de dados detectados desde o último início das medições.
- `sample_rate`: a taxa de amostragem do sensor em amostras por segundo.

<a id="load_cell_probe"></a>
## load_cell_probe

As seguintes informações estão disponíveis para `[load_cell_probe]`:

- todos os itens de [load_cell](Status_Reference.md#load_cell)
- todos os itens de [probe](Status_Reference.md#probe)
- `endstop_tare_counts`: a sonda de célula de carga mantém um valor de tara independente da célula de carga. Ele é redefinido no início de cada sondagem.
- `last_trigger_time`: carimbo de data/hora do último acionamento de referenciamento.
- `last_z_result`: o resultado de posição Z do último toque.
- `is_last_tap_valid`: True se o resultado do último toque for válido.

<a id="manual_probe"></a>
## manual_probe

As seguintes informações estão disponíveis no objeto `manual_probe`:

- `is_active`: retorna True se um script auxiliar de sondagem manual estiver ativo no momento.
- `z_position`: a altura atual do bico (como a impressora a entende no momento).
- `z_position_lower`: última tentativa de sondagem logo abaixo da altura atual.
- `z_position_upper`: última tentativa de sondagem logo acima da altura atual.

<a id="mcu"></a>
## mcu

As seguintes informações estão disponíveis nos objetos [mcu](Config_Reference.md#mcu) e [mcu some_name](Config_Reference.md#mcu-my_extra_mcu):

- `mcu_version`: a versão do código Klipper relatada pelo microcontrolador.
- `mcu_build_versions`: informações sobre as ferramentas de compilação usadas para gerar o código do microcontrolador (conforme relatado pelo microcontrolador).
- `mcu_constants.<constant_name>`: constantes de tempo de compilação relatadas pelo microcontrolador. As constantes disponíveis podem variar entre arquiteturas de microcontrolador e a cada revisão do código.
- `mcu_kconfig`: a configuração mínima de compilação (saída de `savedefconfig`) a partir da qual o firmware do microcontrolador foi compilado, conforme relatado pelo microcontrolador. `None` se o firmware for anterior a este recurso ou não o fornecer.
- `last_stats.<statistics_name>`: informações estatísticas sobre a conexão do microcontrolador.

<a id="motion_report"></a>
## motion_report

As seguintes informações estão disponíveis no objeto `motion_report` (este objeto está automaticamente disponível se alguma seção de configuração de motor de passo estiver definida):

- `live_position`: a posição solicitada do cabeçote de impressão interpolada para o momento atual. Este valor é codificado como uma [coordenada](#accessing-coordinates).
- `live_velocity`: a velocidade solicitada do cabeçote de impressão (em mm/s) no momento atual.
- `live_extruder_velocity`: a velocidade solicitada da extrusora (em mm/s) no momento atual.

<a id="output_pin"></a>
## output_pin

As seguintes informações estão disponíveis nos objetos [output_pin some_name](Config_Reference.md#output_pin) e [pwm_tool some_name](Config_Reference.md#pwm_tool):

- `value`: o "valor" do pino, conforme definido por um comando `SET_PIN`.

<a id="palette2"></a>
## palette2

As seguintes informações estão disponíveis no objeto [palette2](Config_Reference.md#palette2):

- `ping`: quantidade do último ping relatado da Palette 2 em porcentagem.
- `remaining_load_length`: ao iniciar uma impressão com Palette 2, este será a quantidade de filamento a carregar na extrusora.
- `is_splicing`: True quando a Palette 2 estiver emendando filamento.

<a id="pause_resume"></a>
## pause_resume

As seguintes informações estão disponíveis no objeto [pause_resume](Config_Reference.md#pause_resume):

- `is_paused`: retorna true se um comando PAUSE tiver sido executado sem um RESUME correspondente.

<a id="print_stats"></a>
## print_stats

As seguintes informações estão disponíveis no objeto `print_stats` (este objeto está automaticamente disponível se uma seção de configuração [virtual_sdcard](Config_Reference.md#virtual_sdcard) estiver definida):

- `filename`, `total_duration`, `print_duration`, `filament_used`, `state`, `message`: informações estimadas sobre a impressão atual quando uma impressão via virtual_sdcard estiver ativa.
- `info.total_layer`: o valor de camada total do último comando de G-Code `SET_PRINT_STATS_INFO TOTAL_LAYER=<value>`.
- `info.current_layer`: o valor de camada atual do último comando de G-Code `SET_PRINT_STATS_INFO CURRENT_LAYER=<value>`.

<a id="probe"></a>
## probe

As seguintes informações estão disponíveis no objeto [probe](Config_Reference.md#probe) (este objeto também está disponível se uma seção de configuração [bltouch](Config_Reference.md#bltouch) estiver definida):

- `name`: retorna o nome da sonda em uso.
- `last_query`: retorna True se a sonda tiver sido relatada como "triggered" durante o último comando QUERY_PROBE. Observe que, se usado em uma macro, devido à ordem de expansão do modelo, o comando QUERY_PROBE deve ser executado antes da macro que contém esta referência.
- `last_probe_position`: os resultados do último comando `PROBE`. Este valor é codificado como uma [coordenada](#accessing-coordinates). O hardware da sonda estima que, se alguém comandasse o cabeçote de impressão para a posição XY `last_probe_position.x`,`last_probe_position.y` e descesse, então a ponta do cabeçote tocaria primeiro a mesa a uma altura Z de `last_probe_position.z`. Essas coordenadas são relativas à estrutura (ou seja, usam o sistema de coordenadas especificado no arquivo de configuração). Observe que, se usado em uma macro, devido à ordem de expansão do modelo, o comando `PROBE` deve ser executado antes da macro que contém esta referência.
- `last_z_result`: este valor está obsoleto; será removido em um futuro próximo.

<a id="pwm_cycle_time"></a>
## pwm_cycle_time

As seguintes informações estão disponíveis em objetos [pwm_cycle_time some_name](Config_Reference.md#pwm_cycle_time):

- `value`: o "valor" do pino, conforme definido por um comando `SET_PIN`.

<a id="quad_gantry_level"></a>
## quad_gantry_level

As seguintes informações estão disponíveis no objeto `quad_gantry_level` (este objeto está disponível se quad_gantry_level estiver definido):

- `applied`: True se o processo de nivelamento do pórtico tiver sido executado e concluído com sucesso.

<a id="query_endstops"></a>
## query_endstops

As seguintes informações estão disponíveis no objeto `query_endstops` (este objeto está disponível se algum fim de curso estiver definido):

- `last_query["<endstop>"]`: retorna True se o fim de curso informado tiver sido relatado como "triggered" durante o último comando QUERY_ENDSTOP. Observe que, se usado em uma macro, devido à ordem de expansão do modelo, o comando QUERY_ENDSTOP deve ser executado antes da macro que contém esta referência.

<a id="screws_tilt_adjust"></a>
## screws_tilt_adjust

As seguintes informações estão disponíveis no objeto `screws_tilt_adjust`:

- `error`: retorna True se o comando `SCREWS_TILT_CALCULATE` mais recente incluiu o parâmetro `MAX_DEVIATION` e qualquer um dos pontos de parafuso sondados excedeu o `MAX_DEVIATION` especificado.
- `max_deviation`: retorna o último valor de `MAX_DEVIATION` do comando `SCREWS_TILT_CALCULATE` mais recente.
- `results["<screw>"]`: um dicionário contendo as seguintes chaves:
   - `z`: a altura Z medida da posição do parafuso.
   - `sign`: uma string especificando a direção de giro do parafuso para o ajuste necessário. Seja "CW" para sentido horário ou "CCW" para sentido anti-horário.
   - `adjust`: o número de voltas do parafuso para ajustar o parafuso, dado no formato "HH:MM," em que "HH" é o número de voltas completas do parafuso e "MM" é o número de "minutos de um mostrador de relógio" representando uma volta parcial do parafuso. (Ex. "01:15" significaria girar o parafuso uma volta e um quarto.)
   - `is_base`: retorna True se este for o parafuso de base.

<a id="servo"></a>
## servo

As seguintes informações estão disponíveis em objetos [servo some_name](Config_Reference.md#servo):

- `printer["servo <config_name>"].value`: a última configuração do pino PWM (um valor entre 0.0 e 1.0) associado ao servo.

<a id="skew_correctionpy"></a>
## skew_correction.py

As seguintes informações estão disponíveis no objeto `skew_correction` (este objeto está disponível se algum skew_correction estiver definido):

- `current_profile_name`: retorna o nome do SKEW_PROFILE carregado no momento.

<a id="stepper_enable"></a>
## stepper_enable

As seguintes informações estão disponíveis no objeto `stepper_enable` (este objeto está disponível se algum motor de passo estiver definido):

- `steppers["<stepper>"]`: retorna True se o motor de passo informado estiver ativado.

<a id="system_stats"></a>
## system_stats

As seguintes informações estão disponíveis no objeto `system_stats` (este objeto está sempre disponível):

- `sysload`, `cputime`, `memavail`: informações sobre o sistema operacional do hospedeiro e a carga do processo.

<a id="temperature-sensors"></a>
## temperature sensors

As seguintes informações estão disponíveis em

Objetos [bme280 config_section_name](Config_Reference.md#bmp180bmp280bme280bmp388bme680-temperature-sensor), [htu21d config_section_name](Config_Reference.md#htu21d-sensor), [sht3x config_section_name](Config_Reference.md#sht3x-sensor), [lm75 config_section_name](Config_Reference.md#lm75-temperature-sensor), [temperature_host config_section_name](Config_Reference.md#host-temperature-sensor) e [temperature_combined config_section_name](Config_Reference.md#combined-temperature-sensor):

- `temperature`: a última temperatura lida do sensor.
- `humidity`, `pressure`, `gas`: os últimos valores lidos do sensor (somente em sensores bme280, htu21d, sht3x e lm75).

<a id="temperature_fan"></a>
## temperature_fan

As seguintes informações estão disponíveis em objetos [temperature_fan some_name](Config_Reference.md#temperature_fan):

- `temperature`: a última temperatura lida do sensor.
- `target`: a temperatura-alvo para o ventilador.

<a id="temperature_sensor"></a>
## temperature_sensor

As seguintes informações estão disponíveis em objetos [temperature_sensor some_name](Config_Reference.md#temperature_sensor):

- `temperature`: a última temperatura lida do sensor.
- `measured_min_temp`, `measured_max_temp`: a menor e a maior temperatura vistas pelo sensor desde a última reinicialização do software hospedeiro Klipper.

<a id="tmc-drivers"></a>
## tmc drivers

As seguintes informações estão disponíveis em objetos [driver de motor de passo TMC](Config_Reference.md#tmc-stepper-driver-configuration) (ex. `[tmc2208 stepper_x]`):

- `mcu_phase_offset`: a posição do motor de passo do microcontrolador correspondente à fase "zero" do driver. Este campo pode ser null se o deslocamento de fase não for conhecido.
- `phase_offset_position`: a "posição comandada" correspondente à fase "zero" do driver. Este campo pode ser null se o deslocamento de fase não for conhecido.
- `drv_status`: os resultados da última consulta de estado do driver. (Somente campos diferentes de zero são relatados.) Este campo será null se o driver não estiver ativado (e, portanto, não for consultado periodicamente).
- `temperature`: a temperatura interna relatada pelo driver. Este campo será null se o driver não estiver ativado ou se o driver não suportar relato de temperatura.
- `run_current`: a corrente de operação definida no momento.
- `hold_current`: a corrente de espera definida no momento.

<a id="toolhead"></a>
## toolhead

As seguintes informações estão disponíveis no objeto `toolhead` (este objeto está sempre disponível):

- `position`: a última posição comandada do cabeçote de impressão relativa ao sistema de coordenadas especificado no arquivo de configuração. Este valor é codificado como uma [coordenada](#accessing-coordinates).
- `extruder`: o nome da extrusora ativa no momento. Por exemplo, em uma macro pode-se usar `printer[printer.toolhead.extruder].target` para obter a temperatura-alvo da extrusora atual.
- `homed_axes`: os eixos cartesianos atuais considerados em estado "homed". É uma string contendo um ou mais de "x", "y", "z".
- `axis_minimum`, `axis_maximum`: os limites de deslocamento dos eixos (mm) após o referenciamento. Este valor é codificado como uma [coordenada](#accessing-coordinates).
- Para impressoras Delta, `cone_start_z` é a altura z máxima no raio máximo (`printer.toolhead.cone_start_z`).
- `max_velocity`, `max_accel`, `minimum_cruise_ratio`, `square_corner_velocity`: os limites de impressão atuais em vigor. Podem diferir das configurações do arquivo de configuração se um comando `SET_VELOCITY_LIMIT` (ou `M204`) alterá-los em tempo de execução.
- `stalls`: o número total de vezes (desde a última reinicialização) em que a impressora teve que ser pausada porque o cabeçote de impressão se moveu mais rápido do que os movimentos podiam ser lidos da entrada de G-Code.
- `extra_axes`: fornece um mecanismo para localizar o componente de coordenada para eixos extras disponíveis em comandos de movimento padrão do tipo `G1`. Consulte a seção [Acesso às coordenadas](#accessing-coordinates) para detalhes.

<a id="dual_carriage"></a>
## dual_carriage

As seguintes informações estão disponíveis em [dual_carriage](Config_Reference.md#dual_carriage) em um robô cartesian, hybrid_corexy ou hybrid_corexz

- `carriage_0`: o modo do carro 0. Os valores possíveis são: "INACTIVE" e "PRIMARY".
- `carriage_1`: o modo do carro 1. Os valores possíveis são: "INACTIVE", "PRIMARY", "COPY" e "MIRROR".

Em uma cinemática `generic_cartesian`, as seguintes informações estão disponíveis em `dual_carriage`:

- `carriages["<carriage>"]`: o modo do carro `<carriage>`. Os valores possíveis são "INACTIVE" e "PRIMARY" para o carro primário e "INACTIVE", "PRIMARY", "COPY" e "MIRROR" para o carro duplo.

<a id="virtual_sdcard"></a>
## virtual_sdcard

As seguintes informações estão disponíveis no objeto [virtual_sdcard](Config_Reference.md#virtual_sdcard):

- `is_active`: retorna True se uma impressão a partir de arquivo estiver ativa no momento.
- `progress`: uma estimativa do progresso atual da impressão (com base no tamanho do arquivo e na posição no arquivo).
- `file_path`: um caminho completo para o arquivo do arquivo carregado no momento.
- `file_position`: a posição atual (em bytes) de uma impressão ativa.
- `file_size`: o tamanho do arquivo (em bytes) do arquivo carregado no momento.

<a id="webhooks"></a>
## webhooks

As seguintes informações estão disponíveis no objeto `webhooks` (este objeto está sempre disponível):

- `state`: retorna uma string indicando o estado atual do Klipper. Os valores possíveis são: "ready", "startup", "shutdown", "error".
- `state_message`: uma string legível por humanos dando contexto adicional sobre o estado atual do Klipper.

<a id="z_thermal_adjust"></a>
## z_thermal_adjust

As seguintes informações estão disponíveis no objeto `z_thermal_adjust` (este objeto está disponível se [z_thermal_adjust](Config_Reference.md#z_thermal_adjust) estiver definido).

- `enabled`: retorna True se o ajuste estiver ativado.
- `temperature`: temperatura atual (suavizada) do sensor definido. [degC]
- `measured_min_temp`: temperatura mínima medida. [degC]
- `measured_max_temp`: temperatura máxima medida. [degC]
- `current_z_adjust`: último ajuste Z calculado [mm].
- `z_adjust_ref_temperature`: temperatura de referência atual usada para cálculo do `current_z_adjust` de Z [degC].

<a id="z_tilt"></a>
## z_tilt

As seguintes informações estão disponíveis no objeto `z_tilt` (este objeto está disponível se z_tilt estiver definido):

- `applied`: True se o processo de nivelamento z-tilt tiver sido executado e concluído com sucesso.

<a id="accessing-coordinates"></a>
## Acessando coordenadas

Alguns campos de status fornecem uma "coordenada". Para usuários de macro, esses campos podem ser acessados pelo nome do componente (por exemplo, `{printer.toolhead.position.x}`), onde o nome do componente pode ser "x", "y" ou "z".

Para desenvolvedores que usam o Servidor de API do Klipper, esses campos são transmitidos como uma lista - por exemplo: `{"toolhead": {"position": [1.0, 2.0, 3.0, 7.3, 19.2]}}` . Os três primeiros componentes da lista correspondem aos eixos x, y e z.

Uma coordenada normalmente terá pelo menos 3 componentes (x, y e z), no entanto também pode haver componentes adicionais. Deve-se ter cuidado ao acessar qualquer um desses componentes adicionais, pois a ordem e o número de componentes podem mudar em tempo de execução.

Pode-se usar `{printer.gcode_move.axis_map}` e/ou `{printer.toolhead.extra_axes}` para determinar o número de componentes e a ordem dos componentes. Por exemplo, para acessar o componente "E" pode-se usar `{printer.toolhead.position[printer.gcode_move.axis_map.E]}`. Ou, se quiser encontrar o componente associado ao objeto "extruder", pode-se usar `{printer.toolhead.position[printer.toolhead.extra_axes.extruder]}`.
