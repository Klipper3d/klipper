/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_ctrltrans.h
 **
 ** A detailed description is available at
 ** @link header file for the usbh_ioreq.c @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_CTRLTRANS_H__
#define __USB_HOST_CTRLTRANS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_app_conf.h"
#include "usb_host_core.h"
#include "usb_host_stddef.h"

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
extern void usb_host_sendctrlsetup(usb_core_instance *pdev, uint8_t *buff, uint8_t hc_num);
extern void usb_host_sendctrldata(usb_core_instance *pdev, uint8_t *buff, uint16_t length, uint8_t hc_num);
extern void usb_host_recvctrldata(usb_core_instance *pdev, uint8_t* buff, uint16_t length, uint8_t hc_num);
extern void usb_host_recvbulkdata( usb_core_instance *pdev, uint8_t *buff, uint16_t length, uint8_t hc_num);
extern void usb_host_sendbulkdata(usb_core_instance *pdev, uint8_t *buff, uint16_t length, uint8_t hc_num);
extern void usb_host_recvintdata(usb_core_instance *pdev, uint8_t *buff, uint16_t length, uint8_t hc_num);
extern void usb_host_sentintdata( usb_core_instance *pdev, uint8_t *buff, uint16_t length, uint8_t hc_num);
extern HOST_STATUS usb_host_ctrlreq (usb_core_instance *pdev, USBH_HOST *phost, uint8_t *buff, uint16_t length);
extern void usb_host_recvisocdata(usb_core_instance *pdev, uint8_t *buff, uint32_t length, uint8_t hc_num);
extern void usb_host_sendisocdata( usb_core_instance *pdev, uint8_t *buff, uint32_t length, uint8_t hc_num);

#endif /* __USB_HOST_CTRLTRANS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
