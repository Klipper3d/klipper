/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_dev_int.c
 **
 ** A detailed description is available at
 ** @link
        Peripheral Device interrupt subroutines.
    @endlink
 **
 **   - 2021-04-16  CDT   First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "usb_dev_int.h"
#include "usb_bsp.h"

/**
 *******************************************************************************
 **\brief  read the endpoint flags
 **\param  pdev: device instance
 **\param  epnum: endpoint number
 **\retval status
 ******************************************************************************/
uint32_t hd_usb_rddevinep(usb_core_instance *pdev, uint8_t epnum)
{
    uint32_t bit_and, tmp_diepmsk, emp;
    tmp_diepmsk  = HD_USB_RDREG32(&pdev->regs.DREGS->DIEPMSK);
    emp          = HD_USB_RDREG32(&pdev->regs.DREGS->DIEPEMPMSK);
    tmp_diepmsk |= ((emp >> epnum) & 0x1ul) << 7u;
    bit_and      = HD_USB_RDREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT) & tmp_diepmsk;
    return bit_and;
}

/**
 *******************************************************************************
 **\brief  check FIFO for the next packet to be loaded
 **\param  pdev: device instance
 **\param  epnum: endpoint number
 **\retval none
 ******************************************************************************/
void hd_usb_wrblanktxfifo(usb_core_instance *pdev, uint32_t epnum)
{
    USB_DEV_EP *ep;
    uint32_t len;
    uint32_t len32b;
    uint16_t tmp_spclen;
    uint32_t fifo_empty_mask;
 
    ep  = &pdev->dev.in_ep[epnum];
    len = ep->xfer_len - ep->xfer_count;
    if (len > ep->maxpacket)
    {
        len = ep->maxpacket;
    }
    len32b     = (len + 3u)>>2;
    tmp_spclen = usb_rdineptxfspcavail(pdev, epnum);
    while ((tmp_spclen >= len32b) && (ep->xfer_count < ep->xfer_len))
    {
        len = ep->xfer_len - ep->xfer_count;

        if (len > ep->maxpacket)
        {
            len = ep->maxpacket;
        }
        len32b          = (len + 3u)>>2;
        hd_usb_wrpkt (pdev , ep->xfer_buff, (uint8_t)epnum, (uint16_t)len);
        ep->xfer_buff  += len;
        ep->xfer_count += len;
        tmp_spclen      = usb_rdineptxfspcavail(pdev, epnum);
    }

    if(len == 0ul)
    {
        fifo_empty_mask = 0x01ul << epnum;
        HD_USB_MDFREG32(&pdev->regs.DREGS->DIEPEMPMSK, fifo_empty_mask, 0ul);
    }
}

