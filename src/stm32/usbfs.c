// Hardware interface to "fullspeed USB controller"
//
// Copyright (C) 2018-2023  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // NULL
#include "board/armcm_boot.h" // armcm_enable_irq
#include "board/armcm_timer.h" // udelay
#include "board/gpio.h" // gpio_out_setup
#include "board/io.h" // writeb
#include "board/usb_cdc.h" // usb_notify_ep0
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // GPIO
#include "sched.h" // DECL_INIT

#if CONFIG_MACH_STM32F1 || CONFIG_MACH_STM32G4
  // Transfer memory is accessed with 32bits, but contains only 16bits of data
  typedef volatile uint32_t epmword_t;
  #define WSIZE 2
  #define USBx_IRQn USB_LP_IRQn
#elif CONFIG_MACH_STM32F0 || CONFIG_MACH_STM32L4
  // Transfer memory is accessed with 16bits and contains 16bits of data
  typedef volatile uint16_t epmword_t;
  #define WSIZE 2
  #define USBx_IRQn USB_IRQn
#elif CONFIG_MACH_STM32G0
  // Transfer memory is accessed with 32bits and contains 32bits of data
  typedef volatile uint32_t epmword_t;
  #define WSIZE 4
  #define USBx_IRQn USB_IRQn
#endif

// The stm32g0 has slightly different register names
#if CONFIG_MACH_STM32G0
  #if CONFIG_MACH_STM32G0B1
    #define USB_IRQn USB_UCPD1_2_IRQn
  #endif
  #define USB USB_DRD_FS
  #define USB_PMAADDR USB_DRD_PMAADDR
  #define USB_EPADDR_FIELD USB_CHEP_ADDR
  #define USB_EP_CTR_RX USB_EP_VTRX
  #define USB_EP_CTR_TX USB_EP_VTTX
  #define USB_EPRX_STAT USB_EP_RX_STRX
  #define USB_EPTX_STAT USB_EP_TX_STTX
  #define USB_ISTR_EP_ID USB_ISTR_IDN
  #define USB_CNTR_FRES USB_CNTR_USBRST
#endif

// Some chip variants do not define these fields
#ifndef USB_EP_DTOG_TX_Pos
#define USB_EP_DTOG_TX_Pos 6
#define USB_EP_DTOG_RX_Pos 14
#endif


/****************************************************************
 * USB transfer memory
 ****************************************************************/

// Layout of the USB transfer memory
#define EPM ((epmword_t*)USB_PMAADDR)
#define EPM_EP_DESC(ep, bufnum) (&EPM[((ep)*2 + (bufnum)) * (4 / WSIZE)])
#define EPM_BUF_OFFSET 0x10
#define EPM_EP_BUF_SIZE (64 / WSIZE + 1)
#define EPM_EP_BUF(ep, bufnum)                                          \
    (&EPM[EPM_BUF_OFFSET + ((ep)*2 + (bufnum)) * EPM_EP_BUF_SIZE])
#define BUFTX 0
#define BUFRX 1

// Configure the usb descriptor for an endpoint
static void
epm_ep_desc_setup(int ep, int bufnum, int rx_size)
{
    uint32_t addr = (EPM_EP_BUF(ep, bufnum) - EPM) * WSIZE;
    uint32_t count_rx = (rx_size <= 30 ? DIV_ROUND_UP(rx_size, 2) << 10
                         : ((DIV_ROUND_UP(rx_size, 32) - 1) << 10) | 0x8000);
    epmword_t *desc = EPM_EP_DESC(ep, bufnum);
    if (WSIZE == 2) {
        desc[0] = addr;
        desc[1] = count_rx;
    } else {
        *desc = addr | (count_rx << 16);
    }
}

// Return number of read bytes on an rx endpoint
static uint32_t
epm_get_ep_count_rx(int ep, int bufnum)
{
    epmword_t *desc = EPM_EP_DESC(ep, bufnum);
    if (WSIZE == 2)
        return desc[1] & 0x3ff;
    return (*desc >> 16) & 0x3ff;
}

