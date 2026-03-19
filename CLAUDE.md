# CLAUDE.md - Microlay Klipper Fork

## Start Here (Project Context)

Read first:
- `C:\microlay\firmware\dental_support_api\webapp\docs\STACK_MAESTRO_IVY.md`

This is the cross-repo source of truth for:
- Full stack architecture (RPI firmware, Flutter Odroid, microl.ai cloud, Klipper/SKR v3)
- Current machine/dev topology
- Production references
- Repository map
- Credentials/key locations (paths only)

## Mandatory Git And Deploy Policy

This repository follows a strict trunk-based workflow during the current single-developer phase.

- Current canonical trunk branch for Microlay-owned delivery in this repo is `origin/master`.
- `origin/upstream-endstop-reqclock-fix` is a legacy/special-purpose branch and must not be used as the default branch for new work unless the user explicitly asks for it.
- Do not create local/remote branches, backup branches, or ad-hoc stashes unless the user explicitly asks for it.
- If the repo is dirty, behind, or isolation is needed, use a temporary clone/worktree under `temp/` instead of inventing a new long-lived branch.
- Anything tested on Ivy must follow: local change -> commit on `master` -> push to `origin/master` -> integrate/deploy from `origin/master` through the repo-backed firmware flow.
- Emergency machine hotfixes are only for recovery and must be reflected in Git and realigned back to the repo before closing the task.
- Before closing a task, ensure the tested commit already exists in `origin/master` and explicitly report any remaining dirty files or drift.
- If/when this repo is renamed to `main`, this policy transfers intact to `origin/main`.

Canonical cross-repo details:
- `C:\microlay\firmware\dental_support_api\webapp\docs\STACK_MAESTRO_IVY.md`
- Section: `1.2.2) Politica obligatoria de Git/trunk y no-drift (todos los repos)`

## Root Hygiene Policy (Mandatory)

Keep the repository root clean.

Allowed in root:
- Canonical Klipper source/build files and top-level docs

Auxiliary files must go to dedicated folders:
- `docs/` for notes, audits, and plans
- `scripts/` for development utilities
- `test/` for tests
- `temp/` for temporary local experiments if needed

Never in root:
- `fix_*`, `tmp_*`, `*_analysis*`, local credentials, ad-hoc exports
