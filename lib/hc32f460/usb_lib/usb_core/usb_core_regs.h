/*******************************************************************************
 * Copyright (C) 2020, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by HDSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 */
/******************************************************************************/
/** \file usb_core_regs.h
 **
 ** A detailed description is available at
 ** @link hardware registers @endlink
 **
 **   - 2018-12-26  CDT First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USB_CORE_REGS_H__
#define __USB_CORE_REGS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdint.h>
#include "usb_app_conf.h"

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#ifndef __IO
    #define     __IO    volatile
#endif

#define USB_MAX_TX_FIFOS                 (12u)

#define USB_HS_MAX_PACKET_SIZE           (512u)
#define USB_FS_MAX_PACKET_SIZE           (64u)
#define USB_MAX_EP0_SIZE                 (64u)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

typedef struct   
{
    __IO uint32_t GOTGCTL;                    /* Control and Status Register                      000h*/
    __IO uint32_t GOTGINT;                    /* Interrupt Register                               004h*/
    __IO uint32_t GAHBCFG;                    /* AHB Configuration Register                       008h*/
    __IO uint32_t GUSBCFG;                    /* USB Configuration Register                       00Ch*/
    __IO uint32_t GRSTCTL;                    /* Reset Register                                   010h*/
    __IO uint32_t GINTSTS;                    /* Interrupt Register                               014h*/
    __IO uint32_t GINTMSK;                    /* Interrupt Mask Register                          018h*/
    __IO uint32_t GRXSTSR;                    /* Receive Sts Q Read Register                      01Ch*/
    __IO uint32_t GRXSTSP;                    /* Receive Sts Q Read & POP Register                020h*/
    __IO uint32_t GRXFSIZ;                    /* Receive FIFO Size Register                       024h*/
    __IO uint32_t GNPTXFSIZ;                  /* Non-Periodic Transmit FIFO Size Register         028h*/
    __IO uint32_t GNPTXSTS;                   /* Non Periodic Transmit FIFO/Queue Status Register 02Ch*/
    __IO uint32_t GI2CCTL;                    /* I2C Access Register                              030h*/
    __IO uint32_t GPVNDCTL;                   /* PHY Vendor Control Register                      034h*/
    __IO uint32_t GGPIO;                      /* General Purpose Input/Output Register            038h*/
    __IO uint32_t GUID;                       /* User ID Register                                 03Ch*/
    uint32_t  Reserved40[48];                 /* Reserved                                    040h-0FFh*/
    __IO uint32_t HPTXFSIZ;                   /* Host Periodic Transmit FIFO Size Register        100h*/
    __IO uint32_t DPTXFSIZ[USB_MAX_TX_FIFOS]; /* Device Periodic Transmit FIFO Size Register          */
}USB_CORE_GREGS;

typedef struct 
{
    volatile uint32_t DCFG;         /* Device Configuration Register                     800h*/
    volatile uint32_t DCTL;         /* Device Control Register                           804h*/
    volatile uint32_t DSTS;         /* Device Status Register (RO)                       808h*/
    uint32_t Reserved0C;            /* Reserved                                          80Ch*/
    volatile uint32_t DIEPMSK;      /* Device IN EP Common Interrupt Mask Register       810h*/
    volatile uint32_t DOEPMSK;      /* Device OUT EP Common Interrupt Mask Register      814h*/
    volatile uint32_t DAINT;        /* Device All EP Interrupt Register                  818h*/
    volatile uint32_t DAINTMSK;     /* Device All EP Interrupt Mask Register             81Ch*/
    uint32_t  Reserved20;           /* Reserved                                          820h*/
    uint32_t Reserved9;             /* Reserved                                          824h*/
    volatile uint32_t DVBUSDIS;     /* Device VBUS Discharge Time Register               828h*/
    volatile uint32_t DVBUSPULSE;   /* Device VBUS Pulsing Time Register                 82Ch*/
    volatile uint32_t DTHRCTL;      /* Device Threshold Control Register                 830h*/
    volatile uint32_t DIEPEMPMSK;   /* Device IN EP FIFO Empty Interrupt Mask Register   834h*/
    volatile uint32_t DEACHINT;     /* Deivce Each EP Interrupt Register                 838h*/
    volatile uint32_t DEACHINTMSK;  /* Device Each EP Interrupt Mask Register            83Ch*/
}USB_CORE_DREGS;

typedef struct 
{
    volatile uint32_t DIEPCTL; /* dev IN Endpoint Control Reg 900h + (ep_num * 20h) + 00h*/
    uint32_t Reserved04;             /* Reserved                       900h + (ep_num * 20h) + 04h*/
    volatile uint32_t DIEPINT; /* dev IN Endpoint Itr Reg     900h + (ep_num * 20h) + 08h*/
    uint32_t Reserved0C;             /* Reserved                       900h + (ep_num * 20h) + 0Ch*/
    volatile uint32_t DIEPTSIZ; /* IN Endpoint Txfer Size   900h + (ep_num * 20h) + 10h*/
    volatile uint32_t DIEPDMA; /* IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h*/
    volatile uint32_t DTXFSTS;/*IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h*/
    uint32_t Reserved18;             /* Reserved  900h+(ep_num*20h)+1Ch-900h+ (ep_num * 20h) + 1Ch*/
}USB_CORE_INEPREGS;

