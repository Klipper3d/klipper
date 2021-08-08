#!/usr/bin/python3

import os

# Enable high power
os.system("gpioset 1 194=0")

# reset STM32
os.system("gpioset 1 196=0")
os.system("gpioget 1 196")

# reset AR100
os.system("/home/debian/klipper/scripts/flash-ar100.py /home/debian/klipper/out/ar100.bin")

os.system("gpioset 1 102=1")
os.system("gpioset 1 120=1")
os.system("gpioset 1 160=1")
os.system("gpioset 1 161=1")

os.system("gpioget 1 100")
os.system("gpioget 1 235")
os.system("gpioget 1 145")
os.system("gpioget 1 34")
