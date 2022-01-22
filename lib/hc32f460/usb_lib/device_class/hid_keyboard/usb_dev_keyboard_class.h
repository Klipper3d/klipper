/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_mouse_class.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_hid_core.c @endlink
 **
 **   - 2018-12-26  1.0  CDT First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_KEYBOARD_CLASS_H__
#define __USB_DEV_KEYBOARD_CLASS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_dev_ctrleptrans.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define KEYBOARD_CFGDESC_SIZE              (41u)
#define KEYBOARD_DESC_SIZE                 (9u)
#define KEYBOARD_REPORT_DESC_SIZE          (66u)

#define KEYBOARD_DESCRIPTOR_TYPE           (0x21u)
#define KEYBOARD_REPORT_DESC               (0x22u)


#define KEYBOARD_REQ_SET_PROTOCOL          (0x0Bu)
#define KEYBOARD_REQ_GET_PROTOCOL          (0x03u)

#define KEYBOARD_REQ_SET_IDLE              (0x0Au)
#define KEYBOARD_REQ_GET_IDLE              (0x02u)


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern usb_dev_class_func  usb_dev_keyboard_cbk;
/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern uint8_t usb_dev_mouse_txreport(usb_core_instance *pdev, uint8_t *report, uint16_t len);

#endif  // __USB_DEV_KEYBOARD_CLASS_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

