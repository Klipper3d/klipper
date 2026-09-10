#!/bin/bash
# Script to check that declared cmd_XXX_params schemas stay in sync with
# their handlers (see scripts/check_gcode_params.py).

# Find SRCDIR from the pathname of this script
SRCDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/.. && pwd )"
cd ${SRCDIR}

find klippy -name '*.py' | xargs ./scripts/check_gcode_params.py
