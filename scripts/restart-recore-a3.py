#!/usr/bin/python3

import os
import time

def compute_crc8_atm(datagram, initial_value=0):
    crc = initial_value
    # Iterate bytes in data
    for byte in datagram:
        # Iterate bits in byte
        for _ in range(0, 8):
            if (crc >> 7) ^ (byte & 0x01):
                crc = ((crc << 1) ^ 0x07) & 0xFF
            else:
                crc = (crc << 1) & 0xFF
            # Shift to next bit
            byte = byte >> 1
    return crc


def write_bytes(addr, reg, data, tty):
    pad = 0x05
    reg |= 0x80
    to_send = [pad, addr, reg]+data
    crc = compute_crc8_atm(to_send)
    to_send += [crc]
    with open(tty, "wb") as f:
        f.write(bytes(to_send))

# reset STM32
os.system("gpioset 1 196=0")
os.system("gpioset 1 196=1")

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


# Enable high power
os.system("gpioset 1 194=1")
time.sleep(0.1)
os.system("gpioset 1 194=0")


# Prep the tty for stepper drivers
os.system("stty -F /dev/ttyS2 raw -echo -echoe")

# Write Rsense int
write_bytes(0, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS2")
write_bytes(1, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS2")
write_bytes(2, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS2")
write_bytes(3, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS2")

# Write Ihold
write_bytes(0, 0x10, [0x00, 0x00, 0x07, 0x85], "/dev/ttyS2")
write_bytes(1, 0x10, [0x00, 0x00, 0x07, 0x85], "/dev/ttyS2")
write_bytes(2, 0x10, [0x00, 0x00, 0x07, 0x85], "/dev/ttyS2")
write_bytes(3, 0x10, [0x00, 0x00, 0x07, 0x85], "/dev/ttyS2")

# Chopconf
write_bytes(0, 0x6C, [0x15, 0x00, 0x00, 0x53], "/dev/ttyS2")
write_bytes(1, 0x6C, [0x15, 0x00, 0x00, 0x53], "/dev/ttyS2")
write_bytes(2, 0x6C, [0x15, 0x00, 0x00, 0x53], "/dev/ttyS2")
write_bytes(3, 0x6C, [0x15, 0x00, 0x00, 0x53], "/dev/ttyS2")

print("Stepper driver states: ")
os.system("gpioget 1 128")
os.system("gpioget 1 129")
os.system("gpioget 1 130")
os.system("gpioget 1 131")
os.system("gpioget 1 132")
os.system("gpioget 1 133")



# restart klipper
os.system("systemctl restart klipper")