typedef struct 
{
    volatile uint32_t DOEPCTL;       /* dev OUT Endpoint Control Reg  B00h + (ep_num * 20h) + 00h*/
    uint32_t Reserved04;         /* Reserved                      B00h + (ep_num * 20h) + 04h*/
    volatile uint32_t DOEPINT;       /* dev OUT Endpoint Itr Reg      B00h + (ep_num * 20h) + 08h*/
    uint32_t Reserved0C;         /* Reserved                      B00h + (ep_num * 20h) + 0Ch*/
    volatile uint32_t DOEPTSIZ;      /* dev OUT Endpoint Txfer Size   B00h + (ep_num * 20h) + 10h*/
    volatile uint32_t DOEPDMA;       /* dev OUT Endpoint DMA Address  B00h + (ep_num * 20h) + 14h*/
    uint32_t Reserved18[2];      /* Reserved B00h + (ep_num * 20h) + 18h - B00h + (ep_num * 20h) + 1Ch*/
}USB_CORE_OUTEPREGS;

typedef struct 
{
    volatile uint32_t HCFG;             /* Host Configuration Register    400h*/
    volatile uint32_t HFIR;      /* Host Frame Interval Register   404h*/
    volatile uint32_t HFNUM;         /* Host Frame Nbr/Frame Remaining 408h*/
    uint32_t Reserved40C;                   /* Reserved                       40Ch*/
    volatile uint32_t HPTXSTS;   /* Host Periodic Tx FIFO/ Queue Status 410h*/
    volatile uint32_t HAINT;   /* Host All Channels Interrupt Register 414h*/
    volatile uint32_t HAINTMSK;   /* Host All Channels Interrupt Mask 418h*/
}USB_CORE_HREGS;

typedef struct 
{
    volatile uint32_t HCCHAR;
    volatile uint32_t HCSPLT;
    volatile uint32_t HCINT;
    volatile uint32_t HCINTMSK;
    volatile uint32_t HCTSIZ;
    volatile uint32_t HCDMA;
    uint32_t Reserved[2];
}USB_CORE_HC_REGS;

typedef struct //000h
{
    USB_CORE_GREGS         *GREGS;
    USB_CORE_DREGS         *DREGS;
    USB_CORE_HREGS         *HREGS;
    USB_CORE_INEPREGS      *INEP_REGS[USB_MAX_TX_FIFOS];
    USB_CORE_OUTEPREGS     *OUTEP_REGS[USB_MAX_TX_FIFOS];
    USB_CORE_HC_REGS       *HC_REGS[USB_MAX_TX_FIFOS];
    volatile uint32_t     *HPRT0;
    volatile uint32_t     *DFIFO[USB_MAX_TX_FIFOS];
    volatile uint32_t     *PCGCCTL;
}HD_USB_REGS;

typedef union 
{
    uint32_t d32;
    struct
    {
        unsigned sesreqscs:1;
        unsigned sesreq:1;
        unsigned vbvalidoven:1;
        unsigned vbvalidovval:1;
        unsigned avalidoven:1;
        unsigned avalidovval:1;
        unsigned bvalidoven:1;
        unsigned bvalidovval:1;
        unsigned hstnegscs:1;
        unsigned hnpreq:1;
        unsigned hstsethnpen:1;
        unsigned devhnpen:1;
        unsigned reserved12_15:4;
        unsigned conidsts:1;
        unsigned dbnctime:1;
        unsigned asesvld:1;
        unsigned bsesvld:1;
        unsigned otgver:1;
        unsigned reserved1:1;
        unsigned multvalidbc:5;
        unsigned chirpen:1;
        unsigned reserved28_31:4;
    }
    b;
} USB_CORE_GOTGCTL_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        /** Current Mode */
        unsigned reserved0_1:2;

        /** Session End Detected */
        unsigned sesenddet:1;

        unsigned reserved3_7:5;

        /** Session Request Success Status Change */
        unsigned sesreqsucstschng:1;
        /** Host Negotiation Success Status Change */
        unsigned hstnegsucstschng:1;

        unsigned reserved10_16:7;

        /** Host Negotiation Detected */
        unsigned hstnegdet:1;
        /** A-Device Timeout Change */
        unsigned adevtoutchng:1;
        /** Debounce Done */
        unsigned debdone:1;
        /** Multi-Valued input changed */
        unsigned mvic:1;

        unsigned reserved31_21:11;
    }
    b;
} USB_CORE_GOTGINT_TypeDef ;
typedef union 
{
    uint32_t d32;
    struct
    {
        unsigned glblintrmsk:1;
#define DWC_GAHBCFG_GLBINT_ENABLE           (1u)

        unsigned hburstlen:4;
#define DWC_GAHBCFG_INT_DMA_BURST_SINGLE    (0u)
#define DWC_GAHBCFG_INT_DMA_BURST_INCR      (1u)
#define DWC_GAHBCFG_INT_DMA_BURST_INCR4     (3u)
#define DWC_GAHBCFG_INT_DMA_BURST_INCR8     (5u)
#define DWC_GAHBCFG_INT_DMA_BURST_INCR16    (7u)

        unsigned dmaenable:1;
#define DWC_GAHBCFG_DMAENABLE               (1u)
        unsigned reserved:1;
        unsigned nptxfemplvl_txfemplvl:1;
        unsigned ptxfemplvl:1;
#define DWC_GAHBCFG_TXFEMPTYLVL_EMPTY       (1u)
#define DWC_GAHBCFG_TXFEMPTYLVL_HALFEMPTY   (0u)
        unsigned reserved9_20:12;
        unsigned remmemsupp:1;
        unsigned notialldmawrit:1;
        unsigned ahbsingle:1;
        unsigned reserved24_31:8;
  }
  b;
} USB_CORE_GAHBCFG_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        unsigned toutcal:3;
        unsigned phyif:1;
        unsigned ulpi_utmi_sel:1;
        unsigned fsintf:1;
        unsigned physel:1;
        unsigned ddrsel:1;
        unsigned srpcap:1;
        unsigned hnpcap:1;
        unsigned usbtrdtim:4;
        unsigned reserved1:1;
        unsigned phylpwrclksel:1;
        unsigned otgutmifssel:1;
        unsigned ulpi_fsls:1;
        unsigned ulpi_auto_res:1;
        unsigned ulpi_clk_sus_m:1;
        unsigned ulpi_ext_vbus_drv:1;
        unsigned ulpi_int_vbus_indicator:1;
        unsigned term_sel_dl_pulse:1;
        unsigned indicator_complement:1;
        unsigned indicator_pass_through:1;
        unsigned ulpi_int_prot_dis:1;
        unsigned ic_usb_cap:1;
        unsigned ic_traffic_pull_remove:1;
        unsigned tx_end_delay:1;
        unsigned force_host_mode:1;
        unsigned force_dev_mode:1;
        unsigned reserved31:1;
    }
    b;
} USB_CORE_GUSBCFG_TypeDef ;

