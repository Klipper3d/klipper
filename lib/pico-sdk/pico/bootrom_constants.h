/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICO_BOOTROM_CONSTANTS_H
#define _PICO_BOOTROM_CONSTANTS_H

#ifndef NO_PICO_PLATFORM
#include "pico/platform.h"
#endif

// ROOT ADDRESSES
#define BOOTROM_MAGIC_OFFSET 0x10
#define BOOTROM_FUNC_TABLE_OFFSET 0x14
#if PICO_RP2040
#define BOOTROM_DATA_TABLE_OFFSET 0x16
#endif

#if PICO_RP2040
#define BOOTROM_VTABLE_OFFSET           0x00
#define BOOTROM_TABLE_LOOKUP_OFFSET     0x18
#else
// todo remove this (or #ifdef it for A1/A2)
#define BOOTROM_IS_A2() ((*(volatile uint8_t *)0x13) == 2)
#define BOOTROM_WELL_KNOWN_PTR_SIZE (BOOTROM_IS_A2() ? 2 : 4)
#if defined(__riscv)
#define BOOTROM_ENTRY_OFFSET            0x7dfc
#define BOOTROM_TABLE_LOOKUP_ENTRY_OFFSET (BOOTROM_ENTRY_OFFSET - BOOTROM_WELL_KNOWN_PTR_SIZE)
#define BOOTROM_TABLE_LOOKUP_OFFSET     (BOOTROM_ENTRY_OFFSET - BOOTROM_WELL_KNOWN_PTR_SIZE*2)
#else
#define BOOTROM_VTABLE_OFFSET 0x00
#define BOOTROM_TABLE_LOOKUP_OFFSET     (BOOTROM_FUNC_TABLE_OFFSET + BOOTROM_WELL_KNOWN_PTR_SIZE)
#endif
#endif

#if !PICO_RP2040 || PICO_COMBINED_DOCS

#define BOOTROM_OK 0
//#define BOOTROM_ERROR_TIMEOUT (-1)
//#define BOOTROM_ERROR_GENERIC (-2)
//#define BOOTROM_ERROR_NO_DATA (-3)                   // E.g. read from an empty buffer/FIFO
#define BOOTROM_ERROR_NOT_PERMITTED (-4)             // Permission violation e.g. write to read-only flash partition
#define BOOTROM_ERROR_INVALID_ARG (-5)               // Argument is outside of range of supported values`
//#define BOOTROM_ERROR_IO (-6)
//#define BOOTROM_ERROR_BADAUTH (-7)
//#define BOOTROM_ERROR_CONNECT_FAILED (-8)
//#define BOOTROM_ERROR_INSUFFICIENT_RESOURCES (-9)  // Dynamic allocation of resources failed
#define BOOTROM_ERROR_INVALID_ADDRESS (-10)          // Address argument was out-of-bounds or was determined to be an address that the caller may not access
#define BOOTROM_ERROR_BAD_ALIGNMENT (-11)            // Address modulo transfer chunk size was nonzero (e.g. word-aligned transfer with address % 4 != 0)
#define BOOTROM_ERROR_INVALID_STATE (-12)            // Something happened or failed to happen in the past, and consequently we (currently) can't service the request
#define BOOTROM_ERROR_BUFFER_TOO_SMALL (-13)         // A user-allocated buffer was too small to hold the result or working state of this function
#define BOOTROM_ERROR_PRECONDITION_NOT_MET (-14)     // This call failed because another ROM function must be called first
#define BOOTROM_ERROR_MODIFIED_DATA (-15)            // Cached data was determined to be inconsistent with the full version of the data it was calculated from
#define BOOTROM_ERROR_INVALID_DATA (-16)             // A data structure failed to validate
#define BOOTROM_ERROR_NOT_FOUND (-17)                // Attempted to access something that does not exist; or, a search failed
#define BOOTROM_ERROR_UNSUPPORTED_MODIFICATION (-18) // Write is impossible based on previous writes; e.g. attempted to clear an OTP bit
#define BOOTROM_ERROR_LOCK_REQUIRED (-19)            // A required lock is not owned
#define BOOTROM_ERROR_LAST (-19)

