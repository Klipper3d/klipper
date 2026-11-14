<a id="configuration-changes"></a>
# Alterações de configuração

[English](../Config_Changes.md) | **Português brasileiro**

Esse documento abrange mudanças recentes de software no arquivo de configuração que não são compatíveis com versões anteriores. É recomendado revisar esse documento ao atualizar o software Klipper.

Todas as datas neste documento são aproximadas.

<a id="changes"></a>
## Mudanças

20261018: Um erro nos cálculos de `PID_CALIBRATE` foi corrigido. Anteriormente, o código recomendava valores de PID que eram o dobro dos valores desejados (em relação ao método "Astrom-Hagglund" padrão). O novo código de `PID_CALIBRATE` corrige esse erro e pode produzir valores de PID que resultam em menos oscilações de aquecimento. Se você está satisfeito com o desempenho atual do aquecedor, nenhuma alteração é necessária. Se você está enfrentando oscilações persistentes de aquecimento, pode ser interessante executar `PID_CALIBRATE` novamente e aplicar as recomendações de PID atualizadas.

20260915: O suporte ao parâmetro `METHOD=tap` de `TEMPERATURE_PROBE_CALIBRATE` foi removido. Use `TEMPERATURE_PROBE_CALIBRATE MANUAL_METHOD=tap` em vez disso.

20260525: A implementação interna de "probe:z_virtual_endstop" mudou. A maioria dos usuários não observará alteração de comportamento. Anteriormente era tecnicamente possível misturar "probe:z_virtual_endstop" com outros tipos de fim de curso de Z e esse comportamento não é mais válido.

20260501: O tratamento da opção de configuração `tap_threshold` de `[probe_eddy_current]` e do parâmetro de G-Code `TAP_THRESHOLD` associado mudou. Será necessário recalibrar o valor. Veja a [documentação da sonda de correntes parasitas](Eddy_Probe.md) para instruções de calibração.