typedef struct 
{
    /** Core Soft Reset (CSftRst) (Device and Host)
    *
    * The application can flush the control logic in the
    * entire core using this bit. This bit resets the
    * pipelines in the AHB Clock domain as well as the
    * PHY Clock domain.
    *
    * The state machines are reset to an IDLE state, the
    * control bits in the CSRs are cleared, all the
    * transmit FIFOs and the receive FIFO are flushed.
    *
    * The status mask bits that control the generation of
    * the interrupt, are cleared, to clear the
    * interrupt. The interrupt status bits are not
    * cleared, so the application can get the status of
    * any events that occurred in the core after it has
    * set this bit.
    *
    * Any transactions on the AHB are terminated as soon
    * as possible following the protocol. Any
    * transactions on the USB are terminated immediately.
    *
    * The configuration settings in the CSRs are
    * unchanged, so the software doesn't have to
    * reprogram these registers (Device
    * Configuration/Host Configuration/Core System
    * Configuration/Core PHY Configuration).
    *
    * The application can write to this bit, any time it
    * wants to reset the core. This is a self clearing
    * bit and the core clears this bit after all the
    * necessary logic is reset in the core, which may
    * take several clocks, depending on the current state
    * of the core.
    */
    unsigned csftrst:1;
    /** Hclk Soft Reset
    *
    * The application uses this bit to reset the control logic in
    * the AHB clock domain. Only AHB clock domain pipelines are
    * reset.
    */
    unsigned hsftrst:1;
    /** Host Frame Counter Reset (Host Only)<br>
    *
    * The application can reset the (micro)frame number
    * counter inside the core, using this bit. When the
    * (micro)frame counter is reset, the subsequent SOF
    * sent out by the core, will have a (micro)frame
    * number of 0.
    */
    unsigned hstfrm:1;
    /** In Token Sequence Learning Queue Flush
    * (INTknQFlsh) (Device Only)
    */
    unsigned intknqflsh:1;
    /** RxFIFO Flush (RxFFlsh) (Device and Host)
    *
    * The application can flush the entire Receive FIFO
    * using this bit. The application must first
    * ensure that the core is not in the middle of a
    * transaction. The application should write into
    * this bit, only after making sure that neither the
    * DMA engine is reading from the RxFIFO nor the MAC
    * is writing the data in to the FIFO. The
    * application should wait until the bit is cleared
    * before performing any other operations. This bit
    * will takes 8 clocks (slowest of PHY or AHB clock)
    * to clear.
    */
    unsigned rxfflsh:1;
    /** TxFIFO Flush (TxFFlsh) (Device and Host).
    *
    * This bit is used to selectively flush a single or
    * all transmit FIFOs. The application must first
    * ensure that the core is not in the middle of a
    * transaction. The application should write into
    * this bit, only after making sure that neither the
    * DMA engine is writing into the TxFIFO nor the MAC
    * is reading the data out of the FIFO. The
    * application should wait until the core clears this
    * bit, before performing any operations. This bit
    * will takes 8 clocks (slowest of PHY or AHB clock)
    * to clear.
    */
    unsigned txfflsh:1;

    /** TxFIFO Number (TxFNum) (Device and Host).
    *
    * This is the FIFO number which needs to be flushed,
    * using the TxFIFO Flush bit. This field should not
    * be changed until the TxFIFO Flush bit is cleared by
    * the core.
    *    - 0x0 : Non Periodic TxFIFO Flush
    *    - 0x1 : Periodic TxFIFO #1 Flush in device mode
    *      or Periodic TxFIFO in host mode
    *    - 0x2 : Periodic TxFIFO #2 Flush in device mode.
    *    - ...
    *    - 0xF : Periodic TxFIFO #15 Flush in device mode
    *    - 0x10: Flush all the Transmit NonPeriodic and
    *      Transmit Periodic FIFOs in the core
    */
    unsigned txfnum:5;
    /** Reserved */
    unsigned reserved11_29:19;
    /** DMA Request Signal.  Indicated DMA request is in
    * probress. Used for debug purpose. */
    unsigned dmareq:1;
    /** AHB Master Idle.  Indicates the AHB Master State
    * Machine is in IDLE condition. */
    unsigned ahbidle:1;
}USB_CORE_GRSTCTL_TypeDef; 
typedef union 
{
    uint32_t d32;
    USB_CORE_GRSTCTL_TypeDef b; 
} USB_GRSTCTL_TypeDef;

