/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_msc_data.c
 **
 ** \brief This file provides all the vital inquiry pages and sense data.
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/
/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_msc_data.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/* USB Mass storage Page 0 Inquiry Data */
const uint8_t MSC_Page00_Inquiry_Data[] = { //7
    0x00,
    0x00,
    0x00,
    (LENGTH_INQUIRY_PAGE00 - 4u),
    0x00,
    0x80,
    0x83
};
/* USB Mass storage sense 6  Data */
const uint8_t MSC_Mode_Sense6_data[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};
/* USB Mass storage sense 10  Data */
const uint8_t MSC_Mode_Sense10_data[] = {
    0x00,
    0x06,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
