/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_core_driver.c
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdio.h>
#include "usb_core_driver.h"
#include "usb_bsp.h"


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  core soft reset
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_coresoftrst(usb_core_instance *pdev)
{
    __IO  uint8_t  tmp_status;
    __IO  uint32_t tmp_grstctl;
    __IO  uint32_t tmp_count;

    tmp_status  = HD_USB_OK;
    tmp_grstctl = 0;
    tmp_count   = 0;
    /* Wait for AHB master to be idle. */
    do{
        hd_usb_udelay(1ul);
        tmp_grstctl = HD_USB_RDREG32(&pdev->regs.GREGS->GRSTCTL);
        if(++tmp_count > 100000ul)
        {
            tmp_status = HD_USB_ERROR;
        }
    }while(tmp_grstctl == 0);

    if(HD_USB_OK == tmp_status)
    {
        // Write the Core Soft Reset bit to reset the USB core
        tmp_count     = 0ul;
        tmp_grstctl  |= 1ul;
        HD_USB_WRREG32(&pdev->regs.GREGS->GRSTCTL, tmp_grstctl);
        // Waite for the reset finishing
        do{
            tmp_grstctl = HD_USB_RDREG32(&pdev->regs.GREGS->GRSTCTL);
            if(tmp_count > 100000ul)
            {
              break;
            }
            tmp_count++;
            hd_usb_udelay(1ul);
        }while(tmp_grstctl & 1ul);
        // Wait for at least 3 PHY clocks after the core resets
        hd_usb_udelay(3ul);
    }
}

/**
 *******************************************************************************
 ** \brief  Writes a packet whose byte number is len into the Tx FIFO associated
 **         with the EP
 ** \param  pdev : device instance
 ** \param  src : source pointer used to hold the transmited data
 ** \param  ch_ep_num : end point index
 ** \param  len : length in bytes
 ** \retval none
 ******************************************************************************/
