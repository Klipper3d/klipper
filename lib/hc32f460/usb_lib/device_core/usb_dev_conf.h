/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_conf.h
 **
 ** A detailed description is available at
 ** @link USB Device configuration file @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_CONF_H__
#define __USB_DEV_CONF_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_app_conf.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DEV_MAX_CFG_NUM                 (1u)
#define USB_MAX_STR_DESC_SIZ            (64u)
#define SELF_POWER                      (0u)

/**
 *******************************************************************************
 ** \brief USB_ZERO_Class_Layer_Parameter
 **
 ******************************************************************************/
#define HID_IN_PACKET                    (64u)
#define HID_OUT_PACKET                   (64u)

#define MAX_CDC_PACKET_SIZE              (64u)      /* IN & OUT Endpoint Packet size */
#define CDC_CMD_PACKET_SZE               (8u)       /* Control Endpoint Packet size */

#define CDC_IN_FRAME_INTERVAL            (5u)       /* Number of frames between IN transfers */
#define APP_RX_DATA_SIZE                 (2048u)    /* Total size of IN buffer*/
#define USB_MAX_STR_DESC_SIZ             (64u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
#define USBD_CLEAR                        0
#define USBD_SET                          1
#define USBD_GET                          2


#endif //__USB_DEV_CONF_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
