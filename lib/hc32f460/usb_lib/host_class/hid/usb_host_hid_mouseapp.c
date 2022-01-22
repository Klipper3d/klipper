/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_hid_mouseapp.c
 **
 ** A detailed description is available at
 ** @link
        This file is the application layer for USB Host HID Mouse Handling.
    @endlink
 **
 **   - 2019-12-13  CDT First version for USB HID mouse demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_hid_mouseapp.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
void mouse_init(void);
void mouse_decode(uint8_t *data);

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#ifdef USB_INTERNAL_DMA_ENABLED
 #if defined   (__CC_ARM) /*!< ARM Compiler */
  __align(4)
 #elif defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
 #elif defined (__GNUC__) /*!< GNU Compiler */
 #pragma pack(4)
 #elif defined  (__TASKING__) /*!< TASKING Compiler */
  __align(4)
 #endif /* __CC_ARM */
#endif

HID_MOUSE_Data_TypeDef HID_MOUSE_Data;
HID_cb_TypeDef HID_MOUSE_cb =
{
    mouse_init,
    mouse_decode,
};

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  initialize the mouse application of the hid class
 ** \param  none
 ** \retval none
 ******************************************************************************/
void mouse_init(void)
{
    /* Call User Init*/
    USR_MOUSE_Init();
}

/**
 *******************************************************************************
 ** \brief  decode the mouse report data
 ** \param  data : buffer of the hid report data
 ** \retval none
 ******************************************************************************/
void mouse_decode(uint8_t *data)
{
    HID_MOUSE_Data.button = data[0];
    HID_MOUSE_Data.x      = data[1];
    HID_MOUSE_Data.y      = data[2];
    USR_MOUSE_ProcessData(&HID_MOUSE_Data);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
