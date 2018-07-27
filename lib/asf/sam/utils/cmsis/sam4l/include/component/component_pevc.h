/**
 * \file
 *
 * \brief Component description for PEVC
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

#ifndef _SAM4L_PEVC_COMPONENT_
#define _SAM4L_PEVC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PEVC */
/* ========================================================================== */
/** \addtogroup SAM4L_PEVC Peripheral Event Controller */
/*@{*/

#define REV_PEVC                    0x200

/* -------- PEVC_CHSR : (PEVC Offset: 0x000) (R/  32) Channel Status Register -------- */
#define PEVC_CHSR_OFFSET            0x000        /**< \brief (PEVC_CHSR offset) Channel Status Register */
#define PEVC_CHSR_RESETVALUE        0x00000000   /**< \brief (PEVC_CHSR reset_value) Channel Status Register */

#define PEVC_CHSR_CHS_Pos           0            /**< \brief (PEVC_CHSR) Channel Status */
#define PEVC_CHSR_CHS_Msk           (0xFFFFFFFFu << PEVC_CHSR_CHS_Pos)
#define PEVC_CHSR_CHS(value)        ((PEVC_CHSR_CHS_Msk & ((value) << PEVC_CHSR_CHS_Pos)))
#define   PEVC_CHSR_CHS_0           (0x0u <<  0) /**< \brief (PEVC_CHSR) Channel j Disabled */
#define   PEVC_CHSR_CHS_1           (0x1u <<  0) /**< \brief (PEVC_CHSR) Channel j Enabled */
#define PEVC_CHSR_MASK              0xFFFFFFFFu  /**< \brief (PEVC_CHSR) MASK Register */

/* -------- PEVC_CHER : (PEVC Offset: 0x004) ( /W 32) Channel Enable Register -------- */
#define PEVC_CHER_OFFSET            0x004        /**< \brief (PEVC_CHER offset) Channel Enable Register */
#define PEVC_CHER_RESETVALUE        0x00000000   /**< \brief (PEVC_CHER reset_value) Channel Enable Register */

#define PEVC_CHER_CHE_Pos           0            /**< \brief (PEVC_CHER) Channel Enable */
#define PEVC_CHER_CHE_Msk           (0xFFFFFFFFu << PEVC_CHER_CHE_Pos)
#define PEVC_CHER_CHE(value)        ((PEVC_CHER_CHE_Msk & ((value) << PEVC_CHER_CHE_Pos)))
#define   PEVC_CHER_CHE_0           (0x0u <<  0) /**< \brief (PEVC_CHER) No Action */
#define   PEVC_CHER_CHE_1           (0x1u <<  0) /**< \brief (PEVC_CHER) Enable Channel j */
#define PEVC_CHER_MASK              0xFFFFFFFFu  /**< \brief (PEVC_CHER) MASK Register */

/* -------- PEVC_CHDR : (PEVC Offset: 0x008) ( /W 32) Channel Disable Register -------- */
#define PEVC_CHDR_OFFSET            0x008        /**< \brief (PEVC_CHDR offset) Channel Disable Register */
#define PEVC_CHDR_RESETVALUE        0x00000000   /**< \brief (PEVC_CHDR reset_value) Channel Disable Register */

#define PEVC_CHDR_CHD_Pos           0            /**< \brief (PEVC_CHDR) Channel Disable */
#define PEVC_CHDR_CHD_Msk           (0xFFFFFFFFu << PEVC_CHDR_CHD_Pos)
#define PEVC_CHDR_CHD(value)        ((PEVC_CHDR_CHD_Msk & ((value) << PEVC_CHDR_CHD_Pos)))
#define   PEVC_CHDR_CHD_0           (0x0u <<  0) /**< \brief (PEVC_CHDR) No Action */
#define   PEVC_CHDR_CHD_1           (0x1u <<  0) /**< \brief (PEVC_CHDR) Disable Channel j */
#define PEVC_CHDR_MASK              0xFFFFFFFFu  /**< \brief (PEVC_CHDR) MASK Register */

