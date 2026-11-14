<a id="probe-calibration"></a>
# Calibração da sonda

[English](../Probe_Calibrate.md) | **Português brasileiro**

Este documento descreve o método para calibrar os deslocamentos (offsets) X, Y e Z de uma "automatic z probe" no Klipper. Isso é útil para usuários que têm uma seção `[probe]` ou `[bltouch]` em seu arquivo de configuração.

<a id="calibrating-probe-x-and-y-offsets"></a>
## Calibrando os deslocamentos de Eixo X e Y da sonda

Para calibrar o deslocamento de eixos X e Y, navegue até a aba "Controle" (Control) do OctoPrint, coloque a impressora na posição inicial e use os botões de movimentação do OctoPrint para mover o extrusor para uma posição próxima ao centro da mesa.

Coloque um pedaço de fita azul (ou similar) na mesa, embaixo da sonda (probe). Navegue até a aba "Terminal" do OctoPrint e emita um comando PROBE:

```
PROBE
```
Faça uma marca na fita diretamente abaixo de onde a sonda está (ou use outro método para anotar o local na mesa).

Emita um comando `GET_POSITION` e registre a localização (XY) da cabeça do extrusor relatada por esse comando. Por exemplo:

```
Recv: // toolhead: X:46.500000 Y:27.000000 Z:15.000000 E:0.000000
```
então seria registrada uma posição da sonda (Eixo X) de 46,5 e uma posição da sonda (Eixo Y) de 27.

Após registrar a posição da sonda, emita uma série de comandos G1 até que o extrusor diretamente acima da marca na mesa. Por exemplo:

```
G1 F300 X57 Y30 Z15
```
para mover o extrusor para uma posição de Eixo X de 57 e uma de Eixo Y de 30. Uma vez que se encontre a posição diretamente acima da marca, use o comando `GET_POSITION` para registrar essa posição. Esta é a posição do bocal (extrusor).

O x_offset é então `nozzle_x_position - probe_x_position` e o y_offset é de forma semelhante `nozzle_y_position - probe_y_position`. Atualize o arquivo printer.cfg com os valores fornecidos, remova a fita/marcas da mesa e emita um comando `RESTART` para que os novos valores entrem em vigor.

<a id="calibrating-probe-z-offset"></a>
## Calibração do Z offset da sonda

Fornecer um z_offset preciso da sonda é fundamental para obter impressões de alta qualidade. O z_offset é a distância entre o bico e a mesa quando a sonda dispara. A ferramenta `PROBE_CALIBRATE` do Klipper pode ser usada para obter esse valor - ela executará uma sondagem automática para medir a posição de disparo Z da sonda e então iniciará uma sondagem manual para obter a altura Z do bico. O z_offset da sonda será então calculado a partir dessas medições.

Comece fazendo o referenciamento (homing) da impressora e mova o cabeçote para uma posição próxima ao centro da mesa. Navegue até a aba de terminal do OctoPrint e execute o comando `PROBE_CALIBRATE` para iniciar a ferramenta.

Esta ferramenta realizará uma sondagem automática, depois elevará o cabeçote, moverá o bico sobre a localização do ponto de sondagem e iniciará a ferramenta de sondagem manual. Se o bico não se mover para uma posição acima do ponto de sondagem automática, então faça `ABORT` na ferramenta de sondagem manual e realize a calibração do deslocamento XY da sonda descrita acima.

