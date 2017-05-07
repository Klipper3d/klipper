// Main starting point for PRU code.
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <string.h> // memset
#include <pru/io.h> // read_r31
#include <pru_cfg.h> // CT_CFG
#include <pru_iep.h> // CT_IEP
#include <pru_intc.h> // CT_INTC
#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/misc.h" // timer_from_us
#include "board/irq.h" // irq_disable
#include "command.h" // shutdown
#include "generic/timer_irq.h" // timer_dispatch_many
#include "internal.h" // IEP_IRQ
#include "sched.h" // sched_main

DECL_CONSTANT(MCU, "pru");


/****************************************************************
 * Timers
 ****************************************************************/

void
irq_disable(void)
{
}

void
irq_enable(void)
{
}

irqstatus_t
irq_save(void)
{
    return 0;
}

void
irq_restore(irqstatus_t flag)
{
}

static void
timer_set(uint32_t value)
{
    CT_IEP.TMR_CMP0 = value;
    CT_INTC.SECR0 = 1 << IEP_EVENT;
}

uint32_t
timer_read_time(void)
{
    return CT_IEP.TMR_CNT;
}

static void
_irq_poll(void)
{
    CT_IEP.TMR_CMP_STS = 0xff;
    uint32_t next = timer_dispatch_many();
    timer_set(next);
}
void
irq_poll(void)
{
    if (read_r31() & (1 << (IEP_IRQ + R31_IRQ_OFFSET)))
        _irq_poll();
}

static void
timer_shutdown(void)
{
    // Reenable timer irq
    timer_set(timer_read_time() + 50);
    CT_IEP.TMR_CMP_STS = 0xff;
    __delay_cycles(4);
    CT_INTC.SECR0 = 1 << IEP_EVENT;
}
DECL_SHUTDOWN(timer_shutdown);

static void
timer_init(void)
{
    timer_set(0);
    CT_IEP.TMR_CMP_CFG = 0x01 << 1;
    CT_IEP.TMR_GLB_CFG = 0x11;
    timer_shutdown();
}


/****************************************************************
 * Allocator
 ****************************************************************/

extern char _heap_start;
static void *heap_ptr = &_heap_start;

#define STACK_SIZE 256
#define END_MEM ((void*)(8*1024 - STACK_SIZE))

// Allocate an area of memory
void *
alloc_chunk(size_t size)
{
    if (heap_ptr + size > END_MEM)
        shutdown("alloc_chunk failed");
    void *data = heap_ptr;
    heap_ptr += size;
    memset(data, 0, size);
    return data;
}

// Allocate an array of chunks
void *
alloc_chunks(size_t size, size_t count, size_t *avail)
{
    size_t can_alloc = 0;
    void *p = heap_ptr;
    for (; can_alloc <= count && p + size <= END_MEM; can_alloc++, p += size)
        ;
    if (!can_alloc)
        shutdown("alloc_chunks failed");
    *avail = can_alloc;
    return alloc_chunk(size * can_alloc);
}


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point
int
main(void)
{
    // allow access to external memory
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    console_init();
    timer_init();

    sched_main();
    return 0;
}
