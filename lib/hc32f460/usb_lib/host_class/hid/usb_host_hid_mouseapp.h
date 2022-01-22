/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_hid_mouseapp.h
 **
 ** A detailed description is available at
 ** @link
        This file contains all the prototypes for the usbh_hid_mouse.c。
    @endlink
 **
 **   - 2019-12-13  CDT First version for USB hid mouse demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_HID_MOUSEAPP_H__
#define __USB_HOST_HID_MOUSEAPP_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_hid_class.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct _HID_MOUSE_Data
{
  uint8_t              z; 
  uint8_t              button;
  uint8_t              x;               /* Not Supported */ 
  uint8_t              y; 
}
HID_MOUSE_Data_TypeDef;

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern HID_cb_TypeDef HID_MOUSE_cb;
extern HID_MOUSE_Data_TypeDef	 HID_MOUSE_Data;

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void  USR_MOUSE_Init (void);
void  USR_MOUSE_ProcessData (HID_MOUSE_Data_TypeDef *data);


#endif /* __USB_HOST_HID_MOUSEAPP_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