void hd_usb_wrpkt(usb_core_instance *pdev, uint8_t *src, uint8_t ch_ep_num, uint16_t len)
{
    __IO uint32_t u32pAdr;
    __IO uint32_t *fifo;
    uint32_t tmp_count32b;
    uint32_t tmp_1;
    if (pdev->basic_cfgs.dmaen == 0u)
    {
        tmp_count32b = 0ul;
        tmp_1        = 0;
        tmp_count32b = (len + 3);
        tmp_count32b = tmp_count32b>>2;
        fifo = pdev->regs.DFIFO[ch_ep_num];
        tmp_1 = 0;
        while(tmp_1 < tmp_count32b)
        {
            HD_USB_WRREG32( fifo, *((uint32_t *)src) );
            u32pAdr  = (uint32_t)src;
            src      = (uint8_t*)(u32pAdr+4);
            tmp_1++;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Reads a packet whose byte number is len from the Rx FIFO
 ** \param  pdev : device instance
 ** \param  dest : destination pointer that point to the received data
 ** \param  len  : number of bytes
 ** \retval none
 ******************************************************************************/
void hd_usb_rdpkt(usb_core_instance *pdev, uint8_t *dest, uint16_t len)
{
    uint32_t tmp_1;
    __IO uint32_t tmp_count32b;
    __IO uint32_t u32pAdr;

    __IO uint32_t *fifo = pdev->regs.DFIFO[0];
    tmp_count32b            = (len + 3);
    tmp_count32b            = tmp_count32b>>2;
    u32pAdr                 = 0u;
    tmp_1                   = 0;
    while(tmp_1 < tmp_count32b)
    {
        *(uint32_t *)dest = HD_USB_RDREG32(fifo);
        u32pAdr           = (uint32_t)dest;
        dest              = (uint8_t*)(u32pAdr + 4);
        tmp_1++;
    }
}

/**
 *******************************************************************************
 ** \brief  Initialize the addresses of the core registers.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_setregaddr(usb_core_instance *pdev)
{
    uint32_t tmp_1, tmp_baseAddress;

    pdev->basic_cfgs.dmaen         = 0;
    /* initialize device cfg following its address */
    tmp_baseAddress                = 0x400C0000ul;
    pdev->basic_cfgs.host_chnum    = 12u;
    pdev->basic_cfgs.dev_epnum     = 6u;

    pdev->regs.GREGS = (USB_CORE_GREGS *)(tmp_baseAddress + 0);
    pdev->regs.DREGS = (USB_CORE_DREGS *)(tmp_baseAddress + 0x800);
    tmp_1 = 0;
    while(tmp_1 < pdev->basic_cfgs.dev_epnum)
    {
        pdev->regs.INEP_REGS[tmp_1]  = (USB_CORE_INEPREGS *)(tmp_baseAddress + 0x900 + (tmp_1 * 0x20));
        pdev->regs.OUTEP_REGS[tmp_1] = (USB_CORE_OUTEPREGS *)(tmp_baseAddress + 0xb00 + (tmp_1 * 0x20));
        tmp_1++;
    }
    tmp_1 = 0;
    while(tmp_1 < pdev->basic_cfgs.dev_epnum)
    {
        pdev->regs.DFIFO[tmp_1]      = (uint32_t *)(tmp_baseAddress + 0x1000 + (tmp_1 * 0x1000));
        tmp_1++;
    }
    pdev->regs.PCGCCTL = (uint32_t *)(tmp_baseAddress + 0xe00);
#ifdef USE_HOST_MODE     //if the application mode is host
    pdev->regs.HREGS = (USB_CORE_HREGS *)(tmp_baseAddress + 0x400);
    pdev->regs.HPRT0 = (uint32_t *)(tmp_baseAddress + 0x440);
    tmp_1 = 0;
    while(tmp_1 < pdev->basic_cfgs.host_chnum)
    {
        pdev->regs.HC_REGS[tmp_1] = (USB_CORE_HC_REGS *)(tmp_baseAddress + 0x500 + (tmp_1 * 0x20));
        tmp_1++;
    }
#endif  //USE_HOST_MODE
}

/**
 *******************************************************************************
 ** \brief  Initializes the USB controller registers and prepares the core
 **         device mode or host mode operation.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_initusbcore(usb_core_instance *pdev)
{
    __IO uint32_t  tmp_gusbcfg;
    __IO uint32_t  tmp_gahbcfg;

    tmp_gusbcfg  = 0;
    tmp_gahbcfg  = 0;
    // reset the core through core soft reset
    hd_usb_coresoftrst(pdev);
    // select the speed mode
    tmp_gusbcfg  = HD_USB_RDREG32(&pdev->regs.GREGS->GUSBCFG);
    tmp_gusbcfg |= 1u<<6; //select the full-speed serial transceiver
    HD_USB_WRREG32(&pdev->regs.GREGS->GUSBCFG, tmp_gusbcfg);
    // reset the core through core soft reset
    hd_usb_coresoftrst(pdev);
    hd_usb_mdelay(20ul);
    if(pdev->basic_cfgs.dmaen == 1u)
    {
        tmp_gahbcfg = HD_USB_RDREG32(&pdev->regs.GREGS->GAHBCFG);
        tmp_gahbcfg = 5u<<1;   //burst length/type(HBstLen) 64-words x32-bit
        tmp_gahbcfg = 1u<<5;   //core operates in a DMA mode
        HD_USB_WRREG32(&pdev->regs.GREGS->GAHBCFG, tmp_gahbcfg);
    }
}

/**
 *******************************************************************************
 ** \brief  Flush a Tx FIFO whose index is num
 ** \param  pdev : device instance
 ** \param  num : txFIFO index
 ** \retval none
 ******************************************************************************/
void hd_usb_txfifoflush(usb_core_instance *pdev, uint32_t num)
{
    __IO uint32_t tmp_grstctl;
    __IO uint32_t tmp_1;

    tmp_1        = 0;
    tmp_grstctl  = 0ul;             //set the init value to be zero
    tmp_grstctl |= 1u<<5;           //set the TxFIFO Flush bit
    tmp_grstctl |= (num&0x1f)<<6;   //TxFIFO Number
    HD_USB_WRREG32(&pdev->regs.GREGS->GRSTCTL, tmp_grstctl);
    // wait for the finishing of txFIFO flushing
    do{
        tmp_grstctl = HD_USB_RDREG32(&pdev->regs.GREGS->GRSTCTL);
        if(tmp_1<=200000ul)
        {
            tmp_1++;
        }
        else
        {
            break;
        }
        hd_usb_udelay(1ul);
    }while(tmp_grstctl&(1u<<5));
    // Wait for at least 3 PHY clocks after the txfifo has been flushed
    hd_usb_udelay(3ul);
}

/**
 *******************************************************************************
 ** \brief  Flush the whole rxFIFO
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_rxfifoflush(usb_core_instance *pdev)
{
    __IO uint32_t tmp_grstctl;
    __IO uint32_t tmp_1;

    tmp_1        = 0;
    tmp_grstctl  = 0;
    tmp_grstctl |= 1ul<<4;      //set the RxFIFO Flush bit
    HD_USB_WRREG32(&pdev->regs.GREGS->GRSTCTL, tmp_grstctl);
    // wait for the finishing of rxFIFO flushing
    do{
        tmp_grstctl = HD_USB_RDREG32( &pdev->regs.GREGS->GRSTCTL);
        if(tmp_1<=200000ul)
        {
            tmp_1++;
        }
        else
        {
            break;
        }
        hd_usb_udelay(1ul);
    }while(tmp_grstctl & (1ul<<4));
    // Wait for at least 3 PHY clocks after the rxfifo has been flushed
    hd_usb_udelay(3ul);
}

/**
 *******************************************************************************
 ** \brief  set the core to be host mode or device mode through the second
 **         input parameter.
 ** \param  pdev : device instance
 ** \param  mode :  mode of HOST_MODE or DEVICE_MODE that the core would be
 ** \retval none
 ******************************************************************************/
void hd_usb_modeset(usb_core_instance *pdev, uint8_t mode)
{
    uint32_t  tmp_gusbcfg;

    tmp_gusbcfg  = HD_USB_RDREG32(&pdev->regs.GREGS->GUSBCFG);
    tmp_gusbcfg &= ~(3ul<<29);
    if ( mode == HOST_MODE)
    {
        tmp_gusbcfg |= 1ul<<29;
    }
    else
    {
        tmp_gusbcfg |= 2ul<<29;
    }
    HD_USB_WRREG32(&pdev->regs.GREGS->GUSBCFG, tmp_gusbcfg);
    // wate for the change to take effect
    hd_usb_mdelay(50ul);
}

/**
 *******************************************************************************
 ** \brief  initializes the initial status of all endpoints of the device to be
 **         disable.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_devepdis(usb_core_instance *pdev)
{
    __IO uint8_t tmp_1;
    USB_CORE_DEPCTL_TypeDef  depctl;

    tmp_1 = 0;
    while(tmp_1 < pdev->basic_cfgs.dev_epnum)
    {
        depctl.b = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(&pdev->regs.INEP_REGS[tmp_1]->DIEPCTL);
        if (depctl.b.epena)
        {
            *(uint32_t*)&depctl.b = 0ul;
            depctl.b.epdis = 1u;
            depctl.b.snak = 1u;
        }
        else
        {
            *(uint32_t*)&depctl.b = 0ul;
        }
        HD_USB_WRREG32( &pdev->regs.INEP_REGS[tmp_1]->DIEPCTL, *(uint32_t*)&depctl.b);
        HD_USB_WRREG32( &pdev->regs.INEP_REGS[tmp_1]->DIEPTSIZ, 0ul);
        HD_USB_WRREG32( &pdev->regs.INEP_REGS[tmp_1]->DIEPINT, 0xFFul);
        tmp_1++;
    }

    tmp_1 = 0;
    while(tmp_1 < pdev->basic_cfgs.dev_epnum)
    {
        depctl.b = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(&pdev->regs.OUTEP_REGS[tmp_1]->DOEPCTL);
        if (depctl.b.epena)
        {
            *(uint32_t*)&depctl.b = 0ul;
            depctl.b.epdis        = 1u;
            depctl.b.snak         = 1u;
        }
        else
        {
            *(uint32_t*)&depctl.b = 0ul;
        }
        HD_USB_WRREG32( &pdev->regs.OUTEP_REGS[tmp_1]->DOEPCTL, *(uint32_t*)&depctl.b);
        HD_USB_WRREG32( &pdev->regs.OUTEP_REGS[tmp_1]->DOEPTSIZ, 0ul);
        HD_USB_WRREG32( &pdev->regs.OUTEP_REGS[tmp_1]->DOEPINT, 0xFFul);
        tmp_1++;
    }
}

/**
 *******************************************************************************
 ** \brief  initializes the USB controller, include the size of txFIFO, rxFIFO
 **         status of endpoints, interrupt register etc. Details are shown as
 **         follows.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_devmodeinit(usb_core_instance *pdev)
{
    USB_CORE_DCFG_TypeDef    dcfg;
    USB_CORE_FSIZ_TypeDef    nptxfifosize;
    USB_CORE_FSIZ_TypeDef    txfifosize;

    dcfg.d32 = 0ul;
    *(uint32_t*)&nptxfifosize.b = 0ul;
    *(uint32_t*)&txfifosize.b   = 0ul;
    dcfg.d32 = HD_USB_RDREG32( &pdev->regs.DREGS->DCFG);
    dcfg.b.perfrint             = 0;
    dcfg.b.devspd               = 3;
    HD_USB_WRREG32( &pdev->regs.DREGS->DCFG, dcfg.d32 );

    HD_USB_WRREG32(&pdev->regs.GREGS->GRXFSIZ, (uint32_t)RX_FIFO_FS_SIZE);

    /* set txFIFO and rxFIFO size of EP0 */
    nptxfifosize.b.depth        = (uint32_t)TX0_FIFO_FS_SIZE;
    nptxfifosize.b.startaddr    = (uint32_t)RX_FIFO_FS_SIZE;
    HD_USB_WRREG32(&pdev->regs.GREGS->GNPTXFSIZ, *(uint32_t*)&nptxfifosize.b );

    /* set txFIFO size of EP1 */
    txfifosize.b.startaddr      = nptxfifosize.b.startaddr + nptxfifosize.b.depth;
    txfifosize.b.depth = (uint32_t)TX1_FIFO_FS_SIZE;
    HD_USB_WRREG32(&pdev->regs.GREGS->DPTXFSIZ[0], *(uint32_t*)&txfifosize.b );

    /* set txFIFO size of EP2 */
    txfifosize.b.startaddr     += txfifosize.b.depth;
    txfifosize.b.depth          = (uint32_t)TX2_FIFO_FS_SIZE;
    HD_USB_WRREG32(&pdev->regs.GREGS->DPTXFSIZ[1], *(uint32_t*)&txfifosize.b );

    /* set txFIFO size of EP3 */
    txfifosize.b.startaddr     += txfifosize.b.depth;
    txfifosize.b.depth = (uint32_t)TX3_FIFO_FS_SIZE;
    HD_USB_WRREG32( &pdev->regs.GREGS->DPTXFSIZ[2], *(uint32_t*)&txfifosize.b );

    /* set txFIFO size of EP4 */
    txfifosize.b.startaddr     += txfifosize.b.depth;
    txfifosize.b.depth          = (uint32_t)TX4_FIFO_FS_SIZE;
    HD_USB_WRREG32( &pdev->regs.GREGS->DPTXFSIZ[3], *(uint32_t*)&txfifosize.b );

    /* set txFIFO size of EP5 */
    txfifosize.b.startaddr     += txfifosize.b.depth;
    txfifosize.b.depth          = (uint32_t)TX5_FIFO_FS_SIZE;
    HD_USB_WRREG32( &pdev->regs.GREGS->DPTXFSIZ[4], *(uint32_t*)&txfifosize.b );

    hd_usb_clrandmskepint(pdev);
    hd_usb_devepdis(pdev);
    hd_usb_coreconn(pdev);
    hd_usb_devinten(pdev);
}

/**
 *******************************************************************************
 ** \brief  Enable the interrupt setting when in device mode.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_devinten(usb_core_instance *pdev)
{
    USB_CORE_GINTMSK_TypeDef  tmp_gintmsk;

    tmp_gintmsk.d32 = 0ul;
    HD_USB_WRREG32( &pdev->regs.GREGS->GINTMSK, 0ul);
    HD_USB_WRREG32( &pdev->regs.GREGS->GINTSTS, 0xBFFFFFFFul);
    /* Enable the normal interrupt setting */
    hd_usb_normalinten(pdev);
    if (pdev->basic_cfgs.dmaen == 0u)
    {
        tmp_gintmsk.b.rxstsqlvl = 1u;
    }
    /* Enable interrupts bits corresponding to the Device mode */
    tmp_gintmsk.b.usbsuspend       = 1u;
    tmp_gintmsk.b.usbreset         = 1u;
    tmp_gintmsk.b.enumdone         = 1u;
    tmp_gintmsk.b.inepintr         = 1u;
    tmp_gintmsk.b.outepintr        = 1u;
    tmp_gintmsk.b.sofintr          = 1u;
    tmp_gintmsk.b.incomplisoin     = 1u;
    tmp_gintmsk.b.incomplisoout    = 1u;
#ifdef VBUS_SENSING_ENABLED
    tmp_gintmsk.b.vbusvint         = 1u;
#endif
    HD_USB_MDFREG32( &pdev->regs.GREGS->GINTMSK, tmp_gintmsk.d32, tmp_gintmsk.d32);
}

