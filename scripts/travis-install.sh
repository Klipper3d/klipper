#!/bin/bash
# Build setup script for travis-ci.org continuous integration testing.
# See travis-build.sh for the actual test steps.

# Stop script early on any error; check variables; be verbose
set -eux


######################################################################
# Install embedded arm gcc
######################################################################

echo "=============== Install embedded arm gcc"
GCC_ARM_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2"
GCC_ARM_SHA="96a029e2ae130a1210eaa69e309ea40463028eab18ba19c1086e4c2dafe69a6a  gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2"
GCC_ARM_FILE="$(basename ${GCC_ARM_URL})"

wget "$GCC_ARM_URL"
FOUND_SHA=`sha256sum "$GCC_ARM_FILE"`
if [ "$FOUND_SHA" != "$GCC_ARM_SHA" ]; then
    echo "ERROR: Mismatch on gcc arm sha256"
    exit -1
fi
tar xf "$GCC_ARM_FILE"


######################################################################
# Create python virtualenv environment
######################################################################

echo "=============== Install python virtualenv"
virtualenv python-env
./python-env/bin/pip install cffi==1.6.0 pyserial==3.2.1 greenlet==0.4.10