typedef union 
{
    uint32_t d32;
    struct
    {
        unsigned reserved0:1;
        unsigned modemismatch:1;
        unsigned reserved:1;
        unsigned sofintr:1;
        unsigned rxstsqlvl:1;
        unsigned nptxfempty:1;
        unsigned ginnakeff:1;
        unsigned goutnakeff:1;
        unsigned ulpickint:1;
        unsigned i2cintr:1;
        unsigned erlysuspend:1;
        unsigned usbsuspend:1;
        unsigned usbreset:1;
        unsigned enumdone:1;
        unsigned isooutdrop:1;
        unsigned eopframe:1;
        unsigned restoredone:1;
        unsigned epmismatch:1;
        unsigned inepintr:1;
        unsigned outepintr:1;
        unsigned incomplisoin:1;
        unsigned incomplisoout:1;
        unsigned fetsusp:1;
        unsigned resetdet:1;
        unsigned portintr:1;
        unsigned hcintr:1;
        unsigned ptxfempty:1;
        unsigned lpmtranrcvd:1;
        unsigned conidstschng:1;
        unsigned disconnect:1;
        unsigned vbusvint:1;
        unsigned wkupintr:1;
    }
    b;
} USB_CORE_GINTMSK_TypeDef;

typedef struct 
{
    unsigned curmode:1;
    unsigned modemismatch:1;
    unsigned reserved:1;
    unsigned sofintr:1;
    unsigned rxstsqlvl:1;
    unsigned nptxfempty:1;
    unsigned ginnakeff:1;
    unsigned goutnakeff:1;
    unsigned ulpickint:1;
    unsigned i2cintr:1;
    unsigned erlysuspend:1;
    unsigned usbsuspend:1;
    unsigned usbreset:1;
    unsigned enumdone:1;
    unsigned isooutdrop:1;
    unsigned eopframe:1;
    unsigned restoredone:1;
    unsigned epmismatch:1;
    unsigned inepint:1;
    unsigned outepintr:1;
    unsigned incomplisoin:1;
    unsigned incomplisoout:1;
    unsigned fetsusp:1;
    unsigned resetdet:1;
    unsigned portintr:1;
    unsigned hcintr:1;
    unsigned ptxfempty:1;
    unsigned lpmtranrcvd:1;
    unsigned conidstschng:1;
    unsigned disconnect:1;
    unsigned vbusvint:1;
    unsigned wkupintr:1;
}stc_bUSB_CORE_GINTSTS_t;   
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_GINTSTS_t b;  
} USB_CORE_GINTSTS_TypeDef ;

typedef struct 
{
    unsigned epnum:4;
    unsigned bcnt:11;
    unsigned dpid:2;

#define DWC_STS_DATA_UPDT       (0x2) // OUT Data Packet
#define DWC_STS_XFER_COMP       (0x3) // OUT Data Transfer Complete

#define DWC_DSTS_GOUT_NAK       (0x1) // Global OUT NAK
#define DWC_DSTS_SETUP_COMP     (0x4) // Setup Phase Complete
#define DWC_DSTS_SETUP_UPDT     (0x6) // SETUP Packet
    unsigned pktsts:4;
    unsigned fn:4;
    unsigned reserved25_31:7;
}stc_bUSB_CORE_DRXSTS_t; /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_DRXSTS_t b;  /* C-STAT */
} USB_CORE_DRXSTS_TypeDef;

typedef struct 
{
    unsigned chnum:4;
    unsigned bcnt:11;
    unsigned dpid:2;

    unsigned pktsts:4;
#define DWC_GRXSTS_PKTSTS_IN                (0x2)
#define DWC_GRXSTS_PKTSTS_IN_XFER_COMP      (0x3)
#define DWC_GRXSTS_PKTSTS_DATA_TOGGLE_ERR   (0x5)
#define DWC_GRXSTS_PKTSTS_CH_HALTED         (0x7)

    unsigned reserved21_31:11;
}stc_bUSB_CORE_GRXSTS_t;  /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_GRXSTS_t b;  
} USB_CORE_GRXFSTS_TypeDef ;

typedef struct 
{
    unsigned startaddr:16;
    unsigned depth:16;
}stc_bUSB_CORE_FSIZ_t; 
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_FSIZ_t b;  /* C-STAT */
} USB_CORE_FSIZ_TypeDef;

typedef struct 
{
    unsigned nptxfspcavail:16;
    unsigned nptxqspcavail:8;
    /** Top of the Non-Periodic Transmit Request Queue
    *   - bit 24 - Terminate (Last entry for the selected
    *     channel/EP)
    *   - bits 26:25 - Token Type
    *     - 2'b00 - IN/OUT
    *     - 2'b01 - Zero Length OUT
    *     - 2'b10 - PING/Complete Split
    *     - 2'b11 - Channel Halt
    *   - bits 30:27 - Channel/EP Number
    */
    unsigned nptxqtop_terminate:1;
    unsigned nptxqtop_token:2;
    unsigned nptxqtop_chnep:4;
    unsigned reserved:1;
}stc_bUSB_CORE_HNPTXSTS_t;  
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HNPTXSTS_t b;  
} USB_CORE_HNPTXSTS_TypeDef;

