#!/bin/bash
# Build setup script for continuous integration testing.
# See ci-build.sh for the actual test steps.

# Stop script early on any error; check variables; be verbose
set -eux

MAIN_DIR=${PWD}
BUILD_DIR=${PWD}/ci_build
CACHE_DIR=${PWD}/ci_cache
mkdir -p ${BUILD_DIR} ${CACHE_DIR}


######################################################################
# Install system dependencies
######################################################################

echo -e "\n\n=============== Install system dependencies\n\n"
PKGS="virtualenv python3-dev libffi-dev build-essential"
PKGS="${PKGS} gcc-avr avr-libc"
PKGS="${PKGS} libnewlib-arm-none-eabi gcc-arm-none-eabi binutils-arm-none-eabi"
PKGS="${PKGS} pv libmpfr-dev libgmp-dev libmpc-dev texinfo bison flex"
sudo apt-get install -y ${PKGS}


######################################################################
# Install or1k-linux-musl toolchain
######################################################################

echo -e "\n\n=============== Install or1k-linux-musl toolchain\n\n"
TOOLCHAIN=or1k-linux-musl-cross
TOOLCHAIN_ZIP=${TOOLCHAIN}.tgz
GCC_VERSION=10
TOOLCHAIN_ZIP_V=${TOOLCHAIN}-${GCC_VERSION}.tgz
URL=https://more.musl.cc/${GCC_VERSION}/x86_64-linux-musl/
if [ ! -f ${CACHE_DIR}/${TOOLCHAIN_ZIP_V} ]; then
    curl ${URL}/${TOOLCHAIN_ZIP} -o ${CACHE_DIR}/${TOOLCHAIN_ZIP_V}
fi
cd ${BUILD_DIR}
tar xf ${CACHE_DIR}/${TOOLCHAIN_ZIP_V}


######################################################################
# Create python3 virtualenv environment
######################################################################

echo -e "\n\n=============== Install python3 virtualenv\n\n"
cd ${MAIN_DIR}
virtualenv -p python3 ${BUILD_DIR}/python-env
${BUILD_DIR}/python-env/bin/pip install -r ${MAIN_DIR}/scripts/klippy-requirements.txt
