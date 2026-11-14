<a id="rotation-distance"></a>
# Distância de rotação (rotation distance)

[English](../Rotation_Distance.md) | **Português brasileiro**

O driver de motor de passo requer um parâmetro `rotation_distance` em cada [secção de configuração do motor de passo](Config_Reference.md#stepper). A `rotation_distance` é a quantidade de movimento no eixo com uma rotação completa do motor de passo. Este documento descreve como esse valor pode ser configurado.

<a id="obtaining-rotation_distance-from-steps_per_mm-or-step_distance"></a>
## Obtendo rotation_distance a partir de steps_per_mm (ou step_distance)

Os projetistas da sua impressora 3D calcularam originalmente `steps_per_mm` a partir de uma distância de rotação. Se você conhece o steps_per_mm, então é possível usar esta fórmula geral para obter essa distância de rotação original:

```
rotation_distance = <full_steps_per_rotation> * <microsteps> / <steps_per_mm>
```

Ou, se você tem uma configuração mais antiga do Klipper e conhece o parâmetro `step_distance`, pode usar esta fórmula:

```
rotation_distance = <full_steps_per_rotation> * <microsteps> * <step_distance>
```

A configuração `<full_steps_per_rotation>` é determinada a partir do tipo de motor de passo. A maioria dos motores de passo são "motores de passo de 1,8 grau" e, portanto, têm 200 passos completos por rotação (360 dividido por 1,8 é 200). Alguns motores de passo são "motores de passo de 0,9 grau" e, portanto, têm 400 passos completos por rotação. Outros motores de passo são raros. Em caso de dúvida, não defina full_steps_per_rotation no arquivo de configuração e use 200 na fórmula acima.

A configuração `<microsteps>` é determinada pelo driver do motor de passo. A maioria dos drivers usa 16 micropassos. Em caso de dúvida, defina `microsteps: 16` na configuração e use 16 na fórmula acima.

Quase todas as impressoras devem ter um número inteiro para `rotation_distance` em eixos tipo X, Y e Z. Se a fórmula acima resultar em um rotation_distance dentro de .01 de um número inteiro, arredonde o valor final para esse whole_number.

<a id="calibrating-rotation_distance-on-extruders"></a>
## Calibrando rotation_distance nas extrusoras

Em uma extrusora, a `rotation_distance` é a distância que o filamento percorre para uma rotação completa do motor de passo. A melhor maneira de obter um valor preciso para essa configuração é usar um procedimento de "medir e aparar" (measure and trim).

Primeiro comece com um palpite inicial para a distância de rotação. Ele pode ser obtido a partir de [steps_per_mm](#obtaining-rotation_distance-from-steps_per_mm-or-step_distance) ou [inspecionando o hardware](#extruder).

Em seguida, use o seguinte procedimento para “medir e aparar”:

1. Certifique-se de que a extrusora tenha filamento, que o bico esteja aquecido a uma temperatura apropriada e que a impressora esteja pronta para extrudar.
1. Use um marcador para fazer uma marca no filamento a cerca de 70mm da entrada do corpo da extrusora. Depois use um paquímetro digital para medir a distância real dessa marca com a maior precisão possível. Anote isso como `<initial_mark_distance>`.
1. Extrude 50mm de filamento com a seguinte sequência de comandos: `G91` seguida de `G1 E50 F60`. Anote 50mm como `<requested_extrude_distance>`. Aguarde a extrusora concluir o movimento (levará cerca de 50 segundos). É importante usar a taxa de extrusão lenta para este teste, pois uma taxa mais rápida pode causar alta pressão na extrusora, o que distorcerá os resultados. (Não use o “botão de extrusão” das interfaces gráficas para este teste, pois elas extrudam em taxa rápida.)
1. Use o paquímetro digital para medir a nova distância entre o corpo da extrusora e a marca no filamento. Anote isso como `<subsequent_mark_distance>`. Em seguida calcule: `actual_extrude_distance = <initial_mark_distance> - <subsequent_mark_distance>`
1. Calcule rotation_distance como: `rotation_distance = <previous_rotation_distance> * <actual_extrude_distance> / <requested_extrude_distance>` Arredonde o novo rotation_distance para três casas decimais.

Se actual_extrude_distance diferir de requested_extrude_distance em mais de cerca de 2mm, é uma boa ideia executar as etapas acima uma segunda vez.

Nota: *não* use um método do tipo "medir e aparar" para calibrar eixos do tipo x, y ou z. O método "medir e aparar" não é preciso o suficiente para esses eixos e provavelmente levará a uma configuração pior. Em vez disso, se necessário, esses eixos podem ser determinados [medindo o hardware de correias, polias e fuso de avanço](#obtaining-rotation_distance-by-inspecting-the-hardware).

<a id="obtaining-rotation_distance-by-inspecting-the-hardware"></a>
## Obtendo rotation_distance pela inspeção do hardware

É possível calcular rotation_distance com conhecimento dos motores de passo e da cinemática da impressora. Isso pode ser útil se o steps_per_mm não for conhecido ou se estiver projetando uma impressora nova.

<a id="belt-driven-axes"></a>
### Eixos acionados por correia

É fácil calcular rotation_distance para um eixo linear que usa correia e polia.

Primeiro determine o tipo de correia. A maioria das impressoras usa passo de correia de 2mm (ou seja, cada dente da correia está a 2mm de distância). Depois conte o número de dentes na polia do motor de passo. O rotation_distance é então calculado como:

```
rotation_distance = <belt_pitch> * <number_of_teeth_on_pulley>
```

Por exemplo, se uma impressora tem correia de 2mm e usa polia com 20 dentes, então a distância de rotação é 40.

<a id="axes-with-a-lead-screw"></a>
### Eixos com fuso de avanço (lead screw)

É fácil calcular a rotation_distance para fusos de avanço comuns usando a seguinte fórmula:

```
rotation_distance = <screw_pitch> * <number_of_separate_threads>
```

Por exemplo, o comum “fuso trapezoidal T8” tem distância de rotação de 8 (tem passo de 2mm e tem 4 roscas separadas).

Impressoras mais antigas com "barras roscadas" (threaded rods) têm apenas um "filete" no fuso de avanço e, portanto, a distância de rotação é o passo do parafuso. (O passo do parafuso é a distância entre cada sulco do parafuso.) Assim, por exemplo, uma barra métrica M6 tem distância de rotação de 1 e uma barra M8 tem distância de rotação de 1,25.

<a id="extruder"></a>
### Extrusora

É possível obter uma rotation distance inicial para extrusoras medindo o diâmetro do "parafuso recartilhado" (hobbed bolt) que empurra o filamento e usando a seguinte fórmula: `rotation_distance = <diameter> * 3.14`

Se a extrusora usa engrenagens, então também será necessário [determinar e definir o gear_ratio](#using-a-gear_ratio) para a extrusora.

A rotation distance real em uma extrusora variará de impressora para impressora, porque a aderência do "parafuso recartilhado" (hobbed bolt) que traciona o filamento pode variar. Ela pode até variar entre rolos de filamento. Após obter uma rotation_distance inicial, use o [procedimento de medir e aparar](#calibrating-rotation_distance-on-extruders) para obter uma configuração mais precisa.

<a id="using-a-gear_ratio"></a>
## Usando gear_ratio

Definir `gear_ratio` pode facilitar a configuração da `rotation_distance` em motores de passo que tenham uma caixa de redução (ou similar) acoplada a eles. A maioria dos motores de passo não tem caixa de redução - em caso de dúvida, não defina `gear_ratio` na configuração.

Quando `gear_ratio` está definido, a `rotation_distance` representa a distância que o eixo se move com uma rotação completa da engrenagem final na caixa de redução. Se, por exemplo, você estiver usando uma caixa de redução com relação "5:1", então pode-se calcular a rotation_distance com [conhecimento do hardware](#obtaining-rotation_distance-by-inspecting-the-hardware) e depois adicionar `gear_ratio: 5:1` à configuração.

Para engrenagens implementadas com correias e polias, é possível determinar o gear_ratio contando os dentes nas polias. Por exemplo, se um motor de passo com polia de 16 dentes aciona a próxima polia com 80 dentes, então deve-se usar `gear_ratio: 80:16`. De fato, pode-se abrir uma "caixa de redução" comum de prateleira e contar os dentes nela para confirmar sua relação de engrenagem.

Observe que às vezes uma caixa de redução terá uma relação de engrenagem ligeiramente diferente da anunciada. As engrenagens comuns do motor da extrusora BMG são um exemplo disso - elas são anunciadas como "3:1", mas na verdade usam engrenamento "50:17". (Usar números de dentes sem um denominador comum pode melhorar o desgaste geral das engrenagens, pois os dentes nem sempre engrenam da mesma maneira a cada revolução.) A comum "caixa de redução planetária 5.18:1" é configurada com mais precisão com `gear_ratio: 57:11`.

Se várias engrenagens forem usadas em um eixo, então é possível fornecer uma lista separada por vírgulas para gear_ratio. Por exemplo, uma caixa de redução "5:1" acionando uma polia de 16 dentes para 80 dentes poderia usar `gear_ratio: 5:1, 80:16`.

Na maioria dos casos, gear_ratio deve ser definido com números inteiros, pois engrenagens e polias comuns têm um número inteiro de dentes. No entanto, nos casos em que uma correia aciona uma polia usando atrito em vez de dentes, pode fazer sentido usar um número de ponto flutuante na relação de engrenagem (por ex., `gear_ratio: 107.237:16`).
