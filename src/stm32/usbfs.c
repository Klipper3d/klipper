// Hardware interface to "fullspeed USB controller"
//
// Copyright (C) 2018-2021  Kevin O'Connor <kevin@koconnor.net>
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

#if CONFIG_MACH_STM32F103 || CONFIG_MACH_STM32G4
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


/****************************************************************
 * USB transfer memory
 ****************************************************************/

// Layout of the USB transfer memory
#define EPM ((epmword_t*)USB_PMAADDR)
#define EPM_EP_DESC(ep) (&EPM[(ep) * (8 / WSIZE)])
#define EPM_BUF_OFFSET 0x10
#define EPM_EP_BUF_SIZE (64 / WSIZE + 1)
#define EPM_EP_TX_BUF(ep) (&EPM[EPM_BUF_OFFSET + (ep)*2*EPM_EP_BUF_SIZE])
#define EPM_EP_RX_BUF(ep) (&EPM[EPM_BUF_OFFSET + (1+(ep)*2)*EPM_EP_BUF_SIZE])

// Configure the usb descriptor for an endpoint
static void
epm_ep_desc_setup(int ep, int rx_size)
{
    uint32_t addr_tx = (EPM_EP_TX_BUF(ep) - EPM) * WSIZE, count_tx = 0;
    uint32_t addr_rx = (EPM_EP_RX_BUF(ep) - EPM) * WSIZE;
    uint32_t count_rx = (rx_size <= 30 ? DIV_ROUND_UP(rx_size, 2) << 10
                         : ((DIV_ROUND_UP(rx_size, 32) - 1) << 10) | 0x8000);
    epmword_t *desc = EPM_EP_DESC(ep);
    if (WSIZE == 2) {
        desc[0] = addr_tx;
        desc[1] = count_tx;
        desc[2] = addr_rx;
        desc[3] = count_rx;
    } else {
        desc[0] = addr_tx | (count_tx << 16);
        desc[1] = addr_rx | (count_rx << 16);
    }
}

// Return number of read bytes on an rx endpoint
static uint32_t
epm_get_ep_count_rx(int ep)
{
    epmword_t *desc = EPM_EP_DESC(ep);
    if (WSIZE == 2)
        return desc[3] & 0x3ff;
    return (desc[1] >> 16) & 0x3ff;
}

// Set number of bytes ready to be transmitted on a tx endpoint
static void
epm_set_ep_count_tx(int ep, uint32_t count)
{
    epmword_t *desc = EPM_EP_DESC(ep);
    if (WSIZE == 2) {
        desc[1] = count;
    } else {
        uint32_t addr_tx = (EPM_EP_TX_BUF(ep) - EPM) * WSIZE;
        desc[0] = addr_tx | (count << 16);
    }
}

// Setup the transfer descriptors in dedicated usb memory
static void
btable_configure(void)
{
    epm_ep_desc_setup(0, USB_CDC_EP0_SIZE);
    epm_ep_desc_setup(USB_CDC_EP_ACM, 0);
    epm_ep_desc_setup(USB_CDC_EP_BULK_OUT, USB_CDC_EP_BULK_OUT_SIZE);
    epm_ep_desc_setup(USB_CDC_EP_BULK_IN, 0);
}

