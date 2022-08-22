#!/usr/bin/env bash
# Script to add extra files to the .gitignore file in `klippy/extras/`
set -euo pipefail

FORCE=false
QUIET=false
VERBOSE=false

function echo_if_not_quiet {
    if [ "$QUIET" = false ]; then
        echo "$1" >&2
    fi
}
function echo_if_verbose {
    if [ "$VERBOSE" = true ]; then
        echo "$1" >&2
    fi
}

function append_to_gitignore {
    echo_if_verbose "Appending '"'!'"${1}' to klippy/extras/.gitignore"
    echo '!'"$1" >>.gitignore
}

while getopts ":f,:h,:q,:v" opt; do
    case $opt in
    h)
        echo "Add extra files to the .gitignore file in 'klippy/extras/'

USAGE:
    $(basename "${0}") [-f] [-q] [-v] [-h]

FLAGS:
    -f  Add all files without confirmation (default: 'no')
    -q  Quiet mode (default: 'no')
    -v  Verbose mode (default: 'no')
    -h  Show this help message"
        exit 0
        ;;
    f)
        FORCE=true
        ;;
    q)
        QUIET=true
        ;;
    v)
        VERBOSE=true
        ;;
    \?)
        echo "Invalid option: -${OPTARG}" >&2
        exit 1
        ;;
    :)
        echo "Option -$OPTARG requires an argument." >&2
        exit 1
        ;;
    esac
done

if [ "${FORCE}" = false ] && [ "${QUIET}" = true ]; then
    echo "-q is only valid with -f" >&2
    exit 1
fi
if [ "${VERBOSE}" = true ] && [ "${QUIET}" = true ]; then
    echo "-v and -q are mutually exclusive" >&2
    exit 1
fi

# Find SRCDIR from the pathname of this script
SRCDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"/.. && pwd)"
cd "${SRCDIR}/klippy/extras/"

readarray -t ignored_files < <(git check-ignore *.py)

if [ ${#ignored_files[@]} -eq 0 ]; then
    echo_if_not_quiet "No files are ignored"
    exit 0
fi

if [ "${FORCE}" = true ]; then
    echo_if_not_quiet 'New (currently ignored) files in klippy/extras:'
    for file in "${ignored_files[@]}"; do
        echo_if_not_quiet "  - ${file}"
    done
    echo_if_not_quiet 'Adding files to klippy/extras/.gitignore...'

    for i in "${ignored_files[@]}"; do
        append_to_gitignore "${i}"
    done
    echo_if_verbose "Added ${#ignored_files[@]} files to klippy/extras/.gitignore"
    echo_if_not_quiet "Done!"
    exit 0
fi

tmpfile=$(mktemp -t klippy-extras-gitignore)
trap 'rm -- "${tmpfile}"' EXIT
for file in "${ignored_files[@]}"; do
    echo "pick ${file}" >>"${tmpfile}"
done

cat <<EOF >>"${tmpfile}"

# Adding new files to klippy/extras/.gitignore
#
# This file is similar in syntax to 'git-rebase-todo'
#
# Commands:
# p, pick <file> = Add the file as an exclusion to .gitignore (ie. will be ignored)
#
# If you remove a line here, that file will continue to be ignored
#
# If you remove everything, the script will be aborted
EOF

${EDITOR:-nano} "${tmpfile}"

# Need to use lastpipe here, as otherwise we can't append to the files_to_add array within the loop
# https://mywiki.wooledge.org/BashFAQ/024

set +m
shopt -s lastpipe
files_to_add=()

grep -v '^#' "${tmpfile}" | while read -r line; do
    if [ "${line}" = "" ]; then
        continue
    fi
    if [ "${line:0:1}" = "#" ]; then
        continue
    fi
    if [ "${line:0:2}" = "p " ]; then
        line="${line:2}"
    elif [ "${line:0:5}" = "pick " ]; then
        line="${line:5}"
    else
        echo "Invalid line: ${line}" >&2
        exit 1
    fi
    if [ ! -f "${line}" ]; then
        echo "File not found: ${line}" >&2
        exit 1
    fi
    files_to_add+=("${line}")
done

if [ ${#files_to_add[@]} -eq 0 ]; then
    echo "No new files to exclude" >&2
    exit 0
fi

echo_if_not_quiet 'Adding files to klippy/extras/.gitignore...'

for file in "${files_to_add[@]}"; do
    append_to_gitignore "${file}"
done
echo_if_verbose "Added ${#files_to_add[@]} files to klippy/extras/.gitignore"
echo_if_not_quiet "Done!"
