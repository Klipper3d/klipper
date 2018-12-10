// Hardware interface to USB on AVR at90usb
//
// Copyright (C) 2018  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <avr/interrupt.h> // USB_COM_vect
#include <string.h> // NULL
#include "autoconf.h" // CONFIG_MACH_at90usb1286
#include "board/usb_cdc.h" // usb_notify_ep0
#include "board/usb_cdc_ep.h" // USB_CDC_EP_BULK_IN
#include "pgm.h" // READP
#include "sched.h" // DECL_INIT

// EPCFG0X definitions
#define EP_TYPE_CONTROL      0x00
#define EP_TYPE_BULK_IN      0x81
#define EP_TYPE_BULK_OUT     0x80
#define EP_TYPE_INTERRUPT_IN 0xC1

// EPCFG1X definitions
#define EP_SINGLE_BUFFER 0x02
#define EP_DOUBLE_BUFFER 0x06
#define EP_SIZE(s) ((s)==64 ? 0x30 : ((s)==32 ? 0x20 : ((s)==16 ? 0x10 : 0x00)))

static void
usb_write_packet(const uint8_t *data, uint8_t len)
{
    while (len--)
        UEDATX = *data++;
}

static void
usb_write_packet_progmem(const uint8_t *data, uint8_t len)
{
    while (len--)
        UEDATX = READP(*data++);
}

static void
usb_read_packet(uint8_t *data, uint8_t len)
{
    while (len--)
        *data++ = UEDATX;
}

int_fast8_t
usb_read_bulk_out(void *data, uint_fast8_t max_len)
{
    UENUM = USB_CDC_EP_BULK_OUT;
    if (!(UEINTX & (1<<RXOUTI))) {
        // No data ready
        UEIENX = 1<<RXOUTE;
        return -1;
    }
    uint8_t len = UEBCLX;
    usb_read_packet(data, len);
    UEINTX = (uint8_t)~((1<<FIFOCON) | (1<<RXOUTI));
    return len;
}

int_fast8_t
usb_send_bulk_in(void *data, uint_fast8_t len)
{
    UENUM = USB_CDC_EP_BULK_IN;
    if (!(UEINTX & (1<<TXINI))) {
        // Buffer full
        UEIENX = 1<<TXINE;
        return -1;
    }
    usb_write_packet(data, len);
    UEINTX = (uint8_t)~((1<<FIFOCON) | (1<<TXINI) | (1<<RXOUTI));
    return len;
}

int_fast8_t
usb_read_ep0(void *data, uint_fast8_t max_len)
{
    UENUM = 0;
    uint8_t ueintx = UEINTX;
    if (ueintx & (1<<RXSTPI))
        return -2;
    if (!(ueintx & (1<<RXOUTI))) {
        // Not ready to receive data
        UEIENX = (1<<RXSTPE) | (1<<RXOUTE);
        return -1;
    }
    usb_read_packet(data, max_len);
    if (UEINTX & (1<<RXSTPI))
        return -2;
    UEINTX = ~(1<<RXOUTI);
    return max_len;
}

int_fast8_t
usb_read_ep0_setup(void *data, uint_fast8_t max_len)
{
    UENUM = 0;
    uint8_t ueintx = UEINTX;
    if (!(ueintx & ((1<<RXSTPI)))) {
        // No data ready to read
        UEIENX = 1<<RXSTPE;
        return -1;
    }
    usb_read_packet(data, max_len);
    UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI));
    return max_len;
}

static int8_t
_usb_send_ep0(const void *data, uint8_t len, uint8_t progmem)
{
    UENUM = 0;
    uint8_t ueintx = UEINTX;
    if (ueintx & ((1<<RXSTPI) | (1<<RXOUTI)))
        return -2;
    if (!(ueintx & (1<<TXINI))) {
        // Not ready to send
        UEIENX = (1<<RXSTPE) | (1<<RXOUTE) | (1<<TXINE);
        return -1;
    }
    if (progmem)
        usb_write_packet_progmem(data, len);
    else
        usb_write_packet(data, len);
    UEINTX = ~(1<<TXINI);
    return len;
}

int_fast8_t
usb_send_ep0(const void *data, uint_fast8_t len)
{
    return _usb_send_ep0(data, len, 0);
}

int_fast8_t
usb_send_ep0_progmem(const void *data, uint_fast8_t len)
{
    return _usb_send_ep0(data, len, 1);
}

