// Code to enable clock lines on stm32
//
// Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/irq.h" // irq_save
#include "internal.h" // struct cline

// Enable a peripheral clock
void
enable_pclock(uint32_t periph_base)
{
    struct cline cl = lookup_clock_line(periph_base);
    irqstatus_t flag = irq_save();
    *cl.en |= cl.bit;
    *cl.en; // Pause 2 cycles to ensure peripheral is enabled
    if (cl.rst) {
        // Reset peripheral
        *cl.rst = cl.bit;
        *cl.rst = 0;
    }
    irq_restore(flag);
}

// Check if a peripheral clock has been enabled
int
is_enabled_pclock(uint32_t periph_base)
{
    struct cline cl = lookup_clock_line(periph_base);
    return *cl.en & cl.bit;
}
