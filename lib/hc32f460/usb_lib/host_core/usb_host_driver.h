/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_driver.h
 **
 ** A detailed description is available at
 ** @link Host layer Header file @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_DRIVER_H__
#define __USB_HOST_DRIVER_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_core_regs.h"
#include "usb_core_driver.h"

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
extern void host_driver_init(usb_core_instance *pdev);
extern uint32_t host_driver_hostch_init(usb_core_instance *pdev, uint8_t hc_num);
extern uint32_t host_driver_submitrequest(usb_core_instance *pdev, uint8_t hc_num);
extern uint32_t host_driver_getcurrentspd (usb_core_instance *pdev);
extern uint32_t host_driver_getdmdpstate(usb_core_instance *pdev);
extern uint32_t host_driver_getvbusdrivestate(usb_core_instance *pdev);
extern void host_driver_portrst(usb_core_instance *pdev);
extern uint32_t host_driver_ifdevconnected(usb_core_instance *pdev);
extern uint32_t host_driver_getcurrentfrm(usb_core_instance *pdev);
extern HOST_CH_XFER_STATE host_driver_getxferstate(usb_core_instance *pdev, uint8_t ch_num);
extern uint32_t host_driver_getxfercnt(usb_core_instance *pdev, uint8_t ch_num);
extern HOST_CH_STATUS host_driver_gethostchstate(usb_core_instance *pdev,  uint8_t ch_num);

#endif //__USB_HOST_DRIVER_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


