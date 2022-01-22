/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_msc_class.c
 **
 ** \brief USB MSC device example.
 **
 **   - 2019-05-15  1.0  CDT First version for USB MSC device demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_msc_mem.h"
#include "usb_dev_msc_class.h"
#include "usb_dev_msc_bot.h"
#include "usb_dev_stdreq.h"

uint8_t dev_msc_maxlun = 0u;
uint8_t dev_msc_altset = 0u;

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
void usb_dev_msc_init(void *pdev);
void usb_dev_msc_deinit(void *pdev);
uint8_t usb_dev_msc_setup(void *pdev, USB_SETUP_REQ *req);
void usb_dev_msc_datain(void *pdev, uint8_t epnum);
void usb_dev_msc_dataout(void *pdev, uint8_t epnum);
uint8_t *usb_dev_msc_getcfgdesc(uint16_t *length);

usb_dev_class_func usb_dev_msc_cbk =
{
    &usb_dev_msc_init,
    &usb_dev_msc_deinit,
    &usb_dev_msc_setup,
    NULL,                       
    NULL,                        
    &usb_dev_msc_getcfgdesc,
    NULL,                        
    &usb_dev_msc_datain,
    &usb_dev_msc_dataout,    
    NULL,
    NULL,
};

__USB_ALIGN_BEGIN uint8_t usb_dev_msc_cfgdesc[USB_MSC_CONFIG_DESC_SIZ] __USB_ALIGN_END =
{
    0x09,                        /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    USB_MSC_CONFIG_DESC_SIZ,
    0x00,
    0x01,                        /* bNumInterfaces: 1 interface */
    0x01,                        /* bConfigurationValue: */
    0x04,                        /* iConfiguration: */
    0xC0,                        /* bmAttributes: */
    0x32,                        /* MaxPower 100 mA */
    /********************  Mass Storage interface descriptor**********/
    0x09,                        /* bLength: Interface Descriptor size */
    0x04,                        /* bDescriptorType: */
    0x00,                        /* bInterfaceNumber: Number of Interface */
    0x00,                        /* bAlternateSetting: Alternate setting */
    0x02,                        /* bNumEndpoints*/
    0x08,                        /* bInterfaceClass: MSC Class */
    0x06,                        /* bInterfaceSubClass : SCSI transparent*/
    0x50,                        /* nInterfaceProtocol */
    0x05,                        /* iInterface: */
    /********************  Mass Storage Endpoints descriptor**********/
    0x07,                        /*Endpoint descriptor length = 7*/
    0x05,                        /*Endpoint descriptor type */
    MSC_IN_EP,                   /*Endpoint address (IN, address 1) */
    0x02,                        /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00,                        /*Polling interval in milliseconds */

    0x07,                        /*Endpoint descriptor length = 7 */
    0x05,                        /*Endpoint descriptor type */
    MSC_OUT_EP,                  /*Endpoint address (OUT, address 1) */
    0x02,                        /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00                         /*Polling interval in milliseconds*/
};

/**
 *******************************************************************************
 ** \brief  Initialize the mass storage interface configuration
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_init(void *pdev)
{
    usb_dev_msc_deinit(pdev);
    hd_usb_opendevep(pdev, MSC_IN_EP, MSC_EPIN_SIZE, USB_EP_BULK);
    hd_usb_opendevep(pdev, MSC_OUT_EP, MSC_EPOUT_SIZE, USB_EP_BULK);
    msc_bot_init(pdev);
}

/**
 *******************************************************************************
 ** \brief  deinitilaize  the mass storage interface configuration
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_deinit(void *pdev)
{
    hd_usb_shutdevep(pdev, MSC_IN_EP);
    hd_usb_shutdevep(pdev, MSC_OUT_EP);
    msc_bot_deinit(pdev);
}

/**
 *******************************************************************************
 ** \brief  process the msc setup requests
 ** \param  pdev : device instance
 ** \param  req : setup request
 ** \retval status
 ******************************************************************************/
uint8_t usb_dev_msc_setup(void *pdev, USB_SETUP_REQ *req)
{
    uint8_t u8Res = USBD_OK;
    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        case USB_REQ_TYPE_CLASS:
            switch (req->bRequest)
            {
                case BOT_GET_MAX_LUN:
                    if((req->wValue == (uint16_t)0) &&
                       (req->wLength == (uint16_t)1) &&
                       ((req->bmRequest & 0x80u) == (uint8_t)0x80))
                    {
                        dev_msc_maxlun = msc_fops->GetMaxLun();
                        if(dev_msc_maxlun > 0u)
                        {
                            hd_usb_ctrldatatx(pdev, &dev_msc_maxlun, 1u);
                        }
                        else
                        {
                            hd_usb_ctrlerr(pdev);
                            u8Res = USBD_FAIL;
                        }
                    }
                    else
                    {
                        hd_usb_ctrlerr(pdev);
                        u8Res = USBD_FAIL;
                    }
                    break;
                case BOT_RESET:
                    if((req->wValue == (uint16_t)0) &&
                       (req->wLength == (uint16_t)0) &&
                       ((req->bmRequest & 0x80u) != (uint8_t)0x80))
                    {
                        msc_bot_rst(pdev);
                    }
                    else
                    {
                        hd_usb_ctrlerr(pdev);
                        u8Res = USBD_FAIL;
                    }
                    break;
                default:
                    hd_usb_ctrlerr(pdev);
                    u8Res = USBD_FAIL;
                    break;
            }
            break;
        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_INTERFACE:
                    hd_usb_ctrldatatx(pdev, &dev_msc_altset, 1u);
                    break;
                case USB_REQ_SET_INTERFACE:
                    dev_msc_altset = (uint8_t)(req->wValue);
                    break;
                case USB_REQ_CLEAR_FEATURE:
                    hd_usb_flsdevep(pdev, (uint8_t)req->wIndex);
                    hd_usb_shutdevep(pdev, (uint8_t)req->wIndex);
                    if((((uint8_t)req->wIndex) & (uint16_t)0x80u) == (uint16_t)0x80)
                    {
                        hd_usb_opendevep(pdev, ((uint8_t)req->wIndex), MSC_EPIN_SIZE, USB_EP_BULK);
                    }
                    else
                    {
                        hd_usb_opendevep(pdev, ((uint8_t)req->wIndex), MSC_EPOUT_SIZE, USB_EP_BULK);
                    }
                    msc_bot_complete_clearfeature(pdev, (uint8_t)req->wIndex);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return u8Res;
}

/**
 *******************************************************************************
 ** \brief  process data IN DATA
 ** \param  pdev : device instance
 ** \param  epnum : endpoint index
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_datain(void *pdev, uint8_t epnum)
{
    msc_bot_datain(pdev, epnum);
}

/**
 *******************************************************************************
 ** \brief  process data OUT DATA
 ** \param  pdev : device instance
 ** \param  epnum : endpoint index
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_dataout(void *pdev, uint8_t epnum)
{
    msc_bot_dataout(pdev, epnum);
}

/**
 *******************************************************************************
 ** \brief  get the configuration descriptor
 ** \param  length : length of data butter in bytes
 ** \retval buffer pointer
 ******************************************************************************/
uint8_t *usb_dev_msc_getcfgdesc(uint16_t *length)
{
    *length = (uint16_t)sizeof(usb_dev_msc_cfgdesc);
    return usb_dev_msc_cfgdesc;
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
