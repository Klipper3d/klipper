// Copyright (C) 2026  Xiaoyue Cui <2508041672@qq.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#ifndef __GD32_USBFS_REGS_H
#define __GD32_USBFS_REGS_H

// The GD32F303 USBD block is register compatible with the classic
// STM32F1 full-speed device peripheral. Keep these definitions local so
// the STM32 device header does not conflict with the GD32 CMSIS header.
typedef struct {
    volatile uint16_t EP0R, RESERVED0;
    volatile uint16_t EP1R, RESERVED1;
    volatile uint16_t EP2R, RESERVED2;
    volatile uint16_t EP3R, RESERVED3;
    volatile uint16_t EP4R, RESERVED4;
    volatile uint16_t EP5R, RESERVED5;
    volatile uint16_t EP6R, RESERVED6;
    volatile uint16_t EP7R, RESERVED7[17];
    volatile uint16_t CNTR, RESERVED8;
    volatile uint16_t ISTR, RESERVED9;
    volatile uint16_t FNR, RESERVEDA;
    volatile uint16_t DADDR, RESERVEDB;
    volatile uint16_t BTABLE, RESERVEDC;
} gd32_usbd_t;

#define USB_BASE USBD_BASE
#define USB_PMAADDR USBD_RAM_BASE
#define USB ((gd32_usbd_t *)USB_BASE)

#define USB_EP_CTR_RX       0x8000U
#define USB_EP_DTOG_RX      0x4000U
#define USB_EPRX_STAT       0x3000U
#define USB_EP_SETUP        0x0800U
#define USB_EP_TYPE_MASK    0x0600U
#define USB_EP_KIND         0x0100U
#define USB_EP_CTR_TX       0x0080U
#define USB_EP_DTOG_TX      0x0040U
#define USB_EPTX_STAT       0x0030U
#define USB_EPADDR_FIELD    0x000fU

#define USB_EP_DTOG_TX_Pos  6U
#define USB_EP_DTOG_RX_Pos  14U

#define USB_EP_TX_STALL     0x0010U
#define USB_EP_TX_NAK       0x0020U
#define USB_EP_TX_VALID     0x0030U
#define USB_EP_RX_STALL     0x1000U
#define USB_EP_RX_NAK       0x2000U
#define USB_EP_RX_VALID     0x3000U

#define USB_EP_BULK         0x0000U
#define USB_EP_CONTROL      0x0200U
#define USB_EP_INTERRUPT    0x0600U

#define USB_CNTR_FRES       0x0001U
#define USB_CNTR_RESETM     0x0400U
#define USB_CNTR_CTRM       0x8000U

#define USB_ISTR_EP_ID      0x000fU
#define USB_ISTR_RESET      0x0400U
#define USB_ISTR_CTR        0x8000U

#define USB_DADDR_EF        0x0080U

#endif
