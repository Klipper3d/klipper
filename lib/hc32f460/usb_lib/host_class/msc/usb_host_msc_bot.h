/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usbh_msc_bot.h
 **
 ** A detailed description is available at
 ** @link
    Header file for usbh_msc_bot.c
  @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_MSC_BOT_H__
#define __USB_HOST_MSC_BOT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_stdreq.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */

/** @defgroup USBH_MSC_BOT
  * \brief This file is the Header file for usbh_msc_core.c
  * @{
  */


/** @defgroup USBH_MSC_BOT_Exported_Types
  * @{
  */

typedef union _USBH_CBW_Block
{
  struct __CBW
  {
    uint32_t CBWSignature;
    uint32_t CBWTag;
    uint32_t CBWTransferLength;
    uint8_t CBWFlags;
    uint8_t CBWLUN;
    uint8_t CBWLength;
    uint8_t CBWCB[16];
}field;
  uint8_t CBWArray[31];
}HostCBWPkt_TypeDef;

#define HOST_MSC_BOT_INIT_STATE        0
#define HOST_MSC_BOT_RESET             1
#define HOST_MSC_GET_MAX_LUN           2
#define HOST_MSC_TEST_UNIT_READY       3
#define HOST_MSC_READ_CAPACITY10       4
#define HOST_MSC_MODE_SENSE6           5
#define HOST_MSC_REQUEST_SENSE         6
#define HOST_MSC_BOT_USB_TRANSFERS     7
#define HOST_MSC_DEFAULT_APPLI_STATE   8
#define HOST_MSC_CTRL_ERROR_STATE      9
#define HOST_MSC_UNRECOVERED_STATE     10

typedef struct 
{
    uint8_t  MSCState;
    uint8_t  MSCStateBkp;
    uint8_t  MSCStateCurrent;
    uint8_t  CmdStateMachine;
    uint8_t  BOTState;
    uint8_t  BOTStateBkp;
    uint8_t  *pRxTxBuff;
    uint16_t DataLength;
    uint8_t  BOTXferStatus;
} USB_HOST_BOTXFER_TypeDef;


typedef union 
{
    struct 
    {
        uint32_t dCSWSignature;
        uint32_t dCSWTag;
        uint32_t dCSWDataResidue;
        uint8_t  dCSWStatus;
    }field;
    uint8_t      CSWArray[13];
}HOST_CSW_PACKET_TypeDef;


#define HOST_MSC_SEND_CBW                 1
#define HOST_MSC_SENT_CBW                 2
#define HOST_MSC_BOT_DATAIN_STATE         3
#define HOST_MSC_BOT_DATAOUT_STATE        4
#define HOST_MSC_RECEIVE_CSW_STATE        5
#define HOST_MSC_DECODE_CSW               6
#define HOST_MSC_BOT_ERROR_IN             7
#define HOST_MSC_BOT_ERROR_OUT            8


#define HOST_MSC_BOT_CBW_SIGNATURE        (0x43425355ul)
#define HOST_MSC_BOT_CBW_TAG              (0x20304050ul)
#define HOST_MSC_BOT_CSW_SIGNATURE        (0x53425355ul)
#define HOST_MSC_CSW_DATA_LENGTH          13
#define HOST_MSC_BOT_CBW_PACKET_LENGTH    31
#define HOST_MSC_CSW_LENGTH               13
#define HOST_MSC_CSW_MAX_LENGTH           63
#define HOST_MSC_DIR_IN                   0
#define HOST_MSC_DIR_OUT                  1
#define HOST_MSC_BOTH_DIR                 2
#define HOST_MSC_PAGE_LENGTH              512


#define CBW_CB_LENGTH                     16
#define CBW_LENGTH                        10
#define CBW_LENGTH_TEST_UNIT_READY        6
 

extern USB_HOST_BOTXFER_TypeDef USBH_MSC_BOTXferParam;
extern HostCBWPkt_TypeDef USBH_MSC_CBWData;
extern HOST_CSW_PACKET_TypeDef USBH_MSC_CSWData;


extern void usb_host_msc_init(usb_core_instance *pdev);
extern void usb_host_msc_botxferprocess(usb_core_instance *pdev, USBH_HOST *phost);
extern uint8_t usb_host_msc_cswdecode(usb_core_instance *pdev, USBH_HOST *phost);
extern HOST_STATUS usb_host_msc_botabort(usb_core_instance *pdev, USBH_HOST *phost, uint8_t direction);
/**
  * @}
  */

#endif  //__USB_HOST_MSC_BOT_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
