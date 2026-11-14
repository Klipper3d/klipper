<a id="g-codes"></a>
# G-Codes

[English](../G-Codes.md) | **Português brasileiro**

Esse documento descreve os comandos que o Klipper suporta. Esses são comandos que você pode inserir no terminal OctoPrint.

<a id="g-code-commands"></a>
## Comandos G-Code

O Klipper suporta os seguintes comandos padrão de G-Code:

- Mover (G0 ou G1): `G1 [X<pos>] [Y<pos>] [Z<pos>] [E<pos>] [F<speed>]`
- Pausa: `G4 P<milliseconds>`
- Ir para a origem: `G28 [X] [Y] [Z]`
- Desligar motores: `M18` ou `M84`
- Aguardar a conclusão dos movimentos atuais: `M400`
- Usar coordenadas absolutas/relativas: `G90`, `G91`
- Forçar distâncias relativas para extrusão: `M83`, `M82`
   - Nota: `M83` força as extrusões a usar coordenadas relativas mesmo que o modo absoluto `G90` esteja selecionado. `M82` desativa esse modo forçado (não força a extrusora a usar coordenadas absolutas).
- Definir posição: `G92 [X<pos>] [Y<pos>] [Z<pos>] [E<pos>]`
- Definir percentual de substituição do fator de velocidade: `M220 S<percent>`
- Definir percentual de substituição do fator de extrusão: `M221 S<percent>`
- Definir aceleração: `M204 S<value>` OU `M204 P<value> T<value>`
   - Nota: se S não for especificado e tanto P quanto T forem especificados, a aceleração é definida para o mínimo de P e T. Se apenas um de P ou T for especificado, o comando não tem efeito.
- Obter temperatura da extrusora: `M105`
- Definir temperatura da extrusora: `M104 [T<index>] [S<temperature>]`
- Definir temperatura da extrusora e aguardar: `M109 [T<index>] S<temperature>`
   - Nota: M109 sempre aguarda a temperatura estabilizar no valor solicitado
- Definir temperatura da mesa: `M140 [S<temperature>]`
- Definir temperatura da mesa e aguardar: `M190 S<temperature>`
   - Nota: M190 sempre aguarda a temperatura estabilizar no valor solicitado
- Definir velocidade do ventilador: `M106 S<value>`
- Desligar ventilador: `M107`
- Parada de emergência: `M112`
- Obter posição atual: `M114`
- Obter versão do firmware: `M115`