/**
 *******************************************************************************
 ** \brief  get the working status of endpoint.
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval current status of the endpoint
 ******************************************************************************/
uint32_t hd_usb_epstatusget(usb_core_instance *pdev ,USB_DEV_EP *ep)
{
    USB_CORE_DEPCTL_TypeDef  tmp_depctl;
    __IO uint32_t *tmp_depctl_addr;
    __IO uint32_t tmp_status = 0ul;

    *(uint32_t*)&tmp_depctl.b = 0ul;
    if (ep->ep_dir == 1u)
    {
        tmp_depctl_addr = &(pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL);
        tmp_depctl.b    = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_depctl_addr);
        if(tmp_depctl.b.stall == 1u)
        {
            tmp_status  = USB_EP_TX_STALL;
        }
        else if(tmp_depctl.b.naksts == 1u)
        {
            tmp_status  = USB_EP_TX_NAK;
        }
        else
        {
            tmp_status  = USB_EP_TX_VALID;
        }
    }
    else
    {
        tmp_depctl_addr = &(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL);
        tmp_depctl.b    = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_depctl_addr);
        if (tmp_depctl.b.stall == 1u)
        {
            tmp_status  = USB_EP_RX_STALL;
        }
        else if (tmp_depctl.b.naksts == 1u)
        {
            tmp_status  = USB_EP_RX_NAK;
        }
        else
        {
            tmp_status  = USB_EP_RX_VALID;
        }
    }

    return tmp_status;
}

/**
 *******************************************************************************
 ** \brief  set the working status of endpoint.
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \param  Status : new Status that the endpoint would be
 ** \retval none
 ******************************************************************************/
void hd_usb_epstatusset(usb_core_instance *pdev, USB_DEV_EP *ep, uint32_t Status)
{
    USB_CORE_DEPCTL_TypeDef  tmp_depctl;
    __IO uint32_t *tmp_depctl_addr;

    uint8_t u8RetFlag                 = 0u;
    *(uint32_t*)&tmp_depctl.b         = 0ul;
    if (ep->ep_dir == 1u)
    {
        tmp_depctl_addr = &(pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL);
        tmp_depctl.b    = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_depctl_addr);
        switch(Status)
        {
            case USB_EP_TX_STALL:
                hd_usb_setepstall(pdev, ep);
                u8RetFlag             = 1u;
            break;
            case USB_EP_TX_NAK:
                tmp_depctl.b.snak     = 1u;
            break;
            case USB_EP_TX_VALID:
                if (tmp_depctl.b.stall == 1u)
                {
                    ep->datax_pid     = 0u;
                    hd_usb_clearepstall(pdev, ep);
                    u8RetFlag         = 1u;
                }
                tmp_depctl.b.cnak     = 1u;
                tmp_depctl.b.usbactep = 1u;
                tmp_depctl.b.epena    = 1u;
            break;
            case USB_EP_TX_DIS:
                tmp_depctl.b.usbactep = 0u;
            break;
            default:
            break;
        }
    }
    else
    {
        tmp_depctl_addr = &(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL);
        tmp_depctl.b = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_depctl_addr);
        switch(Status)
        {
            case USB_EP_RX_STALL:
                tmp_depctl.b.stall    = 1u;
            break;
            case USB_EP_RX_NAK:
                tmp_depctl.b.snak     = 1u;
            break;
            case USB_EP_RX_VALID:
                if (tmp_depctl.b.stall == 1u)
                {
                    ep->datax_pid     = 0u;
                    hd_usb_clearepstall(pdev, ep);
                    u8RetFlag         = 1u;
                }
                tmp_depctl.b.cnak     = 1u;
                tmp_depctl.b.usbactep = 1u;
                tmp_depctl.b.epena    = 1u;
            break;
            case USB_EP_RX_DIS:
                tmp_depctl.b.usbactep = 0u;
            break;
            default:
            break;
        }
    }
    if(1u != u8RetFlag)
    {
        HD_USB_WRREG32(tmp_depctl_addr, *(uint32_t*)&tmp_depctl.b);
    }
}

