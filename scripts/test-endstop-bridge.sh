#!/bin/sh
# Offline-only host, actual firmware C, and dual-MCU Klippy tests.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_ROOT=${BUILD_ROOT:-/tmp/klipper-endstop-bridge}
JOBS=${JOBS:-2}
mkdir -p "$BUILD_ROOT/avr" "$BUILD_ROOT/tests"
cd "$ROOT"
python3 -m unittest discover -s test -p test_endstop_bridge.py -v
gcc -std=gnu11 -g -no-pie -fsanitize=address,undefined \
    -Itest/endstop_bridge test/endstop_bridge/harness.c \
    -o "$BUILD_ROOT/endstop-bridge-test"
"$BUILD_ROOT/endstop-bridge-test"
cp test/configs/atmega2560.config "$BUILD_ROOT/avr.config"
make KCONFIG_CONFIG="$BUILD_ROOT/avr.config" OUT="$BUILD_ROOT/avr/" olddefconfig
make KCONFIG_CONFIG="$BUILD_ROOT/avr.config" OUT="$BUILD_ROOT/avr/" -j"$JOBS"
cp "$BUILD_ROOT/avr/klipper.dict" "$BUILD_ROOT/tests/atmega2560.dict"
python3 scripts/test_klippy.py -d "$BUILD_ROOT/tests" -t "$BUILD_ROOT/tests" \
    test/klippy/endstop_bridge.test test/klippy/endstop_bridge_xyz.test \
    test/klippy/load_cell.test
