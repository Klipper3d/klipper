<a id="configuration-reference"></a>
# Referência de configuração

[English](../Config_Reference.md) | **Português brasileiro**

Esse documento é uma referência às opções disponíveis no arquivo de configurações do Klipper.

As descrições neste documento são formatadas de modo que seja possível recortá-las e colá-las em um arquivo de configuração da impressora. Veja o [documento de instalação](Installation.md) para informações sobre como configurar o Klipper e escolher um arquivo de configuração inicial.

<a id="micro-controller-configuration"></a>
## Configuração do microcontrolador

<a id="format-of-micro-controller-pin-names"></a>
### Formato dos nomes de pinos do microcontrolador

Muitas opções de configuração exigem o nome de um pino do microcontrolador. O Klipper usa os nomes de hardware para esses pinos - por exemplo `PA4`.

Nomes de pinos podem ser precedidos por `!` para indicar que uma polaridade reversa deve ser usada (por ex., acionar em nível baixo em vez de alto).

Pinos de entrada podem ser precedidos por `^` para indicar que um resistor de pull-up de hardware deve ser habilitado para o pino. Se o microcontrolador suportar resistores de pull-down, um pino de entrada pode alternativamente ser precedido por `~`.

Observe que algumas seções de configuração podem "criar" pinos adicionais. Quando isso ocorrer, a seção de configuração que define os pinos deve estar listada no arquivo de configuração antes de quaisquer seções que usem esses pinos.

<a id="mcu"></a>
### [mcu]

Configuração do microcontrolador primário.

```
[mcu]
serial:
#   A porta serial para conectar ao MCU. Se estiver em dúvida (ou se ela mudar)
#   consulte a seção "Onde está minha porta serial?" do FAQ. Este parâmetro
#   deve ser informado ao usar porta serial.
#baud: 250000
#   A taxa baud a usar. O padrão é 250000.
#canbus_uuid:
#   Se estiver usando dispositivo conectado a barramento CAN, isto define o
#   identificador único de chip para conectar. Este valor deve ser informado ao
#   usar barramento CAN para comunicação.
#canbus_interface:
#   Se estiver usando dispositivo conectado a barramento CAN, isto define a
#   interface de rede CAN a usar. O padrão é 'can0'.
#restart_method:
#   Isto controla o mecanismo que o host usará para resetar o microcontrolador.
#   As opções são 'arduino', 'cheetah', 'rpi_usb' e 'command'. O método
#   'arduino' (alternar DTR) é comum em placas Arduino e clones. O método
#   'cheetah' é um método especial necessário para algumas placas Fysetc
#   Cheetah. O método 'rpi_usb' é útil em placas Raspberry Pi com
#   microcontroladores alimentados por USB - ele desliga brevemente a
#   alimentação de todas as portas USB para realizar reset do microcontrolador.
#   O método 'command' envolve enviar comando Klipper ao microcontrolador para
#   que ele se resete. O padrão é 'arduino' se o microcontrolador se comunica
#   por porta serial, 'command' nos demais casos.
```

<a id="mcu-my_extra_mcu"></a>
### [mcu my_extra_mcu]

Microcontroladores adicionais (pode-se definir qualquer número de secções com prefixo "mcu"). Microcontroladores adicionais introduzem pinos extras que podem ser configurados como aquecedores, motores de passo, ventoinhas, etc. Por exemplo, se uma secção "[mcu extra_mcu]" é inserida, então pinos como "extra_mcu:ar9" podem ser usados em outros lugares da configuração (onde "ar9" é o nome do pino de hardware ou apelido em determinado MCU)

```
[mcu my_extra_mcu]
# Consulte a seção "mcu" para parâmetros de configuração.
```

<a id="common-kinematic-settings"></a>
## Definições comuns para cinemática

<a id="printer"></a>
### [printer]

A seção de impressora controla configurações de alto nível da impressora.

```
[printer]
kinematics:
#   O tipo de impressora em uso. Esta opção pode ser uma de: cartesian, corexy,
#   corexz, hybrid_corexy, hybrid_corexz, generic_cartesian, rotary_delta,
#   delta, deltesian, polar, winch ou none. Este parâmetro deve ser
#   especificado.
max_velocity:
#   Velocidade máxima (em mm/s) do cabeçote (em relação à impressão). Este
#   valor pode ser alterado em tempo de execução usando o
#   comando SET_VELOCITY_LIMIT. Este parâmetro deve ser especificado.
max_accel:
#   Aceleração máxima (em mm/s^2) do cabeçote (em relação à impressão). Embora
#   este parâmetro seja descrito como aceleração "máxima", na prática a maioria
#   dos movimentos que aceleram ou desaceleram o fará na taxa especificada
#   aqui. O valor especificado aqui pode ser alterado em tempo de execução
#   usando o comando SET_VELOCITY_LIMIT. Este parâmetro deve ser especificado.
#minimum_cruise_ratio: 0.5
#   A maioria dos movimentos acelera até velocidade de cruzeiro, viaja nessa
#   velocidade de cruzeiro e depois desacelera. Porém, alguns movimentos que
#   percorrem distância curta poderiam nominalmente acelerar e depois
#   desacelerar imediatamente. Esta opção reduz a velocidade máxima desses
#   movimentos para garantir que sempre exista distância mínima percorrida em
#   velocidade de cruzeiro. Isto é, impõe distância mínima percorrida em
#   velocidade de cruzeiro em relação à distância total percorrida. Destina-se
#   a reduzir a velocidade máxima de movimentos curtos em zigue-zague (e assim
#   reduzir vibração da impressora desses movimentos). Por exemplo,
#   minimum_cruise_ratio de 0.5 garantiria que movimento isolado de 1.5mm teria
#   distância mínima de cruzeiro de 0.75mm. Especifique relação de 0.0 para
#   desativar este recurso (não haveria distância mínima de cruzeiro imposta
#   entre aceleração e desaceleração). O valor especificado aqui pode ser
#   alterado em tempo de execução usando o
#   comando SET_VELOCITY_LIMIT. O padrão é 0.5.
#square_corner_velocity: 5.0
#   A velocidade máxima (em mm/s) que o cabeçote pode percorrer em canto de 90
#   graus. Valor diferente de zero pode reduzir mudanças nas taxas de fluxo da
#   extrusora ao permitir mudanças instantâneas de velocidade do cabeçote
#   durante cantos. Este valor configura o algoritmo interno de canto por
#   velocidade centrípeta; cantos com ângulos maiores que 90 graus terão
#   velocidade de canto maior enquanto cantos com ângulos menores que 90 graus
#   terão velocidade de canto menor. Se definido como zero, o cabeçote
#   desacelera até zero em cada canto. O valor especificado aqui pode ser
#   alterado em tempo de execução usando o comando SET_VELOCITY_LIMIT. O padrão
#   é 5mm/s.
```

<a id="stepper"></a>
### [stepper]

Definições de motor de passo. Diferentes tipos de impressora (conforme especificado pela opção "kinematics" na seção de configuração [printer]) exigem nomes diferentes para o motor de passo (por ex., `stepper_x` vs `stepper_a`). Abaixo estão definições comuns de motores de passo.

Veja o [documento de distância de rotação](Rotation_Distance.md) para informações sobre como calcular o parâmetro `rotation_distance`. Veja o documento [Referenciamento com múltiplos MCUs](Multi_MCU_Homing.md) para informações sobre referenciamento usando múltiplos microcontroladores.

```
[stepper_x]
step_pin:
#   Pino GPIO de passo (acionado em nível alto). Este parâmetro deve ser
#   informado.
dir_pin:
#   Pino GPIO de direção (nível alto indica direção positiva). Este parâmetro
#   deve ser informado.
enable_pin:
#   Pino de enable (o padrão é enable em nível alto; use ! para indicar enable
#   em nível baixo). Se este parâmetro não for informado, o driver do motor de
#   passo deve permanecer sempre habilitado.
rotation_distance:
#   Distância (em mm) que o eixo percorre com uma rotação completa do motor de
#   passo (ou da engrenagem final se gear_ratio for especificado). Este
#   parâmetro deve ser informado.
microsteps:
#   O número de micropassos que o driver do motor de passo usa. Este parâmetro
#   deve ser informado.
#full_steps_per_rotation: 200
#   O número de passos completos para uma rotação do motor de passo. Defina
#   como 200 para motor de passo de 1.8 grau ou defina como 400 para motor de
#   0.9 grau. O padrão é 200.
#gear_ratio:
#   A relação de engrenagem se o motor de passo estiver conectado ao eixo por
#   caixa de redução. Por exemplo, pode-se especificar "5:1" se redução de 5
#   para 1 estiver em uso. Se o eixo tiver várias caixas de redução, pode-se
#   especificar lista de relações separadas por vírgula (por exemplo, "57:11,
#   2:1"). Se gear_ratio for especificado, então rotation_distance especifica a
#   distância que o eixo percorre para uma rotação completa da engrenagem
#   final. O padrão é não usar relação de engrenagem.
#step_pulse_duration:
#   O tempo mínimo entre a borda do sinal de pulso de passo e a borda seguinte
#   do sinal de unstep. Também é usado para definir o tempo mínimo entre pulso
#   de passo e sinal de mudança de direção. O padrão é 0.000000100 (100ns) para
#   motores TMC configurados em modo UART ou SPI, e o padrão é 0.000002 (que é
#   2us) para todos os demais motores.
endstop_pin:
#   Pino de detecção da chave de fim de curso. Se este pino de fim de curso
#   estiver em mcu diferente do motor de passo, então habilita multi-mcu homing
#   (referenciamento). Este parâmetro deve ser informado para os motores X, Y e
#   Z em impressoras estilo cartesian.
#position_min: 0
#   Distância mínima válida (em mm) para onde o usuário pode comandar o
#   movimento do motor. O padrão é 0mm.
position_endstop:
#   Localização do fim de curso (em mm). Este parâmetro deve ser informado para
#   os motores X, Y e Z em impressoras estilo cartesian.
position_max:
#   Distância máxima válida (em mm) para onde o usuário pode comandar o
#   movimento do motor. Este parâmetro deve ser informado para os motores X, Y
#   e Z em impressoras estilo cartesian.
#homing_speed: 5.0
#   Velocidade máxima (em mm/s) do motor durante homing (referenciamento). O
#   padrão é 5mm/s.
#homing_retract_dist: 5.0
#   Distância de retirada (em mm) antes de executar homing (referenciamento)
#   pela segunda vez durante homing (referenciamento). Defina como zero para
#   desativar o segundo homing (referenciamento). O padrão é 5mm.
#homing_retract_speed:
#   Velocidade a usar no movimento de retirada após homing (referenciamento)
#   caso deva ser diferente da velocidade de homing (referenciamento), que é o
#   padrão para este parâmetro
#second_homing_speed:
#   Velocidade (em mm/s) do motor ao executar o segundo homing
#   (referenciamento). O padrão é homing_speed/2.
#homing_positive_dir:
#   Se true, homing (referenciamento) fará o motor mover em direção positiva
#   (para longe do zero); se false, referencia em direção ao zero. É melhor
#   usar o padrão do que especificar este parâmetro. O padrão é true se
#   position_endstop estiver próximo de position_max e false se estiver próximo
#   de position_min.
```

<a id="cartesian-kinematics"></a>
### Cinemática cartesiana

Veja [example-cartesian.cfg](../../config/example-cartesian.cfg) para um exemplo de arquivo de configuração de cinemática cartesiana.

Somente parâmetros específicos de impressoras cartesianas são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: cartesian
max_z_velocity:
#   Isto define a velocidade máxima (em mm/s) de movimento ao longo do eixo z.
#   Esta configuração pode ser usada para restringir a velocidade máxima do
#   motor de passo z. O padrão é usar max_velocity para max_z_velocity.
max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. Limita a aceleração do motor de passo z. O padrão é usar max_accel para
#   max_z_accel.

# A seção stepper_x é usada para descrever o motor que controla o eixo X em
# robô cartesian.
[stepper_x]

# A seção stepper_y é usada para descrever o motor que controla o eixo Y em
# robô cartesian.
[stepper_y]

# A seção stepper_z é usada para descrever o motor que controla o eixo Z em
# robô cartesian.
[stepper_z]
```

<a id="linear-delta-kinematics"></a>
### Cinemática Linear Delta

Veja [example-delta.cfg](../../config/example-delta.cfg) para um exemplo de arquivo de configuração de cinemática linear delta. Veja o [guia de calibração delta](Delta_Calibrate.md) para informações sobre calibração.

Somente parâmetros específicos de impressoras linear delta são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: delta
max_z_velocity:
#   Para impressoras delta isto limita a velocidade máxima (em mm/s) de
#   movimentos com deslocamento no eixo z. Esta configuração pode ser usada
#   para reduzir a velocidade máxima de movimentos para cima e para baixo (que
#   exigem taxa de passos maior que outros movimentos em impressora delta). O
#   padrão é usar max_velocity para max_z_velocity.
#max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. Definir isto pode ser útil se a impressora puder atingir aceleração
#   maior em movimentos XY que em movimentos Z (por exemplo, ao usar input
#   shaper). O padrão é usar max_accel para max_z_accel.
#minimum_z_position: 0
#   A posição Z mínima para onde o usuário pode comandar o deslocamento do
#   cabeçote. O padrão é 0.
delta_radius:
#   Raio (em mm) do círculo horizontal formado pelas três torres de eixo
#   linear. Este parâmetro também pode ser calculado como:
#    delta_radius = smooth_rod_offset - effector_offset - carriage_offset
#   Este parâmetro deve ser informado.
#print_radius:
#   O raio (em mm) de coordenadas XY válidas do cabeçote. Pode-se usar esta
#   configuração para personalizar a verificação de alcance dos movimentos do
#   cabeçote. Se valor grande for especificado aqui, pode ser possível comandar
#   o cabeçote para colisão com torre. O padrão é usar delta_radius para
#   print_radius (o que normalmente evitaria colisão com torre).

# A seção stepper_a descreve o motor que controla a torre dianteira esquerda (a
# 210 graus). Esta seção também controla os parâmetros de homing
# (referenciamento) (homing_speed, homing_retract_dist) para todas as torres.
[stepper_a]
position_endstop:
#   Distância (em mm) entre o bico e a mesa quando o bico está no centro da
#   área de construção e o fim de curso aciona. Este parâmetro deve ser
#   informado para stepper_a; para stepper_b e stepper_c este parâmetro usa por
#   padrão o valor especificado para stepper_a.
arm_length:
#   Comprimento (em mm) da haste diagonal que conecta esta torre ao cabeçote de
#   impressão. Este parâmetro deve ser informado para stepper_a; para stepper_b
#   e stepper_c este parâmetro usa por padrão o valor especificado para
#   stepper_a.
#angle:
#   Esta opção especifica o ângulo (em graus) em que a torre está. O padrão é
#   210 para stepper_a, 330 para stepper_b e 90 para stepper_c.

# A seção stepper_b descreve o motor que controla a torre dianteira direita (a
# 330 graus).
[stepper_b]

# A seção stepper_c descreve o motor que controla a torre traseira (a 90
# graus).
[stepper_c]

# A seção delta_calibrate habilita comando estendido g-code DELTA_CALIBRATE que
# pode calibrar as posições e ângulos de fim de curso das torres.
[delta_calibrate]
radius:
#   Raio (em mm) da área que pode ser sondada. Este é o raio das coordenadas do
#   bico a serem sondadas; se usar sonda automática com deslocamento XY,
#   escolha raio pequeno o suficiente para que a sonda sempre fique sobre a
#   mesa. Este parâmetro deve ser informado.
#speed: 50
#   A velocidade (em mm/s) de movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para onde o cabeçote deve ser comandado a mover logo antes
#   de iniciar operação de sondagem. O padrão é 5.
```

<a id="deltesian-kinematics"></a>
### Cinemática Deltesiana

Veja [example-deltesian.cfg](../../config/example-deltesian.cfg) para um exemplo de arquivo de configuração de cinemática deltesiana.

Somente parâmetros específicos de impressoras deltesianas são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: deltesian
max_z_velocity:
#   Para impressoras deltesian, isto limita a velocidade máxima (em mm/s) de
#   movimentos com deslocamento no eixo z. Esta configuração pode ser usada
#   para reduzir a velocidade máxima de movimentos para cima e para baixo (que
#   exigem taxa de passos maior que outros movimentos em impressora deltesian).
#   O padrão é usar max_velocity para max_z_velocity.
#max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. Definir isto pode ser útil se a impressora puder atingir aceleração
#   maior em movimentos XY que em movimentos Z (por exemplo, ao usar input
#   shaper). O padrão é usar max_accel para max_z_accel.
#minimum_z_position: 0
#   A posição Z mínima para onde o usuário pode comandar o deslocamento do
#   cabeçote. O padrão é 0.
#min_angle: 5
#   Isto representa o ângulo mínimo (em graus) em relação à horizontal que os
#   braços deltesian podem atingir. Este parâmetro destina-se a impedir que os
#   braços fiquem completamente horizontais, o que arriscaria inversão
#   acidental do eixo XZ. O padrão é 5.
#print_width:
#   A distância (em mm) de coordenadas X válidas do cabeçote. Pode-se usar esta
#   configuração para personalizar a verificação de alcance dos movimentos do
#   cabeçote. Se valor grande for especificado aqui, pode ser possível comandar
#   o cabeçote para colisão com torre. Esta configuração normalmente
#   corresponde à largura da mesa (em mm).
#slow_ratio: 3
#   A relação usada para limitar velocidade e aceleração em movimentos próximos
#   aos extremos do eixo X. Se distância vertical dividida por distância
#   horizontal exceder o valor de slow_ratio, então velocidade e aceleração são
#   limitadas à metade de seus valores nominais. Se distância vertical dividida
#   por distância horizontal exceder o dobro do valor de slow_ratio, então
#   velocidade e aceleração são limitadas a um quarto de seus valores nominais.
#   O padrão é 3.

# A seção stepper_left é usada para descrever o motor que controla a torre
# esquerda. Esta seção também controla os parâmetros de homing
# (referenciamento) (homing_speed, homing_retract_dist) para todas as torres.
[stepper_left]
position_endstop:
#   Distância (em mm) entre o bico e a mesa quando o bico está no centro da
#   área de construção e os fins de curso são acionados. Este parâmetro deve
#   ser informado para stepper_left; para stepper_right este parâmetro usa por
#   padrão o valor especificado para stepper_left.
arm_length:
#   Comprimento (em mm) da haste diagonal que conecta o carro da torre ao
#   cabeçote de impressão. Este parâmetro deve ser informado para stepper_left;
#   para stepper_right, este parâmetro usa por padrão o valor especificado para
#   stepper_left.
arm_x_length:
#   Distância horizontal entre o cabeçote de impressão e a torre quando a
#   impressora está com homing (referenciamento) feito. Este parâmetro deve ser
#   informado para stepper_left; para stepper_right, este parâmetro usa por
#   padrão o valor especificado para stepper_left.

# A seção stepper_right é usada para descrever o motor que controla a torre
# direita.
[stepper_right]

# A seção stepper_y é usada para descrever o motor que controla o eixo Y em
# robô deltesian.
[stepper_y]
```

<a id="corexy-kinematics"></a>
### Cinemática CoreXY

Veja [example-corexy.cfg](../../config/example-corexy.cfg) para um exemplo de arquivo de cinemática corexy (e h-bot).

Somente parâmetros específicos de impressoras corexy são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: corexy
max_z_velocity:
#   Isto define a velocidade máxima (em mm/s) de movimento ao longo do eixo z.
#   Esta configuração pode ser usada para restringir a velocidade máxima do
#   motor de passo z. O padrão é usar max_velocity para max_z_velocity.
max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. Limita a aceleração do motor de passo z. O padrão é usar max_accel para
#   max_z_accel.

# A seção stepper_x é usada para descrever o eixo X bem como o motor que
# controla o movimento X+Y.
[stepper_x]

# A seção stepper_y é usada para descrever o eixo Y bem como o motor que
# controla o movimento X-Y.
[stepper_y]

# A seção stepper_z é usada para descrever o motor que controla o eixo Z.
[stepper_z]
```

<a id="corexz-kinematics"></a>
### Cinemática CoreXZ

Veja [example-corexz.cfg](../../config/example-corexz.cfg) para um exemplo de arquivo de configuração de cinemática corexz.

Somente parâmetros específicos de impressoras corexz são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: corexz
max_z_velocity:
#   Isto define a velocidade máxima (em mm/s) de movimento ao longo do eixo z.
#   O padrão é usar max_velocity para max_z_velocity.
max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. O padrão é usar max_accel para max_z_accel.

# A seção stepper_x é usada para descrever o eixo X bem como o motor que
# controla o movimento X+Z.
[stepper_x]

# A seção stepper_y é usada para descrever o motor que controla o eixo Y.
[stepper_y]

# A seção stepper_z é usada para descrever o eixo Z bem como o motor que
# controla o movimento X-Z.
[stepper_z]
```

<a id="hybrid-corexy-kinematics"></a>
### Cinemática Hybrid-CoreXY

Veja [example-hybrid-corexy.cfg](../../config/example-hybrid-corexy.cfg) para um exemplo de arquivo de configuração de cinemática hybrid corexy.

Esta cinemática também é conhecida como cinemática Markforged.

Somente parâmetros específicos de impressoras hybrid corexy são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: hybrid_corexy
max_z_velocity:
#   Isto define a velocidade máxima (em mm/s) de movimento ao longo do eixo z.
#   O padrão é usar max_velocity para max_z_velocity.
max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. O padrão é usar max_accel para max_z_accel.

# A seção stepper_x é usada para descrever o eixo X bem como o motor que
# controla o movimento X-Y.
[stepper_x]

# A seção stepper_y é usada para descrever o motor que controla o eixo Y.
[stepper_y]

# A seção stepper_z é usada para descrever o motor que controla o eixo Z.
[stepper_z]
```

<a id="hybrid-corexz-kinematics"></a>
### Cinemática Hybrid-CoreXZ

Veja [example-hybrid-corexz.cfg](../../config/example-hybrid-corexz.cfg) para um exemplo de arquivo de configuração de cinemática hybrid corexz.

Esta cinemática também é conhecida como cinemática Markforged.

Somente parâmetros específicos de impressoras hybrid corexy são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

```
[printer]
kinematics: hybrid_corexz
max_z_velocity:
#   Isto define a velocidade máxima (em mm/s) de movimento ao longo do eixo z.
#   O padrão é usar max_velocity para max_z_velocity.
max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. O padrão é usar max_accel para max_z_accel.

# A seção stepper_x é usada para descrever o eixo X bem como o motor que
# controla o movimento X-Z.
[stepper_x]

# A seção stepper_y é usada para descrever o motor que controla o eixo Y.
[stepper_y]

# A seção stepper_z é usada para descrever o motor que controla o eixo Z.
[stepper_z]
```

<a id="polar-kinematics"></a>
### Cinemática Polar

Veja [example-polar.cfg](../../config/example-polar.cfg) para um exemplo de arquivo de configuração de cinemática polar.

Somente parâmetros específicos de impressoras polares são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

CINEMÁTICA POLAR É UM TRABALHO EM ANDAMENTO. Movimentos em torno da posição 0, 0 sabidamente não funcionam corretamente.

```
[printer]
kinematics: polar
max_z_velocity:
#   Isto define a velocidade máxima (em mm/s) de movimento ao longo do eixo z.
#   Esta configuração pode ser usada para restringir a velocidade máxima do
#   motor de passo z. O padrão é usar max_velocity para max_z_velocity.
max_z_accel:
#   Isto define a aceleração máxima (em mm/s^2) de movimento ao longo do eixo
#   z. Limita a aceleração do motor de passo z. O padrão é usar max_accel para
#   max_z_accel.
# max_angular_velocity: 0
#   Isto limita a velocidade angular máxima (em rad/s) de um movimento. Valores
#   menores resultarão em tempos de impressão maiores, mas evitam movimentos
#   rápidos demais próximos ao centro. Valor 0 desativa o escalonamento. O
#   padrão é não aplicar limites de velocidade angular máxima.

# A seção stepper_bed é usada para descrever o motor que controla a mesa.
[stepper_bed]
gear_ratio:
#   gear_ratio deve ser especificado e rotation_distance não pode ser
#   especificado. Por exemplo, se a mesa tiver polia de 80 dentes acionada por
#   motor com polia de 16 dentes, especifica-se relação de engrenagem de
#   "80:16". Este parâmetro deve ser informado.

# A seção stepper_arm é usada para descrever o motor que controla o carro no
# braço.
[stepper_arm]

# A seção stepper_z é usada para descrever o motor que controla o eixo Z.
[stepper_z]
```

<a id="rotary-delta-kinematics"></a>
### Cinemática Rotary Delta

Veja [example-rotary-delta.cfg](../../config/example-rotary-delta.cfg) para um exemplo de arquivo de configuração de cinemática rotary delta.

Somente parâmetros específicos de impressoras rotary delta são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

CINEMÁTICA ROTARY DELTA É UM TRABALHO EM ANDAMENTO. Movimentos de referenciamento podem exceder o tempo limite e algumas verificações de limites não estão implementadas.

```
[printer]
kinematics: rotary_delta
max_z_velocity:
#   Para impressoras delta isto limita a velocidade máxima (em mm/s) de
#   movimentos com deslocamento no eixo z. Esta configuração pode ser usada
#   para reduzir a velocidade máxima de movimentos para cima e para baixo (que
#   exigem taxa de passos maior que outros movimentos em impressora delta). O
#   padrão é usar max_velocity para max_z_velocity.
#minimum_z_position: 0
#   A posição Z mínima para onde o usuário pode comandar o deslocamento do
#   cabeçote. O padrão é 0.
shoulder_radius:
#   Raio (em mm) do círculo horizontal formado pelas três juntas de ombro,
#   menos o raio do círculo formado pelas juntas do efetuador. Este parâmetro
#   também pode ser calculado como:
#     shoulder_radius = (delta_f - delta_e) / sqrt(12)
#   Este parâmetro deve ser informado.
shoulder_height:
#   Distância (em mm) das juntas de ombro até a mesa, menos a altura do
#   cabeçote efetuador. Este parâmetro deve ser informado.

# A seção stepper_a descreve o motor que controla o braço traseiro direito (a
# 30 graus). Esta seção também controla os parâmetros de homing
# (referenciamento) (homing_speed, homing_retract_dist) para todos os braços.
[stepper_a]
gear_ratio:
#   gear_ratio deve ser especificado e rotation_distance não pode ser
#   especificado. Por exemplo, se o braço tiver polia de 80 dentes acionada por
#   polia com 16 dentes, que por sua vez está conectada a polia de 60 dentes
#   acionada por motor com polia de 16 dentes, especifica-se relação de
#   engrenagem de "80:16, 60:16". Este parâmetro deve ser informado.
position_endstop:
#   Distância (em mm) entre o bico e a mesa quando o bico está no centro da
#   área de construção e o fim de curso aciona. Este parâmetro deve ser
#   informado para stepper_a; para stepper_b e stepper_c este parâmetro usa por
#   padrão o valor especificado para stepper_a.
upper_arm_length:
#   Comprimento (em mm) do braço que conecta a junta de ombro à junta de
#   cotovelo. Este parâmetro deve ser informado para stepper_a; para stepper_b
#   e stepper_c este parâmetro usa por padrão o valor especificado para
#   stepper_a.
lower_arm_length:
#   Comprimento (em mm) do braço que conecta a junta de cotovelo à junta do
#   efetuador. Este parâmetro deve ser informado para stepper_a; para stepper_b
#   e stepper_c este parâmetro usa por padrão o valor especificado para
#   stepper_a.
#angle:
#   Esta opção especifica o ângulo (em graus) em que o braço está. O padrão é
#   30 para stepper_a, 150 para stepper_b e 270 para stepper_c.

# A seção stepper_b descreve o motor que controla o braço traseiro esquerdo (a
# 150 graus).
[stepper_b]

# A seção stepper_c descreve o motor que controla o braço dianteiro (a 270
# graus).
[stepper_c]

# A seção delta_calibrate habilita comando estendido g-code DELTA_CALIBRATE que
# pode calibrar as posições de fim de curso dos ombros.
[delta_calibrate]
radius:
#   Raio (em mm) da área que pode ser sondada. Este é o raio das coordenadas do
#   bico a serem sondadas; se usar sonda automática com deslocamento XY,
#   escolha raio pequeno o suficiente para que a sonda sempre fique sobre a
#   mesa. Este parâmetro deve ser informado.
#speed: 50
#   A velocidade (em mm/s) de movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para onde o cabeçote deve ser comandado a mover logo antes
#   de iniciar operação de sondagem. O padrão é 5.
```

<a id="cable-winch-kinematics"></a>
### Cinemática via guincho de cabo

Veja o [example-winch.cfg](../../config/example-winch.cfg) para um exemplo de arquivo de configuração de cinemática de guincho por cabos.

