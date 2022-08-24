#!/bin/bash
# Format python files passed as arguments, or format dirty and uncached files.
# If GIT_COMMIT environment variable is set to a reference, format the files
# affected by this commit only.
# If AMEND is set, amend the HEAD commit.

set -e

if [ "$#" -eq 0 ]; then
    if [ -z "$GIT_COMMIT" ]; then
        # Take dirty and cached files
        FILES=$(git diff-index HEAD -r --name-only)
    else
        # Take modified files by a commit (eg. for tree-filter)
        FILES=$(git diff-tree --no-commit-id --name-only -r --diff-filter=AM "$GIT_COMMIT")
    fi
    readarray -t PYFILES < <(echo "$FILES" | grep '\.py$' || true)
else
    PYFILES=("$@")
fi

if [ ${#PYFILES[@]} -eq 0 ]; then
    exit 0
fi

set +e # Formatters are allowed to fail

black $BLACK_ARGS "${PYFILES[@]}"
autoflake --in-place "${PYFILES[@]}"
autopep8 --in-place -a -a "${PYFILES[@]}"
black $BLACK_ARGS "${PYFILES[@]}"

if [ "$AMEND" ]; then
    git add -u "${PYFILES[@]}"
    git commit --amend --no-edit -q --allow-empty
fi
