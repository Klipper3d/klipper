#!/bin/bash
# Test script for travis-ci.org continuous integration.

# Stop script early on any error; check variables; be verbose
set -eux

# Paths to tools installed by travis-install.sh
MAIN_DIR=${PWD}
BUILD_DIR=${PWD}/travis_build
export PATH=${BUILD_DIR}/gcc-arm-none-eabi-7-2017-q4-major/bin:${PATH}
export PATH=${BUILD_DIR}/pru-gcc/bin:${PATH}
PYTHON=${BUILD_DIR}/python-env/bin/python


######################################################################
# Check for whitespace errors
######################################################################

echo "travis_fold:start:check_whitespace"
echo "=============== Check whitespace"
WS_DIRS="config/ docs/ klippy/ scripts/ src/ test/"
WS_EXCLUDE="-path scripts/kconfig -prune"
WS_FILES="-o -iname '*.[csh]' -o -name '*.py' -o -name '*.sh'"
WS_FILES="$WS_FILES -o -name '*.md' -o -name '*.cfg'"
WS_FILES="$WS_FILES -o -name '*.test' -o -name '*.config'"
WS_FILES="$WS_FILES -o -iname '*.lds' -o -iname 'Makefile' -o -iname 'Kconfig'"
eval find $WS_DIRS $WS_EXCLUDE $WS_FILES | xargs ./scripts/check_whitespace.py
echo "travis_fold:end:check_whitespace"


######################################################################
# Run compile tests for several different MCU types
######################################################################

DICTDIR=${BUILD_DIR}/dict
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

HOSTDIR=${BUILD_DIR}/hosttest
mkdir -p ${HOSTDIR}

echo "travis_fold:start:klippy"
echo "=============== Test invoke klippy"
$PYTHON scripts/test_klippy.py -d ${DICTDIR} test/klippy/*.test
echo "travis_fold:end:klippy"
