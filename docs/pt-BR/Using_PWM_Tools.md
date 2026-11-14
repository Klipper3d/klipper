<a id="using-pwm-tools"></a>
# Usando ferramentas PWM

[English](../Using_PWM_Tools.md) | **Português brasileiro**

Este documento descreve como configurar um laser ou spindle controlado por PWM usando `pwm_tool` e algumas macros.

<a id="how-does-it-work"></a>
## Como funciona?

Com o reaproveitamento da saída pwm do ventilador do cabeçote de impressão, é possível controlar lasers ou spindles. Isso é útil se você usa cabeçotes de impressão comutáveis, por exemplo a E3D toolchanger ou uma solução DIY. Normalmente, ferramentas CAM como o LaserWeb podem ser configuradas para usar comandos `M3-M5`, que significam *velocidade do spindle CW* (`M3 S[0-255]`), *velocidade do spindle CCW* (`M4 S[0-255]`) e *parada do spindle* (`M5`).

**Aviso:** ao acionar um laser, mantenha todas as precauções de segurança em que conseguir pensar! Lasers de diodo geralmente são invertidos. Isso significa que, quando a MCU reinicia, o laser ficará *totalmente ligado* pelo tempo que a MCU levar para reiniciar. Por precaução, recomenda-se *sempre* usar óculos de proteção contra laser apropriados para o comprimento de onda correto se o laser estiver energizado; e desconectar o laser quando ele não for necessário. Além disso, você deve configurar um tempo-limite de segurança, de modo que, quando seu hospedeiro ou MCU encontrar um erro, a ferramenta pare.

Para um exemplo de configuração, consulte [config/sample-pwm-tool.cfg](../../config/sample-pwm-tool.cfg).

<a id="commands"></a>
## Comandos

`M3/M4 S<value>` : Define o ciclo de trabalho (duty-cycle) do PWM. Valores entre 0 e 255. `M5` : Interrompe a saída PWM para o valor de desligamento.

<a id="laserweb-configuration"></a>
## Configuração do Laserweb

Se você usa o Laserweb, uma configuração funcional seria:

```gcode
GCODE START:
    M5            ; Desative o laser
    G21           ; Defina as unidades como mm
    G90           ; Posicionamento absoluto
    G0 Z0 F7000   ; Defina a velocidade sem corte

GCODE END:
    M5            ; Desative o laser
    G91           ; relativo
    G0 Z+20 F4000 ;
    G90           ; absoluto

GCODE HOMING:
    M5            ; Desative o laser
    G28           ; Referencie todos os eixos

TOOL ON:
    M3 $INTENSITY

TOOL OFF:
    M5            ; Desative o laser

LASER INTENSITY:
    S
```
