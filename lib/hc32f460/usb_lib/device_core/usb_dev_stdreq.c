/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_stdreq.c
 **
 ** A detailed description is available at
 ** @link
        This file provides the standard USB requests following chapter 9.
    @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "usb_dev_stdreq.h"
#include "usb_dev_ctrleptrans.h"
#include "usb_dev_desc.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
__USB_ALIGN_BEGIN uint32_t dev_ep_status __USB_ALIGN_END  = 0u;
__USB_ALIGN_BEGIN uint32_t  dev_default_cfg __USB_ALIGN_END  = 0u;
__USB_ALIGN_BEGIN uint32_t  dev_cfg_status __USB_ALIGN_END  = 0u;

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void hd_usb_getdesc(usb_core_instance *pdev, const USB_SETUP_REQ *req);
void hd_usb_setaddr(usb_core_instance  *pdev, const USB_SETUP_REQ *req);
void hd_usb_setconfig(usb_core_instance *pdev, const USB_SETUP_REQ *req);
void hd_usb_getconfig(usb_core_instance *pdev, const USB_SETUP_REQ *req);
void hd_usb_getstatus(usb_core_instance  *pdev, const USB_SETUP_REQ *req);
void hd_usb_getintf(usb_core_instance *pdev);
void hd_usb_setfeature(usb_core_instance *pdev, USB_SETUP_REQ *req);
void hd_usb_clrfeature(usb_core_instance *pdev, USB_SETUP_REQ *req);
uint8_t hd_usb_getlength(uint8_t *buf);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
extern __IO USB_CORE_DCTL_TypeDef SET_TEST_MODE;

/**
 *******************************************************************************
 ** \brief  Handle standard usb device requests
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_standarddevreq(usb_core_instance *pdev, USB_SETUP_REQ *req)
{
    if(req->bRequest == USB_REQ_GET_DESCRIPTOR)
    {
        hd_usb_getdesc (pdev, req) ;
    }
    else if(req->bRequest == USB_REQ_SET_ADDRESS)
    {
        hd_usb_setaddr(pdev, req);
    }
    else if(req->bRequest == USB_REQ_SET_CONFIGURATION)
    {
        hd_usb_setconfig (pdev , req);
    }
    else if(req->bRequest == USB_REQ_GET_CONFIGURATION)
    {
        hd_usb_getconfig (pdev , req);
    }
    else if(req->bRequest == USB_REQ_GET_STATUS)
    {
        hd_usb_getstatus (pdev , req);
    }
    else if(req->bRequest == USB_REQ_SET_FEATURE)
    {
        hd_usb_setfeature (pdev , req);
    }
    else if(req->bRequest == USB_REQ_CLEAR_FEATURE)
    {
        hd_usb_clrfeature (pdev , req);
    }
    else
    {
        if (pdev->dev.class_callback->ep0_setup (pdev, req))
        {
            hd_usb_ctrlerr(pdev);
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Handle standard usb interface requests
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_standarditfreq(usb_core_instance *pdev, USB_SETUP_REQ *req)
{
    uint8_t u8RetFlag = 0u;

    if(req->bRequest == USB_REQ_GET_STATUS)
    {
        hd_usb_getstatus (pdev , req);
        u8RetFlag = 1u;
    }
    else if(req->bRequest == USB_REQ_GET_INTERFACE)
    {
        if (req->bmRequest != 0x21u)
        {
            hd_usb_getintf (pdev);
            u8RetFlag = 1u;
        }
    }
    else if(req->bRequest == 0xfe)
    {
        hd_usb_getintf (pdev);
        u8RetFlag = 1u;
    }
    else
    {

    }

    if(1u != u8RetFlag)
    {
        if(pdev->dev.device_cur_status == USB_DEV_CONFIGURED)
        {
            if (LOBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
            {
                pdev->dev.class_callback->ep0_setup(pdev, req);
                if(req->wLength == 0u)
                {
                    hd_usb_ctrlstatustx(pdev);
                }
            }
            else
            {
                hd_usb_ctrlerr(pdev);
            }
        }
        else
        {
            hd_usb_ctrlerr(pdev);
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Handle standard usb endpoint requests
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_standardepreq(usb_core_instance *pdev, USB_SETUP_REQ *req)
{
    __IO uint8_t   ep_addr;

    ep_addr  = LOBYTE(req->wIndex);

    switch (req->bRequest)
    {
        case USB_REQ_SET_FEATURE :
        switch (pdev->dev.device_cur_status)
            {
                case USB_DEV_ADDRESSED:
                    if ((ep_addr != 0x00u) && (ep_addr != 0x80u))
                    {
                        hd_usb_stalldevep(pdev , ep_addr);
                    }
                    break;
                case USB_DEV_CONFIGURED:
                    if (req->wValue == USB_FEATURE_EP_HALT)
                    {
                        if ((ep_addr != 0x00u) && (ep_addr != 0x80u))
                        {
                            hd_usb_stalldevep(pdev , ep_addr);
                        }
                    }
                    pdev->dev.class_callback->ep0_setup(pdev, req);
                    hd_usb_ctrlstatustx(pdev);
                    break;
                default:
                    hd_usb_ctrlerr(pdev);
                    break;
            }
            break;
        case USB_REQ_CLEAR_FEATURE :
            switch (pdev->dev.device_cur_status)
            {
                case USB_DEV_ADDRESSED:
                    if ((ep_addr != 0x00u) && (ep_addr != 0x80u))
                    {
                        hd_usb_stalldevep(pdev , ep_addr);
                    }
                    break;
                case USB_DEV_CONFIGURED:
                    if (req->wValue == USB_FEATURE_EP_HALT)
                    {
                        if ((ep_addr != 0x00u) && (ep_addr != 0x80u))
                        {
                            hd_usb_clrstall(pdev , ep_addr);
                            pdev->dev.class_callback->ep0_setup(pdev, req);
                        }
                        hd_usb_ctrlstatustx(pdev);
                    }
                    break;
                default:
                    hd_usb_ctrlerr(pdev);
                    break;
            }
            break;
        case USB_REQ_GET_STATUS:
            switch (pdev->dev.device_cur_status)
            {
                case USB_DEV_ADDRESSED:
                    if ((ep_addr != 0x00u) && (ep_addr != 0x80u))
                    {
                    hd_usb_stalldevep(pdev , ep_addr);
                    }
                    break;
                case USB_DEV_CONFIGURED:
                    if ((ep_addr & 0x80u)== 0x80u)
                    {
                        if(pdev->dev.in_ep[(ep_addr & 0x7F) % USB_MAX_TX_FIFOS].ep_stall)
                        {
                            dev_ep_status = 0x0001u;
                        }
                        else
                        {
                            dev_ep_status = 0x0000u;
                        }
                    }
                    else if ((ep_addr & 0x80u)== 0x00u)
                    {
                        if(pdev->dev.out_ep[(ep_addr & 0x7F) % USB_MAX_TX_FIFOS].ep_stall)
                        {
                            dev_ep_status = 0x0001u;
                        }
                        else
                        {
                            dev_ep_status = 0x0000u;
                        }
                    }
                    else
                    {
                        //
                    }
                    hd_usb_ctrldatatx (pdev,(uint8_t *)&dev_ep_status, 2u);
                    break;

                default:
                    hd_usb_ctrlerr(pdev);
                    break;
            }
            break;
        default:
        break;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle Get Descriptor requests
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval status
 ******************************************************************************/
