/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "picoboot_connection.h"

#if false && !defined(NDEBUG)
#define output(format,...) printf(format, __VA_ARGS__)
#else
#define output(format,...) ((void)0)
#endif

static bool verbose;

// todo test sparse binary (well actually two range is this)

#define VENDOR_ID_RASPBERRY_PI 0x2e8au
#define PRODUCT_ID_RP2_USBBOOT 0x0003u
#define PRODUCT_ID_PICOPROBE   0x0004u
#define PRODUCT_ID_MICROPYTHON 0x0005u
#define PRODUCT_ID_STDIO_USB   0x000au

uint32_t crc32_for_byte(uint32_t remainder) {
    const uint32_t POLYNOMIAL = 0x4C11DB7;
    remainder <<= 24u;
    for (uint bit = 8; bit > 0; bit--) {
        if (remainder & 0x80000000)
            remainder = (remainder << 1) ^ POLYNOMIAL;
        else
            remainder = (remainder << 1);
    }
    return remainder;
}

uint32_t crc32_sw(const uint8_t *buf, uint count, uint32_t crc) {
    static uint32_t table[0x100];
    if (!table[1]) {
        for (uint i = 0; i < count_of(table); i++) {
            table[i] = crc32_for_byte(i);
        }
    }
    for (uint i = 0; i < count; ++i) {
        crc = (crc << 8u) ^ table[(uint8_t) ((crc >> 24u) ^ buf[i])];
    }
    return crc;
}

uint interface;
uint out_ep;
uint in_ep;

enum picoboot_device_result picoboot_open_device(libusb_device *device, libusb_device_handle **dev_handle) {
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *config;

    *dev_handle = NULL;
    int ret = libusb_get_device_descriptor(device, &desc);
    if (ret && verbose) {
        output("Failed to read device descriptor");
    }
    if (!ret) {
        if (desc.idVendor != VENDOR_ID_RASPBERRY_PI) {
            return dr_vidpid_unknown;
        }
        switch (desc.idProduct) {
            case PRODUCT_ID_MICROPYTHON:
                return dr_vidpid_micropython;
            case PRODUCT_ID_PICOPROBE:
                return dr_vidpid_picoprobe;
            case PRODUCT_ID_STDIO_USB:
                return dr_vidpid_stdio_usb;
            case PRODUCT_ID_RP2_USBBOOT:
                break;
            default:
                return dr_vidpid_unknown;
        }
        ret = libusb_get_active_config_descriptor(device, &config);
        if (ret && verbose) {
            output("Failed to read config descriptor\n");
        }
    }

    if (!ret) {
        ret  = libusb_open(device, dev_handle);
        if (ret && verbose) {
            output("Failed to open device %d\n", ret);
        }
        if (ret) {
            return dr_vidpid_bootrom_cant_connect;
        }
    }

    if (!ret) {
        if (config->bNumInterfaces == 1) {
            interface = 0;
        } else {
            interface = 1;
        }
        if (config->interface[interface].altsetting[0].bInterfaceClass == 0xff &&
            config->interface[interface].altsetting[0].bNumEndpoints == 2) {
            out_ep = config->interface[interface].altsetting[0].endpoint[0].bEndpointAddress;
            in_ep = config->interface[interface].altsetting[0].endpoint[1].bEndpointAddress;
        }
        if (out_ep && in_ep && !(out_ep & 0x80u) && (in_ep & 0x80u)) {
            if (verbose) output("Found PICOBOOT interface\n");
            ret = libusb_claim_interface(*dev_handle, interface);
            if (ret) {
                if (verbose) output("Failed to claim interface\n");
                return dr_vidpid_bootrom_no_interface;
            }

            return dr_vidpid_bootrom_ok;
        } else {
            if (verbose) output("Did not find PICOBOOT interface\n");
            return dr_vidpid_bootrom_no_interface;
        }
    }

    assert(ret);

    if (*dev_handle) {
        libusb_close(*dev_handle);
        *dev_handle = NULL;
    }

    return dr_error;
}

