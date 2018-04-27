#ifndef __GENERIC_USB_CDC_H
#define __GENERIC_USB_CDC_H

#include <stdint.h> // uint_fast8_t

enum {
    USB_CDC_EP0_SIZE = 16,

    // XXX - endpoint ids may need to changed per-board
    USB_CDC_EP_ACM = 1,
    USB_CDC_EP_ACM_SIZE = 8,

    USB_CDC_EP_BULK_OUT = 2,
    USB_CDC_EP_BULK_OUT_SIZE = 64,

    USB_CDC_EP_BULK_IN = 5,
    USB_CDC_EP_BULK_IN_SIZE = 64,
};

// callbacks provided by board specific code
int_fast8_t usb_read_bulk_out(void *data, uint_fast8_t max_len);
int_fast8_t usb_send_bulk_in(void *data, uint_fast8_t len);
int_fast8_t usb_read_setup(void *data, uint_fast8_t max_len);
int_fast8_t usb_send_setup(const void *data, uint_fast8_t len);
void usb_send_pgm_setup(void *data, uint_fast8_t len);
void usb_set_stall(void);
void usb_set_address(uint_fast8_t addr);
void usb_set_configure(void);

// usb_cdc.c
void usb_notify_bulk_in(void);
void usb_notify_bulk_out(void);
void usb_notify_setup(void);

#endif // usb_cdc.h
