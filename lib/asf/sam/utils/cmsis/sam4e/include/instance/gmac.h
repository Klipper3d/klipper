/**
 * \file
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _SAM4E_GMAC_INSTANCE_
#define _SAM4E_GMAC_INSTANCE_

/* ========== Register definition for GMAC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_GMAC_NCR                        (0x40034000U) /**< \brief (GMAC) Network Control Register */
  #define REG_GMAC_NCFGR                      (0x40034004U) /**< \brief (GMAC) Network Configuration Register */
  #define REG_GMAC_NSR                        (0x40034008U) /**< \brief (GMAC) Network Status Register */
  #define REG_GMAC_UR                         (0x4003400CU) /**< \brief (GMAC) User Register */
  #define REG_GMAC_DCFGR                      (0x40034010U) /**< \brief (GMAC) DMA Configuration Register */
  #define REG_GMAC_TSR                        (0x40034014U) /**< \brief (GMAC) Transmit Status Register */
  #define REG_GMAC_RBQB                       (0x40034018U) /**< \brief (GMAC) Receive Buffer Queue Base Address */
  #define REG_GMAC_TBQB                       (0x4003401CU) /**< \brief (GMAC) Transmit Buffer Queue Base Address */
  #define REG_GMAC_RSR                        (0x40034020U) /**< \brief (GMAC) Receive Status Register */
  #define REG_GMAC_ISR                        (0x40034024U) /**< \brief (GMAC) Interrupt Status Register */
  #define REG_GMAC_IER                        (0x40034028U) /**< \brief (GMAC) Interrupt Enable Register */
  #define REG_GMAC_IDR                        (0x4003402CU) /**< \brief (GMAC) Interrupt Disable Register */
  #define REG_GMAC_IMR                        (0x40034030U) /**< \brief (GMAC) Interrupt Mask Register */
  #define REG_GMAC_MAN                        (0x40034034U) /**< \brief (GMAC) PHY Maintenance Register */
  #define REG_GMAC_RPQ                        (0x40034038U) /**< \brief (GMAC) Received Pause Quantum Register */
  #define REG_GMAC_TPQ                        (0x4003403CU) /**< \brief (GMAC) Transmit Pause Quantum Register */
  #define REG_GMAC_HRB                        (0x40034080U) /**< \brief (GMAC) Hash Register Bottom [31:0] */
  #define REG_GMAC_HRT                        (0x40034084U) /**< \brief (GMAC) Hash Register Top [63:32] */
  #define REG_GMAC_SAB1                       (0x40034088U) /**< \brief (GMAC) Specific Address 1 Bottom [31:0] Register */
  #define REG_GMAC_SAT1                       (0x4003408CU) /**< \brief (GMAC) Specific Address 1 Top [47:32] Register */
  #define REG_GMAC_SAB2                       (0x40034090U) /**< \brief (GMAC) Specific Address 2 Bottom [31:0] Register */
  #define REG_GMAC_SAT2                       (0x40034094U) /**< \brief (GMAC) Specific Address 2 Top [47:32] Register */
  #define REG_GMAC_SAB3                       (0x40034098U) /**< \brief (GMAC) Specific Address 3 Bottom [31:0] Register */
  #define REG_GMAC_SAT3                       (0x4003409CU) /**< \brief (GMAC) Specific Address 3 Top [47:32] Register */
  #define REG_GMAC_SAB4                       (0x400340A0U) /**< \brief (GMAC) Specific Address 4 Bottom [31:0] Register */
  #define REG_GMAC_SAT4                       (0x400340A4U) /**< \brief (GMAC) Specific Address 4 Top [47:32] Register */
  #define REG_GMAC_TIDM                       (0x400340A8U) /**< \brief (GMAC) Type ID Match 1 Register */
  #define REG_GMAC_IPGS                       (0x400340BCU) /**< \brief (GMAC) IPG Stretch Register */
  #define REG_GMAC_SVLAN                      (0x400340C0U) /**< \brief (GMAC) Stacked VLAN Register */
  #define REG_GMAC_TPFCP                      (0x400340C4U) /**< \brief (GMAC) Transmit PFC Pause Register */
  #define REG_GMAC_SAMB1                      (0x400340C8U) /**< \brief (GMAC) Specific Address 1 Mask Bottom [31:0] Register */
  #define REG_GMAC_SAMT1                      (0x400340CCU) /**< \brief (GMAC) Specific Address 1 Mask Top [47:32] Register */
  #define REG_GMAC_OTLO                       (0x40034100U) /**< \brief (GMAC) Octets Transmitted [31:0] Register */
  #define REG_GMAC_OTHI                       (0x40034104U) /**< \brief (GMAC) Octets Transmitted [47:32] Register */
  #define REG_GMAC_FT                         (0x40034108U) /**< \brief (GMAC) Frames Transmitted Register */
  #define REG_GMAC_BCFT                       (0x4003410CU) /**< \brief (GMAC) Broadcast Frames Transmitted Register */
  #define REG_GMAC_MFT                        (0x40034110U) /**< \brief (GMAC) Multicast Frames Transmitted Register */
  #define REG_GMAC_PFT                        (0x40034114U) /**< \brief (GMAC) Pause Frames Transmitted Register */
  #define REG_GMAC_BFT64                      (0x40034118U) /**< \brief (GMAC) 64 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT127                    (0x4003411CU) /**< \brief (GMAC) 65 to 127 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT255                    (0x40034120U) /**< \brief (GMAC) 128 to 255 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT511                    (0x40034124U) /**< \brief (GMAC) 256 to 511 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT1023                   (0x40034128U) /**< \brief (GMAC) 512 to 1023 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT1518                   (0x4003412CU) /**< \brief (GMAC) 1024 to 1518 Byte Frames Transmitted Register */
  #define REG_GMAC_GTBFT1518                  (0x40034130U) /**< \brief (GMAC) Greater Than 1518 Byte Frames Transmitted Register */
  #define REG_GMAC_TUR                        (0x40034134U) /**< \brief (GMAC) Transmit Underruns Register */
  #define REG_GMAC_SCF                        (0x40034138U) /**< \brief (GMAC) Single Collision Frames Register */
  #define REG_GMAC_MCF                        (0x4003413CU) /**< \brief (GMAC) Multiple Collision Frames Register */
  #define REG_GMAC_EC                         (0x40034140U) /**< \brief (GMAC) Excessive Collisions Register */
  #define REG_GMAC_LC                         (0x40034144U) /**< \brief (GMAC) Late Collisions Register */
  #define REG_GMAC_DTF                        (0x40034148U) /**< \brief (GMAC) Deferred Transmission Frames Register */
  #define REG_GMAC_CSE                        (0x4003414CU) /**< \brief (GMAC) Carrier Sense Errors Register */
  #define REG_GMAC_ORLO                       (0x40034150U) /**< \brief (GMAC) Octets Received [31:0] Received */
  #define REG_GMAC_ORHI                       (0x40034154U) /**< \brief (GMAC) Octets Received [47:32] Received */
  #define REG_GMAC_FR                         (0x40034158U) /**< \brief (GMAC) Frames Received Register */
  #define REG_GMAC_BCFR                       (0x4003415CU) /**< \brief (GMAC) Broadcast Frames Received Register */
  #define REG_GMAC_MFR                        (0x40034160U) /**< \brief (GMAC) Multicast Frames Received Register */
  #define REG_GMAC_PFR                        (0x40034164U) /**< \brief (GMAC) Pause Frames Received Register */
  #define REG_GMAC_BFR64                      (0x40034168U) /**< \brief (GMAC) 64 Byte Frames Received Register */
  #define REG_GMAC_TBFR127                    (0x4003416CU) /**< \brief (GMAC) 65 to 127 Byte Frames Received Register */
  #define REG_GMAC_TBFR255                    (0x40034170U) /**< \brief (GMAC) 128 to 255 Byte Frames Received Register */
  #define REG_GMAC_TBFR511                    (0x40034174U) /**< \brief (GMAC) 256 to 511Byte Frames Received Register */
  #define REG_GMAC_TBFR1023                   (0x40034178U) /**< \brief (GMAC) 512 to 1023 Byte Frames Received Register */
  #define REG_GMAC_TBFR1518                   (0x4003417CU) /**< \brief (GMAC) 1024 to 1518 Byte Frames Received Register */
  #define REG_GMAC_TMXBFR                     (0x40034180U) /**< \brief (GMAC) 1519 to Maximum Byte Frames Received Register */
  #define REG_GMAC_UFR                        (0x40034184U) /**< \brief (GMAC) Undersize Frames Received Register */
  #define REG_GMAC_OFR                        (0x40034188U) /**< \brief (GMAC) Oversize Frames Received Register */
  #define REG_GMAC_JR                         (0x4003418CU) /**< \brief (GMAC) Jabbers Received Register */
  #define REG_GMAC_FCSE                       (0x40034190U) /**< \brief (GMAC) Frame Check Sequence Errors Register */
  #define REG_GMAC_LFFE                       (0x40034194U) /**< \brief (GMAC) Length Field Frame Errors Register */
  #define REG_GMAC_RSE                        (0x40034198U) /**< \brief (GMAC) Receive Symbol Errors Register */
  #define REG_GMAC_AE                         (0x4003419CU) /**< \brief (GMAC) Alignment Errors Register */
  #define REG_GMAC_RRE                        (0x400341A0U) /**< \brief (GMAC) Receive Resource Errors Register */
  #define REG_GMAC_ROE                        (0x400341A4U) /**< \brief (GMAC) Receive Overrun Register */
  #define REG_GMAC_IHCE                       (0x400341A8U) /**< \brief (GMAC) IP Header Checksum Errors Register */
  #define REG_GMAC_TCE                        (0x400341ACU) /**< \brief (GMAC) TCP Checksum Errors Register */
  #define REG_GMAC_UCE                        (0x400341B0U) /**< \brief (GMAC) UDP Checksum Errors Register */
  #define REG_GMAC_TSSSL                      (0x400341C8U) /**< \brief (GMAC) 1588 Timer Sync Strobe Seconds [31:0] Register */
  #define REG_GMAC_TSSN                       (0x400341CCU) /**< \brief (GMAC) 1588 Timer Sync Strobe Nanoseconds Register */
  #define REG_GMAC_TSL                        (0x400341D0U) /**< \brief (GMAC) 1588 Timer Seconds [31:0] Register */
  #define REG_GMAC_TN                         (0x400341D4U) /**< \brief (GMAC) 1588 Timer Nanoseconds Register */
  #define REG_GMAC_TA                         (0x400341D8U) /**< \brief (GMAC) 1588 Timer Adjust Register */
  #define REG_GMAC_TI                         (0x400341DCU) /**< \brief (GMAC) 1588 Timer Increment Register */
  #define REG_GMAC_EFTS                       (0x400341E0U) /**< \brief (GMAC) PTP Event Frame Transmitted Seconds */
  #define REG_GMAC_EFTN                       (0x400341E4U) /**< \brief (GMAC) PTP Event Frame Transmitted Nanoseconds */
  #define REG_GMAC_EFRS                       (0x400341E8U) /**< \brief (GMAC) PTP Event Frame Received Seconds */
  #define REG_GMAC_EFRN                       (0x400341ECU) /**< \brief (GMAC) PTP Event Frame Received Nanoseconds */
  #define REG_GMAC_PEFTS                      (0x400341F0U) /**< \brief (GMAC) PTP Peer Event Frame Transmitted Seconds */
  #define REG_GMAC_PEFTN                      (0x400341F4U) /**< \brief (GMAC) PTP Peer Event Frame Transmitted Nanoseconds */
  #define REG_GMAC_PEFRS                      (0x400341F8U) /**< \brief (GMAC) PTP Peer Event Frame Received Seconds */
  #define REG_GMAC_PEFRN                      (0x400341FCU) /**< \brief (GMAC) PTP Peer Event Frame Received Nanoseconds */