#ifdef VBUS_SENSING_ENABLED
/**
 *******************************************************************************
 **\brief  Indicates that the USB_OTG controller has detected a connection
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_sessionrequest_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTSTS_TypeDef  gintsts;
    USB_CORE_PCGCCTL_TypeDef  power;

    dev_int_cbkpr->devctrlconnect (pdev, 1);

    gintsts.d32          = 0ul;
    gintsts.b.vbusvint   = 1u;
    HD_USB_WRREG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);

    if(pdev->basic_cfgs.low_power)
    {
        power.d32        = HD_USB_RDREG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0u;
        power.b.stoppclk = 0u;
        HD_USB_WRREG32(pdev->regs.PCGCCTL, power.d32);
    }
}
#endif

/**
 *******************************************************************************
 **\brief  Indicates that the controller has detected a resume or
 **        remote Wake-up sequence
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_resume_isr(usb_core_instance *pdev)
{
    USB_CORE_GINTSTS_TypeDef  gintsts;
    USB_CORE_DCTL_TypeDef     devctl;
    USB_CORE_PCGCCTL_TypeDef  power;

    if(pdev->basic_cfgs.low_power)
    {
        power.d32        = HD_USB_RDREG32(&pdev->regs.PCGCCTL);
        power.b.gatehclk = 0u;
        power.b.stoppclk = 0u;
        HD_USB_WRREG32(pdev->regs.PCGCCTL, power.d32);
    }

    devctl.d32          = 0ul;
    devctl.b.rmtwkupsig = 1u;
    HD_USB_MDFREG32(&pdev->regs.DREGS->DCTL, devctl.d32, 0ul);

    dev_int_cbkpr->Resume (pdev);

    gintsts.d32        = 0ul;
    gintsts.b.wkupintr = 1u;
    HD_USB_WRREG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
}

/**
 *******************************************************************************
 **\brief  Indicates that SUSPEND state has been detected on the USB
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_susp_isr(usb_core_instance *pdev)
{
    uint32_t gintsts;
    uint32_t power;
    uint32_t dsts;
    uint8_t  prev_status = 0u;

    dev_int_cbkpr->Suspend (pdev);
    dsts = HD_USB_RDREG32(&pdev->regs.DREGS->DSTS); 
    gintsts = 1<<11;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, gintsts);
    
    prev_status = pdev->dev.device_cur_status;
    if((prev_status  == USB_DEV_CONFIGURED) &&
       (pdev->basic_cfgs.low_power) && ((dsts&1) != 0)  &&
       (pdev->dev.connection_status == 1u))
    {
        power = 1;
        HD_USB_MDFREG32(pdev->regs.PCGCCTL, 0ul, power);
        power = 2;
        HD_USB_MDFREG32(pdev->regs.PCGCCTL, 0ul, power);

    }
}

/**
 *******************************************************************************
 **\brief  Indicates that at leadt one IN EP has a pending interrupt
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_inep_isr(usb_core_instance *pdev)
{
    uint32_t tmp_diepint;
    uint32_t ep_intr;
    uint8_t  epnum;
    uint32_t fifoemptymsk;
    
    ep_intr = hd_usb_getalliepintr(pdev);
    epnum = 0;
    while(ep_intr && (epnum<=5))
    {
        if(ep_intr&0x1u) 
        {
            tmp_diepint = hd_usb_rddevinep(pdev, epnum);
            if(tmp_diepint & XFER_COMPL)
            {
                fifoemptymsk = ((uint32_t)1) << epnum;
                HD_USB_MDFREG32(&pdev->regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0ul);
                HD_USB_WRREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT, XFER_COMPL);                
                dev_int_cbkpr->DataInStage(pdev , epnum);
                if (pdev->basic_cfgs.dmaen == 1u)
                {
                    if((epnum == 0u) && (pdev->dev.device_state == USB_EP0_STATUS_IN))
                    {
                        hd_usb_ep0revcfg(pdev);
                        pdev->dev.device_state = USB_EP0_IDLE;
                    }
                }
            }
            if(tmp_diepint & EPDISABLED)
            {
                HD_USB_WRREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT, EPDISABLED);
            }            
            if(tmp_diepint & TIME_OUT)
            {
                HD_USB_WRREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT, TIME_OUT);
            }
            if(tmp_diepint & INTKNTXFEMP)
            {
                HD_USB_WRREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT, INTKNTXFEMP);
            }
            if(tmp_diepint & INEPNAKEFF)
            {
                HD_USB_WRREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT, INEPNAKEFF);
            }
            if(tmp_diepint & TXFEMP)
            {
                hd_usb_wrblanktxfifo(pdev, epnum);
                HD_USB_WRREG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT, TXFEMP);
            }
        }
        epnum++;
        ep_intr >>= 1u;
    }
}

/**
 *******************************************************************************
 **\brief  Indicates that an OUT EP has a pending Interrupt
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_outep_isr(usb_core_instance *pdev)
{
    uint32_t ep_intr;
    USB_CORE_DEPXFRSIZ_TypeDef  deptsiz;
    uint32_t tmp_doepint;
    uint8_t epnum = 0ul;
    uint32_t xfer;    
    uint32_t u32ReadEpSizeTmp = 0ul; 
 
    ep_intr = hd_usb_getalloepintr(pdev);
    while (ep_intr && (epnum<=5))
    {
        if(ep_intr&0x1ul)
        {            
            tmp_doepint = hd_usb_getoepintbit(pdev, epnum);
            if(tmp_doepint & XFER_COMPL)
            {
                HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT, XFER_COMPL);
                if (pdev->basic_cfgs.dmaen == 1u)
                {
                    u32ReadEpSizeTmp = HD_USB_RDREG32(&(pdev->regs.OUTEP_REGS[epnum]->DOEPTSIZ));
                    deptsiz.b        = *(stc_bUSB_CORE_DEPXFRSIZ_t*)&u32ReadEpSizeTmp;
                    xfer             = __MIN(pdev->dev.out_ep[epnum].maxpacket,pdev->dev.out_ep[epnum].xfer_len);
                    pdev->dev.out_ep[epnum].xfer_count = xfer - deptsiz.b.xfersize;
                    if (epnum != 0u)
                    {
                        pdev->dev.out_ep[epnum].xfer_count = pdev->dev.out_ep[epnum].xfer_len - deptsiz.b.xfersize;
                    }
                }
                dev_int_cbkpr->DataOutStage(pdev, epnum);
                if (pdev->basic_cfgs.dmaen == 1u)
                {
                    if((epnum == 0u) && (pdev->dev.device_state == USB_EP0_STATUS_OUT))
                    {
                        hd_usb_ep0revcfg(pdev);
                        pdev->dev.device_state = USB_EP0_IDLE;
                    }
                }
            }
            if(tmp_doepint & EPDISABLED)
            {
                HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT, EPDISABLED);
            }
            if(epnum == 0)
            {
                tmp_doepint = hd_usb_getoepintbit(pdev, epnum);
                if(tmp_doepint & SETUP_BIT)
                {
                    dev_int_cbkpr->SetupStage(pdev);
                    HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT, SETUP_BIT);
                }
            }
        }
        epnum++;
        ep_intr >>= 1u;
    }
}

/**
 *******************************************************************************
 **\brief  Handles the SOF Interrupts
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_sof_isr(usb_core_instance *pdev)
{
    uint32_t  tmp_gintsts;
    dev_int_cbkpr->SOF(pdev);
    tmp_gintsts = ((uint32_t)1)<<3;
    HD_USB_WRREG32 (&pdev->regs.GREGS->GINTSTS, tmp_gintsts);
}

/**
 *******************************************************************************
 **\brief  Handles the Rx Status Queue Level Interrupt
 **\param  pdev: device instance
 **\retval status
 ******************************************************************************/
