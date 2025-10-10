// Support for extracting the hardware chip id on lpc176x
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_USB_SERIAL_NUMBER_CHIPID
#include "generic/irq.h" // irq_disable
#include "generic/usb_cdc.h" // usb_fill_serial
#include "generic/usbstd.h" // usb_string_descriptor
#include "sched.h" // DECL_INIT

// IAP interface
#define IAP_LOCATION        0x1fff1ff1
#define IAP_CMD_READ_UID    58
#define IAP_UID_LEN         16
typedef void (*IAP)(uint32_t *, uint32_t *);

static struct {
    struct usb_string_descriptor desc;
    uint16_t data[IAP_UID_LEN * 2];
} cdc_chipid;

struct usb_string_descriptor *
usbserial_get_serialid(void)
{
   return &cdc_chipid.desc;
}

void
chipid_init(void)
{
    if (!CONFIG_USB_SERIAL_NUMBER_CHIPID)
        return;

    uint32_t iap_cmd_uid[5] = {IAP_CMD_READ_UID, 0, 0, 0, 0};
    uint32_t iap_resp[5];
    IAP iap_entry = (IAP)IAP_LOCATION;
    irq_disable();
    iap_entry(iap_cmd_uid, iap_resp);
    irq_enable();

    usb_fill_serial(&cdc_chipid.desc, ARRAY_SIZE(cdc_chipid.data)
                    , &iap_resp[1]);
}
DECL_INIT(chipid_init);