/* -------- PEVC_SEV : (PEVC Offset: 0x010) ( /W 32) Software Event -------- */
#define PEVC_SEV_OFFSET             0x010        /**< \brief (PEVC_SEV offset) Software Event */
#define PEVC_SEV_RESETVALUE         0x00000000   /**< \brief (PEVC_SEV reset_value) Software Event */

#define PEVC_SEV_SEV_Pos            0            /**< \brief (PEVC_SEV) Software Event */
#define PEVC_SEV_SEV_Msk            (0xFFFFFFFFu << PEVC_SEV_SEV_Pos)
#define PEVC_SEV_SEV(value)         ((PEVC_SEV_SEV_Msk & ((value) << PEVC_SEV_SEV_Pos)))
#define   PEVC_SEV_SEV_0            (0x0u <<  0) /**< \brief (PEVC_SEV) No Action */
#define   PEVC_SEV_SEV_1            (0x1u <<  0) /**< \brief (PEVC_SEV) CPU forces software event to channel j */
#define PEVC_SEV_MASK               0xFFFFFFFFu  /**< \brief (PEVC_SEV) MASK Register */

/* -------- PEVC_BUSY : (PEVC Offset: 0x014) (R/  32) Channel / User Busy -------- */
#define PEVC_BUSY_OFFSET            0x014        /**< \brief (PEVC_BUSY offset) Channel / User Busy */
#define PEVC_BUSY_RESETVALUE        0x00000000   /**< \brief (PEVC_BUSY reset_value) Channel / User Busy */

#define PEVC_BUSY_BUSY_Pos          0            /**< \brief (PEVC_BUSY) Channel Status */
#define PEVC_BUSY_BUSY_Msk          (0xFFFFFFFFu << PEVC_BUSY_BUSY_Pos)
#define PEVC_BUSY_BUSY(value)       ((PEVC_BUSY_BUSY_Msk & ((value) << PEVC_BUSY_BUSY_Pos)))
#define   PEVC_BUSY_BUSY_0          (0x0u <<  0) /**< \brief (PEVC_BUSY) No Action */
#define   PEVC_BUSY_BUSY_1          (0x1u <<  0) /**< \brief (PEVC_BUSY) Channel j or User j is Busy */
#define PEVC_BUSY_MASK              0xFFFFFFFFu  /**< \brief (PEVC_BUSY) MASK Register */

/* -------- PEVC_TRIER : (PEVC Offset: 0x020) ( /W 32) Trigger Interrupt Mask Enable Register -------- */
#define PEVC_TRIER_OFFSET           0x020        /**< \brief (PEVC_TRIER offset) Trigger Interrupt Mask Enable Register */
#define PEVC_TRIER_RESETVALUE       0x00000000   /**< \brief (PEVC_TRIER reset_value) Trigger Interrupt Mask Enable Register */

#define PEVC_TRIER_TRIE_Pos         0            /**< \brief (PEVC_TRIER) Trigger Interrupt Enable */
#define PEVC_TRIER_TRIE_Msk         (0xFFFFFFFFu << PEVC_TRIER_TRIE_Pos)
#define PEVC_TRIER_TRIE(value)      ((PEVC_TRIER_TRIE_Msk & ((value) << PEVC_TRIER_TRIE_Pos)))
#define   PEVC_TRIER_TRIE_0         (0x0u <<  0) /**< \brief (PEVC_TRIER) No Action */
#define   PEVC_TRIER_TRIE_1         (0x1u <<  0) /**< \brief (PEVC_TRIER) Enable Trigger j Interrupt */
#define PEVC_TRIER_MASK             0xFFFFFFFFu  /**< \brief (PEVC_TRIER) MASK Register */

/* -------- PEVC_TRIDR : (PEVC Offset: 0x024) ( /W 32) Trigger Interrupt Mask Disable Register -------- */
#define PEVC_TRIDR_OFFSET           0x024        /**< \brief (PEVC_TRIDR offset) Trigger Interrupt Mask Disable Register */
#define PEVC_TRIDR_RESETVALUE       0x00000000   /**< \brief (PEVC_TRIDR reset_value) Trigger Interrupt Mask Disable Register */

