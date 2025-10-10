// Support for extracting the hardware chip id on rp2040
//
// Copyright (C) 2019-2024  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_USB_SERIAL_NUMBER_CHIPID
#include "board/canserial.h" // canserial_set_uuid
#include "generic/usb_cdc.h" // usb_fill_serial
#include "generic/usbstd.h" // usb_string_descriptor
#include "internal.h" // bootrom_read_unique_id
#include "sched.h" // DECL_INIT

#define CHIP_UID_LEN 8

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
    if (!(CONFIG_USB_SERIAL_NUMBER_CHIPID || CONFIG_CANBUS))
        return;

    uint8_t data[CHIP_UID_LEN] = {};
    bootrom_read_unique_id(data, sizeof(data));

    if (CONFIG_USB_SERIAL_NUMBER_CHIPID)
        usb_fill_serial(&cdc_chipid.desc, ARRAY_SIZE(cdc_chipid.data), data);
    if (CONFIG_CANBUS)
        canserial_set_uuid(data, CHIP_UID_LEN);
}
DECL_INIT(chipid_init);