/**
 *******************************************************************************
 ** \brief  enable the EP0 to be actiove
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void  hd_usb_ep0activate(usb_core_instance *pdev)
{
    USB_CORE_DSTS_TypeDef    tmp_dsts;
    USB_CORE_DEPCTL_TypeDef  tmp_diepctl;
    USB_CORE_DCTL_TypeDef    tmp_dctl;

    tmp_dctl.d32 = 0ul;
    tmp_dsts.b = *(__IO stc_bUSB_CORE_DSTS_t*)&HD_USB_RDREG32(&pdev->regs.DREGS->DSTS);
    tmp_diepctl.d32 = HD_USB_RDREG32(&pdev->regs.INEP_REGS[0]->DIEPCTL);
    /* Set the MPS of the DIEPCTL0 based on the enumeration speed */
    if(tmp_dsts.b.enumspd == DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ \
        || tmp_dsts.b.enumspd == DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ\
        || tmp_dsts.b.enumspd == DSTS_ENUMSPD_FS_PHY_48MHZ)
    {
        tmp_diepctl.b.mps = 0;
    }
    else if(tmp_dsts.b.enumspd == DSTS_ENUMSPD_LS_PHY_6MHZ)
    {
        tmp_diepctl.b.mps = 3;
    }
    else
    {
        ;
    }
    HD_USB_WRREG32(&pdev->regs.INEP_REGS[0]->DIEPCTL, tmp_diepctl.d32);
    tmp_dctl.b.cgnpinnak = 1u;
    HD_USB_MDFREG32(&pdev->regs.DREGS->DCTL, tmp_dctl.d32, tmp_dctl.d32);
}


/**
 *******************************************************************************
 ** \brief  enable an EP to be active
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval none
 ******************************************************************************/
void hd_usb_epactive(usb_core_instance *pdev, USB_DEV_EP *ep)
{
    USB_CORE_DEPCTL_TypeDef  tmp_depctl;
    USB_CORE_DAINT_TypeDef  tmp_daintmsk;
    __IO uint32_t *tmp_addr;

    *(uint32_t*)&tmp_depctl.b = 0ul;
    tmp_daintmsk.d32          = 0ul;
    if (ep->ep_dir == 1u)
    {
        tmp_addr              = &pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL;
        tmp_daintmsk.ep.in    = (uint32_t)1 << ep->epidx;
    }
    else
    {
        tmp_addr              = &pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL;
        tmp_daintmsk.ep.out   = (uint32_t)1 << ep->epidx;
    }
    tmp_depctl.b = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_addr);
    if (!tmp_depctl.b.usbactep)
    {
        tmp_depctl.b.mps      = ep->maxpacket;
        tmp_depctl.b.eptype   = ep->trans_type;
        tmp_depctl.b.txfnum   = ep->tx_fifo_num;
        tmp_depctl.b.setd0pid = 1u;
        tmp_depctl.b.usbactep = 1u;
        HD_USB_WRREG32(tmp_addr, *(uint32_t*)&tmp_depctl.d32);
    }
    HD_USB_MDFREG32(&pdev->regs.DREGS->DAINTMSK, 0ul, tmp_daintmsk.d32);
}

/**
 *******************************************************************************
 ** \brief  enable an EP to be deactive state if it is active
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval none
 ******************************************************************************/
void hd_usb_epdeactive(usb_core_instance *pdev, USB_DEV_EP *ep)
{
    __IO uint32_t  tmp_depctl;
    USB_CORE_DAINT_TypeDef  tmp_daintmsk;
    __IO uint32_t *addr;

    tmp_depctl       = 0ul;
    tmp_daintmsk.d32 = 0ul;
    if (ep->ep_dir == 1u)
    {
        addr                = &pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL;
        tmp_daintmsk.ep.in  = (uint32_t)1 << ep->epidx;
    }
    else
    {
        addr                = &pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL;
        tmp_daintmsk.ep.out = (uint32_t)1 << ep->epidx;
    }
    tmp_depctl             &= ~(1ul<<15);
    HD_USB_WRREG32(addr, tmp_depctl);
    HD_USB_MDFREG32(&pdev->regs.DREGS->DAINTMSK, tmp_daintmsk.d32, 0ul);
}

/**
 *******************************************************************************
 ** \brief  Setup the data into the EP and begin to transmit data.
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval none
 ******************************************************************************/
