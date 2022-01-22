/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usbh_conf_template.h
 **
 ** A detailed description is available at
 ** @link General USB Host library configuration @endlink
 **
 **   - 2018-12-26  CDT First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USBH_CONF__H__
#define __USBH_CONF__H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USBH_MAX_NUM_ENDPOINTS                (2u)
#define USBH_MAX_NUM_INTERFACES               (2u)
#ifdef USE_USB_OTG_FS
#define USBH_MSC_MPS_SIZE                 (0x40)
#else
#define USBH_MSC_MPS_SIZE                 (0x200)
#endif

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/


#endif //__USBH_CONF__H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
