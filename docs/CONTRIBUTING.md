# Contributing to Klipper

Thank you for contributing to Klipper! This document describes the
process for contributing changes to Klipper.

Please see the [contact page](Contact.md) for information on reporting
an issue or for details on contacting the developers.

## Overview of Contribution Process

Contributions to Klipper generally follow a high-level process:

1. A submitter starts by creating a
   [GitHub Pull Request](https://github.com/Klipper3d/klipper/pulls)
   when a submission is ready for widespread deployment.
2. When a [reviewer](#reviewers) is available to
   [review](#what-to-expect-in-a-review) the submission, they will
   assign themselves to the Pull Request on GitHub. The goal of the
   review is to look for defects and to check that the submission
   follows documented guidelines.
3. After a successful review, the reviewer will "approve the review"
   on GitHub and a [maintainer](#reviewers) will commit the change to
   the Klipper master branch.

When working on enhancements, consider starting (or contributing to) a
topic on [Klipper Discourse](Contact.md). An ongoing discussion on the
forum can improve visibility of development work and may attract
others interested in testing new work.

## What to expect in a review

Contributions to Klipper are reviewed before merging. The primary goal
of the review process is to check for defects and to check that the
submission follows guidelines specified in the Klipper documentation.

It is understood that there are many ways to accomplish a task; it is
not the intent of the review to discuss the "best" implementation.
Where possible, review discussions focused on facts and measurements
are preferable.

The majority of submissions will result in feedback from a review. Be
prepared to obtain feedback, provide further details, and to update
the submission if needed.

Common things a reviewer will look for:

1. Is the submission free of defects and is it ready to be widely
   deployed?

   Submitters are expected to test their changes prior to submission.
   The reviewers look for errors, but they don't, in general, test
   submissions. An accepted submission is often deployed to thousands
   of printers within a few weeks of acceptance. Quality of
   submissions is therefore considered a priority.

   The main [Klipper3d/klipper](https://github.com/Klipper3d/klipper)
   GitHub repository does not accept experimental work. Submitters
   should perform experimentation, debugging, and testing in their own
   repositories. The [Klipper Discourse](Contact.md) server is a good
   place to raise awareness of new work and to find users interested
   in providing real-world feedback.

   Submissions must pass all [regression test cases](Debugging.md).

   Code submissions should not contain excessive debugging code,
   debugging options, nor run-time debug logging.

   Comments in code submissions should focus on enhancing code
   maintenance. Submissions should not contain "commented out code"
   nor excessive comments describing past implementations. There
   should not be excessive "todo" comments.

   Updates to documentation should not declare that they are a "work
   in progress".

2. Is the copyright of the submission clear, non-gratuitous, and
   compatible?

   New C files and Python files should have an unambiguous copyright
   statement. See the existing files for the preferred format.
   Declaring a copyright on an existing file when making minor changes
   to that file is discouraged.

   Code taken from 3rd party sources must be compatible with the
   Klipper license (GNU GPLv3). Large 3rd party code additions should
   be added to the `lib/` directory (and follow the format described
   in [lib/README](../lib/README)).

   Submitters must provide a
   [Signed-off-by line](#format-of-commit-messages) using their full
   real name. It indicates the submitter agrees with the
   [developer certificate of origin](developer-certificate-of-origin).

3. Does the submission follow guidelines specified in the Klipper
   documentation?

   In particular, code should follow the guidelines in
   [Code_Overview.md](Code_Overview.md) and config files should follow
   the guidelines in [Example_Configs.md](Example_Configs.md).

4. Is the Klipper documentation updated to reflect new changes?

   At a minimum, the reference documentation must be updated with
   corresponding changes to the code:
   * All commands and command parameters must be documented in
     [G-Codes.md](G-Codes.md).
   * All user facing modules and their config parameters must be
     documented in [Config_Reference.md](Config_Reference.md).
   * All exported "status variables" must be documented in
     [Status_Reference.md](Status_Reference.md).
   * All new "webhooks" and their parameters must be documented in
     [API_Server.md](API_Server.md).
   * Any change that makes a non-backwards compatible change to a
     command or config file setting must be documented in
     [Config_Changes.md](Config_Changes.md).

   New documents should be added to [Overview.md](Overview.md) and be
   added to the website index
   [docs/_klipper3d/mkdocs.yml](../docs/_klipper3d/mkdocs.yml).

5. Are commits well formed, address a single topic per commit, and
   independent?

   Commit messages should follow the
   [preferred format](#format-of-commit-messages).

   Commits must not have a merge conflict. New additions to the
   Klipper master branch are always done via a "rebase" or "squash and
   rebase". It is generally not necessary for submitters to re-merge
   their submission on every update to the Klipper master repository.
   However, if there is a merge conflict, then submitters are
   recommended to use `git rebase` to address the conflict.

   Each commit should address a single high-level change. Large
   changes should be broken up into multiple independent commits. Each
   commit should "stand on its own" so that tools like `git bisect`
   and `git revert` work reliably.

   Whitespace changes should not be mixed with functional changes. In
   general, gratuitous whitespace changes are not accepted unless they
   are from the established "owner" of the code being modified.

6. Does the submission provide a "high impact" benefit to real-world
   users performing real-world tasks?

   Reviewers need to identify, at least in their own minds, roughly
   "who the target audience is", a rough scale of "the size of that
   audience", the "benefit" they will obtain, how the "benefit is
   measured", and the "results of those measurement tests". In most
   cases this will be obvious to both the submitter and the reviewer,
   and it is not explicitly stated during a review.

   Submissions to the master Klipper branch are expected to have a
   noteworthy target audience. As a general "rule of thumb",
   submissions should target a user base of at least a 100 real-world
   users.

   If a reviewer asks for details on the "benefit" of a submission,
   please don't consider it criticism. Being able to understand the
   real-world benefits of a change is a natural part of a review.

   When discussing benefits it is preferable to discuss "facts and
   measurements" instead of "opinions and theories". In general,
   reviewers are not looking for responses of the form "this
   submission may improve quality because of ...", nor are they
   looking for responses of the form "someone may find option X
   useful", nor are they looking for responses of the form "this
   submission adds a feature that firmware X implements". Instead, it
   is generally preferable to discuss details on how the quality
   improvement was measured and what were the results of those
   measurements - for example, "tests on Acme X1000 printers show
   improved corners as seen in picture ...", or for example "print
   time of real-world object X on a Foomatic X900 printer went from 4
   hours to 3.5 hours". It is understood that testing of this type can
   take significant time and effort. Some of Klipper's most notable
   features took years of discussion, rework, testing, and
   documentation prior to being merged into the master branch.

   All new modules, config options, commands, command parameters, and
   documents should have "high impact". We do not want to burden users
   with options that they can not reasonably configure nor do we want
   to burden them with options that don't provide a notable benefit.

   A reviewer may ask for clarification on how a user is to configure
   an option - an ideal response will contain details on the process -
   for example, "users of the MegaX500 are expected to set option X to
   99.3 while users of the Elite100Y are expected to calibrate option
   X using procedure ...".

   If the goal of an option is to make the code more modular then
   prefer using code constants instead of user facing config options.

   New modules, new options, and new parameters should not provide
   similar functionality to existing modules - if the differences are
   arbitrary than it's preferable to utilize the existing system or
   refactor the existing code.

Klipper does not implement a strict "coding style guide", but
modifications to existing code should follow the high-level code flow,
code indentation style, and format of that existing code. Submissions
of new modules and systems have more flexibility in coding style, but
it is preferable for that new code to follow an internally consistent
style and to generally follow industry wide coding norms.

It is not a goal of a review to discuss "better implementations".
However, if a reviewer struggles to understand the implementation of a
submission, then they may ask for changes to make the implementation
more transparent. In particular, if reviewers can not convince
themselves that a submission is free of defects then changes may be
necessary.

As part of a review, a reviewer may create an alternate Pull Request
for a topic. This may be done to avoid excessive "back and forth" on
minor procedural items and thus streamline the submission process. It
may also be done because the discussion inspires a reviewer to build
an alternative implementation. Both situations are a normal result of
a review and should not be considered criticism of the original
submission.

### Helping with reviews

We appreciate help with reviews! It is not necessary to be a
[listed reviewer](#reviewers) to perform a review. Submitters of
GitHub Pull Requests are also encouraged to review their own
submissions.

To help with a review, follow the steps outlined in
[what to expect in a review](#what-to-expect-in-a-review) to verify
the submission. After completing the review, add a comment to the
GitHub Pull Request with your findings. If the submission passes the
review then please state that explicitly in the comment - for example
something like "I reviewed this change according to the steps in the
CONTRIBUTING document and everything looks good to me". If unable to
complete some steps in the review then please explicitly state which
steps were reviewed and which steps were not reviewed - for example
something like "I didn't check the code for defects, but I reviewed
everything else in the CONTRIBUTING document and it looks good".

We also appreciate testing of submissions. If the code was tested then
please add a comment to the GitHub Pull Request with the results of
your test - success or failure. Please explicitly state that the code
was tested and the results - for example something like "I tested this
code on my Acme900Z printer with a vase print and the results were
good".

### Reviewers

The Klipper "reviewers" are:

| Name                   | GitHub Id         | Areas of interest |
| ---------------------- | ----------------- | ----------------- |
| Dmitry Butyugin        | @dmbutyugin       | Input shaping, resonance testing, kinematics |
| Eric Callahan          | @Arksine          | Bed leveling, MCU flashing |
| Kevin O'Connor         | @KevinOConnor     | Core motion system, Micro-controller code |
| Paul McGowan           | @mental405        | Configuration files, documentation |

Please do not "ping" any of the reviewers and please do not direct
submissions at them. All of the reviewers monitor the forums and PRs,
and will take on reviews when they have time to.

The Klipper "maintainers" are:

| Name                   | GitHub name       |
| ---------------------- | ----------------- |
| Kevin O'Connor         | @KevinOConnor     |

## Format of commit messages

Each commit should have a commit message formatted similar to the
following:

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

In the above example, `module` should be the name of a file or
directory in the repository (without a file extension). For example,
`clocksync: Fix typo in pause() call at connect time`. The purpose of
specifying a module name in the commit message is to help provide
context for the commit comments.

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