static bool is_halted(libusb_device_handle *usb_device, int ep) {
    uint8_t data[2];

    int transferred = libusb_control_transfer(
            usb_device,
            /*LIBUSB_REQUEST_TYPE_STANDARD | */LIBUSB_RECIPIENT_ENDPOINT | LIBUSB_ENDPOINT_IN,
            LIBUSB_REQUEST_GET_STATUS,
            0, ep,
            data, sizeof(data),
            1000);
    if (transferred != sizeof(data)) {
        output("Get status failed\n");
        return false;
    }
    if (data[0] & 1) {
        if (verbose) output("%d was halted\n", ep);
        return true;
    }
    if (verbose) output("%d was not halted\n", ep);
    return false;
}

int picoboot_reset(libusb_device_handle *usb_device) {
    if (verbose) output("RESET\n");
    if (is_halted(usb_device, in_ep))
        libusb_clear_halt(usb_device, in_ep);
    if (is_halted(usb_device, out_ep))
        libusb_clear_halt(usb_device, out_ep);
    int ret =
            libusb_control_transfer(usb_device, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE,
                                    PICOBOOT_IF_RESET, 0, interface, NULL, 0, 1000);

    if (ret != 0) {
        output("  ...failed\n");
        return ret;
    }
    if (verbose) output("  ...ok\n");
    return 0;
}

int picoboot_cmd_status_verbose(libusb_device_handle *usb_device, struct picoboot_cmd_status *status, bool local_verbose) {
    struct picoboot_cmd_status s;
    if (!status) status = &s;

    if (local_verbose) output("CMD_STATUS\n");
    int ret =
            libusb_control_transfer(usb_device,
                                    LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_ENDPOINT_IN,
                                    PICOBOOT_IF_CMD_STATUS, 0, interface, (uint8_t *) status, sizeof(*status), 1000);

    if (ret != sizeof(*status)) {
        output("  ...failed\n");
        return ret;
    }
    if (local_verbose)
        output("  ... cmd %02x%s tok=%08x status=%d\n", status->bCmdId, status->bInProgress ? " (in progress)" : "",
               status->dToken, status->dStatusCode);
    return 0;
}

int picoboot_cmd_status(libusb_device_handle *usb_device, struct picoboot_cmd_status *status) {
    return picoboot_cmd_status_verbose(usb_device, status, verbose);
}

int one_time_bulk_timeout;

int picoboot_cmd(libusb_device_handle *usb_device, struct picoboot_cmd *cmd, uint8_t *buffer, uint buf_size) {
    int sent = 0;
    int ret;

    static int token = 1;
    cmd->dMagic = PICOBOOT_MAGIC;
    cmd->dToken = token++;
    ret = libusb_bulk_transfer(usb_device, out_ep, (uint8_t *) cmd, sizeof(struct picoboot_cmd), &sent, 3000);

    if (ret != 0 || sent != sizeof(struct picoboot_cmd)) {
        output("   ...failed to send command %d\n", ret);
        return ret;
    }

    int timeout = 10000;
    if (one_time_bulk_timeout) {
        timeout = one_time_bulk_timeout;
        one_time_bulk_timeout = 0;
    }
    if (cmd->dTransferLength != 0) {
        assert(buf_size >= cmd->dTransferLength);
        if (cmd->bCmdId & 0x80u) {
            if (verbose) output("  receive %d...\n", cmd->dTransferLength);
            int received = 0;
            ret = libusb_bulk_transfer(usb_device, in_ep, buffer, cmd->dTransferLength, &received, timeout);
            if (ret != 0 || received != (int) cmd->dTransferLength) {
                output("  ...failed to receive data %d %d/%d\n", ret, received, cmd->dTransferLength);
                if (!ret) ret = 1;
                return ret;
            }
        } else {
            if (verbose) output("  send %d...\n", cmd->dTransferLength);
            ret = libusb_bulk_transfer(usb_device, out_ep, buffer, cmd->dTransferLength, &sent, timeout);
            if (ret != 0 || sent != (int) cmd->dTransferLength) {
                output("  ...failed to send data %d %d/%d\n", ret, sent, cmd->dTransferLength);
                if (!ret) ret = 1;
                picoboot_cmd_status_verbose(usb_device, NULL, true);
                return ret;
            }
        }
    }

    // ack is in opposite direction
    int received = 0;
    uint8_t spoon[64];
    if (cmd->bCmdId & 0x80u) {
        if (verbose) output("zero length out\n");
        ret = libusb_bulk_transfer(usb_device, out_ep, spoon, 1, &received, cmd->dTransferLength == 0 ? timeout : 3000);
    } else {
        if (verbose) output("zero length in\n");
        ret = libusb_bulk_transfer(usb_device, in_ep, spoon, 1, &received, cmd->dTransferLength == 0 ? timeout : 3000);
    }
    return ret;
}

