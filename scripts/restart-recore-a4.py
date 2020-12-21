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
os.system("/home/debian/klipper/scripts/flash-ar100.py /home/debian/klipper/out/ar100.bin")

os.system("gpioset 1 102=1")
os.system("gpioset 1 120=1")
os.system("gpioset 1 160=1")
os.system("gpioset 1 161=1")

os.system("gpioget 1 100")
os.system("gpioget 1 235")
os.system("gpioget 1 145")
os.system("gpioget 1 34")



def prepare_motors():
    print("prepare motors")
    # Prep the tty for stepper drivers
    os.system("stty -F /dev/ttyS2 raw -echo -echoe")
    os.system("stty -F /dev/ttyS3 raw -echo -echoe")

    for addr in [0, 1, 2, 3]:
        # GCONF: Write Rsense int
        write_bytes(addr, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS2")
        write_bytes(addr, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS3")
        # Write Ihold
        ihold = (5<<0) # 0..31
        irun  = (10<<5) # 0..31
        write_bytes(addr, 0x10, [0x00, 0x00, (irun>>8), (irun | ihold) & 0xFF], "/dev/ttyS2")
        write_bytes(addr, 0x10, [0x00, 0x00, (irun>>8), (irun | ihold) & 0xFF], "/dev/ttyS3")
        # Chopconf
        write_bytes(addr, 0x6C, [0x43, 0x00, 0x00, 0x53], "/dev/ttyS2")
        write_bytes(addr, 0x6C, [0x43, 0x00, 0x00, 0x53], "/dev/ttyS3")

prepare_motors()