void hd_usb_epntransbegin(usb_core_instance *pdev, USB_DEV_EP *ep)
{
    USB_CORE_DEPCTL_TypeDef     depctl;
    USB_CORE_DEPXFRSIZ_TypeDef  deptsiz;
    USB_CORE_DSTS_TypeDef       dsts;
    uint32_t fifoemptymsk = 0ul;

    depctl.d32             = 0ul;
    *(uint32_t*)&deptsiz.b = 0ul;
    if (ep->ep_dir == 1u)
    {
        depctl.d32  = HD_USB_RDREG32(&(pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL));
        deptsiz.b   = *(__IO stc_bUSB_CORE_DEPXFRSIZ_t*)&HD_USB_RDREG32(&(pdev->regs.INEP_REGS[ep->epidx]->DIEPTSIZ));
        /* Zero Length Packet? */
        if (ep->xfer_len == 0ul)
        {
            deptsiz.b.xfersize = 0u;
            deptsiz.b.pktcnt   = 1u;
        }
        else
        {
            /* Program the transfer size and packet count
            * as follows: xfersize = N * maxpacket +
            * short_packet pktcnt = N + (short_packet
            * exist ? 1 : 0)
            */
            deptsiz.b.xfersize = ep->xfer_len;
            deptsiz.b.pktcnt   = (ep->xfer_len - 1u + ep->maxpacket) / ep->maxpacket;

            if (ep->trans_type == EP_TYPE_ISOC)
            {
                deptsiz.b.mc = 1u;
            }
        }
        HD_USB_WRREG32(&pdev->regs.INEP_REGS[ep->epidx]->DIEPTSIZ, *(uint32_t*)&deptsiz.b);

        if (pdev->basic_cfgs.dmaen == 1u)
        {
            HD_USB_WRREG32(&pdev->regs.INEP_REGS[ep->epidx]->DIEPDMA, ep->dma_addr);
        }
        else
        {
            if (ep->trans_type != EP_TYPE_ISOC)
            {
                /* Enable the Tx FIFO Empty Interrupt for this EP */
                if (ep->xfer_len > 0u)
                {
                    fifoemptymsk = (uint32_t)1 << ep->epidx;
                    HD_USB_MDFREG32(&pdev->regs.DREGS->DIEPEMPMSK, 0ul, fifoemptymsk);
                }
            }
        }

        if (ep->trans_type == EP_TYPE_ISOC)
        {
            dsts.b = *(__IO stc_bUSB_CORE_DSTS_t*)&HD_USB_RDREG32(&pdev->regs.DREGS->DSTS);

            if (((dsts.b.soffn)&0x1u) == 0u)
            {
                depctl.b.setd1pid = 1u;
            }
            else
            {
                depctl.b.setd0pid = 1u;
            }
        }

        depctl.b.cnak  = 1u;
        depctl.b.epena = 1u;
        HD_USB_WRREG32(&pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL, depctl.d32);

        if (ep->trans_type == EP_TYPE_ISOC)
        {
            hd_usb_wrpkt(pdev, ep->xfer_buff, ep->epidx, (uint16_t)ep->xfer_len);
        }
    }
    else
    {
        depctl.d32  = HD_USB_RDREG32(&(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL));
        deptsiz.b = *(__IO stc_bUSB_CORE_DEPXFRSIZ_t*)&HD_USB_RDREG32(&(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPTSIZ));
        /* Program the transfer size and packet count as follows:
        * pktcnt = N
        * xfersize = N * maxpacket
        */
        if (ep->xfer_len == 0u)
        {
            deptsiz.b.xfersize = ep->maxpacket;
            deptsiz.b.pktcnt   = 1u;
        }
        else
        {
            deptsiz.b.pktcnt   = (ep->xfer_len + (ep->maxpacket - 1u)) / ep->maxpacket;
            deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
            ep->xfer_len       = deptsiz.b.xfersize;
        }
        HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPTSIZ, *(uint32_t*)&deptsiz.b);

        if (pdev->basic_cfgs.dmaen == 1u)
        {
            HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPDMA, ep->dma_addr);
        }

        if (ep->trans_type == EP_TYPE_ISOC)
        {
            if (ep->datax_pid)
            {
                depctl.b.setd1pid = 1u;
            }
            else
            {
                depctl.b.setd0pid = 1u;
            }
        }
        depctl.b.cnak  = 1u;
        depctl.b.epena = 1u;
        HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL, depctl.d32);
    }
}

/**
 *******************************************************************************
 ** \brief  Setup the data into the EP0 and begin to transmit data.
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval none
 ******************************************************************************/
void hd_usb_ep0transbegin(usb_core_instance *pdev, USB_DEV_EP *ep)
{
    USB_CORE_DEPCTL_TypeDef      depctl;
    USB_CORE_DEP0XFRSIZ_TypeDef  deptsiz;
    USB_CORE_INEPREGS          *in_regs;
    uint32_t fifoemptymsk = 0ul;

    depctl.d32   = 0ul;
    deptsiz.d32  = 0ul;
    if (ep->ep_dir == 1u)
    {
        in_regs     = pdev->regs.INEP_REGS[0];
        depctl.d32  = HD_USB_RDREG32(&in_regs->DIEPCTL);
        deptsiz.d32 = HD_USB_RDREG32(&in_regs->DIEPTSIZ);
        /* Zero Length Packet? */
        if (ep->xfer_len == 0u)
        {
            deptsiz.b.xfersize = 0u;
            deptsiz.b.pktcnt   = 1u;
        }
        else
        {
            if (ep->xfer_len > ep->maxpacket)
            {
                ep->xfer_len       = ep->maxpacket;
                deptsiz.b.xfersize = ep->maxpacket;
            }
            else
            {
                deptsiz.b.xfersize = ep->xfer_len;
            }
            deptsiz.b.pktcnt       = 1u;
        }
        HD_USB_WRREG32(&in_regs->DIEPTSIZ, deptsiz.d32);

        if (pdev->basic_cfgs.dmaen == 1u)
        {
            HD_USB_WRREG32(&pdev->regs.INEP_REGS[ep->epidx]->DIEPDMA, ep->dma_addr);
        }

        depctl.b.cnak  = 1u;
        depctl.b.epena = 1u;
        HD_USB_WRREG32(&in_regs->DIEPCTL, depctl.d32);

        if (pdev->basic_cfgs.dmaen == 0u)
        {
            /* Enable the Tx FIFO Empty Interrupt for this EP */
            if (ep->xfer_len > 0u)
            {
                fifoemptymsk |= (uint32_t)1 << ep->epidx;
                HD_USB_MDFREG32(&pdev->regs.DREGS->DIEPEMPMSK, 0ul, fifoemptymsk);
            }
        }
    }
    else
    {
        depctl.d32  = HD_USB_RDREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL);
        deptsiz.d32 = HD_USB_RDREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPTSIZ);
        /* Program the transfer size and packet count as follows:
        * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
        * pktcnt = N           */
        if (ep->xfer_len == 0u)
        {
            deptsiz.b.xfersize = 0u;
            deptsiz.b.pktcnt   = 1u;
        }
        else
        {
            ep->xfer_len       = __MIN(ep->rem_data_len, ep->maxpacket);
            deptsiz.b.xfersize = ep->xfer_len;
            deptsiz.b.pktcnt   = 1u;
        }
        HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPTSIZ, deptsiz.d32);
        if (pdev->basic_cfgs.dmaen == 1u)
        {
            HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[ep->epidx]->DOEPDMA, ep->dma_addr);
        }
        depctl.b.cnak  = 1u;
        depctl.b.epena = 1u;
        HD_USB_WRREG32 (&(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL), depctl.d32);
    }
}

/**
 *******************************************************************************
 ** \brief  Set the EP to be stall status
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval none
 ******************************************************************************/
void hd_usb_setepstall(usb_core_instance *pdev, USB_DEV_EP *ep)
{
    USB_CORE_DEPCTL_TypeDef  tmp_depctl;
    __IO uint32_t *tmp_depctl_addr;

    *(uint32_t*)&tmp_depctl.b = 0ul;
    if (ep->ep_dir == 1u)
    {
        tmp_depctl_addr = &(pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL);
        tmp_depctl.b    = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_depctl_addr);
        if (tmp_depctl.b.epena)
        {
            tmp_depctl.b.epdis = 1u;
        }
        tmp_depctl.b.stall     = 1u;
        HD_USB_WRREG32(tmp_depctl_addr, *(uint32_t*)&tmp_depctl.b);
    }
    else
    {
        tmp_depctl_addr    = &(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL);
        tmp_depctl.b       = *(__IO stc_bUSB_CORE_DEPCTL_t*)&HD_USB_RDREG32(tmp_depctl_addr);
        tmp_depctl.b.stall = 1u;
        HD_USB_WRREG32(tmp_depctl_addr, *(uint32_t*)&tmp_depctl.b);
    }
}

/**
 *******************************************************************************
 ** \brief  clear the stall status of a EP
 ** \param  pdev : device instance
 ** \param  ep   : endpoint instance
 ** \retval none
 ******************************************************************************/
