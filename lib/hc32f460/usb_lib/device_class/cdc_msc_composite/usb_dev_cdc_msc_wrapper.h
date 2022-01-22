/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_hid_msc_wrapper.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_hid_msc_wrapper.c @endlink
 **
 **   - 2019-11-19  1.0  CDT First version for MSC_HID composite demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_CDC_MSC_WRAPPER_H__
#define __USB_DEV_CDC_MSC_WRAPPER_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_dev_ctrleptrans.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define USB_MSC_CDC_CONFIG_DESC_SIZ  (USB_CDC_CONFIG_DESC_SIZ -9u + USB_MSC_CONFIG_DESC_SIZ)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern usb_dev_class_func  usb_dev_composite_cbk;

#endif  /* __USB_DEV_CDC_MSC_WRAPPER_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