typedef struct 
{
    unsigned txfspcavail:16;
    unsigned reserved:16;
}stc_bUSB_CORE_DTXFSTSn_t;  
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_DTXFSTSn_t b;  
} USB_CORE_DTXFSTSn_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        unsigned rwdata:8;
        unsigned regaddr:8;
        unsigned addr:7;
        unsigned i2cen:1;
        unsigned ack:1;
        unsigned i2csuspctl:1;
        unsigned i2cdevaddr:2;
        unsigned i2cdatse0:1;
        unsigned reserved:1;
        unsigned rw:1;
        unsigned bsydne:1;
    }
    b;
} USB_CORE_GI2CCTL_TypeDef;

typedef union 
{
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
        unsigned regdata:8;
        unsigned vctrl:8;
        unsigned regaddr16_21:6;
        unsigned regwr:1;
        unsigned reserved23_24:2;
        unsigned newregreq:1;
        unsigned vstsbsy:1;
        unsigned vstsdone:1;
        unsigned reserved28_30:3;
        unsigned disulpidrvr:1;
    } b;
} USB_CORE_GPVNDCTL_TypeDef;


typedef union 
{
    uint32_t d32;
    struct
    {
        /** Device Speed */
        unsigned devspd:2;
        /** Non Zero Length Status OUT Handshake */
        unsigned nzstsouthshk:1;
#define DWC_DCFG_SEND_STALL 1

        unsigned ena32khzs:1;
        /** Device Addresses */
        unsigned devaddr:7;
        /** Periodic Frame Interval */
        unsigned perfrint:2;
#define DWC_DCFG_FRAME_INTERVAL_80 (0u)
#define DWC_DCFG_FRAME_INTERVAL_85 (1u)
#define DWC_DCFG_FRAME_INTERVAL_90 (2u)
#define DWC_DCFG_FRAME_INTERVAL_95 (3u)

        /** Enable Device OUT NAK for bulk in DDMA mode */
        unsigned endevoutnak:1;

        unsigned reserved14_17:4;
        /** In Endpoint Mis-match count */
        unsigned epmscnt:5;
        /** Enable Descriptor DMA in Device mode */
        unsigned descdma:1;
        unsigned perschintvl:2;
        unsigned resvalid:6;
    }
    b;
} USB_CORE_DCFG_TypeDef;

typedef union 
{
    uint32_t d32;
    struct
    {
        /** Remote Wakeup */
        unsigned rmtwkupsig:1;
        /** Soft Disconnect */
        unsigned sftdiscon:1;
        /** Global Non-Periodic IN NAK Status */
        unsigned gnpinnaksts:1;
        /** Global OUT NAK Status */
        unsigned goutnaksts:1;
        /** Test Control */
        unsigned tstctl:3;
        /** Set Global Non-Periodic IN NAK */
        unsigned sgnpinnak:1;
        /** Clear Global Non-Periodic IN NAK */
        unsigned cgnpinnak:1;
        /** Set Global OUT NAK */
        unsigned sgoutnak:1;
        /** Clear Global OUT NAK */
        unsigned cgoutnak:1;
        /** Power-On Programming Done */
        unsigned pwronprgdone:1;
        /** Reserved */
        unsigned reserved:1;
        /** Global Multi Count */
        unsigned gmc:2;
        /** Ignore Frame Number for ISOC EPs */
        unsigned ifrmnum:1;
        /** NAK on Babble */
        unsigned nakonbble:1;
        /** Enable Continue on BNA */
        unsigned encontonbna:1;
        /** Enable deep sleep besl reject feature*/
        unsigned besl_reject:1;

        unsigned reserved17_31:13;
    }
    b;
} USB_CORE_DCTL_TypeDef;

typedef struct 
{
            /** Suspend Status */
            unsigned suspsts:1;
            /** Enumerated Speed */
            unsigned enumspd:2;
#define DWC_DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ (0u)
#define DWC_DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ (1u)
#define DWC_DSTS_ENUMSPD_LS_PHY_6MHZ           (2u)
#define DWC_DSTS_ENUMSPD_FS_PHY_48MHZ          (3u)
            /** Erratic Error */
            unsigned errticerr:1;
            unsigned reserved4_7:4;
            /** Frame or Microframe Number of the received SOF */
            unsigned soffn:14;
            unsigned reserved22_31:10;
}stc_bUSB_CORE_DSTS_t;
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_DSTS_t b;
} USB_CORE_DSTS_TypeDef;

typedef struct 
{
            /** Transfer complete mask */
            unsigned xfercompl:1;
            /** Endpoint disable mask */
            unsigned epdisabled:1;
            /** AHB Error mask */
            unsigned ahberr:1;
            /** TimeOUT Handshake mask (non-ISOC EPs) */
            unsigned timeout:1;
            /** IN Token received with TxF Empty mask */
            unsigned intktxfemp:1;
            /** IN Token Received with EP mismatch mask */
            unsigned intknepmis:1;
            /** IN Endpoint NAK Effective mask */
            unsigned inepnakeff:1;
            /** Reserved */
            unsigned emptyintr:1;

            unsigned txfifoundrn:1;

            /** BNA Interrupt mask */
            unsigned bna:1;

            unsigned reserved10_12:3;
            /** BNA Interrupt mask */
            unsigned nak:1;

            unsigned reserved14_31:18;
}stc_bUSB_CORE_DIEPINTn_t;  /* C-STAT */
typedef union _USB_CORE_DIEPINTn_TypeDef
{
    uint32_t d32;
    stc_bUSB_CORE_DIEPINTn_t b;  /* C-STAT */
} USB_CORE_DIEPINTn_TypeDef;

