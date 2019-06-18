// Hardware interface to "fullspeed USB controller" on stm32f1
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // NULL
#include "autoconf.h" // CONFIG_STM_FLASH_START_2000
#include "board/armcm_timer.h" // udelay
#include "board/gpio.h" // gpio_out_setup
#include "board/io.h" // writeb
#include "board/irq.h" // irq_disable
#include "board/usb_cdc.h" // usb_notify_ep0
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "internal.h" // GPIO
#include "sched.h" // DECL_INIT
#include "stm32f1xx.h" // USB


/****************************************************************
 * USB transfer memory
 ****************************************************************/

struct ep_desc {
    uint32_t addr_tx, count_tx, addr_rx, count_rx;
};

struct ep_mem {
    struct ep_desc ep0, ep_acm, ep_bulk_out, ep_bulk_in;
    uint32_t ep0_tx[USB_CDC_EP0_SIZE / 2];
    uint32_t ep0_rx[USB_CDC_EP0_SIZE / 2];
    uint32_t ep_acm_tx[USB_CDC_EP_ACM_SIZE / 2];
    uint32_t ep_bulk_out_rx[USB_CDC_EP_BULK_OUT_SIZE / 2];
    uint32_t ep_bulk_in_tx[USB_CDC_EP_BULK_IN_SIZE / 2];
};

#define USB_BTABLE ((struct ep_mem *)(USB_BASE + 0x400))

#define CALC_ADDR(p) (((void*)(p) - (void*)USB_BTABLE) / 2)
#define CALC_SIZE(s) ((s) > 32 ? (DIV_ROUND_UP((s), 32) << 10) | 0x8000 \
                      : DIV_ROUND_UP((s), 2) << 10)

// Setup the transfer descriptors in dedicated usb memory
static void
btable_configure(void)
{
    USB_BTABLE->ep0.count_tx = 0;
    USB_BTABLE->ep0.addr_tx = CALC_ADDR(USB_BTABLE->ep0_tx);
    USB_BTABLE->ep0.count_rx = CALC_SIZE(USB_CDC_EP0_SIZE);
    USB_BTABLE->ep0.addr_rx = CALC_ADDR(USB_BTABLE->ep0_rx);

    USB_BTABLE->ep_acm.count_tx = 0;
    USB_BTABLE->ep_acm.addr_tx = CALC_ADDR(USB_BTABLE->ep_acm_tx);

    USB_BTABLE->ep_bulk_out.count_rx = CALC_SIZE(USB_CDC_EP_BULK_OUT_SIZE);
    USB_BTABLE->ep_bulk_out.addr_rx = CALC_ADDR(USB_BTABLE->ep_bulk_out_rx);

    USB_BTABLE->ep_bulk_in.count_tx = 0;
    USB_BTABLE->ep_bulk_in.addr_tx = CALC_ADDR(USB_BTABLE->ep_bulk_in_tx);
}

// Read a packet stored in dedicated usb memory
static void
btable_read_packet(uint8_t *dest, uint32_t *src, int count)
{
    uint_fast8_t i;
    for (i=0; i<(count/2); i++) {
        uint32_t d = *src++;
        *dest++ = d;
        *dest++ = d >> 8;
    }
    if (count & 1)
        *dest = *src;
}

// Write a packet to dedicated usb memory
static void
btable_write_packet(uint32_t *dest, const uint8_t *src, int count)
{
    int i;
    for (i=0; i<(count/2); i++) {
        uint8_t b1 = *src++, b2 = *src++;
        *dest++ = b1 | (b2 << 8);
    }
    if (count & 1)
        *dest = *src;
}


/****************************************************************
 * USB endpoint register
 ****************************************************************/

#define USB_EPR ((volatile uint32_t *)USB_BASE)

#define EP_BULK (0 << USB_EP0R_EP_TYPE_Pos)
#define EP_CONTROL (1 << USB_EP0R_EP_TYPE_Pos)
#define EP_INTERRUPT (3 << USB_EP0R_EP_TYPE_Pos)
#define RX_STALL (1 << USB_EP0R_STAT_RX_Pos)
#define RX_NAK (2 << USB_EP0R_STAT_RX_Pos)
#define RX_VALID (3 << USB_EP0R_STAT_RX_Pos)
#define TX_STALL (1 << USB_EP0R_STAT_TX_Pos)
#define TX_NAK (2 << USB_EP0R_STAT_TX_Pos)
#define TX_VALID (3 << USB_EP0R_STAT_TX_Pos)
#define EPR_RWBITS (USB_EP0R_EA | USB_EP0R_EP_KIND | USB_EP0R_EP_TYPE)
#define EPR_RWCBITS (USB_EP0R_CTR_RX | USB_EP0R_CTR_TX)

static uint32_t
set_stat_rx_bits(uint32_t epr, uint32_t bits)
{
    return ((epr & (EPR_RWBITS | USB_EP0R_STAT_RX_Msk)) ^ bits) | EPR_RWCBITS;
}