Para mais detalhes sobre os comandos acima, consulte a [documentação G-Code do RepRap](http://reprap.org/wiki/G-code).

O objetivo do Klipper é suportar os comandos G-Code produzidos por softwares comuns de terceiros (por exemplo, OctoPrint, Printrun, Slic3r, Cura, etc.) em suas configurações padrão. Não é objetivo suportar todos os comandos G-Code possíveis. Em vez disso, o Klipper prefere ["comandos estendidos de G-Code"](#additional-commands) legíveis por humanos. Da mesma forma, a saída do terminal G-Code destina-se apenas a ser legível por humanos - consulte o [documento do Servidor de API](API_Server.md) se estiver controlando o Klipper a partir de um software externo.

Se for necessário um comando G-Code menos comum, pode ser possível implementá-lo com uma seção de configuração [gcode_macro](Config_Reference.md#gcode_macro) personalizada. Por exemplo, pode-se usar isso para implementar: `G12`, `G29`, `G30`, `G31`, `M42`, `M80`, `M81`, `T1`, etc.

<a id="additional-commands"></a>
## Comandos Adicionais

O Klipper usa comandos G-Code "estendidos" para configuração geral e status. Todos esses comandos estendidos seguem um formato semelhante - começam com um nome de comando e podem ser seguidos por um ou mais parâmetros. Por exemplo: `SET_SERVO SERVO=myservo ANGLE=5.3`. Neste documento, os comandos e parâmetros são mostrados em maiúsculas, porém não diferenciam maiúsculas de minúsculas. (Assim, "SET_SERVO" e "set_servo" executam o mesmo comando.)

Esta seção está organizada por nome de módulo do Klipper, que geralmente segue os nomes de seção especificados no [arquivo de configuração da impressora](Config_Reference.md). Observe que alguns módulos são carregados automaticamente.

<a id="adxl345"></a>
### [adxl345]

Os comandos a seguir estão disponíveis quando uma [seção de configuração adxl345](Config_Reference.md#adxl345) está habilitada.

<a id="accelerometer_measure"></a>
#### ACCELEROMETER_MEASURE

`ACCELEROMETER_MEASURE [CHIP=<config_name>] [NAME=<value>]`: inicia medições do acelerômetro no número solicitado de amostras por segundo. Se CHIP não for especificado, o padrão é "adxl345". O comando funciona em modo iniciar-parar: quando executado pela primeira vez, inicia as medições; na próxima execução, interrompe-as. Os resultados das medições são gravados em um arquivo chamado `/tmp/adxl345-<chip>-<name>.csv`, em que `<chip>` é o nome do chip do acelerômetro (`my_chip_name` de `[adxl345 my_chip_name]`) e `<name>` é o parâmetro NAME opcional. Se NAME não for especificado, o padrão é a hora atual no formato "YYYYMMDD_HHMMSS". Se o acelerômetro não tiver nome em sua seção de configuração (simplesmente `[adxl345]`), a parte `<chip>` do nome não é gerada.

<a id="accelerometer_query"></a>
#### ACCELEROMETER_QUERY

`ACCELEROMETER_QUERY [CHIP=<config_name>] [RATE=<value>]`: consulta o valor atual do acelerômetro. Se CHIP não for especificado, o padrão é "adxl345". Se RATE não for especificado, o valor padrão é usado. Este comando é útil para testar a conexão com o acelerômetro ADXL345: um dos valores retornados deve ser uma aceleração de queda livre (+/- algum ruído do chip).

<a id="accelerometer_debug_read"></a>
#### ACCELEROMETER_DEBUG_READ

`ACCELEROMETER_DEBUG_READ [CHIP=<config_name>] REG=<register>`: consulta o registrador "register" do ADXL345 (por exemplo, 44 ou 0x2C). Pode ser útil para depuração.

<a id="accelerometer_debug_write"></a>
#### ACCELEROMETER_DEBUG_WRITE

`ACCELEROMETER_DEBUG_WRITE [CHIP=<config_name>] REG=<register> VAL=<value>`: grava o "value" bruto em um registrador "register". Tanto "value" quanto "register" podem ser um inteiro decimal ou hexadecimal. Use com cuidado e consulte a folha de dados do ADXL345 como referência.

<a id="angle"></a>
### [angle]

Os comandos a seguir estão disponíveis quando uma [seção de configuração angle](Config_Reference.md#angle) está habilitada.

<a id="angle_calibrate"></a>
#### ANGLE_CALIBRATE

`ANGLE_CALIBRATE CHIP=<chip_name>`: realiza calibração de ângulo no sensor informado (deve haver uma seção de configuração `[angle chip_name]` que tenha especificado um parâmetro `stepper`). IMPORTANTE - esta ferramenta comandará o motor de passo a se mover sem verificar os limites cinemáticos normais. O ideal é que o motor seja desconectado de qualquer carro da impressora antes de realizar a calibração. Se o motor de passo não puder ser desconectado da impressora, certifique-se de que o carro esteja próximo ao centro de seu trilho antes de iniciar a calibração. (O motor de passo pode se mover para frente ou para trás duas rotações completas durante este teste.) Após concluir este teste, use o comando `SAVE_CONFIG` para salvar os dados de calibração no arquivo de configuração. Para usar esta ferramenta, o pacote Python "numpy" deve estar instalado (consulte o documento [medição de ressonância](Measuring_Resonances.md#software-installation) para mais informações).

<a id="angle_chip_calibrate"></a>
#### ANGLE_CHIP_CALIBRATE

`ANGLE_CHIP_CALIBRATE CHIP=<chip_name>`: realiza calibração interna do sensor, se implementada (MT6826S/MT6835).

- **MT68XX**: o motor deve ser desconectado de qualquer carro da impressora antes de realizar a calibração. Após a calibração, o sensor deve ser reiniciado desconectando a alimentação.

<a id="angle_debug_read"></a>
#### ANGLE_DEBUG_READ

`ANGLE_DEBUG_READ CHIP=<config_name> REG=<register>`: consulta o registrador "register" do sensor (por exemplo, 44 ou 0x2C). Pode ser útil para depuração. Disponível apenas para chips tle5012b.

<a id="angle_debug_write"></a>
#### ANGLE_DEBUG_WRITE

`ANGLE_DEBUG_WRITE CHIP=<config_name> REG=<register> VAL=<value>`: grava o "value" bruto no registrador "register". Tanto "value" quanto "register" podem ser um inteiro decimal ou hexadecimal. Use com cuidado e consulte a folha de dados do sensor como referência. Disponível apenas para chips tle5012b.

<a id="axis_twist_compensation"></a>
### [axis_twist_compensation]

Os comandos a seguir estão disponíveis quando a [seção de configuração axis_twist_compensation](Config_Reference.md#axis_twist_compensation) está habilitada.

<a id="axis_twist_compensation_calibrate"></a>
#### AXIS_TWIST_COMPENSATION_CALIBRATE

`AXIS_TWIST_COMPENSATION_CALIBRATE [AXIS=<X|Y>] [SAMPLE_COUNT=<value>] [MANUAL_METHOD=manual]`

Calibra a compensação de torção do eixo especificando o eixo de destino ou habilitando a calibração automática.

- **AXIS:** define o eixo (`X` ou `Y`) para o qual a compensação de torção será calibrada. Se não especificado, o eixo assume o padrão `'X'`.

<a id="bed_mesh"></a>
### [bed_mesh]

Os comandos a seguir estão disponíveis quando a [seção de configuração bed_mesh](Config_Reference.md#bed_mesh) está habilitada (consulte também o [guia de malha da mesa](Bed_Mesh.md)).

<a id="bed_mesh_calibrate"></a>
#### BED_MESH_CALIBRATE

`BED_MESH_CALIBRATE [PROFILE=<name>] [METHOD=manual] [HORIZONTAL_MOVE_Z=<value>] [<probe_parameter>=<value>] [<mesh_parameter>=<value>] [ADAPTIVE=1] [ADAPTIVE_MARGIN=<value>]`: este comando sonda a mesa usando pontos gerados especificados pelos parâmetros na configuração. Após a sondagem, uma malha é gerada e o movimento Z é ajustado conforme a malha. A malha fica imediatamente ativa após a conclusão bem-sucedida de `BED_MESH_CALIBRATE`. A malha será salva em um perfil especificado pelo parâmetro `PROFILE`, ou `default` se não especificado. Se ADAPTIVE=1 for especificado, o nome do perfil começará com `adaptive-` e não deve ser salvo para reutilização. Consulte o comando PROBE para detalhes sobre os parâmetros opcionais da sonda. Se METHOD=manual for especificado, a ferramenta de sondagem manual é ativada - consulte o comando MANUAL_PROBE acima para detalhes sobre os comandos adicionais disponíveis enquanto esta ferramenta estiver ativa. O valor opcional `HORIZONTAL_MOVE_Z` substitui a opção `horizontal_move_z` especificada no arquivo de configuração. Se ADAPTIVE=1 for especificado, os objetos definidos pelo arquivo Gcode impresso serão usados para definir a área sondada. O valor opcional `ADAPTIVE_MARGIN` substitui a opção `adaptive_margin` especificada no arquivo de configuração.

<a id="bed_mesh_output"></a>
#### BED_MESH_OUTPUT

`BED_MESH_OUTPUT PGP=[<0:1>]`: Este comando exibe os valores atuais de Z medidos e os valores atuais da malha no terminal. Se PGP=1 for especificado, as coordenadas X, Y geradas pelo bed_mesh, juntamente com seus índices associados, serão exibidas no terminal.

<a id="bed_mesh_map"></a>
#### BED_MESH_MAP

`BED_MESH_MAP`: Semelhante ao BED_MESH_OUTPUT, este comando imprime o estado atual da malha no terminal. Em vez de imprimir os valores em um formato legível para humanos, o estado é serializado no formato JSON. Isso permite que os plugins do OctoPrint capturem facilmente os dados e gerem mapas de altura que aproximam a superfície da cama.

<a id="bed_mesh_clear"></a>
#### BED_MESH_CLEAR

`BED_MESH_CLEAR`: Este comando apaga a malha e remove todos os ajustes em z. É recomendável colocar isto em seu end-gcode.

<a id="bed_mesh_profile"></a>
#### BED_MESH_PROFILE

`BED_MESH_PROFILE LOAD=<name> SAVE=<name> REMOVE=<name>`: Este comando fornece gerenciamento de perfil para o estado da malha. LOAD irá restaurar o estado da malha a partir do perfil correspondente ao nome fornecido. SAVE irá salvar o estado atual da malha em um perfil correspondente ao nome fornecido. REMOVE irá excluir o perfil correspondente ao nome fornecido da memória persistente. Observe que, após a execução das operações SAVE ou REMOVE, o comando SAVE_CONFIG deve ser executado para tornar as alterações permanentes na memória persistente.

<a id="bed_mesh_offset"></a>
#### BED_MESH_OFFSET

`BED_MESH_OFFSET [X=<value>] [Y=<value>] [ZFADE=<value]`: aplica deslocamentos X, Y e/ou ZFADE à consulta da malha. Isso é útil para impressoras com extrusoras independentes, pois um deslocamento é necessário para produzir o ajuste Z correto após uma troca de ferramenta. Observe que um deslocamento ZFADE não aplica ajuste z adicional diretamente, ele é usado para corrigir o cálculo de `fade` quando um `gcode offset` foi aplicado ao eixo Z.

<a id="bed_screws"></a>
### [bed_screws]

Os comandos a seguir estão disponíveis quando a [seção de configuração bed_screws](Config_Reference.md#bed_screws) está habilitada (consulte também o [guia de nivelamento manual](Manual_Level.md#adjusting-bed-leveling-screws)).

<a id="bed_screws_adjust"></a>
#### BED_SCREWS_ADJUST

`BED_SCREWS_ADJUST`: Este comando ativará a ferramenta de ajuste dos parafusos da cama. Ele comandará o bico para diferentes locais (conforme definido no arquivo de configuração) e permitirá fazer ajustes nos parafusos da cama para que ela mantenha uma distância constante do bico.

<a id="bed_tilt"></a>
### [bed_tilt]

Os comandos a seguir estão disponíveis quando a [seção de configuração bed_tilt](Config_Reference.md#bed_tilt) está habilitada.

<a id="bed_tilt_calibrate"></a>
#### BED_TILT_CALIBRATE

`BED_TILT_CALIBRATE [METHOD=manual] [HORIZONTAL_MOVE_Z=<value>] [<probe_parameter>=<value>]`: este comando sondará os pontos especificados na configuração e recomendará ajustes atualizados de inclinação x e y. Consulte o comando PROBE para detalhes sobre os parâmetros opcionais da sonda. Se METHOD=manual for especificado, a ferramenta de sondagem manual é ativada - consulte o comando MANUAL_PROBE acima para detalhes sobre os comandos adicionais disponíveis enquanto esta ferramenta estiver ativa. O valor opcional `HORIZONTAL_MOVE_Z` substitui a opção `horizontal_move_z` especificada no arquivo de configuração.

<a id="bltouch"></a>
### [bltouch]

O comando a seguir está disponível quando uma [seção de configuração bltouch](Config_Reference.md#bltouch) está habilitada (consulte também o [guia BL-Touch](BLTouch.md)).

<a id="bltouch_debug"></a>
#### BLTOUCH_DEBUG

`BLTOUCH_DEBUG COMMAND=<command>`: envia um comando ao BLTouch. Pode ser útil para depuração. Os comandos disponíveis são: `pin_down`, `touch_mode`, `pin_up`, `self_test`, `reset`. Um BL-Touch V3.0 ou V3.1 também pode suportar os comandos `set_5V_output_mode`, `set_OD_output_mode`, `output_mode_store`.

<a id="bltouch_store"></a>
#### BLTOUCH_STORE

`BLTOUCH_STORE MODE=<output_mode>`: armazena um modo de saída na EEPROM de um BLTouch V3.1. Os output_modes disponíveis são: `5V`, `OD`

<a id="configfile"></a>
### [configfile]

O módulo configfile é carregado automaticamente.

<a id="save_config"></a>
#### SAVE_CONFIG

`SAVE_CONFIG`: este comando sobrescreverá o arquivo principal de configuração da impressora e reiniciará o software hospedeiro. Este comando é usado em conjunto com outros comandos de calibração para armazenar os resultados dos testes de calibração.

<a id="delayed_gcode"></a>
### [delayed_gcode]

O comando a seguir está habilitado se uma [seção de configuração delayed_gcode](Config_Reference.md#delayed_gcode) tiver sido habilitada (consulte também o [guia de modelos](Command_Templates.md#delayed-gcodes)).

<a id="update_delayed_gcode"></a>
#### UPDATE_DELAYED_GCODE

`UPDATE_DELAYED_GCODE [ID=<name>] [DURATION=<seconds>]`: atualiza a duração do atraso para o [delayed_gcode] identificado e inicia o temporizador para execução do gcode. Um valor 0 cancelará a execução pendente de um delayed gcode.

<a id="delta_calibrate"></a>
### [delta_calibrate]

Os comandos a seguir estão disponíveis quando a [seção de configuração delta_calibrate](Config_Reference.md#linear-delta-kinematics) está habilitada (consulte também o [guia de calibração delta](Delta_Calibrate.md)).

<a id="delta_calibrate-1"></a>
#### DELTA_CALIBRATE

`DELTA_CALIBRATE [METHOD=manual] [HORIZONTAL_MOVE_Z=<value>] [<probe_parameter>=<value>]`: este comando sondará sete pontos na mesa e recomendará posições atualizadas de fim de curso, ângulos das torres e raio. Consulte o comando PROBE para detalhes sobre os parâmetros opcionais da sonda. Se METHOD=manual for especificado, a ferramenta de sondagem manual é ativada - consulte o comando MANUAL_PROBE acima para detalhes sobre os comandos adicionais disponíveis enquanto esta ferramenta estiver ativa. O valor opcional `HORIZONTAL_MOVE_Z` substitui a opção `horizontal_move_z` especificada no arquivo de configuração.

<a id="delta_analyze"></a>
#### DELTA_ANALYZE

`DELTA_ANALYZE`: este comando é usado durante a calibração aprimorada de delta. Consulte [Calibração de Delta](Delta_Calibrate.md) para detalhes.

<a id="display"></a>
### [display]

O comando a seguir está disponível quando uma [seção de configuração display](Config_Reference.md#gcode_macro) está habilitada.

<a id="set_display_group"></a>
#### SET_DISPLAY_GROUP

`SET_DISPLAY_GROUP [DISPLAY=<display>] GROUP=<group>`: define o grupo de exibição ativo de uma tela LCD. Isso permite definir vários grupos de dados de exibição na configuração, por exemplo, `[display_data <group> <elementname>]`, e alternar entre eles usando este comando estendido de gcode. Se DISPLAY não for especificado, o padrão é "display" (a tela principal).

<a id="display_status"></a>
### [display_status]

O módulo display_status é carregado automaticamente se uma [seção de configuração display](Config_Reference.md#display) estiver ativada. Ele fornece os seguintes comandos padrão de G-Code:

- Mensagem de exibição: `M117 <message>`
- Definir percentual de construção: `M73 P<percent>`

O seguinte comando G-Code estendido também é providenciado:

- `SET_DISPLAY_TEXT MSG=<message>`: Executa o equivalente a M117, definindo o `MSG` informado como a mensagem atual da tela. Se `MSG` for omitido, a tela será limpa.

<a id="dual_carriage"></a>
### [dual_carriage]

O comando a seguir está disponível quando a [seção de configuração dual_carriage](Config_Reference.md#dual_carriage) está habilitada.

<a id="set_dual_carriage"></a>
#### SET_DUAL_CARRIAGE

`SET_DUAL_CARRIAGE CARRIAGE=<carriage> [MODE=[PRIMARY|COPY|MIRROR|INACTIVE]]`: Este comando alterará o modo do carro especificado. Se nenhum `MODE` for informado, o padrão é `PRIMARY`. `<carriage>` deve referenciar um carro primário ou duplo definido para cinemática `generic_cartesian` ou ser 0 (para o carro primário) ou 1 (para o carro duplo) para todas as outras cinemáticas compatíveis com IDEX. Definir o modo como `PRIMARY` desativa todos os outros carros no mesmo eixo e faz o carro especificado executar os comandos de movimento G-Code subsequentes como estão. Antes de ativar o modo `COPY` ou `MIRROR` para um carro, um carro diferente deve ser ativado como `PRIMARY` no mesmo eixo. Quando definido para qualquer um desses dois modos, o carro acompanhará os movimentos G-Code subsequentes e copiará os movimentos relativos (no modo `COPY`) ou os executará na direção oposta (espelhada) (no modo `MIRROR`). Definir o modo como `INACTIVE` desativa o carro e faz com que ele ignore outros movimentos G-Code. Observe que desativar o carro primário no eixo não desabilita outros carros funcionando no modo `COPY` ou `MIRROR`, o que pode ser usado para desabilitar a impressão de uma peça com falha por qualquer uma das ferramentas e estacionar essa ferramenta para evitar colisões com uma peça inacabada, consulte esta [configuração de exemplo](../../config/sample-corexyuv.cfg) para exemplos de macros.

<a id="save_dual_carriage_state"></a>
#### SAVE_DUAL_CARRIAGE_STATE

`SAVE_DUAL_CARRIAGE_STATE [NAME=<state_name>]`: Salva as posições atuais dos carros duplos e seus modos. Salvar e restaurar o estado DUAL_CARRIAGE pode ser útil em scripts e macros, bem como em substituições da rotina de referenciamento (homing). Se NAME for informado, permite nomear o estado salvo com a string fornecida. Se NAME não for informado, o padrão é "default".

<a id="restore_dual_carriage_state"></a>
#### RESTORE_DUAL_CARRIAGE_STATE

`RESTORE_DUAL_CARRIAGE_STATE [NAME=<state_name>] [MOVE=[0|1] [MOVE_SPEED=<speed>]]`: Restaura os estados salvos anteriormente de todos os carros duplos e seus carros primários. Este comando restaura os modos dos carros e os move para suas posições salvas anteriormente, a menos que "MOVE=0" seja especificado. Se as posições estiverem sendo restauradas e "MOVE_SPEED" for especificado, então os carros se moverão com no máximo a velocidade informada (em mm/s); caso contrário, as velocidades de referenciamento (homing) dos carros correspondentes serão usadas como referência. Observe que os carros restauram suas posições somente sobre seus próprios eixos, o que pode ser necessário para restaurar corretamente os modos COPY e MIRROR do carro duplo. Além disso, este comando atualiza a posição do cabeçote do Klipper para cada eixo que possui carros duplos: ela é definida para corresponder à posição real do carro primário ativado de um eixo ou, se um eixo não tiver um carro primário salvo, para a posição do eixo quando o comando `SAVE_DUAL_CARRIAGE_STATE` foi chamado.

<a id="endstop_phase"></a>
### [endstop_phase]

Os comandos a seguir estão disponíveis quando uma [seção de configuração endstop_phase](Config_Reference.md#endstop_phase) está habilitada (consulte também o [guia de fase do fim de curso](Endstop_Phase.md)).

<a id="endstop_phase_calibrate"></a>
#### ENDSTOP_PHASE_CALIBRATE

`ENDSTOP_PHASE_CALIBRATE [STEPPER=<config_name>]`: Se nenhum parâmetro STEPPER for informado, este comando relatará estatísticas sobre as fases do motor de passo de fim de curso durante operações de referenciamento (homing) anteriores. Quando um parâmetro STEPPER for informado, ele providencia que a configuração de fase de fim de curso informada seja gravada no arquivo de configuração (em conjunto com o comando SAVE_CONFIG).

<a id="exclude_object"></a>
### [exclude_object]

Os comandos a seguir estão disponíveis quando uma [seção de configuração exclude_object](Config_Reference.md#exclude_object) está habilitada (consulte também o [guia de exclusão de objetos](Exclude_Object.md)):

<a id="exclude_object-1"></a>
#### `EXCLUDE_OBJECT`

`EXCLUDE_OBJECT [NAME=object_name] [CURRENT=1] [RESET=1]`: Sem parâmetros, isto retornará uma lista de todos os objetos atualmente excluídos.

Quando o parâmetro `NAME` é informado, o objeto nomeado será excluído da impressão.

Quando o parâmetro `CURRENT` é informado, o objeto atual será excluído da impressão.

Quando o parâmetro `RESET` é informado, a lista de objetos excluídos será limpa. Incluir adicionalmente `NAME` redefinirá apenas o objeto nomeado. Isso **pode** causar falhas de impressão, se camadas já tiverem sido puladas.

<a id="exclude_object_define"></a>
#### `EXCLUDE_OBJECT_DEFINE`

`EXCLUDE_OBJECT_DEFINE [NAME=object_name [CENTER=X,Y] [POLYGON=[[x,y],...]] [RESET=1] [JSON=1]`: Fornece um resumo de um objeto no arquivo.

Sem parâmetros fornecidos, isso listará os objetos definidos conhecidos pelo Klipper. Retorna uma lista de strings, a menos que o parâmetro `JSON` seja informado, quando retornará detalhes dos objetos em formato json.

Quando o parâmetro `NAME` é incluído, isso define um objeto a ser excluído.

- `NAME`: este parâmetro é obrigatório. É o identificador usado por outros comandos neste módulo.
- `CENTER`: uma coordenada X,Y para o objeto.
- `POLYGON`: uma matriz de coordenadas X,Y que fornecem um contorno para o objeto.

Quando o parâmetro `RESET` é fornecido, todos os objetos definidos serão apagados e o módulo `[exclude_object]` será redefinido.

<a id="exclude_object_start"></a>
#### `EXCLUDE_OBJECT_START`

`EXCLUDE_OBJECT_START NAME=object_name`: Este comando recebe um parâmetro `NAME` e indica o início do gcode de um objeto na camada atual.

<a id="exclude_object_end"></a>
#### `EXCLUDE_OBJECT_END`

`EXCLUDE_OBJECT_END [NAME=object_name]`: Indica o fim do gcode do objeto para a camada. É pareado com `EXCLUDE_OBJECT_START`. Um parâmetro `NAME` é opcional e apenas emitirá um aviso quando o nome informado não corresponder ao objeto atual.

<a id="extruder"></a>
### [extruder]

Os comandos a seguir estão disponíveis se uma [seção de configuração extruder](Config_Reference.md#extruder) estiver habilitada:

<a id="activate_extruder"></a>
#### ACTIVATE_EXTRUDER

`ACTIVATE_EXTRUDER EXTRUDER=<config_name>`: Em uma impressora com várias seções de configuração [extrusora](Config_Reference.md#extruder), este comando troca o hotend ativo.

<a id="set_pressure_advance"></a>
#### SET_PRESSURE_ADVANCE

`SET_PRESSURE_ADVANCE [EXTRUDER=<config_name>] [ADVANCE=<pressure_advance>] [SMOOTH_TIME=<pressure_advance_smooth_time>]`: Define os parâmetros de pressure advance de um motor de passo da extrusora (conforme definido em uma seção de configuração [extrusora](Config_Reference.md#extruder) ou [extruder_stepper](Config_Reference.md#extruder_stepper)). Se EXTRUDER não for especificado, o padrão é o motor de passo definido no hotend ativo.

<a id="set_extruder_rotation_distance"></a>
#### SET_EXTRUDER_ROTATION_DISTANCE

`SET_EXTRUDER_ROTATION_DISTANCE EXTRUDER=<config_name> [DISTANCE=<distance>]`: Define um novo valor para a "rotation distance" do motor de passo da extrusora informado (conforme definido em uma seção de configuração [extrusora](Config_Reference.md#extruder) ou [extruder_stepper](Config_Reference.md#extruder_stepper)). Se a rotation distance for um número negativo, então o movimento do motor de passo será invertido (em relação à direção do motor de passo especificada no arquivo de configuração). Configurações alteradas não são mantidas ao reiniciar o Klipper. Use com cautela, pois pequenas alterações podem resultar em pressão excessiva entre a extrusora e o hotend. Faça a calibração adequada com filamento antes de usar. Se o valor 'DISTANCE' não for informado, então este comando retornará a rotation distance atual.

<a id="sync_extruder_motion"></a>
#### SYNC_EXTRUDER_MOTION

`SYNC_EXTRUDER_MOTION EXTRUDER=<name> MOTION_QUEUE=<name>`: Este comando fará com que o motor de passo especificado por EXTRUDER (conforme definido em uma seção de configuração [extrusora](Config_Reference.md#extruder) ou [extruder_stepper](Config_Reference.md#extruder_stepper)) fique sincronizado ao movimento de uma extrusora especificada por MOTION_QUEUE (conforme definido em uma seção de configuração [extrusora](Config_Reference.md#extruder)). Se MOTION_QUEUE for uma string vazia, então o motor de passo será dessincronizado de todo movimento da extrusora.

<a id="fan_generic"></a>
### [fan_generic]

O comando a seguir está disponível quando uma [seção de configuração fan_generic](Config_Reference.md#fan_generic) está habilitada.

<a id="set_fan_speed"></a>
#### SET_FAN_SPEED

`SET_FAN_SPEED FAN=config_name SPEED=<speed>` Este comando define a velocidade de um ventilador. "speed" deve estar entre 0.0 e 1.0.

`SET_FAN_SPEED FAN=config_name TEMPLATE=<template_name> [<param_x>=<literal>]`: se `TEMPLATE` for especificado, ele atribui um [display_template](Config_Reference.md#display_template) ao ventilador informado. Por exemplo, se foi definida uma seção de configuração `[display_template my_fan_template]`, pode-se atribuir `TEMPLATE=my_fan_template` aqui. O display_template deve produzir uma string contendo um número de ponto flutuante com o valor desejado. O modelo será avaliado continuamente e o ventilador será ajustado automaticamente para a velocidade resultante. Pode-se definir parâmetros do display_template a serem usados durante a avaliação do modelo (os parâmetros serão analisados como literais Python). Se TEMPLATE for uma string vazia, este comando limpará qualquer modelo atribuído anteriormente ao pino (pode-se então usar comandos `SET_FAN_SPEED` para gerenciar os valores diretamente).

<a id="filament_switch_sensor"></a>
### [filament_switch_sensor]

O comando a seguir está disponível quando uma seção de configuração [filament_switch_sensor](Config_Reference.md#filament_switch_sensor) ou [filament_motion_sensor](Config_Reference.md#filament_motion_sensor) está habilitada.

<a id="query_filament_sensor"></a>
#### QUERY_FILAMENT_SENSOR

`QUERY_FILAMENT_SENSOR SENSOR=<sensor_name>`: Consulta o status atual do sensor de filamento. Os dados exibidos no terminal dependerão do tipo de sensor definido na configuração.

<a id="set_filament_sensor"></a>
#### SET_FILAMENT_SENSOR

`SET_FILAMENT_SENSOR SENSOR=<sensor_name> ENABLE=[0|1]`: Liga/desliga o sensor de filamento. Se ENABLE for definido como 0, o sensor de filamento será desabilitado; se definido como 1, estará habilitado.

<a id="firmware_retraction"></a>
### [firmware_retraction]

Os seguintes comandos G-Code padrão estão disponíveis quando a [seção de configuração firmware_retraction](Config_Reference.md#firmware_retraction) está habilitada. Estes comandos permitem utilizar o recurso de retirada por firmware disponível em muitos fatiadores, para reduzir fios (stringing) durante movimentos sem extrusão de uma parte da impressão para outra. Configurar adequadamente o pressure advance reduz o comprimento de retirada necessário.

- `G10`: Retrai a extrusora usando os parâmetros configurados atualmente.
- `G11`: Desretrai a extrusora usando os parâmetros configurados atualmente.

Os comandos adicionais a seguir também estão disponíveis.

<a id="set_retraction"></a>
#### SET_RETRACTION

`SET_RETRACTION [RETRACT_LENGTH=<mm>] [RETRACT_SPEED=<mm/s>] [UNRETRACT_EXTRA_LENGTH=<mm>] [UNRETRACT_SPEED=<mm/s>]`: Ajusta os parâmetros usados pela retirada por firmware. RETRACT_LENGTH determina o comprimento de filamento a retrair e desretrair. A velocidade de retirada é ajustada via RETRACT_SPEED e normalmente é definida de forma relativamente alta. A velocidade de desretração é ajustada via UNRETRACT_SPEED e não é particularmente crítica, embora frequentemente menor que RETRACT_SPEED. Em alguns casos é útil adicionar uma pequena quantidade de comprimento adicional na desretração, e isto é definido via UNRETRACT_EXTRA_LENGTH. SET_RETRACTION é comumente definido como parte da configuração por filamento do fatiador, pois filamentos diferentes exigem configurações de parâmetros diferentes.

<a id="get_retraction"></a>
#### GET_RETRACTION

`GET_RETRACTION`: Consulta os parâmetros atuais usados pela retirada por firmware e os exibe no terminal.

<a id="force_move"></a>
### [force_move]

O módulo force_move é carregado automaticamente, porém alguns comandos exigem definir `enable_force_move` na [configuração da impressora](Config_Reference.md#force_move).

<a id="stepper_buzz"></a>
#### STEPPER_BUZZ

`STEPPER_BUZZ STEPPER=<config_name>`: Move o motor de passo informado um mm para frente e depois um mm para trás, repetido 10 vezes. Esta é uma ferramenta de diagnóstico para ajudar a verificar a conectividade do motor de passo.

<a id="force_move-1"></a>
#### FORCE_MOVE

`FORCE_MOVE STEPPER=<config_name> DISTANCE=<value> VELOCITY=<value> [ACCEL=<value>]`: Este comando moverá à força o motor de passo informado pela distância informada (em mm) na velocidade constante informada (em mm/s). Se ACCEL for especificado e for maior que zero, então a aceleração informada (em mm/s^2) será usada; caso contrário, nenhuma aceleração será executada. Nenhuma verificação de limites é executada; nenhuma atualização cinemática é feita; outros motores de passo paralelos em um eixo não serão movidos. Tenha cautela, pois um comando incorreto pode causar danos! Usar este comando quase certamente colocará a cinemática de baixo nível em um estado incorreto; emita um G28 depois para redefinir a cinemática. Este comando destina-se a diagnósticos e depuração de baixo nível.

<a id="set_kinematic_position"></a>
#### SET_KINEMATIC_POSITION

`SET_KINEMATIC_POSITION [X=<value>] [Y=<value>] [Z=<value>] [SET_HOMED=<[X][Y][Z]>] [CLEAR_HOMED=<[X][Y][Z]>]`: força o código cinemático de baixo nível a considerar que o cabeçote está na posição cartesiana informada e define/limpa o status de homing. Este é um comando de diagnóstico e depuração; use SET_GCODE_OFFSET e/ou G92 para transformações regulares de eixos. Definir uma posição incorreta ou inválida pode levar a erros internos de software.

Os parâmetros `X`, `Y` e `Z` são usados para alterar o rastreamento de posição cinemática de baixo nível. Se algum desses parâmetros não for definido, a posição não é alterada - por exemplo, `SET_KINEMATIC_POSITION Z=10` definiria todos os eixos como com homing feito, definiria a posição interna Z como 10 e deixaria as posições X e Y inalteradas. Alterar o rastreamento de posição interna não depende do estado interno de homing - pode-se alterar a posição tanto para eixos com homing feito quanto não, e da mesma forma pode-se definir ou limpar o estado de homing de um eixo sem alterar sua posição interna.

O parâmetro `SET_HOMED` assume o padrão `XYZ`, o que instrui a cinemática a considerar todos os eixos como com homing feito. Um comando `SET_KINEMATIC_POSITION` puro resultará em todos os eixos sendo considerados com homing feito (e sem alterar sua posição atual). Se não for desejado alterar o estado dos eixos com homing, atribua `SET_HOMED` a uma string vazia - por exemplo: `SET_KINEMATIC_POSITION SET_HOMED= X=10`. Também é possível solicitar que um eixo individual seja considerado com homing (por exemplo, `SET_HOMED=X`), mas observe que cinemáticas de estilo não cartesiano (como a cinemática delta) podem não suportar definir um eixo individual como com homing.

O parâmetro `CLEAR_HOMED` instrui a cinemática a considerar os eixos informados como sem homing. Por exemplo, `CLEAR_HOMED=XYZ` solicitaria que todos os eixos fossem considerados sem homing (exigindo assim homing antes do movimento nesses eixos). O padrão é `SET_HOMED=XYZ` mesmo se `CLEAR_HOMED` estiver presente, portanto o comando `SET_KINEMATIC_POSITION CLEAR_HOMED=Z` definirá X e Y como com homing e limpará o estado de homing de Z. Use `SET_KINEMATIC_POSITION SET_HOMED= CLEAR_HOMED=Z` se o objetivo for limpar apenas o estado de homing de Z. Se um eixo não for especificado nem em `SET_HOMED` nem em `CLEAR_HOMED`, seu estado de homing não é alterado, e se for especificado em ambos, `CLEAR_HOMED` tem precedência. É possível solicitar a limpeza de um eixo individual, mas em cinemáticas de estilo não cartesiano (como a cinemática delta) isso pode resultar na limpeza do estado de homing de eixos adicionais. Observe que o parâmetro `CLEAR` atualmente é um apelido para o parâmetro `CLEAR_HOMED`, mas esse apelido será removido no futuro.

<a id="gcode"></a>
### [gcode]

O módulo gcode é carregado automaticamente.

<a id="restart"></a>
#### RESTART

`RESTART`: Isto fará com que o software hospedeiro recarregue sua configuração e execute uma reinicialização interna. Este comando não limpará o estado de erro do microcontrolador (consulte FIRMWARE_RESTART) nem carregará novo software (consulte [o FAQ](FAQ.md#how-do-i-upgrade-to-the-latest-software)).

<a id="firmware_restart"></a>
#### FIRMWARE_RESTART

`FIRMWARE_RESTART`: Isto é semelhante a um comando RESTART, mas também limpa qualquer estado de erro do microcontrolador.

<a id="status"></a>
#### STATUS

`STATUS`: Relata o status do software hospedeiro Klipper.

<a id="help"></a>
#### HELP

`HELP`: Relata a lista de comandos G-Code estendidos disponíveis.

<a id="gcode_arcs"></a>
### [gcode_arcs]

Os seguintes comandos G-Code padrão estão disponíveis se uma [seção de configuração gcode_arcs](Config_Reference.md#gcode_arcs) estiver habilitada:

- Movimento em arco horário (G2), Movimento em arco anti-horário (G3): `G2|G3 [X<pos>] [Y<pos>] [Z<pos>] [E<pos>] [F<speed>] I<value> J<value>|I<value> K<value>|J<value> K<value>`
- Seleção de Plano do Arco: G17 (plano XY), G18 (plano XZ), G19 (plano YZ)

<a id="gcode_macro"></a>
### [gcode_macro]

O comando a seguir está disponível quando uma [seção de configuração gcode_macro](Config_Reference.md#gcode_macro) está habilitada (consulte também o [guia de modelos de comando](Command_Templates.md)).

<a id="set_gcode_variable"></a>
#### SET_GCODE_VARIABLE

`SET_GCODE_VARIABLE MACRO=<macro_name> VARIABLE=<name> VALUE=<value>`: Este comando permite alterar o valor de uma variável gcode_macro em tempo de execução. O VALUE informado é analisado como um literal Python.

<a id="gcode_move"></a>
### [gcode_move]

O módulo gcode_move é carregado automaticamente.

<a id="get_position"></a>
#### GET_POSITION

`GET_POSITION`: Retorna informações sobre a localização atual do cabeçote. Consulte a documentação de desenvolvedor sobre [saída de GET_POSITION](Code_Overview.md#coordinate-systems) para mais informações.

<a id="set_gcode_offset"></a>
#### SET_GCODE_OFFSET

`SET_GCODE_OFFSET [X=<pos>|X_ADJUST=<adjust>] [Y=<pos>|Y_ADJUST=<adjust>] [Z=<pos>|Z_ADJUST=<adjust>] [MOVE=1 [MOVE_SPEED=<speed>]]`: Define um deslocamento (offset) posicional a ser aplicado a futuros comandos G-Code. Isto é comumente usado para alterar virtualmente o deslocamento (offset) Z da mesa ou para definir deslocamentos (offsets) XY do bico ao trocar de extrusora. Por exemplo, se "SET_GCODE_OFFSET Z=0.2" for enviado, então futuros movimentos G-Code terão 0.2mm adicionados à sua altura Z. Se os parâmetros no estilo X_ADJUST forem usados, então o ajuste será adicionado a qualquer deslocamento (offset) existente (por ex., "SET_GCODE_OFFSET Z=-0.2" seguido de "SET_GCODE_OFFSET Z_ADJUST=0.3" resultaria em um deslocamento (offset) Z total de 0.1). Se "MOVE=1" for especificado, então um movimento do cabeçote será emitido para aplicar o deslocamento (offset) informado (caso contrário, o deslocamento terá efeito no próximo movimento G-Code absoluto que especificar o eixo informado). Se "MOVE_SPEED" for especificado, então o movimento do cabeçote será executado com a velocidade informada (em mm/s); caso contrário, o movimento do cabeçote usará a última velocidade G-Code especificada.

<a id="save_gcode_state"></a>
#### SAVE_GCODE_STATE

`SAVE_GCODE_STATE [NAME=<state_name>]`: Salva o estado atual de análise de coordenadas do g-code. Salvar e restaurar o estado do g-code é útil em scripts e macros. Este comando salva o modo de coordenadas absolutas do g-code atual (G90/G91), o modo forçado de extrusão relativa (M83/M82), a origem (G92), o deslocamento (offset) (SET_GCODE_OFFSET), a substituição de velocidade (M220), a substituição da extrusora (M221), a velocidade de movimento, a posição XYZ atual e a posição relativa "E" da extrusora. Se NAME for informado, permite nomear o estado salvo com a string fornecida. Se NAME não for informado, o padrão é "default".

<a id="restore_gcode_state"></a>
#### RESTORE_GCODE_STATE

`RESTORE_GCODE_STATE [NAME=<state_name>] [MOVE=1 [MOVE_SPEED=<speed>]]`: Restaura um estado salvo anteriormente via SAVE_GCODE_STATE. Se "MOVE=1" for especificado, então um movimento do cabeçote será emitido para mover de volta à posição XYZ anterior. Se "MOVE_SPEED" for especificado, então o movimento do cabeçote será executado com a velocidade informada (em mm/s); caso contrário, o movimento do cabeçote usará a velocidade g-code restaurada.

<a id="generic_cartesian"></a>
### [generic_cartesian]

Os comandos nesta seção tornam-se disponíveis automaticamente quando `kinematics: generic_cartesian` é especificado como a cinemática da impressora.

<a id="set_stepper_carriages"></a>
#### SET_STEPPER_CARRIAGES

`SET_STEPPER_CARRIAGES STEPPER=<stepper_name> CARRIAGES=<carriages> [DISABLE_CHECKS=[0|1]]`: Define ou atualiza os carros do motor de passo. `<stepper_name>` deve referenciar um motor de passo existente definido em `printer.cfg`, e `<carriages>` descreve os carros que o motor de passo move. Consulte [Cinemática cartesiana genérica](Config_Reference.md#generic-cartesian-kinematics) para uma visão mais detalhada do parâmetro `carriages` na seção de configuração do motor de passo. Observe que só é possível alterar os coeficientes ou sinais dos carros com este comando, mas um usuário não pode adicionar ou remover os carros que o motor de passo controla.

`SET_STEPPER_CARRIAGES` é uma ferramenta avançada, e recomenda-se que o usuário tenha extremo cuidado ao usá-la, pois especificar uma configuração incorreta pode danificar fisicamente a impressora.

Observe que `SET_STEPPER_CARRIAGES` realiza certas validações internas da nova cinemática da impressora após a alteração. Tenha em mente que, se detectar um problema, pode deixar a cinemática da impressora em um estado inválido. Isso significa que, se `SET_STEPPER_CARRIAGES` relatar um erro, não é seguro emitir outros comandos GCode, e o usuário deve inspecionar a mensagem de erro e corrigir o problema ou restaurar manualmente a configuração anterior do(s) motor(es) de passo.

Como `SET_STEPPER_CARRIAGES` pode atualizar a configuração de um único motor de passo por vez, algumas sequências de alterações podem levar a configurações cinemáticas intermediárias inválidas, mesmo que a configuração final seja válida. Nesses casos, o usuário pode passar o parâmetro `DISABLE_CHECKS=1` para todos os comandos exceto o último para desabilitar verificações intermediárias. Por exemplo, se `stepper a` e `stepper b` tiverem inicialmente os carros `carriage_x-carriage_y` e `carriage_x+carriage_y` respectivamente, a seguinte sequência de comandos permitirá ao usuário efetivamente trocar os controles dos carros: `SET_STEPPER_CARRIAGES STEPPER=a CARRIAGES=carriage_x+carriage_y DISABLE_CHECKS=1` e `SET_STEPPER_CARRIAGES STEPPER=b CARRIAGES=carriage_x-carriage_y`, ainda validando o estado cinemático final.

<a id="hall_filament_width_sensor"></a>
### [hall_filament_width_sensor]

Os comandos a seguir estão disponíveis quando a [seção de configuração do sensor de largura do filamento tsl1401cl](Config_Reference.md#tsl1401cl_filament_width_sensor) ou a [seção de configuração do sensor de largura do filamento hall](Config_Reference.md#hall_filament_width_sensor) está habilitada (consulte também [Sensor de largura do filamento TSLl401CL](TSL1401CL_Filament_Width_Sensor.md) e [Sensor de largura do filamento Hall](Hall_Filament_Width_Sensor.md)):

<a id="query_filament_width"></a>
#### QUERY_FILAMENT_WIDTH

`QUERY_FILAMENT_WIDTH`: Retorna a largura do filamento medida atualmente, o estado do sensor de largura, o estado do sensor de filamento e o estado da compensação de fluxo.

<a id="reset_filament_width_sensor"></a>
#### RESET_FILAMENT_WIDTH_SENSOR

`RESET_FILAMENT_WIDTH_SENSOR`: Limpa todas as leituras do sensor. Útil após a troca de filamento. Redefine a taxa de fluxo para 100%.

<a id="disable_filament_width_sensor"></a>
#### DISABLE_FILAMENT_WIDTH_SENSOR

`DISABLE_FILAMENT_WIDTH_SENSOR`: Desliga o sensor de largura do filamento e para de usá-lo para compensação de fluxo. Redefine a taxa de fluxo para 100%.

<a id="enable_filament_width_sensor"></a>
#### ENABLE_FILAMENT_WIDTH_SENSOR

`ENABLE_FILAMENT_WIDTH_SENSOR [FLOW_COMPENSATION=[0|1]`: Liga o sensor de largura do filamento e habilita ou desabilita a compensação de fluxo. Se `FLOW_COMPENSATION` não for especificado, o estado atual da compensação de fluxo é preservado.

<a id="query_raw_filament_width"></a>
#### QUERY_RAW_FILAMENT_WIDTH

`QUERY_RAW_FILAMENT_WIDTH`: Retorna as leituras atuais do canal ADC e o valor RAW do sensor para pontos de calibração.

<a id="enable_filament_width_log"></a>
#### ENABLE_FILAMENT_WIDTH_LOG

`ENABLE_FILAMENT_WIDTH_LOG`: Ativa o registro (log) de diâmetro.

<a id="disable_filament_width_log"></a>
#### DISABLE_FILAMENT_WIDTH_LOG

`DISABLE_FILAMENT_WIDTH_LOG`: Desativa o registro (log) de diâmetro.

<a id="heaters"></a>
### [heaters]

O módulo heaters é carregado automaticamente se um aquecedor estiver definido no arquivo de configuração.

<a id="turn_off_heaters"></a>
#### TURN_OFF_HEATERS

`TURN_OFF_HEATERS`: Desliga todos os aquecedores.

<a id="temperature_wait"></a>
#### TEMPERATURE_WAIT

`TEMPERATURE_WAIT SENSOR=<config_name> [MINIMUM=<target>] [MAXIMUM=<target>]`: Aguarda até que o sensor de temperatura informado esteja em ou acima do MINIMUM fornecido e/ou em ou abaixo do MAXIMUM fornecido.

<a id="set_heater_temperature"></a>
#### SET_HEATER_TEMPERATURE

`SET_HEATER_TEMPERATURE HEATER=<heater_name> [TARGET=<target_temperature>]`: Define a temperatura-alvo para um aquecedor. Se uma temperatura-alvo não for informada, a meta é 0.

<a id="idle_timeout"></a>
### [idle_timeout]

O módulo idle_timeout é carregado automaticamente.

<a id="set_idle_timeout"></a>
#### SET_IDLE_TIMEOUT

`SET_IDLE_TIMEOUT [TIMEOUT=<timeout>]`: Permite ao usuário definir o tempo limite de inatividade (em segundos).

<a id="input_shaper"></a>
### [input_shaper]

O comando a seguir está habilitado se uma [seção de configuração input_shaper](Config_Reference.md#input_shaper) tiver sido habilitada (consulte também o [guia de compensação de ressonância](Resonance_Compensation.md)).

<a id="set_input_shaper"></a>
#### SET_INPUT_SHAPER

`SET_INPUT_SHAPER [SHAPER_FREQ_X=<shaper_freq_x>] [SHAPER_FREQ_Y=<shaper_freq_y>] [SHAPER_FREQ_Y=<shaper_freq_z>] [DAMPING_RATIO_X=<damping_ratio_x>] [DAMPING_RATIO_Y=<damping_ratio_y>] [DAMPING_RATIO_Z=<damping_ratio_z>] [SHAPER_TYPE=<shaper>] [SHAPER_TYPE_X=<shaper_type_x>] [SHAPER_TYPE_Y=<shaper_type_y>] [SHAPER_TYPE_Z=<shaper_type_z>]`: Modifica os parâmetros do input shaper. Observe que o parâmetro SHAPER_TYPE redefine o input shaper para todos os eixos mesmo que tipos de shaper diferentes tenham sido configurados na seção [input_shaper]. SHAPER_TYPE não pode ser usado junto com nenhum dos parâmetros SHAPER_TYPE_X, SHAPER_TYPE_Y e SHAPER_TYPE_Z. Consulte [referência de configuração](Config_Reference.md#input_shaper) para mais detalhes sobre cada um desses parâmetros.

<a id="led"></a>
### [led]

O comando a seguir está disponível quando qualquer uma das [seções de configuração led](Config_Reference.md#leds) está habilitada.

<a id="set_led"></a>
#### SET_LED

`SET_LED LED=<config_name> RED=<value> GREEN=<value> BLUE=<value> WHITE=<value> [INDEX=<index>] [TRANSMIT=0] [SYNC=1]`: Isto define a saída do LED. Cada `<value>` de cor deve estar entre 0.0 e 1.0. A opção WHITE é válida somente em LEDs RGBW. Se o LED suportar vários chips em cadeia (daisy-chain), pode-se especificar INDEX para alterar a cor apenas do chip informado (1 para o primeiro chip, 2 para o segundo, etc.). Se INDEX não for informado, então todos os LEDs na cadeia (daisy-chain) serão definidos com a cor informada. Se TRANSMIT=0 for especificado, então a alteração de cor só será feita no próximo comando SET_LED que não especificar TRANSMIT=0; isto pode ser útil em combinação com o parâmetro INDEX para agrupar múltiplas atualizações em uma cadeia (daisy-chain). Por padrão, o comando SET_LED sincronizará suas alterações com outros comandos gcode em andamento. Isto pode levar a comportamento indesejável se os LEDs estiverem sendo definidos enquanto a impressora não está imprimindo, pois redefinirá o tempo limite de inatividade. Se uma temporização cuidadosa não for necessária, o parâmetro opcional SYNC=0 pode ser especificado para aplicar as alterações sem redefinir o tempo limite de inatividade.

<a id="set_led_template"></a>
#### SET_LED_TEMPLATE

`SET_LED_TEMPLATE LED=<led_name> TEMPLATE=<template_name> [<param_x>=<literal>] [INDEX=<index>]`: Atribui um [display_template](Config_Reference.md#display_template) a um [LED](Config_Reference.md#leds) informado. Por exemplo, se foi definida uma seção de configuração `[display_template my_led_template]`, então pode-se atribuir `TEMPLATE=my_led_template` aqui. O display_template deve produzir uma string separada por vírgulas contendo quatro números de ponto flutuante correspondentes às configurações de cor vermelha, verde, azul e branca. O modelo (template) será avaliado continuamente e o LED será automaticamente definido com as cores resultantes. Pode-se definir parâmetros do display_template para usar durante a avaliação do modelo (os parâmetros serão analisados como literais Python). Se INDEX não for especificado, então todos os chips na cadeia (daisy-chain) do LED serão definidos com o modelo; caso contrário, somente o chip com o índice informado será atualizado. Se TEMPLATE for uma string vazia, então este comando limpará qualquer modelo anterior atribuído ao LED (pode-se então usar comandos `SET_LED` para gerenciar as configurações de cor do LED).

<a id="load_cell"></a>
### [load_cell]

Os comandos a seguir estão habilitados se uma [seção de configuração load_cell](Config_Reference.md#load_cell) tiver sido habilitada.

<a id="load_cell_diagnostic"></a>
### LOAD_CELL_DIAGNOSTIC

`LOAD_CELL_DIAGNOSTIC [LOAD_CELL=<config_name>]`: Este comando coleta 10 segundos de dados da célula de carga e relata estatísticas que podem ajudar a verificar a operação adequada da célula de carga. Este comando pode ser executado tanto em células de carga calibradas quanto não calibradas.

<a id="load_cell_calibrate"></a>
### LOAD_CELL_CALIBRATE

`LOAD_CELL_CALIBRATE [LOAD_CELL=<config_name>]`: Inicia o utilitário de calibração guiada. A calibração é um processo em 3 etapas:

1. Primeiro remova toda a carga da célula de carga e execute o comando `TARE`
1. Em seguida aplique uma carga conhecida à célula de carga e execute o comando `CALIBRATE GRAMS=nnn`
1. Por fim use o comando `ACCEPT` para salvar os resultados

Você pode cancelar o processo de calibração a qualquer momento com `ABORT`.

<a id="load_cell_tare"></a>
### LOAD_CELL_TARE

`LOAD_CELL_TARE [LOAD_CELL=<config_name>]`: Isto funciona exatamente como o botão de tara em uma balança digital. Define a leitura bruta atual da célula de carga como o valor de referência do ponto zero. A resposta é a porcentagem da faixa do sensor que foi lida e o valor bruto em contagens. Se a célula de carga estiver calibrada, uma força em gramas também é relatada.

<a id="load_cell_read-load_cellname"></a>
### LOAD_CELL_READ load_cell="name"

`LOAD_CELL_READ [LOAD_CELL=<config_name>]`: Este comando faz uma leitura da célula de carga. A resposta é a porcentagem da faixa do sensor que foi lida e o valor bruto em contagens. Se a célula de carga estiver calibrada, uma força em gramas também é relatada.

<a id="load_cell_probe"></a>
### [load_cell_probe]

Os comandos abaixo estão habilitados se uma [seção de configuração load_cell](Config_Reference.md#load_cell_probe) tiver sido habilitada.

Além disso, comandos que executam sondagens, como [`PROBE`](#probe), [`PROBE_ACCURACY`](#probe_accuracy), [`BED_MESH_CALIBRATE`](#bed_mesh_calibrate) etc. aceitarão parâmetros adicionais se um `[load_cell_probe]` estiver definido. Os parâmetros substituem as configurações correspondentes da configuração [`[load_cell_probe]`](Config_Reference.md#load_cell_probe):

- `FORCE_SAFETY_LIMIT=<grams>`
- `TRIGGER_FORCE=<grams>`
- `DRIFT_FILTER_CUTOFF_FREQUENCY=<frequency_hz>`
- `DRIFT_FILTER_DELAY=<1|2>`
- `BUZZ_FILTER_CUTOFF_FREQUENCY=<frequency_hz>`
- `BUZZ_FILTER_DELAY=<1|2>`
- `NOTCH_FILTER_FREQUENCIES=<list of frequency_hz>`
- `NOTCH_FILTER_QUALITY=<quality>`
- `TARE_TIME=<seconds>`

<a id="load_cell_test_tap"></a>
### LOAD_CELL_TEST_TAP

`LOAD_CELL_TEST_TAP [TAPS=<taps>] [TIMEOUT=<timeout>]`: Executa uma rotina de teste que relata toques na célula de carga. O cabeçote não se moverá, mas a sonda de célula de carga detectará toques como se estivesse sondando. Isto pode ser usado como verificação de sanidade para garantir que a sonda funciona. Esta ferramenta substitui QUERY_ENDSTOPS e QUERY_PROBE para sondas de célula de carga.

- `TAPS`: o número de toques que a ferramenta espera
- `TIMEOOUT`: o tempo, em segundos, que a ferramenta aguarda por cada toque antes de abortar.

<a id="manual_probe"></a>
### [manual_probe]

O módulo manual_probe é carregado automaticamente.

<a id="manual_probe-1"></a>
#### MANUAL_PROBE

`MANUAL_PROBE [SPEED=<speed>]`: Executa um script auxiliar útil para medir a altura do bico em um determinado local. Se SPEED for especificado, define a velocidade dos comandos TESTZ (o padrão é 5mm/s). Durante uma sondagem manual, os seguintes comandos adicionais estão disponíveis:

- `ACCEPT`: Este comando aceita a posição Z atual e conclui a ferramenta de sondagem manual.
- `ABORT`: Este comando encerra a ferramenta de sondagem manual.
- `TESTZ Z=<value>`: Este comando move o bico para cima ou para baixo pela quantidade especificada em "value". Por exemplo, `TESTZ Z=-.1` moveria o bico .1mm para baixo enquanto `TESTZ Z=.1` moveria o bico .1mm para cima. O valor também pode ser `+`, `-`, `++` ou `--` para mover o bico para cima ou para baixo em uma quantidade relativa às tentativas anteriores.

<a id="z_endstop_calibrate"></a>
#### Z_ENDSTOP_CALIBRATE

`Z_ENDSTOP_CALIBRATE [SPEED=<speed>]`: Executa um script auxiliar útil para calibrar uma configuração position_endstop de Z. Consulte o comando MANUAL_PROBE para detalhes sobre os parâmetros e os comandos adicionais disponíveis enquanto a ferramenta está ativa.

<a id="z_offset_apply_endstop"></a>
#### Z_OFFSET_APPLY_ENDSTOP

`Z_OFFSET_APPLY_ENDSTOP`: Toma o deslocamento (offset) Gcode Z atual (também conhecido como babystepping) e o subtrai do endstop_position do stepper_z. Isto serve para pegar um valor de babystepping usado com frequência e "torná-lo permanente". Requer um `SAVE_CONFIG` para ter efeito.

<a id="manual_stepper"></a>
### [manual_stepper]

O comando a seguir está disponível quando uma [seção de configuração manual_stepper](Config_Reference.md#manual_stepper) está habilitada.

<a id="manual_stepper-1"></a>
#### MANUAL_STEPPER

`MANUAL_STEPPER STEPPER=config_name [ENABLE=[0|1]] [SET_POSITION=<pos>] [SPEED=<speed>] [ACCEL=<accel>] [MOVE=<pos>] [SYNC=0]]`: Este comando alterará o estado do motor de passo. Use o parâmetro ENABLE para habilitar/desabilitar o motor de passo. Use o parâmetro SET_POSITION para forçar o motor de passo a considerar que está na posição informada. Use o parâmetro MOVE para solicitar um movimento para a posição informada. Se SPEED e/ou ACCEL forem especificados, então os valores informados serão usados em vez dos padrões especificados no arquivo de configuração. Se um ACCEL de zero for especificado, então nenhuma aceleração será executada. Normalmente futuros comandos G-Code serão agendados para executar após a conclusão do movimento do motor de passo, entretanto se um movimento manual do motor de passo usar SYNC=0, então futuros comandos de movimento G-Code poderão executar em paralelo com o movimento do motor de passo.

`MANUAL_STEPPER STEPPER=config_name [SPEED=<speed>] [ACCEL=<accel>] MOVE=<pos> STOP_ON_ENDSTOP=<check_type>`: se STOP_ON_ENDSTOP for especificado, o movimento terminará mais cedo se ocorrer um evento de fim de curso. O parâmetro `STOP_ON_ENDSTOP` pode ser definido com um dos seguintes valores:

* `probe`: o movimento parará quando o fim de curso relatar acionado.
* `home`: o movimento parará quando o fim de curso relatar acionado e a posição final do manual_stepper será definida de modo que a posição de acionamento corresponda à posição especificada no parâmetro `MOVE`.
* `inverted_probe`, `inverted_home`: como acima, porém o movimento parará quando o fim de curso relatar que está em estado não acionado.
* `try_probe`, `try_inverted_probe`, `try_home`, `try_inverted_home`: como acima, mas nenhum erro será relatado se o movimento for totalmente concluído sem que um evento de fim de curso interrompa o movimento mais cedo.

`MANUAL_STEPPER STEPPER=config_name GCODE_AXIS=[A-Z] [LIMIT_VELOCITY=<velocity>] [LIMIT_ACCEL=<accel>] [INSTANTANEOUS_CORNER_VELOCITY=<velocity>]`: se o parâmetro `GCODE_AXIS` for especificado, ele configura o motor de passo como um eixo extra nos comandos de movimento `G1`. Por exemplo, se fosse emitido um comando `MANUAL_STEPPER ... GCODE_AXIS=R`, poderiam ser emitidos comandos como `G1 X10 Y20 R30` para mover o motor de passo. Os movimentos resultantes ocorrerão de forma síncrona com os movimentos xyz do cabeçote associado. Se o motor estiver associado a um `GCODE_AXIS`, não será mais possível emitir movimentos usando o comando `MANUAL_STEPPER` acima - pode-se cancelar o registro do motor de passo com um comando `MANUAL_STEPPER ... GCODE_AXIS=` para retomar o controle manual do motor. Os parâmetros `LIMIT_VELOCITY` e `LIMIT_ACCEL` permitem reduzir a velocidade dos movimentos `G1` se esses movimentos resultarem em velocidade ou aceleração acima dos limites especificados. O `INSTANTANEOUS_CORNER_VELOCITY` especifica a variação instantânea máxima de velocidade (em mm/s) do motor durante a junção de dois movimentos (o padrão é 1mm/s).

<a id="mcp4018"></a>
### [mcp4018]

O comando a seguir está disponível quando uma [seção de configuração mcp4018](Config_Reference.md#mcp4018) está habilitada.

<a id="set_digipot"></a>
#### SET_DIGIPOT

`SET_DIGIPOT DIGIPOT=config_name WIPER=<value>`: este comando alterará o valor atual do digipot. Este valor normalmente deve estar entre 0.0 e 1.0, a menos que uma 'scale' esteja definida na configuração. Quando 'scale' está definida, esse valor deve estar entre 0.0 e 'scale'.

<a id="output_pin"></a>
### [output_pin]

O comando a seguir está disponível quando uma [seção de configuração output_pin](Config_Reference.md#output_pin) ou [seção de configuração pwm_tool](Config_Reference.md#pwm_tool) está habilitada.

<a id="set_pin"></a>
#### SET_PIN

`SET_PIN PIN=config_name VALUE=<value>`: Define o pino para o `VALUE` de saída informado. VALUE deve ser 0 ou 1 para pinos de saída "digitais". Para pinos PWM, defina um valor entre 0.0 e 1.0, ou entre 0.0 e `scale` se uma escala estiver configurada na seção de configuração output_pin.

`SET_PIN PIN=config_name TEMPLATE=<template_name> [<param_x>=<literal>]`: Se `TEMPLATE` for especificado, então ele atribui um [display_template](Config_Reference.md#display_template) ao pino indicado. Por exemplo, se uma seção de configuração `[display_template my_pin_template]` tiver sido definida, então pode-se atribuir `TEMPLATE=my_pin_template` aqui. O display_template deve produzir uma string contendo um número de ponto flutuante com o valor desejado. O modelo será avaliado continuamente e o pino será ajustado automaticamente para o valor resultante. Podem-se definir parâmetros de display_template para usar durante a avaliação do modelo (os parâmetros serão analisados como literais Python). Se TEMPLATE for uma string vazia, então este comando limpará qualquer modelo atribuído anteriormente ao pino (podendo-se então usar comandos `SET_PIN` para gerenciar os valores diretamente).

<a id="palette2"></a>
### [palette2]

Os seguintes comandos estão disponíveis quando a [seção de configuração palette2](Config_Reference.md#palette2) está habilitada.

Impressões Palette funcionam incorporando OCodes especiais (Omega Codes) no arquivo GCode:

- `O1`...`O32`: Estes códigos são lidos do fluxo GCode e processados por este módulo e passados para o dispositivo Palette 2.

Os comandos adicionais a seguir também estão disponíveis.

<a id="palette_connect"></a>
#### PALETTE_CONNECT

`PALETTE_CONNECT`: Este comando inicializa a conexão com o Palette 2.

<a id="palette_disconnect"></a>
#### PALETTE_DISCONNECT

`PALETTE_DISCONNECT`: Este comando desconecta do Palette 2.

<a id="palette_clear"></a>
#### PALETTE_CLEAR

`PALETTE_CLEAR`: Este comando instrui o Palette 2 a limpar todos os caminhos de entrada e saída de filamento.

<a id="palette_cut"></a>
#### PALETTE_CUT

`PALETTE_CUT`: Este comando instrui o Palette 2 a cortar o filamento atualmente carregado no núcleo de emenda (splice core).

<a id="palette_smart_load"></a>
#### PALETTE_SMART_LOAD

`PALETTE_SMART_LOAD`: Este comando inicia a sequência de carregamento inteligente (smart load) no Palette 2. O filamento é carregado automaticamente extrudando-o pela distância calibrada no dispositivo para a impressora, e instrui o Palette 2 assim que o carregamento for concluído. Este comando é o mesmo que pressionar **Smart Load** diretamente na tela do Palette 2 após o carregamento do filamento ser concluído.

<a id="pause_resume"></a>
### [pause_resume]

Os seguintes comandos estão disponíveis quando a [seção de configuração pause_resume](Config_Reference.md#pause_resume) está habilitada:

<a id="pause"></a>
#### PAUSE

`PAUSE`: Pausa a impressão atual. A posição atual é capturada para restauração ao retomar.

<a id="resume"></a>
#### RESUME

`RESUME [VELOCITY=<value>]`: Retoma a impressão a partir de uma pausa, primeiro restaurando a posição capturada anteriormente. O parâmetro VELOCITY determina a velocidade com que a ferramenta deve retornar à posição original capturada.

<a id="clear_pause"></a>
#### CLEAR_PAUSE

`CLEAR_PAUSE`: Limpa o estado pausado atual sem retomar a impressão. Isto é útil se decidir cancelar uma impressão após um PAUSE. Recomenda-se adicionar isto ao seu gcode inicial para garantir que o estado pausado esteja limpo para cada impressão.

<a id="cancel_print"></a>
#### CANCEL_PRINT

`CANCEL_PRINT`: Cancela a impressão atual.

<a id="pid_calibrate"></a>
### [pid_calibrate]

O módulo pid_calibrate é carregado automaticamente se um aquecedor estiver definido no arquivo de configuração.

<a id="pid_calibrate-1"></a>
#### PID_CALIBRATE

`PID_CALIBRATE HEATER=<config_name> TARGET=<temperature> [WRITE_FILE=1]`: Executa um teste de calibração PID. O aquecedor especificado será ligado até que a temperatura-alvo especificada seja atingida, e então o aquecedor será desligado e ligado por vários ciclos. Se o parâmetro WRITE_FILE estiver habilitado, então o arquivo /tmp/heattest.txt será criado com um registro de todas as amostras de temperatura coletadas durante o teste.

<a id="print_stats"></a>
### [print_stats]

O módulo print_stats é carregado automaticamente.

<a id="set_print_stats_info"></a>
#### SET_PRINT_STATS_INFO

`SET_PRINT_STATS_INFO [TOTAL_LAYER=<total_layer_count>] [CURRENT_LAYER= <current_layer>]`: Passa informações do fatiador como camada atual e total para o Klipper. Adicione `SET_PRINT_STATS_INFO [TOTAL_LAYER=<total_layer_count>]` à seção de gcode inicial do seu fatiador e `SET_PRINT_STATS_INFO [CURRENT_LAYER= <current_layer>]` na seção de gcode de troca de camada para passar informações de camada do seu fatiador para o Klipper.

<a id="probe"></a>
### [probe]

Os seguintes comandos estão disponíveis quando uma [seção de configuração probe](Config_Reference.md#probe) ou [seção de configuração bltouch](Config_Reference.md#bltouch) está habilitada (consulte também o [guia de calibração da sonda](Probe_Calibrate.md)).

<a id="probe-1"></a>
#### PROBE

`PROBE [PROBE_SPEED=<mm/s>] [LIFT_SPEED=<mm/s>] [SAMPLES=<count>] [SAMPLE_RETRACT_DIST=<mm>] [SAMPLES_TOLERANCE=<mm>] [SAMPLES_TOLERANCE_RETRIES=<count>] [SAMPLES_RESULT=median|average]`: Move o bico para baixo até que a sonda dispare. Se qualquer um dos parâmetros opcionais for informado, eles substituem sua configuração equivalente na [seção de configuração probe](Config_Reference.md#probe).

<a id="query_probe"></a>
#### QUERY_PROBE

`QUERY_PROBE`: Relata o status atual da sonda ("triggered" ou "open").

<a id="probe_accuracy"></a>
#### PROBE_ACCURACY

`PROBE_ACCURACY [PROBE_SPEED=<mm/s>] [SAMPLES=<count>] [SAMPLE_RETRACT_DIST=<mm>]`: Calcula o máximo, mínimo, média, mediana e desvio padrão de múltiplas amostras de sondagem. Por padrão, 10 SAMPLES são coletadas. Caso contrário, os parâmetros opcionais assumem por padrão sua configuração equivalente na seção de configuração probe.

<a id="probe_calibrate"></a>
#### PROBE_CALIBRATE

`PROBE_CALIBRATE [SPEED=<speed>] [<probe_parameter>=<value>]`: Executa um script auxiliar útil para calibrar o z_offset da sonda. Consulte o comando PROBE para detalhes sobre os parâmetros opcionais da sonda. Consulte o comando MANUAL_PROBE para detalhes sobre o parâmetro SPEED e os comandos adicionais disponíveis enquanto a ferramenta está ativa. Observe que o comando PROBE_CALIBRATE usa a variável de velocidade para mover na direção XY bem como em Z.

<a id="z_offset_apply_probe"></a>
#### Z_OFFSET_APPLY_PROBE

`Z_OFFSET_APPLY_PROBE`: Pega o deslocamento Gcode Z atual (também chamado de babystepping) e o subtrai do z_offset da sonda. Isso serve para pegar um valor de babystepping usado com frequência e torná-lo permanente. Requer um `SAVE_CONFIG` para entrar em vigor.

<a id="probe_eddy_current"></a>
### [probe_eddy_current]

Os comandos abaixo estão disponíveis quando uma [seção de configuração probe_eddy_current](Config_Reference.md#probe_eddy_current) está habilitada.

Além disso, comandos que realizam sondagens, como [`PROBE`](#probe), [`PROBE_ACCURACY`](#probe_accuracy), [`BED_MESH_CALIBRATE`](#bed_mesh_calibrate) etc. aceitarão parâmetros adicionais se uma seção `[probe_eddy_current]` estiver definida:

- `METHOD=<scan|rapid_scan|tap>`: Isso altera o mecanismo de sondagem:
   - `METHOD=scan`: O cabeçote não desce. Em vez disso, o cabeçote fará uma breve pausa acima de cada local alvo e retornará a altura medida nessa posição.
   - `METHOD=rapid_scan`: O cabeçote não desce e não faz pausa em cada local alvo. O valor retornado é a altura medida por volta do momento em que o cabeçote estava próximo de cada posição alvo.
   - `METHOD=tap`: O cabeçote descerá até o bico encostar na mesa. Este método só está disponível se `tap_threshold` estiver especificado na seção de configuração `[probe_eddy_current]`.
   - padrão: Se nenhum parâmetro `METHOD` for especificado, então o comportamento padrão é o cabeçote descer até a sonda detectar que a distância até a mesa está igual ou abaixo do parâmetro `z_offset` especificado na seção de configuração `[probe_eddy_current]`.
- `SAMPLE_TIME=<time>`: Ao usar sondagem com `METHOD=scan`, isso especifica o tempo (em segundos) de pausa em cada ponto alvo. Ao usar `METHOD=rapid_scan`, isso especifica a janela de tempo de medição em cada alvo. Se não especificado, o padrão é 0.100 (que é 100ms).
- `TAP_THRESHOLD=<value>`: Isso substitui o `tap_threshold` especificado na seção de configuração `[probe_eddy_current]` ao sondar usando `METHOD=tap`.

O comando `Z_OFFSET_APPLY_PROBE` também é estendido para aceitar um parâmetro `METHOD=tap`. Quando nenhum parâmetro METHOD é fornecido, o comando `Z_OFFSET_APPLY_PROBE` altera a calibração da sonda para aplicar o deslocamento Z atual de G-Code a futuras sondagens `scan`, `rapid_scan` e sondagens padrão. Se `METHOD=tap` for especificado, então o comando em vez disso aplica a alteração a `tap_z_offset` para que futuras sondagens `tap` sejam atualizadas para usar o deslocamento Z atual de G-Code.

<a id="probe_eddy_current_calibrate"></a>
#### PROBE_EDDY_CURRENT_CALIBRATE

`PROBE_EDDY_CURRENT_CALIBRATE CHIP=<config_name> [MANUAL_METHOD=manual]`: Isso inicia uma ferramenta que calibra as frequências de ressonância da sonda para as alturas Z correspondentes. A ferramenta levará alguns minutos para concluir. Após a conclusão, use o comando SAVE_CONFIG para armazenar os resultados no arquivo printer.cfg.

<a id="probe_eddy_current_tap_calibrate"></a>
#### PROBE_EDDY_CURRENT_TAP_CALIBRATE

`PROBE_EDDY_CURRENT_TAP_CALIBRATE [TAP=guess|refine|verify]`: Isso inicia uma ferramenta que pode calibrar o parâmetro "tap_threshold" da sonda. Veja a [documentação da sonda eddy](Eddy_Probe.md#tap-calibration) para detalhes.

<a id="ldc_calibrate_drive_current"></a>
#### LDC_CALIBRATE_DRIVE_CURRENT

`LDC_CALIBRATE_DRIVE_CURRENT CHIP=<config_name>` Esta ferramenta calibrará o registrador DRIVE_CURRENT0 do ldc1612. Antes de usar esta ferramenta, mova a sonda para que fique próxima ao centro da mesa e cerca de 20mm acima da superfície da mesa. Execute este comando para determinar um DRIVE_CURRENT apropriado para a sonda. Após executar este comando, use o comando SAVE_CONFIG para armazenar essa nova configuração no arquivo de configuração printer.cfg.

<a id="pwm_cycle_time"></a>
### [pwm_cycle_time]

O seguinte comando está disponível quando uma [seção de configuração pwm_cycle_time](Config_Reference.md#pwm_cycle_time) está habilitada.

<a id="set_pin-1"></a>
#### SET_PIN

`SET_PIN PIN=config_name VALUE=<value> [CYCLE_TIME=<cycle_time>]`: Este comando funciona de forma semelhante aos comandos SET_PIN de [output_pin](#output_pin). O comando aqui permite definir um tempo de ciclo explícito usando o parâmetro CYCLE_TIME (especificado em segundos). Observe que o parâmetro CYCLE_TIME não é armazenado entre comandos SET_PIN (qualquer comando SET_PIN sem um parâmetro CYCLE_TIME explícito usará o `cycle_time` especificado na seção de configuração pwm_cycle_time).

<a id="quad_gantry_level"></a>
### [quad_gantry_level]

Os seguintes comandos estão disponíveis quando a [seção de configuração quad_gantry_level](Config_Reference.md#quad_gantry_level) está habilitada.

<a id="quad_gantry_level-1"></a>
#### QUAD_GANTRY_LEVEL

`QUAD_GANTRY_LEVEL [RETRIES=<value>] [RETRY_TOLERANCE=<value>] [HORIZONTAL_MOVE_Z=<value>] [<probe_parameter>=<value>]`: Este comando sondará os pontos especificados na configuração e fará ajustes independentes em cada motor de passo Z para compensar a inclinação. Veja o comando PROBE para detalhes sobre os parâmetros opcionais de sondagem. Os valores opcionais `RETRIES`, `RETRY_TOLERANCE` e `HORIZONTAL_MOVE_Z` substituem as opções especificadas no arquivo de configuração.

<a id="query_adc"></a>
### [query_adc]

O módulo query_adc é carregado automaticamente.

<a id="query_adc-1"></a>
#### QUERY_ADC

`QUERY_ADC [NAME=<config_name>] [PULLUP=<value>]`: Informa o último valor analógico recebido para um pino analógico configurado. Se NAME não for fornecido, a lista de nomes adc disponíveis é informada. Se PULLUP for fornecido (como valor em Ohms), o valor analógico bruto junto com a resistência equivalente dado esse pullup é informado.

<a id="query_endstops"></a>
### [query_endstops]

O módulo query_endstops é carregado automaticamente. Os seguintes comandos G-Code padrão estão disponíveis atualmente, mas seu uso não é recomendado:

- Obter estado dos fins de curso: `M119` (Use QUERY_ENDSTOPS em vez disso.)

<a id="query_endstops-1"></a>
#### QUERY_ENDSTOPS

`QUERY_ENDSTOPS`: Sonda os fins de curso dos eixos e informa se estão "triggered" ou em estado "open". Este comando é normalmente usado para verificar se um fim de curso está funcionando corretamente.

<a id="resonance_tester"></a>
### [resonance_tester]

Os seguintes comandos estão disponíveis quando uma [seção de configuração resonance_tester](Config_Reference.md#resonance_tester) está habilitada (consulte também o [guia de medição de ressonâncias](Measuring_Resonances.md)).

<a id="measure_axes_noise"></a>
#### MEASURE_AXES_NOISE

`MEASURE_AXES_NOISE`: Mede e exibe o ruído de todos os eixos de todos os chips de acelerômetro habilitados.

<a id="test_resonances"></a>
#### TEST_RESONANCES

`TEST_RESONANCES AXIS=<axis> [OUTPUT=<resonances,raw_data>] [NAME=<name>] [FREQ_START=<min_freq>] [FREQ_END=<max_freq>] [ACCEL_PER_HZ=<accel_per_hz>] [HZ_PER_SEC=<hz_per_sec>] [CHIPS=<chip_name>] [POINT=x,y,z] [INPUT_SHAPING=<0:1>]`: Executa o teste de ressonância em todos os pontos de sondagem configurados para o "axis" solicitado e mede a aceleração usando os chips de acelerômetro configurados para o respectivo eixo. "axis" pode ser X, Y ou Z, ou especificar uma direção arbitrária como `AXIS=dx,dy[,dz]`, onde dx, dy, dz são números de ponto flutuante definindo um vetor de direção (p. ex. `AXIS=X`, `AXIS=Y`, ou `AXIS=1,-1` para definir uma direção diagonal no plano XY, ou `AXIS=0,1,1` para definir uma direção no plano YZ). Observe que `AXIS=dx,dy` e `AXIS=-dx,-dy` são equivalentes. `chip_name` pode ser um ou mais chips de aceleração configurados, delimitados por vírgula, por exemplo `CHIPS="adxl345, adxl345 rpi"`. Se POINT for especificado, ele substituirá o(s) ponto(s) configurado(s) em `[resonance_tester]`. Se `INPUT_SHAPING=0` ou não definido (padrão), desabilita a modelagem de entrada para o teste de ressonância, pois não é válido executar o teste de ressonância com o modelador de entrada habilitado. O parâmetro `OUTPUT` é uma lista separada por vírgulas indicando quais saídas serão gravadas. Se `raw_data` for solicitado, então os dados brutos do acelerômetro são gravados em um arquivo ou série de arquivos `/tmp/raw_data_<axis>_[<chip_name>_][<point>_]<name>.csv` (a parte `<point>_` do nome é gerada somente se mais de 1 ponto de sondagem estiver configurado ou se POINT for especificado). Se `resonances` for especificado, a resposta em frequência é calculada (entre todos os pontos de sondagem) e gravada no arquivo `/tmp/resonances_<axis>_<name>.csv`. Se não definido, OUTPUT assume `resonances` por padrão, e NAME assume por padrão a hora atual no formato "YYYYMMDD_HHMMSS".

<a id="shaper_calibrate"></a>
#### SHAPER_CALIBRATE

`SHAPER_CALIBRATE [AXIS=<axis>] [NAME=<name>] [FREQ_START=<min_freq>] [FREQ_END=<max_freq>] [ACCEL_PER_HZ=<accel_per_hz>][HZ_PER_SEC=<hz_per_sec>] [CHIPS=<chip_name>] [MAX_SMOOTHING=<max_smoothing>] [INPUT_SHAPING=<0:1>]`: De forma semelhante a `TEST_RESONANCES`, executa o teste de ressonância conforme configurado e tenta encontrar os parâmetros ideais para o modelador de entrada para o eixo solicitado (ou para ambos os eixos X e Y se o parâmetro `AXIS` não for definido). Se `MAX_SMOOTHING` não for definido, seu valor é obtido da seção `[resonance_tester]`, com o padrão sendo indefinido. Veja [Suavização máxima](Measuring_Resonances.md#max-smoothing) no guia de medição de ressonâncias para mais informações sobre o uso deste recurso. Os resultados do ajuste são impressos no console, e as respostas em frequência e os diferentes valores dos modeladores de entrada são gravados em arquivo(s) CSV `/tmp/calibration_data_<axis>_<name>.csv`. Salvo especificação em contrário, NAME assume por padrão a hora atual no formato "YYYYMMDD_HHMMSS". Observe que os parâmetros sugeridos do modelador de entrada podem ser persistidos na configuração emitindo o comando `SAVE_CONFIG`, e se `[input_shaper]` já estava habilitado anteriormente, esses parâmetros entram em vigor imediatamente.

<a id="respond"></a>
### [respond]

Os seguintes comandos G-Code padrão estão disponíveis quando a [seção de configuração respond](Config_Reference.md#respond) está habilitada:

- `M118 <message>`: ecoa a mensagem precedida do prefixo padrão configurado (ou `echo: ` se nenhum prefixo estiver configurado).

Os comandos adicionais a seguir também estão disponíveis.

<a id="respond-1"></a>
#### RESPOND

- `RESPOND MSG="<message>"`: ecoa a mensagem precedida do prefixo padrão configurado (ou `echo: ` se nenhum prefixo estiver configurado).
- `RESPOND TYPE=echo MSG="<message>"`: ecoa a mensagem precedida de `echo: `.
- `RESPOND TYPE=echo_no_space MSG="<message>"`: ecoa a mensagem precedida de `echo:` sem espaço entre o prefixo e a mensagem, útil para compatibilidade com alguns plugins do OctoPrint que esperam formatação muito específica.
- `RESPOND TYPE=command MSG="<message>"`: ecoa a mensagem precedida de `// `. O OctoPrint pode ser configurado para responder a essas mensagens (p. ex. `RESPOND TYPE=command MSG=action:pause`).
- `RESPOND TYPE=error MSG="<message>"`: ecoa a mensagem precedida de `!! `.
- `RESPOND PREFIX=<prefix> MSG="<message>"`: ecoa a mensagem precedida de `<prefix>`. (O parâmetro `PREFIX` terá prioridade sobre o parâmetro `TYPE`)

<a id="save_variables"></a>
### [save_variables]

O seguinte comando está habilitado se uma [seção de configuração save_variables](Config_Reference.md#save_variables) tiver sido habilitada.

<a id="save_variable"></a>
#### SAVE_VARIABLE

`SAVE_VARIABLE VARIABLE=<name> VALUE=<value>`: Salva a variável no disco para que possa ser usada entre reinicializações. A VARIABLE deve estar em minúsculas. Todas as variáveis armazenadas são carregadas no dicionário `printer.save_variables.variables` na inicialização e podem ser usadas em macros gcode. O VALUE fornecido é analisado como um literal Python.

<a id="screws_tilt_adjust"></a>
### [screws_tilt_adjust]

Os seguintes comandos estão disponíveis quando a [seção de configuração screws_tilt_adjust](Config_Reference.md#screws_tilt_adjust) está habilitada (consulte também o [guia de nivelamento manual](Manual_Level.md#adjusting-bed-leveling-screws-using-the-bed-probe)).

<a id="screws_tilt_calculate"></a>
#### SCREWS_TILT_CALCULATE

`SCREWS_TILT_CALCULATE [DIRECTION=CW|CCW] [MAX_DEVIATION=<value>] [HORIZONTAL_MOVE_Z=<value>] [<probe_parameter>=<value>]`: Este comando invocará a ferramenta de ajuste dos parafusos da mesa. Ele comandará o bico para diferentes locais (conforme definido no arquivo de configuração), sondando a altura z, e calculará o número de voltas dos botões para ajustar o nivelamento da mesa. Se DIRECTION for especificado, as voltas dos botões serão todas na mesma direção, horária (CW) ou anti-horária (CCW). Veja o comando PROBE para detalhes sobre os parâmetros opcionais de sondagem. IMPORTANTE: Você DEVE sempre fazer um G28 antes de usar este comando. Se MAX_DEVIATION for especificado, o comando gerará um erro de gcode se qualquer diferença na altura do parafuso em relação à altura do parafuso base for maior que o valor fornecido. O valor opcional `HORIZONTAL_MOVE_Z` substitui a opção `horizontal_move_z` especificada no arquivo de configuração.

<a id="sdcard_loop"></a>
### [sdcard_loop]

Quando a [seção de configuração sdcard_loop](Config_Reference.md#sdcard_loop) está habilitada, os seguintes comandos estendidos estão disponíveis.

<a id="sdcard_loop_begin"></a>
#### SDCARD_LOOP_BEGIN

`SDCARD_LOOP_BEGIN COUNT=<count>`: Inicia uma seção em loop na impressão SD. Uma contagem de 0 indica que a seção deve ser repetida indefinidamente.

<a id="sdcard_loop_end"></a>
#### SDCARD_LOOP_END

`SDCARD_LOOP_END`: Encerra uma seção em loop na impressão SD.

<a id="sdcard_loop_desist"></a>
#### SDCARD_LOOP_DESIST

`SDCARD_LOOP_DESIST`: Conclui os loops existentes sem mais iterações.

<a id="servo"></a>
### [servo]

Os seguintes comandos estão disponíveis quando uma [seção de configuração servo](Config_Reference.md#servo) está habilitada.

<a id="set_servo"></a>
#### SET_SERVO

`SET_SERVO SERVO=config_name [ANGLE=<degrees> | WIDTH=<seconds>]`: Define a posição do servo para o ângulo dado (em graus) ou largura de pulso (em segundos). Use `WIDTH=0` para desabilitar a saída do servo.

<a id="skew_correction"></a>
### [skew_correction]

Os seguintes comandos estão disponíveis quando a [seção de configuração skew_correction](Config_Reference.md#skew_correction) está habilitada (consulte também o guia [Correção de Inclinação](Skew_Correction.md)).

<a id="set_skew"></a>
#### SET_SKEW

`SET_SKEW [XY=<ac_length,bd_length,ad_length>] [XZ=<ac,bd,ad>] [YZ=<ac,bd,ad>] [CLEAR=<0|1>]`: Configura o módulo [skew_correction] com medidas (em mm) obtidas de uma impressão de calibração. Pode-se inserir medidas para qualquer combinação de planos; planos não informados manterão seu valor atual. Se `CLEAR=1` for informado, toda a correção de inclinação será desabilitada.

<a id="get_current_skew"></a>
#### GET_CURRENT_SKEW

`GET_CURRENT_SKEW`: Informa a inclinação atual da impressora para cada plano tanto em radianos quanto em graus. A inclinação é calculada com base nos parâmetros fornecidos via gcode `SET_SKEW`.

<a id="calc_measured_skew"></a>
#### CALC_MEASURED_SKEW

`CALC_MEASURED_SKEW [AC=<ac_length>] [BD=<bd_length>] [AD=<ad_length>]`: Calcula e informa a inclinação (em radianos e graus) com base em uma impressão medida. Isso pode ser útil para determinar a inclinação atual da impressora após a correção ter sido aplicada. Também pode ser útil antes da correção ser aplicada para determinar se a correção de inclinação é necessária. Veja [Correção de inclinação](Skew_Correction.md) para detalhes sobre objetos de calibração de inclinação e medidas.

<a id="skew_profile"></a>
#### SKEW_PROFILE

`SKEW_PROFILE [LOAD=<name>] [SAVE=<name>] [REMOVE=<name>]`: Gerenciamento de perfis para skew_correction. LOAD restaurará o estado de inclinação a partir do perfil correspondente ao nome fornecido. SAVE salvará o estado atual de inclinação em um perfil correspondente ao nome fornecido. Remove excluirá o perfil correspondente ao nome fornecido da memória persistente. Observe que após operações de SAVE ou REMOVE, o gcode SAVE_CONFIG deve ser executado para tornar permanentes as alterações na memória persistente.

<a id="smart_effector"></a>
### [smart_effector]

Vários comandos estão disponíveis quando uma [seção de configuração smart_effector](Config_Reference.md#smart_effector) está habilitada. Certifique-se de verificar a documentação oficial do Smart Effector na [Duet3D Wiki](https://duet3d.dozuki.com/Wiki/Smart_effector_and_carriage_adapters_for_delta_printer) antes de alterar os parâmetros do Smart Effector. Verifique também o [guia de calibração da sonda](Probe_Calibrate.md).

<a id="set_smart_effector"></a>
#### SET_SMART_EFFECTOR

`SET_SMART_EFFECTOR [SENSITIVITY=<sensitivity>] [ACCEL=<accel>] [RECOVERY_TIME=<time>]`: Define os parâmetros do Smart Effector. Quando `SENSITIVITY` é especificado, o respectivo valor é gravado na EEPROM do SmartEffector (requer que `control_pin` seja fornecido). Valores aceitáveis de `<sensitivity>` são 0..255, o padrão é 50. Valores mais baixos exigem menos força de contato do bico para disparar (mas há maior risco de disparo falso devido a vibrações durante a sondagem), e valores mais altos reduzem disparos falsos (mas exigem maior força de contato para disparar). Como a sensibilidade é gravada na EEPROM, ela é preservada após o desligamento e, portanto, não precisa ser configurada a cada inicialização da impressora. `ACCEL` e `RECOVERY_TIME` permitem substituir os parâmetros correspondentes em tempo de execução; veja a [seção de configuração](Config_Reference.md#smart_effector) do Smart Effector para mais informações sobre esses parâmetros.

<a id="reset_smart_effector"></a>
#### RESET_SMART_EFFECTOR

`RESET_SMART_EFFECTOR`: Redefine a sensibilidade do Smart Effector para suas configurações de fábrica. Requer que `control_pin` seja fornecido na seção de configuração.

<a id="stepper_enable"></a>
### [stepper_enable]

O módulo stepper_enable é carregado automaticamente.

<a id="set_stepper_enable"></a>
#### SET_STEPPER_ENABLE

`SET_STEPPER_ENABLE STEPPER=<config_name> ENABLE=[0|1]`: Habilita ou desabilita somente o motor de passo informado. Esta é uma ferramenta de diagnóstico e depuração e deve ser usada com cuidado. Desabilitar o motor de um eixo não redefine as informações de origem (homing). Mover manualmente um motor de passo desabilitado pode fazer a máquina operar o motor fora dos limites seguros. Isso pode causar danos aos componentes dos eixos, aos hotends e à superfície de impressão.

<a id="temperature_fan"></a>
### [temperature_fan]

O seguinte comando está disponível quando uma [seção de configuração temperature_fan](Config_Reference.md#temperature_fan) está habilitada.

<a id="set_temperature_fan_target"></a>
#### SET_TEMPERATURE_FAN_TARGET

`SET_TEMPERATURE_FAN_TARGET temperature_fan=<temperature_fan_name> [target=<target_temperature>] [min_speed=<min_speed>] [max_speed=<max_speed>]`: Define a temperatura alvo para um temperature_fan. Se um alvo não for fornecido, ele é definido para a temperatura especificada no arquivo de configuração. Se velocidades não forem fornecidas, nenhuma alteração é aplicada.

<a id="temperature_probe"></a>
### [temperature_probe]

Os seguintes comandos estão disponíveis quando uma [seção de configuração temperature_probe](Config_Reference.md#temperature_probe) está habilitada.

<a id="temperature_probe_calibrate"></a>
#### TEMPERATURE_PROBE_CALIBRATE

`TEMPERATURE_PROBE_CALIBRATE [PROBE=<probe name>] [TARGET=<value>] [STEP=<value>] [MANUAL_METHOD=<method>]`: Inicia a calibração de deriva da sonda para sondas baseadas em corrente de Foucault (eddy current). O `TARGET` é uma temperatura alvo para a última amostra. Quando a temperatura registrada durante uma amostra exceder o `TARGET`, a calibração será concluída. O parâmetro `STEP` define o delta de temperatura (em C) entre amostras. Após uma amostra ter sido coletada, este delta é usado para agendar uma chamada para `TEMPERATURE_PROBE_NEXT`. O `STEP` padrão é 2. O `METHOD` aceita apenas `tap` como opção; se especificado, a sondagem será automatizada.

<a id="temperature_probe_next"></a>
#### TEMPERATURE_PROBE_NEXT

`TEMPERATURE_PROBE_NEXT`: Após o início da calibração, este comando é executado para coletar a próxima amostra. Ele é agendado automaticamente para executar quando o delta especificado por `STEP` for alcançado, porém também é possível executar manualmente este comando para forçar uma nova amostra. Este comando só está disponível durante a calibração.

<a id="temperature_probe_complete"></a>
#### TEMPERATURE_PROBE_COMPLETE:

`TEMPERATURE_PROBE_COMPLETE`: Pode ser usado para encerrar a calibração e salvar o resultado atual antes que a temperatura `TARGET` seja alcançada. Este comando só está disponível durante a calibração.

<a id="abort"></a>
#### ABORT

`ABORT`: Interrompe o processo de calibração, descartando os resultados atuais. Este comando só está disponível durante a calibração de deriva.

<a id="temperature_probe_enable"></a>
### TEMPERATURE_PROBE_ENABLE

`TEMPERATURE_PROBE_ENABLE ENABLE=[0|1]`: Ativa ou desativa a compensação de deriva por temperatura. Se ENABLE for definido como 0, a compensação de deriva será desabilitada; se definido como 1, será habilitada.

<a id="tmcxxxx"></a>
### [tmcXXXX]

Os seguintes comandos estão disponíveis quando qualquer uma das [seções de configuração tmcXXXX](Config_Reference.md#tmc-stepper-driver-configuration) está habilitada.

<a id="dump_tmc"></a>
#### DUMP_TMC

`DUMP_TMC STEPPER=<name> [REGISTER=<name>]`: Este comando lerá todos os registradores do driver TMC e informará seus valores. Se um REGISTER for fornecido, somente o registrador especificado será exibido.

<a id="init_tmc"></a>
#### INIT_TMC

`INIT_TMC STEPPER=<name>`: Este comando inicializará os registradores TMC. Necessário para reabilitar o driver se a energia do chip for desligada e ligada novamente.

<a id="set_tmc_current"></a>
#### SET_TMC_CURRENT

`SET_TMC_CURRENT STEPPER=<name> CURRENT=<amps> HOLDCURRENT=<amps>`: Isso ajustará as correntes de operação e de repouso do driver TMC. `HOLDCURRENT` não se aplica a drivers tmc2660. Quando usado em um driver que possui o campo `globalscaler` (tmc5160 e tmc2240), se StealthChop2 for usado, o motor de passo deve ser mantido parado por >130ms para que o driver execute a calibração AT#1.

<a id="set_tmc_field"></a>
#### SET_TMC_FIELD

`SET_TMC_FIELD STEPPER=<name> FIELD=<field> VALUE=<value> VELOCITY=<value>`: Isso alterará o valor do campo de registrador especificado do driver TMC. Este comando destina-se somente a diagnósticos e depuração de baixo nível, pois alterar os campos durante a execução pode levar a comportamento indesejado e potencialmente perigoso da impressora. Alterações permanentes devem ser feitas usando o arquivo de configuração da impressora. Nenhuma verificação de sanidade é realizada para os valores dados. Uma VELOCITY também pode ser especificada em vez de VALUE. Essa velocidade é convertida para a representação de valor TSTEP de 20 bits. Use o argumento VELOCITY somente para campos que representam velocidades.

<a id="toolhead"></a>
### [toolhead]

O módulo toolhead é carregado automaticamente.

<a id="set_velocity_limit"></a>
#### SET_VELOCITY_LIMIT

`SET_VELOCITY_LIMIT [VELOCITY=<value>] [ACCEL=<value>] [MINIMUM_CRUISE_RATIO=<value>] [SQUARE_CORNER_VELOCITY=<value>]`: Este comando pode alterar os limites de velocidade especificados no arquivo de configuração da impressora. Veja a [seção de configuração printer](Config_Reference.md#printer) para uma descrição de cada parâmetro.

<a id="tuning_tower"></a>
### [tuning_tower]

O módulo tuning_tower é carregado automaticamente.

<a id="tuning_tower-1"></a>
#### TUNING_TOWER

`TUNING_TOWER COMMAND=<command> PARAMETER=<name> START=<value> [SKIP=<value>] [FACTOR=<value> [BAND=<value>]] | [STEP_DELTA=<value> STEP_HEIGHT=<value>]`: Uma ferramenta para ajustar um parâmetro em cada altura Z durante uma impressão. A ferramenta executará o `COMMAND` dado com o `PARAMETER` dado atribuído a um valor que varia com `Z` de acordo com uma fórmula. Use `FACTOR` se você for usar régua ou paquímetro para medir a altura Z do valor ideal, ou `STEP_DELTA` e `STEP_HEIGHT` se o modelo da torre de ajuste tiver faixas de valores discretos, como é comum com torres de temperatura. Se `SKIP=<value>` for especificado, o processo de ajuste não começa até que a altura Z `<value>` seja alcançada, e abaixo disso o valor será definido como `START`; neste caso, a `z_height` usada nas fórmulas abaixo é na verdade `max(z - skip, 0)`. Há três combinações possíveis de opções:

- `FACTOR`: O valor muda a uma taxa de `factor` por milímetro. A fórmula usada é: `value = start + factor * z_height`. Você pode inserir a altura Z ideal diretamente na fórmula para determinar o valor ideal do parâmetro.
- `FACTOR` e `BAND`: O valor muda a uma taxa média de `factor` por milímetro, mas em faixas discretas em que o ajuste só será feito a cada `BAND` milímetros de altura Z. A fórmula usada é: `value = start + factor * ((floor(z_height / band) + .5) * band)`.
- `STEP_DELTA` e `STEP_HEIGHT`: O valor muda em `STEP_DELTA` a cada `STEP_HEIGHT` milímetros. A fórmula usada é: `value = start + step_delta * floor(z_height / step_height)`. Você pode simplesmente contar faixas ou ler os rótulos da torre de ajuste para determinar o valor ideal.

<a id="virtual_sdcard"></a>
### [virtual_sdcard]

O Klipper aceita os seguintes comandos G-Code padrão se a [seção de configuração virtual_sdcard](Config_Reference.md#virtual_sdcard) estiver habilitada:

- Listar cartão SD: `M20`
- Inicializar cartão SD: `M21`
- Selecionar arquivo SD: `M23 <filename>`
- Iniciar/retomar impressão SD: `M24`
- Pausar impressão SD: `M25`
- Definir posição SD: `M26 S<offset>`
- Informar estado da impressão SD: `M27`

Além disso, os seguintes comandos estendidos estão disponíveis quando a seção de configuração "virtual_sdcard" está habilitada.

<a id="sdcard_print_file"></a>
#### SDCARD_PRINT_FILE

`SDCARD_PRINT_FILE FILENAME=<filename>`: Carrega um arquivo e inicia a impressão SD.

<a id="sdcard_reset_file"></a>
#### SDCARD_RESET_FILE

`SDCARD_RESET_FILE`: Descarrega o arquivo e limpa o estado SD.

<a id="z_thermal_adjust"></a>
### [z_thermal_adjust]

Os seguintes comandos estão disponíveis quando a [seção de configuração z_thermal_adjust](Config_Reference.md#z_thermal_adjust) está habilitada.

<a id="set_z_thermal_adjust"></a>
#### SET_Z_THERMAL_ADJUST

`SET_Z_THERMAL_ADJUST [ENABLE=<0:1>] [TEMP_COEFF=<value>] [REF_TEMP=<value>]`: Habilita ou desabilita o ajuste térmico Z com `ENABLE`. Desabilitar não remove nenhum ajuste já aplicado, mas congelará o valor atual de ajuste - isso evita movimento Z descendente potencialmente inseguro. Reabilitar pode potencialmente causar movimento ascendente da ferramenta à medida que o ajuste é atualizado e aplicado. `TEMP_COEFF` permite ajuste em tempo de execução do coeficiente de temperatura do ajuste (ou seja, o parâmetro de configuração `TEMP_COEFF`). Valores de `TEMP_COEFF` não são salvos na configuração. `REF_TEMP` substitui manualmente a temperatura de referência normalmente definida durante a origem (para uso em, p. ex., rotinas de origem não padrão) - será redefinida automaticamente após a origem.

<a id="z_tilt"></a>
### [z_tilt]

Os seguintes comandos estão disponíveis quando a [seção de configuração z_tilt](Config_Reference.md#z_tilt) está habilitada.

<a id="z_tilt_adjust"></a>
#### Z_TILT_ADJUST

`Z_TILT_ADJUST [RETRIES=<value>] [RETRY_TOLERANCE=<value>] [HORIZONTAL_MOVE_Z=<value>] [<probe_parameter>=<value>]`: Este comando sondará os pontos especificados na configuração e fará ajustes independentes em cada motor de passo Z para compensar a inclinação. Veja o comando PROBE para detalhes sobre os parâmetros opcionais de sondagem. Os valores opcionais `RETRIES`, `RETRY_TOLERANCE` e `HORIZONTAL_MOVE_Z` substituem as opções especificadas no arquivo de configuração.