// Set number of bytes ready to be transmitted on a tx endpoint
static void
epm_set_ep_count_tx(int ep, int bufnum, uint32_t count)
{
    epmword_t *desc = EPM_EP_DESC(ep, bufnum);
    if (WSIZE == 2) {
        desc[1] = count;
    } else {
        uint32_t addr_tx = (EPM_EP_BUF(ep, bufnum) - EPM) * WSIZE;
        *desc = addr_tx | (count << 16);
    }
}

// Setup the transfer descriptors in dedicated usb memory
static void
btable_configure(void)
{
    epm_ep_desc_setup(0, BUFTX, 0);
    epm_ep_desc_setup(0, BUFRX, USB_CDC_EP0_SIZE);
    epm_ep_desc_setup(USB_CDC_EP_ACM, BUFTX, 0);
    epm_ep_desc_setup(USB_CDC_EP_ACM, BUFRX, 0);
    epm_ep_desc_setup(USB_CDC_EP_BULK_OUT, 0, USB_CDC_EP_BULK_OUT_SIZE);
    epm_ep_desc_setup(USB_CDC_EP_BULK_OUT, 1, USB_CDC_EP_BULK_OUT_SIZE);
    epm_ep_desc_setup(USB_CDC_EP_BULK_IN, 0, 0);
    epm_ep_desc_setup(USB_CDC_EP_BULK_IN, 1, 0);
}

// Read a packet stored in dedicated usb memory
static uint32_t
btable_read_packet(int ep, int bufnum, uint8_t *dest, int max_len)
{
    epmword_t *src = EPM_EP_BUF(ep, bufnum);
    uint32_t count = epm_get_ep_count_rx(ep, bufnum);
    if (count > max_len)
        count = max_len;
    int i;
    for (i=0; i<count/WSIZE; i++) {
        uint32_t d = *src++;
        *dest++ = d;
        *dest++ = d >> 8;
        if (WSIZE == 4) {
            *dest++ = d >> 16;
            *dest++ = d >> 24;
        }
    }
    if (count & (WSIZE-1)) {
        uint32_t d = *src;
        *dest++ = d;
        if ((count & (WSIZE-1)) > 1)
            *dest++ = d >> 8;
        if ((count & (WSIZE-1)) > 2)
            *dest++ = d >> 16;
    }
    return count;
}

// Write a packet to dedicated usb memory
static void
btable_write_packet(int ep, int bufnum, const uint8_t *src, int count)
{
    epmword_t *dest = EPM_EP_BUF(ep, bufnum);
    int i;
    for (i=0; i<count/WSIZE; i++) {
        uint8_t b1 = *src++, b2 = *src++, b3 = 0, b4 = 0;
        if (WSIZE == 4) {
            b3 = *src++;
            b4 = *src++;
        }
        *dest++ = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24);
    }
    if (count & (WSIZE-1)) {
        uint32_t d = *src++;
        if ((count & (WSIZE-1)) > 1)
            d |= *src++ << 8;
        if ((count & (WSIZE-1)) > 2)
            d |= *src++ << 16;
        *dest = d;
    }
    epm_set_ep_count_tx(ep, bufnum, count);
}


/****************************************************************
 * USB endpoint register
 ****************************************************************/

#define USB_EPR ((volatile uint32_t *)USB_BASE)

#define EPR_TBITS (USB_EP_DTOG_RX | USB_EPRX_STAT       \
                   | USB_EP_DTOG_TX | USB_EPTX_STAT)
#define EPR_RWBITS (USB_EPADDR_FIELD | USB_EP_KIND | USB_EP_TYPE_MASK)
#define EPR_RWCBITS (USB_EP_CTR_RX | USB_EP_CTR_TX)

// Calculate the memory update needed to set the epr register
static uint32_t
calc_epr_bits(uint32_t epr, uint32_t mask, uint32_t value)
{
    uint32_t tmask = mask & EPR_TBITS, tvalue = value & tmask;
    uint32_t rwmask = mask & EPR_RWBITS, rwbits = value & rwmask;
    uint32_t cbits = EPR_RWCBITS & ~mask;
    return (((epr & (EPR_RWBITS | tmask)) ^ tvalue) & ~rwmask) | rwbits | cbits;
}