#define PEVC_TRIDR_TRID_Pos         0            /**< \brief (PEVC_TRIDR) Trigger Interrupt Disable */
#define PEVC_TRIDR_TRID_Msk         (0xFFFFFFFFu << PEVC_TRIDR_TRID_Pos)
#define PEVC_TRIDR_TRID(value)      ((PEVC_TRIDR_TRID_Msk & ((value) << PEVC_TRIDR_TRID_Pos)))
#define   PEVC_TRIDR_TRID_0         (0x0u <<  0) /**< \brief (PEVC_TRIDR) No Action */
#define   PEVC_TRIDR_TRID_1         (0x1u <<  0) /**< \brief (PEVC_TRIDR) Disable Trigger j Interrupt */
#define PEVC_TRIDR_MASK             0xFFFFFFFFu  /**< \brief (PEVC_TRIDR) MASK Register */

/* -------- PEVC_TRIMR : (PEVC Offset: 0x028) (R/  32) Trigger Interrupt Mask Register -------- */
#define PEVC_TRIMR_OFFSET           0x028        /**< \brief (PEVC_TRIMR offset) Trigger Interrupt Mask Register */
#define PEVC_TRIMR_RESETVALUE       0x00000000   /**< \brief (PEVC_TRIMR reset_value) Trigger Interrupt Mask Register */

#define PEVC_TRIMR_TRIM_Pos         0            /**< \brief (PEVC_TRIMR) Trigger Interrupt Mask */
#define PEVC_TRIMR_TRIM_Msk         (0xFFFFFFFFu << PEVC_TRIMR_TRIM_Pos)
#define PEVC_TRIMR_TRIM(value)      ((PEVC_TRIMR_TRIM_Msk & ((value) << PEVC_TRIMR_TRIM_Pos)))
#define   PEVC_TRIMR_TRIM_0         (0x0u <<  0) /**< \brief (PEVC_TRIMR) Trigger j Interrupt Disabled */
#define   PEVC_TRIMR_TRIM_1         (0x1u <<  0) /**< \brief (PEVC_TRIMR) Trigger j Interrupt Enabled */
#define PEVC_TRIMR_MASK             0xFFFFFFFFu  /**< \brief (PEVC_TRIMR) MASK Register */

/* -------- PEVC_TRSR : (PEVC Offset: 0x030) (R/  32) Trigger Status Register -------- */
#define PEVC_TRSR_OFFSET            0x030        /**< \brief (PEVC_TRSR offset) Trigger Status Register */
#define PEVC_TRSR_RESETVALUE        0x00000000   /**< \brief (PEVC_TRSR reset_value) Trigger Status Register */

#define PEVC_TRSR_TRS_Pos           0            /**< \brief (PEVC_TRSR) Trigger Interrupt Status */
#define PEVC_TRSR_TRS_Msk           (0xFFFFFFFFu << PEVC_TRSR_TRS_Pos)
#define PEVC_TRSR_TRS(value)        ((PEVC_TRSR_TRS_Msk & ((value) << PEVC_TRSR_TRS_Pos)))
#define   PEVC_TRSR_TRS_0           (0x0u <<  0) /**< \brief (PEVC_TRSR) Channel j did not send out an Event in the past */
#define   PEVC_TRSR_TRS_1           (0x1u <<  0) /**< \brief (PEVC_TRSR) Channel j did send out an Event in the past */
#define PEVC_TRSR_MASK              0xFFFFFFFFu  /**< \brief (PEVC_TRSR) MASK Register */

/* -------- PEVC_TRSCR : (PEVC Offset: 0x034) ( /W 32) Trigger Status Clear Register -------- */
#define PEVC_TRSCR_OFFSET           0x034        /**< \brief (PEVC_TRSCR offset) Trigger Status Clear Register */
#define PEVC_TRSCR_RESETVALUE       0x00000000   /**< \brief (PEVC_TRSCR reset_value) Trigger Status Clear Register */