void hd_usb_getdesc(usb_core_instance *pdev, const USB_SETUP_REQ *req)
{
    uint16_t len = 0u;
    uint8_t *pbuf = NULL;
    uint8_t u8ErrFlag = 0u;

    switch (req->wValue >> 8u)
    {
        case USB_DESC_TYPE_DEVICE:
            pbuf = pdev->dev.desc_callback->get_dev_desc(&len);
            break;
        case USB_DESC_TYPE_CONFIGURATION:
            pbuf   = (uint8_t *)pdev->dev.class_callback->class_getconfigdesc(&len);
            pbuf[1] = USB_DESC_TYPE_CONFIGURATION;
            break;
        case USB_DESC_TYPE_STRING:
            switch ((uint8_t)(req->wValue))
            {
                case LANGID_STR_IDX:
                    pbuf = pdev->dev.desc_callback->get_dev_langiddesc(&len);
                    break;
                case MFC_STR_IDX:
                    pbuf = pdev->dev.desc_callback->get_dev_manufacturerstr(&len);
                    break;
                case PRODUCT_STR_IDX:
                    pbuf = pdev->dev.desc_callback->get_dev_productstr(&len);
                    break;
                case SERIAL_STR_IDX:
                    pbuf = pdev->dev.desc_callback->get_dev_serialstr(&len);
                    break;
                case CONFIG_STR_IDX:
                    pbuf = pdev->dev.desc_callback->get_dev_configstr(&len);
                    break;
                case INTERFACE_STR_IDX:
                    pbuf = pdev->dev.desc_callback->get_dev_interfacestr(&len);
                    break;
                default:
                    hd_usb_ctrlerr(pdev);
                    u8ErrFlag = 1u;
                    break;
                }
                break;
        case USB_DESC_TYPE_DEVICE_QUALIFIER:
            hd_usb_ctrlerr(pdev);
            u8ErrFlag = 1u;
            break;
        case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
            hd_usb_ctrlerr(pdev);
            u8ErrFlag = 1u;
            break;
        default:
            hd_usb_ctrlerr(pdev);
            u8ErrFlag = 1u;
            break;
    }

    if(0u == u8ErrFlag)
    {
        if((len != 0u)&& (req->wLength != 0u))
        {
            len = __MIN(len , req->wLength);
            hd_usb_ctrldatatx (pdev, pbuf, len);
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Set device address
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_setaddr(usb_core_instance  *pdev, const USB_SETUP_REQ *req)
{
    uint8_t  dev_addr;

    if ((req->wIndex == 0u) && (req->wLength == 0u))
    {
        dev_addr = (uint8_t)(req->wValue) & 0x7Fu;

        if (pdev->dev.device_cur_status == USB_DEV_CONFIGURED)
        {
            hd_usb_ctrlerr(pdev);
        }
        else
        {
            pdev->dev.device_address = dev_addr;
            hd_usb_addrset(pdev, dev_addr);
            hd_usb_ctrlstatustx(pdev);

            if (dev_addr != 0u)
            {
                pdev->dev.device_cur_status  = USB_DEV_ADDRESSED;
            }
            else
            {
                pdev->dev.device_cur_status  = USB_DEV_DEFAULT;
            }
        }
    }
    else
    {
        hd_usb_ctrlerr(pdev);
    }
}

/**
 *******************************************************************************
 ** \brief  Handle Set device configuration request
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_setconfig(usb_core_instance *pdev, const USB_SETUP_REQ *req)
{
    static uint8_t  tmp_cfgidx;

    tmp_cfgidx = (uint8_t)(req->wValue);

    if (tmp_cfgidx > DEV_MAX_CFG_NUM )
    {
        hd_usb_ctrlerr(pdev);
    }
    else
    {
        switch (pdev->dev.device_cur_status)
        {
            case USB_DEV_ADDRESSED:
                if (tmp_cfgidx)
                {
                    pdev->dev.device_config     = tmp_cfgidx;
                    pdev->dev.device_cur_status = USB_DEV_CONFIGURED;
                    hd_usb_dev_ctrlconfig(pdev , tmp_cfgidx, USBD_SET);;
                    hd_usb_ctrlstatustx(pdev);
                }
                else
                {
                    hd_usb_ctrlstatustx(pdev);
                }
                break;
            case USB_DEV_CONFIGURED:
                if (tmp_cfgidx == 0u)
                {
                    pdev->dev.device_cur_status = USB_DEV_ADDRESSED;
                    pdev->dev.device_config     = tmp_cfgidx;
                    hd_usb_dev_ctrlconfig(pdev , tmp_cfgidx, USBD_CLEAR);
                    hd_usb_ctrlstatustx(pdev);
                }
                else if(tmp_cfgidx != pdev->dev.device_config)
                {
                    /* Clear old configuration */
                    hd_usb_dev_ctrlconfig(pdev , pdev->dev.device_config, USBD_CLEAR);
                    /* set new configuration */
                    pdev->dev.device_config = tmp_cfgidx;
                    hd_usb_dev_ctrlconfig(pdev , tmp_cfgidx, USBD_SET);
                    hd_usb_ctrlstatustx(pdev);
                }
                else
                {
                    hd_usb_ctrlstatustx(pdev);
                }
                break;
            case USB_DEV_SUSPENDED:

                break;
            default:
                hd_usb_ctrlerr(pdev);
                break;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Handle Get device configuration request
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_getconfig(usb_core_instance *pdev, const USB_SETUP_REQ *req)
{
    if (req->wLength != 1u)
    {
        hd_usb_ctrlerr(pdev);
    }
    else
    {
        switch (pdev->dev.device_cur_status)
        {
            case USB_DEV_ADDRESSED:
                hd_usb_ctrldatatx (pdev, (uint8_t *)&dev_default_cfg, 1u);
                break;
            case USB_DEV_CONFIGURED:
                hd_usb_ctrldatatx (pdev, (uint8_t *)&pdev->dev.device_config, 1u);
                break;
            default:
                hd_usb_ctrlerr(pdev);
                break;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Handle Get Status request
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_getstatus(usb_core_instance  *pdev, const USB_SETUP_REQ *req)
{
    (void)(req);
    switch (pdev->dev.device_cur_status)
    {
        case USB_DEV_ADDRESSED:
        case USB_DEV_CONFIGURED:
#if (1u == SELF_POWER)
            dev_cfg_status = USB_CONFIG_SELF_POWERED;
#else
            dev_cfg_status = 0x00u;
#endif
            if (pdev->dev.device_remote_wakeup)
            {
            dev_cfg_status |= USB_CONFIG_REMOTE_WAKEUP;
            }
            hd_usb_ctrldatatx (pdev, (uint8_t *)&dev_cfg_status, 2u);
            break;
        default :
            hd_usb_ctrlerr(pdev);
            break;
    }
}

/**
 *******************************************************************************
 ** \brief  hd_usb_getintf
 ** \param  pdev : device instance
 ** \retval reval: none
 ******************************************************************************/
void hd_usb_getintf(usb_core_instance  *pdev)
{
    dev_cfg_status = 0u;
    hd_usb_ctrldatatx (pdev, (uint8_t *)&dev_cfg_status, 1u);
}

/**
 *******************************************************************************
 ** \brief  Handle Set device feature request
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_setfeature(usb_core_instance *pdev, USB_SETUP_REQ *req)
{
    USB_CORE_DCTL_TypeDef     dctl;
    uint8_t test_mode = 0u;

    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
    {
        pdev->dev.device_remote_wakeup = 1u;
        pdev->dev.class_callback->ep0_setup(pdev, req);
        hd_usb_ctrlstatustx(pdev);
    }
    else if ((req->wValue == USB_FEATURE_TEST_MODE) && ((req->wIndex & 0xFFu) == 0u))
    {
        dctl.d32            = HD_USB_RDREG32(&pdev->regs.DREGS->DCTL);
        test_mode           = (uint8_t)req->wIndex >> 8u;
        test_mode           = test_mode&(~0xf8);
        dctl.b.tstctl       = test_mode;
        SET_TEST_MODE       = dctl;
        pdev->dev.test_mode = 1u;
        hd_usb_ctrlstatustx(pdev);
    }
    else
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle clear device feature request
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval none
 ******************************************************************************/
void hd_usb_clrfeature(usb_core_instance *pdev, USB_SETUP_REQ *req)
{
    switch (pdev->dev.device_cur_status)
    {
        case USB_DEV_ADDRESSED:
        case USB_DEV_CONFIGURED:
            if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
            {
            pdev->dev.device_remote_wakeup = 0u;
            pdev->dev.class_callback->ep0_setup(pdev, req);
            hd_usb_ctrlstatustx(pdev);
            }
            break;
        default :
            hd_usb_ctrlerr(pdev);
            break;
    }
}

/**
 *******************************************************************************
 ** \brief  Copy buffer into setup structure
 ** \param  pdev: device instance
 ** \param  req: usb request
 ** \retval None
 ******************************************************************************/
void hd_usb_parsesetupreq(usb_core_instance *pdev, USB_SETUP_REQ *req)
{
    req->bmRequest     = *(uint8_t *)  (pdev->dev.setup_pkt_buf);
    req->bRequest      = *(uint8_t *)  (pdev->dev.setup_pkt_buf +  1u);
    req->wValue        = SWAPBYTE      (pdev->dev.setup_pkt_buf +  2u);
    req->wIndex        = SWAPBYTE      (pdev->dev.setup_pkt_buf +  4u);
    req->wLength       = SWAPBYTE      (pdev->dev.setup_pkt_buf +  6u);

    pdev->dev.in_ep[0].ctl_data_len = req->wLength;
    pdev->dev.device_state = USB_EP0_SETUP;
}

/**
 *******************************************************************************
 ** \brief  Handle USB low level Error
 ** \param  pdev: device instance
 ** \retval None
 ******************************************************************************/
void hd_usb_ctrlerr(usb_core_instance *pdev)
{
    hd_usb_stalldevep(pdev , 0x80u);
    hd_usb_stalldevep(pdev , 0u);
    hd_usb_ep0outstart(pdev);
}

/**
 *******************************************************************************
 ** \brief  Convert Ascii string into unicode one
 ** \param  desc : descriptor buffer
 ** \param  unicode : Formatted string buffer (unicode)
 ** \param  len : descriptor length
 ** \retval None
 ******************************************************************************/
void hd_usb_getstring(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t tmp_idx = 0u;

    if (desc != NULL)
    {
        *len =  (uint16_t)hd_usb_getlength(desc) * 2u + 2u;
        unicode[tmp_idx++] = (uint8_t)*len;
        unicode[tmp_idx++] =  USB_DESC_TYPE_STRING;

        while (*desc != (uint8_t)0u)
        {
            unicode[tmp_idx++] = *desc++;
            unicode[tmp_idx++] =  0x00u;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  get the string length
 ** \param  buf : pointer to the ascii string buffer
 ** \retval string length
 ******************************************************************************/
uint8_t hd_usb_getlength(uint8_t *buf)
{
    uint8_t  tmp_len = 0u;

    while (*buf != (uint8_t)0u)
    {
        tmp_len++;
        buf++;
    }

    return tmp_len;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