20260408: O script `lib/canboot/flash_can.py` foi atualizado para a versão mais atual do [Katapult](https://github.com/Arksine/katapult) e, como tal, renomeado para `lib/katapult/flashtool.py`. Se você chama esse script diretamente em vez de usar os Makefiles existentes, precisará alterar o caminho do script para `lib/katapult/flashtool.py`.

20260318: As opções de configuração `speed`, `lift_speed`, `samples`, `sample_retract_dist`, `samples_result`, `samples_tolerance` e `samples_tolerance_retries` de `[probe_eddy_current]` não se aplicam mais a comandos de sondagem usando `METHOD=scan`, `METHOD=rapid_scan` nem `METHOD=tap`. Para usar configurações diferentes, forneça o parâmetro equivalente `PROBE_SPEED`, `LIFT_SPEED`, `SAMPLES`, `SAMPLE_RETRACT_DIST`, `SAMPLES_RESULT`, `SAMPLES_TOLERANCE` ou `SAMPLES_TOLERANCE_RETRIES` junto ao comando de sondagem.

20260318: A opção de configuração `z_offset` de `[probe_eddy_current]` foi renomeada para `descend_z`. Usar o nome antigo está obsoleto e será removido em um futuro próximo.

20260214: O parâmetro `STOP_ON_ENDSTOP` do comando G-Code `MANUAL_STEPPER` mudou. Veja a documentação de [MANUAL_STEPPER](G-Codes.md#manual_stepper) para detalhes. Usar os valores inteiros anteriores (-2, -1, 1, 2) está obsoleto e o suporte será removido em um futuro próximo.

20260207: O comportamento i2c de baixo nível de dispositivos sx1509 e uc1701 mudou. Anteriormente um erro de i2c resultava em desligamento, e agora erros de i2c ao comunicar com esses dispositivos gerarão apenas avisos no arquivo de registro (log).

20260109: O valor de status `{printer.probe.last_z_result}` está obsoleto; será removido em um futuro próximo. Use `{printer.probe.last_probe_position}` em vez disso, e observe que esse novo valor já tem os deslocamentos xyz configurados da sonda aplicados.

20260109: A saída de texto do console de g-code dos comandos `PROBE`, `PROBE_ACCURACY` e similares mudou. Agora as alturas Z são relatadas em relação à posição Z nominal da mesa em vez de relativas ao `z_offset` configurado da sonda. Da mesma forma, relatórios intermediários de x e y da sonda no console também terão o `x_offset` e o `y_offset` configurados da sonda aplicados.

20260109: O módulo `[screws_tilt_adjust]` agora relata a variável de status `{printer.screws_tilt_adjust.result.screw1.z}` com o `z_offset` da sonda aplicado. Ou seja, anteriormente era necessário subtrair o `z_offset` configurado da sonda para encontrar o desvio Z absoluto na localização do parafuso em questão e agora não se deve mais aplicar o `z_offset`.

20251122: Uma opção `axis` foi adicionada às seções `[carriage <name>]` para cinemática `generic_cartesian`, permitindo nomes arbitrários para carros primários. Recomenda-se aos usuários especificar explicitamente a opção `axis` agora.

20251106: Os campos de status `{printer.toolhead.position}`, `{printer.gcode_move.position}`, `{printer.gcode_move.gcode_position}` e `{printer.motion_report.live_position}` estão mudando. Essas coordenadas antes sempre continham quatro componentes, mas agora podem conter componentes adicionais. A ordem e o número de componentes podem mudar em tempo de execução - veja a [referência de status](Status_Reference.md#accessing-coordinates) para detalhes importantes. Acessar qualquer uma dessas coordenadas em macros usando o acessador ".e" está obsoleto - use algo como `{printer.toolhead.position[printer.gcode_move.axis_map.E]}` como alternativa.

20251106: Os campos de status `{printer.gcode_move.homing_origin}`, `{printer.toolhead.axis_min}` e `{printer.toolhead.axis_max}` atualmente contêm quatro componentes onde o quarto componente é sempre zero. Esse comportamento está obsoleto. No futuro essas coordenadas podem conter apenas três componentes. Para informações adicionais veja a [referência de status](Status_Reference.md#accessing-coordinates).

20251010: Durante a impressão normal o processamento de comandos agora tentará se manter um segundo à frente do movimento da impressora (reduzido de dois segundos anteriormente).

20251003: O suporte à opção não documentada `max_stepper_error` na seção de configuração `[printer]` foi removido.

20250916: As definições dos modeladores de entrada (input shapers) EI, 2HUMP_EI e 3HUMP_EI foram atualizadas. Para melhor desempenho recomenda-se recalibrar os modeladores de entrada, especialmente se alguns desses modeladores estiverem em uso atualmente.

20250811: O suporte ao parâmetro `max_accel_to_decel` na seção de configuração `[printer]` foi removido e o suporte ao parâmetro `ACCEL_TO_DECEL` no comando `SET_VELOCITY_LIMIT` foi removido. Essas capacidades estavam obsoletas desde 20240313.

20250721: Os módulos `[pca9632]` e `[mcp4018]` não aceitam mais as opções `scl_pin` e `sda_pin`. Use `i2c_software_scl_pin` e `i2c_software_sda_pin` em vez disso.

20250428: O `cycle_time` máximo para `[output_pin]`, `[pwm_cycle_time]`, `[pwm_tool]` pwm e seções de configuração semelhantes agora é de 3 segundos (reduzido de 5 segundos). O `maximum_mcu_duration` em `[pwm_tool]` agora também é de 3 segundos.

20250418: O recurso `STOP_ON_ENDSTOP` de manual_stepper agora pode levar menos tempo para concluir. Anteriormente, o comando aguardava todo o tempo que o movimento poderia possivelmente levar mesmo que o fim de curso tivesse disparado antes. Agora, o comando termina logo após o disparo do fim de curso.

20250417: Dispositivos SPI usando "SPI por software" agora têm limitação de taxa. Anteriormente, o `spi_speed` na configuração era ignorado e a velocidade de transmissão era limitada apenas pela velocidade de processamento do microcontrolador. Agora, as velocidades são limitadas pelo parâmetro de configuração `spi_speed` (as velocidades reais de hardware provavelmente serão menores que o valor configurado devido à sobrecarga de software).

20250411: Klipper v0.13.0 lançado.

20250308: O parâmetro `AUTO` do comando `AXIS_TWIST_COMPENSATION_CALIBRATE` foi removido.

20250131: A opção `VARIABLE=<name>` em `SAVE_VARIABLE` exige valor em minúsculas. Por exemplo, `extruder` em vez de `Extruder` com maiúsculas mistas ou `EXTRUDER` em maiúsculas. Usar qualquer letra maiúscula gerará um erro.

20241203: O teste de ressonância foi alterado para incluir movimentos lentos de varredura. Essa alteração exige que o(s) ponto(s) de teste tenha(m) alguma folga no plano X/Y (+/- 30 mm a partir do ponto de teste deve ser suficiente ao usar as configurações padrão). O novo teste geralmente deve produzir resultados de teste mais precisos e confiáveis. No entanto, se necessário, o comportamento anterior do teste pode ser restaurado adicionando as opções `sweeping_period: 0` e `accel_per_hz: 75` à seção de configuração `[resonance_tester]`.

20241201: Em alguns casos o Klipper pode ter ignorado caracteres iniciais ou espaços em um comando G-Code tradicional. Por exemplo, "99M123" pode ter sido interpretado como "M123" e "M 321" pode ter sido interpretado como "M321". O Klipper agora relatará esses casos com um aviso de "comando desconhecido".

20241112: A opção `CHIPS=<chip_name>` em `TEST_RESONANCES` e `SHAPER_CALIBRATE` exige especificar o(s) nome(s) completo(s) do(s) chip(s) de aceleração. Por exemplo, `adxl345 rpi` em vez de nome curto - `rpi`.

20240912: Os comandos `SET_PIN`, `SET_SERVO`, `SET_FAN_SPEED`, `M106` e `M107` agora são agrupados (collated). Anteriormente, se muitas atualizações para o mesmo objeto fossem emitidas mais rápido que o tempo mínimo de agendamento (tipicamente 100ms), as atualizações reais poderiam ficar enfileiradas para um futuro distante. Agora, se muitas atualizações forem emitidas em rápida sucessão, é possível que apenas a solicitação mais recente seja aplicada. Se o comportamento anterior for necessário, considere adicionar comandos explícitos de atraso `G4` entre as atualizações.

20240912: O suporte aos parâmetros `maximum_mcu_duration` e `static_value` em seções de configuração `[output_pin]` foi removido. Essas opções estavam obsoletas desde 20240123.

20240415: O parâmetro `on_error_gcode` na seção de configuração `[virtual_sdcard]` agora tem um padrão. Se esse parâmetro não for especificado, agora o padrão é `TURN_OFF_HEATERS`. Se o comportamento anterior for desejado (não realizar nenhuma ação padrão em caso de erro durante uma impressão via virtual_sdcard), defina `on_error_gcode` com um valor vazio.

20240313: O parâmetro `max_accel_to_decel` na seção de configuração `[printer]` foi descontinuado. O parâmetro `ACCEL_TO_DECEL` do comando `SET_VELOCITY_LIMIT` foi descontinuado. O status `printer.toolhead.max_accel_to_decel` foi removido. Use o parâmetro [minimum_cruise_ratio](Config_Reference.md#printer) em vez disso. Os recursos obsoletos serão removidos em um futuro próximo, e usá-los nesse ínterim pode resultar em comportamento sutilmente diferente.

20240215: Vários recursos obsoletos foram removidos. Usar "NTC 100K beta 3950" como nome de termistor foi removido (obsoleto em 20211110). Os comandos `SYNC_STEPPER_TO_EXTRUDER` e `SET_EXTRUDER_STEP_DISTANCE` foram removidos, e a opção de configuração `shared_heater` da extrusora foi removida (obsoleta em 20220210). A opção `relative_reference_index` de bed_mesh foi removida (obsoleta em 20230619).

20240123: O parâmetro CYCLE_TIME de SET_PIN de output_pin foi removido. Use o novo módulo [pwm_cycle_time](Config_Reference.md#pwm_cycle_time) se for necessário alterar dinamicamente o tempo de ciclo de um pino pwm.

20240123: O parâmetro `maximum_mcu_duration` de output_pin está obsoleto. Use uma [seção de configuração pwm_tool](Config_Reference.md#pwm_tool) em vez disso. A opção será removida em um futuro próximo.

20240123: O parâmetro `static_value` de output_pin está obsoleto. Substitua pelos parâmetros `value` e `shutdown_value`. A opção será removida em um futuro próximo.

20231216: O `[hall_filament_width_sensor]` foi alterado para disparar falta de filamento quando a espessura do filamento excede `max_diameter`. O diâmetro máximo usa como padrão `default_nominal_filament_diameter + max_difference`. Veja a [referência de configuração [hall_filament_width_sensor]](Config_Reference.md#hall_filament_width_sensor) para mais detalhes.

20231207: Vários parâmetros de configuração não documentados na seção de configuração `[printer]` foram removidos (os parâmetros buffer_time_low, buffer_time_high, buffer_time_start e move_flush_time).

20231110: Klipper v0.12.0 lançado.

20230826: Se `safe_distance` for definido ou calculado como 0 em `[dual_carriage]`, as verificações de proximidade dos carros serão desativadas conforme a documentação. O usuário pode desejar configurar `safe_distance` explicitamente para evitar colisões acidentais dos carros entre si. Além disso, a ordem de referenciamento do carro primário e do carro duplo mudou em algumas configurações (certas configurações quando ambos os carros fazem referenciamento na mesma direção, veja a [referência de configuração [dual_carriage]](Config_Reference.md#dual_carriage) para mais detalhes).

20230810: O script flash-sdcard.sh agora suporta ambas as variantes da Bigtreetech SKR-3, STM32H743 e STM32H723. Para isso, a etiqueta original de btt-skr-3 mudou para btt-skr-3-h743 ou btt-skr-3-h723.

20230729: O status exportado para `dual_carriage` mudou. Em vez de exportar `mode` e `active_carriage`, os modos individuais de cada carro são exportados como `printer.dual_carriage.carriage_0` e `printer.dual_carriage.carriage_1`.

20230619: A opção `relative_reference_index` foi descontinuada e substituída pela opção `zero_reference_position`. Consulte a [documentação de malha da mesa](Bed_Mesh.md#the-deprecated-relative_reference_index) para detalhes sobre como atualizar a configuração. Com essa descontinuação, `RELATIVE_REFERENCE_INDEX` não está mais disponível como parâmetro para o comando gcode `BED_MESH_CALIBRATE`.

20230530: A frequência canbus padrão em "make menuconfig" agora é 1000000. Se usar canbus e for necessário usar canbus com alguma outra frequência, certifique-se de selecionar "Enable extra low-level configuration options" e especificar a "CAN bus speed" desejada em "make menuconfig" ao compilar e gravar o microcontrolador.

20230525: O comando `SHAPER_CALIBRATE` aplica imediatamente os parâmetros do modelador de entrada se `[input_shaper]` já estava habilitado.

20230407: O contador `stalled_bytes` no registro (log) e no campo `printer.mcu.last_stats` foi renomeado para `upcoming_bytes`.

20230323: Em drivers tmc5160, `multistep_filt` agora está habilitado por padrão. Defina `driver_MULTISTEP_FILT: False` na configuração de tmc5160 para o comportamento anterior.

20230304: O comando `SET_TMC_CURRENT` agora ajusta corretamente o registrador globalscaler para drivers que o possuem. Isso remove uma limitação onde, em tmc5160, as correntes não podiam ser elevadas via `SET_TMC_CURRENT` acima do valor de `run_current` definido no arquivo de configuração. No entanto, isso tem um efeito colateral: após executar `SET_TMC_CURRENT`, o motor de passo deve ser mantido parado por >130ms caso StealthChop2 seja usado, para que a calibração AT#1 seja executada pelo driver.

20230202: O formato das informações de status de `printer.screws_tilt_adjust` mudou. As informações agora são armazenadas como um dicionário de parafusos com as medições resultantes. Veja a [referência de status](Status_Reference.md#screws_tilt_adjust) para detalhes.

20230201: O módulo `[bed_mesh]` não carrega mais o perfil `default` na inicialização. Recomenda-se que usuários que usam o perfil `default` adicionem `BED_MESH_PROFILE LOAD=default` à sua macro `START_PRINT` (ou à configuração "Start G-Code" do fatiador, quando aplicável).

20230103: Agora é possível com o script flash-sdcard.sh gravar ambas as variantes da Bigtreetech SKR-2, STM32F407 e STM32F429. Isso significa que a etiqueta original de btt-skr2 mudou para btt-skr-2-f407 ou btt-skr-2-f429.

20221128: Klipper v0.11.0 lançado.

20221122: Anteriormente, com safe_z_home, era possível que o z_hop após o referenciamento g28 ocorresse na direção Z negativa. Agora, um z_hop é realizado após g28 somente se resultar em um salto positivo, espelhando o comportamento do z_hop que ocorre antes do referenciamento g28.

20220616: Anteriormente era possível gravar um rp2040 em modo bootloader executando `make flash FLASH_DEVICE=first`. O comando equivalente agora é `make flash FLASH_DEVICE=2e8a:0003`.

20220612: O microcontrolador rp2040 agora tem uma solução de contorno para a errata USB "rp2040-e5". Isso deve tornar as conexões USB iniciais mais confiáveis. No entanto, pode resultar em alteração de comportamento para o pino gpio15. É improvável que a alteração de comportamento de gpio15 seja perceptível.

20220407: A opção de configuração `pid_integral_max` de temperature_fan foi removida (estava obsoleta desde 20210612).

20220407: A ordem de cores padrão para LEDs pca9632 agora é "RGBW". Adicione uma configuração explícita `color_order: RBGW` à seção de configuração pca9632 para obter o comportamento anterior.

20220330: O formato das informações de status de `printer.neopixel.color_data` para módulos neopixel e dotstar mudou. As informações agora são armazenadas como uma lista de listas de cores (em vez de uma lista de dicionários). Veja a [referência de status](Status_Reference.md#led) para detalhes.

20220307: `M73` não definirá mais o progresso de impressão como 0 se `P` estiver ausente.

20220304: Não há mais um padrão para o parâmetro `extruder` de seções de configuração [extruder_stepper](Config_Reference.md#extruder_stepper). Se desejado, especifique `extruder: extruder` explicitamente para associar o motor de passo à fila de movimento "extruder" na inicialização.

20220210: O comando `SYNC_STEPPER_TO_EXTRUDER` está obsoleto; o comando `SET_EXTRUDER_STEP_DISTANCE` está obsoleto; a opção de configuração `shared_heater` de [extruder](Config_Reference.md#extruder) está obsoleta. Esses recursos serão removidos em um futuro próximo. Substitua `SET_EXTRUDER_STEP_DISTANCE` por `SET_EXTRUDER_ROTATION_DISTANCE`. Substitua `SYNC_STEPPER_TO_EXTRUDER` por `SYNC_EXTRUDER_MOTION`. Substitua seções de configuração de extrusora usando `shared_heater` por seções de configuração [extruder_stepper](Config_Reference.md#extruder_stepper) e atualize quaisquer macros de ativação para usar [SYNC_EXTRUDER_MOTION](G-Codes.md#sync_extruder_motion).

20220116: O código de cálculo de `run_current` de tmc2130, tmc2208, tmc2209 e tmc2660 mudou. Para algumas configurações de `run_current` os drivers agora podem ser configurados de forma diferente. Essa nova configuração deve ser mais precisa, mas pode invalidar o ajuste anterior do driver tmc.

20211230: Scripts para ajustar o modelador de entrada (`scripts/calibrate_shaper.py` e `scripts/graph_accelerometer.py`) foram migrados para usar Python3 por padrão. Como resultado, os usuários devem instalar versões Python3 de certos pacotes (por ex. `sudo apt install python3-numpy python3-matplotlib`) para continuar usando esses scripts. Para mais detalhes, consulte [Instalação de software](Measuring_Resonances.md#software-installation). Alternativamente, os usuários podem forçar temporariamente a execução desses scripts sob Python 2 chamando explicitamente o interpretador Python2 no console: `python2 ~/klipper/scripts/calibrate_shaper.py ...`

20211110: O sensor de temperatura "NTC 100K beta 3950" está obsoleto. Esse sensor será removido em um futuro próximo. A maioria dos usuários achará o sensor de temperatura "Generic 3950" mais preciso. Para continuar usando a definição antiga (tipicamente menos precisa), defina um [termistor](Config_Reference.md#thermistor) personalizado com `temperature1: 25`, `resistance1: 100000` e `beta: 3950`.

20211104: A opção "step pulse duration" em "make menuconfig" foi removida. A duração padrão de passo para drivers TMC configurados em modo UART ou SPI agora é 100ns. Uma nova configuração `step_pulse_duration` na [seção de configuração stepper](Config_Reference.md#stepper) deve ser definida para todos os motores de passo que precisarem de duração de pulso personalizada.

20211102: Vários recursos obsoletos foram removidos. A opção `step_distance` do motor de passo foi removida (obsoleta em 20201222). O apelido de sensor `rpi_temperature` foi removido (obsoleto em 20210219). A opção `pin_map` de mcu foi removida (obsoleta em 20210325). O `default_parameter_<name>` de gcode_macro e o acesso de macro a parâmetros de comando exceto via pseudo-variável `params` foram removidos (obsoletos em 20210503). A opção `pid_integral_max` do aquecedor foi removida (obsoleta em 20210612).

20210929: Klipper v0.10.0 lançado.

20210903: O [`smooth_time`](Config_Reference.md#extruder) padrão para aquecedores mudou para 1 segundo (de 2 segundos). Para a maioria das impressoras isso resultará em controle de temperatura mais estável.

20210830: O nome padrão de adxl345 agora é "adxl345". O parâmetro CHIP padrão para `ACCELEROMETER_MEASURE` e `ACCELEROMETER_QUERY` agora também é "adxl345".

20210830: O comando ACCELEROMETER_MEASURE de adxl345 não suporta mais um parâmetro RATE. Para alterar a taxa de consulta, atualize o arquivo printer.cfg e emita um comando RESTART.

20210821: Várias configurações em `printer.configfile.settings` agora serão relatadas como listas em vez de strings brutas. Se a string bruta real for desejada, use `printer.configfile.config` em vez disso.

20210819: Em alguns casos, um movimento de referenciamento `G28` pode terminar em uma posição nominalmente fora da faixa válida de movimento. Em situações raras isso pode resultar em erros confusos de "movimento fora da faixa" após o referenciamento. Se isso ocorrer, altere seus scripts iniciais para mover o cabeçote para uma posição válida imediatamente após o referenciamento.

20210814: Os pseudo-pinos somente analógicos nos atmega168 e atmega328 foram renomeados de PE0/PE1 para PE2/PE3.

20210720: Uma seção controller_fan agora monitora todos os motores de passo por padrão (não apenas os motores de passo cinemáticos). Se o comportamento anterior for desejado, veja a opção de configuração `stepper` na [referência de configuração](Config_Reference.md#controller_fan).

20210703: Uma seção de configuração `samd_sercom` agora deve especificar o barramento sercom que está configurando por meio da opção `sercom`.

20210612: A opção de configuração `pid_integral_max` nas seções "heater" e "temperature_fan" foram descontinuadas. A opção será removida em um futuro próximo.

20210503: A opção de configuração `default_parameter_<name>` do gcode_macro está obsoleta. Use a pseudovariável `params` para acessar os parâmetros da macro. Outros métodos de acesso aos parâmetros da macro serão removidos em breve. A maioria dos usuários pode substituir uma opção de configuração `default_parameter_NAME: VALUE` por uma linha como a seguinte no início da macro: ` {% set NAME = params.NAME|default(VALUE)|float %}`. Veja exemplos no [documento Modelos de Comando](Command_Templates.md#macro-parameters).

20210430: O comando SET_VELOCITY_LIMIT (e M204) agora podem definir velocidade, aceleração e "square_corner_velocity" maiores que os valores especificados no arquivo de configuração.

20210325: Suporte para a configuração `pin_map` descontinuado. Use o arquivo [sample-aliases.cfg](../../config/sample-aliases.cfg) para traduzir para os nomes reais dos pinos do microcontrolador. A opção de configuração `pin_map` será removida em breve.

20210313: O suporte do Klipper a microcontroladores que se comunicam com barramento CAN mudou. Se estiver usando barramento CAN, todos os microcontroladores devem ser regravados e a [configuração do Klipper deve ser atualizada](CANBUS.md).

20210310: O TMC2660 padrão para o driver_SFILT foi alterado de 1 para 0.

20210227: Drivers de motor de passo TMC em modo UART ou SPI agora são consultados uma vez por segundo sempre que estiverem habilitados - se não for possível contatar o driver ou se o driver relatar um erro, o Klipper passará para um estado de desligamento.

20210219: O módulo `rpi_temperature` foi renomeado para `temperature_host`. Substitua qualquer ocorrência de `sensor_type: rpi_temperature` por `sensor_type: temperature_host`. O caminho para o arquivo de temperatura pode ser especificado na variável de configuração `sensor_path`. O nome `rpi_temperature` está obsoleto e será removido em breve.

20210201: O comando `TEST_RESONANCES` agora desativará a modelagem de entrada (input shaping) se ela estava habilitada anteriormente (e a reabilitará após o teste). Para substituir esse comportamento e manter a modelagem de entrada habilitada, pode-se passar um parâmetro adicional `INPUT_SHAPING=1` ao comando.

20210201: O comando `ACCELEROMETER_MEASURE` agora anexará o nome do chip acelerômetro ao nome do arquivo de saída se o chip recebeu um nome na seção adxl345 correspondente do printer.cfg.

20201222: A configuração `step_distance` nas seções de configuração de motores de passo está obsoleta. É aconselhável atualizar a configuração para usar a configuração [`rotation_distance`](Rotation_Distance.md). O suporte a `step_distance` será removido em breve.

20201218: A configuração `endstop_phase` no módulo endstop_phase foi substituída por `trigger_phase`. Se estiver usando o módulo de fases de fim de curso, será necessário converter para [`rotation_distance`](Rotation_Distance.md) e recalibrar quaisquer fases de fim de curso executando o comando ENDSTOP_PHASE_CALIBRATE.

20201218: Impressoras delta rotativas e polares agora devem especificar um `gear_ratio` para seus motores de passo rotativos, e não podem mais especificar um parâmetro `step_distance`. Veja o formato do novo parâmetro gear_ratio na [referência de configuração](Config_Reference.md#stepper).

20201213: Não é válido especificar um "position_endstop" de Z ao usar "probe:z_virtual_endstop". Um erro agora será gerado se um "position_endstop" de Z for especificado com "probe:z_virtual_endstop". Remova a definição de "position_endstop" de Z para corrigir o erro.

20201120: A seção de configuração `[board_pins]` agora especifica o nome do mcu em um parâmetro explícito `mcu:`. Se estiver usando board_pins para um mcu secundário, a configuração deve ser atualizada para especificar esse nome. Veja mais detalhes na [referência de configuração](Config_Reference.md#board_pins).

20201112: O tempo reportado por `print_stats.print_duration` foi alterado. A duração anterior a primeira extrusão detectada foi excluída.

20201029: A opção de configuração `color_order_GRB` do neopixel foi removida. Se necessário, atualize a configuração para definir a nova opção `color_order` como RGB, GRB, RGBW ou GRBW.

20201029: A opção serial na seção de configuração do mcu não usa mais /dev/ttyS0 como padrão. Na rara situação em que /dev/ttyS0 for a porta serial desejada, ela deve ser especificada explicitamente.

20201020: Klipper v0.9.0 lançado.

20200902: O cálculo de conversão de resistência para temperatura do RTD para conversores MAX31865 foi corrigido para não ler baixo. Se você estiver usando tal dispositivo, deve recalibrar sua temperatura de impressão e as configurações de PID.

20200816: O objeto `printer.gcode` da macro de gcode foi renomeado para `printer.gcode_move`. Várias variáveis não documentadas em `printer.toolhead` e `printer.gcode` foram removidas. Veja docs/Command_Templates.md para uma lista de variáveis de modelo disponíveis.

20200816: O sistema "action_" da macro de gcode mudou. Substitua quaisquer chamadas para `printer.gcode.action_emergency_stop()` por `action_emergency_stop()`, `printer.gcode.action_respond_info()` por `action_respond_info()` e `printer.gcode.action_respond_error()` por `action_raise_error()`.

20200809: O sistema de menus foi reescrito. Se o menu foi personalizado, será necessário atualizar para a nova configuração. Veja config/example-menu.cfg para detalhes de configuração e veja klippy/extras/display/menu.cfg para exemplos.

20200731: O comportamento do atributo `progress` informado pelo objeto de impressora `virtual_sdcard` mudou. O progresso não é mais redefinido para 0 quando uma impressão é pausada. Agora ele sempre informará o progresso com base na posição interna do arquivo, ou 0 se nenhum arquivo estiver carregado no momento.

20200725: O parâmetro de configuração `enable` do servo e o parâmetro `ENABLE` do SET_SERVO foram removidos. Atualize quaisquer macros para usar `SET_SERVO SERVO=my_servo WIDTH=0` para desativar um servo.

20200608: O suporte a tela de LCD mudou o nome de alguns "glifos" internos. Se um leiaute de tela personalizado foi implementado, pode ser necessário atualizar para os nomes de glifo mais recentes (veja klippy/extras/display/display.cfg para uma lista de glifos disponíveis).

20200606: Os nomes de pinos no mcu linux mudaram. Os pinos agora têm nomes no formato `gpiochip<chipid>/gpio<gpio>`. Para gpiochip0 você também pode usar um `gpio<gpio>` curto. Por exemplo, o que antes era referido como `P20` agora se torna `gpio20` ou `gpiochip0/gpio20`.

20200603: O leiaute padrão de LCD 16x4 não mostrará mais o tempo restante estimado em uma impressão. (Somente o tempo decorrido será mostrado.) Se o comportamento antigo for desejado, pode-se personalizar a exibição do menu com essa informação (veja a descrição de display_data em config/example-extras.cfg para detalhes).

20200531: O id padrão de fornecedor/produto USB agora é 0x1d50/0x614e. Esses novos ids são reservados para o Klipper (agradecimentos ao projeto openmoko). Essa mudança não deve exigir alterações de configuração, mas os novos ids podem aparecer nos registros do sistema.

20200524: O valor padrão para o campo pwm_freq do tmc5160 agora é zero (em vez de um).

20200425: A variável de modelo de comando gcode_macro `printer.heater` foi renomeada para `printer.heaters`.

20200313: O leiaute padrão de lcd para impressoras com múltiplas extrusoras com tela 16x4 mudou. O leiaute de tela de extrusora única agora é o padrão e mostrará a extrusora atualmente ativa. Para usar o leiaute de exibição anterior, defina "display_group: _multiextruder_16x4" na seção [display] do arquivo printer.cfg.

20200308: O item de menu padrão `__test` foi removido. Se o arquivo de configuração tiver um menu personalizado, remova todas as referências a esse item de menu `__test`.

20200308: As opções "deck" e "card" do menu foram removidas. Para personalizar o leiaute de uma tela de lcd, use as novas seções de configuração display_data (veja config/example-extras.cfg para detalhes).

20200109: O módulo bed_mesh agora referencia a localização da sonda para a configuração da malha. Assim, algumas opções de configuração foram renomeadas para refletir com mais precisão sua funcionalidade pretendida. Para mesas retangulares, `min_point` e `max_point` foram renomeados para `mesh_min` e `mesh_max`, respectivamente. Para mesas redondas, `bed_radius` foi renomeado para `mesh_radius`. Uma nova opção `mesh_origin` também foi adicionada para mesas redondas. Observe que essas mudanças também são incompatíveis com perfis de malha salvos anteriormente. Se um perfil incompatível for detectado, ele será ignorado e agendado para remoção. O processo de remoção pode ser concluído emitindo o comando SAVE_CONFIG. O usuário precisará recalibrar cada perfil.

20191218: A seção de configuração de tela não suporta mais "lcd_type: st7567". Use o tipo de tela "uc1701" em vez disso - defina "lcd_type: uc1701" e altere o "rs_pin: some_pin" para "rst_pin: some_pin". Também pode ser necessário adicionar uma configuração "contrast: 60".

20191210: Os comandos integrados T0, T1, T2, ... foram removidos. As opções de configuração activate_gcode e deactivate_gcode da extrusora foram removidas. Se esses comandos (e scripts) forem necessários, defina macros individuais estilo [gcode_macro T0] que chamem o comando ACTIVATE_EXTRUDER. Veja os arquivos config/sample-idex.cfg e sample-multi-extruder.cfg para exemplos.

20191210: O suporte ao comando M206 foi removido. Substitua por chamadas para SET_GCODE_OFFSET. Se o suporte a M206 for necessário, adicione uma seção de configuração [gcode_macro M206] que chame SET_GCODE_OFFSET. (Por exemplo "SET_GCODE_OFFSET Z=-{params.Z}".)

20191202: O suporte ao parâmetro "S" não documentado do comando "G4" foi removido. Substitua qualquer ocorrência de S pelo parâmetro padrão "P" (o atraso especificado em milissegundos).

20191126: Os nomes USB mudaram em microcontroladores com suporte USB nativo. Eles agora usam um id de chip exclusivo por padrão (quando disponível). Se uma seção de configuração "mcu" usar uma configuração "serial" que comece com "/dev/serial/by-id/", pode ser necessário atualizar a configuração. Execute "ls /dev/serial/by-id/*" em um terminal ssh para determinar o novo id.

20191121: O parâmetro pressure_advance_lookahead_time foi removido. Veja example.cfg para configurações alternativas.

20191112: A capacidade de habilitação virtual do driver de motor de passo tmc agora é habilitada automaticamente se o motor de passo não tiver um pino dedicado de habilitação do motor. Remova referências a tmcXXXX:virtual_enable da configuração. A capacidade de controlar múltiplos pinos na configuração enable_pin do motor foi removida. Se múltiplos pinos forem necessários, use uma seção de configuração multi_pin.

20191107: A seção de configuração da extrusora primária deve ser especificada como "extruder" e não pode mais ser especificada como "extruder0". Modelos de comando Gcode que consultam o status da extrusora agora são acessados via "{printer.extruder}".

20191021: Klipper v0.8.0 lançado

20191003: A opção move_to_previous em [safe_z_homing] agora usa False como padrão. (Era efetivamente False antes de 20190918.)

20190918: A opção zhop em [safe_z_homing] é sempre reaplicada após a conclusão do referenciamento (homing) do eixo Z. Isso pode exigir que usuários atualizem scripts personalizados baseados neste módulo.

20190806: O comando SET_NEOPIXEL foi renomeado para SET_LED.

20190726: O código digital-para-analógico do mcp4728 mudou. O i2c_address padrão agora é 0x60 e a referência de tensão agora é relativa à referência interna de 2,048 volts do mcp4728.

20190710: A opção z_hop foi removida da seção de configuração [firmware_retract]. O suporte a z_hop estava incompleto e podia causar comportamento incorreto com vários fatiadores comuns.

20190710: Os parâmetros opcionais do comando PROBE_ACCURACY mudaram. Pode ser necessário atualizar quaisquer macros ou scripts que usem esse comando.

20190628: Todas as opções de configuração foram removidas da seção [skew_correction]. A configuração para skew_correction agora é feita via gcode SET_SKEW. Veja [Correção de Inclinação](Skew_Correction.md) para o uso recomendado.

20190607: Os parâmetros "variable_X" do gcode_macro (junto com o parâmetro VALUE de SET_GCODE_VARIABLE) agora são analisados como literais Python. Se um valor precisar receber uma string, envolva o valor entre aspas para que seja avaliado como string.

20190606: As opções de configuração "samples", "samples_result" e "sample_retract_dist" foram movidas para a seção de configuração "probe". Essas opções não são mais suportadas nas seções de configuração "delta_calibrate", "bed_tilt", "bed_mesh", "screws_tilt_adjust", "z_tilt" ou "quad_gantry_level".

20190528: A variável mágica "status" na avaliação de modelo do gcode_macro foi renomeada para "printer".

20190520: O comando SET_GCODE_OFFSET mudou; atualize quaisquer macros de g-code de acordo. O comando não aplicará mais o deslocamento solicitado ao próximo comando G1. O comportamento antigo pode ser aproximado usando o novo parâmetro "MOVE=1".

20190404: Os pacotes de software host Python foram atualizados. Os usuários precisarão executar novamente o script ~/klipper/scripts/install-octopi.sh (ou atualizar de outra forma as dependências python se não estiverem usando uma instalação OctoPi padrão).

20190404: Os parâmetros i2c_bus e spi_bus (em várias seções de configuração) agora recebem um nome de barramento em vez de um número.

20190404: Os parâmetros de configuração do sx1509 mudaram. O parâmetro 'address' agora é 'i2c_address' e deve ser especificado como um número decimal. Onde 0x3E era usado anteriormente, especifique 62.

20190328: O valor min_speed em configuração [temperature_fan] agora será respeitado e a ventoinha sempre funcionará nessa velocidade ou superior no modo PID.

20190322: O valor padrão para "driver_HEND" nas seções de configuração [tmc2660] foi alterado de 6 para 3. O campo "driver_VSENSE" foi removido (agora é calculado automaticamente a partir de run_current).

20190310: A seção de configuração [controller_fan] agora sempre recebe um nome (como [controller_fan my_controller_fan]).

20190308: O campo "driver_BLANK_TIME_SELECT" nas seções de configuração [tmc2130] e [tmc2208] foi renomeado para "driver_TBL".

20190308: A seção de configuração [tmc2660] mudou. Um novo parâmetro de configuração sense_resistor agora deve ser fornecido. O significado de vários parâmetros driver_XXX mudou.

20190228: Usuários de SPI ou I2C em placas SAMD21 agora devem especificar os pinos do barramento via seção de configuração [samd_sercom].

20190224: A opção bed_shape foi removida do bed_mesh. A opção radius foi renomeada para bed_radius. Usuários com mesas redondas devem fornecer as opções bed_radius e round_probe_count.

20190107: O parâmetro i2c_address na seção de configuração mcp4451 mudou. Esta é uma configuração comum em Smoothieboards. O novo valor é metade do valor antigo (88 deve ser alterado para 44, e 90 deve ser alterado para 45).

20181220: Klipper v0.7.0 lançado