void hd_usb_clearepstall(usb_core_instance *pdev, USB_DEV_EP *ep)
{
    USB_CORE_DEPCTL_TypeDef  tmp_depctl;
    __IO uint32_t *tmp_depctl_addr;

    tmp_depctl.d32 = 0ul;

    if (ep->ep_dir == 1u)
    {
        tmp_depctl_addr = &(pdev->regs.INEP_REGS[ep->epidx]->DIEPCTL);
    }
    else
    {
        tmp_depctl_addr = &(pdev->regs.OUTEP_REGS[ep->epidx]->DOEPCTL);
    }
    tmp_depctl.d32     = HD_USB_RDREG32(tmp_depctl_addr);
    tmp_depctl.b.stall = 0u;
    if ((ep->trans_type == EP_TYPE_INTR) || (ep->trans_type == EP_TYPE_BULK))
    {
        tmp_depctl.b.setd0pid = 1u;
    }
    HD_USB_WRREG32(tmp_depctl_addr, tmp_depctl.d32);
}

/**
 *******************************************************************************
 ** \brief  configure the EPO to receive data packets
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_ep0revcfg(usb_core_instance *pdev)
{
    USB_CORE_DEP0XFRSIZ_TypeDef  tmp_doeptsize0;
    USB_CORE_DEPCTL_TypeDef  tmp_doepctl;

    tmp_doeptsize0.d32                 = 0ul;
    tmp_doeptsize0.b.supcnt            = 3u;
    tmp_doeptsize0.b.pktcnt            = 1u;
    tmp_doeptsize0.b.xfersize          = 64u;
    pdev->dev.out_ep[0].xfer_len       = 64u;
    pdev->dev.out_ep[0].rem_data_len   = 64u;
    pdev->dev.out_ep[0].total_data_len = 64u;
    HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[0]->DOEPTSIZ, tmp_doeptsize0.d32 );

    if (pdev->basic_cfgs.dmaen == 1u)
    {
        tmp_doepctl.d32     = 0ul;
        HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[0]->DOEPDMA, (uint32_t)&pdev->dev.setup_pkt_buf);
        tmp_doepctl.d32     = HD_USB_RDREG32(&pdev->regs.OUTEP_REGS[0]->DOEPCTL);
        tmp_doepctl.b.epena = 1u;
        tmp_doepctl.d32     = 0x80008000ul;
        HD_USB_WRREG32(&pdev->regs.OUTEP_REGS[0]->DOEPCTL, tmp_doepctl.d32);
    }
}

/**
 *******************************************************************************
 ** \brief  enable remote wakeup active
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_remotewakeupen(usb_core_instance *pdev)
{
    USB_CORE_DCTL_TypeDef     dctl;
    USB_CORE_DSTS_TypeDef     dsts;
    USB_CORE_PCGCCTL_TypeDef  power;

    if (pdev->dev.device_remote_wakeup)
    {
        dsts.b = *(__IO stc_bUSB_CORE_DSTS_t*)&HD_USB_RDREG32(&pdev->regs.DREGS->DSTS);
        if(dsts.b.suspsts == 1u)
        {
            if(pdev->basic_cfgs.low_power)
            {
                /* un-gate USB Core clock */
                power.d32 = HD_USB_RDREG32(&pdev->regs.PCGCCTL);
                power.b.gatehclk = 0u;
                power.b.stoppclk = 0u;
                HD_USB_WRREG32(pdev->regs.PCGCCTL, power.d32);
            }
        }
    }
    dctl.d32 = 0ul;
    dctl.b.rmtwkupsig = 1u;
    HD_USB_MDFREG32(&pdev->regs.DREGS->DCTL, 0ul, dctl.d32);
    hd_usb_mdelay(5ul);
    HD_USB_MDFREG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0ul );
}

/**
 *******************************************************************************
 ** \brief  control the device to connect or disconnect
 ** \param  pdev: device instance
 ** \param  link: 1(conn) or 0(disconn)
 ** \retval none
 ******************************************************************************/
void hd_usb_ctrldevconnect(usb_core_instance *pdev, uint8_t link)
{
    USB_CORE_DCTL_TypeDef  dctl;
    dctl.d32 = HD_USB_RDREG32(&pdev->regs.DREGS->DCTL);
    dctl.b.sftdiscon  = link;
    HD_USB_WRREG32(&pdev->regs.DREGS->DCTL, dctl.d32);
    hd_usb_mdelay(3ul);
}

#ifdef USE_HOST_MODE
/**
 *******************************************************************************
 ** \brief  Initializes the USB controller when it is host mode
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_hostmodeinit(usb_core_instance *pdev)
{
    __IO uint8_t tmp_1;
    /* configure charge pump IO */
    hd_usb_bsp_cfgvbus(pdev);
    hd_usb_vbusctrl(pdev, 1u);
    hd_usb_mdelay(50ul);
    
    HD_USB_WRREG32(pdev->regs.PCGCCTL, 0ul);  //reset the register-PCGCCTL
    hd_usb_fslspclkselset(pdev , HCFG_6_MHZ); //PHY clock is running at 6MHz
    hd_usb_hprtrst(pdev);                     //reset the port
    hd_usb_enumspeed(pdev);                   //FS or LS bases on the maximum speed supported by the connected device
    hd_usb_sethostfifo(pdev);
    /* Flush all the txFIFO and the whole rxFIFO */
    hd_usb_txfifoflush(pdev, 0x10ul);
    hd_usb_rxfifoflush(pdev);
    /* Clear all HC Interrupt bits that are pending */
    tmp_1 = 0;
    while(tmp_1 < pdev->basic_cfgs.host_chnum)
    {
        HD_USB_WRREG32( &pdev->regs.HC_REGS[tmp_1]->HCINT, 0xFFFFFFFFul );
        HD_USB_WRREG32( &pdev->regs.HC_REGS[tmp_1]->HCINTMSK, 0ul );
        tmp_1++;
    }
    hd_usb_hostinten(pdev);
}

/**
 *******************************************************************************
 ** \brief  set the vbus if state is 1 or reset the vbus if state is 0.
 ** \param  pdev : device instance
 ** \param  state : the vbus state it would be.
 ** \retval none
 ******************************************************************************/
void hd_usb_vbusctrl(usb_core_instance *pdev, uint8_t state)
{
    USB_CORE_HPRT_TypeDef     tmp_hprt;
    __IO uint32_t u32ReadHprtTemp = 0ul;
    *(uint32_t*)&tmp_hprt.b  = 0ul;
    /* enable or disable the external charge pump */
    hd_usb_bsp_drivevbus(pdev, state);
    u32ReadHprtTemp = hd_usb_rdhprt(pdev);
    tmp_hprt.b      = *(stc_bUSB_CORE_HPRT0_t*)&u32ReadHprtTemp;
    if ((tmp_hprt.b.prtpwr == 0u ) && (state == 1u ))
    {
        tmp_hprt.b.prtpwr = 1u;
        HD_USB_WRREG32(pdev->regs.HPRT0, *(uint32_t*)&tmp_hprt.b);
    }
    if ((tmp_hprt.b.prtpwr == 1u ) && (state == 0u ))
    {
        tmp_hprt.b.prtpwr = 0u;
        HD_USB_WRREG32(pdev->regs.HPRT0, *(uint32_t*)&tmp_hprt.b);
    }
    
}

