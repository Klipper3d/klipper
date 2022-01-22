/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_driver.c
 **
 ** A detailed description is available at
 ** @link
        Peripheral Device Interface Layer
    @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_driver.h"
#include "usb_bsp.h"

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

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Device intialize
 ** \param  pdev : Device instance
 ** \retval : None
 ******************************************************************************/
void hd_usb_initdev(usb_core_instance *pdev)
{
    uint32_t tmp_1;
    USB_DEV_EP *iep, *oep;

    hd_usb_setregaddr (pdev);
    pdev->dev.device_cur_status = (uint8_t)USB_DEV_DEFAULT;
    pdev->dev.device_address = 0u;
    tmp_1 = 0;
    do{
        iep = &pdev->dev.in_ep[tmp_1];
        oep = &pdev->dev.out_ep[tmp_1];
        iep->ep_dir = 1u;
        oep->ep_dir = 0u;
        iep->epidx = (uint8_t)tmp_1;
        oep->epidx = iep->epidx;
        iep->tx_fifo_num = (uint16_t)tmp_1;
        oep->tx_fifo_num = iep->tx_fifo_num;
        iep->trans_type = EP_TYPE_CTRL;
        oep->trans_type = iep->trans_type;
        iep->maxpacket = USB_MAX_EP0_SIZE;
        oep->maxpacket = iep->maxpacket;
        iep->xfer_buff = 0u;
        oep->xfer_buff = iep->xfer_buff;
        iep->xfer_len = 0u;
        oep->xfer_len = iep->xfer_len;
        tmp_1++;
    }while(tmp_1 < pdev->basic_cfgs.dev_epnum);

    hd_usb_gintdis(pdev);
    /*Init the Core (common init.) */
    hd_usb_initusbcore(pdev);
    /* Force Device Mode*/
    hd_usb_modeset(pdev, DEVICE_MODE);
    /* Init Device */
    hd_usb_devmodeinit(pdev);
    /* Enable USB Global interrupt */
    hd_usb_ginten(pdev);
}

/**
 *******************************************************************************
 ** \brief  Configure and open an endpoint
 ** \param  pdev : Device instance
 ** \param  epdesc : Endpoint Descriptor
 ** \retval reval: none
 ******************************************************************************/
void hd_usb_opendevep(usb_core_instance *pdev, uint8_t ep_addr, uint16_t ep_mps, uint8_t ep_type)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1, tmp_2;
    
    tmp_1 = ep_addr>>7;       //EP type, it is IN(=1) or OUT(=0)
    tmp_2 = ep_addr & 0x7f;   //EP number
    if(tmp_1 == 1)
    {
        ep = &pdev->dev.in_ep[tmp_2];
    }
    else
    {
        ep = &pdev->dev.out_ep[tmp_2];
    }
    
    ep->epidx   = tmp_2;

    ep->ep_dir = tmp_1;
    ep->maxpacket = ep_mps;
    ep->trans_type = ep_type;
    if (tmp_1 == 1)
    {
        /* Assign a Tx FIFO */
        ep->tx_fifo_num = tmp_2;
    }
    /* Set initial data PID. */
    if (ep_type == USB_EP_BULK )
    {
        ep->data_pid_start = 0u;
    }
    hd_usb_epactive(pdev , ep );
}

/**
 *******************************************************************************
 ** \brief  called when an EP is disabled
 ** \param pdev: device instance
 ** \param ep_addr: endpoint address
 ** \retval reval: none
 ******************************************************************************/
void hd_usb_shutdevep(usb_core_instance *pdev , uint8_t  ep_addr)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1, tmp_2;
    
    tmp_1 = ep_addr>>7;       //EP type, it is IN(=1) or OUT(=0)
    tmp_2 = ep_addr & 0x7f;   //EP number
    if (tmp_1 == 1)
    {
        ep = &pdev->dev.in_ep[tmp_2];
    }
    else
    {
        ep = &pdev->dev.out_ep[tmp_2];
    }
    ep->epidx   = tmp_2;
    ep->ep_dir = tmp_1;
    hd_usb_epdeactive(pdev , ep );
}

/**
 *******************************************************************************
 ** \brief  hd_usb_readytorx
 ** \param pdev: device instance
 ** \param ep_addr: endpoint address
 ** \param pbuf: pointer to Rx buffer
 ** \param buf_len: data length
 ** \retval : status
 ******************************************************************************/
void hd_usb_readytorx( usb_core_instance *pdev, uint8_t ep_addr, uint8_t *pbuf, uint16_t buf_len)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1;
    
    tmp_1 = ep_addr & 0x7f;   //EP number   
    ep = &pdev->dev.out_ep[tmp_1];
    /*setup and start the Xfer */
    ep->xfer_buff = pbuf;
    ep->xfer_len = buf_len;
    ep->xfer_count = 0u;
    ep->ep_dir = 0u;
    ep->epidx = tmp_1;

    if (pdev->basic_cfgs.dmaen == 1u)
    {
        ep->dma_addr = (uint32_t)pbuf;
    }

    if ( tmp_1 == 0u )
    {
        hd_usb_ep0transbegin(pdev , ep);
    }
    else
    {
        hd_usb_epntransbegin(pdev, ep );
    }
}

/**
 *******************************************************************************
 ** \brief  configures EPO to receive SETUP packets from host
 ** \param pdev: device instance
 ** \retval  none
 ******************************************************************************/
void hd_usb_ep0outstart(usb_core_instance *pdev)
{
    hd_usb_ep0revcfg(pdev);
}

/**
 *******************************************************************************
 ** \brief  Transmit data over USB
 ** \param pdev: device instance
 ** \param ep_addr: endpoint address
 ** \param pbuf: pointer to Tx buffer
 ** \param buf_len: data length
 ** \retval  none
 ******************************************************************************/
