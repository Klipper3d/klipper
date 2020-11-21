#!/bin/bash
# Test script for continuous integration.

# Stop script early on any error; check variables
set -eu

. scripts/ci-funcs

######################################################################
# Run compile tests for several different MCU types
######################################################################

DICTDIR=${BUILD_DIR}/dict
mkdir -p ${DICTDIR}

for TARGET in test/configs/*.config ; do
    start_test mcu_compile "$TARGET"
    make clean
    make distclean
    unset CC
    cp ${TARGET} .config
    make olddefconfig
    make V=1
    finish_test mcu_compile "$TARGET"
    cp out/klipper.dict ${DICTDIR}/$(basename ${TARGET} .config).dict
done
