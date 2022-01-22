/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_defines.h
 **
 ** A detailed description is available at
 ** @link Header of the Core Layer @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEF_H__
#define __USB_DEF_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief ENUMERATION TYPE
 **
 ******************************************************************************/
enum HD_USB_SPEED {
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH
};

/**
 *******************************************************************************
 ** 
 ******************************************************************************/
#define SELF_POWERED (1u << 6u)
#define REMOTE_WAKEUP (1u << 5u)

/**
 *******************************************************************************
 ** \brief working mode of the USB core
 ******************************************************************************/
#define DEVICE_MODE                            (0u)
#define HOST_MODE                              (1u)

/**
 *******************************************************************************
 ** \brief  Macro definations for device mode
 **
 ******************************************************************************/
#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     (0u)
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     (1u)
#define DSTS_ENUMSPD_LS_PHY_6MHZ               (2u)
#define DSTS_ENUMSPD_FS_PHY_48MHZ              (3u)

#define EP_TYPE_CTRL                           (0u)
#define EP_TYPE_ISOC                           (1u)
#define EP_TYPE_BULK                           (2u)
#define EP_TYPE_INTR                           (3u)
#define EP_TYPE_MSK                            (3u)

#define STS_GOUT_NAK                           (1u)
#define STS_DATA_UPDT                          (2u)
#define STS_XFER_COMP                          (3u)
#define STS_SETUP_COMP                         (4u)
#define STS_SETUP_UPDT                         (6u)

/**
 *******************************************************************************
 ** \brief Macro definations for host mode
 ******************************************************************************/
#define PID_DATA0                              (0u)
#define PID_DATA2                              (1u)
#define PID_DATA1                              (2u)
#define PID_SETUP                              (3u)

#define PRTSPD_FULL_SPEED                      (1u)
#define PRTSPD_LOW_SPEED                       (2u)

#define HCFG_30_60_MHZ                         (0u)
#define HCFG_48_MHZ                            (1u)
#define HCFG_6_MHZ                             (2u)

#define CTRL_TRANSFER                          (0u)
#define ISOC_TRANSFER                          (1u)
#define BULK_TRANSFER                          (2u)
#define INT_TRANSFER                           (3u)

#ifndef __MIN
#define  __MIN(a, b)      (((a) < (b)) ? (a) : (b))
#endif

/**
 *******************************************************************************
 ** \brief Following macro definations are for reading, writing or modifying the
 **        regsters.
 ******************************************************************************/
#define HD_USB_RDREG32(reg)  (*(__IO uint32_t *)(reg))
#define HD_USB_WRREG32(reg,value) (*(__IO uint32_t *)(reg) = (value))
#define HD_USB_MDFREG32(reg,clear_mask,set_mask) (HD_USB_WRREG32((reg), (((HD_USB_RDREG32((reg))) & ~(clear_mask)) | (set_mask))))

#endif //__USB_DEFINES__H__



