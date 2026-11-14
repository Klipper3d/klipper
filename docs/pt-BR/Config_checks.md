<a id="configuration-checks"></a>
# Verificações de configuração

[English](../Config_checks.md) | **Português brasileiro**

Este documento fornece uma lista de etapas que ajudam a confirmar as configurações de pinos no arquivo printer.cfg do Klipper. É uma boa ideia executar essas etapas logo após seguir o passos do [documento de instalação](Installation.md).

Durante este guia, pode ser necessário fazer alterações no arquivo de configuração do Klipper. Certifique-se de enviar um comando RESTART após cada alteração no arquivo de configuração, para garantir que a alteração tenha efeito (digite "restart" na aba terminal do Octoprint e clique em "Enviar" ou "Send"). Também é uma boa ideia enviar um comando STATUS após cada RESTART para verificar se o arquivo de configuração foi carregado corretamente.

<a id="verify-temperature"></a>
## Verifique a temperatura

Comece verificando se as temperaturas estão sendo informadas corretamente. Navegue até a seção do gráfico de temperatura na interface do usuário. Verifique se a temperatura do bico e do leito (se aplicável) está presente e não aumentando. Se estiver aumentando, desligue a impressora. Se as temperaturas não forem precisas, revise as configurações "sensor_type" e "sensor_pin" para o bico e/ou leito.

<a id="verify-m112"></a>
## Verificar o M112

Navegue até o console de comando e emita um comando M112 na caixa de terminais. Este comando solicita que o Klipper entre em estado de “desligamento”. Isso causará a exibição de um erro, que pode ser eliminado com um comando FIRMWARE_RESTART no console de comando. Octoprint também exigirá uma reconexão. Em seguida, navegue até a seção do gráfico de temperatura e verifique se as temperaturas continuam a atualizar e não aumentam. Se as temperaturas estiverem aumentando, desligue a impressora.

<a id="verify-heaters"></a>
## Verifique os aquecedores

Navegue até a seção do gráfico de temperatura e digite 50 seguido de enter na caixa de temperatura da extrusora/ferramenta. A temperatura da extrusora no gráfico deve começar a aumentar (dentro de cerca de 30 segundos). Em seguida, vá para a caixa suspensa de temperatura da extrusora e selecione "Desligado". Após alguns minutos, a temperatura deve começar a retornar ao valor inicial da temperatura ambiente. Se a temperatura não aumentar, verifique a configuração "heater_pin" na configuração.

Se a impressora tiver uma base aquecida, efetue o procedimento com descrito no teste acima com a base.

<a id="verify-stepper-motor-enable-pin"></a>
## Verifique o pino de ativação do motor de passo

Verifique se todos os eixos da impressora podem se mover livremente manualmente (os motores de passo estão desativados). Caso contrário, emita um comando M84 para desabilitar os motores. Se algum dos eixos ainda não puder se mover livremente, verifique a configuração do stepper "enable_pin" para o eixo fornecido. Na maioria dos drivers de motor de passo comuns, o pino de habilitação do motor está "ativo baixo" e, portanto, o pino de habilitação deve ter um "!" antes do pino (por exemplo, "enable_pin: !PA1").

<a id="verify-endstops"></a>
## Verifique os fins de curso

Mova manualmente todos os eixos da impressora para que nenhum deles entre em contato com um fim de curso. Envie um comando QUERY_ENDSTOPS por meio do console de comando. Ele deverá responder com o estado atual de todos os fins de curso configurados e todos eles deverão reportar um estado "open". Para cada um dos fins de curso, execute novamente o comando QUERY_ENDSTOPS enquanto aciona manualmente o fim de curso. O comando QUERY_ENDSTOPS deve reportar o fim de curso como "TRIGGERED".

Se o fim de curso parecer invertido (ele informa "open" quando acionado e vice-versa), adicione um "!" à definição do pino (por exemplo, "endstop_pin: ^!PA2"), ou remova o "!" se já houver um presente.

Se o fim de curso não mudar de estado, pode indicar que a chave de fim de curso está conectada a um pino diferente. No entanto, também pode exigir uma alteração na configuração "pullup" do pino (o '^' no início do nome endstop_pin - a maioria das impressoras usará um resistor "pullup" e o '^' deve estar presente).

<a id="verify-stepper-motors"></a>
## Verifique os motores de passo

