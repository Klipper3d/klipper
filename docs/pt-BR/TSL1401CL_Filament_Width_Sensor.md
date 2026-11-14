<a id="tsl1401cl-filament-width-sensor"></a>
# sensor de espessura de filamento TSL1401CL

[English](../TSL1401CL_Filament_Width_Sensor.md) | **Português brasileiro**

Este documento descreve o módulo hospedeiro Filament Width Sensor. O hardware usado para desenvolver este módulo hospedeiro é baseado no conjunto linear de sensores TSL1401CL, mas ele pode funcionar com qualquer conjunto de sensores que tenha saída analógica. Você pode encontrar projetos no [Thingiverse](https://www.thingiverse.com/search?q=filament%20width%20sensor).

Para usar um conjunto de sensores como sensor de largura do filamento, leia a [Referência de configuração](Config_Reference.md#tsl1401cl_filament_width_sensor) e a [documentação de códigos G](G-Codes.md#hall_filament_width_sensor).

<a id="how-does-it-work"></a>
## Como funciona?

O sensor gera uma saída analógica com base na largura calculada do filamento. A tensão de saída sempre equivale à largura detectada do filamento (Ex. 1.65v, 1.70v, 3.0v). O módulo hospedeiro monitora as mudanças de tensão e ajusta o multiplicador de extrusão.

<a id="note"></a>
## Observação:

As leituras da sonda são feitas com intervalos de 10 mm por padrão. Se necessário, você pode alterar esse ajuste editando o parâmetro ***MEASUREMENT_INTERVAL_MM*** no arquivo **filament_width_sensor.py**.
