// Main starting point for PRU code.
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <string.h> // memset
#include <pru/io.h> // read_r31
#include <pru_iep.h> // CT_IEP
#include <pru_intc.h> // CT_INTC
#include <rsc_types.h> // resource_table
#include "board/misc.h" // alloc_chunk
#include "board/io.h" // readl
#include "board/irq.h" // irq_disable
#include "command.h" // shutdown
#include "generic/timer_irq.h" // timer_dispatch_many
#include "internal.h" // SHARED_MEM
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
}

uint32_t
timer_read_time(void)
{
    return CT_IEP.TMR_CNT;
}

static void
_irq_poll(void)
{
    if (CT_INTC.SECR0 & (1 << IEP_EVENT)) {
        CT_IEP.TMR_CMP_STS = 0xff;
        uint32_t next = timer_dispatch_many();
        timer_set(next);
    }
    CT_INTC.SECR0 = (1 << IEP_EVENT) | (1 << KICK_PRU1_EVENT);
}
void
irq_poll(void)
{
    if (read_r31() & (1 << (WAKE_PRU1_IRQ + R31_IRQ_OFFSET)))
        _irq_poll();
}

void
timer_shutdown(void)
{
    // Reenable timer irq
    timer_set(timer_read_time() + 50);
    CT_IEP.TMR_CMP_STS = 0xff;
    __delay_cycles(4);
    CT_INTC.SECR0 = 1 << IEP_EVENT;
}
DECL_SHUTDOWN(timer_shutdown);

void
timer_init(void)
{
    CT_IEP.TMR_CNT = 0;
    timer_shutdown();
}
DECL_INIT(timer_init);


/****************************************************************
 * Console IO
 ****************************************************************/

// Return a buffer (and length) containing any incoming messages
char *
console_get_input(uint8_t *plen)
{
    uint32_t read_count = readl(&SHARED_MEM->read_count);
    if (read_count > 64)
        read_count = 64;
    *plen = read_count;
    return SHARED_MEM->read_data;
}

// Remove from the receive buffer the given number of bytes
void
console_pop_input(uint8_t len)
{
    writel(&SHARED_MEM->read_count, 0);
}

// Return an output buffer that the caller may fill with transmit messages
char *
console_get_output(uint8_t len)
{
    if (len > sizeof(SHARED_MEM->send_data[0].data))
        return NULL;
    uint32_t send_push_pos = SHARED_MEM->send_push_pos;
    if (readl(&SHARED_MEM->send_data[send_push_pos].count))
        // Queue full
        return NULL;
    return SHARED_MEM->send_data[send_push_pos].data;
}

// Accept the given number of bytes added to the transmit buffer
void
console_push_output(uint8_t len)
{
    uint32_t send_push_pos = SHARED_MEM->send_push_pos;
    writel(&SHARED_MEM->send_data[send_push_pos].count, len);
    write_r31(R31_WRITE_IRQ_SELECT | (KICK_PRU0_EVENT - R31_WRITE_IRQ_OFFSET));
    SHARED_MEM->send_push_pos = (
        (send_push_pos + 1) % ARRAY_SIZE(SHARED_MEM->send_data));
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
 * Resource table
 ****************************************************************/

struct my_resource_table {
    struct resource_table base;

    uint32_t offset[1]; /* Should match 'num' in actual definition */
} resourceTable __visible __section(".resource_table") = {
    {
        1,              /* Resource table version: only version 1 is
                         * supported by the current driver */
        0,              /* number of entries in the table */
        { 0, 0 },       /* reserved, must be zero */
    },
};


/****************************************************************
 * Startup
 ****************************************************************/

// Main entry point
int
main(void)
{
    // Wait for PRU0 to initialize
    while (readl(&SHARED_MEM->signal) != SIGNAL_PRU0_WAITING)
        ;
    writel(&SHARED_MEM->signal, SIGNAL_PRU1_READY);

    sched_main();
    return 0;
}
