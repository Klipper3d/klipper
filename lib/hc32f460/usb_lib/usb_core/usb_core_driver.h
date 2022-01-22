/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_core_driver.h
 **
 **
 ** @link Header of the Core Layer Driver @endlink
 **
 **   - 2021-03-29  Linsq First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_CORE_DRIVER_H__
#define __USB_CORE_DRIVER_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "usb_app_conf.h"
#include "usb_core_regs.h"
#include "usb_defines.h"


#if   defined ( __CC_ARM )
  #define __ASM            __asm
  #define __INLINE         __inline

#elif defined ( __GNUC__ )
  #define __ASM            __asm
  #define __INLINE         inline

#elif defined ( __ICCARM__ )
  #define __ASM            __asm
  #define __INLINE         inline
#endif

#ifdef USB_INTERNAL_DMA_ENABLED
    #if defined   (__GNUC__)        /* GNU Compiler */
        #define __USB_ALIGN_END    __attribute__ ((aligned (4)))
        #define __USB_ALIGN_BEGIN
    #else
        #define __USB_ALIGN_END
        #if defined   (__CC_ARM)      /* ARM Compiler */
            #define __USB_ALIGN_BEGIN    __align(4)
        #elif defined (__ICCARM__)    /* IAR Compiler */
            #define __USB_ALIGN_BEGIN
        #elif defined  (__TASKING__)  /* TASKING Compiler */
            #define __USB_ALIGN_BEGIN    __align(4)
        #endif /* __CC_ARM */
    #endif /* __GNUC__ */
#else
    #define __USB_ALIGN_BEGIN
    #define __USB_ALIGN_END
#endif

/*******************************************************************************
 * GLOBAL #define
 ******************************************************************************/
#define USB_EP0_IDLE                        (0u)
#define USB_EP0_SETUP                       (1u)
#define USB_EP0_DATA_IN                     (2u)
#define USB_EP0_DATA_OUT                    (3u)
#define USB_EP0_STATUS_IN                   (4u)
#define USB_EP0_STATUS_OUT                  (5u)
#define USB_EP0_STALL                       (6u)

#define USB_EP_TX_DIS                       (0x0000u)
#define USB_EP_TX_STALL                     (0x0010u)
#define USB_EP_TX_NAK                       (0x0020u)
#define USB_EP_TX_VALID                     (0x0030u)

#define USB_EP_RX_DIS                       (0x0000u)
#define USB_EP_RX_STALL                     (0x1000u)
#define USB_EP_RX_NAK                       (0x2000u)
#define USB_EP_RX_VALID                     (0x3000u)

#define MAX_DATA_LENGTH                     (0x200u)

/*******************************************************************************
 *       typedef
 ******************************************************************************/

#define HD_USB_OK      0u
#define HD_USB_ERROR   1u

//status of the host channel
typedef enum {
    HOST_CH_IDLE = 0u,
    HOST_CH_XFERCOMPL,
    HOST_CH_CHHLTD,
    HOST_CH_NAK,
    HOST_CH_NYET,
    HOST_CH_STALL,
    HOST_CH_XACTERR,
    HOST_CH_BBLERR,
    HOST_CH_DATATGLERR,
    HOST_CH_AHBERR,
    HOST_CH_FRMOVRUN,
    HOST_CH_BNAINTR,
    HOST_CH_XCS_XACT_ERR,
    HOST_CH_DESC_LST_ROLLINTR
}HOST_CH_STATUS;

typedef enum {
    HOST_CH_XFER_IDLE = 0u,
    HOST_CH_XFER_DONE,
    HOST_CH_XFER_UNREADY,
    HOST_CH_XFER_ERROR,
    HOST_CH_XFER_STALL
}HOST_CH_XFER_STATE;

typedef enum {
    CTRL_START = 0u,
    CTRL_XFRC,
    CTRL_HALTED,
    CTRL_NAK,
    CTRL_STALL,
    CTRL_XACTERR,
    CTRL_BBLERR,
    CTRL_DATATGLERR,
    CTRL_FAIL
}CTRL_HANDLE_STATUS;