/**
 *******************************************************************************
 ** \brief  Enables the related interrupts when the core is host mode
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_hostinten(usb_core_instance *pdev)
{
    USB_CORE_GINTMSK_TypeDef  intmsk;
    intmsk.d32 = 0ul;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTMSK, 0ul);

    /* Clear the pending interrupt bits */
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, 0xFFFFFFFFul);

    /* Enable the normal interrupt bits */
    hd_usb_normalinten(pdev);

    if (pdev->basic_cfgs.dmaen == 0u)
    {
        intmsk.b.rxstsqlvl  = 1u;
    }
    intmsk.b.portintr   = 1u;
    intmsk.b.hcintr     = 1u;
    intmsk.b.disconnect = 1u;
    intmsk.b.sofintr    = 1u;
    intmsk.b.incomplisoout  = 1u;
    HD_USB_MDFREG32(&pdev->regs.GREGS->GINTMSK, intmsk.d32, intmsk.d32);
}

/**
 *******************************************************************************
 ** \brief  Reset the port, the 1'b0 state must last at lease 10ms.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_hprtrst(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hprt;

    tmp_hprt.d32 = hd_usb_rdhprt(pdev);
    tmp_hprt.b.prtrst = 1u;
    HD_USB_WRREG32(pdev->regs.HPRT0, tmp_hprt.d32);
    hd_usb_mdelay (10ul);
    tmp_hprt.b.prtrst = 0u;
    HD_USB_WRREG32(pdev->regs.HPRT0, tmp_hprt.d32);
    hd_usb_mdelay (20ul);
}

/**
 *******************************************************************************
 ** \brief  Prepares transferring packets on a host channel
 ** \param  pdev : device instance
 ** \param  hc_num : channel index
 ** \retval status in byte
 ******************************************************************************/
uint8_t hd_usb_inithch(usb_core_instance *pdev, uint8_t hc_num)
{
    __IO uint8_t                status;
    uint32_t               intr_enable;
    USB_CORE_HCINTMSK_TypeDef   tmp_hcintmsk;
    USB_CORE_GINTMSK_TypeDef    tmp_gintmsk;
    USB_CORE_HCCHAR_TypeDef     tmp_hcchar;
    USB_CORE_HCINTn_TypeDef     tmp_hcint;

    status = HD_USB_OK;
    tmp_gintmsk.d32  = 0ul;
    tmp_hcintmsk.d32 = 0ul;
    tmp_hcchar.d32   = 0ul;
    tmp_hcint.d32    = 0xFFFFFFFFul;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCINT, tmp_hcint.d32);
    /* Enable the channel interrupts required for this transferring */
    tmp_hcintmsk.d32 = 0ul;

    if (pdev->basic_cfgs.dmaen == 1u)
    {
        tmp_hcintmsk.b.ahberr = 1u;
    }

    switch (pdev->host.hc[hc_num].ep_type)
    {
        case EP_TYPE_CTRL:
        case EP_TYPE_BULK:
            tmp_hcintmsk.b.xfercompl   = 1u;
            tmp_hcintmsk.b.stall       = 1u;
            tmp_hcintmsk.b.xacterr     = 1u;
            tmp_hcintmsk.b.datatglerr  = 1u;
            tmp_hcintmsk.b.nak = 1u;
            if (pdev->host.hc[hc_num].is_epin)
            {
                tmp_hcintmsk.b.bblerr  = 1u;
            }
            else
            {
                tmp_hcintmsk.b.nyet    = 1u;
                if (pdev->host.hc[hc_num].do_ping)
                {
                    tmp_hcintmsk.b.ack = 1u;
                }
            }
            break;
        case EP_TYPE_INTR:
            tmp_hcintmsk.b.xfercompl   = 1u;
            tmp_hcintmsk.b.nak         = 1u;
            tmp_hcintmsk.b.stall       = 1u;
            tmp_hcintmsk.b.xacterr     = 1u;
            tmp_hcintmsk.b.datatglerr  = 1u;
            tmp_hcintmsk.b.frmovrun    = 1u;

            if (pdev->host.hc[hc_num].is_epin)
            {
                tmp_hcintmsk.b.bblerr  = 1u;
            }
            break;
        case EP_TYPE_ISOC:
            tmp_hcintmsk.b.xfercompl   = 1u;
            tmp_hcintmsk.b.frmovrun    = 1u;
            tmp_hcintmsk.b.ack         = 1u;

            if (pdev->host.hc[hc_num].is_epin)
            {
                tmp_hcintmsk.b.xacterr = 1u;
                tmp_hcintmsk.b.bblerr  = 1u;
            }
            break;
    }

    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCINTMSK, tmp_hcintmsk.d32);

    intr_enable = ((uint32_t)1 )<< hc_num;
    HD_USB_MDFREG32(&pdev->regs.HREGS->HAINTMSK, 0ul, intr_enable);

    /* enable the host channel interrupts */
    tmp_gintmsk.b.hcintr = 1ul;
    HD_USB_MDFREG32(&pdev->regs.GREGS->GINTMSK, 0ul, tmp_gintmsk.d32);

    /* modify HCCHAR */
    tmp_hcchar.d32 = 0ul;
    tmp_hcchar.b.devaddr = pdev->host.hc[hc_num].dev_addr;
    tmp_hcchar.b.epnum   = pdev->host.hc[hc_num].ep_idx;
    tmp_hcchar.b.epdir   = pdev->host.hc[hc_num].is_epin;
    tmp_hcchar.b.lspddev = (pdev->host.hc[hc_num].ch_speed == PRTSPD_LOW_SPEED);
    tmp_hcchar.b.eptype  = pdev->host.hc[hc_num].ep_type;
    tmp_hcchar.b.mps     = pdev->host.hc[hc_num].max_packet;
    if (pdev->host.hc[hc_num].ep_type == INT_TRANSFER)
    {
        tmp_hcchar.b.oddfrm  = 1u;
    }
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, tmp_hcchar.d32);
    return status;
}

/**
 *******************************************************************************
 ** \brief  Start transfer on the channel whose index is hc_num.
 ** \param  pdev : device instance
 ** \param  hc_num : channel index
 ** \retval status in 8 bits
 ******************************************************************************/
