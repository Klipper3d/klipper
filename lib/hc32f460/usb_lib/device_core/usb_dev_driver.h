/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_driver.h
 **
 ** A detailed description is available at
 ** @link Peripheral Driver Header file @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_DRIVER_H__
#define __USB_DEV_DRIVER_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_core_driver.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct
{
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
}EP_DESCRIPTOR , *PEP_DESCRIPTOR;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_EP_CONTROL                       (0u)
#define USB_EP_ISOC                          (1u)
#define USB_EP_BULK                          (2u)
#define USB_EP_INT                           (3u)
#define USB_EP_MASK                          (3u)

/*  Device Status */
#define USB_DEV_DEFAULT                          (1u)
#define USB_DEV_ADDRESSED                        (2u)
#define USB_DEV_CONFIGURED                       (3u)
#define USB_DEV_SUSPENDED                        (4u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern void hd_usb_initdev(usb_core_instance *pdev);
extern void hd_usb_conndevctrl(usb_core_instance *pdev, uint8_t link);
extern void hd_usb_addrset(usb_core_instance *pdev, uint8_t devaddr);
extern void hd_usb_opendevep(usb_core_instance *pdev, uint8_t ep_addr, uint16_t ep_mps, uint8_t ep_type);
extern void hd_usb_shutdevep(usb_core_instance *pdev , uint8_t ep_addr);
extern void hd_usb_readytorx(usb_core_instance *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t buf_len);
extern void hd_usb_ep0outstart(usb_core_instance *pdev);
extern void hd_usb_deveptx(usb_core_instance *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t buf_len);
extern void hd_usb_stalldevep(usb_core_instance *pdev, uint8_t epnum);
extern void hd_usb_clrstall(usb_core_instance *pdev, uint8_t epnum);
extern void hd_usb_flsdevep(usb_core_instance *pdev, uint8_t epnum);
extern uint32_t hd_usb_devepstatusget(usb_core_instance *pdev, uint8_t epnum);
extern void hd_usb_devepstatusset(usb_core_instance *pdev, uint8_t epnum, uint32_t Status);

#endif //__USB_DEV_DRIVER_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
