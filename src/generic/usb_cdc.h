#ifndef __GENERIC_USB_CDC_H
#define __GENERIC_USB_CDC_H

#include <stdint.h> // uint_fast8_t

// callbacks provided by board specific code
int_fast8_t usb_read_bulk_out(void *data, uint_fast8_t max_len);
int_fast8_t usb_send_bulk_in(void *data, uint_fast8_t len);
int_fast8_t usb_read_ep0(void *data, uint_fast8_t max_len);
int_fast8_t usb_read_ep0_setup(void *data, uint_fast8_t max_len);
int_fast8_t usb_send_ep0(const void *data, uint_fast8_t len);
int_fast8_t usb_send_ep0_progmem(const void *data, uint_fast8_t len);
void usb_stall_ep0(void);
void usb_set_address(uint_fast8_t addr);
void usb_set_configure(void);
struct usb_string_descriptor *usbserial_get_serialid(void);

// usb_cdc.c
void usb_fill_serial(struct usb_string_descriptor *desc, int strlen, void *id);
void usb_notify_bulk_in(void);
void usb_notify_bulk_out(void);
void usb_notify_ep0(void);

#endif // usb_cdc.h
