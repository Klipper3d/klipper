<a id="contributing-to-klipper"></a>
# Contribuindo para o Klipper

[English](../CONTRIBUTING.md) | **Português brasileiro**

Obrigado por contribuir com o Klipper! Este documento descreve o processo para contribuir com alterações no Klipper.

Consulte a [página de contato](Contact.md) para obter informações sobre como relatar um problema ou detalhes sobre como entrar em contato com os desenvolvedores.

<a id="overview-of-contribution-process"></a>
## Visão geral do processo de contribuição

As contribuições para o Klipper geralmente seguem um processo de alto nível:

1. Um proponente começa criando um [Pull request do GitHub](https://github.com/Klipper3d/klipper/pulls) quando uma submissão está pronta para implantação ampla.
1. Quando um [revisor](#reviewers) estiver disponível para [revisar](#what-to-expect-in-a-review) a submissão, ele se atribuirá ao Pull Request no GitHub. O objetivo da revisão é procurar defeitos e verificar se a submissão segue as diretrizes documentadas.
1. Após uma revisão bem-sucedida, o revisor fará "approve the review" no GitHub e um [mantenedor](#reviewers) commitará a alteração no ramo master do Klipper.

Ao trabalhar em melhorias, considere iniciar (ou contribuir para) um tópico no [Klipper Discourse](Contact.md). Uma discussão contínua no fórum pode melhorar a visibilidade do trabalho de desenvolvimento e pode atrair outras pessoas interessadas em testar o novo trabalho.

<a id="what-to-expect-in-a-review"></a>
## O que esperar em uma revisão

As contribuições para o Klipper são revisadas antes da fusão. O objetivo principal do processo de revisão é verificar defeitos e verificar se o envio segue as diretrizes especificadas na documentação do Klipper.

Entende-se que há muitas maneiras de realizar uma tarefa; não é intenção da revisão discutir a "melhor" implementação. Quando possível, são preferíveis discussões de revisão focadas em fatos e medições.

A maioria dos envios resultará em retorno de uma revisão. Esteja preparado para obter retorno, fornecer mais detalhes e atualizar o envio se necessário.

Itens comuns que um revisor procurará:

1. O envio está livre de defeitos e pronto para ser amplamente implantado?

   Espera-se que os remetentes testem suas alterações antes do envio. Os revisores procuram erros, mas, em geral, não testam os envios. Um envio aceito costuma ser implantado em milhares de impressoras dentro de poucas semanas após a aceitação. A qualidade dos envios é, portanto, considerada uma prioridade.

   O repositório GitHub principal [Klipper3d/klipper](https://github.com/Klipper3d/klipper) não aceita trabalho experimental. Os remetentes devem realizar experimentação, depuração e testes em seus próprios repositórios. O servidor [Klipper Discourse](Contact.md) é um bom lugar para divulgar o novo trabalho e encontrar usuários interessados em fornecer retorno do mundo real.

   Os envios devem passar em todos os [casos de teste de regressão](Debugging.md).

   Ao corrigir um defeito no código, os remetentes devem ter uma compreensão geral da causa raiz desse defeito, e a correção deve visar essa causa raiz.

   Os envios de código não devem conter código de depuração excessivo, opções de depuração, nem registro de depuração em tempo de execução.

   Os comentários nos envios de código devem focar em melhorar a manutenção do código. Os envios não devem conter "código comentado" nem comentários excessivos descrevendo implementações passadas. Não deve haver comentários "todo" excessivos.

   Atualizações da documentação não devem declarar que são um "work in progress".
1. A submissão oferece um benefício de "high impact" para usuários reais executando tarefas reais?

   Os revisores precisam identificar, pelo menos em suas próprias mentes, aproximadamente "quem é o público-alvo", uma escala aproximada do "tamanho desse público", o "benefício" que obterão, como o "benefício é medido" e os "resultados desses testes de medição". Na maioria dos casos isso será óbvio tanto para o remetente quanto para o revisor, e não é declarado explicitamente durante uma revisão.

   Espera-se que os envios para o ramo principal do Klipper tenham um público-alvo notável. Como "regra geral", os envios devem visar uma base de usuários de pelo menos 100 usuários do mundo real.

   Se um revisor pedir detalhes sobre o "benefício" de um envio, não considere isso uma crítica. Ser capaz de entender os benefícios do mundo real de uma alteração é uma parte natural de uma revisão.

   Ao discutir benefícios, é preferível discutir "fatos e medições". Em geral, os revisores não procuram respostas do tipo "alguém pode achar a opção X útil", nem procuram respostas do tipo "este envio adiciona um recurso que o firmware X implementa". Em vez disso, geralmente é preferível discutir detalhes sobre como a melhoria de qualidade foi medida e quais foram os resultados dessas medições - por exemplo, "testes em impressoras Acme X1000 mostram cantos melhorados como visto na imagem ...", ou por exemplo "o tempo de impressão do objeto X do mundo real em uma impressora Foomatic X900 passou de 4 horas para 3,5 horas". Entende-se que testes desse tipo podem exigir tempo e esforço significativos. Alguns dos recursos mais notáveis do Klipper levaram meses de discussão, retrabalho, testes e documentação antes de serem fundidos no ramo principal.

   Todos os novos módulos, opções de configuração, comandos, parâmetros de comando e documentos devem ter "alto impacto". Não queremos sobrecarregar os usuários com opções que eles não podem razoavelmente configurar nem queremos sobrecarregá-los com opções que não forneçam um benefício notável.

   Um revisor pode pedir esclarecimentos sobre como um usuário deve configurar uma opção - uma resposta ideal conterá detalhes sobre o processo - por exemplo, "espera-se que usuários do MegaX500 definam a opção X como 99.3, enquanto usuários do Elite100Y devem calibrar a opção X usando o procedimento ...".

   Se o objetivo de uma opção for tornar o código mais modular, prefira usar constantes de código em vez de opções de configuração voltadas ao usuário.

   Novos módulos, novas opções e novos parâmetros não devem fornecer funcionalidade similar à de módulos existentes - se as diferenças forem arbitrárias, é preferível utilizar o sistema existente ou refatorar o código existente.
1. O copyright da submissão é claro, não gratuito e compatível?

   Novos arquivos C e arquivos Python devem ter uma declaração de direitos autorais inequívoca. Consulte os arquivos existentes para o formato preferido. Declarar direitos autorais em um arquivo existente ao fazer pequenas alterações nesse arquivo é desencorajado.

   O código obtido de fontes de terceiros deve ser compatível com a licença do Klipper (GNU GPLv3). Adições grandes de código de terceiros devem ser incluídas no diretório `lib/` (e seguir o formato descrito em [lib/README](../../lib/README)).

   Os proponentes devem fornecer uma [linha Signed-off-by](#format-of-commit-messages) usando seu nome real completo. Ela indica que o proponente concorda com o [certificado de origem do desenvolvedor](../developer-certificate-of-origin).
1. A submissão segue as diretrizes especificadas na documentação do Klipper?

   Em particular, o código deve seguir as diretrizes em [Code_Overview.md](Code_Overview.md) e os arquivos de configuração devem seguir as diretrizes em [Example_Configs.md](Example_Configs.md).
1. A documentação do Klipper foi atualizada para refletir as novas alterações?

   No mínimo, a documentação de referência deve ser atualizada com as alterações correspondentes ao código:

   * Todos os comandos e parâmetros de comando devem ser documentados em [G-Codes.md](G-Codes.md).
   * Todos os módulos voltados ao usuário e seus parâmetros de configuração devem ser documentados em [Config_Reference.md](Config_Reference.md).
   * Todas as "status variables" exportadas devem ser documentadas em [Status_Reference.md](Status_Reference.md).
   * Todos os novos "webhooks" e seus parâmetros devem ser documentados em [API_Server.md](API_Server.md).
   * Qualquer alteração que faça uma mudança não compatível com versões anteriores em um comando ou configuração de arquivo de configuração deve ser documentada em [Config_Changes.md](Config_Changes.md).

Novos documentos devem ser adicionados a [Overview.md](Overview.md) e ao índice do site [docs/_klipper3d/mkdocs.yml](../_klipper3d/mkdocs.yml).

1. Os commits são bem formados, tratam de um único tópico por commit e são independentes?

   As mensagens de commit devem seguir o [formato preferido](#format-of-commit-messages).

   Os commits não devem ter conflito de fusão. Novas adições ao ramo principal do Klipper são sempre feitas via "rebase" ou "squash and rebase". Geralmente não é necessário que os remetentes refaçam a fusão de seu envio a cada atualização do repositório principal do Klipper. No entanto, se houver um conflito de fusão, recomenda-se que os remetentes usem `git rebase` para resolver o conflito.

   Cada commit deve tratar de uma única alteração de alto nível. Alterações grandes devem ser divididas em vários commits independentes. Cada commit deve "funcionar por conta própria" para que ferramentas como `git bisect` e `git revert` funcionem de forma confiável.

   Alterações de espaço em branco não devem ser misturadas com alterações funcionais. Em geral, alterações gratuitas de espaço em branco não são aceitas, a menos que sejam do "dono" estabelecido do código sendo modificado.

O Klipper não implementa um "guia de estilo de codificação" estrito, mas modificações no código existente devem seguir o fluxo de código de alto nível, o estilo de recuo de código e o formato desse código existente. Envios de novos módulos e sistemas têm mais flexibilidade no estilo de codificação, mas é preferível que esse novo código siga um estilo internamente consistente e geralmente siga as normas gerais de codificação do setor.

Não é objetivo de uma revisão discutir "melhores implementações". No entanto, se um revisor tiver dificuldade para entender a implementação de um envio, ele poderá pedir alterações para tornar a implementação mais transparente. Em particular, se os revisores não conseguirem se convencer de que um envio está livre de defeitos, alterações poderão ser necessárias.

Como parte de uma revisão, um revisor pode criar um Pull Request alternativo para um tópico. Isso pode ser feito para evitar "idas e vindas" excessivas em itens processuais menores e, assim, agilizar o processo de envio. Também pode ser feito porque a discussão inspira um revisor a criar uma implementação alternativa. Ambas as situações são um resultado normal de uma revisão e não devem ser consideradas crítica ao envio original.

<a id="helping-with-reviews"></a>
### Ajudando com revisões

Agradecemos a ajuda com revisões! Não é necessário ser um [revisor listado](#reviewers) para realizar uma revisão. Remetentes de Pull Requests do GitHub também são incentivados a revisar seus próprios envios.

Para ajudar com uma revisão, siga as etapas descritas em [o que esperar em uma revisão](#what-to-expect-in-a-review) para verificar o envio. Após concluir a revisão, adicione um comentário ao Pull Request do GitHub com suas descobertas. Se o envio passar na revisão, declare isso explicitamente no comentário - por exemplo, algo como "Revisei esta alteração de acordo com as etapas do documento CONTRIBUTING e tudo parece bom para mim". Se não for possível concluir algumas etapas da revisão, declare explicitamente quais etapas foram revisadas e quais não foram - por exemplo, algo como "Não verifiquei o código quanto a defeitos, mas revisei todo o resto do documento CONTRIBUTING e parece bom".

Também agradecemos testes de envios. Se o código foi testado, adicione um comentário ao Pull Request do GitHub com os resultados do seu teste - sucesso ou falha. Declare explicitamente que o código foi testado e os resultados - por exemplo, algo como "Testei este código na minha impressora Acme900Z com uma impressão de vaso e os resultados foram bons".

<a id="reviewers"></a>
### Revisores

Os "revisores" do Klipper são:

| Nome | Id do GitHub | Áreas de interesse |
| --- | --- | --- |
| Dmitry Butyugin | @dmbutyugin | Modelagem de entrada, teste de ressonância, cinemática |
| Eric Callahan | @Arksine | Nivelamento da mesa, gravação de MCU |
| James Hartley | @JamesH1978 | Arquivos de configuração |
| Kevin O'Connor | @KevinOConnor | Sistema de movimento principal, código de microcontrolador |

Não faça "ping" em nenhum dos revisores e não direcione envios a eles. Todos os revisores acompanham os fóruns e os PRs, e assumirão revisões quando tiverem tempo.

Os "mantenedores" do Klipper são:

| Nome | Nome no GitHub |
| --- | --- |
| Kevin O'Connor | @KevinOConnor |

<a id="format-of-commit-messages"></a>
## Formato das mensagens de commit

Cada commit deve ter uma mensagem de commit formatada de forma semelhante ao seguinte:

```
module: Capitalized, short (50 chars or less) summary

More detailed explanatory text, if necessary.  Wrap it to about 75
characters or so.  In some contexts, the first line is treated as the
subject of an email and the rest of the text as the body.  The blank
line separating the summary from the body is critical (unless you omit
the body entirely); tools like rebase can get confused if you run the
two together.

Further paragraphs come after blank lines..

Signed-off-by: My Name <myemail@example.org>
```

No exemplo acima, `module` deve ser o nome de um arquivo ou diretório no repositório (sem extensão de arquivo). Por exemplo, `clocksync: Fix typo in pause() call at connect time`. O objetivo de especificar um nome de módulo na mensagem de commit é ajudar a fornecer contexto para os comentários do commit.

É importante ter uma linha "Signed-off-by" em cada commit - ela certifica que você concorda com o [certificado de origem do desenvolvedor](../developer-certificate-of-origin). Ela deve conter seu nome real (desculpe, sem pseudônimos ou contribuições anônimas) e conter um endereço de e-mail atual.

<a id="contributing-to-klipper-translations"></a>
## Contribuindo para as traduções do Klipper

O [projeto Klipper-translations](https://github.com/Klipper3d/klipper-translations) é um projeto dedicado a traduzir o Klipper para diferentes idiomas. O [Weblate](https://hosted.weblate.org/projects/klipper/) hospeda todas as cadeias Gettext para tradução e revisão. Os locais podem ser exibidos em [klipper3d.org](https://www.klipper3d.org) quando satisfizerem os seguintes requisitos:

- [ ] 75% de cobertura total
- [ ] Todos os títulos (H1) traduzidos
- [ ] Um PR atualizado da hierarquia de navegação em klipper-translations.

Para reduzir a frustração de traduzir termos específicos do domínio e ganhar conhecimento sobre as traduções em andamento, você pode enviar um PR modificando o `readme.md` do [projeto Klipper-translations](https://github.com/Klipper3d/klipper-translations). Quando uma tradução estiver pronta, a modificação correspondente no projeto Klipper pode ser feita.

Se já existir uma tradução no repositório do Klipper e ela não atender mais à lista de verificação acima, ela será marcada como desatualizada após um mês sem atualizações.

Uma vez que os requisitos sejam atendidos, você precisa:

1. atualize o repositório klipper-translations [active_translations](https://github.com/Klipper3d/klipper-translations/blob/translations/active_translations)
1. Opcional: adicione um arquivo manual-index.md na pasta `docs\locals\<lang>` do repositório klipper-translations para substituir o index.md específico do idioma (o index.md gerado não é renderizado corretamente).

Problemas conhecidos:

1. Atualmente, não há um método para traduzir corretamente as imagens na documentação
1. É impossível traduzir títulos em mkdocs.yml.
