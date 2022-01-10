// Simple rp2040 picoboot based flash tool for use with Klipper
//
// Copyright (C) 2022  Lasse Dalegaard <dalegaard@gmail.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "picoboot_connection.h"
#include "boot/uf2.h"

#define FLASH_MAX_SIZE (FLASH_END - FLASH_START)
#define FLASH_NUM_WRITE_BLOCKS (FLASH_MAX_SIZE / PAGE_SIZE)
#define FLASH_NUM_ERASE_BLOCKS (FLASH_MAX_SIZE / FLASH_SECTOR_ERASE_SIZE)

struct flash_data {
    size_t num_blocks;
    uint8_t flash_data[FLASH_MAX_SIZE];
    bool write_blocks[FLASH_NUM_WRITE_BLOCKS];
    bool erase_blocks[FLASH_NUM_ERASE_BLOCKS];
};

int load_flash_data(const char *filename, struct flash_data *target) {
    int rc = 0;
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open image file %s\n", filename);
        rc = errno;
        goto do_exit;
    }

    target->num_blocks = 0;
    memset(target->write_blocks, 0, sizeof(target->write_blocks));
    memset(target->erase_blocks, 0, sizeof(target->erase_blocks));

    struct uf2_block block;

    while (1) {
        if(fread(&block, sizeof(struct uf2_block), 1, file) != 1) {
            if (feof(file)) {
                break;
            }
            fprintf(stderr, "Unexpected EOF reading image\n");
            rc = errno;
            goto do_exit;
        }

        // Check magic numbers
        if (block.magic_start0 != UF2_MAGIC_START0) continue;
        if (block.magic_start1 != UF2_MAGIC_START1) continue;
        if (block.magic_end != UF2_MAGIC_END) continue;

        // Check block is valid for flashing
        // Always family ID.
        if (!(block.flags & UF2_FLAG_FAMILY_ID_PRESENT)) continue;
        if (block.file_size != RP2040_FAMILY_ID) continue;
        if (block.flags & UF2_FLAG_NOT_MAIN_FLASH) continue;
        if (block.payload_size != PAGE_SIZE) continue;

        // Bounds and alignment checking
        if (block.target_addr != (block.target_addr & ~(PAGE_SIZE-1))) continue;
        if (block.target_addr > FLASH_END - PAGE_SIZE) continue;
        if (block.target_addr < FLASH_START) continue;

        uint32_t offset = block.target_addr - FLASH_START;

        // Copy data and mark the matching write and erase blocks
        memcpy(&target->flash_data[offset], block.data, PAGE_SIZE);
        target->write_blocks[offset / PAGE_SIZE] = 1;
        target->erase_blocks[offset / FLASH_SECTOR_ERASE_SIZE] = 1;

        target->num_blocks++;
    }

do_exit:
    if (file) {
        fclose(file);
    }
    return rc;
}

const char *status_codes_strings[] = {
    "ok",
    "unknown command",
    "bad address alignment",
    "interleaved write",
    "invalid address",
    "invalid cmd length",
    "invalid transfer length",
    "rebooting",
    "unknown error",
};

int report_error(libusb_device_handle *handle, const char *cmd) {
    struct picoboot_cmd_status status;
    status.dStatusCode = 0;
    int rc = picoboot_cmd_status(handle, &status);
    if (rc) {
        fprintf(stderr, "Command %s failed, and it was not possible to "
                "query PICOBOOT status\n", cmd);
    } else {
        if (status.dStatusCode == 0) status.dStatusCode = 8;
        fprintf(stderr, "Command %s failed with status %d: %s\n",
                cmd, status.dStatusCode,
                status_codes_strings[status.dStatusCode]);
    }
    return 1;
};

