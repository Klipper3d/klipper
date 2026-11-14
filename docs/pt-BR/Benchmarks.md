<a id="benchmarks"></a>
# Testes de desempenho

[English](../Benchmarks.md) | **Português brasileiro**

Este documento descreve os benchmarks do Klipper.

<a id="micro-controller-benchmarks"></a>
## Benchmarks de microcontroladores

Esta seção descreve o mecanismo usado para gerar os benchmarks de taxa de passo do microcontrolador Klipper.

O objetivo principal dos testes de desempenho é fornecer um mecanismo consistente para medir o impacto de mudanças de código no software. Um objetivo secundário é fornecer métricas de alto nível para comparar o desempenho entre chips e entre plataformas de software.

O teste de taxa de passos foi projetado para encontrar a taxa máxima de passos que o hardware e o software podem alcançar. Essa taxa de passos do teste não é alcançável no uso diário, pois o Klipper precisa executar outras tarefas (p. ex., comunicação MCU/host, leitura de temperatura, verificação de fim de curso) em qualquer uso real.

Em geral, os pinos para os testes de desempenho são escolhidos para piscar LEDs ou outros pinos inócuos. **Sempre verifique se é seguro acionar os pinos configurados antes de executar um teste de desempenho.** Não é recomendado acionar um motor de passo real durante um teste de desempenho.

<a id="step-rate-benchmark-test"></a>
### Teste de taxa de passos

O teste é executado usando a ferramenta console.py (descrita em [Debugging.md](Debugging.md)). O microcontrolador é configurado para a plataforma de hardware específica (veja abaixo) e então o seguinte é colado na janela do terminal console.py:

```
SET start_clock {clock+freq}
SET ticks 1000

reset_step_clock oid=0 clock={start_clock}
set_next_step_dir oid=0 dir=0
queue_step oid=0 interval={ticks} count=60000 add=0
set_next_step_dir oid=0 dir=1
queue_step oid=0 interval=3000 count=1 add=0

reset_step_clock oid=1 clock={start_clock}
set_next_step_dir oid=1 dir=0
queue_step oid=1 interval={ticks} count=60000 add=0
set_next_step_dir oid=1 dir=1
queue_step oid=1 interval=3000 count=1 add=0

reset_step_clock oid=2 clock={start_clock}
set_next_step_dir oid=2 dir=0
queue_step oid=2 interval={ticks} count=60000 add=0
set_next_step_dir oid=2 dir=1
queue_step oid=2 interval=3000 count=1 add=0
```

O acima testa três motores de passo gerando passos simultaneamente. Se a execução do acima resultar em erro "Rescheduled timer in the past" ou "Stepper too far in past", isso indica que o parâmetro `ticks` está muito baixo (resulta em uma taxa de passos muito rápida). O objetivo é encontrar a configuração mais baixa do parâmetro ticks que resulte de forma confiável na conclusão bem-sucedida do teste. Deve ser possível bissectar o parâmetro ticks até encontrar um valor estável.

Em caso de falha, pode-se copiar e colar o seguinte para limpar o erro em preparação para o próximo teste:

```
clear_shutdown
```

Para obter os testes de desempenho de um único motor de passo, a mesma sequência de configuração é usada, mas apenas o primeiro bloco do teste acima é copiado e colado na janela console.py.

Para produzir os testes de desempenho encontrados no documento [Recursos](Features.md), o número total de passos por segundo é calculado multiplicando o número de motores de passo ativos pela frequência nominal do MCU e dividindo pelo parâmetro ticks final. Os resultados são arredondados para o K mais próximo. Por exemplo, com três motores de passo ativos:

```
ECHO Test result is: {"%.0fK" % (3. * freq / ticks / 1000.)}
```

Os benchmarks são executados com parâmetros adequados para drivers TMC. Para microcontroladores que suportam `STEPPER_BOTH_EDGE=1` (conforme relatado na linha `MCU config` quando o console.py inicia pela primeira vez), use `step_pulse_duration=0` e `invert_step=-1` para habilitar passo otimizado em ambas as bordas do pulso de passo. Para outros microcontroladores, use um `step_pulse_duration` correspondente a 100ns.

