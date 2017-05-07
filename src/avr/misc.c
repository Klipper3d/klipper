// AVR miscellaneous platform code
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/io.h> // AVR_STACK_POINTER_REG
#include <stdlib.h> // __malloc_heap_end
#include <string.h> // memset
#include <util/crc16.h> // _crc_ccitt_update
#include "autoconf.h" // CONFIG_AVR_STACK_SIZE
#include "board/misc.h" // alloc_maxsize
#include "command.h" // shutdown
#include "compiler.h" // ALIGN
#include "sched.h" // sched_shutdown

// Optimized crc16_ccitt for the avr processor
uint16_t
crc16_ccitt(char *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;
    while (len--)
        crc = _crc_ccitt_update(crc, *buf++);
    return crc;
}

// Allocate an area of memory
void *
alloc_chunk(size_t size)
{
    void *data = malloc(size);
    if (!data)
        shutdown("alloc_chunk failed");
    memset(data, 0, size);
    return data;
}

// Allocate an array of chunks
void *
alloc_chunks(size_t size, size_t count, size_t *avail)
{
    uint16_t memend = ALIGN(AVR_STACK_POINTER_REG, 256);
    __malloc_heap_end = (void*)memend - CONFIG_AVR_STACK_SIZE;
    extern char *__brkval;
    uint16_t maxsize = __malloc_heap_end - __brkval - 2;
    if ((int16_t)maxsize < 0)
        maxsize = 0;
    if (count * size > maxsize)
        count = maxsize / size;
    if (!count)
        shutdown("alloc_chunks failed");
    *avail = count;
    return alloc_chunk(count * size);
}
