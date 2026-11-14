<a id="bl-touch"></a>
# BL-Touch

[English](../BLTouch.md) | **Português brasileiro**

<a id="connecting-bl-touch"></a>
## Conectando o BL-Touch

Um **aviso** antes de começar: Evite tocar o pino do BL-Touch com os dedos nus, pois ele é bastante sensível à gordura dos dedos. E se você tocar nele, seja muito cuidadoso para não entortar ou empurrar nada.

Conecte o conector “servo” do BL-Touch a um `control_pin` de acordo com a documentação do BL-Touch ou com a documentação da sua MCU. Na fiação original, o fio amarelo do trio é o `control_pin` e o fio branco do par é o `sensor_pin`. Você precisa configurar esses pinos de acordo com a sua fiação. A maioria dos dispositivos BL-Touch exige um pullup no pino da sonda (prefixe o nome do pino com "^"). Por exemplo:

```
[bltouch]
sensor_pin: ^P1.24
control_pin: P1.26
```

Se o BL-Touch for usado para fazer o homing do eixo Z, defina `endstop_pin: probe:z_virtual_endstop` e remova `position_endstop` na seção de configuração `[stepper_z]`, depois adicione uma seção de configuração `[safe_z_home]` para elevar o eixo Z, fazer o homing dos eixos XY, mover para o centro da mesa e fazer o homing do eixo Z. Por exemplo:

```
[safe_z_home]
home_xy_position: 100, 100 # Altere as coordenadas para o centro da mesa de impressão
speed: 50
z_hop: 10                 # Eleve 10mm
z_hop_speed: 5
```

É importante que o movimento z_hop em safe_z_home seja alto o suficiente para que a sonda não atinja nada, mesmo que o pino da sonda esteja em seu estado mais baixo.

<a id="initial-tests"></a>
## Testes iniciais

Antes de continuar, verifique se o BL-Touch está montado na altura correta; o pino deve ficar cerca de 2 mm acima do bico quando retraído

Ao ligar a impressora, a sonda BL-Touch deve realizar um autoteste e mover o pino para cima e para baixo algumas vezes. Quando o autoteste for concluído, o pino deve estar retraído e o LED vermelho da sonda deve estar aceso. Se houver algum erro, por exemplo se a sonda estiver piscando em vermelho ou se o pino estiver abaixado em vez de levantado, desligue a impressora e verifique a fiação e a configuração.

Se o acima estiver correto, é hora de testar se o pino de controle está funcionando corretamente. Primeiro execute `BLTOUCH_DEBUG COMMAND=pin_down` no terminal da sua impressora. Verifique se o pino se move para baixo e se o LED vermelho da sonda se apaga. Caso contrário, verifique novamente sua fiação e configuração. Em seguida, emita `BLTOUCH_DEBUG COMMAND=pin_up`, verifique se o pino se move para cima e se a luz vermelha acende novamente. Se estiver piscando, há algum problema.

