/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_cdc_class.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_cdc_core.c @endlink
 **
 **   - 2021-04-16  CDT   First version for USB CDC VCP demo.
 **
 ******************************************************************************/
/*******************************************************************************
 * Include files
 ******************************************************************************/
#ifndef __USB_DEV_CDC_CLASS_H__
#define __USB_DEV_CDC_CLASS_H__

#include  "usb_dev_ctrleptrans.h"
#include "cdc_data_process.h"
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_CDC_CONFIG_DESC_SIZ                (75u)
#define USB_CDC_DESC_SIZ                       (75u-9u)

#define CDC_DESCRIPTOR_TYPE                    (0x21u)

#define DEVICE_CLASS_CDC                       (0x02u)
#define DEVICE_SUBCLASS_CDC                    (0x00u)

#define STANDARD_ENDPOINT_DESC_SIZE            (0x09u)

#define MAX_CDC_IN_PACKET_SIZE                 (MAX_CDC_PACKET_SIZE)

#define MAX_CDC_OUT_PACKET_SIZE                (MAX_CDC_PACKET_SIZE)

/**************************************************/
/* CDC Requests                                   */
/**************************************************/
#define SEND_ENCAPSULATED_COMMAND               (0x00u)
#define GET_ENCAPSULATED_RESPONSE               (0x01u)
#define SET_COMM_FEATURE                        (0x02u)
#define GET_COMM_FEATURE                        (0x03u)
#define CLEAR_COMM_FEATURE                      (0x04u)
#define SET_LINE_CODING                         (0x20u)
#define GET_LINE_CODING                         (0x21u)
#define SET_CONTROL_LINE_STATE                  (0x22u)
#define SEND_BREAK                              (0x23u)
#define NO_CMD                                  (0xFFu)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

extern usb_dev_class_func  class_cdc_cbk;

extern void usb_dev_cdc_init(void *pdev);
extern void usb_dev_cdc_deinit(void *pdev);
extern uint8_t usb_dev_cdc_setup(void *pdev, USB_SETUP_REQ *req);
extern void usb_dev_cdc_ctrlep_rxready(void *pdev);
extern void usb_dev_cdc_datain(void *pdev, uint8_t epnum);
extern void usb_dev_cdc_dataout(void *pdev, uint8_t epnum);
extern uint8_t usb_dev_cdc_sof(void *pdev);
extern uint8_t *usb_dev_cdc_getcfgdesc(uint16_t *length);
#endif  // __USB_DEV_CDC_CLASS_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
