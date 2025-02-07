/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOOT_PICOBOOT_CONSTANTS_H
#define _BOOT_PICOBOOT_CONSTANTS_H

#define REBOOT2_TYPE_MASK  0x0f

// note these match REBOOT_TYPE in pico/bootrom_constants.h (also 0 is used for PC_SP for backwards compatibility with RP2040)
// values 0-7 are secure/non-secure
#define REBOOT2_FLAG_REBOOT_TYPE_NORMAL       0x0 // param0 = diagnostic partition
#define REBOOT2_FLAG_REBOOT_TYPE_BOOTSEL      0x2 // param0 = bootsel_flags, param1 = gpio_config
#define REBOOT2_FLAG_REBOOT_TYPE_RAM_IMAGE    0x3 // param0 = image_base, param1 = image_end
#define REBOOT2_FLAG_REBOOT_TYPE_FLASH_UPDATE 0x4 // param0 = update_base

// values 8-15 are secure only
#define REBOOT2_FLAG_REBOOT_TYPE_PC_SP        0xd

#define REBOOT2_FLAG_REBOOT_TO_ARM            0x10
#define REBOOT2_FLAG_REBOOT_TO_RISCV          0x20

#define REBOOT2_FLAG_NO_RETURN_ON_SUCCESS    0x100

#define BOOTSEL_FLAG_DISABLE_MSD_INTERFACE      0x01
#define BOOTSEL_FLAG_DISABLE_PICOBOOT_INTERFACE 0x02
#define BOOTSEL_FLAG_GPIO_PIN_ACTIVE_LOW        0x10
#define BOOTSEL_FLAG_GPIO_PIN_SPECIFIED         0x20

#define PICOBOOT_GET_INFO_SYS              1
#define PICOBOOT_GET_INFO_PARTTION_TABLE   2
#define PICOBOOT_GET_INFO_UF2_TARGET_PARTITION 3
#define PICOBOOT_GET_INFO_UF2_STATUS       4

#define UF2_STATUS_IGNORED_FAMILY             0x01
#define UF2_STATUS_ABORT_EXCLUSIVELY_LOCKED   0x10
#define UF2_STATUS_ABORT_BAD_ADDRESS          0x20
#define UF2_STATUS_ABORT_WRITE_ERROR          0x40
#define UF2_STATUS_ABORT_REBOOT_FAILED        0x80
#endif