/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usbd_desc.c
 **
 ** A detailed description is available at
 ** @link
        Host driver interrupt subroutines.
    @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usb_core_driver.h"
#include "usb_defines.h"
#include "usb_host_int.h"


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#if defined   (__CC_ARM) /*!< ARM Compiler */
#pragma O0
#elif defined (__GNUC__) /*!< GNU Compiler */
#pragma GCC optimize ("O0")
#elif defined  (__TASKING__) /*!< TASKING Compiler */
#pragma optimize=0
#endif /* __CC_ARM */
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
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */
/**
 *******************************************************************************
 ** \brief  processes interrupt for a specific host channel which is used for OUT EP
 ** \param  pdev : device instance
 ** \param  chnum: channel index
 ** \retval none
 ******************************************************************************/
void hd_usb_host_chx_out_isr(usb_core_instance *pdev , uint8_t chnum)
{
    USB_CORE_HCCHAR_TypeDef     hcchar;  
    uint32_t hcintval, hcintmskval;
    
    hcchar.b = *(__IO stc_bUSB_CORE_HCCHAR_t*)&HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCCHAR); 
    hcintval = HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCINT);
    hcintmskval = HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCINTMSK);
    hcintval = hcintval & hcintmskval;
    if(hcintval & HCINT_AHBERR)
    {
        hd_usb_host_clrint(pdev, chnum, hc_ahberr);
        hd_usb_host_int_unmskchhltd(pdev, chnum);
    }
    else if(hcintval & HCINT_ACK)
    {
        hd_usb_host_clrint(pdev, chnum, hc_ack);
    }
    else if(hcintval & HCINT_FRMOVRUN)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_frmovrun);
    }
    else if(hcintval & HCINT_XFERCOMPL)
    {
        pdev->host.ErrCnt[chnum] = 0u;
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_xfercompl);
        pdev->host.HC_Status[chnum] = HOST_CH_XFERCOMPL;
    }
    else if(hcintval & HCINT_STALL)
    {
        hd_usb_host_clrint(pdev, chnum, hc_stall);
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        pdev->host.HC_Status[chnum] = HOST_CH_STALL;
    }
    else if(hcintval & HCINT_NAK)
    {
        pdev->host.ErrCnt[chnum] = 0u;
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_nak);
        pdev->host.HC_Status[chnum] = HOST_CH_NAK;
    }
    else if(hcintval & HCINT_XACTERR)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        pdev->host.ErrCnt[chnum] ++;
        pdev->host.HC_Status[chnum] = HOST_CH_XACTERR;
        hd_usb_host_clrint(pdev, chnum, hc_xacterr);
    }
    else if(hcintval & HCINT_NYET)
    {
        pdev->host.ErrCnt[chnum] = 0u;
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_nyet);
        pdev->host.HC_Status[chnum] = HOST_CH_NYET;
    }
    else if(hcintval & HCINT_DATATGLERR)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_nak);
        pdev->host.HC_Status[chnum] = HOST_CH_DATATGLERR;
        hd_usb_host_clrint(pdev, chnum, hc_datatglerr);
    }
    else if(hcintval & HCINT_CHHLTD)
    {
        hd_usb_host_int_mskchhltd(pdev, chnum);
        if(pdev->host.HC_Status[chnum] == HOST_CH_XFERCOMPL)
        {
            pdev->host.URB_State[chnum] = HOST_CH_XFER_DONE;

            if (hcchar.b.eptype == EP_TYPE_BULK)
            {
                pdev->host.hc[chnum].out_toggle ^= 1;
            }
        }
        else if(pdev->host.HC_Status[chnum] == HOST_CH_NAK)
        {
            pdev->host.URB_State[chnum] = HOST_CH_XFER_UNREADY;
        }
        else if(pdev->host.HC_Status[chnum] == HOST_CH_NYET)
        {
            if(pdev->host.hc[chnum].do_ping == 1)
            {
                hd_usb_pingtokenissue(pdev, chnum);
            }
            pdev->host.URB_State[chnum] = HOST_CH_XFER_UNREADY;
        }
        else if(pdev->host.HC_Status[chnum] == HOST_CH_STALL)
        {
            pdev->host.URB_State[chnum] = HOST_CH_XFER_STALL;
        }
        else if(pdev->host.HC_Status[chnum] == HOST_CH_XACTERR)
        {
            if (pdev->host.ErrCnt[chnum] == 3ul)
            {
                pdev->host.URB_State[chnum] = HOST_CH_XFER_ERROR;
                pdev->host.ErrCnt[chnum] = 0ul;
            }
        }
        else
        {
            ;
        }
        hd_usb_host_clrint(pdev, chnum, hc_chhltd);
    }
}
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */
/**
 *******************************************************************************
 ** \brief  processes interrupt for a specific host Channel which is used for IN EP
 ** \param  pdev : device instance
 ** \param  chnum: channel index
 ** \retval none
 ******************************************************************************/