#define RT_FLAG_FUNC_RISCV      0x0001
#define RT_FLAG_FUNC_RISCV_FAR  0x0003
#define RT_FLAG_FUNC_ARM_SEC    0x0004
// reserved for 32-bit pointer: 0x0008
#define RT_FLAG_FUNC_ARM_NONSEC 0x0010
// reserved for 32-bit pointer: 0x0020
#define RT_FLAG_DATA            0x0040
// reserved for 32-bit pointer: 0x0080

#define PARTITION_TABLE_MAX_PARTITIONS    16
// note this is deliberately > MAX_PARTITIONs is likely to be, and also -1 as a signed byte
#define PARTITION_TABLE_NO_PARTITION_INDEX 0xff

// todo these are duplicated in picoboot_constants.h
// values 0-7 are secure/non-secure
#define BOOT_TYPE_NORMAL     0
#define BOOT_TYPE_BOOTSEL    2
#define BOOT_TYPE_RAM_IMAGE  3
#define BOOT_TYPE_FLASH_UPDATE 4

// values 8-15 are secure only
#define BOOT_TYPE_PC_SP      0xd

// ORed in if a bootloader chained into the image
#define BOOT_TYPE_CHAINED_FLAG 0x80

// call from NS to S
#ifndef __riscv
#define BOOTROM_API_CALLBACK_secure_call 0
#endif
#define BOOTROM_API_CALLBACK_COUNT 1

#define BOOTROM_LOCK_SHA_256   0
#define BOOTROM_LOCK_FLASH_OP  1
#define BOOTROM_LOCK_OTP       2
#define BOOTROM_LOCK_MAX       2

#define BOOTROM_LOCK_ENABLE    7

#define BOOT_PARTITION_NONE (-1)
#define BOOT_PARTITION_SLOT0 (-2)
#define BOOT_PARTITION_SLOT1 (-3)
#define BOOT_PARTITION_WINDOW (-4)

#define BOOT_DIAGNOSTIC_WINDOW_SEARCHED                           0x01
// note if both BOOT_DIAGNOSTIC_INVALID_BLOCK_LOOP and BOOT_DIAGNOSTIC_VALID_BLOCK_LOOP then the block loop was valid
// but it has a PARTITION_TABLE which while it passed the initial verification (and hash/sig) had invalid contents
// (discovered when it was later loaded)
#define BOOT_DIAGNOSTIC_INVALID_BLOCK_LOOP                        0x02
#define BOOT_DIAGNOSTIC_VALID_BLOCK_LOOP                          0x04
#define BOOT_DIAGNOSTIC_VALID_IMAGE_DEF                           0x08
#define BOOT_DIAGNOSTIC_HAS_PARTITION_TABLE                       0x10
#define BOOT_DIAGNOSTIC_CONSIDERED                                0x20
#define BOOT_DIAGNOSTIC_CHOSEN                                    0x40
#define BOOT_DIAGNOSTIC_PARTITION_TABLE_LSB 7
#define BOOT_DIAGNOSTIC_PARTITION_TABLE_MATCHING_KEY_FOR_VERIFY   0x80
#define BOOT_DIAGNOSTIC_PARTITION_TABLE_HASH_FOR_VERIFY          0x100
#define BOOT_DIAGNOSTIC_PARTITION_TABLE_VERIFIED_OK              0x200
#define BOOT_DIAGNOSTIC_IMAGE_DEF_LSB 10
#define BOOT_DIAGNOSTIC_IMAGE_DEF_MATCHING_KEY_FOR_VERIFY        0x400
#define BOOT_DIAGNOSTIC_IMAGE_DEF_HASH_FOR_VERIFY                0x800
#define BOOT_DIAGNOSTIC_IMAGE_DEF_VERIFIED_OK                   0x1000

#define BOOT_DIAGNOSTIC_LOAD_MAP_ENTRIES_LOADED                 0x2000
#define BOOT_DIAGNOSTIC_IMAGE_LAUNCHED                          0x4000
#define BOOT_DIAGNOSTIC_IMAGE_CONDITION_FAILURE                 0x8000

