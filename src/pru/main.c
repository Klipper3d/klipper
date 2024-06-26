// Main starting point for PRU code.
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include <pru/io.h> // read_r31
#include <pru_iep.h> // CT_IEP

#include <rsc_types.h> // resource_table
#include "board/misc.h" // dynmem_start
#include "board/io.h" // readl
#include "board/irq.h" // irq_disable
#include "command.h" // shutdown
#include "generic/timer_irq.h" // timer_dispatch_many
#include "internal.h" // SHARED_MEM
#include "sched.h" // sched_main

DECL_CONSTANT_STR("MCU", "pru");


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

void
irq_wait(void)
{
    asm("slp 1");
    irq_poll();
}

// Set the next timer wake up time
static void
timer_set(uint32_t value)
{
    if (!value)
        value = 1;
    CT_IEP.TMR_CMP0 = value;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return CT_IEP.TMR_CNT;
}

// Activate timer dispatch as soon as possible
void
timer_kick(void)
{
    timer_set(timer_read_time() + 50);
    CT_IEP.TMR_CMP_STS = 0xff;
    __delay_cycles(4);
    PRU_INTC.SECR0 = 1 << IEP_EVENT;
}

static uint32_t in_timer_dispatch;

static void
_irq_poll(void)
{
    uint32_t secr0 = PRU_INTC.SECR0;
    if (secr0 & (1 << KICK_PRU1_EVENT)) {
        PRU_INTC.SECR0 = 1 << KICK_PRU1_EVENT;
        sched_wake_tasks();
    }
    if (secr0 & (1 << IEP_EVENT)) {
        CT_IEP.TMR_CMP_STS = 0xff;
        in_timer_dispatch = 1;
        uint32_t next = timer_dispatch_many();
        timer_set(next);
        PRU_INTC.SECR0 = 1 << IEP_EVENT;
        in_timer_dispatch = 0;
    }
}
void __attribute__((optimize("O2")))
irq_poll(void)
{
    if (read_r31() & (1 << (WAKE_PRU1_IRQ + R31_IRQ_OFFSET)))
        _irq_poll();
}

void
timer_init(void)
{
    CT_IEP.TMR_CMP_CFG = 0x01 << 1;
    CT_IEP.TMR_GLB_CFG = 0x11;
    CT_IEP.TMR_CNT = 0xffffffff;
    timer_kick();
}
DECL_INIT(timer_init);


/****************************************************************
 * Console IO
 ****************************************************************/

// Writes over 496 bytes don't fit in a single "rpmsg" page
DECL_CONSTANT("RECEIVE_WINDOW", 496 - 1);

// Process any incoming commands
void
console_task(void)
{
    const struct command_parser *cp = SHARED_MEM->next_command;
    if (!cp)
        return;

    if (sched_is_shutdown() && !(cp->flags & HF_IN_SHUTDOWN)) {
        sched_report_shutdown();
    } else {
        void (*func)(uint32_t*) = cp->func;
        func(SHARED_MEM->next_command_args);
    }

    writel(&SHARED_MEM->next_command, 0);
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    SHARED_MEM->next_encoder_args = args;
    writel(&SHARED_MEM->next_encoder, (uint32_t)ce);

    // Signal PRU0 to transmit message - 20 | (18-16)  = 22 = 0010 0010
    write_r31(R31_WRITE_IRQ_SELECT | (KICK_PRU0_EVENT - R31_WRITE_IRQ_OFFSET));
    uint32_t itd = in_timer_dispatch;
    while (readl(&SHARED_MEM->next_encoder))
        if (!itd)
            irq_poll();
}

void
console_shutdown(void)
{
    writel(&SHARED_MEM->next_command, 0);
    writel(&SHARED_MEM->next_encoder, 0);
    in_timer_dispatch = 0;
}
DECL_SHUTDOWN(console_shutdown);

// Handle shutdown request from PRU0
static void
shutdown_handler(uint32_t *args)
{
    shutdown("Request from PRU0");
}
const struct command_parser shutdown_request = {
    .func = shutdown_handler,
};


/****************************************************************
 * Dynamic memory
 ****************************************************************/

#define STACK_SIZE 256

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    extern char _heap_start;
    return &_heap_start;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    return (void*)(8*1024 - STACK_SIZE);
}

/****************************************************************
 * Startup
 ****************************************************************/

// Support config_reset
DECL_COMMAND_FLAGS(config_reset, HF_IN_SHUTDOWN, "config_reset");

// Main entry point
int
main(void)
{
    // Wait for PRU0 to initialize
    while (readl(&SHARED_MEM->signal) != SIGNAL_PRU0_WAITING)
        ;
    SHARED_MEM->command_index = command_index;
    SHARED_MEM->command_index_size = command_index_size;
    SHARED_MEM->shutdown_handler = &shutdown_request;
    writel(&SHARED_MEM->signal, SIGNAL_PRU1_READY);

    sched_main();
    return 0;
}
