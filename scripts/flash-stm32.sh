#!/bin/bash
gpioset 1 197=1
gpioset 1 196=0
stm32flash -i -196,196 -w out/klipper.bin -v -g 0x00 /dev/ttyS3
gpioset 1 197=0
stm32flash -i -196,196 /dev/ttyS3