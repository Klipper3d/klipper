<a id="hall-filament-width-sensor"></a>
# Sensor Hall de espessura do filamento

[English](../Hall_Filament_Width_Sensor.md) | **Português brasileiro**

Este documento descreve o módulo hospedeiro do Sensor de Largura do Filamento. O hardware usado para desenvolver este módulo hospedeiro é baseado em dois sensores Hall lineares (ss49e, por exemplo). Os sensores no corpo estão localizados em lados opostos. Princípio de funcionamento: dois sensores Hall funcionam em modo diferencial, a deriva térmica é a mesma para o sensor. Compensação especial de temperatura não é necessária.

Você pode encontrar projetos em [Thingiverse](https://www.thingiverse.com/thing:4138933), um vídeo de montagem também está disponível no [Youtube](https://www.youtube.com/watch?v=TDO9tME8vp4)

Para usar o sensor Hall de largura do filamento, leia a [Referência de Configuração](Config_Reference.md#hall_filament_width_sensor) e a [documentação de G-Codes](G-Codes.md#hall_filament_width_sensor).

<a id="how-does-it-work"></a>
## Como funciona?

O sensor gera duas saídas analógicas com base na largura calculada do filamento. A soma da tensão de saída sempre equivale à largura detectada do filamento. O módulo hospedeiro monitora as mudanças de tensão e ajusta o multiplicador de extrusão. Eu uso o conector aux2 em uma placa do tipo ramps com os pinos analog11 e analog12. Você pode usar pinos e placas diferentes.

<a id="template-for-menu-variables"></a>
## Modelo para variáveis de menu

```
[menu __main __filament __width_current]
type: command
enable: {'hall_filament_width_sensor' in printer}
name: Dia: {'%.2F' % printer.hall_filament_width_sensor.Diameter}
index: 0

[menu __main __filament __raw_width_current]
type: command
enable: {'hall_filament_width_sensor' in printer}
name: Raw: {'%4.0F' % printer.hall_filament_width_sensor.Raw}
index: 1
```

<a id="calibration-procedure"></a>
## Procedimento de calibração

Para obter o valor bruto do sensor, você pode usar o item de menu ou o comando **QUERY_RAW_FILAMENT_WIDTH** no terminal.

1. Insira a primeira haste de calibração (tamanho 1,5 mm) e obtenha o primeiro valor bruto da sonda
1. Insira a segunda haste de calibração (tamanho 2,0 mm) e obtenha o segundo valor bruto da sonda
1. Salve os valores brutos da sonda nos parâmetros de configuração `Raw_dia1` e `Raw_dia2`

<a id="how-to-enable-sensor"></a>
## Como habilitar o sensor

Por padrão, o sensor é desabilitado ao ligar.

Para habilitar o sensor, emita o comando **ENABLE_FILAMENT_WIDTH_SENSOR** ou defina o parâmetro `enable` como `true`.

<a id="use-as-a-runout-switch-only"></a>
## Usar apenas como chave de fim de filamento

Por padrão, o sensor mede o diâmetro do filamento e ajusta o multiplicador de extrusão para compensar variações.

Se você quiser usar o sensor apenas como chave de fim de filamento, defina o parâmetro de configuração `enable_flow_compensation` como `false`. Neste modo, o sensor acionará apenas eventos de fim de filamento quando o filamento não for detectado, ele não modificará o multiplicador de extrusão.

Isso é útil para impressoras em que o sensor de filamento não é preciso o suficiente para compensação de fluxo, mas pode detectar com segurança o fim do filamento, ou ao imprimir com filamentos flexíveis que têm características de diâmetro instáveis.

Emita **ENABLE_FILAMENT_WIDTH_SENSOR FLOW_COMPENSATION=1** para habilitar a compensação de fluxo ou **ENABLE_FILAMENT_WIDTH_SENSOR FLOW_COMPENSATION=0** para desabilitá-la.

Observe que desabilitar a compensação da largura do filamento redefine automaticamente o multiplicador de extrusão para 100%.

**QUERY_FILAMENT_WIDTH** inclui o estado atual da compensação de fluxo em sua saída.

<a id="logging"></a>
## Registro de dados

Por padrão, o registro do diâmetro é desabilitado ao ligar.

Emita o comando **ENABLE_FILAMENT_WIDTH_LOG** para iniciar o registro e emita o comando **DISABLE_FILAMENT_WIDTH_LOG** para interromper o registro. Para habilitar o registro ao ligar, defina o parâmetro `logging` como `true`.

O diâmetro do filamento é registrado a cada intervalo de medição (10 mm por padrão).
