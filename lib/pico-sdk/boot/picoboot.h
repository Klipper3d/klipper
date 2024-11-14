/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOOT_PICOBOOT_H
#define _BOOT_PICOBOOT_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#ifndef NO_PICO_PLATFORM
#include "pico/platform.h"
#endif

/** \file picoboot.h
*  \defgroup boot_picoboot_headers boot_picoboot_headers
*
* \brief Header file for the PICOBOOT USB interface exposed by an RP2xxx chip in BOOTSEL mode
*/

#include "picoboot_constants.h"

#define PICOBOOT_MAGIC 0x431fd10bu

// --------------------------------------------
// CONTROL REQUESTS FOR THE PICOBOOT INTERFACE
// --------------------------------------------

// size 0 OUT - un-stall EPs and reset
#define PICOBOOT_IF_RESET 0x41

// size 16 IN - return the status of the last command
#define PICOBOOT_IF_CMD_STATUS 0x42

// --------------------------------------------------
// COMMAND REQUESTS SENT TO THE PICOBOOT OUT ENDPOINT
// --------------------------------------------------
//
// picoboot_cmd structure of size 32 is sent to OUT endpoint
// transfer_length bytes are transferred via IN/OUT
// device responds on success with 0 length ACK packet set via OUT/IN
// device may stall the transferring endpoint in case of error

enum picoboot_cmd_id {
    PC_EXCLUSIVE_ACCESS = 0x1,
    PC_REBOOT = 0x2,
    PC_FLASH_ERASE = 0x3,
    PC_READ = 0x84, // either RAM or FLASH
    PC_WRITE = 0x5, // either RAM or FLASH (does no erase)
    PC_EXIT_XIP = 0x6,
    PC_ENTER_CMD_XIP = 0x7,
    PC_EXEC = 0x8,
    PC_VECTORIZE_FLASH = 0x9,
    // RP2350 only below here
    PC_REBOOT2 = 0xa,
    PC_GET_INFO = 0x8b,
    PC_OTP_READ = 0x8c,
    PC_OTP_WRITE = 0xd,
    //PC_EXEC2 = 0xe, // currently unused
};

enum picoboot_status {
    PICOBOOT_OK = 0,
    PICOBOOT_UNKNOWN_CMD = 1,
    PICOBOOT_INVALID_CMD_LENGTH = 2,
    PICOBOOT_INVALID_TRANSFER_LENGTH = 3,
    PICOBOOT_INVALID_ADDRESS = 4,
    PICOBOOT_BAD_ALIGNMENT = 5,
    PICOBOOT_INTERLEAVED_WRITE = 6,
    PICOBOOT_REBOOTING = 7,
    PICOBOOT_UNKNOWN_ERROR = 8,
    PICOBOOT_INVALID_STATE = 9,
    PICOBOOT_NOT_PERMITTED = 10,
    PICOBOOT_INVALID_ARG = 11,
    PICOBOOT_BUFFER_TOO_SMALL = 12,
    PICOBOOT_PRECONDITION_NOT_MET = 13,
    PICOBOOT_MODIFIED_DATA = 14,
    PICOBOOT_INVALID_DATA = 15,
    PICOBOOT_NOT_FOUND = 16,
    PICOBOOT_UNSUPPORTED_MODIFICATION = 17,
};

struct __packed picoboot_reboot_cmd {
    uint32_t dPC; // 0 means reset into regular boot path
    uint32_t dSP;
    uint32_t dDelayMS;
};


// note this (with pc_sp) union member has the same layout as picoboot_reboot_cmd except with extra dFlags
struct __packed picoboot_reboot2_cmd {
    uint32_t dFlags;
    uint32_t dDelayMS;
    uint32_t dParam0;
    uint32_t dParam1;
};

// used for EXEC, VECTORIZE_FLASH
struct __packed picoboot_address_only_cmd {
    uint32_t dAddr;
};

// used for READ, WRITE, FLASH_ERASE
struct __packed picoboot_range_cmd {
    uint32_t dAddr;
    uint32_t dSize;
};

struct __packed picoboot_exec2_cmd {
    uint32_t image_base;
    uint32_t image_size;
    uint32_t workarea_base;
    uint32_t workarea_size;
};

enum picoboot_exclusive_type {
    NOT_EXCLUSIVE = 0,
    EXCLUSIVE,
    EXCLUSIVE_AND_EJECT
};

struct __packed picoboot_exclusive_cmd {
    uint8_t bExclusive;
};

struct __packed picoboot_otp_cmd {
    uint16_t wRow; // OTP row
    uint16_t wRowCount; // number of rows to transfer
    uint8_t bEcc; // use error correction (16 bit per register vs 24 (stored as 32) bit raw)
};


struct __packed picoboot_get_info_cmd {
    uint8_t bType;
    uint8_t bParam;
    uint16_t wParam;
    uint32_t dParams[3];
};

// little endian
struct __packed __aligned(4) picoboot_cmd {
    uint32_t dMagic;
    uint32_t dToken; // an identifier for this token to correlate with a status response
    uint8_t bCmdId; // top bit set for IN
    uint8_t bCmdSize; // bytes of actual data in the arg part of this structure
    uint16_t _unused;
    uint32_t dTransferLength; // length of IN/OUT transfer (or 0) if none
    union {
        uint8_t args[16];
        struct picoboot_reboot_cmd reboot_cmd;
        struct picoboot_range_cmd range_cmd;
        struct picoboot_address_only_cmd address_only_cmd;
        struct picoboot_exclusive_cmd exclusive_cmd;
        struct picoboot_reboot2_cmd reboot2_cmd;
        struct picoboot_otp_cmd otp_cmd;
        struct picoboot_get_info_cmd get_info_cmd;
        struct picoboot_exec2_cmd exec2_cmd;
    };
};
static_assert(32 == sizeof(struct picoboot_cmd), "picoboot_cmd must be 32 bytes big");

struct __packed __aligned(4) picoboot_cmd_status {
    uint32_t dToken;
    uint32_t dStatusCode;
    uint8_t bCmdId;
    uint8_t bInProgress;
    uint8_t _pad[6];
};

static_assert(16 == sizeof(struct picoboot_cmd_status), "picoboot_cmd_status must be 16 bytes big");

#endif