<a id="avr-step-rate-benchmark"></a>
### Teste de taxa de passos AVR

A seguinte sequência de configuração é usada em chips AVR:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA5 dir_pin=PA4 invert_step=0 step_pulse_ticks=32
config_stepper oid=1 step_pin=PA3 dir_pin=PA2 invert_step=0 step_pulse_ticks=32
config_stepper oid=2 step_pin=PC7 dir_pin=PC6 invert_step=0 step_pulse_ticks=32
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `avr-gcc (GCC) 5.4.0`. Ambos os testes de 16Mhz e 20Mhz foram executados usando simulavr configurado para um atmega644p (testes anteriores confirmaram que os resultados do simulavr correspondem aos testes tanto em um at90usb de 16Mhz quanto em um atmega2560 de 16Mhz).

| avr | ticks |
| --- | --- |
| 1 motor de passo | 102 |
| 3 motores de passo | 486 |

<a id="arduino-due-step-rate-benchmark"></a>
### Teste de taxa de passos Arduino Due

A seguinte sequência de configuração é usada no Due:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PB27 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB26 dir_pin=PC30 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA21 dir_pin=PC30 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| sam3x8e | ticks |
| --- | --- |
| 1 motor de passo | 66 |
| 3 motores de passo | 257 |

<a id="duet-maestro-step-rate-benchmark"></a>
### Teste de taxa de passos Duet Maestro

A seguinte sequência de configuração é usada no Duet Maestro:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PC26 dir_pin=PC18 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PC26 dir_pin=PA8 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PC26 dir_pin=PB4 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| sam4s8c | ticks |
| --- | --- |
| 1 motor de passo | 71 |
| 3 motores de passo | 260 |

<a id="duet-wifi-step-rate-benchmark"></a>
### Teste de taxa de passos Duet Wifi

A seguinte sequência de configuração é usada no Duet Wifi:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PD6 dir_pin=PD11 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PD7 dir_pin=PD12 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PD8 dir_pin=PD13 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `gcc version 10.3.1 20210621 (release) (GNU Arm Embedded Toolchain 10.3-2021.07)`.

| sam4e8e | ticks |
| --- | --- |
| 1 motor de passo | 48 |
| 3 motores de passo | 215 |

<a id="beaglebone-pru-step-rate-benchmark"></a>
### Teste de taxa de passos Beaglebone PRU

A seguinte sequência de configuração é usada no PRU:

```
allocate_oids count=3
config_stepper oid=0 step_pin=gpio0_23 dir_pin=gpio1_12 invert_step=0 step_pulse_ticks=20
config_stepper oid=1 step_pin=gpio1_15 dir_pin=gpio0_26 invert_step=0 step_pulse_ticks=20
config_stepper oid=2 step_pin=gpio0_22 dir_pin=gpio2_1 invert_step=0 step_pulse_ticks=20
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `pru-gcc (GCC) 8.0.0 20170530 (experimental)`.

| pru | ticks |
| --- | --- |
| 1 motor de passo | 231 |
| 3 motores de passo | 847 |

<a id="stm32f042-step-rate-benchmark"></a>
### Teste de taxa de passos STM32F042

A seguinte sequência de configuração é usada no STM32F042:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA1 dir_pin=PA2 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PA3 dir_pin=PA2 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PB8 dir_pin=PA2 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| stm32f042 | ticks |
| --- | --- |
| 1 motor de passo | 59 |
| 3 motores de passo | 249 |

<a id="stm32f103-step-rate-benchmark"></a>
### Teste de taxa de passos STM32F103

A seguinte sequência de configuração é usada no STM32F103:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PC13 dir_pin=PB5 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB3 dir_pin=PB6 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA4 dir_pin=PB7 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| stm32f103 | ticks |
| --- | --- |
| 1 motor de passo | 61 |
| 3 motores de passo | 264 |

<a id="stm32f4-step-rate-benchmark"></a>
### Teste de taxa de passos STM32F4

A seguinte sequência de configuração é usada no STM32F4:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA5 dir_pin=PB5 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB2 dir_pin=PB6 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PB3 dir_pin=PB7 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`. Os resultados do STM32F407 foram obtidos executando um binário STM32F407 em um STM32F446 (e assim usando um clock de 168Mhz).