typedef struct
{
    uint8_t       dev_addr;
    uint8_t       ep_idx;
    uint8_t       is_epin;
    uint8_t       ch_speed;
    uint8_t       do_ping;
    uint8_t       ep_type;
    uint16_t      max_packet;
    uint8_t       pid_type;
    uint8_t       in_toggle;
    uint8_t       out_toggle;
    /* transaction level variables*/
    uint32_t      dma_addr;
    uint32_t      xfer_len;
    uint32_t      xfer_count;
    uint8_t       *xfer_buff;
}USB_HOST_CH;

typedef struct
{
    uint8_t        epidx;
    uint8_t        ep_dir;
    uint8_t        trans_type;
    uint8_t        ep_stall;
    uint8_t        data_pid_start;
    uint8_t        datax_pid;
    uint16_t       tx_fifo_num;
    uint32_t       maxpacket;
    /* Transfer level variables */
    uint32_t       rem_data_len;
    uint32_t       total_data_len;
    uint32_t       ctl_data_len;
    /* transaction level variables*/
    uint32_t       dma_addr;
    uint32_t       xfer_len;
    uint32_t       xfer_count;
    uint8_t        *xfer_buff;
}USB_DEV_EP;

typedef struct
{
    uint8_t       host_chnum;
    uint8_t       dev_epnum;
    uint8_t       dmaen;
    uint8_t       low_power;
} USB_CORE_BASIC_CFGS;

typedef  struct
{
    uint8_t   bmRequest;
    uint8_t   bRequest;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
} USB_SETUP_REQ;

typedef struct
{
    uint8_t  *(*get_dev_desc)(uint16_t *length);
    uint8_t  *(*get_dev_langiddesc)(uint16_t *length);
    uint8_t  *(*get_dev_manufacturerstr)(uint16_t *length);
    uint8_t  *(*get_dev_productstr)(uint16_t *length);
    uint8_t  *(*get_dev_serialstr)(uint16_t *length);
    uint8_t  *(*get_dev_configstr)(uint16_t *length);
    uint8_t  *(*get_dev_interfacestr)(uint16_t *length);
} usb_dev_desc_func;

typedef struct
{
    void     (*class_init)                (void *pdev);
    void     (*class_deinit)              (void *pdev);
    uint8_t  (*ep0_setup)                 (void *pdev, USB_SETUP_REQ *req);
    void     (*ep0_datain)                (void *pdev);
    void     (*ep0_dataout)               (void *pdev);
    uint8_t  *(*class_getconfigdesc)      (uint16_t *length);
    uint8_t  (*class_sof)                 (void *pdev);
    void     (*class_datain)              (void *pdev, uint8_t epnum);
    void     (*class_dataout)             (void *pdev, uint8_t epnum);
    void     (*class_syn_in_incomplt)     (void *pdev);
    void     (*class_syn_out_incomplt)    (void *pdev);
} usb_dev_class_func;

typedef struct
{
    void (*user_init)(void);
    void (*user_devrst)(void);
    void (*user_devconfig)(void);
    void (*user_devsusp)(void);
    void (*user_devresume)(void);
    void (*user_devconn)(void);
    void (*user_devdisconn)(void);
}usb_dev_user_func;

typedef struct
{
    __IO uint8_t           device_config;
    __IO uint8_t           device_address;
    __IO uint8_t           device_state;
    __IO uint8_t           device_old_status;
    __IO uint8_t           device_cur_status;
    __IO uint8_t           connection_status;
    __IO uint8_t           device_remote_wakeup;
    __IO uint8_t           test_mode;
    USB_DEV_EP             in_ep[USB_MAX_TX_FIFOS];
    USB_DEV_EP             out_ep[USB_MAX_TX_FIFOS];
    uint8_t                setup_pkt_buf[24];
    usb_dev_class_func     *class_callback;
    usb_dev_user_func      *user_callback;
    usb_dev_desc_func      *desc_callback;
}USB_DEV_PARAM;

