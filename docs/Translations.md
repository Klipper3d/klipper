# Public documentation and translation policy

**English** | [Português brasileiro](pt-BR/Translations.md)

## Language contract

English is the canonical language of this fork. Every public Markdown guide,
README, release note and maintained plan has a complete Brazilian Portuguese
companion and an explicit language switch. Private documents and machine backups
are excluded. A copied English paragraph or a Portuguese summary is not a
finished translation.

Public integration commands, parameters, enums, configuration keys and status
fields remain English in both languages. The screen can independently use
English or Brazilian Portuguese. Human macro descriptions may be localized;
executable identifiers must not be translated or provided as Portuguese aliases.
Future release titles, release bodies and default public messages are English;
release notes also have Portuguese companions in this repository.

## Pinned upstream sources and credits

The 53 upstream guides completed for v2 reuse current-source, non-fuzzy,
non-obsolete Portuguese gettext translations from
[Klipper translations at 8cf7c1336ac7f02b1c094854948a8351d9552d50](https://github.com/Klipper3d/klipper-translations/tree/8cf7c1336ac7f02b1c094854948a8351d9552d50/docs/locales/pt_BR/LC_MESSAGES),
dated 2026-11-03. The English documents in this fork, not an old rendered
Portuguese page, determine the content. Fork-specific additions and source
differences are translated separately.

Credit remains with the Klipper authors and the
[Klipper Portuguese translation community](https://hosted.weblate.org/projects/klipper/-/pt_BR/).
Catalog headers at that pinned revision name Anonymous, Bernardo Scolnik,
Bertelli, Elyan Mouzinho Santana, Felipe Nogueira, Francisco Lima,
Matheus Mösken Diegues, Projetos Tech YouX, Rafael Neuwirth Swierczynski,
Roberto Besser, Wagner Reck, Zeldazul and gbonaspetti. These headers are not an
exhaustive contributor history; the pinned repository preserves that history.
Existing copyright notices, authors, source links and normative legal text are
retained. The documentation continues under this repository's
[GNU GPLv3 license](../COPYING); see also the
[upstream license](https://github.com/Klipper3d/klipper-translations/blob/8cf7c1336ac7f02b1c094854948a8351d9552d50/COPYING).
No new license or exclusive authorship is claimed for reused material.

## Completion and technical review

Missing passages and explanatory example comments were completed with
LLM-assisted editorial translation and read-only technical comparison against
English. The temporary import used upstream's gettext/Markdown approach with
`mdpo==2.1.4`. Catalog lookup must not silently fall back to English, including
when the renderer treats link-label newlines differently from extraction.

Commands, option examples, pins, formulas, numbers and literal diagnostic output
are preserved. Explanatory comments and human link labels are translated;
stable English anchor IDs remain available. Terminal output, interface field
labels, proper names and normative licenses may intentionally remain literal.
Review corrected inherited mistranslations of API keys, endstop scope, safety
negations and mesh fade warnings. Demonstrable command-name typos in upstream
guides were corrected consistently in English and Portuguese against registered
Klipper commands.

The checked-in Markdown is the maintained source. Temporary review batches and
catalog assembly tools are not production dependencies or a second publishing
system. Subsequent edits must update both language versions.

## Validation and publication

Run the regression tests and strict inventory-based checker:

```sh
python3 -m unittest discover -s test/public_docs -p 'test_*.py'
python3 scripts/check_public_docs.py
```

The checker covers language pairs and switches, local links and assets,
translated stable anchors, technical headings, inline code, executable examples,
commented configuration options, omitted source links, copied English prose,
legacy integration commands and private exclusion. Both pull-request CI and
release publication require the strict check. Structural success does not
replace semantic review, hardware commissioning or protected release checks.