Somente parâmetros específicos de impressoras de guincho por cabos são descritos aqui - veja [configurações cinemáticas comuns](#common-kinematic-settings) para os parâmetros disponíveis.

SUPORTE A GUINCHO DE CABOS É EXPERIMENTAL. Homing (referenciamento) não está implementado na cinemática via guincho de cabo. Para referenciar a impressora, envie manualmente comandos de movimento até que o cabeçote esteja em 0, 0, 0 e então envie o comando `G28`.

```
[printer]
kinematics: winch

# A seção stepper_a descreve o motor conectado ao primeiro guincho de cabo.
# Podem ser definidos mínimo de 3 e máximo de 26 guinchos de cabo (stepper_a a
# stepper_z), embora seja comum definir 4.
[stepper_a]
rotation_distance:
#   rotation_distance é a distância nominal (em mm) que o cabeçote se desloca
#   em direção ao guincho de cabo para cada rotação completa do motor de passo.
#   Este parâmetro deve ser informado.
anchor_x:
anchor_y:
anchor_z:
#   As posições X, Y e Z do guincho de cabo no espaço cartesiano. Estes
#   parâmetros devem ser informados.
```

<a id="generic-cartesian-kinematics"></a>
### Cinemática Cartesiana Genérica

Veja [example-generic-cartesian.cfg](../../config/example-generic-caretesian.cfg) para um exemplo de arquivo de configuração de cinemática cartesiana genérica.

Esta classe de cinemática de impressora permite ao usuário definir de maneira bastante flexível uma cinemática arbitrária estilo cartesiana. Em princípio, as cinemáticas cartesiana regular, corexy e hybrid_corexy também podem ser definidas dessa forma. Porém, mais importante, várias cinemáticas de outra forma não suportadas, como hybrid_corexy invertida ou corexyuv, podem ser definidas usando esta cinemática.

Notavelmente, a definição de uma cinemática cartesiana genérica desvia-se significativamente dos outros tipos de cinemática. Ela segue a seguinte convenção: o usuário define um conjunto de carros com certa faixa de movimento que podem se mover independentemente uns dos outros (eles devem se mover sobre os eixos cartesianos X, Y e Z, daí o nome da cinemática) e fins de curso correspondentes que permitem ao firmware determinar a posição dos carros durante o referenciamento, bem como um conjunto de motores de passo que movem esses carros. A seção `[printer]` deve especificar a cinemática e outras configurações de nível de impressora da mesma forma que a cinemática cartesiana regular:

```
[printer]
kinematics: generic_cartesian
max_velocity:
max_accel:
#minimum_cruise_ratio:
#square_corner_velocity:
#max_z_velocity:
#max_z_accel:

```

Em seguida, o usuário deve definir três carros primários para os eixos X, Y e Z, por ex.:

```
[carriage carriage_x]
axis:
#   Eixo de um carro, seja x, y ou z. Este parâmetro deve ser informado, a
#   menos que o próprio nome do carro seja x, y ou z.
endstop_pin:
#   Pino de detecção da chave de fim de curso. Se este pino de fim de curso
#   estiver em mcu diferente do motor de passo que move este carro, então
#   habilita multi-mcu homing (referenciamento). Este parâmetro deve ser
#   informado.
#position_min: 0
#   Distância mínima válida (em mm) para onde o usuário pode comandar o
#   deslocamento do carro. O padrão é 0mm.
position_endstop:
#   Localização do fim de curso (em mm). Este parâmetro deve ser informado.
position_max:
#   Distância máxima válida (em mm) para onde o usuário pode comandar o
#   deslocamento do motor. Este parâmetro deve ser informado.
#homing_speed: 5.0
#   Velocidade máxima (em mm/s) do carro durante homing (referenciamento). O
#   padrão é 5mm/s.
#homing_retract_dist: 5.0
#   Distância de afastamento (em mm) antes de executar o
#   homing(referenciamento) pela segunda vez durante o homing(referenciamento).
#   Defina como zero para desativar o segundo referenciamento. O padrão é 5mm.
#homing_retract_speed:
#   Velocidade a ser usada no movimento de retirada após o
#   homing(referenciamento) caso deva ser diferente da velocidade de
#   homing(referenciamento), que é o padrão para este parâmetro
#second_homing_speed:
#   Velocidade (em mm/s) do carro ao executar o segundo referenciamento. O
#   padrão é homing_speed/2.
#homing_positive_dir:
#   Se true, o homing(referenciamento) fará o carro mover-se em direção
#   positiva (para longe do zero); se false, referenciar em direção ao zero. É
#   melhor usar o padrão do que especificar este parâmetro. O padrão é true se
#   position_endstop estiver próximo de position_max e false se estiver próximo
#   de position_min.
```

Depois, o usuário especifica os motores de passo que movem esses carros, por exemplo

```
[stepper my_stepper]
carriages:
#   Uma string que descreve os carros que o motor de passo move. Todos os
#   carros definidos podem ser especificados aqui, bem como suas combinações
#   lineares, e.g. carriage_x, carriage_x+carriage_y,
#   carriage_y-0.5*carriage_z, carriage_x-carriage_z, etc. Este parâmetro deve
#   ser fornecido.
step_pin:
dir_pin:
enable_pin:
rotation_distance:
microsteps:
#full_steps_per_rotation: 200
#gear_ratio:
#step_pulse_duration:
```
Veja a seção [stepper](#stepper) para mais informações sobre os parâmetros regulares de motores de passo. O parâmetro `carriages` define como o motor afeta o movimento dos carros. Por exemplo, `carriage_x+carriage_y` indica que o movimento do motor na direção positiva pela distância `d` move os carros `carriage_x` e `carriage_y` pela mesma distância `d` na direção positiva, enquanto `carriage_x-0.5*carriage_y` significa que o movimento do motor na direção positiva pela distância `d` move o carro `carriage_x` pela distância `d` na direção positiva, mas o carro `carriage_y` percorrerá a distância `d/2` na direção negativa.

Mais de um motor de passo pode ser definido para acionar o mesmo eixo ou correia. Por exemplo, em configurações CoreXY AWD, dois motores acionando a mesma correia podem ser definidos como

```
[carriage carriage_x]
endstop_pin: ...
...

[carriage carriage_y]
endstop_pin: ...
...

[stepper a0]
carriages: carriage_x-carriage_y
step_pin: ...
dir_pin: ...
enable_pin: ...
rotation_distance: ...
...

[stepper a1]
carriages: carriage_x-carriage_y
step_pin: ...
dir_pin: ...
enable_pin: ...
rotation_distance: ...
...
```
com os motores `a0` e `a1` tendo seus próprios pinos de controle, mas compartilhando os mesmos `carriages` e fins de curso correspondentes.

Há situações em que o usuário deseja ter mais de um fim de curso por eixo. Exemplos de tais configurações incluem eixo Y acionado por dois motores de passo independentes com correias fixadas em ambas as extremidades do pórtico X, com efetivamente dois carros no eixo Y cada um tendo um fim de curso independente, e eixo Z com múltiplos motores com cada motor tendo seu próprio fim de curso (não confundir com configurações com múltiplos motores Z mas apenas um único fim de curso). Essas configurações podem ser declaradas especificando carro(s) adicional(is) com seus fins de curso:

```
[extra_carriage my_carriage]
primary_carriage:
#   O nome do carro primário ao qual este carro corresponde. Ele também define
#   efetivamente o eixo sobre o qual o carro se move. Este parâmetro deve ser
#   fornecido.
endstop_pin:
#   Pino de detecção da chave de fim de curso. Este parâmetro deve ser
#   fornecido.
```

e os motores de passo correspondentes, por exemplo:

```
[extra_carriage carriage_y1]
primary_carriage: carriage_y
endstop_pin: ...

[stepper sy1]
carriages: carriage_y1
...
```
Notavelmente, um `[extra_carriage]` não define parâmetros como `position_min`, `position_max` e `position_endstop`, mas em vez disso os herda do `primary_carriage` especificado, compartilhando assim a mesma faixa de movimento com o carro primário.

Para referências sobre como configurar setups IDEX, veja a seção [carro duplo](#dual_carriage).

<a id="none-kinematics"></a>
### Cinemática Nenhuma

É possível definir uma cinemática especial "none" para desativar o suporte cinemático no Klipper. Isso pode ser útil para controlar dispositivos que não são impressoras 3d típicas ou para fins de depuração.

```
[printer]
kinematics: none
max_velocity: 1
max_accel: 1
#   Os parâmetros max_velocity e max_accel devem ser definidos. Os valores não
#   são usados para cinemática "none".
```

<a id="common-extruder-and-heated-bed-support"></a>
## Suporte comum para extrusora e mesa aquecida.

<a id="extruder"></a>
### [extruder]

A seção de extrusora é usada para descrever os parâmetros do aquecedor para o bico quente (hotend) junto com o motor de passo que controla a extrusora. Veja a [referência de comandos](G-Codes.md#extruder) para informações adicionais. Veja o [guia de avanço de pressão](Pressure_Advance.md) para informações sobre ajuste do avanço de pressão.

```
[extruder]
step_pin:
dir_pin:
enable_pin:
microsteps:
rotation_distance:
#full_steps_per_rotation:
#gear_ratio:
#   Consulte a seção "stepper" para uma descrição dos parâmetros acima. Se
#   nenhum dos parâmetros acima for especificado, então nenhum motor de passo
#   será associado ao hotend do bico (embora um comando SYNC_EXTRUDER_MOTION
#   possa associar um em tempo de execução).
nozzle_diameter:
#   Diâmetro do orifício do bico (em mm). Este parâmetro deve ser fornecido.
filament_diameter:
#   O diâmetro nominal do filamento bruto (em mm) ao entrar na extrusora. Este
#   parâmetro deve ser fornecido.
#max_extrude_cross_section:
#   Área máxima (em mm^2) da seção transversal de extrusão (eg, largura de
#   extrusão multiplicada pela altura da camada). Esta configuração evita
#   quantidades excessivas de extrusão durante movimentos XY relativamente
#   pequenos. Se um movimento solicitar uma taxa de extrusão que exceda este
#   valor, um erro será retornado. O padrão é: 4.0 * nozzle_diameter^2
#instantaneous_corner_velocity: 1.000
#   A variação máxima instantânea de velocidade (em mm/s) da extrusora durante
#   a junção de dois movimentos. O padrão é 1mm/s.
#max_extrude_only_distance: 50.0
#   Comprimento máximo (em mm de filamento bruto) que um movimento de retirada
#   ou somente extrusão pode ter. Se um movimento de retirada ou somente
#   extrusão solicitar uma distância maior que este valor, um erro será
#   retornado. O padrão é 50mm.
#max_extrude_only_velocity:
#max_extrude_only_accel:
#   Velocidade máxima (em mm/s) e aceleração (em mm/s^2) do motor da extrusora
#   para retiradas e movimentos somente extrusão. Estas configurações não têm
#   nenhum impacto sobre movimentos normais de impressão. Se não especificadas,
#   elas são calculadas para corresponder ao limite que um movimento de
#   impressão XY com seção transversal de 4.0*nozzle_diameter^2 teria.
#pressure_advance: 0.0
#   A quantidade de filamento bruto a empurrar para dentro da extrusora durante
#   a aceleração da extrusora. Uma quantidade igual de filamento sofre retirada
#   durante a desaceleração. É medida em milímetros por milímetro/segundo. O
#   padrão é 0, que desativa pressure advance.
#pressure_advance_smooth_time: 0.040
#   Um intervalo de tempo (em segundos) a ser usado ao calcular a velocidade
#   média da extrusora para pressure advance. Um valor maior resulta em
#   movimentos mais suaves da extrusora. Este parâmetro não pode exceder 200ms.
#   Esta configuração aplica-se somente se pressure_advance for diferente de
#   zero. O padrão é 0.040 (40 milissegundos).
#
# As variáveis restantes descrevem o aquecedor da extrusora.
heater_pin:
#   Pino de saída PWM que controla o aquecedor. Este parâmetro deve ser
#   fornecido.
#max_power: 1.0
#   A potência máxima (expressa como um valor de 0.0 a 1.0) para a qual
#   heater_pin pode ser ajustado. O valor 1.0 permite que o pino seja mantido
#   totalmente ativado por períodos prolongados, enquanto um valor de 0.5
#   permitiria que o pino ficasse ativado por no máximo metade do tempo. Esta
#   configuração pode ser usada para limitar a potência total de saída (por
#   períodos prolongados) para o aquecedor. O padrão é 1.0.
sensor_type:
#   Tipo de sensor - termistores comuns são "EPCOS 100K B57560G104F", "ATC
#   Semitec 104GT-2", "ATC Semitec 104NT-4-R025H42G", "Generic 3950","Honeywell
#   100K 135-104LAG-J01", "NTC 100K MGB18-104F39050L32", "SliceEngineering 450"
#   e "TDK NTCG104LH104JT1". Consulte a seção "Sensores de temperatura" para
#   outros sensores. Este parâmetro deve ser fornecido.
sensor_pin:
#   Pino de entrada analógica conectado ao sensor. Este parâmetro deve ser
#   fornecido.
#pullup_resistor: 4700
#   A resistência (em ohms) do pullup ligado ao termistor. Este parâmetro é
#   válido somente quando o sensor é um termistor. O padrão é 4700 ohms.
#smooth_time: 1.0
#   Um valor de tempo (em segundos) ao longo do qual as medições de temperatura
#   serão suavizadas para reduzir o impacto do ruído de medição. O padrão é 1
#   segundos.
control:
#   Algoritmo de controle (pid ou watermark). Este parâmetro deve ser
#   fornecido.
pid_Kp:
pid_Ki:
pid_Kd:
#   As configurações proporcional (pid_Kp), integral (pid_Ki) e derivativa
#   (pid_Kd) para o sistema de controle por realimentação PID. Klipper avalia
#   as configurações PID com a seguinte fórmula geral:
#     heater_pwm = (Kp*error + Ki*integral(error) - Kd*derivative(error)) / 255
#   Onde "error" é "requested_temperature - measured_temperature" e
#   "heater_pwm" é a taxa de aquecimento solicitada, com 0.0 totalmente
#   desligado e 1.0 totalmente ligado. Considere usar o comando PID_CALIBRATE
#   para obter estes parâmetros. Os parâmetros pid_Kp, pid_Ki e pid_Kd devem
#   ser fornecidos para aquecedores PID.
#max_delta: 2.0
#   Em aquecedores controlados por 'watermark', este é o número de graus em
#   Celsius acima da temperatura alvo antes de desativar o aquecedor, bem como
#   o número de graus abaixo do alvo antes de reativar o aquecedor. O padrão é
#   2 graus Celsius.
#pwm_cycle_time: 0.100
#   Tempo em segundos para cada ciclo PWM por software do aquecedor. Não é
#   recomendado definir isto a menos que haja um requisito elétrico para
#   comutar o aquecedor mais rápido que 10 vezes por segundo. O padrão é 0.100
#   segundos.
#min_extrude_temp: 170
#   A temperatura mínima (em Celsius) na qual comandos de movimento da
#   extrusora podem ser emitidos. O padrão é 170 Celsius.
min_temp:
max_temp:
#   A faixa máxima de temperaturas válidas (em Celsius) dentro da qual o
#   aquecedor deve permanecer. Isto controla um recurso de segurança
#   implementado no código do microcontrolador - caso a temperatura medida
#   fique fora desta faixa, o microcontrolador entrará em estado de
#   desligamento. Esta verificação pode ajudar a detectar algumas falhas de
#   hardware do aquecedor e do sensor. Defina esta faixa apenas larga o
#   suficiente para que temperaturas razoáveis não resultem em erro. Estes
#   parâmetros devem ser fornecidos.
```

<a id="heater_bed"></a>
### [heater_bed]

A seção heater_bed descreve uma mesa aquecida. Ela usa as mesmas configurações de aquecedor descritas na seção "extruder".

```
[heater_bed]
heater_pin:
sensor_type:
sensor_pin:
control:
min_temp:
max_temp:
#   Consulte a seção "extruder" para uma descrição dos parâmetros acima.
```

<a id="bed-level-support"></a>
## Suporte ao nível da mesa

<a id="bed_mesh"></a>
### [bed_mesh]

Nivelamento de Mesa por Malha. Pode-se definir uma seção de configuração bed_mesh para habilitar transformações de movimento que deslocam o eixo z com base em uma malha gerada a partir de pontos sondados. Ao usar uma sonda para referenciar o eixo z, é recomendado definir uma seção safe_z_home no printer.cfg para referenciar em direção ao centro da área de impressão.

Veja o [guia de malha da mesa](Bed_Mesh.md) e a [referência de comandos](G-Codes.md#bed_mesh) para informações adicionais.

Exemplos Visuais:

```
 rectangular bed, probe_count = 3, 3:
             x---x---x (max_point)
             |
             x---x---x
                     |
 (min_point) x---x---x

 round bed, round_probe_count = 5, bed_radius = r:
                 x (0, r) end
               /
             x---x---x
                       \
 (-r, 0) x---x---x---x---x (r, 0)
           \
             x---x---x
                   /
                 x  (0, -r) start
```

```
[bed_mesh]
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover-se
#   imediatamente antes de iniciar uma operação de sondagem. O padrão é 5.
#mesh_radius:
#   Define o raio da malha a ser sondada para mesas redondas. Observe que o
#   raio é relativo à coordenada especificada pela opção mesh_origin. Este
#   parâmetro deve ser fornecido para mesas redondas e omitido para mesas
#   retangulares.
#mesh_origin:
#   Define a coordenada central X, Y da malha para mesas redondas. Esta
#   coordenada é relativa à localização da sonda. Pode ser útil ajustar
#   mesh_origin em um esforço para maximizar o tamanho do raio da malha. O
#   padrão é 0, 0. Este parâmetro deve ser omitido para mesas retangulares.
#mesh_min:
#   Define a coordenada mínima X, Y da malha para mesas retangulares. Esta
#   coordenada é relativa à localização da sonda. Este será o primeiro ponto
#   sondado, mais próximo da origem. Este parâmetro deve ser fornecido para
#   mesas retangulares.
#mesh_max:
#   Define a coordenada máxima X, Y da malha para mesas retangulares. Segue o
#   mesmo princípio de mesh_min, porém este será o ponto sondado mais distante
#   da origem da mesa. Este parâmetro deve ser fornecido para mesas
#   retangulares.
#probe_count: 3, 3
#   Para mesas retangulares, este é um par de valores inteiros X, Y separados
#   por vírgula que define o número de pontos a sondar ao longo de cada eixo.
#   Um único valor também é válido, caso em que esse valor será aplicado a
#   ambos os eixos. O padrão é 3, 3.
#round_probe_count: 5
#   Para mesas redondas, este valor inteiro define o número máximo de pontos a
#   sondar ao longo de cada eixo. Este valor deve ser um número ímpar. O padrão
#   é 5.
#fade_start: 1.0
#   A posição Z em gcode na qual começar a eliminar gradualmente o ajuste Z
#   quando fade estiver ativado. O padrão é 1.0.
#fade_end: 0.0
#   A posição Z em gcode na qual a eliminação gradual se conclui. Quando
#   definido para um valor abaixo de fade_start, fade é desativado. Deve-se
#   observar que fade pode adicionar escala indesejada ao longo do eixo Z de
#   uma impressão. Se um usuário desejar ativar fade, um valor de 10.0 é
#   recomendado. O padrão é 0.0, que desativa fade.
#fade_target:
#   A posição Z na qual fade deve convergir. Quando este valor é definido para
#   um valor diferente de zero, ele deve estar dentro da faixa de valores Z na
#   malha. Usuários que desejarem convergir para a posição de
#   homing(referenciamento) Z devem definir isto como 0. O padrão é o valor Z
#   médio da malha.
#split_delta_z: .025
#   A quantidade de diferença Z (em mm) ao longo de um movimento que acionará
#   uma divisão. O padrão é .025.
#move_check_distance: 5.0
#   A distância (em mm) ao longo de um movimento para verificar split_delta_z.
#   Este também é o comprimento mínimo pelo qual um movimento pode ser
#   dividido. O padrão é 5.0.
#mesh_pps: 2, 2
#   Um par de inteiros X, Y separados por vírgula que define o número de pontos
#   por segmento a interpolar na malha ao longo de cada eixo. Um "segmento"
#   pode ser definido como o espaço entre cada ponto sondado. O usuário pode
#   inserir um único valor que será aplicado a ambos os eixos. O padrão é 2, 2.
#algorithm: lagrange
#   O algoritmo de interpolação a ser usado. Pode ser "lagrange" ou "bicubic".
#   Esta opção não afetará grades 3x3, que são forçadas a usar amostragem
#   lagrange. O padrão é lagrange.
#bicubic_tension: .2
#   Ao usar o algoritmo bicubic, o parâmetro tension acima pode ser aplicado
#   para alterar a quantidade de inclinação interpolada. Números maiores
#   aumentarão a quantidade de inclinação, o que resulta em mais curvatura na
#   malha. O padrão é .2.
#zero_reference_position:
#   Uma coordenada X,Y opcional que especifica o local na mesa onde Z = 0.
#   Quando esta opção é especificada, a malha será deslocada de modo que ajuste
#   Z zero ocorra neste local. O padrão é sem referência zero.
#faulty_region_1_min:
#faulty_region_1_max:
#   Pontos opcionais que definem uma região defeituosa. Consulte
#   docs/Bed_Mesh.md para detalhes sobre regiões defeituosas. Até 99 regiões
#   defeituosas podem ser adicionadas. Por padrão, nenhuma região defeituosa é
#   definida.
#adaptive_margin:
#   Uma margem opcional (em mm) a ser adicionada ao redor da área da mesa usada
#   pelos objetos de impressão definidos ao gerar uma malha adaptativa.
#scan_overshoot:
#  A quantidade máxima de deslocamento (em mm) disponível fora da malha. Para
#  mesas retangulares, isto se aplica ao deslocamento no eixo X, e para mesas
#  redondas aplica-se a todo o raio. A ferramenta deve ser capaz de deslocar a
#  quantidade especificada para fora da malha. Este valor é usado para otimizar
#  o caminho de deslocamento ao executar um "rapid scan". O valor mínimo que
#  pode ser especificado é 1. O padrão é sem overshoot.
```

<a id="bed_tilt"></a>
### [bed_tilt]

Compensação por inclinação da mesa. Pode-se definir uma seção de configuração bed_tilt que habilite transformações de movimento que considerem uma mesa inclinada. Note que bed_mesh e bed_tilt são incompatíveis; ambos não podem ser definidos ao mesmo tempo.

Veja a [referência de comandos](G-Codes.md#bed_tilt) para informações adicionais.

```
[bed_tilt]
#x_adjust: 0
#   A quantidade a adicionar à altura Z de cada movimento para cada mm no eixo
#   X. O padrão é 0.
#y_adjust: 0
#   A quantidade a adicionar à altura Z de cada movimento para cada mm no eixo
#   Y. O padrão é 0.
#z_adjust: 0
#   A quantidade a adicionar à altura Z quando o bico está nominalmente em 0,
#   0. O padrão é 0. Os parâmetros restantes controlam um comando g-code
#   estendido BED_TILT_CALIBRATE que pode ser usado para calibrar parâmetros de
#   ajuste x e y apropriados.
#points:
#   Uma lista de coordenadas X, Y (uma por linha; linhas subsequentes
#   indentadas) que devem ser sondadas durante um comando BED_TILT_CALIBRATE.
#   Especifique coordenadas do bico e certifique-se de que a sonda esteja acima
#   da mesa nas coordenadas de bico fornecidas. O padrão é não ativar o
#   comando.
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover-se
#   imediatamente antes de iniciar uma operação de sondagem. O padrão é 5.
```

<a id="bed_screws"></a>
### [bed_screws]

Ferramenta para ajudar a ajustar os parafusos de nivelamento da mesa. Pode-se definir uma seção de configuração [bed_screws] para habilitar um comando g-code BED_SCREWS_ADJUST.

Veja o [guia de nivelamento](Manual_Level.md#adjusting-bed-leveling-screws) e a [referência de comandos](G-Codes.md#bed_screws) para informações adicionais.

```
[bed_screws]
#screw1:
#   A coordenada X, Y do primeiro parafuso de nivelamento da mesa. Esta é uma
#   posição para comandar o bico que fica diretamente acima do parafuso da mesa
#   (ou o mais próximo possível ainda acima da mesa). Este parâmetro deve ser
#   fornecido.
#screw1_name:
#   Um nome arbitrário para o parafuso fornecido. Este nome é exibido quando o
#   script auxiliar é executado. O padrão é usar um nome baseado na localização
#   XY do parafuso.
#screw1_fine_adjust:
#   Uma coordenada X, Y para comandar o bico de modo que se possa ajustar
#   finamente o parafuso de nivelamento da mesa. O padrão é não realizar
#   ajustes finos no parafuso da mesa.
#screw2:
#screw2_name:
#screw2_fine_adjust:
#...
#   Parafusos adicionais de nivelamento da mesa. Pelo menos três parafusos
#   devem ser definidos.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover-se ao
#   passar de uma localização de parafuso para a próxima. O padrão é 5.
#probe_height: 0
#   A altura da sonda (em mm) após ajustar para a expansão térmica da mesa e do
#   bico. O padrão é zero.
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#probe_speed: 5
#   A velocidade (em mm/s) ao mover-se de uma posição horizontal_move_z para
#   uma posição probe_height. O padrão é 5.
```

<a id="screws_tilt_adjust"></a>
### [screws_tilt_adjust]

Ferramenta para ajudar a ajustar a inclinação dos parafusos da mesa usando sonda Z. Pode-se definir uma seção de configuração screws_tilt_adjust para habilitar um comando g-code SCREWS_TILT_CALCULATE.

Veja o [guia de nivelamento](Manual_Level.md#adjusting-bed-leveling-screws-using-the-bed-probe) e a [referência de comandos](G-Codes.md#screws_tilt_adjust) para informações adicionais.

```
[screws_tilt_adjust]
#screw1:
#   A coordenada (X, Y) do primeiro parafuso de nivelamento da mesa. Esta é uma
#   posição para comandar o bico de modo que a sonda fique diretamente acima do
#   parafuso da mesa (ou o mais próximo possível ainda acima da mesa). Este é o
#   parafuso base usado nos cálculos. Este parâmetro deve ser fornecido.
#screw1_name:
#   Um nome arbitrário para o parafuso fornecido. Este nome é exibido quando o
#   script auxiliar é executado. O padrão é usar um nome baseado na localização
#   XY do parafuso.
#screw2:
#screw2_name:
#...
#   Parafusos adicionais de nivelamento da mesa. Pelo menos dois parafusos
#   devem ser definidos.
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover-se
#   imediatamente antes de iniciar uma operação de sondagem. O padrão é 5.
#screw_thread: CW-M3
#   O tipo de parafuso usado para nivelamento da mesa, M3, M4 ou M5, e a
#   direção de rotação do botão usado para nivelar a mesa. Valores aceitos:
#   CW-M3, CCW-M3, CW-M4, CCW-M4, CW-M5, CCW-M5. O valor padrão é CW-M3, que a
#   maioria das impressoras usa. Uma rotação do botão em sentido horário
#   diminui a folga entre o bico e a mesa. Inversamente, uma rotação em sentido
#   anti-horário aumenta a folga.
```

<a id="z_tilt"></a>
### [z_tilt]

Ajuste de inclinação com múltiplos motores de passo Z. Este recurso permite ajuste independente de múltiplos motores z (veja a seção "stepper_z1") para ajustar a inclinação. Se esta seção estiver presente, um [comando G-Code](G-Codes.md#z_tilt) estendido Z_TILT_ADJUST fica disponível.

```
[z_tilt]
#z_positions:
#   Uma lista de coordenadas X, Y (uma por linha; linhas subsequentes
#   indentadas) que descreve a localização de cada "ponto de pivô" da mesa. O
#   "ponto de pivô" é o ponto onde a mesa se fixa ao dado motor de passo Z. Ele
#   é descrito usando coordenadas do bico (a posição X, Y do bico se ele
#   pudesse mover-se diretamente acima do ponto). A primeira entrada
#   corresponde a stepper_z, a segunda a stepper_z1, a terceira a stepper_z2,
#   etc. Este parâmetro deve ser fornecido.
#points:
#   Uma lista de coordenadas X, Y (uma por linha; linhas subsequentes
#   indentadas) que devem ser sondadas durante um comando Z_TILT_ADJUST.
#   Especifique coordenadas do bico e certifique-se de que a sonda esteja acima
#   da mesa nas coordenadas de bico fornecidas. Este parâmetro deve ser
#   fornecido.
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover-se
#   imediatamente antes de iniciar uma operação de sondagem. O padrão é 5.
#retries: 0
#   Número de vezes a tentar novamente se os pontos sondados não estiverem
#   dentro da tolerância.
#retry_tolerance: 0
#   Se retries estiverem ativados, tentar novamente se os pontos sondados maior
#   e menor diferirem mais que retry_tolerance. Observe que a menor unidade de
#   alteração aqui seria um único passo. No entanto, se você estiver sondando
#   mais pontos que motores de passo, então provavelmente terá um valor mínimo
#   fixo para a faixa de pontos sondados que você pode aprender observando a
#   saída do comando.
```

<a id="quad_gantry_level"></a>
### [quad_gantry_level]

Nivelamento de pórtico móvel usando 4 motores Z controlados independentemente. Corrige efeitos de paraboloide hiperbólico (batata chips) em pórtico móvel que é mais flexível. AVISO: Usar isso em uma mesa móvel pode levar a resultados indesejáveis. Se esta seção estiver presente, um comando G-Code estendido QUAD_GANTRY_LEVEL fica disponível. Esta rotina assume a seguinte configuração de motores Z:

```
 ----------------
 |Z1          Z2|
 |  ---------   |
 |  |       |   |
 |  |       |   |
 |  x--------   |
 |Z           Z3|
 ----------------
```
Onde x é o ponto 0, 0 na mesa

```
[quad_gantry_level]
#gantry_corners:
#   Uma lista separada por nova linha de coordenadas X, Y que descreve os dois
#   cantos opostos do pórtico. A primeira entrada corresponde a Z, a segunda a
#   Z2. Este parâmetro deve ser fornecido.
#points:
#   Uma lista separada por nova linha de quatro pontos X, Y que devem ser
#   sondados durante um comando QUAD_GANTRY_LEVEL. A ordem das localizações é
#   importante e deve corresponder à localização Z, Z1, Z2 e Z3 em ordem. Este
#   parâmetro deve ser fornecido. Para precisão máxima, certifique-se de que
#   seus offsets da sonda estejam configurados.
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover-se
#   imediatamente antes de iniciar uma operação de sondagem. O padrão é 5.
#max_adjust: 4
#   Limite de segurança; se um ajuste maior que este valor for solicitado,
#   quad_gantry_level abortará.
#retries: 0
#   Número de vezes a tentar novamente se os pontos sondados não estiverem
#   dentro da tolerância.
#retry_tolerance: 0
#   Se retries estiverem ativados, tentar novamente se os pontos sondados maior
#   e menor diferirem mais que retry_tolerance.
```

<a id="skew_correction"></a>
### [skew_correction]

Correção de Inclinação da Impressora. É possível usar software para corrigir a inclinação da impressora em 3 planos, xy, xz, yz. Isso é feito imprimindo um modelo de calibração ao longo de um plano e medindo três comprimentos. Devido à natureza da correção de inclinação, esses comprimentos são definidos via gcode. Veja [Correção de Inclinação](Skew_Correction.md) e [Referência de Comandos](G-Codes.md#skew_correction) para detalhes.

```
[skew_correction]
```

<a id="z_thermal_adjust"></a>
### [z_thermal_adjust]

Ajuste de posição Z do cabeçote dependente de temperatura. Compensa em tempo real o movimento vertical do cabeçote causado pela expansão térmica da estrutura da impressora usando um sensor de temperatura (tipicamente acoplado a uma seção vertical da estrutura).

Veja também: [comandos g-code estendidos](G-Codes.md#z_thermal_adjust).

```
[z_thermal_adjust]
#temp_coeff:
#   O coeficiente de expansão por temperatura, em mm/degC. Por exemplo, um
#   temp_coeff de 0.01 mm/degC moverá o eixo Z para baixo em 0.01 mm para cada
#   grau Celsius que o sensor de temperatura aumentar. O padrão é 0.0 mm/degC,
#   que não aplica nenhum ajuste.
#smooth_time:
#   Janela de suavização aplicada ao sensor de temperatura, em segundos. Pode
#   reduzir o ruído do motor devido a pequenas correções excessivas em resposta
#   ao ruído do sensor. O padrão é 2.0 segundos.
#z_adjust_off_above:
#   Desativa ajustes acima desta altura Z [mm]. A última correção calculada
#   permanecerá aplicada até que o cabeçote se mova abaixo da altura Z
#   especificada novamente. O padrão é 99999999.0 mm (sempre ativo).
#max_z_adjustment:
#   Ajuste absoluto máximo que pode ser aplicado ao eixo Z [mm]. O padrão é
#   99999999.0 mm (sem limite).
#sensor_type:
#sensor_pin:
#min_temp:
#max_temp:
#   Configuração do sensor de temperatura. Consulte a seção "extruder" para a
#   definição dos parâmetros acima.
#gcode_id:
#   Consulte a seção "heater_generic" para a definição deste parâmetro.
```

<a id="customized-homing"></a>
## Referenciamento personalizado

<a id="safe_z_home"></a>
### [safe_z_home]

Referenciamento Z seguro. Pode-se usar este mecanismo para referenciar o eixo Z em uma coordenada X, Y específica. Isso é útil se o cabeçote, por exemplo, precisar se mover para o centro da mesa antes que Z possa ser referenciado.

```
[safe_z_home]
home_xy_position:
#   Uma coordenada X, Y (e.g. 100, 100) onde o homing(referenciamento) Z deve
#   ser realizado. Este parâmetro deve ser fornecido.
#speed: 50.0
#   Velocidade na qual o cabeçote é movido para a coordenada segura de
#   referenciamento Z. O padrão é 50 mm/s
#z_hop:
#   Distância (em mm) para elevar o eixo Z antes do homing(referenciamento).
#   Isto é aplicado a qualquer comando de homing(referenciamento), mesmo que
#   não referencie o eixo Z. Se o eixo Z já estiver referenciado e a posição Z
#   atual for menor que z_hop, então isto elevará o cabeçote a uma altura de
#   z_hop. Se o eixo Z ainda não estiver referenciado, o cabeçote é elevado em
#   z_hop. O padrão é não implementar Z hop.
#z_hop_speed: 15.0
#   Velocidade (em mm/s) na qual o eixo Z é elevado antes do
#   homing(referenciamento). O padrão é 15 mm/s.
#move_to_previous: False
#   Quando definido como True, os eixos X e Y são retornados às suas posições
#   anteriores após o homing(referenciamento) do eixo Z. O padrão é False.
```

<a id="homing_override"></a>
### [homing_override]

Sobrescrita de referenciamento (homing). Pode-se usar este mecanismo para executar uma série de comandos G-code no lugar de um G28 encontrado na entrada normal de G-code. Isso pode ser útil em impressoras que exigem um procedimento específico para referenciar a máquina.

```
[homing_override]
gcode:
#   Uma lista de comandos G-Code para executar no lugar de comandos G28
#   encontrados na entrada normal de g-code. Consulte docs/Command_Templates.md
#   para o formato G-Code. Se um G28 estiver contido nesta lista de comandos,
#   então ele invocará o procedimento normal de homing(referenciamento) para a
#   impressora. Os comandos listados aqui devem referenciar todos os eixos.
#   Este parâmetro deve ser fornecido.
#axes: xyz
#   Os eixos a sobrescrever. Por exemplo, se isto for definido como "z", então
#   o script de sobrescrita será executado somente quando o eixo Z for
#   referenciado (eg, via um comando "G28" ou "G28 Z0"). Observe que o script
#   de sobrescrita ainda deve referenciar todos os eixos. O padrão é "xyz", que
#   faz com que o script de sobrescrita seja executado no lugar de todos os
#   comandos G28.
#set_position_x:
#set_position_y:
#set_position_z:
#   Se especificado, a impressora assumirá que o eixo está na posição
#   especificada antes de executar os comandos g-code acima. Definir isto
#   desativa verificações de homing(referenciamento) para esse eixo. Isto pode
#   ser útil se o cabeçote precisar mover-se antes de invocar o mecanismo
#   normal G28 para um eixo. O padrão é não forçar uma posição para um eixo.
```

<a id="endstop_phase"></a>
### [endstop_phase]

Fins de curso com ajuste de fase do motor de passo. Para usar este recurso, defina uma seção de configuração com o prefixo "endstop_phase" seguido do nome da seção de configuração do motor de passo correspondente (por exemplo, "[endstop_phase stepper_z]"). Este recurso pode melhorar a precisão das chaves de fim de curso. Adicione uma declaração simples "[endstop_phase]" para habilitar o comando ENDSTOP_PHASE_CALIBRATE.

Consulte o [guia de fases de fim de curso](Endstop_Phase.md) e a [referência de comandos](G-Codes.md#endstop_phase) para informações adicionais.

```
[endstop_phase stepper_z]
#endstop_accuracy:
#   Define a precisão esperada (em mm) do fim de curso. Isto representa a
#   distância máxima de erro com que o fim de curso pode disparar (eg, se um
#   fim de curso puder ocasionalmente disparar 100um adiantado ou até 100um
#   atrasado, então defina isto como 0.200 para 200um). O padrão é
#   4*rotation_distance/full_steps_per_rotation.
#trigger_phase:
#   Isto especifica a fase do driver do motor de passo a esperar ao atingir o
#   fim de curso. Ela é composta por dois números separados por um caractere de
#   barra (forward slash) - a fase e o número total de fases (eg, "7/64").
#   Defina este valor somente se tiver certeza de que o driver do motor de
#   passo é reiniciado toda vez que o mcu é reiniciado. Se isto não for
#   definido, então a fase do motor de passo será detectada no primeiro
#   referenciamento e essa fase será usada em todos os referenciamentos
#   subsequentes.
#endstop_align_zero: False
#   Se true, então position_endstop do eixo será efetivamente modificado de
#   modo que a posição zero para o eixo ocorra em um passo completo no motor de
#   passo. (Se usado no eixo Z e a altura da camada de impressão for um
#   múltiplo de uma distância de passo completo, então cada camada ocorrerá em
#   um passo completo.) O padrão é False.
```

<a id="g-code-macros-and-events"></a>
## Macros de G-Code e eventos

<a id="gcode_macro"></a>
### [gcode_macro]

Macros de G-Code (pode-se definir qualquer número de seções com o prefixo "gcode_macro"). Consulte o [guia de modelos de comando](Command_Templates.md) para mais informações.

```
[gcode_macro my_cmd]
#gcode:
#   Uma lista de comandos G-Code para executar no lugar de "my_cmd". Consulte
#   docs/Command_Templates.md para o formato G-Code. Este parâmetro deve ser
#   fornecido.
#variable_<name>:
#   Pode-se especificar qualquer número de opções com prefixo "variable_". O
#   nome de variável fornecido receberá o valor fornecido (analisado como um
#   literal Python) e estará disponível durante a expansão da macro. Por
#   exemplo, uma config com "variable_fan_speed = 75" pode ter comandos gcode
#   contendo "M106 S{ fan_speed * 255 }". Variáveis podem ser alteradas em
#   tempo de execução usando o comando SET_GCODE_VARIABLE (consulte
#   docs/Command_Templates.md para detalhes). Nomes de variáveis não podem usar
#   caracteres maiúsculos.
#rename_existing:
#   Esta opção fará com que a macro sobrescreva um comando G-Code existente e
#   forneça a definição anterior do comando através do nome fornecido aqui.
#   Isto pode ser usado para sobrescrever comandos G-Code integrados. Deve-se
#   ter cuidado ao sobrescrever comandos, pois pode causar resultados complexos
#   e inesperados. O padrão é não sobrescrever um comando G-Code existente.
#description: G-Code macro
#   Isto adicionará uma breve descrição usada no comando HELP ou ao usar o
#   recurso de conclusão automática. Padrão "G-Code macro"
```

<a id="delayed_gcode"></a>
### [delayed_gcode]

Executa um G-code após um atraso definido. Consulte o [guia de modelos de comando](Command_Templates.md#delayed-gcodes) e a [referência de comandos](G-Codes.md#delayed_gcode) para mais informações.

```
[delayed_gcode my_delayed_gcode]
gcode:
#   Uma lista de comandos G-Code para executar quando a duração do atraso tiver
#   decorrido. Modelos G-Code são suportados. Este parâmetro deve ser
#   fornecido.
#initial_duration: 0.0
#   A duração do atraso inicial (em segundos). Se definida com valor diferente
#   de zero, o delayed_gcode executará o número especificado de segundos após a
#   impressora entrar no estado "ready". Isso pode ser útil para procedimentos
#   de inicialização ou um delayed_gcode repetitivo. Se definida como 0, o
#   delayed_gcode não executará na inicialização. O padrão é 0.
```

<a id="save_variables"></a>
### [save_variables]

Suporte a salvar variáveis em disco para que sejam mantidas entre reinicializações. Consulte [modelos de comando](Command_Templates.md#save-variables-to-disk) e a [referência de G-Code](G-Codes.md#save_variables) para mais informações.

```
[save_variables]
filename:
#   Obrigatório - forneça um nome de arquivo que seria usado para salvar as
#   variáveis no disco, por exemplo ~/variables.cfg
```

<a id="idle_timeout"></a>
### [idle_timeout]

Tempo limite de ociosidade. Um tempo limite de ociosidade é habilitado automaticamente - adicione uma seção de configuração idle_timeout explícita para alterar as configurações padrão.

```
[idle_timeout]
#gcode:
#   Uma lista de comandos G-Code para executar em tempo limite de ociosidade.
#   Consulte docs/Command_Templates.md para o formato de G-Code. O padrão é
#   executar "TURN_OFF_HEATERS" e "M84".
#timeout: 600
#   Tempo ocioso (em segundos) para aguardar antes de executar os comandos
#   G-Code acima. O padrão é 600 segundos.
```

<a id="optional-g-code-features"></a>
## Recursos opcionais de G-Code

<a id="virtual_sdcard"></a>
### [virtual_sdcard]

Um cartão sd virtual pode ser útil se a máquina hospedeira não é rápida o suficiente para rodar o OctoPrint bem. Isto permite que o softwares hospedeiro do Klipper imprima diretamente o arquivo gcode da máquina hospedeira usando comandos padrões G-Code de cartão sd (ex, M24)

```
[virtual_sdcard]
path:
#   O caminho do diretório local na máquina hospedeira onde procurar arquivos
#   g-code. Este é um diretório somente leitura (escritas de arquivos sdcard
#   não são suportadas). Pode-se apontar isso para o diretório de upload do
#   OctoPrint (geralmente ~/.octoprint/uploads/ ). Este parâmetro deve ser
#   fornecido.
#on_error_gcode:
#   Uma lista de comandos G-Code para executar quando um erro é relatado.
#   Consulte docs/Command_Templates.md para o formato de G-Code. O padrão é
#   executar TURN_OFF_HEATERS.
```

<a id="sdcard_loop"></a>
### [sdcard_loop]

Algumas impressoras com recursos de liberação da mesa, como um ejetor de peças ou uma impressora de esteira (belt printer), podem se beneficiar da repetição em laço (loop) de trechos do arquivo do cartão SD. (Por exemplo, para imprimir a mesma peça repetidamente, ou repetir uma seção de uma peça para uma corrente ou outro padrão repetido).

Consulte a [referência de comandos](G-Codes.md#sdcard_loop) para os comandos suportados. Consulte o arquivo [sample-macros.cfg](../../config/sample-macros.cfg) para uma macro de G-Code M808 compatível com Marlin.

```
[sdcard_loop]
```

<a id="force_move"></a>
### [force_move]

Suporte a mover manualmente motores de passo para fins de diagnóstico. Observe que usar este recurso pode colocar a impressora em um estado inválido - consulte a [referência de comandos](G-Codes.md#force_move) para detalhes importantes.

```
[force_move]
#enable_force_move: False
#   Defina como true para habilitar os comandos G-Code estendidos FORCE_MOVE e
#   SET_KINEMATIC_POSITION. O padrão é false.
```

<a id="pause_resume"></a>
### [pause_resume]

Funcionalidade de Pausa/Continuação (Pause/Resume) com suporte a captura e restauração de posição. Consulte a [referência de comandos](G-Codes.md#pause_resume) para mais informações.

```
[pause_resume]
#recover_velocity: 50.
#   Quando capture/restore está habilitado, a velocidade com que retornar à
#   posição capturada (em mm/s). O padrão é 50.0 mm/s.
```

<a id="firmware_retraction"></a>
### [firmware_retraction]

Retirada (retraction) de filamento por firmware. Isso habilita os comandos GCODE G10 (retirada) e G11 (des-retirada) emitidos por muitos fatiadores. Os parâmetros abaixo fornecem os padrões iniciais, embora os valores possam ser ajustados via comando SET_RETRACTION [comando](G-Codes.md#firmware_retraction)), permitindo configurações por filamento e ajuste em tempo de execução.

```
[firmware_retraction]
#retract_length: 0
#   O comprimento de filamento (em mm) para executar a retirada quando G10 é
#   ativado, e para desfazer a retirada quando G11 é ativado (mas veja
#   unretract_extra_length abaixo). O padrão é 0 mm.
#retract_speed: 20
#   A velocidade da retirada, em mm/s. O padrão é 20 mm/s.
#unretract_extra_length: 0
#   O comprimento (em mm) de filamento *adicional* a adicionar ao desfazer a
#   retirada.
#unretract_speed: 10
#   A velocidade para desfazer a retirada, em mm/s. O padrão é 10 mm/s.
```

<a id="gcode_arcs"></a>
### [gcode_arcs]

Suporte a comandos de arco em G-code (G2/G3).

```
[gcode_arcs]
#resolution: 1.0
#   Um arco será dividido em segmentos. O comprimento de cada segmento será
#   igual à resolução em mm definida acima. Valores mais baixos produzirão um
#   arco mais fino, mas também mais trabalho para sua máquina. Arcos menores
#   que o valor configurado se tornarão linhas retas. O padrão é 1mm.
```

<a id="respond"></a>
### [respond]

Habilita os [comandos](G-Codes.md#respond) estendidos "M118" e "RESPOND".

```
[respond]
#default_type: echo
#   Define o prefixo padrão da saída de "M118" e "RESPOND" para um dos
#   seguintes:
#       echo: "echo: " (Este é o padrão)
#       command: "// "
#       error: "!! "
#default_prefix: echo:
#   Define diretamente o prefixo padrão. Se presente, este valor substituirá o
#   "default_type".
```

<a id="exclude_object"></a>
### [exclude_object]

Habilita suporte para excluir ou cancelar objetos individuais durante o processo de impressão.

Consulte o [guia de exclusão de objetos](Exclude_Object.md) e a [referência de comandos](G-Codes.md#exclude_object) para informações adicionais. Consulte o arquivo [sample-macros.cfg](../../config/sample-macros.cfg) para uma macro de G-Code M486 compatível com Marlin/RepRapFirmware.

```
[exclude_object]
```

<a id="resonance-compensation"></a>
## Compensação de ressonância

<a id="input_shaper"></a>
### [input_shaper]

Habilita a [compensação de ressonância](Resonance_Compensation.md). Consulte também a [referência de comandos](G-Codes.md#input_shaper).

```
[input_shaper]
#shaper_freq_x: 0
#   Uma frequência (em Hz) do input shaper para o eixo X. Geralmente é uma
#   frequência de ressonância do eixo X que o input shaper deve suprimir. Para
#   shapers mais complexos, como os input shapers EI de 2 e 3 hump, este
#   parâmetro pode ser definido a partir de considerações diferentes. O valor
#   padrão é 0, o que desabilita input shaping para o eixo X.
#shaper_freq_y: 0
#   Uma frequência (em Hz) do input shaper para o eixo Y. Geralmente é uma
#   frequência de ressonância do eixo Y que o input shaper deve suprimir. Para
#   shapers mais complexos, como os input shapers EI de 2 e 3 hump, este
#   parâmetro pode ser definido a partir de considerações diferentes. O valor
#   padrão é 0, o que desabilita input shaping para o eixo Y.
#shaper_freq_z: 0
#   Uma frequência (em Hz) do input shaper para o eixo Z. O valor padrão é 0, o
#   que desabilita input shaping para o eixo Z.
#shaper_type: mzv
#   Um tipo de input shaper a usar para todos os eixos. Os shapers suportados
#   são zv, mzv, zvd, ei, 2hump_ei e 3hump_ei. Alguns shapers suportam
#   parâmetros adicionais opcionais, por exemplo mzv(n=4,t=0.9) ou
#   ei(v_tol=0.1). O padrão é o input shaper mzv (sem parâmetros).
#shaper_type_x:
#shaper_type_y:
#shaper_type_z:
#   Se shaper_type não estiver definido, estes parâmetros podem ser usados para
#   configurar diferentes input shapers para os eixos X, Y e Z. Os mesmos
#   valores são suportados como para o parâmetro shaper_type.
#damping_ratio_x: 0.1
#damping_ratio_y: 0.1
#damping_ratio_z: 0.1
#   Razões de amortecimento das vibrações dos eixos X e Y usadas pelos input
#   shapers para melhorar a supressão de vibração. O valor padrão é 0.1, que é
#   um bom valor geral para a maioria das impressoras. Na maioria das
#   circunstâncias este parâmetro não requer ajuste e não deve ser alterado.
```

<a id="adxl345"></a>
### [adxl345]

Suporte a acelerômetros ADXL345. Este suporte permite consultar medições do acelerômetro a partir do sensor. Isso habilita um comando ACCELEROMETER_MEASURE (consulte [G-Codes](G-Codes.md#adxl345) para mais informações). O nome padrão do chip é "default", mas pode-se especificar um nome explícito (ex.: [adxl345 my_chip_name]).

```
[adxl345]
cs_pin:
#   O pino SPI enable para a sonda. Este parâmetro deve ser fornecido.
#spi_speed: 5000000
#   A velocidade SPI (em hz) a usar ao comunicar com o chip. O padrão é
#   5000000.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#axes_map: x, y, z
#   O eixo do acelerômetro para cada um dos eixos X, Y e Z da impressora. Isso
#   pode ser útil se o acelerômetro estiver montado em uma orientação que não
#   corresponde à orientação da impressora. Por exemplo, pode-se definir isso
#   como "y, x, z" para trocar os eixos X e Y. Também é possível negar um eixo
#   se a direção do acelerômetro estiver invertida (por exemplo, "x, z, -y"). O
#   padrão é "x, y, z".
#rate: 3200
#   Taxa de dados de saída para ADXL345. ADXL345 suporta os seguintes dados
#   taxas: 3200, 1600, 800, 400, 200, 100, 50 e 25. Observe que
#   não é recomendado alterar esta taxa a partir do padrão 3200, e taxas abaixo
#   de 800 afetarão consideravelmente a qualidade das medições de ressonância.
```

<a id="icm20948"></a>
### [icm20948]

Suporte a acelerômetros icm20948.

```
[icm20948]
#i2c_address:
#   O padrão é 104 (0x68). Se AD0 estiver em nível alto, será 0x69.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed: 400000
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima. O "i2c_speed" padrão é 400000.
#axes_map: x, y, z
#   Consulte a seção "adxl345" para informações sobre este parâmetro.
```

<a id="lis2dw"></a>
### [lis2dw]

Suporte a acelerômetros LIS2DW.

```
[lis2dw]
#cs_pin:
#   O pino SPI enable para a sonda. Este parâmetro deve ser fornecido se usar
#   SPI.
#spi_speed: 5000000
#   A velocidade SPI (em hz) a usar ao comunicar com o chip. O padrão é
#   5000000.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#i2c_address:
#   O padrão é 25 (0x19). Se SA0 estiver em nível alto, será 24 (0x18).
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed: 400000
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima. O "i2c_speed" padrão é 400000.
#axes_map: x, y, z
#   Consulte a seção "adxl345" para informações sobre este parâmetro.
```

<a id="lis3dh"></a>
### [lis3dh]

Suporte a acelerômetros LIS3DH.

```
[lis3dh]
#cs_pin:
#   O pino SPI enable para a sonda. Este parâmetro deve ser fornecido se usar
#   SPI.
#spi_speed: 5000000
#   A velocidade SPI (em hz) a usar ao comunicar com o chip. O padrão é
#   5000000.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#i2c_address:
#   O padrão é 25 (0x19). Se SA0 estiver em nível alto, será 24 (0x18).
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed: 400000
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima. O "i2c_speed" padrão é 400000.
#axes_map: x, y, z
#   Consulte a seção "adxl345" para informações sobre este parâmetro.
```

<a id="bmi160"></a>
### [bmi160]

Acelerômetro BMI160. Este sensor pode ser consultado via barramento I2C ou SPI.

```
[bmi160]
#i2c_address:
#   O padrão é 105 (0x69). Se SA0 estiver ligado a GND, use 104 (0x68). Usado
#   apenas para I2C.
#i2c_mcu:
#i2c_bus:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima. Usado apenas para I2C.
#cs_pin:
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima. Usado apenas para SPI.
#axes_map: x, y, z
#   Consulte a seção "adxl345" para informações sobre este parâmetro.
```

**Importante:** Muitos módulos BMI160 usam rótulos de pinos ambíguos. Para SPI:

- Use **SCL** para clock (não SCX)
- Use **SDA** para MOSI (não SDX)
- Use **SA0** para MISO
- Use **CS** para chip select

Os pinos identificados como SCX/SDX são para o barramento auxiliar do magnetômetro.

<a id="mpu9250"></a>
### [mpu9250]

Suporte a acelerômetros MPU-9250, MPU-9255, MPU-6515, MPU-6050 e MPU-6500 (pode-se definir qualquer número de seções com o prefixo "mpu9250").

```
[mpu9250 my_accelerometer]
#i2c_address:
#   O padrão é 104 (0x68). Se AD0 estiver em nível alto, será 0x69.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed: 400000
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima. O "i2c_speed" padrão é 400000.
#axes_map: x, y, z
#   Consulte a seção "adxl345" para informações sobre este parâmetro.
```

<a id="resonance_tester"></a>
### [resonance_tester]

Suporte a teste de ressonância e calibração automática do modelador de entrada (input shaper). Para usar a maior parte da funcionalidade deste módulo, dependências adicionais de software devem ser instaladas; consulte [Medição de ressonâncias](Measuring_Resonances.md) e a [referência de comandos](G-Codes.md#resonance_tester) para mais informações. Consulte a seção [Suavização máxima](Measuring_Resonances.md#max-smoothing) do guia de medição de ressonâncias para mais informações sobre o parâmetro `max_smoothing` e seu uso.

```
[resonance_tester]
#probe_points:
#   Uma lista de coordenadas X, Y, Z de pontos (um ponto por linha) para testar
#   ressonâncias. Pelo menos um ponto é obrigatório. Certifique-se de que todos
#   os pontos com alguma margem de segurança no plano XY (~alguns centímetros)
#   sejam alcançáveis pelo cabeçote.
#accel_chip:
#   Um nome do chip acelerômetro a usar para medições. Se um acelerômetro foi
#   definido sem nome explícito, este parâmetro pode simplesmente referenciá-lo
#   pelo tipo, por exemplo "accel_chip: adxl345", caso contrário um nome
#   completo deve ser fornecido, por exemplo "accel_chip: adxl345
#   my_chip_name". Ou este, ou os próximos dois parâmetros devem ser definidos.
#accel_chip_x:
#accel_chip_y:
#   Nomes dos chips acelerômetros a usar para medições para cada um dos eixos.
#   Pode ser útil, por exemplo, em impressora bed slinger, se dois
#   acelerômetros separados estiverem montados na mesa (para o eixo Y) e no
#   cabeçote (para o eixo X). Estes parâmetros têm o mesmo formato que o
#   parâmetro 'accel_chip'. Somente 'accel_chip' ou estes dois parâmetros devem
#   ser fornecidos.
#accel_chip_z:
#   Um nome do chip acelerômetro a usar para medições do eixo Z. Este parâmetro
#   tem o mesmo formato que 'accel_chip'. O padrão é não configurar um
#   acelerômetro para o eixo Z.
#max_smoothing:
#   Suavização máxima de input shaper a permitir para cada eixo durante a
#   autocalibração do shaper (com o comando 'SHAPER_CALIBRATE'). Por padrão
#   nenhuma suavização máxima é especificada. Consulte o guia
#   Measuring_Resonances para mais detalhes sobre o uso deste recurso.
#move_speed: 50
#   A velocidade (em mm/s) para mover o cabeçote até os pontos de teste e entre
#   eles durante a calibração. O padrão é 50.
#min_freq: 5
#   Frequência mínima para testar ressonâncias. O padrão é 5 Hz.
#max_freq: 135
#   Frequência máxima para testar ressonâncias. O padrão é 135 Hz.
#max_freq_z: 100
#   Frequência máxima para testar o eixo Z quanto a ressonâncias. O padrão é
#   100 Hz.
#accel_per_hz: 60
#   Este parâmetro é usado para determinar qual aceleração usar para testar uma
#   frequência específica: accel = accel_per_hz * freq. Quanto maior o valor,
#   maior é a energia das oscilações. Pode ser definido com valor menor que o
#   padrão se as ressonâncias ficarem muito fortes na impressora. Entretanto,
#   valores menores tornam as medições de ressonâncias de alta frequência menos
#   precisas. O valor padrão é 60 (mm/sec).
#accel_per_hz_z: 15
#   Este parâmetro tem o mesmo significado que accel_per_hz, mas se aplica
#   especificamente ao eixo Z. O padrão é 15 (mm/sec).
#hz_per_sec: 1
#   Determina a velocidade do teste. Ao testar todas as frequências no
#   intervalo [min_freq, max_freq], a cada segundo a frequência aumenta em
#   hz_per_sec. Valores pequenos tornam o teste lento, e valores grandes
#   diminuirão a precisão do teste. O valor padrão é 1.0 (Hz/sec == sec^-2).
#sweeping_accel: 400
#   Uma aceleração dos movimentos lentos de varredura. O padrão é 400 mm/sec^2.
#sweeping_accel_z: 50
#   O mesmo que sweeping_accel acima, mas para o eixo Z. O padrão é 50
#   mm/sec^2.
#sweeping_period: 1.2
#   Um período dos movimentos lentos de varredura. Definir este parâmetro como
#   0 desabilita movimentos lentos de varredura. Evite defini-lo com valor
#   diferente de zero muito pequeno para não contaminar as medições. O padrão é
#   1.2 sec, que é uma boa escolha geral.
```

<a id="config-file-helpers"></a>
## Auxiliares de arquivo de configuração

<a id="board_pins"></a>
### [board_pins]

Apelidos para pinos da placa (pode-se definir qualquer número de secções com um prefixo "board_pins"). Use isto para definir apelidos para pinos em um microcontrolador.

```
[board_pins my_aliases]
mcu: mcu
#   Uma lista separada por vírgulas de microcontroladores que podem usar os
#   apelidos. O padrão é aplicar os apelidos ao "mcu" principal.
aliases:
aliases_<name>:
#   Uma lista separada por vírgulas de apelidos "name=value" a criar para o
#   microcontrolador dado. Por exemplo, "EXP1_1=PE6" criaria um apelido
#   "EXP1_1" para o pino "PE6". Entretanto, se "value" estiver entre "<>" então
#   "name" é criado como pino reservado (por exemplo, "EXP1_9=<GND>" reservaria
#   "EXP1_9"). Qualquer número de opções começando com "aliases_" pode ser
#   especificado.
```

<a id="include"></a>
### [include]

Suporta a inclusão de arquivos. Pode-se incluir arquivos de configuração adicionais a partir do arquivo configuração principal. Caracteres coringa também podem ser utilizados (ex. [include configs/*.cfg]).

```
[include my_other_config.cfg]
```

<a id="duplicate_pin_override"></a>
### [duplicate_pin_override]

Esta ferramenta permite que um único pino de microcontrolador seja definido várias vezes em um arquivo de configuração sem a verificação normal de erros. Isso destina-se a fins de diagnóstico e depuração. Esta seção não é necessária onde o Klipper suporta o uso do mesmo pino várias vezes, e usar esta sobrescrita pode causar resultados confusos e inesperados.

```
[duplicate_pin_override]
pins:
#   Uma lista separada por vírgulas de pinos que podem ser usados várias vezes
#   em um arquivo de configuração sem as verificações normais de erro. Este
#   parâmetro deve ser fornecido.
```

<a id="bed-probing-hardware"></a>
## Hardware de sondagem da mesa

<a id="probe"></a>
### [probe]

Sonda de altura Z. Pode-se definir esta seção para habilitar o hardware de sondagem de altura Z. Quando esta seção está habilitada, os [comandos](G-Codes.md#probe) estendidos de G-code PROBE e QUERY_PROBE ficam disponíveis. Consulte também o [guia de calibração da sonda](Probe_Calibrate.md). A seção de sonda também cria um pino virtual "probe:z_virtual_endstop". Pode-se definir o endstop_pin do stepper_z para este pino virtual em impressoras estilo cartesiano que usam a sonda no lugar de um fim de curso de Z. Se estiver usando "probe:z_virtual_endstop", não defina um position_endstop na seção de configuração stepper_z.

```
[probe]
pin:
#   Pino de detecção da sonda. Se o pino estiver em um microcontrolador
#   diferente dos motores de passo de Z, então habilita "multi-mcu homing".
#   Este parâmetro deve ser fornecido.
#deactivate_on_each_sample: True
#   Isso determina se o Klipper deve executar gcode de desativação entre cada
#   tentativa de sondagem ao executar uma sequência de sondagem múltipla. O
#   padrão é True.
#x_offset: 0.0
#   A distância (em mm) entre a sonda e o bico ao longo do eixo x. O padrão é
#   0.
#y_offset: 0.0
#   A distância (em mm) entre a sonda e o bico ao longo do eixo y. O padrão é
#   0.
z_offset:
#   A distância (em mm) entre a mesa e o bico quando a sonda dispara. Este
#   parâmetro deve ser fornecido.
#speed: 5.0
#   Velocidade (em mm/s) do eixo Z durante a sondagem. Pode ser possível
#   alterar este valor em tempo de execução via parâmetro de comando
#   "PROBE_SPEED". O padrão é 5mm/s.
#samples: 1
#   O número de vezes para sondar cada ponto. Os valores z sondados terão a
#   média calculada. Pode ser possível alterar este valor em tempo de execução
#   via parâmetro de comando "SAMPLES". O padrão é sondar 1 vez.
#sample_retract_dist: 2.0
#   A distância (em mm) para elevar o cabeçote entre cada amostra (se amostrar
#   mais de uma vez). Pode ser possível alterar este valor em tempo de execução
#   via parâmetro de comando "SAMPLE_RETRACT_DIST". O padrão é 2mm.
#lift_speed:
#   Velocidade (em mm/s) do eixo Z ao elevar a sonda entre amostras. Pode ser
#   possível alterar este valor em tempo de execução via parâmetro de comando
#   "LIFT_SPEED". O padrão é usar o mesmo valor do parâmetro 'speed'.
#samples_result: average
#   O método de cálculo ao amostrar mais de uma vez - seja "median" ou
#   "average". Pode ser possível alterar este valor em tempo de execução via
#   parâmetro de comando "SAMPLES_RESULT". O padrão é average.
#samples_tolerance: 0.100
#   A distância Z máxima (em mm) que uma amostra pode diferir de outras
#   amostras. Se esta tolerância for excedida, então ou um erro é relatado ou a
#   tentativa é reiniciada (veja samples_tolerance_retries). Pode ser possível
#   alterar este valor em tempo de execução via parâmetro de comando
#   "SAMPLES_TOLERANCE". O padrão é 0.100mm.
#samples_tolerance_retries: 0
#   O número de vezes para tentar novamente se for encontrada uma amostra que
#   excede samples_tolerance. Em uma nova tentativa, todas as amostras atuais
#   são descartadas e a tentativa de sondagem é reiniciada. Se um conjunto
#   válido de amostras não for obtido no número dado de novas tentativas, então
#   um erro é relatado. Pode ser possível alterar este valor em tempo de
#   execução via parâmetro de comando "SAMPLES_TOLERANCE_RETRIES". O padrão é
#   zero, o que faz com que um erro seja relatado na primeira amostra que
#   excede samples_tolerance.
#activate_gcode:
#   Uma lista de comandos G-Code para executar antes de cada tentativa de
#   sondagem. Consulte docs/Command_Templates.md para o formato de G-Code. Isso
#   pode ser útil se a sonda precisar ser ativada de alguma forma. Não emita
#   aqui nenhum comando que mova o cabeçote (por exemplo, G1). O padrão é não
#   executar nenhum comando G-Code especial na ativação.
#deactivate_gcode:
#   Uma lista de comandos G-Code para executar após a conclusão de cada
#   tentativa de sondagem. Consulte docs/Command_Templates.md para o formato de
#   G-Code. Não emita aqui nenhum comando que mova o cabeçote. O padrão é não
#   executar nenhum comando G-Code especial na desativação.
```

<a id="bltouch"></a>
### [bltouch]

Sonda BLTouch. Pode-se definir esta secção (ao invés de uma secção probe) para habilitar uma sonda BLTouch. Veja [Guia BL-Touch](BLTouch.md) e [Referencia de comando](G-Codes.md#bltouch) para mais informações. Um pino virtual "probe:z_virtual_endstop" também é criado (veja a secção "probe" para detalhes).

```
[bltouch]
sensor_pin:
#   Pino conectado ao pino sensor do BLTouch. A maioria dos dispositivos
#   BLTouch requer um pullup no pino sensor (prefixe o nome do pino com "^").
#   Este parâmetro deve ser fornecido.
control_pin:
#   Pino conectado ao pino de controle do BLTouch. Este parâmetro deve ser
#   fornecido.
#pin_move_time: 0.680
#   A quantidade de tempo (em segundos) para aguardar o pino do BLTouch mover
#   para cima ou para baixo. O padrão é 0.680 segundos.
#stow_on_each_sample: True
#   Isso determina se o Klipper deve comandar o pino a mover para cima entre
#   cada tentativa de sondagem ao executar uma sequência de sondagem múltipla.
#   Leia as instruções em docs/BLTouch.md antes de definir isso como False. O
#   padrão é True.
#probe_with_touch_mode: False
#   Se isso for definido como True, então o Klipper sondará com o dispositivo
#   em "touch_mode". O padrão é False (sondagem em modo "pin_down").
#pin_up_reports_not_triggered: True
#   Defina se o BLTouch relata consistentemente a sonda em estado "not
#   triggered" após um comando "pin_up" bem-sucedido. Isso deve ser True para
#   todos os dispositivos BLTouch genuínos. Leia as instruções em
#   docs/BLTouch.md antes de definir isso como False. O padrão é True.
#pin_up_touch_mode_reports_triggered: True
#   Defina se o BLTouch relata consistentemente um estado "triggered" após os
#   comandos "pin_up" seguidos de "touch_mode". Isso deve ser True para todos
#   os dispositivos BLTouch genuínos. Leia as instruções em docs/BLTouch.md
#   antes de definir isso como False. O padrão é True.
#set_output_mode:
#   Solicita um modo de saída específico do pino sensor no BLTouch V3.0 (e
#   posterior). Esta configuração não deve ser usada em outros tipos de sonda.
#   Defina como "5V" para solicitar uma saída do pino sensor de 5 Volts (use
#   somente se a placa controladora precisar do modo 5V e for tolerante a 5V em
#   sua linha de sinal de entrada). Defina como "OD" para solicitar que a saída
#   do pino sensor use modo open drain. O padrão é não solicitar um modo de
#   saída.
#x_offset:
#y_offset:
#z_offset:
#speed:
#lift_speed:
#samples:
#sample_retract_dist:
#samples_result:
#samples_tolerance:
#samples_tolerance_retries:
#   Consulte a seção "probe" para informações sobre estes parâmetros.
```

<a id="smart_effector"></a>
### [smart_effector]

O "Smart Effector" da Duet3d implementa uma sonda Z usando um sensor de força. Pode-se definir esta seção em vez de `[probe]` para habilitar os recursos específicos do Smart Effector. Isso também habilita [comandos em tempo de execução](G-Codes.md#smart_effector) para ajustar os parâmetros do Smart Effector em tempo de execução.

```
[smart_effector]
pin:
#   Pino conectado ao pino de saída Z Probe do Smart Effector (pino 5). Observe
#   que resistor de pullup na placa geralmente não é necessário. Entretanto, se
#   o pino de saída estiver conectado ao pino da placa com resistor de pullup,
#   esse resistor deve ser de valor alto (por exemplo, 10K Ohm ou mais).
#   Algumas placas têm resistor de pullup de valor baixo na entrada da sonda Z,
#   o que provavelmente resultará em estado de sonda sempre disparada. Neste
#   caso, conecte o Smart Effector a um pino diferente na placa. Este parâmetro
#   é obrigatório.
#control_pin:
#   Pino conectado ao pino de entrada de controle do Smart Effector (pino 7).
#   Se fornecido, comandos de programação de sensibilidade do Smart Effector
#   ficam disponíveis.
#probe_accel:
#   Se definido, limita a aceleração dos movimentos de sondagem (em mm/sec^2).
#   Uma aceleração grande e repentina no início do movimento de sondagem pode
#   causar disparo espúrio da sonda, especialmente se o hotend for pesado. Para
#   evitar isso, pode ser necessário reduzir a aceleração dos movimentos de
#   sondagem via este parâmetro.
#recovery_time: 0.4
#   Um atraso entre os movimentos de deslocamento e os movimentos de sondagem
#   em segundos. Um movimento de deslocamento rápido antes da sondagem pode
#   resultar em disparo espúrio da sonda. Isso pode causar erros 'Probe
#   triggered prior to movement' se nenhum atraso for definido. O valor 0
#   desabilita o atraso de recuperação. O valor padrão é 0.4.
#x_offset:
#y_offset:
#   Deve ser deixado indefinido (ou definido como 0).
z_offset:
#   Altura de disparo da sonda. Comece com -0.1 (mm), e ajuste posteriormente
#   usando o comando `PROBE_CALIBRATE`. Este parâmetro deve ser fornecido.
#speed:
#   Velocidade (em mm/s) do eixo Z durante a sondagem. Recomenda-se começar com
#   a velocidade de sondagem de 20 mm/s e ajustar conforme necessário para
#   melhorar a exatidão e a repetibilidade do disparo da sonda.
#samples:
#sample_retract_dist:
#samples_result:
#samples_tolerance:
#samples_tolerance_retries:
#activate_gcode:
#deactivate_gcode:
#deactivate_on_each_sample:
#   Consulte a seção "probe" para mais informações sobre os parâmetros acima.
```

<a id="probe_eddy_current"></a>
### [probe_eddy_current]

Suporte a sondas indutivas de correntes parasitas (eddy current). Pode-se definir esta seção (em vez de uma seção de sonda) para habilitar esta sonda. Consulte a [referência de comandos](G-Codes.md#probe_eddy_current) para mais informações.

```
[probe_eddy_current my_eddy_probe]
sensor_type: ldc1612
#   O chip sensor usado para realizar medições de eddy current. Este parâmetro
#   deve ser fornecido e deve ser definido como ldc1612.
#frequency:
#   A frequência de cristal externo (em Hz) do chip LDC1612. O padrão é
#   12000000.
#intb_pin:
#   Pino gpio do MCU conectado ao pino INTB da sonda ldc1612 (se disponível). O
#   padrão é não usar o pino INTB.
#max_sensor_hz:
#   Frequência ressonante máxima esperada relatada pela sonda (em Hz). Isso é
#   usado durante a configuração interna da taxa de clock. Este valor
#   geralmente só é configurado se o software relatar um aviso sugerindo que o
#   valor deve ser aumentado. O padrão é 5000000.
#descend_z:
#   A distância nominal (em mm) entre o bico e a mesa em que uma tentativa de
#   sondagem deve parar. Este parâmetro deve ser fornecido.
#i2c_address:
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   As configurações i2c para o chip sensor. Consulte a seção "Configurações
#   comuns de I2C" para uma descrição dos parâmetros acima.
#x_offset:
#y_offset:
#   A distância (em mm) entre a sonda e o bico ao longo dos eixos x e y. O
#   padrão é 0.
#speed:
#lift_speed:
#samples:
#sample_retract_dist:
#samples_result:
#samples_tolerance:
#samples_tolerance_retries:
#   Consulte a seção "probe" para informações sobre estes parâmetros. Observe
#   que as configurações aqui se aplicam somente a comandos regulares de sonda.
#   Estas configurações não têm efeito se usar uma "METHOD" de sonda de "scan",
#   "rapid_scan" ou "tap".
#tap_threshold:
#   Limiar de parada de descida (em Hz/mm) para sondagem "tap". Valores maiores
#   reduzem a chance de o cabeçote parar incorretamente cedo devido a ruído,
#   enquanto aumentam o risco de o cabeçote não parar corretamente quando toca
#   primeiro na mesa. Consulte Eddy_Probe.md para mais informações. Este valor
#   pode ser substituído em tempo de execução usando o parâmetro
#   "TAP_THRESHOLD" em comandos de sonda. O padrão é não habilitar sondagem
#   "tap".
#tap_z_offset: 0.0
#   A altura Z (em mm) do bico em relação à mesa no ponto de contato detectado
#   durante sondagem "tap". Nominalmente isso seria 0.0 para indicar que o
#   ponto de contato tem distância zero, mas pode-se definir isso para levar em
#   conta folga, expansão térmica, viés sistemático de sondagem ou similar. O
#   padrão é zero.
```

<a id="axis_twist_compensation"></a>
### [axis_twist_compensation]

Uma ferramenta para compensar leituras imprecisas da sonda devido a torção no pórtico dos eixos X ou Y. Consulte o [Guia de compensação de torção de eixo](Axis_Twist_Compensation.md) para informações mais detalhadas sobre sintomas, configuração e preparação.

```
[axis_twist_compensation]
#speed: 50
#   A velocidade (em mm/s) dos movimentos sem sondagem durante a calibração. O
#   padrão é 50.
#horizontal_move_z: 5
#   A altura (em mm) para a qual o cabeçote deve ser comandado a mover logo
#   antes de iniciar uma operação de sonda. O padrão é 5.
calibrate_start_x: 20
#   Define a coordenada X mínima da calibração. Isso deve ser a coordenada X
#   que posiciona o bico na posição inicial de calibração.
calibrate_end_x: 200
#   Define a coordenada X máxima da calibração. Isso deve ser a coordenada X
#   que posiciona o bico na posição final de calibração.
calibrate_y: 112.5
#   Define a coordenada Y da calibração. Isso deve ser a coordenada Y que
#   posiciona o bico durante o processo de calibração. Recomenda-se que este
#   parâmetro esteja próximo ao centro da mesa.

# Para compensação de torção do eixo Y, especifique os seguintes parâmetros:
calibrate_start_y: ...
#   Define a coordenada Y mínima da calibração. Isso deve ser a coordenada Y
#   que posiciona o bico na posição inicial de calibração para o eixo Y. Este
#   parâmetro deve ser fornecido se compensar torção do eixo Y.
calibrate_end_y: ...
#   Define a coordenada Y máxima da calibração. Isso deve ser a coordenada Y
#   que posiciona o bico na posição final de calibração para o eixo Y. Este
#   parâmetro deve ser fornecido se compensar torção do eixo Y.
calibrate_x: ...
#   Define a coordenada X da calibração para compensação de torção do eixo Y.
#   Isso deve ser a coordenada X que posiciona o bico durante o processo de
#   calibração para compensação de torção do eixo Y. Este parâmetro deve ser
#   fornecido e recomenda-se que esteja próximo ao centro da mesa.
```

<a id="additional-stepper-motors-and-extruders"></a>
## Motores de passo e extrusoras adicionais

<a id="stepper_z1"></a>
### [stepper_z1]

Eixos com múltiplos motores de passo. Em uma impressora estilo cartesiano, o motor de passo que controla um determinado eixo pode ter blocos adicionais de configuração definindo motores de passo que devem ser acionados em conjunto com o motor de passo primário. Pode-se definir qualquer número de seções com sufixo numérico a partir de 1 (por exemplo, "stepper_z1", "stepper_z2", etc.).

```
[stepper_z1]
#step_pin:
#dir_pin:
#enable_pin:
#microsteps:
#rotation_distance:
#   Consulte a seção "stepper" para a definição dos parâmetros acima.
#endstop_pin:
#   Se um endstop_pin estiver definido para o motor de passo adicional, então o
#   motor de passo fará homing (referenciamento) até o fim de curso ser
#   disparado. Caso contrário, o motor de passo fará homing (referenciamento)
#   até o fim de curso no motor de passo primário para o eixo ser disparado.
```

<a id="extruder1"></a>
### [extruder1]

Em uma impressora com múltiplas extrusoras, adicione uma seção adicional de extrusora para cada extrusora adicional. As seções adicionais de extrusora devem ser denominadas "extruder1", "extruder2", "extruder3" e assim por diante. Consulte a seção "extruder" para uma descrição dos parâmetros disponíveis.

Consulte [sample-multi-extruder.cfg](../../config/sample-multi-extruder.cfg) para um exemplo de configuração.

```
[extruder1]
#step_pin:
#dir_pin:
#...
#   Consulte a seção "extruder" para parâmetros disponíveis de motor de passo e
#   aquecedor.
#shared_heater:
#   Esta opção está obsoleta e não deve mais ser especificada.
```

<a id="dual_carriage"></a>
### [dual_carriage]

Suporte a impressoras cartesianas, generic_cartesian e hybrid_corexy/z com carros duplos em um único eixo. O modo do carro pode ser definido via comando estendido de G-code SET_DUAL_CARRIAGE. Por exemplo, o comando "SET_DUAL_CARRIAGE CARRIAGE=1" ativará o carro definido nesta seção (CARRIAGE=0 retornará a ativação para o carro primário). O suporte a carro duplo é normalmente combinado com extrusoras extras - o comando SET_DUAL_CARRIAGE é frequentemente chamado ao mesmo tempo que o comando ACTIVATE_EXTRUDER. Certifique-se de estacionar os carros durante a desativação. Observe que durante o referenciamento G28, normalmente o carro primário é referenciado primeiro, seguido pelo carro definido na seção de configuração `[dual_carriage]`. Entretanto, o carro `[dual_carriage]` será referenciado primeiro se ambos os carros referenciarem em direção positiva e o carro [dual_carriage] tiver um `position_endstop` maior que o carro primário, ou se ambos os carros referenciarem em direção negativa e o carro `[dual_carriage]` tiver um `position_endstop` menor que o carro primário.

Adicionalmente, você pode usar os comandos "SET_DUAL_CARRIAGE CARRIAGE=1 MODE=COPY" ou "SET_DUAL_CARRIAGE CARRIAGE=1 MODE=MIRROR" para ativar tanto o modo de copiar ou de espelhar o carrinho duplo, em ambos os casos ele irá seguir o movimento do carrinho 0 de acordo. Estes comandos podem ser usados para imprimir duas partes simultaneamente - tanto duas partes idênticas (no modo COPY) ou espelhadas (no modo MIRROR). Note que os modos COPY e MIRROR também requerem configuração apropriada da extrusora no carrinho duplo, o que pode ser feito normalmente com o comando  "SYNC_EXTRUDER_MOTION MOTION_QUEUE=extruder EXTRUDER=<dual_carriage_extruder>" ou outro similar.

Consulte [sample-idex.cfg](../../config/sample-idex.cfg) para um exemplo de configuração com cinemática cartesiana convencional.

```
[dual_carriage]
axis:
#   O eixo em que este carro extra está (x ou y). Este parâmetro deve ser
#   fornecido.
#safe_distance:
#   A distância mínima (em mm) a impor entre os carros duplo e primário. Se um
#   comando G-Code for executado que aproximará os carros mais que o limite
#   especificado, tal comando será rejeitado com um erro. Se safe_distance não
#   for fornecido, ele será inferido de position_min e position_max para os
#   carros duplo e primário. Se definido como 0 (ou safe_distance não estiver
#   definido e position_min e position_max forem idênticos para os carros
#   primário e duplo), as verificações de proximidade dos carros serão
#   desabilitadas.
#step_pin:
#dir_pin:
#enable_pin:
#microsteps:
#rotation_distance:
#endstop_pin:
#position_endstop:
#position_min:
#position_max:
#   Consulte a seção "stepper" para a definição dos parâmetros acima.
```

Para um exemplo de configuração de carro duplo com cinemática `generic_cartesian`, consulte a seguinte configuração de [exemplo](../../config/example-generic-caretesian.cfg). Observe que, neste caso, a configuração de `[dual_carriage]` difere da configuração descrita acima:

```
[dual_carriage my_dc_carriage]
#primary_carriage:
#   Define o carro correspondente no mesmo gantry que este carro duplo e o eixo
#   duplo correspondente. Deve corresponder a um nome de um `[carriage]`
#   definido ou outro `[dual_carriage]` independente. Se não definido, que é o
#   padrão, define um carro duplo independente de um `[carriage]` com o mesmo
#   eixo que este (por exemplo, em um gantry diferente).
#axis:
#   Eixo de um carro, x ou y. Se 'primary_carriage' estiver definido, então
#   este parâmetro assume por padrão o parâmetro 'axis' desse carro primário,
#   caso contrário este parâmetro deve ser definido.
#safe_distance:
#   A distância mínima (em mm) a impor entre os carros duplo e primário. Se um
#   comando G-Code for executado que aproxime os carros além do limite
#   especificado, tal comando será rejeitado com um erro. Se safe_distance não
#   for fornecido, ele será inferido a partir de position_min e position_max
#   dos carros duplo e primário. Se definido como 0 (ou se safe_distance não
#   estiver definido e position_min e position_max forem idênticos para os
#   carros primário e duplo), as verificações de proximidade dos carros serão
#   desativadas. Válido somente para um dual_carriage com 'primary_carriage'
#   definido.
endstop_pin:
#position_min:
position_endstop:
position_max:
#homing_speed:
#homing_retract_dist:
#homing_retract_speed:
#second_homing_speed:
#homing_positive_dir:
...
```
Consulte a seção [cartesiano genérico](#generic-cartesian-kinematics) para mais informações sobre os parâmetros regulares de `carriage`.

Em seguida, o usuário deve definir um ou mais motores de passo que movem o carro duplo (e outros carros conforme apropriado), por exemplo

```
[carriage carriage_x]
...

[carriage carriage_y]
...

[dual_carriage carriage_u]
primary_carriage: carriage_x
...

[stepper dc_stepper]
carriages: carriage_u-carriage_y
...
```

`[dual_carriage]` exige configuração especial para o modelador de entrada (input shaper). Em geral, é necessário executar a calibração do modelador de entrada duas vezes - para o `dual_carriage` e seu `primary_carriage` para o eixo que compartilham. Então o modelador de entrada pode ser configurado como segue, assumindo o exemplo acima:

```
[input_shaper]
# Intencionalmente vazio

[delayed_gcode init_shaper]
initial_duration: 0.1
gcode:
  SET_DUAL_CARRIAGE CARRIAGE=carriage_u
  SET_INPUT_SHAPER SHAPER_TYPE_X=<carriage_u_shaper> SHAPER_FREQ_X=<carriage_x_freq> SHAPER_TYPE_Y=<carriage_y_shaper> SHAPER_FREQ_Y=<carriage_y_freq>
  SET_DUAL_CARRIAGE CARRIAGE=carriage_x
  SET_INPUT_SHAPER SHAPER_TYPE_X=<carriage_x_shaper> SHAPER_FREQ_X=<carriage_x_freq> SHAPER_TYPE_Y=<carriage_y_shaper> SHAPER_FREQ_Y=<carriage_y_freq>
```
Observe que `SHAPER_TYPE_Y` e `SHAPER_FREQ_Y` devem ser iguais em ambos os comandos neste caso, pois os mesmos motores acionam o eixo Y quando qualquer um dos carros `carriage_x` e `carriage_u` está ativo.

Vale notar que a cinemática `generic_cartesian` pode suportar dois carros duplos para os eixos X e Y. Para referência, consulte por exemplo uma [amostra](../../config/sample-corexyuv.cfg) de configuração CoreXYUV.

<a id="extruder_stepper"></a>
### [extruder_stepper]

Suporte a motores de passo adicionais sincronizados ao movimento de uma extrusora (pode-se definir qualquer número de seções com o prefixo "extruder_stepper").

Consulte a [referência de comandos](G-Codes.md#extruder) para mais informações.

```
[extruder_stepper my_extra_stepper]
extruder:
#   A extrusora à qual este motor de passo está sincronizado. Se definido como
#   uma string vazia, então o motor de passo não será sincronizado a uma
#   extrusora. Este parâmetro deve ser fornecido.
#step_pin:
#dir_pin:
#enable_pin:
#microsteps:
#rotation_distance:
#   Consulte a seção "stepper" para a definição dos parâmetros acima.
```

<a id="manual_stepper"></a>
### [manual_stepper]

Motores de passo manuais (pode-se definir qualquer número de seções com o prefixo "manual_stepper"). Estes são motores de passo controlados pelo comando G-code MANUAL_STEPPER. Por exemplo: "MANUAL_STEPPER STEPPER=my_stepper MOVE=10 SPEED=5". Consulte o arquivo [G-Codes](G-Codes.md#manual_stepper) para uma descrição do comando MANUAL_STEPPER. Os motores de passo não estão conectados à cinemática normal da impressora.

```
[manual_stepper my_stepper]
#step_pin:
#dir_pin:
#enable_pin:
#microsteps:
#rotation_distance:
#   Consulte a seção "stepper" para a descrição destes parâmetros.
#velocity:
#   Define a velocidade padrão (em mm/s) para o motor de passo. Este valor será
#   usado se um comando MANUAL_STEPPER não especificar um parâmetro SPEED. O
#   padrão é 5mm/s.
#accel:
#   Define a aceleração padrão (em mm/s^2) para o motor de passo. Uma
#   aceleração de zero resultará em nenhuma aceleração. Este valor será usado
#   se um comando MANUAL_STEPPER não especificar um parâmetro ACCEL. O padrão é
#   zero.
#endstop_pin:
#   Pino de detecção da chave de fim de curso. Se especificado, então pode-se
#   realizar movimentos de homing (referenciamento) adicionando um parâmetro
#   STOP_ON_ENDSTOP aos comandos de movimento MANUAL_STEPPER.
#position_min:
#position_max:
#   As posições mínima e máxima para as quais o motor de passo pode ser
#   comandado a mover-se. Se especificadas, então não se pode comandar o motor
#   de passo a mover-se além da posição dada. Observe que estes limites não
#   impedem definir uma posição arbitrária com o `MANUAL_STEPPER
#   SET_POSITION=x`. O padrão é não impor um limite.
```

<a id="custom-heaters-and-sensors"></a>
## Aquecedores e sensores personalizados

<a id="verify_heater"></a>
### [verify_heater]

Verificação de aquecedor e sensor de temperatura. A verificação de aquecedor é habilitada automaticamente para cada aquecedor configurado na impressora. Use seções verify_heater para alterar as configurações padrão.

```
[verify_heater heater_config_name]
#max_error: 120
#   O "erro cumulativo de temperatura" máximo antes de gerar um erro. Valores
#   menores resultam em verificação mais rigorosa e valores maiores permitem
#   mais tempo antes que um erro seja relatado. Especificamente, a temperatura
#   é inspecionada uma vez por segundo e se estiver próxima da temperatura
#   alvo, então um "contador de erros" interno é zerado; caso contrário, se a
#   temperatura estiver abaixo da faixa alvo, então o contador é incrementado
#   pela quantidade em que a temperatura relatada difere dessa faixa. Se o
#   contador exceder este "max_error", então um erro é gerado. O padrão é 120.
#check_gain_time:
#   Isto controla a verificação do aquecedor durante o aquecimento inicial.
#   Valores menores resultam em verificação mais rigorosa e valores maiores
#   permitem mais tempo antes que um erro seja relatado. Especificamente,
#   durante o aquecimento inicial, enquanto o aquecedor aumentar de temperatura
#   dentro deste intervalo de tempo (especificado em segundos), então o
#   "contador de erros" interno é zerado. O padrão é 20 segundos para
#   extrusoras e 60 segundos para heater_bed.
#hysteresis: 5
#   A diferença máxima de temperatura (em Celsius) em relação a uma temperatura
#   alvo que é considerada dentro da faixa do alvo. Isto controla a verificação
#   de faixa de max_error. É raro personalizar este valor. O padrão é 5.
#heating_gain: 2
#   A temperatura mínima (em Celsius) que o aquecedor deve aumentar durante a
#   verificação check_gain_time. É raro personalizar este valor. O padrão é 2.
```

<a id="homing_heaters"></a>
### [homing_heaters]

Ferramenta para desabilitar aquecedores ao referenciar (fazer homing) ou sondar um eixo.

```
[homing_heaters]
#steppers:
#   Uma lista separada por vírgulas de motores de passo que devem fazer com que
#   os aquecedores sejam desativados. O padrão é desativar os aquecedores para
#   qualquer movimento de homing (referenciamento)/sondagem. Exemplo típico:
#   stepper_z
#heaters:
#   Uma lista separada por vírgulas de aquecedores a desativar durante
#   movimentos de homing (referenciamento)/sondagem. O padrão é desativar todos
#   os aquecedores. Exemplo típico: extruder, heater_bed
```

<a id="thermistor"></a>
### [thermistor]

Termistores personalizados (pode-se definir qualquer número de seções com o prefixo "thermistor"). Um termistor personalizado pode ser usado no campo sensor_type de uma seção de configuração de aquecedor. (Por exemplo, se for definida uma seção "[thermistor my_thermistor]", pode-se usar um "sensor_type: my_thermistor" ao definir um aquecedor.) Certifique-se de colocar a seção do termistor no arquivo de configuração acima de seu primeiro uso em uma seção de aquecedor.

```
[thermistor my_thermistor]
#temperature1:
#resistance1:
#temperature2:
#resistance2:
#temperature3:
#resistance3:
#   Três medições de resistência (em Ohms) nas temperaturas dadas (em Celsius).
#   As três medições serão usadas para calcular os coeficientes de
#   Steinhart-Hart para o termistor. Estes parâmetros devem ser fornecidos ao
#   usar Steinhart-Hart para definir o termistor.
#beta:
#   Alternativamente, pode-se definir temperature1, resistance1 e beta para
#   definir os parâmetros do termistor. Este parâmetro deve ser fornecido ao
#   usar "beta" para definir o termistor.
```

<a id="adc_temperature"></a>
### [adc_temperature]

Sensores personalizados de temperatura ADC (pode-se definir qualquer número de seções com o prefixo "adc_temperature"). Isso permite definir um sensor personalizado de temperatura que mede uma tensão em um pino de conversor analógico-digital (ADC) e usa interpolação linear entre um conjunto configurado de medições de temperatura/tensão (ou temperatura/resistência) para determinar a temperatura. O sensor resultante pode ser usado como sensor_type em uma seção de aquecedor. (Por exemplo, se for definida uma seção "[adc_temperature my_sensor]", pode-se usar um "sensor_type: my_sensor" ao definir um aquecedor.) Certifique-se de colocar a seção do sensor no arquivo de configuração acima de seu primeiro uso em uma seção de aquecedor.

```
[adc_temperature my_sensor]
#temperature1:
#voltage1:
#temperature2:
#voltage2:
#...
#   Um conjunto de temperaturas (em Celsius) e tensões (em Volts) a usar como
#   referência ao converter uma temperatura. Uma seção de aquecedor que usa
#   este sensor também pode especificar parâmetros adc_voltage e voltage_offset
#   para definir a tensão do ADC (consulte a seção "Amplificadores comuns de
#   temperatura" para detalhes). Pelo menos duas medições devem ser fornecidas.
#temperature1:
#resistance1:
#temperature2:
#resistance2:
#...
#   Alternativamente pode-se especificar um conjunto de temperaturas (em
#   Celsius) e resistência (em Ohms) a usar como referência ao converter uma
#   temperatura. Uma seção de aquecedor que usa este sensor também pode
#   especificar um parâmetro pullup_resistor (consulte a seção "extruder" para
#   detalhes). Pelo menos duas medições devem ser fornecidas.
```

<a id="heater_generic"></a>
### [heater_generic]

Aquecedores genéricos (pode-se definir qualquer número de seções com o prefixo "heater_generic"). Estes aquecedores comportam-se de forma semelhante aos aquecedores padrão (extrusoras, mesas aquecidas). Use o comando SET_HEATER_TEMPERATURE (consulte [G-Codes](G-Codes.md#heaters) para detalhes) para definir a temperatura alvo.

```
[heater_generic my_generic_heater]
#gcode_id:
#   O id a usar ao relatar a temperatura no comando M105. Este parâmetro deve
#   ser fornecido.
#heater_pin:
#max_power:
#sensor_type:
#sensor_pin:
#smooth_time:
#control:
#pid_Kp:
#pid_Ki:
#pid_Kd:
#pwm_cycle_time:
#min_temp:
#max_temp:
#   Consulte a seção "extruder" para a definição dos parâmetros acima.
```

<a id="temperature_sensor"></a>
### [temperature_sensor]

Sensores genéricos de temperatura. Pode-se definir qualquer número de sensores adicionais de temperatura que são relatados via comando M105.

```
[temperature_sensor my_sensor]
#sensor_type:
#sensor_pin:
#min_temp:
#max_temp:
#   Consulte a seção "extruder" para a definição dos parâmetros acima.
#gcode_id:
#   Consulte a seção "heater_generic" para a definição deste parâmetro.
```

<a id="temperature_probe"></a>
### [temperature_probe]

Relata a temperatura da bobina da sonda. Inclui calibração opcional de deriva térmica para sondas baseadas em correntes parasitas (eddy current). Uma seção `[temperature_probe]` pode ser vinculada a uma `[probe_eddy_current]` usando o mesmo sufixo para ambas as seções.

```
[temperature_probe my_probe]
#sensor_type:
#sensor_pin:
#min_temp:
#max_temp:
#   Configuração do sensor de temperatura. Consulte a seção "extruder" para a
#   definição dos parâmetros acima.
#smooth_time:
#   Um valor de tempo (em segundos) durante o qual as medições de temperatura
#   serão suavizadas para reduzir o impacto do ruído de medição. O padrão é 2.0
#   segundos.
#gcode_id:
#   Consulte a seção "heater_generic" para a definição deste parâmetro.
#speed:
#   A velocidade de deslocamento [mm/s] para movimentos xy durante a
#   calibração. O padrão é a velocidade definida pela sonda.
#horizontal_move_z:
#   A distância z [mm] da mesa na qual ocorrerão movimentos xy durante a
#   calibração. O padrão é 2mm.
#resting_z:
#   A distância z [mm] da mesa na qual a ferramenta repousará para aquecer a
#   bobina da sonda durante a calibração. O padrão é .4mm
#calibration_position:
#   A posição X, Y, Z para onde a ferramenta deve ser movida quando a
#   calibração de deriva da sonda for inicializada. Este é o local onde
#   ocorrerá a primeira sondagem manual. Se omitido, o comportamento padrão é
#   não mover a ferramenta antes da primeira sondagem manual.
#calibration_bed_temp:
#   A temperatura máxima segura da mesa (em C) usada para aquecer a sonda
#   durante a calibração de deriva da sonda. Quando definido, o procedimento de
#   calibração ligará a mesa após a primeira amostra ser coletada. Quando o
#   procedimento de calibração estiver concluído, a temperatura da mesa será
#   definida como zero. Quando omitido, o comportamento padrão é não definir a
#   temperatura da mesa.
#calibration_extruder_temp:
#   A temperatura da extrusora (em C) definida para a sonda durante a
#   calibração de deriva. Quando esta opção é fornecida, o procedimento
#   aguardará até que a temperatura especificada seja atingida antes de
#   solicitar a primeira sondagem manual. Quando o procedimento de calibração
#   estiver concluído, a temperatura da extrusora será definida como 0. Quando
#   omitido, o comportamento padrão é não definir a temperatura da extrusora.
#extruder_heating_z: 50.
#   O local Z onde ocorrerá o aquecimento da extrusora se a opção
#   "calibration_extruder_temp" estiver definida. Recomenda-se aquecer a
#   extrusora a alguma distância da mesa para minimizar seu impacto na
#   temperatura da bobina da sonda. O padrão é 50.
#max_validation_temp: 60.
#   A temperatura máxima usada para validar a calibração. Recomenda-se definir
#   isto com um valor entre 100 e 120 para impressoras fechadas. O padrão é 60.
```

<a id="temperature-sensors"></a>
## Sensores de temperatura

O Klipper inclui definições para muitos tipos de sensores de temperatura. Estes sensores podem ser usados em qualquer seção de configuração que exija um sensor de temperatura (como uma seção `[extruder]` ou `[heater_bed]`).

<a id="common-thermistors"></a>
### Termistores comuns

Termistores comuns. Os seguintes parâmetros estão disponíveis nas secções dos aquecedores que utilizam um desses sensores.

```
sensor_type:
#   Um de "EPCOS 100K B57560G104F", "ATC Semitec 104GT-2", "ATC Semitec
#   104NT-4-R025H42G", "Generic 3950", "Honeywell 100K 135-104LAG-J01", "NTC
#   100K MGB18-104F39050L32", "SliceEngineering 450" ou "TDK NTCG104LH104JT1"
sensor_pin:
#   Pino de entrada analógica conectado ao termistor. Este parâmetro deve ser
#   fornecido.
#pullup_resistor: 4700
#   A resistência (em ohms) do pullup ligado ao termistor. O padrão é 4700
#   ohms.
#inline_resistor: 0
#   A resistência (em ohms) de um resistor extra (que não varia com o calor)
#   que é colocado em linha com o termistor. É raro definir isto. O padrão é 0
#   ohms.
```

<a id="common-temperature-amplifiers"></a>
### Amplificadores de temperatura comuns

Amplificadores de temperatura comuns. Os seguintes parâmetros estão disponíveis nas seções de aquecedor que utilizam um desses sensores.

```
sensor_type:
#   Um de "PT100 INA826", "AD595", "AD597", "AD8494", "AD8495", "AD8496" ou
#   "AD8497".
sensor_pin:
#   Pino de entrada analógica conectado ao sensor. Este parâmetro deve ser
#   fornecido.
#adc_voltage: 5.0
#   A tensão de comparação do ADC (em Volts). O padrão é 5 volts.
#voltage_offset: 0
#   O offset de tensão do ADC (em Volts). O padrão é 0.
```

<a id="directly-connected-pt1000-sensor"></a>
### Sensor PT1000 conectado diretamente

Sensor PT1000 conectado diretamente. Os seguintes parâmetros estão disponíveis em seções de aquecedor que usam um destes sensores.

```
sensor_type: PT1000
sensor_pin:
#   Pino de entrada analógica conectado ao sensor. Este parâmetro deve ser
#   fornecido.
#pullup_resistor: 4700
#   A resistência (em ohms) do pullup ligado ao sensor. O padrão é 4700 ohms.
```

<a id="maxxxxxx-temperature-sensors"></a>
### Sensores de temperatura MAXxxxxx

Sensores baseados em temperatura com interface periférica serial (SPI) MAXxxxxx. Os seguintes parâmetros estão disponíveis em seções de aquecedor que usam um destes tipos de sensor.

```
sensor_type:
#   Um de "MAX6675", "MAX31855", "MAX31856" ou "MAX31865".
sensor_pin:
#   A linha de chip select para o chip do sensor. Este parâmetro deve ser
#   fornecido.
#spi_speed: 4000000
#   A velocidade SPI (em hz) a usar ao comunicar-se com o chip. O padrão é
#   4000000.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para a descrição dos
#   parâmetros acima.
#tc_type: K
#tc_use_50Hz_filter: False
#tc_averaging_count: 1
#   Os parâmetros acima controlam os parâmetros de sensor dos chips MAX31856.
#   Os padrões para cada parâmetro estão ao lado do nome do parâmetro na lista
#   acima.
#rtd_nominal_r: 100
#rtd_reference_r: 430
#rtd_num_of_wires: 2
#rtd_use_50Hz_filter: False
#   Os parâmetros acima controlam os parâmetros de sensor dos chips MAX31865.
#   Os padrões para cada parâmetro estão ao lado do nome do parâmetro na lista
#   acima.
```

<a id="bmp180bmp280bme280bmp388bme680-temperature-sensor"></a>
### Sensor de temperatura BMP180/BMP280/BME280/BMP388/BME680

Sensores ambientais com interface de dois fios (I2C) BMP180/BMP280/BME280/BMP388/BME680. Observe que estes sensores não se destinam ao uso com extrusoras e mesas aquecidas, mas sim para monitorar temperatura ambiente (C), pressão (hPa), umidade relativa e, no caso do BME680, nível de gás. Consulte [sample-macros.cfg](../../config/sample-macros.cfg) para uma gcode_macro que pode ser usada para relatar pressão e umidade além da temperatura.

```
sensor_type: BME280
#i2c_address:
#   O padrão é 118 (0x76). Os sensores BMP180, BMP388 e alguns sensores BME280
#   têm endereço 119 (0x77).
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
```

<a id="aht10aht20aht21-temperature-sensor"></a>
### Sensor de temperatura AHT10/AHT20/AHT21

Sensores ambientais com interface de dois fios (I2C) AHT10/AHT15/AHT20/AHT21/AHT30. Observe que estes sensores não se destinam ao uso com extrusoras e mesas aquecidas, mas sim para monitorar temperatura ambiente (C) e umidade relativa. Consulte [sample-macros.cfg](../../config/sample-macros.cfg) para uma gcode_macro que pode ser usada para relatar umidade além da temperatura.

```
sensor_type: AHT1X
#   Deve ser "AHT1X", "AHT2X", "AHT3X". Alguns sensores AHT20 podem usar
#   "AHT1X"
#i2c_address:
#   O padrão é 56 (0x38). Alguns sensores AHT10 oferecem a opção de usar 57
#   (0x39) movendo um resistor.
#i2c_mcu:
#i2c_bus:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
#aht10_report_time:
#   Intervalo em segundos entre leituras. O padrão é 30, o mínimo é 5
```

<a id="htu21d-sensor"></a>
### Sensor HTU21D

Sensor ambiental com interface de dois fios (I2C) da família HTU21D. Observe que este sensor não se destina ao uso com extrusoras e mesas aquecidas, mas sim para monitorar temperatura ambiente (C) e umidade relativa. Consulte [sample-macros.cfg](../../config/sample-macros.cfg) para uma gcode_macro que pode ser usada para relatar umidade além da temperatura.

```
sensor_type:
#   Deve ser "HTU21D", "SI7013", "SI7020", "SI7021" ou "SHT21"
#i2c_address:
#   O padrão é 64 (0x40).
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
#htu21d_hold_master:
#   Se o sensor pode segurar o barramento I2C durante a leitura. Se True,
#   nenhuma outra comunicação do barramento pode ser realizada enquanto a
#   leitura estiver em andamento. O padrão é False.
#htu21d_resolution:
#   A resolução da leitura de temperatura e umidade. Valores válidos são:
#   'TEMP14_HUM12' -> 14bit para Temp e 12bit para umidade 'TEMP13_HUM10' ->
#   13bit para Temp e 10bit para umidade 'TEMP12_HUM08' -> 12bit para Temp e
#   08bit para umidade 'TEMP11_HUM11' -> 11bit para Temp e 11bit para umidade.
#   O padrão é: "TEMP11_HUM11"
#htu21d_report_time:
#   Intervalo em segundos entre leituras. O padrão é 30
```

<a id="sht3x-sensor"></a>
### Sensor SHT3X

Sensor ambiental com interface de dois fios (I2C) da família SHT3X. Estes sensores têm faixa de -55~125 C, portanto são utilizáveis, por exemplo, para monitoramento de temperatura da câmara. Eles também podem funcionar como controladores simples de ventilador/aquecedor.

```
sensor_type: SHT3X
#i2c_address:
#   O padrão é 68 (0x44).
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
```

<a id="lm75-temperature-sensor"></a>
### Sensor de temperatura LM75

Sensores de temperatura LM75/LM75A conectados por dois fios (I2C). Estes sensores têm faixa de -55~125 C, portanto são utilizáveis, por exemplo, para monitoramento de temperatura da câmara. Eles também podem funcionar como controladores simples de ventilador/aquecedor.

```
sensor_type: LM75
#i2c_address:
#   O padrão é 72 (0x48). A faixa normal é 72-79 (0x48-0x4F) e os 3 bits baixos
#   do endereço são configurados via pinos no chip (geralmente com jumpers ou
#   ligados de forma fixa).
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
#lm75_report_time:
#   Intervalo em segundos entre leituras. O padrão é 0.8, com mínimo de 0.5.
```

<a id="builtin-micro-controller-temperature-sensor"></a>
### Sensor de temperatura embutido no microcontrolador

Os microcontroladores atsam, atsamd, stm32 e rp2040 contêm um sensor interno de temperatura. Pode-se usar o sensor "temperature_mcu" para monitorar estas temperaturas.

```
sensor_type: temperature_mcu
#sensor_mcu: mcu
#   O microcontrolador do qual ler. O padrão é "mcu".
#sensor_temperature1:
#sensor_adc1:
#   Especifique os dois parâmetros acima (uma temperatura em Celsius e um valor
#   ADC como float entre 0.0 e 1.0) para calibrar a temperatura do
#   microcontrolador. Isto pode melhorar a precisão da temperatura relatada em
#   alguns chips. Uma maneira típica de obter estas informações de calibração é
#   remover completamente a energia da impressora por algumas horas (para
#   garantir que esteja na temperatura ambiente), então ligá-la e usar o
#   comando QUERY_ADC para obter uma medição ADC. Use algum outro sensor de
#   temperatura na impressora para encontrar a temperatura ambiente
#   correspondente. O padrão é usar os dados de calibração de fábrica no
#   microcontrolador (se aplicável) ou os valores nominais da especificação do
#   microcontrolador.
#sensor_temperature2:
#sensor_adc2:
#   Se sensor_temperature1/sensor_adc1 for especificado, então também se pode
#   especificar dados de calibração sensor_temperature2/sensor_adc2. Fazer isso
#   pode fornecer informações calibradas de "inclinação de temperatura". O
#   padrão é usar os dados de calibração de fábrica no microcontrolador (se
#   aplicável) ou os valores nominais da especificação do microcontrolador.
```

<a id="host-temperature-sensor"></a>
### Sensor de temperatura do hospedeiro (host)

Temperatura da máquina (ex.: Raspberry Pi) que executa o software hospedeiro (host).

```
sensor_type: temperature_host
#sensor_path:
#   O caminho para o arquivo de sistema de temperatura. O padrão é
#   "/sys/class/thermal/thermal_zone0/temp", que é o arquivo de sistema de
#   temperatura em um computador Raspberry Pi.
```

<a id="ds18b20-temperature-sensor"></a>
### Sensor de temperatura DS18B20

DS18B20 é um sensor digital de temperatura de 1 fio (w1). Observe que este sensor não se destina ao uso com extrusoras e mesas aquecidas, mas sim para monitorar temperatura ambiente (C). Estes sensores têm faixa de até 125 C, portanto são utilizáveis, por exemplo, para monitoramento de temperatura da câmara. Eles também podem funcionar como controladores simples de ventilador/aquecedor. Sensores DS18B20 são suportados apenas no "host mcu", ex.: Raspberry Pi. O módulo do kernel Linux w1-gpio deve estar instalado.

```
sensor_type: DS18B20
serial_no:
#   Cada dispositivo 1-wire tem um número de série único usado para identificar
#   o dispositivo, geralmente no formato 28-031674b175ff. Este parâmetro deve
#   ser fornecido. Dispositivos 1-wire conectados podem ser listados usando o
#   seguinte comando Linux: ls /sys/bus/w1/devices/
#ds18_report_time:
#   Intervalo em segundos entre leituras. O padrão é 3.0, com mínimo de 1.0
#sensor_mcu:
#   O microcontrolador do qual ler. Deve ser o host_mcu
```

<a id="combined-temperature-sensor"></a>
### Sensor de temperatura combinado

O sensor de temperatura combinado é um sensor de temperatura virtual baseado em vários outros sensores. Este sensor pode ser utilizado com extrusoras, heater_generic e mesas aquecidas.

```
sensor_type: temperature_combined
#sensor_list:
#   Deve ser fornecido. Lista de sensores a combinar no novo sensor "virtual".
#   Por ex. 'temperature_sensor sensor1,extruder,heater_bed'
#combination_method:
#   Deve ser fornecido. Método de combinação usado para o sensor. As opções
#   disponíveis são 'max', 'min', 'mean'.
#maximum_deviation:
#   Deve ser fornecido. Desvio máximo permissível entre os sensores a combinar
#   (por ex. 5 graus). Para desativá-lo, use um valor grande (por ex. 999.9)
```

<a id="fans"></a>
## Ventiladores

<a id="fan"></a>
### [fan]

Ventilador de resfriamento de impressão.

```
[fan]
pin:
#   Pino de saída que controla o ventilador. Este parâmetro deve ser fornecido.
#max_power: 1.0
#   A potência máxima (expressa como um valor de 0.0 a 1.0) com a qual o pino
#   pode ser configurado. O valor 1.0 permite que o pino seja configurado
#   totalmente ativado por períodos prolongados, enquanto um valor de 0.5
#   permitiria que o pino fosse ativado por não mais que metade do tempo. Esta
#   configuração pode ser usada para limitar a potência total de saída (por
#   períodos prolongados) para o ventilador. Se este valor for menor que 1.0,
#   então as solicitações de velocidade do ventilador serão escalonadas entre
#   zero e max_power (por exemplo, se max_power for .9 e uma velocidade de
#   ventilador de 80% for solicitada, então a potência do ventilador será
#   definida como 72%). O padrão é 1.0.
#shutdown_speed: 0
#   A velocidade desejada do ventilador (expressa como um valor de 0.0 a 1.0)
#   se o software do microcontrolador entrar em estado de erro. O padrão é 0.
#cycle_time: 0.010
#   A quantidade de tempo (em segundos) para cada ciclo de potência PWM para o
#   ventilador. Recomenda-se que seja 10 milissegundos ou maior ao usar PWM
#   baseado em software. O padrão é 0.010 segundos.
#hardware_pwm: False
#   Ative isto para usar PWM por hardware em vez de PWM por software. A maioria
#   dos ventiladores não funciona bem com PWM por hardware, portanto não se
#   recomenda ativar isto a menos que haja um requisito elétrico para comutar
#   em velocidades muito altas. Ao usar PWM por hardware, o tempo de ciclo real
#   é limitado pela implementação e pode ser significativamente diferente do
#   cycle_time solicitado. O padrão é False.
#kick_start_time: 0.100
#   Tempo (em segundos) para acionar o ventilador em velocidade máxima ao
#   ativá-lo pela primeira vez ou ao aumentá-lo em mais de 50% (ajuda a fazer o
#   ventilador girar). O padrão é 0.100 segundos.
#off_below: 0.0
#   A velocidade mínima de entrada que alimentará o ventilador (expressa como
#   um valor de 0.0 a 1.0). Quando uma velocidade menor que off_below for
#   solicitada, o ventilador será desligado. Esta configuração pode ser usada
#   para evitar paradas do ventilador e para garantir que partidas forçadas
#   sejam eficazes. O padrão é 0.0.
#
#   Esta configuração deve ser recalibrada sempre que max_power for ajustado.
#   Para calibrar esta configuração, comece com off_below definido como 0.0 e o
#   ventilador girando. Reduza gradualmente a velocidade do ventilador para
#   determinar a velocidade mínima de entrada que aciona o ventilador de forma
#   confiável sem paradas. Defina off_below como o ciclo de trabalho
#   correspondente a este valor (por exemplo, 12% -> 0.12) ou ligeiramente
#   maior.
#tachometer_pin:
#   Pino de entrada do tacômetro para monitorar a velocidade do ventilador.
#   Geralmente é necessário um pullup. Este parâmetro é opcional.
#tachometer_ppr: 2
#   Quando tachometer_pin é especificado, este é o número de pulsos por
#   revolução do sinal do tacômetro. Para um ventilador BLDC isto é normalmente
#   metade do número de polos. O padrão é 2.
#tachometer_poll_interval: 0.0015
#   Quando tachometer_pin é especificado, este é o período de sondagem do pino
#   do tacômetro, em segundos. O padrão é 0.0015, que é rápido o suficiente
#   para ventiladores abaixo de 10000 RPM a 2 PPR. Isto deve ser menor que
#   30/(tachometer_ppr*rpm), com alguma margem, onde rpm é a velocidade máxima
#   (em RPM) do ventilador.
#enable_pin:
#   Pino opcional para ativar a energia do ventilador. Isto pode ser útil para
#   ventiladores com entradas PWM dedicadas. Alguns destes ventiladores
#   permanecem ligados mesmo com entrada PWM de 0%. Nesse caso, o pino PWM pode
#   ser usado normalmente, e por ex. um FET comutado pelo terra (pino de
#   ventilador padrão) pode ser usado para controlar a energia do ventilador.
```

<a id="heater_fan"></a>
### [heater_fan]

Ventiladores de resfriamento do aquecedor (pode-se definir qualquer número de seções com o prefixo "heater_fan"). Um "heater fan" é um ventilador que será ligado sempre que seu aquecedor associado estiver ativo. Por padrão, um heater_fan tem shutdown_speed igual a max_power.

```
[heater_fan heatbreak_cooling_fan]
#pin:
#max_power:
#shutdown_speed:
#cycle_time:
#hardware_pwm:
#kick_start_time:
#off_below:
#tachometer_pin:
#tachometer_ppr:
#tachometer_poll_interval:
#enable_pin:
#   Consulte a seção "fan" para a descrição dos parâmetros acima.
#heater: extruder
#   Nome da seção de configuração que define o aquecedor ao qual este
#   ventilador está associado. Se uma lista separada por vírgulas de nomes de
#   aquecedores for fornecida aqui, então o ventilador será ativado quando
#   qualquer um dos aquecedores dados estiver ativado. O padrão é "extruder".
#heater_temp: 50.0
#   Uma temperatura (em Celsius) abaixo da qual o aquecedor deve cair antes que
#   o ventilador seja desativado. O padrão é 50 Celsius.
#fan_speed: 1.0
#   A velocidade do ventilador (expressa como um valor de 0.0 a 1.0) com a qual
#   o ventilador será configurado quando seu aquecedor associado estiver
#   ativado. O padrão é 1.0
```

<a id="controller_fan"></a>
### [controller_fan]

Controlador de ventoinha de refrigeração (pode-se definir quaisquer número de secções com o prefixo "controller_fan"). Um "controlador de ventoinha" é uma ventoinha que será ativada sempre que um aquecedor ou driver de motor de passo é ativado. A Ventoinha irá desativada sempre que o tempo idle_timeout for atingido para garantir que nenhum superaquecimento irá acontecer após a desativação do componente sendo monitorado.

```
[controller_fan my_controller_fan]
#pin:
#max_power:
#shutdown_speed:
#cycle_time:
#hardware_pwm:
#kick_start_time:
#off_below:
#tachometer_pin:
#tachometer_ppr:
#tachometer_poll_interval:
#enable_pin:
#   Consulte a seção "fan" para a descrição dos parâmetros acima.
#fan_speed: 1.0
#   A velocidade do ventilador (expressa como um valor de 0.0 a 1.0) com a qual
#   o ventilador será configurado quando um aquecedor ou driver de motor de
#   passo estiver ativo. O padrão é 1.0
#idle_timeout:
#   A quantidade de tempo (em segundos) após um driver de motor de passo ou
#   aquecedor ter estado ativo durante a qual o ventilador deve ser mantido em
#   funcionamento. O padrão é 30 segundos.
#idle_speed:
#   A velocidade do ventilador (expressa como um valor de 0.0 a 1.0) com a qual
#   o ventilador será configurado quando um aquecedor ou driver de motor de
#   passo esteve ativo e antes que idle_timeout seja alcançado. O padrão é
#   fan_speed.
#heater:
#stepper:
#   Nome da seção de configuração que define o aquecedor/motor de passo ao qual
#   este ventilador está associado. Se uma lista separada por vírgulas de nomes
#   de aquecedores/motores de passo for fornecida aqui, então o ventilador será
#   ativado quando qualquer um dos aquecedores/motores de passo dados estiver
#   ativado. O aquecedor padrão é "extruder", o motor de passo padrão são todos
#   eles.
```

<a id="temperature_fan"></a>
### [temperature_fan]

Ventiladores de resfriamento acionados por temperatura (pode-se definir qualquer número de seções com o prefixo "temperature_fan"). Um "temperature fan" é um ventilador que será ligado sempre que seu sensor associado estiver acima de uma temperatura definida. Por padrão, um temperature_fan tem shutdown_speed igual a max_power.

Consulte a [referência de comandos](G-Codes.md#temperature_fan) para informações adicionais.

```
[temperature_fan my_temp_fan]
#pin:
#max_power:
#shutdown_speed:
#cycle_time:
#hardware_pwm:
#kick_start_time:
#off_below:
#tachometer_pin:
#tachometer_ppr:
#tachometer_poll_interval:
#enable_pin:
#   Consulte a seção "fan" para a descrição dos parâmetros acima.
#sensor_type:
#sensor_pin:
#control:
#max_delta:
#min_temp:
#max_temp:
#   Consulte a seção "extruder" para a descrição dos parâmetros acima.
#pid_Kp:
#pid_Ki:
#pid_Kd:
#   As configurações proporcional (pid_Kp), integral (pid_Ki) e derivativa
#   (pid_Kd) para o sistema de controle por realimentação PID. O Klipper avalia
#   as configurações PID com a seguinte fórmula geral:
#     fan_pwm = max_power - (Kp*e + Ki*integral(e) - Kd*derivative(e)) / 255
#   Onde "e" é "target_temperature - measured_temperature" e "fan_pwm" é a taxa
#   solicitada do ventilador com 0.0 sendo totalmente desligado e 1.0 sendo
#   totalmente ligado. Os parâmetros pid_Kp, pid_Ki e pid_Kd devem ser
#   fornecidos quando o algoritmo de controle PID estiver ativado.
#pid_deriv_time: 2.0
#   Um valor de tempo (em segundos) durante o qual as medições de temperatura
#   serão suavizadas ao usar o algoritmo de controle PID. Isto pode reduzir o
#   impacto do ruído de medição. O padrão é 2 segundos.
#target_temp: 40.0
#   Uma temperatura (em Celsius) que será a temperatura alvo. O padrão é 40
#   graus.
#max_speed: 1.0
#   A velocidade do ventilador (expressa como um valor de 0.0 a 1.0) com a qual
#   o ventilador será configurado quando a temperatura do sensor exceder o
#   valor definido. O padrão é 1.0.
#min_speed: 0.3
#   A velocidade mínima do ventilador (expressa como um valor de 0.0 a 1.0) com
#   a qual o ventilador será configurado para ventiladores de temperatura PID.
#   O padrão é 0.3.
#gcode_id:
#   Se definido, a temperatura será relatada em consultas M105 usando o id
#   dado. O padrão é não relatar a temperatura via M105.
```

<a id="fan_generic"></a>
### [fan_generic]

Ventilador controlado manualmente (pode-se definir qualquer número de seções com o prefixo "fan_generic"). A velocidade de um ventilador controlado manualmente é definida com o [comando](G-Codes.md#fan_generic) G-code SET_FAN_SPEED.

```
[fan_generic extruder_partfan]
#pin:
#max_power:
#shutdown_speed:
#cycle_time:
#hardware_pwm:
#kick_start_time:
#off_below:
#tachometer_pin:
#tachometer_ppr:
#tachometer_poll_interval:
#enable_pin:
#   Consulte a seção "fan" para a descrição dos parâmetros acima.
```

<a id="leds"></a>
## LEDs

<a id="led"></a>
### [led]

Suporte a LEDs (e fitas de LED) controlados via pinos PWM do microcontrolador (pode-se definir qualquer número de seções com o prefixo "led"). Consulte a [referência de comandos](G-Codes.md#led) para mais informações.

```
[led my_led]
#red_pin:
#green_pin:
#blue_pin:
#white_pin:
#   O pino que controla a cor dada do LED. Pelo menos um dos parâmetros acima
#   deve ser fornecido.
#cycle_time: 0.010
#   A quantidade de tempo (em segundos) por ciclo PWM. Recomenda-se que seja 10
#   milissegundos ou maior ao usar PWM baseado em software. O padrão é 0.010
#   segundos.
#hardware_pwm: False
#   Ative isto para usar PWM por hardware em vez de PWM por software. Ao usar
#   PWM por hardware, o tempo de ciclo real é limitado pela implementação e
#   pode ser significativamente diferente do cycle_time solicitado. O padrão é
#   False.
#initial_RED: 0.0
#initial_GREEN: 0.0
#initial_BLUE: 0.0
#initial_WHITE: 0.0
#   Define a cor inicial do LED. Cada valor deve estar entre 0.0 e 1.0. O
#   padrão para cada cor é 0.
```

<a id="neopixel"></a>
### [neopixel]

Suporte a LED Neopixel (também conhecido como WS2812) (pode-se definir qualquer número de seções com o prefixo "neopixel"). Consulte a [referência de comandos](G-Codes.md#led) para mais informações.

Observe que a implementação do [MCU Linux](RPi_microcontroller.md) atualmente não suporta neopixels conectados diretamente. O projeto atual usando a interface do kernel Linux não permite este cenário porque a interface GPIO do kernel não é rápida o suficiente para fornecer as taxas de pulso necessárias.

```
[neopixel my_neopixel]
pin:
#   O pino conectado ao neopixel. Este parâmetro deve ser fornecido.
#chain_count:
#   O número de chips Neopixel que estão "encadeados em série" ao pino
#   fornecido. O padrão é 1 (o que indica que apenas um único Neopixel está
#   conectado ao pino).
#color_order: GRB
#   Define a ordem de pixels exigida pelo hardware LED (usando uma string
#   contendo as letras R, G, B, W com W opcional). Alternativamente, isto pode
#   ser uma lista separada por vírgulas de ordens de pixels - uma para cada LED
#   na cadeia. O padrão é GRB.
#initial_RED: 0.0
#initial_GREEN: 0.0
#initial_BLUE: 0.0
#initial_WHITE: 0.0
#   Consulte a seção "led" para informações sobre estes parâmetros.
```

<a id="dotstar"></a>
### [dotstar]

Suporte a LED Dotstar (também conhecido como APA102) (pode-se definir qualquer número de seções com o prefixo "dotstar"). Consulte a [referência de comandos](G-Codes.md#led) para mais informações.

```
[dotstar my_dotstar]
data_pin:
#   O pino conectado à linha de dados do dotstar. Este parâmetro deve ser
#   fornecido.
clock_pin:
#   O pino conectado à linha de clock do dotstar. Este parâmetro deve ser
#   fornecido.
#chain_count:
#   Consulte a seção "neopixel" para informações sobre este parâmetro.
#initial_RED: 0.0
#initial_GREEN: 0.0
#initial_BLUE: 0.0
#   Consulte a seção "led" para informações sobre estes parâmetros.
```

<a id="pca9533"></a>
### [pca9533]

Suporte a LED PCA9533. O PCA9533 é usado na mightyboard.

```
[pca9533 my_pca9533]
#i2c_address: 98
#   O endereço i2c que o chip está usando no barramento i2c. Use 98 para o
#   PCA9533/1, 99 para o PCA9533/2. O padrão é 98.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
#initial_RED: 0.0
#initial_GREEN: 0.0
#initial_BLUE: 0.0
#initial_WHITE: 0.0
#   Consulte a seção "led" para informações sobre estes parâmetros.
```

<a id="pca9632"></a>
### [pca9632]

Suporte a LED PCA9632. O PCA9632 é usado na FlashForge Dreamer.

```
[pca9632 my_pca9632]
#i2c_address: 98
#   O endereço i2c que o chip está usando no barramento i2c. Pode ser 96, 97,
#   98 ou 99. O padrão é 98.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para a descrição dos
#   parâmetros acima.
#color_order: RGBW
#   Define a ordem de pixels do LED (usando uma string contendo as letras R, G,
#   B, W). O padrão é RGBW.
#initial_RED: 0.0
#initial_GREEN: 0.0
#initial_BLUE: 0.0
#initial_WHITE: 0.0
#   Consulte a seção "led" para informações sobre estes parâmetros.
```

<a id="additional-servos-buttons-and-other-pins"></a>
## Servos, botões e outros pinos adicionais

<a id="servo"></a>
### [servo]

Servos (pode-se definir qualquer número de seções com o prefixo "servo"). Os servos podem ser controlados usando o [comando](G-Codes.md#servo) G-code SET_SERVO. Por exemplo: SET_SERVO SERVO=my_servo ANGLE=180

```
[servo my_servo]
pin:
#   Pino de saída PWM que controla o servo. Este parâmetro deve ser fornecido.
#maximum_servo_angle: 180
#   O ângulo máximo (em graus) com o qual este servo pode ser configurado. O
#   padrão é 180 graus.
#minimum_pulse_width: 0.001
#   O tempo mínimo de largura de pulso (em segundos). Isto deve corresponder a
#   um ângulo de 0 graus. O padrão é 0.001 segundos.
#maximum_pulse_width: 0.002
#   O tempo máximo de largura de pulso (em segundos). Isto deve corresponder a
#   um ângulo de maximum_servo_angle. O padrão é 0.002 segundos.
#initial_angle:
#   Ângulo inicial (em graus) com o qual configurar o servo. O padrão é não
#   enviar nenhum sinal na inicialização.
#initial_pulse_width:
#   Tempo inicial de largura de pulso (em segundos) com o qual configurar o
#   servo. (Isto é válido somente se initial_angle não estiver definido.) O
#   padrão é não enviar nenhum sinal na inicialização.
```

<a id="gcode_button"></a>
### [gcode_button]

Executa G-code quando um botão é pressionado ou liberado (ou quando um pino muda de estado). Pode-se verificar o estado do botão usando `QUERY_BUTTON button=my_gcode_button`.

```
[gcode_button my_gcode_button]
pin:
#   O pino no qual o botão está conectado. Este parâmetro deve ser fornecido.
#analog_range:
#   Duas resistências separadas por vírgula (em Ohms) especificando as faixas
#   de resistência mínima e máxima para o botão. Se analog_range for fornecido,
#   então o pino deve ser um pino com capacidade analógica. O padrão é usar
#   gpio digital para o botão.
#analog_pullup_resistor:
#   A resistência de pullup (em Ohms) quando analog_range é especificado. O
#   padrão é 4700 ohms.
#press_gcode:
#   Uma lista de comandos G-Code a executar quando o botão é pressionado.
#   Modelos de G-Code são suportados. Este parâmetro deve ser informado.
#release_gcode:
#   Uma lista de comandos G-Code a executar quando o botão é liberado. Modelos
#   de G-Code são suportados. O padrão é não executar nenhum comando na
#   liberação do botão.
#debounce_delay:
#   Um período de tempo em segundos para eliminar o ricochete dos eventos antes
#   de executar o gcode do botão. Se o botão for pressionado e liberado durante
#   este atraso, todo o pressionamento do botão é ignorado. O padrão é 0.
```

<a id="output_pin"></a>
### [output_pin]

Pinos de saída configuráveis em tempo de execução (pode-se definir qualquer número de seções com o prefixo "output_pin"). Pinos configurados aqui serão configurados como pinos de saída e podem ser modificados em tempo de execução usando [comandos](G-Codes.md#output_pin) estendidos do tipo "SET_PIN PIN=my_pin VALUE=.1".

```
[output_pin my_pin]
pin:
#   O pino a configurar como saída. Este parâmetro deve ser informado.
#pwm: False
#   Defina se o pino de saída deve ser capaz de modulação por largura de pulso.
#   Se for true, os campos de value devem ficar entre 0 e 1; se for false os
#   campos de value devem ser 0 ou 1. O padrão é False.
#value:
#   O valor para definir inicialmente o pino durante a configuração do MCU. O
#   padrão é 0 (para tensão baixa).
#shutdown_value:
#   O valor para definir o pino em um evento de shutdown do MCU. O padrão é 0
#   (para tensão baixa).
#cycle_time: 0.100
#   A quantidade de tempo (em segundos) por ciclo de PWM. Recomenda-se que seja
#   10 milissegundos ou mais ao usar PWM baseado em software. O padrão é 0.100
#   segundos para pinos pwm.
#hardware_pwm: False
#   Ative para usar PWM por hardware em vez de PWM por software. Ao usar PWM
#   por hardware o tempo real de ciclo é limitado pela implementação e pode ser
#   significativamente diferente do cycle_time solicitado. O padrão é False.
#scale:
#   Este parâmetro pode ser usado para alterar como os parâmetros 'value' e
#   'shutdown_value' são interpretados para pinos pwm. Se informado, então o
#   parâmetro 'value' deve ficar entre 0.0 e 'scale'. Isso pode ser útil ao
#   configurar um pino PWM que controla uma referência de tensão de motor de
#   passo. O 'scale' pode ser definido para a amperagem equivalente do motor de
#   passo caso o PWM estivesse totalmente ativado, e então o parâmetro 'value'
#   pode ser especificado usando a amperagem desejada para o motor de passo. O
#   padrão é não escalonar o parâmetro 'value'.
#maximum_mcu_duration:
#static_value:
#   Estas opções estão obsoletas e não devem mais ser especificadas.
```

<a id="static_pwm_clock"></a>
### [static_pwm_clock]

Pino de saída estático configurável (pode-se definir qualquer número de seções com o prefixo "static_pwm_clock"). Pinos configurados aqui serão configurados como pinos de saída de relógio (clock). Geralmente usados para fornecer entrada de relógio a outro hardware na placa.

```
[static_pwm_clock my_pin]
pin:
#   O pino a configurar como saída. Este parâmetro deve ser informado.
#frequency: 100
#   Frequência de saída alvo.
```

<a id="pwm_tool"></a>
### [pwm_tool]

Pinos de saída digital com modulação por largura de pulso capazes de atualizações em alta velocidade (pode-se definir qualquer número de seções com o prefixo "output_pin"). Pinos configurados aqui serão configurados como pinos de saída e podem ser modificados em tempo de execução usando [comandos](G-Codes.md#output_pin) estendidos de G-code do tipo "SET_PIN PIN=my_pin VALUE=.1".

```
[pwm_tool my_tool]
pin:
#   O pino a configurar como saída. Este parâmetro deve ser informado.
#maximum_mcu_duration:
#   A duração máxima que um valor sem shutdown pode ser mantido pelo MCU sem
#   confirmação do host. Se o host não conseguir acompanhar uma atualização, o
#   MCU entrará em shutdown e definirá todos os pinos para seus respectivos
#   valores de shutdown.
#   O padrão é 0 (desativado).
#   Valores usuais ficam em torno de 5 segundos.
#value:
#shutdown_value:
#cycle_time: 0.100
#hardware_pwm: False
#scale:
#   Consulte a seção "output_pin" para a definição destes parâmetros.
```

<a id="pwm_cycle_time"></a>
### [pwm_cycle_time]

Pinos de saída configuráveis em tempo de execução com temporização dinâmica de ciclo PWM (pode-se definir qualquer número de seções com o prefixo "pwm_cycle_time"). Pinos configurados aqui serão configurados como pinos de saída e podem ser modificados em tempo de execução usando [comandos](G-Codes.md#pwm_cycle_time) estendidos de G-code do tipo "SET_PIN PIN=my_pin VALUE=.1 CYCLE_TIME=0.100".

```
[pwm_cycle_time my_pin]
pin:
#value:
#shutdown_value:
#cycle_time: 0.100
#scale:
#   Consulte a seção "output_pin" para informações sobre estes parâmetros.
```

<a id="static_digital_output"></a>
### [static_digital_output]

Pinos de saída digital configurados estaticamente (pode-se definir qualquer número de seções com o prefixo "static_digital_output"). Pinos configurados aqui serão configurados como saída GPIO durante a configuração do MCU. Eles não podem ser alterados em tempo de execução.

```
[static_digital_output my_output_pins]
pins:
#   Uma lista separada por vírgulas de pinos a definir como pinos de saída
#   GPIO. O pino será definido em nível alto a menos que o nome do pino seja
#   precedido por "!". Este parâmetro deve ser informado.
```

<a id="multi_pin"></a>
### [multi_pin]

Saídas de múltiplos pinos (pode-se definir qualquer número de seções com o prefixo "multi_pin"). Uma saída multi_pin cria um apelido interno de pino que pode modificar múltiplos pinos de saída cada vez que o pino apelido é definido. Por exemplo, pode-se definir um objeto "[multi_pin my_fan]" contendo dois pinos e então definir "pin=multi_pin:my_fan" na seção "[fan]" - a cada alteração do ventilador ambos os pinos de saída seriam atualizados. Estes apelidos não podem ser usados com pinos de motor de passo.

```
[multi_pin my_multi_pin]
pins:
#   Uma lista separada por vírgulas de pinos associados a este alias. Este
#   parâmetro deve ser informado.
```

<a id="tmc-stepper-driver-configuration"></a>
## Configuração de drivers de motor de passo TMC

Configuração dos drivers de motor passo Trinamic em modo UART/SPI. Informações adicionais estão [Guia dos drivers TMC](TMC_Drivers.md) e na [referência de comandos](G-Codes.md#tmcxxxx).

<a id="tmc2130"></a>
### [tmc2130]

Configura um driver de motor de passo TMC2130 via barramento SPI. Para utilizar essa funcionalidade, defina uma secção de configuração com o prefixo "tmc2130" seguido do nome da secção de configuração do motor de passo correspondente (por exemplo, "[tmc2130 stepper_x]").

```
[tmc2130 stepper_x]
cs_pin:
#   O pino correspondente à linha de chip select do TMC2130. Este pino será
#   definido em nível baixo no início das mensagens SPI e elevado para nível
#   alto após a conclusão da mensagem. Este parâmetro deve ser informado.
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#chain_position:
#chain_length:
#   Estes parâmetros configuram uma cadeia SPI em margarida. Os dois parâmetros
#   definem a posição do motor de passo na cadeia e o comprimento total da
#   cadeia. A posição 1 corresponde ao motor de passo que se conecta ao sinal
#   MOSI. O padrão é não usar cadeia SPI em margarida.
#interpolate: True
#   Se true, ativa a interpolação de passos (o driver executará passos
#   internamente a uma taxa de 256 micropassos). Esta interpolação introduz um
#   pequeno desvio posicional sistêmico - consulte TMC_Drivers.md para
#   detalhes. O padrão é True.
run_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   durante o movimento do motor de passo. Este parâmetro deve ser informado.
#hold_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   quando o motor de passo não está em movimento. Definir um hold_current não
#   é recomendado (consulte TMC_Drivers.md para detalhes). O padrão é não
#   reduzir a corrente.
#sense_resistor: 0.110
#   A resistência (em ohms) do resistor de detecção do motor. O padrão é 0.110
#   ohms.
#stealthchop_threshold: 0
#   A velocidade (em mm/s) para definir o limiar de "stealthChop". Quando
#   definido, o modo "stealthChop" será ativado se a velocidade do motor de
#   passo estiver abaixo deste valor. Observe que o código de "sensorless
#   homing" pode substituir temporariamente esta definição durante operações de
#   homing (referenciamento). O padrão é 0, o que desativa o modo
#   "stealthChop".
#coolstep_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "CoolStep" do
#   driver TMC. Se definido, o recurso coolstep será ativado quando a
#   velocidade do motor de passo estiver próxima ou acima deste valor.
#   Importante - se coolstep_threshold estiver definido e "sensorless homing"
#   for usado, deve-se garantir que a velocidade de homing (referenciamento)
#   esteja acima do limiar de coolstep! O padrão é não ativar o recurso
#   coolstep.
#high_velocity_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "high velocity"
#   (THIGH) do driver TMC. Isso é normalmente usado para desativar o recurso
#   "CoolStep" em altas velocidades. O padrão é não definir um limiar de "high
#   velocity" do TMC.
#driver_MSLUT0: 2863314260
#driver_MSLUT1: 1251300522
#driver_MSLUT2: 608774441
#driver_MSLUT3: 269500962
#driver_MSLUT4: 4227858431
#driver_MSLUT5: 3048961917
#driver_MSLUT6: 1227445590
#driver_MSLUT7: 4211234
#driver_W0: 2
#driver_W1: 1
#driver_W2: 1
#driver_W3: 1
#driver_X1: 128
#driver_X2: 255
#driver_X3: 255
#driver_START_SIN: 0
#driver_START_SIN90: 247
#   Estes campos controlam diretamente os registradores da Tabela de
#   micropassos. A tabela de onda ideal é específica para cada motor e pode
#   variar com a corrente. Uma configuração ideal terá artefatos mínimos de
#   impressão causados por movimento não linear do motor de passo. Os valores
#   especificados acima são os valores padrão usados pelo driver. O valor deve
#   ser especificado como um inteiro decimal (a forma hex não é suportada).
#   Para calcular os campos da tabela de onda, consulte a "Planilha de cálculo"
#   do tmc2130 no site da Trinamic.
#driver_IHOLDDELAY: 8
#driver_TPOWERDOWN: 0
#driver_TBL: 1
#driver_TOFF: 4
#driver_HEND: 7
#driver_HSTRT: 0
#driver_VHIGHFS: 0
#driver_VHIGHCHM: 0
#driver_PWM_AUTOSCALE: True
#driver_PWM_FREQ: 1
#driver_PWM_GRAD: 4
#driver_PWM_AMPL: 128
#driver_FREEWHEEL: 0
#driver_SGT: 0
#driver_SEMIN: 0
#driver_SEUP: 0
#driver_SEMAX: 0
#driver_SEDN: 0
#driver_SEIMIN: 0
#driver_SFILT: 0
#   Defina o registrador informado durante a configuração do chip TMC2130. Isso
#   pode ser usado para definir parâmetros personalizados do motor. Os padrões
#   de cada parâmetro estão ao lado do nome do parâmetro na lista acima.
#diag0_pin:
#diag1_pin:
#   O pino do microcontrolador ligado a uma das linhas DIAG do chip TMC2130.
#   Apenas um único pino diag deve ser especificado. O pino é "active low" e
#   portanto normalmente é precedido por "^!". Definir isso cria um pino
#   virtual "tmc2130_stepper_x:virtual_endstop" que pode ser usado como
#   endstop_pin do motor de passo. Fazer isso ativa "sensorless homing".
#   (Certifique-se de também definir driver_SGT com um valor de sensibilidade
#   apropriado.) O padrão é não ativar sensorless homing.
```

<a id="tmc2208"></a>
### [tmc2208]

Configura um driver de motor de passo TMC2208 (ou TMC2224) UART via um único fio. Para usar essa funcionalidade, defina uma secção de configuração com um prefixo "tmc2208" seguido do nome da secção de configuração do motor de passo (por exemplo, "[tmc2208 stepper_x]").

```
[tmc2208 stepper_x]
uart_pin:
#   O pino conectado à linha PDN_UART do TMC2208. Este parâmetro deve ser
#   informado.
#tx_pin:
#   Se usar linhas separadas de recepção e transmissão para comunicar com o
#   driver então defina uart_pin como o pino de recepção e tx_pin como o pino
#   de transmissão. O padrão é usar uart_pin tanto para leitura quanto para
#   escrita.
#select_pins:
#   Uma lista separada por vírgulas de pinos a definir antes de acessar a UART
#   do tmc2208. Isso pode ser útil para configurar um mux analógico para
#   comunicação UART. O padrão é não configurar nenhum pino.
#interpolate: True
#   Se true, ativa a interpolação de passos (o driver executará passos
#   internamente a uma taxa de 256 micropassos). Esta interpolação introduz um
#   pequeno desvio posicional sistêmico - consulte TMC_Drivers.md para
#   detalhes. O padrão é True.
run_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   durante o movimento do motor de passo. Este parâmetro deve ser informado.
#hold_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   quando o motor de passo não está em movimento. Definir um hold_current não
#   é recomendado (consulte TMC_Drivers.md para detalhes). O padrão é não
#   reduzir a corrente.
#sense_resistor: 0.110
#   A resistência (em ohms) do resistor de detecção do motor. O padrão é 0.110
#   ohms.
#stealthchop_threshold: 0
#   A velocidade (em mm/s) para definir o limiar de "stealthChop". Quando
#   definido, o modo "stealthChop" será ativado se a velocidade do motor de
#   passo estiver abaixo deste valor. Observe que o código de "sensorless
#   homing" pode substituir temporariamente esta definição durante operações de
#   homing (referenciamento). O padrão é 0, o que desativa o modo
#   "stealthChop".
#driver_MULTISTEP_FILT: True
#driver_IHOLDDELAY: 8
#driver_TPOWERDOWN: 20
#driver_TBL: 2
#driver_TOFF: 3
#driver_HEND: 0
#driver_HSTRT: 5
#driver_PWM_AUTOGRAD: True
#driver_PWM_AUTOSCALE: True
#driver_PWM_LIM: 12
#driver_PWM_REG: 8
#driver_PWM_FREQ: 1
#driver_PWM_GRAD: 14
#driver_PWM_OFS: 36
#driver_FREEWHEEL: 0
#   Defina o registrador informado durante a configuração do chip TMC2208. Isso
#   pode ser usado para definir parâmetros personalizados do motor. Os padrões
#   de cada parâmetro estão ao lado do nome do parâmetro na lista acima.
```

<a id="tmc2209"></a>
### [tmc2209]

Configura um driver de motor de passo TMC2209 UART via um único fio. Para usar essa funcionalidade, defina uma secção de configuração com um prefixo "tmc2209" seguido do nome da secção de configuração do motor de passo (por exemplo, "[tmc2209 stepper_x]").

```
[tmc2209 stepper_x]
uart_pin:
#tx_pin:
#select_pins:
#interpolate: True
run_current:
#hold_current:
#sense_resistor: 0.110
#stealthchop_threshold: 0
#   Consulte a seção "tmc2208" para a definição destes parâmetros.
#coolstep_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "CoolStep" do
#   driver TMC. Se definido, o recurso coolstep será ativado quando a
#   velocidade do motor de passo estiver próxima ou acima deste valor.
#   Importante - se coolstep_threshold estiver definido e "sensorless homing"
#   for usado, deve-se garantir que a velocidade de homing (referenciamento)
#   esteja acima do limiar de coolstep! O padrão é não ativar o recurso
#   coolstep.
#uart_address:
#   O endereço do chip TMC2209 para mensagens UART (um inteiro entre 0 e 3).
#   Isso é normalmente usado quando vários chips TMC2209 estão conectados ao
#   mesmo pino UART. O padrão é zero.
#driver_MULTISTEP_FILT: True
#driver_IHOLDDELAY: 8
#driver_TPOWERDOWN: 20
#driver_TBL: 2
#driver_TOFF: 3
#driver_HEND: 0
#driver_HSTRT: 5
#driver_PWM_AUTOGRAD: True
#driver_PWM_AUTOSCALE: True
#driver_PWM_LIM: 12
#driver_PWM_REG: 8
#driver_PWM_FREQ: 1
#driver_PWM_GRAD: 14
#driver_PWM_OFS: 36
#driver_FREEWHEEL: 0
#driver_SGTHRS: 0
#driver_SEMIN: 0
#driver_SEUP: 0
#driver_SEMAX: 0
#driver_SEDN: 0
#driver_SEIMIN: 0
#   Defina o registrador informado durante a configuração do chip TMC2209. Isso
#   pode ser usado para definir parâmetros personalizados do motor. Os padrões
#   de cada parâmetro estão ao lado do nome do parâmetro na lista acima.
#diag_pin:
#   O pino do microcontrolador ligado à linha DIAG do chip TMC2209. O pino é
#   normalmente precedido por "^" para ativar um pullup. Definir isso cria um
#   pino virtual "tmc2209_stepper_x:virtual_endstop" que pode ser usado como
#   endstop_pin do motor de passo. Fazer isso ativa "sensorless homing".
#   (Certifique-se de também definir driver_SGTHRS com um valor de
#   sensibilidade apropriado.) O padrão é não ativar sensorless homing.
```

<a id="tmc2660"></a>
### [tmc2660]

Configura um driver de motor de passo TMC2660 via barramento SPI. Para usar essa funcionalidade, defina uma secção de configuração com um prefixo "tmc2660" seguido do nome da secção de configuração do motor de passo (por exemplo, "[tmc2660 stepper_x]").

```
[tmc2660 stepper_x]
cs_pin:
#   O pino correspondente à linha de chip select do TMC2660. Este pino será
#   definido em nível baixo no início das mensagens SPI e definido em nível
#   alto após a conclusão da transferência da mensagem. Este parâmetro deve ser
#   informado.
#spi_speed: 4000000
#   Frequência do barramento SPI usada para comunicar com o driver de motor de
#   passo TMC2660. O padrão é 4000000.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#interpolate: True
#   Se true, ativa a interpolação de passos (o driver executará passos
#   internamente a uma taxa de 256 micropassos). Isso só funciona se microsteps
#   estiver definido como 16. A interpolação introduz um pequeno desvio
#   posicional sistêmico - consulte TMC_Drivers.md para detalhes. O padrão é
#   True.
run_current:
#   A quantidade de corrente (em amps RMS) usada pelo driver durante o
#   movimento do motor de passo. Este parâmetro deve ser informado.
#sense_resistor:
#   A resistência (em ohms) do resistor de detecção do motor. Este parâmetro
#   deve ser informado.
#idle_current_percent: 100
#   A porcentagem de run_current para a qual o driver do motor de passo será
#   reduzido quando o tempo limite de inatividade expirar (é preciso configurar
#   o tempo limite usando uma seção de configuração [idle_timeout]). A corrente
#   será elevada novamente assim que o motor de passo tiver que se mover
#   novamente. Certifique-se de definir isso com um valor alto o suficiente
#   para que os motores de passo não percam sua posição. Há também um pequeno
#   atraso até que a corrente seja elevada novamente, portanto leve isso em
#   conta ao comandar movimentos rápidos enquanto o motor de passo está
#   inativo. O padrão é 100 (sem redução).
#driver_TBL: 2
#driver_RNDTF: 0
#driver_HDEC: 0
#driver_CHM: 0
#driver_HEND: 3
#driver_HSTRT: 3
#driver_TOFF: 4
#driver_SEIMIN: 0
#driver_SEDN: 0
#driver_SEMAX: 0
#driver_SEUP: 0
#driver_SEMIN: 0
#driver_SFILT: 0
#driver_SGT: 0
#driver_SLPH: 0
#driver_SLPL: 0
#driver_DISS2G: 0
#driver_TS2G: 3
#   Defina o parâmetro informado durante a configuração do chip TMC2660. Isso
#   pode ser usado para definir parâmetros personalizados do driver. Os padrões
#   de cada parâmetro estão ao lado do nome do parâmetro na lista acima.
#   Consulte o datasheet do TMC2660 sobre o que cada parâmetro faz e quais são
#   as restrições sobre combinações de parâmetros. Esteja especialmente atento
#   ao registrador CHOPCONF, onde definir CHM como zero ou um levará a mudanças
#   de layout (o primeiro bit de HDEC) é interpretado como o MSB de HSTRT neste
#   caso).
```

<a id="tmc2240"></a>
### [tmc2240]

Configura um driver de motor de passo TMC2240 via barramento SPI ou UART. Para usar este recurso, defina uma seção de configuração com o prefixo "tmc2240" seguido do nome da seção de configuração do motor de passo correspondente (por exemplo, "[tmc2240 stepper_x]").

```
[tmc2240 stepper_x]
cs_pin:
#   O pino correspondente à linha de chip select do TMC2240. Este pino será
#   definido em nível baixo no início das mensagens SPI e elevado para nível
#   alto após a conclusão da mensagem. Este parâmetro deve ser informado.
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#uart_pin:
#   O pino conectado à linha DIAG1/SW do TMC2240. Se este parâmetro for
#   informado a comunicação UART é usada em vez de SPI.
#chain_position:
#chain_length:
#   Estes parâmetros configuram uma cadeia SPI em margarida. Os dois parâmetros
#   definem a posição do motor de passo na cadeia e o comprimento total da
#   cadeia. A posição 1 corresponde ao motor de passo que se conecta ao sinal
#   MOSI. O padrão é não usar cadeia SPI em margarida.
#interpolate: True
#   Se true, ativa a interpolação de passos (o driver executará passos
#   internamente a uma taxa de 256 micropassos). O padrão é True.
run_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   durante o movimento do motor de passo. Este parâmetro deve ser informado.
#hold_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   quando o motor de passo não está em movimento. Definir um hold_current não
#   é recomendado (consulte TMC_Drivers.md para detalhes). O padrão é não
#   reduzir a corrente.
#rref: 12000
#   A resistência (em ohms) do resistor entre IREF e GND. O padrão é 12000.
#stealthchop_threshold: 0
#   A velocidade (em mm/s) para definir o limiar de "stealthChop". Quando
#   definido, o modo "stealthChop" será ativado se a velocidade do motor de
#   passo estiver abaixo deste valor. Observe que o código de "sensorless
#   homing" pode substituir temporariamente esta definição durante operações de
#   homing (referenciamento). O padrão é 0, o que desativa o modo
#   "stealthChop".
#coolstep_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "CoolStep" do
#   driver TMC. Se definido, o recurso coolstep será ativado quando a
#   velocidade do motor de passo estiver próxima ou acima deste valor.
#   Importante - se coolstep_threshold estiver definido e "sensorless homing"
#   for usado, deve-se garantir que a velocidade de homing (referenciamento)
#   esteja acima do limiar de coolstep! O padrão é não ativar o recurso
#   coolstep.
#high_velocity_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "high velocity"
#   (THIGH) do driver TMC. Isso é normalmente usado para desativar o recurso
#   "CoolStep" em altas velocidades. O padrão é não definir um limiar de "high
#   velocity" do TMC.
#driver_MSLUT0: 2863314260
#driver_MSLUT1: 1251300522
#driver_MSLUT2: 608774441
#driver_MSLUT3: 269500962
#driver_MSLUT4: 4227858431
#driver_MSLUT5: 3048961917
#driver_MSLUT6: 1227445590
#driver_MSLUT7: 4211234
#driver_W0: 2
#driver_W1: 1
#driver_W2: 1
#driver_W3: 1
#driver_X1: 128
#driver_X2: 255
#driver_X3: 255
#driver_START_SIN: 0
#driver_START_SIN90: 247
#driver_OFFSET_SIN90: 0
#   Estes campos controlam diretamente os registradores da Tabela de
#   micropassos. A tabela de onda ideal é específica para cada motor e pode
#   variar com a corrente. Uma configuração ideal terá artefatos mínimos de
#   impressão causados por movimento não linear do motor de passo. Os valores
#   especificados acima são os valores padrão usados pelo driver. O valor deve
#   ser especificado como um inteiro decimal (a forma hex não é suportada).
#   Para calcular os campos da tabela de onda, consulte a "Planilha de cálculo"
#   do tmc2130 no site da Trinamic. Adicionalmente, este driver também tem o
#   campo OFFSET_SIN90 que pode ser usado para ajustar um motor com bobinas
#   desbalanceadas. Consulte a seção `Sine Wave Lookup Table` no datasheet para
#   informações sobre este campo e como ajustá-lo.
#driver_MULTISTEP_FILT: True
#driver_IHOLDDELAY: 6
#driver_IRUNDELAY: 4
#driver_TPOWERDOWN: 10
#driver_TBL: 2
#driver_TOFF: 3
#driver_HEND: 2
#driver_HSTRT: 5
#driver_FD3: 0
#driver_TPFD: 4
#driver_CHM: 0
#driver_VHIGHFS: 0
#driver_VHIGHCHM: 0
#driver_DISS2G: 0
#driver_DISS2VS: 0
#driver_PWM_AUTOSCALE: True
#driver_PWM_AUTOGRAD: True
#driver_PWM_FREQ: 0
#driver_FREEWHEEL: 0
#driver_PWM_GRAD: 0
#driver_PWM_OFS: 29
#driver_PWM_REG: 4
#driver_PWM_LIM: 12
#driver_SGT: 0
#driver_SEMIN: 0
#driver_SEUP: 0
#driver_SEMAX: 0
#driver_SEDN: 0
#driver_SEIMIN: 0
#driver_SFILT: 0
#driver_SG4_THRS: 0
#driver_SG4_ANGLE_OFFSET: 1
#driver_SLOPE_CONTROL: 0
#   Defina o registrador informado durante a configuração do chip TMC2240. Isso
#   pode ser usado para definir parâmetros personalizados do motor. Os padrões
#   de cada parâmetro estão ao lado do nome do parâmetro na lista acima.
#diag0_pin:
#diag1_pin:
#   O pino do microcontrolador ligado a uma das linhas DIAG do chip TMC2240.
#   Apenas um único pino diag deve ser especificado. O pino é "active low" e
#   portanto normalmente é precedido por "^!". Definir isso cria um pino
#   virtual "tmc2240_stepper_x:virtual_endstop" que pode ser usado como
#   endstop_pin do motor de passo. Fazer isso ativa "sensorless homing".
#   (Certifique-se de também definir driver_SGT OU driver_SG4_THRS com um valor
#   de sensibilidade apropriado.) O padrão é não ativar sensorless homing.
```

<a id="tmc5160"></a>
### [tmc5160]

Configura um driver de motor de passo TMC5160 via barramento SPI. Para usar essa funcionalidade, defina uma secção de configuração com um prefixo "tmc5160" seguido do nome da secção de configuração do motor de passo (por exemplo, "[tmc5160 stepper_x]").

```
[tmc5160 stepper_x]
cs_pin:
#   O pino correspondente à linha de chip select do TMC5160. Este pino será
#   definido em nível baixo no início das mensagens SPI e elevado para nível
#   alto após a conclusão da mensagem. Este parâmetro deve ser informado.
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#chain_position:
#chain_length:
#   Estes parâmetros configuram uma cadeia SPI em margarida. Os dois parâmetros
#   definem a posição do motor de passo na cadeia e o comprimento total da
#   cadeia. A posição 1 corresponde ao motor de passo que se conecta ao sinal
#   MOSI. O padrão é não usar cadeia SPI em margarida.
#interpolate: True
#   Se true, ativa a interpolação de passos (o driver executará passos
#   internamente a uma taxa de 256 micropassos). O padrão é True.
run_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   durante o movimento do motor de passo. Este parâmetro deve ser informado.
#hold_current:
#   A quantidade de corrente (em amps RMS) para configurar o driver a usar
#   quando o motor de passo não está em movimento. Definir um hold_current não
#   é recomendado (consulte TMC_Drivers.md para detalhes). O padrão é não
#   reduzir a corrente.
#sense_resistor: 0.075
#   A resistência (em ohms) do resistor de detecção do motor. O padrão é 0.075
#   ohms.
#stealthchop_threshold: 0
#   A velocidade (em mm/s) para definir o limiar de "stealthChop". Quando
#   definido, o modo "stealthChop" será ativado se a velocidade do motor de
#   passo estiver abaixo deste valor. Observe que o código de "sensorless
#   homing" pode substituir temporariamente esta definição durante operações de
#   homing (referenciamento). O padrão é 0, o que desativa o modo
#   "stealthChop".
#coolstep_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "CoolStep" do
#   driver TMC. Se definido, o recurso coolstep será ativado quando a
#   velocidade do motor de passo estiver próxima ou acima deste valor.
#   Importante - se coolstep_threshold estiver definido e "sensorless homing"
#   for usado, deve-se garantir que a velocidade de homing (referenciamento)
#   esteja acima do limiar de coolstep! O padrão é não ativar o recurso
#   coolstep.
#high_velocity_threshold:
#   A velocidade (em mm/s) para definir o limiar interno de "high velocity"
#   (THIGH) do driver TMC. Isso é normalmente usado para desativar o recurso
#   "CoolStep" em altas velocidades. O padrão é não definir um limiar de "high
#   velocity" do TMC.
#driver_MSLUT0: 2863314260
#driver_MSLUT1: 1251300522
#driver_MSLUT2: 608774441
#driver_MSLUT3: 269500962
#driver_MSLUT4: 4227858431
#driver_MSLUT5: 3048961917
#driver_MSLUT6: 1227445590
#driver_MSLUT7: 4211234
#driver_W0: 2
#driver_W1: 1
#driver_W2: 1
#driver_W3: 1
#driver_X1: 128
#driver_X2: 255
#driver_X3: 255
#driver_START_SIN: 0
#driver_START_SIN90: 247
#   Estes campos controlam diretamente os registradores da Tabela de
#   micropassos. A tabela de onda ideal é específica para cada motor e pode
#   variar com a corrente. Uma configuração ideal terá artefatos mínimos de
#   impressão causados por movimento não linear do motor de passo. Os valores
#   especificados acima são os valores padrão usados pelo driver. O valor deve
#   ser especificado como um inteiro decimal (a forma hex não é suportada).
#   Para calcular os campos da tabela de onda, consulte a "Planilha de cálculo"
#   do tmc2130 no site da Trinamic.
#driver_MULTISTEP_FILT: True
#driver_IHOLDDELAY: 6
#driver_TPOWERDOWN: 10
#driver_TBL: 2
#driver_TOFF: 3
#driver_HEND: 2
#driver_HSTRT: 5
#driver_FD3: 0
#driver_TPFD: 4
#driver_CHM: 0
#driver_VHIGHFS: 0
#driver_VHIGHCHM: 0
#driver_DISS2G: 0
#driver_DISS2VS: 0
#driver_PWM_AUTOSCALE: True
#driver_PWM_AUTOGRAD: True
#driver_PWM_FREQ: 0
#driver_FREEWHEEL: 0
#driver_PWM_GRAD: 0
#driver_PWM_OFS: 30
#driver_PWM_REG: 4
#driver_PWM_LIM: 12
#driver_SGT: 0
#driver_SEMIN: 0
#driver_SEUP: 0
#driver_SEMAX: 0
#driver_SEDN: 0
#driver_SEIMIN: 0
#driver_SFILT: 0
#driver_DRVSTRENGTH: 0
#driver_BBMCLKS: 4
#driver_BBMTIME: 0
#driver_FILT_ISENSE: 0
#   Defina o registrador informado durante a configuração do chip TMC5160. Isso
#   pode ser usado para definir parâmetros personalizados do motor. Os padrões
#   de cada parâmetro estão ao lado do nome do parâmetro na lista acima.
#diag0_pin:
#diag1_pin:
#   O pino do microcontrolador ligado a uma das linhas DIAG do chip TMC5160.
#   Apenas um único pino diag deve ser especificado. O pino é "active low" e
#   portanto normalmente é precedido por "^!". Definir isso cria um pino
#   virtual "tmc5160_stepper_x:virtual_endstop" que pode ser usado como
#   endstop_pin do motor de passo. Fazer isso ativa "sensorless homing".
#   (Certifique-se de também definir driver_SGT com um valor de sensibilidade
#   apropriado.) O padrão é não ativar sensorless homing.
```

<a id="tmc5262"></a>
### [tmc5262]

Configura um driver de motor de passo TMC5262 via SPI. Defina uma seção com o prefixo "tmc5262" seguido do nome do motor de passo correspondente (por exemplo, "[tmc5262 stepper_x]").

Esta implementação suporta SpreadCycle com fim de curso físico.

```
[tmc5262 stepper_x]
cs_pin:
#   O pino conectado à entrada de chip select do driver. Este parâmetro deve
#   ser informado.
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI".
#chain_position:
#chain_length:
#   Consulte a seção tmc5160 para definições de cadeia SPI em margarida.
#interpolate: True
#   Ative a interpolação para 256 micropassos. O padrão é True.
run_current:
#   Corrente do motor em amps RMS. Este parâmetro deve ser informado. O Klipper
#   seleciona a menor faixa de corrente de hardware que comporta este valor.
#   SET_TMC_CURRENT é limitado a essa faixa; configure um run_current maior e
#   reinicie para selecionar uma faixa maior se necessário.
#hold_current:
#   Corrente em amps RMS quando o motor não está em movimento. O padrão é
#   run_current. Definir um hold_current separado não é recomendado; consulte
#   TMC_Drivers.md.
#rref: 12000
#   Resistência em ohms do resistor de referência entre REF e GND. Obtenha este
#   valor na documentação ou no esquemático da placa do driver. O padrão é
#   12000. O driver usa detecção de corrente integrada, não um resistor de
#   detecção externo.
#driver_MSLUT0: 2863314260
#driver_MSLUT1: 1251300522
#driver_MSLUT2: 608774441
#driver_MSLUT3: 269500962
#driver_MSLUT4: 4227858431
#driver_MSLUT5: 3048961917
#driver_MSLUT6: 1227445590
#driver_MSLUT7: 4211234
#driver_W0: 2
#driver_W1: 1
#driver_W2: 1
#driver_W3: 1
#driver_X1: 128
#driver_X2: 255
#driver_X3: 255
#driver_START_SIN: 0
#driver_START_SIN90: 247
#driver_OFFSET_SIN90: 0
#   Consulte a seção tmc2240 para definições da Tabela de micropassos.
#driver_MULTISTEP_FILT: True
#driver_IHOLDDELAY: 7
#driver_IRUNDELAY: 4
#driver_TPOWERDOWN: 10
#driver_TBL: 2
#driver_TOFF: 3
#driver_HEND: 2
#driver_HSTRT: 5
#driver_FD3: 0
#driver_DISFDCC: 0
#driver_TPFD: 4
#driver_CHM: 0
#driver_SLOPE_CONTROL: 3
#   Defina o campo de registrador correspondente durante a configuração do
#   driver. Consulte o datasheet do TMC5262 para definições dos campos. Os
#   padrões são mostrados acima.
```


<a id="run-time-stepper-motor-current-configuration"></a>
## Configuração de corrente de motor de passo em tempo de execução

<a id="ad5206"></a>
### [ad5206]

Digipots AD5206 configurados estaticamente conectados via barramento SPI (pode-se definir qualquer número de seções com o prefixo "ad5206").

```
[ad5206 my_digipot]
enable_pin:
#   O pino correspondente à linha de chip select do AD5206. Este pino será
#   definido em nível baixo no início das mensagens SPI e elevado para nível
#   alto após a conclusão da mensagem. Este parâmetro deve ser informado.
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
#channel_1:
#channel_2:
#channel_3:
#channel_4:
#channel_5:
#channel_6:
#   O valor para definir estaticamente o canal informado do AD5206. Isso é
#   normalmente definido como um número entre 0.0 e 1.0 com 1.0 sendo a
#   resistência mais alta e 0.0 sendo a resistência mais baixa. No entanto, a
#   faixa pode ser alterada com o parâmetro 'scale' (veja abaixo). Se um canal
#   não for especificado então ele é deixado sem configuração.
#scale:
#   Este parâmetro pode ser usado para alterar como os parâmetros 'channel_x'
#   são interpretados. Se informado, então os parâmetros 'channel_x' devem
#   ficar entre 0.0 e 'scale'. Isso pode ser útil quando o AD5206 é usado para
#   definir referências de tensão de motor de passo. O 'scale' pode ser
#   definido para a amperagem equivalente do motor de passo caso o AD5206
#   estivesse em sua resistência mais alta, e então os parâmetros 'channel_x'
#   podem ser especificados usando o valor de amperagem desejado para o motor
#   de passo. O padrão é não escalonar os parâmetros 'channel_x'.
```

<a id="mcp4451"></a>
### [mcp4451]

Digipot MCP4451 configurado estaticamente conectado via barramento I2C (pode-se definir qualquer número de seções com o prefixo "mcp4451").

```
[mcp4451 my_digipot]
i2c_address:
#   O endereço i2c que o chip está usando no barramento i2c. Este parâmetro
#   deve ser informado.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima.
#wiper_0:
#wiper_1:
#wiper_2:
#wiper_3:
#   O valor para definir estaticamente o "wiper" informado do MCP4451. Isso é
#   normalmente definido como um número entre 0.0 e 1.0 com 1.0 sendo a
#   resistência mais alta e 0.0 sendo a resistência mais baixa. No entanto, a
#   faixa pode ser alterada com o parâmetro 'scale' (veja abaixo). Se um wiper
#   não for especificado então ele é deixado sem configuração.
#scale:
#   Este parâmetro pode ser usado para alterar como os parâmetros 'wiper_x' são
#   interpretados. Se informado, então os parâmetros 'wiper_x' devem ficar
#   entre 0.0 e 'scale'. Isso pode ser útil quando o MCP4451 é usado para
#   definir referências de tensão de motor de passo. O 'scale' pode ser
#   definido para a amperagem equivalente do motor de passo caso o MCP4451
#   estivesse em sua resistência mais alta, e então os parâmetros 'wiper_x'
#   podem ser especificados usando o valor de amperagem desejado para o motor
#   de passo. O padrão é não escalonar os parâmetros 'wiper_x'.
```

<a id="mcp4728"></a>
### [mcp4728]

Conversor digital-analógico MCP4728 configurado estaticamente conectado via barramento I2C (pode-se definir qualquer número de seções com o prefixo "mcp4728").

```
[mcp4728 my_dac]
#i2c_address: 96
#   O endereço i2c que o chip está usando no barramento i2c. O padrão é 96.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima.
#channel_a:
#channel_b:
#channel_c:
#channel_d:
#   O valor para definir estaticamente o canal indicado do MCP4728. Normalmente
#   é definido como um número entre 0.0 e 1.0, sendo 1.0 a tensão mais alta
#   (2.048V) e 0.0 a tensão mais baixa. Entretanto, a faixa pode ser alterada
#   com o parâmetro 'scale' (veja abaixo). Se um canal não for especificado,
#   ele permanece sem configuração.
#scale:
#   Este parâmetro pode ser usado para alterar como os parâmetros 'channel_x'
#   são interpretados. Se fornecido, então os parâmetros 'channel_x' devem
#   estar entre 0.0 e 'scale'. Isso pode ser útil quando o MCP4728 é usado para
#   definir referências de tensão do motor de passo. O 'scale' pode ser
#   definido como a amperagem equivalente do motor de passo se o MCP4728
#   estivesse em sua tensão mais alta (2.048V), e então os parâmetros
#   'channel_x' podem ser especificados usando o valor de amperagem desejado
#   para o motor de passo. O padrão é não aplicar escala aos parâmetros
#   'channel_x'.
```

<a id="mcp4018"></a>
### [mcp4018]

Digipot MCP4018 configurado estaticamente conectado via i2c (pode-se definir qualquer número de seções com o prefixo "mcp4018").

```
[mcp4018 my_digipot]
#i2c_address: 47
#   O endereço i2c que o chip está usando no barramento i2c. O padrão é 47.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima.
wiper:
#   O valor para definir estaticamente o "wiper" indicado do MCP4018.
#   Normalmente é definido como um número entre 0.0 e 1.0, sendo 1.0 a
#   resistência mais alta e 0.0 a resistência mais baixa. Entretanto, a faixa
#   pode ser alterada com o parâmetro 'scale' (veja abaixo). Este parâmetro
#   deve ser fornecido.
#scale:
#   Este parâmetro pode ser usado para alterar como o parâmetro 'wiper' é
#   interpretado. Se fornecido, então o parâmetro 'wiper' deve estar entre 0.0
#   e 'scale'. Isso pode ser útil quando o MCP4018 é usado para definir
#   referências de tensão do motor de passo. O 'scale' pode ser definido como a
#   amperagem equivalente do motor de passo se o MCP4018 estiver em sua
#   resistência mais alta, e então o parâmetro 'wiper' pode ser especificado
#   usando o valor de amperagem desejado para o motor de passo. O padrão é não
#   aplicar escala ao parâmetro 'wiper'.
```

<a id="display-support"></a>
## Suporte a tela

<a id="display"></a>
### [display]

Suporte a uma tela conectada ao microcontrolador.

```
[display]
lcd_type:
#   O tipo de chip de LCD em uso. Pode ser "hd44780", "hd44780_spi",
#   "aip31068_spi", "st7920", "emulated_st7920", "uc1701", "ssd1306" ou
#   "sh1106". Consulte as seções de display abaixo para informações sobre cada
#   tipo e parâmetros adicionais que eles fornecem. Este parâmetro deve ser
#   fornecido.
#display_group:
#   O nome do grupo display_data a mostrar no display. Isso controla o conteúdo
#   da tela (consulte a seção "display_data" para mais informações). O padrão é
#   _default_20x4 para displays hd44780 ou aip31068_spi e _default_16x4 para
#   outros displays.
#menu_timeout:
#   Tempo limite para o menu. Permanecer inativo por esta quantidade de
#   segundos acionará a saída do menu ou o retorno ao menu raiz quando autorun
#   estiver habilitado. O padrão é 0 segundos (desabilitado)
#menu_root:
#   Nome da seção de menu principal a mostrar ao clicar no encoder na tela
#   inicial. O padrão é __main, e isso mostra os menus padrão conforme
#   definidos em klippy/extras/display/menu.cfg
#menu_reverse_navigation:
#   Quando habilitado, inverterá as direções para cima e para baixo na
#   navegação por lista. O padrão é False. Este parâmetro é opcional.
#encoder_pins:
#   Os pinos conectados ao encoder. 2 pinos devem ser fornecidos ao usar
#   encoder. Este parâmetro deve ser fornecido ao usar menu.
#encoder_steps_per_detent:
#   Quantos passos o encoder emite por detent ("click"). Se o encoder precisar
#   de dois detents para mover entre entradas ou mover duas entradas a partir
#   de um detent, tente alterar isso. Os valores permitidos são 2
#   (half-stepping) ou 4 (full-stepping). O padrão é 4.
#click_pin:
#   O pino conectado ao botão 'enter' ou ao 'click' do encoder. Este parâmetro
#   deve ser fornecido ao usar menu. A presença de um parâmetro de configuração
#   'analog_range_click_pin' transforma este parâmetro de digital em analógico.
#back_pin:
#   O pino conectado ao botão 'back'. Este parâmetro é opcional, o menu pode
#   ser usado sem ele. A presença de um parâmetro de configuração
#   'analog_range_back_pin' transforma este parâmetro de digital em analógico.
#up_pin:
#   O pino conectado ao botão 'up'. Este parâmetro deve ser fornecido ao usar
#   menu sem encoder. A presença de um parâmetro de configuração
#   'analog_range_up_pin' transforma este parâmetro de digital em analógico.
#down_pin:
#   O pino conectado ao botão 'down'. Este parâmetro deve ser fornecido ao usar
#   menu sem encoder. A presença de um parâmetro de configuração
#   'analog_range_down_pin' transforma este parâmetro de digital em analógico.
#kill_pin:
#   O pino conectado ao botão 'kill'. Este botão chamará parada de emergência.
#   A presença de um parâmetro de configuração 'analog_range_kill_pin'
#   transforma este parâmetro de digital em analógico.
#analog_pullup_resistor: 4700
#   A resistência (em ohms) do pullup ligado ao botão analógico. O padrão é
#   4700 ohms.
#analog_range_click_pin:
#   A faixa de resistência para um botão 'enter'. Os valores mínimo e máximo da
#   faixa separados por vírgula devem ser fornecidos ao usar botão analógico.
#analog_range_back_pin:
#   A faixa de resistência para um botão 'back'. Os valores mínimo e máximo da
#   faixa separados por vírgula devem ser fornecidos ao usar botão analógico.
#analog_range_up_pin:
#   A faixa de resistência para um botão 'up'. Os valores mínimo e máximo da
#   faixa separados por vírgula devem ser fornecidos ao usar botão analógico.
#analog_range_down_pin:
#   A faixa de resistência para um botão 'down'. Os valores mínimo e máximo da
#   faixa separados por vírgula devem ser fornecidos ao usar botão analógico.
#analog_range_kill_pin:
#   A faixa de resistência para um botão 'kill'. Os valores mínimo e máximo da
#   faixa separados por vírgula devem ser fornecidos ao usar botão analógico.
```

<a id="hd44780-display"></a>
#### Tela hd44780

Informações sobre configuração de telas hd44780 (usadas em telas do tipo "RepRapDiscount 2004 Smart Controller").

```
[display]
lcd_type: hd44780
#   Defina como "hd44780" para displays hd44780.
rs_pin:
e_pin:
d4_pin:
d5_pin:
d6_pin:
d7_pin:
#   Os pinos conectados a um lcd do tipo hd44780. Estes parâmetros devem ser
#   fornecidos.
#hd44780_protocol_init: True
#   Executa inicialização de protocolo 8-bit/4-bit em um display hd44780. Isso
#   é necessário em dispositivos hd44780 reais. Entretanto, pode ser necessário
#   desabilitar isso em alguns dispositivos "clone". O padrão é True.
#line_length:
#   Define o número de caracteres por linha para um lcd do tipo hd44780. Os
#   valores possíveis são 20 (padrão) e 16. O número de linhas é fixo em 4.
#hd44780_delay: 0.000040
#   O tempo mínimo (em segundos) para aguardar após enviar um byte de comando
#   ou de dados para o chip hd44780. Alguns displays (normalmente os com
#   oscilador interno mais lento) precisam de mais tempo que o padrão e
#   mostrarão caracteres errados ou que mudam aleatoriamente. Altere isso
#   somente se isso ocorrer. Nesse caso, aumente o valor em passos de 0.000001
#   (1us) e verifique o display por um período mais longo (por exemplo, uma
#   impressão completa) a cada passo, pois os erros podem aparecer apenas
#   esporadicamente. Uma vez que o display esteja confiável, adicione uma
#   pequena margem (alguns microssegundos) pois o temporização do display pode
#   variar com a temperatura. Use o menor valor confiável, pois o
#   micro-controlador aguarda este atraso entre cada byte enviado ao display. O
#   máximo é 0.000060. O padrão é 0.000040.
...
```

<a id="hd44780_spi-display"></a>
#### Tela hd44780_spi

Informações sobre configuração de uma tela hd44780_spi - uma tela 20x04 controlada via "shift register" de hardware (usada em impressoras baseadas em mightyboard).

```
[display]
lcd_type: hd44780_spi
#   Defina como "hd44780_spi" para displays hd44780_spi.
latch_pin:
spi_software_sclk_pin:
spi_software_mosi_pin:
spi_software_miso_pin:
#   Os pinos conectados ao shift register que controla o display. O
#   spi_software_miso_pin precisa ser definido como um pino não usado da placa
#   principal da impressora pois o shift register não tem pino MISO, mas a
#   implementação de spi por software exige que este pino seja configurado.
#hd44780_protocol_init: True
#   Executa inicialização de protocolo 8-bit/4-bit em um display hd44780. Isso
#   é necessário em dispositivos hd44780 reais. Entretanto, pode ser necessário
#   desabilitar isso em alguns dispositivos "clone". O padrão é True.
#line_length:
#   Define o número de caracteres por linha para um lcd do tipo hd44780. Os
#   valores possíveis são 20 (padrão) e 16. O número de linhas é fixo em 4.
...
```

<a id="aip31068_spi-display"></a>
#### Tela aip31068_spi

Informações sobre configuração de uma tela aip31068_spi - muito semelhante à hd44780_spi, uma tela 20x04 (20 símbolos por 4 linhas) com protocolo interno ligeiramente diferente.

```
[display]
lcd_type: aip31068_spi
latch_pin:
spi_software_sclk_pin:
spi_software_mosi_pin:
spi_software_miso_pin:
#   Os pinos conectados ao shift register que controla o display. O
#   spi_software_miso_pin precisa ser definido como um pino não usado da placa
#   principal da impressora pois o shift register não tem pino MISO, mas a
#   implementação de spi por software exige que este pino seja configurado.
#line_length:
#   Define o número de caracteres por linha para um lcd do tipo hd44780. Os
#   valores possíveis são 20 (padrão) e 16. O número de linhas é fixo em 4.
...
```

<a id="st7920-display"></a>
#### Tela st7920

Informações sobre configuração de telas st7920 (usadas em telas do tipo "RepRapDiscount 12864 Full Graphic Smart Controller").

```
[display]
lcd_type: st7920
#   Defina como "st7920" para displays st7920.
cs_pin:
sclk_pin:
sid_pin:
#   Os pinos conectados a um lcd do tipo st7920. Estes parâmetros devem ser
#   fornecidos.
...
```

<a id="emulated_st7920-display"></a>
#### Tela emulated_st7920

Informações sobre configuração de uma tela st7920 emulada - encontrada em alguns "dispositivos touchscreen de 2,4 polegadas" e semelhantes.

```
[display]
lcd_type: emulated_st7920
#   Defina como "emulated_st7920" para displays emulated_st7920.
en_pin:
spi_software_sclk_pin:
spi_software_mosi_pin:
spi_software_miso_pin:
#   Os pinos conectados a um lcd do tipo emulated_st7920. O en_pin corresponde
#   ao cs_pin do lcd do tipo st7920, spi_software_sclk_pin corresponde a
#   sclk_pin e spi_software_mosi_pin corresponde a sid_pin. O
#   spi_software_miso_pin precisa ser definido como um pino não usado da placa
#   principal da impressora pois o st7920 não tem pino MISO mas a implementação
#   de spi por software exige que este pino seja configurado.
...
```

<a id="uc1701-display"></a>
#### Tela uc1701

Informações sobre configuração de telas uc1701 (usadas em telas do tipo "MKS Mini 12864").

```
[display]
lcd_type: uc1701
#   Defina como "uc1701" para displays uc1701.
cs_pin:
a0_pin:
#   Os pinos conectados a um lcd do tipo uc1701. Estes parâmetros devem ser
#   fornecidos.
#rst_pin:
#   O pino conectado ao pino "rst" no lcd. Se não for especificado, então o
#   hardware deve ter um pull-up na linha correspondente do lcd.
#contrast:
#   O contraste a definir. O valor pode variar de 0 a 63 e o padrão é 40.
...
```

<a id="ssd1306-and-sh1106-displays"></a>
#### Telas ssd1306 e sh1106

Informações sobre configuração de telas ssd1306 e sh1106.

```
[display]
lcd_type:
#   Defina como "ssd1306" ou "sh1106" para o tipo de display indicado.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Parâmetros opcionais disponíveis para displays conectados via barramento
#   i2c. Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima.
#cs_pin:
#dc_pin:
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Os pinos conectados ao lcd quando em modo spi "4-wire". Consulte a seção
#   "Configurações comuns de SPI" para uma descrição dos parâmetros que começam
#   com "spi_". O padrão é usar modo i2c para o display.
#reset_pin:
#   Um pino de reset pode ser especificado no display. Se não for especificado,
#   então o hardware deve ter um pull-up na linha correspondente do lcd.
#contrast:
#   O contraste a definir. O valor pode variar de 0 a 256 e o padrão é 239.
#vcomh: 0
#   Define o valor Vcomh no display. Este valor está associado a um efeito de
#   "smearing" em alguns displays OLED. O valor pode variar de 0 a 63. O padrão
#   é 0.
#invert: False
#   TRUE inverte os pixels em certos displays OLED. O padrão é False.
#x_offset: 0
#   Define o valor de deslocamento horizontal em displays SH1106. O padrão é 0.
...
```

<a id="display_data"></a>
### [display_data]

Suporte a exibir dados personalizados em uma tela LCD. Pode-se criar qualquer número de grupos de exibição e qualquer número de itens de dados sob esses grupos. A tela mostrará todos os itens de dados de um determinado grupo se a opção display_group na seção [display] estiver definida para o nome do grupo em questão.

Um [conjunto padrão de itens do visor](../../klippy/extras/display/display.cfg) é criado automaticamente. Pode-se substituir ou estender esses items do display_data sobrescrevendo os valores padrão no arquivo principal de configuração (printer.cfg).

```
[display_data my_group_name my_data_name]
position:
#   Linha e coluna do display separadas por vírgula que devem ser usadas para
#   mostrar as informações. Este parâmetro deve ser fornecido.
text:
#   O texto a mostrar na posição indicada. Este campo é avaliado usando modelos
#   de comando (consulte docs/Command_Templates.md). Este parâmetro deve ser
#   fornecido.
```

<a id="display_template"></a>
### [display_template]

Texto de exibição com "macros" de dados (pode-se definir qualquer número de seções com o prefixo display_template). Consulte o documento [modelos de comando](Command_Templates.md) para informações sobre avaliação de modelos.

Este recurso permite reduzir definições repetitivas em seções display_data. Pode-se usar a função embutida `render()` em seções display_data para avaliar um modelo. Por exemplo, se fosse definido `[display_template my_template]`, então poder-se-ia usar `{ render('my_template') }` em uma seção display_data.

Este recurso também pode ser usado para atualizações contínuas de LED usando o comando [SET_LED_TEMPLATE](G-Codes.md#set_led_template).

```
[display_template my_template_name]
#param_<name>:
#   Pode-se especificar qualquer número de opções com prefixo "param_". O nome
#   indicado receberá o valor indicado (interpretado como literal Python) e
#   estará disponível durante a expansão de macro. Se o parâmetro for passado
#   na chamada para render() então esse valor será usado durante a expansão de
#   macro. Por exemplo, uma configuração com "param_speed = 75" pode ter um
#   chamador com "render('my_template_name', param_speed=80)". Nomes de
#   parâmetro não podem usar caracteres maiúsculos.
text:
#   O texto a retornar quando este modelo for renderizado. Este campo é
#   avaliado usando modelos de comando (consulte docs/Command_Templates.md).
#   Este parâmetro deve ser fornecido.
```

<a id="display_glyph"></a>
### [display_glyph]

Exibe um glifo personalizado em telas que o suportam. Ao nome informado será atribuído o dado de exibição informado, que então pode ser referenciado nos modelos de exibição pelo seu nome entre dois símbolos "til" (tilde), ex.: `~my_display_glyph~`

Consulte [sample-glyphs.cfg](../../config/sample-glyphs.cfg) para alguns exemplos.

```
[display_glyph my_display_glyph]
#data:
#   Os dados do display, armazenados como 16 linhas consistindo de 16 bits (1
#   por pixel) onde '.' é um pixel em branco e '*' é um pixel aceso (por
#   exemplo, "****************" para mostrar uma linha horizontal sólida).
#   Alternativamente, pode-se usar '0' para pixel em branco e '1' para pixel
#   aceso. Coloque cada linha do display em uma linha de configuração separada.
#   O glyph deve consistir de exatamente 16 linhas com 16 bits cada. Este
#   parâmetro é opcional.
#hd44780_data:
#   Glyph a usar em displays hd44780 20x4. O glyph deve consistir de exatamente
#   8 linhas com 5 bits cada. Este parâmetro é opcional.
#hd44780_slot:
#   O índice de hardware hd44780 (0..7) para armazenar o glyph. Se múltiplas
#   imagens distintas usarem o mesmo slot então certifique-se de usar apenas
#   uma dessas imagens em qualquer tela dada. Este parâmetro é exigido se
#   hd44780_data for especificado.
```

<a id="display-my_extra_display"></a>
### [display my_extra_display]

Se uma seção primária [display] foi definida em printer.cfg como mostrado acima, é possível definir múltiplas telas auxiliares. Observe que telas auxiliares atualmente não suportam funcionalidade de menu, portanto não suportam as opções "menu" ou configuração de botões.

```
[display my_extra_display]
# Consulte a seção "display" para parâmetros disponíveis.
```

<a id="menu"></a>
### [menu]

Menus personalizáveis de tela LCD.

Um [conjunto padrão de menus](../../klippy/extras/display/menu.cfg) é automaticamente criado. Pode-se substituir ou extender o menu sobrescrevendo os valores padrão no arquivo principal de configuração (printer.cfg).

Consulte o [documento de modelos de comando](Command_Templates.md#menu-templates) para informações sobre atributos de menu disponíveis durante a renderização de modelos.

```
# Parâmetros comuns disponíveis para todas as seções de configuração de menu.
#[menu __some_list __some_name]
#type: disabled
#   Elemento de menu permanentemente desabilitado, apenas o atributo exigido é
#   'type'. Permite desabilitar/ocultar facilmente itens de menu existentes.

#[menu some_name]
#type:
#   Um entre command, input, list, text: command - elemento básico de menu com
#   vários gatilhos de script input - igual a 'command' mas tem capacidades de
#   alteração de valor. Pressionar iniciará/parará o modo de edição. list -
#   permite que itens de menu sejam agrupados em uma lista rolável. Adicione à
#   lista criando configurações de menu usando "some_list" como prefixo - para
#                 exemplo: [menu some_list some_item_in_the_list]
#       vsdlist - igual a 'list' mas anexará arquivos do sdcard virtual (será
#       removido no futuro)
#name:
#   Nome do item de menu - avaliado como modelo.
#enable:
#   Modelo que avalia para True ou False.
#index:
#   Posição onde um item precisa ser inserido na lista. Por padrão o item é
#   adicionado ao final.

#[menu some_list]
#type: list
#name:
#enable:
#   Veja acima uma descrição destes parâmetros.

#[menu some_list some_command]
#type: command
#name:
#enable:
#   Veja acima uma descrição destes parâmetros.
#gcode:
#   Script a executar em clique de botão ou clique longo. Avaliado como modelo.

#[menu some_list some_input]
#type: input
#name:
#enable:
#   Veja acima uma descrição destes parâmetros.
#input:
#   Valor inicial a usar ao editar - avaliado como modelo. O resultado deve ser
#   float.
#input_min:
#   Valor mínimo da faixa - avaliado como modelo. Padrão -99999.
#input_max:
#   Valor máximo da faixa - avaliado como modelo. Padrão 99999.
#input_step:
#   Passo de edição - Deve ser um inteiro positivo ou valor float. Tem passo
#   interno de taxa rápida. Quando "(input_max - input_min) / input_step > 100"
#   então o passo de taxa rápida é 10 * input_step senão o passo de taxa rápida
#   é o mesmo input_step.
#realtime:
#   Este atributo aceita valor booleano estático. Quando habilitado, então o
#   script gcode é executado após cada alteração de valor. O padrão é False.
#gcode:
#   Script a executar em clique de botão, clique longo ou alteração de valor.
#   Avaliado como modelo. O clique de botão acionará o início ou fim do modo de
#   edição.
```

<a id="filament-sensors"></a>
## Sensores de filamento

<a id="filament_switch_sensor"></a>
### [filament_switch_sensor]

Sensor de filamento por chave (Filament Switch Sensor). Suporte a detecção de inserção e esgotamento de filamento usando um sensor de chave, como uma chave de fim de curso.

Consulte a [referência de comandos](G-Codes.md#filament_switch_sensor) para mais informações.

```
[filament_switch_sensor my_sensor]
#pause_on_runout: True
#   Quando definido como True, um PAUSE será executado imediatamente após um
#   runout ser detectado. Observe que se pause_on_runout for False e o
#   runout_gcode for omitido então a detecção de runout é desabilitada. O
#   padrão é True.
#runout_gcode:
#   Uma lista de comandos G-Code a executar após um runout de filamento ser
#   detectado. Consulte docs/Command_Templates.md para o formato de G-Code. Se
#   pause_on_runout estiver definido como True este G-Code será executado após
#   o PAUSE estar completo. O padrão é não executar nenhum comando G-Code.
#insert_gcode:
#   Uma lista de comandos G-Code a executar após uma inserção de filamento ser
#   detectada. Consulte docs/Command_Templates.md para o formato de G-Code. O
#   padrão é não executar nenhum comando G-Code, o que desabilita a detecção de
#   inserção.
#event_delay: 3.0
#   A quantidade mínima de tempo em segundos para atrasar entre eventos.
#   Eventos acionados durante este período serão silenciosamente ignorados. O
#   padrão é 3 segundos.
#pause_delay: 0.5
#   A quantidade de tempo a atrasar, em segundos, entre o despacho do comando
#   de pausa e a execução do runout_gcode. Pode ser útil aumentar este atraso
#   se o OctoPrint exibir comportamento estranho de pausa. O padrão é 0.5
#   segundos.
#debounce_delay:
#   Um período de tempo em segundos para debounce de eventos antes de executar
#   o gcode da chave. A chave deve ser mantida em um único estado por pelo
#   menos este tempo para ativar. Se a chave for alternada on/off durante este
#   atraso, o evento é ignorado. O padrão é 0.
#switch_pin:
#   O pino ao qual a chave está conectada. Este parâmetro deve ser fornecido.
```

<a id="filament_motion_sensor"></a>
### [filament_motion_sensor]

Sensor de movimento de filamento (Filament Motion Sensor). Suporte a detecção de inserção e esgotamento de filamento usando um codificador (encoder) que alterna o pino de saída durante o movimento do filamento através do sensor.

Consulte a [referência de comandos](G-Codes.md#filament_switch_sensor) para mais informações.

```
[filament_motion_sensor my_sensor]
detection_length: 7.0
#   O comprimento mínimo de filamento puxado através do sensor para acionar uma
#   mudança de estado na switch_pin O padrão é 7 mm.
extruder:
#   O nome da seção extruder ou extruder_stepper à qual este sensor está
#   associado. Este parâmetro deve ser fornecido.
switch_pin:
#pause_on_runout:
#runout_gcode:
#insert_gcode:
#event_delay:
#pause_delay:
#   Consulte a seção "filament_switch_sensor" para uma descrição dos parâmetros
#   acima.
```

<a id="tsl1401cl_filament_width_sensor"></a>
### [tsl1401cl_filament_width_sensor]

Sensor de largura de filamento baseado em TSLl401CL. Consulte o [guia](TSL1401CL_Filament_Width_Sensor.md) para mais informações.

```
[tsl1401cl_filament_width_sensor]
#pin:
#default_nominal_filament_diameter: 1.75 # (mm)
#   Diferença máxima permitida de diâmetro do filamento como mm.
#max_difference: 0.2
#   A distância do sensor até a câmara de fusão como mm.
#measurement_delay: 100
```

<a id="hall_filament_width_sensor"></a>
### [hall_filament_width_sensor]

Sensor de largura de filamento Hall (consulte [Sensor de largura de filamento Hall](Hall_Filament_Width_Sensor.md)).

```
[hall_filament_width_sensor]
adc1:
adc2:
#   Pinos de entrada analógica conectados ao sensor. Estes parâmetros devem ser
#   fornecidos.
#cal_dia1: 1.50
#cal_dia2: 2.00
#   Os valores de calibração (em mm) para os sensores. O padrão é 1.50 para
#   cal_dia1 e 2.00 para cal_dia2.
#raw_dia1: 9500
#raw_dia2: 10500
#   Os valores brutos de calibração para os sensores. Os valores devem ser
#   diferentes. O padrão é 9500 para raw_dia1 e 10500 para raw_dia2.
#default_nominal_filament_diameter: 1.75
#   O diâmetro nominal do filamento. Este parâmetro deve ser fornecido.
#max_difference: 0.200
#   Diferença máxima permitida de diâmetro do filamento em milímetros (mm). Se
#   a diferença entre o diâmetro nominal do filamento e a saída do sensor for
#   maior que +- max_difference, o multiplicador de extrusão é retornado para
#   100%. Deve ser menor que default_nominal_filament_diameter. O padrão é
#   0.200.
#measurement_delay: 70
#   A distância do sensor até a câmara de fusão/hot-end em milímetros (mm). O
#   filamento entre o sensor e o hot-end será tratado como
#   default_nominal_filament_diameter. O módulo host trabalha com lógica FIFO.
#   Ele mantém cada valor do sensor e posição em um array e os retira de volta
#   na posição correta. Este parâmetro deve ser fornecido.
#enable: False
#   Sensor habilitado ou desabilitado após ligar. O padrão é desabilitar.
#enable_flow_compensation: True
#   Compensação de fluxo habilitada ou desabilitada. Se definida como False, o
#   sensor não modificará o multiplicador de extrusão e apenas acionará eventos
#   de runout. O padrão é True.
#measurement_interval: 10
#   A distância aproximada (em mm) entre leituras do sensor. O padrão é 10mm.
#logging: False
#   A saída de diâmetro para terminal e klipper.log pode ser ligada/desligada
#   por comando.
#min_diameter: 1.0
#   Diâmetro mínimo para acionar filament_switch_sensor virtual.
#max_diameter:
#   Diâmetro máximo para acionar filament_switch_sensor virtual. O padrão é
#   default_nominal_filament_diameter + max_difference.
#use_current_dia_while_delay: False
#   Usa o diâmetro atual em vez do diâmetro nominal enquanto o atraso de
#   medição ainda não foi percorrido.
#pause_on_runout:
#runout_gcode:
#insert_gcode:
#event_delay:
#pause_delay:
#   Consulte a seção "filament_switch_sensor" para uma descrição dos parâmetros
#   acima.
```

<a id="load-cells"></a>
## Células de carga

<a id="load_cell"></a>
### [load_cell]

Célula de carga. Usa um sensor ADC ligado a uma célula de carga para criar uma balança digital.

```
[load_cell]
sensor_type:
#   Isso deve ser um dos tipos de sensor suportados, veja abaixo.
#counts_per_gram:
#   O número float de contagens do sensor que indica 1 grama de força. Este
#   valor é calculado pelo comando LOAD_CELL_CALIBRATE.
#reference_tare_counts:
#   O valor inteiro de tara, em contagens brutas do sensor, obtido quando
#   LOAD_CELL_CALIBRATE é executado. Este é o valor padrão de tara quando o
#   klipper inicia.
#sensor_orientation:
#   Altera a orientação do sensor. Pode ser 'normal' ou 'inverted'. O padrão é
#   'normal'. Use 'inverted' se o sensor reportar um valor de força decrescente
#   quando colocado sob carga.
```

<a id="cs1237"></a>
#### CS1237

O backend de sensor CS1237 suporta tanto `[load_cell]` quanto `[load_cell_probe]`.

```
[load_cell my_cs1237]
sensor_type: cs1237
sclk_pin:
#   O pino conectado à linha de clock do CS1237. Este parâmetro deve ser
#   fornecido.
dout_pin:
#   O pino conectado à linha bidirecional DOUT do CS1237. Este parâmetro deve
#   ser fornecido.
#sample_rate: 640
#   Valores válidos para sample_rate são 10, 40, 640 ou 1280. O padrão é 640.
#gain: 128
#   Valores válidos para gain são 1, 2, 64 ou 128. O padrão é 128.
#reference_output: True
#   Habilita a saída de referência REFO do CS1237. O padrão é True.
```

Ambos os pinos devem estar no mesmo MCU. DOUT é bidirecional durante a configuração.

<a id="hx711"></a>
#### HX711

Este é um chip de 24 bits e baixa taxa de amostragem que usa comunicação "bit-bang". É adequado para balanças de filamento.

```
[load_cell]
sensor_type: hx711
sclk_pin:
#   O pino conectado à linha de clock do HX711. Este parâmetro deve ser
#   fornecido.
dout_pin:
#   O pino conectado à linha de saída de dados do HX711. Este parâmetro deve
#   ser fornecido.
#gain: A-128
#   Valores válidos para gain são: A-128, A-64, B-32. O padrão é A-128. 'A'
#   denota o canal de entrada e o número denota o ganho. Somente as 3
#   combinações listadas são suportadas pelo chip. Observe que alterar a
#   configuração de ganho também seleciona o canal sendo lido.
#sample_rate: 80
#   Valores válidos para sample_rate são 80 ou 10. O valor padrão é 80. Isso
#   deve corresponder à fiação do chip. A taxa de amostragem não pode ser
#   alterada por software.
```

<a id="hx717"></a>
#### HX717

Esta é a versão com taxa de amostragem 4x maior do HX711, adequada para sondagem.

```
[load_cell]
sensor_type: hx717
sclk_pin:
#   O pino conectado à linha de clock do HX717. Este parâmetro deve ser
#   fornecido.
dout_pin:
#   O pino conectado à linha de saída de dados do HX717. Este parâmetro deve
#   ser fornecido.
#gain: A-128
#   Valores válidos para gain são A-128, B-64, A-64, B-8. 'A' denota o canal de
#   entrada e o número denota a configuração de ganho. Somente as 4 combinações
#   listadas são suportadas pelo chip. Observe que alterar a configuração de
#   ganho também seleciona o canal sendo lido.
#sample_rate: 320
#   Valores válidos para sample_rate são: 10, 20, 80, 320. O padrão é 320. Isso
#   deve corresponder à fiação do chip. A taxa de amostragem não pode ser
#   alterada por software.
```

<a id="ads1220"></a>
#### ADS1220

O ADS1220 é um ADC de 24 bits que suporta taxa de amostragem de até 2Khz configurável via software.

```
[load_cell]
sensor_type: ads1220
cs_pin:
#   O pino conectado à linha de chip select do ADS1220. Este parâmetro deve ser
#   fornecido.
#spi_speed: 512000
#   Este chip suporta 2 velocidades: 256000 ou 512000. A velocidade mais rápida
#   só é habilitada quando uma das taxas de amostragem Turbo é usada. A
#   spi_speed correta é selecionada com base na taxa de amostragem.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
data_ready_pin:
#   Pino conectado à linha data ready do ADS1220. Este parâmetro deve ser
#   fornecido.
#gain: 128
#   Valores válidos de ganho são 128, 64, 32, 16, 8, 4, 2, 1 O padrão é 128
#pga_bypass: False
#   Desabilita o Amplificador de Ganho Programável interno. Se True o PGA será
#   desabilitado para ganhos 1, 2 e 4. O PGA está sempre habilitado para
#   configurações de ganho 8 a 128, independentemente da configuração
#   pga_bypass. Se AVSS for usado como entrada pga_bypass é forçado para True.
#   O padrão é False.
#sample_rate: 660
#   Este chip suporta duas faixas de taxas de amostragem, Normal e Turbo. Em
#   modo turbo o clock interno do chip roda duas vezes mais rápido e a
#   velocidade de comunicação SPI também é dobrada. Taxas de amostragem
#   normais: 20, 45, 90, 175, 330, 600, 1000 Taxas de amostragem Turbo: 40, 90,
#   180, 350, 660, 1200, 2000 O padrão é 660
#input_mux:
#   Configuração do multiplexador de entrada, seleciona um par de pinos a usar.
#   O primeiro pino é o positivo, AINP, e o segundo pino é o negativo, AINN.
#   Valores válidos são: 'AIN0_AIN1', 'AIN0_AIN2', 'AIN0_AIN3', 'AIN1_AIN2',
#   'AIN1_AIN3', 'AIN2_AIN3', 'AIN1_AIN0', 'AIN3_AIN2', 'AIN0_AVSS',
#   'AIN1_AVSS', 'AIN2_AVSS' e 'AIN3_AVSS'. Se AVSS for usado o PGA é ignorado
#   e a configuração pga_bypass será forçada para True. O padrão é AIN0_AIN1.
#vref:
#   A referência de tensão selecionada. Valores válidos são: 'internal',
#   'REF0', 'REF1' e 'analog_supply'. O padrão é 'internal'.
```

<a id="ads131m0x"></a>
#### ADS131M0x

O ADS131M0x é uma família de ADCs delta-sigma rápidos de 24 bits. Dois sensores desta família são suportados: ADS131M02 com dois canais diferenciais de amostragem simultânea e ADS131M04 com quatro canais. Eles possuem amplificador de ganho programável (PGA) com ganhos de até 128, taxas de amostragem configuráveis de até 64000 amostras por segundo, e requerem entrada de relógio externo (300 kHz a 8.4 MHz, 8.192 MHz nominal).

```
[load_cell]
sensor_type: ads131m02
#   Selecione 'ads131m02' para a variante de 2 canais ou 'ads131m04' para a
#   variante de 4 canais. Este parâmetro deve ser fornecido.
cs_pin:
#   O pino conectado à linha de chip select. Este parâmetro deve ser fornecido.
#spi_speed: 4000000
#   A velocidade do barramento SPI. O padrão é 4 MHz.
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
data_ready_pin:
#   Pino conectado à linha data ready (DRDY). Este parâmetro deve ser
#   fornecido.
#adc_channel: 0
#   O canal ADC a ler. Para o ADS131M02, valores válidos são 0 e 1. Para o
#   ADS131M04, valores válidos são 0, 1, 2 e 3. O padrão é 0.
#clock_freq:
#   A frequência de clock externo (fCLKIN) em Hz fornecida ao pino CLKIN. A
#   faixa válida é 300000 a 8400000. A frequência nominal de clock para o
#   ADS131M0x é 8192000 Hz; recomenda-se usar uma fonte de clock próxima a esta
#   frequência. clock_freq ou pwm_clock deve ser fornecido.
#pwm_clock:
#   Referência a uma seção [static_pwm_clock] que gera o sinal de clock para o
#   pino CLKIN. A frequência deste clock é usada como fCLKIN. clock_freq ou
#   pwm_clock deve ser fornecido.
#sample_rate: 500.0
#   A taxa de amostragem de saída desejada em amostras por segundo. O firmware
#   selecionará a taxa disponível mais próxima, se possível. Quando a
#   frequência nominal de clock de 8192000 Hz é usada e o modo global-chop está
#   desabilitado, as seguintes taxas estão disponíveis: 250, 500, 1000, 2000,
#   4000, 8000, 16000, 32000 e 64000. A taxa de amostragem efetiva real pode
#   ser verificada via comando LOAD_CELL_DIAGNOSTIC. O padrão é 500.
#gain: 128
#   A configuração de ganho do PGA. Valores válidos são: 1, 2, 4, 8, 16, 32, 64
#   e 128. O padrão é 128.
#enable_global_chop: False
#   Habilita o modo global-chop para reduzir erros internos de offset do
#   sistema calculando a média de duas conversões com polaridades de entrada
#   opostas. O padrão é False.
#global_chop_delay: 16
#   O atraso de global-chop em períodos de clock do modulador, usado somente
#   quando enable_global_chop é True. Valores maiores permitem mais tempo de
#   estabilização entre trocas de entrada. Valores válidos são todas as
#   potências de 2 de 2 a 65536. O padrão é 16.
```

<a id="load_cell_probe"></a>
### [load_cell_probe]

Sonda de célula de carga. Isso combina a funcionalidade de [probe] e [load_cell].

```
[load_cell_probe]
sensor_type:
#   Este deve ser um dos tipos suportados de sensor bulk ADC e suportar fins de
#   curso de célula de carga no mcu.
#counts_per_gram:
#reference_tare_counts:
#sensor_orientation:
#   Estes parâmetros devem ser configurados antes que a sonda opere. Consulte a
#   seção [load_cell] para mais detalhes.
#force_safety_limit: 2000
#   O limite seguro para a força de sondagem em relação a reference_tare_counts
#   em load_cell. O padrão é +/-2Kg.
#trigger_force: 75.0
#   A força na qual a sonda será acionada. 75g é o padrão.
#drift_filter_cutoff_frequency: 0.8
#   Habilita tara contínua opcional durante homing (referenciamento) & sondagem
#   para rejeitar deriva. O valor é uma frequência, em Hz, abaixo da qual a
#   deriva será ignorada. Esta opção requer a biblioteca SciPy. Padrão: None
#drift_filter_delay: 2
#   O atraso, ou 'order', do filtro de deriva. Isso controla o número de
#   amostras necessárias para fazer uma detecção de acionamento. Pode ser 1 ou
#   2, o padrão é 2.
#buzz_filter_cutoff_frequency: 100.0
#   O valor é uma frequência, em Hz, acima da qual o ruído de alta frequência
#   na célula de carga será filtrado e ignorado. Esta opção requer a biblioteca
#   SciPy. Padrão: None
#buzz_filter_delay: 2
#   O atraso, ou 'order', do filtro de buzz. Isso controla o número de amostras
#   necessárias para fazer uma detecção de acionamento. Pode ser 1 ou 2, o
#   padrão é 2.
#notch_filter_frequencies: 50, 60
#   1 ou 2 frequências, em Hz, para filtrar dos dados da célula de carga. Isso
#   se destina a rejeitar ruído da rede elétrica. Esta opção requer a
#   biblioteca SciPy. Padrão: None
#notch_filter_quality: 2.0
#   Controla quão estreita é a faixa de frequências que o filtro notch remove.
#   Números maiores produzem um filtro mais estreito. O valor mínimo é 0.5 e o
#   máximo é 3.0. Padrão: 2.0
#tare_time:
#   O tempo em segundos usado para tarar load_cell antes de cada sondagem. O
#   valor padrão é: 4 / 60 = 0.066. Isso coleta amostras de 4 ciclos da rede
#   elétrica de 60Hz para cancelar o ruído da rede elétrica.
#z_offset:
#speed:
#samples:
#sample_retract_dist:
#lift_speed:
#samples_result:
#samples_tolerance:
#samples_tolerance_retries:
#activate_gcode:
#deactivate_gcode:
#   Consulte a seção "[probe]" para uma descrição dos parâmetros acima.
```

<a id="board-specific-hardware-support"></a>
## Suporte a hardware específico de placas

<a id="sx1509"></a>
### [sx1509]

Configura um expansor I2C para GPIO SX1509. Devido ao atraso imposto pela comunicação I2C, NÃO se deve usar pinos SX1509 como pinos de enable, step ou dir de motor de passo ou qualquer outro pino que exija bit-banging rápido. Eles são mais bem usados como saídas digitais estáticas ou controladas por G-code ou pinos de hardware-pwm para, por exemplo, ventiladores. Pode-se definir qualquer número de seções com o prefixo "sx1509". Cada expansor fornece um conjunto de 16 pinos (sx1509_my_sx1509:PIN_0 a sx1509_my_sx1509:PIN_15) que podem ser usados na configuração da impressora.

Consulte o arquivo [generic-duet2-duex.cfg](../../config/generic-duet2-duex.cfg) para um exemplo.

```
[sx1509 my_sx1509]
i2c_address:
#   Endereço I2C usado por este expansor. Dependendo dos jumpers de hardware,
#   este é um dos seguintes endereços: 62 63 112 113. Este parâmetro deve ser
#   fornecido.
#i2c_mcu:
#i2c_bus:
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#i2c_speed:
#   Consulte a seção "Configurações comuns de I2C" para uma descrição dos
#   parâmetros acima.
```

<a id="samd_sercom"></a>
### [samd_sercom]

Configuração SAMD SERCOM para especificar quais pinos usar em um determinado SERCOM. Pode-se definir qualquer número de seções com o prefixo "samd_sercom". Cada SERCOM deve ser configurado antes de usá-lo como periférico SPI ou I2C. Coloque esta seção de configuração acima de qualquer outra seção que utilize barramentos SPI ou I2C.

```
[samd_sercom my_sercom]
sercom:
#   O nome do barramento sercom a ser configurado no microcontrolador. Os nomes
#   disponíveis são "sercom0", "sercom1", etc.. Este parâmetro deve ser
#   fornecido.
tx_pin:
#   Pino MOSI para comunicação SPI, ou pino SDA (data) para comunicação I2C. O
#   pino deve ter uma configuração pinmux válida para o periférico SERCOM
#   fornecido. Este parâmetro deve ser fornecido.
#rx_pin:
#   Pino MISO para comunicação SPI. Este pino não é usado para comunicação I2C
#   (I2C usa tx_pin tanto para envio quanto para recebimento). O pino deve ter
#   uma configuração pinmux válida para o periférico SERCOM fornecido. Este
#   parâmetro é opcional.
clk_pin:
#   Pino CLK para comunicação SPI, ou pino SCL (clock) para comunicação I2C. O
#   pino deve ter uma configuração pinmux válida para o periférico SERCOM
#   fornecido. Este parâmetro deve ser fornecido.
```

<a id="adc_scaled"></a>
### [adc_scaled]

Escalonamento analógico Duet2 Maestro por leituras de vref e vssa. Definir uma seção adc_scaled habilita pinos ADC virtuais (como "my_name:PB0") que são ajustados automaticamente pelos pinos de monitoramento vref e vssa da placa. Certifique-se de definir esta seção de configuração acima de quaisquer seções de configuração que usem um destes pinos virtuais.

Consulte o arquivo [generic-duet2-maestro.cfg](../../config/generic-duet2-maestro.cfg) para um exemplo.

```
[adc_scaled my_name]
vref_pin:
#   O pino ADC a ser usado para monitoramento de VREF. Este parâmetro deve ser
#   fornecido.
vssa_pin:
#   O pino ADC a ser usado para monitoramento de VSSA. Este parâmetro deve ser
#   fornecido.
#smooth_time: 2.0
#   Um valor de tempo (em segundos) durante o qual as medições de vref e vssa
#   serão suavizadas para reduzir o impacto do ruído de medição. O padrão é 2
#   segundos.
```

<a id="ads1x1x"></a>
### [ads1x1x]

ADS1013, ADS1014, ADS1015, ADS1113, ADS1114 e ADS1115 são conversores analógico-digitais baseados em I2C que podem ser usados para sensores de temperatura. Eles fornecem 4 pinos de entrada analógica, seja como linha simples ou como entrada diferencial.

Nota: Tenha cuidado ao usar este sensor para controlar aquecedores. Os valores min_temp e max_temp do aquecedor são verificados apenas no hospedeiro e somente se o hospedeiro estiver em execução e operando normalmente. (Entradas ADC conectadas diretamente ao microcontrolador verificam min_temp e max_temp dentro do microcontrolador e não exigem uma conexão funcional com o hospedeiro.)

```
[ads1x1x my_ads1x1x]
chip: ADS1115
#pga: 4.096V
#   O valor padrão é 4.096V. A faixa máxima de tensão usada para a entrada.
#   Isso escala todos os valores lidos do ADC. As opções são: 6.144V, 4.096V,
#   2.048V, 1.024V, 0.512V, 0.256V
#adc_voltage: 3.3
#   A tensão de alimentação do dispositivo. Isso permite escala adicional por
#   software para todos os valores lidos do ADC.
i2c_mcu: host
i2c_bus: i2c.1
#address_pin: GND
#   O valor padrão é GND. Pode haver até quatro dispositivos endereçados
#   dependendo da fiação do dispositivo. Verifique o datasheet para detalhes. O
#   i2c_address pode ser especificado diretamente em vez de usar address_pin.
```

O chip fornece pinos que podem ser usados em outros sensores.

```
sensor_type: ...
#   Pode ser qualquer thermistor ou adc_temperature.
sensor_pin: my_ads1x1x:AIN0
#   Uma combinação do nome do chip ads1x1x e do pino. Os valores de pino
#   possíveis são AIN0, AIN1, AIN2 e AIN3 para linhas single ended e DIFF01,
#   DIFF03, DIFF13 e DIFF23 para diferencial entre suas linhas correspondentes.
#   Por exemplo DIFF03 mede o diferencial entre a linha 0 e 3. Somente
#   combinações específicas para os diferenciais são permitidas.
```

<a id="replicape"></a>
### [replicape]

Suporte a Replicape - consulte o [guia beaglebone](Beaglebone.md) e o arquivo [generic-replicape.cfg](../../config/generic-replicape.cfg) para um exemplo.

```
# A seção de configuração "replicape" adiciona pinos virtuais de habilitação de
# motor de passo "replicape:stepper_x_enable" (para motores de passo X, Y, Z, E
# e H) e pinos de saída PWM "replicape:power_x" (para hotbed, e, h, fan0, fan1,
# fan2 e fan3) que podem então ser usados em outro lugar no arquivo de
# configuração.
[replicape]
revision:
#   A revisão de hardware replicape. Atualmente somente a revisão "B3" é
#   suportada. Este parâmetro deve ser fornecido.
#enable_pin: !gpio0_20
#   O pino de habilitação global replicape. O padrão é !gpio0_20 (aka P9_41).
host_mcu:
#   O nome da seção de configuração mcu que se comunica com a instância mcu de
#   "linux process" do Klipper. Este parâmetro deve ser fornecido.
#standstill_power_down: False
#   Este parâmetro controla a linha CFG6_ENN em todos os motores de passo. True
#   define as linhas de habilitação como "open". O padrão é False.
#stepper_x_microstep_mode:
#stepper_y_microstep_mode:
#stepper_z_microstep_mode:
#stepper_e_microstep_mode:
#stepper_h_microstep_mode:
#   Este parâmetro controla os pinos CFG1 e CFG2 do driver de motor de passo
#   fornecido. As opções disponíveis são: disable, 1, 2, spread2, 4, 16,
#   spread4, spread16, stealth4 e stealth16. O padrão é disable.
#stepper_x_current:
#stepper_y_current:
#stepper_z_current:
#stepper_e_current:
#stepper_h_current:
#   A corrente máxima configurada (em amperes) do driver de motor de passo.
#   Este parâmetro deve ser fornecido se o motor de passo não estiver em um
#   modo disable.
#stepper_x_chopper_off_time_high:
#stepper_y_chopper_off_time_high:
#stepper_z_chopper_off_time_high:
#stepper_e_chopper_off_time_high:
#stepper_h_chopper_off_time_high:
#   Este parâmetro controla o pino CFG0 do driver de motor de passo (True
#   define CFG0 em nível alto, False define em nível baixo). O padrão é False.
#stepper_x_chopper_hysteresis_high:
#stepper_y_chopper_hysteresis_high:
#stepper_z_chopper_hysteresis_high:
#stepper_e_chopper_hysteresis_high:
#stepper_h_chopper_hysteresis_high:
#   Este parâmetro controla o pino CFG4 do driver de motor de passo (True
#   define CFG4 em nível alto, False define em nível baixo). O padrão é False.
#stepper_x_chopper_blank_time_high:
#stepper_y_chopper_blank_time_high:
#stepper_z_chopper_blank_time_high:
#stepper_e_chopper_blank_time_high:
#stepper_h_chopper_blank_time_high:
#   Este parâmetro controla o pino CFG5 do driver de motor de passo (True
#   define CFG5 em nível alto, False define em nível baixo). O padrão é True.
```

<a id="other-custom-modules"></a>
## Outros módulos personalizados

<a id="palette2"></a>
### [palette2]

Suporte multmaterial Palette 2 - fornece uma integração mais estreita com suporte a dispositivos Palette 2 em modo conectado.

Este módulo também requer `[virtual_sdcard]` e `[pause_resume]` para funcionalidade completa.

Se você usar este módulo, não use o plugin Palette 2 para Octoprint, pois eles entrarão em conflito, e 1 falhará ao inicializar corretamente, provavelmente abortando sua impressão.

Se você usa Octoprint e transmite gcode pela porta serial em vez de imprimir a partir do virtual_sd, remover **M1** e **M0** de *comandos de pausa* em *Settings > Serial Connection > Firmware & protocol* evitará a necessidade de iniciar a impressão na Palette 2 e retomar no Octoprint para sua impressão começar.

```
[palette2]
serial:
#   A porta serial para conectar ao Palette 2.
#baud: 115200
#   A taxa de baud a ser usada. O padrão é 115200.
#feedrate_splice: 0.8
#   A velocidade de avanço a ser usada durante emenda, o padrão é 0.8
#feedrate_normal: 1.0
#   A velocidade de avanço a ser usada após emenda, o padrão é 1.0
#auto_load_speed: 2
#   Velocidade de avanço de extrusão durante carregamento automático, o padrão
#   é 2 (mm/s)
#auto_cancel_variation: 0.1
#   Cancela automaticamente a impressão quando a variação de ping está acima
#   deste limite
```

<a id="angle"></a>
### [angle]

Suporte a sensor de ângulo magnético hall para leitura de medições do eixo do motor de passo usando chips SPI a1333, as5047d, mt6816, mt6826s ou tle5012b. As medições estão disponíveis via [Servidor de API](API_Server.md) e [ferramenta de análise de movimento](Debugging.md#motion-analysis-and-data-logging). Consulte a [referência de G-Code](G-Codes.md#angle) para os comandos disponíveis.

```
[angle my_angle_sensor]
sensor_type:
#   O tipo do chip sensor hall magnético. As opções disponíveis são "a1333",
#   "as5047d", "mt6816", "mt6826s" e "tle5012b". Este parâmetro deve ser
#   especificado.
#sample_period: 0.000400
#   O período de consulta (em segundos) a ser usado durante medições. O padrão
#   é 0.000400 (que é 2500 amostras por segundo).
#stepper:
#   O nome do stepper ao qual o sensor de ângulo está conectado (por exemplo,
#   "stepper_x"). Definir este valor habilita uma ferramenta de calibração de
#   ângulo. Para usar este recurso, o pacote Python "numpy" deve estar
#   instalado. O padrão é não habilitar calibração de ângulo para o sensor de
#   ângulo.
cs_pin:
#   O pino de habilitação SPI do sensor. Este parâmetro deve ser fornecido.
#spi_speed:
#spi_bus:
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Consulte a seção "Configurações comuns de SPI" para uma descrição dos
#   parâmetros acima.
```

<a id="common-bus-parameters"></a>
## Parâmetros comuns para barramentos

<a id="common-spi-settings"></a>
### Definições comuns para SPI

Os parâmetros a seguir estão geralmente disponíveis para dispositivos que usam barramento SPI.

```
#spi_speed:
#   A velocidade SPI (em hz) a ser usada ao comunicar com o dispositivo. O
#   padrão depende do tipo de dispositivo.
#spi_bus:
#   Se o microcontrolador suporta múltiplos barramentos SPI, pode-se
#   especificar o nome do barramento do microcontrolador aqui. O padrão depende
#   do tipo de microcontrolador.
#spi_software_sclk_pin:
#spi_software_mosi_pin:
#spi_software_miso_pin:
#   Especifique os parâmetros acima para usar "SPI por software". Este modo não
#   requer suporte de hardware do microcontrolador (normalmente quaisquer pinos
#   de uso geral podem ser usados). O padrão é não usar "SPI por software".
```

<a id="common-i2c-settings"></a>
### Definições comuns para I2C

Os parâmetros a seguir estão geralmente disponíveis para dispositivos que usam barramento I2C.

Observe que o suporte atual do Klipper a I2C no microcontrolador geralmente não tolera ruído na linha. Erros inesperados nos fios I2C podem fazer o Klipper gerar um erro em tempo de execução. O suporte do Klipper à recuperação de erros varia entre cada tipo de microcontrolador. Geralmente é recomendado usar apenas dispositivos I2C que estejam na mesma placa de circuito impresso do microcontrolador.

A maioria das implementações de microcontrolador do Klipper suporta apenas `i2c_speed` de 100000 (*modo padrão*, 100kbit/s). O microcontrolador "Linux" do Klipper suporta velocidade 400000 (*modo rápido*, 400kbit/s), mas ela deve ser [configurada no sistema operacional](RPi_microcontroller.md#optional-enabling-i2c) e o parâmetro `i2c_speed` é caso contrário ignorado. O microcontrolador "RP2040" do Klipper e a família AVR ATmega e alguns STM32 (F0, G0, G4, L4, F7, H7) suportam taxa de 400000 via parâmetro `i2c_speed`. Todos os outros microcontroladores do Klipper usam taxa 100000 e ignoram o parâmetro `i2c_speed`.

```
#i2c_address:
#   O endereço i2c do dispositivo. Isso deve ser especificado como um número
#   decimal (não em hexadecimal). O padrão depende do tipo de dispositivo.
#i2c_mcu:
#   O nome do microcontrolador ao qual o chip está conectado. O padrão é "mcu".
#i2c_bus:
#   Se o microcontrolador suporta múltiplos barramentos I2C, pode-se
#   especificar o nome do barramento do microcontrolador aqui. O padrão depende
#   do tipo de microcontrolador.
#i2c_software_scl_pin:
#i2c_software_sda_pin:
#   Especifique estes parâmetros para usar suporte I2C "bit-banging" por
#   software do microcontrolador. Os dois parâmetros devem ser os dois pinos no
#   microcontrolador a serem usados para os fios scl e sda. O padrão é usar
#   suporte I2C baseado em hardware conforme especificado pelo parâmetro
#   i2c_bus.
#i2c_speed:
#   A velocidade I2C (em Hz) a ser usada ao comunicar com o dispositivo. A
#   implementação do Klipper na maioria dos microcontroladores é codificada
#   como 100000 e alterar este valor não tem efeito. O padrão é 100000. Linux,
#   RP2040 e ATmega suportam 400000.
```