#define BOOT_PARSED_BLOCK_DIAGNOSTIC_MATCHING_KEY_FOR_VERIFY       0x1 // if this is present and VERIFIED_OK isn't the sig check failed
#define BOOT_PARSED_BLOCK_DIAGNOSTIC_HASH_FOR_VERIFY               0x2 // if this is present and VERIFIED_OL isn't then hash check failed
#define BOOT_PARSED_BLOCK_DIAGNOSTIC_VERIFIED_OK                   0x4

#define BOOT_TBYB_AND_UPDATE_FLAG_BUY_PENDING                      0x1
#define BOOT_TBYB_AND_UPDATE_FLAG_OTP_VERSION_APPLIED              0x2
#define BOOT_TBYB_AND_UPDATE_FLAG_OTHER_ERASED                     0x4

#ifndef __ASSEMBLER__
// Limited to 3 arguments in case of varm multiplex hint (trashes Arm r3)
typedef int (*bootrom_api_callback_generic_t)(uint32_t r0, uint32_t r1, uint32_t r2);
// Return negative for error, else number of bytes transferred:
//typedef int (*bootrom_api_callback_stdout_put_blocking_t)(const uint8_t *buffer, uint32_t size);
//typedef int (*bootrom_api_callback_stdin_get_t)(uint8_t *buffer, uint32_t size);
//typedef void (*bootrom_api_callback_core1_security_setup_t)(void);
#endif

#endif

/*! \brief Return a bootrom lookup code based on two ASCII characters
 * \ingroup pico_bootrom
 *
 * These codes are uses to lookup data or function addresses in the bootrom
 *
 * \param c1 the first character
 * \param c2 the second character
 * \return the 'code' to use in rom_func_lookup() or rom_data_lookup()
 */
#define ROM_TABLE_CODE(c1, c2) ((c1) | ((c2) << 8))

// ROM FUNCTIONS

// RP2040 & RP2350
#define ROM_DATA_SOFTWARE_GIT_REVISION          ROM_TABLE_CODE('G', 'R')
#define ROM_FUNC_FLASH_ENTER_CMD_XIP            ROM_TABLE_CODE('C', 'X')
#define ROM_FUNC_FLASH_EXIT_XIP                 ROM_TABLE_CODE('E', 'X')
#define ROM_FUNC_FLASH_FLUSH_CACHE              ROM_TABLE_CODE('F', 'C')
#define ROM_FUNC_CONNECT_INTERNAL_FLASH         ROM_TABLE_CODE('I', 'F')
#define ROM_FUNC_FLASH_RANGE_ERASE              ROM_TABLE_CODE('R', 'E')
#define ROM_FUNC_FLASH_RANGE_PROGRAM            ROM_TABLE_CODE('R', 'P')


#if PICO_RP2040
// RP2040 only
#define ROM_FUNC_MEMCPY44                       ROM_TABLE_CODE('C', '4')
#define ROM_DATA_COPYRIGHT                      ROM_TABLE_CODE('C', 'R')
#define ROM_FUNC_CLZ32                          ROM_TABLE_CODE('L', '3')
#define ROM_FUNC_MEMCPY                         ROM_TABLE_CODE('M', 'C')
#define ROM_FUNC_MEMSET                         ROM_TABLE_CODE('M', 'S')
#define ROM_FUNC_POPCOUNT32                     ROM_TABLE_CODE('P', '3')
#define ROM_FUNC_REVERSE32                      ROM_TABLE_CODE('R', '3')
#define ROM_FUNC_MEMSET4                        ROM_TABLE_CODE('S', '4')
#define ROM_FUNC_CTZ32                          ROM_TABLE_CODE('T', '3')
#define ROM_FUNC_RESET_USB_BOOT                 ROM_TABLE_CODE('U', 'B')
#endif

