<a id="protocol"></a>
# Protocolo

[English](../Protocol.md) | **Português brasileiro**

O protocolo de mensagens Klipper é usado para comunicação de baixo nível entre o software host Klipper e o software do microcontrolador Klipper. Em um alto nível, o protocolo pode ser pensado como uma série de strings de comando e resposta que são compactadas, transmitidas e processadas no lado receptor. Um exemplo de série de comandos em formato legível por humanos não compactado pode ser semelhante a:

```
set_digital_out pin=PA3 value=1
set_digital_out pin=PA7 value=1
schedule_digital_out oid=8 clock=4000000 value=0
queue_step oid=7 interval=7458 count=10 add=331
queue_step oid=7 interval=11717 count=4 add=1281
```

Consulte o documento [comandos mcu](MCU_Commands.md) para informações sobre os comandos disponíveis. Consulte o documento [depuração (debugging)](Debugging.md) para informações sobre como traduzir um arquivo G-Code em seus comandos de microcontrolador legíveis por humanos correspondentes.

Esta página fornece uma descrição de alto nível do próprio protocolo de mensagens do Klipper. Ela descreve como as mensagens são declaradas, codificadas em formato binário (o esquema de "compressão") e transmitidas.

O objetivo do protocolo é permitir um canal de comunicação sem erros entre o hospedeiro (host) e o microcontrolador que seja de baixa latência, baixa largura de banda e baixa complexidade para o microcontrolador.

<a id="micro-controller-interface"></a>
## Interface do microcontrolador

