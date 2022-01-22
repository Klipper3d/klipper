/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usbd_msc_data.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_data.c @endlink
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_MSC_DATA_H__
#define __USB_DEV_MSC_DATA_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "usb_app_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define MODE_SENSE6_LEN                 (8u)
#define MODE_SENSE10_LEN                (8u)
#define LENGTH_INQUIRY_PAGE00           (7u)
#define LENGTH_FORMAT_CAPACITIES        (20u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern const uint8_t MSC_Page00_Inquiry_Data[LENGTH_INQUIRY_PAGE00];
extern const uint8_t MSC_Mode_Sense6_data[MODE_SENSE6_LEN];
extern const uint8_t MSC_Mode_Sense10_data[MODE_SENSE10_LEN];

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/

#endif /* __USB_DEV_MSC_DATA_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