#define PEVC_TRSCR_TRSC_Pos         0            /**< \brief (PEVC_TRSCR) Trigger Interrupt Status Clear */
#define PEVC_TRSCR_TRSC_Msk         (0xFFFFFFFFu << PEVC_TRSCR_TRSC_Pos)
#define PEVC_TRSCR_TRSC(value)      ((PEVC_TRSCR_TRSC_Msk & ((value) << PEVC_TRSCR_TRSC_Pos)))
#define   PEVC_TRSCR_TRSC_0         (0x0u <<  0) /**< \brief (PEVC_TRSCR) No Action */
#define   PEVC_TRSCR_TRSC_1         (0x1u <<  0) /**< \brief (PEVC_TRSCR) Clear TRSR[j] */
#define PEVC_TRSCR_MASK             0xFFFFFFFFu  /**< \brief (PEVC_TRSCR) MASK Register */

/* -------- PEVC_OVIER : (PEVC Offset: 0x040) ( /W 32) Overrun Interrupt Mask Enable Register -------- */
#define PEVC_OVIER_OFFSET           0x040        /**< \brief (PEVC_OVIER offset) Overrun Interrupt Mask Enable Register */
#define PEVC_OVIER_RESETVALUE       0x00000000   /**< \brief (PEVC_OVIER reset_value) Overrun Interrupt Mask Enable Register */

#define PEVC_OVIER_OVIE_Pos         0            /**< \brief (PEVC_OVIER) Overrun Interrupt Enable */
#define PEVC_OVIER_OVIE_Msk         (0xFFFFFFFFu << PEVC_OVIER_OVIE_Pos)
#define PEVC_OVIER_OVIE(value)      ((PEVC_OVIER_OVIE_Msk & ((value) << PEVC_OVIER_OVIE_Pos)))
#define   PEVC_OVIER_OVIE_0         (0x0u <<  0) /**< \brief (PEVC_OVIER) No Action */
#define   PEVC_OVIER_OVIE_1         (0x1u <<  0) /**< \brief (PEVC_OVIER) Enable Overrun Interrupt for Channel j */
#define PEVC_OVIER_MASK             0xFFFFFFFFu  /**< \brief (PEVC_OVIER) MASK Register */

/* -------- PEVC_OVIDR : (PEVC Offset: 0x044) ( /W 32) Overrun Interrupt Mask Disable Register -------- */
#define PEVC_OVIDR_OFFSET           0x044        /**< \brief (PEVC_OVIDR offset) Overrun Interrupt Mask Disable Register */
#define PEVC_OVIDR_RESETVALUE       0x00000000   /**< \brief (PEVC_OVIDR reset_value) Overrun Interrupt Mask Disable Register */

#define PEVC_OVIDR_OVID_Pos         0            /**< \brief (PEVC_OVIDR) Overrun Interrupt Disable */
#define PEVC_OVIDR_OVID_Msk         (0xFFFFFFFFu << PEVC_OVIDR_OVID_Pos)
#define PEVC_OVIDR_OVID(value)      ((PEVC_OVIDR_OVID_Msk & ((value) << PEVC_OVIDR_OVID_Pos)))
#define   PEVC_OVIDR_OVID_0         (0x0u <<  0) /**< \brief (PEVC_OVIDR) No Action */
#define   PEVC_OVIDR_OVID_1         (0x1u <<  0) /**< \brief (PEVC_OVIDR) Enable Overrun Interrupt for Channel j */
#define PEVC_OVIDR_MASK             0xFFFFFFFFu  /**< \brief (PEVC_OVIDR) MASK Register */

/* -------- PEVC_OVIMR : (PEVC Offset: 0x048) (R/  32) Overrun Interrupt Mask Register -------- */
#define PEVC_OVIMR_OFFSET           0x048        /**< \brief (PEVC_OVIMR offset) Overrun Interrupt Mask Register */
#define PEVC_OVIMR_RESETVALUE       0x00000000   /**< \brief (PEVC_OVIMR reset_value) Overrun Interrupt Mask Register */

