// Support for extracting the hardware chip id on atsamd
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_USB_SERIAL_NUMBER_CHIPID
#include "generic/canserial.h" // canserial_set_uuid
#include "generic/usb_cdc.h" // usb_fill_serial
#include "generic/usbstd.h" // usb_string_descriptor
#include "sched.h" // DECL_INIT

#define CHIP_UID_LEN 16

static struct {
    struct usb_string_descriptor desc;
    uint16_t data[CHIP_UID_LEN * 2];
} cdc_chipid;

struct usb_string_descriptor *
usbserial_get_serialid(void)
{
   return &cdc_chipid.desc;
}

void
chipid_init(void)
{
    if (!CONFIG_USB_SERIAL_NUMBER_CHIPID && !CONFIG_CANBUS)
        return;

    uint32_t id[4];
    if (CONFIG_MACH_SAMX2) {
        id[0] = *(uint32_t*)0x0080A00C;
        id[1] = *(uint32_t*)0x0080A040;
        id[2] = *(uint32_t*)0x0080A044;
        id[3] = *(uint32_t*)0x0080A048;
    } else {
        id[0] = *(uint32_t*)0x008061FC;
        id[1] = *(uint32_t*)0x00806010;
        id[2] = *(uint32_t*)0x00806014;
        id[3] = *(uint32_t*)0x00806018;
    }

    if (CONFIG_USB_SERIAL_NUMBER_CHIPID)
        usb_fill_serial(&cdc_chipid.desc, ARRAY_SIZE(cdc_chipid.data), id);

    if (CONFIG_CANBUS)
        canserial_set_uuid((void*)id, CHIP_UID_LEN);
}
DECL_INIT(chipid_init);
