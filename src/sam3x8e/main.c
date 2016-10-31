// Main starting point for SAM3x8e boards.
//
// Copyright (C) 2016  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // console_get_input
#include "sam3x8e.h" // WDT
#include "sched.h" // sched_main


/****************************************************************
 * watchdog handler
 ****************************************************************/

static void
watchdog_reset(void)
{
    WDT->WDT_CR = 0xA5000001;
}
DECL_TASK(watchdog_reset);

static void
watchdog_init(void)
{
    uint32_t timeout = 32768 / 2;  // 500ms timeout
    WDT->WDT_MR = WDT_MR_WDRSTEN | WDT_MR_WDV(timeout) | WDT_MR_WDD(timeout);
}
DECL_INIT(watchdog_init);


/****************************************************************
 * irq clearing on fault
 ****************************************************************/

// Clear the active irq if a shutdown happened in an irq handler
static void
clear_active_irq(void)
{
    uint32_t psr;
    asm volatile("mrs %0, psr" : "=r" (psr));
    if (!(psr & 0x1ff))
        // Shutdown did not occur in an irq - nothing to do.
        return;
    // Clear active irq status
    psr = 1<<24; // T-bit
    uint32_t temp;
    asm volatile(
        "  push { %1 }\n"
        "  adr %0, 1f\n"
        "  push { %0 }\n"
        "  push { r0, r1, r2, r3, r12, lr }\n"
        "  bx %2\n"
        "1:\n"
        : "=&r"(temp) : "r"(psr), "r"(0xfffffff9) : "cc");
}
DECL_SHUTDOWN(clear_active_irq);


/****************************************************************
 * misc functions
 ****************************************************************/

size_t
alloc_maxsize(size_t reqsize)
{
    return reqsize;
}

void * __visible
_sbrk(int nbytes)
{
    extern char _end;
    static void *heap_ptr = (void *)&_end;
    void *pos = heap_ptr;
    heap_ptr = pos + nbytes;
    return pos;
}

// Main entry point
int
main(void)
{
    SystemInit();
    sched_main();
    return 0;
}
