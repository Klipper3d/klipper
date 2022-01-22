/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_houst_hid_keybouarapp.h
 **
 ** A detailed description is available at
 ** @link
        This file contains all the prototypes for the usbh_hid_keybd.c.
    @endlink
 **
 **   - 2019-12-13  CDT First version for USB mouse_keyboard demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_HID_KEYBOARDAPP_H__
#define __USB_HOST_HID_KEYBOARDAPP_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_app_conf.h"
#include "usb_host_hid_class.h"
#include "usb_host_user.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define QWERTY_KEYBOARD
//#define AZERTY_KEYBOARD

#define  KBD_LEFT_CTRL                                  0x01
#define  KBD_LEFT_SHIFT                                 0x02
#define  KBD_LEFT_ALT                                   0x04
#define  KBD_LEFT_GUI                                   0x08
#define  KBD_RIGHT_CTRL                                 0x10
#define  KBD_RIGHT_SHIFT                                0x20
#define  KBD_RIGHT_ALT                                  0x40
#define  KBD_RIGHT_GUI                                  0x80

#define  KBR_MAX_NBR_PRESSED                            6

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern HID_cb_TypeDef HID_KEYBRD_cb;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/


#endif /* __USB_HOST_HID_KEYBOARDAPP_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
