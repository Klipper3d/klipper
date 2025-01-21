/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "picoboot_connection.h"
#include "pico/bootrom_constants.h"

#if ENABLE_DEBUG_LOG
#include <stdio.h>
#define output(...) printf(__VA_ARGS__)
#else
#define output(format,...) ((void)0)
#endif

static bool verbose;
static bool definitely_exclusive;
static enum {
    XIP_UNKOWN,
    XIP_ACTIVE,
    XIP_INACTIVE,
} xip_state;

// todo test sparse binary (well actually two range is this)

uint32_t crc32_for_byte(uint32_t remainder) {
    const uint32_t POLYNOMIAL = 0x4C11DB7;
    remainder <<= 24u;
    for (unsigned int bit = 8; bit > 0; bit--) {
        if (remainder & 0x80000000)
            remainder = (remainder << 1) ^ POLYNOMIAL;
        else
            remainder = (remainder << 1);
    }
    return remainder;
}

uint32_t crc32_sw(const uint8_t *buf, unsigned int count, uint32_t crc) {
    static uint32_t table[0x100];
    if (!table[1]) {
        for (unsigned int i = 0; i < count_of(table); i++) {
            table[i] = crc32_for_byte(i);
        }
    }
    for (unsigned int i = 0; i < count; ++i) {
        crc = (crc << 8u) ^ table[(uint8_t) ((crc >> 24u) ^ buf[i])];
    }
    return crc;
}

unsigned int interface;
unsigned int out_ep;
unsigned int in_ep;

enum picoboot_device_result picoboot_open_device(libusb_device *device, libusb_device_handle **dev_handle, model_t *model, int vid, int pid, const char* ser) {
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *config;

    definitely_exclusive = false;
    *dev_handle = NULL;
    *model = unknown;
    int ret = libusb_get_device_descriptor(device, &desc);
    enum picoboot_device_result res = dr_vidpid_unknown;
    if (ret && verbose) {
        output("Failed to read device descriptor\n");
    }
    if (!ret) {
        if (pid >= 0) {
            bool match_vid = (vid < 0 ? VENDOR_ID_RASPBERRY_PI : (unsigned int)vid) == desc.idVendor;
            bool match_pid = pid == desc.idProduct;
            if (!(match_vid && match_pid)) {
                return dr_vidpid_unknown;
            }
        } else if (vid != 0) { // ignore vid/pid filtering if no pid and vid == 0
            if (desc.idVendor != (vid < 0 ? VENDOR_ID_RASPBERRY_PI : (unsigned int)vid)) {
                return dr_vidpid_unknown;
            }
            switch (desc.idProduct) {
                case PRODUCT_ID_MICROPYTHON:
                    return dr_vidpid_micropython;
                case PRODUCT_ID_PICOPROBE:
                    return dr_vidpid_picoprobe;
                case PRODUCT_ID_RP2040_STDIO_USB:
                case PRODUCT_ID_STDIO_USB:
                    res = dr_vidpid_stdio_usb;
                    break;
                case PRODUCT_ID_RP2040_USBBOOT:
                    *model = rp2040;
                    break;
                case PRODUCT_ID_RP2350_USBBOOT:
                    *model = rp2350;
                    break;
                default:
                    return dr_vidpid_unknown;
            }
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
            if (vid == 0 || strlen(ser) != 0) {
                // didn't check vid or ser, so treat as unknown
                return dr_vidpid_unknown;
            } else if (res != dr_vidpid_unknown) {
                return res;
            } else {
                return dr_vidpid_bootrom_cant_connect;
            }
        }
    }

    if (res == dr_vidpid_stdio_usb) {
        if (strlen(ser) != 0) {
            // Check USB serial number
            char ser_str[128];
            libusb_get_string_descriptor_ascii(*dev_handle, desc.iSerialNumber, (unsigned char*)ser_str, sizeof(ser_str));
            if (strcmp(ser, ser_str)) {
                return dr_vidpid_unknown;
            } else {
                return res;
            }
        } else {
            return res;
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
        } else {
            if (verbose) output("Did not find PICOBOOT interface\n");
            return dr_vidpid_bootrom_no_interface;
        }
    }

