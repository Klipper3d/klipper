#!/usr/bin/env python3
# Encodes STM32 firmwares to be flashable from SD card by Chitu motherboards.
# Relocate firmware to 0x08008800!

# Copied from Marlin and modified.
# Licensed under GPL-3.0

import os
import struct
import uuid
import sys
import hashlib

def calculate_crc(contents, seed):
    accumulating_xor_value = seed;

    for i in range(0, len(contents), 4):
        value = struct.unpack('<I', contents[ i : i + 4])[0]
        accumulating_xor_value = accumulating_xor_value ^ value
    return accumulating_xor_value

def xor_block(r0, r1, block_number, block_size, file_key):
    # This is the loop counter
    loop_counter = 0x0

    # This is the key length
    key_length = 0x18

    # This is an initial seed
    xor_seed = 0x4bad

    # This is the block counter
    block_number = xor_seed * block_number

    #load the xor key from the file
    r7 =  file_key

    for loop_counter in range(0, block_size):
        # meant to make sure different bits of the key are used.
        xor_seed = int(loop_counter/key_length)

        # IP is a scratch register / R12
        ip = loop_counter - (key_length * xor_seed)

        # xor_seed = (loop_counter * loop_counter) + block_number
        xor_seed = (loop_counter * loop_counter) + block_number

        # shift the xor_seed left by the bits in IP.
        xor_seed = xor_seed >> ip

        # load a byte into IP
        ip = r0[loop_counter]

        # XOR the seed with r7
        xor_seed = xor_seed ^ r7

        # and then with IP
        xor_seed = xor_seed ^ ip

        #Now store the byte back
        r1[loop_counter] = xor_seed & 0xFF

        #increment the loop_counter
        loop_counter = loop_counter + 1


def encode_file(input, output_file, file_length):
    input_file = bytearray(input.read())
    block_size = 0x800
    key_length = 0x18

    file_digest = hashlib.md5(input_file).digest()
    uid_value = uuid.UUID(bytes=file_digest)
    print("Update UUID ", uid_value)
    file_key = int(uid_value.hex[0:8], 16)

    xor_crc = 0xef3d4323;

    # the input file is exepcted to be in chunks of 0x800
    # so round the size
    while len(input_file) % block_size != 0:
        input_file.extend(b'0x0')

    # write the file header
    output_file.write(struct.pack(">I", 0x443D2D3F))
    # encode the contents using a known file header key

    # write the file_key
    output_file.write(struct.pack("<I", file_key))

    #TODO - how to enforce that the firmware aligns to block boundaries?
    block_count = int(len(input_file) / block_size)
    print("Block Count is ", block_count)
    for block_number in range(0, block_count):
        block_offset = (block_number * block_size)
        block_end = block_offset + block_size
        block_array = bytearray(input_file[block_offset: block_end])
        xor_block(block_array, block_array, block_number, block_size, file_key)
        for n in range (0, block_size):
            input_file[block_offset + n] = block_array[n]

        # update the expected CRC value.
        xor_crc = calculate_crc(block_array, xor_crc)

    # write CRC
    output_file.write(struct.pack("<I", xor_crc))

    # finally, append the encoded results.
    output_file.write(input_file)
    return

def main():
    if len(sys.argv) != 3:
        print("Usage: update_chitu <input_file> <output_file>")
        exit(1)

    fw, output = sys.argv[1:]

    if not os.path.isfile(fw):
        print("Usage: update_chitu <input_file> <output_file>")
        print("Firmware file", fw, "does not exist")
        exit(1)

    firmware = open(fw, "rb")
    update = open(output, "wb")
    length = os.path.getsize(fw)

    encode_file(firmware, update, length)

    firmware.close()
    update.close()

    print("Encoding complete.")

if __name__ == '__main__':
    main()
