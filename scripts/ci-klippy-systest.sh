#!/bin/bash

# Stop script early on any error; check variables; be verbose
set -eux

MAIN_DIR=${PWD}
BUILD_DIR=${PWD}/ci_build
CACHE_DIR=${PWD}/ci_cache

######################################################################
# Verify klippy host software via system tests
######################################################################

start_test klippy "Test invoke klippy"
KLIPPY="$KLIPPY" $PYTHON scripts/test_klippy.py -d ${DICTDIR} test/klippy/*.test
finish_test klippy "Test invoke klippy"