    if (!ret) {
        if (*model == unknown) {
            struct picoboot_get_info_cmd info_cmd;
            info_cmd.bType = PICOBOOT_GET_INFO_SYS,
            info_cmd.dParams[0] = (uint32_t) (SYS_INFO_CHIP_INFO);
            uint32_t word_buf[64];
            // RP2040 doesn't have this function, so returns non-zero
            int info_ret = picoboot_get_info(*dev_handle, &info_cmd, (uint8_t*)word_buf, sizeof(word_buf));
            if (info_ret) {
                *model = rp2040;
            } else {
                *model = rp2350;
            }
        }
        if (strlen(ser) != 0) {
            if (*model == rp2040) {
                // Check flash ID, as USB serial number is not unique
                uint64_t ser_num = strtoull(ser, NULL, 16);
                uint64_t id = 0;
                int id_ret = picoboot_flash_id(*dev_handle, &id);
                if (verbose) output("Flash ID %"PRIX64"\n", id);
                if (id_ret || (ser_num != id)) {
                    return dr_vidpid_unknown;
                }
            } else {
                // Check USB serial number
                char ser_str[128];
                libusb_get_string_descriptor_ascii(*dev_handle, desc.iSerialNumber, (unsigned char*)ser_str, sizeof(ser_str));
                if (strcmp(ser, ser_str)) {
                    return dr_vidpid_unknown;
                }
            }
        }
        return dr_vidpid_bootrom_ok;
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
    definitely_exclusive = false;
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

int picoboot_cmd(libusb_device_handle *usb_device, struct picoboot_cmd *cmd, uint8_t *buffer, unsigned int buf_size) {
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

    int saved_xip_state = xip_state;
    bool saved_exclusive = definitely_exclusive;
    xip_state = XIP_UNKOWN;
    definitely_exclusive = false;
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
    if (!ret) {
        // do our defensive best to keep the xip_state up to date
        switch (cmd->bCmdId) {
            case PC_EXIT_XIP:
                xip_state = XIP_INACTIVE;
                break;
            case PC_ENTER_CMD_XIP:
                xip_state = XIP_ACTIVE;
                break;
            case PC_READ:
            case PC_WRITE:
                // whitelist PC_READ and PC_WRITE as not affecting xip state
                xip_state = saved_xip_state;
                break;
            default:
                xip_state = XIP_UNKOWN;
                break;
        }
        // do our defensive best to keep the exclusive var up to date
        switch (cmd->bCmdId) {
            case PC_EXCLUSIVE_ACCESS:
                definitely_exclusive = cmd->exclusive_cmd.bExclusive;
                break;
            case PC_ENTER_CMD_XIP:
            case PC_EXIT_XIP:
            case PC_READ:
            case PC_WRITE:
                // whitelist PC_READ and PC_WRITE as not affecting xip state
                definitely_exclusive = saved_exclusive;
                break;
            default:
                definitely_exclusive = false;
                break;
        }
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
    if (definitely_exclusive && xip_state == XIP_INACTIVE) {
        if (verbose) output("Skipping EXIT_XIP");
        return 0;
    }
    struct picoboot_cmd cmd;
    if (verbose) output("EXIT_XIP\n");
    cmd.bCmdId = PC_EXIT_XIP;
    cmd.bCmdSize = 0;
    cmd.dTransferLength = 0;
    xip_state = XIP_INACTIVE;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_enter_cmd_xip(libusb_device_handle *usb_device) {
    struct picoboot_cmd cmd;
    if (verbose) output("ENTER_CMD_XIP\n");
    cmd.bCmdId = PC_ENTER_CMD_XIP;
    cmd.bCmdSize = 0;
    cmd.dTransferLength = 0;
    xip_state = XIP_ACTIVE;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_reboot(libusb_device_handle *usb_device, uint32_t pc, uint32_t sp, uint32_t delay_ms) {
    struct picoboot_cmd cmd;
    if (verbose) output("REBOOT %08x %08x %u\n", (unsigned int) pc, (unsigned int) sp, (unsigned int) delay_ms);
    cmd.bCmdId = PC_REBOOT;
    cmd.bCmdSize = sizeof(cmd.reboot_cmd);
    cmd.dTransferLength = 0;
    cmd.reboot_cmd.dPC = pc;
    cmd.reboot_cmd.dSP = sp;
    cmd.reboot_cmd.dDelayMS = delay_ms;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_reboot2(libusb_device_handle *usb_device, struct picoboot_reboot2_cmd *reboot_cmd) {
    struct picoboot_cmd cmd;
    if (verbose) output("REBOOT %08x %08x %08x %u\n", (unsigned int)reboot_cmd->dFlags, (unsigned int) reboot_cmd->dParam0, (unsigned int) reboot_cmd->dParam1, (unsigned int) reboot_cmd->dDelayMS);
    cmd.bCmdId = PC_REBOOT2;
    cmd.bCmdSize = sizeof(cmd.reboot2_cmd);
    cmd.reboot2_cmd = *reboot_cmd;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_exec(libusb_device_handle *usb_device, uint32_t addr) {
    struct picoboot_cmd cmd;
    // shouldn't be necessary any more
    // addr |= 1u; // Thumb bit
    if (verbose) output("EXEC %08x\n", (unsigned int) addr);
    cmd.bCmdId = PC_EXEC;
    cmd.bCmdSize = sizeof(cmd.address_only_cmd);
    cmd.dTransferLength = 0;
    cmd.address_only_cmd.dAddr = addr;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

// int picoboot_exec2(libusb_device_handle *usb_device, struct picoboot_exec2_cmd *exec2_cmd) {
//     struct picoboot_cmd cmd;
//     // shouldn't be necessary any more
//     // addr |= 1u; // Thumb bit
//     //if (verbose) output("EXEC2 %08x\n", (unsigned int) exec2_cmd->scan_base);
//     cmd.bCmdId = PC_EXEC2;
//     cmd.bCmdSize = sizeof(cmd.exec2_cmd);
//     cmd.dTransferLength = 0;
//     cmd.exec2_cmd = *exec2_cmd;
//     return picoboot_cmd(usb_device, &cmd, NULL, 0);
// } // currently unused


int picoboot_flash_erase(libusb_device_handle *usb_device, uint32_t addr, uint32_t len) {
    struct picoboot_cmd cmd;
    if (verbose) output("FLASH_ERASE %08x+%08x\n", (unsigned int) addr, (unsigned int) len);
    cmd.bCmdId = PC_FLASH_ERASE;
    cmd.bCmdSize = sizeof(cmd.range_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.range_cmd.dSize = len;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_vector(libusb_device_handle *usb_device, uint32_t addr) {
    struct picoboot_cmd cmd;
    if (verbose) output("VECTOR %08x\n", (unsigned int) addr);
    cmd.bCmdId = PC_VECTORIZE_FLASH;
    cmd.bCmdSize = sizeof(cmd.address_only_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.dTransferLength = 0;
    return picoboot_cmd(usb_device, &cmd, NULL, 0);
}

int picoboot_write(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len) {
    struct picoboot_cmd cmd;
    if (verbose) output("WRITE %08x+%08x\n", (unsigned int) addr, (unsigned int) len);
    cmd.bCmdId = PC_WRITE;
    cmd.bCmdSize = sizeof(cmd.range_cmd);
    cmd.range_cmd.dAddr = addr;
    cmd.range_cmd.dSize = cmd.dTransferLength = len;
    return picoboot_cmd(usb_device, &cmd, buffer, len);
}

int picoboot_read(libusb_device_handle *usb_device, uint32_t addr, uint8_t *buffer, uint32_t len) {
    memset(buffer, 0xaa, len);
    if (verbose) output("READ %08x+%08x\n", (unsigned int) addr, (unsigned int) len);
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

int picoboot_otp_write(libusb_device_handle *usb_device, struct picoboot_otp_cmd *otp_cmd, uint8_t *buffer, uint32_t len) {
    struct picoboot_cmd cmd;
    if (verbose) output("OTP WRITE %04x+%08x ecc=%d\n", (unsigned int) otp_cmd->wRow, otp_cmd->wRowCount, otp_cmd->bEcc);
    cmd.bCmdId = PC_OTP_WRITE;
#ifdef _MSC_VER
    cmd.bCmdSize = 5;   // for some reason with MSVC, and only with picoboot_otp_cmd, the size is 6 not 5??
#else
    cmd.bCmdSize = sizeof(cmd.otp_cmd);
#endif
    cmd.otp_cmd = *otp_cmd;
    cmd.dTransferLength = len;
    one_time_bulk_timeout = 5000 + len * 5;
    return picoboot_cmd(usb_device, &cmd, buffer, len);
}

int picoboot_otp_read(libusb_device_handle *usb_device, struct picoboot_otp_cmd *otp_cmd, uint8_t *buffer, uint32_t len) {
    struct picoboot_cmd cmd;
    if (verbose) output("OTP READ %04x+%08x ecc=%d\n", (unsigned int) otp_cmd->wRow, otp_cmd->wRowCount, otp_cmd->bEcc);
    cmd.bCmdId = PC_OTP_READ;
#ifdef _MSC_VER
    cmd.bCmdSize = 5;   // for some reason with MSVC, and only with picoboot_otp_cmd, the size is 6 not 5??
#else
    cmd.bCmdSize = sizeof(cmd.otp_cmd);
#endif
    cmd.otp_cmd = *otp_cmd;
    cmd.dTransferLength = len;
    return picoboot_cmd(usb_device, &cmd, buffer, len);
}

int picoboot_get_info(libusb_device_handle *usb_device, struct picoboot_get_info_cmd *get_info_cmd, uint8_t *buffer, uint32_t len) {
    if (verbose) output("GET_INFO\n");
    struct picoboot_cmd cmd;
    cmd.bCmdId = PC_GET_INFO;
    cmd.bCmdSize = sizeof(cmd.get_info_cmd);
    cmd.get_info_cmd = *get_info_cmd;
    cmd.dTransferLength = len;
    int ret = picoboot_cmd(usb_device, &cmd, buffer, len);
    return ret;
}

#if 1
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
#define PICOBOOT_POKE_CMD_PROG_SIZE (size_t)(8 + 8)

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
#define PICOBOOT_PEEK_CMD_PROG_SIZE (size_t)(12 + 4)

// todo - compile this - currently taken from github PR #86
static const size_t picoboot_flash_id_cmd_len = 152;
static const uint8_t picoboot_flash_id_cmd[] = {
    // void flash_get_unique_id(void)
    0x02, 0xa0, 0x06, 0xa1, 0x00, 0x4a, 0x11, 0xe0, 
    // int buflen
    0x0d, 0x00, 0x00, 0x00,
    // char txbuf[13]
    0x4b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // char rxbuf[13]
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // void flash_do_cmd(txbuf, rxbuf, buflen)
    0x80, 0x23, 0xf0, 0xb5, 0x17, 0x4e, 0x9b, 0x00,
    0x34, 0x68, 0x63, 0x40, 0xc0, 0x24, 0xa4, 0x00,
    0x23, 0x40, 0x15, 0x4c, 0x23, 0x60, 0xc0, 0x24,
    0x13, 0x00, 0x64, 0x05, 0x17, 0x00, 0x1f, 0x43,
    0x06, 0xd1, 0xc0, 0x23, 0x32, 0x68, 0x9b, 0x00,
    0x93, 0x43, 0x0f, 0x4a, 0x13, 0x60, 0xf0, 0xbd,
    0x08, 0x25, 0xa7, 0x6a, 0x3d, 0x40, 0xac, 0x46,
    0x02, 0x25, 0x2f, 0x42, 0x08, 0xd0, 0x00, 0x2a,
    0x06, 0xd0, 0x9f, 0x1a, 0x0d, 0x2f, 0x03, 0xd8,
    0x07, 0x78, 0x01, 0x3a, 0x27, 0x66, 0x01, 0x30,
    0x65, 0x46, 0x00, 0x2d, 0xe2, 0xd0, 0x00, 0x2b,
    0xe0, 0xd0, 0x27, 0x6e, 0x01, 0x3b, 0x0f, 0x70,
    0x01, 0x31, 0xdb, 0xe7, 0x0c, 0x80, 0x01, 0x40,
    0x0c, 0x90, 0x01, 0x40,
};
#define PICOBOOT_FLASH_ID_CMD_PROG_SIZE (size_t)(152)

// TODO better place for this e.g. the USB DPRAM location the controller has already put it in
#define PEEK_POKE_CODE_LOC 0x20000000u

#define FLASH_ID_CODE_LOC 0x15000000 // XIP_SRAM_BASE on RP2040, as we're not using XIP so probably fine
#define FLASH_ID_UID_ADDR (FLASH_ID_CODE_LOC + 28 + 1 + 4)

int picoboot_poke(libusb_device_handle *usb_device, uint32_t addr, uint32_t data) {
    uint8_t prog[PICOBOOT_POKE_CMD_PROG_SIZE];
    output("POKE (D)%08x -> (A)%08x\n", data, addr);
    memcpy(prog, picoboot_poke_cmd, picoboot_poke_cmd_len);
    *(uint32_t *) (prog + picoboot_poke_cmd_len) = data;
    *(uint32_t *) (prog + picoboot_poke_cmd_len + 4) = addr;

    int ret = picoboot_write(usb_device, PEEK_POKE_CODE_LOC, prog, PICOBOOT_POKE_CMD_PROG_SIZE);
    if (ret)
        return ret;
    return picoboot_exec(usb_device, PEEK_POKE_CODE_LOC);
}

// TODO haven't checked the store goes to the right address :)
int picoboot_peek(libusb_device_handle *usb_device, uint32_t addr, uint32_t *data) {
    uint8_t prog[PICOBOOT_PEEK_CMD_PROG_SIZE];
    output("PEEK %08x\n", addr);
    memcpy(prog, picoboot_peek_cmd, picoboot_peek_cmd_len);
    *(uint32_t *) (prog + picoboot_peek_cmd_len) = addr;

    int ret = picoboot_write(usb_device, PEEK_POKE_CODE_LOC, prog, PICOBOOT_PEEK_CMD_PROG_SIZE);
    if (ret)
        return ret;
    ret = picoboot_exec(usb_device, PEEK_POKE_CODE_LOC);
    if (ret)
        return ret;
    return picoboot_read(usb_device, PEEK_POKE_CODE_LOC + picoboot_peek_cmd_len, (uint8_t *) data, sizeof(uint32_t));
}

int picoboot_flash_id(libusb_device_handle *usb_device, uint64_t *data) {
    picoboot_exclusive_access(usb_device, 1);
    uint8_t prog[PICOBOOT_FLASH_ID_CMD_PROG_SIZE];
    uint64_t id;
    output("GET FLASH ID\n");
    memcpy(prog, picoboot_flash_id_cmd, picoboot_flash_id_cmd_len);

    // ensure XIP is exited before executing
    int ret = picoboot_exit_xip(usb_device);
    if (ret) 
        goto flash_id_return;
    ret = picoboot_write(usb_device, FLASH_ID_CODE_LOC, prog, PICOBOOT_FLASH_ID_CMD_PROG_SIZE);
    if (ret)
        goto flash_id_return;
    ret = picoboot_exec(usb_device, FLASH_ID_CODE_LOC);
    if (ret)
        goto flash_id_return;
    ret = picoboot_read(usb_device, FLASH_ID_UID_ADDR, (uint8_t *) &id, sizeof(uint64_t));
    *data = (((id & 0x00000000000000FF) << 56) |
            ((id & 0x000000000000FF00) << 40) |
            ((id & 0x0000000000FF0000) << 24) |
            ((id & 0x00000000FF000000) <<  8) |
            ((id & 0x000000FF00000000) >>  8) |
            ((id & 0x0000FF0000000000) >> 24) |
            ((id & 0x00FF000000000000) >> 40) |
            ((id & 0xFF00000000000000) >> 56));

flash_id_return:
    picoboot_exclusive_access(usb_device, 0);
    return ret;
}
#endif