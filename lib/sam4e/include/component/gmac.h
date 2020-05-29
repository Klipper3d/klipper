/* ---------------------------------------------------------------------------- */
/*                  Atmel Microcontroller Software Support                      */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Atmel Corporation                                        */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Atmel's name may not be used to endorse or promote products derived from     */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR   */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE   */
/* DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,      */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,  */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    */
/* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING         */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                           */
/* ---------------------------------------------------------------------------- */

#ifndef _SAM4E_GMAC_COMPONENT_
#define _SAM4E_GMAC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Gigabit Ethernet MAC */
/* ============================================================================= */
/** \addtogroup SAM4E_GMAC Gigabit Ethernet MAC */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief GmacSa hardware registers */
typedef struct {
  RwReg   GMAC_SAB;       /**< \brief (GmacSa Offset: 0x0) Specific Address 1 Bottom [31:0] Register */
  RwReg   GMAC_SAT;       /**< \brief (GmacSa Offset: 0x4) Specific Address 1 Top [47:32] Register */
} GmacSa;
/** \brief Gmac hardware registers */
#define GMACSA_NUMBER 4
typedef struct {
  RwReg   GMAC_NCR;       /**< \brief (Gmac Offset: 0x000) Network Control Register */
  RwReg   GMAC_NCFGR;     /**< \brief (Gmac Offset: 0x004) Network Configuration Register */
  RoReg   GMAC_NSR;       /**< \brief (Gmac Offset: 0x008) Network Status Register */
  RwReg   GMAC_UR;        /**< \brief (Gmac Offset: 0x00C) User Register */
  RwReg   GMAC_DCFGR;     /**< \brief (Gmac Offset: 0x010) DMA Configuration Register */
  RwReg   GMAC_TSR;       /**< \brief (Gmac Offset: 0x014) Transmit Status Register */
  RwReg   GMAC_RBQB;      /**< \brief (Gmac Offset: 0x018) Receive Buffer Queue Base Address */
  RwReg   GMAC_TBQB;      /**< \brief (Gmac Offset: 0x01C) Transmit Buffer Queue Base Address */
  RwReg   GMAC_RSR;       /**< \brief (Gmac Offset: 0x020) Receive Status Register */
  RoReg   GMAC_ISR;       /**< \brief (Gmac Offset: 0x024) Interrupt Status Register */
  WoReg   GMAC_IER;       /**< \brief (Gmac Offset: 0x028) Interrupt Enable Register */
  WoReg   GMAC_IDR;       /**< \brief (Gmac Offset: 0x02C) Interrupt Disable Register */
  RoReg   GMAC_IMR;       /**< \brief (Gmac Offset: 0x030) Interrupt Mask Register */
  RwReg   GMAC_MAN;       /**< \brief (Gmac Offset: 0x034) PHY Maintenance Register */
  RoReg   GMAC_RPQ;       /**< \brief (Gmac Offset: 0x038) Received Pause Quantum Register */
  RwReg   GMAC_TPQ;       /**< \brief (Gmac Offset: 0x03C) Transmit Pause Quantum Register */
  RoReg   Reserved1[16];
  RwReg   GMAC_HRB;       /**< \brief (Gmac Offset: 0x080) Hash Register Bottom [31:0] */
  RwReg   GMAC_HRT;       /**< \brief (Gmac Offset: 0x084) Hash Register Top [63:32] */
  GmacSa  GMAC_SA[GMACSA_NUMBER]; /**< \brief (Gmac Offset: 0x088) 1 .. 4 */
  RwReg   GMAC_TIDM[4];   /**< \brief (Gmac Offset: 0x0A8) Type ID Match 1 Register */
  RoReg   Reserved2[1];
  RwReg   GMAC_IPGS;      /**< \brief (Gmac Offset: 0x0BC) IPG Stretch Register */
  RwReg   GMAC_SVLAN;     /**< \brief (Gmac Offset: 0x0C0) Stacked VLAN Register */
  RwReg   GMAC_TPFCP;     /**< \brief (Gmac Offset: 0x0C4) Transmit PFC Pause Register */
  RwReg   GMAC_SAMB1;     /**< \brief (Gmac Offset: 0x0C8) Specific Address 1 Mask Bottom [31:0] Register */
  RwReg   GMAC_SAMT1;     /**< \brief (Gmac Offset: 0x0CC) Specific Address 1 Mask Top [47:32] Register */
  RoReg   Reserved3[12];
  RoReg   GMAC_OTLO;      /**< \brief (Gmac Offset: 0x100) Octets Transmitted [31:0] Register */
  RoReg   GMAC_OTHI;      /**< \brief (Gmac Offset: 0x104) Octets Transmitted [47:32] Register */
  RoReg   GMAC_FT;        /**< \brief (Gmac Offset: 0x108) Frames Transmitted Register */
  RoReg   GMAC_BCFT;      /**< \brief (Gmac Offset: 0x10C) Broadcast Frames Transmitted Register */
  RoReg   GMAC_MFT;       /**< \brief (Gmac Offset: 0x110) Multicast Frames Transmitted Register */
  RoReg   GMAC_PFT;       /**< \brief (Gmac Offset: 0x114) Pause Frames Transmitted Register */
  RoReg   GMAC_BFT64;     /**< \brief (Gmac Offset: 0x118) 64 Byte Frames Transmitted Register */
  RoReg   GMAC_TBFT127;   /**< \brief (Gmac Offset: 0x11C) 65 to 127 Byte Frames Transmitted Register */
  RoReg   GMAC_TBFT255;   /**< \brief (Gmac Offset: 0x120) 128 to 255 Byte Frames Transmitted Register */
  RoReg   GMAC_TBFT511;   /**< \brief (Gmac Offset: 0x124) 256 to 511 Byte Frames Transmitted Register */
  RoReg   GMAC_TBFT1023;  /**< \brief (Gmac Offset: 0x128) 512 to 1023 Byte Frames Transmitted Register */
  RoReg   GMAC_TBFT1518;  /**< \brief (Gmac Offset: 0x12C) 1024 to 1518 Byte Frames Transmitted Register */
  RoReg   GMAC_GTBFT1518; /**< \brief (Gmac Offset: 0x130) Greater Than 1518 Byte Frames Transmitted Register */
  RoReg   GMAC_TUR;       /**< \brief (Gmac Offset: 0x134) Transmit Under Runs Register */
  RoReg   GMAC_SCF;       /**< \brief (Gmac Offset: 0x138) Single Collision Frames Register */
  RoReg   GMAC_MCF;       /**< \brief (Gmac Offset: 0x13C) Multiple Collision Frames Register */
  RoReg   GMAC_EC;        /**< \brief (Gmac Offset: 0x140) Excessive Collisions Register */
  RoReg   GMAC_LC;        /**< \brief (Gmac Offset: 0x144) Late Collisions Register */
  RoReg   GMAC_DTF;       /**< \brief (Gmac Offset: 0x148) Deferred Transmission Frames Register */
  RoReg   GMAC_CSE;       /**< \brief (Gmac Offset: 0x14C) Carrier Sense Errors Register */
  RoReg   GMAC_ORLO;      /**< \brief (Gmac Offset: 0x150) Octets Received [31:0] Received */
  RoReg   GMAC_ORHI;      /**< \brief (Gmac Offset: 0x154) Octets Received [47:32] Received */
  RoReg   GMAC_FR;        /**< \brief (Gmac Offset: 0x158) Frames Received Register */
  RoReg   GMAC_BCFR;      /**< \brief (Gmac Offset: 0x15C) Broadcast Frames Received Register */
  RoReg   GMAC_MFR;       /**< \brief (Gmac Offset: 0x160) Multicast Frames Received Register */
  RoReg   GMAC_PFR;       /**< \brief (Gmac Offset: 0x164) Pause Frames Received Register */
  RoReg   GMAC_BFR64;     /**< \brief (Gmac Offset: 0x168) 64 Byte Frames Received Register */
  RoReg   GMAC_TBFR127;   /**< \brief (Gmac Offset: 0x16C) 65 to 127 Byte Frames Received Register */
  RoReg   GMAC_TBFR255;   /**< \brief (Gmac Offset: 0x170) 128 to 255 Byte Frames Received Register */
  RoReg   GMAC_TBFR511;   /**< \brief (Gmac Offset: 0x174) 256 to 511Byte Frames Received Register */
  RoReg   GMAC_TBFR1023;  /**< \brief (Gmac Offset: 0x178) 512 to 1023 Byte Frames Received Register */
  RoReg   GMAC_TBFR1518;  /**< \brief (Gmac Offset: 0x17C) 1024 to 1518 Byte Frames Received Register */
  RoReg   GMAC_TMXBFR;    /**< \brief (Gmac Offset: 0x180) 1519 to Maximum Byte Frames Received Register */
  RoReg   GMAC_UFR;       /**< \brief (Gmac Offset: 0x184) Undersize Frames Received Register */
  RoReg   GMAC_OFR;       /**< \brief (Gmac Offset: 0x188) Oversize Frames Received Register */
  RoReg   GMAC_JR;        /**< \brief (Gmac Offset: 0x18C) Jabbers Received Register */
  RoReg   GMAC_FCSE;      /**< \brief (Gmac Offset: 0x190) Frame Check Sequence Errors Register */
  RoReg   GMAC_LFFE;      /**< \brief (Gmac Offset: 0x194) Length Field Frame Errors Register */
  RoReg   GMAC_RSE;       /**< \brief (Gmac Offset: 0x198) Receive Symbol Errors Register */
  RoReg   GMAC_AE;        /**< \brief (Gmac Offset: 0x19C) Alignment Errors Register */
  RoReg   GMAC_RRE;       /**< \brief (Gmac Offset: 0x1A0) Receive Resource Errors Register */
  RoReg   GMAC_ROE;       /**< \brief (Gmac Offset: 0x1A4) Receive Overrun Register */
  RoReg   GMAC_IHCE;      /**< \brief (Gmac Offset: 0x1A8) IP Header Checksum Errors Register */
  RoReg   GMAC_TCE;       /**< \brief (Gmac Offset: 0x1AC) TCP Checksum Errors Register */
  RoReg   GMAC_UCE;       /**< \brief (Gmac Offset: 0x1B0) UDP Checksum Errors Register */
  RoReg   Reserved4[5];
  RwReg   GMAC_TSSS;      /**< \brief (Gmac Offset: 0x1C8) 1588 Timer Sync Strobe Seconds Register */
  RwReg   GMAC_TSSN;      /**< \brief (Gmac Offset: 0x1CC) 1588 Timer Sync Strobe Nanoseconds Register */
  RwReg   GMAC_TS;        /**< \brief (Gmac Offset: 0x1D0) 1588 Timer Seconds Register */
  RwReg   GMAC_TN;        /**< \brief (Gmac Offset: 0x1D4) 1588 Timer Nanoseconds Register */
  WoReg   GMAC_TA;        /**< \brief (Gmac Offset: 0x1D8) 1588 Timer Adjust Register */
  RwReg   GMAC_TI;        /**< \brief (Gmac Offset: 0x1DC) 1588 Timer Increment Register */
  RoReg   GMAC_EFTS;      /**< \brief (Gmac Offset: 0x1E0) PTP Event Frame Transmitted Seconds */
  RoReg   GMAC_EFTN;      /**< \brief (Gmac Offset: 0x1E4) PTP Event Frame Transmitted Nanoseconds */
  RoReg   GMAC_EFRS;      /**< \brief (Gmac Offset: 0x1E8) PTP Event Frame Received Seconds */
  RoReg   GMAC_EFRN;      /**< \brief (Gmac Offset: 0x1EC) PTP Event Frame Received Nanoseconds */
  RoReg   GMAC_PEFTS;     /**< \brief (Gmac Offset: 0x1F0) PTP Peer Event Frame Transmitted Seconds */
  RoReg   GMAC_PEFTN;     /**< \brief (Gmac Offset: 0x1F4) PTP Peer Event Frame Transmitted Nanoseconds */
  RoReg   GMAC_PEFRS;     /**< \brief (Gmac Offset: 0x1F8) PTP Peer Event Frame Received Seconds */
  RoReg   GMAC_PEFRN;     /**< \brief (Gmac Offset: 0x1FC) PTP Peer Event Frame Received Nanoseconds */
} Gmac;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- GMAC_NCR : (GMAC Offset: 0x000) Network Control Register -------- */
#define GMAC_NCR_LB (0x1u << 0) /**< \brief (GMAC_NCR) Loop Back */
#define GMAC_NCR_LBL (0x1u << 1) /**< \brief (GMAC_NCR) Loop Back Local */
#define GMAC_NCR_RXEN (0x1u << 2) /**< \brief (GMAC_NCR) Receive Enable */
#define GMAC_NCR_TXEN (0x1u << 3) /**< \brief (GMAC_NCR) Transmit Enable */
#define GMAC_NCR_MPE (0x1u << 4) /**< \brief (GMAC_NCR) Management Port Enable */
#define GMAC_NCR_CLRSTAT (0x1u << 5) /**< \brief (GMAC_NCR) Clear Statistics Registers */
#define GMAC_NCR_INCSTAT (0x1u << 6) /**< \brief (GMAC_NCR) Increment Statistics Registers */
#define GMAC_NCR_WESTAT (0x1u << 7) /**< \brief (GMAC_NCR) Write Enable for Statistics Registers */
#define GMAC_NCR_BP (0x1u << 8) /**< \brief (GMAC_NCR) Back pressure */
#define GMAC_NCR_TSTART (0x1u << 9) /**< \brief (GMAC_NCR) Start Transmission */
#define GMAC_NCR_THALT (0x1u << 10) /**< \brief (GMAC_NCR) Transmit Halt */
#define GMAC_NCR_TXPF (0x1u << 11) /**< \brief (GMAC_NCR) Transmit Pause Frame */
#define GMAC_NCR_TXZQPF (0x1u << 12) /**< \brief (GMAC_NCR) Transmit Zero Quantum Pause Frame */
#define GMAC_NCR_RDS (0x1u << 14) /**< \brief (GMAC_NCR) Read Snapshot */
#define GMAC_NCR_SRTSM (0x1u << 15) /**< \brief (GMAC_NCR) Store Receive Time Stamp to Memory */
#define GMAC_NCR_ENPBPR (0x1u << 16) /**< \brief (GMAC_NCR) Enable PFC Priority-based Pause Reception */
#define GMAC_NCR_TXPBPF (0x1u << 17) /**< \brief (GMAC_NCR) Transmit PFC Priority-based Pause Frame */
#define GMAC_NCR_FNP (0x1u << 18) /**< \brief (GMAC_NCR) Flush Next Packet */
/* -------- GMAC_NCFGR : (GMAC Offset: 0x004) Network Configuration Register -------- */
#define GMAC_NCFGR_SPD (0x1u << 0) /**< \brief (GMAC_NCFGR) Speed */
#define GMAC_NCFGR_FD (0x1u << 1) /**< \brief (GMAC_NCFGR) Full Duplex */
#define GMAC_NCFGR_DNVLAN (0x1u << 2) /**< \brief (GMAC_NCFGR) Discard Non-VLAN FRAMES */
#define GMAC_NCFGR_JFRAME (0x1u << 3) /**< \brief (GMAC_NCFGR) Jumbo Frame Size */
#define GMAC_NCFGR_CAF (0x1u << 4) /**< \brief (GMAC_NCFGR) Copy All Frames */
#define GMAC_NCFGR_NBC (0x1u << 5) /**< \brief (GMAC_NCFGR) No Broadcast */
#define GMAC_NCFGR_MTIHEN (0x1u << 6) /**< \brief (GMAC_NCFGR) Multicast Hash Enable */
#define GMAC_NCFGR_UNIHEN (0x1u << 7) /**< \brief (GMAC_NCFGR) Unicast Hash Enable */
#define GMAC_NCFGR_MAXFS (0x1u << 8) /**< \brief (GMAC_NCFGR) 1536 Maximum Frame Size */
#define GMAC_NCFGR_RTY (0x1u << 12) /**< \brief (GMAC_NCFGR) Retry Test */
#define GMAC_NCFGR_PEN (0x1u << 13) /**< \brief (GMAC_NCFGR) Pause Enable */
#define GMAC_NCFGR_RXBUFO_Pos 14
#define GMAC_NCFGR_RXBUFO_Msk (0x3u << GMAC_NCFGR_RXBUFO_Pos) /**< \brief (GMAC_NCFGR) Receive Buffer Offset */
#define GMAC_NCFGR_RXBUFO(value) ((GMAC_NCFGR_RXBUFO_Msk & ((value) << GMAC_NCFGR_RXBUFO_Pos)))
#define GMAC_NCFGR_LFERD (0x1u << 16) /**< \brief (GMAC_NCFGR) Length Field Error Frame Discard */
#define GMAC_NCFGR_RFCS (0x1u << 17) /**< \brief (GMAC_NCFGR) Remove FCS */
#define GMAC_NCFGR_CLK_Pos 18
#define GMAC_NCFGR_CLK_Msk (0x7u << GMAC_NCFGR_CLK_Pos) /**< \brief (GMAC_NCFGR) MDC CLock Division */
#define   GMAC_NCFGR_CLK_MCK_8 (0x0u << 18) /**< \brief (GMAC_NCFGR) MCK divided by 8 (MCK up to 20 MHz) */
#define   GMAC_NCFGR_CLK_MCK_16 (0x1u << 18) /**< \brief (GMAC_NCFGR) MCK divided by 16 (MCK up to 40 MHz) */
#define   GMAC_NCFGR_CLK_MCK_32 (0x2u << 18) /**< \brief (GMAC_NCFGR) MCK divided by 32 (MCK up to 80 MHz) */
#define   GMAC_NCFGR_CLK_MCK_48 (0x3u << 18) /**< \brief (GMAC_NCFGR) MCK divided by 48 (MCK up to 120MHz) */
#define   GMAC_NCFGR_CLK_MCK_64 (0x4u << 18) /**< \brief (GMAC_NCFGR) MCK divided by 64 (MCK up to 160 MHz) */
#define   GMAC_NCFGR_CLK_MCK_96 (0x5u << 18) /**< \brief (GMAC_NCFGR) MCK divided by 96 (MCK up to 240 MHz) */
#define GMAC_NCFGR_DBW_Pos 21
#define GMAC_NCFGR_DBW_Msk (0x3u << GMAC_NCFGR_DBW_Pos) /**< \brief (GMAC_NCFGR) Data Bus Width */
#define GMAC_NCFGR_DBW(value) ((GMAC_NCFGR_DBW_Msk & ((value) << GMAC_NCFGR_DBW_Pos)))
#define GMAC_NCFGR_DCPF (0x1u << 23) /**< \brief (GMAC_NCFGR) Disable Copy of Pause Frames */
#define GMAC_NCFGR_RXCOEN (0x1u << 24) /**< \brief (GMAC_NCFGR) Receive Checksum Offload Enable */
#define GMAC_NCFGR_EFRHD (0x1u << 25) /**< \brief (GMAC_NCFGR) Enable Frames Received in Half Duplex */
#define GMAC_NCFGR_IRXFCS (0x1u << 26) /**< \brief (GMAC_NCFGR) Ignore RX FCS */
#define GMAC_NCFGR_IPGSEN (0x1u << 28) /**< \brief (GMAC_NCFGR) IP Stretch Enable */
#define GMAC_NCFGR_RXBP (0x1u << 29) /**< \brief (GMAC_NCFGR) Receive Bad Preamble */
#define GMAC_NCFGR_IRXER (0x1u << 30) /**< \brief (GMAC_NCFGR) Ignore IPG GRXER */
/* -------- GMAC_NSR : (GMAC Offset: 0x008) Network Status Register -------- */
#define GMAC_NSR_MDIO (0x1u << 1) /**< \brief (GMAC_NSR) MDIO Input Status */
#define GMAC_NSR_IDLE (0x1u << 2) /**< \brief (GMAC_NSR) PHY Management Logic Idle */
/* -------- GMAC_UR : (GMAC Offset: 0x00C) User Register -------- */
#define GMAC_UR_RMIIMII (0x1u << 0) /**< \brief (GMAC_UR)  */
#define GMAC_UR_HDFC (0x1u << 6) /**< \brief (GMAC_UR) Half Duplex Flow Control */
#define GMAC_UR_BPDG (0x1u << 7) /**< \brief (GMAC_UR) BPDG Bypass Deglitchers */
/* -------- GMAC_DCFGR : (GMAC Offset: 0x010) DMA Configuration Register -------- */
#define GMAC_DCFGR_FBLDO_Pos 0
#define GMAC_DCFGR_FBLDO_Msk (0x1fu << GMAC_DCFGR_FBLDO_Pos) /**< \brief (GMAC_DCFGR) Fixed Burst Length for DMA Data Operations: */
#define   GMAC_DCFGR_FBLDO_SINGLE (0x1u << 0) /**< \brief (GMAC_DCFGR) 00001: Always use SINGLE AHB bursts */
#define   GMAC_DCFGR_FBLDO_INCR4 (0x4u << 0) /**< \brief (GMAC_DCFGR) 001xx: Attempt to use INCR4 AHB bursts (Default) */
#define   GMAC_DCFGR_FBLDO_INCR8 (0x8u << 0) /**< \brief (GMAC_DCFGR) 01xxx: Attempt to use INCR8 AHB bursts */
#define   GMAC_DCFGR_FBLDO_INCR16 (0x10u << 0) /**< \brief (GMAC_DCFGR) 1xxxx: Attempt to use INCR16 AHB bursts */
#define GMAC_DCFGR_ESMA (0x1u << 6) /**< \brief (GMAC_DCFGR) Endian Swap Mode Enable for Management Descriptor Accesses */
#define GMAC_DCFGR_ESPA (0x1u << 7) /**< \brief (GMAC_DCFGR) Endian Swap Mode Enable for Packet Data Accesses */
#define GMAC_DCFGR_TXCOEN (0x1u << 11) /**< \brief (GMAC_DCFGR) Transmitter Checksum Generation Offload Enable */
#define GMAC_DCFGR_DRBS_Pos 16
#define GMAC_DCFGR_DRBS_Msk (0xffu << GMAC_DCFGR_DRBS_Pos) /**< \brief (GMAC_DCFGR) DMA Receive Buffer Size */
#define GMAC_DCFGR_DRBS(value) ((GMAC_DCFGR_DRBS_Msk & ((value) << GMAC_DCFGR_DRBS_Pos)))
/* -------- GMAC_TSR : (GMAC Offset: 0x014) Transmit Status Register -------- */
#define GMAC_TSR_UBR (0x1u << 0) /**< \brief (GMAC_TSR) Used Bit Read */
#define GMAC_TSR_COL (0x1u << 1) /**< \brief (GMAC_TSR) Collision Occurred */
#define GMAC_TSR_RLE (0x1u << 2) /**< \brief (GMAC_TSR) Retry Limit Exceeded */
#define GMAC_TSR_TXGO (0x1u << 3) /**< \brief (GMAC_TSR) Transmit Go */
#define GMAC_TSR_TFC (0x1u << 4) /**< \brief (GMAC_TSR) Transmit Frame Corruption due to AHB error */
#define GMAC_TSR_TXCOMP (0x1u << 5) /**< \brief (GMAC_TSR) Transmit Complete */
#define GMAC_TSR_UND (0x1u << 6) /**< \brief (GMAC_TSR) Transmit Under Run */
#define GMAC_TSR_HRESP (0x1u << 8) /**< \brief (GMAC_TSR) HRESP Not OK */
/* -------- GMAC_RBQB : (GMAC Offset: 0x018) Receive Buffer Queue Base Address -------- */
#define GMAC_RBQB_ADDR_Pos 2
#define GMAC_RBQB_ADDR_Msk (0x3fffffffu << GMAC_RBQB_ADDR_Pos) /**< \brief (GMAC_RBQB) Receive buffer queue base address */
#define GMAC_RBQB_ADDR(value) ((GMAC_RBQB_ADDR_Msk & ((value) << GMAC_RBQB_ADDR_Pos)))
/* -------- GMAC_TBQB : (GMAC Offset: 0x01C) Transmit Buffer Queue Base Address -------- */
#define GMAC_TBQB_ADDR_Pos 2
#define GMAC_TBQB_ADDR_Msk (0x3fffffffu << GMAC_TBQB_ADDR_Pos) /**< \brief (GMAC_TBQB) Transmit Buffer Queue Base Address */
#define GMAC_TBQB_ADDR(value) ((GMAC_TBQB_ADDR_Msk & ((value) << GMAC_TBQB_ADDR_Pos)))
/* -------- GMAC_RSR : (GMAC Offset: 0x020) Receive Status Register -------- */
#define GMAC_RSR_BNA (0x1u << 0) /**< \brief (GMAC_RSR) Buffer Not Available */
#define GMAC_RSR_REC (0x1u << 1) /**< \brief (GMAC_RSR) Frame Received */
#define GMAC_RSR_RXOVR (0x1u << 2) /**< \brief (GMAC_RSR) Receive Overrun */
#define GMAC_RSR_HNO (0x1u << 3) /**< \brief (GMAC_RSR) HRESP Not OK */
/* -------- GMAC_ISR : (GMAC Offset: 0x024) Interrupt Status Register -------- */
#define GMAC_ISR_MFS (0x1u << 0) /**< \brief (GMAC_ISR) Management Frame Sent */
#define GMAC_ISR_RCOMP (0x1u << 1) /**< \brief (GMAC_ISR) Receive Complete */
#define GMAC_ISR_RXUBR (0x1u << 2) /**< \brief (GMAC_ISR) RX Used Bit Read */
#define GMAC_ISR_TXUBR (0x1u << 3) /**< \brief (GMAC_ISR) TX Used Bit Read */
#define GMAC_ISR_TUR (0x1u << 4) /**< \brief (GMAC_ISR) Transmit Under Run */
#define GMAC_ISR_RLEX (0x1u << 5) /**< \brief (GMAC_ISR) Retry Limit Exceeded */
#define GMAC_ISR_TFC (0x1u << 6) /**< \brief (GMAC_ISR) Transmit Frame Corruption due to AHB error */
#define GMAC_ISR_TCOMP (0x1u << 7) /**< \brief (GMAC_ISR) Transmit Complete */
#define GMAC_ISR_ROVR (0x1u << 10) /**< \brief (GMAC_ISR) Receive Overrun */
#define GMAC_ISR_HRESP (0x1u << 11) /**< \brief (GMAC_ISR) HRESP Not OK */
#define GMAC_ISR_PFNZ (0x1u << 12) /**< \brief (GMAC_ISR) Pause Frame with Non-zero Pause Quantum Received */
#define GMAC_ISR_PTZ (0x1u << 13) /**< \brief (GMAC_ISR) Pause Time Zero */
#define GMAC_ISR_PFTR (0x1u << 14) /**< \brief (GMAC_ISR) Pause Frame Transmitted */
#define GMAC_ISR_DRQFR (0x1u << 18) /**< \brief (GMAC_ISR) PTP Delay Request Frame Received */
#define GMAC_ISR_SFR (0x1u << 19) /**< \brief (GMAC_ISR) PTP Sync Frame Received */
#define GMAC_ISR_DRQFT (0x1u << 20) /**< \brief (GMAC_ISR) PTP Delay Request Frame Transmitted */
#define GMAC_ISR_SFT (0x1u << 21) /**< \brief (GMAC_ISR) PTP Sync Frame Transmitted */
#define GMAC_ISR_PDRQFR (0x1u << 22) /**< \brief (GMAC_ISR) PDelay Request Frame Received */
#define GMAC_ISR_PDRSFR (0x1u << 23) /**< \brief (GMAC_ISR) PDelay Response Frame Received */
#define GMAC_ISR_PDRQFT (0x1u << 24) /**< \brief (GMAC_ISR) PDelay Request Frame Transmitted */
#define GMAC_ISR_PDRSFT (0x1u << 25) /**< \brief (GMAC_ISR) PDelay Response Frame Transmitted */
#define GMAC_ISR_SRI (0x1u << 26) /**< \brief (GMAC_ISR) TSU Seconds Register Increment */
#define GMAC_ISR_WOL (0x1u << 28) /**< \brief (GMAC_ISR) Wake On LAN */
/* -------- GMAC_IER : (GMAC Offset: 0x028) Interrupt Enable Register -------- */
#define GMAC_IER_MFS (0x1u << 0) /**< \brief (GMAC_IER) Management Frame Sent */
#define GMAC_IER_RCOMP (0x1u << 1) /**< \brief (GMAC_IER) Receive Complete */
#define GMAC_IER_RXUBR (0x1u << 2) /**< \brief (GMAC_IER) RX Used Bit Read */
#define GMAC_IER_TXUBR (0x1u << 3) /**< \brief (GMAC_IER) TX Used Bit Read */
#define GMAC_IER_TUR (0x1u << 4) /**< \brief (GMAC_IER) Transmit Under Run */
#define GMAC_IER_RLEX (0x1u << 5) /**< \brief (GMAC_IER) Retry Limit Exceeded or Late Collision */
#define GMAC_IER_TFC (0x1u << 6) /**< \brief (GMAC_IER) Transmit Frame Corruption due to AHB error */
#define GMAC_IER_TCOMP (0x1u << 7) /**< \brief (GMAC_IER) Transmit Complete */
#define GMAC_IER_ROVR (0x1u << 10) /**< \brief (GMAC_IER) Receive Overrun */
#define GMAC_IER_HRESP (0x1u << 11) /**< \brief (GMAC_IER) HRESP Not OK */
#define GMAC_IER_PFNZ (0x1u << 12) /**< \brief (GMAC_IER) Pause Frame with Non-zero Pause Quantum Received */
#define GMAC_IER_PTZ (0x1u << 13) /**< \brief (GMAC_IER) Pause Time Zero */
#define GMAC_IER_PFTR (0x1u << 14) /**< \brief (GMAC_IER) Pause Frame Transmitted */
#define GMAC_IER_EXINT (0x1u << 15) /**< \brief (GMAC_IER) External Interrupt */
#define GMAC_IER_DRQFR (0x1u << 18) /**< \brief (GMAC_IER) PTP Delay Request Frame Received */
#define GMAC_IER_SFR (0x1u << 19) /**< \brief (GMAC_IER) PTP Sync Frame Received */
#define GMAC_IER_DRQFT (0x1u << 20) /**< \brief (GMAC_IER) PTP Delay Request Frame Transmitted */
#define GMAC_IER_SFT (0x1u << 21) /**< \brief (GMAC_IER) PTP Sync Frame Transmitted */
#define GMAC_IER_PDRQFR (0x1u << 22) /**< \brief (GMAC_IER) PDelay Request Frame Received */
#define GMAC_IER_PDRSFR (0x1u << 23) /**< \brief (GMAC_IER) PDelay Response Frame Received */
#define GMAC_IER_PDRQFT (0x1u << 24) /**< \brief (GMAC_IER) PDelay Request Frame Transmitted */
#define GMAC_IER_PDRSFT (0x1u << 25) /**< \brief (GMAC_IER) PDelay Response Frame Transmitted */
#define GMAC_IER_SRI (0x1u << 26) /**< \brief (GMAC_IER) TSU Seconds Register Increment */
#define GMAC_IER_WOL (0x1u << 28) /**< \brief (GMAC_IER) Wake On LAN */
/* -------- GMAC_IDR : (GMAC Offset: 0x02C) Interrupt Disable Register -------- */
#define GMAC_IDR_MFS (0x1u << 0) /**< \brief (GMAC_IDR) Management Frame Sent */
#define GMAC_IDR_RCOMP (0x1u << 1) /**< \brief (GMAC_IDR) Receive Complete */
#define GMAC_IDR_RXUBR (0x1u << 2) /**< \brief (GMAC_IDR) RX Used Bit Read */
#define GMAC_IDR_TXUBR (0x1u << 3) /**< \brief (GMAC_IDR) TX Used Bit Read */
#define GMAC_IDR_TUR (0x1u << 4) /**< \brief (GMAC_IDR) Transmit Under Run */
#define GMAC_IDR_RLEX (0x1u << 5) /**< \brief (GMAC_IDR) Retry Limit Exceeded or Late Collision */
#define GMAC_IDR_TFC (0x1u << 6) /**< \brief (GMAC_IDR) Transmit Frame Corruption due to AHB error */
#define GMAC_IDR_TCOMP (0x1u << 7) /**< \brief (GMAC_IDR) Transmit Complete */
#define GMAC_IDR_ROVR (0x1u << 10) /**< \brief (GMAC_IDR) Receive Overrun */
#define GMAC_IDR_HRESP (0x1u << 11) /**< \brief (GMAC_IDR) HRESP Not OK */
#define GMAC_IDR_PFNZ (0x1u << 12) /**< \brief (GMAC_IDR) Pause Frame with Non-zero Pause Quantum Received */
#define GMAC_IDR_PTZ (0x1u << 13) /**< \brief (GMAC_IDR) Pause Time Zero */
#define GMAC_IDR_PFTR (0x1u << 14) /**< \brief (GMAC_IDR) Pause Frame Transmitted */
#define GMAC_IDR_EXINT (0x1u << 15) /**< \brief (GMAC_IDR) External Interrupt */
#define GMAC_IDR_DRQFR (0x1u << 18) /**< \brief (GMAC_IDR) PTP Delay Request Frame Received */
#define GMAC_IDR_SFR (0x1u << 19) /**< \brief (GMAC_IDR) PTP Sync Frame Received */
#define GMAC_IDR_DRQFT (0x1u << 20) /**< \brief (GMAC_IDR) PTP Delay Request Frame Transmitted */
#define GMAC_IDR_SFT (0x1u << 21) /**< \brief (GMAC_IDR) PTP Sync Frame Transmitted */
#define GMAC_IDR_PDRQFR (0x1u << 22) /**< \brief (GMAC_IDR) PDelay Request Frame Received */
#define GMAC_IDR_PDRSFR (0x1u << 23) /**< \brief (GMAC_IDR) PDelay Response Frame Received */
#define GMAC_IDR_PDRQFT (0x1u << 24) /**< \brief (GMAC_IDR) PDelay Request Frame Transmitted */
#define GMAC_IDR_PDRSFT (0x1u << 25) /**< \brief (GMAC_IDR) PDelay Response Frame Transmitted */
#define GMAC_IDR_SRI (0x1u << 26) /**< \brief (GMAC_IDR) TSU Seconds Register Increment */
#define GMAC_IDR_WOL (0x1u << 28) /**< \brief (GMAC_IDR) Wake On LAN */
/* -------- GMAC_IMR : (GMAC Offset: 0x030) Interrupt Mask Register -------- */
#define GMAC_IMR_MFS (0x1u << 0) /**< \brief (GMAC_IMR) Management Frame Sent */
#define GMAC_IMR_RCOMP (0x1u << 1) /**< \brief (GMAC_IMR) Receive Complete */
#define GMAC_IMR_RXUBR (0x1u << 2) /**< \brief (GMAC_IMR) RX Used Bit Read */
#define GMAC_IMR_TXUBR (0x1u << 3) /**< \brief (GMAC_IMR) TX Used Bit Read */
#define GMAC_IMR_TUR (0x1u << 4) /**< \brief (GMAC_IMR) Transmit Under Run */
#define GMAC_IMR_RLEX (0x1u << 5) /**< \brief (GMAC_IMR) Retry Limit Exceeded */
#define GMAC_IMR_TFC (0x1u << 6) /**< \brief (GMAC_IMR) Transmit Frame Corruption due to AHB error */
#define GMAC_IMR_TCOMP (0x1u << 7) /**< \brief (GMAC_IMR) Transmit Complete */
#define GMAC_IMR_ROVR (0x1u << 10) /**< \brief (GMAC_IMR) Receive Overrun */
#define GMAC_IMR_HRESP (0x1u << 11) /**< \brief (GMAC_IMR) HRESP Not OK */
#define GMAC_IMR_PFNZ (0x1u << 12) /**< \brief (GMAC_IMR) Pause Frame with Non-zero Pause Quantum Received */
#define GMAC_IMR_PTZ (0x1u << 13) /**< \brief (GMAC_IMR) Pause Time Zero */
#define GMAC_IMR_PFTR (0x1u << 14) /**< \brief (GMAC_IMR) Pause Frame Transmitted */
#define GMAC_IMR_EXINT (0x1u << 15) /**< \brief (GMAC_IMR) External Interrupt */
#define GMAC_IMR_DRQFR (0x1u << 18) /**< \brief (GMAC_IMR) PTP Delay Request Frame Received */
#define GMAC_IMR_SFR (0x1u << 19) /**< \brief (GMAC_IMR) PTP Sync Frame Received */
#define GMAC_IMR_DRQFT (0x1u << 20) /**< \brief (GMAC_IMR) PTP Delay Request Frame Transmitted */
#define GMAC_IMR_SFT (0x1u << 21) /**< \brief (GMAC_IMR) PTP Sync Frame Transmitted */
#define GMAC_IMR_PDRQFR (0x1u << 22) /**< \brief (GMAC_IMR) PDelay Request Frame Received */
#define GMAC_IMR_PDRSFR (0x1u << 23) /**< \brief (GMAC_IMR) PDelay Response Frame Received */
#define GMAC_IMR_PDRQFT (0x1u << 24) /**< \brief (GMAC_IMR) PDelay Request Frame Transmitted */
#define GMAC_IMR_PDRSFT (0x1u << 25) /**< \brief (GMAC_IMR) PDelay Response Frame Transmitted */
/* -------- GMAC_MAN : (GMAC Offset: 0x034) PHY Maintenance Register -------- */
#define GMAC_MAN_DATA_Pos 0
#define GMAC_MAN_DATA_Msk (0xffffu << GMAC_MAN_DATA_Pos) /**< \brief (GMAC_MAN) PHY Data */
#define GMAC_MAN_DATA(value) ((GMAC_MAN_DATA_Msk & ((value) << GMAC_MAN_DATA_Pos)))
#define GMAC_MAN_WTN_Pos 16
#define GMAC_MAN_WTN_Msk (0x3u << GMAC_MAN_WTN_Pos) /**< \brief (GMAC_MAN) Write Ten */
#define GMAC_MAN_WTN(value) ((GMAC_MAN_WTN_Msk & ((value) << GMAC_MAN_WTN_Pos)))
#define GMAC_MAN_REGA_Pos 18
#define GMAC_MAN_REGA_Msk (0x1fu << GMAC_MAN_REGA_Pos) /**< \brief (GMAC_MAN) Register Address */
#define GMAC_MAN_REGA(value) ((GMAC_MAN_REGA_Msk & ((value) << GMAC_MAN_REGA_Pos)))
#define GMAC_MAN_PHYA_Pos 23
#define GMAC_MAN_PHYA_Msk (0x1fu << GMAC_MAN_PHYA_Pos) /**< \brief (GMAC_MAN) PHY Address */
#define GMAC_MAN_PHYA(value) ((GMAC_MAN_PHYA_Msk & ((value) << GMAC_MAN_PHYA_Pos)))
#define GMAC_MAN_OP_Pos 28
#define GMAC_MAN_OP_Msk (0x3u << GMAC_MAN_OP_Pos) /**< \brief (GMAC_MAN) Operation */
#define GMAC_MAN_OP(value) ((GMAC_MAN_OP_Msk & ((value) << GMAC_MAN_OP_Pos)))
#define GMAC_MAN_CLTTO (0x1u << 30) /**< \brief (GMAC_MAN) Clause 22 Operation */
#define GMAC_MAN_WZO (0x1u << 31) /**< \brief (GMAC_MAN) Write ZERO */
/* -------- GMAC_RPQ : (GMAC Offset: 0x038) Received Pause Quantum Register -------- */
#define GMAC_RPQ_RPQ_Pos 0
#define GMAC_RPQ_RPQ_Msk (0xffffu << GMAC_RPQ_RPQ_Pos) /**< \brief (GMAC_RPQ) Received Pause Quantum */
/* -------- GMAC_TPQ : (GMAC Offset: 0x03C) Transmit Pause Quantum Register -------- */
#define GMAC_TPQ_TPQ_Pos 0
#define GMAC_TPQ_TPQ_Msk (0xffffu << GMAC_TPQ_TPQ_Pos) /**< \brief (GMAC_TPQ) Transmit Pause Quantum */
#define GMAC_TPQ_TPQ(value) ((GMAC_TPQ_TPQ_Msk & ((value) << GMAC_TPQ_TPQ_Pos)))
/* -------- GMAC_HRB : (GMAC Offset: 0x080) Hash Register Bottom [31:0] -------- */
#define GMAC_HRB_ADDR_Pos 0
#define GMAC_HRB_ADDR_Msk (0xffffffffu << GMAC_HRB_ADDR_Pos) /**< \brief (GMAC_HRB) Hash Address */
#define GMAC_HRB_ADDR(value) ((GMAC_HRB_ADDR_Msk & ((value) << GMAC_HRB_ADDR_Pos)))
/* -------- GMAC_HRT : (GMAC Offset: 0x084) Hash Register Top [63:32] -------- */
#define GMAC_HRT_ADDR_Pos 0
#define GMAC_HRT_ADDR_Msk (0xffffffffu << GMAC_HRT_ADDR_Pos) /**< \brief (GMAC_HRT) Hash Address */
#define GMAC_HRT_ADDR(value) ((GMAC_HRT_ADDR_Msk & ((value) << GMAC_HRT_ADDR_Pos)))
/* -------- GMAC_SAB1 : (GMAC Offset: 0x088) Specific Address 1 Bottom [31:0] Register -------- */
#define GMAC_SAB1_ADDR_Pos 0
#define GMAC_SAB1_ADDR_Msk (0xffffffffu << GMAC_SAB1_ADDR_Pos) /**< \brief (GMAC_SAB1) Specific Address 1 */
#define GMAC_SAB1_ADDR(value) ((GMAC_SAB1_ADDR_Msk & ((value) << GMAC_SAB1_ADDR_Pos)))
/* -------- GMAC_SAT1 : (GMAC Offset: 0x08C) Specific Address 1 Top [47:32] Register -------- */
#define GMAC_SAT1_ADDR_Pos 0
#define GMAC_SAT1_ADDR_Msk (0xffffu << GMAC_SAT1_ADDR_Pos) /**< \brief (GMAC_SAT1) Specific Address 1 */
#define GMAC_SAT1_ADDR(value) ((GMAC_SAT1_ADDR_Msk & ((value) << GMAC_SAT1_ADDR_Pos)))
/* -------- GMAC_SAB2 : (GMAC Offset: 0x090) Specific Address 2 Bottom [31:0] Register -------- */
#define GMAC_SAB2_ADDR_Pos 0
#define GMAC_SAB2_ADDR_Msk (0xffffffffu << GMAC_SAB2_ADDR_Pos) /**< \brief (GMAC_SAB2) Specific Address 2 */
#define GMAC_SAB2_ADDR(value) ((GMAC_SAB2_ADDR_Msk & ((value) << GMAC_SAB2_ADDR_Pos)))
/* -------- GMAC_SAT2 : (GMAC Offset: 0x094) Specific Address 2 Top [47:32] Register -------- */
#define GMAC_SAT2_ADDR_Pos 0
#define GMAC_SAT2_ADDR_Msk (0xffffu << GMAC_SAT2_ADDR_Pos) /**< \brief (GMAC_SAT2) Specific Address 2 */
#define GMAC_SAT2_ADDR(value) ((GMAC_SAT2_ADDR_Msk & ((value) << GMAC_SAT2_ADDR_Pos)))
/* -------- GMAC_SAB3 : (GMAC Offset: 0x098) Specific Address 3 Bottom [31:0] Register -------- */
#define GMAC_SAB3_ADDR_Pos 0
#define GMAC_SAB3_ADDR_Msk (0xffffffffu << GMAC_SAB3_ADDR_Pos) /**< \brief (GMAC_SAB3) Specific Address 3 */
#define GMAC_SAB3_ADDR(value) ((GMAC_SAB3_ADDR_Msk & ((value) << GMAC_SAB3_ADDR_Pos)))
/* -------- GMAC_SAT3 : (GMAC Offset: 0x09C) Specific Address 3 Top [47:32] Register -------- */
#define GMAC_SAT3_ADDR_Pos 0
#define GMAC_SAT3_ADDR_Msk (0xffffu << GMAC_SAT3_ADDR_Pos) /**< \brief (GMAC_SAT3) Specific Address 3 */
#define GMAC_SAT3_ADDR(value) ((GMAC_SAT3_ADDR_Msk & ((value) << GMAC_SAT3_ADDR_Pos)))
/* -------- GMAC_SAB4 : (GMAC Offset: 0x0A0) Specific Address 4 Bottom [31:0] Register -------- */
#define GMAC_SAB4_ADDR_Pos 0
#define GMAC_SAB4_ADDR_Msk (0xffffffffu << GMAC_SAB4_ADDR_Pos) /**< \brief (GMAC_SAB4) Specific Address 4 */
#define GMAC_SAB4_ADDR(value) ((GMAC_SAB4_ADDR_Msk & ((value) << GMAC_SAB4_ADDR_Pos)))
/* -------- GMAC_SAT4 : (GMAC Offset: 0x0A4) Specific Address 4 Top [47:32] Register -------- */
#define GMAC_SAT4_ADDR_Pos 0
#define GMAC_SAT4_ADDR_Msk (0xffffu << GMAC_SAT4_ADDR_Pos) /**< \brief (GMAC_SAT4) Specific Address 4 */
#define GMAC_SAT4_ADDR(value) ((GMAC_SAT4_ADDR_Msk & ((value) << GMAC_SAT4_ADDR_Pos)))
/* -------- GMAC_TIDM[4] : (GMAC Offset: 0x0A8) Type ID Match 1 Register -------- */
#define GMAC_TIDM_TID_Pos 0
#define GMAC_TIDM_TID_Msk (0xffffu << GMAC_TIDM_TID_Pos) /**< \brief (GMAC_TIDM[4]) Type ID Match 1 */
#define GMAC_TIDM_TID(value) ((GMAC_TIDM_TID_Msk & ((value) << GMAC_TIDM_TID_Pos)))
/* -------- GMAC_IPGS : (GMAC Offset: 0x0BC) IPG Stretch Register -------- */
#define GMAC_IPGS_FL_Pos 0
#define GMAC_IPGS_FL_Msk (0xffffu << GMAC_IPGS_FL_Pos) /**< \brief (GMAC_IPGS) Frame Length */
#define GMAC_IPGS_FL(value) ((GMAC_IPGS_FL_Msk & ((value) << GMAC_IPGS_FL_Pos)))
/* -------- GMAC_SVLAN : (GMAC Offset: 0x0C0) Stacked VLAN Register -------- */
#define GMAC_SVLAN_VLAN_TYPE_Pos 0
#define GMAC_SVLAN_VLAN_TYPE_Msk (0xffffu << GMAC_SVLAN_VLAN_TYPE_Pos) /**< \brief (GMAC_SVLAN) User Defined VLAN_TYPE Field */
#define GMAC_SVLAN_VLAN_TYPE(value) ((GMAC_SVLAN_VLAN_TYPE_Msk & ((value) << GMAC_SVLAN_VLAN_TYPE_Pos)))
#define GMAC_SVLAN_ESVLAN (0x1u << 31) /**< \brief (GMAC_SVLAN) Enable Stacked VLAN Processing Mode */
/* -------- GMAC_TPFCP : (GMAC Offset: 0x0C4) Transmit PFC Pause Register -------- */
#define GMAC_TPFCP_PEV_Pos 0
#define GMAC_TPFCP_PEV_Msk (0xffu << GMAC_TPFCP_PEV_Pos) /**< \brief (GMAC_TPFCP) Priority Enable Vector */
#define GMAC_TPFCP_PEV(value) ((GMAC_TPFCP_PEV_Msk & ((value) << GMAC_TPFCP_PEV_Pos)))
#define GMAC_TPFCP_PQ_Pos 8
#define GMAC_TPFCP_PQ_Msk (0xffu << GMAC_TPFCP_PQ_Pos) /**< \brief (GMAC_TPFCP) Pause Quantum */
#define GMAC_TPFCP_PQ(value) ((GMAC_TPFCP_PQ_Msk & ((value) << GMAC_TPFCP_PQ_Pos)))
/* -------- GMAC_SAMB1 : (GMAC Offset: 0x0C8) Specific Address 1 Mask Bottom [31:0] Register -------- */
#define GMAC_SAMB1_ADDR_Pos 0
#define GMAC_SAMB1_ADDR_Msk (0xffffffffu << GMAC_SAMB1_ADDR_Pos) /**< \brief (GMAC_SAMB1) Specific Address 1 Mask */
#define GMAC_SAMB1_ADDR(value) ((GMAC_SAMB1_ADDR_Msk & ((value) << GMAC_SAMB1_ADDR_Pos)))
/* -------- GMAC_SAMT1 : (GMAC Offset: 0x0CC) Specific Address 1 Mask Top [47:32] Register -------- */
#define GMAC_SAMT1_ADDR_Pos 0
#define GMAC_SAMT1_ADDR_Msk (0xffffu << GMAC_SAMT1_ADDR_Pos) /**< \brief (GMAC_SAMT1) Specific Address 1 Mask */
#define GMAC_SAMT1_ADDR(value) ((GMAC_SAMT1_ADDR_Msk & ((value) << GMAC_SAMT1_ADDR_Pos)))
/* -------- GMAC_OTLO : (GMAC Offset: 0x100) Octets Transmitted [31:0] Register -------- */
#define GMAC_OTLO_TXO_Pos 0
#define GMAC_OTLO_TXO_Msk (0xffffffffu << GMAC_OTLO_TXO_Pos) /**< \brief (GMAC_OTLO) Transmitted Octets */
/* -------- GMAC_OTHI : (GMAC Offset: 0x104) Octets Transmitted [47:32] Register -------- */
#define GMAC_OTHI_TXO_Pos 0
#define GMAC_OTHI_TXO_Msk (0xffffu << GMAC_OTHI_TXO_Pos) /**< \brief (GMAC_OTHI) Transmitted Octets */
/* -------- GMAC_FT : (GMAC Offset: 0x108) Frames Transmitted Register -------- */
#define GMAC_FT_FTX_Pos 0
#define GMAC_FT_FTX_Msk (0xffffffffu << GMAC_FT_FTX_Pos) /**< \brief (GMAC_FT) Frames Transmitted without Error */
/* -------- GMAC_BCFT : (GMAC Offset: 0x10C) Broadcast Frames Transmitted Register -------- */
#define GMAC_BCFT_BFTX_Pos 0
#define GMAC_BCFT_BFTX_Msk (0xffffffffu << GMAC_BCFT_BFTX_Pos) /**< \brief (GMAC_BCFT) Broadcast Frames Transmitted without Error */
/* -------- GMAC_MFT : (GMAC Offset: 0x110) Multicast Frames Transmitted Register -------- */
#define GMAC_MFT_MFTX_Pos 0
#define GMAC_MFT_MFTX_Msk (0xffffffffu << GMAC_MFT_MFTX_Pos) /**< \brief (GMAC_MFT) Multicast Frames Transmitted without Error */
/* -------- GMAC_PFT : (GMAC Offset: 0x114) Pause Frames Transmitted Register -------- */
#define GMAC_PFT_PFTX_Pos 0
#define GMAC_PFT_PFTX_Msk (0xffffu << GMAC_PFT_PFTX_Pos) /**< \brief (GMAC_PFT) Pause Frames Transmitted Register */
/* -------- GMAC_BFT64 : (GMAC Offset: 0x118) 64 Byte Frames Transmitted Register -------- */
#define GMAC_BFT64_NFTX_Pos 0
#define GMAC_BFT64_NFTX_Msk (0xffffffffu << GMAC_BFT64_NFTX_Pos) /**< \brief (GMAC_BFT64) 64 Byte Frames Transmitted without Error */
/* -------- GMAC_TBFT127 : (GMAC Offset: 0x11C) 65 to 127 Byte Frames Transmitted Register -------- */
#define GMAC_TBFT127_NFTX_Pos 0
#define GMAC_TBFT127_NFTX_Msk (0xffffffffu << GMAC_TBFT127_NFTX_Pos) /**< \brief (GMAC_TBFT127) 65 to 127 Byte Frames Transmitted without Error */
/* -------- GMAC_TBFT255 : (GMAC Offset: 0x120) 128 to 255 Byte Frames Transmitted Register -------- */
#define GMAC_TBFT255_NFTX_Pos 0
#define GMAC_TBFT255_NFTX_Msk (0xffffffffu << GMAC_TBFT255_NFTX_Pos) /**< \brief (GMAC_TBFT255) 128 to 255 Byte Frames Transmitted without Error */
/* -------- GMAC_TBFT511 : (GMAC Offset: 0x124) 256 to 511 Byte Frames Transmitted Register -------- */
#define GMAC_TBFT511_NFTX_Pos 0
#define GMAC_TBFT511_NFTX_Msk (0xffffffffu << GMAC_TBFT511_NFTX_Pos) /**< \brief (GMAC_TBFT511) 256 to 511 Byte Frames Transmitted without Error */
/* -------- GMAC_TBFT1023 : (GMAC Offset: 0x128) 512 to 1023 Byte Frames Transmitted Register -------- */
#define GMAC_TBFT1023_NFTX_Pos 0
#define GMAC_TBFT1023_NFTX_Msk (0xffffffffu << GMAC_TBFT1023_NFTX_Pos) /**< \brief (GMAC_TBFT1023) 512 to 1023 Byte Frames Transmitted without Error */
/* -------- GMAC_TBFT1518 : (GMAC Offset: 0x12C) 1024 to 1518 Byte Frames Transmitted Register -------- */
#define GMAC_TBFT1518_NFTX_Pos 0
#define GMAC_TBFT1518_NFTX_Msk (0xffffffffu << GMAC_TBFT1518_NFTX_Pos) /**< \brief (GMAC_TBFT1518) 1024 to 1518 Byte Frames Transmitted without Error */
/* -------- GMAC_GTBFT1518 : (GMAC Offset: 0x130) Greater Than 1518 Byte Frames Transmitted Register -------- */
#define GMAC_GTBFT1518_NFTX_Pos 0
#define GMAC_GTBFT1518_NFTX_Msk (0xffffffffu << GMAC_GTBFT1518_NFTX_Pos) /**< \brief (GMAC_GTBFT1518) Greater than 1518 Byte Frames Transmitted without Error */
/* -------- GMAC_TUR : (GMAC Offset: 0x134) Transmit Under Runs Register -------- */
#define GMAC_TUR_TXUNR_Pos 0
#define GMAC_TUR_TXUNR_Msk (0x3ffu << GMAC_TUR_TXUNR_Pos) /**< \brief (GMAC_TUR) Transmit Under Runs */
/* -------- GMAC_SCF : (GMAC Offset: 0x138) Single Collision Frames Register -------- */
#define GMAC_SCF_SCOL_Pos 0
#define GMAC_SCF_SCOL_Msk (0x3ffffu << GMAC_SCF_SCOL_Pos) /**< \brief (GMAC_SCF) Single Collision */
/* -------- GMAC_MCF : (GMAC Offset: 0x13C) Multiple Collision Frames Register -------- */
#define GMAC_MCF_MCOL_Pos 0
#define GMAC_MCF_MCOL_Msk (0x3ffffu << GMAC_MCF_MCOL_Pos) /**< \brief (GMAC_MCF) Multiple Collision */
/* -------- GMAC_EC : (GMAC Offset: 0x140) Excessive Collisions Register -------- */
#define GMAC_EC_XCOL_Pos 0
#define GMAC_EC_XCOL_Msk (0x3ffu << GMAC_EC_XCOL_Pos) /**< \brief (GMAC_EC) Excessive Collisions */
/* -------- GMAC_LC : (GMAC Offset: 0x144) Late Collisions Register -------- */
#define GMAC_LC_LCOL_Pos 0
#define GMAC_LC_LCOL_Msk (0x3ffu << GMAC_LC_LCOL_Pos) /**< \brief (GMAC_LC) Late Collisions */
/* -------- GMAC_DTF : (GMAC Offset: 0x148) Deferred Transmission Frames Register -------- */
#define GMAC_DTF_DEFT_Pos 0
#define GMAC_DTF_DEFT_Msk (0x3ffffu << GMAC_DTF_DEFT_Pos) /**< \brief (GMAC_DTF) Deferred Transmission */
/* -------- GMAC_CSE : (GMAC Offset: 0x14C) Carrier Sense Errors Register -------- */
#define GMAC_CSE_CSR_Pos 0
#define GMAC_CSE_CSR_Msk (0x3ffu << GMAC_CSE_CSR_Pos) /**< \brief (GMAC_CSE) Carrier Sense Error */
/* -------- GMAC_ORLO : (GMAC Offset: 0x150) Octets Received [31:0] Received -------- */
#define GMAC_ORLO_RXO_Pos 0
#define GMAC_ORLO_RXO_Msk (0xffffffffu << GMAC_ORLO_RXO_Pos) /**< \brief (GMAC_ORLO) Received Octets */
/* -------- GMAC_ORHI : (GMAC Offset: 0x154) Octets Received [47:32] Received -------- */
#define GMAC_ORHI_RXO_Pos 0
#define GMAC_ORHI_RXO_Msk (0xffffu << GMAC_ORHI_RXO_Pos) /**< \brief (GMAC_ORHI) Received Octets */
/* -------- GMAC_FR : (GMAC Offset: 0x158) Frames Received Register -------- */
#define GMAC_FR_FRX_Pos 0
#define GMAC_FR_FRX_Msk (0xffffffffu << GMAC_FR_FRX_Pos) /**< \brief (GMAC_FR) Frames Received without Error */
/* -------- GMAC_BCFR : (GMAC Offset: 0x15C) Broadcast Frames Received Register -------- */
#define GMAC_BCFR_BFRX_Pos 0
#define GMAC_BCFR_BFRX_Msk (0xffffffffu << GMAC_BCFR_BFRX_Pos) /**< \brief (GMAC_BCFR) Broadcast Frames Received without Error */
/* -------- GMAC_MFR : (GMAC Offset: 0x160) Multicast Frames Received Register -------- */
#define GMAC_MFR_MFRX_Pos 0
#define GMAC_MFR_MFRX_Msk (0xffffffffu << GMAC_MFR_MFRX_Pos) /**< \brief (GMAC_MFR) Multicast Frames Received without Error */
/* -------- GMAC_PFR : (GMAC Offset: 0x164) Pause Frames Received Register -------- */
#define GMAC_PFR_PFRX_Pos 0
#define GMAC_PFR_PFRX_Msk (0xffffu << GMAC_PFR_PFRX_Pos) /**< \brief (GMAC_PFR) Pause Frames Received Register */
/* -------- GMAC_BFR64 : (GMAC Offset: 0x168) 64 Byte Frames Received Register -------- */
#define GMAC_BFR64_NFRX_Pos 0
#define GMAC_BFR64_NFRX_Msk (0xffffffffu << GMAC_BFR64_NFRX_Pos) /**< \brief (GMAC_BFR64) 64 Byte Frames Received without Error */
/* -------- GMAC_TBFR127 : (GMAC Offset: 0x16C) 65 to 127 Byte Frames Received Register -------- */
#define GMAC_TBFR127_NFRX_Pos 0
#define GMAC_TBFR127_NFRX_Msk (0xffffffffu << GMAC_TBFR127_NFRX_Pos) /**< \brief (GMAC_TBFR127) 65 to 127 Byte Frames Received without Error */
/* -------- GMAC_TBFR255 : (GMAC Offset: 0x170) 128 to 255 Byte Frames Received Register -------- */
#define GMAC_TBFR255_NFRX_Pos 0
#define GMAC_TBFR255_NFRX_Msk (0xffffffffu << GMAC_TBFR255_NFRX_Pos) /**< \brief (GMAC_TBFR255) 128 to 255 Byte Frames Received without Error */
/* -------- GMAC_TBFR511 : (GMAC Offset: 0x174) 256 to 511Byte Frames Received Register -------- */
#define GMAC_TBFR511_NFRX_Pos 0
#define GMAC_TBFR511_NFRX_Msk (0xffffffffu << GMAC_TBFR511_NFRX_Pos) /**< \brief (GMAC_TBFR511) 256 to 511 Byte Frames Received without Error */
/* -------- GMAC_TBFR1023 : (GMAC Offset: 0x178) 512 to 1023 Byte Frames Received Register -------- */
#define GMAC_TBFR1023_NFRX_Pos 0
#define GMAC_TBFR1023_NFRX_Msk (0xffffffffu << GMAC_TBFR1023_NFRX_Pos) /**< \brief (GMAC_TBFR1023) 512 to 1023 Byte Frames Received without Error */
/* -------- GMAC_TBFR1518 : (GMAC Offset: 0x17C) 1024 to 1518 Byte Frames Received Register -------- */
#define GMAC_TBFR1518_NFRX_Pos 0
#define GMAC_TBFR1518_NFRX_Msk (0xffffffffu << GMAC_TBFR1518_NFRX_Pos) /**< \brief (GMAC_TBFR1518) 1024 to 1518 Byte Frames Received without Error */
/* -------- GMAC_TMXBFR : (GMAC Offset: 0x180) 1519 to Maximum Byte Frames Received Register -------- */
#define GMAC_TMXBFR_NFRX_Pos 0
#define GMAC_TMXBFR_NFRX_Msk (0xffffffffu << GMAC_TMXBFR_NFRX_Pos) /**< \brief (GMAC_TMXBFR) 1519 to Maximum Byte Frames Received without Error */
/* -------- GMAC_UFR : (GMAC Offset: 0x184) Undersize Frames Received Register -------- */
#define GMAC_UFR_UFRX_Pos 0
#define GMAC_UFR_UFRX_Msk (0x3ffu << GMAC_UFR_UFRX_Pos) /**< \brief (GMAC_UFR) Undersize Frames Received */
/* -------- GMAC_OFR : (GMAC Offset: 0x188) Oversize Frames Received Register -------- */
#define GMAC_OFR_OFRX_Pos 0
#define GMAC_OFR_OFRX_Msk (0x3ffu << GMAC_OFR_OFRX_Pos) /**< \brief (GMAC_OFR) Oversized Frames Received */
/* -------- GMAC_JR : (GMAC Offset: 0x18C) Jabbers Received Register -------- */
#define GMAC_JR_JRX_Pos 0
#define GMAC_JR_JRX_Msk (0x3ffu << GMAC_JR_JRX_Pos) /**< \brief (GMAC_JR) Jabbers Received */
/* -------- GMAC_FCSE : (GMAC Offset: 0x190) Frame Check Sequence Errors Register -------- */
#define GMAC_FCSE_FCKR_Pos 0
#define GMAC_FCSE_FCKR_Msk (0x3ffu << GMAC_FCSE_FCKR_Pos) /**< \brief (GMAC_FCSE) Frame Check Sequence Errors */
/* -------- GMAC_LFFE : (GMAC Offset: 0x194) Length Field Frame Errors Register -------- */
#define GMAC_LFFE_LFER_Pos 0
#define GMAC_LFFE_LFER_Msk (0x3ffu << GMAC_LFFE_LFER_Pos) /**< \brief (GMAC_LFFE) Length Field Frame Errors */
/* -------- GMAC_RSE : (GMAC Offset: 0x198) Receive Symbol Errors Register -------- */
#define GMAC_RSE_RXSE_Pos 0
#define GMAC_RSE_RXSE_Msk (0x3ffu << GMAC_RSE_RXSE_Pos) /**< \brief (GMAC_RSE) Receive Symbol Errors */
/* -------- GMAC_AE : (GMAC Offset: 0x19C) Alignment Errors Register -------- */
#define GMAC_AE_AER_Pos 0
#define GMAC_AE_AER_Msk (0x3ffu << GMAC_AE_AER_Pos) /**< \brief (GMAC_AE) Alignment Errors */
/* -------- GMAC_RRE : (GMAC Offset: 0x1A0) Receive Resource Errors Register -------- */
#define GMAC_RRE_RXRER_Pos 0
#define GMAC_RRE_RXRER_Msk (0x3ffffu << GMAC_RRE_RXRER_Pos) /**< \brief (GMAC_RRE) Receive Resource Errors */
/* -------- GMAC_ROE : (GMAC Offset: 0x1A4) Receive Overrun Register -------- */
#define GMAC_ROE_RXOVR_Pos 0
#define GMAC_ROE_RXOVR_Msk (0x3ffu << GMAC_ROE_RXOVR_Pos) /**< \brief (GMAC_ROE) Receive Overruns */
/* -------- GMAC_IHCE : (GMAC Offset: 0x1A8) IP Header Checksum Errors Register -------- */
#define GMAC_IHCE_HCKER_Pos 0
#define GMAC_IHCE_HCKER_Msk (0xffu << GMAC_IHCE_HCKER_Pos) /**< \brief (GMAC_IHCE) IP Header Checksum Errors */
/* -------- GMAC_TCE : (GMAC Offset: 0x1AC) TCP Checksum Errors Register -------- */
#define GMAC_TCE_TCKER_Pos 0
#define GMAC_TCE_TCKER_Msk (0xffu << GMAC_TCE_TCKER_Pos) /**< \brief (GMAC_TCE) TCP Checksum Errors */
/* -------- GMAC_UCE : (GMAC Offset: 0x1B0) UDP Checksum Errors Register -------- */
#define GMAC_UCE_UCKER_Pos 0
#define GMAC_UCE_UCKER_Msk (0xffu << GMAC_UCE_UCKER_Pos) /**< \brief (GMAC_UCE) UDP Checksum Errors */
/* -------- GMAC_TSSS : (GMAC Offset: 0x1C8) 1588 Timer Sync Strobe Seconds Register -------- */
#define GMAC_TSSS_VTS_Pos 0
#define GMAC_TSSS_VTS_Msk (0xffffffffu << GMAC_TSSS_VTS_Pos) /**< \brief (GMAC_TSSS) Value of Timer Seconds Register Capture */
#define GMAC_TSSS_VTS(value) ((GMAC_TSSS_VTS_Msk & ((value) << GMAC_TSSS_VTS_Pos)))
/* -------- GMAC_TSSN : (GMAC Offset: 0x1CC) 1588 Timer Sync Strobe Nanoseconds Register -------- */
#define GMAC_TSSN_VTN_Pos 0
#define GMAC_TSSN_VTN_Msk (0x3fffffffu << GMAC_TSSN_VTN_Pos) /**< \brief (GMAC_TSSN) Value Timer Nanoseconds Register Capture */
#define GMAC_TSSN_VTN(value) ((GMAC_TSSN_VTN_Msk & ((value) << GMAC_TSSN_VTN_Pos)))
/* -------- GMAC_TS : (GMAC Offset: 0x1D0) 1588 Timer Seconds Register -------- */
#define GMAC_TS_TCS_Pos 0
#define GMAC_TS_TCS_Msk (0xffffffffu << GMAC_TS_TCS_Pos) /**< \brief (GMAC_TS) Timer Count in Seconds */
#define GMAC_TS_TCS(value) ((GMAC_TS_TCS_Msk & ((value) << GMAC_TS_TCS_Pos)))
/* -------- GMAC_TN : (GMAC Offset: 0x1D4) 1588 Timer Nanoseconds Register -------- */
#define GMAC_TN_TNS_Pos 0
#define GMAC_TN_TNS_Msk (0x3fffffffu << GMAC_TN_TNS_Pos) /**< \brief (GMAC_TN) Timer Count in Nanoseconds */
#define GMAC_TN_TNS(value) ((GMAC_TN_TNS_Msk & ((value) << GMAC_TN_TNS_Pos)))
/* -------- GMAC_TA : (GMAC Offset: 0x1D8) 1588 Timer Adjust Register -------- */
#define GMAC_TA_ITDT_Pos 0
#define GMAC_TA_ITDT_Msk (0x3fffffffu << GMAC_TA_ITDT_Pos) /**< \brief (GMAC_TA) Increment/Decrement */
#define GMAC_TA_ITDT(value) ((GMAC_TA_ITDT_Msk & ((value) << GMAC_TA_ITDT_Pos)))
#define GMAC_TA_ADJ (0x1u << 31) /**< \brief (GMAC_TA) Adjust 1588 Timer */
/* -------- GMAC_TI : (GMAC Offset: 0x1DC) 1588 Timer Increment Register -------- */
#define GMAC_TI_CNS_Pos 0
#define GMAC_TI_CNS_Msk (0xffu << GMAC_TI_CNS_Pos) /**< \brief (GMAC_TI) Count Nanoseconds */
#define GMAC_TI_CNS(value) ((GMAC_TI_CNS_Msk & ((value) << GMAC_TI_CNS_Pos)))
#define GMAC_TI_ACNS_Pos 8
#define GMAC_TI_ACNS_Msk (0xffu << GMAC_TI_ACNS_Pos) /**< \brief (GMAC_TI) Alternative Count Nanoseconds */
#define GMAC_TI_ACNS(value) ((GMAC_TI_ACNS_Msk & ((value) << GMAC_TI_ACNS_Pos)))
#define GMAC_TI_NIT_Pos 16
#define GMAC_TI_NIT_Msk (0xffu << GMAC_TI_NIT_Pos) /**< \brief (GMAC_TI) Number of Increments */
#define GMAC_TI_NIT(value) ((GMAC_TI_NIT_Msk & ((value) << GMAC_TI_NIT_Pos)))
/* -------- GMAC_EFTS : (GMAC Offset: 0x1E0) PTP Event Frame Transmitted Seconds -------- */
#define GMAC_EFTS_RUD_Pos 0
#define GMAC_EFTS_RUD_Msk (0xffffffffu << GMAC_EFTS_RUD_Pos) /**< \brief (GMAC_EFTS) Register Update */
/* -------- GMAC_EFTN : (GMAC Offset: 0x1E4) PTP Event Frame Transmitted Nanoseconds -------- */
#define GMAC_EFTN_RUD_Pos 0
#define GMAC_EFTN_RUD_Msk (0x3fffffffu << GMAC_EFTN_RUD_Pos) /**< \brief (GMAC_EFTN) Register Update */
/* -------- GMAC_EFRS : (GMAC Offset: 0x1E8) PTP Event Frame Received Seconds -------- */
#define GMAC_EFRS_RUD_Pos 0
#define GMAC_EFRS_RUD_Msk (0xffffffffu << GMAC_EFRS_RUD_Pos) /**< \brief (GMAC_EFRS) Register Update */
/* -------- GMAC_EFRN : (GMAC Offset: 0x1EC) PTP Event Frame Received Nanoseconds -------- */
#define GMAC_EFRN_RUD_Pos 0
#define GMAC_EFRN_RUD_Msk (0x3fffffffu << GMAC_EFRN_RUD_Pos) /**< \brief (GMAC_EFRN) Register Update */
/* -------- GMAC_PEFTS : (GMAC Offset: 0x1F0) PTP Peer Event Frame Transmitted Seconds -------- */
#define GMAC_PEFTS_RUD_Pos 0
#define GMAC_PEFTS_RUD_Msk (0xffffffffu << GMAC_PEFTS_RUD_Pos) /**< \brief (GMAC_PEFTS) Register Update */
/* -------- GMAC_PEFTN : (GMAC Offset: 0x1F4) PTP Peer Event Frame Transmitted Nanoseconds -------- */
#define GMAC_PEFTN_RUD_Pos 0
#define GMAC_PEFTN_RUD_Msk (0x3fffffffu << GMAC_PEFTN_RUD_Pos) /**< \brief (GMAC_PEFTN) Register Update */
/* -------- GMAC_PEFRS : (GMAC Offset: 0x1F8) PTP Peer Event Frame Received Seconds -------- */
#define GMAC_PEFRS_RUD_Pos 0
#define GMAC_PEFRS_RUD_Msk (0xffffffffu << GMAC_PEFRS_RUD_Pos) /**< \brief (GMAC_PEFRS) Register Update */
/* -------- GMAC_PEFRN : (GMAC Offset: 0x1FC) PTP Peer Event Frame Received Nanoseconds -------- */
#define GMAC_PEFRN_RUD_Pos 0
#define GMAC_PEFRN_RUD_Msk (0x3fffffffu << GMAC_PEFRN_RUD_Pos) /**< \brief (GMAC_PEFRN) Register Update */

/*@}*/


#endif /* _SAM4E_GMAC_COMPONENT_ */
