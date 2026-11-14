<a id="releases"></a>
# Lançamentos

[English](../Releases.md) | **Português brasileiro**

Histórico de atualizações do Klipper. Por favor, consulte [installation](Installation.md) para mais informação de instalação do Klipper.

<a id="klipper-0130"></a>
## Klipper 0.13.0

Disponível em 20250411. Principais alterações nesta versão:

* Novo mecanismo de teste de ressonância por "vibrações de varredura" para o modelador de entrada.
* Ventiladores e pinos GPIO agora podem receber uma fórmula (via "templates" do Jinja2).
* O código bed_mesh agora aceita "malha de mesa adaptativa". A área sondada pode ser ajustada ao tamanho da impressão.
* Um novo parâmetro cinemático `minimum_cruise_ratio` foi adicionado (ele substitui o parâmetro anterior `max_accel_to_decel`).
* Vários sensores novos adicionados:
   * Suporte para sensores de corrente "eddy" ldc1612. Isso inclui suporte a sondagem, sondagem rápida por "varredura" e calibração de temperatura.
   * Novo suporte para medições de "célula de carga". Suporte para conectar essas células de carga a sensores ADC hx71x e ads1220.
   * Suporte para sensores de temperatura BMP180, BMP388 e SHT3x. Suporte para medir temperatura com chips ADC ADS1x1x.
   * Novo suporte a acelerômetros lis3dh e icm20948.
   * Suporte para sensores de “ângulo hall” mt6816 e mt6826s.
* Novas melhorias de microcontrolador:
   * Novo suporte para microcontroladores rp2350.
   * Os chips rp2040 existentes agora operam a 200MHz (antes 125Mhz).
   * O código do microcontrolador agora pode definir muito mais comandos (até 16384, antes 128).
* Outros módulos adicionados: aip31068_spi, canbus_stats, error_mcu, garbage_collection, pwm_cycle_time, pwm_tool, garbage_collection.
* Diversas correções de erros e limpezas de código.

<a id="klipper-0120"></a>
## Klipper 0.12.0

Disponível em 20231110. Principais alterações nesta versão:

* Suporte para modos COPY e MIRROR em impressoras IDEX.
* Diversas melhorias de microcontrolador:
   * Suporte para novas arquiteturas ar100 e hc32f460.
   * Suporte para variantes de chip stm32f7, stm32g0b0, stm32g07x, stm32g4, stm32h723, n32g45x, samc21 e samd21j18.
   * Tratamento aprimorado de reinicialização DFU e Katapult.
   * Desempenho aprimorado no modo de ponte USB para barramento CAN.
   * Desempenho aprimorado no “mcu linux”.
   * Novo suporte para i2c baseado em software.
* Novo suporte de hardware para drivers de motor de passo tmc2240, acelerômetros lis2dw12 e sensores de temperatura aht10.
* Novos módulos axis_twist_compensation e temperature_combined adicionados.
* Novo suporte para arcos de G-code nos planos XY, XZ e YZ.
* Diversas correções de erros e limpezas de código.

<a id="klipper-0110"></a>
## Klipper 0.11.0

Disponível em 20221128. Principais alterações nesta versão:

* Otimização “passo em ambas as bordas” dos drivers de motor de passo Trinamic.
* Suporte para Python3. O código hospedeiro do Klipper será executado com Python2 ou Python3.
* Suporte aprimorado ao barramento CAN. Suporte para barramento CAN em chips rp2040, stm32g0, stm32h7, same51 e same54. Suporte para modo de “ponte USB para barramento CAN”.
* Suporte para gerenciador de inicialização CanBoot.
* Suporte para acelerômetros mpu9250 e mpu6050.
* Tratamento de erros aprimorado para sensores de temperatura max31856, max31855, max31865 e max6675.
* Agora é possível configurar LEDs para atualização durante comandos G-Code de longa duração usando suporte a “modelo” de LED.
* Diversas melhorias de microcontrolador. Novo suporte para chips stm32h743, stm32h750, stm32l412, stm32g0b1, same70, same51 e same54. Suporte para leituras i2c em atsamd e stm32f0. Suporte a pwm por hardware em stm32. Despacho de eventos baseado em sinal no mcu Linux. Novo suporte a rp2040 para “make flash”, i2c e errata USB rp2040-e5.
* Novos módulos adicionados: angle, dac084S085, exclude_object, led, mpu9250, pca9632, smart_effector, z_thermal_adjust. Nova cinemática deltesian adicionada. Nova ferramenta dump_mcu adicionada.
* Diversas correções de erros e limpezas de código.

