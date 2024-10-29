// Code for interacting with bootrom on rp235x chips
//
// Copyright (C) 2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdint.h> // uint32_t
#include "boot/picoboot_constants.h" // REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL
#include "hardware/address_mapped.h" // static_assert
#include "internal.h" // bootrom_read_unique_id
#include "pico/bootrom_constants.h" // RT_FLAG_FUNC_ARM_NONSEC

static void *
rom_func_lookup(uint32_t code)
{
    typedef void *(*rom_table_lookup_fn)(uint32_t code, uint32_t mask);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    rom_table_lookup_fn rom_table_lookup = (rom_table_lookup_fn)
        (uintptr_t)*(uint16_t*)(BOOTROM_TABLE_LOOKUP_OFFSET);
#pragma GCC diagnostic pop
    return rom_table_lookup(code, RT_FLAG_FUNC_ARM_SEC);
}


/****************************************************************
 * Reboot to USB rom bootloader
 ****************************************************************/

void
bootrom_reboot_usb_bootloader(void)
{
    typedef int (*rom_reboot_fn)(uint32_t flags, uint32_t delay_ms
                                 , uint32_t p0, uint32_t p1);
    rom_reboot_fn func = rom_func_lookup(ROM_FUNC_REBOOT);
    func(REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL | REBOOT2_FLAG_NO_RETURN_ON_SUCCESS
         , 10, 0, 0);
}


/****************************************************************
 * Unique id reading
 ****************************************************************/

#define PICO_UNIQUE_BOARD_ID_SIZE_BYTES 8

void
bootrom_read_unique_id(uint8_t *out, uint32_t maxlen)
{
    typedef int (*rom_get_sys_info_fn)(uint8_t *out_buffer
                                       , uint32_t out_buffer_word_size
                                       , uint32_t flags);
    rom_get_sys_info_fn func = rom_func_lookup(ROM_FUNC_GET_SYS_INFO);
    uint8_t data[9 * 4];
    func(data, 9, SYS_INFO_CHIP_INFO);
    int i;
    for (i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; i++)
        out[i] = data[PICO_UNIQUE_BOARD_ID_SIZE_BYTES - 1 + 2 * 4 - i];
}