void
usb_stall_ep0(void)
{
    UENUM = 0;
    UECONX = (1<<STALLRQ) | (1<<EPEN);
    UEIENX = 1<<RXSTPE;
}

static uint8_t set_address;

void
usb_set_address(uint_fast8_t addr)
{
    set_address = addr | (1<<ADDEN);
    _usb_send_ep0(NULL, 0, 0);
    UEIENX = (1<<RXSTPE) | (1<<TXINE);
}

void
usb_set_configure(void)
{
    UENUM = USB_CDC_EP_ACM;
    UECONX = 1<<EPEN;
    UECFG0X = EP_TYPE_INTERRUPT_IN;
    UECFG1X = EP_SIZE(USB_CDC_EP_ACM_SIZE) | EP_SINGLE_BUFFER;

    UENUM = USB_CDC_EP_BULK_OUT;
    UECONX = 1<<EPEN;
    UECFG0X = EP_TYPE_BULK_OUT;
    UECFG1X = EP_SIZE(USB_CDC_EP_BULK_OUT_SIZE) | EP_DOUBLE_BUFFER;
    UEIENX = 1<<RXOUTE;

    UENUM = USB_CDC_EP_BULK_IN;
    UECONX = 1<<EPEN;
    UECFG0X = EP_TYPE_BULK_IN;
    UECFG1X = EP_SIZE(USB_CDC_EP_BULK_IN_SIZE) | EP_DOUBLE_BUFFER;
    UEIENX = 1<<TXINE;
}

#if CONFIG_MACH_at90usb1286
#define UHWCON_Init ((1<<UIMOD) | (1<<UVREGE))
#define PLLCSR_Init ((1<<PLLP2) | (1<<PLLP0) | (1<<PLLE))
#elif CONFIG_MACH_at90usb646
#define UHWCON_Init ((1<<UIMOD) | (1<<UVREGE))
#define PLLCSR_Init ((1<<PLLP2) | (1<<PLLP1) | (1<<PLLE))
#elif CONFIG_MACH_atmega32u4
#define UHWCON_Init (1<<UVREGE)
#define PLLCSR_Init ((1<<PINDIV) | (1<<PLLE))
#endif

void
usbserial_init(void)
{
    // Set USB controller to device mode
    UHWCON = UHWCON_Init;

    // Enable USB clock
    USBCON = (1<<USBE) | (1<<FRZCLK);
    PLLCSR = PLLCSR_Init;
    while (!(PLLCSR & (1<<PLOCK)))
        ;
    USBCON = (1<<USBE) | (1<<OTGPADE);

    // Enable USB pullup
    UDCON = 0;

    // Enable interrupts
    UDIEN = 1<<EORSTE;
}
DECL_INIT(usbserial_init);

ISR(USB_GEN_vect)
{
    uint8_t udint = UDINT;
    UDINT = 0;
    if (udint & (1<<EORSTI)) {
        // Configure endpoint 0 after usb reset completes
        uint8_t old_uenum = UENUM;
        UENUM = 0;
        UECONX = 1<<EPEN;
        UECFG0X = EP_TYPE_CONTROL;
        UECFG1X = EP_SIZE(USB_CDC_EP0_SIZE) | EP_SINGLE_BUFFER;
        UEIENX = 1<<RXSTPE;
        UENUM = old_uenum;
    }
}

ISR(USB_COM_vect)
{
    uint8_t ueint = UEINT, old_uenum = UENUM;
    if (ueint & (1<<0)) {
        UENUM = 0;
        UEIENX = 0;
        usb_notify_ep0();

        uint8_t ueintx = UEINTX;
        if (!(ueintx & (1<<RXSTPI)) && (ueintx & (1<<TXINI)) && set_address) {
            // Ack from set_address command sent - now update address
            UDADDR = set_address;
            set_address = 0;
        }
    }
    if (ueint & (1<<USB_CDC_EP_BULK_OUT)) {
        UENUM = USB_CDC_EP_BULK_OUT;
        UEIENX = 0;
        usb_notify_bulk_out();
    }
    if (ueint & (1<<USB_CDC_EP_BULK_IN)) {
        UENUM = USB_CDC_EP_BULK_IN;
        UEIENX = 0;
        usb_notify_bulk_in();
    }
    UENUM = old_uenum;
}
