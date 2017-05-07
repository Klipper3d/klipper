// Dummy implementation for alloc commands
//
// Copyright (C) 2016,2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h> // malloc
#include <string.h> // memset
#include "command.h" // shutdown
#include "misc.h" // alloc_chunk
#include "sched.h" // sched_shutdown

// Return the maximum allocation size that can succeed up to 'reqsize'
size_t
alloc_chunk_maxsize(size_t reqsize)
{
    return reqsize;
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
    *avail = count;
    return alloc_chunk(size * count);
}
