<a id="canbus-protocol"></a>
# Protocolo CANBUS

[English](../CANBUS_protocol.md) | **Português brasileiro**

Este documento descreve o protocolo que o Klipper usa para comunicar-se via [barramento CAN](https://en.wikipedia.org/wiki/CAN_bus). Consulte [CANBUS.md](CANBUS.md) para informações sobre como configurar o Klipper com barramento CAN.

<a id="micro-controller-id-assignment"></a>
## Atribuição de id do microcontrolador

O Klipper usa apenas pacotes do barramento CAN de tamanho padrão CAN 2.0A, que são limitados a 8 bytes de dados e a um identificador CAN de 11 bits. Para permitir uma comunicação eficiente, cada microcontrolador recebe em tempo de execução um `canbus_nodeid` exclusivo de 1 byte para o tráfego geral de comandos e respostas do Klipper. As mensagens de comando do Klipper do hospedeiro para o microcontrolador usam o id do barramento CAN de `canbus_nodeid * 2 + 256`, enquanto as mensagens de resposta do Klipper do microcontrolador para o hospedeiro usam `canbus_nodeid * 2 + 256 + 1`.

Cada microcontrolador tem um identificador de chip exclusivo atribuído de fábrica que é usado durante a atribuição de ids. Esse identificador pode exceder o tamanho de um pacote CAN, por isso uma função de hash é usada para gerar um id exclusivo de 6 bytes (`canbus_uuid`) a partir do id de fábrica.

<a id="admin-messages"></a>
## Mensagens de administração

As mensagens de administração são usadas para atribuição de ids. As mensagens de administração enviadas do hospedeiro para o microcontrolador usam o id do barramento CAN `0x3f0` e as mensagens enviadas do microcontrolador para o hospedeiro usam o id do barramento CAN `0x3f1`. Todos os microcontroladores escutam mensagens no id `0x3f0`; esse id pode ser entendido como um "endereço de broadcast".

<a id="cmd_query_unassigned-message"></a>
### Mensagem CMD_QUERY_UNASSIGNED

Este comando consulta todos os microcontroladores que ainda não receberam um `canbus_nodeid`. Microcontroladores não atribuídos responderão com uma mensagem de resposta RESP_NEED_NODEID.

O formato da mensagem CMD_QUERY_UNASSIGNED é: `<1-byte message_id = 0x00>`

<a id="cmd_set_klipper_nodeid-message"></a>
### Mensagem CMD_SET_KLIPPER_NODEID

Este comando atribui um `canbus_nodeid` ao microcontrolador com um determinado `canbus_uuid`.

O formato da mensagem CMD_SET_KLIPPER_NODEID é: `<1-byte message_id = 0x01><6-byte canbus_uuid><1-byte canbus_nodeid>`

<a id="resp_need_nodeid-message"></a>
### Mensagem RESP_NEED_NODEID

O formato da mensagem RESP_NEED_NODEID é: `<1-byte message_id = 0x20><6-byte canbus_uuid><1-byte set_klipper_nodeid = 0x01>`

<a id="data-packets"></a>
## Pacotes de dados

Um microcontrolador que recebeu um nodeid por meio do comando CMD_SET_KLIPPER_NODEID pode enviar e receber pacotes de dados.

Os dados do pacote nas mensagens que usam o id do barramento CAN de recepção do nó (`canbus_nodeid * 2 + 256`) são simplesmente anexados a um buffer, e quando uma [mensagem do protocolo do mcu](Protocol.md) completa é encontrada, seu conteúdo é analisado e processado. Os dados são tratados como um fluxo de bytes - não há exigência de que o início de um bloco de mensagem do Klipper esteja alinhado com o início de um pacote do barramento CAN.

Da mesma forma, as mensagens de resposta do protocolo do mcu são enviadas do microcontrolador para o hospedeiro copiando os dados da mensagem para um ou mais pacotes com o id do barramento CAN de transmissão do nó (`canbus_nodeid * 2 + 256 + 1`).