| stm32f446 | ticks |
| --- | --- |
| 1 motor de passo | 46 |
| 3 motores de passo | 205 |

| stm32f407 | ticks |
| --- | --- |
| 1 motor de passo | 46 |
| 3 motores de passo | 205 |

<a id="stm32h7-step-rate-benchmark"></a>
### Teste de taxa de passos STM32H7

A seguinte sequência de configuração é usada no STM32H723:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA13 dir_pin=PB5 invert_step=-1 step_pulse_ticks=52
config_stepper oid=1 step_pin=PB2 dir_pin=PB6 invert_step=-1 step_pulse_ticks=52
config_stepper oid=2 step_pin=PB3 dir_pin=PB7 invert_step=-1 step_pulse_ticks=52
finalize_config crc=0
```

O teste foi executado pela última vez no commit `554ae78d` com a versão gcc `arm-none-eabi-gcc (Fedora 14.1.0-1.fc40) 14.1.0`.

| stm32h723 | ticks |
| --- | --- |
| 1 motor de passo | 70 |
| 3 motores de passo | 181 |

<a id="stm32g0b1-step-rate-benchmark"></a>
### Teste de taxa de passos STM32G0B1

A seguinte sequência de configuração é usada no STM32G0B1:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PB13 dir_pin=PB12 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB10 dir_pin=PB2 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PB0 dir_pin=PC5 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `247cd753` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`.

| stm32g0b1 | ticks |
| --- | --- |
| 1 motor de passo | 58 |
| 3 motores de passo | 243 |

<a id="stm32g4-step-rate-benchmark"></a>
### Teste de taxa de passos STM32G4

A seguinte sequência de configuração é usada no STM32G431:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA0 dir_pin=PB5 invert_step=-1 step_pulse_ticks=17
config_stepper oid=1 step_pin=PB2 dir_pin=PB6 invert_step=-1 step_pulse_ticks=17
config_stepper oid=2 step_pin=PB3 dir_pin=PB7 invert_step=-1 step_pulse_ticks=17
finalize_config crc=0
```

O teste foi executado pela última vez no commit `cfa48fe3` com a versão gcc `arm-none-eabi-gcc (Fedora 14.1.0-1.fc40) 14.1.0`.

| stm32g431 | ticks |
| --- | --- |
| 1 motor de passo | 47 |
| 3 motores de passo | 208 |

<a id="lpc176x-step-rate-benchmark"></a>
### Teste de taxa de passos LPC176x

A seguinte sequência de configuração é usada no LPC176x:

```
allocate_oids count=3
config_stepper oid=0 step_pin=P1.20 dir_pin=P1.18 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=P1.21 dir_pin=P1.18 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=P1.23 dir_pin=P1.18 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0`. Os resultados do LPC1769 de 120Mhz foram obtidos por overclock de um LPC1768 para 120Mhz.

| lpc1768 | ticks |
| --- | --- |
| 1 motor de passo | 52 |
| 3 motores de passo | 222 |

| lpc1769 | ticks |
| --- | --- |
| 1 motor de passo | 51 |
| 3 motores de passo | 222 |

<a id="samd21-step-rate-benchmark"></a>
### Teste de taxa de passos SAMD21

A seguinte sequência de configuração é usada no SAMD21:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA27 dir_pin=PA20 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PB3 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA17 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0` em um microcontrolador SAMD21G18.

| samd21 | ticks |
| --- | --- |
| 1 motor de passo | 70 |
| 3 motores de passo | 306 |

<a id="samd51-step-rate-benchmark"></a>
### Teste de taxa de passos SAMD51

