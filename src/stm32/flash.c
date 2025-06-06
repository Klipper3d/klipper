// Flash (IAP) functionality for STM32
//
// Copyright (C) 2021 Eric Callahan <arksine.code@gmail.com
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memset
#include "autoconf.h" // CONFIG_MACH_STM32F103
#include "board/io.h" // writew
#include "flash.h" // flash_write_block
#include "internal.h" // FLASH

// Some chips have slightly different register names
#if CONFIG_MACH_STM32G0
#define FLASH_SR_BSY (FLASH_SR_BSY1 | FLASH_SR_BSY2)
#elif CONFIG_MACH_STM32H7
#define CR CR1
#define SR SR1
#define KEYR KEYR1
#endif

// Wait for flash hardware to report ready
static void
wait_flash(void)
{
    while (FLASH->SR & FLASH_SR_BSY)
        ;
}

#ifndef FLASH_KEY1 // Some stm32 headers don't define this
#define FLASH_KEY1 (0x45670123UL)
#define FLASH_KEY2 (0xCDEF89ABUL)
#endif

// Issue low-level flash hardware unlock sequence
static void
unlock_flash(void)
{
    if (FLASH->CR & FLASH_CR_LOCK) {
        // Unlock Flash Erase
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
    wait_flash();
}

// Place low-level flash hardware into a locked state
static void
lock_flash(void)
{
    FLASH->CR = FLASH_CR_LOCK;
}

// Issue a low-level flash hardware erase request for a flash page
static void
erase_page(uint32_t page_address)
{
#if CONFIG_MACH_STM32F2 || CONFIG_MACH_STM32F4
    uint32_t sidx;
    if (page_address < 0x08010000)
        sidx = (page_address - 0x08000000) / (16 * 1024);
    else if (page_address < 0x08020000)
        sidx = 4;
    else
        sidx = 5 + (page_address - 0x08020000) / (128 * 1024);
    sidx = sidx > 0x0f ? 0x0f : sidx;
    FLASH->CR = (FLASH_CR_PSIZE_1 | FLASH_CR_STRT | FLASH_CR_SER
                 | (sidx << FLASH_CR_SNB_Pos));
#elif CONFIG_MACH_STM32F0 || CONFIG_MACH_STM32F1
    FLASH->CR = FLASH_CR_PER;
    FLASH->AR = page_address;
    FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT;
#elif CONFIG_MACH_STM32G0 || CONFIG_MACH_STM32G4
    uint32_t pidx = (page_address - 0x08000000) / (2 * 1024);
    if (pidx >= 64) {
        uint16_t *flash_size = (void*)FLASHSIZE_BASE;
        if (*flash_size <= 256)
            pidx = pidx + 256 - 64;
        else
            pidx = pidx < 128 ? pidx : pidx + 256 - 128;
    }
    pidx = pidx > 0x3ff ? 0x3ff : pidx;
    FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT | (pidx << FLASH_CR_PNB_Pos);
#elif CONFIG_MACH_STM32H7
    uint32_t snb = (page_address - 0x08000000) / (128 * 1024);
    snb = snb > 7 ? 7 : snb;
    FLASH->CR = FLASH_CR_SER | FLASH_CR_START | (snb << FLASH_CR_SNB_Pos);
    while (FLASH->SR & FLASH_SR_QW)
        ;
    SCB_InvalidateDCache_by_Addr((void*)page_address, 128*1024);
#endif
    wait_flash();
}

void
flash_erase_page(uint32_t page_address)
{
    unlock_flash();
    erase_page(page_address);
    lock_flash();
}