Uma vez iniciada a ferramenta de sondagem manual, siga as etapas descritas em ["o teste do papel"](Bed_Level.md#the-paper-test) para determinar a distância real entre o bico e a mesa na localização dada. Uma vez concluídas essas etapas, pode-se dar `ACCEPT` na posição e salvar os resultados no arquivo de configuração com:

```
SAVE_CONFIG
```

Observe que se uma alteração for feita no sistema de movimento da impressora, na posição do hotend ou na localização da sonda, isso invalidará os resultados do PROBE_CALIBRATE.

Se a sonda tiver um deslocamento X ou Y e a inclinação da mesa for alterada (por ex., ajustando os parafusos da mesa, executando DELTA_CALIBRATE, executando Z_TILT_ADJUST, executando QUAD_GANTRY_LEVEL ou similar), isso invalidará os resultados do PROBE_CALIBRATE. Após fazer qualquer um dos ajustes acima, será necessário executar PROBE_CALIBRATE novamente.

Se os resultados do PROBE_CALIBRATE forem invalidados, quaisquer resultados anteriores de [malha da mesa](Bed_Mesh.md) obtidos usando a sonda também serão invalidados - será necessário executar novamente o BED_MESH_CALIBRATE após recalibrar a sonda.

<a id="repeatability-check"></a>
## Verificação de repetibilidade

Após calibrar os deslocamentos X, Y e Z da sonda, é recomendável verificar se a sonda fornece resultados repetíveis. Comece fazendo o referenciamento da impressora e mova o cabeçote para uma posição próxima ao centro da mesa. Navegue até a aba de terminal do OctoPrint e execute o comando `PROBE_ACCURACY`.

Este comando executará a sonda dez vezes e produzirá uma saída semelhante à seguinte:

```
Recv: // probe accuracy: at X:0.000 Y:0.000 Z:10.000
Recv: // and read 10 times with speed of 5 mm/s
Recv: // probe at -0.003,0.005 is z=2.506948
Recv: // probe at -0.003,0.005 is z=2.519448
Recv: // probe at -0.003,0.005 is z=2.519448
Recv: // probe at -0.003,0.005 is z=2.506948
Recv: // probe at -0.003,0.005 is z=2.519448
Recv: // probe at -0.003,0.005 is z=2.519448
Recv: // probe at -0.003,0.005 is z=2.506948
Recv: // probe at -0.003,0.005 is z=2.506948
Recv: // probe at -0.003,0.005 is z=2.519448
Recv: // probe at -0.003,0.005 is z=2.506948
Recv: // probe accuracy results: maximum 2.519448, minimum 2.506948, range 0.012500, average 2.513198, median 2.513198, standard deviation 0.006250
```

Idealmente, a ferramenta informará um valor máximo e mínimo idênticos. (Ou seja, idealmente a sonda obtém um resultado idêntico em todas as dez sondagens.) No entanto, é normal que os valores mínimo e máximo difiram em uma "step distance" Z ou até 5 microns (.005mm). Uma "step distance" é `rotation_distance/(full_steps_per_rotation*microsteps)`. A distância entre o valor mínimo e o valor máximo é chamada de intervalo (range). Assim, no exemplo acima, como a impressora usa uma step distance Z de .0125, um intervalo de 0.012500 seria considerado normal.

Se os resultados do teste mostrarem um valor de intervalo maior que 25 mícrons (.025mm), a sonda não tem precisão suficiente para procedimentos típicos de nivelamento da mesa. Pode ser possível ajustar a velocidade da sonda e/ou a altura inicial da sondagem para melhorar a repetibilidade da sonda. O comando `PROBE_ACCURACY` permite executar testes com parâmetros diferentes para ver seu impacto - consulte o [documento de G-Codes](G-Codes.md#probe_accuracy) para mais detalhes. Se a sonda geralmente obtém resultados repetíveis, mas apresenta um valor atípico ocasional, pode ser possível compensar isso usando várias amostras em cada sondagem - leia a descrição dos parâmetros de configuração `samples` da sonda na [referência de configuração](Config_Reference.md#probe) para mais detalhes.

Se forem necessárias nova velocidade de sondagem, contagem de amostras ou outras configurações, atualize o arquivo printer.cfg e emita um comando `RESTART`. Nesse caso, é recomendável [calibrar o z_offset](#calibrating-probe-z-offset) novamente. Se não for possível obter resultados repetíveis, não use a sonda para nivelamento da mesa. O Klipper possui várias ferramentas de sondagem manual que podem ser usadas em vez disso - consulte o [documento Bed Level](Bed_Level.md) para mais detalhes.

<a id="location-bias-check"></a>
## Verificação de viés de localização

Algumas sondas podem ter um viés sistemático que corrompe os resultados da sonda em certas localizações do toolhead. Por exemplo, se o suporte da sonda inclinar ligeiramente ao se mover ao longo do eixo Y, isso pode fazer com que a sonda reporte resultados enviesados em diferentes posições Y.

Este é um problema comum com sondas em impressoras delta, no entanto pode ocorrer em todas as impressoras.

Pode-se verificar um viés de localização usando o comando `PROBE_CALIBRATE` para medir o z_offset da sonda em várias localizações X e Y. Idealmente, o z_offset da sonda seria um valor constante em cada localização da impressora.

Para impressoras delta, tente medir o z_offset em uma posição próxima à torre A, em uma posição próxima à torre B e em uma posição próxima à torre C. Para impressoras cartesianas, corexy e similares, tente medir o z_offset em posições próximas aos quatro cantos da mesa.

Antes de iniciar este teste, primeiro calibre os deslocamentos X, Y e Z da sonda conforme descrito no início deste documento. Em seguida, faça o referenciamento da impressora e navegue até a primeira posição XY. Siga as etapas em [calibração do Z offset da sonda](#calibrating-probe-z-offset) para executar o comando `PROBE_CALIBRATE`, os comandos `TESTZ` e o comando `ACCEPT`, mas não execute `SAVE_CONFIG`. Anote o z_offset reportado encontrado. Em seguida, navegue até as outras posições XY, repita estas etapas de `PROBE_CALIBRATE` e anote o z_offset reportado.

Se a diferença entre o z_offset mínimo reportado e o z_offset máximo reportado for maior que 25 microns (.025mm), então a sonda não é adequada para procedimentos típicos de nivelamento da mesa. Consulte o [documento Bed Level](Bed_Level.md) para alternativas de sondagem manual.

<a id="temperature-bias"></a>
## Viés de temperatura

Muitas sondas têm um viés sistemático ao sondar em temperaturas diferentes. Por exemplo, a sonda pode disparar consistentemente a uma altura menor quando a sonda está a uma temperatura mais alta.

Recomenda-se executar as ferramentas de nivelamento da mesa a uma temperatura consistente para levar em conta esse viés. Por exemplo, sempre execute as ferramentas quando a impressora estiver à temperatura ambiente, ou sempre execute as ferramentas após a impressora ter atingido uma temperatura de impressão consistente. Em ambos os casos, é recomendável aguardar vários minutos após a temperatura desejada ser atingida, para que o conjunto da impressora esteja consistentemente na temperatura desejada.

Para verificar um viés de temperatura, comece com a impressora à temperatura ambiente e então faça o referenciamento da impressora, mova o cabeçote para uma posição próxima ao centro da mesa e execute o comando `PROBE_ACCURACY`. Anote os resultados. Então, sem fazer referenciamento ou desativar os motores de passo, aqueça o bico e a mesa da impressora até a temperatura de impressão e execute o comando `PROBE_ACCURACY` novamente. Idealmente, o comando reportará resultados idênticos. Como acima, se a sonda tiver um viés de temperatura, tenha o cuidado de sempre usar a sonda a uma temperatura consistente.
