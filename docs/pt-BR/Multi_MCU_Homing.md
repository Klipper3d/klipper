<a id="multiple-micro-controller-homing-and-probing"></a>
# Homing e sondagem de vários microcontroladores

[English](../Multi_MCU_Homing.md) | **Português brasileiro**

O Klipper oferece suporte a um mecanismo para fazer o referenciamento (homing) com um fim de curso conectado a um microcontrolador enquanto seus motores de passo estão em um microcontrolador diferente. Esse suporte é chamado de "multi-mcu homing". Esse recurso também é usado quando uma sonda Z está em um microcontrolador diferente dos motores de passo Z.

Esse recurso pode ser útil para simplificar a fiação, pois pode ser mais conveniente conectar um fim de curso ou sonda a um microcontrolador mais próximo. No entanto, o uso desse recurso pode resultar em "overshoot" (ultrapassagem) dos motores de passo durante operações de referenciamento (homing) e sondagem.

A ultrapassagem (overshoot) ocorre devido a possíveis atrasos na transmissão de mensagens entre o microcontrolador que monitora o fim de curso e os microcontroladores que movem os motores de passo. O código do Klipper foi projetado para limitar esse atraso a no máximo 25ms. (Quando o multi-mcu homing é ativado, os microcontroladores enviam mensagens periódicas de status e verificam se as mensagens de status correspondentes são recebidas dentro de 25ms.)

Assim, por exemplo, se fizer o referenciamento a 10mm/s, é possível que ocorra uma ultrapassagem de até 0.250mm (10mm/s * .025s == 0.250mm). Deve-se ter cuidado ao configurar o multi-mcu homing para levar em conta esse tipo de ultrapassagem. Usar velocidades mais lentas de referenciamento ou sondagem pode reduzir a ultrapassagem.

A ultrapassagem do motor de passo não deve afetar negativamente a precisão do procedimento de referenciamento e sondagem. O código do Klipper detectará a ultrapassagem e a levará em conta em seus cálculos. No entanto, é importante que o projeto do hardware seja capaz de suportar a ultrapassagem sem causar danos à máquina.

Para usar esse recurso de "multi-mcu homing", o hardware deve ter latência previsivelmente baixa entre o computador hospedeiro (host) e todos os microcontroladores. Normalmente, o tempo de ida e volta (round-trip) deve ser consistentemente inferior a 10ms. Latência alta (mesmo por curtos períodos) provavelmente resultará em falhas de referenciamento.

Se a latência alta resultar em falha (ou se algum outro problema de comunicação for detectado), o Klipper gerará um erro "Communication timeout during homing".

Observe que um eixo com vários motores de passo (por ex., `stepper_z` e `stepper_z1`) precisa estar no mesmo microcontrolador para usar o multi-mcu homing. Por exemplo, se um fim de curso estiver em um microcontrolador separado do `stepper_z`, então o `stepper_z1` deverá estar no mesmo microcontrolador que o `stepper_z`.
