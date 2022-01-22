/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_stdreq.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_req.c @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_STDREQ_H__
#define __USB_DEV_STDREQ_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_dev_core.h"
#include  "usb_dev_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void hd_usb_standarddevreq(usb_core_instance *pdev, USB_SETUP_REQ *req);
void hd_usb_standarditfreq(usb_core_instance *pdev, USB_SETUP_REQ *req);
void hd_usb_standardepreq(usb_core_instance *pdev, USB_SETUP_REQ *req);
void hd_usb_parsesetupreq(usb_core_instance *pdev, USB_SETUP_REQ *req);
void hd_usb_ctrlerr(usb_core_instance *pdev);
void hd_usb_getstring(uint8_t *desc, uint8_t *unicode, uint16_t *len);

#endif /* __USB_DEV_STDREQ_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