#if !PICO_RP2040 || PICO_COMBINED_DOCS
// RP2350 only
#define ROM_FUNC_PICK_AB_PARTITION              ROM_TABLE_CODE('A', 'B')
#define ROM_FUNC_CHAIN_IMAGE                    ROM_TABLE_CODE('C', 'I')
#define ROM_FUNC_EXPLICIT_BUY                   ROM_TABLE_CODE('E', 'B')
#define ROM_FUNC_FLASH_RUNTIME_TO_STORAGE_ADDR  ROM_TABLE_CODE('F', 'A')
#define ROM_DATA_FLASH_DEVINFO16_PTR            ROM_TABLE_CODE('F', 'D')
#define ROM_FUNC_FLASH_OP                       ROM_TABLE_CODE('F', 'O')
#define ROM_FUNC_GET_B_PARTITION                ROM_TABLE_CODE('G', 'B')
#define ROM_FUNC_GET_PARTITION_TABLE_INFO       ROM_TABLE_CODE('G', 'P')
#define ROM_FUNC_GET_SYS_INFO                   ROM_TABLE_CODE('G', 'S')
#define ROM_FUNC_GET_UF2_TARGET_PARTITION       ROM_TABLE_CODE('G', 'U')
#define ROM_FUNC_LOAD_PARTITION_TABLE           ROM_TABLE_CODE('L', 'P')
#define ROM_FUNC_OTP_ACCESS                     ROM_TABLE_CODE('O', 'A')
#define ROM_DATA_PARTITION_TABLE_PTR            ROM_TABLE_CODE('P', 'T')
#define ROM_FUNC_FLASH_RESET_ADDRESS_TRANS      ROM_TABLE_CODE('R', 'A')
#define ROM_FUNC_REBOOT                         ROM_TABLE_CODE('R', 'B')
#define ROM_FUNC_SET_ROM_CALLBACK               ROM_TABLE_CODE('R', 'C')
#define ROM_FUNC_SECURE_CALL                    ROM_TABLE_CODE('S', 'C')
#define ROM_FUNC_SET_NS_API_PERMISSION          ROM_TABLE_CODE('S', 'P')
#define ROM_FUNC_BOOTROM_STATE_RESET            ROM_TABLE_CODE('S', 'R')
#define ROM_FUNC_SET_BOOTROM_STACK              ROM_TABLE_CODE('S', 'S')
#define ROM_DATA_SAVED_XIP_SETUP_FUNC_PTR       ROM_TABLE_CODE('X', 'F')
#define ROM_FUNC_FLASH_SELECT_XIP_READ_MODE     ROM_TABLE_CODE('X', 'M')
#define ROM_FUNC_VALIDATE_NS_BUFFER             ROM_TABLE_CODE('V', 'B')
#endif

// these form a bit set
#define BOOTROM_STATE_RESET_CURRENT_CORE 0x01
#define BOOTROM_STATE_RESET_OTHER_CORE   0x02
#define BOOTROM_STATE_RESET_GLOBAL_STATE 0x04 // reset any global state (e.g. permissions)

// partition level stuff is returned first (note PT_INFO flags is only 16 bits)

// 3 words: pt_count, unpartitioned_perm_loc, unpartioned_perm_flags
#define PT_INFO_PT_INFO                         0x0001
#define PT_INFO_SINGLE_PARTITION                0x8000 // marker to just include a single partition in the results)

// then in order per partition selected

// 2 words: unpartitioned_perm_loc, unpartioned_perm_flags
#define PT_INFO_PARTITION_LOCATION_AND_FLAGS    0x0010
// 2 words: id lsb first
#define PT_INFO_PARTITION_ID                    0x0020
// n+1 words: n, family_id...
#define PT_INFO_PARTITION_FAMILY_IDS            0x0040
// (n+3)/4 words... bytes are: n (len), c0, c1, ... cn-1 padded to word boundary with zeroes
#define PT_INFO_PARTITION_NAME                  0x0080

// items are returned in order
// 3 words package_id, device_id, wafer_id
#define SYS_INFO_CHIP_INFO                      0x0001
// 1 word: chip specific critical bits
#define SYS_INFO_CRITICAL                       0x0002
// 1 word: bytes: cpu_type, supported_cpu_type_bitfield
#define SYS_INFO_CPU_INFO                       0x0004
// 1 word: same as FLASH_DEVINFO row in OTP
#define SYS_INFO_FLASH_DEV_INFO                 0x0008
// 4 words
#define SYS_INFO_BOOT_RANDOM                    0x0010
// 2 words lsb first
#define SYS_INFO_NONCE                          0x0020
// 4 words boot_info, boot_diagnostic, boot_param0, boot_param1
#define SYS_INFO_BOOT_INFO                      0x0040

