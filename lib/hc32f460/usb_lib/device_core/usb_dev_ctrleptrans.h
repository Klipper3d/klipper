/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_ctrleptrans.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_ioreq.c file @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_CTRLEPTRANS_H__
#define __USB_DEV_CTRLEPTRANS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include  "usb_dev_def.h"
#include  "usb_dev_core.h"


/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern void hd_usb_ctrldatatx(usb_core_instance  *pdev, uint8_t *pbuf, uint16_t len);
extern void hd_usb_ctrldatacontinuetx(usb_core_instance *pdev, uint8_t *pbuf, uint16_t len);
extern void hd_usb_ctrldatarx(usb_core_instance  *pdev, uint8_t *pbuf, uint16_t len);
extern void hd_usb_ctrldatacontinuerx (usb_core_instance *pdev, uint8_t *pbuf, uint16_t len);
extern void hd_usb_ctrlstatustx(usb_core_instance *pdev);
extern void hd_usb_ctrlstatusrx(usb_core_instance *pdev);
extern uint16_t hd_usb_getrxcnt(usb_core_instance *pdev, uint8_t epnum);
#endif /* __USB_DEV_CTRLEPTRANS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
