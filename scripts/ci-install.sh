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
PKGS="virtualenv python2-dev libffi-dev build-essential"
PKGS="${PKGS} gcc-avr avr-libc"
PKGS="${PKGS} libnewlib-arm-none-eabi gcc-arm-none-eabi binutils-arm-none-eabi"
PKGS="${PKGS} pv libmpfr-dev libgmp-dev libmpc-dev texinfo bison flex"
sudo apt-get install ${PKGS}


######################################################################
# Install (or build) pru gcc
######################################################################

echo -e "\n\n=============== Install embedded pru gcc\n\n"
PRU_FILE=${CACHE_DIR}/gnupru.tar.gz
PRU_DIR=${BUILD_DIR}/pru-gcc

if [ ! -f ${PRU_FILE} ]; then
    cd ${BUILD_DIR}
    git config --global user.email "you@example.com"
    git config --global user.name "Your Name"
    git clone https://github.com/dinuxbg/gnupru -b 2023.01 --depth 1
    cd gnupru
    export PREFIX=${PRU_DIR}
    ./download-and-prepare.sh 2>&1 | pv -nli 30 > ${BUILD_DIR}/gnupru-build.log
    ./build.sh 2>&1 | pv -nli 30 >> ${BUILD_DIR}/gnupru-build.log
    cd ${BUILD_DIR}
    tar cfz ${PRU_FILE} pru-gcc/
else
    cd ${BUILD_DIR}
    tar xfz ${PRU_FILE}
fi

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


######################################################################
# Create python2 virtualenv environment
######################################################################

echo -e "\n\n=============== Install python2 virtualenv\n\n"
cd ${MAIN_DIR}
virtualenv -p python2 ${BUILD_DIR}/python2-env
${BUILD_DIR}/python2-env/bin/pip install -r ${MAIN_DIR}/scripts/klippy-requirements.txt