typedef struct
{
    uint16_t                 channel[USB_MAX_TX_FIFOS];
    USB_HOST_CH              hc[USB_MAX_TX_FIFOS];
    __IO uint32_t            is_dev_connect;
    uint8_t                  Rx_Buffer[MAX_DATA_LENGTH];
    __IO uint32_t            ErrCnt[USB_MAX_TX_FIFOS];
    __IO uint32_t            XferCnt[USB_MAX_TX_FIFOS];
    __IO HOST_CH_STATUS      HC_Status[USB_MAX_TX_FIFOS];
    __IO HOST_CH_XFER_STATE  URB_State[USB_MAX_TX_FIFOS];
}USB_HOST_PARAM;

typedef struct
{
    USB_CORE_BASIC_CFGS    basic_cfgs;
    HD_USB_REGS            regs;
#ifdef USE_DEVICE_MODE
    USB_DEV_PARAM          dev;
#endif
#ifdef USE_HOST_MODE
    USB_HOST_PARAM         host;
#endif
}usb_core_instance;

/*******************************************************************************
 *            inline functions
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  get the current mode of the usb core from the corresponding register
 ** \param  pdev : device instance
 ** \retval current mode \@=1: host mode \@=0: device mode
 ******************************************************************************/
static __INLINE uint8_t hd_usb_getcurmod(usb_core_instance *pdev)
{
    uint8_t tmp;
    USB_CORE_GINTSTS_TypeDef tmp_gintsts;
    tmp_gintsts.d32 = HD_USB_RDREG32(&pdev->regs.GREGS->GINTSTS);
    tmp             = tmp_gintsts.b.curmode;
    return tmp;
}

/**
 *******************************************************************************
 ** \brief  Initializes the normal interrupts
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_normalinten(usb_core_instance *pdev)
{
    USB_CORE_GINTMSK_TypeDef  tmp_gintmsk;
    tmp_gintmsk.d32          = 0ul;
    HD_USB_WRREG32(&pdev->regs.GREGS->GOTGINT, 0xFFFFFFFFul);
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTSTS, 0xBFFFFFFFul);
    tmp_gintmsk.b.wkupintr   = 1u;
    tmp_gintmsk.b.usbsuspend = 1u;
    HD_USB_WRREG32(&pdev->regs.GREGS->GINTMSK, tmp_gintmsk.d32);
}

/**
 *******************************************************************************
 ** \brief  clear all the pending device interrupt bits and mask the IN and OUT
 **         endpoint interrupts.
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_clrandmskepint(usb_core_instance *pdev)
{
    HD_USB_WRREG32(&pdev->regs.DREGS->DIEPMSK, 0ul );
    HD_USB_WRREG32(&pdev->regs.DREGS->DOEPMSK, 0ul );
    HD_USB_WRREG32(&pdev->regs.DREGS->DAINT, 0xFFFFFFFFul );
    HD_USB_WRREG32(&pdev->regs.DREGS->DAINTMSK, 0ul );
}

/**
 *******************************************************************************
 ** \brief  generate a device connect signal to the USB host
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_coreconn(usb_core_instance *pdev)
{
    USB_CORE_DCTL_TypeDef tmp_dctl;
    tmp_dctl.d32         = HD_USB_RDREG32(&pdev->regs.DREGS->DCTL);
    tmp_dctl.b.sftdiscon = 0u;
    HD_USB_WRREG32(&pdev->regs.DREGS->DCTL, tmp_dctl.d32);
}

/**
 *******************************************************************************
 ** \brief  test of mode processing
 **
 ** \param  pdev: device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_runtestmode(usb_core_instance *pdev, uint32_t reg)
{
    HD_USB_WRREG32(&pdev->regs.DREGS->DCTL, reg);
}

/**
 *******************************************************************************
 ** \brief  Enables the controller's Global interrupts in the AHB Configuration
 **         registers.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_ginten(usb_core_instance *pdev)
{
    USB_CORE_GAHBCFG_TypeDef  tmp_gahbcfg;

    tmp_gahbcfg.d32           = 0ul;
    tmp_gahbcfg.b.glblintrmsk = 1u;
    HD_USB_MDFREG32(&pdev->regs.GREGS->GAHBCFG, 0ul, tmp_gahbcfg.d32);
}

/**
 *******************************************************************************
 ** \brief  Disable the controller's Global interrupt in the AHB Configuration
 **         register.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_gintdis(usb_core_instance *pdev)
{
    USB_CORE_GAHBCFG_TypeDef  tmp_gahbcfg;
    tmp_gahbcfg.d32           = 0ul;
    tmp_gahbcfg.b.glblintrmsk = 1u;
    HD_USB_MDFREG32(&pdev->regs.GREGS->GAHBCFG, tmp_gahbcfg.d32, 0ul);
}

/**
 *******************************************************************************
 ** \brief  Get the Core Interrupt bits from the interrupt register not including
 **         the bits that are masked.
 ** \param  pdev : device instance
 ** \retval status[32bits]
 ******************************************************************************/