void hd_usb_rxstsqlvl_isr(usb_core_instance *pdev)
{
    uint32_t   tmp_gintmsk;
    uint32_t   tmp_grxsts;
    USB_DEV_EP    *ep;
    uint8_t    epnum;        
    uint8_t    pkt_status;   
    uint16_t   byte_cnt;    
    
    tmp_gintmsk = ((uint32_t)1)<<4;
    HD_USB_MDFREG32(&pdev->regs.GREGS->GINTMSK, tmp_gintmsk, 0);   
    
    tmp_grxsts  = HD_USB_RDREG32(&pdev->regs.GREGS->GRXSTSP);
    epnum       = (uint8_t)(tmp_grxsts & 0x0f);                
    pkt_status  = (uint8_t)((tmp_grxsts>>17) & 0x0f);     
    byte_cnt    = (uint16_t)((tmp_grxsts>>4)&0x07ff);       
    ep          = &pdev->dev.out_ep[epnum];    
    switch (pkt_status)
    {

        case STS_DATA_UPDT:
            if (byte_cnt)
            {
                hd_usb_rdpkt(pdev, ep->xfer_buff, byte_cnt);
                ep->xfer_buff  += byte_cnt;
                ep->xfer_count += byte_cnt;
            } else
            {
                ;
            }
            break;
        case STS_SETUP_UPDT:
            /* Copy the setup packet received in FIFO into the setup buffer in RAM */
            hd_usb_rdpkt(pdev, pdev->dev.setup_pkt_buf, 8);
            ep->xfer_count += byte_cnt;
            break;            
        case STS_GOUT_NAK:            
        case STS_XFER_COMP:
        case STS_SETUP_COMP:
            break;
        default:
            break;
    }
    HD_USB_MDFREG32(&pdev->regs.GREGS->GINTMSK, 0, tmp_gintmsk);
}