int picoboot_flash(libusb_device_handle *handle, struct flash_data *image) {
    fprintf(stderr, "Resetting interface\n");
    if (picoboot_reset(handle)) {
        return report_error(handle, "reset");
    }

    fprintf(stderr, "Locking\n");
    if (picoboot_exclusive_access(handle, EXCLUSIVE)) {
        return report_error(handle, "exclusive_access");
    }

    fprintf(stderr, "Exiting XIP mode\n");
    if (picoboot_exit_xip(handle)) {
        return report_error(handle, "exit_xip");
    }

    fprintf(stderr, "Erasing\n");
    for(size_t i = 0; i < FLASH_NUM_ERASE_BLOCKS; i++) {
        if (!image->erase_blocks[i]) continue;
        uint32_t addr = FLASH_START + i * FLASH_SECTOR_ERASE_SIZE;
        if (picoboot_flash_erase(handle, addr, FLASH_SECTOR_ERASE_SIZE)) {
            return report_error(handle, "flash_erase");
        }
    }

    fprintf(stderr, "Flashing\n");
    for(size_t i = 0; i < FLASH_NUM_WRITE_BLOCKS; i++) {
        if (!image->write_blocks[i]) continue;
        uint32_t addr = FLASH_START + i * PAGE_SIZE;
        uint8_t *buf = &image->flash_data[i * PAGE_SIZE];
        if (picoboot_write(handle, addr, buf, PAGE_SIZE)) {
            return report_error(handle, "write");
        }
    }

    fprintf(stderr, "Rebooting device\n");
    if (picoboot_reboot(handle, 0, 0, 500)) {
        return report_error(handle, "reboot");
    }

    return 0;
}

void print_usage(char *argv[]) {
    fprintf(stderr, "Usage: %s <uf2 image> [bus addr]\n", argv[0]);
    exit(1);
}

int main(int argc, char *argv[]) {
    libusb_context *ctx = 0;
    struct libusb_device **devs = 0;
    libusb_device_handle *handle = 0;
    struct flash_data *image = malloc(sizeof(struct flash_data));
    int rc = 0;

    if (argc != 2 && argc != 4) {
        print_usage(argv);
    }

    if (load_flash_data(argv[1], image)) {
        fprintf(stderr, "Could not load flash image, exiting\n");
        rc = 1;
        goto do_exit;
    }
    fprintf(stderr, "Loaded UF2 image with %lu pages\n", image->num_blocks);

    bool has_target = false;
    uint8_t target_bus = 0;
    uint8_t target_address = 0;
    if(argc == 4) {
        has_target = true;

        char *endptr;
        target_bus = strtol(argv[2], &endptr, 10);
        if (endptr == argv[2] || *endptr != 0) print_usage(argv);

        target_address = strtol(argv[3], &endptr, 10);
        if (endptr == argv[3] || *endptr != 0) print_usage(argv);
    }

    if (libusb_init(&ctx)) {
        fprintf(stderr, "Could not initialize libusb\n");
        rc = 1;
        goto do_exit;
    }

    ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        fprintf(stderr, "Failed to enumerate USB devices: %s",
                libusb_strerror(cnt));
        rc = 1;
        goto do_exit;
    }

    for (libusb_device **dev = devs; *dev; ++dev) {
        if (has_target) {
            if (target_bus != libusb_get_bus_number(*dev)) continue;
            if (target_address != libusb_get_device_address(*dev)) continue;
        }
        enum picoboot_device_result res = picoboot_open_device(*dev, &handle);
        if (res == dr_vidpid_bootrom_ok) {
            break;
        }
        if (handle) {
            libusb_close(handle);
            handle = 0;
        }
    }

    if (!handle) {
        fprintf(stderr, "No rp2040 in BOOTSEL mode was found\n");
        goto do_exit;
    }

    libusb_device *dev = libusb_get_device(handle);
    fprintf(stderr, "Found rp2040 device on USB bus %d address %d\n",
        libusb_get_bus_number(dev), libusb_get_device_address(dev));
    fprintf(stderr, "Flashing...\n");

    rc = picoboot_flash(handle, image);

do_exit:
    if (handle) {
        libusb_close(handle);
    }
    if (devs) {
        libusb_free_device_list(devs, 1);
    }
    if (ctx) {
        libusb_exit(ctx);
    }
    free(image);
    return rc;
}