uint8_t hd_usb_hchtransbegin(usb_core_instance *pdev, uint8_t hc_num)
{
    __IO uint8_t status = HD_USB_OK;
    USB_CORE_HCCHAR_TypeDef   tmp_hcchar;
    USB_CORE_HCTSIZn_TypeDef  tmp_hctsiz;
    USB_CORE_HNPTXSTS_TypeDef tmp_hnptxsts;
    USB_CORE_HPTXSTS_TypeDef  tmp_hptxsts;
    USB_CORE_GINTMSK_TypeDef  tmp_gintmsk;
    uint16_t                 len_words = 0u;

    uint16_t num_packets;
    uint16_t max_hc_pkt_count;

    max_hc_pkt_count = 256u;
    tmp_hctsiz.d32 = 0ul;
    tmp_hcchar.d32 = 0ul;
    tmp_gintmsk.d32 = 0ul;

    /* Compute the expected number of packets associated to the transfer */
    if (pdev->host.hc[hc_num].xfer_len > 0u)
    {
        num_packets = (uint16_t)((pdev->host.hc[hc_num].xfer_len + \
        (uint32_t)pdev->host.hc[hc_num].max_packet - 1ul) / (uint32_t)pdev->host.hc[hc_num].max_packet);

        if (num_packets > max_hc_pkt_count)
        {
            num_packets                    = max_hc_pkt_count;
            pdev->host.hc[hc_num].xfer_len = (uint32_t)num_packets * \
            (uint32_t)pdev->host.hc[hc_num].max_packet;
        }
    }
    else
    {
        num_packets = 1u;
    }
    if (pdev->host.hc[hc_num].is_epin)
    {
        pdev->host.hc[hc_num].xfer_len = (uint32_t)num_packets * \
        (uint32_t)pdev->host.hc[hc_num].max_packet;
    }

    tmp_hctsiz.b.xfersize = pdev->host.hc[hc_num].xfer_len;
    tmp_hctsiz.b.pktcnt   = num_packets;
    tmp_hctsiz.b.pid = pdev->host.hc[hc_num].pid_type;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCTSIZ, tmp_hctsiz.d32);

    if (pdev->basic_cfgs.dmaen == 1u)
    {
        HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCDMA, (unsigned int)pdev->host.hc[hc_num].xfer_buff);
    }

    tmp_hcchar.d32 = HD_USB_RDREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
    tmp_hcchar.b.oddfrm = hd_usb_ifevenframe(pdev);

    /* enable this host channel whose number is hc_num */
    tmp_hcchar.b.chen  = 1u;
    tmp_hcchar.b.chdis = 0u;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, tmp_hcchar.d32);

    if (pdev->basic_cfgs.dmaen == 0u)
    {
        if((pdev->host.hc[hc_num].is_epin == 0u) && (pdev->host.hc[hc_num].xfer_len > 0u))
        {
            switch(pdev->host.hc[hc_num].ep_type)
            {
                /* Non-periodic transmit */
                case EP_TYPE_CTRL:
                case EP_TYPE_BULK:
                    tmp_hnptxsts.b = *(__IO stc_bUSB_CORE_HNPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.GREGS->GNPTXSTS);  /* C-STAT */
                    len_words = (uint16_t)((pdev->host.hc[hc_num].xfer_len + 3ul) / 4ul);
                    /* check if the amount of free space available in the non-periodic txFIFO is enough */
                    if(len_words > tmp_hnptxsts.b.nptxfspcavail)
                    {
                        /* enable interrrupt of nptxfempty of GINTMSK*/
                        tmp_gintmsk.b.nptxfempty = 1u;
                        HD_USB_MDFREG32( &pdev->regs.GREGS->GINTMSK, 0ul, tmp_gintmsk.d32);
                    }
                    break;
                /* Periodic trnsmit */
                case EP_TYPE_INTR:
                case EP_TYPE_ISOC:
                    tmp_hptxsts.b = *(__IO stc_bUSB_CORE_HPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.HREGS->HPTXSTS);   /* C-STAT */
                    len_words = (uint16_t)((pdev->host.hc[hc_num].xfer_len + 3ul) / 4ul);
                    /* check if the space of periodic TxFIFO is enough */
                    if(len_words > tmp_hptxsts.b.ptxfspcavail) /* split the transfer */
                    {
                        /* enable interrrupt of ptxfempty of GINTMSK */
                        tmp_gintmsk.b.ptxfempty = 1u;
                        HD_USB_MDFREG32( &pdev->regs.GREGS->GINTMSK, 0ul, tmp_gintmsk.d32);
                    }
                    break;
                default:
                    break;
            }

            hd_usb_wrpkt(pdev, pdev->host.hc[hc_num].xfer_buff , hc_num, (uint16_t)pdev->host.hc[hc_num].xfer_len);
        }
    }
    return status;
}

/**
 *******************************************************************************
 ** \brief  Stop the host and flush all the txFIFOs and the whole rxFIFO.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
void hd_usb_hoststop(usb_core_instance *pdev)
{
    __IO uint32_t   tmp_1 = 0;

    HD_USB_WRREG32(&pdev->regs.HREGS->HAINTMSK , 0ul);
    HD_USB_WRREG32(&pdev->regs.HREGS->HAINT, 0xFFFFFFFFul);

    do{
        hd_usb_chrst(pdev, tmp_1);
        tmp_1++;
    }while(tmp_1 < pdev->basic_cfgs.host_chnum);
    /* flush all the txFIFOs and the whole rxFIFO */
    hd_usb_rxfifoflush(pdev);
    hd_usb_txfifoflush(pdev, 0x10ul);
}

/**
 *******************************************************************************
 ** \brief  make the channel to halt
 ** \param  pdev : device instance
 ** \param  hc_num : channel index
 ** \retval none
 ******************************************************************************/
void hd_usb_hchstop(usb_core_instance *pdev, uint8_t hc_num)
{
    USB_CORE_HNPTXSTS_TypeDef            tmp_nptxsts;
    USB_CORE_HPTXSTS_TypeDef             tmp_hptxsts;
    USB_CORE_HCCHAR_TypeDef              tmp_hcchar;

    *(uint32_t*)&tmp_nptxsts.b = 0ul;
    *(uint32_t*)&tmp_hptxsts.b = 0ul;
    tmp_hcchar.b       = *(__IO stc_bUSB_CORE_HCCHAR_t*)&HD_USB_RDREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
    tmp_hcchar.b.chen  = 1u;
    tmp_hcchar.b.chdis = 1u;
    /* Check for space in the request queue to issue the halt. */
    if ((tmp_hcchar.b.eptype == CTRL_TRANSFER) || (tmp_hcchar.b.eptype == BULK_TRANSFER))
    {
        tmp_nptxsts.b = *(__IO stc_bUSB_CORE_HNPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.GREGS->GNPTXSTS);
        if (tmp_nptxsts.b.nptxqspcavail == 0u)
        {
            tmp_hcchar.b.chen = 0u;
        }
    }
    else
    {
        tmp_hptxsts.b = *(__IO stc_bUSB_CORE_HPTXSTS_t*)&HD_USB_RDREG32(&pdev->regs.HREGS->HPTXSTS);
        if (tmp_hptxsts.b.ptxqspcavail == 0u)
        {
            tmp_hcchar.b.chen = 0u;
        }
    }
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, *(uint32_t*)&tmp_hcchar.b);
}

#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