Use o comando STEPPER_BUZZ para verificar a conectividade de cada motor de passo. Comece posicionando manualmente o eixo em questão em um ponto intermediário e então execute `STEPPER_BUZZ STEPPER=stepper_x` no console de comandos. O comando STEPPER_BUZZ fará o motor de passo em questão mover um milímetro na direção positiva e então retornará à posição inicial. (Se o fim de curso estiver definido em position_endstop=0, então no início de cada movimento o motor de passo se afastará do fim de curso.) Ele executará essa oscilação dez vezes.

Se o motor de passo não se mover, verifique as configurações "enable_pin" e "step_pin" do motor. Se o motor de passo se mover, mas não retornar à sua posição original, verifique a configuração de "dir_pin". Se o motor de passo oscilar em uma direção incorreta, geralmente indica que o "dir_pin" do eixo precisa ser invertido. Isso é feito adicionando um '!' ao "dir_pin" no arquivo de configuração da impressora (ou removendo-o se já houver um). Se o motor se mover significativamente mais ou significativamente menos do que um milímetro, verifique a configuração de "rotation_distance".

Execute o teste acima para cada um dos motores de passo definido no arquivo de configuração. (Defina o parâmetro STEPPER do comando STEPPER_BUZZ com o nome da seção de configuração que deve ser testada). Se não houver filamento na extrusora, poderá usar STEPPER_BUZZ para verificar a conectividade do motor da extrusora (use STEPPER=extruder). Caso contrário, é melhor testar o motor da extrusora separadamente (consulte a próxima seção).

Depois de verificar todos os fins de curso e verificar todos os motores de passo, o mecanismo de homing deverá ser testado. Execute o comando G28 para referenciar todos os eixos. Corte a alimentação da impressora se ela não fizer homing corretamente. Se necessário, execute novamente as etapas de verificação de fim de curso e do motor de passo.

<a id="verify-extruder-motor"></a>
## Verifique o motor da extrusora

Para testar o motor da extrusora será necessário aquecer a extrusora até uma temperatura de impressão. Navegue até a seção do gráfico de temperatura e selecione uma temperatura alvo na caixa suspensa de temperatura (ou insira manualmente uma temperatura apropriada). Aguarde a impressora atingir a temperatura desejada. Em seguida, navegue até o console de comando e clique no botão “Extrudar”. Verifique se o motor da extrusora gira na direção correta. Caso contrário, consulte as dicas de solução de problemas na seção anterior para confirmar as configurações "enable_pin", "step_pin" e "dir_pin" para a extrusora.

<a id="calibrate-pid-settings"></a>
## Calibrar configurações de PID

O Klipper suporta [controle PID](https://en.wikipedia.org/wiki/PID_controller) para os aquecedores da extrusora e da mesa. Para usar esse mecanismo de controle, é necessário calibrar as configurações de PID em cada impressora (as configurações de PID encontradas em outros firmwares ou nos arquivos de configuração de exemplo frequentemente funcionam mal).

Para calibrar a extrusora, navegue até o console de comandos e execute o comando PID_CALIBRATE. Por exemplo: `PID_CALIBRATE HEATER=extruder TARGET=170`

Ao concluir o teste de ajuste, execute `SAVE_CONFIG` para atualizar o arquivo printer.cfg com as novas configurações de PID.

Se a impressora tiver uma base aquecida e for compatível com o acionamento por PWM (Pulse Width Modulation, ou modulação por largura de pulso em português), é recomendável usar o controle PID para a base. (Quando o aquecedor da base é controlado usando o algoritmo PID, ele pode ligar e desligar dez vezes por segundo, o que pode não ser adequado para aquecedores que usam um interruptor mecânico). Na maioria dos casos o comando para calibração PID da base é: `PID_CALIBRATE HEATER=heater_bed TARGET=60`

<a id="next-steps"></a>
## Próximos passos

Este guia tem como objetivo ajudar na verificação básica das configurações dos pinos no arquivo de configuração do Klipper. Certifique-se de ler o guia [nivelamento da base](Bed_Level.md). Consulte também o documento [Fatiadores](Slicers.md) para obter informações sobre como configurar apropriadamente um fatiador (ou slicer, em inglês) com Klipper.

Depois de verificar se o básico da impressão funciona, é uma boa ideia considerar a calibração [avanço da pressão (pressure advance, em inglês)](Pressure_Advance.md).

Pode ser necessário realizar outros tipos de calibração detalhada da impressora - vários guias estão disponíveis on-line para ajudar com isso (por exemplo, pesquise na web por "3d printer calibration"). Como exemplo, se você observar o efeito chamado ringing, pode tentar seguir o guia de ajuste de [compensação de ressonância](Resonance_Compensation.md).
