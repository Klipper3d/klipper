/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_hid_cdc_wrapper.c
 **
 ** A detailed description is available at
 ** @link
        This file provides the MSC_HID composite functions.
  @endlink

 **   - 2019-11-19  1.0  CDT First version for USB MSC_HID composite demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_custom_hid_class.h"
#include "usb_dev_msc_class.h"
#include "usb_dev_hid_msc_wrapper.h"
#include "usb_dev_desc.h"
#include "usb_dev_stdreq.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/*********************************************
   MSC Device library callbacks
*********************************************/
extern void usb_dev_msc_init(void *pdev);
extern void usb_dev_msc_deinit(void *pdev);
extern uint8_t usb_dev_msc_setup(void *pdev, USB_SETUP_REQ *req);
extern void usb_dev_msc_datain(void *pdev, uint8_t epnum);
extern void usb_dev_msc_dataout(void *pdev, uint8_t epnum);
extern uint8_t *usb_dev_msc_getcfgdesc(uint16_t *length);
/*********************************************
   HID Device library callbacks
*********************************************/
extern void usb_dev_hid_init(void *pdev);
extern void usb_dev_hid_deinit(void *pdev);
extern uint8_t usb_dev_hid_setup(void *pdev, USB_SETUP_REQ *req);
extern uint8_t *usb_dev_hid_getcfgdesc(uint16_t *length);
extern void usb_dev_hid_datain(void *pdev, uint8_t epnum);
extern void usb_dev_hid_dataout(void *pdev, uint8_t epnum);
extern void usb_dev_hid_ctrlep_rxready(void *pdev);
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void usb_dev_msc_hid_init(void *pdev);
void usb_dev_msc_hid_deinit(void *pdev);
uint8_t usb_dev_msc_hid_setup(void *pdev, USB_SETUP_REQ *req);
void usb_dev_msc_hid_datain(void *pdev, uint8_t epnum);
void usb_dev_msc_hid_dataout(void *pdev , uint8_t epnum);
uint8_t *usb_dev_msc_hid_getcfgdesc(uint16_t *length);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

usb_dev_class_func usb_dev_composite_cbk =
{
    &usb_dev_msc_hid_init,
    &usb_dev_msc_hid_deinit,
    &usb_dev_msc_hid_setup,
    NULL, 
    NULL,
    &usb_dev_msc_hid_getcfgdesc,
    NULL, 
    &usb_dev_msc_hid_datain,
    &usb_dev_msc_hid_dataout, 
    NULL,
    NULL,
};

__USB_ALIGN_BEGIN static uint8_t usb_dev_msc_hid_cfgdesc[USB_MSC_HID_CONFIG_DESC_SIZ] __USB_ALIGN_END =
{
    0x09,                          /* bLength: Configuration Descriptor size */
    USB_CFG_DESCRIPTOR_TYPE,       /* bDescriptorType: Configuration */
    USB_MSC_HID_CONFIG_DESC_SIZ,   /* wTotalLength: Bytes returned */
    0x00,
    0x02,                          /*bNumInterfaces: 2 interface*/
    0x01,                          /*bConfigurationValue: Configuration value*/
    0x00,                          /*iConfiguration: Index of string descriptor describing the configuration*/
    0xE0,                          /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32,                          /*MaxPower 100 mA: this current is used for detecting Vbus*/
    /************** Descriptor of HID interface ****************/
    /* 09 */
    0x09,                          /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE, /*bDescriptorType: Interface descriptor type*/
    0x00,                          /*bInterfaceNumber: Number of Interface*/
    0x00,                          /*bAlternateSetting: Alternate setting*/
    0x02,                          /*bNumEndpoints*/
    0x03,                          /*bInterfaceClass: HID*/
    0x01,                          /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x02,                          /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                             /*iInterface: Index of string descriptor*/
    /******************** Descriptor of HID ********************/
    /* 18 */
    0x09,                          /*bLength: HID Descriptor size*/
    CUSTOM_HID_DESCRIPTOR_TYPE,    /*bDescriptorType: HID*/
    0x11,                          /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,                          /*bCountryCode: Hardware target country*/
    0x01,                          /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,                          /*bDescriptorType*/
    CUSTOM_HID_REPORT_DESC_SIZE,   /*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of HID endpoint ********************/
    /* 27 */
    0x07,                          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE,  /*bDescriptorType:*/
    HID_IN_EP,                     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                          /*bmAttributes: Interrupt endpoint*/
    HID_IN_PACKET,                 /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x0A,                          /*bInterval: Polling Interval (10 ms)*/
    /* 34 */
    0x07,                          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE,  /*bDescriptorType:*/
    HID_OUT_EP,                    /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                          /*bmAttributes: Interrupt endpoint*/
    HID_OUT_PACKET,                /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x0A,                          /*bInterval: Polling Interval (10 ms)*/
    /* 41 */
    /********************  Mass Storage interface ********************/
    0x09,                     /* bLength: Interface Descriptor size */
    0x04,                     /* bDescriptorType: */
    0x01,                     /* bInterfaceNumber: Number of Interface */
    0x00,                     /* bAlternateSetting: Alternate setting */
    0x02,                     /* bNumEndpoints*/
    0x08,                     /* bInterfaceClass: MSC Class */
    0x06,                     /* bInterfaceSubClass : SCSI transparent*/
    0x50,                     /* nInterfaceProtocol */
    0x05,                     /* iInterface: */
    /********************  Mass Storage Endpoints ********************/
    0x07,                     /*Endpoint descriptor length = 7*/
    0x05,                     /*Endpoint descriptor type */
    MSC_IN_EP,                /*Endpoint address (IN, address 1) */
    0x02,                     /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00,                     /*Polling interval in milliseconds */

    0x07,                     /*Endpoint descriptor length = 7 */
    0x05,                     /*Endpoint descriptor type */
    MSC_OUT_EP,               /*Endpoint address (OUT, address 1) */
    0x02,                     /*Bulk endpoint type */
    LOBYTE(MSC_MAX_PACKET),
    HIBYTE(MSC_MAX_PACKET),
    0x00                      /*Polling interval in milliseconds*/
} ;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  Initialize the composite device of MSC-HID interface
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_hid_init(void *pdev)
{
    usb_dev_hid_init(pdev);
    usb_dev_msc_init(pdev);
}

