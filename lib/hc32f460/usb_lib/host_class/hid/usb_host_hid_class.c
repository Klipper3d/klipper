/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_hid_class.c
 **
 ** A detailed description is available at
 ** @link
        This file is the HID Layer Handlers for USB Host HID class.
    @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_hid_class.h"
#include "usb_host_hid_mouseapp.h"
#include "usb_host_hid_keyboardapp.h"

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN HID_Machine_TypeDef        HID_Machine __USB_ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN HID_Report_TypeDef         HID_Report __USB_ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN usb_setup_typedef          HID_Setup __USB_ALIGN_END ;

#ifdef USB_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_INTERNAL_DMA_ENABLED */
__USB_ALIGN_BEGIN USB_HOST_HDIDesc_TypeDef       HID_Desc __USB_ALIGN_END ;

__IO uint8_t start_toggle = 0;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
HOST_STATUS usb_host_hid_itfinit(usb_core_instance *pdev, void *phost);
void usb_host_hid_itfdeinit(usb_core_instance *pdev);
void usb_host_parse_hiddesc(USB_HOST_HDIDesc_TypeDef *desc, uint8_t *buf);
HOST_STATUS usb_host_hid_process(usb_core_instance *pdev, void *phost);
HOST_STATUS usb_host_hid_classreq(usb_core_instance *pdev, void *phost);
HOST_STATUS usb_host_get_hidreportdesc(usb_core_instance *pdev, USBH_HOST *phost, uint16_t length);
HOST_STATUS usb_host_get_hiddesc(usb_core_instance *pdev, USBH_HOST *phost, uint16_t length);
HOST_STATUS usb_host_set_hididle(usb_core_instance *pdev,
                                 USBH_HOST *phost,
                                 uint8_t duration,
                                 uint8_t reportId);
HOST_STATUS usb_host_set_hidprotocol(usb_core_instance *pdev, USBH_HOST *phost, uint8_t protocol);


usb_host_class_callback_func  USBH_HID_cb =
{
    usb_host_hid_itfinit,
    usb_host_hid_itfdeinit,
    usb_host_hid_classreq,
    usb_host_hid_process
};


/**
 *******************************************************************************
 ** \brief  initialize the hid class
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status: defination by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_hid_itfinit(usb_core_instance *pdev, void *phost)
{
    uint8_t maxEP;
    USBH_HOST *pphost = phost;

    uint8_t epnum =0;
    HOST_STATUS status = HSTATUS_BUSY ;
    HID_Machine.state = HID_ERROR;

    if(pphost->device_prop.devitfdesc[0].bInterfaceSubClass  == HID_BOOT_CODE)
    {
        switch(pphost->device_prop.devitfdesc[0].bInterfaceProtocol)
        {
            case HID_MOUSE_BOOT_CODE:
                HID_Machine.cb = &HID_MOUSE_cb;
                break;
            case HID_KEYBRD_BOOT_CODE:
                HID_Machine.cb = &HID_KEYBRD_cb;
                break;
            default:
                break;
        }
        HID_Machine.state     = HID_IDLE;
        HID_Machine.ctl_state = HID_REQ_IDLE;
        HID_Machine.ep_addr   = pphost->device_prop.devepdesc[0][0].bEndpointAddress;
        HID_Machine.length    = pphost->device_prop.devepdesc[0][0].wMaxPacketSize;
        HID_Machine.poll      = pphost->device_prop.devepdesc[0][0].bInterval ;

        if (HID_Machine.poll  < HID_MIN_POLL)
        {
            HID_Machine.poll = HID_MIN_POLL;
        }


        /* Check fo available number of endpoints */
        /* Find the number of EPs in the Interface Descriptor */
        /* Choose the lower number in order not to overrun the buffer allocated */
        maxEP = ( (pphost->device_prop.devitfdesc[0].bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ?
                pphost->device_prop.devitfdesc[0].bNumEndpoints :
                    USBH_MAX_NUM_ENDPOINTS);

        /* Decode endpoint IN and OUT address from interface descriptor */
        for (epnum=0; epnum < maxEP; epnum++)
        {
            if(pphost->device_prop.devepdesc[0][epnum].bEndpointAddress & 0x80)
            {
                HID_Machine.HIDIntInEp = (pphost->device_prop.devepdesc[0][epnum].bEndpointAddress);
                HID_Machine.hc_num_in  = usb_host_distrch(pdev,
                                          pphost->device_prop.devepdesc[0][epnum].bEndpointAddress);
                usb_host_chopen(pdev,
                                HID_Machine.hc_num_in,
                                pphost->device_prop.devaddr,
                                pphost->device_prop.devspeed,
                                EP_TYPE_INTR,
                                HID_Machine.length);
            }
            else
            {
                HID_Machine.HIDIntOutEp = (pphost->device_prop.devepdesc[0][epnum].bEndpointAddress);
                HID_Machine.hc_num_out  = usb_host_distrch(pdev,
                                          pphost->device_prop.devepdesc[0][epnum].bEndpointAddress);
                usb_host_chopen(pdev,
                                HID_Machine.hc_num_out,
                                pphost->device_prop.devaddr,
                                pphost->device_prop.devspeed,
                                EP_TYPE_INTR,
                                HID_Machine.length);
            }
        }
        start_toggle =0;
        status = HSTATUS_OK;
    }
    else
    {
        pphost->user_callbk->huser_devunsupported();
    }

    return status;
}

