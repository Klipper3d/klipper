// Hardware interface to SAM4 USB Device Port
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // NULL
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/irq.h" // irq_disable
#include "board/usb_cdc.h" // usb_notify_ep0
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // UDP
#include "sched.h" // DECL_INIT

#define CSR_EP0 (UDP_CSR_EPTYPE_CTRL | UDP_CSR_EPEDS)
#define CSR_ACM (UDP_CSR_EPTYPE_INT_IN | UDP_CSR_EPEDS)
#define CSR_BULK_OUT (UDP_CSR_EPTYPE_BULK_OUT | UDP_CSR_EPEDS)
#define CSR_BULK_IN (UDP_CSR_EPTYPE_BULK_IN | UDP_CSR_EPEDS)

static void
usb_write_packet(uint32_t ep, const uint8_t *data, uint32_t len)
{
    while (len--)
        UDP->UDP_FDR[ep] = *data++;
}

static uint32_t
usb_read_packet(uint32_t ep, uint32_t csr, uint8_t *data, uint32_t max_len)
{
    uint32_t pk_len = (csr & UDP_CSR_RXBYTECNT_Msk) >> UDP_CSR_RXBYTECNT_Pos;
    uint32_t len = pk_len < max_len ? pk_len : max_len, orig_len = len;
    while (len--)
        *data++ = UDP->UDP_FDR[ep];
    return orig_len;
}

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    static uint32_t next_bk = UDP_CSR_RX_DATA_BK0;
    const uint32_t other_irqs = (UDP_CSR_RXSETUP | UDP_CSR_STALLSENT
                                 | UDP_CSR_TXCOMP);
    uint32_t csr = UDP->UDP_CSR[USB_CDC_EP_BULK_OUT];
    uint32_t bk = csr & (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
    if (!bk) {
        // Not ready to receive data
        if (csr & other_irqs)
            UDP->UDP_CSR[USB_CDC_EP_BULK_OUT] = (
                CSR_BULK_OUT | UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
        UDP->UDP_IER = 1<<USB_CDC_EP_BULK_OUT;
        return -1;
    }
    uint32_t len = usb_read_packet(USB_CDC_EP_BULK_OUT, csr, data, max_len);
    if (bk == (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1))
        bk = next_bk;
    next_bk = bk ^ (UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
    UDP->UDP_CSR[USB_CDC_EP_BULK_OUT] = CSR_BULK_OUT | next_bk;
    return len;
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    const uint32_t other_irqs = (UDP_CSR_RXSETUP | UDP_CSR_STALLSENT
                                 | UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
    uint32_t csr = UDP->UDP_CSR[USB_CDC_EP_BULK_IN];
    if (csr & UDP_CSR_TXPKTRDY) {
        // Not ready to send
        if (csr & other_irqs)
            UDP->UDP_CSR[USB_CDC_EP_BULK_IN] = CSR_BULK_IN | UDP_CSR_TXCOMP;
        UDP->UDP_IER = 1<<USB_CDC_EP_BULK_IN;
        return -1;
    }
    usb_write_packet(USB_CDC_EP_BULK_IN, data, len);
    UDP->UDP_CSR[USB_CDC_EP_BULK_IN] = CSR_BULK_IN | UDP_CSR_TXPKTRDY;
    return len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    const uint32_t other_irqs = (UDP_CSR_RXSETUP | UDP_CSR_STALLSENT
                                 | UDP_CSR_TXCOMP | UDP_CSR_RX_DATA_BK1);
    uint32_t csr = UDP->UDP_CSR[0];
    if (csr & other_irqs)
        return -2;
    if (!(csr & UDP_CSR_RX_DATA_BK0)) {
        // Not ready to receive data
        UDP->UDP_IER = 1<<0;
        return -1;
    }
    uint32_t len = usb_read_packet(0, csr, data, max_len);
    if (UDP->UDP_CSR[0] & other_irqs)
        return -2;
    UDP->UDP_CSR[0] = CSR_EP0 | other_irqs;
    return len;
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    const uint32_t other_irqs = (UDP_CSR_STALLSENT | UDP_CSR_TXCOMP
                                 | UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
    uint32_t csr = UDP->UDP_CSR[0];
    if (!(csr & UDP_CSR_RXSETUP)) {
        // No data ready to read
        if (csr & other_irqs)
            UDP->UDP_CSR[0] = CSR_EP0 | UDP_CSR_RXSETUP;
        UDP->UDP_IER = 1<<0;
        return -1;
    }
    uint32_t len = usb_read_packet(0, csr, data, max_len);
    uint32_t dir = *(uint8_t*)data & 0x80 ? UDP_CSR_DIR : 0;
    UDP->UDP_CSR[0] = CSR_EP0 | dir;
    return len;
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    const uint32_t other_irqs = (UDP_CSR_RXSETUP | UDP_CSR_STALLSENT
                                 | UDP_CSR_RX_DATA_BK0 | UDP_CSR_RX_DATA_BK1);
    uint32_t csr = UDP->UDP_CSR[0];
    if (csr & other_irqs)
        return -2;
    if (csr & UDP_CSR_TXPKTRDY) {
        // Not ready to send
        UDP->UDP_IER = 1<<0;
        return -1;
    }
    usb_write_packet(0, data, len);
    uint32_t dir = csr & UDP_CSR_DIR;
    UDP->UDP_CSR[0] = CSR_EP0 | dir | UDP_CSR_TXPKTRDY | other_irqs;
    return len;
}

void
usb_stall_ep0(void)
{
    UDP->UDP_CSR[0] = CSR_EP0 | UDP_CSR_FORCESTALL;
    UDP->UDP_IER = 1<<0;
}

static uint32_t set_address;

void
usb_set_address(uint_fast8_t addr)
{
    set_address = addr | UDP_FADDR_FEN;
    usb_send_ep0(NULL, 0);
    UDP->UDP_IER = 1<<0;
}

void
usb_set_configure(void)
{
    UDP->UDP_CSR[USB_CDC_EP_ACM] = CSR_ACM;
    UDP->UDP_CSR[USB_CDC_EP_BULK_OUT] = CSR_BULK_OUT;
    UDP->UDP_CSR[USB_CDC_EP_BULK_IN] = CSR_BULK_IN;
    UDP->UDP_GLB_STAT |= UDP_GLB_STAT_CONFG;
}

// Configure endpoint 0 after usb reset completes
static void
handle_end_reset(void)
{
    UDP->UDP_ICR = UDP_ISR_ENDBUSRES;

    UDP->UDP_CSR[0] = CSR_EP0;
    UDP->UDP_IER = 1<<0;

    UDP->UDP_TXVC = UDP_TXVC_PUON;
}

void
UDP_Handler(void)
{
    uint32_t s = UDP->UDP_ISR;
    UDP->UDP_IDR = s;
    if (s & UDP_ISR_ENDBUSRES)
        handle_end_reset();
    if (s & UDP_ISR_RXRSM)
        UDP->UDP_ICR = UDP_ISR_RXRSM;
    if (s & (1<<0)) {
        usb_notify_ep0();

        if (set_address && UDP->UDP_CSR[0] & UDP_CSR_TXCOMP) {
            // Ack from set_address command sent - now update address
            UDP->UDP_FADDR = set_address;
            UDP->UDP_GLB_STAT |= UDP_GLB_STAT_FADDEN;
            set_address = 0;
        }
    }
    if (s & (1<<USB_CDC_EP_BULK_OUT))
        usb_notify_bulk_out();
    if (s & (1<<USB_CDC_EP_BULK_IN))
        usb_notify_bulk_in();
}

DECL_CONSTANT_STR("RESERVE_PINS_USB", "PB10,PB11");

void
usbserial_init(void)
{
    // Enable clocks
    enable_pclock(ID_UDP);
    PMC->PMC_USB = PMC_USB_USBDIV(5 - 1); // PLLA=240Mhz; divide by 5 for 48Mhz
    PMC->PMC_SCER = PMC_SCER_UDP;

    // Enable USB pullup
    UDP->UDP_TXVC = UDP_TXVC_PUON | UDP_TXVC_TXVDIS;

    // Enable interrupts
    UDP->UDP_ICR = 0xffffffff;
    armcm_enable_irq(UDP_Handler, UDP_IRQn, 1);
}
DECL_INIT(usbserial_init);
