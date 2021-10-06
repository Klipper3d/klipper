#!/bin/bash
# Script to check whitespace in Klipper source code.

# Find SRCDIR from the pathname of this script
SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"
cd ${SRCDIR}

# Run whitespace tool on all source files
WS_DIRS="config/ docs/ klippy/ scripts/ src/ test/"
WS_EXCLUDE="-path scripts/kconfig -prune"
WS_FILES="-o -iname '*.[csh]' -o -name '*.py' -o -name '*.sh'"
WS_FILES="$WS_FILES -o -name '*.md' -o -name '*.cfg' -o -name '*.txt'"
WS_FILES="$WS_FILES -o -name '*.html' -o -name '*.css'"
WS_FILES="$WS_FILES -o -name '*.yaml' -o -name '*.yml'"
WS_FILES="$WS_FILES -o -name '*.css' -o -name '*.yaml' -o -name '*.yml'"
WS_FILES="$WS_FILES -o -name '*.test' -o -name '*.config'"
WS_FILES="$WS_FILES -o -iname '*.lds' -o -iname 'Makefile' -o -iname 'Kconfig'"
eval find $WS_DIRS $WS_EXCLUDE $WS_FILES | xargs ./scripts/check_whitespace.py
