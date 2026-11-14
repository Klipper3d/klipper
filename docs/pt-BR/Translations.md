# Documentação pública e política de tradução

[English](../Translations.md) | **Português brasileiro**

<a id="language-contract"></a>
## Contrato de idiomas

O inglês é o idioma canônico deste fork. Todo guia Markdown público, README,
nota de release e plano mantido tem um par integral em português brasileiro
e um seletor explícito de idioma. Documentos privados e backups de máquinas
ficam excluídos. Um parágrafo copiado em inglês ou um resumo em português
não constitui uma tradução concluída.

Comandos, parâmetros, enums, chaves de configuração e campos de status públicos
da integração continuam em inglês nos dois idiomas. A tela pode usar inglês
ou português brasileiro independentemente. Descrições humanas de macros podem
ser localizadas; identificadores executáveis não devem ser traduzidos nem
oferecidos como aliases em português. Títulos e corpos de releases futuras
e mensagens públicas padrão são em inglês; as notas de release também têm
pares em português neste repositório.

<a id="pinned-upstream-sources-and-credits"></a>
## Fontes upstream fixadas e créditos

Os 53 guias upstream concluídos para a v2 reutilizam traduções gettext em
português compatíveis com a fonte atual, não aproximadas (fuzzy) e não obsoletas,
do [Klipper translations em 8cf7c1336ac7f02b1c094854948a8351d9552d50](https://github.com/Klipper3d/klipper-translations/tree/8cf7c1336ac7f02b1c094854948a8351d9552d50/docs/locales/pt_BR/LC_MESSAGES),
de 2026-11-03. Os documentos em inglês deste fork, e não uma página antiga
renderizada em português, determinam o conteúdo. Acréscimos próprios do fork
e diferenças de fonte são traduzidos separadamente.

Os créditos continuam com os autores do Klipper e a
[comunidade de tradução do Klipper para português](https://hosted.weblate.org/projects/klipper/-/pt_BR/).
Os cabeçalhos dos catálogos nessa revisão fixada citam Anonymous,
Bernardo Scolnik, Bertelli, Elyan Mouzinho Santana, Felipe Nogueira,
Francisco Lima, Matheus Mösken Diegues, Projetos Tech YouX,
Rafael Neuwirth Swierczynski, Roberto Besser, Wagner Reck, Zeldazul e
gbonaspetti. Esses cabeçalhos não são um histórico exaustivo de colaboradores;
o repositório fixado preserva esse histórico. Avisos de copyright, autores,
links de origem e textos legais normativos existentes foram preservados.
A documentação continua sob a [licença GNU GPLv3](../../COPYING) deste
repositório; consulte também a
[licença upstream](https://github.com/Klipper3d/klipper-translations/blob/8cf7c1336ac7f02b1c094854948a8351d9552d50/COPYING).
Não se reivindica nova licença nem autoria exclusiva sobre material reutilizado.

<a id="completion-and-technical-review"></a>
## Complementação e revisão técnica

Passagens ausentes e comentários explicativos dos exemplos foram concluídos
com tradução editorial assistida por LLM e comparação técnica somente leitura
com o inglês. A importação temporária usou a abordagem gettext/Markdown upstream
com `mdpo==2.1.4`. A busca no catálogo não pode retornar silenciosamente ao
inglês, inclusive quando o renderizador trata quebras de linha nos rótulos de
links de maneira diferente da extração.

Comandos, exemplos de opções, pinos, fórmulas, números e saídas diagnósticas
literais são preservados. Comentários explicativos e rótulos humanos de links
são traduzidos; IDs estáveis de âncoras em inglês continuam disponíveis.
Saídas de terminal, nomes de campos de interfaces, nomes próprios e licenças
normativas podem permanecer literais intencionalmente. A revisão corrigiu
traduções herdadas incorretas de chaves de API, escopo de fins de curso,
negações de segurança e avisos de fade da malha. Erros demonstráveis em nomes
de comandos dos guias upstream foram corrigidos consistentemente em inglês
e português conforme os comandos registrados do Klipper.

O Markdown versionado é a fonte mantida. Lotes de revisão e ferramentas de
montagem de catálogos temporários não são dependências de produção nem um
segundo sistema de publicação. Edições seguintes devem atualizar os dois idiomas.

<a id="validation-and-publication"></a>
## Validação e publicação

Execute os testes de regressão e o checker estrito baseado no inventário:

```sh
python3 -m unittest discover -s test/public_docs -p 'test_*.py'
python3 scripts/check_public_docs.py
```

O checker cobre pares e seletores de idioma, links e arquivos locais, âncoras
estáveis traduzidas, títulos técnicos, código inline, exemplos executáveis,
opções de configuração comentadas, links da fonte omitidos, prosa inglesa
copiada, comandos legados da integração e exclusão de conteúdo privado.
Tanto o CI de pull requests quanto a publicação da release exigem o modo
estrito. Aprovação estrutural não substitui revisão semântica, comissionamento
de hardware ou verificações protegidas da release.