/**
 *******************************************************************************
 ** \brief  deinitialize the channels for the hid class
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void usb_host_hid_itfdeinit(usb_core_instance *pdev)
{
    if(HID_Machine.hc_num_in != 0x00)
    {
        hd_usb_hchstop(pdev, HID_Machine.hc_num_in);
        usb_host_freech(pdev, HID_Machine.hc_num_in);
        HID_Machine.hc_num_in = 0;     
    }

    if(HID_Machine.hc_num_out != 0x00)
    {
        hd_usb_hchstop(pdev, HID_Machine.hc_num_out);
        usb_host_freech(pdev, HID_Machine.hc_num_out);
        HID_Machine.hc_num_out = 0;    
    }
    start_toggle = 0;
}

/**
 *******************************************************************************
 ** \brief  This function is responsible for handling requests about HID class.
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_hid_classreq(usb_core_instance *pdev, void *phost)
{
    HOST_STATUS status;
    HOST_STATUS classReqStatus;
    USBH_HOST *pphost = phost;
    status            = HSTATUS_BUSY;
    classReqStatus    = HSTATUS_BUSY;
    switch (HID_Machine.ctl_state)
    {
        case HID_IDLE:
        case HID_REQ_GET_HID_DESC:
            if (usb_host_get_hiddesc(pdev, pphost, USB_HID_DESC_SIZE)== HSTATUS_OK)
            {
              usb_host_parse_hiddesc(&HID_Desc, pdev->host.Rx_Buffer);
              HID_Machine.ctl_state = HID_REQ_GET_REPORT_DESC;
            }
            break;
        case HID_REQ_GET_REPORT_DESC:
            if (usb_host_get_hidreportdesc(pdev , pphost, HID_Desc.wItemLength) == HSTATUS_OK)
            {
              HID_Machine.ctl_state = HID_REQ_SET_IDLE;
            }
            break;
        case HID_REQ_SET_IDLE:
            classReqStatus = usb_host_set_hididle (pdev, pphost, 0, 0);
            if (classReqStatus == HSTATUS_OK)
            {
                HID_Machine.ctl_state = HID_REQ_SET_PROTOCOL;
            }
            else if(classReqStatus == HSTATUS_UNSUPPORTED)
            {
                HID_Machine.ctl_state = HID_REQ_SET_PROTOCOL;
            }
            break;
        case HID_REQ_SET_PROTOCOL:
            if (usb_host_set_hidprotocol (pdev ,pphost, 0) == HSTATUS_OK)
            {
                HID_Machine.ctl_state = HID_REQ_IDLE;
                status = HSTATUS_OK;
            }
            break;
        default:
            break;
    }
    return status;
}

/**
 *******************************************************************************
 ** \brief  This function is to process the state machine for HID data transmitting
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_hid_process(usb_core_instance *pdev, void *phost)
{
    HOST_STATUS status;
    USBH_HOST *pphost = phost;
    status = HSTATUS_OK;
    switch (HID_Machine.state)
    {
        case HID_IDLE:
          HID_Machine.cb->Init();
          HID_Machine.state = HID_SYNC;
        case HID_SYNC:
          if(hd_usb_ifevenframe(pdev) == TRUE)
          {
              HID_Machine.state = HID_GET_DATA;
          }
          break;
        case HID_GET_DATA:
            usb_host_recvintdata(pdev,
                                 HID_Machine.buff,
                                 HID_Machine.length,
                                 HID_Machine.hc_num_in);
            start_toggle = 1;
            HID_Machine.state = HID_POLL;
            HID_Machine.timer = host_driver_getcurrentfrm(pdev);
            break;
        case HID_POLL:
            if(( host_driver_getcurrentfrm(pdev) - HID_Machine.timer) >= HID_Machine.poll)
            {
                HID_Machine.state = HID_GET_DATA;
            }
            else if(host_driver_getxferstate(pdev , HID_Machine.hc_num_in) == HOST_CH_XFER_DONE)
            {
                if(start_toggle == 1) 
                {
                    start_toggle = 0;
                    HID_Machine.cb->Decode(HID_Machine.buff);
                }
            }
            else if(host_driver_getxferstate(pdev, HID_Machine.hc_num_in) == HOST_CH_XFER_STALL) 
            {
                /* Issue Clear Feature on interrupt IN endpoint */
                if( (usb_host_clrfeature(pdev,
                                    pphost,
                                    HID_Machine.ep_addr,
                                    HID_Machine.hc_num_in)) == HSTATUS_OK)
                {
                    HID_Machine.state = HID_GET_DATA;
                }

            }
            break;
        default:
            break;
    }
    return status;
}

