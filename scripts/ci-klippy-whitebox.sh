#!/bin/bash

# Stop script early on any error; check variables; be verbose
set -eux

. scripts/ci-funcs

######################################################################
# Verify klippy host software via system tests
######################################################################

start_test klippy "Test klippy with static analysis"
(
    cd ${MAIN_DIR}/klippy
    tox
    rm -rf .tox .mypy_cache/ .stestr/ .eggs || true
)
finish_test klippy "Test klippy with static analysis"
