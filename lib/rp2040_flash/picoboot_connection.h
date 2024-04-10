/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICOBOOT_CONNECTION_H
#define _PICOBOOT_CONNECTION_H

// todo we should use fully encapsulate libusb

#include <assert.h>
#include <libusb.h>
#include "boot/picoboot.h"

#ifdef __cplusplus
extern "C" {
#endif

enum picoboot_device_result {
    dr_vidpid_bootrom_ok,
    dr_vidpid_bootrom_no_interface,
    dr_vidpid_bootrom_cant_connect,
    dr_vidpid_micropython,
    dr_vidpid_picoprobe,
    dr_vidpid_unknown,
    dr_error,
    dr_vidpid_stdio_usb,
};

enum picoboot_device_result picoboot_open_device(libusb_device *device, libusb_device_handle **dev_handle);

int picoboot_reset(libusb_device_handle *usb_device);
int picoboot_cmd_status_verbose(libusb_device_handle *usb_device, struct picoboot_cmd_status *status,
                                bool local_verbose);
int picoboot_cmd_status(libusb_device_handle *usb_device, struct picoboot_cmd_status *status);
int picoboot_exclusive_access(libusb_device_handle *usb_device, uint8_t exclusive);
int picoboot_enter_cmd_xip(libusb_device_handle *usb_device);
int picoboot_exit_xip(libusb_device_handle *usb_device);
int picoboot_reboot(libusb_device_handle *usb_device, uint32_t pc, uint32_t sp, uint32_t delay_ms);
int picoboot_exec(libusb_device_handle *usb_device, uint32_t addr);
int picoboot_flash_erase(libusb_device_handle *usb_device, uint32_t addr, uint32_t len);
int picoboot_vector(libusb_device_handle *usb_device, uint32_t addr);
int picoboot_write(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len);
int picoboot_read(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len);
int picoboot_poke(libusb_device_handle *usb_device, uint32_t addr, uint32_t data);
int picoboot_peek(libusb_device_handle *usb_device, uint32_t addr, uint32_t *data);

#define ROM_START   0x00000000
#define ROM_END     0x00004000
#define FLASH_START 0x10000000
#define FLASH_END   0x11000000 // this is maximum
#define XIP_SRAM_BASE 0x15000000
#define XIP_SRAM_END 0x15004000

#define SRAM_START  0x20000000
#define SRAM_END    0x20042000

#define SRAM_UNSTRIPED_START  0x21000000
#define SRAM_UNSTRIPED_END    0x21040000

// we require 256 (as this is the page size supported by the device)
#define LOG2_PAGE_SIZE 8u
#define PAGE_SIZE (1u << LOG2_PAGE_SIZE)
#define FLASH_SECTOR_ERASE_SIZE 4096u

enum memory_type {
    rom,
    flash,
    sram,
    sram_unstriped,
    xip_sram,
    invalid,
};

// inclusive of ends
static inline enum memory_type get_memory_type(uint32_t addr) {
    if (addr >= ROM_START && addr <= ROM_END) {
        return rom;
    }
    if (addr >= FLASH_START && addr <= FLASH_END) {
        return flash;
    }
    if (addr >= SRAM_START && addr <= SRAM_END) {
        return sram;
    }
    if (addr >= SRAM_UNSTRIPED_START && addr <= SRAM_UNSTRIPED_END) {
        return sram_unstriped;
    }
    if (addr >= XIP_SRAM_BASE && addr <= XIP_SRAM_END) {
        return xip_sram;
    }
    return invalid;
}

static inline bool is_transfer_aligned(uint32_t addr) {
    enum memory_type t = get_memory_type(addr);
    return t != invalid && !(t == flash && addr & (PAGE_SIZE-1));
}

static inline bool is_size_aligned(uint32_t addr, int size) {
#ifndef _MSC_VER
    assert(__builtin_popcount(size)==1);
#endif
    return !(addr & (size-1));
}

#ifdef __cplusplus
}
#endif
#endif
