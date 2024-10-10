// Hardware interface to USB on SAM3X
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // NULL
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/usb_cdc.h" // usb_notify_ep0
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "internal.h" // UOTGHS
#include "sched.h" // DECL_INIT

#if CONFIG_MACH_SAME70
#include "same70_usb.h" // Fixes for upstream header changes
#define CFG_UOTGHS_CTRL (UOTGHS_CTRL_UIMOD | UOTGHS_CTRL_USBE)
#else
#define CFG_UOTGHS_CTRL (UOTGHS_CTRL_UIMOD | UOTGHS_CTRL_OTGPADE | \
                         UOTGHS_CTRL_USBE)
#endif

#define EP_SIZE(s) ((s)==64 ? UOTGHS_DEVEPTCFG_EPSIZE_64_BYTE :         \
                    ((s)==32 ? UOTGHS_DEVEPTCFG_EPSIZE_32_BYTE :        \
                     ((s)==16 ? UOTGHS_DEVEPTCFG_EPSIZE_16_BYTE :       \
                      UOTGHS_DEVEPTCFG_EPSIZE_8_BYTE)))

#define usb_fifo(ep) ((void*)UOTGHS_RAM_ADDR + ((ep) * 0x8000))

static void
usb_write_packet(uint32_t ep, const uint8_t *data, uint32_t len)
{
    uint8_t *dest = usb_fifo(ep);
    while (len--)
        *dest++ = *data++;
    __DMB();
}

static void
usb_read_packet(uint32_t ep, uint8_t *data, uint32_t len)
{
    uint8_t *src = usb_fifo(ep);
    while (len--)
        *data++ = *src++;
}

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    uint32_t eps = UOTGHS->UOTGHS_DEVEPTISR[USB_CDC_EP_BULK_OUT];
    if (!(eps & UOTGHS_DEVEPTISR_RXOUTI)) {
        // No data ready
        UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << USB_CDC_EP_BULK_OUT;
        return -1;
    }
    uint32_t len = (eps&UOTGHS_DEVEPTISR_BYCT_Msk) >> UOTGHS_DEVEPTISR_BYCT_Pos;
    usb_read_packet(USB_CDC_EP_BULK_OUT, data, len > max_len ? max_len : len);
    UOTGHS->UOTGHS_DEVEPTICR[USB_CDC_EP_BULK_OUT] = UOTGHS_DEVEPTICR_RXOUTIC;
    UOTGHS->UOTGHS_DEVEPTIDR[USB_CDC_EP_BULK_OUT] = UOTGHS_DEVEPTIDR_FIFOCONC;
    return len;
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    uint32_t eps = UOTGHS->UOTGHS_DEVEPTISR[USB_CDC_EP_BULK_IN];
    if (!(eps & UOTGHS_DEVEPTISR_TXINI)) {
        // Buffer full
        UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << USB_CDC_EP_BULK_IN;
        return -1;
    }
    usb_write_packet(USB_CDC_EP_BULK_IN, data, len);
    UOTGHS->UOTGHS_DEVEPTICR[USB_CDC_EP_BULK_IN] = UOTGHS_DEVEPTICR_TXINIC;
    UOTGHS->UOTGHS_DEVEPTIDR[USB_CDC_EP_BULK_IN] = UOTGHS_DEVEPTIDR_FIFOCONC;
    return len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    uint32_t eps = UOTGHS->UOTGHS_DEVEPTISR[0];
    if (eps & UOTGHS_DEVEPTISR_RXSTPI)
        return -2;
    if (!(eps & UOTGHS_DEVEPTISR_RXOUTI)) {
        // Not ready to receive data
        UOTGHS->UOTGHS_DEVEPTIER[0] = UOTGHS_DEVEPTIER_RXOUTES;
        UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << 0;
        return -1;
    }
    usb_read_packet(0, data, max_len);
    if (UOTGHS->UOTGHS_DEVEPTISR[0] & UOTGHS_DEVEPTISR_RXSTPI)
        return -2;
    UOTGHS->UOTGHS_DEVEPTICR[0] = UOTGHS_DEVEPTICR_RXOUTIC;
    return max_len;
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    uint32_t eps = UOTGHS->UOTGHS_DEVEPTISR[0];
    if (!(eps & UOTGHS_DEVEPTISR_RXSTPI)) {
        // No data ready to read
        UOTGHS->UOTGHS_DEVEPTIDR[0] = (UOTGHS_DEVEPTIDR_TXINEC
                                       | UOTGHS_DEVEPTIDR_RXOUTEC);
        UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << 0;
        return -1;
    }
    usb_read_packet(0, data, max_len);
    UOTGHS->UOTGHS_DEVEPTICR[0] = (UOTGHS_DEVEPTICR_RXSTPIC
                                   | UOTGHS_DEVEPTICR_RXOUTIC);
    return max_len;
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    uint32_t eps = UOTGHS->UOTGHS_DEVEPTISR[0];
    if (eps & (UOTGHS_DEVEPTISR_RXSTPI | UOTGHS_DEVEPTISR_RXOUTI))
        return -2;
    if (!(eps & UOTGHS_DEVEPTISR_TXINI)) {
        // Not ready to send
        UOTGHS->UOTGHS_DEVEPTIER[0] = (UOTGHS_DEVEPTIER_TXINES
                                       | UOTGHS_DEVEPTIER_RXOUTES);
        UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << 0;
        return -1;
    }
    usb_write_packet(0, data, len);
    UOTGHS->UOTGHS_DEVEPTICR[0] = UOTGHS_DEVEPTICR_TXINIC;
    return len;
}

