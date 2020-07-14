#!/usr/bin/python3

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

# Write Rsense int
write_bytes(0, 0x00, [0x00, 0x00, 0x01, 0x82], "/dev/ttyS2")

# Write Ihold
write_bytes(0, 0x10, [0x00, 0x00, 0x07, 0x85], "/dev/ttyS2")

# Chopconf
write_bytes(0, 0x6C, [0x15, 0x00, 0x00, 0x53], "/dev/ttyS2")

