#!/bin/bash
# Test script for continuous integration.

# Stop script early on any error; check variables
set -eu

# Paths to tools installed by ci-install.sh
MAIN_DIR=${PWD}
BUILD_DIR=${PWD}/ci_build
export PATH=${BUILD_DIR}/pru-gcc/bin:${PATH}
export PATH=${BUILD_DIR}/or1k-linux-musl-cross/bin:${PATH}
PYTHON=${BUILD_DIR}/python-env/bin/python

######################################################################
# Section grouping output message helpers
######################################################################

start_test() {
    echo "::group::=============== $1 $2"
    set -x
}

finish_test() {
    set +x
    echo "=============== Finished $2"
    echo "::endgroup::"
}

######################################################################
# Check for whitespace errors
######################################################################

#start_test check_whitespace "Check whitespace"
#./scripts/check_whitespace.sh
#finish_test check_whitespace "Check whitespace"

######################################################################
# Run compile tests for several different MCU types
######################################################################

OUTPUTDIR=${BUILD_DIR}/output
mkdir -p ${OUTPUTDIR}

TARGET="test/configs/displayv3se.config"

start_test mcu_compile "$TARGET"
make clean
make distclean
unset CC
cp ${TARGET} .config
make olddefconfig
make V=1
ls -l out/*.bin
md5sum out/*.bin
finish_test mcu_compile "$TARGET"
cp out/klipper.bin ${OUTPUTDIR}/$(basename ${TARGET} .config).bin

######################################################################
# Verify klippy host software
######################################################################

start_test klippy "Test klippy import (Python3)"
$PYTHON klippy/klippy.py --import-test
finish_test klippy "Test klippy import (Python3)"
