/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_core.c
 **
 ** A detailed description is available at
 ** @link
        This file provides the USBD core functions.
    @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "usb_dev_core.h"
#include "usb_dev_stdreq.h"
#include "usb_dev_ctrleptrans.h"
#include "usb_dev_int.h"
#include "usb_bsp.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void hd_usb_setup_process(usb_core_instance *pdev);
void hd_usb_dataout_process(usb_core_instance *pdev, uint8_t epnum);
void hd_usb_datain_process(usb_core_instance *pdev , uint8_t epnum);
void hd_usb_sof_process(usb_core_instance *pdev);
void hd_usb_dev_rst(usb_core_instance *pdev);
void hd_usb_dev_susp(usb_core_instance *pdev);
void hd_usb_dev_resume(usb_core_instance *pdev);
void hd_usb_ctrlconn(usb_core_instance *pdev, uint8_t conn);
void hd_usb_isoinincomplt_process(usb_core_instance *pdev);
void hd_usb_isooutincomplt_process(usb_core_instance *pdev);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
__IO USB_CORE_DCTL_TypeDef SET_TEST_MODE;
usb_dev_int_cbk_typedef dev_int_cbk =
{
    &hd_usb_dev_rst,
    &hd_usb_ctrlconn,
    &hd_usb_dev_susp,
    &hd_usb_dev_resume,    
    &hd_usb_sof_process,
    &hd_usb_setup_process,
    &hd_usb_dataout_process,
    &hd_usb_datain_process,   
    &hd_usb_isoinincomplt_process,
    &hd_usb_isooutincomplt_process  
};

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
usb_dev_int_cbk_typedef  *dev_int_cbkpr = &dev_int_cbk;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  Initailizes the device stack and load the class driver
 ** \param  pdev: device instance
 ** \param  core_address: core ID
 ** \param  class_cb: Class callback structure address
 ** \param  usr_cb: User callback structure address
 ** \retval None
 ******************************************************************************/
void hd_usb_dev_init(usb_core_instance *pdev,
               usb_dev_desc_func *pDevice,
               usb_dev_class_func *class_cb,
               usb_dev_user_func *usr_cb)
{
    hd_usb_bsp_init(pdev);
    pdev->dev.class_callback = class_cb;
    pdev->dev.user_callback  = usr_cb;
    pdev->dev.desc_callback  = pDevice;
    hd_usb_initdev(pdev);
    pdev->dev.user_callback->user_init();
    pdev->dev.device_state   = USB_EP0_IDLE;
    hd_usb_bsp_nvicconfig();
}

void hd_usb_dev_deinit(usb_core_instance *pdev)
{
    (void)(pdev);
}
/**
 *******************************************************************************
 ** \brief  set current configuration or clear current configuration
 ** \param  pdev: device instance
 ** \param  cfgidx: configuration index
 ** \param  action: USBD_SET or USBD_CLEAR
 ** \retval none
 ******************************************************************************/
