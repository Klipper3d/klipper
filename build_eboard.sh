#!/bin/bash
# Activate the Klipper Mod Cross Toolchain - sourced by build scripts
cp -rf mcu_configs/eboard_config .config

make clean CROSS_PREFIX=arm-none-eabi-
make CROSS_PREFIX=arm-none-eabi-

cp -rf  out/klipper.hex mcu_configs/eboard.hex
cp -rf  out/klipper.bin mcu_configs/eboard.bin