<a id="klipper-0100"></a>
## Klipper 0.10.0

Disponível em 20210929. Principais alterações nesta versão:

* Suporte para “referenciamento multi-MCU”. Agora é possível que um motor de passo e seu fim de curso estejam conectados a microcontroladores separados. Isso simplifica a fiação de sondas Z em “placas de cabeçote”.
* O Klipper agora tem um [Servidor Discord da comunidade](https://discord.klipper3d.org) e um [Servidor Discourse da comunidade](https://community.klipper3d.org).
* O [site do Klipper](https://www.klipper3d.org) agora usa a infraestrutura “mkdocs”. Há também um projeto [Traduções do Klipper](https://github.com/Klipper3d/klipper-translations).
* Suporte automatizado para gravação de firmware via cartão SD em muitas placas.
* Novo suporte cinemático para impressoras “Hybrid CoreXY” e “Hybrid CoreXZ”.
* O Klipper agora usa `rotation_distance` para configurar distâncias de deslocamento do motor de passo.
* O código hospedeiro principal do Klipper agora pode se comunicar diretamente com microcontroladores usando o barramento CAN.
* Novo sistema de “análise de movimento”. As atualizações internas de movimento do Klipper e os resultados dos sensores podem ser rastreados e registrados para análise.
* Drivers de motor de passo Trinamic agora são monitorados continuamente quanto a condições de erro.
* Suporte para o microcontrolador rp2040 (placas Raspberry Pi Pico).
* O sistema “make menuconfig” agora utiliza kconfiglib.
* Muitos módulos adicionais adicionados: ds18b20, duplicate_pin_override, filament_motion_sensor, palette2, motion_report, pca9533, pulse_counter, save_variables, sdcard_loop, temperature_host, temperature_mcu
* Diversas correções de erros e limpezas de código.

<a id="klipper-090"></a>
## Klipper 0.9.0

Disponível em 20201020. Principais alterações nesta versão:

* Suporte para “Input Shaping” - um mecanismo para neutralizar a ressonância da impressora. Pode reduzir ou eliminar “ringing” nas impressões.
* Novo sistema “Smooth Pressure Advance”. Ele implementa “Pressure Advance” sem introduzir alterações instantâneas de velocidade. Também agora é possível ajustar o pressure advance usando o método de “Torre de ajuste”.
* Novo servidor de API “webhooks”. Ele fornece uma interface JSON programável para o Klipper.
* A tela LCD e o menu agora são configuráveis usando a linguagem de modelos Jinja2.
* Os drivers de motor de passo TMC2208 agora podem ser usados em modo “standalone” com o Klipper.
* Suporte aprimorado a BL-Touch v3.
* Identificação USB aprimorada. O Klipper agora tem seu próprio código de identificação USB e os microcontroladores agora podem informar seus números de série exclusivos durante a identificação USB.
* Novo suporte cinemático para impressoras “Rotary Delta” e “CoreXZ”.
* Melhorias de microcontrolador: suporte para stm32f070, suporte para stm32f207, suporte para pinos GPIO no “MCU Linux”, suporte a “gerenciador de inicialização HID” stm32, suporte ao gerenciador de inicialização Chitu, suporte ao gerenciador de inicialização MKS Robin.
* Tratamento aprimorado de eventos de “coleta de lixo” do Python.
* Muitos módulos adicionais adicionados: adc_scaled, adxl345, bme280, display_status, extruder_stepper, fan_generic, hall_filament_width_sensor, htu21d, homing_heaters, input_shaper, lm75, print_stats, resonance_tester, shaper_calibrate, query_adc, graph_accelerometer, graph_extruder, graph_motion, graph_shaper, graph_temp_sensor, whconsole
* Diversas correções de erros e limpezas de código.

<a id="klipper-091"></a>
### Klipper 0.9.1

Disponível em 20201028. Versão contendo apenas correções de bugs.

<a id="klipper-080"></a>
## Klipper 0.8.0

Disponível em 20191021. Principais alterações nesta versão:

* Novo suporte a modelos de comando G-Code. O G-Code no arquivo de configuração agora é avaliado com a linguagem de modelos Jinja2.
* Melhorias nos drivers Trinamic de motor de passo:
   * Novo suporte para drivers TMC2209 e TMC5160.
   * Comandos G-Code DUMP_TMC, SET_TMC_CURRENT e INIT_TMC aprimorados.
   * Suporte aprimorado para tratamento UART de TMC com multiplexador analógico.
* Suporte aprimorado a referenciamento, sondagem e nivelamento da mesa:
   * Novos módulos manual_probe, bed_screws, screws_tilt_adjust, skew_correction, safe_z_home adicionados.
   * Sondagem multiamostra aprimorada com lógica de mediana, média e repetição.
   * Documentação aprimorada para BL-Touch, calibração de sonda, calibração de fim de curso, calibração de delta, referenciamento sem sensor e calibração de fase de fim de curso.
   * Suporte aprimorado a referenciamento em eixo Z longo.
* Muitas melhorias de microcontrolador do Klipper:
   * Klipper portado para: SAM3X8C, SAM4S8C, SAMD51, STM32F042, STM32F4
   * Novas implementações de driver CDC USB em SAM3X, SAM4, STM32F4.
   * Suporte aprimorado para gravação do Klipper via USB.
   * Suporte a SPI por software.
   * Filtragem de temperatura muito aprimorada no LPC176x.
   * Configurações iniciais de pino de saída podem ser configuradas no microcontrolador.
* Novo site com a documentação do Klipper: http://klipper3d.org/
   * O Klipper agora tem um logotipo.
* Suporte experimental para cinemáticas polares e de “guincho de cabos”.
* O arquivo de configuração agora pode incluir outros arquivos de configuração.
* Muitos módulos adicionais adicionados: board_pins, controller_fan, delayed_gcode, dotstar, filament_switch_sensor, firmware_retraction, gcode_arcs, gcode_button, heater_generic, manual_stepper, mcp4018, mcp4728, neopixel, pause_resume, respond, temperature_sensor tsl1401cl_filament_width_sensor, tuning_tower
* Muitos comandos adicionais adicionados: RESTORE_GCODE_STATE, SAVE_GCODE_STATE, SET_GCODE_VARIABLE, SET_HEATER_TEMPERATURE, SET_IDLE_TIMEOUT, SET_TEMPERATURE_FAN_TARGET
* Diversas correções de erros e limpezas de código.

<a id="klipper-070"></a>
## Klipper 0.7.0

Disponível em 20181220. Principais alterações nesta versão:

* O Klipper agora suporta nivelamento de mesa “mesh”
* Novo suporte para calibração delta “aprimorada” (calibra dimensões de impressão x/y em impressoras delta)
* Suporte para configuração em tempo de execução de drivers de motor de passo Trinamic (tmc2130, tmc2208, tmc2660)
* Suporte aprimorado a sensores de temperatura: MAX6675, MAX31855, MAX31856, MAX31865, termistores personalizados, sensores comuns estilo pt100
* Diversos módulos novos: temperature_fan, sx1509, force_move, mcp4451, z_tilt, quad_gantry_level, endstop_phase, bltouch
* Diversos comandos novos adicionados: SAVE_CONFIG, SET_PRESSURE_ADVANCE, SET_GCODE_OFFSET, SET_VELOCITY_LIMIT, STEPPER_BUZZ, TURN_OFF_HEATERS, M204, macros personalizadas de G-code
* Suporte expandido a telas LCD:
   * Suporte para menus em tempo de execução
   * Novos ícones de tela
   * Suporte para telas “uc1701” e “ssd1306”
* Suporte adicional a microcontroladores:
   * Klipper portado para: LPC176x (Smoothieboards), SAM4E8E (Duet2), SAMD21 (Arduino Zero), STM32F103 (dispositivos “Blue pill”), atmega32u4
   * Novo driver CDC USB genérico implementado em AVR, LPC176x, SAMD21 e STM32F103
   * Melhorias de desempenho em processadores ARM
* O código de cinemática foi reescrito para usar um “solucionador iterativo”
* Novos casos de teste automáticos para o software hospedeiro do Klipper
* Muitos arquivos de configuração de exemplo novos para impressoras comuns de prateleira
* Atualizações de documentação para gerenciadores de inicialização, avaliação de desempenho, porte de microcontrolador, verificações de configuração, mapeamento de pinos, configurações de fatiador, empacotamento e mais
* Várias correções de bugs e limpezas de código

<a id="klipper-060"></a>
## Klipper 0.6.0

Disponível em 20180331. Principais alterações nesta versão:

* Verificações aprimoradas de falha de hardware de aquecedor e termistor
* Suporte para sondas Z
* Suporte inicial para calibração automática de parâmetros em deltas (via novo comando delta_calibrate)
* Suporte inicial para compensação de inclinação da mesa (via comando bed_tilt_calibrate)
* Suporte inicial para “referenciamento seguro” e sobrescritas de referenciamento
* Suporte inicial para exibição de estado em telas estilo RepRapDiscount 2004 e 12864
* Novas melhorias multi-extrusora:
   * Suporte para aquecedores compartilhados
   * Suporte inicial para carros duplos
* Suporte para configurar vários motores de passo por eixo (p. ex., Z duplo)
* Suporte para pinos de saída digitais e pwm personalizados (com novo comando SET_PIN)
* Suporte inicial para um “cartão SD virtual” que permite imprimir diretamente pelo Klipper (ajuda em máquinas lentas demais para executar bem o OctoPrint)
* Suporte para definir comprimentos de braço diferentes em cada torre de uma delta
* Suporte para comandos G-Code M220/M221 (sobrescrita de fator de velocidade / sobrescrita de fator de extrusão)
* Diversas atualizações de documentação:
   * Muitos arquivos de configuração de exemplo novos para impressoras comuns de prateleira
   * Novo exemplo de configuração multi-MCU
   * Novo exemplo de configuração de sensor bltouch
   * Novos documentos de perguntas frequentes, verificação de configuração e G-Code
* Suporte inicial para testes de integração contínua em todos os commits do GitHub
* Várias correções de bugs e limpezas de código

<a id="klipper-050"></a>
## Klipper 0.5.0

Disponível em 20171025. Principais mudanças nesta versão:

* Suporte para impressoras com múltiplas extrusoras.
* Suporte inicial para execução no Beaglebone PRU. Suporte inicial para a placa Replicape.
* Suporte inicial para execução do código do microcontrolador em um processo Linux em tempo real.
* Suporte para múltiplos microcontroladores. (Por exemplo, pode-se controlar uma extrusora com um microcontrolador e o restante da impressora com outro.) Sincronização de relógio por software é implementada para coordenar ações entre microcontroladores.
* Melhorias de desempenho do motor de passo (AVRs de 20Mhz até 189K passos por segundo).
* Suporte para controle de servos e suporte para definição de ventoinhas de resfriamento do bico.
* Várias correções de bugs e limpezas de código

<a id="klipper-040"></a>
## Klipper 0.4.0

Disponível em 20170503. Principais mudanças nesta versão:

* Instalação melhorada em máquinas Raspberry Pi. A maior parte da instalação agora é por script.
* Suporte para cinemática corexy
* Atualizações de documentação: Novo documento Cinemática, novo guia de ajuste de Pressure Advance, novos arquivos de configuração de exemplo e mais
* Melhorias de desempenho do motor de passo (AVRs de 20Mhz acima de 175K passos por segundo, Arduino Due acima de 460K)
* Suporte para resets automáticos do microcontrolador. Suporte para resets via chaveamento da energia USB no Raspberry Pi.
* O algoritmo de pressure advance agora funciona com antecipação (look-ahead) para reduzir mudanças de pressão durante curvas.
* Suporte para limitar a velocidade máxima de movimentos curtos em zigue-zague
* Suporte para sensores AD595
* Várias correções de bugs e limpezas de código

<a id="klipper-030"></a>
## Klipper 0.3.0

Disponível em 20161223. Principais mudanças nesta versão:

* Documentação melhorada
* Suporte para robôs com cinemática delta
* Suporte para microcontrolador Arduino Due (ARM cortex-M3)
* Suporte para microcontroladores AVR baseados em USB
* Suporte para algoritmo de "pressure advance" - reduz vazamento (ooze) durante impressões.
* Novo recurso de "fim de curso baseado em fase do motor de passo" - permite maior precisão no referenciamento (homing) de fim de curso.
* Suporte para comandos "extended g-code" como "help", "restart" e "status".
* Suporte para recarregar a configuração do Klipper e reiniciar o software host emitindo um comando "restart" a partir do terminal.
* Melhorias de desempenho do motor de passo (AVRs de 20Mhz até 158K passos por segundo).
* Relatório de erros melhorado. A maioria dos erros agora é mostrada via terminal junto com ajuda sobre como resolver.
* Várias correções de bugs e limpezas de código

<a id="klipper-020"></a>
## Klipper 0.2.0

Versão inicial do Klipper. Disponível em 20160525. Principais recursos disponíveis na versão inicial incluem:

* Suporte básico para impressoras cartesianas (motores de passo, extrusora, mesa aquecida, ventoinha de resfriamento).
* Suporte para comandos g-code comuns. Suporte para interface com OctoPrint.
* Tratamento de aceleração e antecipação (lookahead)
* Suporte para microcontroladores AVR via portas seriais padrão