O próximo passo é confirmar se o pino da sonda está funcionando corretamente. Execute `BLTOUCH_DEBUG COMMAND=pin_down`, verifique se o pino se move para baixo, execute `BLTOUCH_DEBUG COMMAND=touch_mode`, execute `QUERY_PROBE` e verifique se esse comando informa "probe: open". Em seguida, empurrando suavemente o pino um pouco para cima com a unha do dedo, execute `QUERY_PROBE` novamente. Verifique se o comando informa "probe: TRIGGERED". Se alguma das consultas não informar a mensagem correta, isso geralmente indica fiação ou configuração incorreta (embora alguns [clones](#bl-touch-clones) possam exigir tratamento especial). Ao concluir este teste, execute `BLTOUCH_DEBUG COMMAND=pin_up` e verifique se o pino se move para cima.

Após concluir os testes do pino de controle e do pino da sonda do BL-Touch, é hora de testar a sondagem, mas com uma variação. Em vez de deixar o pino da sonda tocar a mesa de impressão, deixe-o tocar a unha do seu dedo. Posicione o cabeçote longe da mesa, emita um `G28` (ou `PROBE` se não estiver usando probe:z_virtual_endstop), aguarde até que o cabeçote comece a se mover para baixo e interrompa o movimento tocando muito suavemente o pino com a unha. Pode ser necessário fazer isso duas vezes, já que a configuração padrão de homing sonda duas vezes. Esteja preparado para desligar a impressora caso ela não pare quando você tocar o pino.

Se isso funcionou, execute outro `G28` (ou `PROBE`), mas desta vez deixe-o tocar a mesa como deveria.

<a id="bl-touch-gone-bad"></a>
## BL-Touch com defeito

Quando o BL-Touch entra em um estado inconsistente, ele começa a piscar em vermelho. Você pode forçá-lo a sair desse estado emitindo:

BLTOUCH_DEBUG COMMAND=reset

Isso pode acontecer se sua calibração for interrompida porque a sonda foi impedida de ser extraída.

No entanto, o BL-Touch também pode não conseguir mais se calibrar. Isso acontece se o parafuso em seu topo estiver na posição errada ou se o núcleo magnético dentro do pino da sonda tiver se deslocado. Se ele tiver se deslocado para cima a ponto de grudar no parafuso, pode não conseguir mais abaixar seu pino. Com esse comportamento, você precisa abrir o parafuso e usar uma caneta esferográfica para empurrá-lo suavemente de volta ao lugar. Reinsira o pino no BL-Touch de modo que ele caia na posição extraída. Reajuste cuidadosamente o parafuso sem cabeça no lugar. Você precisa encontrar a posição certa para que ele consiga abaixar e levantar o pino e a luz vermelha acenda e apague. Use os comandos `reset`, `pin_up` e `pin_down` para conseguir isso.

<a id="bl-touch-clones"></a>
## "Clones" de BL-Touch

Muitos dispositivos "clone" de BL-Touch funcionam corretamente com o Klipper usando a configuração padrão. No entanto, alguns dispositivos "clone" podem não suportar o comando `QUERY_PROBE` e alguns dispositivos "clone" podem exigir a configuração de `pin_up_reports_not_triggered` ou `pin_up_touch_mode_reports_triggered`.

Importante! Não configure `pin_up_reports_not_triggered` ou `pin_up_touch_mode_reports_triggered` como False sem antes seguir estas instruções. Não configure nenhum deles como False em um BL-Touch genuíno. Definir incorretamente esses valores como False pode aumentar o tempo de sondagem e pode aumentar o risco de danificar a impressora.

Alguns dispositivos "clone" não suportam `touch_mode` e, como resultado, o comando `QUERY_PROBE` não funciona. Apesar disso, ainda pode ser possível realizar sondagem e homing com esses dispositivos. Nesses dispositivos, o comando `QUERY_PROBE` durante os [testes iniciais](#initial-tests) não terá êxito, porém o teste subsequente de `G28` (ou `PROBE`) terá êxito. Pode ser possível usar esses dispositivos "clone" com o Klipper se não se utilizar o comando `QUERY_PROBE` e não se habilitar o recurso `probe_with_touch_mode`.

Alguns dispositivos "clone" não conseguem realizar o teste interno de verificação da sonda do Klipper. Nesses dispositivos, tentativas de homing ou sondagem podem fazer o Klipper informar um erro "BLTouch failed to verify sensor state". Se isso ocorrer, execute manualmente as etapas para confirmar que o pino da sonda está funcionando conforme descrito na [seção de testes iniciais](#initial-tests). Se os comandos `QUERY_PROBE` nesse teste sempre produzirem os resultados esperados e ainda assim ocorrerem erros "BLTouch failed to verify sensor state", pode ser necessário definir `pin_up_touch_mode_reports_triggered` como False no arquivo de configuração do Klipper.

Um número raro de dispositivos "clone" antigos não consegue informar quando levantou sua sonda com êxito. Nesses dispositivos, o Klipper informará um erro "BLTouch failed to raise probe" após cada tentativa de homing ou sondagem. Pode-se testar esses dispositivos - mova o cabeçote para longe da mesa, execute `BLTOUCH_DEBUG COMMAND=pin_down`, verifique se o pino se moveu para baixo, execute `QUERY_PROBE`, verifique se esse comando informa "probe: open", execute `BLTOUCH_DEBUG COMMAND=pin_up`, verifique se o pino se moveu para cima e execute `QUERY_PROBE`. Se o pino permanecer levantado, o dispositivo não entrar em estado de erro, e a primeira consulta informar "probe: open" enquanto a segunda consulta informar "probe: TRIGGERED", então isso indica que `pin_up_reports_not_triggered` deve ser definido como False no arquivo de configuração do Klipper.

<a id="bl-touch-v3"></a>
## BL-Touch v3

Alguns dispositivos BL-Touch v3.0 e BL-Touch 3.1 podem exigir a configuração de `probe_with_touch_mode` no arquivo de configuração da impressora.

Se o BL-Touch v3.0 tiver seu fio de sinal conectado a um pino de fim de curso (com um capacitor de filtragem de ruído), então o BL-Touch v3.0 pode não conseguir enviar um sinal de forma consistente durante homing e sondagem. Se os comandos `QUERY_PROBE` na [seção de testes iniciais](#initial-tests) sempre produzirem os resultados esperados, mas o cabeçote nem sempre parar durante os comandos G28/PROBE, então isso é indicativo desse problema. Uma solução alternativa é definir `probe_with_touch_mode: True` no arquivo de configuração.

O BL-Touch v3.1 pode entrar incorretamente em estado de erro após uma tentativa de sondagem bem-sucedida. Os sintomas são uma luz piscando ocasional no BL-Touch v3.1 que dura alguns segundos após ele tocar a mesa com êxito. O Klipper deve limpar esse erro automaticamente e ele geralmente é inofensivo. No entanto, pode-se definir `probe_with_touch_mode` no arquivo de configuração para evitar esse problema.

Importante! Alguns dispositivos "clone" e o BL-Touch v2.0 (e anteriores) podem ter precisão reduzida quando `probe_with_touch_mode` está definido como True. Definir isso como True também aumenta o tempo necessário para implantar a sonda. Se for configurar esse valor em um "clone" ou em um dispositivo BL-Touch mais antigo, certifique-se de testar a precisão da sonda antes e depois de definir esse valor (use o comando `PROBE_ACCURACY` para testar).

<a id="multi-probing-without-stowing"></a>
## Sondagem múltipla sem recolher

Por padrão, o Klipper implantará a sonda no início de cada tentativa de sondagem e depois recolherá a sonda. Essa implantação e recolhimento repetitivos da sonda podem aumentar o tempo total das sequências de calibração que envolvem muitas medições de sondagem. O Klipper suporta manter a sonda implantada entre sondagens consecutivas, o que pode reduzir o tempo total de sondagem. Esse modo é habilitado configurando `stow_on_each_sample` como False no arquivo de configuração.

Importante! Definir `stow_on_each_sample` como False pode levar o Klipper a fazer movimentos horizontais do cabeçote com a sonda implantada. Certifique-se de verificar se todas as operações de sondagem têm folga Z suficiente antes de definir esse valor como False. Se houver folga insuficiente, um movimento horizontal pode fazer o pino prender em uma obstrução e resultar em danos à impressora.

Importante! Recomenda-se usar `probe_with_touch_mode` configurado como True ao usar `stow_on_each_sample` configurado como False. Alguns dispositivos "clone" podem não detectar um contato subsequente com a mesa se `probe_with_touch_mode` não estiver definido. Em todos os dispositivos, usar a combinação dessas duas configurações simplifica a sinalização do dispositivo, o que pode melhorar a estabilidade geral.

Observe, no entanto, que alguns dispositivos "clone" e o BL-Touch v2.0 (e anteriores) podem ter precisão reduzida quando `probe_with_touch_mode` está definido como True. Nesses dispositivos, é uma boa ideia testar a precisão da sonda antes e depois de definir `probe_with_touch_mode` (use o comando `PROBE_ACCURACY` para testar).

<a id="calibrating-the-bl-touch-offsets"></a>
## Calibração dos deslocamentos do BL-Touch

Siga as instruções no guia [Calibração da sonda](Probe_Calibrate.md) para definir os parâmetros de configuração x_offset, y_offset e z_offset.

É uma boa ideia verificar se o deslocamento Z está próximo de 1mm. Caso contrário, provavelmente você desejará mover a sonda para cima ou para baixo para corrigir isso. Você deseja que ela dispare bem antes de o bico atingir a mesa, para que possível filamento preso ou uma mesa empenada não afete nenhuma ação de sondagem. Mas, ao mesmo tempo, você deseja que a posição retraída fique o mais acima possível do bico para evitar que ela toque nas peças impressas. Se um ajuste for feito na posição da sonda, execute novamente as etapas de calibração da sonda.

<a id="bl-touch-output-mode"></a>
## Modo de saída do BL-Touch

* Um BL-Touch V3.0 suporta definir um modo de saída 5V ou OPEN-DRAIN, um BL-Touch V3.1 também suporta isso, mas também pode armazenar isso em sua EEPROM interna. Se sua placa controladora precisar do nível lógico alto fixo de 5V do modo 5V, você pode definir o parâmetro 'set_output_mode' na seção [bltouch] do arquivo de configuração da impressora como "5V".

   *** Use o modo 5V somente se a linha de entrada da sua placa controladora for tolerante a 5V. É por isso que a configuração padrão dessas versões de BL-Touch é o modo OPEN-DRAIN. Você pode potencialmente danificar a CPU da sua placa controladora ***

   Portanto: Se uma placa controladora PRECISA do modo 5V E ela é tolerante a 5V em sua linha de sinal de entrada E se

   - se você tem uma BL-Touch Smart V3.0, você precisa usar o parâmetro 'set_output_mode: 5V' para garantir essa configuração a cada inicialização, pois a sonda não consegue memorizar a configuração necessária.
   - se você tem uma BL-Touch Smart V3.1, você tem a opção de usar 'set_output_mode: 5V' ou armazenar o modo uma vez pelo uso manual de um comando 'BLTOUCH_STORE MODE=5V' e NÃO usar o parâmetro 'set_output_mode:'.
   - se você tem alguma outra sonda: Algumas sondas têm uma trilha na placa de circuito para cortar ou um jumper para configurar a fim de definir (permanentemente) o modo de saída. Nesse caso, omita completamente o parâmetro 'set_output_mode'.
Se você tem uma V3.1, não automatize nem repita o armazenamento do modo de saída para evitar desgastar a EEPROM da sonda. A EEPROM da BLTouch suporta cerca de 100.000 atualizações. 100 gravações por dia somariam cerca de 3 anos de operação até desgastá-la. Assim, o armazenamento do modo de saída em uma V3.1 foi projetado pelo fornecedor para ser uma operação complicada (o padrão de fábrica sendo um modo OPEN DRAIN seguro) e não é adequado para ser emitido repetidamente por qualquer fatiador, macro ou qualquer outra coisa; ele deve ser usado de preferência apenas ao integrar a sonda pela primeira vez à eletrônica da impressora.
