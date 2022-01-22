/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_cfgch.c
 **
 ** A detailed description is available at
 ** @link
        This file implements functions for opening and closing host channels
    @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_cfgch.h"

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
uint16_t usb_host_getfreech(usb_core_instance *pdev);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  configure and open a pipe
 ** \param  pdev : device instance
 ** \param  hc_num: host channel index
 ** \param  dev_address: USB Device address allocated to attached device
 ** \param  speed : core speed
 ** \param  ep_type: communication type of the EP
 ** \param  mps: max size of the packet
 ** \retval none
 ******************************************************************************/
void usb_host_chopen(usb_core_instance *pdev,
                               uint8_t  hc_num,
                               uint8_t  dev_address,
                               uint8_t  speed,
                               uint8_t  ep_type,
                               uint16_t mps)
{
    pdev->host.hc[hc_num].ep_idx     =(uint8_t) pdev->host.channel[hc_num]& 0x7Fu;
    pdev->host.hc[hc_num].is_epin    = (pdev->host.channel[hc_num] & 0x80u ) == 0x80u;
    pdev->host.hc[hc_num].dev_addr   = dev_address;
    pdev->host.hc[hc_num].ep_type    = ep_type;
    pdev->host.hc[hc_num].max_packet = mps;
    pdev->host.hc[hc_num].ch_speed   = speed;
    pdev->host.hc[hc_num].in_toggle  = 0u;
    pdev->host.hc[hc_num].out_toggle = 0u;
    
    hd_usb_inithch(pdev, hc_num) ;
}

/**
 *******************************************************************************
 ** \brief  change a pipe on host
 ** \param  pdev : device instance
 ** \param  hc_num: host channel index
 ** \param  dev_address: USB Device address allocated to attached device
 ** \param  speed : core speed
 ** \param  ep_type: communication type of EP
 ** \param  mps: max size of packet
 ** \retval none
 ******************************************************************************/
void usb_host_mdfch(usb_core_instance *pdev,
                              uint8_t  hc_num,
                              uint8_t  dev_address,
                              uint8_t  speed,
                              uint8_t  ep_type,
                              uint16_t mps)
{
    (void)(ep_type);
    if(dev_address != 0u)
    {
        pdev->host.hc[hc_num].dev_addr   = dev_address;
    }
    if((pdev->host.hc[hc_num].max_packet != mps) && (mps != 0u))
    {
        pdev->host.hc[hc_num].max_packet = mps;
    }
    if((pdev->host.hc[hc_num].ch_speed != speed ) && (speed != 0u ))
    {
        pdev->host.hc[hc_num].ch_speed   = speed;
    }

    hd_usb_inithch(pdev, hc_num);
}

/**
 *******************************************************************************
 ** \brief  distribute a new channel for the pipe
 ** \param  pdev : device instance
 ** \param  ep_addr: EP index the channel distributed for
 ** \retval hc_num: host channel index
 ******************************************************************************/
uint8_t usb_host_distrch(usb_core_instance *pdev, uint8_t ep_addr)
{
    __IO uint16_t hc_num;

    hc_num =  usb_host_getfreech(pdev);
    if (hc_num != HC_ERROR)
    {
        pdev->host.channel[hc_num & (USB_MAX_TX_FIFOS-1u)] = HC_USED | ep_addr;
    }
    return (uint8_t)hc_num;
}

/**
 *******************************************************************************
 ** \brief  free the USB host channel assigned by idx
 ** \param  pdev : device instance
 ** \param  idx: Channel number to be freed
 ** \retval Status
 ******************************************************************************/
uint8_t usb_host_freech(usb_core_instance *pdev, uint8_t idx)
{
    if(idx < MAX_CHNUM)
    {
        pdev->host.channel[idx & (USB_MAX_TX_FIFOS-1u)] &= HC_USED_MASK;
    }
    return HSTATUS_OK;
}

/**
 *******************************************************************************
 ** \brief  free all the USB host channels
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void usb_host_dedistrallch(usb_core_instance *pdev)
{
    uint8_t idx;

    for (idx = 2u; idx < MAX_CHNUM ; idx ++)
    {
        pdev->host.channel[idx & (USB_MAX_TX_FIFOS-1u)] = 0u;
    }
}

/**
 *******************************************************************************
 ** \brief  Get a free channel number so that can be distributed to a device endpoint
 ** \param  pdev : device instance
 ** \retval idx: the free channel index 
 ******************************************************************************/
uint16_t usb_host_getfreech(usb_core_instance *pdev)
{
    __IO uint8_t tmp_idx = 0u;
    uint16_t u16Ret = HC_ERROR;

    for (tmp_idx = 0u ; tmp_idx < MAX_CHNUM ; tmp_idx++)
    {
        if ((pdev->host.channel[tmp_idx & (USB_MAX_TX_FIFOS-1u)] & HC_USED) == 0u)
        {
            u16Ret = HC_OK;
            break;
        }
    }

    if(u16Ret == HC_OK)
    {
        u16Ret = tmp_idx;
    }
    else
    {
        u16Ret = HC_ERROR;
    }

    return u16Ret;
}

/*******************************************************************************
 * EOF 
 ******************************************************************************/
