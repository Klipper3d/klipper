// Standard definitions for USB CDC devices
#ifndef __GENERIC_USBSTD_CDC_H
#define __GENERIC_USBSTD_CDC_H

#define USB_CDC_SUBCLASS_ACM 0x02

#define USB_CDC_ACM_PROTO_AT_V25TER 1

struct usb_cdc_header_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType;
    uint16_t bcdCDC;
} PACKED;

#define USB_CDC_HEADER_TYPE 0x00
#define USB_CDC_ACM_TYPE 0x02
#define USB_CDC_UNION_TYPE 0x06

#define USB_CDC_CS_INTERFACE 0x24
#define USB_CDC_CS_ENDPOINT 0x25

struct usb_cdc_acm_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType;
    uint8_t bmCapabilities;
} PACKED;

struct usb_cdc_union_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType;
    uint8_t bMasterInterface0;
    uint8_t bSlaveInterface0;
} PACKED;

#define USB_CDC_REQ_SET_LINE_CODING 0x20
#define USB_CDC_REQ_GET_LINE_CODING 0x21
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE 0x22

struct usb_cdc_line_coding {
    uint32_t dwDTERate;
    uint8_t bCharFormat;
    uint8_t bParityType;
    uint8_t bDataBits;
} PACKED;

#endif // usbstd_cdc.h