typedef union _USB_CORE_DIEPINTn_TypeDef   USB_CORE_DIEPMSK_TypeDef ;

/* Define bit band for C-STAT */
#define  in_xfercompl         (0x00000001ul)
#define  in_epdisabled        (0x00000002ul)
#define  in_ahberr            (0x00000004ul)
#define  in_timeout           (0x00000008ul)
#define  in_intktxfemp        (0x00000010ul)
#define  in_intknepmis        (0x00000020ul)
#define  in_inepnakeff        (0x00000040ul)
#define  in_emptyintr         (0x00000080ul)
#define  in_txfifoundrn       (0x00000100ul)
#define  in_bna               (0x00000200ul)
#define  in_nak               (0x00002000ul)

#define  out_xfercompl        (0x00000001ul)
#define  out_epdisabled       (0x00000002ul)
#define  out_ahberr           (0x00000004ul)
#define  out_setup            (0x00000008ul)
#define  out_outtknepdis      (0x00000010ul)
#define  out_stsphsercvd      (0x00000020ul)
#define  out_back2backsetup   (0x00000040ul)
#define  out_outpkterr        (0x00000100ul)
#define  out_bna              (0x00000200ul)
#define  out_pktdrpsts        (0x00000800ul)
#define  out_babble           (0x00001000ul)
#define  out_nak              (0x00002000ul)
#define  out_nyet             (0x00004000ul)
#define  out_sr               (0x00008000ul)

typedef struct 
{
            /** Transfer complete */
            unsigned xfercompl:1;
            /** Endpoint disable  */
            unsigned epdisabled:1;
            /** AHB Error */
            unsigned ahberr:1;
            /** Setup Phase Done (contorl EPs) */
            unsigned setup:1;
            /** OUT Token Received when Endpoint Disabled */
            unsigned outtknepdis:1;

            unsigned stsphsercvd:1;
            /** Back-to-Back SETUP Packets Received */
            unsigned back2backsetup:1;

            unsigned reserved7:1;
            /** OUT packet Error */
            unsigned outpkterr:1;
            /** BNA Interrupt */
            unsigned bna:1;

            unsigned reserved10:1;
            /** Packet Drop Status */
            unsigned pktdrpsts:1;
            /** Babble Interrupt */
            unsigned babble:1;
            /** NAK Interrupt */
            unsigned nak:1;
            /** NYET Interrupt */
            unsigned nyet:1;
            /** Bit indicating setup packet received */
            unsigned sr:1;

            unsigned reserved16_31:16;
}stc_bUSB_CORE_DOEPINTn_t;  /* C-STAT */
typedef union _USB_CORE_DOEPINTn_TypeDef
{
    uint32_t d32;
    stc_bUSB_CORE_DOEPINTn_t b;  /* C-STAT */
} USB_CORE_DOEPINTn_TypeDef;

typedef union _USB_CORE_DOEPINTn_TypeDef   USB_CORE_DOEPMSK_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct {
        /** IN Endpoint bits */
        unsigned in:16;
        /** OUT Endpoint bits */
        unsigned out:16;
    } ep;
    struct {
        /** IN Endpoint bits */
        unsigned inep0:1;
        unsigned inep1:1;
        unsigned inep2:1;
        unsigned inep3:1;
        unsigned inep4:1;
        unsigned inep5:1;
        unsigned inep6:1;
        unsigned inep7:1;
        unsigned inep8:1;
        unsigned inep9:1;
        unsigned inep10:1;
        unsigned inep11:1;
        unsigned inep12:1;
        unsigned inep13:1;
        unsigned inep14:1;
        unsigned inep15:1;
        /** OUT Endpoint bits */
        unsigned outep0:1;
        unsigned outep1:1;
        unsigned outep2:1;
        unsigned outep3:1;
        unsigned outep4:1;
        unsigned outep5:1;
        unsigned outep6:1;
        unsigned outep7:1;
        unsigned outep8:1;
        unsigned outep9:1;
        unsigned outep10:1;
        unsigned outep11:1;
        unsigned outep12:1;
        unsigned outep13:1;
        unsigned outep14:1;
        unsigned outep15:1;
    } b;
} USB_CORE_DAINT_TypeDef;

typedef union 
{
    uint32_t d32;
    struct
    {
        /** non ISO Tx Thr. Enable */
        unsigned non_iso_thr_en:1;
        /** ISO Tx Thr. Enable */
        unsigned iso_thr_en:1;
        /** Tx Thr. Length */
        unsigned tx_thr_len:9;
        /** AHB Threshold ratio */
        unsigned ahb_thr_ratio:2;
        /** Reserved */
        unsigned reserved13_15:3;
        /** Rx Thr. Enable */
        unsigned rx_thr_en:1;
        /** Rx Thr. Length */
        unsigned rx_thr_len:9;
        unsigned reserved26:1;
        /** Arbiter Parking Enable*/
        unsigned arbprken:1;
        /** Reserved */
        unsigned reserved28_31:4;
    }
    b;
} USB_CORE_DTHRCTL_TypeDef;