A seguinte sequência de configuração é usada no SAMD51:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PA22 dir_pin=PA20 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PA22 dir_pin=PA21 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PA22 dir_pin=PA19 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `arm-none-eabi-gcc (Fedora 10.2.0-4.fc34) 10.2.0` em um microcontrolador SAMD51J19A.

| samd51 | ticks |
| --- | --- |
| 1 motor de passo | 39 |
| 3 motores de passo | 191 |
| 1 motor de passo (200Mhz) | 39 |
| 3 motores de passo (200Mhz) | 181 |

<a id="same70-step-rate-benchmark"></a>
### Teste de taxa de passos SAME70

A seguinte sequência de configuração é usada no SAME70:

```
allocate_oids count=3
config_stepper oid=0 step_pin=PC18 dir_pin=PB5 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PC16 dir_pin=PD10 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PC28 dir_pin=PA4 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `34e9ea55` com a versão gcc `arm-none-eabi-gcc (NixOS 10.3-2021.10) 10.3.1` em um microcontrolador SAME70Q20B.

| same70 | ticks |
| --- | --- |
| 1 motor de passo | 45 |
| 3 motores de passo | 190 |

<a id="ar100-step-rate-benchmark"></a>
### Benchmark de taxa de passos AR100

A seguinte sequência de configuração é usada na CPU AR100 (Allwinner A64):

```
allocate_oids count=3
config_stepper oid=0 step_pin=PL10 dir_pin=PE14 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=PL11 dir_pin=PE15 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=PL12 dir_pin=PE16 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0

