<a id="bootloader-entry"></a>
# Entrada no bootloader

[English](../Bootloader_Entry.md) | **Português brasileiro**

O Klipper pode ser instruído a reiniciar em um [bootloader](Bootloaders.md) de uma das seguintes maneiras:

<a id="requesting-the-bootloader"></a>
## Solicitação do bootloader

<a id="virtual-serial"></a>
### Serial virtual

Se uma porta serial virtual (USB-ACM) estiver em uso, pulsar DTR a 1200 baud solicitará o bootloader.

<a id="python-with-flash_usb"></a>
#### Python (com `flash_usb`)

Para entrar no bootloader usando python (usando `flash_usb`):

```shell
> cd klipper/scripts
> python3 -c 'import flash_usb as u; u.enter_bootloader("<DEVICE>")'
Entering bootloader on <DEVICE>
```

Onde `<DEVICE>` é seu dispositivo serial, como `/dev/serial.by-id/usb-Klipper[...]` ou `/dev/ttyACM0`

Observe que, se isso falhar, nenhuma saída será impressa; o sucesso é indicado pela impressão de `Entering bootloader on <DEVICE>`.

<a id="picocom"></a>
#### Picocom

```shell
picocom -b 1200 <DEVICE>
<Ctrl-A><Ctrl-P>
```

Onde `<DEVICE>` é seu dispositivo serial, como `/dev/serial.by-id/usb-Klipper[...]` ou `/dev/ttyACM0`

`<Ctrl-A><Ctrl-P>` significa manter `Ctrl` pressionado, pressionar e soltar `a`, pressionar e soltar `p` e, em seguida, soltar `Ctrl`

<a id="physical-serial"></a>
### Serial física

Se uma porta serial física estiver sendo usada no MCU (mesmo que um adaptador serial USB esteja sendo usado para conectar-se a ela), enviar a string `<SPACE><FS><SPACE>Request Serial Bootloader!!<SPACE>~` solicita o bootloader.

`<SPACE>` é um espaço literal ASCII, 0x20.

`<FS>` é o separador de arquivo ASCII, 0x1c.

Observe que esta não é uma mensagem válida conforme o [protocolo do MCU](Protocol.md#micro-controller-interface), mas os caracteres de sincronismo (`~`) ainda são respeitados.

Como esta mensagem deve ser a única coisa no "bloco" em que é recebida, prefixar um caractere de sincronismo extra pode aumentar a confiabilidade se outras ferramentas estavam acessando anteriormente a porta serial.

<a id="shell"></a>
#### Shell

```shell
stty <BAUD> < /dev/<DEVICE>
echo $'~ \x1c Request Serial Bootloader!! ~' >> /dev/<DEVICE>
```

Onde `<DEVICE>` é sua porta serial, como `/dev/ttyS0` ou `/dev/serial/by-id/gpio-serial2`, e

`<BAUD>` é a taxa de transmissão (baud rate) da porta serial, como `115200`.

<a id="canbus"></a>
### CANBUS

Se CANBUS estiver em uso, uma [mensagem administrativa](CANBUS_protocol.md#admin-messages) especial solicitará o bootloader. Esta mensagem será respeitada mesmo que o dispositivo já tenha um nodeid, e também será processada se o MCU estiver em estado de "shutdown".

Este método também se aplica a dispositivos operando em modo [CANBridge](CANBUS.md#usb-to-can-bus-bridge-mode).

<a id="katapults-flashtoolpy"></a>
#### flashtool.py do Katapult

```shell
python3 ./katapult/scripts/flashtool.py -i <CAN_IFACE> -u <UUID> -r
```

Onde `<CAN_IFACE>` é a interface CAN a ser usada. Se estiver usando `can0`, tanto `-i` quanto `<CAN_IFACE>` podem ser omitidos.

`<UUID>` é o UUID do seu dispositivo CAN.

Consulte a [documentação CANBUS](CANBUS.md#finding-the-canbus_uuid-for-new-micro-controllers) para informações sobre como localizar o UUID CAN dos seus dispositivos.

<a id="entering-the-bootloader"></a>
## Entrada no bootloader

Quando o Klipper recebe uma das solicitações de bootloader acima:

Se o Katapult (anteriormente conhecido como CANBoot) estiver disponível, o Klipper solicitará que o Katapult permaneça ativo na próxima inicialização e, em seguida, reiniciará o MCU (entrando, portanto, no Katapult).

Se o Katapult não estiver disponível, o Klipper tentará entrar em um bootloader específico da plataforma, como o modo DFU do STM32 ([ver observação](#stm32-dfu-warning)).

Em resumo, o Klipper reiniciará no Katapult se instalado, e depois em um bootloader específico do hardware, se disponível.

Para detalhes sobre os bootloaders específicos em várias plataformas, consulte [Bootloaders](Bootloaders.md)

<a id="notes"></a>
## Observações

<a id="stm32-dfu-warning"></a>
### Aviso sobre DFU do STM32

Observe que, em algumas placas, como a Octopus Pro v1, entrar em modo DFU pode causar ações indesejadas (como alimentar o aquecedor enquanto em modo DFU). Recomenda-se desconectar os aquecedores e, de outra forma, impedir operações indesejadas ao usar o modo DFU. Consulte a documentação da sua placa para mais detalhes.
