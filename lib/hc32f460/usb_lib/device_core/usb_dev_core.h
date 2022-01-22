/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_core.h
 **
 ** A detailed description is available at
 ** @link header file for the usb_dev_driver.c @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_CORE_H__
#define __USB_DEV_CORE_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_driver.h"
#include "usb_dev_def.h"
#include "usb_dev_conf.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USBD_OK     0
#define USBD_BUSY   1
#define USBD_FAIL   2
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern void hd_usb_dev_init(usb_core_instance *pdev,
               usb_dev_desc_func *pDevice,
               usb_dev_class_func *class_cb,
               usb_dev_user_func *usr_cb);
extern void hd_usb_dev_deinit(usb_core_instance *pdev);
extern void hd_usb_dev_ctrlconfig(usb_core_instance  *pdev, uint8_t cfgidx, uint8_t action);
#endif /* __USB_DEV_CORE_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
