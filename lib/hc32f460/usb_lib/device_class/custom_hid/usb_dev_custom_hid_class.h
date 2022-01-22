/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_custom_hid_class.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_customhid_core.c @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_CUSTOMHID_HID_CORE_H__
#define __USB_DEV_CUSTOMHID_HID_CORE_H__

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
/**
 *******************************************************************************
 ** \brief 
 **
 ******************************************************************************/
#define CUSTOM_HID_REPORT_DESC_SIZE         0x21

#define USB_CUSTOM_HID_CONFIG_DESC_SIZ      (41u)
#define USB_CUSTOM_HID_DESC_SIZ             (9u)

#define CUSTOM_HID_DESCRIPTOR_TYPE          (0x21u)
#define CUSTOM_HID_REPORT_DESC              (0x22u)

#define CUSTOM_HID_REQ_SET_PROTOCOL         (0x0Bu)
#define CUSTOM_HID_REQ_GET_PROTOCOL         (0x03u)

#define CUSTOM_HID_REQ_SET_IDLE             (0x0Au)
#define CUSTOM_HID_REQ_GET_IDLE             (0x02u)

#define CUSTOM_HID_REQ_SET_REPORT           (0x09u)
#define CUSTOM_HID_REQ_GET_REPORT           (0x01u)

#define LED1_REPORT_ID                      (0x01u)
#define LED1_REPORT_COUNT                   (0x01u)

#define LED2_REPORT_ID                      (0x02u)
#define LED2_REPORT_COUNT                   (0x01u)

#define LED3_REPORT_ID                      (0x03u)
#define LED3_REPORT_COUNT                   (0x01u)

#define LED4_REPORT_ID                      (0x04u)
#define LED4_REPORT_COUNT                   (0x01u)

#define KEY_REPORT_ID                       (0x05u)
#define TAMPER_REPORT_ID                    (0x06u)
#define ADC_REPORT_ID                       (0x07u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern usb_dev_class_func  class_customhid_cbk;
extern uint8_t Report_buf[2u];
extern uint8_t Send_Buf[2u];

extern void usb_dev_hid_init(void *pdev);
extern void usb_dev_hid_deinit(void *pdev);
extern uint8_t usb_dev_hid_setup(void *pdev, USB_SETUP_REQ *req);
extern uint8_t *usb_dev_hid_getcfgdesc(uint16_t *length);
extern void usb_dev_hid_datain(void *pdev, uint8_t epnum);
extern void usb_dev_hid_dataout(void *pdev, uint8_t epnum);
extern uint8_t usb_dev_hid_txreport(usb_core_instance *pdev, uint8_t *report, uint16_t len);

#endif   //__USB_DEV_CUSTOMHID_HID_CORE_H__  

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