#define PEVC_OVIMR_OVIM_Pos         0            /**< \brief (PEVC_OVIMR) Overrun Interrupt Mask */
#define PEVC_OVIMR_OVIM_Msk         (0xFFFFFFFFu << PEVC_OVIMR_OVIM_Pos)
#define PEVC_OVIMR_OVIM(value)      ((PEVC_OVIMR_OVIM_Msk & ((value) << PEVC_OVIMR_OVIM_Pos)))
#define   PEVC_OVIMR_OVIM_0         (0x0u <<  0) /**< \brief (PEVC_OVIMR) Overrun Interrupt for Channel j Disabled */
#define   PEVC_OVIMR_OVIM_1         (0x1u <<  0) /**< \brief (PEVC_OVIMR) Overrun Interrupt for Channel j Enabled */
#define PEVC_OVIMR_MASK             0xFFFFFFFFu  /**< \brief (PEVC_OVIMR) MASK Register */

/* -------- PEVC_OVSR : (PEVC Offset: 0x050) (R/  32) Overrun Status Register -------- */
#define PEVC_OVSR_OFFSET            0x050        /**< \brief (PEVC_OVSR offset) Overrun Status Register */
#define PEVC_OVSR_RESETVALUE        0x00000000   /**< \brief (PEVC_OVSR reset_value) Overrun Status Register */

#define PEVC_OVSR_OVS_Pos           0            /**< \brief (PEVC_OVSR) Overrun Interrupt Status */
#define PEVC_OVSR_OVS_Msk           (0xFFFFFFFFu << PEVC_OVSR_OVS_Pos)
#define PEVC_OVSR_OVS(value)        ((PEVC_OVSR_OVS_Msk & ((value) << PEVC_OVSR_OVS_Pos)))
#define   PEVC_OVSR_OVS_0           (0x0u <<  0) /**< \brief (PEVC_OVSR) No Overrun occured on Channel j */
#define   PEVC_OVSR_OVS_1           (0x1u <<  0) /**< \brief (PEVC_OVSR) Overrun occured on Channel j */
#define PEVC_OVSR_MASK              0xFFFFFFFFu  /**< \brief (PEVC_OVSR) MASK Register */

/* -------- PEVC_OVSCR : (PEVC Offset: 0x054) ( /W 32) Overrun Status Clear Register -------- */
#define PEVC_OVSCR_OFFSET           0x054        /**< \brief (PEVC_OVSCR offset) Overrun Status Clear Register */
#define PEVC_OVSCR_RESETVALUE       0x00000000   /**< \brief (PEVC_OVSCR reset_value) Overrun Status Clear Register */

#define PEVC_OVSCR_OVSC_Pos         0            /**< \brief (PEVC_OVSCR) Overrun Interrupt Status Clear */
#define PEVC_OVSCR_OVSC_Msk         (0xFFFFFFFFu << PEVC_OVSCR_OVSC_Pos)
#define PEVC_OVSCR_OVSC(value)      ((PEVC_OVSCR_OVSC_Msk & ((value) << PEVC_OVSCR_OVSC_Pos)))
#define   PEVC_OVSCR_OVSC_0         (0x0u <<  0) /**< \brief (PEVC_OVSCR) No Action */
#define   PEVC_OVSCR_OVSC_1         (0x1u <<  0) /**< \brief (PEVC_OVSCR) Clear Overrun Status Bit j */
#define PEVC_OVSCR_MASK             0xFFFFFFFFu  /**< \brief (PEVC_OVSCR) MASK Register */

/* -------- PEVC_CHMX : (PEVC Offset: 0x100) (R/W 32) CHMX Channel Multiplexer -------- */
#define PEVC_CHMX_OFFSET            0x100        /**< \brief (PEVC_CHMX offset) Channel Multiplexer */
#define PEVC_CHMX_RESETVALUE        0x00000000   /**< \brief (PEVC_CHMX reset_value) Channel Multiplexer */

