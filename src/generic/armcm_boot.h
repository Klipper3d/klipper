#ifndef __GENERIC_ARMCM_BOOT_H
#define __GENERIC_ARMCM_BOOT_H

#include "ctr.h" // DECL_CTR_INT

void armcm_main(void);

// Declare an IRQ handler
#define DECL_ARMCM_IRQ(FUNC, NUM)                                       \
    DECL_CTR_INT("DECL_ARMCM_IRQ " __stringify(FUNC), 1, CTR_INT(NUM))

// Statically declare an IRQ handler and run-time enable it
#define armcm_enable_irq(FUNC, NUM, PRIORITY) do {      \
        DECL_ARMCM_IRQ(FUNC, (NUM));                    \
        NVIC_SetPriority((NUM), (PRIORITY));            \
        NVIC_EnableIRQ((NUM));                          \
    } while (0)

// Vectors created by scripts/buildcommands.py from DECL_ARMCM_IRQ commands
extern const void * const VectorTable[];

#endif // armcm_boot.h