/**
 *******************************************************************************
 ** \brief  de-initialize the composite device of MSC-HID interface
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_hid_deinit(void *pdev)
{
    usb_dev_hid_deinit(pdev);
    usb_dev_msc_deinit(pdev);
}

/**
 *******************************************************************************
 ** \brief  process the setup requests
 ** \param  pdev : device instance
 ** \param  req : setup request
 ** \retval status
 ******************************************************************************/
uint8_t usb_dev_msc_hid_setup(void *pdev, USB_SETUP_REQ *req)
{
    uint8_t u8Res = USBD_OK;
    switch (req->bmRequest & USB_REQ_RECIPIENT_MASK)
    {
        case USB_REQ_RECIPIENT_INTERFACE:
            if (req->wIndex == 0x00)
            {
              u8Res = usb_dev_hid_setup(pdev, req);
            }
            else
            {
              u8Res = usb_dev_msc_setup(pdev, req);
            }
            break;

        case USB_REQ_RECIPIENT_ENDPOINT:
            if (req->wIndex == HID_IN_EP)
            {
              u8Res = usb_dev_hid_setup(pdev, req);
            }
            else
            {
              u8Res = usb_dev_msc_setup(pdev, req);
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
void usb_dev_msc_hid_datain(void *pdev, uint8_t epnum)
{
    if (epnum == (uint8_t)(MSC_IN_EP&((uint8_t)~0x80u)))
    {
        usb_dev_msc_datain(pdev, epnum);
    }
    else
    {
        usb_dev_hid_datain(pdev, epnum);
    }
}

/**
 *******************************************************************************
 ** \brief  process data OUT DATA
 ** \param  pdev : device instance
 ** \param  epnum : endpoint index
 ** \retval none
 ******************************************************************************/
void usb_dev_msc_hid_dataout(void *pdev , uint8_t epnum)
{
    if (epnum == (uint8_t)(MSC_OUT_EP&((uint8_t)~0x80u)))
    {
        usb_dev_msc_dataout(pdev, epnum);
    }
    else
    {
        usb_dev_hid_dataout(pdev, epnum);
    }
}

/**
 *******************************************************************************
 ** \brief  get the configuration descriptor
 ** \param  length : length of data butter in bytes
 ** \retval buffer pointer
 ******************************************************************************/
uint8_t *usb_dev_msc_hid_getcfgdesc(uint16_t *length)
{
    *length = (uint16_t)sizeof (usb_dev_msc_hid_cfgdesc);
    return usb_dev_msc_hid_cfgdesc;
}
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