void hd_usb_host_chx_in_isr(usb_core_instance *pdev, uint8_t chnum)
{
    USB_CORE_HCCHAR_TypeDef     hcchar;
    USB_CORE_HCTSIZn_TypeDef    hctsiz; 
    uint32_t                    u32eptypetmp = 0ul; 
    uint32_t                    hcintval, hcintmskval;
    
    hcchar.b = *(__IO stc_bUSB_CORE_HCCHAR_t*)&HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCCHAR); 
    hcintval = HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCINT);
    hcintmskval = HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCINTMSK);
    hcintval = hcintval & hcintmskval;

    if(hcintval & HCINT_AHBERR)
    {
        hd_usb_host_clrint(pdev, chnum, hc_ahberr);
        hd_usb_host_int_unmskchhltd(pdev, chnum);
    }
    else if(hcintval & HCINT_ACK)
    {
        hd_usb_host_clrint(pdev, chnum, hc_ack);
    }
    else if(hcintval & HCINT_STALL)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        pdev->host.HC_Status[chnum] = HOST_CH_STALL;
        hd_usb_host_clrint(pdev, chnum, hc_nak); 
        hd_usb_host_clrint(pdev, chnum, hc_stall);       
        hd_usb_hchstop(pdev, chnum);
    }
    else if(hcintval & HCINT_DATATGLERR)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_nak);
        pdev->host.HC_Status[chnum] = HOST_CH_DATATGLERR;
        hd_usb_host_clrint(pdev, chnum, hc_datatglerr);
    }
    else if(hcintval & HCINT_FRMOVRUN)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_frmovrun);
    }
    else if(hcintval & HCINT_XFERCOMPL)
    {
        if (pdev->basic_cfgs.dmaen == 1u)
        {
            hctsiz.b = *(__IO stc_bUSB_CORE_HCTSIZn_t*)&HD_USB_RDREG32(&pdev->regs.HC_REGS[chnum]->HCTSIZ);  
            pdev->host.XferCnt[chnum] =  pdev->host.hc[chnum].xfer_len - hctsiz.b.xfersize;
        }
        pdev->host.HC_Status[chnum] = HOST_CH_XFERCOMPL;
        pdev->host.ErrCnt [chnum]= 0u;
        hd_usb_host_clrint(pdev, chnum, hc_xfercompl);
        switch(hcchar.b.eptype)
        {
            case EP_TYPE_CTRL:
            case EP_TYPE_BULK:
                hd_usb_host_int_unmskchhltd(pdev, chnum);
                hd_usb_hchstop(pdev, chnum);
                hd_usb_host_clrint(pdev, chnum, hc_nak);
                pdev->host.hc[chnum].in_toggle ^= (uint8_t)1;
            break;
            case EP_TYPE_INTR:
                hcchar.b.oddfrm  = 1u;
                HD_USB_WRREG32(&pdev->regs.HC_REGS[chnum]->HCCHAR, *(uint32_t*)&hcchar.b);   
                pdev->host.URB_State[chnum] = HOST_CH_XFER_DONE;
            break;
            case EP_TYPE_ISOC:
                if(pdev->host.HC_Status[chnum] == HOST_CH_XFERCOMPL)
                {
                    pdev->host.URB_State[chnum] = HOST_CH_XFER_DONE;
                }
            break;
            default:
            break;
    }
    }
    else if(hcintval & HCINT_CHHLTD)
    {
        hd_usb_host_int_mskchhltd(pdev, chnum);
        if(pdev->host.HC_Status[chnum] == HOST_CH_XFERCOMPL)
        {
            pdev->host.URB_State[chnum] = HOST_CH_XFER_DONE;
        }
        else if (pdev->host.HC_Status[chnum] == HOST_CH_STALL)
        {
            pdev->host.URB_State[chnum] = HOST_CH_XFER_STALL;
        }
        else if(pdev->host.HC_Status[chnum] == HOST_CH_XACTERR)
        {
            pdev->host.ErrCnt[chnum] = 0u;
            pdev->host.URB_State[chnum] = HOST_CH_XFER_ERROR;
        }
        else if(pdev->host.HC_Status[chnum] == HOST_CH_DATATGLERR)
        {
            pdev->host.ErrCnt[chnum] = 0u;
            pdev->host.URB_State[chnum] = HOST_CH_XFER_ERROR;
        }
        else if(hcchar.b.eptype == EP_TYPE_INTR)
        {
            pdev->host.hc[chnum].in_toggle ^= (uint8_t)1;
        }
        else
        {
            ;
        }
        hd_usb_host_clrint(pdev, chnum, hc_chhltd);
    }
    else if(hcintval & HCINT_XACTERR)
    {
        hd_usb_host_int_unmskchhltd(pdev, chnum);
        pdev->host.ErrCnt[chnum] ++;
        pdev->host.HC_Status[chnum] = HOST_CH_XACTERR;
        hd_usb_hchstop(pdev, chnum);
        hd_usb_host_clrint(pdev, chnum, hc_xacterr);
    }
    else if(hcintval & HCINT_NAK)
    {
        u32eptypetmp = hcchar.b.eptype;
        if(u32eptypetmp == EP_TYPE_INTR)
        {
            hd_usb_host_int_unmskchhltd(pdev, chnum);
            hd_usb_hchstop(pdev, chnum);
        }
        else if ((u32eptypetmp == EP_TYPE_CTRL)||(u32eptypetmp == EP_TYPE_BULK))
        {
            hcchar.b.chen = 1u;
            hcchar.b.chdis = 0u;
            HD_USB_WRREG32(&pdev->regs.HC_REGS[chnum]->HCCHAR, *(uint32_t*)&hcchar.b);  
        }
        else
        {
           ;
        }
        pdev->host.HC_Status[chnum] = HOST_CH_NAK;
        hd_usb_host_clrint(pdev, chnum, hc_nak);
    }
}

