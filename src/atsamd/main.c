// Main starting point for SAMD boards
//
// Copyright (C) 2018-2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/armcm_boot.h" // armcm_main
#include "board/misc.h" // bootloader_request
#include "board/io.h" // writel
#include "board/irq.h" // irq_disable
#include "internal.h" // SystemInit
#include "sched.h" // sched_main

void
bootloader_request(void)
{
    if (!CONFIG_FLASH_START)
        return;
    // Bootloader hack
    irq_disable();
#if CONFIG_MACH_SAMD21
    writel((void*)0x20007FFC, 0x07738135);
#elif CONFIG_MACH_SAMX5
    writel((void*)(HSRAM_ADDR + HSRAM_SIZE - 4), 0xf01669ef);
#endif
    NVIC_SystemReset();
}

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    SystemInit();
    sched_main();
}