void hd_usb_dev_ctrlconfig(usb_core_instance  *pdev, uint8_t cfgidx, uint8_t action)
{
    __IO uint8_t tmp_1;

    (void)(cfgidx);
    tmp_1 = action;
    if(tmp_1 == USBD_SET)         //set configuration
    {
        pdev->dev.class_callback->class_init(pdev);
        pdev->dev.user_callback->user_devconfig();        
    }
    else if(tmp_1 == USBD_CLEAR)  //clear configuration
    {
        pdev->dev.class_callback->class_deinit(pdev);
    }
    else
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle Reset event
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_dev_rst(usb_core_instance  *pdev)
{
    hd_usb_opendevep(pdev, 0x00u, USB_MAX_EP0_SIZE, EP_TYPE_CTRL);
    hd_usb_opendevep(pdev, 0x80u, USB_MAX_EP0_SIZE, EP_TYPE_CTRL);
    pdev->dev.device_cur_status = USB_DEV_DEFAULT;
    pdev->dev.user_callback->user_devrst( );
}


/**
 *******************************************************************************
 ** \brief  Handle device connection and disconnection event
 ** \param  pdev: device instance
 ** \param  conn: 1 or 0
 ** \retval none
 ******************************************************************************/
void hd_usb_ctrlconn(usb_core_instance *pdev, uint8_t conn)
{
    __IO uint8_t tmp_1;
    tmp_1 = conn;
    if(tmp_1)
    {
        pdev->dev.user_callback->user_devconn();
        pdev->dev.connection_status = 1u;
    }
    else
    {
        pdev->dev.user_callback->user_devdisconn();
        pdev->dev.class_callback->class_deinit(pdev);
        pdev->dev.connection_status = 0u;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle Suspend event
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_dev_susp(usb_core_instance *pdev)
{
    pdev->dev.device_old_status  = pdev->dev.device_cur_status;
    pdev->dev.device_cur_status  = USB_DEV_SUSPENDED;
    pdev->dev.user_callback->user_devsusp();
}

/**
 *******************************************************************************
 ** \brief  Handle Resume event
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_dev_resume(usb_core_instance *pdev)
{
    pdev->dev.user_callback->user_devresume();
    pdev->dev.device_cur_status = pdev->dev.device_old_status;
    pdev->dev.device_cur_status = USB_DEV_CONFIGURED;
}

/**
 *******************************************************************************
 ** \brief  Handle SOF event
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_sof_process(usb_core_instance *pdev)
{
    if(pdev->dev.class_callback->class_sof)
    {
        pdev->dev.class_callback->class_sof(pdev);
    }
}

/**
 *******************************************************************************
 ** \brief  Handle the setup stage
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_setup_process(usb_core_instance *pdev)
{
    USB_SETUP_REQ req;

    hd_usb_parsesetupreq(pdev , &req);

    switch (req.bmRequest & 0x1Fu)
    {
        case USB_REQ_RECIPIENT_DEVICE:
            hd_usb_standarddevreq (pdev, &req);
            break;

        case USB_REQ_RECIPIENT_INTERFACE:
            hd_usb_standarditfreq(pdev, &req);
            break;

        case USB_REQ_RECIPIENT_ENDPOINT:
            hd_usb_standardepreq(pdev, &req);
            break;

        default:
            hd_usb_stalldevep(pdev, req.bmRequest & 0x80u);
            break;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle data out stage
 ** \param  pdev: device instance
 ** \param  epnum: endpoint index
 ** \retval none
 ******************************************************************************/
void hd_usb_dataout_process(usb_core_instance *pdev, uint8_t epnum)
{
    USB_DEV_EP *ep;

    if(epnum == 0u)
    {
        ep = &pdev->dev.out_ep[0];
        if ( pdev->dev.device_state == USB_EP0_DATA_OUT)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len  -=  ep->maxpacket;

                if(pdev->basic_cfgs.dmaen == 1u)
                {
                    /* in slave mode this, is handled by the RxSTSQLvl ISR */
                    ep->xfer_buff += ep->maxpacket;
                }              
                hd_usb_readytorx (pdev, 0u, ep->xfer_buff, (uint16_t)__MIN(ep->rem_data_len ,ep->maxpacket));
            }
            else
            {
                if (ep->xfer_count > ep->rem_data_len)
                {

                }
                ep->rem_data_len = 0u;
                if((pdev->dev.class_callback->ep0_dataout != NULL)&&
                    (pdev->dev.device_cur_status == USB_DEV_CONFIGURED))
                {
                    pdev->dev.class_callback->ep0_dataout(pdev);
                }
                hd_usb_ctrlstatustx(pdev);
            }
        }
    }
    else if((pdev->dev.class_callback->class_dataout != NULL)&&(pdev->dev.device_cur_status == USB_DEV_CONFIGURED))
    {
        pdev->dev.class_callback->class_dataout(pdev, epnum);
    }
    else
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle data in stage
 ** \param  pdev: device instance
 ** \param  epnum: endpoint index
 ** \retval none
 ******************************************************************************/
void hd_usb_datain_process(usb_core_instance *pdev , uint8_t epnum)
{
    USB_DEV_EP *ep;

    if(epnum == 0u)
    {
        ep = &pdev->dev.in_ep[0];
        if ( pdev->dev.device_state == USB_EP0_DATA_IN)
        {
            if(ep->rem_data_len > ep->maxpacket)
            {
                ep->rem_data_len  -=  ep->maxpacket;
                if(pdev->basic_cfgs.dmaen == 1u)
                {
                    /* in slave mode this, is handled by the TxFifoEmpty ISR */
                    ep->xfer_buff += ep->maxpacket;
                }
                  hd_usb_deveptx (pdev, 0u, ep->xfer_buff, ep->rem_data_len);  
            }
            else
            {
                /* last packet is MPS multiple, so send ZLP packet */
                if((ep->total_data_len % ep->maxpacket == 0u) &&
                    (ep->total_data_len >= ep->maxpacket) &&
                    (ep->total_data_len < ep->ctl_data_len ))
                {
                    hd_usb_deveptx(pdev, 0u, NULL, 0);
                    ep->ctl_data_len = 0u;
                }
                else
                {
                    if((pdev->dev.class_callback->ep0_datain != NULL)&&
                        (pdev->dev.device_cur_status == USB_DEV_CONFIGURED))
                    {
                        pdev->dev.class_callback->ep0_datain(pdev);
                    }
                    hd_usb_ctrlstatusrx(pdev);
                }
            }
        }
        if (pdev->dev.test_mode == 1u)
        {
            hd_usb_runtestmode(pdev, SET_TEST_MODE.d32);
            pdev->dev.test_mode = 0u;
        }
        else
        {
        }
    }
    else if((pdev->dev.class_callback->class_datain != NULL)&&(pdev->dev.device_cur_status == USB_DEV_CONFIGURED))
    {
        pdev->dev.class_callback->class_datain(pdev, epnum);
    }
    else
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief  Handle iso in incomplete event
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_isoinincomplt_process(usb_core_instance *pdev)
{
    pdev->dev.class_callback->class_syn_in_incomplt(pdev);
}

/**
 *******************************************************************************
 ** \brief  Handle iso out incomplete event
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_isooutincomplt_process(usb_core_instance *pdev)
{
    pdev->dev.class_callback->class_syn_out_incomplt(pdev);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
