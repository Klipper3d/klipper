#!/bin/bash

# Force script to exit if an error occurs
set -e

# Find SRCDIR from the pathname of this script
SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"

DO_BUILD="n"
`docker image inspect klipper-builder >/dev/null 2>&1` || DO_BUILD="y"  # image missing
[ "$1" = "-b" ] && shift && DO_BUILD="y"  # force build
if [ "$DO_BUILD" = "y" ]; then
    docker build \
        -f "${SRCDIR}/scripts/Dockerfile" \
        --target builder \
        --progress=plain \
        -t klipper-builder \
        "${SRCDIR}" || exit 1
fi

exec docker run -it --rm -u `id -u`:`id -g` -v "${SRCDIR}":"${SRCDIR}":rw -w "${SRCDIR}" klipper-builder "$@"