/**
 *******************************************************************************
 **\brief  This interrupt occurs when a USB Reset is detected
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_reset_isr(usb_core_instance *pdev)
{
    USB_CORE_DAINT_TypeDef    daintmsk;
    USB_CORE_DOEPMSK_TypeDef  doepmsk;
    USB_CORE_DIEPMSK_TypeDef  diepmsk;
    USB_CORE_DCFG_TypeDef     dcfg;
    USB_CORE_DCTL_TypeDef     dctl;
    USB_CORE_GINTSTS_TypeDef  gintsts;
    uint32_t i;

    dctl.d32          = 0ul;
    daintmsk.d32      = 0ul;
    doepmsk.d32       = 0ul;
    diepmsk.d32       = 0ul;
    dcfg.d32          = 0ul;
    gintsts.d32       = 0ul;
    dctl.b.rmtwkupsig = 1u;
    HD_USB_MDFREG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0ul );
    hd_usb_txfifoflush(pdev, (uint32_t)0);
    for (i = 0ul; i < pdev->basic_cfgs.dev_epnum ; i++)
    {
        HD_USB_WRREG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFFul);
        HD_USB_WRREG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFFul);
    }
    HD_USB_WRREG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFFul );

    daintmsk.ep.in        = 1u;
    daintmsk.ep.out       = 1u;
    HD_USB_WRREG32( &pdev->regs.DREGS->DAINTMSK, daintmsk.d32 );
    doepmsk.b.setup       = 1u;
    doepmsk.b.xfercompl   = 1u;
    doepmsk.b.epdisabled  = 1u;
    doepmsk.b.stsphsercvd = 1u;
    HD_USB_WRREG32( &pdev->regs.DREGS->DOEPMSK, doepmsk.d32 );
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    HD_USB_WRREG32( &pdev->regs.DREGS->DOUTEP1MSK, doepmsk.d32 );
#endif
    diepmsk.b.xfercompl   = 1u;
    diepmsk.b.timeout     = 1u;
    diepmsk.b.epdisabled  = 1u;

    HD_USB_WRREG32( &pdev->regs.DREGS->DIEPMSK, diepmsk.d32 );
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
    HD_USB_WRREG32( &pdev->regs.DREGS->DINEP1MSK, diepmsk.d32 );
#endif
    dcfg.d32              = HD_USB_RDREG32( &pdev->regs.DREGS->DCFG);
    dcfg.b.devaddr        = 0u;
    HD_USB_WRREG32( &pdev->regs.DREGS->DCFG, dcfg.d32);
    hd_usb_ep0revcfg(pdev);
    gintsts.d32           = 0ul;
    gintsts.b.usbreset    = 1ul;
    HD_USB_WRREG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
    dev_int_cbkpr->Reset(pdev);
}

/**
 *******************************************************************************
 **\brief  Read the device status register and set the device speed
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_enumfinish_isr(usb_core_instance *pdev)
{
    uint32_t  gintsts;

    hd_usb_ep0activate(pdev);
    usb_setaroundtim(pdev);
    gintsts = ((uint32_t)1)<<13;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, gintsts);
}

/**
 *******************************************************************************
 **\brief  handle the ISO IN incomplete interrupt
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_isoinincomplt_isr(usb_core_instance *pdev)
{
    uint32_t gintsts;

    dev_int_cbkpr->IsoINIncomplete (pdev);
    gintsts = ((uint32_t)1)<<20;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, gintsts);
}

/**
 *******************************************************************************
 **\brief  handle the ISO OUT incomplete interrupt
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_isooutincomplt_isr(usb_core_instance *pdev)
{
    uint32_t gintsts;

    dev_int_cbkpr->IsoOUTIncomplete (pdev);
    gintsts = ((uint32_t)1)<<21;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, gintsts);
}

/**
 *******************************************************************************
 **\brief  handles all USB Interrupts
 **\param  pdev: device instance
 **\retval none
 ******************************************************************************/
void hd_usb_isr_handler(usb_core_instance *pdev)
{
    uint32_t tmp_gintsts;

    if(!hd_usb_getcurmod(pdev))
    {        
        tmp_gintsts = hd_usb_getcoreintr(pdev);        
        if(!tmp_gintsts)
        {
            return;
        }
        else if(tmp_gintsts & OUTEP_INT)
        {
            hd_usb_outep_isr(pdev);
        }
        else if(tmp_gintsts & INEP_INT)
        {
            hd_usb_inep_isr(pdev);
        }
        else if(tmp_gintsts & MODEMIS_INT)
        {
            HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, MODEMIS_INT);
        }
        else if(tmp_gintsts & WAKEUP_INT)
        {
            hd_usb_resume_isr(pdev);
        }
        else if(tmp_gintsts & USBSUSP_INT)
        {
            hd_usb_susp_isr(pdev);
        }
        else if(tmp_gintsts & SOF_INT)
        {
            hd_usb_sof_isr(pdev);
        }
        else if(tmp_gintsts & RXFLVL_INT)
        {
            hd_usb_rxstsqlvl_isr(pdev);
        }
        else if(tmp_gintsts & USBRST_INT)
        {
            hd_usb_reset_isr(pdev);
        }
        else if(tmp_gintsts & ENUMDONE_INT)
        {
            hd_usb_enumfinish_isr(pdev);
        }
        else if(tmp_gintsts & INCOMPLSOIN)
        {
            hd_usb_isoinincomplt_isr(pdev);
        }
        else if(tmp_gintsts & INCOMPLSOOUT)
        {
            hd_usb_isooutincomplt_isr(pdev);
        }
#ifdef VBUS_SENSING_ENABLED
        else if(tmp_gintsts & VBUSV_INT)
        {
            hd_usb_sessionrequest_isr(pdev);
        }
        else
        {
           ;
        }
#endif
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
