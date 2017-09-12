#!/bin/bash
# Test script for travis-ci.org continuous integration.

# Stop script early on any error; check variables; be verbose
set -eux

# Paths to tools installed by travis-install.sh
export PATH=${PWD}/gcc-arm-none-eabi-7-2017-q4-major/bin:${PATH}
PYTHON=${PWD}/python-env/bin/python


######################################################################
# Run compile tests for several different MCU types
######################################################################

DICTDIR=${PWD}/dict
mkdir -p ${DICTDIR}

for TARGET in test/configs/*.config ; do
    echo "travis_fold:start:mcu_compile $TARGET"
    echo "=============== Test compile $TARGET"
    make clean
    make distclean
    unset CC
    cp ${TARGET} .config
    make olddefconfig
    make V=1
    cp out/klipper.dict ${DICTDIR}/$(basename ${TARGET} .config).dict
    echo "travis_fold:end:mcu_compile $TARGET"
done


######################################################################
# Verify klippy host software
######################################################################

HOSTDIR=${PWD}/hosttest
mkdir -p ${HOSTDIR}

echo "travis_fold:start:klippy"
echo "=============== Test invoke klippy"
$PYTHON scripts/test_klippy.py -d ${DICTDIR} test/klippy/*.test
echo "travis_fold:end:klippy"