int picoboot_exclusive_access(libusb_device_handle *usb_device, uint8_t exclusive) {
    if (verbose) output("EXCLUSIVE ACCESS %d\n", exclusive);
    struct picoboot_cmd cmd;
    cmd.bCmdId = PC_EXCLUSIVE_ACCESS;
    cmd.exclusive_cmd.bExclusive = exclusive;
    cmd.bCmdSize = sizeof(struct picoboot_exclusive_cmd);
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_exit_xip(libusb_device_handle *usb_device) {
    struct picoboot_cmd cmd;
    if (verbose) output("EXIT_XIP\n");
    cmd.bCmdId = PC_EXIT_XIP;
    cmd.bCmdSize = 0;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_enter_cmd_xip(libusb_device_handle *usb_device) {
    struct picoboot_cmd cmd;
    if (verbose) output("ENTER_CMD_XIP\n");
    cmd.bCmdId = PC_ENTER_CMD_XIP;
    cmd.bCmdSize = 0;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_reboot(libusb_device_handle *usb_device, uint32_t pc, uint32_t sp, uint32_t delay_ms) {
    struct picoboot_cmd cmd;
    if (verbose) output("REBOOT %08x %08x %u\n", (uint) pc, (uint) sp, (uint) delay_ms);
    cmd.bCmdId = PC_REBOOT;
    cmd.bCmdSize = sizeof(cmd.reboot_cmd);
    cmd.dTransferLength = 0;
    cmd.reboot_cmd.dPC = pc;
    cmd.reboot_cmd.dSP = sp;
    cmd.reboot_cmd.dDelayMS = delay_ms;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_exec(libusb_device_handle *usb_device, uint32_t addr) {
    struct picoboot_cmd cmd;
    // shouldn't be necessary any more
    // addr |= 1u; // Thumb bit
    if (verbose) output("EXEC %08x\n", (uint) addr);
    cmd.bCmdId = PC_EXEC;
    cmd.bCmdSize = sizeof(cmd.address_only_cmd);
    cmd.dTransferLength = 0;
    cmd.address_only_cmd.dAddr = addr;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_flash_erase(libusb_device_handle *usb_device, uint32_t addr, uint32_t len) {
    struct picoboot_cmd cmd;
    if (verbose) output("FLASH_ERASE %08x+%08x\n", (uint) addr, (uint) len);
    cmd.bCmdId = PC_FLASH_ERASE;
    cmd.bCmdSize = sizeof(cmd.range_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.range_cmd.dSize = len;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_vector(libusb_device_handle *usb_device, uint32_t addr) {
    struct picoboot_cmd cmd;
    if (verbose) output("VECTOR %08x\n", (uint) addr);
    cmd.bCmdId = PC_VECTORIZE_FLASH;
    cmd.bCmdSize = sizeof(cmd.address_only_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_write(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len) {
    struct picoboot_cmd cmd;
    if (verbose) output("WRITE %08x+%08x\n", (uint) addr, (uint) len);
    cmd.bCmdId = PC_WRITE;
    cmd.bCmdSize = sizeof(cmd.range_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.range_cmd.dSize = cmd.dTransferLength = len;
    return picoboot_cmd(usb_device, &cmd, buffer, len);
}

int picoboot_read(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len) {
    memset(buffer, 0xaa, len);
    if (verbose) output("READ %08x+%08x\n", (uint) addr, (uint) len);
    struct picoboot_cmd cmd;
    cmd.bCmdId = PC_READ;
    cmd.bCmdSize = sizeof(cmd.range_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.range_cmd.dSize = cmd.dTransferLength = len;
    int ret = picoboot_cmd(usb_device, &cmd, buffer, len);
    if (!ret && len < 256 && verbose) {
        for (uint32_t i = 0; i < len; i += 32) {
            output("\t");
            for (uint32_t j = i; j < MIN(len, i + 32); j++) {
                output("0x%02x, ", buffer[j]);
            }
            output("\n");
        }
    }
    return ret;
}


#if 0
// Peek/poke via EXEC

// 00000000 <poke>:
//    0:   4801        ldr r0, [pc, #4]    ; (8 <data>)
//    2:   4902        ldr r1, [pc, #8]    ; (c <addr>)
//    4:   6008        str r0, [r1, #0]
//    6:   4770        bx  lr
// 00000008 <data>:
//    8:   12345678    .word   0x12345678
// 0000000c <addr>:
//    c:   9abcdef0    .word   0x9abcdef0


static const size_t picoboot_poke_cmd_len = 8;
static const uint8_t picoboot_poke_cmd[] = {
        0x01, 0x48, 0x02, 0x49, 0x08, 0x60, 0x70, 0x47
};

// 00000000 <peek>:
//    0:   4802        ldr r0, [pc, #8]    ; (c <inout>)
//    2:   6800        ldr r0, [r0, #0]
//    4:   4679        mov r1, pc
//    6:   6048        str r0, [r1, #4]
//    8:   4770        bx  lr
//    a:   46c0        nop         ; (mov r8, r8)
// 0000000c <inout>:
//    c:   0add7355    .word   0x0add7355

static const size_t picoboot_peek_cmd_len = 12;
static const uint8_t picoboot_peek_cmd[] = {
        0x02, 0x48, 0x00, 0x68, 0x79, 0x46, 0x48, 0x60, 0x70, 0x47, 0xc0, 0x46
};

// TODO better place for this e.g. the USB DPRAM location the controller has already put it in
#define PEEK_POKE_CODE_LOC 0x20000000u

int picoboot_poke(libusb_device_handle *usb_device, uint32_t addr, uint32_t data) {
    const size_t prog_size = picoboot_poke_cmd_len + 8;
    uint8_t prog[prog_size];
    output("POKE (D)%08x -> (A)%08x\n", data, addr);
    memcpy(prog, picoboot_poke_cmd, picoboot_poke_cmd_len);
    *(uint32_t *) (prog + picoboot_poke_cmd_len) = data;
    *(uint32_t *) (prog + picoboot_poke_cmd_len + 4) = addr;

    int ret = picoboot_write(usb_device, PEEK_POKE_CODE_LOC, prog, prog_size);
    if (ret)
        return ret;
    return picoboot_exec(usb_device, PEEK_POKE_CODE_LOC);
}

// TODO haven't checked the store goes to the right address :)
int picoboot_peek(libusb_device_handle *usb_device, uint32_t addr, uint32_t *data) {
    const size_t prog_size = picoboot_peek_cmd_len + 4;
    uint8_t prog[prog_size];
    output("PEEK %08x\n", addr);
    memcpy(prog, picoboot_peek_cmd, picoboot_peek_cmd_len);
    *(uint32_t *) (prog + picoboot_peek_cmd_len) = addr;

    int ret = picoboot_write(usb_device, PEEK_POKE_CODE_LOC, prog, prog_size);
    if (ret)
        return ret;
    ret = picoboot_exec(usb_device, PEEK_POKE_CODE_LOC);
    if (ret)
        return ret;
    return picoboot_read(usb_device, PEEK_POKE_CODE_LOC + picoboot_peek_cmd_len, (uint8_t *) data, sizeof(uint32_t));
}
#endif