#define PEVC_CHMX_EVMX_Pos          0            /**< \brief (PEVC_CHMX) Event Multiplexer */
#define PEVC_CHMX_EVMX_Msk          (0x3Fu << PEVC_CHMX_EVMX_Pos)
#define PEVC_CHMX_EVMX(value)       ((PEVC_CHMX_EVMX_Msk & ((value) << PEVC_CHMX_EVMX_Pos)))
#define   PEVC_CHMX_EVMX_0          (0x0u <<  0) /**< \brief (PEVC_CHMX) Event 0 */
#define   PEVC_CHMX_EVMX_1          (0x1u <<  0) /**< \brief (PEVC_CHMX) Event 1 */
#define PEVC_CHMX_SMX_Pos           8            /**< \brief (PEVC_CHMX) Software Event Multiplexer */
#define PEVC_CHMX_SMX               (0x1u << PEVC_CHMX_SMX_Pos)
#define   PEVC_CHMX_SMX_0           (0x0u <<  8) /**< \brief (PEVC_CHMX) Hardware events */
#define   PEVC_CHMX_SMX_1           (0x1u <<  8) /**< \brief (PEVC_CHMX) Software event */
#define PEVC_CHMX_MASK              0x0000013Fu  /**< \brief (PEVC_CHMX) MASK Register */

/* -------- PEVC_EVS : (PEVC Offset: 0x200) (R/W 32) EVS Event Shaper -------- */
#define PEVC_EVS_OFFSET             0x200        /**< \brief (PEVC_EVS offset) Event Shaper */
#define PEVC_EVS_RESETVALUE         0x00000000   /**< \brief (PEVC_EVS reset_value) Event Shaper */

#define PEVC_EVS_EN_Pos             0            /**< \brief (PEVC_EVS) Event Shaper Enable */
#define PEVC_EVS_EN                 (0x1u << PEVC_EVS_EN_Pos)
#define   PEVC_EVS_EN_0             (0x0u <<  0) /**< \brief (PEVC_EVS) No Action */
#define   PEVC_EVS_EN_1             (0x1u <<  0) /**< \brief (PEVC_EVS) Event Shaper enable */
#define PEVC_EVS_IGFR_Pos           16           /**< \brief (PEVC_EVS) Input Glitch Filter Rise */
#define PEVC_EVS_IGFR               (0x1u << PEVC_EVS_IGFR_Pos)
#define   PEVC_EVS_IGFR_0           (0x0u << 16) /**< \brief (PEVC_EVS) No Action */
#define   PEVC_EVS_IGFR_1           (0x1u << 16) /**< \brief (PEVC_EVS) Input Glitch Filter : a rising edge on event input will raise trigger output */
#define PEVC_EVS_IGFF_Pos           17           /**< \brief (PEVC_EVS) Input Glitch Filter Fall */
#define PEVC_EVS_IGFF               (0x1u << PEVC_EVS_IGFF_Pos)
#define   PEVC_EVS_IGFF_0           (0x0u << 17) /**< \brief (PEVC_EVS) No Action */
#define   PEVC_EVS_IGFF_1           (0x1u << 17) /**< \brief (PEVC_EVS) Input Glitch Filter : a falling edge on event input will raise trigger output */
#define PEVC_EVS_IGFON_Pos          18           /**< \brief (PEVC_EVS) Input Glitch Filter Status */
#define PEVC_EVS_IGFON              (0x1u << PEVC_EVS_IGFON_Pos)
#define PEVC_EVS_MASK               0x00070001u  /**< \brief (PEVC_EVS) MASK Register */

/* -------- PEVC_IGFDR : (PEVC Offset: 0x300) (R/W 32) Input Glitch Filter Divider Register -------- */
#define PEVC_IGFDR_OFFSET           0x300        /**< \brief (PEVC_IGFDR offset) Input Glitch Filter Divider Register */
#define PEVC_IGFDR_RESETVALUE       0x00000000   /**< \brief (PEVC_IGFDR reset_value) Input Glitch Filter Divider Register */

