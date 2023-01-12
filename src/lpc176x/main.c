// Main starting point for LPC176x boards.
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_FREQ
#include "board/armcm_boot.h" // armcm_main
#include "board/armcm_reset.h" // try_request_canboot
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "internal.h" // enable_pclock
#include "sched.h" // sched_main


/****************************************************************
 * watchdog handler
 ****************************************************************/

void
watchdog_reset(void)
{
    LPC_WDT->WDFEED = 0xaa;
    LPC_WDT->WDFEED = 0x55;
}
DECL_TASK(watchdog_reset);

void
watchdog_init(void)
{
    LPC_WDT->WDTC = 4000000 / 2; // 500ms timeout
    LPC_WDT->WDCLKSEL = 1<<31; // Lock to internal RC
    LPC_WDT->WDMOD = 0x03; // select reset and enable
    watchdog_reset();
}
DECL_INIT(watchdog_init);


/****************************************************************
 * misc functions
 ****************************************************************/

// Try to reboot into bootloader
void
bootloader_request(void)
{
    if (!CONFIG_FLASH_APPLICATION_ADDRESS)
        return;
    try_request_canboot();
    // Disable USB and pause for 5ms so host recognizes a disconnect
    irq_disable();
    if (CONFIG_USB)
        usb_disconnect();
    // The "LPC17xx-DFU-Bootloader" will enter the bootloader if the
    // watchdog timeout flag is set.
    LPC_WDT->WDMOD = 0x07;
    NVIC_SystemReset();
}

// Check if a peripheral clock has been enabled
int
is_enabled_pclock(uint32_t pclk)
{
    return !!(LPC_SC->PCONP & (1<<pclk));
}

// Enable a peripheral clock
void
enable_pclock(uint32_t pclk)
{
    LPC_SC->PCONP |= 1<<pclk;
}

// Return the frequency of the given peripheral clock
uint32_t
get_pclock_frequency(uint32_t pclk)
{
    return CONFIG_CLOCK_FREQ;
}

// Main entry point - called from armcm_boot.c:ResetHandler()
void
armcm_main(void)
{
    SystemInit();
    sched_main();
}
