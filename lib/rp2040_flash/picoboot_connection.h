/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PICOBOOT_CONNECTION_H
#define _PICOBOOT_CONNECTION_H

// todo we should use fully encapsulate libusb

#include <assert.h>
#if HAS_LIBUSB
#include <libusb.h>
#endif
#include "boot/picoboot.h"

#include "addresses.h"

#define VENDOR_ID_RASPBERRY_PI 0x2e8au
#define PRODUCT_ID_RP2040_USBBOOT 0x0003u
#define PRODUCT_ID_PICOPROBE   0x0004u
#define PRODUCT_ID_MICROPYTHON 0x0005u
#define PRODUCT_ID_STDIO_USB   0x0009u
#define PRODUCT_ID_RP2040_STDIO_USB 0x000au
#define PRODUCT_ID_RP2350_USBBOOT 0x000fu

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

typedef enum {
    rp2040,
    rp2350,
    unknown
} model_t;

typedef enum {
    rp2350_a2,
    rp2350_unknown
} rp2350_version_t;

#if HAS_LIBUSB
// note that vid and pid are filters, unless both are specified in which case a device with that VID and PID is allowed for RP2350
enum picoboot_device_result picoboot_open_device(libusb_device *device, libusb_device_handle **dev_handle, model_t *model, int vid, int pid, const char* ser);

int picoboot_reset(libusb_device_handle *usb_device);
int picoboot_cmd_status_verbose(libusb_device_handle *usb_device, struct picoboot_cmd_status *status,
                                bool local_verbose);
int picoboot_cmd_status(libusb_device_handle *usb_device, struct picoboot_cmd_status *status);
int picoboot_exclusive_access(libusb_device_handle *usb_device, uint8_t exclusive);
int picoboot_enter_cmd_xip(libusb_device_handle *usb_device);
int picoboot_exit_xip(libusb_device_handle *usb_device);
int picoboot_reboot(libusb_device_handle *usb_device, uint32_t pc, uint32_t sp, uint32_t delay_ms);
int picoboot_reboot2(libusb_device_handle *usb_device, struct picoboot_reboot2_cmd *reboot_cmd);
int picoboot_get_info(libusb_device_handle *usb_device, struct picoboot_get_info_cmd *cmd, uint8_t *buffer, uint32_t len);
int picoboot_exec(libusb_device_handle *usb_device, uint32_t addr);
// int picoboot_exec2(libusb_device_handle *usb_device, struct picoboot_exec2_cmd *exec2_cmd); // currently unused
int picoboot_flash_erase(libusb_device_handle *usb_device, uint32_t addr, uint32_t len);
int picoboot_vector(libusb_device_handle *usb_device, uint32_t addr);
int picoboot_write(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len);
int picoboot_read(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len);
int picoboot_otp_write(libusb_device_handle *usb_device, struct picoboot_otp_cmd *otp_cmd, uint8_t *buffer, uint32_t len);
int picoboot_otp_read(libusb_device_handle *usb_device, struct picoboot_otp_cmd *otp_cmd, uint8_t *buffer, uint32_t len);
int picoboot_poke(libusb_device_handle *usb_device, uint32_t addr, uint32_t data);
int picoboot_peek(libusb_device_handle *usb_device, uint32_t addr, uint32_t *data);
int picoboot_flash_id(libusb_device_handle *usb_device, uint64_t *data);
#endif

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
static inline enum memory_type get_memory_type(uint32_t addr, model_t model) {
    if (addr >= FLASH_START && addr <= FLASH_END_RP2040) {
        return flash;
    }
    if (addr >= ROM_START && addr <= ROM_END_RP2040) {
        return rom;
    }
    if (addr >= SRAM_START && addr <= SRAM_END_RP2040) {
        return sram;
    }
    if (model == rp2350) {
        if (addr >= FLASH_START && addr <= FLASH_END_RP2350) {
            return flash;
        }
        if (addr >= ROM_START && addr <= ROM_END_RP2350) {
            return rom;
        }
        if (addr >= SRAM_START && addr <= SRAM_END_RP2350) {
            return sram;
        }
    }
    if (addr >= MAIN_RAM_BANKED_START && addr <= MAIN_RAM_BANKED_END) {
        return sram_unstriped;
    }
    if (model == rp2040) {
        if (addr >= XIP_SRAM_START_RP2040 && addr <= XIP_SRAM_END_RP2040) {
            return xip_sram;
        }
    } else if (model == rp2350) {
        if (addr >= XIP_SRAM_START_RP2350 && addr <= XIP_SRAM_END_RP2350) {
            return xip_sram;
        }
    }
    return invalid;
}

static inline bool is_transfer_aligned(uint32_t addr, model_t model) {
    enum memory_type t = get_memory_type(addr, model);
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
