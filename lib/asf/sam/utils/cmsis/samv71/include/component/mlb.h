/**
 * \file
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef _SAMV71_MLB_COMPONENT_
#define _SAMV71_MLB_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Media LB */
/* ============================================================================= */
/** \addtogroup SAMV71_MLB Media LB */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Mlb hardware registers */
typedef struct {
  __IO uint32_t MLB_MLBC0;       /**< \brief (Mlb Offset: 0x000) MediaLB Control 0 Register */
  __I  uint32_t Reserved1[2];
  __IO uint32_t MLB_MS0;         /**< \brief (Mlb Offset: 0x00C) MediaLB Channel Status 0 Register */
  __I  uint32_t Reserved2[1];
  __IO uint32_t MLB_MS1;         /**< \brief (Mlb Offset: 0x014) MediaLB Channel Status1 Register */
  __I  uint32_t Reserved3[2];
  __IO uint32_t MLB_MSS;         /**< \brief (Mlb Offset: 0x020) MediaLB System Status Register */
  __I  uint32_t MLB_MSD;         /**< \brief (Mlb Offset: 0x024) MediaLB System Data Register */
  __I  uint32_t Reserved4[1];
  __IO uint32_t MLB_MIEN;        /**< \brief (Mlb Offset: 0x02C) MediaLB Interrupt Enable Register */
  __I  uint32_t Reserved5[3];
  __IO uint32_t MLB_MLBC1;       /**< \brief (Mlb Offset: 0x03C) MediaLB Control 1 Register */
  __I  uint32_t Reserved6[1];
  __I  uint32_t Reserved7[15];
  __IO uint32_t MLB_HCTL;        /**< \brief (Mlb Offset: 0x080) HBI Control Register */
  __I  uint32_t Reserved8[1];
  __IO uint32_t MLB_HCMR[2];     /**< \brief (Mlb Offset: 0x088) HBI Channel Mask 0 Register */
  __I  uint32_t MLB_HCER[2];     /**< \brief (Mlb Offset: 0x090) HBI Channel Error 0 Register */
  __I  uint32_t MLB_HCBR[2];     /**< \brief (Mlb Offset: 0x098) HBI Channel Busy 0 Register */
  __I  uint32_t Reserved9[8];
  __IO uint32_t MLB_MDAT[4];     /**< \brief (Mlb Offset: 0x0C0) MIF Data 0 Register */
  __IO uint32_t MLB_MDWE[4];     /**< \brief (Mlb Offset: 0x0D0) MIF Data Write Enable 0 Register */
  __IO uint32_t MLB_MCTL;        /**< \brief (Mlb Offset: 0x0E0) MIF Control Register */
  __IO uint32_t MLB_MADR;        /**< \brief (Mlb Offset: 0x0E4) MIF Address Register */
  __I  uint32_t Reserved10[182];
  __IO uint32_t MLB_ACTL;        /**< \brief (Mlb Offset: 0x3C0) AHB Control Register */
  __I  uint32_t Reserved11[3];
  __IO uint32_t MLB_ACSR[2];     /**< \brief (Mlb Offset: 0x3D0) AHB Channel Status 0 Register */
  __IO uint32_t MLB_ACMR[2];     /**< \brief (Mlb Offset: 0x3D8) AHB Channel Mask 0 Register */
} Mlb;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- MLB_MLBC0 : (MLB Offset: 0x000) MediaLB Control 0 Register -------- */
#define MLB_MLBC0_MLBEN (0x1u << 0) /**< \brief (MLB_MLBC0) MediaLB Enable */
#define MLB_MLBC0_MLBCLK_Pos 2
#define MLB_MLBC0_MLBCLK_Msk (0x7u << MLB_MLBC0_MLBCLK_Pos) /**< \brief (MLB_MLBC0) MLB_CLK (MediaLB clock) speed select */
#define MLB_MLBC0_MLBCLK(value) ((MLB_MLBC0_MLBCLK_Msk & ((value) << MLB_MLBC0_MLBCLK_Pos)))
#define   MLB_MLBC0_MLBCLK_256_FS (0x0u << 2) /**< \brief (MLB_MLBC0) 256xFs (for MLBPEN = 0) */
#define   MLB_MLBC0_MLBCLK_512_FS (0x1u << 2) /**< \brief (MLB_MLBC0) 512xFs (for MLBPEN = 0) */
#define   MLB_MLBC0_MLBCLK_1024_FS (0x2u << 2) /**< \brief (MLB_MLBC0) 1024xFs (for MLBPEN = 0) */
#define MLB_MLBC0_ZERO (0x1u << 5) /**< \brief (MLB_MLBC0) Must be Written to 0 */
#define MLB_MLBC0_MLBLK (0x1u << 7) /**< \brief (MLB_MLBC0) MediaLB Lock Status (read-only) */
#define MLB_MLBC0_ASYRETRY (0x1u << 12) /**< \brief (MLB_MLBC0) Asynchronous Tx Packet Retry */
#define MLB_MLBC0_CTLRETRY (0x1u << 14) /**< \brief (MLB_MLBC0) Control Tx Packet Retry */
#define MLB_MLBC0_FCNT_Pos 15
#define MLB_MLBC0_FCNT_Msk (0x7u << MLB_MLBC0_FCNT_Pos) /**< \brief (MLB_MLBC0) The number of frames per sub-buffer for synchronous channels */
#define MLB_MLBC0_FCNT(value) ((MLB_MLBC0_FCNT_Msk & ((value) << MLB_MLBC0_FCNT_Pos)))
#define   MLB_MLBC0_FCNT_1_FRAME (0x0u << 15) /**< \brief (MLB_MLBC0) 1 frame per sub-buffer (Operation is the same as Standard mode.) */
#define   MLB_MLBC0_FCNT_2_FRAMES (0x1u << 15) /**< \brief (MLB_MLBC0) 2 frames per sub-buffer */
#define   MLB_MLBC0_FCNT_4_FRAMES (0x2u << 15) /**< \brief (MLB_MLBC0) 4 frames per sub-buffer */
#define   MLB_MLBC0_FCNT_8_FRAMES (0x3u << 15) /**< \brief (MLB_MLBC0) 8 frames per sub-buffer */
#define   MLB_MLBC0_FCNT_16_FRAMES (0x4u << 15) /**< \brief (MLB_MLBC0) 16 frames per sub-buffer */
#define   MLB_MLBC0_FCNT_32_FRAMES (0x5u << 15) /**< \brief (MLB_MLBC0) 32 frames per sub-buffer */
#define   MLB_MLBC0_FCNT_64_FRAMES (0x6u << 15) /**< \brief (MLB_MLBC0) 64 frames per sub-buffer */
/* -------- MLB_MS0 : (MLB Offset: 0x00C) MediaLB Channel Status 0 Register -------- */
#define MLB_MS0_MCS_Pos 0
#define MLB_MS0_MCS_Msk (0xffffffffu << MLB_MS0_MCS_Pos) /**< \brief (MLB_MS0) MediaLB Channel Status [31:0] (cleared by writing a 0) */
#define MLB_MS0_MCS(value) ((MLB_MS0_MCS_Msk & ((value) << MLB_MS0_MCS_Pos)))
/* -------- MLB_MS1 : (MLB Offset: 0x014) MediaLB Channel Status1 Register -------- */
#define MLB_MS1_MCS_Pos 0
#define MLB_MS1_MCS_Msk (0xffffffffu << MLB_MS1_MCS_Pos) /**< \brief (MLB_MS1) MediaLB Channel Status [63:32] (cleared by writing a 0) */
#define MLB_MS1_MCS(value) ((MLB_MS1_MCS_Msk & ((value) << MLB_MS1_MCS_Pos)))
/* -------- MLB_MSS : (MLB Offset: 0x020) MediaLB System Status Register -------- */
#define MLB_MSS_RSTSYSCMD (0x1u << 0) /**< \brief (MLB_MSS) Reset System Command Detected in the System Quadlet (cleared by writing a 0) */
#define MLB_MSS_LKSYSCMD (0x1u << 1) /**< \brief (MLB_MSS) Network Lock System Command Detected in the System Quadlet (cleared by writing a 0) */
#define MLB_MSS_ULKSYSCMD (0x1u << 2) /**< \brief (MLB_MSS) Network Unlock System Command Detected in the System Quadlet (cleared by writing a 0) */
#define MLB_MSS_CSSYSCMD (0x1u << 3) /**< \brief (MLB_MSS) Channel Scan System Command Detected in the System Quadlet (cleared by writing a 0) */
#define MLB_MSS_SWSYSCMD (0x1u << 4) /**< \brief (MLB_MSS) Software System Command Detected in the System Quadlet (cleared by writing a 0) */
#define MLB_MSS_SERVREQ (0x1u << 5) /**< \brief (MLB_MSS) Service Request Enabled */
/* -------- MLB_MSD : (MLB Offset: 0x024) MediaLB System Data Register -------- */
#define MLB_MSD_SD0_Pos 0
#define MLB_MSD_SD0_Msk (0xffu << MLB_MSD_SD0_Pos) /**< \brief (MLB_MSD) System Data (Byte 0) */
#define MLB_MSD_SD1_Pos 8
#define MLB_MSD_SD1_Msk (0xffu << MLB_MSD_SD1_Pos) /**< \brief (MLB_MSD) System Data (Byte 1) */
#define MLB_MSD_SD2_Pos 16
#define MLB_MSD_SD2_Msk (0xffu << MLB_MSD_SD2_Pos) /**< \brief (MLB_MSD) System Data (Byte 2) */
#define MLB_MSD_SD3_Pos 24
#define MLB_MSD_SD3_Msk (0xffu << MLB_MSD_SD3_Pos) /**< \brief (MLB_MSD) System Data (Byte 3) */
/* -------- MLB_MIEN : (MLB Offset: 0x02C) MediaLB Interrupt Enable Register -------- */
#define MLB_MIEN_ISOC_PE (0x1u << 0) /**< \brief (MLB_MIEN) Isochronous Rx Protocol Error Enable */
#define MLB_MIEN_ISOC_BUFO (0x1u << 1) /**< \brief (MLB_MIEN) Isochronous Rx Buffer Overflow Enable */
#define MLB_MIEN_SYNC_PE (0x1u << 16) /**< \brief (MLB_MIEN) Synchronous Protocol Error Enable */
#define MLB_MIEN_ARX_DONE (0x1u << 17) /**< \brief (MLB_MIEN) Asynchronous Rx Done Enable */
#define MLB_MIEN_ARX_PE (0x1u << 18) /**< \brief (MLB_MIEN) Asynchronous Rx Protocol Error Enable */
#define MLB_MIEN_ARX_BREAK (0x1u << 19) /**< \brief (MLB_MIEN) Asynchronous Rx Break Enable */
#define MLB_MIEN_ATX_DONE (0x1u << 20) /**< \brief (MLB_MIEN) Asynchronous Tx Packet Done Enable */
#define MLB_MIEN_ATX_PE (0x1u << 21) /**< \brief (MLB_MIEN) Asynchronous Tx Protocol Error Enable */
#define MLB_MIEN_ATX_BREAK (0x1u << 22) /**< \brief (MLB_MIEN) Asynchronous Tx Break Enable */
#define MLB_MIEN_CRX_DONE (0x1u << 24) /**< \brief (MLB_MIEN) Control Rx Packet Done Enable */
#define MLB_MIEN_CRX_PE (0x1u << 25) /**< \brief (MLB_MIEN) Control Rx Protocol Error Enable */
#define MLB_MIEN_CRX_BREAK (0x1u << 26) /**< \brief (MLB_MIEN) Control Rx Break Enable */
#define MLB_MIEN_CTX_DONE (0x1u << 27) /**< \brief (MLB_MIEN) Control Tx Packet Done Enable */
#define MLB_MIEN_CTX_PE (0x1u << 28) /**< \brief (MLB_MIEN) Control Tx Protocol Error Enable */
#define MLB_MIEN_CTX_BREAK (0x1u << 29) /**< \brief (MLB_MIEN) Control Tx Break Enable */
/* -------- MLB_MLBC1 : (MLB Offset: 0x03C) MediaLB Control 1 Register -------- */
#define MLB_MLBC1_LOCK (0x1u << 6) /**< \brief (MLB_MLBC1) MediaLB Lock Error Status (cleared by writing a 0) */
#define MLB_MLBC1_CLKM (0x1u << 7) /**< \brief (MLB_MLBC1) MediaLB Clock Missing Status (cleared by writing a 0) */
#define MLB_MLBC1_NDA_Pos 8
#define MLB_MLBC1_NDA_Msk (0xffu << MLB_MLBC1_NDA_Pos) /**< \brief (MLB_MLBC1) Node Device Address */
#define MLB_MLBC1_NDA(value) ((MLB_MLBC1_NDA_Msk & ((value) << MLB_MLBC1_NDA_Pos)))
/* -------- MLB_HCTL : (MLB Offset: 0x080) HBI Control Register -------- */
#define MLB_HCTL_RST0 (0x1u << 0) /**< \brief (MLB_HCTL) Address Generation Unit 0 Software Reset */
#define MLB_HCTL_RST1 (0x1u << 1) /**< \brief (MLB_HCTL) Address Generation Unit 1 Software Reset */
#define MLB_HCTL_EN (0x1u << 15) /**< \brief (MLB_HCTL) HBI Enable */
/* -------- MLB_HCMR[2] : (MLB Offset: 0x088) HBI Channel Mask 0 Register -------- */
#define MLB_HCMR_CHM_Pos 0
#define MLB_HCMR_CHM_Msk (0xffffffffu << MLB_HCMR_CHM_Pos) /**< \brief (MLB_HCMR[2]) Bitwise Channel Mask Bit [31:0] */
#define MLB_HCMR_CHM(value) ((MLB_HCMR_CHM_Msk & ((value) << MLB_HCMR_CHM_Pos)))
/* -------- MLB_HCER[2] : (MLB Offset: 0x090) HBI Channel Error 0 Register -------- */
#define MLB_HCER_CERR_Pos 0
#define MLB_HCER_CERR_Msk (0xffffffffu << MLB_HCER_CERR_Pos) /**< \brief (MLB_HCER[2]) Bitwise Channel Error Bit [31:0] */
/* -------- MLB_HCBR[2] : (MLB Offset: 0x098) HBI Channel Busy 0 Register -------- */
#define MLB_HCBR_CHB_Pos 0
#define MLB_HCBR_CHB_Msk (0xffffffffu << MLB_HCBR_CHB_Pos) /**< \brief (MLB_HCBR[2]) Bitwise Channel Busy Bit [31:0] */
/* -------- MLB_MDAT[4] : (MLB Offset: 0x0C0) MIF Data 0 Register -------- */
#define MLB_MDAT_DATA_Pos 0
#define MLB_MDAT_DATA_Msk (0xffffffffu << MLB_MDAT_DATA_Pos) /**< \brief (MLB_MDAT[4]) CRT or DBR Data */
#define MLB_MDAT_DATA(value) ((MLB_MDAT_DATA_Msk & ((value) << MLB_MDAT_DATA_Pos)))
/* -------- MLB_MDWE[4] : (MLB Offset: 0x0D0) MIF Data Write Enable 0 Register -------- */
#define MLB_MDWE_MASK_Pos 0
#define MLB_MDWE_MASK_Msk (0xffffffffu << MLB_MDWE_MASK_Pos) /**< \brief (MLB_MDWE[4]) Bitwise write enable for CTR data - bits[31:0] */
#define MLB_MDWE_MASK(value) ((MLB_MDWE_MASK_Msk & ((value) << MLB_MDWE_MASK_Pos)))
/* -------- MLB_MCTL : (MLB Offset: 0x0E0) MIF Control Register -------- */
#define MLB_MCTL_XCMP (0x1u << 0) /**< \brief (MLB_MCTL) Transfer Complete (Write 0 to Clear) */
/* -------- MLB_MADR : (MLB Offset: 0x0E4) MIF Address Register -------- */
#define MLB_MADR_ADDR_Pos 0
#define MLB_MADR_ADDR_Msk (0x3fffu << MLB_MADR_ADDR_Pos) /**< \brief (MLB_MADR) CTR or DBR Address */
#define MLB_MADR_ADDR(value) ((MLB_MADR_ADDR_Msk & ((value) << MLB_MADR_ADDR_Pos)))
#define MLB_MADR_TB (0x1u << 30) /**< \brief (MLB_MADR) Target Location Bit */
#define   MLB_MADR_TB_CTR (0x0u << 30) /**< \brief (MLB_MADR) Selects CTR */
#define   MLB_MADR_TB_DBR (0x1u << 30) /**< \brief (MLB_MADR) Selects DBR */
#define MLB_MADR_WNR (0x1u << 31) /**< \brief (MLB_MADR) Write-Not-Read Selection */
/* -------- MLB_ACTL : (MLB Offset: 0x3C0) AHB Control Register -------- */
#define MLB_ACTL_SCE (0x1u << 0) /**< \brief (MLB_ACTL) Software Clear Enable */
#define MLB_ACTL_SMX (0x1u << 1) /**< \brief (MLB_ACTL) AHB Interrupt Mux Enable */
#define MLB_ACTL_DMA_MODE (0x1u << 2) /**< \brief (MLB_ACTL) DMA Mode */
#define MLB_ACTL_MPB (0x1u << 4) /**< \brief (MLB_ACTL) DMA Packet Buffering Mode */
#define   MLB_ACTL_MPB_SINGLE_PACKET (0x0u << 4) /**< \brief (MLB_ACTL) Single-packet mode */
#define   MLB_ACTL_MPB_MULTIPLE_PACKET (0x1u << 4) /**< \brief (MLB_ACTL) Multiple-packet mode */
/* -------- MLB_ACSR[2] : (MLB Offset: 0x3D0) AHB Channel Status 0 Register -------- */
#define MLB_ACSR_CHS_Pos 0
#define MLB_ACSR_CHS_Msk (0xffffffffu << MLB_ACSR_CHS_Pos) /**< \brief (MLB_ACSR[2]) Interrupt Status for Logical Channels [31:0] (cleared by writing a 1) */
#define MLB_ACSR_CHS(value) ((MLB_ACSR_CHS_Msk & ((value) << MLB_ACSR_CHS_Pos)))
/* -------- MLB_ACMR[2] : (MLB Offset: 0x3D8) AHB Channel Mask 0 Register -------- */
#define MLB_ACMR_CHM_Pos 0
#define MLB_ACMR_CHM_Msk (0xffffffffu << MLB_ACMR_CHM_Pos) /**< \brief (MLB_ACMR[2]) Bitwise Channel Mask Bits 31 to 0 */
#define MLB_ACMR_CHM(value) ((MLB_ACMR_CHM_Msk & ((value) << MLB_ACMR_CHM_Pos)))

/*@}*/


#endif /* _SAMV71_MLB_COMPONENT_ */
