/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_msc_class.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_core.c @endlink
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_MSC_CLASS_H__
#define __USB_DEV_MSC_CLASS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_dev_ctrleptrans.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define BOT_GET_MAX_LUN              (0xFEu)
#define BOT_RESET                    (0xFFu)
#define USB_MSC_CONFIG_DESC_SIZ      (32u)

#define MSC_EPIN_SIZE                (MSC_MAX_PACKET)
#define MSC_EPOUT_SIZE               (MSC_MAX_PACKET)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
extern usb_dev_class_func usb_dev_msc_cbk;

#endif  /* __USB_DEV_MSC_CLASS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