static __INLINE uint32_t hd_usb_getcoreintr(usb_core_instance *pdev)
{
    uint32_t v = 0ul;
    v  = HD_USB_RDREG32(&pdev->regs.GREGS->GINTSTS);
    v &= HD_USB_RDREG32(&pdev->regs.GREGS->GINTMSK);
    return v;
}

/**
 *******************************************************************************
 ** \brief  Get the out endpoint interrupt bits from the all endpoint interrupt
 **         register not including the bits masked.
 ** \param  pdev : device instance
 ** \retval The status that shows which OUT EP have interrupted.
 ******************************************************************************/
static __INLINE uint32_t hd_usb_getalloepintr(usb_core_instance *pdev)
{
    uint32_t v;
    v  = HD_USB_RDREG32(&pdev->regs.DREGS->DAINT);
    v &= HD_USB_RDREG32(&pdev->regs.DREGS->DAINTMSK);
    return ((v & 0xffff0000ul) >> 16u);
}

/**
 *******************************************************************************
 ** \brief  Get the Device OUT EP Interrupt register(DOEPINT) not including the
 **         interrupt bits that are masked.
 ** \param  pdev : device instance
 ** \param  epnum: end point index
 ** \retval all the interrupt bits on DOEPINTn while n = epnum
 ******************************************************************************/
static __INLINE uint32_t hd_usb_getoepintbit(usb_core_instance *pdev , uint8_t epnum)
{
    uint32_t v;
    v  = HD_USB_RDREG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT);
    v &= HD_USB_RDREG32(&pdev->regs.DREGS->DOEPMSK);
    return v;
}

/**
 *******************************************************************************
 ** \brief  Get the IN endpoint interrupt bits from the all endpoint interrupt
 **         register not including the bits masked.
 ** \param  pdev : device instance
 ** \retval The status that shows which IN EP have interrupted.
 ******************************************************************************/
static __INLINE uint32_t hd_usb_getalliepintr(usb_core_instance *pdev)
{
    uint32_t v;
    v  = HD_USB_RDREG32(&pdev->regs.DREGS->DAINT);
    v &= HD_USB_RDREG32(&pdev->regs.DREGS->DAINTMSK);
    return (v & 0xfffful);
}

/**
 *******************************************************************************
 ** \brief  Set the device a new address.
 ** \param  pdev: device instance
 ** \param  address: device address which will be set to the corresponding register.
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_devaddrset(usb_core_instance *pdev, uint8_t address)
{
    USB_CORE_DCFG_TypeDef  dcfg;
    dcfg.d32       = 0ul;
    dcfg.b.devaddr = address;
    HD_USB_MDFREG32(&pdev->regs.DREGS->DCFG, 0ul, dcfg.d32);
}

#ifdef USE_HOST_MODE

/**
 *******************************************************************************
 ** \brief  Read the register HPRT and reset the following bits.
 ** \param  pdev : device instance
 ** \retval value of HPRT
 ******************************************************************************/
static __INLINE uint32_t hd_usb_rdhprt(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef  tmp_hprt;

    tmp_hprt.d32              = HD_USB_RDREG32(pdev->regs.HPRT0);
    tmp_hprt.b.prtena         = 0u;
    tmp_hprt.b.prtconndet     = 0u;
    tmp_hprt.b.prtenchng      = 0u;
    tmp_hprt.b.prtovrcurrchng = 0u;
    return tmp_hprt.d32;
}