typedef struct 
{
            /** Maximum Packet Size
             * IN/OUT EPn
             * IN/OUT EP0 - 2 bits
             *   2'b00: 64 Bytes
             *   2'b01: 32
             *   2'b10: 16
             *   2'b11: 8 */
            unsigned mps:11;
#define DWC_DEP0CTL_MPS_64   (0u)
#define DWC_DEP0CTL_MPS_32   (1u)
#define DWC_DEP0CTL_MPS_16   (2u)
#define DWC_DEP0CTL_MPS_8    (3u)

            /** Next Endpoint
             * IN EPn/IN EP0
             * OUT EPn/OUT EP0 - reserved */
            unsigned nextep:4;

            /** USB Active Endpoint */
            unsigned usbactep:1;

            /** Endpoint DPID (INTR/Bulk IN and OUT endpoints)
             * This field contains the PID of the packet going to
             * be received or transmitted on this endpoint. The
             * application should program the PID of the first
             * packet going to be received or transmitted on this
             * endpoint , after the endpoint is
             * activated. Application use the SetD1PID and
             * SetD0PID fields of this register to program either
             * D0 or D1 PID.
             *
             * The encoding for this field is
             *   - 0: D0
             *   - 1: D1
             */
            unsigned dpid:1;

            /** NAK Status */
            unsigned naksts:1;

            /** Endpoint Type
             *  2'b00: Control
             *  2'b01: Isochronous
             *  2'b10: Bulk
             *  2'b11: Interrupt */
            unsigned eptype:2;

            /** Snoop Mode
             * OUT EPn/OUT EP0
             * IN EPn/IN EP0 - reserved */
            unsigned snp:1;

            /** Stall Handshake */
            unsigned stall:1;

            /** Tx Fifo Number
             * IN EPn/IN EP0
             * OUT EPn/OUT EP0 - reserved */
            unsigned txfnum:4;

            /** Clear NAK */
            unsigned cnak:1;
            /** Set NAK */
            unsigned snak:1;
            /** Set DATA0 PID (INTR/Bulk IN and OUT endpoints)
             * Writing to this field sets the Endpoint DPID (DPID)
             * field in this register to DATA0. Set Even
             * (micro)frame (SetEvenFr) (ISO IN and OUT Endpoints)
             * Writing to this field sets the Even/Odd
             * (micro)frame (EO_FrNum) field to even (micro)
             * frame.
             */
            unsigned setd0pid:1;
            /** Set DATA1 PID (INTR/Bulk IN and OUT endpoints)
             * Writing to this field sets the Endpoint DPID (DPID)
             * field in this register to DATA1 Set Odd
             * (micro)frame (SetOddFr) (ISO IN and OUT Endpoints)
             * Writing to this field sets the Even/Odd
             * (micro)frame (EO_FrNum) field to odd (micro) frame.
             */
            unsigned setd1pid:1;

            /** Endpoint Disable */
            unsigned epdis:1;
            /** Endpoint Enable */
            unsigned epena:1;
}stc_bUSB_CORE_DEPCTL_t;  /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_DEPCTL_t b;  /* C-STAT */
} USB_CORE_DEPCTL_TypeDef;

typedef struct 
{
            /** Transfer size */
            unsigned xfersize:19;
/** Max packet count for EP (pow(2,10)-1) */
#define MAX_PKT_CNT 1023
            /** Packet Count */
            unsigned pktcnt:10;
            /** Multi Count - Periodic IN endpoints */
            unsigned mc:2;
            unsigned reserved:1;
}stc_bUSB_CORE_DEPXFRSIZ_t;
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_DEPXFRSIZ_t b;  /* C-STAT */
} USB_CORE_DEPXFRSIZ_TypeDef ;
typedef union 
{
    uint32_t d32;
    struct
    {
        /** Transfer size */
        unsigned xfersize:7;
        /** Reserved */
        unsigned reserved7_18:12;
        /** Packet Count */
        unsigned pktcnt:2;
        /** Reserved */
        unsigned reserved21_28:8;
        /**Setup Packet Count (DOEPTSIZ0 Only) */
        unsigned supcnt:2;
        unsigned reserved31;
    }
    b;
} USB_CORE_DEP0XFRSIZ_TypeDef ;

typedef struct 
{
    uint32_t fslspclksel :
        2;
    uint32_t fslssupp :
        1;
}stc_bUSB_CORE_HCFG_t;   /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HCFG_t b;  /* C-STAT */
} USB_CORE_HCFG_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        uint32_t frint :
            16;
        uint32_t Reserved :
            16;
    }
    b;
} USB_CORE_HFRMINTRVL_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        uint32_t frnum :
            16;
        uint32_t frrem :
            16;
    }
    b;
} USB_CORE_HFNUM_TypeDef ;

typedef struct 
{
    uint32_t ptxfspcavail :
        16;
    uint32_t ptxqspcavail :
        8;
    struct
    {
        uint32_t terminate :
            1;
        uint32_t token :
            2;
        uint32_t chnum :
            4;
        uint32_t odd_even :
            1;
    } ptxqtop;
}stc_bUSB_CORE_HPTXSTS_t;  /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HPTXSTS_t b;  /* C-STAT */
} USB_CORE_HPTXSTS_TypeDef ;

