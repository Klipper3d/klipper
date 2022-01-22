/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_def.h
 **
 ** A detailed description is available at
 ** @link general defines for the usb device library @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_DEF_H__
#define __USB_DEV_DEF_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_conf.h"
#include "usb_core_driver.h"
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifndef NULL
#define NULL    0u
#endif

#define  USB_LEN_DEV_QUALIFIER_DESC                     (0x0Au)
#define  USB_LEN_DEV_DESC                               (0x12u)
#define  USB_LEN_CFG_DESC                               (0x09u)
#define  USB_LEN_IF_DESC                                (0x09u)
#define  USB_LEN_EP_DESC                                (0x07u)
#define  USB_LEN_OTG_DESC                               (0x03u)

#define  LANGID_STR_IDX                                 (0x00u)
#define  MFC_STR_IDX                                    (0x01u)
#define  PRODUCT_STR_IDX                                (0x02u)
#define  SERIAL_STR_IDX                                 (0x03u)
#define  CONFIG_STR_IDX                                 (0x04u)
#define  INTERFACE_STR_IDX                              (0x05u)

#define  USB_REQ_TYPE_STANDARD                          (0x00u)
#define  USB_REQ_TYPE_CLASS                             (0x20u)
#define  USB_REQ_TYPE_VENDOR                            (0x40u)
#define  USB_REQ_TYPE_MASK                              (0x60u)

#define  USB_REQ_RECIPIENT_DEVICE                       (0x00u)
#define  USB_REQ_RECIPIENT_INTERFACE                    (0x01u)
#define  USB_REQ_RECIPIENT_ENDPOINT                     (0x02u)
#define  USB_REQ_RECIPIENT_MASK                         (0x03u)

#define  USB_REQ_GET_STATUS                             (0x00u)
#define  USB_REQ_CLEAR_FEATURE                          (0x01u)
#define  USB_REQ_SET_FEATURE                            (0x03u)
#define  USB_REQ_SET_ADDRESS                            (0x05u)
#define  USB_REQ_GET_DESCRIPTOR                         (0x06u)
#define  USB_REQ_SET_DESCRIPTOR                         (0x07u)
#define  USB_REQ_GET_CONFIGURATION                      (0x08u)
#define  USB_REQ_SET_CONFIGURATION                      (0x09u)
#define  USB_REQ_GET_INTERFACE                          (0x0Au)
#define  USB_REQ_SET_INTERFACE                          (0x0Bu)
#define  USB_REQ_SYNCH_FRAME                            (0x0Cu)

#define  USB_DESC_TYPE_DEVICE                              (1u)
#define  USB_DESC_TYPE_CONFIGURATION                       (2u)
#define  USB_DESC_TYPE_STRING                              (3u)
#define  USB_DESC_TYPE_INTERFACE                           (4u)
#define  USB_DESC_TYPE_ENDPOINT                            (5u)
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    (6u)
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           (7u)

#define USB_CONFIG_REMOTE_WAKEUP                           (2u)
#define USB_CONFIG_SELF_POWERED                            (1u)

#define USB_FEATURE_EP_HALT                                (0u)
#define USB_FEATURE_REMOTE_WAKEUP                          (1u)
#define USB_FEATURE_TEST_MODE                              (2u)

#define SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                              (uint16_t)(((uint16_t)(*(((uint8_t *)(addr)) + 1u))) << 8u))

#define LOBYTE(x)  ((uint8_t)((uint16_t)(x) & 0x00FFu))
#define HIBYTE(x)  ((uint8_t)(((uint16_t)(x) & 0xFF00u) >>8u))

#define VBUS_SENSING_ENABLED
/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/


#endif /* __USB_DEV_DEF_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