#else
  #define REG_GMAC_NCR       (*(__IO uint32_t*)0x40034000U) /**< \brief (GMAC) Network Control Register */
  #define REG_GMAC_NCFGR     (*(__IO uint32_t*)0x40034004U) /**< \brief (GMAC) Network Configuration Register */
  #define REG_GMAC_NSR       (*(__I  uint32_t*)0x40034008U) /**< \brief (GMAC) Network Status Register */
  #define REG_GMAC_UR        (*(__IO uint32_t*)0x4003400CU) /**< \brief (GMAC) User Register */
  #define REG_GMAC_DCFGR     (*(__IO uint32_t*)0x40034010U) /**< \brief (GMAC) DMA Configuration Register */
  #define REG_GMAC_TSR       (*(__IO uint32_t*)0x40034014U) /**< \brief (GMAC) Transmit Status Register */
  #define REG_GMAC_RBQB      (*(__IO uint32_t*)0x40034018U) /**< \brief (GMAC) Receive Buffer Queue Base Address */
  #define REG_GMAC_TBQB      (*(__IO uint32_t*)0x4003401CU) /**< \brief (GMAC) Transmit Buffer Queue Base Address */
  #define REG_GMAC_RSR       (*(__IO uint32_t*)0x40034020U) /**< \brief (GMAC) Receive Status Register */
  #define REG_GMAC_ISR       (*(__I  uint32_t*)0x40034024U) /**< \brief (GMAC) Interrupt Status Register */
  #define REG_GMAC_IER       (*(__O  uint32_t*)0x40034028U) /**< \brief (GMAC) Interrupt Enable Register */
  #define REG_GMAC_IDR       (*(__O  uint32_t*)0x4003402CU) /**< \brief (GMAC) Interrupt Disable Register */
  #define REG_GMAC_IMR       (*(__I  uint32_t*)0x40034030U) /**< \brief (GMAC) Interrupt Mask Register */
  #define REG_GMAC_MAN       (*(__IO uint32_t*)0x40034034U) /**< \brief (GMAC) PHY Maintenance Register */
  #define REG_GMAC_RPQ       (*(__I  uint32_t*)0x40034038U) /**< \brief (GMAC) Received Pause Quantum Register */
  #define REG_GMAC_TPQ       (*(__IO uint32_t*)0x4003403CU) /**< \brief (GMAC) Transmit Pause Quantum Register */
  #define REG_GMAC_HRB       (*(__IO uint32_t*)0x40034080U) /**< \brief (GMAC) Hash Register Bottom [31:0] */
  #define REG_GMAC_HRT       (*(__IO uint32_t*)0x40034084U) /**< \brief (GMAC) Hash Register Top [63:32] */
  #define REG_GMAC_SAB1      (*(__IO uint32_t*)0x40034088U) /**< \brief (GMAC) Specific Address 1 Bottom [31:0] Register */
  #define REG_GMAC_SAT1      (*(__IO uint32_t*)0x4003408CU) /**< \brief (GMAC) Specific Address 1 Top [47:32] Register */
  #define REG_GMAC_SAB2      (*(__IO uint32_t*)0x40034090U) /**< \brief (GMAC) Specific Address 2 Bottom [31:0] Register */
  #define REG_GMAC_SAT2      (*(__IO uint32_t*)0x40034094U) /**< \brief (GMAC) Specific Address 2 Top [47:32] Register */
  #define REG_GMAC_SAB3      (*(__IO uint32_t*)0x40034098U) /**< \brief (GMAC) Specific Address 3 Bottom [31:0] Register */
  #define REG_GMAC_SAT3      (*(__IO uint32_t*)0x4003409CU) /**< \brief (GMAC) Specific Address 3 Top [47:32] Register */
  #define REG_GMAC_SAB4      (*(__IO uint32_t*)0x400340A0U) /**< \brief (GMAC) Specific Address 4 Bottom [31:0] Register */
  #define REG_GMAC_SAT4      (*(__IO uint32_t*)0x400340A4U) /**< \brief (GMAC) Specific Address 4 Top [47:32] Register */
  #define REG_GMAC_TIDM      (*(__IO uint32_t*)0x400340A8U) /**< \brief (GMAC) Type ID Match 1 Register */
  #define REG_GMAC_IPGS      (*(__IO uint32_t*)0x400340BCU) /**< \brief (GMAC) IPG Stretch Register */
  #define REG_GMAC_SVLAN     (*(__IO uint32_t*)0x400340C0U) /**< \brief (GMAC) Stacked VLAN Register */
  #define REG_GMAC_TPFCP     (*(__IO uint32_t*)0x400340C4U) /**< \brief (GMAC) Transmit PFC Pause Register */
  #define REG_GMAC_SAMB1     (*(__IO uint32_t*)0x400340C8U) /**< \brief (GMAC) Specific Address 1 Mask Bottom [31:0] Register */
  #define REG_GMAC_SAMT1     (*(__IO uint32_t*)0x400340CCU) /**< \brief (GMAC) Specific Address 1 Mask Top [47:32] Register */
  #define REG_GMAC_OTLO      (*(__I  uint32_t*)0x40034100U) /**< \brief (GMAC) Octets Transmitted [31:0] Register */
  #define REG_GMAC_OTHI      (*(__I  uint32_t*)0x40034104U) /**< \brief (GMAC) Octets Transmitted [47:32] Register */
  #define REG_GMAC_FT        (*(__I  uint32_t*)0x40034108U) /**< \brief (GMAC) Frames Transmitted Register */
  #define REG_GMAC_BCFT      (*(__I  uint32_t*)0x4003410CU) /**< \brief (GMAC) Broadcast Frames Transmitted Register */
  #define REG_GMAC_MFT       (*(__I  uint32_t*)0x40034110U) /**< \brief (GMAC) Multicast Frames Transmitted Register */
  #define REG_GMAC_PFT       (*(__I  uint32_t*)0x40034114U) /**< \brief (GMAC) Pause Frames Transmitted Register */
  #define REG_GMAC_BFT64     (*(__I  uint32_t*)0x40034118U) /**< \brief (GMAC) 64 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT127   (*(__I  uint32_t*)0x4003411CU) /**< \brief (GMAC) 65 to 127 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT255   (*(__I  uint32_t*)0x40034120U) /**< \brief (GMAC) 128 to 255 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT511   (*(__I  uint32_t*)0x40034124U) /**< \brief (GMAC) 256 to 511 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT1023  (*(__I  uint32_t*)0x40034128U) /**< \brief (GMAC) 512 to 1023 Byte Frames Transmitted Register */
  #define REG_GMAC_TBFT1518  (*(__I  uint32_t*)0x4003412CU) /**< \brief (GMAC) 1024 to 1518 Byte Frames Transmitted Register */
  #define REG_GMAC_GTBFT1518 (*(__I  uint32_t*)0x40034130U) /**< \brief (GMAC) Greater Than 1518 Byte Frames Transmitted Register */
  #define REG_GMAC_TUR       (*(__I  uint32_t*)0x40034134U) /**< \brief (GMAC) Transmit Underruns Register */
  #define REG_GMAC_SCF       (*(__I  uint32_t*)0x40034138U) /**< \brief (GMAC) Single Collision Frames Register */
  #define REG_GMAC_MCF       (*(__I  uint32_t*)0x4003413CU) /**< \brief (GMAC) Multiple Collision Frames Register */
  #define REG_GMAC_EC        (*(__I  uint32_t*)0x40034140U) /**< \brief (GMAC) Excessive Collisions Register */
  #define REG_GMAC_LC        (*(__I  uint32_t*)0x40034144U) /**< \brief (GMAC) Late Collisions Register */
  #define REG_GMAC_DTF       (*(__I  uint32_t*)0x40034148U) /**< \brief (GMAC) Deferred Transmission Frames Register */
  #define REG_GMAC_CSE       (*(__I  uint32_t*)0x4003414CU) /**< \brief (GMAC) Carrier Sense Errors Register */
  #define REG_GMAC_ORLO      (*(__I  uint32_t*)0x40034150U) /**< \brief (GMAC) Octets Received [31:0] Received */
  #define REG_GMAC_ORHI      (*(__I  uint32_t*)0x40034154U) /**< \brief (GMAC) Octets Received [47:32] Received */
  #define REG_GMAC_FR        (*(__I  uint32_t*)0x40034158U) /**< \brief (GMAC) Frames Received Register */
  #define REG_GMAC_BCFR      (*(__I  uint32_t*)0x4003415CU) /**< \brief (GMAC) Broadcast Frames Received Register */
  #define REG_GMAC_MFR       (*(__I  uint32_t*)0x40034160U) /**< \brief (GMAC) Multicast Frames Received Register */
  #define REG_GMAC_PFR       (*(__I  uint32_t*)0x40034164U) /**< \brief (GMAC) Pause Frames Received Register */
  #define REG_GMAC_BFR64     (*(__I  uint32_t*)0x40034168U) /**< \brief (GMAC) 64 Byte Frames Received Register */
  #define REG_GMAC_TBFR127   (*(__I  uint32_t*)0x4003416CU) /**< \brief (GMAC) 65 to 127 Byte Frames Received Register */
  #define REG_GMAC_TBFR255   (*(__I  uint32_t*)0x40034170U) /**< \brief (GMAC) 128 to 255 Byte Frames Received Register */
  #define REG_GMAC_TBFR511   (*(__I  uint32_t*)0x40034174U) /**< \brief (GMAC) 256 to 511Byte Frames Received Register */
  #define REG_GMAC_TBFR1023  (*(__I  uint32_t*)0x40034178U) /**< \brief (GMAC) 512 to 1023 Byte Frames Received Register */
  #define REG_GMAC_TBFR1518  (*(__I  uint32_t*)0x4003417CU) /**< \brief (GMAC) 1024 to 1518 Byte Frames Received Register */
  #define REG_GMAC_TMXBFR    (*(__I  uint32_t*)0x40034180U) /**< \brief (GMAC) 1519 to Maximum Byte Frames Received Register */
  #define REG_GMAC_UFR       (*(__I  uint32_t*)0x40034184U) /**< \brief (GMAC) Undersize Frames Received Register */
  #define REG_GMAC_OFR       (*(__I  uint32_t*)0x40034188U) /**< \brief (GMAC) Oversize Frames Received Register */
  #define REG_GMAC_JR        (*(__I  uint32_t*)0x4003418CU) /**< \brief (GMAC) Jabbers Received Register */
  #define REG_GMAC_FCSE      (*(__I  uint32_t*)0x40034190U) /**< \brief (GMAC) Frame Check Sequence Errors Register */
  #define REG_GMAC_LFFE      (*(__I  uint32_t*)0x40034194U) /**< \brief (GMAC) Length Field Frame Errors Register */
  #define REG_GMAC_RSE       (*(__I  uint32_t*)0x40034198U) /**< \brief (GMAC) Receive Symbol Errors Register */
  #define REG_GMAC_AE        (*(__I  uint32_t*)0x4003419CU) /**< \brief (GMAC) Alignment Errors Register */
  #define REG_GMAC_RRE       (*(__I  uint32_t*)0x400341A0U) /**< \brief (GMAC) Receive Resource Errors Register */
  #define REG_GMAC_ROE       (*(__I  uint32_t*)0x400341A4U) /**< \brief (GMAC) Receive Overrun Register */
  #define REG_GMAC_IHCE      (*(__I  uint32_t*)0x400341A8U) /**< \brief (GMAC) IP Header Checksum Errors Register */
  #define REG_GMAC_TCE       (*(__I  uint32_t*)0x400341ACU) /**< \brief (GMAC) TCP Checksum Errors Register */
  #define REG_GMAC_UCE       (*(__I  uint32_t*)0x400341B0U) /**< \brief (GMAC) UDP Checksum Errors Register */
  #define REG_GMAC_TSSSL     (*(__IO uint32_t*)0x400341C8U) /**< \brief (GMAC) 1588 Timer Sync Strobe Seconds [31:0] Register */
  #define REG_GMAC_TSSN      (*(__IO uint32_t*)0x400341CCU) /**< \brief (GMAC) 1588 Timer Sync Strobe Nanoseconds Register */
  #define REG_GMAC_TSL       (*(__IO uint32_t*)0x400341D0U) /**< \brief (GMAC) 1588 Timer Seconds [31:0] Register */
  #define REG_GMAC_TN        (*(__IO uint32_t*)0x400341D4U) /**< \brief (GMAC) 1588 Timer Nanoseconds Register */
  #define REG_GMAC_TA        (*(__O  uint32_t*)0x400341D8U) /**< \brief (GMAC) 1588 Timer Adjust Register */
  #define REG_GMAC_TI        (*(__IO uint32_t*)0x400341DCU) /**< \brief (GMAC) 1588 Timer Increment Register */
  #define REG_GMAC_EFTS      (*(__I  uint32_t*)0x400341E0U) /**< \brief (GMAC) PTP Event Frame Transmitted Seconds */
  #define REG_GMAC_EFTN      (*(__I  uint32_t*)0x400341E4U) /**< \brief (GMAC) PTP Event Frame Transmitted Nanoseconds */
  #define REG_GMAC_EFRS      (*(__I  uint32_t*)0x400341E8U) /**< \brief (GMAC) PTP Event Frame Received Seconds */
  #define REG_GMAC_EFRN      (*(__I  uint32_t*)0x400341ECU) /**< \brief (GMAC) PTP Event Frame Received Nanoseconds */
  #define REG_GMAC_PEFTS     (*(__I  uint32_t*)0x400341F0U) /**< \brief (GMAC) PTP Peer Event Frame Transmitted Seconds */
  #define REG_GMAC_PEFTN     (*(__I  uint32_t*)0x400341F4U) /**< \brief (GMAC) PTP Peer Event Frame Transmitted Nanoseconds */
  #define REG_GMAC_PEFRS     (*(__I  uint32_t*)0x400341F8U) /**< \brief (GMAC) PTP Peer Event Frame Received Seconds */
  #define REG_GMAC_PEFRN     (*(__I  uint32_t*)0x400341FCU) /**< \brief (GMAC) PTP Peer Event Frame Received Nanoseconds */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_GMAC_INSTANCE_ */
