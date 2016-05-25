// AVR allocation checking code.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/io.h> // AVR_STACK_POINTER_REG
#include <stdlib.h> // __malloc_heap_end
#include "autoconf.h" // CONFIG_AVR_STACK_SIZE
#include "compiler.h" // ALIGN
#include "misc.h" // alloc_maxsize

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