/**
 *******************************************************************************
 ** \brief  this function processes the channel interrupt
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_hc_isr(usb_core_instance *pdev)
{
    USB_CORE_HCCHAR_TypeDef       hcchar;
    uint32_t tmp_cnt = 0ul; 
    uint32_t haintval;

    haintval = HD_USB_RDREG32(&pdev->regs.HREGS->HAINT);
    for (tmp_cnt = 0ul; tmp_cnt < pdev->basic_cfgs.host_chnum ; tmp_cnt++)
    {
        if(haintval & ((uint32_t)1 << tmp_cnt))   
        {
            hcchar.b = *(__IO stc_bUSB_CORE_HCCHAR_t*)&HD_USB_RDREG32(&pdev->regs.HC_REGS[tmp_cnt]->HCCHAR);  
            if (hcchar.b.epdir)
            {
                hd_usb_host_chx_in_isr(pdev, tmp_cnt);
            }
            else
            {
                hd_usb_host_chx_out_isr(pdev, tmp_cnt);
            }
        }
    }
}

/**
 *******************************************************************************
 ** \brief  process the start-of-frame interrupt in host mode.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_sof_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTSTS_TypeDef      tmp_gintsts;
 
    tmp_gintsts.d32 = 0ul;
    tmp_gintsts.b.sofintr = 1u;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, tmp_gintsts.d32);
}

/**
 *******************************************************************************
 ** \brief  processes disconnect interrupt
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_disconn_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTSTS_TypeDef      gintsts;
  
    hd_usb_gintdis(pdev);
    hd_usb_vbusctrl(pdev, 0u);
    gintsts.d32 = 0ul;
    gintsts.b.disconnect = 1u;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    pdev->host.is_dev_connect = 0u;
}
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */
/**
 *******************************************************************************
 ** \brief  processes non-periodic txFIFO empty interrupt.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_nptxfifoempty_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTMSK_TypeDef      intmsk;
    USB_CORE_HNPTXSTS_TypeDef     hnptxsts;
    uint16_t                     len_words , len;

    hnptxsts.b = *(__IO stc_bUSB_CORE_HNPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.GREGS->GNPTXSTS);

    len_words = (uint16_t)((pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_len + (uint32_t)3) / 4ul);
    while ((hnptxsts.b.nptxfspcavail > len_words)&& (pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_len != 0u))
    {
        len = (uint16_t)(hnptxsts.b.nptxfspcavail * 4ul);
        if (len > pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_len)
        {
            len = (uint16_t)pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_len;
            intmsk.d32 = 0ul;
            intmsk.b.nptxfempty = 1u;
            HD_USB_MDFREG32( &pdev->regs.GREGS->GINTMSK, intmsk.d32, 0ul);
        }
        len_words = (uint16_t)((pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_len + 3ul) / 4ul);
        hd_usb_wrpkt (pdev , pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_buff, (uint8_t)hnptxsts.b.nptxqtop_chnep, len);
        pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_buff  += len;
        pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_len   -= len;
        pdev->host.hc[hnptxsts.b.nptxqtop_chnep].xfer_count += len;
        hnptxsts.b = *(__IO stc_bUSB_CORE_HNPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.GREGS->GNPTXSTS);  
    }
}
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */
/**
 *******************************************************************************
 ** \brief  processes periodic txFIFO empty interrupt
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_ptxfifoempty_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTMSK_TypeDef      intmsk;
    USB_CORE_HPTXSTS_TypeDef      hptxsts;
    uint16_t                      len_words, len;

    hptxsts.b = *(__IO stc_bUSB_CORE_HPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.HREGS->HPTXSTS); 
    len_words = (uint16_t)((pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_len + 3ul) / 4ul);
    while ((hptxsts.b.ptxfspcavail > len_words)&&(pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_len != 0u))
    {
        len = (uint16_t)(hptxsts.b.ptxfspcavail * 4ul);
        if (len > pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_len)
        {
            len = (uint16_t)pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_len;         
            intmsk.d32 = 0ul;
            intmsk.b.ptxfempty = 1u;
            HD_USB_MDFREG32( &pdev->regs.GREGS->GINTMSK, intmsk.d32, 0ul);
        }
        len_words = (uint16_t)((pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_len + 3ul) / 4ul);
        hd_usb_wrpkt (pdev , pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_buff, (uint8_t)hptxsts.b.ptxqtop.chnum, len);
        pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_buff  += len;
        pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_len   -= len;
        pdev->host.hc[hptxsts.b.ptxqtop.chnum].xfer_count  += len;
        hptxsts.b = *(__IO stc_bUSB_CORE_HPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.HREGS->HPTXSTS);  
    }
}

/**
 *******************************************************************************
 ** \brief  This function determines which interrupt conditions have occurred
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */
void hd_usb_host_port_isr(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hprt;
    USB_CORE_HPRT_TypeDef  tmp_hprt_bk;
    USB_CORE_HCFG_TypeDef   hcfg;
    uint32_t do_reset = 0ul;
    tmp_hprt_bk.d32 = 0ul;

    tmp_hprt.b = *(__IO stc_bUSB_CORE_HPRT0_t*)&HD_USB_RDREG32(pdev->regs.HPRT0); 
    tmp_hprt_bk.d32 = HD_USB_RDREG32(pdev->regs.HPRT0);
    /* Clear the interrupt bits in GINTSTS */
    tmp_hprt_bk.b.prtena = 0u;
    tmp_hprt_bk.b.prtconndet = 0u;
    tmp_hprt_bk.b.prtenchng = 0u;
    tmp_hprt_bk.b.prtovrcurrchng = 0u;

    //check if a port connect have been detected
    if (tmp_hprt.b.prtconndet)
    {
        tmp_hprt_bk.b.prtconndet = 1u;
        if(host_driver_getvbusdrivestate(pdev) != 0ul)
        {
            pdev->host.is_dev_connect = 1u;
        }
    }
    //check if port enable or disable change
    if (tmp_hprt.b.prtenchng)
    {
        tmp_hprt_bk.b.prtenchng = 1u;
        
        if (tmp_hprt.b.prtena == 1u)
        {
            if ((tmp_hprt.b.prtspd == PRTSPD_LOW_SPEED) ||
            (tmp_hprt.b.prtspd == PRTSPD_FULL_SPEED))
            {
                hcfg.b = *(__IO stc_bUSB_CORE_HCFG_t*)&HD_USB_RDREG32(&pdev->regs.HREGS->HCFG); 
                if (tmp_hprt.b.prtspd == PRTSPD_LOW_SPEED)
                {
                    if (hcfg.b.fslspclksel != HCFG_6_MHZ)
                    {
                        do_reset = 1u;
                    }
                }
                else
                {
                    //1ms*(PHY clock frequency for FS/LS)-1
                    HD_USB_WRREG32(&pdev->regs.HREGS->HFIR, 48000ul );
                    if (hcfg.b.fslspclksel != HCFG_48_MHZ)
                    {
                        hd_usb_fslspclkselset(pdev, HCFG_48_MHZ);
                        do_reset = 1u;
                    }
                }
            }
            else
            {
                do_reset = 1u;
            }
        }
    }
    if (tmp_hprt.b.prtovrcurrchng)
    {
        tmp_hprt_bk.b.prtovrcurrchng = 1u;
    }
    if (do_reset)
    {
        hd_usb_hprtrst(pdev);
    }
    HD_USB_WRREG32(pdev->regs.HPRT0, tmp_hprt_bk.d32);
}

