#!/bin/bash
# Build setup script for continuous integration testing.
# See ci-build.sh for the actual test steps.

# Stop script early on any error; check variables; be verbose
set -eux

MAIN_DIR=${PWD}
BUILD_DIR=${PWD}/ci_build
CACHE_DIR=${PWD}/ci_cache

######################################################################
# Create python virtualenv environment
######################################################################

echo -e "\n\n=============== Install python virtualenv\n\n"
cd ${MAIN_DIR}
virtualenv -p python2 ${BUILD_DIR}/python-env

# Install klippy in venv.
${BUILD_DIR}/python-env/bin/pip install -e ${MAIN_DIR}/klippy