/**
 *******************************************************************************
 ** \brief  Issues a ping token
 ** \param  pdev : device instance
 ** \param  hc_num: the host channel index
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_pingtokenissue(usb_core_instance *pdev , uint8_t hc_num)
{
    USB_CORE_HCCHAR_TypeDef    tmp_hcchar;
    USB_CORE_HCTSIZn_TypeDef   tmp_hctsiz;

    tmp_hctsiz.d32      = 0ul;
    tmp_hctsiz.b.dopng  = 1u;
    tmp_hctsiz.b.pktcnt = 1u;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCTSIZ, tmp_hctsiz.d32);

    tmp_hcchar.d32      = HD_USB_RDREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR);
    tmp_hcchar.b.chen   = 1u;
    tmp_hcchar.b.chdis  = 0u;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[hc_num]->HCCHAR, tmp_hcchar.d32);
}

/**
 ** \brief  This function returns the frame number for sof packet
 ** \param  pdev : device instance
 ** \retval Frame number
 ******************************************************************************/
static __INLINE uint8_t hd_usb_ifevenframe (usb_core_instance *pdev)
{
    return !(HD_USB_RDREG32(&pdev->regs.HREGS->HFNUM) & 0x1u);
}

/**
 *******************************************************************************
 ** \brief  Initializes the FSLSPClkSel field of the HCFG register on the PHY type
 ** \param  pdev : device instance
 ** \param  freq : clock frequency
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_fslspclkselset(usb_core_instance *pdev , uint8_t freq)
{
    USB_CORE_HCFG_TypeDef   tmp_hcfg;

    tmp_hcfg.d32           = HD_USB_RDREG32(&pdev->regs.HREGS->HCFG);
    tmp_hcfg.b.fslspclksel = freq;
    HD_USB_WRREG32(&pdev->regs.HREGS->HCFG, tmp_hcfg.d32);
}

/**
 *******************************************************************************
 ** \brief  suspend the port
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_prtsusp(usb_core_instance *pdev)
{
    USB_CORE_HPRT_TypeDef     port0;

    port0.d32       = hd_usb_rdhprt(pdev);
    port0.b.prtsusp = 1u;
    port0.b.prtres  = 0u;
    HD_USB_WRREG32(pdev->regs.HPRT0, port0.d32);
}

/**
 *******************************************************************************
 ** \brief  control the enumeration speed of the core, this function make sure that
 **         the maximum speed supperted by the connected device.
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_enumspeed(usb_core_instance *pdev)
{
    USB_CORE_HCFG_TypeDef  tmp_hcfg;
    tmp_hcfg.d32        = HD_USB_RDREG32(&pdev->regs.HREGS->HCFG);
    tmp_hcfg.b.fslssupp = 0u;
    HD_USB_WRREG32(&pdev->regs.HREGS->HCFG, tmp_hcfg.d32);
}

/**
 *******************************************************************************
 ** \brief  set the TXFIFO and depth for non-periodic and periodic and RXFIFO size
 ** \param  pdev : device instance
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_sethostfifo(usb_core_instance *pdev)
{
    USB_CORE_FSIZ_TypeDef            nptxfifosize;
    USB_CORE_FSIZ_TypeDef            ptxfifosize;
    nptxfifosize.d32 = 0ul;
    ptxfifosize.d32  = 0ul;

    HD_USB_WRREG32(&pdev->regs.GREGS->GRXFSIZ, (uint32_t)RX_FIFO_FS_SIZE);   //set the RxFIFO Depth
    nptxfifosize.b.startaddr = RX_FIFO_FS_SIZE;                              //non-periodic transmit RAM start address
    nptxfifosize.b.depth     = TXH_NP_FS_FIFOSIZ;                                //set the non-periodic TxFIFO depth
    HD_USB_WRREG32(&pdev->regs.GREGS->GNPTXFSIZ, nptxfifosize.d32);          //set the register-GNPTXFSIZ
    ptxfifosize.b.startaddr  = RX_FIFO_FS_SIZE + TXH_NP_FS_FIFOSIZ;           //set the host periodic TxFIFO start address
    ptxfifosize.b.depth      = TXH_P_FS_FIFOSIZ;                              //set the host periodic TxFIFO depth
    HD_USB_WRREG32(&pdev->regs.GREGS->HPTXFSIZ, ptxfifosize.d32);
}

/**
 *******************************************************************************
 ** \brief  reset the channel whose channel number is ch_idx
 ** \param  pdev : device instance
 ** \param  ch_idx: channel number
 ** \retval none
 ******************************************************************************/
