/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_cdc_ctrl.c
 **
 ** A detailed description is available at
 ** @link
        This file implements the CDC class control functions
    @endlink
 **
 **   - 2021-08-27  CDT First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_cdc_ctrl.h"

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
CDC_InterfaceDesc_Typedef         CDC_Desc;
CDC_LineCodingTypeDef             CDC_GetLineCode;
CDC_LineCodingTypeDef             CDC_SetLineCode;

extern CDC_Requests               CDC_ReqState;


/**
 *******************************************************************************
 ** \brief  host send ctrl request to get line coding 
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status define by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_cdc_getlinecoding(usb_core_instance *pdev , USBH_HOST *phost)
{
  phost->ctrlparam.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS | \
                              USB_REQ_RECIPIENT_INTERFACE;
  
  phost->ctrlparam.setup.b.bRequest = CDC_GET_LINE_CODING;
  phost->ctrlparam.setup.b.wValue.w = 0;
  phost->ctrlparam.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bControlInterface; /*At to be checked*/
  phost->ctrlparam.setup.b.wLength.w = LINE_CODING_STRUCTURE_SIZE;           
  
      
  return usb_host_ctrlreq(pdev, phost, CDC_GetLineCode.Array, LINE_CODING_STRUCTURE_SIZE);
}

/**
 *******************************************************************************
 ** \brief  host send ctrl request to set line coding
            to config Baud rate/Stop bits/Parity/Data bits
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status define by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_cdc_setlinecoding(usb_core_instance *pdev, USBH_HOST *phost)
{

  phost->ctrlparam.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | \
                              USB_REQ_RECIPIENT_INTERFACE;
  
  phost->ctrlparam.setup.b.bRequest = CDC_SET_LINE_CODING;
  phost->ctrlparam.setup.b.wValue.w = 0;

  phost->ctrlparam.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bControlInterface;

  phost->ctrlparam.setup.b.wLength.w = LINE_CODING_STRUCTURE_SIZE;           
  
  return usb_host_ctrlreq(pdev, phost, CDC_SetLineCode.Array , LINE_CODING_STRUCTURE_SIZE );  
}

/**
 *******************************************************************************
 ** \brief  host send ctrl request to set control line state
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status define by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_cdc_setcontrollinestate(usb_core_instance *pdev, USBH_HOST *phost)
{  
  phost->ctrlparam.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS | \
                              USB_REQ_RECIPIENT_INTERFACE;
  
  phost->ctrlparam.setup.b.bRequest = CDC_SET_CONTROL_LINE_STATE;
  
  phost->ctrlparam.setup.b.wValue.w = CDC_DEACTIVATE_CARRIER_SIGNAL_RTS | \
                          CDC_DEACTIVATE_SIGNAL_DTR;

  phost->ctrlparam.setup.b.wIndex.w = CDC_Desc.CDC_UnionFuncDesc.bControlInterface;

  phost->ctrlparam.setup.b.wLength.w = 0;           
  
  return usb_host_ctrlreq(pdev, phost, 0 , 0 );  
}

/**
 *******************************************************************************
 ** \brief  user call this function to issue set line coding request
            before call this funtion user need fill CDC_SetLineCode with config params
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval none
 ******************************************************************************/
void usb_host_cdc_issue_setlinecoding(usb_core_instance *pdev, USBH_HOST *phost)
{
  phost->host_state_backup = phost->host_state  ;
  phost->host_state = HOST_CLASS_REQ;
  CDC_ReqState = CDC_SET_LINE_CODING_RQUEST;
}

/**
 *******************************************************************************
 ** \brief  user call this function to issue get line coding request
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval none
 ******************************************************************************/
void usb_host_cdc_issue_getlinecoding(usb_core_instance *pdev, USBH_HOST *phost)
{
  phost->host_state_backup =  phost->host_state ;
  phost->host_state = HOST_CLASS_REQ;
  CDC_ReqState = CDC_GET_LINE_CODING_RQUEST;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

