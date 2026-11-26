// AVR support for bootloader entry
//
// Copyright (C) 2026  Hans-Albert Maritz <maritz.hans@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>

// Caterina and compatible LUFA CDC bootloaders (Arduino Leonardo, Pro Micro,
// Prusa MMU3, etc.) check for a magic word at RAMEND-1 after a watchdog reset.
// If present the bootloader holds for ~8s to allow programming; if absent it
// jumps to the application immediately.  A watchdog reset does not clear SRAM,
// so the word written here survives to be read by the bootloader.
#define MAGIC_BOOT_KEY 0x7777u

// Handle reboot requests
void
bootloader_request(void)
{
    *(volatile uint16_t *)(RAMEND - 1) = MAGIC_BOOT_KEY;
    wdt_enable(WDTO_15MS);
    for (;;)
        ;
}