#define BOOTROM_NS_API_get_sys_info 0
#define BOOTROM_NS_API_checked_flash_op 1
#define BOOTROM_NS_API_flash_runtime_to_storage_addr 2
#define BOOTROM_NS_API_get_partition_table_info 3
#define BOOTROM_NS_API_secure_call 4
#define BOOTROM_NS_API_otp_access 5
#define BOOTROM_NS_API_reboot 6
#define BOOTROM_NS_API_get_b_partition 7
#define BOOTROM_NS_API_COUNT 8

#ifndef __ASSEMBLER__

typedef struct {
    uint32_t permissions_and_location;
    uint32_t permissions_and_flags;
} resident_partition_t;
static_assert(sizeof(resident_partition_t) == 8, "");

#define OTP_CMD_ROW_BITS                    0x0000ffffu
#define OTP_CMD_ROW_LSB                     0u
#define OTP_CMD_WRITE_BITS                  0x00010000u
#define OTP_CMD_ECC_BITS                    0x00020000u

typedef struct otp_cmd {
    uint32_t flags;
} otp_cmd_t;

typedef enum {
    BOOTROM_XIP_MODE_03H_SERIAL = 0,
    BOOTROM_XIP_MODE_0BH_SERIAL,
    BOOTROM_XIP_MODE_BBH_DUAL,
    BOOTROM_XIP_MODE_EBH_QUAD,
    BOOTROM_XIP_MODE_N_MODES
} bootrom_xip_mode_t;

// The checked flash API wraps the low-level flash routines from generic_flash, adding bounds
// checking, permission checking against the resident partition table, and simple address
// translation. The low-level API deals with flash offsets (i.e. distance from the start of the
// first flash device, measured in bytes) but the checked flash API accepts one of two types of
// address:
//
// - Flash runtime addresses: the address of some flash-resident data or code in the currently
//   running image. The flash addresses your binary is "linked at" by the linker.
// - Flash storage addresses: a flash offset, plus the address base where QSPI hardware is first
//   mapped on the system bus (XIP_BASE constant from addressmap.h)
//
// These addresses are one and the same *if* the currently running program is stored at the
// beginning of flash. They are different if the start of your image has been "rolled" by the flash
// boot path to make it appear at the address it was linked at even though it is stored at a
// different location in flash, which is necessary when you have A/B images for example.
//
// The address translation between flash runtime and flash storage addresses is configured in
// hardware by the QMI_ATRANSx registers, and this API assumes those registers contain a valid
// address mapping which it can use to translate runtime to storage addresses.

typedef struct cflash_flags {
    uint32_t flags;
} cflash_flags_t;

// Bits which are permitted to be set in a flags variable -- any other bits being set is an error
#define CFLASH_FLAGS_BITS               0x00070301u

// Used to tell checked flash API which space a given address belongs to
#define CFLASH_ASPACE_BITS              0x00000001u
#define CFLASH_ASPACE_LSB               0u
#define CFLASH_ASPACE_VALUE_STORAGE     0u
#define CFLASH_ASPACE_VALUE_RUNTIME     1u

// Used to tell checked flash APIs the effective security level of a flash access (may be forced to
// one of these values for the NonSecure-exported version of this API)
#define CFLASH_SECLEVEL_BITS            0x00000300u
#define CFLASH_SECLEVEL_LSB             8u
// Zero is not a valid security level:
#define CFLASH_SECLEVEL_VALUE_SECURE    1u
#define CFLASH_SECLEVEL_VALUE_NONSECURE 2u
#define CFLASH_SECLEVEL_VALUE_BOOTLOADER 3u

#define CFLASH_OP_BITS                  0x00070000u
#define CFLASH_OP_LSB                   16u
// Erase size_bytes bytes of flash, starting at address addr. Both addr and size_bytes must be a
// multiple of 4096 bytes (one flash sector).
#define CFLASH_OP_VALUE_ERASE           0u
// Program size_bytes bytes of flash, starting at address addr. Both addr and size_bytes must be a
// multiple of 256 bytes (one flash page).
#define CFLASH_OP_VALUE_PROGRAM         1u
// Read size_bytes bytes of flash, starting at address addr. There are no alignment restrictions on
// addr or size_bytes.
#define CFLASH_OP_VALUE_READ            2u
#define CFLASH_OP_MAX                   2u

#endif

#endif
