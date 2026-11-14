<a id="contact"></a>
# Contato

[English](../Contact.md) | **Português brasileiro**

Este documento fornece informações de contato para o Klipper.

<a id="discourse-forum"></a>
## Fórum Discourse

Há um [servidor Discourse da Comunidade Klipper](https://community.klipper3d.org) para discussões estilo "fórum" sobre o Klipper. Observe que Discourse não é Discord.

<a id="discord-chat"></a>
## Chat no Discord

Há um servidor Discord dedicado ao Klipper em: <https://discord.klipper3d.org>. Observe que Discord não é Discourse.

Este servidor é administrado por uma comunidade de entusiastasdo Klipper dedicados a discussões sobre o Klipper. Ele permite que os usuários conversem com outros usuários em tempo real.

<a id="i-have-a-question-about-klipper"></a>
## Eu tenho uma pergunta sobre o Klipper

Muitas perguntas que recebemos já foram respondidas na [documentação do Klipper](Overview.md). Leia a documentação e siga as instruções fornecidas.

Também é possível pesquisar por perguntas semelhantes no [Fórum Discourse do Klipper](#discourse-forum).

Se você tem interesse em compartilhar seu conhecimento e experiência com outros usuários do Klipper, pode participar do [Fórum Discourse do Klipper](#discourse-forum) ou do [Chat Discord do Klipper](#discord-chat). Ambos são comunidades onde usuários do Klipper podem discutir o Klipper com outros usuários.

Se você tem uma pergunta geral ou está enfrentando problemas gerais de impressão, considere também um fórum geral de impressão 3D ou um fórum dedicado ao hardware da impressora.

<a id="i-have-a-feature-request"></a>
## Eu tenho um pedido de recurso

Todos os recursos novos exigem alguém interessado e capaz de implementar esse recurso. Se você tem interesse em ajudar a implementar ou testar um recurso novo, pode pesquisar desenvolvimentos em andamento no [Fórum Discourse do Klipper](#discourse-forum). Há também o [Chat Discord do Klipper](#discord-chat) para discussões entre colaboradores.

<a id="help-it-doesnt-work"></a>
## Ajuda! Isso não funciona!

Se você está experienciando problemas, recomendamos que leia atentamente a [documentação do Klipper](Overview.md) e se certifique que todos os passos foram seguidos.

Se você está enfrentando um problema de impressão, recomendamos inspecionar cuidadosamente o hardware da impressora (todas as juntas, fios, parafusos etc.) e verificar se nada está anormal. Verificamos que a maioria dos problemas de impressão não está relacionada ao software Klipper. Se você encontrar um problema no hardware da impressora, considere pesquisar em fóruns gerais de impressão 3D ou em fóruns dedicados ao hardware da impressora.

Também é possível pesquisar por problemas semelhantes no [Fórum Discourse do Klipper](#discourse-forum).

Se você tem interesse em compartilhar seu conhecimento e experiência com outros usuários do Klipper, pode participar do [Fórum Discourse do Klipper](#discourse-forum) ou do [Chat Discord do Klipper](#discord-chat). Ambos são comunidades onde usuários do Klipper podem discutir o Klipper com outros usuários.

<a id="i-found-a-bug-in-the-klipper-software"></a>
## Encontrei um bug no Klipper

Klipper é um projeto de código aberto e apreciamos quando colaboradores diagnosticam erros no software.

Problemas devem ser relatados no [Fórum Discourse do Klipper](#discourse-forum).

Tem informações importantes que serão necessárias para correção do bug. Por favor siga os seguintes passos:

1. Certifique-se de estar executando o código não modificado de <https://github.com/Klipper3d/klipper>. Se o código tiver sido modificado ou obtido de outra fonte, você deverá reproduzir o problema no código não modificado de <https://github.com/Klipper3d/klipper> antes de relatar.
1. Se possível, execute um comando `M112` imediatamente após a ocorrência do problema. Isso faz com que o Klipper entre em um "estado de desligamento" e fará com que informações adicionais de depuração sejam gravadas no arquivo log.
1. Obtenha o arquivo de registro do Klipper sobre o evento. O arquivo de registro foi projetado para responder a dúvidas comuns que os desenvolvedores do Klipper tenham a respeito do software e seu ambiente (versão de software, tipo de hardware, configuração, horário do evento, e outras centenas de questões).
   1. As interfaces web dedicadas do Klipper têm a capacidade de obter diretamente o arquivo de log do Klipper. É a maneira mais fácil de obter o log ao usar uma dessas interfaces. Caso contrário, um utilitário "scp" ou "sftp" é necessário para copiar o arquivo de log para o seu computador. O utilitário "scp" acompanha por padrão os desktops Linux e MacOS. Há utilitários scp gratuitos para outros desktops (por ex., WinSCP). O arquivo de log pode estar localizado no arquivo `~/printer_data/logs/klippy.log` (se estiver usando um utilitário scp gráfico, procure a pasta "printer_data", depois a pasta "logs" dentro dela, depois o arquivo `klippy.log`). O arquivo de log pode alternativamente estar localizado no arquivo `/tmp/klippy.log` (se estiver usando um utilitário scp gráfico que não consegue copiar diretamente `/tmp/klippy.log`, clique repetidamente em `..` ou "pasta pai" até chegar ao diretório raiz, clique na pasta `tmp` e então selecione o arquivo `klippy.log`).
   1. Copie o arquivo log para seu computador para que ele possa ser anexado a um relatório de problemas.
   1. Não modifique o arquivo log de forma alguma; não forneça apenas um trecho do log. Somente o arquivo log completo e não modificado fornece as informações necessárias.
   1. É uma boa ideia compactar o arquivo de log com .zip ou .gzip.
1. Abra um novo tópico no [Fórum Discourse do Klipper](#discourse-forum) e forneça uma descrição clara do problema. Outros contribuidores do Klipper precisarão entender quais etapas foram executadas, qual era o resultado desejado e qual resultado realmente ocorreu. O arquivo de log compactado do Klipper deve ser anexado a esse tópico.

<a id="i-am-making-changes-that-id-like-to-include-in-klipper"></a>
## Estou fazendo modificações que gostaria de incluir no Klipper

Klipper é um software de código aberto e nós apreciamos novas contribuições.

Consulte o [documento CONTRIBUTING](CONTRIBUTING.md) para informações.

Há vários [documentos para desenvolvedores](Overview.md#developer-documentation). Se você tiver dúvidas sobre o código, também pode perguntar no [Fórum Discourse do Klipper](#discourse-forum) ou no [Chat Discord do Klipper](#discord-chat).

<a id="professional-services"></a>
## Serviços profissionais

![](../img/klipper-logo-small.png)

Desenvolvimento de software personalizado, suporte e soluções de software: <https://ko-fi.com/koconnor>
