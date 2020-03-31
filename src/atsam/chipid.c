// Support for extracting the hardware chip id on sam3/sam4
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "generic/irq.h" // irq_disable
#include "generic/usb_cdc.h" // usb_fill_serial
#include "generic/usbstd.h" // usb_string_descriptor
#include "internal.h" // EFC0
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

// Compatibility definitions for sam4e8e
#ifndef EFC0
#define EFC0 EFC
#define IFLASH0_ADDR IFLASH_ADDR
#endif

void noinline __section(".ramfunc.read_chip_id")
read_chip_id(uint32_t *id)
{
    // Workaround sam3 errata
    uint32_t fmr = EFC0->EEFC_FMR;
    EFC0->EEFC_FMR = fmr | EEFC_FMR_SCOD;

    // Send the STUI command
    while (!(EFC0->EEFC_FSR & EEFC_FSR_FRDY))
        ;
    EFC0->EEFC_FCR = EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_STUI;
    while (EFC0->EEFC_FSR & EEFC_FSR_FRDY)
        ;

    // Copy the id
    id[0] = *(uint32_t*)(IFLASH0_ADDR + 0x00);
    id[1] = *(uint32_t*)(IFLASH0_ADDR + 0x04);
    id[2] = *(uint32_t*)(IFLASH0_ADDR + 0x08);
    id[3] = *(uint32_t*)(IFLASH0_ADDR + 0x0c);

    // Send the SPUI command
    EFC0->EEFC_FCR = EEFC_FCR_FKEY_PASSWD | EEFC_FCR_FCMD_SPUI;
    while (!(EFC0->EEFC_FSR & EEFC_FSR_FRDY))
        ;

    // Restore fmr
    EFC0->EEFC_FMR = fmr;
}

void
chipid_init(void)
{
    if (!CONFIG_USB_SERIAL_NUMBER_CHIPID)
        return;

    uint32_t id[4];
    irq_disable();
    read_chip_id(id);
    irq_enable();

    usb_fill_serial(&cdc_chipid.desc, ARRAY_SIZE(cdc_chipid.data), id);
}
DECL_INIT(chipid_init);