// Check if double buffering endpoint hardware can no longer send/receive
static int
epr_is_dbuf_blocking(uint32_t epr)
{
    return !(((epr >> (USB_EP_DTOG_RX_Pos - USB_EP_DTOG_TX_Pos)) ^ epr)
             & USB_EP_DTOG_TX);
}


/****************************************************************
 * USB interface
 ****************************************************************/

static uint32_t bulk_out_pop_count, bulk_out_push_flag;

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    if (readl(&bulk_out_push_flag))
        // No data ready
        return -1;
    uint32_t ep = USB_CDC_EP_BULK_OUT;
    int bufnum = bulk_out_pop_count & 1;
    bulk_out_pop_count++;
    uint32_t count = btable_read_packet(ep, bufnum, data, max_len);
    writel(&bulk_out_push_flag, USB_EP_DTOG_TX);

    // Check if irq handler pulled another packet before push flag update
    uint32_t epr = USB_EPR[ep];
    if (epr_is_dbuf_blocking(epr) && readl(&bulk_out_push_flag)) {
        // Second packet was already read - must notify hardware
        writel(&bulk_out_push_flag, 0);
        USB_EPR[ep] = calc_epr_bits(epr, 0, 0) | USB_EP_DTOG_TX;
    }

    return count;
}

static uint32_t bulk_in_push_pos, bulk_in_pop_flag;
#define BI_START 2

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    if (readl(&bulk_in_pop_flag))
        // No buffer space available
        return -1;
    uint32_t ep = USB_CDC_EP_BULK_IN;
    uint32_t bipp = bulk_in_push_pos, bufnum = bipp & 1;
    bulk_in_push_pos = bipp ^ 1;
    btable_write_packet(ep, bufnum, data, len);
    writel(&bulk_in_pop_flag, USB_EP_DTOG_RX);

    // Check if hardware needs to be notified
    uint32_t epr = USB_EPR[ep];
    if (epr_is_dbuf_blocking(epr) && readl(&bulk_in_pop_flag)) {
        writel(&bulk_in_pop_flag, 0);
        if (unlikely(bipp & BI_START)) {
            // Two packets are always sent when starting in double
            // buffering mode, so wait for second packet before starting.
            if (bipp == (BI_START | 1)) {
                bulk_in_push_pos = 0;
                writel(&bulk_in_pop_flag, USB_EP_KIND); // Dummy flag
                USB_EPR[ep] = calc_epr_bits(epr, USB_EPTX_STAT
                                            , USB_EP_TX_VALID);
            }
        } else {
            USB_EPR[ep] = calc_epr_bits(epr, 0, 0) | USB_EP_DTOG_RX;
        }
    }

    return len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    uint32_t ep = 0, epr = USB_EPR[ep];
    if ((epr & USB_EPRX_STAT) != USB_EP_RX_NAK)
        // No data ready
        return -1;
    uint32_t count = btable_read_packet(ep, BUFRX, data, max_len);
    USB_EPR[ep] = calc_epr_bits(epr, USB_EPRX_STAT | USB_EPTX_STAT
                                , USB_EP_RX_VALID | USB_EP_TX_NAK);
    return count;
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    return usb_read_ep0(data, max_len);
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    uint32_t ep = 0, epr = USB_EPR[ep];
    if ((epr & USB_EPRX_STAT) != USB_EP_RX_VALID)
        // Transfer interrupted
        return -2;
    if ((epr & USB_EPTX_STAT) != USB_EP_TX_NAK)
        // No buffer space available
        return -1;
    btable_write_packet(ep, BUFTX, data, len);
    USB_EPR[ep] = calc_epr_bits(epr, USB_EPTX_STAT, USB_EP_TX_VALID);
    return len;
}

void
usb_stall_ep0(void)
{
    uint32_t ep = 0, epr = USB_EPR[ep];
    USB_EPR[ep] = calc_epr_bits(epr, USB_EPRX_STAT | USB_EPTX_STAT
                                , USB_EP_RX_STALL | USB_EP_TX_STALL);
}

static uint8_t set_address;

void
usb_set_address(uint_fast8_t addr)
{
    writeb(&set_address, addr | USB_DADDR_EF);
    usb_send_ep0(NULL, 0);
}

