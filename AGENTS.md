## Start Here (All Agents)

Read this file first:
- `C:\microlay\firmware\dental_support_api\webapp\docs\STACK_MAESTRO_IVY.md`

It provides full-stack onboarding context:
- Platform architecture (firmware, Flutter app, cloud backend, Klipper/SKR v3)
- Current dev topology (`192.168.1.141`, Odroid integration)
- Production references (AWS Lightsail)
- Repository/folder map
- Credentials/key locations (paths only)

## Repo-Specific References

- `README.md`

## Git And Deploy Policy (Mandatory)

This repository follows a strict trunk-based workflow during the current single-developer phase.

1. Current canonical trunk branch for Microlay-owned delivery in this repo is `origin/master`.
2. `origin/upstream-endstop-reqclock-fix` is a legacy/special-purpose branch and must not be used as the default branch for new work unless the user explicitly asks for it.
3. Agents must not create local or remote branches, backup branches, or ad-hoc stashes unless the user explicitly asks for it.
4. If the repo is dirty, behind, or you need isolation from unrelated local work, use a temporary clone/worktree under `temp/` instead of inventing a new long-lived branch.
5. Any Klipper change that is going to be tested on Ivy must follow: local change -> commit on `master` -> push to `origin/master` -> integrate/deploy from `origin/master` through the repo-backed firmware flow.
6. Emergency hotfixes on the machine are allowed only for recovery; they must be reflected in Git and aligned back to the repo before closing the task.
7. Before closing a task, agents must confirm that the tested commit already exists in `origin/master` and explicitly report any remaining dirty files or drift.
8. If/when this repo is renamed to `main`, this policy transfers intact to `origin/main`.

Canonical cross-repo policy and operational details:
- `C:\microlay\firmware\dental_support_api\webapp\docs\STACK_MAESTRO_IVY.md`
- Section: `1.2.2) Politica obligatoria de Git/trunk y no-drift (todos los repos)`

## Root Hygiene Policy (Mandatory)

Keep the repository root clean.

Allowed in root:
- Canonical source, build files, and top-level docs already used by Klipper

Auxiliary files must go to dedicated folders:
- `docs/` for notes, audits, and plans
- `scripts/` for development utilities
- `test/` for test assets/code
- `temp/` for temporary local experiments if needed

Never in root:
- `fix_*`, `tmp_*`, `*_analysis*`, local credentials, ad-hoc exports

If a destination is unclear, create a dedicated folder or ask before adding files in root.
