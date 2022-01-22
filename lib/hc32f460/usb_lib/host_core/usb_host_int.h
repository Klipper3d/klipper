/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_host_int.h
 **
 ** A detailed description is available at
 ** @link Peripheral Device Interface Layer @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_HOST_INT_H__
#define __USB_HOST_INT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_host_driver.h"

#define    HCINT_XFERCOMPL  ((uint32_t)1)<<0
#define    HCINT_CHHLTD     ((uint32_t)1)<<1
#define    HCINT_AHBERR     ((uint32_t)1)<<2
#define    HCINT_STALL      ((uint32_t)1)<<3
#define    HCINT_NAK        ((uint32_t)1)<<4
#define    HCINT_ACK        ((uint32_t)1)<<5
#define    HCINT_NYET       ((uint32_t)1)<<6
#define    HCINT_XACTERR    ((uint32_t)1)<<7
#define    HCINT_FRMOVRUN   ((uint32_t)1)<<9
#define    HCINT_DATATGLERR ((uint32_t)1)<<10



/**
 *******************************************************************************
 ** \brief  clear the interrupt flag bit
 ** \param  pdev : device instance
 ** \param  ch_num: the channel index
 ** \param  intbit: the interrupt bit of the register HCINTn
 ** \retval none
 ******************************************************************************/    
static __INLINE void hd_usb_host_clrint(usb_core_instance *pdev, uint32_t ch_num, uint32_t intbit)
{
    USB_CORE_HCINTn_TypeDef  tmp_hcintx;
    *(uint32_t*)&tmp_hcintx.b = intbit;
    HD_USB_WRREG32(&(pdev->regs.HC_REGS[ch_num]->HCINT), tmp_hcintx.d32);
}
    
/**
 *******************************************************************************
 ** \brief  mask the interrupt of ChHltd
 ** \param  pdev : device instance
 ** \param  ch_num: channel index of the host application
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_host_int_mskchhltd(usb_core_instance *pdev, uint32_t ch_num)
{
    USB_CORE_HCINTMSK_TypeDef  tmp_hcintmsk; 
    tmp_hcintmsk.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK); 
    tmp_hcintmsk.b.chhltd = 0u; 
    HD_USB_WRREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK, tmp_hcintmsk.d32);
}

/**
 *******************************************************************************
 ** \brief  unmask the interrupt of ChHltd
 ** \param  pdev : device instance
 ** \param  ch_num: channel index of the host application
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_host_int_unmskchhltd(usb_core_instance *pdev, uint32_t ch_num) 
{
    USB_CORE_HCINTMSK_TypeDef  tmp_hcintmsk; 
    tmp_hcintmsk.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK); 
    tmp_hcintmsk.b.chhltd = 1u; 
    HD_USB_WRREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK, tmp_hcintmsk.d32);    
}    

/**
 *******************************************************************************
 ** \brief  mask the interrupt of ACK
 ** \param  pdev : device instance
 ** \param  ch_num: channel index of the host application
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_host_int_mskack(usb_core_instance *pdev, uint32_t ch_num)  
{
    USB_CORE_HCINTMSK_TypeDef  tmp_hcintmsk; 
    tmp_hcintmsk.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK); 
    tmp_hcintmsk.b.ack = 0u; 
    HD_USB_WRREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK, tmp_hcintmsk.d32);
}    

/**
 *******************************************************************************
 ** \brief  unmask the interrupt of ACK
 ** \param  pdev : device instance
 ** \param  ch_num: channel index of the host application
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_host_int_unmskack(usb_core_instance *pdev, uint32_t ch_num)  
{
    USB_CORE_HCINTMSK_TypeDef  tmp_hcintmsk; 
    tmp_hcintmsk.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK); 
    tmp_hcintmsk.b.ack = 1u; 
    HD_USB_WRREG32(&pdev->regs.HC_REGS[ch_num]->HCINTMSK, tmp_hcintmsk.d32);    
}
    
/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
extern void hd_usb_host_isr(usb_core_instance *pdev);

#endif //__USB_HOST_INT_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


