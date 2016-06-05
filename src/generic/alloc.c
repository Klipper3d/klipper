// Dummy implementation for alloc commands
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "misc.h"

// Return the maximum allocation size that can succeed up to 'reqsize'
size_t alloc_maxsize(size_t reqsize)
{
    return reqsize;
}