void
usb_stall_ep0(void)
{
    UOTGHS->UOTGHS_DEVEPTIER[0] = UOTGHS_DEVEPTIER_STALLRQS;
    UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << 0;
}

static uint8_t set_address;

void
usb_set_address(uint_fast8_t addr)
{
    set_address = addr | UOTGHS_DEVCTRL_ADDEN;
    usb_send_ep0(NULL, 0);
    UOTGHS->UOTGHS_DEVEPTIER[0] = UOTGHS_DEVEPTIER_TXINES;
    UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << 0;
}

void
usb_set_configure(void)
{
    UOTGHS->UOTGHS_DEVEPT = ((UOTGHS_DEVEPT_EPEN0 << 0)
                             | (UOTGHS_DEVEPT_EPEN0 << USB_CDC_EP_BULK_OUT)
                             | (UOTGHS_DEVEPT_EPEN0 << USB_CDC_EP_BULK_IN)
                             | (UOTGHS_DEVEPT_EPEN0 << USB_CDC_EP_ACM));

    UOTGHS->UOTGHS_DEVEPTCFG[USB_CDC_EP_BULK_OUT] = (
        EP_SIZE(USB_CDC_EP_BULK_OUT_SIZE)
        | UOTGHS_DEVEPTCFG_EPTYPE_BLK | UOTGHS_DEVEPTCFG_EPBK_2_BANK
        | UOTGHS_DEVEPTCFG_ALLOC);
    UOTGHS->UOTGHS_DEVEPTIER[USB_CDC_EP_BULK_OUT] = UOTGHS_DEVEPTIER_RXOUTES;
    UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << USB_CDC_EP_BULK_OUT;

    UOTGHS->UOTGHS_DEVEPTCFG[USB_CDC_EP_BULK_IN] = (
        EP_SIZE(USB_CDC_EP_BULK_IN_SIZE) | UOTGHS_DEVEPTCFG_EPDIR_IN
        | UOTGHS_DEVEPTCFG_EPTYPE_BLK | UOTGHS_DEVEPTCFG_EPBK_2_BANK
        | UOTGHS_DEVEPTCFG_ALLOC);
    UOTGHS->UOTGHS_DEVEPTIER[USB_CDC_EP_BULK_IN] = UOTGHS_DEVEPTIER_TXINES;
    UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << USB_CDC_EP_BULK_IN;

    UOTGHS->UOTGHS_DEVEPTCFG[USB_CDC_EP_ACM] = (
        EP_SIZE(USB_CDC_EP_ACM_SIZE) | UOTGHS_DEVEPTCFG_EPDIR_IN
        | UOTGHS_DEVEPTCFG_EPTYPE_INTRPT | UOTGHS_DEVEPTCFG_EPBK_2_BANK
        | UOTGHS_DEVEPTCFG_ALLOC);
}

// Configure endpoint 0 after usb reset completes
static void
handle_end_reset(void)
{
    UOTGHS->UOTGHS_DEVEPT = UOTGHS_DEVEPT_EPEN0 << 0;
    UOTGHS->UOTGHS_DEVEPTCFG[0] = (
        EP_SIZE(USB_CDC_EP0_SIZE)
        | UOTGHS_DEVEPTCFG_EPTYPE_CTRL | UOTGHS_DEVEPTCFG_EPBK_1_BANK
        | UOTGHS_DEVEPTCFG_ALLOC);
    UOTGHS->UOTGHS_DEVEPTIER[0] = UOTGHS_DEVEPTIER_RXSTPES;
    UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_PEP_0 << 0;

    UOTGHS->UOTGHS_DEVICR = UOTGHS_DEVICR_EORSTC;
}

void
UOTGHS_Handler(void)
{
    uint32_t s = UOTGHS->UOTGHS_DEVISR;
    if (s & UOTGHS_DEVISR_EORST) {
        handle_end_reset();
        return;
    }
    UOTGHS->UOTGHS_DEVIDR = s;
    if (s & (UOTGHS_DEVISR_PEP_0 << 0)) {
        usb_notify_ep0();

        if (set_address
            && (UOTGHS->UOTGHS_DEVEPTISR[0] & UOTGHS_DEVEPTISR_TXINI)) {
            // Ack from set_address command sent - now update address
            UOTGHS->UOTGHS_DEVCTRL = (set_address
                                      | UOTGHS_DEVCTRL_SPDCONF_FORCED_FS);
            set_address = 0;
        }
    }
    if (s & (UOTGHS_DEVISR_PEP_0 << USB_CDC_EP_BULK_OUT))
        usb_notify_bulk_out();
    if (s & (UOTGHS_DEVISR_PEP_0 << USB_CDC_EP_BULK_IN))
        usb_notify_bulk_in();
}

void
usbserial_init(void)
{
    // Setup USB clock
    enable_pclock(ID_UOTGHS);
    PMC->CKGR_UCKR = CKGR_UCKR_UPLLCOUNT(3) | CKGR_UCKR_UPLLEN;
    while (!(PMC->PMC_SR & PMC_SR_LOCKU))
        ;

    // Enable USB
    UOTGHS->UOTGHS_CTRL = CFG_UOTGHS_CTRL;
    UOTGHS->UOTGHS_DEVCTRL = UOTGHS_DEVCTRL_SPDCONF_FORCED_FS;

    // Enable interrupts
    armcm_enable_irq(UOTGHS_Handler, UOTGHS_IRQn, 1);
    UOTGHS->UOTGHS_DEVIER = UOTGHS_DEVIER_EORSTES;
}
DECL_INIT(usbserial_init);
