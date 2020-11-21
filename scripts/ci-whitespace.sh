#!/bin/bash
# Test script for continuous integration.

# Stop script early on any error; check variables
set -eu

. scripts/ci-funcs

######################################################################
# Check for whitespace errors
######################################################################

start_test check_whitespace "Check whitespace"
./scripts/check_whitespace.sh
finish_test check_whitespace "Check whitespace"
