/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_ctrleptrans.c
 **
 ** A detailed description is available at
 ** @link
    This file provides the IO requests APIs for control endpoints.
    @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_ctrleptrans.h"


/**
 *******************************************************************************
 ** \brief  send data on the ctl pipe
 ** \param  pdev: device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be sent
 ** \retval none
 ******************************************************************************/
void hd_usb_ctrldatatx(usb_core_instance *pdev, uint8_t *pbuf, uint16_t len)
{
    pdev->dev.in_ep[0].total_data_len = len;
    pdev->dev.in_ep[0].rem_data_len   = len;
    pdev->dev.device_state            = USB_EP0_DATA_IN;

    hd_usb_deveptx(pdev, 0u, pbuf, (uint32_t)len);
}

/**
 *******************************************************************************
 ** \brief  continue sending data on the ctl pipe
 ** \param  pdev: device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be sent
 ** \retval none
 ******************************************************************************/
void hd_usb_ctrldatacontinuetx(usb_core_instance *pdev, uint8_t *pbuf, uint16_t len)
{
    hd_usb_deveptx(pdev, 0u, pbuf, (uint32_t)len);
}

/**
 *******************************************************************************
 ** \brief  receive data on the ctl pipe
 ** \param  pdev: device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be received
 ** \retval status
 ******************************************************************************/
void hd_usb_ctrldatarx(usb_core_instance *pdev, uint8_t *pbuf, uint16_t len)
{
    pdev->dev.out_ep[0].total_data_len = len;
    pdev->dev.out_ep[0].rem_data_len   = len;
    pdev->dev.device_state             = USB_EP0_DATA_OUT;

    hd_usb_readytorx(pdev, 0u, pbuf, len);
}

/**
 *******************************************************************************
 ** \brief  continue receive data on the ctl pipe
 ** \param  pdev: device instance
 ** \param  buff: pointer to data buffer
 ** \param  len: length of data to be received
 ** \retval status
 ******************************************************************************/
void hd_usb_ctrldatacontinuerx(usb_core_instance *pdev, uint8_t *pbuf, uint16_t len)
{
    hd_usb_readytorx(pdev, 0u, pbuf, len);
}
/**
 *******************************************************************************
 ** \brief  send zero lzngth packet on the ctl pipe
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_ctrlstatustx(usb_core_instance *pdev)
{
    pdev->dev.device_state = USB_EP0_STATUS_IN;
    hd_usb_deveptx(pdev, 0u, pdev->dev.setup_pkt_buf, 0u);
}

/**
 *******************************************************************************
 ** \brief  receive zero lzngth packet on the ctl pipe
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_ctrlstatusrx(usb_core_instance *pdev)
{
    pdev->dev.device_state = USB_EP0_STATUS_OUT;
    hd_usb_readytorx(pdev, 0u, pdev->dev.setup_pkt_buf, 0u);
}

/**
 *******************************************************************************
 ** \brief  hd_usb_getrxcnt
 **         returns the received data length
 ** \param  pdev: USB OTG device instance
 **         epnum: endpoint index
 ** \retval Rx Data blength
 ******************************************************************************/
uint16_t hd_usb_getrxcnt(usb_core_instance *pdev, uint8_t epnum)
{
    return (uint16_t)pdev->dev.out_ep[epnum].xfer_count;
}
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
