# Contributing to Klipper

Thank you for contributing to Klipper! Please take a moment to read
this document.

## Creating a new issue

Please see the [contact page](Contact.md) for information on creating
an issue.

## Submitting a pull request

Contributions of Code and documentation are managed through github
pull requests.  Each commit should have a commit message formatted
similar to the following:

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

It is important to have a "Signed-off-by" line on each commit - it
certifies that you agree to the
[developer certificate of origin](developer-certificate-of-origin). It
must contain your real name (sorry, no pseudonyms or anonymous
contributions) and contain a current email address.

## Contributing to Klipper Translations

[Klipper-translations Project](https://github.com/Klipper3d/klipper-translations)
is a project dedicated to translating Klipper to different languages.
[Weblate](https://hosted.weblate.org/projects/klipper/) hosts all the
Gettext strings for translating and reviewing. Locales can merge into
the Klipper project once they satisfy the following requirements:

- [ ] 75% Total coverage
- [ ] All titles (H1) are covered
- [ ] An updated navigation hierarchy PR in klipper-translations.

The navigation hierarchy is in `docs\_klipper3d\mkdocs.yml`.

To reduce the frustration of translating domain-specific terms and
gain awareness of the ongoing translations, you can submit a PR
modifying the
[Klipper-translations Project](https://github.com/Klipper3d/klipper-translations)
`readme.md`. Once a translation is ready, the corresponding
modification to the Klipper project can be made.

If a translation already exists in the Klipper repository and no
longer meets the checklist above, it will be marked out-of-date after
a month without updates.

Please follow the following format for `mkdocs.yml` navigation
hierarchy:

```yml
nav:
  - existing hierachy
  - <language>:
    - locales/<language code>/md file
```

Note: Currently, there isn't a method for correctly translating
pictures in the documentation.