/**
 *******************************************************************************
 ** \brief  processes the rxFIFO non-empty interrupt
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */
void hd_usb_host_rxflvl_isr(usb_core_instance *pdev)
{
    USB_CORE_GRXFSTS_TypeDef       grxsts;
    USB_CORE_GINTMSK_TypeDef       tmp_gintmsk;
    USB_CORE_HCTSIZn_TypeDef       hctsiz;
    USB_CORE_HCCHAR_TypeDef        hcchar;
    __IO uint8_t                   chnum = 0u;
    uint32_t                       count;
    uint8_t                        u8Tmp = 0u;   
    uint8_t*                       pu8Tmp = (void  *)0u; 
    uint8_t                        tmp_chnum = 0u; 
    uint32_t                       u32Tmp;

    tmp_gintmsk.d32 = 0ul;
    tmp_gintmsk.b.rxstsqlvl = 1u;
    HD_USB_MDFREG32( &pdev->regs.GREGS->GINTMSK, tmp_gintmsk.d32, 0ul);

    u32Tmp = HD_USB_RDREG32(&pdev->regs.GREGS->GRXSTSP);
    grxsts.b = *(__IO stc_bUSB_CORE_GRXSTS_t*)&u32Tmp;
    chnum = (uint8_t)grxsts.b.chnum;
    u8Tmp = chnum; 
    hcchar.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[u8Tmp]->HCCHAR);  

    switch (grxsts.b.pktsts)
    {
        case 2:              //IN dat packet received
            pu8Tmp = pdev->host.hc[chnum].xfer_buff;
            if ((grxsts.b.bcnt > 0u) && (pu8Tmp != (void  *)0u))
            {
                hd_usb_rdpkt(pdev, pdev->host.hc[chnum].xfer_buff, (uint16_t)grxsts.b.bcnt);
                pdev->host.hc[grxsts.b.chnum].xfer_buff += grxsts.b.bcnt;
                pdev->host.hc[grxsts.b.chnum].xfer_count  += grxsts.b.bcnt;

                count = pdev->host.hc[chnum].xfer_count;
                pdev->host.XferCnt[chnum]  = count;

                tmp_chnum = chnum;
                hctsiz.b = *(__IO stc_bUSB_CORE_HCTSIZn_t*)&HD_USB_RDREG32(&pdev->regs.HC_REGS[tmp_chnum]->HCTSIZ);  
                if(hctsiz.b.pktcnt > 0u)
                {
                    hcchar.b.chen = 1u;
                    hcchar.b.chdis = 0u;
                    HD_USB_WRREG32(&pdev->regs.HC_REGS[chnum]->HCCHAR, hcchar.d32);
                }
            }
            break;

        case 3:              //IN transfer completed(trigger an interrupt)
            break;
        case 5:              //Daat toggle error(trigger an interrupt)
            break;
        case 7:              //Channel halted(trigger an interrupt)
            break;
            default:
        break;
    }
    tmp_gintmsk.b.rxstsqlvl = 1u;
    HD_USB_MDFREG32(&pdev->regs.GREGS->GINTMSK, 0ul, tmp_gintmsk.d32);
}

