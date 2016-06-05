// AVR miscellaneous platform code
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/io.h> // AVR_STACK_POINTER_REG
#include <stdlib.h> // __malloc_heap_end
#include <util/crc16.h> // _crc_ccitt_update
#include "autoconf.h" // CONFIG_AVR_STACK_SIZE
#include "board/misc.h" // alloc_maxsize
#include "compiler.h" // ALIGN

// Optimized crc16_ccitt for the avr processor
uint16_t
crc16_ccitt(char *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    while (len--)
        crc = _crc_ccitt_update(crc, *buf++);
    return crc;
}

// Return the maximum allocation size that can succeed up to 'reqsize'
size_t
alloc_maxsize(size_t reqsize)
{
    uint16_t memend = ALIGN(AVR_STACK_POINTER_REG, 256);
    __malloc_heap_end = (void*)memend - CONFIG_AVR_STACK_SIZE;
    extern char *__brkval;
    int16_t maxsize = __malloc_heap_end - __brkval - 2;
    if (maxsize < 0)
        return 0;
    if (reqsize < maxsize)
        return reqsize;
    return maxsize;
}