#define PEVC_IGFDR_IGFDR_Pos        0            /**< \brief (PEVC_IGFDR) Input Glitch Filter Divider Register */
#define PEVC_IGFDR_IGFDR_Msk        (0xFu << PEVC_IGFDR_IGFDR_Pos)
#define PEVC_IGFDR_IGFDR(value)     ((PEVC_IGFDR_IGFDR_Msk & ((value) << PEVC_IGFDR_IGFDR_Pos)))
#define PEVC_IGFDR_MASK             0x0000000Fu  /**< \brief (PEVC_IGFDR) MASK Register */

/* -------- PEVC_PARAMETER : (PEVC Offset: 0x3F8) (R/  32) Parameter -------- */
#define PEVC_PARAMETER_OFFSET       0x3F8        /**< \brief (PEVC_PARAMETER offset) Parameter */
#define PEVC_PARAMETER_RESETVALUE   0x14061824   /**< \brief (PEVC_PARAMETER reset_value) Parameter */

#define PEVC_PARAMETER_IGF_COUNT_Pos 0            /**< \brief (PEVC_PARAMETER) Number of Input Glitch Filters */
#define PEVC_PARAMETER_IGF_COUNT_Msk (0xFFu << PEVC_PARAMETER_IGF_COUNT_Pos)
#define PEVC_PARAMETER_IGF_COUNT(value) ((PEVC_PARAMETER_IGF_COUNT_Msk & ((value) << PEVC_PARAMETER_IGF_COUNT_Pos)))
#define PEVC_PARAMETER_EVS_COUNT_Pos 8            /**< \brief (PEVC_PARAMETER) Number of Event Shapers */
#define PEVC_PARAMETER_EVS_COUNT_Msk (0xFFu << PEVC_PARAMETER_EVS_COUNT_Pos)
#define PEVC_PARAMETER_EVS_COUNT(value) ((PEVC_PARAMETER_EVS_COUNT_Msk & ((value) << PEVC_PARAMETER_EVS_COUNT_Pos)))
#define PEVC_PARAMETER_EVIN_Pos     16           /**< \brief (PEVC_PARAMETER) Number of Event Inputs / Generators */
#define PEVC_PARAMETER_EVIN_Msk     (0xFFu << PEVC_PARAMETER_EVIN_Pos)
#define PEVC_PARAMETER_EVIN(value)  ((PEVC_PARAMETER_EVIN_Msk & ((value) << PEVC_PARAMETER_EVIN_Pos)))
#define PEVC_PARAMETER_TRIGOUT_Pos  24           /**< \brief (PEVC_PARAMETER) Number of Trigger Outputs / Channels / Users */
#define PEVC_PARAMETER_TRIGOUT_Msk  (0xFFu << PEVC_PARAMETER_TRIGOUT_Pos)
#define PEVC_PARAMETER_TRIGOUT(value) ((PEVC_PARAMETER_TRIGOUT_Msk & ((value) << PEVC_PARAMETER_TRIGOUT_Pos)))
#define PEVC_PARAMETER_MASK         0xFFFFFFFFu  /**< \brief (PEVC_PARAMETER) MASK Register */

/* -------- PEVC_VERSION : (PEVC Offset: 0x3FC) (R/  32) Version -------- */
#define PEVC_VERSION_OFFSET         0x3FC        /**< \brief (PEVC_VERSION offset) Version */
#define PEVC_VERSION_RESETVALUE     0x00000200   /**< \brief (PEVC_VERSION reset_value) Version */

