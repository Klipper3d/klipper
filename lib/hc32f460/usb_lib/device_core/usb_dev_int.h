/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_int.h
 **
 ** A detailed description is available at
 ** @link Peripheral Device Interface Layer @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_DEV_INT_H__
#define __USB_DEV_INT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_dev_driver.h"

//The bit of the diepint/doepint
#define XFER_COMPL        1ul
#define EPDISABLED        1ul<<1
#define TIME_OUT          1ul<<3
#define SETUP_BIT         1ul<<3
#define INTKNTXFEMP       1ul<<4
#define INEPNAKEFF        1ul<<6
#define TXFEMP            1ul<<7

//The bit of the GINTSTS
#define MODEMIS_INT       1ul<<1
#define SOF_INT           1ul<<3
#define RXFLVL_INT        1ul<<4
#define USBSUSP_INT       1ul<<11
#define USBRST_INT        1ul<<12
#define ENUMDONE_INT      1ul<<13
#define INEP_INT          1ul<<18
#define OUTEP_INT         1ul<<19
#define INCOMPLSOIN       1ul<<20
#define INCOMPLSOOUT      1ul<<21
#define VBUSV_INT         1ul<<30
#define WAKEUP_INT        1ul<<31

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct 
{
    void (* Reset)            (usb_core_instance *pdev);
    void (* devctrlconnect)   (usb_core_instance *pdev, uint8_t conn);
    void (* Suspend)          (usb_core_instance *pdev);
    void (* Resume)           (usb_core_instance *pdev);    
    void (* SOF)              (usb_core_instance *pdev);
    void (* SetupStage)       (usb_core_instance *pdev);
    void (* DataOutStage)     (usb_core_instance *pdev, uint8_t epnum);
    void (* DataInStage)      (usb_core_instance *pdev, uint8_t epnum);    
    void (* IsoINIncomplete)  (usb_core_instance *pdev);
    void (* IsoOUTIncomplete) (usb_core_instance *pdev);
}usb_dev_int_cbk_typedef;

/**
 *******************************************************************************
 **\brief  get the IN Endpoint TxFIFO avail space
 **\param  pdev: device instance
 **\param  epnum: endpoint number
 **\retval the avail space in 32-bit words
 ******************************************************************************/
static __INLINE uint16_t usb_rdineptxfspcavail(usb_core_instance *pdev, uint32_t epnum)
{
    uint32_t tmp_dtxfsts;
    uint16_t tmp_spclen;
    tmp_dtxfsts = HD_USB_RDREG32( &pdev->regs.INEP_REGS[epnum]->DTXFSTS);
    tmp_spclen = tmp_dtxfsts & 0xffff;
    return tmp_spclen;
}

/**
 *******************************************************************************
 **\brief  set the USB turn around time
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
static __INLINE void usb_setaroundtim(usb_core_instance *pdev)
{
    USB_CORE_GUSBCFG_TypeDef  gusbcfg;

    gusbcfg.d32 = HD_USB_RDREG32(&pdev->regs.GREGS->GUSBCFG);
    gusbcfg.b.usbtrdtim = 9u;
    HD_USB_WRREG32(&pdev->regs.GREGS->GUSBCFG, gusbcfg.d32);
}


extern usb_dev_int_cbk_typedef *dev_int_cbkpr;

extern void hd_usb_isr_handler(usb_core_instance *pdev);
#endif // __USB_DEV_INT_H__

/*******************************************************************************
 * EOF 
 ******************************************************************************/
