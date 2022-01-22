/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_driver.c
 **
 ** A detailed description is available at
 ** @link
        Host Interface Layer.
    @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_core_driver.h"
#include "usb_host_driver.h"
#include "usb_app_conf.h"
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
 ** \brief  Initialize the driver for the host mode
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void host_driver_init(usb_core_instance *pdev)
{
    uint8_t i = 0u;
    
    pdev->host.is_dev_connect = 0u;
    for (i= 0u; i< USB_MAX_TX_FIFOS; i++)
    {
        pdev->host.ErrCnt[i]      = 0u;
        pdev->host.XferCnt[i]     = 0u;
        pdev->host.HC_Status[i]   = HOST_CH_IDLE;
    }
    pdev->host.hc[0].max_packet   = 8u;

    hd_usb_setregaddr(pdev);

    hd_usb_gintdis(pdev);
    hd_usb_initusbcore(pdev);
    /* force to work in host mode*/
    hd_usb_modeset(pdev, HOST_MODE);
    hd_usb_hostmodeinit(pdev);
    hd_usb_ginten(pdev);
}

/**
 *******************************************************************************
 ** \brief  get current speed when in host mode
 ** \param  pdev : device instance
 ** \retval current speed
 ******************************************************************************/
uint32_t host_driver_getcurrentspd (usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hppt;
    tmp_hppt.b = *(__IO stc_bUSB_CORE_HPRT0_t*)&HD_USB_RDREG32(pdev->regs.HPRT0);
    return tmp_hppt.b.prtspd;
}

/**
 *******************************************************************************
 ** \brief  get current DM DP state
 ** \param  pdev : device instance
 ** \retval DM DP state
 **         0x00 DM L, DP L
 **         0x01 DM L, DP H
 **         0x02 DM H, DP L
 **         0x03 DM H, DP H
 ******************************************************************************/
uint32_t host_driver_getdmdpstate(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hppt;
    tmp_hppt.b = *(__IO stc_bUSB_CORE_HPRT0_t*)&HD_USB_RDREG32(pdev->regs.HPRT0);
    return tmp_hppt.b.prtlnsts;
}

/**
 *******************************************************************************
 ** \brief  get vbus drive state
 ** \param  pdev : device instance
 ** \retval vbus driver state
 **         0x00 vbus driver disable
 **         0x01 vbus driver enable
 ******************************************************************************/
uint32_t host_driver_getvbusdrivestate(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hppt;
    tmp_hppt.b = *(__IO stc_bUSB_CORE_HPRT0_t*)&HD_USB_RDREG32(pdev->regs.HPRT0);
    return tmp_hppt.b.prtpwr;
}

/**
 ** \brief  issue a command to reset the port
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void host_driver_portrst(usb_core_instance *pdev)
{
    hd_usb_hprtrst(pdev);
}

/**
 *******************************************************************************
 ** \brief  get the connected status of the device
 ** \param  pdev : device instance
 ** \retval 1 @connected or 0 @disconnected
 **
 ******************************************************************************/
uint32_t host_driver_ifdevconnected(usb_core_instance *pdev)
{
    return (pdev->host.is_dev_connect);
}

/**
 *******************************************************************************
 ** \brief  gets the frame number for of sof packet
 ** \param  pdev : device instance
 ** \retval number of frame 
 ******************************************************************************/
uint32_t host_driver_getcurrentfrm(usb_core_instance *pdev)
{
    return (HD_USB_RDREG32(&pdev->regs.HREGS->HFNUM) & 0xFFFFul) ;
}

/**
 *******************************************************************************
 ** \brief  gets the last xfer state
 ** \param  pdev : device instance
 ** \retval HOST_CH_XFER_STATE
 ******************************************************************************/
HOST_CH_XFER_STATE host_driver_getxferstate(usb_core_instance *pdev, uint8_t ch_num)
{
    return pdev->host.URB_State[ch_num] ;
}

/**
 *******************************************************************************
 ** \brief  gets the xfer count
 ** \param  pdev : device instance
 ** \param  hc_num: channel number
 ** \retval number of data trandmitted in bytes
 ******************************************************************************/
uint32_t host_driver_getxfercnt(usb_core_instance *pdev, uint8_t ch_num)
{
    return pdev->host.XferCnt[ch_num] ;
}

/**
 *******************************************************************************
 ** \brief  gets the host channel status
 ** \param  pdev : device instance
 ** \param  hc_num: channel number
 ** \retval HOST_CH_STATUS
 ******************************************************************************/
HOST_CH_STATUS host_driver_gethostchstate(usb_core_instance *pdev,  uint8_t ch_num)
{
    return pdev->host.HC_Status[ch_num] ;
}

/**
 *******************************************************************************
 ** \brief  prepare a host channel and start a transfer
 ** \param  pdev : device instance
 ** \param  hc_num: channel number
 ** \retval status
 ******************************************************************************/
uint32_t host_driver_hostch_init(usb_core_instance *pdev, uint8_t hc_num)
{
    return hd_usb_inithch(pdev, hc_num);
}

/**
 *******************************************************************************
 ** \brief  prepare a host channel and start a transfer
 ** \param  pdev : device instance
 ** \param  hc_num: channel number
 ** \retval status
 ******************************************************************************/
uint32_t host_driver_submitrequest(usb_core_instance *pdev, uint8_t hc_num)
{
    pdev->host.URB_State[hc_num] = HOST_CH_XFER_IDLE;
    pdev->host.hc[hc_num].xfer_count = 0u ;
    return hd_usb_hchtransbegin(pdev, hc_num);
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