#define PEVC_VERSION_VERSION_Pos    0            /**< \brief (PEVC_VERSION) Version Number */
#define PEVC_VERSION_VERSION_Msk    (0xFFFu << PEVC_VERSION_VERSION_Pos)
#define PEVC_VERSION_VERSION(value) ((PEVC_VERSION_VERSION_Msk & ((value) << PEVC_VERSION_VERSION_Pos)))
#define PEVC_VERSION_VARIANT_Pos    16           /**< \brief (PEVC_VERSION) Variant Number */
#define PEVC_VERSION_VARIANT_Msk    (0xFu << PEVC_VERSION_VARIANT_Pos)
#define PEVC_VERSION_VARIANT(value) ((PEVC_VERSION_VARIANT_Msk & ((value) << PEVC_VERSION_VARIANT_Pos)))
#define PEVC_VERSION_MASK           0x000F0FFFu  /**< \brief (PEVC_VERSION) MASK Register */

/** \brief PevcChmx hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   PEVC_CHMX;          /**< \brief (PEVC Offset: 0x000) Channel Multiplexer */
} PevcChmx;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief PevcEvs hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   PEVC_EVS;           /**< \brief (PEVC Offset: 0x000) Event Shaper */
} PevcEvs;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief PEVC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RoReg   PEVC_CHSR;          /**< \brief (PEVC Offset: 0x000) Channel Status Register */
  WoReg   PEVC_CHER;          /**< \brief (PEVC Offset: 0x004) Channel Enable Register */
  WoReg   PEVC_CHDR;          /**< \brief (PEVC Offset: 0x008) Channel Disable Register */
  RoReg8  Reserved1[0x4];
  WoReg   PEVC_SEV;           /**< \brief (PEVC Offset: 0x010) Software Event */
  RoReg   PEVC_BUSY;          /**< \brief (PEVC Offset: 0x014) Channel / User Busy */
  RoReg8  Reserved2[0x8];
  WoReg   PEVC_TRIER;         /**< \brief (PEVC Offset: 0x020) Trigger Interrupt Mask Enable Register */
  WoReg   PEVC_TRIDR;         /**< \brief (PEVC Offset: 0x024) Trigger Interrupt Mask Disable Register */
  RoReg   PEVC_TRIMR;         /**< \brief (PEVC Offset: 0x028) Trigger Interrupt Mask Register */
  RoReg8  Reserved3[0x4];
  RoReg   PEVC_TRSR;          /**< \brief (PEVC Offset: 0x030) Trigger Status Register */
  WoReg   PEVC_TRSCR;         /**< \brief (PEVC Offset: 0x034) Trigger Status Clear Register */
  RoReg8  Reserved4[0x8];
  WoReg   PEVC_OVIER;         /**< \brief (PEVC Offset: 0x040) Overrun Interrupt Mask Enable Register */
  WoReg   PEVC_OVIDR;         /**< \brief (PEVC Offset: 0x044) Overrun Interrupt Mask Disable Register */
  RoReg   PEVC_OVIMR;         /**< \brief (PEVC Offset: 0x048) Overrun Interrupt Mask Register */
  RoReg8  Reserved5[0x4];
  RoReg   PEVC_OVSR;          /**< \brief (PEVC Offset: 0x050) Overrun Status Register */
  WoReg   PEVC_OVSCR;         /**< \brief (PEVC Offset: 0x054) Overrun Status Clear Register */
  RoReg8  Reserved6[0xA8];
  PevcChmx PEVC_CHMX[19];      /**< \brief (PEVC Offset: 0x100) PevcChmx groups [TRIGOUT_BITS] */
  RoReg8  Reserved7[0xB4];
  PevcEvs PEVC_EVS[31];       /**< \brief (PEVC Offset: 0x200) PevcEvs groups [EVIN_BITS] */
  RoReg8  Reserved8[0x84];
  RwReg   PEVC_IGFDR;         /**< \brief (PEVC Offset: 0x300) Input Glitch Filter Divider Register */
  RoReg8  Reserved9[0xF4];
  RoReg   PEVC_PARAMETER;     /**< \brief (PEVC Offset: 0x3F8) Parameter */
  RoReg   PEVC_VERSION;       /**< \brief (PEVC Offset: 0x3FC) Version */
} Pevc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_PEVC_COMPONENT_ */
