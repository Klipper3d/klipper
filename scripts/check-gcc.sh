#!/bin/sh
# This script checks for a broken Ubuntu 18.04 arm-none-eabi-gcc compile

f1="$1"
f2="$2"

# Get the cross toolchain readelf as an env var, if set by the makefile
READELF="${READELF:-readelf}"

s1=`${READELF} -A "$f1" | grep "Tag_ARM_ISA_use"`
s2=`${READELF} -A "$f2" | grep "Tag_ARM_ISA_use"`

if [ "$s1" != "$s2" ]; then
    echo ""
    echo "ERROR: The compiler failed to correctly compile Klipper"
    echo "It will be necessary to upgrade the compiler"
    echo "See: https://bugs.launchpad.net/ubuntu/+source/newlib/+bug/1767223"
    echo ""
    rm -f "$f1"
    exit 99
fi
