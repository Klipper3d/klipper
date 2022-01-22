/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_cfgch.h
 **
 ** A detailed description is available at
 ** @link header file for the usbh_hcs.c @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_CFGCH_H__
#define __USB_HOST_CFGCH_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_core.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief USBH_HCS_Exported_Defines
 **
 ******************************************************************************/
#define MAX_CHNUM           (12u)

#define HC_OK            (0x0000u)
#define HC_USED          (0x8000u)
#define HC_ERROR         (0xFFFFu)
#define HC_USED_MASK     (0x7FFFu)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern uint8_t usb_host_distrch(usb_core_instance *pdev, uint8_t ep_addr);

extern uint8_t usb_host_freech(usb_core_instance *pdev, uint8_t idx);

extern void usb_host_dedistrallch(usb_core_instance *pdev);

extern void usb_host_chopen(usb_core_instance *pdev,
                                      uint8_t  hc_num,
                                      uint8_t  dev_address,
                                      uint8_t  speed,
                                      uint8_t  ep_type,
                                      uint16_t mps);

extern void usb_host_mdfch(usb_core_instance *pdev,
                                     uint8_t  hc_num,
                                     uint8_t  dev_address,
                                     uint8_t  speed,
                                     uint8_t  ep_type,
                                     uint16_t mps);

#endif /* __USB_HOST_CFGCH_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
