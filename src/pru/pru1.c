// Code to halt the unneeded PRU1
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <pru/io.h> // __halt
#include <rsc_types.h> // resource_table
#include "compiler.h" // __section

struct my_resource_table {
    struct resource_table base;

    uint32_t offset[1]; /* Should match 'num' in actual definition */
} resourceTable __section(".resource_table") = {
    {
        1,              /* Resource table version: only version 1 is
                         * supported by the current driver */
        0,              /* number of entries in the table */
        { 0, 0 },       /* reserved, must be zero */
    },
};

int
main(void)
{
    __halt();
}
