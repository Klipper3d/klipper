/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_msc_scsi.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_scsi.c @endlink
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_MSC_SCSI_H__
#define __USB_DEV_MSC_SCSI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_def.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct _SENSE_ITEM
{
    char Skey;
    union
    {
        struct _ASCs
        {
            char ASC;
            char ASCQ;
        }b;
        unsigned int ASC;
        char *       pData;
    } w;
} SCSI_Sense_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define SENSE_LIST_DEEPTH                           (4u)

/* SCSI Commands */
#define SCSI_FORMAT_UNIT                            (0x04u)
#define SCSI_INQUIRY                                (0x12u)
#define SCSI_MODE_SELECT6                           (0x15u)
#define SCSI_MODE_SELECT10                          (0x55u)
#define SCSI_MODE_SENSE6                            (0x1Au)
#define SCSI_MODE_SENSE10                           (0x5Au)
#define SCSI_ALLOW_MEDIUM_REMOVAL                   (0x1Eu)
#define SCSI_READ6                                  (0x08u)
#define SCSI_READ10                                 (0x28u)
#define SCSI_READ12                                 (0xA8u)
#define SCSI_READ16                                 (0x88u)

#define SCSI_READ_CAPACITY10                        (0x25u)
#define SCSI_READ_CAPACITY16                        (0x9Eu)

#define SCSI_REQUEST_SENSE                          (0x03u)
#define SCSI_START_STOP_UNIT                        (0x1Bu)
#define SCSI_TEST_UNIT_READY                        (0x00u)
#define SCSI_WRITE6                                 (0x0Au)
#define SCSI_WRITE10                                (0x2Au)
#define SCSI_WRITE12                                (0xAAu)
#define SCSI_WRITE16                                (0x8Au)

#define SCSI_VERIFY10                               (0x2Fu)
#define SCSI_VERIFY12                               (0xAFu)
#define SCSI_VERIFY16                               (0x8Fu)

#define SCSI_SEND_DIAGNOSTIC                        (0x1Du)
#define SCSI_READ_FORMAT_CAPACITIES                 (0x23u)

#define NO_SENSE                                    (0u)
#define RECOVERED_ERROR                             (1u)
#define NOT_READY                                   (2u)
#define MEDIUM_ERROR                                (3u)
#define HARDWARE_ERROR                              (4u)
#define ILLEGAL_REQUEST                             (5u)
#define UNIT_ATTENTION                              (6u)
#define DATA_PROTECT                                (7u)
#define BLANK_CHECK                                 (8u)
#define VENDOR_SPECIFIC                             (9u)
#define COPY_ABORTED                                (10u)
#define ABORTED_COMMAND                             (11u)
#define VOLUME_OVERFLOW                             (13u)
#define MISCOMPARE                                  (14u)

#define INVALID_CDB                                 (0x20u)
#define INVALID_FIELED_IN_COMMAND                   (0x24u)
#define PARAMETER_LIST_LENGTH_ERROR                 (0x1Au)
#define INVALID_FIELD_IN_PARAMETER_LIST             (0x26u)
#define ADDRESS_OUT_OF_RANGE                        (0x21u)
#define MEDIUM_NOT_PRESENT                          (0x3Au)
#define MEDIUM_HAVE_CHANGED                         (0x28u)
#define WRITE_PROTECTED                             (0x27u)
#define UNRECOVERED_READ_ERROR                      (0x11u)
#define WRITE_FAULT                                 (0x03u)

#define READ_FORMAT_CAPACITY_DATA_LEN               (0x0Cu)
#define READ_CAPACITY10_DATA_LEN                    (0x08u)
#define MODE_SENSE10_DATA_LEN                       (0x08u)
#define MODE_SENSE6_DATA_LEN                        (0x04u)
#define REQUEST_SENSE_DATA_LEN                      (0x12u)
#define STANDARD_INQUIRY_DATA_LEN                   (0x24u)
#define BLKVFY                                      (0x04u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern SCSI_Sense_TypeDef SCSI_Sense [SENSE_LIST_DEEPTH];
extern uint8_t SCSI_Sense_Head;
extern uint8_t SCSI_Sense_Tail;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
extern int8_t scsi_cmdprocess(usb_core_instance *pdev, uint8_t lun, uint8_t *params);
extern void scsi_sensecode(uint8_t sKey, uint8_t ASC);

#endif /* __USB_DEV_MSC_SCSI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
