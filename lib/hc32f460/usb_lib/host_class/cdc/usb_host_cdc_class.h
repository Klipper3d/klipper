/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_cdc_class.h
 **
 ** A detailed description is available at
 ** @link
    This file contains all the prototypes for the usb_host_cdc_class.c
  @endlink
 **
 **   - 2021-08-27  CDT First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_CDC_CLASS_H__
#define __USB_HOST_CDC_CLASS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_cdc_ctrl.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
 
#define COMMUNICATION_DEVICE_CLASS_CODE                         (0x02)
#define COMMUNICATION_INTERFACE_CLASS_CODE                      (0x02)

#define DATA_INTERFACE_CLASS_CODE                               (0x0A)

#define RESERVED                                                (0x00)
#define DIRECT_LINE_CONTROL_MODEL                               (0x01)
#define ABSTRACT_CONTROL_MODEL                                  (0x02)
#define TELEPHONE_CONTROL_MODEL                                 (0x03)
#define MULTICHANNEL_CONTROL_MODEL                              (0x04)   
#define CAPI_CONTROL_MODEL                                      (0x05)
#define ETHERNET_NETWORKING_CONTROL_MODEL                       (0x06)
#define ATM_NETWORKING_CONTROL_MODEL                            (0x07)


#define NO_CLASS_SPECIFIC_PROTOCOL_CODE                         (0x00)
#define COMMON_AT_COMMAND                                       (0x01)
#define VENDOR_SPECIFIC                                         (0xFF)


#define CS_INTERFACE                                            (0x24)
#define CDC_PAGE_SIZE_64                                        (0x40)


/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

typedef enum
{
  CDC_IDLE= 0,
  CDC_READ_DATA,
  CDC_SEND_DATA,
  CDC_DATA_SENT,
  CDC_BUSY,
  CDC_GET_DATA,   
  CDC_POLL,
  CDC_CTRL_STATE
}
CDC_State;

typedef struct _CDCXfer
{
  volatile CDC_State CDCState;
  uint8_t* pRxTxBuff;
  uint8_t* pFillBuff;
  uint8_t* pEmptyBuff;
  uint32_t BufferLen;
  uint16_t DataLength;
} CDC_Xfer_TypeDef;

typedef struct CDC_UserCb
{
  void  (*Send)       (uint8_t  *);             
  void  (*Receive)    (uint8_t *, uint32_t len);       
  
} CDC_Usercb_TypeDef;

typedef struct _CDC_CommInterface
{
  uint8_t              hc_num_in; 
  uint8_t              hc_num_out;
  uint8_t              notificationEp;
  CDC_State            state; 
  uint8_t              buff[8];
  uint16_t             length;
  uint8_t              ep_addr;  
}
CDC_CommInterface_Typedef ;

typedef struct _CDC_DataInterface
{
  uint8_t              hc_num_in; 
  uint8_t              hc_num_out;
  uint8_t              cdcOutEp;
  uint8_t              cdcInEp;
  CDC_State            state; 
  uint8_t              buff[8];
  uint16_t             length;
  uint8_t              ep_addr;
}
CDC_DataInterface_Typedef ;

typedef struct _CDC_Process
{
  CDC_CommInterface_Typedef CDC_CommItf;
  CDC_DataInterface_Typedef CDC_DataItf;
}
CDC_Machine_TypeDef;

/*******************************************************************************
 * Global definitions ('extern')
 ******************************************************************************/
extern usb_host_class_callback_func  CDC_cb;

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void  usb_host_cdc_senddata(uint8_t *data, uint16_t length);
void  usb_host_cdc_enable_receive( usb_core_instance *pdev);
void  usb_host_cdc_disable_receive( usb_core_instance *pdev);


#endif /* __USB_HOST_CDC_CLASS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