static __INLINE void hd_usb_chrst(usb_core_instance *pdev, uint8_t ch_idx)
{
    USB_CORE_HCCHAR_TypeDef  hcchar;
    hcchar.d32     = HD_USB_RDREG32(&pdev->regs.HC_REGS[ch_idx]->HCCHAR);
    hcchar.b.chen  = 0u;
    hcchar.b.chdis = 1u;
    hcchar.b.epdir = 0u;
    HD_USB_WRREG32(&pdev->regs.HC_REGS[ch_idx]->HCCHAR, hcchar.d32);
}
#endif //end of USE_HOST_MODE

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern void hd_usb_initusbcore(usb_core_instance *pdev);
extern void hd_usb_setregaddr(usb_core_instance *pdev);
extern void hd_usb_rdpkt(usb_core_instance *pdev, uint8_t *dest, uint16_t len);
extern void hd_usb_wrpkt(usb_core_instance *pdev, uint8_t *src, uint8_t ch_ep_num, uint16_t len);
extern void hd_usb_txfifoflush(usb_core_instance *pdev , uint32_t num);
extern void hd_usb_rxfifoflush(usb_core_instance *pdev);
extern void hd_usb_modeset(usb_core_instance *pdev, uint8_t mode);
extern void hd_usb_ctrldevconnect(usb_core_instance *pdev, uint8_t link);

#ifdef USE_HOST_MODE
extern void hd_usb_hostmodeinit(usb_core_instance *pdev);
extern void hd_usb_hostinten(usb_core_instance *pdev);
extern uint8_t hd_usb_inithch(usb_core_instance *pdev, uint8_t hc_num);
extern void hd_usb_hchstop(usb_core_instance *pdev, uint8_t hc_num);
extern uint8_t hd_usb_hchtransbegin(usb_core_instance *pdev, uint8_t hc_num);
extern void hd_usb_hprtrst(usb_core_instance *pdev);
extern void hd_usb_vbusctrl(usb_core_instance *pdev, uint8_t state);
#endif

#ifdef USE_DEVICE_MODE
extern void hd_usb_devmodeinit(usb_core_instance *pdev);
extern void hd_usb_devinten(usb_core_instance *pdev);
extern void hd_usb_ep0activate(usb_core_instance *pdev);
extern void hd_usb_epactive(usb_core_instance *pdev, USB_DEV_EP *ep);
extern void hd_usb_epdeactive(usb_core_instance *pdev, USB_DEV_EP *ep);
extern void hd_usb_epntransbegin(usb_core_instance *pdev, USB_DEV_EP *ep);
extern void hd_usb_ep0transbegin(usb_core_instance *pdev, USB_DEV_EP *ep);
extern void hd_usb_setepstall(usb_core_instance *pdev, USB_DEV_EP *ep);
extern void hd_usb_clearepstall(usb_core_instance *pdev, USB_DEV_EP *ep);
extern uint8_t USBH_IsEvenFrame(usb_core_instance *pdev);
extern void hd_usb_ep0revcfg(usb_core_instance *pdev);
extern void hd_usb_remotewakeupen(usb_core_instance *pdev);
extern void hd_usb_epstatusset(usb_core_instance *pdev, USB_DEV_EP *ep, uint32_t Status);
extern uint32_t hd_usb_epstatusget(usb_core_instance *pdev, USB_DEV_EP *ep);
#endif

#endif  // __USB_CORE_DRIVER_H__

/*******************************************************************************
 * EOF
 ******************************************************************************/
