/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_msc_scsi.h
 **
 ** A detailed description is available at
 ** @link
        Header file for usbh_msc_scsi.c
    @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_MSC_SCSI_H__
#define __USB_HOST_MSC_SCSI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_stdreq.h"


/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief USBH_MSC_SCSI_Exported_Types
 **
 ******************************************************************************/
typedef enum {
    USB_HOST_MSC_OK = 0u,
    USB_HOST_MSC_FAIL,
    USB_HOST_MSC_PHASE_ERROR,
    USB_HOST_MSC_BUSY
}USB_HOST_MSC_STATUS;

typedef struct 
{
    uint32_t MSC_Capacity;
    uint32_t MSC_Sense;
    uint16_t MSC_PageLength;
    uint8_t  MSC_BulkOutEP;
    uint8_t  MSC_BulkInEP;
    uint8_t  MSC_WriteProtect;
}MSC_PARAMETER;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_HOST_MSC_CMD_SEND              1
#define USB_HOST_MSC_CMD_WAIT              2

#define OPCODE_TEST_UNIT_READY            (0x00u)
#define OPCODE_READ_CAPACITY10            (0x25u)
#define OPCODE_MODE_SENSE6                (0x1Au)
#define OPCODE_READ10                     (0x28u)
#define OPCODE_WRITE10                    (0x2Au)
#define OPCODE_REQUEST_SENSE              (0x03u)

#define DESC_REQUEST_SENSE                (0x00u)
#define ALLOCATION_LENGTH_REQUEST_SENSE   (63u)
#define XFER_LEN_READ_CAPACITY10          (8u)
#define XFER_LEN_MODE_SENSE6              (63u)

#define MASK_MODE_SENSE_WRITE_PROTECT     (0x80u)
#define MODE_SENSE_PAGE_CONTROL_FIELD     (0x00u)
#define MODE_SENSE_PAGE_CODE              (0x3Fu)
#define DISK_WRITE_PROTECTED              (0x01u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern MSC_PARAMETER USB_HOST_MSC_Param;
/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern uint8_t usb_host_msc_TestUnitReady(usb_core_instance *pdev);
extern uint8_t usb_host_msc_ReadCapacity10(usb_core_instance *pdev);
extern uint8_t usb_host_msc_ModeSense6(usb_core_instance *pdev);
extern uint8_t usb_host_msc_RequestSense(usb_core_instance *pdev);
extern uint8_t usb_host_msc_Write10(usb_core_instance *pdev,
                                    uint8_t *dataBuffer,
                                    uint32_t address,
                                    uint32_t nbOfbytes);
extern uint8_t usb_host_msc_Read10(usb_core_instance *pdev,
                                   uint8_t *dataBuffer,
                                   uint32_t address,
                                   uint32_t nbOfbytes);

#endif  //__USB_HOST_MSC_SCSI_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


