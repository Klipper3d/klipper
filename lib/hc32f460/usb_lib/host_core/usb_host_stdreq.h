/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_stdreq.h
 **
 ** A detailed description is available at
 ** @link Header file for usbh_stdreq.c @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_STDREQ_H__
#define __USB_HOST_STDREQ_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_app_conf.h"
#include "usb_host_driver.h"
#include "usb_host_core.h"
#include "usb_host_stddef.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief USBH_STDREQ_Exported_Defines
 **
 ******************************************************************************/
/*Standard Feature Selector for clear feature command*/
#define FEATURE_SELECTOR_ENDPOINT         (0x00u)
#define FEATURE_SELECTOR_DEVICE           (0x01u)


#define INTERFACE_DESC_TYPE               (0x04u)
#define ENDPOINT_DESC_TYPE                (0x05u)
#define INTERFACE_DESC_SIZE               (0x09u)


#define USBH_HID_CLASS                    (0x03u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern HOST_STATUS usb_host_getdesc(usb_core_instance *pdev,
                                            USBH_HOST *phost,
                                              uint8_t req_type,
                                             uint16_t value_idx,
                                              uint8_t *buff,
                                             uint16_t length );

extern HOST_STATUS usb_host_getdevdesc(usb_core_instance *pdev, USBH_HOST *phost, uint8_t length);

extern HOST_STATUS usb_host_getstringdesc(usb_core_instance *pdev, USBH_HOST *phost, uint8_t string_index, uint8_t *buff, uint16_t length);

extern HOST_STATUS usb_host_setconfig(usb_core_instance *pdev, USBH_HOST *phost, uint16_t cfg_idx);

extern HOST_STATUS usb_host_getcfgdesc(usb_core_instance *pdev, USBH_HOST *phost, uint16_t  length);

extern HOST_STATUS usb_host_setdevaddr(usb_core_instance *pdev, USBH_HOST *phost, uint8_t DeviceAddress);

extern HOST_STATUS usb_host_clrfeature(usb_core_instance *pdev, USBH_HOST *phost, uint8_t ep_num, uint8_t hc_num);

extern HOST_STATUS usb_host_setintf(usb_core_instance *pdev, USBH_HOST *phost, uint8_t ep_num, uint8_t altSetting);

extern USB_HOST_DescHeader_TypeDef *usb_host_getnextdesc(uint8_t *pbuf, uint16_t *ptr);


#endif /* __USB_HOST_STDREQ_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
