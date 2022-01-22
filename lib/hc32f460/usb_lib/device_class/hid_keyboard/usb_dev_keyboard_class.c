/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_keyboard_class.c
 **
 ** A detailed description is available at
 ** @link
        This file provides the HID core functions.
    @endlink
 **
 **   - 2021-05-19  1.0  CDT First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_keyboard_class.h"
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

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void usb_dev_keyboard_init(void *pdev);
void usb_dev_keyboard_deinit(void *pdev);
uint8_t usb_dev_keyboard_setup(void *pdev, USB_SETUP_REQ *req);
uint8_t *usb_dev_keyboard_getcfgdesc(uint16_t *length);
void usb_dev_keyboard_datain(void *pdev, uint8_t epnum);
void usb_dev_keyboard_dataout(void *pdev, uint8_t epnum);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
usb_dev_class_func  usb_dev_keyboard_cbk =
{
    &usb_dev_keyboard_init,
    &usb_dev_keyboard_deinit,
    &usb_dev_keyboard_setup,
    NULL,
    NULL,
    &usb_dev_keyboard_getcfgdesc,
    NULL,
    &usb_dev_keyboard_datain,
    &usb_dev_keyboard_dataout,
    NULL,
    NULL
};

__USB_ALIGN_BEGIN static uint32_t  USB_DEV_KEYBOARD_AltSet  __USB_ALIGN_END = 0u;
__USB_ALIGN_BEGIN static uint32_t  USB_DEV_KEYBOARD_Protocol  __USB_ALIGN_END = 0u;
__USB_ALIGN_BEGIN static uint32_t  USB_DEV_KEYBOARD_IdleState __USB_ALIGN_END = 0u;

/* USB HID device Configuration Descriptor */
__USB_ALIGN_BEGIN static uint8_t USB_DEV_KEYBOARD_CfgDesc[KEYBOARD_CFGDESC_SIZE] __USB_ALIGN_END =
{
    0x09,                             /* bLength: Configuration Descriptor size */
    USB_CFG_DESCRIPTOR_TYPE,          /* bDescriptorType: Configuration */
    KEYBOARD_CFGDESC_SIZE,               /* wTotalLength: Bytes returned */
    0x00,
    0x01,                             /*bNumInterfaces: 1 interface*/
    0x01,                             /*bConfigurationValue: Configuration value*/
    0x00,                             /*iConfiguration: Index of string descriptor describing the configuration*/
    0x80,                             /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32,                             /*MaxPower 100 mA: this current is used for detecting Vbus*/
    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                             /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,    /*bDescriptorType: Interface descriptor type*/
    0x00,                             /*bInterfaceNumber: Number of Interface*/
    0x00,                             /*bAlternateSetting: Alternate setting*/
    0x02,                             /*bNumEndpoints*/
    0x03,                             /*bInterfaceClass: HID*/
    0x01,                             /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,                             /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0x00,                             /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                             /*bLength: HID Descriptor size*/
    KEYBOARD_DESCRIPTOR_TYPE,            /*bDescriptorType: HID*/
    0x10,                             /*bcdHID: HID Class Spec release number*/
    0x01,
    0x21,                             /*bCountryCode: Hardware target country*/
    0x01,                             /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,                             /*bDescriptorType*/
    KEYBOARD_REPORT_DESC_SIZE,           /*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                             /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /*bDescriptorType:*/
    HID_IN_EP,                        /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                             /*bmAttributes: Interrupt endpoint*/
    0x08,                             /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x0A,                             /*bInterval: Polling Interval (10 ms)*/
    /* 34 */
    0x07,                             /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /*bDescriptorType:*/
    HID_OUT_EP,                       /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                             /*bmAttributes: Interrupt endpoint*/
    HID_OUT_PACKET,                   /*wMaxPacketSize: 4 Byte max */
    0x00,
    0x0A,                             /*bInterval: Polling Interval (10 ms)*/
    /* 41 */
} ;

__USB_ALIGN_BEGIN static uint8_t HID_KEYBOARD_ReportDesc[KEYBOARD_REPORT_DESC_SIZE] __USB_ALIGN_END =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x00,                    //   USAGE_MAXIMUM (Reserved (no event indicated))
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0x15, 0x01,                    //   LOGICAL_MINIMUM (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0xc0                           // END_COLLECTION
};


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
__IO uint8_t u8KeyRevBuf[64];
/**
 *******************************************************************************
 ** \brief  Initialize the interface of KEYBOARD HID
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void usb_dev_keyboard_init(void *pdev)
{
    hd_usb_opendevep(pdev, HID_IN_EP, 8, USB_EP_INT);
    hd_usb_opendevep(pdev, HID_OUT_EP, HID_OUT_PACKET, USB_EP_INT);
    hd_usb_readytorx(pdev, HID_OUT_EP, (uint8_t *)u8KeyRevBuf, 64);
    hd_usb_devepstatusset(pdev, HID_OUT_EP, USB_EP_RX_VALID);
}

/**
 *******************************************************************************
 ** \brief  DeInitialize the interface of KEYBOARD HID
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void usb_dev_keyboard_deinit(void *pdev)
{
    hd_usb_shutdevep(pdev, HID_IN_EP);
    hd_usb_shutdevep(pdev, HID_OUT_EP);
}

/**
 *******************************************************************************
 ** \brief  Handle the KEYBOARD HID SETUP request
 ** \param  pdev: device instance
 ** \param  req: usb requests
 ** \retval status
 ******************************************************************************/
