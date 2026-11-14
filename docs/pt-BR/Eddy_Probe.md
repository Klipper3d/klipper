<a id="eddy-current-inductive-probe"></a>
# Sonda indutiva por correntes parasitas (Eddy Current)

[English](../Eddy_Probe.md) | **Português brasileiro**

Este documento descreve o suporte a sondas indutivas por [correntes parasitas](https://en.wikipedia.org/wiki/Eddy_current) no Klipper.

Essas sondas detectam a mesa medindo a [frequência de ressonância](https://en.wikipedia.org/wiki/Resonance) de uma bobina dentro do sensor. Quanto mais próxima essa bobina estiver de uma mesa metálica, maior a frequência de ressonância da bobina. As medições de frequência podem assim ser usadas para estimar a distância entre sensor e mesa.

<a id="probing-mechanisms"></a>
## Mecanismos de sondagem

Diferentemente das sondas de mesa tradicionais, um sensor por correntes parasitas suporta quatro métodos diferentes de sondagem: padrão, "scan", "rapid_scan" e "tap". Os diferentes métodos de sondagem são ativados passando um parâmetro `METHOD=xxx` aos comandos de sonda (por exemplo, `PROBE METHOD=tap`). Cada método de sondagem tem vantagens e desvantagens conforme descrito abaixo.

<a id="default-probing-method"></a>
### Método de sondagem padrão

O método de sondagem padrão comporta-se mais como uma sonda de mesa tradicional. O cabeçote desce em direção à mesa até o sensor detectar que está próximo da mesa e então várias medições do sensor são feitas na posição parada para estimar a distância entre sensor e mesa. Este mecanismo de sondagem é ativado ao não especificar um parâmetro `METHOD` em comandos do tipo sonda (por ex., um comando `PROBE` simples).

Vantagens:

* É o método de sondagem de uso mais geral. Fornece boa precisão com boa flexibilidade.
* Pode ser usado em muitas posições iniciais do cabeçote. É necessário garantir que a posição XY do cabeçote coloque o sensor sobre a mesa metálica, mas fora isso há flexibilidade na altura inicial exata.

Desvantagens:

* Os resultados da sonda estão sujeitos à deriva térmica. As distâncias relatadas pela sonda correlacionam-se com as distâncias medidas durante a calibração inicial (via `PROBE_EDDY_CURRENT_CALIBRATE`) e os resultados podem ser impactados se a sondagem for executada em temperatura diferente. Alterações na temperatura da mesa, da bobina do sensor, da eletrônica do sensor ou de qualquer metal próximo ao sensor podem impactar os resultados. O impacto é pequeno (pense em mícrons), mas a precisão aceitável para uma sonda de mesa também é pequena (novamente pense em mícrons). Para melhores resultados, recomenda-se executar a calibração e as sondagens subsequentes em temperatura consistente.

Quando usar:

Este é o método de sondagem padrão, e é recomendado para a maioria das ações de sondagem. Em particular, é o tipo de sonda recomendado para ferramentas de alinhamento da mesa como `QUAD_GANTRY_LEVEL`, `Z_TILT_ADJUST`, `SCREWS_TILT_CALCULATE`, `DELTA_CALIBRATE` e semelhantes.

<a id="scan-probing-method"></a>
### Método de sondagem "scan"

O método de sondagem "scan" é semelhante ao método padrão, exceto que a sonda não desce em direção à mesa. Em vez disso, a sonda coleta medições do sensor na posição Z atual para estimar a distância entre o sensor e a mesa. É útil para `BED_MESH_CALIBRATE`, pois toda a mesa pode ser varrida apenas com movimentos horizontais.

Vantagens:

* A posição Z não muda durante a sondagem e há menos chance de folga do motor de passo Z (e similares) impactar as medições. Isso pode ser particularmente útil quando apenas medições relativas de altura Z são desejadas (por ex., ao usar `zero_reference_position` com `BED_MESH_CALIBRATE`).
* Uma varredura completa da mesa pode levar menos tempo do que o método padrão.

Desvantagens:

* A mesa deve estar quase paralela aos trilhos XY da impressora e não deve haver grandes desvios na altura da mesa. Para resultados aceitáveis, a varredura da mesa deve ser executada com um `HORIZONTAL_MOVE_Z` baixo para que o sensor permaneça próximo à mesa durante toda a varredura. (Quanto menor a distância, mais precisos os resultados.) Na prática, isso requer que a distância entre o bico e a mesa seja de no máximo cerca de um milímetro, e nessas distâncias quaisquer desvios notáveis da mesa podem resultar em colisão entre bico e mesa durante o movimento horizontal.
* O método "scan" tem as mesmas desvantagens de desvio térmico descritas para o método padrão. Para melhores resultados, recomenda-se executar a calibração e as sondagens subsequentes a uma temperatura consistente.

Quando usar:

O método "scan" é normalmente usado durante a calibração da malha da mesa. Recomenda-se sempre verificar se a mesa está paralela aos trilhos XY da impressora antes de realizar uma varredura da mesa. Dependendo do hardware da impressora, pode-se usar uma ferramenta automatizada utilizando o método de sondagem padrão para verificar se a mesa está paralela - por exemplo: `QUAD_GANTRY_LEVEL RETRY_TOLERANCE=0.250`, `Z_TILT_ADJUST RETRY_TOLERANCE=0.250` ou `SCREWS_TILT_CALCULATE MAX_TOLERANCE=0.250`.

Uma malha da mesa pode então ser executada com algo semelhante a `BED_MESH_CALIBRATE METHOD=scan HORIZONTAL_MOVE_Z=1`.

<a id="rapid_scan-probing-method"></a>
### Método de sondagem "rapid_scan"

O método de sondagem "rapid_scan" é muito semelhante ao método "scan", exceto que a sonda não pausa em cada ponto a ser medido. Em vez disso, medições feitas durante o movimento horizontal próximo a cada ponto de sondagem são usadas para estimar a distância entre o sensor e a mesa.

Vantagens:

* Uma varredura completa da mesa em "rapid_scan" pode ser ligeiramente mais rápida do que o método "scan".
* Caso contrário, tem as mesmas vantagens do método "scan".

Desvantagens:

* Os resultados de um "rapid_scan" podem ser menos precisos do que os do método "scan".
* Mesmas desvantagens das sondas "scan" (a mesa deve estar paralela e desvio térmico).

Quando usar:

Uma "rapid_scan" pode ser útil ao realizar uma varredura grande e detalhada da malha da mesa para fins de diagnóstico. Nessa situação, o tempo reduzido de varredura pode compensar a possível perda de precisão.

Para impressão normal, uma malha da mesa usando o método "scan" regular é geralmente preferível para melhor precisão e tempo adicional mínimo de sondagem.

Uma vez verificado que a mesa está paralela aos trilhos XY, pode-se executar uma varredura rápida da malha da mesa com algo semelhante a `BED_MESH_CALIBRATE METHOD=rapid_scan HORIZONTAL_MOVE_Z=1`.

<a id="tap-probing-method"></a>
### Método de sondagem "tap"

Durante a sondagem "tap", o cabeçote desce até o bico tocar a mesa, o bico é então elevado para longe da mesa, e as medições do sensor durante o movimento de elevação são analisadas para determinar o local onde o bico perde o contato com a mesa.

Vantagens:

* Os resultados da sonda são determinados pelo ponto real de contato entre o bico e a mesa em vez de medições indiretas entre sensor e mesa. Isso pode ser particularmente útil se trocarmos de bico frequentemente, pois os resultados levarão em conta a geometria do bico atual.
* Uma sonda "tap" não tem os problemas de desvio térmico associados aos outros métodos de sondagem. A calibração principal da sonda não é utilizada durante sondagens tap e, portanto, não é necessário acompanhar temperaturas entre a calibração inicial e as sondagens subsequentes.
* Imprecisões de "torção" dos eixos são menos problemáticas durante sondagens tap, pois não há deslocamento XY da sonda a compensar. No entanto, ainda é preciso garantir que a posição XY do cabeçote coloque tanto o bico quanto o sensor acima da mesa antes da sondagem tap.

Desvantagens:

* É preciso garantir que tanto o bico quanto a mesa estejam limpos antes da sondagem tap. Qualquer filamento no bico ou detrito na mesa pode distorcer significativamente os resultados da sonda.
* É preciso garantir que o bico esteja a cerca de 3-20mm de distância da mesa antes de iniciar cada tentativa de sonda "tap". Se o bico começar muito próximo à mesa, o contato pode não ser detectado, o que pode resultar em colisão descontrolada entre bico e mesa. Se o bico começar muito longe da mesa, as medições do sensor não são precisas e a tentativa de tap pode falhar ou fornecer resultados imprecisos.
* O hardware da impressora deve permitir que o bico faça contato total com a mesa. Não deve haver interruptores de fim de curso ou batentes do carro que façam contato antes de o bico contatar a mesa.
* É preciso garantir que a temperatura do bico não esteja alta demais para a mesa. Uma temperatura alta demais pode derreter os revestimentos de PEI de algumas mesas, por exemplo.

Quando usar:

Uma sonda "tap" é frequentemente usada como uma etapa durante um processo de referenciamento/nivelamento em várias etapas para levar em conta a geometria atual do bico e reduzir erros associados à deriva térmica. Por exemplo, pode-se implantar uma macro que referencia, chama `Z_TILT_ADJUST` com o método de sondagem padrão, aquece a impressora a uma temperatura intermediária, limpa o bico esfregando-o repetidamente sobre uma escova, realiza uma sondagem "tap", usa `SET_KINEMATIC_POSITION` com os resultados do tap, executa `BED_MESH_CALIBRATE` utilizando uma `zero_reference_position` e então leva a impressora à temperatura normal de impressão. As etapas reais para utilizar uma sonda "tap" dependem fortemente do hardware específico da impressora.

Uma sondagem "tap" pode ser iniciada com algo como `PROBE METHOD=tap`.

<a id="configuration"></a>
## Configuração

Para configurar uma sonda de corrente parasita, comece declarando uma [seção de configuração probe_eddy_current](Config_Reference.md#probe_eddy_current) no arquivo printer.cfg. Recomenda-se definir `descend_z` como 0.5mm. É típico que o sensor exija `x_offset` e `y_offset`. Se esses valores não forem conhecidos, deve-se estimar os valores durante a calibração inicial.

Então reinicie a impressora e prossiga para as seguintes etapas de calibração.

<a id="calibrating-drive-current"></a>
### Calibrando a corrente de acionamento

A primeira etapa da calibração é determinar o DRIVE_CURRENT apropriado para o sensor. Referencie a impressora e navegue com o cabeçote de modo que o sensor fique próximo ao centro da mesa e cerca de 20mm acima da mesa. Então emita um comando `LDC_CALIBRATE_DRIVE_CURRENT CHIP=<config_name>`. Por exemplo, se a seção de configuração foi nomeada `[probe_eddy_current my_eddy_probe]`, então deve-se executar `LDC_CALIBRATE_DRIVE_CURRENT CHIP=my_eddy_probe`. Este comando deve ser concluído em poucos segundos. Após concluído, emita um comando `SAVE_CONFIG` para salvar os resultados no printer.cfg e reiniciar.

<a id="calibrating-z-heights"></a>
### Calibrando as alturas Z

A segunda etapa da calibração é correlacionar as leituras do sensor com as alturas Z correspondentes. Referencie a impressora e navegue com o cabeçote de modo que o bico fique próximo ao centro da mesa. Então execute um comando `PROBE_EDDY_CURRENT_CALIBRATE CHIP=my_eddy_probe`. Assim que a ferramenta iniciar, siga as etapas descritas em ["o teste do papel"](Bed_Level.md#the-paper-test) para determinar a distância real entre o bico e a mesa no local indicado. Uma vez concluídas essas etapas, pode-se dar `ACCEPT` na posição. A ferramenta então moverá o cabeçote de modo que o sensor fique acima do ponto onde o bico estava e executará uma série de movimentos para correlacionar o sensor às posições Z. Isso levará alguns minutos. Após a conclusão da ferramenta, ela exibirá os dados de desempenho do sensor:

```
probe_eddy_current: noise 0.000642mm, MAD_Hz=11.314 in 2525 queries
Total frequency range: 45000.012 Hz
z: 0.250 # noise 0.000200mm, MAD_Hz=11.000
z: 0.530 # noise 0.000300mm, MAD_Hz=12.000
z: 1.010 # noise 0.000400mm, MAD_Hz=14.000
z: 2.010 # noise 0.000600mm, MAD_Hz=12.000
z: 3.010 # noise 0.000700mm, MAD_Hz=9.000
```
emita um comando `SAVE_CONFIG` para salvar os resultados no printer.cfg e reiniciar.

Após a calibração inicial, é uma boa ideia verificar se `x_offset` e `y_offset` estão precisos. Siga as etapas para [calibrar os deslocamentos X e Y da sonda](Probe_Calibrate.md#calibrating-probe-x-and-y-offsets). Se `x_offset` ou `y_offset` for modificado, certifique-se de executar o comando `PROBE_EDDY_CURRENT_CALIBRATE` (conforme descrito acima) após fazer a alteração.

Observe que sensores de corrente parasita são suscetíveis à "deriva térmica". Ou seja, alterações de temperatura podem resultar em alterações na altura Z relatada. Alterações tanto na temperatura da superfície da mesa quanto na temperatura do hardware do sensor podem alterar os resultados. Portanto, para melhores resultados, a calibração feita aqui e a sondagem subsequente que utiliza essa calibração devem ser feitas na mesma temperatura.

<a id="tap-calibration"></a>
### Calibração do tap

Para utilizar a sondagem "tap", é necessário configurar alguns parâmetros.

Deve ser possível comandar o cabeçote para baixo do plano nominal da mesa. Isso normalmente é feito definindo `position_min: -1` na seção de configuração `[stepper_z]` do printer.cfg (ou ajuste semelhante, como `minimum_z_position`, dependendo da cinemática). Isso é necessário para garantir que o bico possa ser comandado a tocar firmemente a mesa. Isso também é para garantir que o bico toque a mesa antes que ele seria comandado a iniciar a desaceleração.

Também é necessário configurar um parâmetro `tap_threshold`. Este parâmetro determina quando o movimento descendente do cabeçote durante uma sondagem "tap" deve ser interrompido. Um valor muito grande pode resultar em contato bico/mesa não detectado, o que pode resultar em colisão descontrolada do bico com a mesa. Um valor muito pequeno pode resultar em tentativa de sondagem "tap" interrompida antes de tocar a mesa, o que pode resultar em erros de sondagem ou resultados imprecisos.

O comando `PROBE_EDDY_CURRENT_TAP_CALIBRATE` pode ser usado para configurar um valor apropriado de `tap_threshold`. Esta ferramenta pode ser executada após concluir a calibração principal `PROBE_EDDY_CURRENT_CALIBRATE`. Siga estas etapas para calibrar `tap_threshold`:

1. Verifique se o bico e a mesa estão limpos. Habilite a impressora, faça o homing da impressora, mova o cabeçote para uma posição próxima ao centro da mesa e certifique-se de que o bico esteja entre 3 - 10 milímetros da mesa.
1. A próxima etapa envolve comandar o bico a fazer contato com a mesa. Esse processo sempre tem alguns riscos, portanto esteja preparado para emitir uma parada de emergência (`M112`) se a descida de sondagem não parar após contatar a mesa. Quando estiver pronto, emita o seguinte comando: `PROBE_EDDY_CURRENT_TAP_CALIBRATE TAP=guess` Esse comando analisa os dados encontrados durante a calibração principal da sonda para fazer uma estimativa grosseira inicial para o valor de tap_threshold e então executa a sonda "tap" correspondente. Idealmente o comando acima fará a sonda descer até atingir a mesa, afastar-se da mesa e então relatar um resultado válido de sondagem. Caso contrário, veja os parágrafos no final desta seção para solução de problemas. Se a tentativa foi bem-sucedida, continue para a próxima etapa.
1. A próxima etapa é executar outra sonda tap com uma configuração de limiar "refinada". A ferramenta utiliza informações coletadas durante uma sonda tap anterior bem-sucedida para determinar esse limiar melhorado. Certifique-se de que o bico esteja próximo ao centro da mesa, que esteja entre 3 - 10mm acima da mesa, esteja pronto para emitir uma parada de emergência e então execute o seguinte comando: `PROBE_EDDY_CURRENT_TAP_CALIBRATE TAP=refine` Idealmente esse comando também terá êxito; caso contrário, veja os parágrafos no final desta seção para solução de problemas. Se a tentativa foi bem-sucedida, continue para a próxima etapa.
1. Se a sondagem com o limiar refinado for bem-sucedida, o próximo teste é verificar que ela é estável ao longo de múltiplas tentativas de sondagem. Certifique-se de que o bico esteja próximo ao centro da mesa, que esteja entre 3 - 10mm acima da mesa, esteja pronto para emitir uma parada de emergência e então execute o seguinte comando: `PROBE_EDDY_CURRENT_TAP_CALIBRATE TAP=verify` Esse comando sondará a mesa cinco vezes seguidas. Idealmente o comando acima também terá êxito; caso contrário, veja os parágrafos no final desta seção para solução de problemas. Se a tentativa foi bem-sucedida, continue para a próxima etapa.
1. Se todas as etapas acima forem bem-sucedidas, pode-se emitir um comando `SAVE_CONFIG` para salvar o parâmetro "tap_threshold" no arquivo printer.cfg. A calibração deve estar agora completa.

Se alguma das etapas acima não for bem-sucedida, pode ser necessário solucionar e determinar manualmente um `tap_threshold` apropriado. Isso é feito executando comandos da forma: `PROBE METHOD=tap TAP_THRESHOLD=<value>` Onde `<value>` é um limiar a testar.

Em geral, se uma tentativa de sondagem interromper antes de tocar a mesa, isso indica que o parâmetro `TAP_THRESHOLD` fornecido está muito baixo. Tente aumentá-lo em cerca de 10% e tente novamente. Da mesma forma, se uma tentativa de sondagem não interromper após tocar a mesa, isso indica que `TAP_THRESHOLD` está muito alto. Considere diminuir o valor tentado pela metade.

Se a ferramenta de calibração automatizada falhou durante o estágio inicial de "estimativa", então pode-se usar o valor de tap_threshold relatado pela ferramenta como ponto de partida para tentativas manuais. Uma vez concluída uma tentativa de sondagem bem-sucedida, pode-se retornar às etapas principais descritas acima a partir do estágio de "refino".

<a id="performing-initial-calibration-when-homing-with-probe"></a>
### Realizando a calibração inicial ao referenciar com a sonda

É possível usar uma sonda de corrente parasita para referenciar um eixo Z. Para usar este processo, defina `endstop_pin` da seção de configuração `[stepper_z]` como `probe:z_virtual_endstop`.

Para referenciar com uma sonda eddy, é necessário primeiro calibrar a sonda via comando `PROBE_EDDY_CURRENT_CALIBRATE`. No entanto, esse comando exige que a impressora já esteja referenciada.

As seguintes etapas podem ser usadas para evitar essa dependência circular na primeiríssima calibração:

1. Defina uma seção de configuração `[probe_eddy_current]` no arquivo printer.cfg conforme descrito na [seção de configuração](#configuration).
1. Certifique-se de que uma seção [movimento forçado](Config_Reference.md#force_move) esteja definida e garanta que sua opção `enable_force_move` esteja presente e definida como true.
1. Ajuste manualmente os carros para que o cabeçote fique próximo ao centro da mesa e a cerca de 20mm de distância da mesa. Emita os comandos `LDC_CALIBRATE_DRIVE_CURRENT CHIP=<config_name>` e `SAVE_CONFIG` conforme descrito na [seção de calibração da corrente de acionamento](#calibrating-drive-current).
1. Mova manualmente o cabeçote para que fique a cerca de 20mm de distância da mesa e faça o homing dos eixos X e Y da impressora. Isso normalmente é feito com um comando `G28 X0 Y0`. Comande a posição X e Y do cabeçote para que o cabeçote fique aproximadamente sobre o centro da mesa. Isso normalmente é feito com um comando como `G1 X50 Y50` (usando valores XY apropriados para a impressora).
1. Ajuste manualmente a mesa para que fique majoritariamente plana em relação aos carros XY do cabeçote (se necessário). Ajuste manualmente o carro Z para que o bico fique a cerca de 20mm da mesa e emita um comando `SET_STEPPER_ENABLE STEPPER=stepper_z`. Emita um comando `SET_KINEMATIC_POSITION Z=25` seguido por um comando `PROBE_EDDY_CURRENT_CALIBRATE CHIP=my_eddy_probe`. Importante - após emitir esses comandos a impressora será capaz de se mover na direção Z, mas não conhece a posição Z real. Deve-se tomar cuidado para evitar solicitações de movimento que possam fazer o cabeçote descer contra a mesa.
1. Complete a calibração da sonda de correntes parasitas conforme descrito na [seção de calibração das alturas Z](#calibrating-z-heights). Emita um comando `SAVE_CONFIG` ao concluir.

Estas etapas são necessárias apenas para obter uma configuração inicial. Se for preciso reexecutar `PROBE_EDDY_CURRENT_CALIBRATE` no futuro, o mecanismo normal deve ser possível uma vez que esta configuração inicial esteja disponível.

<a id="thermal-drift-calibration"></a>
## Calibração de deriva térmica

Como com todas as sondas indutivas, sondas de corrente parasita estão sujeitas a deriva térmica significativa. Se a sonda eddy tiver um sensor de temperatura na bobina, é possível configurar um `[temperature_probe]` para relatar a temperatura da bobina e habilitar compensação de deriva por software. Para vincular uma sonda de temperatura a uma sonda de corrente parasita, a seção `[temperature_probe]` deve compartilhar o nome com a seção `[probe_eddy_current]`. Por exemplo:

```
[probe_eddy_current my_probe]
# configuração de sonda de corrente parasita...

[temperature_probe my_probe]
# configuração de sonda de temperatura...
```

Veja a [referência de configuração](Config_Reference.md#temperature_probe) para mais detalhes sobre como configurar uma `temperature_probe`. Recomenda-se configurar as opções `calibration_position`, `calibration_extruder_temp`, `extruder_heating_z` e `calibration_bed_temp`, pois isso automatizará algumas das etapas descritas abaixo. Se a impressora a ser calibrada for fechada, é fortemente recomendado definir a opção `max_validation_temp` com um valor entre 100 e 120.

Fabricantes de sondas eddy podem oferecer uma calibração de deriva padrão que pode ser adicionada manualmente à opção `drift_calibration` da seção `[probe_eddy_current]`. Se não oferecerem, ou se a calibração padrão não tiver bom desempenho no seu sistema, o módulo `temperature_probe` oferece um procedimento de calibração manual via comando G-code `TEMPERATURE_PROBE_CALIBRATE`.

Antes de realizar a calibração, o usuário deve ter uma ideia de qual é a temperatura máxima alcançável da bobina da sonda de temperatura. Essa temperatura deve ser usada para definir o parâmetro `TARGET` do comando `TEMPERATURE_PROBE_CALIBRATE`. O objetivo é calibrar na faixa de temperatura mais ampla possível, portanto é desejável começar com a impressora fria e terminar com a bobina na temperatura máxima que ela pode atingir.

Uma vez que um `[temperature_probe]` esteja configurado, as seguintes etapas podem ser seguidas para realizar a calibração de deriva térmica:

- A sonda deve ser calibrada usando `PROBE_EDDY_CURRENT_CALIBRATE` quando um `[temperature_probe]` estiver configurado e vinculado. Isso captura a temperatura durante a calibração, o que é necessário para executar a compensação de desvio térmico.
- Certifique-se de que o bico esteja livre de detritos e filamento.
- A mesa, o bico e a bobina da sonda devem estar frios antes da calibração.
- As seguintes etapas são necessárias se as opções `calibration_position`, `calibration_extruder_temp` e `extruder_heating_z` em `[temperature_probe]` **NÃO** estiverem configuradas:
   - Mova a ferramenta para o centro da mesa. O Z deve estar 30mm+ acima da mesa.
   - Aqueça a extrusora a uma temperatura acima da temperatura máxima segura da mesa. 150-170C deve ser suficiente para a maioria das configurações. O propósito de aquecer a extrusora é evitar a expansão do bico durante a calibração.
   - Quando a temperatura da extrusora tiver estabilizado, mova o eixo Z para baixo até cerca de 1mm acima da mesa.
- Inicie a calibração de desvio. Se o nome da sonda for `my_probe` e a temperatura máxima da sonda que podemos atingir for 80C, o comando gcode apropriado é `TEMPERATURE_PROBE_CALIBRATE PROBE=my_probe TARGET=80`. Se configurada, a ferramenta se moverá para a coordenada X,Y especificada por `calibration_position` e o valor Z especificado por `extruder_heating_z`. Após aquecer a extrusora até a temperatura especificada, a ferramenta se moverá para o valor Z especificado por `calibration_position`.
- O procedimento solicitará uma sondagem manual. Execute a sondagem manual com o teste do papel e `ACCEPT`. O procedimento de calibração coletará o primeiro conjunto de amostras com a sonda e então estacionará a sonda na posição de aquecimento.
- Se `calibration_bed_temp` **NÃO** estiver configurado, ligue o aquecimento da mesa na temperatura máxima segura. Caso contrário, esta etapa será executada automaticamente.
- Por padrão, o procedimento de calibração solicitará uma sondagem manual a cada 2C entre amostras até que o `TARGET` seja atingido. O delta de temperatura entre amostras pode ser personalizado definindo o parâmetro `STEP` em `TEMPERATURE_PROBE_CALIBRATE`. Deve-se ter cuidado ao definir um valor personalizado de `STEP`, um valor alto demais pode solicitar poucas amostras, resultando em calibração ruim.
- Os seguintes comandos gcode adicionais estão disponíveis durante a calibração de desvio:
   - `TEMPERATURE_PROBE_NEXT` pode ser usado para forçar uma nova amostra antes que o delta de etapa seja atingido.
   - `TEMPERATURE_PROBE_COMPLETE` pode ser usado para concluir a calibração antes que o `TARGET` seja atingido.
   - `ABORT` pode ser usado para encerrar a calibração e descartar os resultados.
- Quando a calibração terminar, use `SAVE_CONFIG` para armazenar a calibração de desvio.

Como se pode concluir, o processo de calibração descrito acima é mais desafiador e demorado do que a maioria dos outros procedimentos. Pode exigir prática e várias tentativas para obter uma calibração ideal.

<a id="errors-description"></a>
## Descrição de erros

<a id="possible-homing-errors-and-actionables"></a>
### Possíveis erros de referenciamento e ações:

- Erro de sensor
   - Verifique os registros para erro detalhado
- Erro Eddy I2C STATUS/DATA.
   - Verifique fiação solta.
   - Tente I2C por software/diminua a taxa I2C
- Dados de leitura inválidos
   - Mesmo que I2C

Possíveis erros de sensor e ações:

- Frequência acima da faixa rígida válida
   - Verifique a configuração de frequência
   - Falha de hardware
- Frequência acima da faixa flexível válida
   - Verifique a configuração de frequência
- Timeout do watchdog de conversão
   - Falha de hardware

Mensagens de aviso de amplitude baixa/alta podem significar:

- Sensor próximo à mesa
- Sensor longe da mesa
- Temperatura mais alta do que a da calibração atual
- Capacitor ausente

Em alguns sensores, não é possível evitar completamente o indicador de aviso de amplitude.

Você pode tentar refazer a calibração `LDC_CALIBRATE_DRIVE_CURRENT` na temperatura de trabalho ou aumentar `reg_drive_current` em 1-2 a partir do valor calibrado.

Geralmente, é como uma luz de verificação do motor. Pode indicar um problema.

<a id="possible-tap-errors-and-actionables"></a>
### Possíveis erros de TAP e ações:

- elevação insuficiente (A vs B) - janela Z muito pequena recebida pelo código
   - LIFT_SPEED/printer.max_z_velocity muito baixo - deve ser pelo menos 2.5 Recomendado >0.5 RPS. Por motivos de cogging do motor.
   - printer.max_z_accel muito baixo - deve ser maior que 35 mm/s^2. Recomendado 300 mm/s^2.
   - SAMPLE_RETRACT_DIST muito baixo Recomendado >2 mm.
- nenhum contato encontrado em z=Z (delta de inclinação A vs B) - análise de retirada gera dados não esperados. Frequentemente o cabeçote parou no ar por causa do ruído de medição.
   - printer.max_z_accel muito alto - ressonâncias Z, considere modelador de entrada Z
   - Vibrações mecânicas - ex. ventoinha do cabeçote
   - Ruído do sensor (temperatura elevada da câmara/mesa)
   - Aumento de tap_threshold normalmente permite aliviar os sintomas
- distância de depressão inválida (Z vs A:B)
   - se Z < A
      - tap_threshold muito baixo
      - PROBE_SPEED muito baixa
      - printer.max_z_accel muito baixo
   - se Z > B
      - Grande inclinação do cabeçote (problema mecânico/de instalação)
      - PROBE_SPEED muito alta
      - tap_threshold muito alto
