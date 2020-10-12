#!/bin/bash
# Build setup script for travis-ci.org continuous integration testing.
# See travis-build.sh for the actual test steps.

# Stop script early on any error; check variables; be verbose
set -eux

MAIN_DIR=${PWD}
BUILD_DIR=${PWD}/travis_build
CACHE_DIR=${PWD}/travis_cache
mkdir -p ${BUILD_DIR} ${CACHE_DIR}


######################################################################
# Install embedded arm gcc
######################################################################

echo -e "\n\n=============== Install embedded arm gcc\n\n"
GCC_ARM_URL="https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2"
GCC_ARM_SHA="96a029e2ae130a1210eaa69e309ea40463028eab18ba19c1086e4c2dafe69a6a  gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2"
GCC_ARM_FILE="$(basename ${GCC_ARM_URL})"

cd ${CACHE_DIR}
if [ ! -f ${GCC_ARM_FILE} ]; then
    wget "$GCC_ARM_URL"
fi
FOUND_SHA=`sha256sum "$GCC_ARM_FILE"`
if [ "$FOUND_SHA" != "$GCC_ARM_SHA" ]; then
    echo "ERROR: Mismatch on gcc arm sha256"
    exit -1
fi
cd ${BUILD_DIR}
tar xf "${CACHE_DIR}/${GCC_ARM_FILE}"


######################################################################
# Install (or build) pru gcc
######################################################################

echo -e "\n\n=============== Install embedded pru gcc\n\n"
PRU_FILE=${CACHE_DIR}/gnupru.tar.gz
PRU_DIR=${BUILD_DIR}/pru-gcc

if [ ! -f ${PRU_FILE} ]; then
    cd ${BUILD_DIR}
    git clone https://github.com/dinuxbg/gnupru -b 2018.03-beta-rc3
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


######################################################################
# Create python virtualenv environment
######################################################################

echo -e "\n\n=============== Install python virtualenv\n\n"
cd ${MAIN_DIR}
virtualenv -p python2 ${BUILD_DIR}/python-env
${BUILD_DIR}/python-env/bin/pip install -r ${MAIN_DIR}/scripts/klippy-requirements.txt