/**
 *******************************************************************************
 ** \brief  Issue hid report descriptor command to the device, parse the report 
 **         descriptor and update the status once the response received.
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \param  length : the length of the hid descriptor in bytes
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_get_hidreportdesc(usb_core_instance *pdev, USBH_HOST *phost, uint16_t length)
{
    HOST_STATUS status;
    status = usb_host_getdesc(pdev,
                              phost,
                              USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,
                              USB_DESC_HID_REPORT,
                              pdev->host.Rx_Buffer,
                              length);
    return status;
}

/**
 *******************************************************************************
 ** \brief  Issue HID descriptor command to the device, parse the descriptor 
 **         and update the status once the response received.
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \param  length : length of HID descriptor in bytes
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_get_hiddesc(usb_core_instance *pdev, USBH_HOST *phost, uint16_t length)
{
    HOST_STATUS status;
    status = usb_host_getdesc(pdev,
                                phost,
                                USB_REQ_RECIPIENT_INTERFACE
                                  | USB_REQ_TYPE_STANDARD,
                                  USB_DESC_HID,
                                  pdev->host.Rx_Buffer,
                                  length);
    return status;
}

/**
 *******************************************************************************
 ** \brief  Set hid application to be idle status.
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \param  duration: Duration for HID Idle request
 ** \param  reportID : Targetted report ID for Set Idle request
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_set_hididle(usb_core_instance *pdev,
                                 USBH_HOST *phost,
                                 uint8_t duration,
                                 uint8_t reportId)
{
    phost->ctrlparam.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;

    phost->ctrlparam.setup.b.bRequest = USB_HID_SET_IDLE;
    phost->ctrlparam.setup.b.wValue.w = (duration << 8 ) | reportId;

    phost->ctrlparam.setup.b.wIndex.w = 0;
    phost->ctrlparam.setup.b.wLength.w = 0;

    return usb_host_ctrlreq(pdev, phost, 0 , 0 );
}

/**
 *******************************************************************************
 ** \brief  Issues a command to set report
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \param  reportType  : the report type to be sent
 ** \param  reportID    : target report ID for set report request
 ** \param  reportLen   : length of data of report to be sent in bytes
 ** \param  reportBuff  : report buffer
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_set_hidreport(usb_core_instance *pdev,
                                   USBH_HOST *phost,
                                   uint8_t reportType,
                                   uint8_t reportId,
                                   uint8_t reportLen,
                                   uint8_t* reportBuff)
{
    phost->ctrlparam.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
    phost->ctrlparam.setup.b.bRequest = USB_HID_SET_REPORT;
    phost->ctrlparam.setup.b.wValue.w = (reportType << 8 ) | reportId;
    phost->ctrlparam.setup.b.wIndex.w = 0;
    phost->ctrlparam.setup.b.wLength.w = reportLen;
    return usb_host_ctrlreq(pdev, phost, reportBuff , reportLen );
}

/**
 *******************************************************************************
 ** \brief  Set protocol state of the hid application.
 ** \param  pdev : device instance
 ** \param  phost: host state set
 ** \param  protocol : protocol for HID application
 ** \retval status defined by HOST_STATUS
 ******************************************************************************/
HOST_STATUS usb_host_set_hidprotocol(usb_core_instance *pdev, USBH_HOST *phost, uint8_t protocol)
{
    phost->ctrlparam.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
    phost->ctrlparam.setup.b.bRequest = USB_HID_SET_PROTOCOL;
    if(protocol != 0)
    {
        // Boot Protocol
        phost->ctrlparam.setup.b.wValue.w = 0;
    }
    else
    {
        //Report Protocol
        phost->ctrlparam.setup.b.wValue.w = 1;
    }
    phost->ctrlparam.setup.b.wIndex.w = 0;
    phost->ctrlparam.setup.b.wLength.w = 0;
    return usb_host_ctrlreq(pdev, phost, 0 , 0 );

}

/**
 *******************************************************************************
 ** \brief  parse the HID descriptor
 ** \param  desc: used to get the fields parsed from the data buffer
 ** \param  buf: Buffer where the source descriptor is available
 ** \retval none
 ******************************************************************************/
void usb_host_parse_hiddesc(USB_HOST_HDIDesc_TypeDef *desc, uint8_t *buf)
{
    desc->bLength                  = *(uint8_t  *) (buf + 0);
    desc->bDescriptorType          = *(uint8_t  *) (buf + 1);
    desc->bcdHID                   =  SMALL_END(buf + 2);
    desc->bCountryCode             = *(uint8_t  *) (buf + 4);
    desc->bNumDescriptors          = *(uint8_t  *) (buf + 5);
    desc->bReportDescriptorType    = *(uint8_t  *) (buf + 6);
    desc->wItemLength              =  SMALL_END(buf + 7);
}

/*******************************************************************************
 * EOF 
 ******************************************************************************/
