#!/usr/bin/python3

import os

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



# Enable high power
os.system("gpioset 1 194=0")

# reset STM32
os.system("gpioset 1 196=0")
os.system("gpioset 1 196=1")

# reset AR100
os.system("/home/klipper/klipper/scripts/flash-ar100.py /home/klipper/klipper/out/ar100.bin")

os.system("gpioset 1 102=1")
os.system("gpioset 1 120=1")
os.system("gpioset 1 160=1")
os.system("gpioset 1 161=1")

os.system("gpioget 1 100")
os.system("gpioget 1 235")
os.system("gpioget 1 145")
os.system("gpioget 1 34")

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



# restart klipper
os.system("systemctl restart klipper")

