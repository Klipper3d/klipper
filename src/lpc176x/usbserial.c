// Hardware interface to USB on lpc176x
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <string.h> // memcpy
#include "LPC17xx.h" // LPC_SC
#include "autoconf.h" // CONFIG_SMOOTHIEWARE_BOOTLOADER
#include "board/armcm_timer.h" // udelay
#include "board/irq.h" // irq_disable
#include "board/misc.h" // timer_read_time
#include "byteorder.h" // cpu_to_le32
#include "command.h" // DECL_CONSTANT_STR
#include "generic/usb_cdc.h" // usb_notify_ep0
#include "internal.h" // gpio_peripheral
#include "sched.h" // DECL_INIT
#include "usb_cdc_ep.h" // USB_CDC_EP_BULK_IN

// Internal endpoint addresses
#define EP0OUT 0x00
#define EP0IN 0x01
#define EP1IN 0x03
#define EP2OUT 0x04
#define EP5IN 0x0b

// USB device interupt status flags
#define EP_SLOW (1<<2)
#define DEV_STAT (1<<3)
#define CCEMPTY (1<<4)
#define CDFULL (1<<5)
#define EP_RLZED (1<<8)

#define RD_EN (1<<0)
#define WR_EN (1<<1)

static void
usb_irq_disable(void)
{
    NVIC_DisableIRQ(USB_IRQn);
}

static void
usb_irq_enable(void)
{
    NVIC_EnableIRQ(USB_IRQn);
}

static void
usb_wait(uint32_t flag)
{
    while (!(LPC_USB->USBDevIntSt & flag))
        ;
    LPC_USB->USBDevIntClr = flag;
}


/****************************************************************
 * Serial Interface Engine (SIE) functions
 ****************************************************************/

#define SIE_CMD_SELECT 0x00
#define SIE_CMD_SET_ENDPOINT_STATUS 0x40
#define SIE_CMD_SET_ADDRESS 0xD0
#define SIE_CMD_CONFIGURE 0xD8
#define SIE_CMD_SET_DEVICE_STATUS 0xFE
#define SIE_CMD_CLEAR_BUFFER 0xF2
#define SIE_CMD_VALIDATE_BUFFER 0xFA

static void
sie_cmd(uint32_t cmd)
{
    LPC_USB->USBDevIntClr = CDFULL | CCEMPTY;
    LPC_USB->USBCmdCode = 0x00000500 | (cmd << 16);
    usb_wait(CCEMPTY);
}

static void
sie_cmd_write(uint32_t cmd, uint32_t data)
{
    sie_cmd(cmd);
    LPC_USB->USBCmdCode = 0x00000100 | (data << 16);
    usb_wait(CCEMPTY);
}

static uint32_t
sie_cmd_read(uint32_t cmd)
{
    sie_cmd(cmd);
    LPC_USB->USBCmdCode = 0x00000200 | (cmd << 16);
    usb_wait(CDFULL);
    return LPC_USB->USBCmdData;
}

static uint32_t
sie_select_and_clear(uint32_t idx)
{
    LPC_USB->USBEpIntClr = 1<<idx;
    usb_wait(CDFULL);
    return LPC_USB->USBCmdData;
}


/****************************************************************
 * Interface
 ****************************************************************/

static int_fast8_t
usb_write_packet(uint32_t ep, const void *data, uint_fast8_t len)
{
    usb_irq_disable();
    uint32_t sts = sie_cmd_read(SIE_CMD_SELECT | ep);
    if (sts & 0x01) {
        // Output buffers full
        usb_irq_enable();
        return -1;
    }

    LPC_USB->USBCtrl = WR_EN | ((ep/2) << 2);
    LPC_USB->USBTxPLen = len;
    if (!len)
        LPC_USB->USBTxData = 0;
    int i;
    for (i = 0; i<DIV_ROUND_UP(len, 4); i++) {
        uint32_t d;
        memcpy(&d, data, sizeof(d));
        data += sizeof(d);
        LPC_USB->USBTxData = cpu_to_le32(d);
    }
    sie_cmd(SIE_CMD_VALIDATE_BUFFER);
    usb_irq_enable();

    return len;
}

static int_fast8_t
usb_read_packet(uint32_t ep, void *data, uint_fast8_t max_len)
{
    usb_irq_disable();
    uint32_t sts = sie_cmd_read(SIE_CMD_SELECT | ep);
    if (!(sts & 0x01)) {
        // No data available
        usb_irq_enable();
        return -1;
    }

    // Determine packet size
    LPC_USB->USBCtrl = RD_EN | ((ep/2) << 2);
    uint32_t plen = LPC_USB->USBRxPLen;
    while (!(plen & (1<<11)))
        plen = LPC_USB->USBRxPLen;
    plen &= 0x3FF;
    if (plen > max_len)
        // XXX - return error code?  must keep reading?
        plen = max_len;
    // Copy data
    uint32_t xfer = plen;
    for (;;) {
        uint32_t d = le32_to_cpu(LPC_USB->USBRxData);
        if (xfer <= sizeof(d)) {
            memcpy(data, &d, xfer);
            break;
        }
        memcpy(data, &d, sizeof(d));
        data += sizeof(d);
        xfer -= sizeof(d);
    }
    // Clear space for next packet
    sts = sie_cmd_read(SIE_CMD_CLEAR_BUFFER);
    usb_irq_enable();
    if (sts & 0x01)
        // Packet overwritten
        return -1;

    return plen;
}

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    return usb_read_packet(EP2OUT, data, max_len);
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    return usb_write_packet(EP5IN, data, len);
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    return usb_read_packet(EP0OUT, data, max_len);
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    return usb_read_ep0(data, max_len);
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    return usb_write_packet(EP0IN, data, len);
}

