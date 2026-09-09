// GD32F303 unique-id support for USB CDC and CAN node UUIDs.
//
// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h"
#if CONFIG_CANBUS
#include "generic/canserial.h"
#endif
#if CONFIG_USBSERIAL
#include "generic/usb_cdc.h"
#include "generic/usbstd.h"
#endif
#include "sched.h"

#define GD32F303_UID_BASE ((void *)0x1ffff7e8U)
#define CHIP_UID_LEN 12

#if CONFIG_USBSERIAL
static struct {
    struct usb_string_descriptor desc;
    uint16_t data[CHIP_UID_LEN * 2];
} cdc_chipid;

struct usb_string_descriptor *
usbserial_get_serialid(void)
{
    return &cdc_chipid.desc;
}
#endif

void
chipid_init(void)
{
#if CONFIG_USBSERIAL
    if (CONFIG_USB_SERIAL_NUMBER_CHIPID)
        usb_fill_serial(&cdc_chipid.desc, ARRAY_SIZE(cdc_chipid.data),
                        GD32F303_UID_BASE);
#endif
#if CONFIG_CANBUS
    canserial_set_uuid(GD32F303_UID_BASE, CHIP_UID_LEN);
#endif
}
DECL_INIT(chipid_init);