void hd_usb_deveptx( usb_core_instance *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t buf_len)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1;
    
    tmp_1 = ep_addr & 0x7f;   //EP number
    ep = &pdev->dev.in_ep[tmp_1];

    /* Setup and start the Transfer */
    ep->ep_dir = 1u;
    ep->epidx = tmp_1;
    ep->xfer_buff = pbuf;
    ep->dma_addr = (uint32_t)pbuf;
    ep->xfer_count = 0u;
    ep->xfer_len  = buf_len;

    if ( tmp_1 == 0 )
    {
        hd_usb_ep0transbegin(pdev , ep);
    }
    else
    {
        hd_usb_epntransbegin(pdev, ep );
    }
}

/**
 *******************************************************************************
 ** \brief  Stall an endpoint.
 ** \param pdev: device instance
 ** \param epnum: endpoint address
 ** \retval : none
 ******************************************************************************/
void hd_usb_stalldevep(usb_core_instance *pdev, uint8_t epnum)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1, tmp_2;
    
    tmp_1 = epnum>>7;       //EP type, it is IN(=1) or OUT(=0)
    tmp_2 = epnum & 0x7f;   //EP number    
    
    if(tmp_1 != 0)
    {
        ep = &pdev->dev.in_ep[tmp_2];
    }
    else
    {
        ep = &pdev->dev.out_ep[tmp_2];
    }

    ep->ep_stall = 1u;
    ep->epidx   = tmp_2;
    if(tmp_1 != 0)
    {
        ep->ep_dir = 1;
    }
    else
    {
        ep->ep_dir = 0;
    }

    hd_usb_setepstall(pdev , ep);
}

/**
 *******************************************************************************
 ** \brief  Clear stall condition on endpoints.
 ** \param pdev: device instance
 ** \param epnum: endpoint address
 ** \retval : status
 ******************************************************************************/
void hd_usb_clrstall(usb_core_instance *pdev, uint8_t epnum)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1, tmp_2;
    
    tmp_1 = epnum>>7;       //EP type, it is IN(=1) or OUT(=0)
    tmp_2 = epnum & 0x7f;   //EP number     
    if(tmp_1 != 0)
    {
        ep = &pdev->dev.in_ep[tmp_2];
    }
    else
    {
        ep = &pdev->dev.out_ep[tmp_2];
    }

    ep->ep_stall = 0u;
    ep->epidx   = tmp_2;
    if(tmp_1 != 0)
    {
        ep->ep_dir = 1;
    }
    else
    {
        ep->ep_dir = 0;
    }    
    
    
    hd_usb_clearepstall(pdev , ep);
}

/**
 *******************************************************************************
 ** \brief  This Function flushes the FIFOs.
 ** \param pdev: device instance
 ** \param epnum: endpoint address
 ** \retval : status
 ******************************************************************************/
void hd_usb_flsdevep(usb_core_instance *pdev, uint8_t epnum)
{
    __IO uint8_t tmp_1;
    
    tmp_1 = epnum>>7;       //EP type, it is IN(=1) or OUT(=0)
    if(tmp_1 != 0)
    {
        hd_usb_txfifoflush(pdev, (uint32_t)epnum & (uint32_t)0x7F);
    }
    else
    {
        hd_usb_rxfifoflush(pdev);
    }
}

/**
 *******************************************************************************
 ** \brief  This Function set USB device address
 ** \param pdev: device instance
 ** \param address: new device address
 ** \retval : none
 ******************************************************************************/
void hd_usb_addrset(usb_core_instance *pdev, uint8_t devaddr)
{
    hd_usb_devaddrset(pdev, devaddr);
}
/**
 *******************************************************************************
 ** \brief  control device connect or disconnect
 ** \param  pdev: device instance
 ** \param  link: 1(conn) or 0(disconn)
 ** \retval None
 ******************************************************************************/
void hd_usb_conndevctrl(usb_core_instance *pdev, uint8_t link)
{
    hd_usb_ctrldevconnect(pdev, link);
}

/**
 *******************************************************************************
 ** \brief  returns the EP Status
 ** \param  pdev : Selected device
 **         epnum : endpoint address
 ** \retval : EP status
 ******************************************************************************/
uint32_t hd_usb_devepstatusget(usb_core_instance *pdev ,uint8_t epnum)
{
    USB_DEV_EP *ep;
    uint32_t Status = 0ul;
    __IO uint8_t tmp_1, tmp_2;
    
    tmp_1 = epnum>>7;       //EP type, it is IN(=1) or OUT(=0)
    tmp_2 = epnum & 0x7f;   //EP number     
    if(tmp_1 != 0)
    {
        ep = &pdev->dev.in_ep[tmp_2];
    }
    else
    {
        ep = &pdev->dev.out_ep[tmp_2];
    }

    Status = hd_usb_epstatusget(pdev ,ep);
    /* Return the current status */
    return Status;
}

/**
 *******************************************************************************
 ** \brief  Set the EP Status
 ** \param  pdev : Selected device
 **         Status : new Status
 **         epnum : EP address
 ** \retval : None
 ******************************************************************************/
void hd_usb_devepstatusset(usb_core_instance *pdev, uint8_t epnum, uint32_t Status)
{
    USB_DEV_EP *ep;
    __IO uint8_t tmp_1, tmp_2;
    
    tmp_1 = epnum>>7;       //EP type, it is IN(=1) or OUT(=0)
    tmp_2 = epnum & 0x7f;   //EP number     
    if(tmp_1 != 0)
    {
        ep = &pdev->dev.in_ep[tmp_2];
    }
    else
    {
        ep = &pdev->dev.out_ep[tmp_2];
    }

    hd_usb_epstatusset(pdev ,ep , Status);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
