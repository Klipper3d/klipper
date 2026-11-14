<a id="code-overview"></a>
# Visão geral do código

[English](../Code_Overview.md) | **Português brasileiro**

Este documento descreve o layout de código geral e o fluxo de código principal do Klipper.

<a id="directory-layout"></a>
## Layout dos diretórios

O diretório **src/** contém a fonte C do código do microcontrolador. Os diretórios **src/atsam/**, **src/atsamd/**, **src/avr/**, **src/linux/**, **src/lpc176x/**, **src/pru/** e **src/stm32/** contêm código de microcontrolador específico da arquitetura. O **src/simulator/** contém esboços de código que permitem que o microcontrolador seja compilado para teste em outras arquiteturas. O diretório **src/generic/** contém código auxiliar que pode ser útil em diferentes arquiteturas. A compilação providencia que inclusões de "board/somefile.h" procurem primeiro no diretório da arquitetura atual (por ex., src/avr/somefile.h) e depois no diretório genérico (por ex., src/generic/somefile.h).

O diretório **klippy/** contém o software hospedeiro. A maior parte do software hospedeiro é escrita em Python, no entanto o diretório **klippy/chelper/** contém algum código auxiliar em C. O diretório **klippy/kinematics/** contém o código de cinemática do robô. O diretório **klippy/extras/** contém os "módulos" extensíveis do código hospedeiro.

O diretório **lib/** contém código de bibliotecas externas de terceiros necessário para compilar alguns alvos.

O diretório **config/** contém arquivos de configuração de impressora de exemplo.

O diretório **scripts/** contém scripts de tempo de compilação úteis para compilar o código do microcontrolador.

O diretório **test/** contém casos de teste automatizados.

Durante a compilação, a compilação pode criar um diretório **out/**. Ele contém objetos temporários de tempo de compilação. O objeto final do microcontrolador que é compilado é **out/klipper.elf.hex** no AVR e **out/klipper.bin** no ARM.

<a id="micro-controller-code-flow"></a>
## Fluxo do código do microcontrolador

A execução do código do microcontrolador começa em código específico da arquitetura (por ex., **src/avr/main.c**) que por fim chama sched_main() localizado em **src/sched.c**. O código sched_main() começa executando todas as funções marcadas com a macro DECL_INIT(). Em seguida, passa a executar repetidamente todas as funções marcadas com a macro DECL_TASK().

Uma das principais funções de tarefa é command_dispatch() localizada em **src/command.c**. Essa função é chamada a partir do código de entrada/saída específico da placa (por ex., **src/avr/serial.c**, **src/generic/serial_irq.c**) e executa as funções de comando associadas aos comandos encontrados no fluxo de entrada. As funções de comando são declaradas usando a macro DECL_COMMAND() (consulte o documento [protocolo](Protocol.md) para mais informações).

As funções de tarefa, inicialização e comando sempre são executadas com interrupções habilitadas (no entanto, podem desabilitar temporariamente as interrupções se necessário). Essas funções devem evitar pausas longas, atrasos ou trabalho que dure um tempo significativo. (Longos atrasos nessas funções de "tarefa" resultam em instabilidade de agendamento para outras "tarefas" - atrasos acima de 100us podem se tornar perceptíveis, atrasos acima de 500us podem resultar em retransmissões de comando, atrasos acima de 100ms podem resultar em reinicializações do watchdog.) Essas funções agendam trabalho em momentos específicos agendando temporizadores.

As funções de temporizador são agendadas chamando sched_add_timer() (localizado em **src/sched.c**). O código do agendador providenciará que a função dada seja chamada no tempo de relógio solicitado. As interrupções de temporizador são inicialmente tratadas em um manipulador de interrupção específico da arquitetura (por ex., **src/avr/timer.c**) que chama sched_timer_dispatch() localizado em **src/sched.c**. A interrupção do temporizador leva à execução das funções de temporizador agendadas. As funções de temporizador sempre são executadas com interrupções desabilitadas. As funções de temporizador devem sempre ser concluídas dentro de poucos microssegundos. Ao concluir o evento de temporizador, a função pode optar por reagendar a si mesma.

Caso um erro seja detectado, o código pode invocar shutdown() (uma macro que chama sched_shutdown() localizado em **src/sched.c**). Invocar shutdown() faz com que todas as funções marcadas com a macro DECL_SHUTDOWN() sejam executadas. As funções de desligamento sempre são executadas com interrupções desabilitadas.

Grande parte da funcionalidade do microcontrolador envolve trabalhar com pinos de Entrada/Saída de Uso Geral (GPIO). Para abstrair o código de baixo nível específico da arquitetura do código de tarefa de alto nível, todos os eventos GPIO são implementados em invólucros específicos da arquitetura (por ex., **src/avr/gpio.c**). O código é compilado com a otimização "-flto -fwhole-program" do gcc, que faz um excelente trabalho ao embutir funções entre unidades de compilação, de modo que a maioria dessas pequenas funções gpio são embutidas em seus chamadores, e não há custo em tempo de execução por usá-las.

<a id="klippy-code-overview"></a>
## Visão geral do código do Klippy

O código hospedeiro (Klippy) destina-se a ser executado em um computador de baixo custo (como um Raspberry Pi) emparelhado com o microcontrolador. O código é escrito principalmente em Python, no entanto usa CFFI para implementar algumas funcionalidades em código C.

A execução inicial começa em **klippy/klippy.py**. Ele lê os argumentos de linha de comando, abre o arquivo de configuração da impressora, instancia os principais objetos da impressora e inicia a conexão serial. A execução principal dos comandos G-code está no método _process_commands() em **klippy/gcode.py**. Esse código traduz os comandos G-code em chamadas de objetos da impressora, que frequentemente traduzem as ações em comandos a serem executados no microcontrolador (conforme declarado pela macro DECL_COMMAND no código do microcontrolador).

Há várias threads no código host do Klipper:

* Existe uma "main thread" Python que trata os comandos G-Code recebidos e é o ponto de partida para a maioria das ações. Esta thread executa o [reactor](https://en.wikipedia.org/wiki/Reactor_pattern) (**klippy/reactor.py**) e a maioria das ações de alto nível se origina de retornos de chamada (callbacks) de eventos de IO e temporizadores desse reactor.
* Uma thread para escrever mensagens no log para que as outras threads não bloqueiem nas escritas de log. Esta thread reside no código **klippy/queuelogger.py** e sua natureza multithread não é exposta à thread Python principal.
* Uma thread por microcontrolador que executa a leitura e escrita de baixo nível das mensagens para aquele microcontrolador. Ela reside no código C **klippy/chelper/serialqueue.c** e sua natureza multithread não é exposta ao código Python.
* Uma thread por microcontrolador para processar mensagens recebidas daquele microcontrolador no código Python. Esta thread é criada em **klippy/serialhdl.py**. Deve-se ter cuidado nos retornos de chamada (callbacks) Python invocados a partir desta thread, pois esta thread pode interagir diretamente com a thread Python principal.
* Uma thread por motor de passo que calcula a temporização dos pulsos de passo do motor de passo e comprime esses tempos. Esta thread reside no código C **klippy/chelper/steppersync.c** e sua natureza multithread não é exposta ao código Python.

<a id="code-flow-of-a-move-command"></a>
## Fluxo de código de um comando de movimento

Um movimento típico da impressora começa quando um comando "G1" é enviado ao hospedeiro Klippy e termina quando os pulsos de passo correspondentes são produzidos no microcontrolador. Esta seção descreve o fluxo de código de um comando de movimento típico. O documento [cinemática](Kinematics.md) fornece mais informações sobre a mecânica dos movimentos.

* O processamento de um comando de movimento começa em gcode.py. O objetivo de gcode.py é traduzir G-code em chamadas internas. Um comando G1 invocará cmd_G1() em klippy/extras/gcode_move.py. O código gcode_move.py trata mudanças de origem (por ex., G92), mudanças entre posições relativas e absolutas (por ex., G90) e mudanças de unidade (por ex., F6000=100mm/s). O caminho do código para um movimento é: `_process_data() -> _process_commands() -> cmd_G1()`. Por fim, a classe ToolHead é invocada para executar a requisição real: `cmd_G1() -> ToolHead.move()`
* A classe ToolHead (em toolhead.py) trata o "look-ahead" e acompanha a temporização das ações de impressão. O caminho principal do código para um movimento é: `ToolHead.move() -> LookAheadQueue.add_move()`, depois `ToolHead.move() -> ToolHead._process_lookahead() -> LookAheadQueue.flush() -> Move.set_junction()`, e então `ToolHead._process_lookahead() -> trapq_append()`.

   * ToolHead.move() cria um objeto Move() com os parâmetros do movimento (em espaço cartesiano e em unidades de segundos e milímetros).
   * A classe de cinemática tem a oportunidade de auditar cada movimento (`ToolHead.move() -> kin.check_move()`). As classes de cinemática estão localizadas no diretório klippy/kinematics/. O código check_move() pode gerar um erro se o movimento não for válido. Se check_move() for concluído com sucesso, então a cinemática subjacente deve ser capaz de tratar o movimento.
   * LookAheadQueue.add_move() coloca o objeto de movimento na fila de "look-ahead".
   * LookAheadQueue.flush() determina as velocidades inicial e final de cada movimento.
   * Move.set_junction() implementa o "gerador de trapézio" em um movimento. O "gerador de trapézio" divide cada movimento em três partes: uma fase de aceleração constante, seguida por uma fase de velocidade constante, seguida por uma fase de desaceleração constante. Cada movimento contém essas três fases nesta ordem, mas algumas fases podem ter duração zero.
   * Quando ToolHead._process_lookahead() é retomado, tudo sobre o movimento é conhecido - sua posição inicial, sua posição final, sua aceleração, suas velocidades inicial/de cruzeiro/final e a distância percorrida durante aceleração/cruzeiro/desaceleração. Todas as informações são armazenadas na classe Move() e estão em espaço cartesiano em unidades de milímetros e segundos.
   * Os movimentos são então colocados em uma "fila de movimento trapezoidal" via trapq_append() (em klippy/chelper/trapq.c). A trapq armazena todas as informações da classe Move() em uma struct C acessível ao código C do host.
* Observe que a extrusora é tratada em sua própria classe de cinemática: `ToolHead._process_lookahead() -> PrinterExtruder.process_move()`. Como a classe Move() especifica o tempo exato do movimento e como os pulsos de passo são enviados ao microcontrolador com temporização específica, os movimentos do motor de passo produzidos pela classe da extrusora estarão sincronizados com o movimento do cabeçote mesmo que o código seja mantido separado.
* Por razões de eficiência, o movimento do motor de passo é gerado no código C em uma thread por motor de passo. As threads são notificadas quando os passos devem ser gerados pelo módulo motion_queuing (klippy/extras/motion_queuing.py): `PrinterMotionQueuing._flush_handler() -> PrinterMotionQueuing._advance_move_time() -> steppersyncmgr_gen_steps() -> se_start_gen_steps()`.
* O Klipper usa um [solucionador iterativo](https://en.wikipedia.org/wiki/Root-finding_algorithm) para gerar os tempos de passo para cada motor de passo. Os tempos de passo são gerados a partir da thread de segundo plano (klippy/chelper/steppersync.c): `se_background_thread() -> se_generate_steps() -> itersolve_generate_steps() -> itersolve_gen_steps_range()` (em klippy/chelper/itersolve.c). O objetivo do solucionador iterativo é encontrar tempos de passo dada uma função que calcula a posição de um motor de passo a partir de um tempo. Isso é feito "adivinhando" repetidamente vários tempos até que a fórmula de posição do motor de passo retorne a posição desejada do próximo passo no motor de passo. O retorno (feedback) produzido a partir de cada palpite é usado para melhorar palpites futuros de modo que o processo convirja rapidamente para o tempo desejado. As fórmulas de posição cinemática do motor de passo estão localizadas no diretório klippy/chelper/ (por ex., kin_cart.c, kin_corexy.c, kin_delta.c, kin_extruder.c).
* Após o solucionador iterativo calcular os tempos de passo, eles são adicionados a um array: `itersolve_gen_steps_range() -> stepcompress_append()` (em klippy/chelper/stepcompress.c). O array (struct stepcompress.queue) armazena os tempos correspondentes do contador de clock do microcontrolador para cada passo. Aqui o valor do "contador de clock do microcontrolador" corresponde diretamente ao contador de hardware do microcontrolador - ele é relativo a quando o microcontrolador foi ligado pela última vez.
* O próximo passo principal é comprimir os passos: `stepcompress_flush() -> compress_bisect_add()` (em klippy/chelper/stepcompress.c). Este código gera e codifica uma série de comandos "queue_step" do microcontrolador que correspondem à lista de tempos de passo do motor de passo construída na etapa anterior. Esses comandos "queue_step" são então enfileirados, priorizados e enviados ao microcontrolador (via steppersync.c:steppersync e serialqueue.c:serialqueue).
* O processamento dos comandos queue_step no microcontrolador começa em src/command.c, que analisa o comando e chama `command_queue_step()`. O código command_queue_step() (em src/stepper.c) apenas anexa os parâmetros de cada comando queue_step a uma fila por motor de passo. Em operação normal, o comando queue_step é analisado e enfileirado pelo menos 100ms antes do horário de seu primeiro passo. Por fim, a geração dos eventos do motor de passo é feita em `stepper_event()`. Ela é chamada a partir da interrupção do temporizador de hardware no horário programado do primeiro passo. O código stepper_event() gera um pulso de passo e então se reagenda para executar no horário do próximo pulso de passo para os parâmetros queue_step dados. Os parâmetros para cada comando queue_step são "interval", "count" e "add". Em alto nível, stepper_event() executa o seguinte, 'count' vezes: `do_step(); next_wake_time = last_wake_time + interval; interval += add;`

O acima pode parecer muita complexidade para executar um movimento. No entanto, as únicas partes realmente interessantes estão nas classes ToolHead e de cinemática. É essa parte do código que especifica os movimentos e suas temporizações. As demais partes do processamento são principalmente apenas comunicação e encanamento.

<a id="adding-a-host-module"></a>
## Adicionando um módulo hospedeiro

O código hospedeiro Klippy tem capacidade de carregamento dinâmico de módulos. Se uma seção de configuração chamada "[my_module]" for encontrada no arquivo de configuração da impressora, o software tentará automaticamente carregar o módulo python klippy/extras/my_module.py . Este sistema de módulos é o método preferido para adicionar nova funcionalidade ao Klipper.

A maneira mais fácil de adicionar um novo módulo é usar um módulo existente como referência - veja **klippy/extras/servo.py** como exemplo.

O seguinte também pode ser útil:

* A execução do módulo começa na função `load_config()` de nível de módulo (para seções de configuração da forma [my_module]) ou em `load_config_prefix()` (para seções de configuração da forma [my_module my_name]). Esta função recebe um objeto "config" e deve retornar um novo "objeto de impressora" associado à seção de configuração dada.
* Durante o processo de instanciação de um novo objeto de impressora, o objeto config pode ser usado para ler parâmetros da seção de configuração dada. Isso é feito usando os métodos `config.get()`, `config.getfloat()`, `config.getint()`, etc. Certifique-se de ler todos os valores da configuração durante a construção do objeto de impressora - se o usuário especificar um parâmetro de configuração que não seja lido durante esta fase, será assumido que é um erro de digitação na configuração e um erro será gerado.
* Use o método `config.get_printer()` para obter uma referência à classe "printer" principal. Esta classe "printer" armazena referências a todos os "objetos de impressora" que foram instanciados. Use o método `printer.lookup_object()` para encontrar referências a outros objetos de impressora. Quase toda a funcionalidade (mesmo módulos cinemáticos centrais) está encapsulada em um desses objetos de impressora. Observe, porém, que quando um novo módulo é instanciado, nem todos os outros objetos de impressora terão sido instanciados. Os módulos "gcode" e "pins" estarão sempre disponíveis, mas para outros módulos é uma boa ideia adiar a busca (lookup).
* Registre tratadores de eventos usando o método `printer.register_event_handler()` se o código precisar ser chamado durante "eventos" gerados por outros objetos de impressora. Cada nome de evento é uma string e, por convenção, é o nome do módulo de origem principal que gera o evento junto com um nome curto para a ação que está ocorrendo (por ex., "klippy:connect"). Os parâmetros passados para cada tratador de evento são específicos do evento dado (assim como o tratamento de exceções e o contexto de execução). Dois eventos comuns de inicialização são:
   * klippy:connect - Este evento é gerado após todos os objetos de impressora serem instanciados. Ele é comumente usado para buscar (lookup) outros objetos de impressora, para verificar configurações e para executar um "handshake" inicial com o hardware da impressora.
   * klippy:ready - Este evento é gerado após todos os tratadores de conexão terem sido concluídos com sucesso. Ele indica que a impressora está transitando para um estado pronto para tratar operações normais. Não gere um erro neste retorno de chamada (callback).
* Se houver um erro na configuração do usuário, certifique-se de gerá-lo durante as fases `load_config()` ou do evento "connect". Use `raise config.error("my error")` ou `raise printer.config_error("my error")` para relatar o erro.
* Não armazene uma referência ao objeto `config` em uma variável membro da classe (nem em qualquer local similar que possa persistir após a conclusão do carregamento inicial do módulo). O objeto `config` é uma referência a uma classe de "fase de carregamento de configuração" e não é válido invocar seus métodos após a conclusão da "fase de carregamento de configuração".
* Use o módulo "pins" para configurar um pino em um microcontrolador. Isso é tipicamente feito com algo similar a `printer.lookup_object("pins").setup_pin("pwm", config.get("my_pin"))`. O objeto retornado pode então ser comandado em tempo de execução.
* Se o objeto de impressora define um método `get_status()` então o módulo pode exportar [informações de status](Status_Reference.md) via [macros](Command_Templates.md) e via [Servidor da API](API_Server.md). O método `get_status()` deve retornar um dicionário Python com chaves que são strings e valores que são inteiros, floats, strings, listas, dicionários, True, False ou None. Tuplas (e tuplas nomeadas) também podem ser usadas (elas aparecem como listas quando acessadas via Servidor da API). Listas e dicionários exportados devem ser tratados como "imutáveis" - se seu conteúdo mudar então um novo objeto deve ser retornado de `get_status()`, caso contrário o Servidor da API não detectará essas mudanças.
* Se o módulo precisar de acesso à temporização do sistema ou a descritores de arquivo externos então use `printer.get_reactor()` para obter acesso à classe global "event reactor". Esta classe reactor permite agendar temporizadores, aguardar entrada em descritores de arquivo e "adormecer" o código host.
* Não use variáveis globais. Todo o estado deve ser armazenado no objeto de impressora retornado da função `load_config()`. Isso é importante pois caso contrário o comando RESTART pode não se comportar como esperado. Além disso, por razões similares, se quaisquer arquivos externos (ou sockets) forem abertos então certifique-se de registrar um tratador de evento "klippy:disconnect" e fechá-los a partir desse retorno de chamada (callback).
* Evite acessar as variáveis membro internas (ou chamar métodos que comecem com sublinhado) de outros objetos de impressora. Observar esta convenção facilita gerenciar mudanças futuras.
* Recomenda-se atribuir um valor a todas as variáveis membro no construtor Python das classes Python. (E portanto evitar utilizar a capacidade do Python de criar dinamicamente novas variáveis membro.)
* Se uma variável Python deve armazenar um valor de ponto flutuante então recomenda-se sempre atribuir e manipular essa variável com constantes de ponto flutuante (e nunca usar constantes inteiras). Por exemplo, prefira `self.speed = 1.` em vez de `self.speed = 1`, e prefira `self.speed = 2. * x` em vez de `self.speed = 2 * x`. O uso consistente de valores de ponto flutuante pode evitar peculiaridades difíceis de depurar nas conversões de tipo do Python.
* Se estiver submetendo o módulo para inclusão no código principal do Klipper, certifique-se de colocar um aviso de copyright no topo do módulo. Consulte os módulos existentes para o formato preferido.

<a id="adding-new-kinematics"></a>
## Adição de novas cinemáticas

Esta seção fornece algumas dicas sobre como adicionar suporte no Klipper para tipos adicionais de cinemática da impressora. Este tipo de atividade requer excelente compreensão das fórmulas matemáticas da cinemática alvo. Também requer habilidades de desenvolvimento de software - embora só seja necessário atualizar o software hospedeiro.

Etapas úteis:

1. Comece estudando a seção "[fluxo de código de um movimento](#code-flow-of-a-move-command)" e o [documento de Cinemática](Kinematics.md).
1. Revise as classes cinemáticas existentes no diretório klippy/kinematics/. As classes cinemáticas têm a tarefa de converter um movimento em coordenadas cartesianas para o movimento em cada motor de passo. Deve ser possível copiar um desses arquivos como ponto de partida.
1. Implemente as funções C de posição cinemática do motor de passo para cada motor de passo se elas ainda não estiverem disponíveis (consulte kin_cart.c, kin_corexy.c e kin_delta.c em klippy/chelper/). A função deve chamar `move_get_coord()` para converter um dado tempo de movimento (em segundos) para uma coordenada cartesiana (em milímetros) e então calcular a posição desejada do motor de passo (em milímetros) a partir dessa coordenada cartesiana.
1. Implemente o método `calc_position()` na nova classe de cinemática. Este método calcula a posição do cabeçote em coordenadas cartesianas a partir da posição de cada motor de passo. Ele não precisa ser eficiente pois tipicamente só é chamado durante operações de homing e sondagem.
1. Outros métodos. Implemente os métodos `check_move()`, `get_status()`, `get_steppers()`, `home()`, `clear_homing_state()` e `set_position()`. Essas funções são tipicamente usadas para fornecer verificações específicas de cinemática. No entanto, no início do desenvolvimento pode-se usar código boiler-plate aqui.
1. Implemente casos de teste. Crie um arquivo g-code com uma série de movimentos que possam testar casos importantes para a cinemática dada. Siga a [documentação de depuração](Debugging.md) para converter este arquivo g-code em comandos de microcontrolador. Isso é útil para exercitar casos de borda e verificar regressões.

<a id="porting-to-a-new-micro-controller"></a>
## Portabilidade para um novo microcontrolador

Esta seção fornece algumas dicas sobre como portar o código de microcontrolador do Klipper para uma nova arquitetura. Este tipo de atividade requer bom conhecimento de desenvolvimento embarcado e acesso prático ao microcontrolador alvo.

Etapas úteis:

1. Comece identificando quaisquer bibliotecas de terceiros que serão usadas durante a portabilidade. Exemplos comuns incluem wrappers "CMSIS" e bibliotecas "HAL" do fabricante. Todo código de terceiros precisa ser compatível com GNU GPLv3. O código de terceiros deve ser registrado no diretório lib/ do Klipper. Atualize o arquivo lib/README com informações sobre onde e quando a biblioteca foi obtida. É preferível copiar o código para o repositório do Klipper sem alterações, mas, se alguma alteração for necessária, essas alterações devem ser listadas explicitamente no arquivo lib/README.
1. Crie um novo subdiretório de arquitetura no diretório src/ e adicione o suporte inicial de Kconfig e Makefile. Use as arquiteturas existentes como guia. O src/simulator fornece um exemplo básico de ponto de partida mínimo.
1. A primeira tarefa principal de codificação é ativar o suporte à comunicação com a placa-alvo. Essa é a etapa mais difícil em uma nova portabilidade. Quando a comunicação básica estiver funcionando, as etapas restantes tendem a ser muito mais fáceis. É típico usar um dispositivo serial do tipo UART durante o desenvolvimento inicial, pois esses tipos de dispositivos de hardware geralmente são mais fáceis de habilitar e controlar. Durante essa fase, faça amplo uso do código auxiliar do diretório src/generic/ (verifique como src/simulator/Makefile inclui o código C genérico na compilação). Também é necessário definir timer_read_time() (que retorna o relógio atual do sistema) nessa fase, mas não é necessário suportar totalmente o tratamento de interrupções do temporizador.
1. Familiarize-se com a ferramenta console.py (conforme descrito no [documento de depuração](Debugging.md)) e verifique a conectividade com o microcontrolador usando-a. Essa ferramenta traduz o protocolo de comunicação de baixo nível do microcontrolador para uma forma legível por humanos.
1. Adicione suporte ao despacho de temporizador a partir de interrupções de hardware. Veja o commit [970831ee](https://github.com/Klipper3d/klipper/commit/970831ee0d3b91897196e92270d98b2a3067427f) do Klipper como exemplo das etapas 1–5 feitas para a arquitetura LPC176x.
1. Ative o suporte básico de entrada e saída GPIO. Veja o commit [c78b9076](https://github.com/Klipper3d/klipper/commit/c78b90767f19c9e8510c3155b89fb7ad64ca3c54) do Klipper como exemplo disso.
1. Ative periféricos adicionais - por exemplo, veja os commits [65613aed](https://github.com/Klipper3d/klipper/commit/65613aeddfb9ef86905cb1dade9e773a02ef3c27), [c812a40a](https://github.com/Klipper3d/klipper/commit/c812a40a3782415e454b04bf7bd2158a6f0ec8b5) e [c381d03a](https://github.com/Klipper3d/klipper/commit/c381d03aad5c3ee761169b7c7bced519cc14da29) do Klipper.
1. Crie um arquivo de configuração de exemplo do Klipper no diretório config/. Teste o microcontrolador com o programa principal klippy.py.
1. Considere adicionar casos de teste de compilação no diretório test/.

Dicas adicionais de codificação:

1. Evite usar "C bitfields" para acessar registradores de E/S; prefira operações diretas de leitura e escrita de inteiros de 32 bits, 16 bits ou 8 bits. As especificações da linguagem C não definem claramente como o compilador deve implementar C bitfields (por ex., endianness e layout de bits), e é difícil determinar quais operações de E/S ocorrerão em uma leitura ou escrita de C bitfield.
1. Prefira escrever valores explícitos nos registradores de E/S em vez de usar operações de leitura-modificação-escrita. Ou seja, se estiver atualizando um campo em um registrador de E/S onde os outros campos têm valores conhecidos, então é preferível escrever explicitamente o conteúdo completo do registrador. Escritas explícitas produzem código menor, mais rápido e mais fácil de depurar.

<a id="coordinate-systems"></a>
## Sistemas de coordenadas

Internamente, o Klipper rastreia principalmente a posição do cabeçote em coordenadas cartesianas relativas ao sistema de coordenadas especificado no arquivo de configuração. Ou seja, a maior parte do código do Klipper nunca sofrerá uma alteração de sistema de coordenadas. Se o usuário fizer uma solicitação para alterar a origem (por ex., um comando `G92`), esse efeito é obtido traduzindo os comandos futuros para o sistema de coordenadas primário.

No entanto, em alguns casos é útil obter a posição do cabeçote em algum outro sistema de coordenadas e o Klipper tem várias ferramentas para facilitar isso. Isso pode ser visto executando o comando GET_POSITION. Por exemplo:

```
Send: GET_POSITION
Recv: // mcu: stepper_a:-2060 stepper_b:-1169 stepper_c:-1613
Recv: // stepper: stepper_a:457.254159 stepper_b:466.085669 stepper_c:465.382132
Recv: // kinematic: X:8.339144 Y:-3.131558 Z:233.347121
Recv: // toolhead: X:8.338078 Y:-3.123175 Z:233.347878 E:0.000000
Recv: // gcode: X:8.338078 Y:-3.123175 Z:233.347878 E:0.000000
Recv: // gcode base: X:0.000000 Y:0.000000 Z:0.000000 E:0.000000
Recv: // gcode homing: X:0.000000 Y:0.000000 Z:0.000000
```

A posição "mcu" (`stepper.get_mcu_position()` no código) é o número total de passos que o microcontrolador emitiu em direção positiva menos o número de passos emitidos em direção negativa desde a última reinicialização do microcontrolador. Se o robô estiver em movimento quando a consulta for emitida, o valor relatado inclui movimentos armazenados em buffer no microcontrolador, mas não inclui movimentos na fila de antecipação (look-ahead).

A posição "stepper" (`stepper.get_commanded_position()`) é a posição do motor de passo em questão conforme rastreada pelo código de cinemática. Isso geralmente corresponde à posição (em mm) do carro ao longo de seu trilho, relativa ao position_endstop especificado no arquivo de configuração. (Algumas cinemáticas rastreiam posições de motores de passo em radianos em vez de milímetros.) Se o robô estiver em movimento quando a consulta for emitida, o valor relatado inclui movimentos armazenados em buffer no microcontrolador, mas não inclui movimentos na fila de antecipação (look-ahead). Pode-se usar as chamadas `toolhead.flush_step_generation()` ou `toolhead.wait_moves()` para esvaziar completamente a antecipação e o código de geração de passos.

A posição "kinematic" (`kin.calc_position()`) é a posição cartesiana do cabeçote derivada das posições "stepper" e é relativa ao sistema de coordenadas especificado no arquivo de configuração. Isso pode diferir da posição cartesiana solicitada devido à granularidade dos motores de passo. Se o robô estiver em movimento quando as posições "stepper" forem obtidas, o valor relatado inclui movimentos armazenados em buffer no microcontrolador, mas não inclui movimentos na fila de antecipação (look-ahead). Pode-se usar as chamadas `toolhead.flush_step_generation()` ou `toolhead.wait_moves()` para esvaziar completamente a antecipação e o código de geração de passos.

A posição "toolhead" (`toolhead.get_position()`) é a última posição solicitada do cabeçote em coordenadas cartesianas relativas ao sistema de coordenadas especificado no arquivo de configuração. Se o robô estiver em movimento quando a consulta for emitida, o valor relatado inclui todos os movimentos solicitados (mesmo aqueles em buffers aguardando emissão para os drivers dos motores de passo).

A posição "gcode" é a última posição solicitada de um comando `G1` (ou `G0`) em coordenadas cartesianas relativas ao sistema de coordenadas especificado no arquivo de configuração. Isso pode diferir da posição "toolhead" se uma transformação de g-code (por ex., bed_mesh, bed_tilt, skew_correction) estiver em vigor. Isso pode diferir das coordenadas reais especificadas no último comando `G1` se a origem do g-code tiver sido alterada (por ex., `G92`, `SET_GCODE_OFFSET`, `M221`). O comando `M114` (`gcode_move.get_status()['gcode_position']`) relatará a última posição de g-code relativa ao sistema atual de coordenadas de g-code.

A "gcode base" é a localização da origem do g-code em coordenadas cartesianas relativas ao sistema de coordenadas especificado no arquivo de configuração. Comandos como `G92`, `SET_GCODE_OFFSET` e `M221` alteram este valor.

O "gcode homing" é a localização a ser usada para a origem do g-code (em coordenadas cartesianas relativas ao sistema de coordenadas especificado no arquivo de configuração) após um comando de referenciamento `G28`. O comando `SET_GCODE_OFFSET` pode alterar este valor.

<a id="time"></a>
## Tempo

Fundamental para a operação do Klipper é o tratamento de relógios, tempos e carimbos de data/hora. O Klipper executa ações na impressora agendando eventos para ocorrerem em um futuro próximo. Por exemplo, para ligar um ventilador, o código pode agendar uma alteração em um pino GPIO em 100ms. É raro o código tentar realizar uma ação instantânea. Assim, o tratamento do tempo dentro do Klipper é crítico para a operação correta.

Há três tipos de tempos rastreados internamente no software hospedeiro do Klipper:

* Tempo de sistema. O tempo de sistema usa o relógio monotônico do sistema - é um número de ponto flutuante armazenado como segundos e (geralmente) é relativo a quando o computador hospedeiro foi iniciado pela última vez. Tempos de sistema têm uso limitado no software - são usados principalmente ao interagir com o sistema operacional. No código hospedeiro, tempos de sistema são frequentemente armazenados em variáveis chamadas *eventtime* ou *curtime*.
* Tempo de impressão. O tempo de impressão é sincronizado com o relógio do microcontrolador principal (o microcontrolador definido na seção de configuração "[mcu]"). É um número de ponto flutuante armazenado como segundos e é relativo a quando o mcu principal foi reiniciado pela última vez. É possível converter de um "tempo de impressão" para o relógio de hardware do microcontrolador principal multiplicando o tempo de impressão pela taxa de frequência configurada estaticamente do mcu. O código hospedeiro de alto nível usa tempos de impressão para calcular quase todas as ações físicas (por ex., movimento do cabeçote, mudanças no aquecedor, etc.). No código hospedeiro, tempos de impressão são geralmente armazenados em variáveis chamadas *print_time* ou *move_time*.
* Relógio do MCU. Este é o contador de relógio de hardware em cada microcontrolador. É armazenado como um inteiro e sua taxa de atualização é relativa à frequência do respectivo microcontrolador. O software hospedeiro traduz seus tempos internos para relógios antes da transmissão ao mcu. O código do mcu só rastreia tempo em ticks de relógio. No código hospedeiro, valores de relógio são rastreados como inteiros de 64 bits, enquanto o código do mcu usa inteiros de 32 bits. No código hospedeiro, relógios são geralmente armazenados em variáveis com nomes contendo *clock* ou *ticks*.

A conversão entre os diferentes formatos de tempo é implementada principalmente no código **klippy/clocksync.py**.

Alguns pontos a observar ao revisar o código:

* Relógios de 32 bits e 64 bits: Para reduzir a largura de banda e melhorar a eficiência do microcontrolador, os relógios no microcontrolador são rastreados como inteiros de 32 bits. Ao comparar dois relógios no código do mcu, a função `timer_is_before()` deve ser sempre usada para garantir que estouros de inteiro sejam tratados corretamente. O software hospedeiro converte relógios de 32 bits para relógios de 64 bits acrescentando os bits de ordem alta do último carimbo de data/hora do mcu recebido - nenhuma mensagem do mcu está jamais mais de 2^31 ticks de relógio no futuro ou no passado, portanto essa conversão nunca é ambígua. O hospedeiro converte de relógios de 64 bits para relógios de 32 bits simplesmente truncando os bits de ordem alta. Para garantir que não haja ambiguidade nessa conversão, o código **klippy/chelper/serialqueue.c** armazenará mensagens em buffer até que estejam dentro de 2^31 ticks de relógio do seu tempo-alvo.
* Múltiplos microcontroladores: o software hospedeiro suporta o uso de múltiplos microcontroladores em uma única impressora. Nesse caso, o "relógio MCU" de cada microcontrolador é acompanhado separadamente. O código clocksync.py trata a deriva de relógio entre microcontroladores modificando a forma como converte de "tempo de impressão" para "relógio MCU". Nos mcus secundários, a frequência mcu usada nessa conversão é atualizada regularmente para compensar a deriva medida.