static uint32_t
set_stat_tx_bits(uint32_t epr, uint32_t bits)
{
    return ((epr & (EPR_RWBITS | USB_EP0R_STAT_TX_Msk)) ^ bits) | EPR_RWCBITS;
}

static uint32_t
set_stat_rxtx_bits(uint32_t epr, uint32_t bits)
{
    uint32_t mask = EPR_RWBITS | USB_EP0R_STAT_RX_Msk | USB_EP0R_STAT_TX_Msk;
    return ((epr & mask) ^ bits) | EPR_RWCBITS;
}


/****************************************************************
 * USB interface
 ****************************************************************/

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    uint32_t epr = USB_EPR[USB_CDC_EP_BULK_OUT];
    if ((epr & USB_EP0R_STAT_RX_Msk) == RX_VALID)
        // No data ready
        return -1;
    uint32_t count = USB_BTABLE->ep_bulk_out.count_rx & 0x3ff;
    if (count > max_len)
        count = max_len;
    btable_read_packet(data, USB_BTABLE->ep_bulk_out_rx, count);
    USB_EPR[USB_CDC_EP_BULK_OUT] = set_stat_rx_bits(epr, RX_VALID);
    return count;
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    uint32_t epr = USB_EPR[USB_CDC_EP_BULK_IN];
    if ((epr & USB_EP0R_STAT_TX_Msk) != TX_NAK)
        // No buffer space available
        return -1;
    btable_write_packet(USB_BTABLE->ep_bulk_in_tx, data, len);
    USB_BTABLE->ep_bulk_in.count_tx = len;
    USB_EPR[USB_CDC_EP_BULK_IN] = set_stat_tx_bits(epr, TX_VALID);
    return len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    uint32_t epr = USB_EPR[0];
    if ((epr & USB_EP0R_STAT_RX_Msk) != RX_NAK)
        // No data ready
        return -1;
    uint32_t count = USB_BTABLE->ep0.count_rx & 0x3ff;
    if (count > max_len)
        count = max_len;
    btable_read_packet(data, USB_BTABLE->ep0_rx, count);
    USB_EPR[0] = set_stat_rxtx_bits(epr, RX_VALID | TX_NAK);
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
    if ((epr & USB_EP0R_STAT_RX_Msk) != RX_VALID)
        // Transfer interrupted
        return -2;
    if ((epr & USB_EP0R_STAT_TX_Msk) != TX_NAK)
        // No buffer space available
        return -1;
    btable_write_packet(USB_BTABLE->ep0_tx, data, len);
    USB_BTABLE->ep0.count_tx = len;
    USB_EPR[0] = set_stat_tx_bits(epr, TX_VALID);
    return len;
}

void
usb_stall_ep0(void)
{
    USB_EPR[0] = set_stat_rxtx_bits(USB_EPR[0], RX_STALL | TX_STALL);
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

void
usb_request_bootloader(void)
{
    if (!CONFIG_STM_FLASH_START_2000)
        return;
    // Enter "stm32duino" bootloader
    irq_disable();
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    PWR->CR |= PWR_CR_DBP;
    BKP->DR10 = 0x01;
    PWR->CR &=~ PWR_CR_DBP;
    NVIC_SystemReset();
}


/****************************************************************
 * Setup and interrupts
 ****************************************************************/

// Initialize the usb controller
void
usb_init(void)
{
    // Pull the D+ pin low briefly to signal a new connection
    gpio_out_setup(GPIO('A', 12), 0);
    udelay(5000);
    gpio_in_setup(GPIO('A', 12), 0);

    // Setup USB packet memory
    btable_configure();

    // Enable USB clock
    RCC->APB1ENR |= RCC_APB1ENR_USBEN;

    // Reset usb controller and enable interrupts
    USB->CNTR = USB_CNTR_FRES;
    USB->BTABLE = 0;
    USB->DADDR = 0;
    USB->CNTR = USB_CNTR_RESETM;
    USB->ISTR = 0;
    NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}
DECL_INIT(usb_init);

// Configure interface after a USB reset event
static void
usb_reset(void)
{
    USB_EPR[0] = 0 | EP_CONTROL | RX_VALID | TX_NAK;
    USB_EPR[USB_CDC_EP_ACM] = USB_CDC_EP_ACM | EP_INTERRUPT | RX_NAK | TX_NAK;
    USB_EPR[USB_CDC_EP_BULK_OUT] = (USB_CDC_EP_BULK_OUT | EP_BULK
                                    | RX_VALID | TX_NAK);
    USB_EPR[USB_CDC_EP_BULK_IN] = (USB_CDC_EP_BULK_IN | EP_BULK
                                   | RX_NAK | TX_NAK);

    USB->CNTR = USB_CNTR_CTRM | USB_CNTR_RESETM;
    USB->DADDR = USB_DADDR_EF;
}

// Main irq handler
void __visible
USB_LP_CAN1_RX0_IRQHandler(void)
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