O protocolo de transmissão do Klipper pode ser pensado como um mecanismo [RPC](https://en.wikipedia.org/wiki/Remote_procedure_call) entre microcontrolador e hospedeiro (host). O software do microcontrolador declara os comandos que o hospedeiro pode invocar junto com as mensagens de resposta que ele pode gerar. O hospedeiro usa essas informações para comandar o microcontrolador a executar ações e para interpretar os resultados.

<a id="declaring-commands"></a>
### Declaração de comandos

O software do microcontrolador declara um "comando" usando a macro DECL_COMMAND() no código C. Por exemplo:

```
DECL_COMMAND(command_update_digital_out, "update_digital_out oid=%c value=%c");
```

O acima declara um comando chamado "update_digital_out". Isso permite que o hospedeiro "invoque" este comando, o que faria com que a função C command_update_digital_out() fosse executada no microcontrolador. O acima também indica que o comando recebe dois parâmetros inteiros. Quando o código C command_update_digital_out() for executado, ele receberá um array contendo esses dois inteiros - o primeiro correspondendo a 'oid' e o segundo correspondendo a 'value'.

Em geral, os parâmetros são descritos com sintaxe de estilo printf() (por ex., "%u"). A formatação corresponde diretamente à visualização legível por humanos dos comandos (por ex., "update_digital_out oid=7 value=1"). No exemplo acima, "value=" é um nome de parâmetro e "%c" indica que o parâmetro é um inteiro. Internamente, o nome do parâmetro é usado apenas como documentação. Neste exemplo, o "%c" também é usado como documentação para indicar que o inteiro esperado tem 1 byte de tamanho (o tamanho inteiro declarado não afeta a análise (parsing) ou codificação).

A compilação (build) do microcontrolador coletará todos os comandos declarados com DECL_COMMAND(), determinará seus parâmetros e providenciará para que possam ser chamados.

<a id="declaring-responses"></a>
### Declaração de respostas

Para enviar informações do microcontrolador para o hospedeiro, uma "resposta" é gerada. Estas são declaradas e transmitidas usando a macro C sendf(). Por exemplo:

```
sendf("status clock=%u status=%c", sched_read_time(), sched_is_shutdown());
```

O acima transmite uma mensagem de resposta "status" que contém dois parâmetros inteiros ("clock" e "status"). A compilação do microcontrolador encontra automaticamente todas as chamadas sendf() e gera codificadores para elas. O primeiro parâmetro da função sendf() descreve a resposta e está no mesmo formato das declarações de comandos.

O hospedeiro pode providenciar o registro de uma função de retorno (callback) para cada resposta. Assim, de fato, os comandos permitem que o hospedeiro invoque funções C no microcontrolador e as respostas permitem que o software do microcontrolador invoque código no hospedeiro.

A macro sendf() deve ser invocada apenas a partir de manipuladores de comando ou de tarefa, e não deve ser invocada a partir de interrupções ou temporizadores. O código não precisa emitir um sendf() em resposta a um comando recebido, não é limitado no número de vezes que sendf() pode ser invocado e pode invocar sendf() a qualquer momento a partir de um manipulador de tarefa.

<a id="output-responses"></a>
#### Respostas de saída

Para simplificar a depuração, há também uma função C output(). Por exemplo:

```
output("The value of %u is %s with size %u.", x, buf, buf_len);
```

A função output() é semelhante em uso a printf() - destina-se a gerar e formatar mensagens arbitrárias para consumo humano.

<a id="declaring-enumerations"></a>
### Declaração de enumerações

Enumerações permitem que o código do hospedeiro use identificadores de string para parâmetros que o microcontrolador trata como inteiros. Elas são declaradas no código do microcontrolador - por exemplo:

```
DECL_ENUMERATION("spi_bus", "spi", 0);

DECL_ENUMERATION_RANGE("pin", "PC0", 16, 8);
```

No primeiro exemplo, a macro DECL_ENUMERATION() define uma enumeração para qualquer mensagem de comando/resposta com nome de parâmetro "spi_bus" ou nome de parâmetro com sufixo "_spi_bus". Para esses parâmetros, a string "spi" é um valor válido e será transmitida com um valor inteiro de zero.

Também é possível declarar um intervalo de enumeração. No segundo exemplo, um parâmetro "pin" (ou qualquer parâmetro com sufixo "_pin") aceitaria PC0, PC1, PC2, ..., PC7 como valores válidos. As strings serão transmitidas com os inteiros 16, 17, 18, ..., 23.

<a id="declaring-constants"></a>
### Declaração de constantes

Constantes também podem ser exportadas. Por exemplo, o seguinte:

```
DECL_CONSTANT("SERIAL_BAUD", 250000);
```

exportaria uma constante chamada "SERIAL_BAUD" com valor de 250000 do microcontrolador para o host. Também é possível declarar uma constante que seja uma string - por exemplo:

```
DECL_CONSTANT_STR("MCU", "pru");
```

<a id="low-level-message-encoding"></a>
## Codificação de mensagens de baixo nível

Para implementar o mecanismo RPC acima, cada comando e resposta é codificado em um formato binário para transmissão. Esta seção descreve o sistema de transmissão.

<a id="message-blocks"></a>
### Blocos de mensagem

Todos os dados enviados do host para o microcontrolador e vice-versa estão contidos em "blocos de mensagem". Um bloco de mensagem tem um cabeçalho de dois bytes e um rodapé de três bytes. O formato de um bloco de mensagem é:

```
<1 byte length><1 byte sequence><n-byte content><2 byte crc><1 byte sync>
```

O byte de comprimento contém o número de bytes no bloco de mensagem, incluindo os bytes de cabeçalho e de rodapé (portanto, o comprimento mínimo da mensagem é de 5 bytes). O comprimento máximo do bloco de mensagem é atualmente de 64 bytes. O byte de sequência contém um número de sequência de 4 bits nos bits de baixa ordem e os bits de alta ordem sempre contêm 0x10 (os bits de alta ordem são reservados para uso futuro). Os bytes de conteúdo contêm dados arbitrários e seu formato é descrito na seção seguinte. Os bytes crc contêm um [CRC](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) CCITT de 16 bits do bloco de mensagem, incluindo os bytes de cabeçalho, mas excluindo os bytes de rodapé. O byte de sincronização é 0x7e.

O formato do bloco de mensagem é inspirado em quadros de mensagem [HDLC](https://en.wikipedia.org/wiki/High-Level_Data_Link_Control). Como no HDLC, o bloco de mensagem pode opcionalmente conter um caractere de sincronismo adicional no início do bloco. Diferentemente do HDLC, um caractere de sincronismo não é exclusivo do enquadramento e pode estar presente no conteúdo do bloco de mensagem.

<a id="message-block-contents"></a>
### Conteúdo do bloco de mensagem

Cada bloco de mensagem enviado do host para o microcontrolador contém uma série de zero ou mais comandos de mensagem em seu conteúdo. Cada comando começa com um command-id inteiro codificado em [Quantidades de Comprimento Variável](#variable-length-quantities) (VLQ) seguido de zero ou mais parâmetros VLQ para o comando em questão.

Como exemplo, os quatro comandos a seguir poderiam ser colocados em um único bloco de mensagem:

```
update_digital_out oid=6 value=1
update_digital_out oid=5 value=0
get_config
get_clock
```

e codificados nos oito inteiros VLQ a seguir:

```
<id_update_digital_out><6><1><id_update_digital_out><5><0><id_get_config><id_get_clock>
```

Para codificar e analisar o conteúdo da mensagem, tanto o host quanto o microcontrolador devem concordar sobre os ids de comando e o número de parâmetros que cada comando possui. Assim, no exemplo acima, tanto o host quanto o microcontrolador saberiam que "id_update_digital_out" é sempre seguido por dois parâmetros, e que "id_get_config" e "id_get_clock" têm zero parâmetros. O host e o microcontrolador compartilham um "dicionário de dados" que mapeia as descrições de comando (por ex., "update_digital_out oid=%c value=%c") para seus command-ids inteiros. Ao processar os dados, o analisador saberá esperar um número específico de parâmetros codificados em VLQ após um dado command id.

O conteúdo das mensagens para blocos enviados do microcontrolador para o host segue o mesmo formato. Os identificadores nessas mensagens são "response ids", mas têm o mesmo propósito e seguem as mesmas regras de codificação. Na prática, blocos de mensagem enviados do microcontrolador para o host nunca contêm mais de uma resposta no conteúdo do bloco de mensagem.

<a id="variable-length-quantities"></a>
#### Quantidades de Comprimento Variável

Consulte o [artigo da Wikipédia](https://en.wikipedia.org/wiki/Variable-length_quantity) para mais informações sobre o formato geral de inteiros codificados em VLQ. O Klipper usa um esquema de codificação que suporta inteiros positivos e negativos. Inteiros próximos de zero usam menos bytes para codificar e inteiros positivos normalmente são codificados usando menos bytes do que inteiros negativos. A tabela a seguir mostra o número de bytes que cada inteiro leva para codificar:

| Inteiro | Tamanho codificado |
| --- | --- |
| -32 .. 95 | 1 |
| -4096 .. 12287 | 2 |
| -524288 .. 1572863 | 3 |
| -67108864 .. 201326591 | 4 |
| -2147483648 .. 4294967295 | 5 |

<a id="variable-length-strings"></a>
#### Strings de comprimento variável

Como exceção às regras de codificação acima, se um parâmetro de um comando ou resposta for uma string dinâmica, então o parâmetro não é codificado como um simples inteiro VLQ. Em vez disso, ele é codificado transmitindo o comprimento como um inteiro codificado em VLQ seguido pelo próprio conteúdo:

```
<VLQ encoded length><n-byte contents>
```

As descrições de comando encontradas no dicionário de dados permitem que tanto o host quanto o microcontrolador saibam quais parâmetros de comando usam codificação VLQ simples e quais parâmetros usam codificação de string.

<a id="data-dictionary"></a>
## Dicionário de dados

Para que comunicações significativas sejam estabelecidas entre o microcontrolador e o host, ambos os lados devem concordar sobre um "dicionário de dados". Este dicionário de dados contém os identificadores inteiros para comandos e respostas junto com suas descrições.

O build do microcontrolador usa o conteúdo das macros DECL_COMMAND() e sendf() para gerar o dicionário de dados. O build atribui automaticamente identificadores únicos a cada comando e resposta. Este sistema permite que tanto o código do host quanto o do microcontrolador usem perfeitamente nomes descritivos legíveis por humanos enquanto ainda usam largura de banda mínima.

O host consulta o dicionário de dados quando se conecta pela primeira vez ao microcontrolador. Uma vez que o host baixa o dicionário de dados do microcontrolador, ele usa esse dicionário de dados para codificar todos os comandos e para analisar todas as respostas do microcontrolador. O host deve, portanto, lidar com um dicionário de dados dinâmico. No entanto, para manter o software do microcontrolador simples, o microcontrolador sempre usa seu dicionário de dados estático (compilado internamente).

O dicionário de dados é consultado enviando comandos "identify" ao microcontrolador. O microcontrolador responderá a cada comando identify com uma mensagem "identify_response". Como esses dois comandos são necessários antes de obter o dicionário de dados, seus ids inteiros e tipos de parâmetro são codificados de forma fixa tanto no microcontrolador quanto no host. O response id de "identify_response" é 0, o command id de "identify" é 1. Além de terem ids fixos, o comando identify e sua resposta são declarados e transmitidos da mesma forma que outros comandos e respostas. Nenhum outro comando ou resposta é fixo.

O formato do próprio dicionário de dados transmitido é uma string JSON comprimida com zlib. O processo de build do microcontrolador gera a string, comprime-a e armazena-a na seção de texto da flash do microcontrolador. O dicionário de dados pode ser muito maior que o tamanho máximo do bloco de mensagem - o host o baixa enviando múltiplos comandos identify solicitando partes progressivas do dicionário de dados. Uma vez obtidas todas as partes, o host montará as partes, descomprimirá os dados e analisará o conteúdo.

Além de informações sobre o protocolo de comunicação, o dicionário de dados também contém a versão do software, enumerações (conforme definido por DECL_ENUMERATION) e constantes (conforme definido por DECL_CONSTANT).

<a id="message-flow"></a>
## Fluxo de mensagens

Os comandos de mensagem enviados do host para o microcontrolador destinam-se a ser livres de erros. O microcontrolador verificará o CRC e os números de sequência em cada bloco de mensagem para garantir que os comandos estejam corretos e em ordem. O microcontrolador sempre processa blocos de mensagem em ordem - caso receba um bloco fora de ordem, ele o descartará e quaisquer outros blocos fora de ordem até receber blocos com a sequenciação correta.

O código de baixo nível do host implementa um sistema de retransmissão automática para blocos de mensagem perdidos e corrompidos enviados ao microcontrolador. Para facilitar isso, o microcontrolador transmite um "bloco de mensagem ack" após cada bloco de mensagem recebido com sucesso. O host agenda um tempo limite após enviar cada bloco e o retransmitirá caso o tempo limite expire sem receber um "ack" correspondente. Além disso, se o microcontrolador detectar um bloco corrompido ou fora de ordem, ele poderá transmitir um "bloco de mensagem nak" para facilitar a retransmissão rápida.

Um "ack" é um bloco de mensagem com conteúdo vazio (isto é, um bloco de mensagem de 5 bytes) e um número de sequência maior que o último número de sequência do host recebido. Um "nak" é um bloco de mensagem com conteúdo vazio e um número de sequência menor que o último número de sequência do host recebido.

O protocolo facilita um sistema de transmissão em "janela" para que o host possa ter muitos blocos de mensagem pendentes em trânsito por vez. (Isso é além dos muitos comandos que podem estar presentes em um dado bloco de mensagem.) Isso permite utilização máxima da largura de banda mesmo no caso de latência de transmissão. Os mecanismos de tempo limite, retransmissão, janelamento e ack são inspirados em mecanismos semelhantes no [TCP](https://en.wikipedia.org/wiki/Transmission_Control_Protocol).

Na outra direção, blocos de mensagem enviados do microcontrolador para o host são projetados para serem livres de erros, mas não têm transmissão garantida. (As respostas não devem estar corrompidas, mas podem se perder.) Isso é feito para manter simples a implementação no microcontrolador. Não há sistema de retransmissão automática para respostas - espera-se que o código de alto nível seja capaz de lidar com uma resposta ausente ocasional (geralmente solicitando novamente o conteúdo ou configurando uma agenda recorrente de transmissão de respostas). O campo de número de sequência em blocos de mensagem enviados ao host é sempre um a mais que o último número de sequência recebido de blocos de mensagem recebidos do host. Ele não é usado para rastrear sequências de blocos de mensagem de resposta.