```

O teste foi executado pela última vez no commit `b7978d37` com a versão gcc `or1k-linux-musl-gcc (GCC) 9.2.0` em um microcontrolador Allwinner A64-H.

| AR100 R_PIO | ticks |
| --- | --- |
| 1 motor de passo | 85 |
| 3 motores de passo | 359 |

<a id="rpxxxx-step-rate-benchmark"></a>
### Teste de taxa de passos RPxxxx

A seguinte sequência de configuração é usada no RP2040 e no RP2350:

```
allocate_oids count=3
config_stepper oid=0 step_pin=gpio25 dir_pin=gpio3 invert_step=-1 step_pulse_ticks=0
config_stepper oid=1 step_pin=gpio26 dir_pin=gpio4 invert_step=-1 step_pulse_ticks=0
config_stepper oid=2 step_pin=gpio27 dir_pin=gpio5 invert_step=-1 step_pulse_ticks=0
finalize_config crc=0
```

O teste foi executado pela última vez no commit `14c105b8` com a versão gcc `arm-none-eabi-gcc (Fedora 14.1.0-1.fc40) 14.1.0` nas placas Raspberry Pi Pico e Pico 2.

| rp2040 (*) | ticks |
| --- | --- |
| 1 motor de passo | 3 |
| 3 motores de passo | 14 |

| rp2350 | ticks |
| --- | --- |
| 1 motor de passo | 36 |
| 3 motores de passo | 169 |

(*) Observe que os ticks reportados do rp2040 são relativos a um temporizador de agendamento de 12Mhz e não correspondem à sua taxa interna de processamento ARM de 200Mhz. Espera-se que 3 ticks de agendamento correspondam a ~42 ciclos do núcleo ARM e 14 ticks de agendamento correspondam a ~225 ciclos do núcleo ARM.

<a id="linux-mcu-step-rate-benchmark"></a>
### Teste de taxa de passos do MCU Linux

A seguinte sequência de configuração é usada em um Raspberry Pi:

```
allocate_oids count=3
config_stepper oid=0 step_pin=gpio2 dir_pin=gpio3 invert_step=0 step_pulse_ticks=5
config_stepper oid=1 step_pin=gpio4 dir_pin=gpio5 invert_step=0 step_pulse_ticks=5
config_stepper oid=2 step_pin=gpio6 dir_pin=gpio17 invert_step=0 step_pulse_ticks=5
finalize_config crc=0
```

O teste foi executado pela última vez no commit `59314d99` com a versão gcc `gcc (Raspbian 8.3.0-6+rpi1) 8.3.0` em um Raspberry Pi 3 (revisão a02082). Foi difícil obter resultados estáveis neste teste de desempenho.

| Linux (RPi3) | ticks |
| --- | --- |
| 1 motor de passo | 160 |
| 3 motores de passo | 380 |

<a id="command-dispatch-benchmark"></a>
## Teste de desempenho de despacho de comandos

O benchmark de despacho de comandos testa quantos comandos "fictícios" o microcontrolador consegue processar. É principalmente um teste do mecanismo de comunicação do hardware. O teste é executado usando a ferramenta console.py (descrita em [Debugging.md](Debugging.md)). O seguinte é colado na janela do terminal console.py:

```
DELAY {clock + 2*freq} get_uptime
FLOOD 100000 0.0 debug_nop
get_uptime
```

Quando o teste for concluído, determine a diferença entre os clocks relatados nas duas mensagens de resposta "uptime". O número total de comandos por segundo é então `100000 * mcu_frequency / clock_diff`.

Os testes USB podem exceder a capacidade de CPU de um Raspberry Pi. Se estiver executando em um Raspberry Pi, Beaglebone ou computador hospedeiro similar, aumente o atraso (p. ex., `DELAY {clock + 20*freq} get_uptime`). Onde aplicável, os testes de desempenho abaixo são com console.py executando em uma máquina de classe desktop com o dispositivo conectado via hub de supervelocidade.

Os testes de barramento CAN podem saturar o controlador host USB de um Raspberry Pi (ao testar via adaptador USB para barramento CAN gs_usb padrão). Onde aplicável, os testes de desempenho de barramento CAN abaixo são com console.py executando em uma máquina de classe desktop com um adaptador USB para barramento CAN conectado via hub USB de supervelocidade.

| MCU | Taxa | Build | Compilador do build |
| --- | --- | --- | --- |
| atmega2560 (serial) | 23K | b161a69e | avr-gcc (GCC) 4.8.1 |
| sam3x8e (serial) | 23K | b161a69e | arm-none-eabi-gcc (Fedora 7.1.0-5.fc27) 7.1.0 |
| rp2350 (CAN) | 59K | 17b8ce4c | arm-none-eabi-gcc (Fedora 14.1.0-1.fc40) 14.1.0 |
| at90usb1286 (USB) | 75K | 01d2183f | avr-gcc (GCC) 5.4.0 |
| ar100 (serial) | 138K | 08d037c6 | or1k-linux-musl-gcc 9.3.0 |
| samd21 (USB) | 223K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| pru (shared memory) | 260K | c5968a08 | pru-gcc (GCC) 8.0.0 20170530 (experimental) |
| stm32f103 (USB) | 355K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| sam3x8e (USB) | 418K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| lpc1768 (USB) | 534K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| lpc1769 (USB) | 628K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| sam4s8c (USB) | 650K | 8d4a5c16 | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| samd51 (USB) | 864K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| stm32f446 (USB) | 870K | 01d2183f | arm-none-eabi-gcc (Fedora 7.4.0-1.fc30) 7.4.0 |
| rp2040 (USB) | 885K | f6718291 | arm-none-eabi-gcc (Fedora 14.1.0-1.fc40) 14.1.0 |
| rp2350 (USB) | 885K | f6718291 | arm-none-eabi-gcc (Fedora 14.1.0-1.fc40) 14.1.0 |

<a id="host-benchmarks"></a>
## Testes de desempenho do hospedeiro

É possível executar testes de temporização no software hospedeiro usando o mecanismo de processamento em "modo lote" (descrito em [Debugging.md](Debugging.md)). Isso normalmente é feito escolhendo um arquivo de G-Code grande e complexo e medindo quanto tempo o software hospedeiro leva para processá-lo. Por exemplo:

```
time ~/klippy-env/bin/python ./klippy/klippy.py config/example-cartesian.cfg -i something_complex.gcode -o /dev/null -d out/klipper.dict
```