/**
 *******************************************************************************
 ** \brief  process the incomplete periodic transfer interrupt(incompIP)
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma optimize = none
#endif /* __CC_ARM */

void hd_usb_host_incomplisoout_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTSTS_TypeDef       gintsts;
    USB_CORE_HCCHAR_TypeDef        hcchar;

    hcchar.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[0]->HCCHAR);
    hcchar.b.chen = 1u;
    hcchar.b.chdis = 1u;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[0]->HCCHAR, hcchar.d32);
    gintsts.d32 = 0ul;
    gintsts.b.incomplisoout = 1u;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
}

/**
 *******************************************************************************
 ** \brief  process the resume/remote wakeup detected interrupt(WkUpInt)
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_wkupint_isr(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hprt;

    tmp_hprt.d32 = hd_usb_rdhprt(pdev);
    tmp_hprt.b.prtres = 0u;
    HD_USB_WRREG32(pdev->regs.HPRT0, tmp_hprt.d32);
}

/**
 *******************************************************************************
 ** \brief  This function process all interrupt of USB in host mode
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_host_isr(usb_core_instance *pdev)
{  
    uint32_t gintstsval; 
    if (hd_usb_getcurmod(pdev))
    {
        gintstsval = hd_usb_getcoreintr(pdev);
        if(gintstsval & (((uint32_t)1)<<3))
        {
            hd_usb_host_sof_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<4))
        {
            hd_usb_host_rxflvl_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<5))
        {
            hd_usb_host_nptxfifoempty_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<26))
        {
            hd_usb_host_ptxfifoempty_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<25))
        {
            hd_usb_host_hc_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<24))
        {
            hd_usb_host_port_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<29))
        {
            hd_usb_host_disconn_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<21))
        {
            hd_usb_host_incomplisoout_isr(pdev);
        }
        if(gintstsval & (((uint32_t)1)<<31))
        {
            hd_usb_host_wkupint_isr(pdev);
        }
    }
}
/*******************************************************************************
 * EOF 
 ******************************************************************************/

