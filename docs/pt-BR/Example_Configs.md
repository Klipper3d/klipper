<a id="example-configurations"></a>
# Exemplo de configurações

[English](../Example_Configs.md) | **Português brasileiro**

Esse documento contém diretrizes para contribuir com um exemplo de configuração do Klipper para o repositório github do Klipper (localizado no [diretório de configuração](../../config)).

Note que o [servidor de Discussão Comunitária do Klipper](https://community.klipper3d.org) também é uma fonte útil para achar e compartilhar arquivos de configuração.

<a id="guidelines"></a>
## Diretrizes

1. Selecione o prefixo de nome de arquivo de configuração apropriado:
   1. O prefixo `printer` é usado para impressoras em sua configuração padrão, vendidas por um fabricante convencional.
   1. O prefixo `generic` é usado para uma placa controladora de impressora 3d que pode ser usada em vários tipos diferentes de impressoras.
   1. O prefixo `kit` é para impressoras 3d que são montadas de acordo especificações amplamente utilizadas. Essas impressoras de "kit" são geralmente diferentes de "impressoras" normais visto que não são vendidas por um fabricante.
   1. O prefixo `sample` é usado para "trechos" de configuração que são possíveis de serem copiados e colados no arquivo de configuração principal.
   1. O prefixo `example` é usado para descrever a cinemática da impressora. Esse tipo de configuração tipicamente é somente adicionado junto ao código para um novo tipo de cinemática de impressora.
1. Todos arquivos de configuração devem terminar com o sufixo `.cfg`. Os arquivos de configuração `printer` devem terminar com um ano, seguido por `.cfg` (p. ex. `-2019.cfg`). Nesse caso, o ano é o ano aproximado em que a impressora foi vendida.
1. Não utilize espaços ou caracteres especiais no nome do arquivo de configuração. O nome de arquivo deve conter somente os caracteres `A-Z`, `a-z`, `0-9`, `-` e `.`.
1. O Klipper deve ser capaz de iniciar os arquivos de exemplo de configuração `printer`, `generic` e `kit` sem erros. Esses arquivos de configuração devem ser adicionados ao caso de teste de regressão [test/klippy/printers.test](../../test/klippy/printers.test). Adicione novos arquivos de configuração a esse caso de teste, na parte apropriada, em ordem alfabética dentro da seção.
1. O exemplo de configuração deve ser para a configuração "padrão" da impressora. (Existem muitas configurações "customizadas" para acompanhar no repositório principal do Klipper.) Analogicamente, somente adicionamos arquivos de exemplo de configuração para impressoras, kits e placas que tenham popularidade com o público em geral (p. ex., devem ter ao menos 100 unidades dela em uso). Considere usar o [servidor de Discussão Comunitária do Klipper](https://community.klipper3d.org) para outras configurações.
1. Especifique apenas os dispositivos presentes na impressora ou placa em questão. Não especifique ajustes particulares da sua instalação.
   1. Para arquivos de configuração `generic`, descreva apenas os dispositivos presentes na placa principal. Por exemplo, não faria sentido adicionar uma seção de configuração de tela a uma configuração "generic", pois não há como saber se a placa estará conectada àquele tipo de tela. Se a placa tiver uma porta de hardware específica para facilitar a conexão de um periférico opcional (por exemplo, uma porta bltouch), pode-se adicionar uma seção de configuração comentada para esse dispositivo.
   1. Não especifique `pressure_advance` em um exemplo de configuração, visto que esse valor é específico ao filamento, não ao hardware da impressora. Da mesma forma, não especifique os ajustes `max_extrude_only_velocity` ou `max_extrude_only_accel`.
   1. Não especifique uma seção de configuração contendo um caminho ou hardware de hospedeiro. Por exemplo, não determine seções de configuração `[virtual_sdcard]` ou `[temperature_host]`.
   1. Somente defina macros que utilizem funcionalidade específica a uma impressora ou determine g-codes que são habitualmente emitidos por fatiadores configurados para a impressora em questão.
1. Sempre que possível, prefira usar a mesma terminologia, redação, indentação e ordem das seções dos arquivos de configuração existentes.
   1. O início de cada arquivo de configuração deve informar o tipo de microcontrolador que o usuário deve selecionar durante "make menuconfig". Também deve conter uma referência a "docs/Config_Reference.md".
   1. Não copie o campo documentação nos arquivos de exemplo de configuração. (Ao se fazer isso se cria uma obrigação de manutenção, visto que em uma atualização na documentação seria necessário modificá-la em vários lugares.)
   1. Arquivos de configuração de exemplo não devem conter uma seção "SAVE_CONFIG". Se necessário, copie os campos relevantes da seção SAVE_CONFIG para a parte apropriada na área da configuração principal.
   1. Use a sintaxe `field: value` ao invés de `field=value`.
   1. Quando adicionar uma `rotation_distance` para uma extrusora é preferível especificar uma `gear_ratio`, caso a extrusora tenha um mecanismo de engrenagens. É esperado que a rotation_distance nas configurações de exemplo esteja correlacionada com a circunferência da engrenagem fresada da extrusora - que é normalmente na faixa de 20 a 35mm. Quando se especifica uma `gear_ratio` é preferível que se informe o número de dentes das engrenagens que compõem o mecanismo (p. ex., prefira `gear_ratio: 80:20` a `gear_ratio: 4:1`). Veja o [documento Distância de Rotação](Rotation_Distance.md#using-a-gear_ratio) para mais informações.
   1. Evite definir valores de campo que estão configurados a seus valores padrão. Por exemplo, não se deve especificar `min_extrude_temp: 170` visto que esse já é o valor padrão.
   1. Onde possível, as linhas não devem ultrapassar 80 colunas.
   1. Evite adicionar mensagens de atribuição ou revisão nos arquivos de configuração. (Por exemplo, evite adicionar linhas como "Esse arquivo foi criado por ...".) Coloque a atribuição e o histórico de mudanças na mensagem de commit do git.
1. Não use recursos obsoletos no arquivo de configuração de exemplo.
1. Não desabilite o sistema de segurança padrão de um arquivo de configuração de exemplo. Por exemplo, a definição não deve especificar uma `max_extrude_cross_section` customizada. Não habilite os recursos de depuração. Por exemplo, não deve haver uma seção de configuração `force_move`.
1. Todas as placas suportadas pelo Klipper conseguem usar a taxa de transmissão serial padrão de 250000. Não recomende uma taxa de transmissão serial diferente no arquivo de exemplo de configuração.

Arquivos de configuração de exemplo são submetidos se criando um "pull request" no github. Por favor siga das instruções no [documento de contribuição](CONTRIBUTING.md).
