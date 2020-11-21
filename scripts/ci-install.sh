#!/bin/bash
# Build setup script for continuous integration testing.
# See ci-build.sh for the actual test steps.

# Stop script early on any error; check variables; be verbose
set -eux

. scripts/ci-funcs

# May already exist; that's not an error
mkdir -p ${BUILD_DIR} ${CACHE_DIR} || true


######################################################################
# Install system dependencies
######################################################################

echo -e "\n\n=============== Install system dependencies\n\n"
# For convenience of testing, this is skipped if you're not a root
# user to make the command easier to run locally as a non-root
# user.  The script will be run as root in CI.
sudo apt-get -y install gcc-avr avr-libc pv libmpfr-dev libgmp-dev \
     libmpc-dev texinfo libncurses5-dev bison flex python-virtualenv \
     virtualenv python-dev tox libffi-dev build-essential \
     libnewlib-arm-none-eabi gcc-arm-none-eabi binutils-arm-none-eabi


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
    git clone https://github.com/dinuxbg/gnupru -b 2018.03-beta-rc3 --depth 1
    cd gnupru
    export PREFIX=${PRU_DIR}
    ./download-and-patch.sh 2>&1 | pv -nli 30 > ${BUILD_DIR}/gnupru-build.log
    ./build.sh 2>&1 | pv -nli 30 >> ${BUILD_DIR}/gnupru-build.log
    cd ${BUILD_DIR}
    tar cfz ${PRU_FILE} pru-gcc/
else
    cd ${BUILD_DIR}
    tar xfz ${PRU_FILE}
fi