uint8_t usb_dev_keyboard_setup(void *pdev, USB_SETUP_REQ *req)
{
    uint16_t   len = 0u;
    uint8_t  *pbuf = NULL;
    uint8_t  u8Res = USBD_OK;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        case USB_REQ_TYPE_CLASS :
            switch (req->bRequest)
            {
                case KEYBOARD_REQ_SET_PROTOCOL:
                    USB_DEV_KEYBOARD_Protocol = (uint8_t)(req->wValue);
                    break;
                case KEYBOARD_REQ_GET_PROTOCOL:
                    hd_usb_ctrldatatx(pdev, (uint8_t *)&USB_DEV_KEYBOARD_Protocol, 1u);
                    break;
                case KEYBOARD_REQ_SET_IDLE:
                    USB_DEV_KEYBOARD_IdleState = (uint8_t)(req->wValue >> 8u);
                    break;
                case KEYBOARD_REQ_GET_IDLE:
                    hd_usb_ctrldatatx(pdev, (uint8_t *)&USB_DEV_KEYBOARD_IdleState, 1u);
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
                case USB_REQ_GET_DESCRIPTOR:
                    if((req->wValue >> 8u) == (uint16_t)KEYBOARD_REPORT_DESC)
                    {
                        len  = (uint16_t)__MIN(KEYBOARD_REPORT_DESC_SIZE , req->wLength);
                        pbuf = HID_KEYBOARD_ReportDesc;
                    }
                    else if((req->wValue >> 8u) == (uint16_t)KEYBOARD_DESCRIPTOR_TYPE)
                    {
                        pbuf = USB_DEV_KEYBOARD_CfgDesc + 0x12;
                        len  = (uint16_t)__MIN(KEYBOARD_DESC_SIZE , req->wLength);
                    }
                    else
                    {
                        //
                    }
                    hd_usb_ctrldatatx(pdev, pbuf, len);
                    break;
                case USB_REQ_GET_INTERFACE :
                    hd_usb_ctrldatatx(pdev, (uint8_t *)&USB_DEV_KEYBOARD_AltSet, 1u);
                    break;
                case USB_REQ_SET_INTERFACE :
                    USB_DEV_KEYBOARD_AltSet = (uint8_t)(req->wValue);
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
 ** \brief  Send KEYBOARD HID report to the host
 ** \param  pdev: device instance
 ** \param  report: pointer to the report buffer
 ** \param  len: the length of the report buffer in bytes
 ** \retval status
 ******************************************************************************/
uint8_t usb_dev_mouse_txreport(usb_core_instance *pdev, uint8_t *report, uint16_t len)
{
    if (pdev->dev.device_cur_status == USB_DEV_CONFIGURED )
    {
        hd_usb_deveptx(pdev, HID_IN_EP, report, (uint32_t)len);
    }
    return USBD_OK;
}

/**
 *******************************************************************************
 ** \brief  get the configuration descriptor
 ** \param  length : pointer data length of the configuration descriptor in bytes
 ** \retval pointer of configuration descriptor buffer
 ******************************************************************************/
uint8_t *usb_dev_keyboard_getcfgdesc(uint16_t *length)
{
    *length = (uint16_t)sizeof (USB_DEV_KEYBOARD_CfgDesc);
    return USB_DEV_KEYBOARD_CfgDesc;
}

/**
 *******************************************************************************
  ** \brief  handle the IN data Stage
  ** \param  pdev: device instance
  ** \param  epnum: endpoint index
  ** \retval none
 ******************************************************************************/
void usb_dev_keyboard_datain(void *pdev, uint8_t epnum)
{
    hd_usb_flsdevep(pdev, HID_IN_EP);
}

/**
 *******************************************************************************
  ** \brief  handle the OUT data Stage
  ** \param  pdev: device instance
  ** \param  epnum: endpoint index
  ** \retval none
 ******************************************************************************/
void usb_dev_keyboard_dataout(void *pdev, uint8_t epnum)
{
    hd_usb_readytorx(pdev, HID_OUT_EP, (uint8_t *)u8KeyRevBuf, 64);
    hd_usb_devepstatusset(pdev, HID_OUT_EP, USB_EP_RX_VALID);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