typedef struct 
{
    uint32_t prtconnsts :
        1;
    uint32_t prtconndet :
        1;
    uint32_t prtena :
        1;
    uint32_t prtenchng :
        1;
    uint32_t prtovrcurract :
        1;
    uint32_t prtovrcurrchng :
        1;
    uint32_t prtres :
        1;
    uint32_t prtsusp :
        1;
    uint32_t prtrst :
        1;
    uint32_t Reserved9 :
        1;
    uint32_t prtlnsts :
        2;
    uint32_t prtpwr :
        1;
    uint32_t prttstctl :
        4;
    uint32_t prtspd :
        2;
    uint32_t Reserved19_31 :
        13;
}stc_bUSB_CORE_HPRT0_t;   /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HPRT0_t b;  /* C-STAT */
} USB_CORE_HPRT_TypeDef ;

typedef struct 
{
    uint32_t chint :
        16;
    uint32_t Reserved :
        16;
}stc_bUSB_CORE_HAINT_t; /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HAINT_t b;  /* C-STAT */
} USB_CORE_HAINT_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        uint32_t chint :
            16;
        uint32_t Reserved :
            16;
    }
    b;
} USB_CORE_HAINTMSK_TypeDef ;

typedef struct 
{
    uint32_t mps :
        11;
    uint32_t epnum :
        4;
    uint32_t epdir :
        1;
    uint32_t Reserved :
        1;
    uint32_t lspddev :
        1;
    uint32_t eptype :
        2;
    uint32_t multicnt :
        2;
    uint32_t devaddr :
        7;
    uint32_t oddfrm :
        1;
    uint32_t chdis :
        1;
    uint32_t chen :
        1;
}stc_bUSB_CORE_HCCHAR_t;   /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HCCHAR_t b;  /* C-STAT */
} USB_CORE_HCCHAR_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        uint32_t prtaddr :
            7;
        uint32_t hubaddr :
            7;
        uint32_t xactpos :
            2;
        uint32_t compsplt :
            1;
        uint32_t Reserved :
            14;
        uint32_t spltena :
            1;
    }
    b;
} USB_CORE_HCSPLT_TypeDef ;


/* Bit define for C-STAT */
#define hc_xfercompl    (0x000000001ul)
#define hc_chhltd       (0x000000002ul)
#define hc_ahberr       (0x000000004ul)
#define hc_stall        (0x000000008ul)
#define hc_nak          (0x000000010ul)
#define hc_ack          (0x000000020ul)
#define hc_nyet         (0x000000040ul)
#define hc_xacterr      (0x000000080ul)
#define hc_bblerr       (0x000000100ul)
#define hc_frmovrun     (0x000000200ul)
#define hc_datatglerr   (0x000000400ul)

typedef struct 
{
    uint32_t xfercompl :
        1;
    uint32_t chhltd :
        1;
    uint32_t ahberr :
        1;
    uint32_t stall :
        1;
    uint32_t nak :
        1;
    uint32_t ack :
        1;
    uint32_t nyet :
        1;
    uint32_t xacterr :
        1;
    uint32_t bblerr :
        1;
    uint32_t frmovrun :
        1;
    uint32_t datatglerr :
        1;
    uint32_t Reserved :
        21;
}stc_bUSB_CORE_HCINTn_t;  /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HCINTn_t b; /* C-STAT */
} USB_CORE_HCINTn_TypeDef ;

typedef struct 
{
    uint32_t xfersize :
        19;
    uint32_t pktcnt :
        10;
    uint32_t pid :
        2;
    uint32_t dopng :
        1;
}stc_bUSB_CORE_HCTSIZn_t;   /* C-STAT */
typedef union 
{
    uint32_t d32;
    stc_bUSB_CORE_HCTSIZn_t b; /* C-STAT */
} USB_CORE_HCTSIZn_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        uint32_t xfercompl :
            1;
        uint32_t chhltd :
            1;
        uint32_t ahberr :
            1;
        uint32_t stall :
            1;
        uint32_t nak :
            1;
        uint32_t ack :
            1;
        uint32_t nyet :
            1;
        uint32_t xacterr :
            1;
        uint32_t bblerr :
            1;
        uint32_t frmovrun :
            1;
        uint32_t datatglerr :
            1;
        uint32_t Reserved :
            21;
    }
    b;
} USB_CORE_HCINTMSK_TypeDef ;

typedef union 
{
    uint32_t d32;
    struct
    {
        /** Stop Pclk */
        unsigned stoppclk:1;
        /** Gate Hclk */
        unsigned gatehclk:1;
        /** Power Clamp */
        unsigned pwrclmp:1;
        /** Reset Power Down Modules */
        unsigned rstpdwnmodule:1;
        /** Reserved */
        unsigned reserved:1;
        /** Enable Sleep Clock Gating (Enbl_L1Gating) */
        unsigned enbl_sleep_gating:1;
        /** PHY In Sleep (PhySleep) */
        unsigned phy_in_sleep:1;
        /** Deep Sleep*/
        unsigned deep_sleep:1;
        unsigned resetaftsusp:1;
        unsigned restoremode:1;
        unsigned enbl_extnd_hiber:1;
        unsigned extnd_hiber_pwrclmp:1;
        unsigned extnd_hiber_switch:1;
        unsigned ess_reg_restored:1;
        unsigned prt_clk_sel:2;
        unsigned port_power:1;
        unsigned max_xcvrselect:2;
        unsigned max_termsel:1;
        unsigned mac_dev_addr:7;
        unsigned p2hd_dev_enum_spd:2;
        unsigned p2hd_prt_spd:2;
        unsigned if_dev_mode:1;
    }
    b;
} USB_CORE_PCGCCTL_TypeDef ;

#endif //__USB_CORE_REGS_H__

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

