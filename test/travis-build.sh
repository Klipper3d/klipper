#!/bin/bash
set -eux
if [ -z ${TARGET+x} ]; then
    if [ -z ${1+x} ]; then
        echo "Need a TARGET as environment variable or first parameter!"
        exit 1
    else
        TARGET="$1"
    fi
fi

echo "Target is '$TARGET'"
make clean
make distclean
unset CC
cp test/configs/${TARGET}.config .config
make olddefconfig

if [ ! -z ${GCC_SRC+x} ]; then
    if [ -z ${GCC_DIR+x} ]; then
        echo "Need a GCC_DIR together with GCC_SRC!"
        exit 1
    fi
    if [ -e "$GCC_DIR" ]; then
        echo "Reusing GCC in '$GCC_DIR'"
    else
        echo "Getting GCC from '$GCC_SRC'"
        wget "$GCC_SRC"
        echo "Unpacking GCC to '$GCC_DIR'"
        tar xf $(basename "$GCC_SRC")
    fi
    export PATH=$GCC_DIR/bin:$PATH
fi

make V=1
