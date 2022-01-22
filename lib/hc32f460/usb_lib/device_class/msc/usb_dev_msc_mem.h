/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_msc_mem.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_mem.c @endlink
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_MSC_MEM_H__
#define __USB_DEV_MSC_MEM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "usb_dev_def.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct
{
    int8_t (* Init)(uint8_t lun);
    int8_t (* GetCapacity)(uint8_t lun, uint32_t *block_num, uint32_t *block_size);
    int8_t (* GetMaxLun)(void);
    int8_t (* IsReady)(uint8_t lun);
    int8_t (* Read)(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
    int8_t (* Write)(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);    
    int8_t (* IsWriteProtected)(uint8_t lun);
    int8_t *pInquiry;
}USB_DEV_MSC_cbk_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define USB_DEV_INQUIRY_LENGTH     (36u)

extern USB_DEV_MSC_cbk_TypeDef *msc_fops;

#endif 

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
