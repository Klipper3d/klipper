<a id="slicers"></a>
# Fatiadores

[English](../Slicers.md) | **Português brasileiro**

Este documento fornece algumas dicas para configurar um aplicativo "slicer" para uso com o Klipper. Slicers comuns usados com o Klipper são Slic3r, Cura, Simplify3D, etc.

<a id="set-the-g-code-flavor-to-marlin"></a>
## Defina o G-Code flavor como Marlin

Muitos fatiadores têm uma opção para configurar o "G-Code flavor". O padrão é frequentemente "Marlin" e funciona bem com o Klipper. A configuração "Smoothieware" também funciona bem com o Klipper.

<a id="klipper-gcode_macro"></a>
## gcode_macro do Klipper

Os fatiadores geralmente permitem configurar sequências de "Start G-Code" e "End G-Code". Muitas vezes é conveniente definir macros personalizadas no arquivo de configuração do Klipper em vez disso - como: `[gcode_macro START_PRINT]` e `[gcode_macro END_PRINT]`. Então pode-se apenas executar START_PRINT e END_PRINT na configuração do fatiador. Definir essas ações na configuração do Klipper pode facilitar o ajuste das etapas iniciais e finais da impressora, pois as alterações não exigem refatiar.

Consulte [sample-macros.cfg](../../config/sample-macros.cfg) para exemplos de macros START_PRINT e END_PRINT.

Consulte a [referência de configuração](Config_Reference.md#gcode_macro) para detalhes sobre como definir um gcode_macro.

<a id="large-retraction-settings-may-require-tuning-klipper"></a>
## Configurações grandes de retirada podem exigir ajuste do Klipper

As velocidades máxima e a aceleração máxima dos movimentos de retirada são controladas no Klipper pelas configurações `max_extrude_only_velocity` e `max_extrude_only_accel`. Essas configurações têm um valor padrão que deve funcionar bem em muitas impressoras. No entanto, se você configurou uma retirada grande no fatiador (p. ex., 5mm ou mais), poderá descobrir que elas limitam a velocidade desejada das retiradas.

Se estiver usando uma retirada grande, considere ajustar o [avanço de pressão](Pressure_Advance.md) do Klipper em vez disso. Caso contrário, se parecer que o cabeçote faz uma "pausa" durante a retirada e a desretirada, considere definir explicitamente `max_extrude_only_velocity` e `max_extrude_only_accel` no arquivo de configuração do Klipper.

<a id="do-not-enable-coasting"></a>
## Não habilite "coasting"

O recurso "coasting" provavelmente resultará em impressões de baixa qualidade com o Klipper. Considere usar o [avanço de pressão](Pressure_Advance.md) do Klipper em vez disso.

Especificamente, se o fatiador alterar drasticamente a taxa de extrusão entre movimentos, o Klipper executará desaceleração e aceleração entre movimentos. Isso provavelmente piorará as bolhas, não melhorará.

Em contraste, é aceitável (e muitas vezes útil) usar a configuração de "retract" do fatiador, a configuração de "wipe" e/ou a configuração de "wipe on retract".

<a id="do-not-use-extra-restart-distance-on-simplify3d"></a>
## Não use "extra restart distance" no Simplify3d

Essa configuração pode causar mudanças drásticas nas taxas de extrusão, o que pode acionar a verificação de seção transversal máxima de extrusão do Klipper. Considere usar o [avanço de pressão](Pressure_Advance.md) do Klipper ou a configuração regular de retract do Simplify3d em vez disso.

<a id="disable-preloadve-on-kisslicer"></a>
## Desabilite "PreloadVE" no KISSlicer

Se estiver usando o software de fatiamento KISSlicer, defina "PreloadVE" como zero. Considere usar o [avanço de pressão](Pressure_Advance.md) do Klipper em vez disso.

<a id="disable-any-advanced-extruder-pressure-settings"></a>
## Desabilite quaisquer configurações "avançadas de pressão da extrusora"

Alguns fatiadores anunciam uma capacidade de "pressão avançada da extrusora". Recomenda-se manter essas opções desabilitadas ao usar o Klipper, pois elas provavelmente resultarão em impressões de baixa qualidade. Considere usar o [avanço de pressão](Pressure_Advance.md) do Klipper em vez disso.

Especificamente, essas configurações do fatiador podem instruir o firmware a fazer alterações bruscas na taxa de extrusão na esperança de que o firmware aproxime essas solicitações e a impressora obtenha aproximadamente a pressão desejada da extrusora. O Klipper, no entanto, utiliza cálculos cinemáticos e temporização precisos. Quando o Klipper é comandado a fazer alterações significativas na taxa de extrusão, ele planeja as alterações correspondentes de velocidade, aceleração e movimento da extrusora - o que não é a intenção do fatiador. O fatiador pode até comandar taxas de extrusão excessivas a ponto de acionar a verificação de seção transversal máxima de extrusão do Klipper.

Em contraste, é aceitável (e muitas vezes útil) usar a configuração de "retract" do fatiador, a configuração de "wipe" e/ou a configuração de "wipe on retract".

<a id="start_print-macros"></a>
## Macros START_PRINT

Ao usar uma macro START_PRINT ou similar, às vezes é útil repassar parâmetros das variáveis do fatiador para a macro.

No Cura, para repassar temperaturas, o seguinte G-code inicial seria usado:

```
START_PRINT BED_TEMP={material_bed_temperature_layer_0} EXTRUDER_TEMP={material_print_temperature_layer_0}
```

Em derivados do slic3r, como PrusaSlicer e SuperSlicer, o seguinte seria usado:

```
START_PRINT EXTRUDER_TEMP=[first_layer_temperature] BED_TEMP=[first_layer_bed_temperature]
```

Observe também que esses fatiadores inserirão seus próprios códigos de aquecimento quando certas condições não forem atendidas. No Cura, a existência das variáveis `{material_bed_temperature_layer_0}` e `{material_print_temperature_layer_0}` é suficiente para mitigar isso. Em derivados do slic3r, você usaria:

```
M140 S0
M104 S0
```

antes da chamada da macro. Observe também que o SuperSlicer tem uma opção de botão "custom gcode only", que alcança o mesmo resultado.

Um exemplo de macro START_PRINT usando esses parâmetros pode ser encontrado em config/sample-macros.cfg