void
usb_set_configure(void)
{
    uint32_t ep = USB_CDC_EP_BULK_OUT;
    bulk_out_pop_count = 0;
    USB_EPR[ep] = calc_epr_bits(USB_EPR[ep], USB_EPRX_STAT, USB_EP_RX_VALID);

    ep = USB_CDC_EP_BULK_IN;
    bulk_in_push_pos = BI_START;
    writel(&bulk_in_pop_flag, 0);
}


/****************************************************************
 * Setup and interrupts
 ****************************************************************/

// Configure interface after a USB reset event
static void
usb_reset(void)
{
    uint32_t ep = 0;
    USB_EPR[ep] = 0 | USB_EP_CONTROL | USB_EP_RX_VALID | USB_EP_TX_NAK;

    ep = USB_CDC_EP_ACM;
    USB_EPR[ep] = (USB_CDC_EP_ACM | USB_EP_INTERRUPT
                   | USB_EP_RX_NAK | USB_EP_TX_NAK);

    ep = USB_CDC_EP_BULK_OUT;
    USB_EPR[ep] = (USB_CDC_EP_BULK_OUT | USB_EP_BULK | USB_EP_KIND
                   | USB_EP_RX_NAK | USB_EP_DTOG_TX);
    bulk_out_push_flag = USB_EP_DTOG_TX;

    ep = USB_CDC_EP_BULK_IN;
    USB_EPR[ep] = (USB_CDC_EP_BULK_IN | USB_EP_BULK | USB_EP_KIND
                   | USB_EP_TX_NAK);
    bulk_in_pop_flag = USB_EP_DTOG_RX;

    USB->CNTR = USB_CNTR_CTRM | USB_CNTR_RESETM;
    USB->DADDR = USB_DADDR_EF;
}

// Main irq handler
void
USB_IRQHandler(void)
{
    uint32_t istr = USB->ISTR;
    if (istr & USB_ISTR_CTR) {
        // Endpoint activity
        uint32_t ep = istr & USB_ISTR_EP_ID, epr = USB_EPR[ep];
        if (ep == USB_CDC_EP_BULK_OUT) {
            USB_EPR[ep] = (calc_epr_bits(epr, USB_EP_CTR_RX | USB_EP_CTR_TX, 0)
                           | bulk_out_push_flag);
            bulk_out_push_flag = 0;
            usb_notify_bulk_out();
        } else if (ep == USB_CDC_EP_BULK_IN) {
            USB_EPR[ep] = (calc_epr_bits(epr, USB_EP_CTR_RX | USB_EP_CTR_TX, 0)
                           | bulk_in_pop_flag);
            bulk_in_pop_flag = 0;
            usb_notify_bulk_in();
        } else if (ep == 0) {
            USB_EPR[ep] = calc_epr_bits(epr, USB_EP_CTR_RX | USB_EP_CTR_TX, 0);
            usb_notify_ep0();
            if (epr & USB_EP_CTR_TX && set_address) {
                // Apply address after last "in" message transmitted
                USB->DADDR = set_address;
                set_address = 0;
            }
        }
    }
    if (istr & USB_ISTR_RESET) {
        // USB Reset
        USB->ISTR = (uint16_t)~USB_ISTR_RESET;
        usb_reset();
    }
}

DECL_CONSTANT_STR("RESERVE_PINS_USB", "PA11,PA12");

// Initialize the usb controller
void
usb_init(void)
{
    if (CONFIG_MACH_STM32F1) {
        // Pull the D+ pin low briefly to signal a new connection
        gpio_out_setup(GPIO('A', 12), 0);
        udelay(5000);
        gpio_in_setup(GPIO('A', 12), 0);
    }

    // Enable USB clock
    enable_pclock(USB_BASE);

    // Setup USB packet memory
    btable_configure();

    // Enable USB pullup
#ifdef USB_BCDR_DPPU
    USB->BCDR = USB_BCDR_DPPU;
#endif

    // Reset usb controller and enable interrupts
    USB->CNTR = USB_CNTR_FRES;
    USB->DADDR = 0;
    USB->CNTR = USB_CNTR_RESETM;
    USB->ISTR = 0;
    armcm_enable_irq(USB_IRQHandler, USBx_IRQn, 1);
}
DECL_INIT(usb_init);