// Read a packet stored in dedicated usb memory
static uint32_t
btable_read_packet(int ep, uint8_t *dest, int max_len)
{
    epmword_t *src = EPM_EP_RX_BUF(ep);
    uint32_t count = epm_get_ep_count_rx(ep);
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
btable_write_packet(int ep, const uint8_t *src, int count)
{
    epmword_t *dest = EPM_EP_TX_BUF(ep);
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
    epm_set_ep_count_tx(ep, count);
}


/****************************************************************
 * USB endpoint register
 ****************************************************************/

#define USB_EPR ((volatile uint32_t *)USB_BASE)

#define EPR_RWBITS (USB_EPADDR_FIELD | USB_EP_KIND | USB_EP_TYPE_MASK)
#define EPR_RWCBITS (USB_EP_CTR_RX | USB_EP_CTR_TX)

static uint32_t
set_stat_rx_bits(uint32_t epr, uint32_t bits)
{
    return ((epr & (EPR_RWBITS | USB_EPRX_STAT)) ^ bits) | EPR_RWCBITS;
}

static uint32_t
set_stat_tx_bits(uint32_t epr, uint32_t bits)
{
    return ((epr & (EPR_RWBITS | USB_EPTX_STAT)) ^ bits) | EPR_RWCBITS;
}

static uint32_t
set_stat_rxtx_bits(uint32_t epr, uint32_t bits)
{
    uint32_t mask = EPR_RWBITS | USB_EPRX_STAT | USB_EPTX_STAT;
    return ((epr & mask) ^ bits) | EPR_RWCBITS;
}


/****************************************************************
 * USB interface
 ****************************************************************/

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    uint32_t epr = USB_EPR[USB_CDC_EP_BULK_OUT];
    if ((epr & USB_EPRX_STAT) == USB_EP_RX_VALID)
        // No data ready
        return -1;
    uint32_t count = btable_read_packet(USB_CDC_EP_BULK_OUT, data, max_len);
    USB_EPR[USB_CDC_EP_BULK_OUT] = set_stat_rx_bits(epr, USB_EP_RX_VALID);
    return count;
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    uint32_t epr = USB_EPR[USB_CDC_EP_BULK_IN];
    if ((epr & USB_EPTX_STAT) != USB_EP_TX_NAK)
        // No buffer space available
        return -1;
    btable_write_packet(USB_CDC_EP_BULK_IN, data, len);
    USB_EPR[USB_CDC_EP_BULK_IN] = set_stat_tx_bits(epr, USB_EP_TX_VALID);
    return len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    uint32_t epr = USB_EPR[0];
    if ((epr & USB_EPRX_STAT) != USB_EP_RX_NAK)
        // No data ready
        return -1;
    uint32_t count = btable_read_packet(0, data, max_len);
    USB_EPR[0] = set_stat_rxtx_bits(epr, USB_EP_RX_VALID | USB_EP_TX_NAK);
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
    uint32_t epr = USB_EPR[0];
    if ((epr & USB_EPRX_STAT) != USB_EP_RX_VALID)
        // Transfer interrupted
        return -2;
    if ((epr & USB_EPTX_STAT) != USB_EP_TX_NAK)
        // No buffer space available
        return -1;
    btable_write_packet(0, data, len);
    USB_EPR[0] = set_stat_tx_bits(epr, USB_EP_TX_VALID);
    return len;
}

void
usb_stall_ep0(void)
{
    USB_EPR[0] = set_stat_rxtx_bits(USB_EPR[0]
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
}


/****************************************************************
 * Setup and interrupts
 ****************************************************************/

// Configure interface after a USB reset event
static void
usb_reset(void)
{
    USB_EPR[0] = 0 | USB_EP_CONTROL | USB_EP_RX_VALID | USB_EP_TX_NAK;
    USB_EPR[USB_CDC_EP_ACM] = (USB_CDC_EP_ACM | USB_EP_INTERRUPT
                               | USB_EP_RX_NAK | USB_EP_TX_NAK);
    USB_EPR[USB_CDC_EP_BULK_OUT] = (USB_CDC_EP_BULK_OUT | USB_EP_BULK
                                    | USB_EP_RX_VALID | USB_EP_TX_NAK);
    USB_EPR[USB_CDC_EP_BULK_IN] = (USB_CDC_EP_BULK_IN | USB_EP_BULK
                                   | USB_EP_RX_NAK | USB_EP_TX_NAK);

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
        uint32_t ep = istr & USB_ISTR_EP_ID;
        uint32_t epr = USB_EPR[ep];
        USB_EPR[ep] = epr & EPR_RWBITS;
        if (ep == 0) {
            usb_notify_ep0();
            if (epr & USB_EP_CTR_TX && set_address) {
                // Apply address after last "in" message transmitted
                USB->DADDR = set_address;
                set_address = 0;
            }
        } else if (ep == USB_CDC_EP_BULK_OUT) {
            usb_notify_bulk_out();
        } else if (ep == USB_CDC_EP_BULK_IN) {
            usb_notify_bulk_in();
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
