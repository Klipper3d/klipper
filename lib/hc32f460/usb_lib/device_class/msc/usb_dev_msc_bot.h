/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_msc_bot.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_bot.c @endlink
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_MSC_BOT_H__
#define __USB_DEV_MSC_BOT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_core.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct
{
    uint32_t dSignature;
    uint32_t dTag;
    uint32_t dDataLength;
    uint8_t  bmFlags;
    uint8_t  bLUN;
    uint8_t  bCBLength;
    uint8_t  CB[16];
}MSC_BOT_CBW_TypeDef;

typedef struct
{
    uint32_t dSignature;
    uint32_t dTag;
    uint32_t dDataResidue;
    uint8_t  bStatus;
}MSC_BOT_CSW_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define BOT_IDLE                      (0u)       /* Idle state */
#define BOT_DATA_OUT                  (1u)       /* Data Out state */
#define BOT_DATA_IN                   (2u)       /* Data In state */
#define BOT_LAST_DATA_IN              (3u)       /* Last Data In Last */
#define BOT_SEND_DATA                 (4u)       /* Send Immediate data */

#define BOT_CBW_SIGNATURE             (0x43425355ul)
#define BOT_CSW_SIGNATURE             (0x53425355ul)
#define BOT_CBW_LENGTH                (31u)
#define BOT_CSW_LENGTH                (13u)

/* CSW Status Definitions */
#define CSW_CMD_PASSED                (0x00u)
#define CSW_CMD_FAILED                (0x01u)
#define CSW_PHASE_ERROR               (0x02u)

/* BOT Status */
#define BOT_STATE_NORMAL              (0u)
#define BOT_STATE_RECOVERY            (1u)
#define BOT_STATE_ERROR               (2u)

#define DIR_IN                        (0u)
#define DIR_OUT                       (1u)
#define BOTH_DIR                      (2u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern uint8_t  MSC_BOT_Data[MSC_MEDIA_PACKET];
extern uint16_t MSC_BOT_DataLen;
extern uint8_t  MSC_BOT_State;
extern uint8_t  MSC_BOT_BurstMode;
extern MSC_BOT_CBW_TypeDef MSC_BOT_cbw;
extern MSC_BOT_CSW_TypeDef MSC_BOT_csw;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
extern void msc_bot_init(usb_core_instance *pdev);
extern void msc_bot_rst(usb_core_instance *pdev);
extern void msc_bot_deinit(usb_core_instance *pdev);
extern void msc_bot_datain(usb_core_instance *pdev, uint8_t epnum);
extern void msc_bot_dataout(usb_core_instance *pdev, uint8_t epnum);
extern void msc_bot_txcsw(usb_core_instance *pdev, uint8_t CSW_Status);
extern void msc_bot_complete_clearfeature(usb_core_instance *pdev, uint8_t epnum);

#endif 