void
usb_stall_ep0(void)
{
    usb_irq_disable();
    sie_cmd_write(SIE_CMD_SET_ENDPOINT_STATUS | 0, (1<<7));
    usb_irq_enable();
}

void
usb_set_address(uint_fast8_t addr)
{
    usb_irq_disable();
    sie_cmd_write(SIE_CMD_SET_ADDRESS, addr | (1<<7));
    usb_irq_enable();
    usb_send_ep0(NULL, 0);
}

static void
realize_endpoint(uint32_t idx, uint32_t packet_size)
{
    LPC_USB->USBDevIntClr = EP_RLZED;
    LPC_USB->USBReEp |= 1<<idx;
    LPC_USB->USBEpInd = idx;
    LPC_USB->USBMaxPSize = packet_size;
    usb_wait(EP_RLZED);
    LPC_USB->USBEpIntEn |= 1<<idx;
    sie_cmd_write(SIE_CMD_SET_ENDPOINT_STATUS | idx, 0);
}

void
usb_set_configure(void)
{
    usb_irq_disable();
    realize_endpoint(EP1IN, USB_CDC_EP_ACM_SIZE);
    realize_endpoint(EP2OUT, USB_CDC_EP_BULK_OUT_SIZE);
    realize_endpoint(EP5IN, USB_CDC_EP_BULK_IN_SIZE);
    sie_cmd_write(SIE_CMD_CONFIGURE, 1);
    usb_irq_enable();
}

void
usb_request_bootloader(void)
{
    if (!CONFIG_SMOOTHIEWARE_BOOTLOADER)
        return;
    // Disable USB and pause for 5ms so host recognizes a disconnect
    irq_disable();
    sie_cmd_write(SIE_CMD_SET_DEVICE_STATUS, 0);
    udelay(5000);
    // The "LPC17xx-DFU-Bootloader" will enter the bootloader if the
    // watchdog timeout flag is set.
    LPC_WDT->WDMOD = 0x07;
    NVIC_SystemReset();
}

DECL_CONSTANT_STR("RESERVE_PINS_USB", "P0.30,P0.29,P2.9");

void
usbserial_init(void)
{
    usb_irq_disable();
    // enable power
    enable_pclock(PCLK_USB);
    // enable clock
    LPC_USB->USBClkCtrl = 0x12;
    while (LPC_USB->USBClkSt != 0x12)
        ;
    // configure USBD-, USBD+, and USB Connect pins
    gpio_peripheral(GPIO(0, 30), 1, 0);
    gpio_peripheral(GPIO(0, 29), 1, 0);
    gpio_peripheral(GPIO(2, 9), 1, 0);
    // enforce a minimum time bus is disconnected before connecting
    udelay(5000);
    // setup endpoints
    realize_endpoint(EP0OUT, USB_CDC_EP0_SIZE);
    realize_endpoint(EP0IN, USB_CDC_EP0_SIZE);
    sie_cmd_write(SIE_CMD_SET_DEVICE_STATUS, 1);
    // enable irqs
    LPC_USB->USBDevIntEn = DEV_STAT | EP_SLOW;
    NVIC_SetPriority(USB_IRQn, 1);
    usb_irq_enable();
}
DECL_INIT(usbserial_init);

void
usbserial_shutdown(void)
{
    usb_irq_enable();
}
DECL_SHUTDOWN(usbserial_shutdown);

void __visible
USB_IRQHandler(void)
{
    uint32_t udis = LPC_USB->USBDevIntSt;
    if (udis & DEV_STAT) {
        LPC_USB->USBDevIntClr = DEV_STAT;
        // XXX - should handle reset and other states
    }
    if (udis & EP_SLOW) {
        uint32_t ueis = LPC_USB->USBEpIntSt;
        if (ueis & (1<<EP0OUT)) {
            sie_select_and_clear(EP0OUT);
            usb_notify_ep0();
        }
        if (ueis & (1<<EP0IN)) {
            sie_select_and_clear(EP0IN);
            usb_notify_ep0();
        }
        if (ueis & (1<<EP2OUT)) {
            sie_select_and_clear(EP2OUT);
            usb_notify_bulk_out();
        }
        if (ueis & (1<<EP5IN)) {
            sie_select_and_clear(EP5IN);
            usb_notify_bulk_in();
        }
        LPC_USB->USBDevIntClr = EP_SLOW;
    }
}
