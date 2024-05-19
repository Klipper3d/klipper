#!/bin/bash

# in case build is executed from outside current dir be a gem and change the dir
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd -P)"
cd $SCRIPT_DIR

CROSS_PREFIX=
CROSS_PREFIX_ARG=
if [ "$1" = "host" ]; then
    export CROSS_PREFIX=mips-linux-gnu-
    CROSS_PREFIX_ARG="CROSS_PREFIX=$CROSS_PREFIX"
fi

cp .config.$1 .config
mkdir -p outfw
make ${CROSS_PREFIX_ARG} clean
make ${CROSS_PREFIX_ARG}

if [ "$1" = "host" ]; then
    if [ -f out/klipper.elf ]; then
        ${CROSS_PREFIX}strip out/klipper.elf -o outfw/klipper_mcu
        chmod ugo+x outfw/klipper_mcu
    fi
else
    mv out/${1}*.bin outfw/
fi
