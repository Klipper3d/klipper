<a id="canbus"></a>
# CANBUS

[English](../CANBUS.md) | **Português brasileiro**

Este documento descreve o suporte ao barramento CAN do Klipper.

<a id="device-hardware"></a>
## Hardware do dispositivo

O Klipper atualmente suporta CAN em chips stm32, SAME5x e rp2040. Além disso, o chip do microcontrolador deve estar em uma placa que tenha um transceptor CAN.

Para compilar para CAN, execute `make menuconfig` e selecione "CAN bus" como interface de comunicação. Por fim, compile o código do microcontrolador e grave-o na placa de destino.

<a id="host-hardware"></a>
## Hardware do host

Para usar um barramento CAN, é necessário ter um adaptador host. Recomenda-se usar um "adaptador USB para CAN". Há muitos adaptadores USB para CAN diferentes disponíveis de fabricantes diferentes. Ao escolher um, recomendamos verificar se o firmware pode ser atualizado nele. (Infelizmente, descobrimos que alguns adaptadores USB executam firmware defeituoso e são bloqueados, então verifique antes de comprar.) Procure adaptadores que possam executar o Klipper diretamente (em seu "modo ponte USB para CAN") ou que executem o [firmware candlelight](https://github.com/candle-usb/candleLight_fw).

Também é necessário configurar o sistema operacional host para usar o adaptador. Isso normalmente é feito criando um novo arquivo chamado `/etc/network/interfaces.d/can0` com o seguinte conteúdo:

```
allow-hotplug can0
iface can0 can static
    bitrate 1000000
    up ip link set $IFACE txqueuelen 128
```

<a id="terminating-resistors"></a>
## Resistores de terminação

Um barramento CAN deve ter dois resistores de 120 ohm entre os fios CANH e CANL. Idealmente, um resistor localizado em cada extremidade do barramento.

Observe que alguns dispositivos têm um resistor embutido de 120 ohm que não pode ser facilmente removido. Alguns dispositivos não incluem resistor algum. Outros dispositivos têm um mecanismo para selecionar o resistor (normalmente conectando um "jumper de pino"). Certifique-se de verificar os esquemas de todos os dispositivos no barramento CAN para garantir que existam dois e somente dois resistores de 120 Ohm no barramento.

Para testar se os resistores estão corretos, pode-se remover a energia da impressora e usar um multímetro para verificar a resistência entre os fios CANH e CANL - ela deve indicar ~60 ohms em um barramento CAN corretamente ligado.

<a id="finding-the-canbus_uuid-for-new-micro-controllers"></a>
## Localizando o canbus_uuid para novos microcontroladores

Cada microcontrolador no barramento CAN recebe um id exclusivo baseado no identificador de chip de fábrica codificado em cada microcontrolador. Para encontrar o id de cada dispositivo microcontrolador, certifique-se de que o hardware esteja ligado e conectado corretamente, e então execute:

```
~/klippy-env/bin/python ~/klipper/scripts/canbus_query.py can0
```

Se dispositivos CAN não inicializados forem detectados, o comando acima relatará linhas como as seguintes:

```
Found canbus_uuid=11aa22bb33cc, Application: Klipper
```

Cada dispositivo terá um identificador exclusivo. No exemplo acima, `11aa22bb33cc` é o "canbus_uuid" do microcontrolador.

Observe que a ferramenta `canbus_query.py` relatará apenas dispositivos não inicializados - se o Klipper (ou uma ferramenta semelhante) configurar o dispositivo, ele não aparecerá mais na lista.

<a id="configuring-klipper"></a>
## Configurando Klipper

Atualize a [configuração de mcu](Config_Reference.md#mcu) do Klipper para usar o barramento CAN para comunicar com o dispositivo - por exemplo:

```
[mcu my_can_mcu]
canbus_uuid: 11aa22bb33cc
```

<a id="usb-to-can-bus-bridge-mode"></a>
## Modo ponte USB para barramento CAN

Alguns microcontroladores suportam a seleção do modo "ponte USB para barramento CAN" durante o "make menuconfig" do Klipper. Este modo pode permitir usar um microcontrolador tanto como "adaptador USB para barramento CAN" quanto como nó Klipper.

Quando o Klipper usa este modo, o microcontrolador aparece como um "adaptador USB CAN bus" no Linux. O próprio "mcu ponte Klipper" aparecerá como se estivesse neste barramento CAN - ele pode ser identificado via `canbus_query.py` e deve ser configurado como outros nós Klipper do barramento CAN.

Algumas observações importantes ao usar este modo:

* É necessário configurar a interface `can0` (ou similar) no Linux para comunicar-se com o barramento. No entanto, a velocidade do barramento CAN do Linux e as opções de temporização de bit (bit-timing) do barramento CAN são ignoradas pelo Klipper. Atualmente, a frequência do barramento CAN é especificada durante "make menuconfig" e a velocidade do barramento especificada no Linux é ignorada.
* Sempre que o "bridge mcu" for reiniciado, o Linux desabilitará a interface `can0` correspondente. Para garantir o tratamento correto dos comandos FIRMWARE_RESTART e RESTART, recomenda-se usar `allow-hotplug` no arquivo `/etc/network/interfaces.d/can0`. Por exemplo:

```
allow-hotplug can0
iface can0 can static
    bitrate 1000000
    up ip link set $IFACE txqueuelen 128
```

* O "bridge mcu" não está de fato no barramento CAN. Mensagens de e para o bridge mcu não serão vistas por outros adaptadores que possam estar no barramento CAN.
* A largura de banda disponível tanto para o próprio "bridge mcu" quanto para todos os dispositivos no barramento CAN é efetivamente limitada pela frequência do barramento CAN. Como resultado, recomenda-se usar uma frequência de barramento CAN de 1000000 ao usar o modo "USB to CAN bus bridge mode".
* Só é válido usar o modo USB para ponte CAN se houver um barramento CAN funcional com pelo menos um outro nó disponível (além do próprio nó da ponte). Use uma configuração USB padrão se o objetivo for comunicar-se apenas com o único dispositivo USB. Usar o modo USB para ponte CAN sem um barramento CAN totalmente funcional (incluindo resistores de terminação e um nó adicional) pode resultar em erros esporádicos mesmo ao comunicar-se com o nó da ponte.
* Uma placa de ponte USB para CAN não aparecerá como dispositivo serial USB, não será exibida ao executar `ls /dev/serial/by-id` e não pode ser configurada no arquivo printer.cfg do Klipper com um parâmetro `serial:`. A placa de ponte aparece como um "USB CAN adapter" e é configurada no printer.cfg como um [nó CAN](#configuring-klipper).

<a id="tips-for-troubleshooting"></a>
## Dicas para solução de problemas

Consulte o documento [solução de problemas do barramento CAN](CANBUS_Troubleshooting.md).
