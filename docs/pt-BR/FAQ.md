<a id="frequently-asked-questions"></a>
# Perguntas frequentes

[English](../FAQ.md) | **Português brasileiro**

<a id="how-can-i-donate-to-the-project"></a>
## Como eu posso doar para o projeto?

Obrigado pelo seu apoio. Veja a [página de patrocinadores](Sponsors.md) para informações.

<a id="how-do-i-calculate-the-rotation_distance-config-parameter"></a>
## Como eu posso calcular o parâmetro de configuração rotation_distance?

Veja o [documento distância de rotação](Rotation_Distance.md).

<a id="wheres-my-serial-port"></a>
## Onde está minha porta serial?

A maneira usual de encontrar a porta serial USB é rodar o comando `ls /dev/serial/by-id/*` via um terminal ssh na máquina hospedeira. Isto deverá produzir uma saída similar ao que segue:

```
/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

O nome encontrado no comando acima é estável e é possível utilizável no arquivo de configuração durante o processo de gravação do código do microcontrolador. Por exemplo, um comando de gravação pode ser similar a:

```
sudo service klipper stop
make flash FLASH_DEVICE=/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
sudo service klipper start
```
e a configuração atualizada pode ser similar a:

```
[mcu]
serial: /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
```

Tenha certeza de copiar-e-colar o nome obtido com o comando "ls" executado acima, pois o nome diferirá para cada impressora.

Se você está usando múltiplos microcontroladores e eles não têm identificadores únicos (comum em placas com um chip USB CH340), então siga as instruções acima usando o comando `ls /dev/serial/by-path/*` como alternativa.

<a id="when-the-micro-controller-restarts-the-device-changes-to-devttyusb1"></a>
## Quando o microcontrolador reinicializa o dispositivo muda para /dev/ttyUSB1

Siga as instruções na secção "[Onde está minha porta serial?](#wheres-my-serial-port)" para evitar que isso ocorra.

<a id="the-make-flash-command-doesnt-work"></a>
## O comando "make flash" não funciona

O código tenta gravar o dispositivo usando métodos comuns para cada plataforma. Infelizmente, há uma enorme variação nos métodos de gravação, então o comando "make flash" pode não funcionar com todas as placas.

Se você estiver tendo falhas intermitentes ou possui uma configuração padrão, então verifique novamente se o Klipper não está rodando quando está gravando (sudo service klipper stop), tenha certeza que OctoPrint não está tentando conectar diretamente no dispositivo(Abra a aba de conexão na página web e clique em Disconnect se a Serial Port aponta para o dispositivo), e confirme que FLASH_DEVICE está correto para sua placa (veja a [questão acima](#wheres-my-serial-port)).

Entretanto, se "make flash" apenas não funciona para sua placa, então você precisará gravar manualmente. Veja se já um arquivo de configuração no [diretório de confiruação](../../config) com instruções específicas para gravar o dispositivo. Também, verifique se a documentação do fabricante da placa para ver se ele descreve como gravar o dispositivo. Finalmente, pode ser possível gravar o dispositivo manualmente usando ferramentas tais como "avrdude" ou "bossac" - veja o [documento de bootloader](Bootloaders.md) para mais informações.

<a id="how-do-i-change-the-serial-baud-rate"></a>
## Como eu posso mudar a taxa de transmissão serial?

A taxa de transmissão recomendada para o Klipper é 250000. Essa taxa de transmissão funciona bem em todas placas de microcontroladores suportadas pelo Klipper. Se você encontrar um guia online recomendando uma taxa de transmissão diferente, então ignore essa parte do guia e continue com o valor padrão de 250000.

Se você ainda assim deseja mudar a taxa de transmissão, então a nova taxa precisará ser configurada no microcontrolador (durante **make menuconfig**) e o código atualizado precisará ser compilado e gravado no microcontrolador. O arquivo printer.cfg do Klipper também precisará ser atualizado para igualar a taxa de transmissão (veja a [referência de configuração](Config_Reference.md#mcu) para detalhes). Por exemplo:

```
[mcu]
baud: 250000
```

A taxa de transmissão apresentado na página do OctoPrint não tem impacto na taxa de transmissão interna do Klipper com o microcontrolador. Sempre utilize no OctoPrint a taxa de transmissão 250000 quando usar o Klipper.

A taxa de transmissão do Klipper não é relacionada a taxa de transmissão do bootloader do microcontrolador. Veja o [documento de bootloader](Bootloaders.md) para mais informações sobre bootloaders.

<a id="can-i-run-klipper-on-something-other-than-a-raspberry-pi-3"></a>
## Eu posso rodar o Klipper em outra coisa além do Raspberry Pi 3?

O hardware recomendado é Raspberry Pi Zero2w, Raspberry Pi 3, Raspberry Pi 4 ou Raspberry Pi 5. O Klipper também executará em outros dispositivos SBC, bem como em hardware x86, conforme descrito abaixo.

O Klipper executará em Raspberry Pi 1, 2 e no Raspberry Pi Zero1, mas essas placas não têm poder de processamento suficiente para executar bem o Klipper. É comum ocorrerem pausas de impressão nessas máquinas mais lentas (A impressora pode se mover mais rápido do que o Klipper consegue enviar comandos de movimento.) Não é recomendado executar o Klipper nessas máquinas mais antigas.

Para executar no Beaglebone, veja as [instruções específicas de instalação para Beaglebone](Beaglebone.md).

Klipper já foi rodado em outras máquinas. O software hospedeiro do Klipper só requer Python rodando em um computador Linux (ou similar). Entretanto, se você deseja rodar isso em uma máquina diferente, você precisará de conhecimento de administração Linux para instalar os pré-requisitos de sistema para a maquina específica. Veja o script [install-octopi.sh](../../scripts/install-octopi.sh) para mais informações sobre os passos de administração Linux necessários.

Se você pretende rodar o software hospedeiro Klipper em num chip low-end, tenha ciência que, no mínimo, uma máquina de hardware com "precisão de ponto flutuante dupla" é necessária.

Se você pretende executar o software hospedeiro Klipper em uma máquina compartilhada de uso geral de classe desktop ou servidor, observe que o Klipper tem alguns requisitos de agendamento em tempo real. Se, durante uma impressão, o computador hospedeiro também realizar uma tarefa intensiva de computação de uso geral (como desfragmentar um disco rígido, renderização 3D, swapping intenso, etc.), isso pode fazer o Klipper relatar erros de impressão.

Nota: Se você não estiver usando uma imagem OctoPi, esteja ciente de que várias distribuições Linux habilitam um pacote "ModemManager" (ou semelhante) que pode interromper a comunicação serial. (O que pode fazer o Klipper relatar erros aparentemente aleatórios de "Lost communication with MCU".) Se você instalar o Klipper em uma dessas distribuições, pode ser necessário desativar esse pacote.

<a id="can-i-run-multiple-instances-of-klipper-on-the-same-host-machine"></a>
## Posso executar várias instâncias do Klipper na mesma máquina hospedeira?

É possível executar várias instâncias do software hospedeiro Klipper, mas fazê-lo requer conhecimento de administração Linux. Os scripts de instalação do Klipper em última análise fazem com que o seguinte comando Unix seja executado:

```
~/klippy-env/bin/python ~/klipper/klippy/klippy.py ~/printer.cfg -l /tmp/klippy.log
```
Pode-se executar várias instâncias do comando acima, desde que cada instância tenha seu próprio arquivo de configuração da impressora, seu próprio arquivo de registro e seu próprio pseudo-tty. Por exemplo:

```
~/klippy-env/bin/python ~/klipper/klippy/klippy.py ~/printer2.cfg -l /tmp/klippy2.log -I /tmp/printer2
```

Se você optar por fazer isso, precisará implementar os scripts necessários de inicialização, parada e instalação (se houver). O script [install-octopi.sh](../../scripts/install-octopi.sh) e o script [klipper-start.sh](../../scripts/klipper-start.sh) podem ser úteis como exemplos.

<a id="do-i-have-to-use-octoprint"></a>
## Preciso usar o OctoPrint?

O software Klipper não depende do OctoPrint. É possível usar software alternativo para enviar comandos ao Klipper, mas fazê-lo requer conhecimento de administração Linux.

O Klipper cria uma "porta serial virtual" via o arquivo "/tmp/printer", e emula uma interface serial clássica de impressora 3D via esse arquivo. Em geral, software alternativo pode funcionar com o Klipper desde que possa ser configurado para usar "/tmp/printer" como porta serial da impressora.

<a id="why-cant-i-move-the-stepper-before-homing-the-printer"></a>
## Por que não posso mover o motor de passo antes de referenciar a impressora?

O código faz isso para reduzir a chance de comandar acidentalmente o cabeçote contra a mesa ou uma parede. Uma vez referenciada a impressora, o software tenta verificar se cada movimento está dentro de position_min/max definidos no arquivo de configuração. Se os motores forem desativados (via comando M84 ou M18), os motores precisarão ser referenciados novamente antes do movimento.

Se você quiser mover o cabeçote após cancelar uma impressão via OctoPrint, considere alterar a sequência de cancelamento do OctoPrint para fazer isso para você. É configurado no OctoPrint via navegador em: Settings->GCODE Scripts

Se você quiser mover o cabeçote após a conclusão de uma impressão, considere adicionar o movimento desejado à seção "custom g-code" do seu fatiador.

Se a impressora exigir algum movimento adicional como parte do próprio processo de referenciamento (ou fundamentalmente não tiver um processo de referenciamento), considere usar uma seção safe_z_home ou homing_override no arquivo de configuração. Se você precisar mover um motor de passo para fins de diagnóstico ou depuração, considere adicionar uma seção force_move ao arquivo de configuração. Veja [referência de configuração](Config_Reference.md#customized-homing) para mais detalhes sobre essas opções.

<a id="why-is-the-z-position_endstop-set-to-05-in-the-default-configs"></a>
## Por que o position_endstop de Z está definido como 0.5 nas configurações padrão?

Para impressoras do estilo cartesiano, o position_endstop de Z especifica a distância do bico à mesa quando o fim de curso aciona. Se possível, recomenda-se usar um fim de curso de Z-máx e referenciar longe da mesa (pois isso reduz o potencial de colisões com a mesa). No entanto, se for preciso referenciar em direção à mesa, recomenda-se posicionar o fim de curso de modo que acione quando o bico ainda estiver a uma pequena distância da mesa. Dessa forma, ao referenciar o eixo, ele parará antes de o bico tocar a mesa. Veja o [documento de nivelamento da mesa](Bed_Level.md) para mais informações.

<a id="i-converted-my-config-from-marlin-and-the-xy-axes-work-fine-but-i-just-get-a-screeching-noise-when-homing-the-z-axis"></a>
## Converti minha configuração do Marlin e os eixos X/Y funcionam bem, mas só ouço um ruído estridente ao referenciar o eixo Z

Resposta curta: Primeiro, certifique-se de ter verificado a configuração dos motores de passo conforme descrito no [documento de verificação de configuração](Config_checks.md). Se o problema persistir, tente reduzir o ajuste max_z_velocity na configuração da impressora.

Resposta longa: Na prática, o Marlin normalmente só consegue dar passos a uma taxa de cerca de 10000 passos por segundo. Se for solicitado a mover a uma velocidade que exigiria uma taxa de passos maior, o Marlin geralmente apenas dará passos o mais rápido que conseguir. O Klipper é capaz de atingir taxas de passos muito maiores, mas o motor de passo pode não ter torque suficiente para se mover em velocidade maior. Assim, para um eixo Z com alta relação de engrenagem ou ajuste alto de micropassos, o max_z_velocity realmente alcançável pode ser menor do que o configurado no Marlin.

<a id="my-tmc-motor-driver-turns-off-in-the-middle-of-a-print"></a>
## Meu driver de motor TMC desliga no meio de uma impressão

Se estiver usando o driver TMC2208 (ou TMC2224) em "modo standalone", certifique-se de usar a [versão mais recente do Klipper](#how-do-i-upgrade-to-the-latest-software). Uma solução alternativa para um problema de driver "stealthchop" do TMC2208 foi adicionada ao Klipper em meados de março de 2020.

<a id="i-keep-getting-random-lost-communication-with-mcu-errors"></a>
## Continuo recebendo erros aleatórios de "Lost communication with MCU"

Isso geralmente é causado por erros de hardware na conexão USB entre a máquina hospedeira e o microcontrolador. Itens a verificar:

- Use um cabo USB de boa qualidade entre a máquina hospedeira e o microcontrolador. Certifique-se de que os conectores estejam firmes.
- Se estiver usando um Raspberry Pi, use uma [fonte de alimentação de boa qualidade](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#power-supply) para o Raspberry Pi e use um [cabo USB de boa qualidade](https://forums.raspberrypi.com/viewtopic.php?p=589877#p589877) para conectar essa fonte de alimentação ao Pi. Se receber avisos de "subtensão" do OctoPrint, isso está relacionado à fonte de alimentação e deve ser corrigido.
- Certifique-se de que a fonte de alimentação da impressora não esteja sobrecarregada. (Flutuações de energia no chip USB do microcontrolador podem resultar em reinicializações desse chip.)
- Verifique se os fios do motor de passo, do aquecedor e outros fios da impressora não estão prensados ou desgastados. (O movimento da impressora pode tensionar um fio defeituoso, fazendo-o perder contato, entrar brevemente em curto ou gerar ruído excessivo.)
- Há relatos de alto ruído USB quando a fonte de alimentação da impressora e a fonte de alimentação de 5V do hospedeiro são misturadas. (Se você constatar que o microcontrolador liga quando a fonte de alimentação da impressora está ligada ou o cabo USB está conectado, isso indica que as fontes de alimentação de 5V estão sendo misturadas.) Pode ajudar configurar o microcontrolador para usar energia de apenas uma fonte. (Como alternativa, se a placa do microcontrolador não puder configurar sua fonte de energia, pode-se modificar um cabo USB para que ele não transporte energia de 5V entre o hospedeiro e o microcontrolador.)

<a id="my-raspberry-pi-keeps-rebooting-during-prints"></a>
## Meu Raspberry Pi continua reiniciando durante as impressões

Isso provavelmente se deve a flutuações de tensão. Siga as mesmas etapas de solução de problemas para um erro de ["Lost communication with MCU"](#i-keep-getting-random-lost-communication-with-mcu-errors).

<a id="when-i-set-restart_methodcommand-my-avr-device-just-hangs-on-a-restart"></a>
## Quando defino `restart_method=command`, meu dispositivo AVR apenas trava ao reiniciar

Algumas versões antigas do bootloader AVR têm um bug conhecido no tratamento de eventos de watchdog. Isso tipicamente se manifesta quando o arquivo printer.cfg tem restart_method definido como "command". Quando o bug ocorre, o dispositivo AVR ficará sem resposta até que a energia seja removida e reaplicada ao dispositivo (os LEDs de energia ou status também podem piscar repetidamente até que a energia seja removida).

A solução alternativa é usar um restart_method diferente de "command" ou gravar um carregador de inicialização atualizado no dispositivo AVR. Gravar um novo carregador de inicialização é uma etapa única que normalmente exige um programador externo - consulte [Carregadores de inicialização](Bootloaders.md) para mais detalhes.

<a id="will-the-heaters-be-left-on-if-the-raspberry-pi-crashes"></a>
## Os aquecedores permanecerão ligados se o Raspberry Pi travar?

O software foi projetado para evitar isso. Quando o hospedeiro ativa um aquecedor, o software hospedeiro precisa confirmar essa ativação a cada 3 segundos. Se o microcontrolador não receber uma confirmação a cada 3 segundos, ele entra em estado de "shutdown", projetado para desligar todos os aquecedores e motores de passo.

Consulte o comando "config_digital_out" no documento [Comandos do MCU](MCU_Commands.md) para mais detalhes.

Além disso, o software do microcontrolador é configurado na inicialização com uma faixa mínima e máxima de temperatura para cada aquecedor (consulte os parâmetros min_temp e max_temp na [referência de configuração](Config_Reference.md#extruder) para detalhes). Se o microcontrolador detectar que a temperatura está fora dessa faixa, ele também entrará em estado de "shutdown".

Separadamente, o software hospedeiro também implementa código para verificar se os aquecedores e os sensores de temperatura estão funcionando corretamente. Consulte a [referência de configuração](Config_Reference.md#verify_heater) para mais detalhes.

<a id="how-do-i-convert-a-marlin-pin-number-to-a-klipper-pin-name"></a>
## Como converto um número de pino do Marlin em um nome de pino do Klipper?

Resposta curta: há um mapeamento disponível no arquivo [sample-aliases.cfg](../../config/sample-aliases.cfg). Use esse arquivo como guia para encontrar os nomes reais dos pinos do microcontrolador. (Também é possível copiar a seção de configuração [board_pins](Config_Reference.md#board_pins) relevante para o seu arquivo de configuração e usar os apelidos na sua configuração, mas é preferível traduzir e usar os nomes reais dos pinos do microcontrolador.) Observe que o arquivo sample-aliases.cfg usa nomes de pinos que começam com o prefixo "ar" em vez de "D" (por exemplo, o pino `D23` do Arduino é o apelido `ar23` do Klipper) e o prefixo "analog" em vez de "A" (por exemplo, o pino `A14` do Arduino é o apelido `analog14` do Klipper).

Resposta longa: o Klipper usa os nomes de pinos padrão definidos pelo microcontrolador. Nos chips Atmega, esses pinos de hardware têm nomes como `PA4`, `PC7` ou `PD2`.

Há muito tempo, o projeto Arduino decidiu evitar o uso dos nomes de hardware padrão em favor de seus próprios nomes de pinos baseados em números incrementais - esses nomes do Arduino geralmente têm a forma `D23` ou `A14`. Essa foi uma escolha infeliz que gerou muita confusão. Em particular, os números de pinos do Arduino frequentemente não correspondem aos mesmos nomes de hardware. Por exemplo, `D21` é `PD0` em uma placa Arduino comum, mas é `PC7` em outra placa Arduino comum.

Para evitar essa confusão, o código principal do Klipper usa os nomes de pinos padrão definidos pelo microcontrolador.

<a id="do-i-have-to-wire-my-device-to-a-specific-type-of-micro-controller-pin"></a>
## Preciso conectar meu dispositivo a um tipo específico de pino do microcontrolador?

Depende do tipo de dispositivo e do tipo de pino:

Pinos ADC (ou pinos analógicos): para termistores e sensores "analógicos" semelhantes, o dispositivo deve ser ligado a um pino do microcontrolador com capacidade "analógica" ou "ADC". Se você configurar o Klipper para usar um pino sem capacidade analógica, o Klipper informará um erro "Not a valid ADC pin".

Pinos PWM (ou pinos de temporizador): o Klipper não usa PWM por hardware por padrão para nenhum dispositivo. Assim, em geral, pode-se ligar aquecedores, ventiladores e dispositivos semelhantes a qualquer pino de E/S de uso geral. No entanto, ventiladores e dispositivos output_pin podem ser opcionalmente configurados para usar `hardware_pwm: True`, caso em que o microcontrolador deve suportar PWM por hardware no pino (caso contrário, o Klipper informará um erro "Not a valid PWM pin").

Pinos IRQ (ou pinos de interrupção): o Klipper não usa interrupções de hardware nos pinos de IO, portanto nunca é necessário conectar um dispositivo a um desses pinos do microcontrolador.

Pinos SPI: ao usar SPI de hardware é necessário conectar os pinos aos pinos do microcontrolador com capacidade SPI. No entanto, a maioria dos dispositivos pode ser configurada para usar "SPI por software", caso em que quaisquer pinos de IO de uso geral podem ser usados.

Pinos I2C: ao usar I2C é necessário conectar os pinos aos pinos do microcontrolador com capacidade I2C.

Outros dispositivos podem ser conectados a qualquer pino de IO de uso geral. Por exemplo, motores de passo, aquecedores, ventiladores, sondas Z, servos, LEDs, telas LCD comuns hd44780/st7920 e a linha de controle UART da Trinamic podem ser conectados a qualquer pino de IO de uso geral.

<a id="how-do-i-cancel-an-m109m190-wait-for-temperature-request"></a>
## Como cancelo uma solicitação de espera de temperatura M109/M190?

Navegue até a aba de terminal do OctoPrint e emita um comando M112 na caixa do terminal. O comando M112 fará o Klipper entrar em estado de "shutdown" e fará o OctoPrint se desconectar do Klipper. Navegue até a área de conexão do OctoPrint e clique em "Connect" para fazer o OctoPrint se reconectar. Volte à aba de terminal e emita um comando FIRMWARE_RESTART para limpar o estado de erro do Klipper. Após concluir essa sequência, a solicitação anterior de aquecimento será cancelada e uma nova impressão poderá ser iniciada.

<a id="can-i-find-out-whether-the-printer-has-lost-steps"></a>
## É possível saber se a impressora perdeu passos?

De certa forma, sim. Faça o homing da impressora, emita um comando `GET_POSITION`, execute sua impressão, faça o homing novamente e emita outro `GET_POSITION`. Em seguida, compare os valores na linha `mcu:`.

Isso pode ser útil para ajustar configurações como correntes dos motores de passo, acelerações e velocidades sem precisar imprimir algo de fato e desperdiçar filamento: basta executar alguns movimentos em alta velocidade entre os comandos `GET_POSITION`.

Observe que as chaves de fim de curso tendem a disparar em posições ligeiramente diferentes, portanto uma diferença de alguns micropassos provavelmente resulta de imprecisões do fim de curso. Um motor de passo só pode perder passos em incrementos de 4 passos completos. (Assim, se estiver usando 16 micropassos, uma perda de passo no motor de passo faria o contador de passos "mcu:" ficar desviado em um múltiplo de 64 micropassos.)

<a id="why-does-klipper-report-errors-i-lost-my-print"></a>
## Por que o Klipper relata erros? Perdi minha impressão!

Resposta curta: queremos saber se nossas impressoras detectam um problema para que a causa subjacente possa ser corrigida e possamos obter impressões de ótima qualidade. Definitivamente não queremos que nossas impressoras produzam silenciosamente impressões de baixa qualidade.

Resposta longa: o Klipper foi projetado para contornar automaticamente muitos problemas transitórios. Por exemplo, ele detecta automaticamente erros de comunicação e retransmite; agenda ações com antecedência e armazena comandos em buffer em várias camadas para permitir temporização precisa mesmo com interferência intermitente. No entanto, se o software detectar um erro do qual não consegue se recuperar, se for comandado a executar uma ação inválida ou se detectar que está irremediavelmente incapaz de executar sua tarefa comandada, o Klipper relatará um erro. Nessas situações há um alto risco de produzir uma impressão de baixa qualidade (ou pior). Espera-se que alertar o usuário o capacite a corrigir o problema subjacente e melhorar a qualidade geral de suas impressões.

Há algumas perguntas relacionadas: Por que o Klipper não pausa a impressão em vez disso? Relata um aviso em vez disso? Verifica erros antes da impressão? Ignora erros em comandos digitados pelo usuário? etc? Atualmente o Klipper lê comandos usando o protocolo G-Code e, infelizmente, o protocolo de comandos G-Code não é flexível o suficiente para tornar essas alternativas práticas hoje. Há interesse dos desenvolvedores em melhorar a experiência do usuário durante eventos anormais, mas espera-se que isso exija um trabalho notável de infraestrutura (incluindo uma mudança para longe do G-Code).

<a id="how-do-i-upgrade-to-the-latest-software"></a>
## Como atualizo para o software mais recente?

O primeiro passo para atualizar o software é revisar o documento mais recente de [alterações de configuração](Config_Changes.md). Ocasionalmente, são feitas alterações no software que exigem que os usuários atualizem suas configurações como parte de uma atualização de software. É uma boa ideia revisar este documento antes de atualizar.

Quando estiver pronto para atualizar, o método geral é acessar via ssh o Raspberry Pi e executar:

```
cd ~/klipper
git pull
~/klipper/scripts/install-octopi.sh
```

Então pode-se recompilar e gravar o código do microcontrolador. Por exemplo:

```
make menuconfig
make clean
make

sudo service klipper stop
make flash FLASH_DEVICE=/dev/ttyACM0
sudo service klipper start
```

No entanto, é comum que apenas o software hospedeiro seja alterado. Nesse caso, pode-se atualizar e reiniciar apenas o software hospedeiro com:

```
cd ~/klipper
git pull
sudo service klipper restart
```

Se após usar esse atalho o software avisar sobre a necessidade de regravar o microcontrolador ou ocorrer algum outro erro incomum, siga as etapas completas de atualização descritas acima.

Se algum erro persistir, verifique novamente o documento de [alterações de configuração](Config_Changes.md), pois pode ser necessário modificar a configuração da impressora.

Observe que os comandos de g-code RESTART e FIRMWARE_RESTART não carregam software novo - os comandos "sudo service klipper restart" e "make flash" acima são necessários para que uma alteração de software tenha efeito.

<a id="how-do-i-uninstall-klipper"></a>
## Como desinstalo o Klipper?

No lado do firmware, nada especial precisa ser feito. Basta seguir as instruções de gravação do novo firmware.

No lado do raspberry pi, há um script de desinstalação disponível em [scripts/klipper-uninstall.sh](../../scripts/klipper-uninstall.sh). Por exemplo:

```
sudo ~/klipper/scripts/klipper-uninstall.sh
rm -rf ~/klippy-env ~/klipper
```
