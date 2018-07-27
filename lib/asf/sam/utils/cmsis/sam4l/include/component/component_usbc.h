/**
 * \file
 *
 * \brief Component description for USBC
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

#ifndef _SAM4L_USBC_COMPONENT_
#define _SAM4L_USBC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR USBC */
/* ========================================================================== */
/** \addtogroup SAM4L_USBC USB 2.0 Interface */
/*@{*/

#define REV_USBC                    0x310

/* -------- USBC_UDCON : (USBC Offset: 0x000) (R/W 32) Device General Control Register -------- */
#define USBC_UDCON_OFFSET           0x000        /**< \brief (USBC_UDCON offset) Device General Control Register */
#define USBC_UDCON_RESETVALUE       0x00000100   /**< \brief (USBC_UDCON reset_value) Device General Control Register */

#define USBC_UDCON_UADD_Pos         0            /**< \brief (USBC_UDCON) USB Address */
#define USBC_UDCON_UADD_Msk         (0x7Fu << USBC_UDCON_UADD_Pos)
#define USBC_UDCON_UADD(value)      ((USBC_UDCON_UADD_Msk & ((value) << USBC_UDCON_UADD_Pos)))
#define USBC_UDCON_ADDEN_Pos        7            /**< \brief (USBC_UDCON) Address Enable */
#define USBC_UDCON_ADDEN            (0x1u << USBC_UDCON_ADDEN_Pos)
#define USBC_UDCON_DETACH_Pos       8            /**< \brief (USBC_UDCON) Detach */
#define USBC_UDCON_DETACH           (0x1u << USBC_UDCON_DETACH_Pos)
#define USBC_UDCON_RMWKUP_Pos       9            /**< \brief (USBC_UDCON) Remote Wake-Up */
#define USBC_UDCON_RMWKUP           (0x1u << USBC_UDCON_RMWKUP_Pos)
#define USBC_UDCON_SPDCONF_Pos      10           /**< \brief (USBC_UDCON) Speed configuration */
#define USBC_UDCON_SPDCONF_Msk      (0x3u << USBC_UDCON_SPDCONF_Pos)
#define USBC_UDCON_SPDCONF(value)   ((USBC_UDCON_SPDCONF_Msk & ((value) << USBC_UDCON_SPDCONF_Pos)))
#define USBC_UDCON_LS_Pos           12           /**< \brief (USBC_UDCON) Low Speed Mode Force */
#define USBC_UDCON_LS               (0x1u << USBC_UDCON_LS_Pos)
#define USBC_UDCON_TSTJ_Pos         13           /**< \brief (USBC_UDCON) Test mode J */
#define USBC_UDCON_TSTJ             (0x1u << USBC_UDCON_TSTJ_Pos)
#define USBC_UDCON_TSTK_Pos         14           /**< \brief (USBC_UDCON) Test mode K */
#define USBC_UDCON_TSTK             (0x1u << USBC_UDCON_TSTK_Pos)
#define USBC_UDCON_TSTPCKT_Pos      15           /**< \brief (USBC_UDCON) Test Packet mode */
#define USBC_UDCON_TSTPCKT          (0x1u << USBC_UDCON_TSTPCKT_Pos)
#define USBC_UDCON_OPMODE2_Pos      16           /**< \brief (USBC_UDCON) Specific Operational mode */
#define USBC_UDCON_OPMODE2          (0x1u << USBC_UDCON_OPMODE2_Pos)
#define USBC_UDCON_GNAK_Pos         17           /**< \brief (USBC_UDCON) Global NAK */
#define USBC_UDCON_GNAK             (0x1u << USBC_UDCON_GNAK_Pos)
#define USBC_UDCON_MASK             0x0003FFFFu  /**< \brief (USBC_UDCON) MASK Register */

/* -------- USBC_UDINT : (USBC Offset: 0x004) (R/  32) Device Global Interupt Register -------- */
#define USBC_UDINT_OFFSET           0x004        /**< \brief (USBC_UDINT offset) Device Global Interupt Register */
#define USBC_UDINT_RESETVALUE       0x00000000   /**< \brief (USBC_UDINT reset_value) Device Global Interupt Register */

#define USBC_UDINT_SUSP_Pos         0            /**< \brief (USBC_UDINT) Suspend Interrupt */
#define USBC_UDINT_SUSP             (0x1u << USBC_UDINT_SUSP_Pos)
#define USBC_UDINT_MSOF_Pos         1            /**< \brief (USBC_UDINT) Micro Start of Frame Interrupt */
#define USBC_UDINT_MSOF             (0x1u << USBC_UDINT_MSOF_Pos)
#define USBC_UDINT_SOF_Pos          2            /**< \brief (USBC_UDINT) Start of Frame Interrupt */
#define USBC_UDINT_SOF              (0x1u << USBC_UDINT_SOF_Pos)
#define USBC_UDINT_EORST_Pos        3            /**< \brief (USBC_UDINT) End of Reset Interrupt */
#define USBC_UDINT_EORST            (0x1u << USBC_UDINT_EORST_Pos)
#define USBC_UDINT_WAKEUP_Pos       4            /**< \brief (USBC_UDINT) Wake-Up Interrupt */
#define USBC_UDINT_WAKEUP           (0x1u << USBC_UDINT_WAKEUP_Pos)
#define USBC_UDINT_EORSM_Pos        5            /**< \brief (USBC_UDINT) End Of Resume Interrupt */
#define USBC_UDINT_EORSM            (0x1u << USBC_UDINT_EORSM_Pos)
#define USBC_UDINT_UPRSM_Pos        6            /**< \brief (USBC_UDINT) Upstream Resume Interrupt */
#define USBC_UDINT_UPRSM            (0x1u << USBC_UDINT_UPRSM_Pos)
#define USBC_UDINT_EP0INT_Pos       12           /**< \brief (USBC_UDINT) Endpoint 0 Interrupt */
#define USBC_UDINT_EP0INT           (0x1u << USBC_UDINT_EP0INT_Pos)
#define USBC_UDINT_EP1INT_Pos       13           /**< \brief (USBC_UDINT) Endpoint 1 Interrupt */
#define USBC_UDINT_EP1INT           (0x1u << USBC_UDINT_EP1INT_Pos)
#define USBC_UDINT_EP2INT_Pos       14           /**< \brief (USBC_UDINT) Endpoint 2 Interrupt */
#define USBC_UDINT_EP2INT           (0x1u << USBC_UDINT_EP2INT_Pos)
#define USBC_UDINT_EP3INT_Pos       15           /**< \brief (USBC_UDINT) Endpoint 3 Interrupt */
#define USBC_UDINT_EP3INT           (0x1u << USBC_UDINT_EP3INT_Pos)
#define USBC_UDINT_EP4INT_Pos       16           /**< \brief (USBC_UDINT) Endpoint 4 Interrupt */
#define USBC_UDINT_EP4INT           (0x1u << USBC_UDINT_EP4INT_Pos)
#define USBC_UDINT_EP5INT_Pos       17           /**< \brief (USBC_UDINT) Endpoint 5 Interrupt */
#define USBC_UDINT_EP5INT           (0x1u << USBC_UDINT_EP5INT_Pos)
#define USBC_UDINT_EP6INT_Pos       18           /**< \brief (USBC_UDINT) Endpoint 6 Interrupt */
#define USBC_UDINT_EP6INT           (0x1u << USBC_UDINT_EP6INT_Pos)
#define USBC_UDINT_EP7INT_Pos       19           /**< \brief (USBC_UDINT) Endpoint 7 Interrupt */
#define USBC_UDINT_EP7INT           (0x1u << USBC_UDINT_EP7INT_Pos)
#define USBC_UDINT_MASK             0x000FF07Fu  /**< \brief (USBC_UDINT) MASK Register */

/* -------- USBC_UDINTCLR : (USBC Offset: 0x008) ( /W 32) Device Global Interrupt Clear Register -------- */
#define USBC_UDINTCLR_OFFSET        0x008        /**< \brief (USBC_UDINTCLR offset) Device Global Interrupt Clear Register */
#define USBC_UDINTCLR_RESETVALUE    0x00000000   /**< \brief (USBC_UDINTCLR reset_value) Device Global Interrupt Clear Register */

#define USBC_UDINTCLR_SUSPC_Pos     0            /**< \brief (USBC_UDINTCLR) SUSP Interrupt Clear */
#define USBC_UDINTCLR_SUSPC         (0x1u << USBC_UDINTCLR_SUSPC_Pos)
#define USBC_UDINTCLR_MSOFC_Pos     1            /**< \brief (USBC_UDINTCLR) MSOF Interrupt Clear */
#define USBC_UDINTCLR_MSOFC         (0x1u << USBC_UDINTCLR_MSOFC_Pos)
#define USBC_UDINTCLR_SOFC_Pos      2            /**< \brief (USBC_UDINTCLR) SOF Interrupt Clear */
#define USBC_UDINTCLR_SOFC          (0x1u << USBC_UDINTCLR_SOFC_Pos)
#define USBC_UDINTCLR_EORSTC_Pos    3            /**< \brief (USBC_UDINTCLR) EORST Interrupt Clear */
#define USBC_UDINTCLR_EORSTC        (0x1u << USBC_UDINTCLR_EORSTC_Pos)
#define USBC_UDINTCLR_WAKEUPC_Pos   4            /**< \brief (USBC_UDINTCLR) WAKEUP Interrupt Clear */
#define USBC_UDINTCLR_WAKEUPC       (0x1u << USBC_UDINTCLR_WAKEUPC_Pos)
#define USBC_UDINTCLR_EORSMC_Pos    5            /**< \brief (USBC_UDINTCLR) EORSM Interrupt Clear */
#define USBC_UDINTCLR_EORSMC        (0x1u << USBC_UDINTCLR_EORSMC_Pos)
#define USBC_UDINTCLR_UPRSMC_Pos    6            /**< \brief (USBC_UDINTCLR) UPRSM Interrupt Clear */
#define USBC_UDINTCLR_UPRSMC        (0x1u << USBC_UDINTCLR_UPRSMC_Pos)
#define USBC_UDINTCLR_MASK          0x0000007Fu  /**< \brief (USBC_UDINTCLR) MASK Register */

/* -------- USBC_UDINTSET : (USBC Offset: 0x00C) ( /W 32) Device Global Interrupt Set Regsiter -------- */
#define USBC_UDINTSET_OFFSET        0x00C        /**< \brief (USBC_UDINTSET offset) Device Global Interrupt Set Regsiter */
#define USBC_UDINTSET_RESETVALUE    0x00000000   /**< \brief (USBC_UDINTSET reset_value) Device Global Interrupt Set Regsiter */

#define USBC_UDINTSET_SUSPS_Pos     0            /**< \brief (USBC_UDINTSET) SUSP Interrupt Set */
#define USBC_UDINTSET_SUSPS         (0x1u << USBC_UDINTSET_SUSPS_Pos)
#define USBC_UDINTSET_MSOFS_Pos     1            /**< \brief (USBC_UDINTSET) MSOF Interrupt Set */
#define USBC_UDINTSET_MSOFS         (0x1u << USBC_UDINTSET_MSOFS_Pos)
#define USBC_UDINTSET_SOFS_Pos      2            /**< \brief (USBC_UDINTSET) SOF Interrupt Set */
#define USBC_UDINTSET_SOFS          (0x1u << USBC_UDINTSET_SOFS_Pos)
#define USBC_UDINTSET_EORSTS_Pos    3            /**< \brief (USBC_UDINTSET) EORST Interrupt Set */
#define USBC_UDINTSET_EORSTS        (0x1u << USBC_UDINTSET_EORSTS_Pos)
#define USBC_UDINTSET_WAKEUPS_Pos   4            /**< \brief (USBC_UDINTSET) WAKEUP Interrupt Set */
#define USBC_UDINTSET_WAKEUPS       (0x1u << USBC_UDINTSET_WAKEUPS_Pos)
#define USBC_UDINTSET_EORSMS_Pos    5            /**< \brief (USBC_UDINTSET) EORSM Interrupt Set */
#define USBC_UDINTSET_EORSMS        (0x1u << USBC_UDINTSET_EORSMS_Pos)
#define USBC_UDINTSET_UPRSMS_Pos    6            /**< \brief (USBC_UDINTSET) UPRSM Interrupt Set */
#define USBC_UDINTSET_UPRSMS        (0x1u << USBC_UDINTSET_UPRSMS_Pos)
#define USBC_UDINTSET_MASK          0x0000007Fu  /**< \brief (USBC_UDINTSET) MASK Register */

/* -------- USBC_UDINTE : (USBC Offset: 0x010) (R/  32) Device Global Interrupt Enable Register -------- */
#define USBC_UDINTE_OFFSET          0x010        /**< \brief (USBC_UDINTE offset) Device Global Interrupt Enable Register */
#define USBC_UDINTE_RESETVALUE      0x00000000   /**< \brief (USBC_UDINTE reset_value) Device Global Interrupt Enable Register */

#define USBC_UDINTE_SUSPE_Pos       0            /**< \brief (USBC_UDINTE) SUSP Interrupt Enable */
#define USBC_UDINTE_SUSPE           (0x1u << USBC_UDINTE_SUSPE_Pos)
#define USBC_UDINTE_MSOFE_Pos       1            /**< \brief (USBC_UDINTE) MSOF Interrupt Enable */
#define USBC_UDINTE_MSOFE           (0x1u << USBC_UDINTE_MSOFE_Pos)
#define USBC_UDINTE_SOFE_Pos        2            /**< \brief (USBC_UDINTE) SOF Interrupt Enable */
#define USBC_UDINTE_SOFE            (0x1u << USBC_UDINTE_SOFE_Pos)
#define USBC_UDINTE_EORSTE_Pos      3            /**< \brief (USBC_UDINTE) EORST Interrupt Enable */
#define USBC_UDINTE_EORSTE          (0x1u << USBC_UDINTE_EORSTE_Pos)
#define USBC_UDINTE_WAKEUPE_Pos     4            /**< \brief (USBC_UDINTE) WAKEUP Interrupt Enable */
#define USBC_UDINTE_WAKEUPE         (0x1u << USBC_UDINTE_WAKEUPE_Pos)
#define USBC_UDINTE_EORSME_Pos      5            /**< \brief (USBC_UDINTE) EORSM Interrupt Enable */
#define USBC_UDINTE_EORSME          (0x1u << USBC_UDINTE_EORSME_Pos)
#define USBC_UDINTE_UPRSME_Pos      6            /**< \brief (USBC_UDINTE) UPRSM Interrupt Enable */
#define USBC_UDINTE_UPRSME          (0x1u << USBC_UDINTE_UPRSME_Pos)
#define USBC_UDINTE_EP0INTE_Pos     12           /**< \brief (USBC_UDINTE) EP0INT Interrupt Enable */
#define USBC_UDINTE_EP0INTE         (0x1u << USBC_UDINTE_EP0INTE_Pos)
#define USBC_UDINTE_EP1INTE_Pos     13           /**< \brief (USBC_UDINTE) EP1INT Interrupt Enable */
#define USBC_UDINTE_EP1INTE         (0x1u << USBC_UDINTE_EP1INTE_Pos)
#define USBC_UDINTE_EP2INTE_Pos     14           /**< \brief (USBC_UDINTE) EP2INT Interrupt Enable */
#define USBC_UDINTE_EP2INTE         (0x1u << USBC_UDINTE_EP2INTE_Pos)
#define USBC_UDINTE_EP3INTE_Pos     15           /**< \brief (USBC_UDINTE) EP3INT Interrupt Enable */
#define USBC_UDINTE_EP3INTE         (0x1u << USBC_UDINTE_EP3INTE_Pos)
#define USBC_UDINTE_EP4INTE_Pos     16           /**< \brief (USBC_UDINTE) EP4INT Interrupt Enable */
#define USBC_UDINTE_EP4INTE         (0x1u << USBC_UDINTE_EP4INTE_Pos)
#define USBC_UDINTE_EP5INTE_Pos     17           /**< \brief (USBC_UDINTE) EP5INT Interrupt Enable */
#define USBC_UDINTE_EP5INTE         (0x1u << USBC_UDINTE_EP5INTE_Pos)
#define USBC_UDINTE_EP6INTE_Pos     18           /**< \brief (USBC_UDINTE) EP6INT Interrupt Enable */
#define USBC_UDINTE_EP6INTE         (0x1u << USBC_UDINTE_EP6INTE_Pos)
#define USBC_UDINTE_EP7INTE_Pos     19           /**< \brief (USBC_UDINTE) EP7INT Interrupt Enable */
#define USBC_UDINTE_EP7INTE         (0x1u << USBC_UDINTE_EP7INTE_Pos)
#define USBC_UDINTE_MASK            0x000FF07Fu  /**< \brief (USBC_UDINTE) MASK Register */

/* -------- USBC_UDINTECLR : (USBC Offset: 0x014) ( /W 32) Device Global Interrupt Enable Clear Register -------- */
#define USBC_UDINTECLR_OFFSET       0x014        /**< \brief (USBC_UDINTECLR offset) Device Global Interrupt Enable Clear Register */
#define USBC_UDINTECLR_RESETVALUE   0x00000000   /**< \brief (USBC_UDINTECLR reset_value) Device Global Interrupt Enable Clear Register */

#define USBC_UDINTECLR_SUSPEC_Pos   0            /**< \brief (USBC_UDINTECLR) SUSP Interrupt Enable Clear */
#define USBC_UDINTECLR_SUSPEC       (0x1u << USBC_UDINTECLR_SUSPEC_Pos)
#define USBC_UDINTECLR_MSOFEC_Pos   1            /**< \brief (USBC_UDINTECLR) MSOF Interrupt Enable Clear */
#define USBC_UDINTECLR_MSOFEC       (0x1u << USBC_UDINTECLR_MSOFEC_Pos)
#define USBC_UDINTECLR_SOFEC_Pos    2            /**< \brief (USBC_UDINTECLR) SOF Interrupt Enable Clear */
#define USBC_UDINTECLR_SOFEC        (0x1u << USBC_UDINTECLR_SOFEC_Pos)
#define USBC_UDINTECLR_EORSTEC_Pos  3            /**< \brief (USBC_UDINTECLR) EORST Interrupt Enable Clear */
#define USBC_UDINTECLR_EORSTEC      (0x1u << USBC_UDINTECLR_EORSTEC_Pos)
#define USBC_UDINTECLR_WAKEUPEC_Pos 4            /**< \brief (USBC_UDINTECLR) WAKEUP Interrupt Enable Clear */
#define USBC_UDINTECLR_WAKEUPEC     (0x1u << USBC_UDINTECLR_WAKEUPEC_Pos)
#define USBC_UDINTECLR_EORSMEC_Pos  5            /**< \brief (USBC_UDINTECLR) EORSM Interrupt Enable Clear */
#define USBC_UDINTECLR_EORSMEC      (0x1u << USBC_UDINTECLR_EORSMEC_Pos)
#define USBC_UDINTECLR_UPRSMEC_Pos  6            /**< \brief (USBC_UDINTECLR) UPRSM Interrupt Enable Clear */
#define USBC_UDINTECLR_UPRSMEC      (0x1u << USBC_UDINTECLR_UPRSMEC_Pos)
#define USBC_UDINTECLR_EP0INTEC_Pos 12           /**< \brief (USBC_UDINTECLR) EP0INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP0INTEC     (0x1u << USBC_UDINTECLR_EP0INTEC_Pos)
#define USBC_UDINTECLR_EP1INTEC_Pos 13           /**< \brief (USBC_UDINTECLR) EP1INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP1INTEC     (0x1u << USBC_UDINTECLR_EP1INTEC_Pos)
#define USBC_UDINTECLR_EP2INTEC_Pos 14           /**< \brief (USBC_UDINTECLR) EP2INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP2INTEC     (0x1u << USBC_UDINTECLR_EP2INTEC_Pos)
#define USBC_UDINTECLR_EP3INTEC_Pos 15           /**< \brief (USBC_UDINTECLR) EP3INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP3INTEC     (0x1u << USBC_UDINTECLR_EP3INTEC_Pos)
#define USBC_UDINTECLR_EP4INTEC_Pos 16           /**< \brief (USBC_UDINTECLR) EP4INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP4INTEC     (0x1u << USBC_UDINTECLR_EP4INTEC_Pos)
#define USBC_UDINTECLR_EP5INTEC_Pos 17           /**< \brief (USBC_UDINTECLR) EP5INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP5INTEC     (0x1u << USBC_UDINTECLR_EP5INTEC_Pos)
#define USBC_UDINTECLR_EP6INTEC_Pos 18           /**< \brief (USBC_UDINTECLR) EP6INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP6INTEC     (0x1u << USBC_UDINTECLR_EP6INTEC_Pos)
#define USBC_UDINTECLR_EP7INTEC_Pos 19           /**< \brief (USBC_UDINTECLR) EP7INT Interrupt Enable Clear */
#define USBC_UDINTECLR_EP7INTEC     (0x1u << USBC_UDINTECLR_EP7INTEC_Pos)
#define USBC_UDINTECLR_MASK         0x000FF07Fu  /**< \brief (USBC_UDINTECLR) MASK Register */

/* -------- USBC_UDINTESET : (USBC Offset: 0x018) ( /W 32) Device Global Interrupt Enable Set Register -------- */
#define USBC_UDINTESET_OFFSET       0x018        /**< \brief (USBC_UDINTESET offset) Device Global Interrupt Enable Set Register */
#define USBC_UDINTESET_RESETVALUE   0x00000000   /**< \brief (USBC_UDINTESET reset_value) Device Global Interrupt Enable Set Register */

#define USBC_UDINTESET_SUSPES_Pos   0            /**< \brief (USBC_UDINTESET) SUSP Interrupt Enable Set */
#define USBC_UDINTESET_SUSPES       (0x1u << USBC_UDINTESET_SUSPES_Pos)
#define USBC_UDINTESET_MSOFES_Pos   1            /**< \brief (USBC_UDINTESET) MSOF Interrupt Enable Set */
#define USBC_UDINTESET_MSOFES       (0x1u << USBC_UDINTESET_MSOFES_Pos)
#define USBC_UDINTESET_SOFES_Pos    2            /**< \brief (USBC_UDINTESET) SOF Interrupt Enable Set */
#define USBC_UDINTESET_SOFES        (0x1u << USBC_UDINTESET_SOFES_Pos)
#define USBC_UDINTESET_EORSTES_Pos  3            /**< \brief (USBC_UDINTESET) EORST Interrupt Enable Set */
#define USBC_UDINTESET_EORSTES      (0x1u << USBC_UDINTESET_EORSTES_Pos)
#define USBC_UDINTESET_WAKEUPES_Pos 4            /**< \brief (USBC_UDINTESET) WAKEUP Interrupt Enable Set */
#define USBC_UDINTESET_WAKEUPES     (0x1u << USBC_UDINTESET_WAKEUPES_Pos)
#define USBC_UDINTESET_EORSMES_Pos  5            /**< \brief (USBC_UDINTESET) EORSM Interrupt Enable Set */
#define USBC_UDINTESET_EORSMES      (0x1u << USBC_UDINTESET_EORSMES_Pos)
#define USBC_UDINTESET_UPRSMES_Pos  6            /**< \brief (USBC_UDINTESET) UPRSM Interrupt Enable Set */
#define USBC_UDINTESET_UPRSMES      (0x1u << USBC_UDINTESET_UPRSMES_Pos)
#define USBC_UDINTESET_EP0INTES_Pos 12           /**< \brief (USBC_UDINTESET) EP0INT Interrupt Enable Set */
#define USBC_UDINTESET_EP0INTES     (0x1u << USBC_UDINTESET_EP0INTES_Pos)
#define USBC_UDINTESET_EP1INTES_Pos 13           /**< \brief (USBC_UDINTESET) EP1INT Interrupt Enable Set */
#define USBC_UDINTESET_EP1INTES     (0x1u << USBC_UDINTESET_EP1INTES_Pos)
#define USBC_UDINTESET_EP2INTES_Pos 14           /**< \brief (USBC_UDINTESET) EP2INT Interrupt Enable Set */
#define USBC_UDINTESET_EP2INTES     (0x1u << USBC_UDINTESET_EP2INTES_Pos)
#define USBC_UDINTESET_EP3INTES_Pos 15           /**< \brief (USBC_UDINTESET) EP3INT Interrupt Enable Set */
#define USBC_UDINTESET_EP3INTES     (0x1u << USBC_UDINTESET_EP3INTES_Pos)
#define USBC_UDINTESET_EP4INTES_Pos 16           /**< \brief (USBC_UDINTESET) EP4INT Interrupt Enable Set */
#define USBC_UDINTESET_EP4INTES     (0x1u << USBC_UDINTESET_EP4INTES_Pos)
#define USBC_UDINTESET_EP5INTES_Pos 17           /**< \brief (USBC_UDINTESET) EP5INT Interrupt Enable Set */
#define USBC_UDINTESET_EP5INTES     (0x1u << USBC_UDINTESET_EP5INTES_Pos)
#define USBC_UDINTESET_EP6INTES_Pos 18           /**< \brief (USBC_UDINTESET) EP6INT Interrupt Enable Set */
#define USBC_UDINTESET_EP6INTES     (0x1u << USBC_UDINTESET_EP6INTES_Pos)
#define USBC_UDINTESET_EP7INTES_Pos 19           /**< \brief (USBC_UDINTESET) EP7INT Interrupt Enable Set */
#define USBC_UDINTESET_EP7INTES     (0x1u << USBC_UDINTESET_EP7INTES_Pos)
#define USBC_UDINTESET_MASK         0x000FF07Fu  /**< \brief (USBC_UDINTESET) MASK Register */

/* -------- USBC_UERST : (USBC Offset: 0x01C) (R/W 32) Endpoint Enable/Reset Register -------- */
#define USBC_UERST_OFFSET           0x01C        /**< \brief (USBC_UERST offset) Endpoint Enable/Reset Register */
#define USBC_UERST_RESETVALUE       0x00000000   /**< \brief (USBC_UERST reset_value) Endpoint Enable/Reset Register */

#define USBC_UERST_EPEN0_Pos        0            /**< \brief (USBC_UERST) Endpoint0 Enable */
#define USBC_UERST_EPEN0            (0x1u << USBC_UERST_EPEN0_Pos)
#define USBC_UERST_EPEN1_Pos        1            /**< \brief (USBC_UERST) Endpoint1 Enable */
#define USBC_UERST_EPEN1            (0x1u << USBC_UERST_EPEN1_Pos)
#define USBC_UERST_EPEN2_Pos        2            /**< \brief (USBC_UERST) Endpoint2 Enable */
#define USBC_UERST_EPEN2            (0x1u << USBC_UERST_EPEN2_Pos)
#define USBC_UERST_EPEN3_Pos        3            /**< \brief (USBC_UERST) Endpoint3 Enable */
#define USBC_UERST_EPEN3            (0x1u << USBC_UERST_EPEN3_Pos)
#define USBC_UERST_EPEN4_Pos        4            /**< \brief (USBC_UERST) Endpoint4 Enable */
#define USBC_UERST_EPEN4            (0x1u << USBC_UERST_EPEN4_Pos)
#define USBC_UERST_EPEN5_Pos        5            /**< \brief (USBC_UERST) Endpoint5 Enable */
#define USBC_UERST_EPEN5            (0x1u << USBC_UERST_EPEN5_Pos)
#define USBC_UERST_EPEN6_Pos        6            /**< \brief (USBC_UERST) Endpoint6 Enable */
#define USBC_UERST_EPEN6            (0x1u << USBC_UERST_EPEN6_Pos)
#define USBC_UERST_EPEN7_Pos        7            /**< \brief (USBC_UERST) Endpoint7 Enable */
#define USBC_UERST_EPEN7            (0x1u << USBC_UERST_EPEN7_Pos)
#define USBC_UERST_MASK             0x000000FFu  /**< \brief (USBC_UERST) MASK Register */

/* -------- USBC_UDFNUM : (USBC Offset: 0x020) (R/  32) Device Frame Number Register -------- */
#define USBC_UDFNUM_OFFSET          0x020        /**< \brief (USBC_UDFNUM offset) Device Frame Number Register */
#define USBC_UDFNUM_RESETVALUE      0x00000000   /**< \brief (USBC_UDFNUM reset_value) Device Frame Number Register */

#define USBC_UDFNUM_MFNUM_Pos       0            /**< \brief (USBC_UDFNUM) Micro Frame Number */
#define USBC_UDFNUM_MFNUM_Msk       (0x7u << USBC_UDFNUM_MFNUM_Pos)
#define USBC_UDFNUM_MFNUM(value)    ((USBC_UDFNUM_MFNUM_Msk & ((value) << USBC_UDFNUM_MFNUM_Pos)))
#define USBC_UDFNUM_FNUM_Pos        3            /**< \brief (USBC_UDFNUM) Frame Number */
#define USBC_UDFNUM_FNUM_Msk        (0x7FFu << USBC_UDFNUM_FNUM_Pos)
#define USBC_UDFNUM_FNUM(value)     ((USBC_UDFNUM_FNUM_Msk & ((value) << USBC_UDFNUM_FNUM_Pos)))
#define USBC_UDFNUM_FNCERR_Pos      15           /**< \brief (USBC_UDFNUM) Frame Number CRC Error */
#define USBC_UDFNUM_FNCERR          (0x1u << USBC_UDFNUM_FNCERR_Pos)
#define USBC_UDFNUM_MASK            0x0000BFFFu  /**< \brief (USBC_UDFNUM) MASK Register */

/* -------- USBC_UECFG0 : (USBC Offset: 0x100) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG0_OFFSET          0x100        /**< \brief (USBC_UECFG0 offset) Endpoint Configuration Register */
#define USBC_UECFG0_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG0 reset_value) Endpoint Configuration Register */

#define USBC_UECFG0_EPBK_Pos        2            /**< \brief (USBC_UECFG0) Endpoint Bank */
#define USBC_UECFG0_EPBK            (0x1u << USBC_UECFG0_EPBK_Pos)
#define   USBC_UECFG0_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG0)  */
#define USBC_UECFG0_EPSIZE_Pos      4            /**< \brief (USBC_UECFG0) Endpoint Size */
#define USBC_UECFG0_EPSIZE_Msk      (0x7u << USBC_UECFG0_EPSIZE_Pos)
#define USBC_UECFG0_EPSIZE(value)   ((USBC_UECFG0_EPSIZE_Msk & ((value) << USBC_UECFG0_EPSIZE_Pos)))
#define   USBC_UECFG0_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG0)  */
#define USBC_UECFG0_EPDIR_Pos       8            /**< \brief (USBC_UECFG0) Endpoint Direction */
#define USBC_UECFG0_EPDIR           (0x1u << USBC_UECFG0_EPDIR_Pos)
#define   USBC_UECFG0_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG0)  */
#define USBC_UECFG0_EPTYPE_Pos      11           /**< \brief (USBC_UECFG0) Endpoint Type */
#define USBC_UECFG0_EPTYPE_Msk      (0x3u << USBC_UECFG0_EPTYPE_Pos)
#define USBC_UECFG0_EPTYPE(value)   ((USBC_UECFG0_EPTYPE_Msk & ((value) << USBC_UECFG0_EPTYPE_Pos)))
#define   USBC_UECFG0_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG0)  */
#define   USBC_UECFG0_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG0)  */
#define USBC_UECFG0_REPNB_Pos       16           /**< \brief (USBC_UECFG0) Redirected Endpoint Number */
#define USBC_UECFG0_REPNB_Msk       (0xFu << USBC_UECFG0_REPNB_Pos)
#define USBC_UECFG0_REPNB(value)    ((USBC_UECFG0_REPNB_Msk & ((value) << USBC_UECFG0_REPNB_Pos)))
#define USBC_UECFG0_MASK            0x000F1974u  /**< \brief (USBC_UECFG0) MASK Register */

/* -------- USBC_UECFG1 : (USBC Offset: 0x104) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG1_OFFSET          0x104        /**< \brief (USBC_UECFG1 offset) Endpoint Configuration Register */
#define USBC_UECFG1_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG1 reset_value) Endpoint Configuration Register */

#define USBC_UECFG1_EPBK_Pos        2            /**< \brief (USBC_UECFG1) Endpoint Bank */
#define USBC_UECFG1_EPBK            (0x1u << USBC_UECFG1_EPBK_Pos)
#define   USBC_UECFG1_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG1)  */
#define USBC_UECFG1_EPSIZE_Pos      4            /**< \brief (USBC_UECFG1) Endpoint Size */
#define USBC_UECFG1_EPSIZE_Msk      (0x7u << USBC_UECFG1_EPSIZE_Pos)
#define USBC_UECFG1_EPSIZE(value)   ((USBC_UECFG1_EPSIZE_Msk & ((value) << USBC_UECFG1_EPSIZE_Pos)))
#define   USBC_UECFG1_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG1)  */
#define USBC_UECFG1_EPDIR_Pos       8            /**< \brief (USBC_UECFG1) Endpoint Direction */
#define USBC_UECFG1_EPDIR           (0x1u << USBC_UECFG1_EPDIR_Pos)
#define   USBC_UECFG1_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG1)  */
#define USBC_UECFG1_EPTYPE_Pos      11           /**< \brief (USBC_UECFG1) Endpoint Type */
#define USBC_UECFG1_EPTYPE_Msk      (0x3u << USBC_UECFG1_EPTYPE_Pos)
#define USBC_UECFG1_EPTYPE(value)   ((USBC_UECFG1_EPTYPE_Msk & ((value) << USBC_UECFG1_EPTYPE_Pos)))
#define   USBC_UECFG1_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG1)  */
#define   USBC_UECFG1_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG1)  */
#define USBC_UECFG1_REPNB_Pos       16           /**< \brief (USBC_UECFG1) Redirected Endpoint Number */
#define USBC_UECFG1_REPNB_Msk       (0xFu << USBC_UECFG1_REPNB_Pos)
#define USBC_UECFG1_REPNB(value)    ((USBC_UECFG1_REPNB_Msk & ((value) << USBC_UECFG1_REPNB_Pos)))
#define USBC_UECFG1_MASK            0x000F1974u  /**< \brief (USBC_UECFG1) MASK Register */

/* -------- USBC_UECFG2 : (USBC Offset: 0x108) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG2_OFFSET          0x108        /**< \brief (USBC_UECFG2 offset) Endpoint Configuration Register */
#define USBC_UECFG2_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG2 reset_value) Endpoint Configuration Register */

#define USBC_UECFG2_EPBK_Pos        2            /**< \brief (USBC_UECFG2) Endpoint Bank */
#define USBC_UECFG2_EPBK            (0x1u << USBC_UECFG2_EPBK_Pos)
#define   USBC_UECFG2_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG2)  */
#define USBC_UECFG2_EPSIZE_Pos      4            /**< \brief (USBC_UECFG2) Endpoint Size */
#define USBC_UECFG2_EPSIZE_Msk      (0x7u << USBC_UECFG2_EPSIZE_Pos)
#define USBC_UECFG2_EPSIZE(value)   ((USBC_UECFG2_EPSIZE_Msk & ((value) << USBC_UECFG2_EPSIZE_Pos)))
#define   USBC_UECFG2_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG2)  */
#define USBC_UECFG2_EPDIR_Pos       8            /**< \brief (USBC_UECFG2) Endpoint Direction */
#define USBC_UECFG2_EPDIR           (0x1u << USBC_UECFG2_EPDIR_Pos)
#define   USBC_UECFG2_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG2)  */
#define USBC_UECFG2_EPTYPE_Pos      11           /**< \brief (USBC_UECFG2) Endpoint Type */
#define USBC_UECFG2_EPTYPE_Msk      (0x3u << USBC_UECFG2_EPTYPE_Pos)
#define USBC_UECFG2_EPTYPE(value)   ((USBC_UECFG2_EPTYPE_Msk & ((value) << USBC_UECFG2_EPTYPE_Pos)))
#define   USBC_UECFG2_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG2)  */
#define   USBC_UECFG2_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG2)  */
#define USBC_UECFG2_REPNB_Pos       16           /**< \brief (USBC_UECFG2) Redirected Endpoint Number */
#define USBC_UECFG2_REPNB_Msk       (0xFu << USBC_UECFG2_REPNB_Pos)
#define USBC_UECFG2_REPNB(value)    ((USBC_UECFG2_REPNB_Msk & ((value) << USBC_UECFG2_REPNB_Pos)))
#define USBC_UECFG2_MASK            0x000F1974u  /**< \brief (USBC_UECFG2) MASK Register */

/* -------- USBC_UECFG3 : (USBC Offset: 0x10C) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG3_OFFSET          0x10C        /**< \brief (USBC_UECFG3 offset) Endpoint Configuration Register */
#define USBC_UECFG3_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG3 reset_value) Endpoint Configuration Register */

#define USBC_UECFG3_EPBK_Pos        2            /**< \brief (USBC_UECFG3) Endpoint Bank */
#define USBC_UECFG3_EPBK            (0x1u << USBC_UECFG3_EPBK_Pos)
#define   USBC_UECFG3_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG3)  */
#define USBC_UECFG3_EPSIZE_Pos      4            /**< \brief (USBC_UECFG3) Endpoint Size */
#define USBC_UECFG3_EPSIZE_Msk      (0x7u << USBC_UECFG3_EPSIZE_Pos)
#define USBC_UECFG3_EPSIZE(value)   ((USBC_UECFG3_EPSIZE_Msk & ((value) << USBC_UECFG3_EPSIZE_Pos)))
#define   USBC_UECFG3_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG3)  */
#define USBC_UECFG3_EPDIR_Pos       8            /**< \brief (USBC_UECFG3) Endpoint Direction */
#define USBC_UECFG3_EPDIR           (0x1u << USBC_UECFG3_EPDIR_Pos)
#define   USBC_UECFG3_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG3)  */
#define USBC_UECFG3_EPTYPE_Pos      11           /**< \brief (USBC_UECFG3) Endpoint Type */
#define USBC_UECFG3_EPTYPE_Msk      (0x3u << USBC_UECFG3_EPTYPE_Pos)
#define USBC_UECFG3_EPTYPE(value)   ((USBC_UECFG3_EPTYPE_Msk & ((value) << USBC_UECFG3_EPTYPE_Pos)))
#define   USBC_UECFG3_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG3)  */
#define   USBC_UECFG3_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG3)  */
#define USBC_UECFG3_REPNB_Pos       16           /**< \brief (USBC_UECFG3) Redirected Endpoint Number */
#define USBC_UECFG3_REPNB_Msk       (0xFu << USBC_UECFG3_REPNB_Pos)
#define USBC_UECFG3_REPNB(value)    ((USBC_UECFG3_REPNB_Msk & ((value) << USBC_UECFG3_REPNB_Pos)))
#define USBC_UECFG3_MASK            0x000F1974u  /**< \brief (USBC_UECFG3) MASK Register */

/* -------- USBC_UECFG4 : (USBC Offset: 0x110) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG4_OFFSET          0x110        /**< \brief (USBC_UECFG4 offset) Endpoint Configuration Register */
#define USBC_UECFG4_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG4 reset_value) Endpoint Configuration Register */

#define USBC_UECFG4_EPBK_Pos        2            /**< \brief (USBC_UECFG4) Endpoint Bank */
#define USBC_UECFG4_EPBK            (0x1u << USBC_UECFG4_EPBK_Pos)
#define   USBC_UECFG4_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG4)  */
#define USBC_UECFG4_EPSIZE_Pos      4            /**< \brief (USBC_UECFG4) Endpoint Size */
#define USBC_UECFG4_EPSIZE_Msk      (0x7u << USBC_UECFG4_EPSIZE_Pos)
#define USBC_UECFG4_EPSIZE(value)   ((USBC_UECFG4_EPSIZE_Msk & ((value) << USBC_UECFG4_EPSIZE_Pos)))
#define   USBC_UECFG4_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG4)  */
#define USBC_UECFG4_EPDIR_Pos       8            /**< \brief (USBC_UECFG4) Endpoint Direction */
#define USBC_UECFG4_EPDIR           (0x1u << USBC_UECFG4_EPDIR_Pos)
#define   USBC_UECFG4_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG4)  */
#define USBC_UECFG4_EPTYPE_Pos      11           /**< \brief (USBC_UECFG4) Endpoint Type */
#define USBC_UECFG4_EPTYPE_Msk      (0x3u << USBC_UECFG4_EPTYPE_Pos)
#define USBC_UECFG4_EPTYPE(value)   ((USBC_UECFG4_EPTYPE_Msk & ((value) << USBC_UECFG4_EPTYPE_Pos)))
#define   USBC_UECFG4_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG4)  */
#define   USBC_UECFG4_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG4)  */
#define USBC_UECFG4_REPNB_Pos       16           /**< \brief (USBC_UECFG4) Redirected Endpoint Number */
#define USBC_UECFG4_REPNB_Msk       (0xFu << USBC_UECFG4_REPNB_Pos)
#define USBC_UECFG4_REPNB(value)    ((USBC_UECFG4_REPNB_Msk & ((value) << USBC_UECFG4_REPNB_Pos)))
#define USBC_UECFG4_MASK            0x000F1974u  /**< \brief (USBC_UECFG4) MASK Register */

/* -------- USBC_UECFG5 : (USBC Offset: 0x114) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG5_OFFSET          0x114        /**< \brief (USBC_UECFG5 offset) Endpoint Configuration Register */
#define USBC_UECFG5_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG5 reset_value) Endpoint Configuration Register */

#define USBC_UECFG5_EPBK_Pos        2            /**< \brief (USBC_UECFG5) Endpoint Bank */
#define USBC_UECFG5_EPBK            (0x1u << USBC_UECFG5_EPBK_Pos)
#define   USBC_UECFG5_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG5)  */
#define USBC_UECFG5_EPSIZE_Pos      4            /**< \brief (USBC_UECFG5) Endpoint Size */
#define USBC_UECFG5_EPSIZE_Msk      (0x7u << USBC_UECFG5_EPSIZE_Pos)
#define USBC_UECFG5_EPSIZE(value)   ((USBC_UECFG5_EPSIZE_Msk & ((value) << USBC_UECFG5_EPSIZE_Pos)))
#define   USBC_UECFG5_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG5)  */
#define USBC_UECFG5_EPDIR_Pos       8            /**< \brief (USBC_UECFG5) Endpoint Direction */
#define USBC_UECFG5_EPDIR           (0x1u << USBC_UECFG5_EPDIR_Pos)
#define   USBC_UECFG5_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG5)  */
#define USBC_UECFG5_EPTYPE_Pos      11           /**< \brief (USBC_UECFG5) Endpoint Type */
#define USBC_UECFG5_EPTYPE_Msk      (0x3u << USBC_UECFG5_EPTYPE_Pos)
#define USBC_UECFG5_EPTYPE(value)   ((USBC_UECFG5_EPTYPE_Msk & ((value) << USBC_UECFG5_EPTYPE_Pos)))
#define   USBC_UECFG5_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG5)  */
#define   USBC_UECFG5_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG5)  */
#define USBC_UECFG5_REPNB_Pos       16           /**< \brief (USBC_UECFG5) Redirected Endpoint Number */
#define USBC_UECFG5_REPNB_Msk       (0xFu << USBC_UECFG5_REPNB_Pos)
#define USBC_UECFG5_REPNB(value)    ((USBC_UECFG5_REPNB_Msk & ((value) << USBC_UECFG5_REPNB_Pos)))
#define USBC_UECFG5_MASK            0x000F1974u  /**< \brief (USBC_UECFG5) MASK Register */

/* -------- USBC_UECFG6 : (USBC Offset: 0x118) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG6_OFFSET          0x118        /**< \brief (USBC_UECFG6 offset) Endpoint Configuration Register */
#define USBC_UECFG6_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG6 reset_value) Endpoint Configuration Register */

#define USBC_UECFG6_EPBK_Pos        2            /**< \brief (USBC_UECFG6) Endpoint Bank */
#define USBC_UECFG6_EPBK            (0x1u << USBC_UECFG6_EPBK_Pos)
#define   USBC_UECFG6_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG6)  */
#define USBC_UECFG6_EPSIZE_Pos      4            /**< \brief (USBC_UECFG6) Endpoint Size */
#define USBC_UECFG6_EPSIZE_Msk      (0x7u << USBC_UECFG6_EPSIZE_Pos)
#define USBC_UECFG6_EPSIZE(value)   ((USBC_UECFG6_EPSIZE_Msk & ((value) << USBC_UECFG6_EPSIZE_Pos)))
#define   USBC_UECFG6_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG6)  */
#define USBC_UECFG6_EPDIR_Pos       8            /**< \brief (USBC_UECFG6) Endpoint Direction */
#define USBC_UECFG6_EPDIR           (0x1u << USBC_UECFG6_EPDIR_Pos)
#define   USBC_UECFG6_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG6)  */
#define USBC_UECFG6_EPTYPE_Pos      11           /**< \brief (USBC_UECFG6) Endpoint Type */
#define USBC_UECFG6_EPTYPE_Msk      (0x3u << USBC_UECFG6_EPTYPE_Pos)
#define USBC_UECFG6_EPTYPE(value)   ((USBC_UECFG6_EPTYPE_Msk & ((value) << USBC_UECFG6_EPTYPE_Pos)))
#define   USBC_UECFG6_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG6)  */
#define   USBC_UECFG6_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG6)  */
#define USBC_UECFG6_REPNB_Pos       16           /**< \brief (USBC_UECFG6) Redirected Endpoint Number */
#define USBC_UECFG6_REPNB_Msk       (0xFu << USBC_UECFG6_REPNB_Pos)
#define USBC_UECFG6_REPNB(value)    ((USBC_UECFG6_REPNB_Msk & ((value) << USBC_UECFG6_REPNB_Pos)))
#define USBC_UECFG6_MASK            0x000F1974u  /**< \brief (USBC_UECFG6) MASK Register */

/* -------- USBC_UECFG7 : (USBC Offset: 0x11C) (R/W 32) Endpoint Configuration Register -------- */
#define USBC_UECFG7_OFFSET          0x11C        /**< \brief (USBC_UECFG7 offset) Endpoint Configuration Register */
#define USBC_UECFG7_RESETVALUE      0x00000000   /**< \brief (USBC_UECFG7 reset_value) Endpoint Configuration Register */

#define USBC_UECFG7_EPBK_Pos        2            /**< \brief (USBC_UECFG7) Endpoint Bank */
#define USBC_UECFG7_EPBK            (0x1u << USBC_UECFG7_EPBK_Pos)
#define   USBC_UECFG7_EPBK_SINGLE   (0x0u <<  2) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPBK_DOUBLE   (0x1u <<  2) /**< \brief (USBC_UECFG7)  */
#define USBC_UECFG7_EPSIZE_Pos      4            /**< \brief (USBC_UECFG7) Endpoint Size */
#define USBC_UECFG7_EPSIZE_Msk      (0x7u << USBC_UECFG7_EPSIZE_Pos)
#define USBC_UECFG7_EPSIZE(value)   ((USBC_UECFG7_EPSIZE_Msk & ((value) << USBC_UECFG7_EPSIZE_Pos)))
#define   USBC_UECFG7_EPSIZE_8      (0x0u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_16     (0x1u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_32     (0x2u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_64     (0x3u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_128    (0x4u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_256    (0x5u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_512    (0x6u <<  4) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPSIZE_1024   (0x7u <<  4) /**< \brief (USBC_UECFG7)  */
#define USBC_UECFG7_EPDIR_Pos       8            /**< \brief (USBC_UECFG7) Endpoint Direction */
#define USBC_UECFG7_EPDIR           (0x1u << USBC_UECFG7_EPDIR_Pos)
#define   USBC_UECFG7_EPDIR_OUT     (0x0u <<  8) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPDIR_IN      (0x1u <<  8) /**< \brief (USBC_UECFG7)  */
#define USBC_UECFG7_EPTYPE_Pos      11           /**< \brief (USBC_UECFG7) Endpoint Type */
#define USBC_UECFG7_EPTYPE_Msk      (0x3u << USBC_UECFG7_EPTYPE_Pos)
#define USBC_UECFG7_EPTYPE(value)   ((USBC_UECFG7_EPTYPE_Msk & ((value) << USBC_UECFG7_EPTYPE_Pos)))
#define   USBC_UECFG7_EPTYPE_CONTROL (0x0u << 11) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPTYPE_ISOCHRONOUS (0x1u << 11) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPTYPE_BULK   (0x2u << 11) /**< \brief (USBC_UECFG7)  */
#define   USBC_UECFG7_EPTYPE_INTERRUPT (0x3u << 11) /**< \brief (USBC_UECFG7)  */
#define USBC_UECFG7_REPNB_Pos       16           /**< \brief (USBC_UECFG7) Redirected Endpoint Number */
#define USBC_UECFG7_REPNB_Msk       (0xFu << USBC_UECFG7_REPNB_Pos)
#define USBC_UECFG7_REPNB(value)    ((USBC_UECFG7_REPNB_Msk & ((value) << USBC_UECFG7_REPNB_Pos)))
#define USBC_UECFG7_MASK            0x000F1974u  /**< \brief (USBC_UECFG7) MASK Register */

/* -------- USBC_UESTA0 : (USBC Offset: 0x130) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA0_OFFSET          0x130        /**< \brief (USBC_UESTA0 offset) Endpoint Status Register */
#define USBC_UESTA0_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA0 reset_value) Endpoint Status Register */

#define USBC_UESTA0_TXINI_Pos       0            /**< \brief (USBC_UESTA0) Transmitted IN Data Interrupt */
#define USBC_UESTA0_TXINI           (0x1u << USBC_UESTA0_TXINI_Pos)
#define USBC_UESTA0_RXOUTI_Pos      1            /**< \brief (USBC_UESTA0) Received OUT Data Interrupt */
#define USBC_UESTA0_RXOUTI          (0x1u << USBC_UESTA0_RXOUTI_Pos)
#define USBC_UESTA0_RXSTPI_Pos      2            /**< \brief (USBC_UESTA0) Received SETUP Interrupt */
#define USBC_UESTA0_RXSTPI          (0x1u << USBC_UESTA0_RXSTPI_Pos)
#define USBC_UESTA0_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA0) NAKed OUT Interrupt */
#define USBC_UESTA0_NAKOUTI         (0x1u << USBC_UESTA0_NAKOUTI_Pos)
#define USBC_UESTA0_NAKINI_Pos      4            /**< \brief (USBC_UESTA0) NAKed IN Interrupt */
#define USBC_UESTA0_NAKINI          (0x1u << USBC_UESTA0_NAKINI_Pos)
#define USBC_UESTA0_STALLEDI_Pos    6            /**< \brief (USBC_UESTA0) STALLed Interrupt */
#define USBC_UESTA0_STALLEDI        (0x1u << USBC_UESTA0_STALLEDI_Pos)
#define USBC_UESTA0_DTSEQ_Pos       8            /**< \brief (USBC_UESTA0) Data Toggle Sequence */
#define USBC_UESTA0_DTSEQ_Msk       (0x3u << USBC_UESTA0_DTSEQ_Pos)
#define USBC_UESTA0_DTSEQ(value)    ((USBC_UESTA0_DTSEQ_Msk & ((value) << USBC_UESTA0_DTSEQ_Pos)))
#define USBC_UESTA0_RAMACERI_Pos    11           /**< \brief (USBC_UESTA0) Ram Access Error Interrupt */
#define USBC_UESTA0_RAMACERI        (0x1u << USBC_UESTA0_RAMACERI_Pos)
#define USBC_UESTA0_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA0) Number Of Busy Banks */
#define USBC_UESTA0_NBUSYBK_Msk     (0x3u << USBC_UESTA0_NBUSYBK_Pos)
#define USBC_UESTA0_NBUSYBK(value)  ((USBC_UESTA0_NBUSYBK_Msk & ((value) << USBC_UESTA0_NBUSYBK_Pos)))
#define USBC_UESTA0_CURRBK_Pos      14           /**< \brief (USBC_UESTA0) Current Bank */
#define USBC_UESTA0_CURRBK_Msk      (0x3u << USBC_UESTA0_CURRBK_Pos)
#define USBC_UESTA0_CURRBK(value)   ((USBC_UESTA0_CURRBK_Msk & ((value) << USBC_UESTA0_CURRBK_Pos)))
#define USBC_UESTA0_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA0) Control Direction */
#define USBC_UESTA0_CTRLDIR         (0x1u << USBC_UESTA0_CTRLDIR_Pos)
#define   USBC_UESTA0_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA0)  */
#define   USBC_UESTA0_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA0)  */
#define USBC_UESTA0_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA0) MASK Register */

/* -------- USBC_UESTA1 : (USBC Offset: 0x134) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA1_OFFSET          0x134        /**< \brief (USBC_UESTA1 offset) Endpoint Status Register */
#define USBC_UESTA1_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA1 reset_value) Endpoint Status Register */

#define USBC_UESTA1_TXINI_Pos       0            /**< \brief (USBC_UESTA1) Transmitted IN Data Interrupt */
#define USBC_UESTA1_TXINI           (0x1u << USBC_UESTA1_TXINI_Pos)
#define USBC_UESTA1_RXOUTI_Pos      1            /**< \brief (USBC_UESTA1) Received OUT Data Interrupt */
#define USBC_UESTA1_RXOUTI          (0x1u << USBC_UESTA1_RXOUTI_Pos)
#define USBC_UESTA1_RXSTPI_Pos      2            /**< \brief (USBC_UESTA1) Received SETUP Interrupt */
#define USBC_UESTA1_RXSTPI          (0x1u << USBC_UESTA1_RXSTPI_Pos)
#define USBC_UESTA1_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA1) NAKed OUT Interrupt */
#define USBC_UESTA1_NAKOUTI         (0x1u << USBC_UESTA1_NAKOUTI_Pos)
#define USBC_UESTA1_NAKINI_Pos      4            /**< \brief (USBC_UESTA1) NAKed IN Interrupt */
#define USBC_UESTA1_NAKINI          (0x1u << USBC_UESTA1_NAKINI_Pos)
#define USBC_UESTA1_STALLEDI_Pos    6            /**< \brief (USBC_UESTA1) STALLed Interrupt */
#define USBC_UESTA1_STALLEDI        (0x1u << USBC_UESTA1_STALLEDI_Pos)
#define USBC_UESTA1_DTSEQ_Pos       8            /**< \brief (USBC_UESTA1) Data Toggle Sequence */
#define USBC_UESTA1_DTSEQ_Msk       (0x3u << USBC_UESTA1_DTSEQ_Pos)
#define USBC_UESTA1_DTSEQ(value)    ((USBC_UESTA1_DTSEQ_Msk & ((value) << USBC_UESTA1_DTSEQ_Pos)))
#define USBC_UESTA1_RAMACERI_Pos    11           /**< \brief (USBC_UESTA1) Ram Access Error Interrupt */
#define USBC_UESTA1_RAMACERI        (0x1u << USBC_UESTA1_RAMACERI_Pos)
#define USBC_UESTA1_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA1) Number Of Busy Banks */
#define USBC_UESTA1_NBUSYBK_Msk     (0x3u << USBC_UESTA1_NBUSYBK_Pos)
#define USBC_UESTA1_NBUSYBK(value)  ((USBC_UESTA1_NBUSYBK_Msk & ((value) << USBC_UESTA1_NBUSYBK_Pos)))
#define USBC_UESTA1_CURRBK_Pos      14           /**< \brief (USBC_UESTA1) Current Bank */
#define USBC_UESTA1_CURRBK_Msk      (0x3u << USBC_UESTA1_CURRBK_Pos)
#define USBC_UESTA1_CURRBK(value)   ((USBC_UESTA1_CURRBK_Msk & ((value) << USBC_UESTA1_CURRBK_Pos)))
#define USBC_UESTA1_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA1) Control Direction */
#define USBC_UESTA1_CTRLDIR         (0x1u << USBC_UESTA1_CTRLDIR_Pos)
#define   USBC_UESTA1_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA1)  */
#define   USBC_UESTA1_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA1)  */
#define USBC_UESTA1_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA1) MASK Register */

/* -------- USBC_UESTA2 : (USBC Offset: 0x138) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA2_OFFSET          0x138        /**< \brief (USBC_UESTA2 offset) Endpoint Status Register */
#define USBC_UESTA2_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA2 reset_value) Endpoint Status Register */

#define USBC_UESTA2_TXINI_Pos       0            /**< \brief (USBC_UESTA2) Transmitted IN Data Interrupt */
#define USBC_UESTA2_TXINI           (0x1u << USBC_UESTA2_TXINI_Pos)
#define USBC_UESTA2_RXOUTI_Pos      1            /**< \brief (USBC_UESTA2) Received OUT Data Interrupt */
#define USBC_UESTA2_RXOUTI          (0x1u << USBC_UESTA2_RXOUTI_Pos)
#define USBC_UESTA2_RXSTPI_Pos      2            /**< \brief (USBC_UESTA2) Received SETUP Interrupt */
#define USBC_UESTA2_RXSTPI          (0x1u << USBC_UESTA2_RXSTPI_Pos)
#define USBC_UESTA2_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA2) NAKed OUT Interrupt */
#define USBC_UESTA2_NAKOUTI         (0x1u << USBC_UESTA2_NAKOUTI_Pos)
#define USBC_UESTA2_NAKINI_Pos      4            /**< \brief (USBC_UESTA2) NAKed IN Interrupt */
#define USBC_UESTA2_NAKINI          (0x1u << USBC_UESTA2_NAKINI_Pos)
#define USBC_UESTA2_STALLEDI_Pos    6            /**< \brief (USBC_UESTA2) STALLed Interrupt */
#define USBC_UESTA2_STALLEDI        (0x1u << USBC_UESTA2_STALLEDI_Pos)
#define USBC_UESTA2_DTSEQ_Pos       8            /**< \brief (USBC_UESTA2) Data Toggle Sequence */
#define USBC_UESTA2_DTSEQ_Msk       (0x3u << USBC_UESTA2_DTSEQ_Pos)
#define USBC_UESTA2_DTSEQ(value)    ((USBC_UESTA2_DTSEQ_Msk & ((value) << USBC_UESTA2_DTSEQ_Pos)))
#define USBC_UESTA2_RAMACERI_Pos    11           /**< \brief (USBC_UESTA2) Ram Access Error Interrupt */
#define USBC_UESTA2_RAMACERI        (0x1u << USBC_UESTA2_RAMACERI_Pos)
#define USBC_UESTA2_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA2) Number Of Busy Banks */
#define USBC_UESTA2_NBUSYBK_Msk     (0x3u << USBC_UESTA2_NBUSYBK_Pos)
#define USBC_UESTA2_NBUSYBK(value)  ((USBC_UESTA2_NBUSYBK_Msk & ((value) << USBC_UESTA2_NBUSYBK_Pos)))
#define USBC_UESTA2_CURRBK_Pos      14           /**< \brief (USBC_UESTA2) Current Bank */
#define USBC_UESTA2_CURRBK_Msk      (0x3u << USBC_UESTA2_CURRBK_Pos)
#define USBC_UESTA2_CURRBK(value)   ((USBC_UESTA2_CURRBK_Msk & ((value) << USBC_UESTA2_CURRBK_Pos)))
#define USBC_UESTA2_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA2) Control Direction */
#define USBC_UESTA2_CTRLDIR         (0x1u << USBC_UESTA2_CTRLDIR_Pos)
#define   USBC_UESTA2_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA2)  */
#define   USBC_UESTA2_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA2)  */
#define USBC_UESTA2_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA2) MASK Register */

/* -------- USBC_UESTA3 : (USBC Offset: 0x13C) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA3_OFFSET          0x13C        /**< \brief (USBC_UESTA3 offset) Endpoint Status Register */
#define USBC_UESTA3_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA3 reset_value) Endpoint Status Register */

#define USBC_UESTA3_TXINI_Pos       0            /**< \brief (USBC_UESTA3) Transmitted IN Data Interrupt */
#define USBC_UESTA3_TXINI           (0x1u << USBC_UESTA3_TXINI_Pos)
#define USBC_UESTA3_RXOUTI_Pos      1            /**< \brief (USBC_UESTA3) Received OUT Data Interrupt */
#define USBC_UESTA3_RXOUTI          (0x1u << USBC_UESTA3_RXOUTI_Pos)
#define USBC_UESTA3_RXSTPI_Pos      2            /**< \brief (USBC_UESTA3) Received SETUP Interrupt */
#define USBC_UESTA3_RXSTPI          (0x1u << USBC_UESTA3_RXSTPI_Pos)
#define USBC_UESTA3_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA3) NAKed OUT Interrupt */
#define USBC_UESTA3_NAKOUTI         (0x1u << USBC_UESTA3_NAKOUTI_Pos)
#define USBC_UESTA3_NAKINI_Pos      4            /**< \brief (USBC_UESTA3) NAKed IN Interrupt */
#define USBC_UESTA3_NAKINI          (0x1u << USBC_UESTA3_NAKINI_Pos)
#define USBC_UESTA3_STALLEDI_Pos    6            /**< \brief (USBC_UESTA3) STALLed Interrupt */
#define USBC_UESTA3_STALLEDI        (0x1u << USBC_UESTA3_STALLEDI_Pos)
#define USBC_UESTA3_DTSEQ_Pos       8            /**< \brief (USBC_UESTA3) Data Toggle Sequence */
#define USBC_UESTA3_DTSEQ_Msk       (0x3u << USBC_UESTA3_DTSEQ_Pos)
#define USBC_UESTA3_DTSEQ(value)    ((USBC_UESTA3_DTSEQ_Msk & ((value) << USBC_UESTA3_DTSEQ_Pos)))
#define USBC_UESTA3_RAMACERI_Pos    11           /**< \brief (USBC_UESTA3) Ram Access Error Interrupt */
#define USBC_UESTA3_RAMACERI        (0x1u << USBC_UESTA3_RAMACERI_Pos)
#define USBC_UESTA3_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA3) Number Of Busy Banks */
#define USBC_UESTA3_NBUSYBK_Msk     (0x3u << USBC_UESTA3_NBUSYBK_Pos)
#define USBC_UESTA3_NBUSYBK(value)  ((USBC_UESTA3_NBUSYBK_Msk & ((value) << USBC_UESTA3_NBUSYBK_Pos)))
#define USBC_UESTA3_CURRBK_Pos      14           /**< \brief (USBC_UESTA3) Current Bank */
#define USBC_UESTA3_CURRBK_Msk      (0x3u << USBC_UESTA3_CURRBK_Pos)
#define USBC_UESTA3_CURRBK(value)   ((USBC_UESTA3_CURRBK_Msk & ((value) << USBC_UESTA3_CURRBK_Pos)))
#define USBC_UESTA3_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA3) Control Direction */
#define USBC_UESTA3_CTRLDIR         (0x1u << USBC_UESTA3_CTRLDIR_Pos)
#define   USBC_UESTA3_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA3)  */
#define   USBC_UESTA3_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA3)  */
#define USBC_UESTA3_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA3) MASK Register */

/* -------- USBC_UESTA4 : (USBC Offset: 0x140) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA4_OFFSET          0x140        /**< \brief (USBC_UESTA4 offset) Endpoint Status Register */
#define USBC_UESTA4_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA4 reset_value) Endpoint Status Register */

#define USBC_UESTA4_TXINI_Pos       0            /**< \brief (USBC_UESTA4) Transmitted IN Data Interrupt */
#define USBC_UESTA4_TXINI           (0x1u << USBC_UESTA4_TXINI_Pos)
#define USBC_UESTA4_RXOUTI_Pos      1            /**< \brief (USBC_UESTA4) Received OUT Data Interrupt */
#define USBC_UESTA4_RXOUTI          (0x1u << USBC_UESTA4_RXOUTI_Pos)
#define USBC_UESTA4_RXSTPI_Pos      2            /**< \brief (USBC_UESTA4) Received SETUP Interrupt */
#define USBC_UESTA4_RXSTPI          (0x1u << USBC_UESTA4_RXSTPI_Pos)
#define USBC_UESTA4_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA4) NAKed OUT Interrupt */
#define USBC_UESTA4_NAKOUTI         (0x1u << USBC_UESTA4_NAKOUTI_Pos)
#define USBC_UESTA4_NAKINI_Pos      4            /**< \brief (USBC_UESTA4) NAKed IN Interrupt */
#define USBC_UESTA4_NAKINI          (0x1u << USBC_UESTA4_NAKINI_Pos)
#define USBC_UESTA4_STALLEDI_Pos    6            /**< \brief (USBC_UESTA4) STALLed Interrupt */
#define USBC_UESTA4_STALLEDI        (0x1u << USBC_UESTA4_STALLEDI_Pos)
#define USBC_UESTA4_DTSEQ_Pos       8            /**< \brief (USBC_UESTA4) Data Toggle Sequence */
#define USBC_UESTA4_DTSEQ_Msk       (0x3u << USBC_UESTA4_DTSEQ_Pos)
#define USBC_UESTA4_DTSEQ(value)    ((USBC_UESTA4_DTSEQ_Msk & ((value) << USBC_UESTA4_DTSEQ_Pos)))
#define USBC_UESTA4_RAMACERI_Pos    11           /**< \brief (USBC_UESTA4) Ram Access Error Interrupt */
#define USBC_UESTA4_RAMACERI        (0x1u << USBC_UESTA4_RAMACERI_Pos)
#define USBC_UESTA4_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA4) Number Of Busy Banks */
#define USBC_UESTA4_NBUSYBK_Msk     (0x3u << USBC_UESTA4_NBUSYBK_Pos)
#define USBC_UESTA4_NBUSYBK(value)  ((USBC_UESTA4_NBUSYBK_Msk & ((value) << USBC_UESTA4_NBUSYBK_Pos)))
#define USBC_UESTA4_CURRBK_Pos      14           /**< \brief (USBC_UESTA4) Current Bank */
#define USBC_UESTA4_CURRBK_Msk      (0x3u << USBC_UESTA4_CURRBK_Pos)
#define USBC_UESTA4_CURRBK(value)   ((USBC_UESTA4_CURRBK_Msk & ((value) << USBC_UESTA4_CURRBK_Pos)))
#define USBC_UESTA4_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA4) Control Direction */
#define USBC_UESTA4_CTRLDIR         (0x1u << USBC_UESTA4_CTRLDIR_Pos)
#define   USBC_UESTA4_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA4)  */
#define   USBC_UESTA4_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA4)  */
#define USBC_UESTA4_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA4) MASK Register */

/* -------- USBC_UESTA5 : (USBC Offset: 0x144) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA5_OFFSET          0x144        /**< \brief (USBC_UESTA5 offset) Endpoint Status Register */
#define USBC_UESTA5_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA5 reset_value) Endpoint Status Register */

#define USBC_UESTA5_TXINI_Pos       0            /**< \brief (USBC_UESTA5) Transmitted IN Data Interrupt */
#define USBC_UESTA5_TXINI           (0x1u << USBC_UESTA5_TXINI_Pos)
#define USBC_UESTA5_RXOUTI_Pos      1            /**< \brief (USBC_UESTA5) Received OUT Data Interrupt */
#define USBC_UESTA5_RXOUTI          (0x1u << USBC_UESTA5_RXOUTI_Pos)
#define USBC_UESTA5_RXSTPI_Pos      2            /**< \brief (USBC_UESTA5) Received SETUP Interrupt */
#define USBC_UESTA5_RXSTPI          (0x1u << USBC_UESTA5_RXSTPI_Pos)
#define USBC_UESTA5_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA5) NAKed OUT Interrupt */
#define USBC_UESTA5_NAKOUTI         (0x1u << USBC_UESTA5_NAKOUTI_Pos)
#define USBC_UESTA5_NAKINI_Pos      4            /**< \brief (USBC_UESTA5) NAKed IN Interrupt */
#define USBC_UESTA5_NAKINI          (0x1u << USBC_UESTA5_NAKINI_Pos)
#define USBC_UESTA5_STALLEDI_Pos    6            /**< \brief (USBC_UESTA5) STALLed Interrupt */
#define USBC_UESTA5_STALLEDI        (0x1u << USBC_UESTA5_STALLEDI_Pos)
#define USBC_UESTA5_DTSEQ_Pos       8            /**< \brief (USBC_UESTA5) Data Toggle Sequence */
#define USBC_UESTA5_DTSEQ_Msk       (0x3u << USBC_UESTA5_DTSEQ_Pos)
#define USBC_UESTA5_DTSEQ(value)    ((USBC_UESTA5_DTSEQ_Msk & ((value) << USBC_UESTA5_DTSEQ_Pos)))
#define USBC_UESTA5_RAMACERI_Pos    11           /**< \brief (USBC_UESTA5) Ram Access Error Interrupt */
#define USBC_UESTA5_RAMACERI        (0x1u << USBC_UESTA5_RAMACERI_Pos)
#define USBC_UESTA5_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA5) Number Of Busy Banks */
#define USBC_UESTA5_NBUSYBK_Msk     (0x3u << USBC_UESTA5_NBUSYBK_Pos)
#define USBC_UESTA5_NBUSYBK(value)  ((USBC_UESTA5_NBUSYBK_Msk & ((value) << USBC_UESTA5_NBUSYBK_Pos)))
#define USBC_UESTA5_CURRBK_Pos      14           /**< \brief (USBC_UESTA5) Current Bank */
#define USBC_UESTA5_CURRBK_Msk      (0x3u << USBC_UESTA5_CURRBK_Pos)
#define USBC_UESTA5_CURRBK(value)   ((USBC_UESTA5_CURRBK_Msk & ((value) << USBC_UESTA5_CURRBK_Pos)))
#define USBC_UESTA5_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA5) Control Direction */
#define USBC_UESTA5_CTRLDIR         (0x1u << USBC_UESTA5_CTRLDIR_Pos)
#define   USBC_UESTA5_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA5)  */
#define   USBC_UESTA5_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA5)  */
#define USBC_UESTA5_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA5) MASK Register */

/* -------- USBC_UESTA6 : (USBC Offset: 0x148) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA6_OFFSET          0x148        /**< \brief (USBC_UESTA6 offset) Endpoint Status Register */
#define USBC_UESTA6_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA6 reset_value) Endpoint Status Register */

#define USBC_UESTA6_TXINI_Pos       0            /**< \brief (USBC_UESTA6) Transmitted IN Data Interrupt */
#define USBC_UESTA6_TXINI           (0x1u << USBC_UESTA6_TXINI_Pos)
#define USBC_UESTA6_RXOUTI_Pos      1            /**< \brief (USBC_UESTA6) Received OUT Data Interrupt */
#define USBC_UESTA6_RXOUTI          (0x1u << USBC_UESTA6_RXOUTI_Pos)
#define USBC_UESTA6_RXSTPI_Pos      2            /**< \brief (USBC_UESTA6) Received SETUP Interrupt */
#define USBC_UESTA6_RXSTPI          (0x1u << USBC_UESTA6_RXSTPI_Pos)
#define USBC_UESTA6_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA6) NAKed OUT Interrupt */
#define USBC_UESTA6_NAKOUTI         (0x1u << USBC_UESTA6_NAKOUTI_Pos)
#define USBC_UESTA6_NAKINI_Pos      4            /**< \brief (USBC_UESTA6) NAKed IN Interrupt */
#define USBC_UESTA6_NAKINI          (0x1u << USBC_UESTA6_NAKINI_Pos)
#define USBC_UESTA6_STALLEDI_Pos    6            /**< \brief (USBC_UESTA6) STALLed Interrupt */
#define USBC_UESTA6_STALLEDI        (0x1u << USBC_UESTA6_STALLEDI_Pos)
#define USBC_UESTA6_DTSEQ_Pos       8            /**< \brief (USBC_UESTA6) Data Toggle Sequence */
#define USBC_UESTA6_DTSEQ_Msk       (0x3u << USBC_UESTA6_DTSEQ_Pos)
#define USBC_UESTA6_DTSEQ(value)    ((USBC_UESTA6_DTSEQ_Msk & ((value) << USBC_UESTA6_DTSEQ_Pos)))
#define USBC_UESTA6_RAMACERI_Pos    11           /**< \brief (USBC_UESTA6) Ram Access Error Interrupt */
#define USBC_UESTA6_RAMACERI        (0x1u << USBC_UESTA6_RAMACERI_Pos)
#define USBC_UESTA6_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA6) Number Of Busy Banks */
#define USBC_UESTA6_NBUSYBK_Msk     (0x3u << USBC_UESTA6_NBUSYBK_Pos)
#define USBC_UESTA6_NBUSYBK(value)  ((USBC_UESTA6_NBUSYBK_Msk & ((value) << USBC_UESTA6_NBUSYBK_Pos)))
#define USBC_UESTA6_CURRBK_Pos      14           /**< \brief (USBC_UESTA6) Current Bank */
#define USBC_UESTA6_CURRBK_Msk      (0x3u << USBC_UESTA6_CURRBK_Pos)
#define USBC_UESTA6_CURRBK(value)   ((USBC_UESTA6_CURRBK_Msk & ((value) << USBC_UESTA6_CURRBK_Pos)))
#define USBC_UESTA6_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA6) Control Direction */
#define USBC_UESTA6_CTRLDIR         (0x1u << USBC_UESTA6_CTRLDIR_Pos)
#define   USBC_UESTA6_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA6)  */
#define   USBC_UESTA6_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA6)  */
#define USBC_UESTA6_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA6) MASK Register */

/* -------- USBC_UESTA7 : (USBC Offset: 0x14C) (R/  32) Endpoint Status Register -------- */
#define USBC_UESTA7_OFFSET          0x14C        /**< \brief (USBC_UESTA7 offset) Endpoint Status Register */
#define USBC_UESTA7_RESETVALUE      0x00000100   /**< \brief (USBC_UESTA7 reset_value) Endpoint Status Register */

#define USBC_UESTA7_TXINI_Pos       0            /**< \brief (USBC_UESTA7) Transmitted IN Data Interrupt */
#define USBC_UESTA7_TXINI           (0x1u << USBC_UESTA7_TXINI_Pos)
#define USBC_UESTA7_RXOUTI_Pos      1            /**< \brief (USBC_UESTA7) Received OUT Data Interrupt */
#define USBC_UESTA7_RXOUTI          (0x1u << USBC_UESTA7_RXOUTI_Pos)
#define USBC_UESTA7_RXSTPI_Pos      2            /**< \brief (USBC_UESTA7) Received SETUP Interrupt */
#define USBC_UESTA7_RXSTPI          (0x1u << USBC_UESTA7_RXSTPI_Pos)
#define USBC_UESTA7_NAKOUTI_Pos     3            /**< \brief (USBC_UESTA7) NAKed OUT Interrupt */
#define USBC_UESTA7_NAKOUTI         (0x1u << USBC_UESTA7_NAKOUTI_Pos)
#define USBC_UESTA7_NAKINI_Pos      4            /**< \brief (USBC_UESTA7) NAKed IN Interrupt */
#define USBC_UESTA7_NAKINI          (0x1u << USBC_UESTA7_NAKINI_Pos)
#define USBC_UESTA7_STALLEDI_Pos    6            /**< \brief (USBC_UESTA7) STALLed Interrupt */
#define USBC_UESTA7_STALLEDI        (0x1u << USBC_UESTA7_STALLEDI_Pos)
#define USBC_UESTA7_DTSEQ_Pos       8            /**< \brief (USBC_UESTA7) Data Toggle Sequence */
#define USBC_UESTA7_DTSEQ_Msk       (0x3u << USBC_UESTA7_DTSEQ_Pos)
#define USBC_UESTA7_DTSEQ(value)    ((USBC_UESTA7_DTSEQ_Msk & ((value) << USBC_UESTA7_DTSEQ_Pos)))
#define USBC_UESTA7_RAMACERI_Pos    11           /**< \brief (USBC_UESTA7) Ram Access Error Interrupt */
#define USBC_UESTA7_RAMACERI        (0x1u << USBC_UESTA7_RAMACERI_Pos)
#define USBC_UESTA7_NBUSYBK_Pos     12           /**< \brief (USBC_UESTA7) Number Of Busy Banks */
#define USBC_UESTA7_NBUSYBK_Msk     (0x3u << USBC_UESTA7_NBUSYBK_Pos)
#define USBC_UESTA7_NBUSYBK(value)  ((USBC_UESTA7_NBUSYBK_Msk & ((value) << USBC_UESTA7_NBUSYBK_Pos)))
#define USBC_UESTA7_CURRBK_Pos      14           /**< \brief (USBC_UESTA7) Current Bank */
#define USBC_UESTA7_CURRBK_Msk      (0x3u << USBC_UESTA7_CURRBK_Pos)
#define USBC_UESTA7_CURRBK(value)   ((USBC_UESTA7_CURRBK_Msk & ((value) << USBC_UESTA7_CURRBK_Pos)))
#define USBC_UESTA7_CTRLDIR_Pos     17           /**< \brief (USBC_UESTA7) Control Direction */
#define USBC_UESTA7_CTRLDIR         (0x1u << USBC_UESTA7_CTRLDIR_Pos)
#define   USBC_UESTA7_CTRLDIR_OUT   (0x0u << 17) /**< \brief (USBC_UESTA7)  */
#define   USBC_UESTA7_CTRLDIR_IN    (0x1u << 17) /**< \brief (USBC_UESTA7)  */
#define USBC_UESTA7_MASK            0x0002FB5Fu  /**< \brief (USBC_UESTA7) MASK Register */

/* -------- USBC_UESTA0CLR : (USBC Offset: 0x160) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA0CLR_OFFSET       0x160        /**< \brief (USBC_UESTA0CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA0CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA0CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA0CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA0CLR) TXINI Clear */
#define USBC_UESTA0CLR_TXINIC       (0x1u << USBC_UESTA0CLR_TXINIC_Pos)
#define USBC_UESTA0CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA0CLR) RXOUTI Clear */
#define USBC_UESTA0CLR_RXOUTIC      (0x1u << USBC_UESTA0CLR_RXOUTIC_Pos)
#define USBC_UESTA0CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA0CLR) RXSTPI Clear */
#define USBC_UESTA0CLR_RXSTPIC      (0x1u << USBC_UESTA0CLR_RXSTPIC_Pos)
#define USBC_UESTA0CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA0CLR) NAKOUTI Clear */
#define USBC_UESTA0CLR_NAKOUTIC     (0x1u << USBC_UESTA0CLR_NAKOUTIC_Pos)
#define USBC_UESTA0CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA0CLR) NAKINI Clear */
#define USBC_UESTA0CLR_NAKINIC      (0x1u << USBC_UESTA0CLR_NAKINIC_Pos)
#define USBC_UESTA0CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA0CLR) STALLEDI Clear */
#define USBC_UESTA0CLR_STALLEDIC    (0x1u << USBC_UESTA0CLR_STALLEDIC_Pos)
#define USBC_UESTA0CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA0CLR) RAMACERI Clear */
#define USBC_UESTA0CLR_RAMACERIC    (0x1u << USBC_UESTA0CLR_RAMACERIC_Pos)
#define USBC_UESTA0CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA0CLR) MASK Register */

/* -------- USBC_UESTA1CLR : (USBC Offset: 0x164) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA1CLR_OFFSET       0x164        /**< \brief (USBC_UESTA1CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA1CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA1CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA1CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA1CLR) TXINI Clear */
#define USBC_UESTA1CLR_TXINIC       (0x1u << USBC_UESTA1CLR_TXINIC_Pos)
#define USBC_UESTA1CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA1CLR) RXOUTI Clear */
#define USBC_UESTA1CLR_RXOUTIC      (0x1u << USBC_UESTA1CLR_RXOUTIC_Pos)
#define USBC_UESTA1CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA1CLR) RXSTPI Clear */
#define USBC_UESTA1CLR_RXSTPIC      (0x1u << USBC_UESTA1CLR_RXSTPIC_Pos)
#define USBC_UESTA1CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA1CLR) NAKOUTI Clear */
#define USBC_UESTA1CLR_NAKOUTIC     (0x1u << USBC_UESTA1CLR_NAKOUTIC_Pos)
#define USBC_UESTA1CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA1CLR) NAKINI Clear */
#define USBC_UESTA1CLR_NAKINIC      (0x1u << USBC_UESTA1CLR_NAKINIC_Pos)
#define USBC_UESTA1CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA1CLR) STALLEDI Clear */
#define USBC_UESTA1CLR_STALLEDIC    (0x1u << USBC_UESTA1CLR_STALLEDIC_Pos)
#define USBC_UESTA1CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA1CLR) RAMACERI Clear */
#define USBC_UESTA1CLR_RAMACERIC    (0x1u << USBC_UESTA1CLR_RAMACERIC_Pos)
#define USBC_UESTA1CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA1CLR) MASK Register */

/* -------- USBC_UESTA2CLR : (USBC Offset: 0x168) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA2CLR_OFFSET       0x168        /**< \brief (USBC_UESTA2CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA2CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA2CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA2CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA2CLR) TXINI Clear */
#define USBC_UESTA2CLR_TXINIC       (0x1u << USBC_UESTA2CLR_TXINIC_Pos)
#define USBC_UESTA2CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA2CLR) RXOUTI Clear */
#define USBC_UESTA2CLR_RXOUTIC      (0x1u << USBC_UESTA2CLR_RXOUTIC_Pos)
#define USBC_UESTA2CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA2CLR) RXSTPI Clear */
#define USBC_UESTA2CLR_RXSTPIC      (0x1u << USBC_UESTA2CLR_RXSTPIC_Pos)
#define USBC_UESTA2CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA2CLR) NAKOUTI Clear */
#define USBC_UESTA2CLR_NAKOUTIC     (0x1u << USBC_UESTA2CLR_NAKOUTIC_Pos)
#define USBC_UESTA2CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA2CLR) NAKINI Clear */
#define USBC_UESTA2CLR_NAKINIC      (0x1u << USBC_UESTA2CLR_NAKINIC_Pos)
#define USBC_UESTA2CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA2CLR) STALLEDI Clear */
#define USBC_UESTA2CLR_STALLEDIC    (0x1u << USBC_UESTA2CLR_STALLEDIC_Pos)
#define USBC_UESTA2CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA2CLR) RAMACERI Clear */
#define USBC_UESTA2CLR_RAMACERIC    (0x1u << USBC_UESTA2CLR_RAMACERIC_Pos)
#define USBC_UESTA2CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA2CLR) MASK Register */

/* -------- USBC_UESTA3CLR : (USBC Offset: 0x16C) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA3CLR_OFFSET       0x16C        /**< \brief (USBC_UESTA3CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA3CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA3CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA3CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA3CLR) TXINI Clear */
#define USBC_UESTA3CLR_TXINIC       (0x1u << USBC_UESTA3CLR_TXINIC_Pos)
#define USBC_UESTA3CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA3CLR) RXOUTI Clear */
#define USBC_UESTA3CLR_RXOUTIC      (0x1u << USBC_UESTA3CLR_RXOUTIC_Pos)
#define USBC_UESTA3CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA3CLR) RXSTPI Clear */
#define USBC_UESTA3CLR_RXSTPIC      (0x1u << USBC_UESTA3CLR_RXSTPIC_Pos)
#define USBC_UESTA3CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA3CLR) NAKOUTI Clear */
#define USBC_UESTA3CLR_NAKOUTIC     (0x1u << USBC_UESTA3CLR_NAKOUTIC_Pos)
#define USBC_UESTA3CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA3CLR) NAKINI Clear */
#define USBC_UESTA3CLR_NAKINIC      (0x1u << USBC_UESTA3CLR_NAKINIC_Pos)
#define USBC_UESTA3CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA3CLR) STALLEDI Clear */
#define USBC_UESTA3CLR_STALLEDIC    (0x1u << USBC_UESTA3CLR_STALLEDIC_Pos)
#define USBC_UESTA3CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA3CLR) RAMACERI Clear */
#define USBC_UESTA3CLR_RAMACERIC    (0x1u << USBC_UESTA3CLR_RAMACERIC_Pos)
#define USBC_UESTA3CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA3CLR) MASK Register */

/* -------- USBC_UESTA4CLR : (USBC Offset: 0x170) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA4CLR_OFFSET       0x170        /**< \brief (USBC_UESTA4CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA4CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA4CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA4CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA4CLR) TXINI Clear */
#define USBC_UESTA4CLR_TXINIC       (0x1u << USBC_UESTA4CLR_TXINIC_Pos)
#define USBC_UESTA4CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA4CLR) RXOUTI Clear */
#define USBC_UESTA4CLR_RXOUTIC      (0x1u << USBC_UESTA4CLR_RXOUTIC_Pos)
#define USBC_UESTA4CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA4CLR) RXSTPI Clear */
#define USBC_UESTA4CLR_RXSTPIC      (0x1u << USBC_UESTA4CLR_RXSTPIC_Pos)
#define USBC_UESTA4CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA4CLR) NAKOUTI Clear */
#define USBC_UESTA4CLR_NAKOUTIC     (0x1u << USBC_UESTA4CLR_NAKOUTIC_Pos)
#define USBC_UESTA4CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA4CLR) NAKINI Clear */
#define USBC_UESTA4CLR_NAKINIC      (0x1u << USBC_UESTA4CLR_NAKINIC_Pos)
#define USBC_UESTA4CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA4CLR) STALLEDI Clear */
#define USBC_UESTA4CLR_STALLEDIC    (0x1u << USBC_UESTA4CLR_STALLEDIC_Pos)
#define USBC_UESTA4CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA4CLR) RAMACERI Clear */
#define USBC_UESTA4CLR_RAMACERIC    (0x1u << USBC_UESTA4CLR_RAMACERIC_Pos)
#define USBC_UESTA4CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA4CLR) MASK Register */

/* -------- USBC_UESTA5CLR : (USBC Offset: 0x174) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA5CLR_OFFSET       0x174        /**< \brief (USBC_UESTA5CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA5CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA5CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA5CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA5CLR) TXINI Clear */
#define USBC_UESTA5CLR_TXINIC       (0x1u << USBC_UESTA5CLR_TXINIC_Pos)
#define USBC_UESTA5CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA5CLR) RXOUTI Clear */
#define USBC_UESTA5CLR_RXOUTIC      (0x1u << USBC_UESTA5CLR_RXOUTIC_Pos)
#define USBC_UESTA5CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA5CLR) RXSTPI Clear */
#define USBC_UESTA5CLR_RXSTPIC      (0x1u << USBC_UESTA5CLR_RXSTPIC_Pos)
#define USBC_UESTA5CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA5CLR) NAKOUTI Clear */
#define USBC_UESTA5CLR_NAKOUTIC     (0x1u << USBC_UESTA5CLR_NAKOUTIC_Pos)
#define USBC_UESTA5CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA5CLR) NAKINI Clear */
#define USBC_UESTA5CLR_NAKINIC      (0x1u << USBC_UESTA5CLR_NAKINIC_Pos)
#define USBC_UESTA5CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA5CLR) STALLEDI Clear */
#define USBC_UESTA5CLR_STALLEDIC    (0x1u << USBC_UESTA5CLR_STALLEDIC_Pos)
#define USBC_UESTA5CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA5CLR) RAMACERI Clear */
#define USBC_UESTA5CLR_RAMACERIC    (0x1u << USBC_UESTA5CLR_RAMACERIC_Pos)
#define USBC_UESTA5CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA5CLR) MASK Register */

/* -------- USBC_UESTA6CLR : (USBC Offset: 0x178) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA6CLR_OFFSET       0x178        /**< \brief (USBC_UESTA6CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA6CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA6CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA6CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA6CLR) TXINI Clear */
#define USBC_UESTA6CLR_TXINIC       (0x1u << USBC_UESTA6CLR_TXINIC_Pos)
#define USBC_UESTA6CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA6CLR) RXOUTI Clear */
#define USBC_UESTA6CLR_RXOUTIC      (0x1u << USBC_UESTA6CLR_RXOUTIC_Pos)
#define USBC_UESTA6CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA6CLR) RXSTPI Clear */
#define USBC_UESTA6CLR_RXSTPIC      (0x1u << USBC_UESTA6CLR_RXSTPIC_Pos)
#define USBC_UESTA6CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA6CLR) NAKOUTI Clear */
#define USBC_UESTA6CLR_NAKOUTIC     (0x1u << USBC_UESTA6CLR_NAKOUTIC_Pos)
#define USBC_UESTA6CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA6CLR) NAKINI Clear */
#define USBC_UESTA6CLR_NAKINIC      (0x1u << USBC_UESTA6CLR_NAKINIC_Pos)
#define USBC_UESTA6CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA6CLR) STALLEDI Clear */
#define USBC_UESTA6CLR_STALLEDIC    (0x1u << USBC_UESTA6CLR_STALLEDIC_Pos)
#define USBC_UESTA6CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA6CLR) RAMACERI Clear */
#define USBC_UESTA6CLR_RAMACERIC    (0x1u << USBC_UESTA6CLR_RAMACERIC_Pos)
#define USBC_UESTA6CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA6CLR) MASK Register */

/* -------- USBC_UESTA7CLR : (USBC Offset: 0x17C) ( /W 32) Endpoint Status Clear Register -------- */
#define USBC_UESTA7CLR_OFFSET       0x17C        /**< \brief (USBC_UESTA7CLR offset) Endpoint Status Clear Register */
#define USBC_UESTA7CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA7CLR reset_value) Endpoint Status Clear Register */

#define USBC_UESTA7CLR_TXINIC_Pos   0            /**< \brief (USBC_UESTA7CLR) TXINI Clear */
#define USBC_UESTA7CLR_TXINIC       (0x1u << USBC_UESTA7CLR_TXINIC_Pos)
#define USBC_UESTA7CLR_RXOUTIC_Pos  1            /**< \brief (USBC_UESTA7CLR) RXOUTI Clear */
#define USBC_UESTA7CLR_RXOUTIC      (0x1u << USBC_UESTA7CLR_RXOUTIC_Pos)
#define USBC_UESTA7CLR_RXSTPIC_Pos  2            /**< \brief (USBC_UESTA7CLR) RXSTPI Clear */
#define USBC_UESTA7CLR_RXSTPIC      (0x1u << USBC_UESTA7CLR_RXSTPIC_Pos)
#define USBC_UESTA7CLR_NAKOUTIC_Pos 3            /**< \brief (USBC_UESTA7CLR) NAKOUTI Clear */
#define USBC_UESTA7CLR_NAKOUTIC     (0x1u << USBC_UESTA7CLR_NAKOUTIC_Pos)
#define USBC_UESTA7CLR_NAKINIC_Pos  4            /**< \brief (USBC_UESTA7CLR) NAKINI Clear */
#define USBC_UESTA7CLR_NAKINIC      (0x1u << USBC_UESTA7CLR_NAKINIC_Pos)
#define USBC_UESTA7CLR_STALLEDIC_Pos 6            /**< \brief (USBC_UESTA7CLR) STALLEDI Clear */
#define USBC_UESTA7CLR_STALLEDIC    (0x1u << USBC_UESTA7CLR_STALLEDIC_Pos)
#define USBC_UESTA7CLR_RAMACERIC_Pos 11           /**< \brief (USBC_UESTA7CLR) RAMACERI Clear */
#define USBC_UESTA7CLR_RAMACERIC    (0x1u << USBC_UESTA7CLR_RAMACERIC_Pos)
#define USBC_UESTA7CLR_MASK         0x0000085Fu  /**< \brief (USBC_UESTA7CLR) MASK Register */

/* -------- USBC_UESTA0SET : (USBC Offset: 0x190) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA0SET_OFFSET       0x190        /**< \brief (USBC_UESTA0SET offset) Endpoint Status Set Register */
#define USBC_UESTA0SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA0SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA0SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA0SET) TXINI Set */
#define USBC_UESTA0SET_TXINIS       (0x1u << USBC_UESTA0SET_TXINIS_Pos)
#define USBC_UESTA0SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA0SET) RXOUTI Set */
#define USBC_UESTA0SET_RXOUTIS      (0x1u << USBC_UESTA0SET_RXOUTIS_Pos)
#define USBC_UESTA0SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA0SET) RXSTPI Set */
#define USBC_UESTA0SET_RXSTPIS      (0x1u << USBC_UESTA0SET_RXSTPIS_Pos)
#define USBC_UESTA0SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA0SET) NAKOUTI Set */
#define USBC_UESTA0SET_NAKOUTIS     (0x1u << USBC_UESTA0SET_NAKOUTIS_Pos)
#define USBC_UESTA0SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA0SET) NAKINI Set */
#define USBC_UESTA0SET_NAKINIS      (0x1u << USBC_UESTA0SET_NAKINIS_Pos)
#define USBC_UESTA0SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA0SET) STALLEDI Set */
#define USBC_UESTA0SET_STALLEDIS    (0x1u << USBC_UESTA0SET_STALLEDIS_Pos)
#define USBC_UESTA0SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA0SET) RAMACERI Set */
#define USBC_UESTA0SET_RAMACERIS    (0x1u << USBC_UESTA0SET_RAMACERIS_Pos)
#define USBC_UESTA0SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA0SET) NBUSYBK Set */
#define USBC_UESTA0SET_NBUSYBKS     (0x1u << USBC_UESTA0SET_NBUSYBKS_Pos)
#define USBC_UESTA0SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA0SET) MASK Register */

/* -------- USBC_UESTA1SET : (USBC Offset: 0x194) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA1SET_OFFSET       0x194        /**< \brief (USBC_UESTA1SET offset) Endpoint Status Set Register */
#define USBC_UESTA1SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA1SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA1SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA1SET) TXINI Set */
#define USBC_UESTA1SET_TXINIS       (0x1u << USBC_UESTA1SET_TXINIS_Pos)
#define USBC_UESTA1SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA1SET) RXOUTI Set */
#define USBC_UESTA1SET_RXOUTIS      (0x1u << USBC_UESTA1SET_RXOUTIS_Pos)
#define USBC_UESTA1SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA1SET) RXSTPI Set */
#define USBC_UESTA1SET_RXSTPIS      (0x1u << USBC_UESTA1SET_RXSTPIS_Pos)
#define USBC_UESTA1SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA1SET) NAKOUTI Set */
#define USBC_UESTA1SET_NAKOUTIS     (0x1u << USBC_UESTA1SET_NAKOUTIS_Pos)
#define USBC_UESTA1SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA1SET) NAKINI Set */
#define USBC_UESTA1SET_NAKINIS      (0x1u << USBC_UESTA1SET_NAKINIS_Pos)
#define USBC_UESTA1SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA1SET) STALLEDI Set */
#define USBC_UESTA1SET_STALLEDIS    (0x1u << USBC_UESTA1SET_STALLEDIS_Pos)
#define USBC_UESTA1SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA1SET) RAMACERI Set */
#define USBC_UESTA1SET_RAMACERIS    (0x1u << USBC_UESTA1SET_RAMACERIS_Pos)
#define USBC_UESTA1SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA1SET) NBUSYBK Set */
#define USBC_UESTA1SET_NBUSYBKS     (0x1u << USBC_UESTA1SET_NBUSYBKS_Pos)
#define USBC_UESTA1SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA1SET) MASK Register */

/* -------- USBC_UESTA2SET : (USBC Offset: 0x198) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA2SET_OFFSET       0x198        /**< \brief (USBC_UESTA2SET offset) Endpoint Status Set Register */
#define USBC_UESTA2SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA2SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA2SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA2SET) TXINI Set */
#define USBC_UESTA2SET_TXINIS       (0x1u << USBC_UESTA2SET_TXINIS_Pos)
#define USBC_UESTA2SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA2SET) RXOUTI Set */
#define USBC_UESTA2SET_RXOUTIS      (0x1u << USBC_UESTA2SET_RXOUTIS_Pos)
#define USBC_UESTA2SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA2SET) RXSTPI Set */
#define USBC_UESTA2SET_RXSTPIS      (0x1u << USBC_UESTA2SET_RXSTPIS_Pos)
#define USBC_UESTA2SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA2SET) NAKOUTI Set */
#define USBC_UESTA2SET_NAKOUTIS     (0x1u << USBC_UESTA2SET_NAKOUTIS_Pos)
#define USBC_UESTA2SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA2SET) NAKINI Set */
#define USBC_UESTA2SET_NAKINIS      (0x1u << USBC_UESTA2SET_NAKINIS_Pos)
#define USBC_UESTA2SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA2SET) STALLEDI Set */
#define USBC_UESTA2SET_STALLEDIS    (0x1u << USBC_UESTA2SET_STALLEDIS_Pos)
#define USBC_UESTA2SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA2SET) RAMACERI Set */
#define USBC_UESTA2SET_RAMACERIS    (0x1u << USBC_UESTA2SET_RAMACERIS_Pos)
#define USBC_UESTA2SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA2SET) NBUSYBK Set */
#define USBC_UESTA2SET_NBUSYBKS     (0x1u << USBC_UESTA2SET_NBUSYBKS_Pos)
#define USBC_UESTA2SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA2SET) MASK Register */

/* -------- USBC_UESTA3SET : (USBC Offset: 0x19C) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA3SET_OFFSET       0x19C        /**< \brief (USBC_UESTA3SET offset) Endpoint Status Set Register */
#define USBC_UESTA3SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA3SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA3SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA3SET) TXINI Set */
#define USBC_UESTA3SET_TXINIS       (0x1u << USBC_UESTA3SET_TXINIS_Pos)
#define USBC_UESTA3SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA3SET) RXOUTI Set */
#define USBC_UESTA3SET_RXOUTIS      (0x1u << USBC_UESTA3SET_RXOUTIS_Pos)
#define USBC_UESTA3SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA3SET) RXSTPI Set */
#define USBC_UESTA3SET_RXSTPIS      (0x1u << USBC_UESTA3SET_RXSTPIS_Pos)
#define USBC_UESTA3SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA3SET) NAKOUTI Set */
#define USBC_UESTA3SET_NAKOUTIS     (0x1u << USBC_UESTA3SET_NAKOUTIS_Pos)
#define USBC_UESTA3SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA3SET) NAKINI Set */
#define USBC_UESTA3SET_NAKINIS      (0x1u << USBC_UESTA3SET_NAKINIS_Pos)
#define USBC_UESTA3SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA3SET) STALLEDI Set */
#define USBC_UESTA3SET_STALLEDIS    (0x1u << USBC_UESTA3SET_STALLEDIS_Pos)
#define USBC_UESTA3SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA3SET) RAMACERI Set */
#define USBC_UESTA3SET_RAMACERIS    (0x1u << USBC_UESTA3SET_RAMACERIS_Pos)
#define USBC_UESTA3SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA3SET) NBUSYBK Set */
#define USBC_UESTA3SET_NBUSYBKS     (0x1u << USBC_UESTA3SET_NBUSYBKS_Pos)
#define USBC_UESTA3SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA3SET) MASK Register */

/* -------- USBC_UESTA4SET : (USBC Offset: 0x1A0) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA4SET_OFFSET       0x1A0        /**< \brief (USBC_UESTA4SET offset) Endpoint Status Set Register */
#define USBC_UESTA4SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA4SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA4SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA4SET) TXINI Set */
#define USBC_UESTA4SET_TXINIS       (0x1u << USBC_UESTA4SET_TXINIS_Pos)
#define USBC_UESTA4SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA4SET) RXOUTI Set */
#define USBC_UESTA4SET_RXOUTIS      (0x1u << USBC_UESTA4SET_RXOUTIS_Pos)
#define USBC_UESTA4SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA4SET) RXSTPI Set */
#define USBC_UESTA4SET_RXSTPIS      (0x1u << USBC_UESTA4SET_RXSTPIS_Pos)
#define USBC_UESTA4SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA4SET) NAKOUTI Set */
#define USBC_UESTA4SET_NAKOUTIS     (0x1u << USBC_UESTA4SET_NAKOUTIS_Pos)
#define USBC_UESTA4SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA4SET) NAKINI Set */
#define USBC_UESTA4SET_NAKINIS      (0x1u << USBC_UESTA4SET_NAKINIS_Pos)
#define USBC_UESTA4SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA4SET) STALLEDI Set */
#define USBC_UESTA4SET_STALLEDIS    (0x1u << USBC_UESTA4SET_STALLEDIS_Pos)
#define USBC_UESTA4SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA4SET) RAMACERI Set */
#define USBC_UESTA4SET_RAMACERIS    (0x1u << USBC_UESTA4SET_RAMACERIS_Pos)
#define USBC_UESTA4SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA4SET) NBUSYBK Set */
#define USBC_UESTA4SET_NBUSYBKS     (0x1u << USBC_UESTA4SET_NBUSYBKS_Pos)
#define USBC_UESTA4SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA4SET) MASK Register */

/* -------- USBC_UESTA5SET : (USBC Offset: 0x1A4) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA5SET_OFFSET       0x1A4        /**< \brief (USBC_UESTA5SET offset) Endpoint Status Set Register */
#define USBC_UESTA5SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA5SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA5SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA5SET) TXINI Set */
#define USBC_UESTA5SET_TXINIS       (0x1u << USBC_UESTA5SET_TXINIS_Pos)
#define USBC_UESTA5SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA5SET) RXOUTI Set */
#define USBC_UESTA5SET_RXOUTIS      (0x1u << USBC_UESTA5SET_RXOUTIS_Pos)
#define USBC_UESTA5SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA5SET) RXSTPI Set */
#define USBC_UESTA5SET_RXSTPIS      (0x1u << USBC_UESTA5SET_RXSTPIS_Pos)
#define USBC_UESTA5SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA5SET) NAKOUTI Set */
#define USBC_UESTA5SET_NAKOUTIS     (0x1u << USBC_UESTA5SET_NAKOUTIS_Pos)
#define USBC_UESTA5SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA5SET) NAKINI Set */
#define USBC_UESTA5SET_NAKINIS      (0x1u << USBC_UESTA5SET_NAKINIS_Pos)
#define USBC_UESTA5SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA5SET) STALLEDI Set */
#define USBC_UESTA5SET_STALLEDIS    (0x1u << USBC_UESTA5SET_STALLEDIS_Pos)
#define USBC_UESTA5SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA5SET) RAMACERI Set */
#define USBC_UESTA5SET_RAMACERIS    (0x1u << USBC_UESTA5SET_RAMACERIS_Pos)
#define USBC_UESTA5SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA5SET) NBUSYBK Set */
#define USBC_UESTA5SET_NBUSYBKS     (0x1u << USBC_UESTA5SET_NBUSYBKS_Pos)
#define USBC_UESTA5SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA5SET) MASK Register */

/* -------- USBC_UESTA6SET : (USBC Offset: 0x1A8) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA6SET_OFFSET       0x1A8        /**< \brief (USBC_UESTA6SET offset) Endpoint Status Set Register */
#define USBC_UESTA6SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA6SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA6SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA6SET) TXINI Set */
#define USBC_UESTA6SET_TXINIS       (0x1u << USBC_UESTA6SET_TXINIS_Pos)
#define USBC_UESTA6SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA6SET) RXOUTI Set */
#define USBC_UESTA6SET_RXOUTIS      (0x1u << USBC_UESTA6SET_RXOUTIS_Pos)
#define USBC_UESTA6SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA6SET) RXSTPI Set */
#define USBC_UESTA6SET_RXSTPIS      (0x1u << USBC_UESTA6SET_RXSTPIS_Pos)
#define USBC_UESTA6SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA6SET) NAKOUTI Set */
#define USBC_UESTA6SET_NAKOUTIS     (0x1u << USBC_UESTA6SET_NAKOUTIS_Pos)
#define USBC_UESTA6SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA6SET) NAKINI Set */
#define USBC_UESTA6SET_NAKINIS      (0x1u << USBC_UESTA6SET_NAKINIS_Pos)
#define USBC_UESTA6SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA6SET) STALLEDI Set */
#define USBC_UESTA6SET_STALLEDIS    (0x1u << USBC_UESTA6SET_STALLEDIS_Pos)
#define USBC_UESTA6SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA6SET) RAMACERI Set */
#define USBC_UESTA6SET_RAMACERIS    (0x1u << USBC_UESTA6SET_RAMACERIS_Pos)
#define USBC_UESTA6SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA6SET) NBUSYBK Set */
#define USBC_UESTA6SET_NBUSYBKS     (0x1u << USBC_UESTA6SET_NBUSYBKS_Pos)
#define USBC_UESTA6SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA6SET) MASK Register */

/* -------- USBC_UESTA7SET : (USBC Offset: 0x1AC) ( /W 32) Endpoint Status Set Register -------- */
#define USBC_UESTA7SET_OFFSET       0x1AC        /**< \brief (USBC_UESTA7SET offset) Endpoint Status Set Register */
#define USBC_UESTA7SET_RESETVALUE   0x00000000   /**< \brief (USBC_UESTA7SET reset_value) Endpoint Status Set Register */

#define USBC_UESTA7SET_TXINIS_Pos   0            /**< \brief (USBC_UESTA7SET) TXINI Set */
#define USBC_UESTA7SET_TXINIS       (0x1u << USBC_UESTA7SET_TXINIS_Pos)
#define USBC_UESTA7SET_RXOUTIS_Pos  1            /**< \brief (USBC_UESTA7SET) RXOUTI Set */
#define USBC_UESTA7SET_RXOUTIS      (0x1u << USBC_UESTA7SET_RXOUTIS_Pos)
#define USBC_UESTA7SET_RXSTPIS_Pos  2            /**< \brief (USBC_UESTA7SET) RXSTPI Set */
#define USBC_UESTA7SET_RXSTPIS      (0x1u << USBC_UESTA7SET_RXSTPIS_Pos)
#define USBC_UESTA7SET_NAKOUTIS_Pos 3            /**< \brief (USBC_UESTA7SET) NAKOUTI Set */
#define USBC_UESTA7SET_NAKOUTIS     (0x1u << USBC_UESTA7SET_NAKOUTIS_Pos)
#define USBC_UESTA7SET_NAKINIS_Pos  4            /**< \brief (USBC_UESTA7SET) NAKINI Set */
#define USBC_UESTA7SET_NAKINIS      (0x1u << USBC_UESTA7SET_NAKINIS_Pos)
#define USBC_UESTA7SET_STALLEDIS_Pos 6            /**< \brief (USBC_UESTA7SET) STALLEDI Set */
#define USBC_UESTA7SET_STALLEDIS    (0x1u << USBC_UESTA7SET_STALLEDIS_Pos)
#define USBC_UESTA7SET_RAMACERIS_Pos 11           /**< \brief (USBC_UESTA7SET) RAMACERI Set */
#define USBC_UESTA7SET_RAMACERIS    (0x1u << USBC_UESTA7SET_RAMACERIS_Pos)
#define USBC_UESTA7SET_NBUSYBKS_Pos 12           /**< \brief (USBC_UESTA7SET) NBUSYBK Set */
#define USBC_UESTA7SET_NBUSYBKS     (0x1u << USBC_UESTA7SET_NBUSYBKS_Pos)
#define USBC_UESTA7SET_MASK         0x0000185Fu  /**< \brief (USBC_UESTA7SET) MASK Register */

/* -------- USBC_UECON0 : (USBC Offset: 0x1C0) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON0_OFFSET          0x1C0        /**< \brief (USBC_UECON0 offset) Endpoint Control Register */
#define USBC_UECON0_RESETVALUE      0x00000000   /**< \brief (USBC_UECON0 reset_value) Endpoint Control Register */

#define USBC_UECON0_TXINE_Pos       0            /**< \brief (USBC_UECON0) TXIN Interrupt Enable */
#define USBC_UECON0_TXINE           (0x1u << USBC_UECON0_TXINE_Pos)
#define USBC_UECON0_RXOUTE_Pos      1            /**< \brief (USBC_UECON0) RXOUT Interrupt Enable */
#define USBC_UECON0_RXOUTE          (0x1u << USBC_UECON0_RXOUTE_Pos)
#define USBC_UECON0_RXSTPE_Pos      2            /**< \brief (USBC_UECON0) RXSTP Interrupt Enable */
#define USBC_UECON0_RXSTPE          (0x1u << USBC_UECON0_RXSTPE_Pos)
#define USBC_UECON0_NAKOUTE_Pos     3            /**< \brief (USBC_UECON0) NAKOUT Interrupt Enable */
#define USBC_UECON0_NAKOUTE         (0x1u << USBC_UECON0_NAKOUTE_Pos)
#define USBC_UECON0_NAKINE_Pos      4            /**< \brief (USBC_UECON0) NAKIN Interrupt Enable */
#define USBC_UECON0_NAKINE          (0x1u << USBC_UECON0_NAKINE_Pos)
#define USBC_UECON0_STALLEDE_Pos    6            /**< \brief (USBC_UECON0) STALLED Interrupt Enable */
#define USBC_UECON0_STALLEDE        (0x1u << USBC_UECON0_STALLEDE_Pos)
#define USBC_UECON0_NREPLY_Pos      8            /**< \brief (USBC_UECON0) No Reply */
#define USBC_UECON0_NREPLY          (0x1u << USBC_UECON0_NREPLY_Pos)
#define USBC_UECON0_RAMACERE_Pos    11           /**< \brief (USBC_UECON0) RAMACER Interrupt Enable */
#define USBC_UECON0_RAMACERE        (0x1u << USBC_UECON0_RAMACERE_Pos)
#define USBC_UECON0_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON0) Number of Busy Banks Interrupt Enable */
#define USBC_UECON0_NBUSYBKE        (0x1u << USBC_UECON0_NBUSYBKE_Pos)
#define USBC_UECON0_KILLBK_Pos      13           /**< \brief (USBC_UECON0) Kill IN Bank */
#define USBC_UECON0_KILLBK          (0x1u << USBC_UECON0_KILLBK_Pos)
#define USBC_UECON0_FIFOCON_Pos     14           /**< \brief (USBC_UECON0) FIFO Control */
#define USBC_UECON0_FIFOCON         (0x1u << USBC_UECON0_FIFOCON_Pos)
#define USBC_UECON0_NYETDIS_Pos     17           /**< \brief (USBC_UECON0) NYET token disable */
#define USBC_UECON0_NYETDIS         (0x1u << USBC_UECON0_NYETDIS_Pos)
#define USBC_UECON0_RSTDT_Pos       18           /**< \brief (USBC_UECON0) Reset Data Toggle */
#define USBC_UECON0_RSTDT           (0x1u << USBC_UECON0_RSTDT_Pos)
#define USBC_UECON0_STALLRQ_Pos     19           /**< \brief (USBC_UECON0) STALL Request */
#define USBC_UECON0_STALLRQ         (0x1u << USBC_UECON0_STALLRQ_Pos)
#define USBC_UECON0_BUSY0_Pos       24           /**< \brief (USBC_UECON0) Busy Bank1 Enable */
#define USBC_UECON0_BUSY0           (0x1u << USBC_UECON0_BUSY0_Pos)
#define USBC_UECON0_BUSY1_Pos       25           /**< \brief (USBC_UECON0) Busy Bank0 Enable */
#define USBC_UECON0_BUSY1           (0x1u << USBC_UECON0_BUSY1_Pos)
#define USBC_UECON0_MASK            0x030E795Fu  /**< \brief (USBC_UECON0) MASK Register */

/* -------- USBC_UECON1 : (USBC Offset: 0x1C4) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON1_OFFSET          0x1C4        /**< \brief (USBC_UECON1 offset) Endpoint Control Register */
#define USBC_UECON1_RESETVALUE      0x00000000   /**< \brief (USBC_UECON1 reset_value) Endpoint Control Register */

#define USBC_UECON1_TXINE_Pos       0            /**< \brief (USBC_UECON1) TXIN Interrupt Enable */
#define USBC_UECON1_TXINE           (0x1u << USBC_UECON1_TXINE_Pos)
#define USBC_UECON1_RXOUTE_Pos      1            /**< \brief (USBC_UECON1) RXOUT Interrupt Enable */
#define USBC_UECON1_RXOUTE          (0x1u << USBC_UECON1_RXOUTE_Pos)
#define USBC_UECON1_RXSTPE_Pos      2            /**< \brief (USBC_UECON1) RXSTP Interrupt Enable */
#define USBC_UECON1_RXSTPE          (0x1u << USBC_UECON1_RXSTPE_Pos)
#define USBC_UECON1_NAKOUTE_Pos     3            /**< \brief (USBC_UECON1) NAKOUT Interrupt Enable */
#define USBC_UECON1_NAKOUTE         (0x1u << USBC_UECON1_NAKOUTE_Pos)
#define USBC_UECON1_NAKINE_Pos      4            /**< \brief (USBC_UECON1) NAKIN Interrupt Enable */
#define USBC_UECON1_NAKINE          (0x1u << USBC_UECON1_NAKINE_Pos)
#define USBC_UECON1_STALLEDE_Pos    6            /**< \brief (USBC_UECON1) STALLED Interrupt Enable */
#define USBC_UECON1_STALLEDE        (0x1u << USBC_UECON1_STALLEDE_Pos)
#define USBC_UECON1_NREPLY_Pos      8            /**< \brief (USBC_UECON1) No Reply */
#define USBC_UECON1_NREPLY          (0x1u << USBC_UECON1_NREPLY_Pos)
#define USBC_UECON1_RAMACERE_Pos    11           /**< \brief (USBC_UECON1) RAMACER Interrupt Enable */
#define USBC_UECON1_RAMACERE        (0x1u << USBC_UECON1_RAMACERE_Pos)
#define USBC_UECON1_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON1) Number of Busy Banks Interrupt Enable */
#define USBC_UECON1_NBUSYBKE        (0x1u << USBC_UECON1_NBUSYBKE_Pos)
#define USBC_UECON1_KILLBK_Pos      13           /**< \brief (USBC_UECON1) Kill IN Bank */
#define USBC_UECON1_KILLBK          (0x1u << USBC_UECON1_KILLBK_Pos)
#define USBC_UECON1_FIFOCON_Pos     14           /**< \brief (USBC_UECON1) FIFO Control */
#define USBC_UECON1_FIFOCON         (0x1u << USBC_UECON1_FIFOCON_Pos)
#define USBC_UECON1_NYETDIS_Pos     17           /**< \brief (USBC_UECON1) NYET Token Enable */
#define USBC_UECON1_NYETDIS         (0x1u << USBC_UECON1_NYETDIS_Pos)
#define USBC_UECON1_RSTDT_Pos       18           /**< \brief (USBC_UECON1) Reset Data Toggle */
#define USBC_UECON1_RSTDT           (0x1u << USBC_UECON1_RSTDT_Pos)
#define USBC_UECON1_STALLRQ_Pos     19           /**< \brief (USBC_UECON1) STALL Request */
#define USBC_UECON1_STALLRQ         (0x1u << USBC_UECON1_STALLRQ_Pos)
#define USBC_UECON1_BUSY0_Pos       24           /**< \brief (USBC_UECON1) Busy Bank1 Enable */
#define USBC_UECON1_BUSY0           (0x1u << USBC_UECON1_BUSY0_Pos)
#define USBC_UECON1_BUSY1_Pos       25           /**< \brief (USBC_UECON1) Busy Bank0 Enable */
#define USBC_UECON1_BUSY1           (0x1u << USBC_UECON1_BUSY1_Pos)
#define USBC_UECON1_MASK            0x030E795Fu  /**< \brief (USBC_UECON1) MASK Register */

/* -------- USBC_UECON2 : (USBC Offset: 0x1C8) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON2_OFFSET          0x1C8        /**< \brief (USBC_UECON2 offset) Endpoint Control Register */
#define USBC_UECON2_RESETVALUE      0x00000000   /**< \brief (USBC_UECON2 reset_value) Endpoint Control Register */

#define USBC_UECON2_TXINE_Pos       0            /**< \brief (USBC_UECON2) TXIN Interrupt Enable */
#define USBC_UECON2_TXINE           (0x1u << USBC_UECON2_TXINE_Pos)
#define USBC_UECON2_RXOUTE_Pos      1            /**< \brief (USBC_UECON2) RXOUT Interrupt Enable */
#define USBC_UECON2_RXOUTE          (0x1u << USBC_UECON2_RXOUTE_Pos)
#define USBC_UECON2_RXSTPE_Pos      2            /**< \brief (USBC_UECON2) RXSTP Interrupt Enable */
#define USBC_UECON2_RXSTPE          (0x1u << USBC_UECON2_RXSTPE_Pos)
#define USBC_UECON2_NAKOUTE_Pos     3            /**< \brief (USBC_UECON2) NAKOUT Interrupt Enable */
#define USBC_UECON2_NAKOUTE         (0x1u << USBC_UECON2_NAKOUTE_Pos)
#define USBC_UECON2_NAKINE_Pos      4            /**< \brief (USBC_UECON2) NAKIN Interrupt Enable */
#define USBC_UECON2_NAKINE          (0x1u << USBC_UECON2_NAKINE_Pos)
#define USBC_UECON2_STALLEDE_Pos    6            /**< \brief (USBC_UECON2) STALLED Interrupt Enable */
#define USBC_UECON2_STALLEDE        (0x1u << USBC_UECON2_STALLEDE_Pos)
#define USBC_UECON2_NREPLY_Pos      8            /**< \brief (USBC_UECON2) No Reply */
#define USBC_UECON2_NREPLY          (0x1u << USBC_UECON2_NREPLY_Pos)
#define USBC_UECON2_RAMACERE_Pos    11           /**< \brief (USBC_UECON2) RAMACER Interrupt Enable */
#define USBC_UECON2_RAMACERE        (0x1u << USBC_UECON2_RAMACERE_Pos)
#define USBC_UECON2_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON2) Number of Busy Banks Interrupt Enable */
#define USBC_UECON2_NBUSYBKE        (0x1u << USBC_UECON2_NBUSYBKE_Pos)
#define USBC_UECON2_KILLBK_Pos      13           /**< \brief (USBC_UECON2) Kill IN Bank */
#define USBC_UECON2_KILLBK          (0x1u << USBC_UECON2_KILLBK_Pos)
#define USBC_UECON2_FIFOCON_Pos     14           /**< \brief (USBC_UECON2) FIFO Control */
#define USBC_UECON2_FIFOCON         (0x1u << USBC_UECON2_FIFOCON_Pos)
#define USBC_UECON2_NYETDIS_Pos     17           /**< \brief (USBC_UECON2) NYET Token Enable */
#define USBC_UECON2_NYETDIS         (0x1u << USBC_UECON2_NYETDIS_Pos)
#define USBC_UECON2_RSTDT_Pos       18           /**< \brief (USBC_UECON2) Reset Data Toggle */
#define USBC_UECON2_RSTDT           (0x1u << USBC_UECON2_RSTDT_Pos)
#define USBC_UECON2_STALLRQ_Pos     19           /**< \brief (USBC_UECON2) STALL Request */
#define USBC_UECON2_STALLRQ         (0x1u << USBC_UECON2_STALLRQ_Pos)
#define USBC_UECON2_BUSY0_Pos       24           /**< \brief (USBC_UECON2) Busy Bank1 Enable */
#define USBC_UECON2_BUSY0           (0x1u << USBC_UECON2_BUSY0_Pos)
#define USBC_UECON2_BUSY1_Pos       25           /**< \brief (USBC_UECON2) Busy Bank0 Enable */
#define USBC_UECON2_BUSY1           (0x1u << USBC_UECON2_BUSY1_Pos)
#define USBC_UECON2_MASK            0x030E795Fu  /**< \brief (USBC_UECON2) MASK Register */

/* -------- USBC_UECON3 : (USBC Offset: 0x1CC) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON3_OFFSET          0x1CC        /**< \brief (USBC_UECON3 offset) Endpoint Control Register */
#define USBC_UECON3_RESETVALUE      0x00000000   /**< \brief (USBC_UECON3 reset_value) Endpoint Control Register */

#define USBC_UECON3_TXINE_Pos       0            /**< \brief (USBC_UECON3) TXIN Interrupt Enable */
#define USBC_UECON3_TXINE           (0x1u << USBC_UECON3_TXINE_Pos)
#define USBC_UECON3_RXOUTE_Pos      1            /**< \brief (USBC_UECON3) RXOUT Interrupt Enable */
#define USBC_UECON3_RXOUTE          (0x1u << USBC_UECON3_RXOUTE_Pos)
#define USBC_UECON3_RXSTPE_Pos      2            /**< \brief (USBC_UECON3) RXSTP Interrupt Enable */
#define USBC_UECON3_RXSTPE          (0x1u << USBC_UECON3_RXSTPE_Pos)
#define USBC_UECON3_NAKOUTE_Pos     3            /**< \brief (USBC_UECON3) NAKOUT Interrupt Enable */
#define USBC_UECON3_NAKOUTE         (0x1u << USBC_UECON3_NAKOUTE_Pos)
#define USBC_UECON3_NAKINE_Pos      4            /**< \brief (USBC_UECON3) NAKIN Interrupt Enable */
#define USBC_UECON3_NAKINE          (0x1u << USBC_UECON3_NAKINE_Pos)
#define USBC_UECON3_STALLEDE_Pos    6            /**< \brief (USBC_UECON3) STALLED Interrupt Enable */
#define USBC_UECON3_STALLEDE        (0x1u << USBC_UECON3_STALLEDE_Pos)
#define USBC_UECON3_NREPLY_Pos      8            /**< \brief (USBC_UECON3) No Reply */
#define USBC_UECON3_NREPLY          (0x1u << USBC_UECON3_NREPLY_Pos)
#define USBC_UECON3_RAMACERE_Pos    11           /**< \brief (USBC_UECON3) RAMACER Interrupt Enable */
#define USBC_UECON3_RAMACERE        (0x1u << USBC_UECON3_RAMACERE_Pos)
#define USBC_UECON3_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON3) Number of Busy Banks Interrupt Enable */
#define USBC_UECON3_NBUSYBKE        (0x1u << USBC_UECON3_NBUSYBKE_Pos)
#define USBC_UECON3_KILLBK_Pos      13           /**< \brief (USBC_UECON3) Kill IN Bank */
#define USBC_UECON3_KILLBK          (0x1u << USBC_UECON3_KILLBK_Pos)
#define USBC_UECON3_FIFOCON_Pos     14           /**< \brief (USBC_UECON3) FIFO Control */
#define USBC_UECON3_FIFOCON         (0x1u << USBC_UECON3_FIFOCON_Pos)
#define USBC_UECON3_NYETDIS_Pos     17           /**< \brief (USBC_UECON3) NYET Token Enable */
#define USBC_UECON3_NYETDIS         (0x1u << USBC_UECON3_NYETDIS_Pos)
#define USBC_UECON3_RSTDT_Pos       18           /**< \brief (USBC_UECON3) Reset Data Toggle */
#define USBC_UECON3_RSTDT           (0x1u << USBC_UECON3_RSTDT_Pos)
#define USBC_UECON3_STALLRQ_Pos     19           /**< \brief (USBC_UECON3) STALL Request */
#define USBC_UECON3_STALLRQ         (0x1u << USBC_UECON3_STALLRQ_Pos)
#define USBC_UECON3_BUSY0_Pos       24           /**< \brief (USBC_UECON3) Busy Bank1 Enable */
#define USBC_UECON3_BUSY0           (0x1u << USBC_UECON3_BUSY0_Pos)
#define USBC_UECON3_BUSY1_Pos       25           /**< \brief (USBC_UECON3) Busy Bank0 Enable */
#define USBC_UECON3_BUSY1           (0x1u << USBC_UECON3_BUSY1_Pos)
#define USBC_UECON3_MASK            0x030E795Fu  /**< \brief (USBC_UECON3) MASK Register */

/* -------- USBC_UECON4 : (USBC Offset: 0x1D0) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON4_OFFSET          0x1D0        /**< \brief (USBC_UECON4 offset) Endpoint Control Register */
#define USBC_UECON4_RESETVALUE      0x00000000   /**< \brief (USBC_UECON4 reset_value) Endpoint Control Register */

#define USBC_UECON4_TXINE_Pos       0            /**< \brief (USBC_UECON4) TXIN Interrupt Enable */
#define USBC_UECON4_TXINE           (0x1u << USBC_UECON4_TXINE_Pos)
#define USBC_UECON4_RXOUTE_Pos      1            /**< \brief (USBC_UECON4) RXOUT Interrupt Enable */
#define USBC_UECON4_RXOUTE          (0x1u << USBC_UECON4_RXOUTE_Pos)
#define USBC_UECON4_RXSTPE_Pos      2            /**< \brief (USBC_UECON4) RXSTP Interrupt Enable */
#define USBC_UECON4_RXSTPE          (0x1u << USBC_UECON4_RXSTPE_Pos)
#define USBC_UECON4_NAKOUTE_Pos     3            /**< \brief (USBC_UECON4) NAKOUT Interrupt Enable */
#define USBC_UECON4_NAKOUTE         (0x1u << USBC_UECON4_NAKOUTE_Pos)
#define USBC_UECON4_NAKINE_Pos      4            /**< \brief (USBC_UECON4) NAKIN Interrupt Enable */
#define USBC_UECON4_NAKINE          (0x1u << USBC_UECON4_NAKINE_Pos)
#define USBC_UECON4_STALLEDE_Pos    6            /**< \brief (USBC_UECON4) STALLED Interrupt Enable */
#define USBC_UECON4_STALLEDE        (0x1u << USBC_UECON4_STALLEDE_Pos)
#define USBC_UECON4_NREPLY_Pos      8            /**< \brief (USBC_UECON4) No Reply */
#define USBC_UECON4_NREPLY          (0x1u << USBC_UECON4_NREPLY_Pos)
#define USBC_UECON4_RAMACERE_Pos    11           /**< \brief (USBC_UECON4) RAMACER Interrupt Enable */
#define USBC_UECON4_RAMACERE        (0x1u << USBC_UECON4_RAMACERE_Pos)
#define USBC_UECON4_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON4) Number of Busy Banks Interrupt Enable */
#define USBC_UECON4_NBUSYBKE        (0x1u << USBC_UECON4_NBUSYBKE_Pos)
#define USBC_UECON4_KILLBK_Pos      13           /**< \brief (USBC_UECON4) Kill IN Bank */
#define USBC_UECON4_KILLBK          (0x1u << USBC_UECON4_KILLBK_Pos)
#define USBC_UECON4_FIFOCON_Pos     14           /**< \brief (USBC_UECON4) FIFO Control */
#define USBC_UECON4_FIFOCON         (0x1u << USBC_UECON4_FIFOCON_Pos)
#define USBC_UECON4_NYETDIS_Pos     17           /**< \brief (USBC_UECON4) NYET Token Enable */
#define USBC_UECON4_NYETDIS         (0x1u << USBC_UECON4_NYETDIS_Pos)
#define USBC_UECON4_RSTDT_Pos       18           /**< \brief (USBC_UECON4) Reset Data Toggle */
#define USBC_UECON4_RSTDT           (0x1u << USBC_UECON4_RSTDT_Pos)
#define USBC_UECON4_STALLRQ_Pos     19           /**< \brief (USBC_UECON4) STALL Request */
#define USBC_UECON4_STALLRQ         (0x1u << USBC_UECON4_STALLRQ_Pos)
#define USBC_UECON4_BUSY0_Pos       24           /**< \brief (USBC_UECON4) Busy Bank1 Enable */
#define USBC_UECON4_BUSY0           (0x1u << USBC_UECON4_BUSY0_Pos)
#define USBC_UECON4_BUSY1_Pos       25           /**< \brief (USBC_UECON4) Busy Bank0 Enable */
#define USBC_UECON4_BUSY1           (0x1u << USBC_UECON4_BUSY1_Pos)
#define USBC_UECON4_MASK            0x030E795Fu  /**< \brief (USBC_UECON4) MASK Register */

/* -------- USBC_UECON5 : (USBC Offset: 0x1D4) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON5_OFFSET          0x1D4        /**< \brief (USBC_UECON5 offset) Endpoint Control Register */
#define USBC_UECON5_RESETVALUE      0x00000000   /**< \brief (USBC_UECON5 reset_value) Endpoint Control Register */

#define USBC_UECON5_TXINE_Pos       0            /**< \brief (USBC_UECON5) TXIN Interrupt Enable */
#define USBC_UECON5_TXINE           (0x1u << USBC_UECON5_TXINE_Pos)
#define USBC_UECON5_RXOUTE_Pos      1            /**< \brief (USBC_UECON5) RXOUT Interrupt Enable */
#define USBC_UECON5_RXOUTE          (0x1u << USBC_UECON5_RXOUTE_Pos)
#define USBC_UECON5_RXSTPE_Pos      2            /**< \brief (USBC_UECON5) RXSTP Interrupt Enable */
#define USBC_UECON5_RXSTPE          (0x1u << USBC_UECON5_RXSTPE_Pos)
#define USBC_UECON5_NAKOUTE_Pos     3            /**< \brief (USBC_UECON5) NAKOUT Interrupt Enable */
#define USBC_UECON5_NAKOUTE         (0x1u << USBC_UECON5_NAKOUTE_Pos)
#define USBC_UECON5_NAKINE_Pos      4            /**< \brief (USBC_UECON5) NAKIN Interrupt Enable */
#define USBC_UECON5_NAKINE          (0x1u << USBC_UECON5_NAKINE_Pos)
#define USBC_UECON5_STALLEDE_Pos    6            /**< \brief (USBC_UECON5) STALLED Interrupt Enable */
#define USBC_UECON5_STALLEDE        (0x1u << USBC_UECON5_STALLEDE_Pos)
#define USBC_UECON5_NREPLY_Pos      8            /**< \brief (USBC_UECON5) No Reply */
#define USBC_UECON5_NREPLY          (0x1u << USBC_UECON5_NREPLY_Pos)
#define USBC_UECON5_RAMACERE_Pos    11           /**< \brief (USBC_UECON5) RAMACER Interrupt Enable */
#define USBC_UECON5_RAMACERE        (0x1u << USBC_UECON5_RAMACERE_Pos)
#define USBC_UECON5_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON5) Number of Busy Banks Interrupt Enable */
#define USBC_UECON5_NBUSYBKE        (0x1u << USBC_UECON5_NBUSYBKE_Pos)
#define USBC_UECON5_KILLBK_Pos      13           /**< \brief (USBC_UECON5) Kill IN Bank */
#define USBC_UECON5_KILLBK          (0x1u << USBC_UECON5_KILLBK_Pos)
#define USBC_UECON5_FIFOCON_Pos     14           /**< \brief (USBC_UECON5) FIFO Control */
#define USBC_UECON5_FIFOCON         (0x1u << USBC_UECON5_FIFOCON_Pos)
#define USBC_UECON5_NYETDIS_Pos     17           /**< \brief (USBC_UECON5) NYET Token Enable */
#define USBC_UECON5_NYETDIS         (0x1u << USBC_UECON5_NYETDIS_Pos)
#define USBC_UECON5_RSTDT_Pos       18           /**< \brief (USBC_UECON5) Reset Data Toggle */
#define USBC_UECON5_RSTDT           (0x1u << USBC_UECON5_RSTDT_Pos)
#define USBC_UECON5_STALLRQ_Pos     19           /**< \brief (USBC_UECON5) STALL Request */
#define USBC_UECON5_STALLRQ         (0x1u << USBC_UECON5_STALLRQ_Pos)
#define USBC_UECON5_BUSY0_Pos       24           /**< \brief (USBC_UECON5) Busy Bank1 Enable */
#define USBC_UECON5_BUSY0           (0x1u << USBC_UECON5_BUSY0_Pos)
#define USBC_UECON5_BUSY1_Pos       25           /**< \brief (USBC_UECON5) Busy Bank0 Enable */
#define USBC_UECON5_BUSY1           (0x1u << USBC_UECON5_BUSY1_Pos)
#define USBC_UECON5_MASK            0x030E795Fu  /**< \brief (USBC_UECON5) MASK Register */

/* -------- USBC_UECON6 : (USBC Offset: 0x1D8) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON6_OFFSET          0x1D8        /**< \brief (USBC_UECON6 offset) Endpoint Control Register */
#define USBC_UECON6_RESETVALUE      0x00000000   /**< \brief (USBC_UECON6 reset_value) Endpoint Control Register */

#define USBC_UECON6_TXINE_Pos       0            /**< \brief (USBC_UECON6) TXIN Interrupt Enable */
#define USBC_UECON6_TXINE           (0x1u << USBC_UECON6_TXINE_Pos)
#define USBC_UECON6_RXOUTE_Pos      1            /**< \brief (USBC_UECON6) RXOUT Interrupt Enable */
#define USBC_UECON6_RXOUTE          (0x1u << USBC_UECON6_RXOUTE_Pos)
#define USBC_UECON6_RXSTPE_Pos      2            /**< \brief (USBC_UECON6) RXSTP Interrupt Enable */
#define USBC_UECON6_RXSTPE          (0x1u << USBC_UECON6_RXSTPE_Pos)
#define USBC_UECON6_NAKOUTE_Pos     3            /**< \brief (USBC_UECON6) NAKOUT Interrupt Enable */
#define USBC_UECON6_NAKOUTE         (0x1u << USBC_UECON6_NAKOUTE_Pos)
#define USBC_UECON6_NAKINE_Pos      4            /**< \brief (USBC_UECON6) NAKIN Interrupt Enable */
#define USBC_UECON6_NAKINE          (0x1u << USBC_UECON6_NAKINE_Pos)
#define USBC_UECON6_STALLEDE_Pos    6            /**< \brief (USBC_UECON6) STALLED Interrupt Enable */
#define USBC_UECON6_STALLEDE        (0x1u << USBC_UECON6_STALLEDE_Pos)
#define USBC_UECON6_NREPLY_Pos      8            /**< \brief (USBC_UECON6) No Reply */
#define USBC_UECON6_NREPLY          (0x1u << USBC_UECON6_NREPLY_Pos)
#define USBC_UECON6_RAMACERE_Pos    11           /**< \brief (USBC_UECON6) RAMACER Interrupt Enable */
#define USBC_UECON6_RAMACERE        (0x1u << USBC_UECON6_RAMACERE_Pos)
#define USBC_UECON6_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON6) Number of Busy Banks Interrupt Enable */
#define USBC_UECON6_NBUSYBKE        (0x1u << USBC_UECON6_NBUSYBKE_Pos)
#define USBC_UECON6_KILLBK_Pos      13           /**< \brief (USBC_UECON6) Kill IN Bank */
#define USBC_UECON6_KILLBK          (0x1u << USBC_UECON6_KILLBK_Pos)
#define USBC_UECON6_FIFOCON_Pos     14           /**< \brief (USBC_UECON6) FIFO Control */
#define USBC_UECON6_FIFOCON         (0x1u << USBC_UECON6_FIFOCON_Pos)
#define USBC_UECON6_NYETDIS_Pos     17           /**< \brief (USBC_UECON6) NYET Token Enable */
#define USBC_UECON6_NYETDIS         (0x1u << USBC_UECON6_NYETDIS_Pos)
#define USBC_UECON6_RSTDT_Pos       18           /**< \brief (USBC_UECON6) Reset Data Toggle */
#define USBC_UECON6_RSTDT           (0x1u << USBC_UECON6_RSTDT_Pos)
#define USBC_UECON6_STALLRQ_Pos     19           /**< \brief (USBC_UECON6) STALL Request */
#define USBC_UECON6_STALLRQ         (0x1u << USBC_UECON6_STALLRQ_Pos)
#define USBC_UECON6_BUSY0_Pos       24           /**< \brief (USBC_UECON6) Busy Bank1 Enable */
#define USBC_UECON6_BUSY0           (0x1u << USBC_UECON6_BUSY0_Pos)
#define USBC_UECON6_BUSY1_Pos       25           /**< \brief (USBC_UECON6) Busy Bank0 Enable */
#define USBC_UECON6_BUSY1           (0x1u << USBC_UECON6_BUSY1_Pos)
#define USBC_UECON6_MASK            0x030E795Fu  /**< \brief (USBC_UECON6) MASK Register */

/* -------- USBC_UECON7 : (USBC Offset: 0x1DC) (R/  32) Endpoint Control Register -------- */
#define USBC_UECON7_OFFSET          0x1DC        /**< \brief (USBC_UECON7 offset) Endpoint Control Register */
#define USBC_UECON7_RESETVALUE      0x00000000   /**< \brief (USBC_UECON7 reset_value) Endpoint Control Register */

#define USBC_UECON7_TXINE_Pos       0            /**< \brief (USBC_UECON7) TXIN Interrupt Enable */
#define USBC_UECON7_TXINE           (0x1u << USBC_UECON7_TXINE_Pos)
#define USBC_UECON7_RXOUTE_Pos      1            /**< \brief (USBC_UECON7) RXOUT Interrupt Enable */
#define USBC_UECON7_RXOUTE          (0x1u << USBC_UECON7_RXOUTE_Pos)
#define USBC_UECON7_RXSTPE_Pos      2            /**< \brief (USBC_UECON7) RXSTP Interrupt Enable */
#define USBC_UECON7_RXSTPE          (0x1u << USBC_UECON7_RXSTPE_Pos)
#define USBC_UECON7_NAKOUTE_Pos     3            /**< \brief (USBC_UECON7) NAKOUT Interrupt Enable */
#define USBC_UECON7_NAKOUTE         (0x1u << USBC_UECON7_NAKOUTE_Pos)
#define USBC_UECON7_NAKINE_Pos      4            /**< \brief (USBC_UECON7) NAKIN Interrupt Enable */
#define USBC_UECON7_NAKINE          (0x1u << USBC_UECON7_NAKINE_Pos)
#define USBC_UECON7_STALLEDE_Pos    6            /**< \brief (USBC_UECON7) STALLED Interrupt Enable */
#define USBC_UECON7_STALLEDE        (0x1u << USBC_UECON7_STALLEDE_Pos)
#define USBC_UECON7_NREPLY_Pos      8            /**< \brief (USBC_UECON7) No Reply */
#define USBC_UECON7_NREPLY          (0x1u << USBC_UECON7_NREPLY_Pos)
#define USBC_UECON7_RAMACERE_Pos    11           /**< \brief (USBC_UECON7) RAMACER Interrupt Enable */
#define USBC_UECON7_RAMACERE        (0x1u << USBC_UECON7_RAMACERE_Pos)
#define USBC_UECON7_NBUSYBKE_Pos    12           /**< \brief (USBC_UECON7) Number of Busy Banks Interrupt Enable */
#define USBC_UECON7_NBUSYBKE        (0x1u << USBC_UECON7_NBUSYBKE_Pos)
#define USBC_UECON7_KILLBK_Pos      13           /**< \brief (USBC_UECON7) Kill IN Bank */
#define USBC_UECON7_KILLBK          (0x1u << USBC_UECON7_KILLBK_Pos)
#define USBC_UECON7_FIFOCON_Pos     14           /**< \brief (USBC_UECON7) FIFO Control */
#define USBC_UECON7_FIFOCON         (0x1u << USBC_UECON7_FIFOCON_Pos)
#define USBC_UECON7_NYETDIS_Pos     17           /**< \brief (USBC_UECON7) NYET Token Enable */
#define USBC_UECON7_NYETDIS         (0x1u << USBC_UECON7_NYETDIS_Pos)
#define USBC_UECON7_RSTDT_Pos       18           /**< \brief (USBC_UECON7) Reset Data Toggle */
#define USBC_UECON7_RSTDT           (0x1u << USBC_UECON7_RSTDT_Pos)
#define USBC_UECON7_STALLRQ_Pos     19           /**< \brief (USBC_UECON7) STALL Request */
#define USBC_UECON7_STALLRQ         (0x1u << USBC_UECON7_STALLRQ_Pos)
#define USBC_UECON7_BUSY0_Pos       24           /**< \brief (USBC_UECON7) Busy Bank1 Enable */
#define USBC_UECON7_BUSY0           (0x1u << USBC_UECON7_BUSY0_Pos)
#define USBC_UECON7_BUSY1_Pos       25           /**< \brief (USBC_UECON7) Busy Bank0 Enable */
#define USBC_UECON7_BUSY1           (0x1u << USBC_UECON7_BUSY1_Pos)
#define USBC_UECON7_MASK            0x030E795Fu  /**< \brief (USBC_UECON7) MASK Register */

/* -------- USBC_UECON0SET : (USBC Offset: 0x1F0) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON0SET_OFFSET       0x1F0        /**< \brief (USBC_UECON0SET offset) Endpoint Control Set Register */
#define USBC_UECON0SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON0SET reset_value) Endpoint Control Set Register */

#define USBC_UECON0SET_TXINES_Pos   0            /**< \brief (USBC_UECON0SET) TXINE Set */
#define USBC_UECON0SET_TXINES       (0x1u << USBC_UECON0SET_TXINES_Pos)
#define USBC_UECON0SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON0SET) RXOUTE Set */
#define USBC_UECON0SET_RXOUTES      (0x1u << USBC_UECON0SET_RXOUTES_Pos)
#define USBC_UECON0SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON0SET) RXSTPE Set */
#define USBC_UECON0SET_RXSTPES      (0x1u << USBC_UECON0SET_RXSTPES_Pos)
#define USBC_UECON0SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON0SET) NAKOUTE Set */
#define USBC_UECON0SET_NAKOUTES     (0x1u << USBC_UECON0SET_NAKOUTES_Pos)
#define USBC_UECON0SET_NAKINES_Pos  4            /**< \brief (USBC_UECON0SET) NAKINE Set */
#define USBC_UECON0SET_NAKINES      (0x1u << USBC_UECON0SET_NAKINES_Pos)
#define USBC_UECON0SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON0SET) STALLEDE Set */
#define USBC_UECON0SET_STALLEDES    (0x1u << USBC_UECON0SET_STALLEDES_Pos)
#define USBC_UECON0SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON0SET) NREPLY Set */
#define USBC_UECON0SET_NREPLYS      (0x1u << USBC_UECON0SET_NREPLYS_Pos)
#define USBC_UECON0SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON0SET) RAMACERE Set */
#define USBC_UECON0SET_RAMACERES    (0x1u << USBC_UECON0SET_RAMACERES_Pos)
#define USBC_UECON0SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON0SET) NBUSYBKE Set */
#define USBC_UECON0SET_NBUSYBKES    (0x1u << USBC_UECON0SET_NBUSYBKES_Pos)
#define USBC_UECON0SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON0SET) KILLBK Set */
#define USBC_UECON0SET_KILLBKS      (0x1u << USBC_UECON0SET_KILLBKS_Pos)
#define USBC_UECON0SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON0SET) NYETDIS Set */
#define USBC_UECON0SET_NYETDISS     (0x1u << USBC_UECON0SET_NYETDISS_Pos)
#define USBC_UECON0SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON0SET) RSTDT Set */
#define USBC_UECON0SET_RSTDTS       (0x1u << USBC_UECON0SET_RSTDTS_Pos)
#define USBC_UECON0SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON0SET) STALLRQ Set */
#define USBC_UECON0SET_STALLRQS     (0x1u << USBC_UECON0SET_STALLRQS_Pos)
#define USBC_UECON0SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON0SET) BUSY0 Set */
#define USBC_UECON0SET_BUSY0S       (0x1u << USBC_UECON0SET_BUSY0S_Pos)
#define USBC_UECON0SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON0SET) BUSY1 Set */
#define USBC_UECON0SET_BUSY1S       (0x1u << USBC_UECON0SET_BUSY1S_Pos)
#define USBC_UECON0SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON0SET) MASK Register */

/* -------- USBC_UECON1SET : (USBC Offset: 0x1F4) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON1SET_OFFSET       0x1F4        /**< \brief (USBC_UECON1SET offset) Endpoint Control Set Register */
#define USBC_UECON1SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON1SET reset_value) Endpoint Control Set Register */

#define USBC_UECON1SET_TXINES_Pos   0            /**< \brief (USBC_UECON1SET) TXINE Set */
#define USBC_UECON1SET_TXINES       (0x1u << USBC_UECON1SET_TXINES_Pos)
#define USBC_UECON1SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON1SET) RXOUTE Set */
#define USBC_UECON1SET_RXOUTES      (0x1u << USBC_UECON1SET_RXOUTES_Pos)
#define USBC_UECON1SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON1SET) RXSTPE Set */
#define USBC_UECON1SET_RXSTPES      (0x1u << USBC_UECON1SET_RXSTPES_Pos)
#define USBC_UECON1SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON1SET) NAKOUTE Set */
#define USBC_UECON1SET_NAKOUTES     (0x1u << USBC_UECON1SET_NAKOUTES_Pos)
#define USBC_UECON1SET_NAKINES_Pos  4            /**< \brief (USBC_UECON1SET) NAKINE Set */
#define USBC_UECON1SET_NAKINES      (0x1u << USBC_UECON1SET_NAKINES_Pos)
#define USBC_UECON1SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON1SET) STALLEDE Set */
#define USBC_UECON1SET_STALLEDES    (0x1u << USBC_UECON1SET_STALLEDES_Pos)
#define USBC_UECON1SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON1SET) NREPLY Set */
#define USBC_UECON1SET_NREPLYS      (0x1u << USBC_UECON1SET_NREPLYS_Pos)
#define USBC_UECON1SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON1SET) RAMACERE Set */
#define USBC_UECON1SET_RAMACERES    (0x1u << USBC_UECON1SET_RAMACERES_Pos)
#define USBC_UECON1SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON1SET) NBUSYBKE Set */
#define USBC_UECON1SET_NBUSYBKES    (0x1u << USBC_UECON1SET_NBUSYBKES_Pos)
#define USBC_UECON1SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON1SET) KILLBK Set */
#define USBC_UECON1SET_KILLBKS      (0x1u << USBC_UECON1SET_KILLBKS_Pos)
#define USBC_UECON1SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON1SET) NYETDIS Set */
#define USBC_UECON1SET_NYETDISS     (0x1u << USBC_UECON1SET_NYETDISS_Pos)
#define USBC_UECON1SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON1SET) RSTDT Set */
#define USBC_UECON1SET_RSTDTS       (0x1u << USBC_UECON1SET_RSTDTS_Pos)
#define USBC_UECON1SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON1SET) STALLRQ Set */
#define USBC_UECON1SET_STALLRQS     (0x1u << USBC_UECON1SET_STALLRQS_Pos)
#define USBC_UECON1SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON1SET) BUSY0 Set */
#define USBC_UECON1SET_BUSY0S       (0x1u << USBC_UECON1SET_BUSY0S_Pos)
#define USBC_UECON1SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON1SET) BUSY1 Set */
#define USBC_UECON1SET_BUSY1S       (0x1u << USBC_UECON1SET_BUSY1S_Pos)
#define USBC_UECON1SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON1SET) MASK Register */

/* -------- USBC_UECON2SET : (USBC Offset: 0x1F8) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON2SET_OFFSET       0x1F8        /**< \brief (USBC_UECON2SET offset) Endpoint Control Set Register */
#define USBC_UECON2SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON2SET reset_value) Endpoint Control Set Register */

#define USBC_UECON2SET_TXINES_Pos   0            /**< \brief (USBC_UECON2SET) TXINE Set */
#define USBC_UECON2SET_TXINES       (0x1u << USBC_UECON2SET_TXINES_Pos)
#define USBC_UECON2SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON2SET) RXOUTE Set */
#define USBC_UECON2SET_RXOUTES      (0x1u << USBC_UECON2SET_RXOUTES_Pos)
#define USBC_UECON2SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON2SET) RXSTPE Set */
#define USBC_UECON2SET_RXSTPES      (0x1u << USBC_UECON2SET_RXSTPES_Pos)
#define USBC_UECON2SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON2SET) NAKOUTE Set */
#define USBC_UECON2SET_NAKOUTES     (0x1u << USBC_UECON2SET_NAKOUTES_Pos)
#define USBC_UECON2SET_NAKINES_Pos  4            /**< \brief (USBC_UECON2SET) NAKINE Set */
#define USBC_UECON2SET_NAKINES      (0x1u << USBC_UECON2SET_NAKINES_Pos)
#define USBC_UECON2SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON2SET) STALLEDE Set */
#define USBC_UECON2SET_STALLEDES    (0x1u << USBC_UECON2SET_STALLEDES_Pos)
#define USBC_UECON2SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON2SET) NREPLY Set */
#define USBC_UECON2SET_NREPLYS      (0x1u << USBC_UECON2SET_NREPLYS_Pos)
#define USBC_UECON2SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON2SET) RAMACERE Set */
#define USBC_UECON2SET_RAMACERES    (0x1u << USBC_UECON2SET_RAMACERES_Pos)
#define USBC_UECON2SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON2SET) NBUSYBKE Set */
#define USBC_UECON2SET_NBUSYBKES    (0x1u << USBC_UECON2SET_NBUSYBKES_Pos)
#define USBC_UECON2SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON2SET) KILLBK Set */
#define USBC_UECON2SET_KILLBKS      (0x1u << USBC_UECON2SET_KILLBKS_Pos)
#define USBC_UECON2SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON2SET) NYETDIS Set */
#define USBC_UECON2SET_NYETDISS     (0x1u << USBC_UECON2SET_NYETDISS_Pos)
#define USBC_UECON2SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON2SET) RSTDT Set */
#define USBC_UECON2SET_RSTDTS       (0x1u << USBC_UECON2SET_RSTDTS_Pos)
#define USBC_UECON2SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON2SET) STALLRQ Set */
#define USBC_UECON2SET_STALLRQS     (0x1u << USBC_UECON2SET_STALLRQS_Pos)
#define USBC_UECON2SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON2SET) BUSY0 Set */
#define USBC_UECON2SET_BUSY0S       (0x1u << USBC_UECON2SET_BUSY0S_Pos)
#define USBC_UECON2SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON2SET) BUSY1 Set */
#define USBC_UECON2SET_BUSY1S       (0x1u << USBC_UECON2SET_BUSY1S_Pos)
#define USBC_UECON2SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON2SET) MASK Register */

/* -------- USBC_UECON3SET : (USBC Offset: 0x1FC) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON3SET_OFFSET       0x1FC        /**< \brief (USBC_UECON3SET offset) Endpoint Control Set Register */
#define USBC_UECON3SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON3SET reset_value) Endpoint Control Set Register */

#define USBC_UECON3SET_TXINES_Pos   0            /**< \brief (USBC_UECON3SET) TXINE Set */
#define USBC_UECON3SET_TXINES       (0x1u << USBC_UECON3SET_TXINES_Pos)
#define USBC_UECON3SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON3SET) RXOUTE Set */
#define USBC_UECON3SET_RXOUTES      (0x1u << USBC_UECON3SET_RXOUTES_Pos)
#define USBC_UECON3SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON3SET) RXSTPE Set */
#define USBC_UECON3SET_RXSTPES      (0x1u << USBC_UECON3SET_RXSTPES_Pos)
#define USBC_UECON3SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON3SET) NAKOUTE Set */
#define USBC_UECON3SET_NAKOUTES     (0x1u << USBC_UECON3SET_NAKOUTES_Pos)
#define USBC_UECON3SET_NAKINES_Pos  4            /**< \brief (USBC_UECON3SET) NAKINE Set */
#define USBC_UECON3SET_NAKINES      (0x1u << USBC_UECON3SET_NAKINES_Pos)
#define USBC_UECON3SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON3SET) STALLEDE Set */
#define USBC_UECON3SET_STALLEDES    (0x1u << USBC_UECON3SET_STALLEDES_Pos)
#define USBC_UECON3SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON3SET) NREPLY Set */
#define USBC_UECON3SET_NREPLYS      (0x1u << USBC_UECON3SET_NREPLYS_Pos)
#define USBC_UECON3SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON3SET) RAMACERE Set */
#define USBC_UECON3SET_RAMACERES    (0x1u << USBC_UECON3SET_RAMACERES_Pos)
#define USBC_UECON3SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON3SET) NBUSYBKE Set */
#define USBC_UECON3SET_NBUSYBKES    (0x1u << USBC_UECON3SET_NBUSYBKES_Pos)
#define USBC_UECON3SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON3SET) KILLBK Set */
#define USBC_UECON3SET_KILLBKS      (0x1u << USBC_UECON3SET_KILLBKS_Pos)
#define USBC_UECON3SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON3SET) NYETDIS Set */
#define USBC_UECON3SET_NYETDISS     (0x1u << USBC_UECON3SET_NYETDISS_Pos)
#define USBC_UECON3SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON3SET) RSTDT Set */
#define USBC_UECON3SET_RSTDTS       (0x1u << USBC_UECON3SET_RSTDTS_Pos)
#define USBC_UECON3SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON3SET) STALLRQ Set */
#define USBC_UECON3SET_STALLRQS     (0x1u << USBC_UECON3SET_STALLRQS_Pos)
#define USBC_UECON3SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON3SET) BUSY0 Set */
#define USBC_UECON3SET_BUSY0S       (0x1u << USBC_UECON3SET_BUSY0S_Pos)
#define USBC_UECON3SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON3SET) BUSY1 Set */
#define USBC_UECON3SET_BUSY1S       (0x1u << USBC_UECON3SET_BUSY1S_Pos)
#define USBC_UECON3SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON3SET) MASK Register */

/* -------- USBC_UECON4SET : (USBC Offset: 0x200) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON4SET_OFFSET       0x200        /**< \brief (USBC_UECON4SET offset) Endpoint Control Set Register */
#define USBC_UECON4SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON4SET reset_value) Endpoint Control Set Register */

#define USBC_UECON4SET_TXINES_Pos   0            /**< \brief (USBC_UECON4SET) TXINE Set */
#define USBC_UECON4SET_TXINES       (0x1u << USBC_UECON4SET_TXINES_Pos)
#define USBC_UECON4SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON4SET) RXOUTE Set */
#define USBC_UECON4SET_RXOUTES      (0x1u << USBC_UECON4SET_RXOUTES_Pos)
#define USBC_UECON4SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON4SET) RXSTPE Set */
#define USBC_UECON4SET_RXSTPES      (0x1u << USBC_UECON4SET_RXSTPES_Pos)
#define USBC_UECON4SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON4SET) NAKOUTE Set */
#define USBC_UECON4SET_NAKOUTES     (0x1u << USBC_UECON4SET_NAKOUTES_Pos)
#define USBC_UECON4SET_NAKINES_Pos  4            /**< \brief (USBC_UECON4SET) NAKINE Set */
#define USBC_UECON4SET_NAKINES      (0x1u << USBC_UECON4SET_NAKINES_Pos)
#define USBC_UECON4SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON4SET) STALLEDE Set */
#define USBC_UECON4SET_STALLEDES    (0x1u << USBC_UECON4SET_STALLEDES_Pos)
#define USBC_UECON4SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON4SET) NREPLY Set */
#define USBC_UECON4SET_NREPLYS      (0x1u << USBC_UECON4SET_NREPLYS_Pos)
#define USBC_UECON4SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON4SET) RAMACERE Set */
#define USBC_UECON4SET_RAMACERES    (0x1u << USBC_UECON4SET_RAMACERES_Pos)
#define USBC_UECON4SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON4SET) NBUSYBKE Set */
#define USBC_UECON4SET_NBUSYBKES    (0x1u << USBC_UECON4SET_NBUSYBKES_Pos)
#define USBC_UECON4SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON4SET) KILLBK Set */
#define USBC_UECON4SET_KILLBKS      (0x1u << USBC_UECON4SET_KILLBKS_Pos)
#define USBC_UECON4SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON4SET) NYETDIS Set */
#define USBC_UECON4SET_NYETDISS     (0x1u << USBC_UECON4SET_NYETDISS_Pos)
#define USBC_UECON4SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON4SET) RSTDT Set */
#define USBC_UECON4SET_RSTDTS       (0x1u << USBC_UECON4SET_RSTDTS_Pos)
#define USBC_UECON4SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON4SET) STALLRQ Set */
#define USBC_UECON4SET_STALLRQS     (0x1u << USBC_UECON4SET_STALLRQS_Pos)
#define USBC_UECON4SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON4SET) BUSY0 Set */
#define USBC_UECON4SET_BUSY0S       (0x1u << USBC_UECON4SET_BUSY0S_Pos)
#define USBC_UECON4SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON4SET) BUSY1 Set */
#define USBC_UECON4SET_BUSY1S       (0x1u << USBC_UECON4SET_BUSY1S_Pos)
#define USBC_UECON4SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON4SET) MASK Register */

/* -------- USBC_UECON5SET : (USBC Offset: 0x204) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON5SET_OFFSET       0x204        /**< \brief (USBC_UECON5SET offset) Endpoint Control Set Register */
#define USBC_UECON5SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON5SET reset_value) Endpoint Control Set Register */

#define USBC_UECON5SET_TXINES_Pos   0            /**< \brief (USBC_UECON5SET) TXINE Set */
#define USBC_UECON5SET_TXINES       (0x1u << USBC_UECON5SET_TXINES_Pos)
#define USBC_UECON5SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON5SET) RXOUTE Set */
#define USBC_UECON5SET_RXOUTES      (0x1u << USBC_UECON5SET_RXOUTES_Pos)
#define USBC_UECON5SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON5SET) RXSTPE Set */
#define USBC_UECON5SET_RXSTPES      (0x1u << USBC_UECON5SET_RXSTPES_Pos)
#define USBC_UECON5SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON5SET) NAKOUTE Set */
#define USBC_UECON5SET_NAKOUTES     (0x1u << USBC_UECON5SET_NAKOUTES_Pos)
#define USBC_UECON5SET_NAKINES_Pos  4            /**< \brief (USBC_UECON5SET) NAKINE Set */
#define USBC_UECON5SET_NAKINES      (0x1u << USBC_UECON5SET_NAKINES_Pos)
#define USBC_UECON5SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON5SET) STALLEDE Set */
#define USBC_UECON5SET_STALLEDES    (0x1u << USBC_UECON5SET_STALLEDES_Pos)
#define USBC_UECON5SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON5SET) NREPLY Set */
#define USBC_UECON5SET_NREPLYS      (0x1u << USBC_UECON5SET_NREPLYS_Pos)
#define USBC_UECON5SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON5SET) RAMACERE Set */
#define USBC_UECON5SET_RAMACERES    (0x1u << USBC_UECON5SET_RAMACERES_Pos)
#define USBC_UECON5SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON5SET) NBUSYBKE Set */
#define USBC_UECON5SET_NBUSYBKES    (0x1u << USBC_UECON5SET_NBUSYBKES_Pos)
#define USBC_UECON5SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON5SET) KILLBK Set */
#define USBC_UECON5SET_KILLBKS      (0x1u << USBC_UECON5SET_KILLBKS_Pos)
#define USBC_UECON5SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON5SET) NYETDIS Set */
#define USBC_UECON5SET_NYETDISS     (0x1u << USBC_UECON5SET_NYETDISS_Pos)
#define USBC_UECON5SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON5SET) RSTDT Set */
#define USBC_UECON5SET_RSTDTS       (0x1u << USBC_UECON5SET_RSTDTS_Pos)
#define USBC_UECON5SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON5SET) STALLRQ Set */
#define USBC_UECON5SET_STALLRQS     (0x1u << USBC_UECON5SET_STALLRQS_Pos)
#define USBC_UECON5SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON5SET) BUSY0 Set */
#define USBC_UECON5SET_BUSY0S       (0x1u << USBC_UECON5SET_BUSY0S_Pos)
#define USBC_UECON5SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON5SET) BUSY1 Set */
#define USBC_UECON5SET_BUSY1S       (0x1u << USBC_UECON5SET_BUSY1S_Pos)
#define USBC_UECON5SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON5SET) MASK Register */

/* -------- USBC_UECON6SET : (USBC Offset: 0x208) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON6SET_OFFSET       0x208        /**< \brief (USBC_UECON6SET offset) Endpoint Control Set Register */
#define USBC_UECON6SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON6SET reset_value) Endpoint Control Set Register */

#define USBC_UECON6SET_TXINES_Pos   0            /**< \brief (USBC_UECON6SET) TXINE Set */
#define USBC_UECON6SET_TXINES       (0x1u << USBC_UECON6SET_TXINES_Pos)
#define USBC_UECON6SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON6SET) RXOUTE Set */
#define USBC_UECON6SET_RXOUTES      (0x1u << USBC_UECON6SET_RXOUTES_Pos)
#define USBC_UECON6SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON6SET) RXSTPE Set */
#define USBC_UECON6SET_RXSTPES      (0x1u << USBC_UECON6SET_RXSTPES_Pos)
#define USBC_UECON6SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON6SET) NAKOUTE Set */
#define USBC_UECON6SET_NAKOUTES     (0x1u << USBC_UECON6SET_NAKOUTES_Pos)
#define USBC_UECON6SET_NAKINES_Pos  4            /**< \brief (USBC_UECON6SET) NAKINE Set */
#define USBC_UECON6SET_NAKINES      (0x1u << USBC_UECON6SET_NAKINES_Pos)
#define USBC_UECON6SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON6SET) STALLEDE Set */
#define USBC_UECON6SET_STALLEDES    (0x1u << USBC_UECON6SET_STALLEDES_Pos)
#define USBC_UECON6SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON6SET) NREPLY Set */
#define USBC_UECON6SET_NREPLYS      (0x1u << USBC_UECON6SET_NREPLYS_Pos)
#define USBC_UECON6SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON6SET) RAMACERE Set */
#define USBC_UECON6SET_RAMACERES    (0x1u << USBC_UECON6SET_RAMACERES_Pos)
#define USBC_UECON6SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON6SET) NBUSYBKE Set */
#define USBC_UECON6SET_NBUSYBKES    (0x1u << USBC_UECON6SET_NBUSYBKES_Pos)
#define USBC_UECON6SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON6SET) KILLBK Set */
#define USBC_UECON6SET_KILLBKS      (0x1u << USBC_UECON6SET_KILLBKS_Pos)
#define USBC_UECON6SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON6SET) NYETDIS Set */
#define USBC_UECON6SET_NYETDISS     (0x1u << USBC_UECON6SET_NYETDISS_Pos)
#define USBC_UECON6SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON6SET) RSTDT Set */
#define USBC_UECON6SET_RSTDTS       (0x1u << USBC_UECON6SET_RSTDTS_Pos)
#define USBC_UECON6SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON6SET) STALLRQ Set */
#define USBC_UECON6SET_STALLRQS     (0x1u << USBC_UECON6SET_STALLRQS_Pos)
#define USBC_UECON6SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON6SET) BUSY0 Set */
#define USBC_UECON6SET_BUSY0S       (0x1u << USBC_UECON6SET_BUSY0S_Pos)
#define USBC_UECON6SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON6SET) BUSY1 Set */
#define USBC_UECON6SET_BUSY1S       (0x1u << USBC_UECON6SET_BUSY1S_Pos)
#define USBC_UECON6SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON6SET) MASK Register */

/* -------- USBC_UECON7SET : (USBC Offset: 0x20C) ( /W 32) Endpoint Control Set Register -------- */
#define USBC_UECON7SET_OFFSET       0x20C        /**< \brief (USBC_UECON7SET offset) Endpoint Control Set Register */
#define USBC_UECON7SET_RESETVALUE   0x00000000   /**< \brief (USBC_UECON7SET reset_value) Endpoint Control Set Register */

#define USBC_UECON7SET_TXINES_Pos   0            /**< \brief (USBC_UECON7SET) TXINE Set */
#define USBC_UECON7SET_TXINES       (0x1u << USBC_UECON7SET_TXINES_Pos)
#define USBC_UECON7SET_RXOUTES_Pos  1            /**< \brief (USBC_UECON7SET) RXOUTE Set */
#define USBC_UECON7SET_RXOUTES      (0x1u << USBC_UECON7SET_RXOUTES_Pos)
#define USBC_UECON7SET_RXSTPES_Pos  2            /**< \brief (USBC_UECON7SET) RXSTPE Set */
#define USBC_UECON7SET_RXSTPES      (0x1u << USBC_UECON7SET_RXSTPES_Pos)
#define USBC_UECON7SET_NAKOUTES_Pos 3            /**< \brief (USBC_UECON7SET) NAKOUTE Set */
#define USBC_UECON7SET_NAKOUTES     (0x1u << USBC_UECON7SET_NAKOUTES_Pos)
#define USBC_UECON7SET_NAKINES_Pos  4            /**< \brief (USBC_UECON7SET) NAKINE Set */
#define USBC_UECON7SET_NAKINES      (0x1u << USBC_UECON7SET_NAKINES_Pos)
#define USBC_UECON7SET_STALLEDES_Pos 6            /**< \brief (USBC_UECON7SET) STALLEDE Set */
#define USBC_UECON7SET_STALLEDES    (0x1u << USBC_UECON7SET_STALLEDES_Pos)
#define USBC_UECON7SET_NREPLYS_Pos  8            /**< \brief (USBC_UECON7SET) NREPLY Set */
#define USBC_UECON7SET_NREPLYS      (0x1u << USBC_UECON7SET_NREPLYS_Pos)
#define USBC_UECON7SET_RAMACERES_Pos 11           /**< \brief (USBC_UECON7SET) RAMACERE Set */
#define USBC_UECON7SET_RAMACERES    (0x1u << USBC_UECON7SET_RAMACERES_Pos)
#define USBC_UECON7SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UECON7SET) NBUSYBKE Set */
#define USBC_UECON7SET_NBUSYBKES    (0x1u << USBC_UECON7SET_NBUSYBKES_Pos)
#define USBC_UECON7SET_KILLBKS_Pos  13           /**< \brief (USBC_UECON7SET) KILLBK Set */
#define USBC_UECON7SET_KILLBKS      (0x1u << USBC_UECON7SET_KILLBKS_Pos)
#define USBC_UECON7SET_NYETDISS_Pos 17           /**< \brief (USBC_UECON7SET) NYETDIS Set */
#define USBC_UECON7SET_NYETDISS     (0x1u << USBC_UECON7SET_NYETDISS_Pos)
#define USBC_UECON7SET_RSTDTS_Pos   18           /**< \brief (USBC_UECON7SET) RSTDT Set */
#define USBC_UECON7SET_RSTDTS       (0x1u << USBC_UECON7SET_RSTDTS_Pos)
#define USBC_UECON7SET_STALLRQS_Pos 19           /**< \brief (USBC_UECON7SET) STALLRQ Set */
#define USBC_UECON7SET_STALLRQS     (0x1u << USBC_UECON7SET_STALLRQS_Pos)
#define USBC_UECON7SET_BUSY0S_Pos   24           /**< \brief (USBC_UECON7SET) BUSY0 Set */
#define USBC_UECON7SET_BUSY0S       (0x1u << USBC_UECON7SET_BUSY0S_Pos)
#define USBC_UECON7SET_BUSY1S_Pos   25           /**< \brief (USBC_UECON7SET) BUSY1 Set */
#define USBC_UECON7SET_BUSY1S       (0x1u << USBC_UECON7SET_BUSY1S_Pos)
#define USBC_UECON7SET_MASK         0x030E395Fu  /**< \brief (USBC_UECON7SET) MASK Register */

/* -------- USBC_UECON0CLR : (USBC Offset: 0x220) ( /W 32) Endpoint Control Clear Register -------- */
#define USBC_UECON0CLR_OFFSET       0x220        /**< \brief (USBC_UECON0CLR offset) Endpoint Control Clear Register */
#define USBC_UECON0CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON0CLR reset_value) Endpoint Control Clear Register */

#define USBC_UECON0CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON0CLR) TXINE Clear */
#define USBC_UECON0CLR_TXINEC       (0x1u << USBC_UECON0CLR_TXINEC_Pos)
#define USBC_UECON0CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON0CLR) RXOUTE Clear */
#define USBC_UECON0CLR_RXOUTEC      (0x1u << USBC_UECON0CLR_RXOUTEC_Pos)
#define USBC_UECON0CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON0CLR) RXSTPE Clear */
#define USBC_UECON0CLR_RXSTPEC      (0x1u << USBC_UECON0CLR_RXSTPEC_Pos)
#define USBC_UECON0CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON0CLR) NAKOUTE Clear */
#define USBC_UECON0CLR_NAKOUTEC     (0x1u << USBC_UECON0CLR_NAKOUTEC_Pos)
#define USBC_UECON0CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON0CLR) NAKINE Clear */
#define USBC_UECON0CLR_NAKINEC      (0x1u << USBC_UECON0CLR_NAKINEC_Pos)
#define USBC_UECON0CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON0CLR) STALLEDE Clear */
#define USBC_UECON0CLR_STALLEDEC    (0x1u << USBC_UECON0CLR_STALLEDEC_Pos)
#define USBC_UECON0CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON0CLR) NREPLY Clear */
#define USBC_UECON0CLR_NREPLYC      (0x1u << USBC_UECON0CLR_NREPLYC_Pos)
#define USBC_UECON0CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON0CLR) RAMACERE Clear */
#define USBC_UECON0CLR_RAMACEREC    (0x1u << USBC_UECON0CLR_RAMACEREC_Pos)
#define USBC_UECON0CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON0CLR) NBUSYBKE Clear */
#define USBC_UECON0CLR_NBUSYBKEC    (0x1u << USBC_UECON0CLR_NBUSYBKEC_Pos)
#define USBC_UECON0CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON0CLR) FIFOCON Clear */
#define USBC_UECON0CLR_FIFOCONC     (0x1u << USBC_UECON0CLR_FIFOCONC_Pos)
#define USBC_UECON0CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON0CLR) NYETDIS Clear */
#define USBC_UECON0CLR_NYETDISC     (0x1u << USBC_UECON0CLR_NYETDISC_Pos)
#define USBC_UECON0CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON0CLR) STALLRQ Clear */
#define USBC_UECON0CLR_STALLRQC     (0x1u << USBC_UECON0CLR_STALLRQC_Pos)
#define USBC_UECON0CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON0CLR) BUSY0 Clear */
#define USBC_UECON0CLR_BUSY0C       (0x1u << USBC_UECON0CLR_BUSY0C_Pos)
#define USBC_UECON0CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON0CLR) BUSY1 Clear */
#define USBC_UECON0CLR_BUSY1C       (0x1u << USBC_UECON0CLR_BUSY1C_Pos)
#define USBC_UECON0CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON0CLR) MASK Register */

/* -------- USBC_UECON1CLR : (USBC Offset: 0x224) ( /W 32) TXINE Clear -------- */
#define USBC_UECON1CLR_OFFSET       0x224        /**< \brief (USBC_UECON1CLR offset) TXINE Clear */
#define USBC_UECON1CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON1CLR reset_value) TXINE Clear */

#define USBC_UECON1CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON1CLR) TXINE Clear */
#define USBC_UECON1CLR_TXINEC       (0x1u << USBC_UECON1CLR_TXINEC_Pos)
#define USBC_UECON1CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON1CLR) RXOUTE Clear */
#define USBC_UECON1CLR_RXOUTEC      (0x1u << USBC_UECON1CLR_RXOUTEC_Pos)
#define USBC_UECON1CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON1CLR) RXOUTE Clear */
#define USBC_UECON1CLR_RXSTPEC      (0x1u << USBC_UECON1CLR_RXSTPEC_Pos)
#define USBC_UECON1CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON1CLR) NAKOUTE Clear */
#define USBC_UECON1CLR_NAKOUTEC     (0x1u << USBC_UECON1CLR_NAKOUTEC_Pos)
#define USBC_UECON1CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON1CLR) NAKINE Clear */
#define USBC_UECON1CLR_NAKINEC      (0x1u << USBC_UECON1CLR_NAKINEC_Pos)
#define USBC_UECON1CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON1CLR) RXSTPE Clear */
#define USBC_UECON1CLR_STALLEDEC    (0x1u << USBC_UECON1CLR_STALLEDEC_Pos)
#define USBC_UECON1CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON1CLR) NREPLY Clear */
#define USBC_UECON1CLR_NREPLYC      (0x1u << USBC_UECON1CLR_NREPLYC_Pos)
#define USBC_UECON1CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON1CLR) RAMACERE Clear */
#define USBC_UECON1CLR_RAMACEREC    (0x1u << USBC_UECON1CLR_RAMACEREC_Pos)
#define USBC_UECON1CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON1CLR) NBUSYBKE Clear */
#define USBC_UECON1CLR_NBUSYBKEC    (0x1u << USBC_UECON1CLR_NBUSYBKEC_Pos)
#define USBC_UECON1CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON1CLR) FIFOCON Clear */
#define USBC_UECON1CLR_FIFOCONC     (0x1u << USBC_UECON1CLR_FIFOCONC_Pos)
#define USBC_UECON1CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON1CLR) NYETDIS Clear */
#define USBC_UECON1CLR_NYETDISC     (0x1u << USBC_UECON1CLR_NYETDISC_Pos)
#define USBC_UECON1CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON1CLR) STALLEDE Clear */
#define USBC_UECON1CLR_STALLRQC     (0x1u << USBC_UECON1CLR_STALLRQC_Pos)
#define USBC_UECON1CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON1CLR) BUSY0 Clear */
#define USBC_UECON1CLR_BUSY0C       (0x1u << USBC_UECON1CLR_BUSY0C_Pos)
#define USBC_UECON1CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON1CLR) BUSY1 Clear */
#define USBC_UECON1CLR_BUSY1C       (0x1u << USBC_UECON1CLR_BUSY1C_Pos)
#define USBC_UECON1CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON1CLR) MASK Register */

/* -------- USBC_UECON2CLR : (USBC Offset: 0x228) ( /W 32) TXINE Clear -------- */
#define USBC_UECON2CLR_OFFSET       0x228        /**< \brief (USBC_UECON2CLR offset) TXINE Clear */
#define USBC_UECON2CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON2CLR reset_value) TXINE Clear */

#define USBC_UECON2CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON2CLR) TXINE Clear */
#define USBC_UECON2CLR_TXINEC       (0x1u << USBC_UECON2CLR_TXINEC_Pos)
#define USBC_UECON2CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON2CLR) RXOUTE Clear */
#define USBC_UECON2CLR_RXOUTEC      (0x1u << USBC_UECON2CLR_RXOUTEC_Pos)
#define USBC_UECON2CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON2CLR) RXOUTE Clear */
#define USBC_UECON2CLR_RXSTPEC      (0x1u << USBC_UECON2CLR_RXSTPEC_Pos)
#define USBC_UECON2CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON2CLR) NAKOUTE Clear */
#define USBC_UECON2CLR_NAKOUTEC     (0x1u << USBC_UECON2CLR_NAKOUTEC_Pos)
#define USBC_UECON2CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON2CLR) NAKINE Clear */
#define USBC_UECON2CLR_NAKINEC      (0x1u << USBC_UECON2CLR_NAKINEC_Pos)
#define USBC_UECON2CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON2CLR) RXSTPE Clear */
#define USBC_UECON2CLR_STALLEDEC    (0x1u << USBC_UECON2CLR_STALLEDEC_Pos)
#define USBC_UECON2CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON2CLR) NREPLY Clear */
#define USBC_UECON2CLR_NREPLYC      (0x1u << USBC_UECON2CLR_NREPLYC_Pos)
#define USBC_UECON2CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON2CLR) RAMACERE Clear */
#define USBC_UECON2CLR_RAMACEREC    (0x1u << USBC_UECON2CLR_RAMACEREC_Pos)
#define USBC_UECON2CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON2CLR) NBUSYBKE Clear */
#define USBC_UECON2CLR_NBUSYBKEC    (0x1u << USBC_UECON2CLR_NBUSYBKEC_Pos)
#define USBC_UECON2CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON2CLR) FIFOCON Clear */
#define USBC_UECON2CLR_FIFOCONC     (0x1u << USBC_UECON2CLR_FIFOCONC_Pos)
#define USBC_UECON2CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON2CLR) NYETDIS Clear */
#define USBC_UECON2CLR_NYETDISC     (0x1u << USBC_UECON2CLR_NYETDISC_Pos)
#define USBC_UECON2CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON2CLR) STALLEDE Clear */
#define USBC_UECON2CLR_STALLRQC     (0x1u << USBC_UECON2CLR_STALLRQC_Pos)
#define USBC_UECON2CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON2CLR) BUSY0 Clear */
#define USBC_UECON2CLR_BUSY0C       (0x1u << USBC_UECON2CLR_BUSY0C_Pos)
#define USBC_UECON2CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON2CLR) BUSY1 Clear */
#define USBC_UECON2CLR_BUSY1C       (0x1u << USBC_UECON2CLR_BUSY1C_Pos)
#define USBC_UECON2CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON2CLR) MASK Register */

/* -------- USBC_UECON3CLR : (USBC Offset: 0x22C) ( /W 32) TXINE Clear -------- */
#define USBC_UECON3CLR_OFFSET       0x22C        /**< \brief (USBC_UECON3CLR offset) TXINE Clear */
#define USBC_UECON3CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON3CLR reset_value) TXINE Clear */

#define USBC_UECON3CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON3CLR) TXINE Clear */
#define USBC_UECON3CLR_TXINEC       (0x1u << USBC_UECON3CLR_TXINEC_Pos)
#define USBC_UECON3CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON3CLR) RXOUTE Clear */
#define USBC_UECON3CLR_RXOUTEC      (0x1u << USBC_UECON3CLR_RXOUTEC_Pos)
#define USBC_UECON3CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON3CLR) RXOUTE Clear */
#define USBC_UECON3CLR_RXSTPEC      (0x1u << USBC_UECON3CLR_RXSTPEC_Pos)
#define USBC_UECON3CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON3CLR) NAKOUTE Clear */
#define USBC_UECON3CLR_NAKOUTEC     (0x1u << USBC_UECON3CLR_NAKOUTEC_Pos)
#define USBC_UECON3CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON3CLR) NAKINE Clear */
#define USBC_UECON3CLR_NAKINEC      (0x1u << USBC_UECON3CLR_NAKINEC_Pos)
#define USBC_UECON3CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON3CLR) RXSTPE Clear */
#define USBC_UECON3CLR_STALLEDEC    (0x1u << USBC_UECON3CLR_STALLEDEC_Pos)
#define USBC_UECON3CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON3CLR) NREPLY Clear */
#define USBC_UECON3CLR_NREPLYC      (0x1u << USBC_UECON3CLR_NREPLYC_Pos)
#define USBC_UECON3CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON3CLR) RAMACERE Clear */
#define USBC_UECON3CLR_RAMACEREC    (0x1u << USBC_UECON3CLR_RAMACEREC_Pos)
#define USBC_UECON3CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON3CLR) NBUSYBKE Clear */
#define USBC_UECON3CLR_NBUSYBKEC    (0x1u << USBC_UECON3CLR_NBUSYBKEC_Pos)
#define USBC_UECON3CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON3CLR) FIFOCON Clear */
#define USBC_UECON3CLR_FIFOCONC     (0x1u << USBC_UECON3CLR_FIFOCONC_Pos)
#define USBC_UECON3CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON3CLR) NYETDIS Clear */
#define USBC_UECON3CLR_NYETDISC     (0x1u << USBC_UECON3CLR_NYETDISC_Pos)
#define USBC_UECON3CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON3CLR) STALLEDE Clear */
#define USBC_UECON3CLR_STALLRQC     (0x1u << USBC_UECON3CLR_STALLRQC_Pos)
#define USBC_UECON3CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON3CLR) BUSY0 Clear */
#define USBC_UECON3CLR_BUSY0C       (0x1u << USBC_UECON3CLR_BUSY0C_Pos)
#define USBC_UECON3CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON3CLR) BUSY1 Clear */
#define USBC_UECON3CLR_BUSY1C       (0x1u << USBC_UECON3CLR_BUSY1C_Pos)
#define USBC_UECON3CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON3CLR) MASK Register */

/* -------- USBC_UECON4CLR : (USBC Offset: 0x230) ( /W 32) TXINE Clear -------- */
#define USBC_UECON4CLR_OFFSET       0x230        /**< \brief (USBC_UECON4CLR offset) TXINE Clear */
#define USBC_UECON4CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON4CLR reset_value) TXINE Clear */

#define USBC_UECON4CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON4CLR) TXINE Clear */
#define USBC_UECON4CLR_TXINEC       (0x1u << USBC_UECON4CLR_TXINEC_Pos)
#define USBC_UECON4CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON4CLR) RXOUTE Clear */
#define USBC_UECON4CLR_RXOUTEC      (0x1u << USBC_UECON4CLR_RXOUTEC_Pos)
#define USBC_UECON4CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON4CLR) RXOUTE Clear */
#define USBC_UECON4CLR_RXSTPEC      (0x1u << USBC_UECON4CLR_RXSTPEC_Pos)
#define USBC_UECON4CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON4CLR) NAKOUTE Clear */
#define USBC_UECON4CLR_NAKOUTEC     (0x1u << USBC_UECON4CLR_NAKOUTEC_Pos)
#define USBC_UECON4CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON4CLR) NAKINE Clear */
#define USBC_UECON4CLR_NAKINEC      (0x1u << USBC_UECON4CLR_NAKINEC_Pos)
#define USBC_UECON4CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON4CLR) RXSTPE Clear */
#define USBC_UECON4CLR_STALLEDEC    (0x1u << USBC_UECON4CLR_STALLEDEC_Pos)
#define USBC_UECON4CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON4CLR) NREPLY Clear */
#define USBC_UECON4CLR_NREPLYC      (0x1u << USBC_UECON4CLR_NREPLYC_Pos)
#define USBC_UECON4CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON4CLR) RAMACERE Clear */
#define USBC_UECON4CLR_RAMACEREC    (0x1u << USBC_UECON4CLR_RAMACEREC_Pos)
#define USBC_UECON4CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON4CLR) NBUSYBKE Clear */
#define USBC_UECON4CLR_NBUSYBKEC    (0x1u << USBC_UECON4CLR_NBUSYBKEC_Pos)
#define USBC_UECON4CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON4CLR) FIFOCON Clear */
#define USBC_UECON4CLR_FIFOCONC     (0x1u << USBC_UECON4CLR_FIFOCONC_Pos)
#define USBC_UECON4CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON4CLR) NYETDIS Clear */
#define USBC_UECON4CLR_NYETDISC     (0x1u << USBC_UECON4CLR_NYETDISC_Pos)
#define USBC_UECON4CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON4CLR) STALLEDE Clear */
#define USBC_UECON4CLR_STALLRQC     (0x1u << USBC_UECON4CLR_STALLRQC_Pos)
#define USBC_UECON4CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON4CLR) BUSY0 Clear */
#define USBC_UECON4CLR_BUSY0C       (0x1u << USBC_UECON4CLR_BUSY0C_Pos)
#define USBC_UECON4CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON4CLR) BUSY1 Clear */
#define USBC_UECON4CLR_BUSY1C       (0x1u << USBC_UECON4CLR_BUSY1C_Pos)
#define USBC_UECON4CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON4CLR) MASK Register */

/* -------- USBC_UECON5CLR : (USBC Offset: 0x234) ( /W 32) TXINE Clear -------- */
#define USBC_UECON5CLR_OFFSET       0x234        /**< \brief (USBC_UECON5CLR offset) TXINE Clear */
#define USBC_UECON5CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON5CLR reset_value) TXINE Clear */

#define USBC_UECON5CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON5CLR) TXINE Clear */
#define USBC_UECON5CLR_TXINEC       (0x1u << USBC_UECON5CLR_TXINEC_Pos)
#define USBC_UECON5CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON5CLR) RXOUTE Clear */
#define USBC_UECON5CLR_RXOUTEC      (0x1u << USBC_UECON5CLR_RXOUTEC_Pos)
#define USBC_UECON5CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON5CLR) RXOUTE Clear */
#define USBC_UECON5CLR_RXSTPEC      (0x1u << USBC_UECON5CLR_RXSTPEC_Pos)
#define USBC_UECON5CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON5CLR) NAKOUTE Clear */
#define USBC_UECON5CLR_NAKOUTEC     (0x1u << USBC_UECON5CLR_NAKOUTEC_Pos)
#define USBC_UECON5CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON5CLR) NAKINE Clear */
#define USBC_UECON5CLR_NAKINEC      (0x1u << USBC_UECON5CLR_NAKINEC_Pos)
#define USBC_UECON5CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON5CLR) RXSTPE Clear */
#define USBC_UECON5CLR_STALLEDEC    (0x1u << USBC_UECON5CLR_STALLEDEC_Pos)
#define USBC_UECON5CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON5CLR) NREPLY Clear */
#define USBC_UECON5CLR_NREPLYC      (0x1u << USBC_UECON5CLR_NREPLYC_Pos)
#define USBC_UECON5CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON5CLR) RAMACERE Clear */
#define USBC_UECON5CLR_RAMACEREC    (0x1u << USBC_UECON5CLR_RAMACEREC_Pos)
#define USBC_UECON5CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON5CLR) NBUSYBKE Clear */
#define USBC_UECON5CLR_NBUSYBKEC    (0x1u << USBC_UECON5CLR_NBUSYBKEC_Pos)
#define USBC_UECON5CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON5CLR) FIFOCON Clear */
#define USBC_UECON5CLR_FIFOCONC     (0x1u << USBC_UECON5CLR_FIFOCONC_Pos)
#define USBC_UECON5CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON5CLR) NYETDIS Clear */
#define USBC_UECON5CLR_NYETDISC     (0x1u << USBC_UECON5CLR_NYETDISC_Pos)
#define USBC_UECON5CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON5CLR) STALLEDE Clear */
#define USBC_UECON5CLR_STALLRQC     (0x1u << USBC_UECON5CLR_STALLRQC_Pos)
#define USBC_UECON5CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON5CLR) BUSY0 Clear */
#define USBC_UECON5CLR_BUSY0C       (0x1u << USBC_UECON5CLR_BUSY0C_Pos)
#define USBC_UECON5CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON5CLR) BUSY1 Clear */
#define USBC_UECON5CLR_BUSY1C       (0x1u << USBC_UECON5CLR_BUSY1C_Pos)
#define USBC_UECON5CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON5CLR) MASK Register */

/* -------- USBC_UECON6CLR : (USBC Offset: 0x238) ( /W 32) TXINE Clear -------- */
#define USBC_UECON6CLR_OFFSET       0x238        /**< \brief (USBC_UECON6CLR offset) TXINE Clear */
#define USBC_UECON6CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON6CLR reset_value) TXINE Clear */

#define USBC_UECON6CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON6CLR) TXINE Clear */
#define USBC_UECON6CLR_TXINEC       (0x1u << USBC_UECON6CLR_TXINEC_Pos)
#define USBC_UECON6CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON6CLR) RXOUTE Clear */
#define USBC_UECON6CLR_RXOUTEC      (0x1u << USBC_UECON6CLR_RXOUTEC_Pos)
#define USBC_UECON6CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON6CLR) RXOUTE Clear */
#define USBC_UECON6CLR_RXSTPEC      (0x1u << USBC_UECON6CLR_RXSTPEC_Pos)
#define USBC_UECON6CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON6CLR) NAKOUTE Clear */
#define USBC_UECON6CLR_NAKOUTEC     (0x1u << USBC_UECON6CLR_NAKOUTEC_Pos)
#define USBC_UECON6CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON6CLR) NAKINE Clear */
#define USBC_UECON6CLR_NAKINEC      (0x1u << USBC_UECON6CLR_NAKINEC_Pos)
#define USBC_UECON6CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON6CLR) RXSTPE Clear */
#define USBC_UECON6CLR_STALLEDEC    (0x1u << USBC_UECON6CLR_STALLEDEC_Pos)
#define USBC_UECON6CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON6CLR) NREPLY Clear */
#define USBC_UECON6CLR_NREPLYC      (0x1u << USBC_UECON6CLR_NREPLYC_Pos)
#define USBC_UECON6CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON6CLR) RAMACERE Clear */
#define USBC_UECON6CLR_RAMACEREC    (0x1u << USBC_UECON6CLR_RAMACEREC_Pos)
#define USBC_UECON6CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON6CLR) NBUSYBKE Clear */
#define USBC_UECON6CLR_NBUSYBKEC    (0x1u << USBC_UECON6CLR_NBUSYBKEC_Pos)
#define USBC_UECON6CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON6CLR) FIFOCON Clear */
#define USBC_UECON6CLR_FIFOCONC     (0x1u << USBC_UECON6CLR_FIFOCONC_Pos)
#define USBC_UECON6CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON6CLR) NYETDIS Clear */
#define USBC_UECON6CLR_NYETDISC     (0x1u << USBC_UECON6CLR_NYETDISC_Pos)
#define USBC_UECON6CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON6CLR) STALLEDE Clear */
#define USBC_UECON6CLR_STALLRQC     (0x1u << USBC_UECON6CLR_STALLRQC_Pos)
#define USBC_UECON6CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON6CLR) BUSY0 Clear */
#define USBC_UECON6CLR_BUSY0C       (0x1u << USBC_UECON6CLR_BUSY0C_Pos)
#define USBC_UECON6CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON6CLR) BUSY1 Clear */
#define USBC_UECON6CLR_BUSY1C       (0x1u << USBC_UECON6CLR_BUSY1C_Pos)
#define USBC_UECON6CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON6CLR) MASK Register */

/* -------- USBC_UECON7CLR : (USBC Offset: 0x23C) ( /W 32) TXINE Clear -------- */
#define USBC_UECON7CLR_OFFSET       0x23C        /**< \brief (USBC_UECON7CLR offset) TXINE Clear */
#define USBC_UECON7CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UECON7CLR reset_value) TXINE Clear */

#define USBC_UECON7CLR_TXINEC_Pos   0            /**< \brief (USBC_UECON7CLR) TXINE Clear */
#define USBC_UECON7CLR_TXINEC       (0x1u << USBC_UECON7CLR_TXINEC_Pos)
#define USBC_UECON7CLR_RXOUTEC_Pos  1            /**< \brief (USBC_UECON7CLR) RXOUTE Clear */
#define USBC_UECON7CLR_RXOUTEC      (0x1u << USBC_UECON7CLR_RXOUTEC_Pos)
#define USBC_UECON7CLR_RXSTPEC_Pos  2            /**< \brief (USBC_UECON7CLR) RXOUTE Clear */
#define USBC_UECON7CLR_RXSTPEC      (0x1u << USBC_UECON7CLR_RXSTPEC_Pos)
#define USBC_UECON7CLR_NAKOUTEC_Pos 3            /**< \brief (USBC_UECON7CLR) NAKOUTE Clear */
#define USBC_UECON7CLR_NAKOUTEC     (0x1u << USBC_UECON7CLR_NAKOUTEC_Pos)
#define USBC_UECON7CLR_NAKINEC_Pos  4            /**< \brief (USBC_UECON7CLR) NAKINE Clear */
#define USBC_UECON7CLR_NAKINEC      (0x1u << USBC_UECON7CLR_NAKINEC_Pos)
#define USBC_UECON7CLR_STALLEDEC_Pos 6            /**< \brief (USBC_UECON7CLR) RXSTPE Clear */
#define USBC_UECON7CLR_STALLEDEC    (0x1u << USBC_UECON7CLR_STALLEDEC_Pos)
#define USBC_UECON7CLR_NREPLYC_Pos  8            /**< \brief (USBC_UECON7CLR) NREPLY Clear */
#define USBC_UECON7CLR_NREPLYC      (0x1u << USBC_UECON7CLR_NREPLYC_Pos)
#define USBC_UECON7CLR_RAMACEREC_Pos 11           /**< \brief (USBC_UECON7CLR) RAMACERE Clear */
#define USBC_UECON7CLR_RAMACEREC    (0x1u << USBC_UECON7CLR_RAMACEREC_Pos)
#define USBC_UECON7CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UECON7CLR) NBUSYBKE Clear */
#define USBC_UECON7CLR_NBUSYBKEC    (0x1u << USBC_UECON7CLR_NBUSYBKEC_Pos)
#define USBC_UECON7CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UECON7CLR) FIFOCON Clear */
#define USBC_UECON7CLR_FIFOCONC     (0x1u << USBC_UECON7CLR_FIFOCONC_Pos)
#define USBC_UECON7CLR_NYETDISC_Pos 17           /**< \brief (USBC_UECON7CLR) NYETDIS Clear */
#define USBC_UECON7CLR_NYETDISC     (0x1u << USBC_UECON7CLR_NYETDISC_Pos)
#define USBC_UECON7CLR_STALLRQC_Pos 19           /**< \brief (USBC_UECON7CLR) STALLEDE Clear */
#define USBC_UECON7CLR_STALLRQC     (0x1u << USBC_UECON7CLR_STALLRQC_Pos)
#define USBC_UECON7CLR_BUSY0C_Pos   24           /**< \brief (USBC_UECON7CLR) BUSY0 Clear */
#define USBC_UECON7CLR_BUSY0C       (0x1u << USBC_UECON7CLR_BUSY0C_Pos)
#define USBC_UECON7CLR_BUSY1C_Pos   25           /**< \brief (USBC_UECON7CLR) BUSY1 Clear */
#define USBC_UECON7CLR_BUSY1C       (0x1u << USBC_UECON7CLR_BUSY1C_Pos)
#define USBC_UECON7CLR_MASK         0x030A595Fu  /**< \brief (USBC_UECON7CLR) MASK Register */

/* -------- USBC_UHCON : (USBC Offset: 0x400) (R/W 32) Host General Control Register -------- */
#define USBC_UHCON_OFFSET           0x400        /**< \brief (USBC_UHCON offset) Host General Control Register */
#define USBC_UHCON_RESETVALUE       0x00000000   /**< \brief (USBC_UHCON reset_value) Host General Control Register */

#define USBC_UHCON_SOFE_Pos         8            /**< \brief (USBC_UHCON) SOF Enable */
#define USBC_UHCON_SOFE             (0x1u << USBC_UHCON_SOFE_Pos)
#define USBC_UHCON_RESET_Pos        9            /**< \brief (USBC_UHCON) Send USB Reset */
#define USBC_UHCON_RESET            (0x1u << USBC_UHCON_RESET_Pos)
#define USBC_UHCON_RESUME_Pos       10           /**< \brief (USBC_UHCON) Send USB Resume */
#define USBC_UHCON_RESUME           (0x1u << USBC_UHCON_RESUME_Pos)
#define USBC_UHCON_SPDCONF_Pos      12           /**< \brief (USBC_UHCON) Speed Configuration */
#define USBC_UHCON_SPDCONF_Msk      (0x3u << USBC_UHCON_SPDCONF_Pos)
#define USBC_UHCON_SPDCONF(value)   ((USBC_UHCON_SPDCONF_Msk & ((value) << USBC_UHCON_SPDCONF_Pos)))
#define USBC_UHCON_TSTJ_Pos         16           /**< \brief (USBC_UHCON) Test J */
#define USBC_UHCON_TSTJ             (0x1u << USBC_UHCON_TSTJ_Pos)
#define USBC_UHCON_TSTK_Pos         17           /**< \brief (USBC_UHCON) Test K */
#define USBC_UHCON_TSTK             (0x1u << USBC_UHCON_TSTK_Pos)
#define USBC_UHCON_MASK             0x00033700u  /**< \brief (USBC_UHCON) MASK Register */

/* -------- USBC_UHINT : (USBC Offset: 0x404) (R/  32) Host Global Interrupt Register -------- */
#define USBC_UHINT_OFFSET           0x404        /**< \brief (USBC_UHINT offset) Host Global Interrupt Register */
#define USBC_UHINT_RESETVALUE       0x00000000   /**< \brief (USBC_UHINT reset_value) Host Global Interrupt Register */

#define USBC_UHINT_DCONNI_Pos       0            /**< \brief (USBC_UHINT) Device Connection Interrupt */
#define USBC_UHINT_DCONNI           (0x1u << USBC_UHINT_DCONNI_Pos)
#define USBC_UHINT_DDISCI_Pos       1            /**< \brief (USBC_UHINT) Device Disconnection Interrupt */
#define USBC_UHINT_DDISCI           (0x1u << USBC_UHINT_DDISCI_Pos)
#define USBC_UHINT_RSTI_Pos         2            /**< \brief (USBC_UHINT) USB Reset Sent Interrupt */
#define USBC_UHINT_RSTI             (0x1u << USBC_UHINT_RSTI_Pos)
#define USBC_UHINT_RSMEDI_Pos       3            /**< \brief (USBC_UHINT) Downstream Resume Sent Interrupt */
#define USBC_UHINT_RSMEDI           (0x1u << USBC_UHINT_RSMEDI_Pos)
#define USBC_UHINT_RXRSMI_Pos       4            /**< \brief (USBC_UHINT) Upstream Resume Received Interrupt */
#define USBC_UHINT_RXRSMI           (0x1u << USBC_UHINT_RXRSMI_Pos)
#define USBC_UHINT_HSOFI_Pos        5            /**< \brief (USBC_UHINT) Host SOF Interrupt */
#define USBC_UHINT_HSOFI            (0x1u << USBC_UHINT_HSOFI_Pos)
#define USBC_UHINT_HWUPI_Pos        6            /**< \brief (USBC_UHINT) Host Wake-Up Interrupt */
#define USBC_UHINT_HWUPI            (0x1u << USBC_UHINT_HWUPI_Pos)
#define USBC_UHINT_P0INT_Pos        8            /**< \brief (USBC_UHINT) Pipe 0 Interrupt */
#define USBC_UHINT_P0INT            (0x1u << USBC_UHINT_P0INT_Pos)
#define USBC_UHINT_P1INT_Pos        9            /**< \brief (USBC_UHINT) Pipe 1 Interrupt */
#define USBC_UHINT_P1INT            (0x1u << USBC_UHINT_P1INT_Pos)
#define USBC_UHINT_P2INT_Pos        10           /**< \brief (USBC_UHINT) Pipe 2 Interrupt */
#define USBC_UHINT_P2INT            (0x1u << USBC_UHINT_P2INT_Pos)
#define USBC_UHINT_P3INT_Pos        11           /**< \brief (USBC_UHINT) Pipe 3 Interrupt */
#define USBC_UHINT_P3INT            (0x1u << USBC_UHINT_P3INT_Pos)
#define USBC_UHINT_P4INT_Pos        12           /**< \brief (USBC_UHINT) Pipe 4 Interrupt */
#define USBC_UHINT_P4INT            (0x1u << USBC_UHINT_P4INT_Pos)
#define USBC_UHINT_P5INT_Pos        13           /**< \brief (USBC_UHINT) Pipe 5 Interrupt */
#define USBC_UHINT_P5INT            (0x1u << USBC_UHINT_P5INT_Pos)
#define USBC_UHINT_P6INT_Pos        14           /**< \brief (USBC_UHINT) Pipe 6 Interrupt */
#define USBC_UHINT_P6INT            (0x1u << USBC_UHINT_P6INT_Pos)
#define USBC_UHINT_MASK             0x00007F7Fu  /**< \brief (USBC_UHINT) MASK Register */

/* -------- USBC_UHINTCLR : (USBC Offset: 0x408) ( /W 32) Host Global Interrrupt Clear Register -------- */
#define USBC_UHINTCLR_OFFSET        0x408        /**< \brief (USBC_UHINTCLR offset) Host Global Interrrupt Clear Register */
#define USBC_UHINTCLR_RESETVALUE    0x00000000   /**< \brief (USBC_UHINTCLR reset_value) Host Global Interrrupt Clear Register */

#define USBC_UHINTCLR_DCONNIC_Pos   0            /**< \brief (USBC_UHINTCLR) DCONNI Clear */
#define USBC_UHINTCLR_DCONNIC       (0x1u << USBC_UHINTCLR_DCONNIC_Pos)
#define USBC_UHINTCLR_DDISCIC_Pos   1            /**< \brief (USBC_UHINTCLR) DDISCI Clear */
#define USBC_UHINTCLR_DDISCIC       (0x1u << USBC_UHINTCLR_DDISCIC_Pos)
#define USBC_UHINTCLR_RSTIC_Pos     2            /**< \brief (USBC_UHINTCLR) RSTI Clear */
#define USBC_UHINTCLR_RSTIC         (0x1u << USBC_UHINTCLR_RSTIC_Pos)
#define USBC_UHINTCLR_RSMEDIC_Pos   3            /**< \brief (USBC_UHINTCLR) RSMEDI Clear */
#define USBC_UHINTCLR_RSMEDIC       (0x1u << USBC_UHINTCLR_RSMEDIC_Pos)
#define USBC_UHINTCLR_RXRSMIC_Pos   4            /**< \brief (USBC_UHINTCLR) RXRSMI Clear */
#define USBC_UHINTCLR_RXRSMIC       (0x1u << USBC_UHINTCLR_RXRSMIC_Pos)
#define USBC_UHINTCLR_HSOFIC_Pos    5            /**< \brief (USBC_UHINTCLR) HSOFI Clear */
#define USBC_UHINTCLR_HSOFIC        (0x1u << USBC_UHINTCLR_HSOFIC_Pos)
#define USBC_UHINTCLR_HWUPIC_Pos    6            /**< \brief (USBC_UHINTCLR) HWUPI Clear */
#define USBC_UHINTCLR_HWUPIC        (0x1u << USBC_UHINTCLR_HWUPIC_Pos)
#define USBC_UHINTCLR_MASK          0x0000007Fu  /**< \brief (USBC_UHINTCLR) MASK Register */

/* -------- USBC_UHINTSET : (USBC Offset: 0x40C) ( /W 32) Host Global Interrupt Set Register -------- */
#define USBC_UHINTSET_OFFSET        0x40C        /**< \brief (USBC_UHINTSET offset) Host Global Interrupt Set Register */
#define USBC_UHINTSET_RESETVALUE    0x00000000   /**< \brief (USBC_UHINTSET reset_value) Host Global Interrupt Set Register */

#define USBC_UHINTSET_DCONNIS_Pos   0            /**< \brief (USBC_UHINTSET) DCONNI Set */
#define USBC_UHINTSET_DCONNIS       (0x1u << USBC_UHINTSET_DCONNIS_Pos)
#define USBC_UHINTSET_DDISCIS_Pos   1            /**< \brief (USBC_UHINTSET) DDISCI Set */
#define USBC_UHINTSET_DDISCIS       (0x1u << USBC_UHINTSET_DDISCIS_Pos)
#define USBC_UHINTSET_RSTIS_Pos     2            /**< \brief (USBC_UHINTSET) RSTI Set */
#define USBC_UHINTSET_RSTIS         (0x1u << USBC_UHINTSET_RSTIS_Pos)
#define USBC_UHINTSET_RSMEDIS_Pos   3            /**< \brief (USBC_UHINTSET) RSMEDI Set */
#define USBC_UHINTSET_RSMEDIS       (0x1u << USBC_UHINTSET_RSMEDIS_Pos)
#define USBC_UHINTSET_RXRSMIS_Pos   4            /**< \brief (USBC_UHINTSET) RXRSMI Set */
#define USBC_UHINTSET_RXRSMIS       (0x1u << USBC_UHINTSET_RXRSMIS_Pos)
#define USBC_UHINTSET_HSOFIS_Pos    5            /**< \brief (USBC_UHINTSET) HSOFI Set */
#define USBC_UHINTSET_HSOFIS        (0x1u << USBC_UHINTSET_HSOFIS_Pos)
#define USBC_UHINTSET_HWUPIS_Pos    6            /**< \brief (USBC_UHINTSET) HWUPI Set */
#define USBC_UHINTSET_HWUPIS        (0x1u << USBC_UHINTSET_HWUPIS_Pos)
#define USBC_UHINTSET_MASK          0x0000007Fu  /**< \brief (USBC_UHINTSET) MASK Register */

/* -------- USBC_UHINTE : (USBC Offset: 0x410) (R/  32) Host Global Interrupt Enable Register -------- */
#define USBC_UHINTE_OFFSET          0x410        /**< \brief (USBC_UHINTE offset) Host Global Interrupt Enable Register */
#define USBC_UHINTE_RESETVALUE      0x00000000   /**< \brief (USBC_UHINTE reset_value) Host Global Interrupt Enable Register */

#define USBC_UHINTE_DCONNIE_Pos     0            /**< \brief (USBC_UHINTE) DCONNI Enable */
#define USBC_UHINTE_DCONNIE         (0x1u << USBC_UHINTE_DCONNIE_Pos)
#define USBC_UHINTE_DDISCIE_Pos     1            /**< \brief (USBC_UHINTE) DDISCI Enable */
#define USBC_UHINTE_DDISCIE         (0x1u << USBC_UHINTE_DDISCIE_Pos)
#define USBC_UHINTE_RSTIE_Pos       2            /**< \brief (USBC_UHINTE) RSTI Enable */
#define USBC_UHINTE_RSTIE           (0x1u << USBC_UHINTE_RSTIE_Pos)
#define USBC_UHINTE_RSMEDIE_Pos     3            /**< \brief (USBC_UHINTE) RSMEDI Enable */
#define USBC_UHINTE_RSMEDIE         (0x1u << USBC_UHINTE_RSMEDIE_Pos)
#define USBC_UHINTE_RXRSMIE_Pos     4            /**< \brief (USBC_UHINTE) RXRSMI Enable */
#define USBC_UHINTE_RXRSMIE         (0x1u << USBC_UHINTE_RXRSMIE_Pos)
#define USBC_UHINTE_HSOFIE_Pos      5            /**< \brief (USBC_UHINTE) HSOFI Enable */
#define USBC_UHINTE_HSOFIE          (0x1u << USBC_UHINTE_HSOFIE_Pos)
#define USBC_UHINTE_HWUPIE_Pos      6            /**< \brief (USBC_UHINTE) HWUPI Enable */
#define USBC_UHINTE_HWUPIE          (0x1u << USBC_UHINTE_HWUPIE_Pos)
#define USBC_UHINTE_P0INTE_Pos      8            /**< \brief (USBC_UHINTE) P0INT Enable */
#define USBC_UHINTE_P0INTE          (0x1u << USBC_UHINTE_P0INTE_Pos)
#define USBC_UHINTE_P1INTE_Pos      9            /**< \brief (USBC_UHINTE) P1INT Enable */
#define USBC_UHINTE_P1INTE          (0x1u << USBC_UHINTE_P1INTE_Pos)
#define USBC_UHINTE_P2INTE_Pos      10           /**< \brief (USBC_UHINTE) P2INT Enable */
#define USBC_UHINTE_P2INTE          (0x1u << USBC_UHINTE_P2INTE_Pos)
#define USBC_UHINTE_P3INTE_Pos      11           /**< \brief (USBC_UHINTE) P3INT Enable */
#define USBC_UHINTE_P3INTE          (0x1u << USBC_UHINTE_P3INTE_Pos)
#define USBC_UHINTE_P4INTE_Pos      12           /**< \brief (USBC_UHINTE) P4INT Enable */
#define USBC_UHINTE_P4INTE          (0x1u << USBC_UHINTE_P4INTE_Pos)
#define USBC_UHINTE_P5INTE_Pos      13           /**< \brief (USBC_UHINTE) P5INT Enable */
#define USBC_UHINTE_P5INTE          (0x1u << USBC_UHINTE_P5INTE_Pos)
#define USBC_UHINTE_P6INTE_Pos      14           /**< \brief (USBC_UHINTE) P6INT Enable */
#define USBC_UHINTE_P6INTE          (0x1u << USBC_UHINTE_P6INTE_Pos)
#define USBC_UHINTE_P7INTE_Pos      15           /**< \brief (USBC_UHINTE) P7INT Enable */
#define USBC_UHINTE_P7INTE          (0x1u << USBC_UHINTE_P7INTE_Pos)
#define USBC_UHINTE_MASK            0x0000FF7Fu  /**< \brief (USBC_UHINTE) MASK Register */

/* -------- USBC_UHINTECLR : (USBC Offset: 0x414) ( /W 32) Host Global Interrupt Enable Clear Register -------- */
#define USBC_UHINTECLR_OFFSET       0x414        /**< \brief (USBC_UHINTECLR offset) Host Global Interrupt Enable Clear Register */
#define USBC_UHINTECLR_RESETVALUE   0x00000000   /**< \brief (USBC_UHINTECLR reset_value) Host Global Interrupt Enable Clear Register */

#define USBC_UHINTECLR_DCONNIEC_Pos 0            /**< \brief (USBC_UHINTECLR) DCONNIE Clear */
#define USBC_UHINTECLR_DCONNIEC     (0x1u << USBC_UHINTECLR_DCONNIEC_Pos)
#define USBC_UHINTECLR_DDISCIEC_Pos 1            /**< \brief (USBC_UHINTECLR) DDISCIE Clear */
#define USBC_UHINTECLR_DDISCIEC     (0x1u << USBC_UHINTECLR_DDISCIEC_Pos)
#define USBC_UHINTECLR_RSTIEC_Pos   2            /**< \brief (USBC_UHINTECLR) RSTIE Clear */
#define USBC_UHINTECLR_RSTIEC       (0x1u << USBC_UHINTECLR_RSTIEC_Pos)
#define USBC_UHINTECLR_RSMEDIEC_Pos 3            /**< \brief (USBC_UHINTECLR) RSMEDIE Clear */
#define USBC_UHINTECLR_RSMEDIEC     (0x1u << USBC_UHINTECLR_RSMEDIEC_Pos)
#define USBC_UHINTECLR_RXRSMIEC_Pos 4            /**< \brief (USBC_UHINTECLR) RXRSMIE Clear */
#define USBC_UHINTECLR_RXRSMIEC     (0x1u << USBC_UHINTECLR_RXRSMIEC_Pos)
#define USBC_UHINTECLR_HSOFIEC_Pos  5            /**< \brief (USBC_UHINTECLR) HSOFIE Clear */
#define USBC_UHINTECLR_HSOFIEC      (0x1u << USBC_UHINTECLR_HSOFIEC_Pos)
#define USBC_UHINTECLR_HWUPIEC_Pos  6            /**< \brief (USBC_UHINTECLR) HWUPIE Clear */
#define USBC_UHINTECLR_HWUPIEC      (0x1u << USBC_UHINTECLR_HWUPIEC_Pos)
#define USBC_UHINTECLR_P0INTEC_Pos  8            /**< \brief (USBC_UHINTECLR) P0INTE Clear */
#define USBC_UHINTECLR_P0INTEC      (0x1u << USBC_UHINTECLR_P0INTEC_Pos)
#define USBC_UHINTECLR_P1INTEC_Pos  9            /**< \brief (USBC_UHINTECLR) P1INTE Clear */
#define USBC_UHINTECLR_P1INTEC      (0x1u << USBC_UHINTECLR_P1INTEC_Pos)
#define USBC_UHINTECLR_P2INTEC_Pos  10           /**< \brief (USBC_UHINTECLR) P2INTE Clear */
#define USBC_UHINTECLR_P2INTEC      (0x1u << USBC_UHINTECLR_P2INTEC_Pos)
#define USBC_UHINTECLR_P3INTEC_Pos  11           /**< \brief (USBC_UHINTECLR) P3INTE Clear */
#define USBC_UHINTECLR_P3INTEC      (0x1u << USBC_UHINTECLR_P3INTEC_Pos)
#define USBC_UHINTECLR_P4INTEC_Pos  12           /**< \brief (USBC_UHINTECLR) P4INTE Clear */
#define USBC_UHINTECLR_P4INTEC      (0x1u << USBC_UHINTECLR_P4INTEC_Pos)
#define USBC_UHINTECLR_P5INTEC_Pos  13           /**< \brief (USBC_UHINTECLR) P5INTE Clear */
#define USBC_UHINTECLR_P5INTEC      (0x1u << USBC_UHINTECLR_P5INTEC_Pos)
#define USBC_UHINTECLR_P6INTEC_Pos  14           /**< \brief (USBC_UHINTECLR) P6INTE Clear */
#define USBC_UHINTECLR_P6INTEC      (0x1u << USBC_UHINTECLR_P6INTEC_Pos)
#define USBC_UHINTECLR_P7INTEC_Pos  15           /**< \brief (USBC_UHINTECLR) P7INTE Clear */
#define USBC_UHINTECLR_P7INTEC      (0x1u << USBC_UHINTECLR_P7INTEC_Pos)
#define USBC_UHINTECLR_MASK         0x0000FF7Fu  /**< \brief (USBC_UHINTECLR) MASK Register */

/* -------- USBC_UHINTESET : (USBC Offset: 0x418) ( /W 32) Host Global Interrupt Enable Set Register -------- */
#define USBC_UHINTESET_OFFSET       0x418        /**< \brief (USBC_UHINTESET offset) Host Global Interrupt Enable Set Register */
#define USBC_UHINTESET_RESETVALUE   0x00000000   /**< \brief (USBC_UHINTESET reset_value) Host Global Interrupt Enable Set Register */

#define USBC_UHINTESET_DCONNIES_Pos 0            /**< \brief (USBC_UHINTESET) DCONNIE Set */
#define USBC_UHINTESET_DCONNIES     (0x1u << USBC_UHINTESET_DCONNIES_Pos)
#define USBC_UHINTESET_DDISCIES_Pos 1            /**< \brief (USBC_UHINTESET) DDISCIE Set */
#define USBC_UHINTESET_DDISCIES     (0x1u << USBC_UHINTESET_DDISCIES_Pos)
#define USBC_UHINTESET_RSTIES_Pos   2            /**< \brief (USBC_UHINTESET) RSTIE Set */
#define USBC_UHINTESET_RSTIES       (0x1u << USBC_UHINTESET_RSTIES_Pos)
#define USBC_UHINTESET_RSMEDIES_Pos 3            /**< \brief (USBC_UHINTESET) RSMEDIE Set */
#define USBC_UHINTESET_RSMEDIES     (0x1u << USBC_UHINTESET_RSMEDIES_Pos)
#define USBC_UHINTESET_RXRSMIES_Pos 4            /**< \brief (USBC_UHINTESET) RXRSMIE Set */
#define USBC_UHINTESET_RXRSMIES     (0x1u << USBC_UHINTESET_RXRSMIES_Pos)
#define USBC_UHINTESET_HSOFIES_Pos  5            /**< \brief (USBC_UHINTESET) HSOFIE Set */
#define USBC_UHINTESET_HSOFIES      (0x1u << USBC_UHINTESET_HSOFIES_Pos)
#define USBC_UHINTESET_HWUPIES_Pos  6            /**< \brief (USBC_UHINTESET) HWUPIE Set */
#define USBC_UHINTESET_HWUPIES      (0x1u << USBC_UHINTESET_HWUPIES_Pos)
#define USBC_UHINTESET_P0INTES_Pos  8            /**< \brief (USBC_UHINTESET) P0INTE Set */
#define USBC_UHINTESET_P0INTES      (0x1u << USBC_UHINTESET_P0INTES_Pos)
#define USBC_UHINTESET_P1INTES_Pos  9            /**< \brief (USBC_UHINTESET) P1INTE Set */
#define USBC_UHINTESET_P1INTES      (0x1u << USBC_UHINTESET_P1INTES_Pos)
#define USBC_UHINTESET_P2INTES_Pos  10           /**< \brief (USBC_UHINTESET) P2INTE Set */
#define USBC_UHINTESET_P2INTES      (0x1u << USBC_UHINTESET_P2INTES_Pos)
#define USBC_UHINTESET_P3INTES_Pos  11           /**< \brief (USBC_UHINTESET) P3INTE Set */
#define USBC_UHINTESET_P3INTES      (0x1u << USBC_UHINTESET_P3INTES_Pos)
#define USBC_UHINTESET_P4INTES_Pos  12           /**< \brief (USBC_UHINTESET) P4INTE Set */
#define USBC_UHINTESET_P4INTES      (0x1u << USBC_UHINTESET_P4INTES_Pos)
#define USBC_UHINTESET_P5INTES_Pos  13           /**< \brief (USBC_UHINTESET) P5INTE Set */
#define USBC_UHINTESET_P5INTES      (0x1u << USBC_UHINTESET_P5INTES_Pos)
#define USBC_UHINTESET_P6INTES_Pos  14           /**< \brief (USBC_UHINTESET) P6INTE Set */
#define USBC_UHINTESET_P6INTES      (0x1u << USBC_UHINTESET_P6INTES_Pos)
#define USBC_UHINTESET_P7INTES_Pos  15           /**< \brief (USBC_UHINTESET) P7INTE Set */
#define USBC_UHINTESET_P7INTES      (0x1u << USBC_UHINTESET_P7INTES_Pos)
#define USBC_UHINTESET_MASK         0x0000FF7Fu  /**< \brief (USBC_UHINTESET) MASK Register */

/* -------- USBC_UPRST : (USBC Offset: 0x41C) (R/W 32) Pipe Reset Register -------- */
#define USBC_UPRST_OFFSET           0x41C        /**< \brief (USBC_UPRST offset) Pipe Reset Register */
#define USBC_UPRST_RESETVALUE       0x00000000   /**< \brief (USBC_UPRST reset_value) Pipe Reset Register */

#define USBC_UPRST_PEN0_Pos         0            /**< \brief (USBC_UPRST) Pipe0 Enable */
#define USBC_UPRST_PEN0             (0x1u << USBC_UPRST_PEN0_Pos)
#define USBC_UPRST_PEN1_Pos         1            /**< \brief (USBC_UPRST) Pipe1 Enable */
#define USBC_UPRST_PEN1             (0x1u << USBC_UPRST_PEN1_Pos)
#define USBC_UPRST_PEN2_Pos         2            /**< \brief (USBC_UPRST) Pipe2 Enable */
#define USBC_UPRST_PEN2             (0x1u << USBC_UPRST_PEN2_Pos)
#define USBC_UPRST_PEN3_Pos         3            /**< \brief (USBC_UPRST) Pipe3 Enable */
#define USBC_UPRST_PEN3             (0x1u << USBC_UPRST_PEN3_Pos)
#define USBC_UPRST_PEN4_Pos         4            /**< \brief (USBC_UPRST) Pipe4 Enable */
#define USBC_UPRST_PEN4             (0x1u << USBC_UPRST_PEN4_Pos)
#define USBC_UPRST_PEN5_Pos         5            /**< \brief (USBC_UPRST) Pipe5 Enable */
#define USBC_UPRST_PEN5             (0x1u << USBC_UPRST_PEN5_Pos)
#define USBC_UPRST_PEN6_Pos         6            /**< \brief (USBC_UPRST) Pipe6 Enable */
#define USBC_UPRST_PEN6             (0x1u << USBC_UPRST_PEN6_Pos)
#define USBC_UPRST_PEN7_Pos         7            /**< \brief (USBC_UPRST) Pipe7 Enable */
#define USBC_UPRST_PEN7             (0x1u << USBC_UPRST_PEN7_Pos)
#define USBC_UPRST_MASK             0x000000FFu  /**< \brief (USBC_UPRST) MASK Register */

/* -------- USBC_UHFNUM : (USBC Offset: 0x420) (R/W 32) Host Frame Number Register -------- */
#define USBC_UHFNUM_OFFSET          0x420        /**< \brief (USBC_UHFNUM offset) Host Frame Number Register */
#define USBC_UHFNUM_RESETVALUE      0x00000000   /**< \brief (USBC_UHFNUM reset_value) Host Frame Number Register */

#define USBC_UHFNUM_MFNUM_Pos       0            /**< \brief (USBC_UHFNUM) Micro Frame Number */
#define USBC_UHFNUM_MFNUM_Msk       (0x7u << USBC_UHFNUM_MFNUM_Pos)
#define USBC_UHFNUM_MFNUM(value)    ((USBC_UHFNUM_MFNUM_Msk & ((value) << USBC_UHFNUM_MFNUM_Pos)))
#define USBC_UHFNUM_FNUM_Pos        3            /**< \brief (USBC_UHFNUM) Frame Number */
#define USBC_UHFNUM_FNUM_Msk        (0x7FFu << USBC_UHFNUM_FNUM_Pos)
#define USBC_UHFNUM_FNUM(value)     ((USBC_UHFNUM_FNUM_Msk & ((value) << USBC_UHFNUM_FNUM_Pos)))
#define USBC_UHFNUM_FLENHIGH_Pos    16           /**< \brief (USBC_UHFNUM) Frame Length */
#define USBC_UHFNUM_FLENHIGH_Msk    (0xFFu << USBC_UHFNUM_FLENHIGH_Pos)
#define USBC_UHFNUM_FLENHIGH(value) ((USBC_UHFNUM_FLENHIGH_Msk & ((value) << USBC_UHFNUM_FLENHIGH_Pos)))
#define USBC_UHFNUM_MASK            0x00FF3FFFu  /**< \brief (USBC_UHFNUM) MASK Register */

/* -------- USBC_UHSOFC : (USBC Offset: 0x424) (R/W 32) Host Start of Frame Control Register -------- */
#define USBC_UHSOFC_OFFSET          0x424        /**< \brief (USBC_UHSOFC offset) Host Start of Frame Control Register */
#define USBC_UHSOFC_RESETVALUE      0x00000000   /**< \brief (USBC_UHSOFC reset_value) Host Start of Frame Control Register */

#define USBC_UHSOFC_FLENC_Pos       0            /**< \brief (USBC_UHSOFC) Frame Length Control */
#define USBC_UHSOFC_FLENC_Msk       (0x3FFFu << USBC_UHSOFC_FLENC_Pos)
#define USBC_UHSOFC_FLENC(value)    ((USBC_UHSOFC_FLENC_Msk & ((value) << USBC_UHSOFC_FLENC_Pos)))
#define USBC_UHSOFC_FLENCE_Pos      16           /**< \brief (USBC_UHSOFC) Frame Length Control Enable */
#define USBC_UHSOFC_FLENCE          (0x1u << USBC_UHSOFC_FLENCE_Pos)
#define USBC_UHSOFC_MASK            0x00013FFFu  /**< \brief (USBC_UHSOFC) MASK Register */

/* -------- USBC_UPCFG0 : (USBC Offset: 0x500) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG0_OFFSET          0x500        /**< \brief (USBC_UPCFG0 offset) Pipe Configuration Register */
#define USBC_UPCFG0_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG0 reset_value) Pipe Configuration Register */

#define USBC_UPCFG0_PBK_Pos         2            /**< \brief (USBC_UPCFG0) Pipe Banks */
#define USBC_UPCFG0_PBK             (0x1u << USBC_UPCFG0_PBK_Pos)
#define   USBC_UPCFG0_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG0)  */
#define USBC_UPCFG0_PSIZE_Pos       4            /**< \brief (USBC_UPCFG0) Pipe Size */
#define USBC_UPCFG0_PSIZE_Msk       (0x7u << USBC_UPCFG0_PSIZE_Pos)
#define USBC_UPCFG0_PSIZE(value)    ((USBC_UPCFG0_PSIZE_Msk & ((value) << USBC_UPCFG0_PSIZE_Pos)))
#define   USBC_UPCFG0_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG0)  */
#define USBC_UPCFG0_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG0) Pipe Token */
#define USBC_UPCFG0_PTOKEN_Msk      (0x3u << USBC_UPCFG0_PTOKEN_Pos)
#define USBC_UPCFG0_PTOKEN(value)   ((USBC_UPCFG0_PTOKEN_Msk & ((value) << USBC_UPCFG0_PTOKEN_Pos)))
#define   USBC_UPCFG0_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG0)  */
#define USBC_UPCFG0_PTYPE_Pos       12           /**< \brief (USBC_UPCFG0) Pipe Type */
#define USBC_UPCFG0_PTYPE_Msk       (0x3u << USBC_UPCFG0_PTYPE_Pos)
#define USBC_UPCFG0_PTYPE(value)    ((USBC_UPCFG0_PTYPE_Msk & ((value) << USBC_UPCFG0_PTYPE_Pos)))
#define   USBC_UPCFG0_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG0)  */
#define   USBC_UPCFG0_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG0)  */
#define USBC_UPCFG0_PINGEN_Pos      20           /**< \brief (USBC_UPCFG0) Ping Enable */
#define USBC_UPCFG0_PINGEN          (0x1u << USBC_UPCFG0_PINGEN_Pos)
#define USBC_UPCFG0_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG0) binterval parameter */
#define USBC_UPCFG0_BINTERVAL_Msk   (0xFFu << USBC_UPCFG0_BINTERVAL_Pos)
#define USBC_UPCFG0_BINTERVAL(value) ((USBC_UPCFG0_BINTERVAL_Msk & ((value) << USBC_UPCFG0_BINTERVAL_Pos)))
#define USBC_UPCFG0_MASK            0xFF103374u  /**< \brief (USBC_UPCFG0) MASK Register */

/* -------- USBC_UPCFG1 : (USBC Offset: 0x504) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG1_OFFSET          0x504        /**< \brief (USBC_UPCFG1 offset) Pipe Configuration Register */
#define USBC_UPCFG1_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG1 reset_value) Pipe Configuration Register */

#define USBC_UPCFG1_PBK_Pos         2            /**< \brief (USBC_UPCFG1) Pipe Banks */
#define USBC_UPCFG1_PBK             (0x1u << USBC_UPCFG1_PBK_Pos)
#define   USBC_UPCFG1_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG1)  */
#define USBC_UPCFG1_PSIZE_Pos       4            /**< \brief (USBC_UPCFG1) Pipe Size */
#define USBC_UPCFG1_PSIZE_Msk       (0x7u << USBC_UPCFG1_PSIZE_Pos)
#define USBC_UPCFG1_PSIZE(value)    ((USBC_UPCFG1_PSIZE_Msk & ((value) << USBC_UPCFG1_PSIZE_Pos)))
#define   USBC_UPCFG1_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG1)  */
#define USBC_UPCFG1_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG1) Pipe Token */
#define USBC_UPCFG1_PTOKEN_Msk      (0x3u << USBC_UPCFG1_PTOKEN_Pos)
#define USBC_UPCFG1_PTOKEN(value)   ((USBC_UPCFG1_PTOKEN_Msk & ((value) << USBC_UPCFG1_PTOKEN_Pos)))
#define   USBC_UPCFG1_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG1)  */
#define USBC_UPCFG1_PTYPE_Pos       12           /**< \brief (USBC_UPCFG1) Pipe Type */
#define USBC_UPCFG1_PTYPE_Msk       (0x3u << USBC_UPCFG1_PTYPE_Pos)
#define USBC_UPCFG1_PTYPE(value)    ((USBC_UPCFG1_PTYPE_Msk & ((value) << USBC_UPCFG1_PTYPE_Pos)))
#define   USBC_UPCFG1_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG1)  */
#define   USBC_UPCFG1_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG1)  */
#define USBC_UPCFG1_PINGEN_Pos      20           /**< \brief (USBC_UPCFG1) Ping Enable */
#define USBC_UPCFG1_PINGEN          (0x1u << USBC_UPCFG1_PINGEN_Pos)
#define USBC_UPCFG1_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG1) binterval parameter */
#define USBC_UPCFG1_BINTERVAL_Msk   (0xFFu << USBC_UPCFG1_BINTERVAL_Pos)
#define USBC_UPCFG1_BINTERVAL(value) ((USBC_UPCFG1_BINTERVAL_Msk & ((value) << USBC_UPCFG1_BINTERVAL_Pos)))
#define USBC_UPCFG1_MASK            0xFF103374u  /**< \brief (USBC_UPCFG1) MASK Register */

/* -------- USBC_UPCFG2 : (USBC Offset: 0x508) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG2_OFFSET          0x508        /**< \brief (USBC_UPCFG2 offset) Pipe Configuration Register */
#define USBC_UPCFG2_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG2 reset_value) Pipe Configuration Register */

#define USBC_UPCFG2_PBK_Pos         2            /**< \brief (USBC_UPCFG2) Pipe Banks */
#define USBC_UPCFG2_PBK             (0x1u << USBC_UPCFG2_PBK_Pos)
#define   USBC_UPCFG2_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG2)  */
#define USBC_UPCFG2_PSIZE_Pos       4            /**< \brief (USBC_UPCFG2) Pipe Size */
#define USBC_UPCFG2_PSIZE_Msk       (0x7u << USBC_UPCFG2_PSIZE_Pos)
#define USBC_UPCFG2_PSIZE(value)    ((USBC_UPCFG2_PSIZE_Msk & ((value) << USBC_UPCFG2_PSIZE_Pos)))
#define   USBC_UPCFG2_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG2)  */
#define USBC_UPCFG2_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG2) Pipe Token */
#define USBC_UPCFG2_PTOKEN_Msk      (0x3u << USBC_UPCFG2_PTOKEN_Pos)
#define USBC_UPCFG2_PTOKEN(value)   ((USBC_UPCFG2_PTOKEN_Msk & ((value) << USBC_UPCFG2_PTOKEN_Pos)))
#define   USBC_UPCFG2_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG2)  */
#define USBC_UPCFG2_PTYPE_Pos       12           /**< \brief (USBC_UPCFG2) Pipe Type */
#define USBC_UPCFG2_PTYPE_Msk       (0x3u << USBC_UPCFG2_PTYPE_Pos)
#define USBC_UPCFG2_PTYPE(value)    ((USBC_UPCFG2_PTYPE_Msk & ((value) << USBC_UPCFG2_PTYPE_Pos)))
#define   USBC_UPCFG2_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG2)  */
#define   USBC_UPCFG2_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG2)  */
#define USBC_UPCFG2_PINGEN_Pos      20           /**< \brief (USBC_UPCFG2) Ping Enable */
#define USBC_UPCFG2_PINGEN          (0x1u << USBC_UPCFG2_PINGEN_Pos)
#define USBC_UPCFG2_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG2) binterval parameter */
#define USBC_UPCFG2_BINTERVAL_Msk   (0xFFu << USBC_UPCFG2_BINTERVAL_Pos)
#define USBC_UPCFG2_BINTERVAL(value) ((USBC_UPCFG2_BINTERVAL_Msk & ((value) << USBC_UPCFG2_BINTERVAL_Pos)))
#define USBC_UPCFG2_MASK            0xFF103374u  /**< \brief (USBC_UPCFG2) MASK Register */

/* -------- USBC_UPCFG3 : (USBC Offset: 0x50C) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG3_OFFSET          0x50C        /**< \brief (USBC_UPCFG3 offset) Pipe Configuration Register */
#define USBC_UPCFG3_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG3 reset_value) Pipe Configuration Register */

#define USBC_UPCFG3_PBK_Pos         2            /**< \brief (USBC_UPCFG3) Pipe Banks */
#define USBC_UPCFG3_PBK             (0x1u << USBC_UPCFG3_PBK_Pos)
#define   USBC_UPCFG3_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG3)  */
#define USBC_UPCFG3_PSIZE_Pos       4            /**< \brief (USBC_UPCFG3) Pipe Size */
#define USBC_UPCFG3_PSIZE_Msk       (0x7u << USBC_UPCFG3_PSIZE_Pos)
#define USBC_UPCFG3_PSIZE(value)    ((USBC_UPCFG3_PSIZE_Msk & ((value) << USBC_UPCFG3_PSIZE_Pos)))
#define   USBC_UPCFG3_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG3)  */
#define USBC_UPCFG3_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG3) Pipe Token */
#define USBC_UPCFG3_PTOKEN_Msk      (0x3u << USBC_UPCFG3_PTOKEN_Pos)
#define USBC_UPCFG3_PTOKEN(value)   ((USBC_UPCFG3_PTOKEN_Msk & ((value) << USBC_UPCFG3_PTOKEN_Pos)))
#define   USBC_UPCFG3_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG3)  */
#define USBC_UPCFG3_PTYPE_Pos       12           /**< \brief (USBC_UPCFG3) Pipe Type */
#define USBC_UPCFG3_PTYPE_Msk       (0x3u << USBC_UPCFG3_PTYPE_Pos)
#define USBC_UPCFG3_PTYPE(value)    ((USBC_UPCFG3_PTYPE_Msk & ((value) << USBC_UPCFG3_PTYPE_Pos)))
#define   USBC_UPCFG3_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG3)  */
#define   USBC_UPCFG3_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG3)  */
#define USBC_UPCFG3_PINGEN_Pos      20           /**< \brief (USBC_UPCFG3) Ping Enable */
#define USBC_UPCFG3_PINGEN          (0x1u << USBC_UPCFG3_PINGEN_Pos)
#define USBC_UPCFG3_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG3) binterval parameter */
#define USBC_UPCFG3_BINTERVAL_Msk   (0xFFu << USBC_UPCFG3_BINTERVAL_Pos)
#define USBC_UPCFG3_BINTERVAL(value) ((USBC_UPCFG3_BINTERVAL_Msk & ((value) << USBC_UPCFG3_BINTERVAL_Pos)))
#define USBC_UPCFG3_MASK            0xFF103374u  /**< \brief (USBC_UPCFG3) MASK Register */

/* -------- USBC_UPCFG4 : (USBC Offset: 0x510) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG4_OFFSET          0x510        /**< \brief (USBC_UPCFG4 offset) Pipe Configuration Register */
#define USBC_UPCFG4_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG4 reset_value) Pipe Configuration Register */

#define USBC_UPCFG4_PBK_Pos         2            /**< \brief (USBC_UPCFG4) Pipe Banks */
#define USBC_UPCFG4_PBK             (0x1u << USBC_UPCFG4_PBK_Pos)
#define   USBC_UPCFG4_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG4)  */
#define USBC_UPCFG4_PSIZE_Pos       4            /**< \brief (USBC_UPCFG4) Pipe Size */
#define USBC_UPCFG4_PSIZE_Msk       (0x7u << USBC_UPCFG4_PSIZE_Pos)
#define USBC_UPCFG4_PSIZE(value)    ((USBC_UPCFG4_PSIZE_Msk & ((value) << USBC_UPCFG4_PSIZE_Pos)))
#define   USBC_UPCFG4_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG4)  */
#define USBC_UPCFG4_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG4) Pipe Token */
#define USBC_UPCFG4_PTOKEN_Msk      (0x3u << USBC_UPCFG4_PTOKEN_Pos)
#define USBC_UPCFG4_PTOKEN(value)   ((USBC_UPCFG4_PTOKEN_Msk & ((value) << USBC_UPCFG4_PTOKEN_Pos)))
#define   USBC_UPCFG4_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG4)  */
#define USBC_UPCFG4_PTYPE_Pos       12           /**< \brief (USBC_UPCFG4) Pipe Type */
#define USBC_UPCFG4_PTYPE_Msk       (0x3u << USBC_UPCFG4_PTYPE_Pos)
#define USBC_UPCFG4_PTYPE(value)    ((USBC_UPCFG4_PTYPE_Msk & ((value) << USBC_UPCFG4_PTYPE_Pos)))
#define   USBC_UPCFG4_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG4)  */
#define   USBC_UPCFG4_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG4)  */
#define USBC_UPCFG4_PINGEN_Pos      20           /**< \brief (USBC_UPCFG4) Ping Enable */
#define USBC_UPCFG4_PINGEN          (0x1u << USBC_UPCFG4_PINGEN_Pos)
#define USBC_UPCFG4_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG4) binterval parameter */
#define USBC_UPCFG4_BINTERVAL_Msk   (0xFFu << USBC_UPCFG4_BINTERVAL_Pos)
#define USBC_UPCFG4_BINTERVAL(value) ((USBC_UPCFG4_BINTERVAL_Msk & ((value) << USBC_UPCFG4_BINTERVAL_Pos)))
#define USBC_UPCFG4_MASK            0xFF103374u  /**< \brief (USBC_UPCFG4) MASK Register */

/* -------- USBC_UPCFG5 : (USBC Offset: 0x514) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG5_OFFSET          0x514        /**< \brief (USBC_UPCFG5 offset) Pipe Configuration Register */
#define USBC_UPCFG5_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG5 reset_value) Pipe Configuration Register */

#define USBC_UPCFG5_PBK_Pos         2            /**< \brief (USBC_UPCFG5) Pipe Banks */
#define USBC_UPCFG5_PBK             (0x1u << USBC_UPCFG5_PBK_Pos)
#define   USBC_UPCFG5_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG5)  */
#define USBC_UPCFG5_PSIZE_Pos       4            /**< \brief (USBC_UPCFG5) Pipe Size */
#define USBC_UPCFG5_PSIZE_Msk       (0x7u << USBC_UPCFG5_PSIZE_Pos)
#define USBC_UPCFG5_PSIZE(value)    ((USBC_UPCFG5_PSIZE_Msk & ((value) << USBC_UPCFG5_PSIZE_Pos)))
#define   USBC_UPCFG5_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG5)  */
#define USBC_UPCFG5_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG5) Pipe Token */
#define USBC_UPCFG5_PTOKEN_Msk      (0x3u << USBC_UPCFG5_PTOKEN_Pos)
#define USBC_UPCFG5_PTOKEN(value)   ((USBC_UPCFG5_PTOKEN_Msk & ((value) << USBC_UPCFG5_PTOKEN_Pos)))
#define   USBC_UPCFG5_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG5)  */
#define USBC_UPCFG5_PTYPE_Pos       12           /**< \brief (USBC_UPCFG5) Pipe Type */
#define USBC_UPCFG5_PTYPE_Msk       (0x3u << USBC_UPCFG5_PTYPE_Pos)
#define USBC_UPCFG5_PTYPE(value)    ((USBC_UPCFG5_PTYPE_Msk & ((value) << USBC_UPCFG5_PTYPE_Pos)))
#define   USBC_UPCFG5_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG5)  */
#define   USBC_UPCFG5_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG5)  */
#define USBC_UPCFG5_PINGEN_Pos      20           /**< \brief (USBC_UPCFG5) Ping Enable */
#define USBC_UPCFG5_PINGEN          (0x1u << USBC_UPCFG5_PINGEN_Pos)
#define USBC_UPCFG5_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG5) binterval parameter */
#define USBC_UPCFG5_BINTERVAL_Msk   (0xFFu << USBC_UPCFG5_BINTERVAL_Pos)
#define USBC_UPCFG5_BINTERVAL(value) ((USBC_UPCFG5_BINTERVAL_Msk & ((value) << USBC_UPCFG5_BINTERVAL_Pos)))
#define USBC_UPCFG5_MASK            0xFF103374u  /**< \brief (USBC_UPCFG5) MASK Register */

/* -------- USBC_UPCFG6 : (USBC Offset: 0x518) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG6_OFFSET          0x518        /**< \brief (USBC_UPCFG6 offset) Pipe Configuration Register */
#define USBC_UPCFG6_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG6 reset_value) Pipe Configuration Register */

#define USBC_UPCFG6_PBK_Pos         2            /**< \brief (USBC_UPCFG6) Pipe Banks */
#define USBC_UPCFG6_PBK             (0x1u << USBC_UPCFG6_PBK_Pos)
#define   USBC_UPCFG6_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG6)  */
#define USBC_UPCFG6_PSIZE_Pos       4            /**< \brief (USBC_UPCFG6) Pipe Size */
#define USBC_UPCFG6_PSIZE_Msk       (0x7u << USBC_UPCFG6_PSIZE_Pos)
#define USBC_UPCFG6_PSIZE(value)    ((USBC_UPCFG6_PSIZE_Msk & ((value) << USBC_UPCFG6_PSIZE_Pos)))
#define   USBC_UPCFG6_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG6)  */
#define USBC_UPCFG6_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG6) Pipe Token */
#define USBC_UPCFG6_PTOKEN_Msk      (0x3u << USBC_UPCFG6_PTOKEN_Pos)
#define USBC_UPCFG6_PTOKEN(value)   ((USBC_UPCFG6_PTOKEN_Msk & ((value) << USBC_UPCFG6_PTOKEN_Pos)))
#define   USBC_UPCFG6_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG6)  */
#define USBC_UPCFG6_PTYPE_Pos       12           /**< \brief (USBC_UPCFG6) Pipe Type */
#define USBC_UPCFG6_PTYPE_Msk       (0x3u << USBC_UPCFG6_PTYPE_Pos)
#define USBC_UPCFG6_PTYPE(value)    ((USBC_UPCFG6_PTYPE_Msk & ((value) << USBC_UPCFG6_PTYPE_Pos)))
#define   USBC_UPCFG6_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG6)  */
#define   USBC_UPCFG6_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG6)  */
#define USBC_UPCFG6_PINGEN_Pos      20           /**< \brief (USBC_UPCFG6) Ping Enable */
#define USBC_UPCFG6_PINGEN          (0x1u << USBC_UPCFG6_PINGEN_Pos)
#define USBC_UPCFG6_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG6) binterval parameter */
#define USBC_UPCFG6_BINTERVAL_Msk   (0xFFu << USBC_UPCFG6_BINTERVAL_Pos)
#define USBC_UPCFG6_BINTERVAL(value) ((USBC_UPCFG6_BINTERVAL_Msk & ((value) << USBC_UPCFG6_BINTERVAL_Pos)))
#define USBC_UPCFG6_MASK            0xFF103374u  /**< \brief (USBC_UPCFG6) MASK Register */

/* -------- USBC_UPCFG7 : (USBC Offset: 0x51C) (R/W 32) Pipe Configuration Register -------- */
#define USBC_UPCFG7_OFFSET          0x51C        /**< \brief (USBC_UPCFG7 offset) Pipe Configuration Register */
#define USBC_UPCFG7_RESETVALUE      0x00000000   /**< \brief (USBC_UPCFG7 reset_value) Pipe Configuration Register */

#define USBC_UPCFG7_PBK_Pos         2            /**< \brief (USBC_UPCFG7) Pipe Banks */
#define USBC_UPCFG7_PBK             (0x1u << USBC_UPCFG7_PBK_Pos)
#define   USBC_UPCFG7_PBK_SINGLE    (0x0u <<  2) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PBK_DOUBLE    (0x1u <<  2) /**< \brief (USBC_UPCFG7)  */
#define USBC_UPCFG7_PSIZE_Pos       4            /**< \brief (USBC_UPCFG7) Pipe Size */
#define USBC_UPCFG7_PSIZE_Msk       (0x7u << USBC_UPCFG7_PSIZE_Pos)
#define USBC_UPCFG7_PSIZE(value)    ((USBC_UPCFG7_PSIZE_Msk & ((value) << USBC_UPCFG7_PSIZE_Pos)))
#define   USBC_UPCFG7_PSIZE_8       (0x0u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_16      (0x1u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_32      (0x2u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_64      (0x3u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_128     (0x4u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_256     (0x5u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_512     (0x6u <<  4) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PSIZE_1024    (0x7u <<  4) /**< \brief (USBC_UPCFG7)  */
#define USBC_UPCFG7_PTOKEN_Pos      8            /**< \brief (USBC_UPCFG7) Pipe Token */
#define USBC_UPCFG7_PTOKEN_Msk      (0x3u << USBC_UPCFG7_PTOKEN_Pos)
#define USBC_UPCFG7_PTOKEN(value)   ((USBC_UPCFG7_PTOKEN_Msk & ((value) << USBC_UPCFG7_PTOKEN_Pos)))
#define   USBC_UPCFG7_PTOKEN_SETUP  (0x0u <<  8) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PTOKEN_IN     (0x1u <<  8) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PTOKEN_OUT    (0x2u <<  8) /**< \brief (USBC_UPCFG7)  */
#define USBC_UPCFG7_PTYPE_Pos       12           /**< \brief (USBC_UPCFG7) Pipe Type */
#define USBC_UPCFG7_PTYPE_Msk       (0x3u << USBC_UPCFG7_PTYPE_Pos)
#define USBC_UPCFG7_PTYPE(value)    ((USBC_UPCFG7_PTYPE_Msk & ((value) << USBC_UPCFG7_PTYPE_Pos)))
#define   USBC_UPCFG7_PTYPE_CONTROL (0x0u << 12) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PTYPE_ISOCHRONOUS (0x1u << 12) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PTYPE_BULK    (0x2u << 12) /**< \brief (USBC_UPCFG7)  */
#define   USBC_UPCFG7_PTYPE_INTERRUPT (0x3u << 12) /**< \brief (USBC_UPCFG7)  */
#define USBC_UPCFG7_PINGEN_Pos      20           /**< \brief (USBC_UPCFG7) Ping Enable */
#define USBC_UPCFG7_PINGEN          (0x1u << USBC_UPCFG7_PINGEN_Pos)
#define USBC_UPCFG7_BINTERVAL_Pos   24           /**< \brief (USBC_UPCFG7) binterval parameter */
#define USBC_UPCFG7_BINTERVAL_Msk   (0xFFu << USBC_UPCFG7_BINTERVAL_Pos)
#define USBC_UPCFG7_BINTERVAL(value) ((USBC_UPCFG7_BINTERVAL_Msk & ((value) << USBC_UPCFG7_BINTERVAL_Pos)))
#define USBC_UPCFG7_MASK            0xFF103374u  /**< \brief (USBC_UPCFG7) MASK Register */

/* -------- USBC_UPSTA0 : (USBC Offset: 0x530) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA0_OFFSET          0x530        /**< \brief (USBC_UPSTA0 offset) Pipe Status Register */
#define USBC_UPSTA0_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA0 reset_value) Pipe Status Register */

#define USBC_UPSTA0_RXINI_Pos       0            /**< \brief (USBC_UPSTA0) Received IN Data Interrupt */
#define USBC_UPSTA0_RXINI           (0x1u << USBC_UPSTA0_RXINI_Pos)
#define USBC_UPSTA0_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA0) Transmitted OUT Data Interrupt */
#define USBC_UPSTA0_TXOUTI          (0x1u << USBC_UPSTA0_TXOUTI_Pos)
#define USBC_UPSTA0_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA0) Transmitted SETUP Interrupt */
#define USBC_UPSTA0_TXSTPI          (0x1u << USBC_UPSTA0_TXSTPI_Pos)
#define USBC_UPSTA0_PERRI_Pos       3            /**< \brief (USBC_UPSTA0) Pipe Error Interrupt */
#define USBC_UPSTA0_PERRI           (0x1u << USBC_UPSTA0_PERRI_Pos)
#define USBC_UPSTA0_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA0) NAKed Interrupt */
#define USBC_UPSTA0_NAKEDI          (0x1u << USBC_UPSTA0_NAKEDI_Pos)
#define USBC_UPSTA0_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA0) Errorflow Interrupt */
#define USBC_UPSTA0_ERRORFI         (0x1u << USBC_UPSTA0_ERRORFI_Pos)
#define USBC_UPSTA0_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA0) Received STALLed Interrupt */
#define USBC_UPSTA0_RXSTALLDI       (0x1u << USBC_UPSTA0_RXSTALLDI_Pos)
#define USBC_UPSTA0_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA0) Data Toggle Sequence */
#define USBC_UPSTA0_DTSEQ_Msk       (0x3u << USBC_UPSTA0_DTSEQ_Pos)
#define USBC_UPSTA0_DTSEQ(value)    ((USBC_UPSTA0_DTSEQ_Msk & ((value) << USBC_UPSTA0_DTSEQ_Pos)))
#define USBC_UPSTA0_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA0) Ram Access Error Interrupt */
#define USBC_UPSTA0_RAMACERI        (0x1u << USBC_UPSTA0_RAMACERI_Pos)
#define USBC_UPSTA0_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA0) Number of Busy Bank */
#define USBC_UPSTA0_NBUSYBK_Msk     (0x3u << USBC_UPSTA0_NBUSYBK_Pos)
#define USBC_UPSTA0_NBUSYBK(value)  ((USBC_UPSTA0_NBUSYBK_Msk & ((value) << USBC_UPSTA0_NBUSYBK_Pos)))
#define USBC_UPSTA0_CURRBK_Pos      14           /**< \brief (USBC_UPSTA0) Current Bank */
#define USBC_UPSTA0_CURRBK_Msk      (0x3u << USBC_UPSTA0_CURRBK_Pos)
#define USBC_UPSTA0_CURRBK(value)   ((USBC_UPSTA0_CURRBK_Msk & ((value) << USBC_UPSTA0_CURRBK_Pos)))
#define USBC_UPSTA0_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA0) MASK Register */

/* -------- USBC_UPSTA1 : (USBC Offset: 0x534) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA1_OFFSET          0x534        /**< \brief (USBC_UPSTA1 offset) Pipe Status Register */
#define USBC_UPSTA1_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA1 reset_value) Pipe Status Register */

#define USBC_UPSTA1_RXINI_Pos       0            /**< \brief (USBC_UPSTA1) Received IN Data Interrupt */
#define USBC_UPSTA1_RXINI           (0x1u << USBC_UPSTA1_RXINI_Pos)
#define USBC_UPSTA1_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA1) Transmitted OUT Data Interrupt */
#define USBC_UPSTA1_TXOUTI          (0x1u << USBC_UPSTA1_TXOUTI_Pos)
#define USBC_UPSTA1_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA1) Transmitted SETUP Interrupt */
#define USBC_UPSTA1_TXSTPI          (0x1u << USBC_UPSTA1_TXSTPI_Pos)
#define USBC_UPSTA1_PERRI_Pos       3            /**< \brief (USBC_UPSTA1) Pipe Error Interrupt */
#define USBC_UPSTA1_PERRI           (0x1u << USBC_UPSTA1_PERRI_Pos)
#define USBC_UPSTA1_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA1) NAKed Interrupt */
#define USBC_UPSTA1_NAKEDI          (0x1u << USBC_UPSTA1_NAKEDI_Pos)
#define USBC_UPSTA1_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA1) Errorflow Interrupt */
#define USBC_UPSTA1_ERRORFI         (0x1u << USBC_UPSTA1_ERRORFI_Pos)
#define USBC_UPSTA1_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA1) Received STALLed Interrupt */
#define USBC_UPSTA1_RXSTALLDI       (0x1u << USBC_UPSTA1_RXSTALLDI_Pos)
#define USBC_UPSTA1_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA1) Data Toggle Sequence */
#define USBC_UPSTA1_DTSEQ_Msk       (0x3u << USBC_UPSTA1_DTSEQ_Pos)
#define USBC_UPSTA1_DTSEQ(value)    ((USBC_UPSTA1_DTSEQ_Msk & ((value) << USBC_UPSTA1_DTSEQ_Pos)))
#define USBC_UPSTA1_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA1) Ram Access Error Interrupt */
#define USBC_UPSTA1_RAMACERI        (0x1u << USBC_UPSTA1_RAMACERI_Pos)
#define USBC_UPSTA1_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA1) Number of Busy Bank */
#define USBC_UPSTA1_NBUSYBK_Msk     (0x3u << USBC_UPSTA1_NBUSYBK_Pos)
#define USBC_UPSTA1_NBUSYBK(value)  ((USBC_UPSTA1_NBUSYBK_Msk & ((value) << USBC_UPSTA1_NBUSYBK_Pos)))
#define USBC_UPSTA1_CURRBK_Pos      14           /**< \brief (USBC_UPSTA1) Current Bank */
#define USBC_UPSTA1_CURRBK_Msk      (0x3u << USBC_UPSTA1_CURRBK_Pos)
#define USBC_UPSTA1_CURRBK(value)   ((USBC_UPSTA1_CURRBK_Msk & ((value) << USBC_UPSTA1_CURRBK_Pos)))
#define USBC_UPSTA1_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA1) MASK Register */

/* -------- USBC_UPSTA2 : (USBC Offset: 0x538) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA2_OFFSET          0x538        /**< \brief (USBC_UPSTA2 offset) Pipe Status Register */
#define USBC_UPSTA2_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA2 reset_value) Pipe Status Register */

#define USBC_UPSTA2_RXINI_Pos       0            /**< \brief (USBC_UPSTA2) Received IN Data Interrupt */
#define USBC_UPSTA2_RXINI           (0x1u << USBC_UPSTA2_RXINI_Pos)
#define USBC_UPSTA2_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA2) Transmitted OUT Data Interrupt */
#define USBC_UPSTA2_TXOUTI          (0x1u << USBC_UPSTA2_TXOUTI_Pos)
#define USBC_UPSTA2_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA2) Transmitted SETUP Interrupt */
#define USBC_UPSTA2_TXSTPI          (0x1u << USBC_UPSTA2_TXSTPI_Pos)
#define USBC_UPSTA2_PERRI_Pos       3            /**< \brief (USBC_UPSTA2) Pipe Error Interrupt */
#define USBC_UPSTA2_PERRI           (0x1u << USBC_UPSTA2_PERRI_Pos)
#define USBC_UPSTA2_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA2) NAKed Interrupt */
#define USBC_UPSTA2_NAKEDI          (0x1u << USBC_UPSTA2_NAKEDI_Pos)
#define USBC_UPSTA2_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA2) Errorflow Interrupt */
#define USBC_UPSTA2_ERRORFI         (0x1u << USBC_UPSTA2_ERRORFI_Pos)
#define USBC_UPSTA2_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA2) Received STALLed Interrupt */
#define USBC_UPSTA2_RXSTALLDI       (0x1u << USBC_UPSTA2_RXSTALLDI_Pos)
#define USBC_UPSTA2_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA2) Data Toggle Sequence */
#define USBC_UPSTA2_DTSEQ_Msk       (0x3u << USBC_UPSTA2_DTSEQ_Pos)
#define USBC_UPSTA2_DTSEQ(value)    ((USBC_UPSTA2_DTSEQ_Msk & ((value) << USBC_UPSTA2_DTSEQ_Pos)))
#define USBC_UPSTA2_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA2) Ram Access Error Interrupt */
#define USBC_UPSTA2_RAMACERI        (0x1u << USBC_UPSTA2_RAMACERI_Pos)
#define USBC_UPSTA2_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA2) Number of Busy Bank */
#define USBC_UPSTA2_NBUSYBK_Msk     (0x3u << USBC_UPSTA2_NBUSYBK_Pos)
#define USBC_UPSTA2_NBUSYBK(value)  ((USBC_UPSTA2_NBUSYBK_Msk & ((value) << USBC_UPSTA2_NBUSYBK_Pos)))
#define USBC_UPSTA2_CURRBK_Pos      14           /**< \brief (USBC_UPSTA2) Current Bank */
#define USBC_UPSTA2_CURRBK_Msk      (0x3u << USBC_UPSTA2_CURRBK_Pos)
#define USBC_UPSTA2_CURRBK(value)   ((USBC_UPSTA2_CURRBK_Msk & ((value) << USBC_UPSTA2_CURRBK_Pos)))
#define USBC_UPSTA2_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA2) MASK Register */

/* -------- USBC_UPSTA3 : (USBC Offset: 0x53C) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA3_OFFSET          0x53C        /**< \brief (USBC_UPSTA3 offset) Pipe Status Register */
#define USBC_UPSTA3_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA3 reset_value) Pipe Status Register */

#define USBC_UPSTA3_RXINI_Pos       0            /**< \brief (USBC_UPSTA3) Received IN Data Interrupt */
#define USBC_UPSTA3_RXINI           (0x1u << USBC_UPSTA3_RXINI_Pos)
#define USBC_UPSTA3_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA3) Transmitted OUT Data Interrupt */
#define USBC_UPSTA3_TXOUTI          (0x1u << USBC_UPSTA3_TXOUTI_Pos)
#define USBC_UPSTA3_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA3) Transmitted SETUP Interrupt */
#define USBC_UPSTA3_TXSTPI          (0x1u << USBC_UPSTA3_TXSTPI_Pos)
#define USBC_UPSTA3_PERRI_Pos       3            /**< \brief (USBC_UPSTA3) Pipe Error Interrupt */
#define USBC_UPSTA3_PERRI           (0x1u << USBC_UPSTA3_PERRI_Pos)
#define USBC_UPSTA3_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA3) NAKed Interrupt */
#define USBC_UPSTA3_NAKEDI          (0x1u << USBC_UPSTA3_NAKEDI_Pos)
#define USBC_UPSTA3_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA3) Errorflow Interrupt */
#define USBC_UPSTA3_ERRORFI         (0x1u << USBC_UPSTA3_ERRORFI_Pos)
#define USBC_UPSTA3_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA3) Received STALLed Interrupt */
#define USBC_UPSTA3_RXSTALLDI       (0x1u << USBC_UPSTA3_RXSTALLDI_Pos)
#define USBC_UPSTA3_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA3) Data Toggle Sequence */
#define USBC_UPSTA3_DTSEQ_Msk       (0x3u << USBC_UPSTA3_DTSEQ_Pos)
#define USBC_UPSTA3_DTSEQ(value)    ((USBC_UPSTA3_DTSEQ_Msk & ((value) << USBC_UPSTA3_DTSEQ_Pos)))
#define USBC_UPSTA3_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA3) Ram Access Error Interrupt */
#define USBC_UPSTA3_RAMACERI        (0x1u << USBC_UPSTA3_RAMACERI_Pos)
#define USBC_UPSTA3_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA3) Number of Busy Bank */
#define USBC_UPSTA3_NBUSYBK_Msk     (0x3u << USBC_UPSTA3_NBUSYBK_Pos)
#define USBC_UPSTA3_NBUSYBK(value)  ((USBC_UPSTA3_NBUSYBK_Msk & ((value) << USBC_UPSTA3_NBUSYBK_Pos)))
#define USBC_UPSTA3_CURRBK_Pos      14           /**< \brief (USBC_UPSTA3) Current Bank */
#define USBC_UPSTA3_CURRBK_Msk      (0x3u << USBC_UPSTA3_CURRBK_Pos)
#define USBC_UPSTA3_CURRBK(value)   ((USBC_UPSTA3_CURRBK_Msk & ((value) << USBC_UPSTA3_CURRBK_Pos)))
#define USBC_UPSTA3_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA3) MASK Register */

/* -------- USBC_UPSTA4 : (USBC Offset: 0x540) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA4_OFFSET          0x540        /**< \brief (USBC_UPSTA4 offset) Pipe Status Register */
#define USBC_UPSTA4_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA4 reset_value) Pipe Status Register */

#define USBC_UPSTA4_RXINI_Pos       0            /**< \brief (USBC_UPSTA4) Received IN Data Interrupt */
#define USBC_UPSTA4_RXINI           (0x1u << USBC_UPSTA4_RXINI_Pos)
#define USBC_UPSTA4_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA4) Transmitted OUT Data Interrupt */
#define USBC_UPSTA4_TXOUTI          (0x1u << USBC_UPSTA4_TXOUTI_Pos)
#define USBC_UPSTA4_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA4) Transmitted SETUP Interrupt */
#define USBC_UPSTA4_TXSTPI          (0x1u << USBC_UPSTA4_TXSTPI_Pos)
#define USBC_UPSTA4_PERRI_Pos       3            /**< \brief (USBC_UPSTA4) Pipe Error Interrupt */
#define USBC_UPSTA4_PERRI           (0x1u << USBC_UPSTA4_PERRI_Pos)
#define USBC_UPSTA4_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA4) NAKed Interrupt */
#define USBC_UPSTA4_NAKEDI          (0x1u << USBC_UPSTA4_NAKEDI_Pos)
#define USBC_UPSTA4_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA4) Errorflow Interrupt */
#define USBC_UPSTA4_ERRORFI         (0x1u << USBC_UPSTA4_ERRORFI_Pos)
#define USBC_UPSTA4_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA4) Received STALLed Interrupt */
#define USBC_UPSTA4_RXSTALLDI       (0x1u << USBC_UPSTA4_RXSTALLDI_Pos)
#define USBC_UPSTA4_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA4) Data Toggle Sequence */
#define USBC_UPSTA4_DTSEQ_Msk       (0x3u << USBC_UPSTA4_DTSEQ_Pos)
#define USBC_UPSTA4_DTSEQ(value)    ((USBC_UPSTA4_DTSEQ_Msk & ((value) << USBC_UPSTA4_DTSEQ_Pos)))
#define USBC_UPSTA4_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA4) Ram Access Error Interrupt */
#define USBC_UPSTA4_RAMACERI        (0x1u << USBC_UPSTA4_RAMACERI_Pos)
#define USBC_UPSTA4_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA4) Number of Busy Bank */
#define USBC_UPSTA4_NBUSYBK_Msk     (0x3u << USBC_UPSTA4_NBUSYBK_Pos)
#define USBC_UPSTA4_NBUSYBK(value)  ((USBC_UPSTA4_NBUSYBK_Msk & ((value) << USBC_UPSTA4_NBUSYBK_Pos)))
#define USBC_UPSTA4_CURRBK_Pos      14           /**< \brief (USBC_UPSTA4) Current Bank */
#define USBC_UPSTA4_CURRBK_Msk      (0x3u << USBC_UPSTA4_CURRBK_Pos)
#define USBC_UPSTA4_CURRBK(value)   ((USBC_UPSTA4_CURRBK_Msk & ((value) << USBC_UPSTA4_CURRBK_Pos)))
#define USBC_UPSTA4_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA4) MASK Register */

/* -------- USBC_UPSTA5 : (USBC Offset: 0x544) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA5_OFFSET          0x544        /**< \brief (USBC_UPSTA5 offset) Pipe Status Register */
#define USBC_UPSTA5_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA5 reset_value) Pipe Status Register */

#define USBC_UPSTA5_RXINI_Pos       0            /**< \brief (USBC_UPSTA5) Received IN Data Interrupt */
#define USBC_UPSTA5_RXINI           (0x1u << USBC_UPSTA5_RXINI_Pos)
#define USBC_UPSTA5_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA5) Transmitted OUT Data Interrupt */
#define USBC_UPSTA5_TXOUTI          (0x1u << USBC_UPSTA5_TXOUTI_Pos)
#define USBC_UPSTA5_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA5) Transmitted SETUP Interrupt */
#define USBC_UPSTA5_TXSTPI          (0x1u << USBC_UPSTA5_TXSTPI_Pos)
#define USBC_UPSTA5_PERRI_Pos       3            /**< \brief (USBC_UPSTA5) Pipe Error Interrupt */
#define USBC_UPSTA5_PERRI           (0x1u << USBC_UPSTA5_PERRI_Pos)
#define USBC_UPSTA5_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA5) NAKed Interrupt */
#define USBC_UPSTA5_NAKEDI          (0x1u << USBC_UPSTA5_NAKEDI_Pos)
#define USBC_UPSTA5_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA5) Errorflow Interrupt */
#define USBC_UPSTA5_ERRORFI         (0x1u << USBC_UPSTA5_ERRORFI_Pos)
#define USBC_UPSTA5_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA5) Received STALLed Interrupt */
#define USBC_UPSTA5_RXSTALLDI       (0x1u << USBC_UPSTA5_RXSTALLDI_Pos)
#define USBC_UPSTA5_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA5) Data Toggle Sequence */
#define USBC_UPSTA5_DTSEQ_Msk       (0x3u << USBC_UPSTA5_DTSEQ_Pos)
#define USBC_UPSTA5_DTSEQ(value)    ((USBC_UPSTA5_DTSEQ_Msk & ((value) << USBC_UPSTA5_DTSEQ_Pos)))
#define USBC_UPSTA5_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA5) Ram Access Error Interrupt */
#define USBC_UPSTA5_RAMACERI        (0x1u << USBC_UPSTA5_RAMACERI_Pos)
#define USBC_UPSTA5_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA5) Number of Busy Bank */
#define USBC_UPSTA5_NBUSYBK_Msk     (0x3u << USBC_UPSTA5_NBUSYBK_Pos)
#define USBC_UPSTA5_NBUSYBK(value)  ((USBC_UPSTA5_NBUSYBK_Msk & ((value) << USBC_UPSTA5_NBUSYBK_Pos)))
#define USBC_UPSTA5_CURRBK_Pos      14           /**< \brief (USBC_UPSTA5) Current Bank */
#define USBC_UPSTA5_CURRBK_Msk      (0x3u << USBC_UPSTA5_CURRBK_Pos)
#define USBC_UPSTA5_CURRBK(value)   ((USBC_UPSTA5_CURRBK_Msk & ((value) << USBC_UPSTA5_CURRBK_Pos)))
#define USBC_UPSTA5_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA5) MASK Register */

/* -------- USBC_UPSTA6 : (USBC Offset: 0x548) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA6_OFFSET          0x548        /**< \brief (USBC_UPSTA6 offset) Pipe Status Register */
#define USBC_UPSTA6_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA6 reset_value) Pipe Status Register */

#define USBC_UPSTA6_RXINI_Pos       0            /**< \brief (USBC_UPSTA6) Received IN Data Interrupt */
#define USBC_UPSTA6_RXINI           (0x1u << USBC_UPSTA6_RXINI_Pos)
#define USBC_UPSTA6_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA6) Transmitted OUT Data Interrupt */
#define USBC_UPSTA6_TXOUTI          (0x1u << USBC_UPSTA6_TXOUTI_Pos)
#define USBC_UPSTA6_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA6) Transmitted SETUP Interrupt */
#define USBC_UPSTA6_TXSTPI          (0x1u << USBC_UPSTA6_TXSTPI_Pos)
#define USBC_UPSTA6_PERRI_Pos       3            /**< \brief (USBC_UPSTA6) Pipe Error Interrupt */
#define USBC_UPSTA6_PERRI           (0x1u << USBC_UPSTA6_PERRI_Pos)
#define USBC_UPSTA6_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA6) NAKed Interrupt */
#define USBC_UPSTA6_NAKEDI          (0x1u << USBC_UPSTA6_NAKEDI_Pos)
#define USBC_UPSTA6_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA6) Errorflow Interrupt */
#define USBC_UPSTA6_ERRORFI         (0x1u << USBC_UPSTA6_ERRORFI_Pos)
#define USBC_UPSTA6_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA6) Received STALLed Interrupt */
#define USBC_UPSTA6_RXSTALLDI       (0x1u << USBC_UPSTA6_RXSTALLDI_Pos)
#define USBC_UPSTA6_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA6) Data Toggle Sequence */
#define USBC_UPSTA6_DTSEQ_Msk       (0x3u << USBC_UPSTA6_DTSEQ_Pos)
#define USBC_UPSTA6_DTSEQ(value)    ((USBC_UPSTA6_DTSEQ_Msk & ((value) << USBC_UPSTA6_DTSEQ_Pos)))
#define USBC_UPSTA6_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA6) Ram Access Error Interrupt */
#define USBC_UPSTA6_RAMACERI        (0x1u << USBC_UPSTA6_RAMACERI_Pos)
#define USBC_UPSTA6_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA6) Number of Busy Bank */
#define USBC_UPSTA6_NBUSYBK_Msk     (0x3u << USBC_UPSTA6_NBUSYBK_Pos)
#define USBC_UPSTA6_NBUSYBK(value)  ((USBC_UPSTA6_NBUSYBK_Msk & ((value) << USBC_UPSTA6_NBUSYBK_Pos)))
#define USBC_UPSTA6_CURRBK_Pos      14           /**< \brief (USBC_UPSTA6) Current Bank */
#define USBC_UPSTA6_CURRBK_Msk      (0x3u << USBC_UPSTA6_CURRBK_Pos)
#define USBC_UPSTA6_CURRBK(value)   ((USBC_UPSTA6_CURRBK_Msk & ((value) << USBC_UPSTA6_CURRBK_Pos)))
#define USBC_UPSTA6_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA6) MASK Register */

/* -------- USBC_UPSTA7 : (USBC Offset: 0x54C) (R/  32) Pipe Status Register -------- */
#define USBC_UPSTA7_OFFSET          0x54C        /**< \brief (USBC_UPSTA7 offset) Pipe Status Register */
#define USBC_UPSTA7_RESETVALUE      0x00000000   /**< \brief (USBC_UPSTA7 reset_value) Pipe Status Register */

#define USBC_UPSTA7_RXINI_Pos       0            /**< \brief (USBC_UPSTA7) Received IN Data Interrupt */
#define USBC_UPSTA7_RXINI           (0x1u << USBC_UPSTA7_RXINI_Pos)
#define USBC_UPSTA7_TXOUTI_Pos      1            /**< \brief (USBC_UPSTA7) Transmitted OUT Data Interrupt */
#define USBC_UPSTA7_TXOUTI          (0x1u << USBC_UPSTA7_TXOUTI_Pos)
#define USBC_UPSTA7_TXSTPI_Pos      2            /**< \brief (USBC_UPSTA7) Transmitted SETUP Interrupt */
#define USBC_UPSTA7_TXSTPI          (0x1u << USBC_UPSTA7_TXSTPI_Pos)
#define USBC_UPSTA7_PERRI_Pos       3            /**< \brief (USBC_UPSTA7) Pipe Error Interrupt */
#define USBC_UPSTA7_PERRI           (0x1u << USBC_UPSTA7_PERRI_Pos)
#define USBC_UPSTA7_NAKEDI_Pos      4            /**< \brief (USBC_UPSTA7) NAKed Interrupt */
#define USBC_UPSTA7_NAKEDI          (0x1u << USBC_UPSTA7_NAKEDI_Pos)
#define USBC_UPSTA7_ERRORFI_Pos     5            /**< \brief (USBC_UPSTA7) Errorflow Interrupt */
#define USBC_UPSTA7_ERRORFI         (0x1u << USBC_UPSTA7_ERRORFI_Pos)
#define USBC_UPSTA7_RXSTALLDI_Pos   6            /**< \brief (USBC_UPSTA7) Received STALLed Interrupt */
#define USBC_UPSTA7_RXSTALLDI       (0x1u << USBC_UPSTA7_RXSTALLDI_Pos)
#define USBC_UPSTA7_DTSEQ_Pos       8            /**< \brief (USBC_UPSTA7) Data Toggle Sequence */
#define USBC_UPSTA7_DTSEQ_Msk       (0x3u << USBC_UPSTA7_DTSEQ_Pos)
#define USBC_UPSTA7_DTSEQ(value)    ((USBC_UPSTA7_DTSEQ_Msk & ((value) << USBC_UPSTA7_DTSEQ_Pos)))
#define USBC_UPSTA7_RAMACERI_Pos    10           /**< \brief (USBC_UPSTA7) Ram Access Error Interrupt */
#define USBC_UPSTA7_RAMACERI        (0x1u << USBC_UPSTA7_RAMACERI_Pos)
#define USBC_UPSTA7_NBUSYBK_Pos     12           /**< \brief (USBC_UPSTA7) Number of Busy Bank */
#define USBC_UPSTA7_NBUSYBK_Msk     (0x3u << USBC_UPSTA7_NBUSYBK_Pos)
#define USBC_UPSTA7_NBUSYBK(value)  ((USBC_UPSTA7_NBUSYBK_Msk & ((value) << USBC_UPSTA7_NBUSYBK_Pos)))
#define USBC_UPSTA7_CURRBK_Pos      14           /**< \brief (USBC_UPSTA7) Current Bank */
#define USBC_UPSTA7_CURRBK_Msk      (0x3u << USBC_UPSTA7_CURRBK_Pos)
#define USBC_UPSTA7_CURRBK(value)   ((USBC_UPSTA7_CURRBK_Msk & ((value) << USBC_UPSTA7_CURRBK_Pos)))
#define USBC_UPSTA7_MASK            0x0000F77Fu  /**< \brief (USBC_UPSTA7) MASK Register */

/* -------- USBC_UPSTA0CLR : (USBC Offset: 0x560) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA0CLR_OFFSET       0x560        /**< \brief (USBC_UPSTA0CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA0CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA0CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA0CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA0CLR) RXINI Clear */
#define USBC_UPSTA0CLR_RXINIC       (0x1u << USBC_UPSTA0CLR_RXINIC_Pos)
#define USBC_UPSTA0CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA0CLR) TXOUTI Clear */
#define USBC_UPSTA0CLR_TXOUTIC      (0x1u << USBC_UPSTA0CLR_TXOUTIC_Pos)
#define USBC_UPSTA0CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA0CLR) TXSTPI Clear */
#define USBC_UPSTA0CLR_TXSTPIC      (0x1u << USBC_UPSTA0CLR_TXSTPIC_Pos)
#define USBC_UPSTA0CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA0CLR) PERRI Clear */
#define USBC_UPSTA0CLR_PERRIC       (0x1u << USBC_UPSTA0CLR_PERRIC_Pos)
#define USBC_UPSTA0CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA0CLR) NAKEDI Clear */
#define USBC_UPSTA0CLR_NAKEDIC      (0x1u << USBC_UPSTA0CLR_NAKEDIC_Pos)
#define USBC_UPSTA0CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA0CLR) ERRORFI Clear */
#define USBC_UPSTA0CLR_ERRORFIC     (0x1u << USBC_UPSTA0CLR_ERRORFIC_Pos)
#define USBC_UPSTA0CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA0CLR) RXSTALLDI Clear */
#define USBC_UPSTA0CLR_RXSTALLDIC   (0x1u << USBC_UPSTA0CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA0CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA0CLR) RAMACERI Clear */
#define USBC_UPSTA0CLR_RAMACERIC    (0x1u << USBC_UPSTA0CLR_RAMACERIC_Pos)
#define USBC_UPSTA0CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA0CLR) MASK Register */

/* -------- USBC_UPSTA1CLR : (USBC Offset: 0x564) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA1CLR_OFFSET       0x564        /**< \brief (USBC_UPSTA1CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA1CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA1CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA1CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA1CLR) RXINI Clear */
#define USBC_UPSTA1CLR_RXINIC       (0x1u << USBC_UPSTA1CLR_RXINIC_Pos)
#define USBC_UPSTA1CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA1CLR) TXOUTI Clear */
#define USBC_UPSTA1CLR_TXOUTIC      (0x1u << USBC_UPSTA1CLR_TXOUTIC_Pos)
#define USBC_UPSTA1CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA1CLR) TXSTPI Clear */
#define USBC_UPSTA1CLR_TXSTPIC      (0x1u << USBC_UPSTA1CLR_TXSTPIC_Pos)
#define USBC_UPSTA1CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA1CLR) PERRI Clear */
#define USBC_UPSTA1CLR_PERRIC       (0x1u << USBC_UPSTA1CLR_PERRIC_Pos)
#define USBC_UPSTA1CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA1CLR) NAKEDI Clear */
#define USBC_UPSTA1CLR_NAKEDIC      (0x1u << USBC_UPSTA1CLR_NAKEDIC_Pos)
#define USBC_UPSTA1CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA1CLR) ERRORFI Clear */
#define USBC_UPSTA1CLR_ERRORFIC     (0x1u << USBC_UPSTA1CLR_ERRORFIC_Pos)
#define USBC_UPSTA1CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA1CLR) RXSTALLDI Clear */
#define USBC_UPSTA1CLR_RXSTALLDIC   (0x1u << USBC_UPSTA1CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA1CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA1CLR) RAMACERI Clear */
#define USBC_UPSTA1CLR_RAMACERIC    (0x1u << USBC_UPSTA1CLR_RAMACERIC_Pos)
#define USBC_UPSTA1CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA1CLR) MASK Register */

/* -------- USBC_UPSTA2CLR : (USBC Offset: 0x568) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA2CLR_OFFSET       0x568        /**< \brief (USBC_UPSTA2CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA2CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA2CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA2CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA2CLR) RXINI Clear */
#define USBC_UPSTA2CLR_RXINIC       (0x1u << USBC_UPSTA2CLR_RXINIC_Pos)
#define USBC_UPSTA2CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA2CLR) TXOUTI Clear */
#define USBC_UPSTA2CLR_TXOUTIC      (0x1u << USBC_UPSTA2CLR_TXOUTIC_Pos)
#define USBC_UPSTA2CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA2CLR) TXSTPI Clear */
#define USBC_UPSTA2CLR_TXSTPIC      (0x1u << USBC_UPSTA2CLR_TXSTPIC_Pos)
#define USBC_UPSTA2CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA2CLR) PERRI Clear */
#define USBC_UPSTA2CLR_PERRIC       (0x1u << USBC_UPSTA2CLR_PERRIC_Pos)
#define USBC_UPSTA2CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA2CLR) NAKEDI Clear */
#define USBC_UPSTA2CLR_NAKEDIC      (0x1u << USBC_UPSTA2CLR_NAKEDIC_Pos)
#define USBC_UPSTA2CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA2CLR) ERRORFI Clear */
#define USBC_UPSTA2CLR_ERRORFIC     (0x1u << USBC_UPSTA2CLR_ERRORFIC_Pos)
#define USBC_UPSTA2CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA2CLR) RXSTALLDI Clear */
#define USBC_UPSTA2CLR_RXSTALLDIC   (0x1u << USBC_UPSTA2CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA2CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA2CLR) RAMACERI Clear */
#define USBC_UPSTA2CLR_RAMACERIC    (0x1u << USBC_UPSTA2CLR_RAMACERIC_Pos)
#define USBC_UPSTA2CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA2CLR) MASK Register */

/* -------- USBC_UPSTA3CLR : (USBC Offset: 0x56C) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA3CLR_OFFSET       0x56C        /**< \brief (USBC_UPSTA3CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA3CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA3CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA3CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA3CLR) RXINI Clear */
#define USBC_UPSTA3CLR_RXINIC       (0x1u << USBC_UPSTA3CLR_RXINIC_Pos)
#define USBC_UPSTA3CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA3CLR) TXOUTI Clear */
#define USBC_UPSTA3CLR_TXOUTIC      (0x1u << USBC_UPSTA3CLR_TXOUTIC_Pos)
#define USBC_UPSTA3CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA3CLR) TXSTPI Clear */
#define USBC_UPSTA3CLR_TXSTPIC      (0x1u << USBC_UPSTA3CLR_TXSTPIC_Pos)
#define USBC_UPSTA3CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA3CLR) PERRI Clear */
#define USBC_UPSTA3CLR_PERRIC       (0x1u << USBC_UPSTA3CLR_PERRIC_Pos)
#define USBC_UPSTA3CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA3CLR) NAKEDI Clear */
#define USBC_UPSTA3CLR_NAKEDIC      (0x1u << USBC_UPSTA3CLR_NAKEDIC_Pos)
#define USBC_UPSTA3CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA3CLR) ERRORFI Clear */
#define USBC_UPSTA3CLR_ERRORFIC     (0x1u << USBC_UPSTA3CLR_ERRORFIC_Pos)
#define USBC_UPSTA3CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA3CLR) RXSTALLDI Clear */
#define USBC_UPSTA3CLR_RXSTALLDIC   (0x1u << USBC_UPSTA3CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA3CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA3CLR) RAMACERI Clear */
#define USBC_UPSTA3CLR_RAMACERIC    (0x1u << USBC_UPSTA3CLR_RAMACERIC_Pos)
#define USBC_UPSTA3CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA3CLR) MASK Register */

/* -------- USBC_UPSTA4CLR : (USBC Offset: 0x570) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA4CLR_OFFSET       0x570        /**< \brief (USBC_UPSTA4CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA4CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA4CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA4CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA4CLR) RXINI Clear */
#define USBC_UPSTA4CLR_RXINIC       (0x1u << USBC_UPSTA4CLR_RXINIC_Pos)
#define USBC_UPSTA4CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA4CLR) TXOUTI Clear */
#define USBC_UPSTA4CLR_TXOUTIC      (0x1u << USBC_UPSTA4CLR_TXOUTIC_Pos)
#define USBC_UPSTA4CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA4CLR) TXSTPI Clear */
#define USBC_UPSTA4CLR_TXSTPIC      (0x1u << USBC_UPSTA4CLR_TXSTPIC_Pos)
#define USBC_UPSTA4CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA4CLR) PERRI Clear */
#define USBC_UPSTA4CLR_PERRIC       (0x1u << USBC_UPSTA4CLR_PERRIC_Pos)
#define USBC_UPSTA4CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA4CLR) NAKEDI Clear */
#define USBC_UPSTA4CLR_NAKEDIC      (0x1u << USBC_UPSTA4CLR_NAKEDIC_Pos)
#define USBC_UPSTA4CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA4CLR) ERRORFI Clear */
#define USBC_UPSTA4CLR_ERRORFIC     (0x1u << USBC_UPSTA4CLR_ERRORFIC_Pos)
#define USBC_UPSTA4CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA4CLR) RXSTALLDI Clear */
#define USBC_UPSTA4CLR_RXSTALLDIC   (0x1u << USBC_UPSTA4CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA4CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA4CLR) RAMACERI Clear */
#define USBC_UPSTA4CLR_RAMACERIC    (0x1u << USBC_UPSTA4CLR_RAMACERIC_Pos)
#define USBC_UPSTA4CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA4CLR) MASK Register */

/* -------- USBC_UPSTA5CLR : (USBC Offset: 0x574) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA5CLR_OFFSET       0x574        /**< \brief (USBC_UPSTA5CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA5CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA5CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA5CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA5CLR) RXINI Clear */
#define USBC_UPSTA5CLR_RXINIC       (0x1u << USBC_UPSTA5CLR_RXINIC_Pos)
#define USBC_UPSTA5CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA5CLR) TXOUTI Clear */
#define USBC_UPSTA5CLR_TXOUTIC      (0x1u << USBC_UPSTA5CLR_TXOUTIC_Pos)
#define USBC_UPSTA5CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA5CLR) TXSTPI Clear */
#define USBC_UPSTA5CLR_TXSTPIC      (0x1u << USBC_UPSTA5CLR_TXSTPIC_Pos)
#define USBC_UPSTA5CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA5CLR) PERRI Clear */
#define USBC_UPSTA5CLR_PERRIC       (0x1u << USBC_UPSTA5CLR_PERRIC_Pos)
#define USBC_UPSTA5CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA5CLR) NAKEDI Clear */
#define USBC_UPSTA5CLR_NAKEDIC      (0x1u << USBC_UPSTA5CLR_NAKEDIC_Pos)
#define USBC_UPSTA5CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA5CLR) ERRORFI Clear */
#define USBC_UPSTA5CLR_ERRORFIC     (0x1u << USBC_UPSTA5CLR_ERRORFIC_Pos)
#define USBC_UPSTA5CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA5CLR) RXSTALLDI Clear */
#define USBC_UPSTA5CLR_RXSTALLDIC   (0x1u << USBC_UPSTA5CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA5CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA5CLR) RAMACERI Clear */
#define USBC_UPSTA5CLR_RAMACERIC    (0x1u << USBC_UPSTA5CLR_RAMACERIC_Pos)
#define USBC_UPSTA5CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA5CLR) MASK Register */

/* -------- USBC_UPSTA6CLR : (USBC Offset: 0x578) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA6CLR_OFFSET       0x578        /**< \brief (USBC_UPSTA6CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA6CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA6CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA6CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA6CLR) RXINI Clear */
#define USBC_UPSTA6CLR_RXINIC       (0x1u << USBC_UPSTA6CLR_RXINIC_Pos)
#define USBC_UPSTA6CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA6CLR) TXOUTI Clear */
#define USBC_UPSTA6CLR_TXOUTIC      (0x1u << USBC_UPSTA6CLR_TXOUTIC_Pos)
#define USBC_UPSTA6CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA6CLR) TXSTPI Clear */
#define USBC_UPSTA6CLR_TXSTPIC      (0x1u << USBC_UPSTA6CLR_TXSTPIC_Pos)
#define USBC_UPSTA6CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA6CLR) PERRI Clear */
#define USBC_UPSTA6CLR_PERRIC       (0x1u << USBC_UPSTA6CLR_PERRIC_Pos)
#define USBC_UPSTA6CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA6CLR) NAKEDI Clear */
#define USBC_UPSTA6CLR_NAKEDIC      (0x1u << USBC_UPSTA6CLR_NAKEDIC_Pos)
#define USBC_UPSTA6CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA6CLR) ERRORFI Clear */
#define USBC_UPSTA6CLR_ERRORFIC     (0x1u << USBC_UPSTA6CLR_ERRORFIC_Pos)
#define USBC_UPSTA6CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA6CLR) RXSTALLDI Clear */
#define USBC_UPSTA6CLR_RXSTALLDIC   (0x1u << USBC_UPSTA6CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA6CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA6CLR) RAMACERI Clear */
#define USBC_UPSTA6CLR_RAMACERIC    (0x1u << USBC_UPSTA6CLR_RAMACERIC_Pos)
#define USBC_UPSTA6CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA6CLR) MASK Register */

/* -------- USBC_UPSTA7CLR : (USBC Offset: 0x57C) ( /W 32) Pipe Status Clear Register -------- */
#define USBC_UPSTA7CLR_OFFSET       0x57C        /**< \brief (USBC_UPSTA7CLR offset) Pipe Status Clear Register */
#define USBC_UPSTA7CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA7CLR reset_value) Pipe Status Clear Register */

#define USBC_UPSTA7CLR_RXINIC_Pos   0            /**< \brief (USBC_UPSTA7CLR) RXINI Clear */
#define USBC_UPSTA7CLR_RXINIC       (0x1u << USBC_UPSTA7CLR_RXINIC_Pos)
#define USBC_UPSTA7CLR_TXOUTIC_Pos  1            /**< \brief (USBC_UPSTA7CLR) TXOUTI Clear */
#define USBC_UPSTA7CLR_TXOUTIC      (0x1u << USBC_UPSTA7CLR_TXOUTIC_Pos)
#define USBC_UPSTA7CLR_TXSTPIC_Pos  2            /**< \brief (USBC_UPSTA7CLR) TXSTPI Clear */
#define USBC_UPSTA7CLR_TXSTPIC      (0x1u << USBC_UPSTA7CLR_TXSTPIC_Pos)
#define USBC_UPSTA7CLR_PERRIC_Pos   3            /**< \brief (USBC_UPSTA7CLR) PERRI Clear */
#define USBC_UPSTA7CLR_PERRIC       (0x1u << USBC_UPSTA7CLR_PERRIC_Pos)
#define USBC_UPSTA7CLR_NAKEDIC_Pos  4            /**< \brief (USBC_UPSTA7CLR) NAKEDI Clear */
#define USBC_UPSTA7CLR_NAKEDIC      (0x1u << USBC_UPSTA7CLR_NAKEDIC_Pos)
#define USBC_UPSTA7CLR_ERRORFIC_Pos 5            /**< \brief (USBC_UPSTA7CLR) ERRORFI Clear */
#define USBC_UPSTA7CLR_ERRORFIC     (0x1u << USBC_UPSTA7CLR_ERRORFIC_Pos)
#define USBC_UPSTA7CLR_RXSTALLDIC_Pos 6            /**< \brief (USBC_UPSTA7CLR) RXSTALLDI Clear */
#define USBC_UPSTA7CLR_RXSTALLDIC   (0x1u << USBC_UPSTA7CLR_RXSTALLDIC_Pos)
#define USBC_UPSTA7CLR_RAMACERIC_Pos 10           /**< \brief (USBC_UPSTA7CLR) RAMACERI Clear */
#define USBC_UPSTA7CLR_RAMACERIC    (0x1u << USBC_UPSTA7CLR_RAMACERIC_Pos)
#define USBC_UPSTA7CLR_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA7CLR) MASK Register */

/* -------- USBC_UPSTA0SET : (USBC Offset: 0x590) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA0SET_OFFSET       0x590        /**< \brief (USBC_UPSTA0SET offset) Pipe Status Set Register */
#define USBC_UPSTA0SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA0SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA0SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA0SET) RXINI Set */
#define USBC_UPSTA0SET_RXINIS       (0x1u << USBC_UPSTA0SET_RXINIS_Pos)
#define USBC_UPSTA0SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA0SET) TXOUTI Set */
#define USBC_UPSTA0SET_TXOUTIS      (0x1u << USBC_UPSTA0SET_TXOUTIS_Pos)
#define USBC_UPSTA0SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA0SET) TXSTPI Set */
#define USBC_UPSTA0SET_TXSTPIS      (0x1u << USBC_UPSTA0SET_TXSTPIS_Pos)
#define USBC_UPSTA0SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA0SET) PERRI Set */
#define USBC_UPSTA0SET_PERRIS       (0x1u << USBC_UPSTA0SET_PERRIS_Pos)
#define USBC_UPSTA0SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA0SET) NAKEDI Set */
#define USBC_UPSTA0SET_NAKEDIS      (0x1u << USBC_UPSTA0SET_NAKEDIS_Pos)
#define USBC_UPSTA0SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA0SET) ERRORFI Set */
#define USBC_UPSTA0SET_ERRORFIS     (0x1u << USBC_UPSTA0SET_ERRORFIS_Pos)
#define USBC_UPSTA0SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA0SET) RXSTALLDI Set */
#define USBC_UPSTA0SET_RXSTALLDIS   (0x1u << USBC_UPSTA0SET_RXSTALLDIS_Pos)
#define USBC_UPSTA0SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA0SET) RAMACERI Set */
#define USBC_UPSTA0SET_RAMACERIS    (0x1u << USBC_UPSTA0SET_RAMACERIS_Pos)
#define USBC_UPSTA0SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA0SET) MASK Register */

/* -------- USBC_UPSTA1SET : (USBC Offset: 0x594) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA1SET_OFFSET       0x594        /**< \brief (USBC_UPSTA1SET offset) Pipe Status Set Register */
#define USBC_UPSTA1SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA1SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA1SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA1SET) RXINI Set */
#define USBC_UPSTA1SET_RXINIS       (0x1u << USBC_UPSTA1SET_RXINIS_Pos)
#define USBC_UPSTA1SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA1SET) TXOUTI Set */
#define USBC_UPSTA1SET_TXOUTIS      (0x1u << USBC_UPSTA1SET_TXOUTIS_Pos)
#define USBC_UPSTA1SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA1SET) TXSTPI Set */
#define USBC_UPSTA1SET_TXSTPIS      (0x1u << USBC_UPSTA1SET_TXSTPIS_Pos)
#define USBC_UPSTA1SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA1SET) PERRI Set */
#define USBC_UPSTA1SET_PERRIS       (0x1u << USBC_UPSTA1SET_PERRIS_Pos)
#define USBC_UPSTA1SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA1SET) NAKEDI Set */
#define USBC_UPSTA1SET_NAKEDIS      (0x1u << USBC_UPSTA1SET_NAKEDIS_Pos)
#define USBC_UPSTA1SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA1SET) ERRORFI Set */
#define USBC_UPSTA1SET_ERRORFIS     (0x1u << USBC_UPSTA1SET_ERRORFIS_Pos)
#define USBC_UPSTA1SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA1SET) RXSTALLDI Set */
#define USBC_UPSTA1SET_RXSTALLDIS   (0x1u << USBC_UPSTA1SET_RXSTALLDIS_Pos)
#define USBC_UPSTA1SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA1SET) RAMACERI Set */
#define USBC_UPSTA1SET_RAMACERIS    (0x1u << USBC_UPSTA1SET_RAMACERIS_Pos)
#define USBC_UPSTA1SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA1SET) MASK Register */

/* -------- USBC_UPSTA2SET : (USBC Offset: 0x598) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA2SET_OFFSET       0x598        /**< \brief (USBC_UPSTA2SET offset) Pipe Status Set Register */
#define USBC_UPSTA2SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA2SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA2SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA2SET) RXINI Set */
#define USBC_UPSTA2SET_RXINIS       (0x1u << USBC_UPSTA2SET_RXINIS_Pos)
#define USBC_UPSTA2SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA2SET) TXOUTI Set */
#define USBC_UPSTA2SET_TXOUTIS      (0x1u << USBC_UPSTA2SET_TXOUTIS_Pos)
#define USBC_UPSTA2SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA2SET) TXSTPI Set */
#define USBC_UPSTA2SET_TXSTPIS      (0x1u << USBC_UPSTA2SET_TXSTPIS_Pos)
#define USBC_UPSTA2SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA2SET) PERRI Set */
#define USBC_UPSTA2SET_PERRIS       (0x1u << USBC_UPSTA2SET_PERRIS_Pos)
#define USBC_UPSTA2SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA2SET) NAKEDI Set */
#define USBC_UPSTA2SET_NAKEDIS      (0x1u << USBC_UPSTA2SET_NAKEDIS_Pos)
#define USBC_UPSTA2SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA2SET) ERRORFI Set */
#define USBC_UPSTA2SET_ERRORFIS     (0x1u << USBC_UPSTA2SET_ERRORFIS_Pos)
#define USBC_UPSTA2SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA2SET) RXSTALLDI Set */
#define USBC_UPSTA2SET_RXSTALLDIS   (0x1u << USBC_UPSTA2SET_RXSTALLDIS_Pos)
#define USBC_UPSTA2SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA2SET) RAMACERI Set */
#define USBC_UPSTA2SET_RAMACERIS    (0x1u << USBC_UPSTA2SET_RAMACERIS_Pos)
#define USBC_UPSTA2SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA2SET) MASK Register */

/* -------- USBC_UPSTA3SET : (USBC Offset: 0x59C) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA3SET_OFFSET       0x59C        /**< \brief (USBC_UPSTA3SET offset) Pipe Status Set Register */
#define USBC_UPSTA3SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA3SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA3SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA3SET) RXINI Set */
#define USBC_UPSTA3SET_RXINIS       (0x1u << USBC_UPSTA3SET_RXINIS_Pos)
#define USBC_UPSTA3SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA3SET) TXOUTI Set */
#define USBC_UPSTA3SET_TXOUTIS      (0x1u << USBC_UPSTA3SET_TXOUTIS_Pos)
#define USBC_UPSTA3SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA3SET) TXSTPI Set */
#define USBC_UPSTA3SET_TXSTPIS      (0x1u << USBC_UPSTA3SET_TXSTPIS_Pos)
#define USBC_UPSTA3SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA3SET) PERRI Set */
#define USBC_UPSTA3SET_PERRIS       (0x1u << USBC_UPSTA3SET_PERRIS_Pos)
#define USBC_UPSTA3SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA3SET) NAKEDI Set */
#define USBC_UPSTA3SET_NAKEDIS      (0x1u << USBC_UPSTA3SET_NAKEDIS_Pos)
#define USBC_UPSTA3SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA3SET) ERRORFI Set */
#define USBC_UPSTA3SET_ERRORFIS     (0x1u << USBC_UPSTA3SET_ERRORFIS_Pos)
#define USBC_UPSTA3SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA3SET) RXSTALLDI Set */
#define USBC_UPSTA3SET_RXSTALLDIS   (0x1u << USBC_UPSTA3SET_RXSTALLDIS_Pos)
#define USBC_UPSTA3SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA3SET) RAMACERI Set */
#define USBC_UPSTA3SET_RAMACERIS    (0x1u << USBC_UPSTA3SET_RAMACERIS_Pos)
#define USBC_UPSTA3SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA3SET) MASK Register */

/* -------- USBC_UPSTA4SET : (USBC Offset: 0x5A0) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA4SET_OFFSET       0x5A0        /**< \brief (USBC_UPSTA4SET offset) Pipe Status Set Register */
#define USBC_UPSTA4SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA4SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA4SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA4SET) RXINI Set */
#define USBC_UPSTA4SET_RXINIS       (0x1u << USBC_UPSTA4SET_RXINIS_Pos)
#define USBC_UPSTA4SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA4SET) TXOUTI Set */
#define USBC_UPSTA4SET_TXOUTIS      (0x1u << USBC_UPSTA4SET_TXOUTIS_Pos)
#define USBC_UPSTA4SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA4SET) TXSTPI Set */
#define USBC_UPSTA4SET_TXSTPIS      (0x1u << USBC_UPSTA4SET_TXSTPIS_Pos)
#define USBC_UPSTA4SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA4SET) PERRI Set */
#define USBC_UPSTA4SET_PERRIS       (0x1u << USBC_UPSTA4SET_PERRIS_Pos)
#define USBC_UPSTA4SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA4SET) NAKEDI Set */
#define USBC_UPSTA4SET_NAKEDIS      (0x1u << USBC_UPSTA4SET_NAKEDIS_Pos)
#define USBC_UPSTA4SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA4SET) ERRORFI Set */
#define USBC_UPSTA4SET_ERRORFIS     (0x1u << USBC_UPSTA4SET_ERRORFIS_Pos)
#define USBC_UPSTA4SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA4SET) RXSTALLDI Set */
#define USBC_UPSTA4SET_RXSTALLDIS   (0x1u << USBC_UPSTA4SET_RXSTALLDIS_Pos)
#define USBC_UPSTA4SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA4SET) RAMACERI Set */
#define USBC_UPSTA4SET_RAMACERIS    (0x1u << USBC_UPSTA4SET_RAMACERIS_Pos)
#define USBC_UPSTA4SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA4SET) MASK Register */

/* -------- USBC_UPSTA5SET : (USBC Offset: 0x5A4) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA5SET_OFFSET       0x5A4        /**< \brief (USBC_UPSTA5SET offset) Pipe Status Set Register */
#define USBC_UPSTA5SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA5SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA5SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA5SET) RXINI Set */
#define USBC_UPSTA5SET_RXINIS       (0x1u << USBC_UPSTA5SET_RXINIS_Pos)
#define USBC_UPSTA5SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA5SET) TXOUTI Set */
#define USBC_UPSTA5SET_TXOUTIS      (0x1u << USBC_UPSTA5SET_TXOUTIS_Pos)
#define USBC_UPSTA5SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA5SET) TXSTPI Set */
#define USBC_UPSTA5SET_TXSTPIS      (0x1u << USBC_UPSTA5SET_TXSTPIS_Pos)
#define USBC_UPSTA5SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA5SET) PERRI Set */
#define USBC_UPSTA5SET_PERRIS       (0x1u << USBC_UPSTA5SET_PERRIS_Pos)
#define USBC_UPSTA5SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA5SET) NAKEDI Set */
#define USBC_UPSTA5SET_NAKEDIS      (0x1u << USBC_UPSTA5SET_NAKEDIS_Pos)
#define USBC_UPSTA5SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA5SET) ERRORFI Set */
#define USBC_UPSTA5SET_ERRORFIS     (0x1u << USBC_UPSTA5SET_ERRORFIS_Pos)
#define USBC_UPSTA5SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA5SET) RXSTALLDI Set */
#define USBC_UPSTA5SET_RXSTALLDIS   (0x1u << USBC_UPSTA5SET_RXSTALLDIS_Pos)
#define USBC_UPSTA5SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA5SET) RAMACERI Set */
#define USBC_UPSTA5SET_RAMACERIS    (0x1u << USBC_UPSTA5SET_RAMACERIS_Pos)
#define USBC_UPSTA5SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA5SET) MASK Register */

/* -------- USBC_UPSTA6SET : (USBC Offset: 0x5A8) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA6SET_OFFSET       0x5A8        /**< \brief (USBC_UPSTA6SET offset) Pipe Status Set Register */
#define USBC_UPSTA6SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA6SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA6SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA6SET) RXINI Set */
#define USBC_UPSTA6SET_RXINIS       (0x1u << USBC_UPSTA6SET_RXINIS_Pos)
#define USBC_UPSTA6SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA6SET) TXOUTI Set */
#define USBC_UPSTA6SET_TXOUTIS      (0x1u << USBC_UPSTA6SET_TXOUTIS_Pos)
#define USBC_UPSTA6SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA6SET) TXSTPI Set */
#define USBC_UPSTA6SET_TXSTPIS      (0x1u << USBC_UPSTA6SET_TXSTPIS_Pos)
#define USBC_UPSTA6SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA6SET) PERRI Set */
#define USBC_UPSTA6SET_PERRIS       (0x1u << USBC_UPSTA6SET_PERRIS_Pos)
#define USBC_UPSTA6SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA6SET) NAKEDI Set */
#define USBC_UPSTA6SET_NAKEDIS      (0x1u << USBC_UPSTA6SET_NAKEDIS_Pos)
#define USBC_UPSTA6SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA6SET) ERRORFI Set */
#define USBC_UPSTA6SET_ERRORFIS     (0x1u << USBC_UPSTA6SET_ERRORFIS_Pos)
#define USBC_UPSTA6SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA6SET) RXSTALLDI Set */
#define USBC_UPSTA6SET_RXSTALLDIS   (0x1u << USBC_UPSTA6SET_RXSTALLDIS_Pos)
#define USBC_UPSTA6SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA6SET) RAMACERI Set */
#define USBC_UPSTA6SET_RAMACERIS    (0x1u << USBC_UPSTA6SET_RAMACERIS_Pos)
#define USBC_UPSTA6SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA6SET) MASK Register */

/* -------- USBC_UPSTA7SET : (USBC Offset: 0x5AC) ( /W 32) Pipe Status Set Register -------- */
#define USBC_UPSTA7SET_OFFSET       0x5AC        /**< \brief (USBC_UPSTA7SET offset) Pipe Status Set Register */
#define USBC_UPSTA7SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPSTA7SET reset_value) Pipe Status Set Register */

#define USBC_UPSTA7SET_RXINIS_Pos   0            /**< \brief (USBC_UPSTA7SET) RXINI Set */
#define USBC_UPSTA7SET_RXINIS       (0x1u << USBC_UPSTA7SET_RXINIS_Pos)
#define USBC_UPSTA7SET_TXOUTIS_Pos  1            /**< \brief (USBC_UPSTA7SET) TXOUTI Set */
#define USBC_UPSTA7SET_TXOUTIS      (0x1u << USBC_UPSTA7SET_TXOUTIS_Pos)
#define USBC_UPSTA7SET_TXSTPIS_Pos  2            /**< \brief (USBC_UPSTA7SET) TXSTPI Set */
#define USBC_UPSTA7SET_TXSTPIS      (0x1u << USBC_UPSTA7SET_TXSTPIS_Pos)
#define USBC_UPSTA7SET_PERRIS_Pos   3            /**< \brief (USBC_UPSTA7SET) PERRI Set */
#define USBC_UPSTA7SET_PERRIS       (0x1u << USBC_UPSTA7SET_PERRIS_Pos)
#define USBC_UPSTA7SET_NAKEDIS_Pos  4            /**< \brief (USBC_UPSTA7SET) NAKEDI Set */
#define USBC_UPSTA7SET_NAKEDIS      (0x1u << USBC_UPSTA7SET_NAKEDIS_Pos)
#define USBC_UPSTA7SET_ERRORFIS_Pos 5            /**< \brief (USBC_UPSTA7SET) ERRORFI Set */
#define USBC_UPSTA7SET_ERRORFIS     (0x1u << USBC_UPSTA7SET_ERRORFIS_Pos)
#define USBC_UPSTA7SET_RXSTALLDIS_Pos 6            /**< \brief (USBC_UPSTA7SET) RXSTALLDI Set */
#define USBC_UPSTA7SET_RXSTALLDIS   (0x1u << USBC_UPSTA7SET_RXSTALLDIS_Pos)
#define USBC_UPSTA7SET_RAMACERIS_Pos 10           /**< \brief (USBC_UPSTA7SET) RAMACERI Set */
#define USBC_UPSTA7SET_RAMACERIS    (0x1u << USBC_UPSTA7SET_RAMACERIS_Pos)
#define USBC_UPSTA7SET_MASK         0x0000047Fu  /**< \brief (USBC_UPSTA7SET) MASK Register */

/* -------- USBC_UPCON0 : (USBC Offset: 0x5C0) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON0_OFFSET          0x5C0        /**< \brief (USBC_UPCON0 offset) Pipe Control Register */
#define USBC_UPCON0_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON0 reset_value) Pipe Control Register */

#define USBC_UPCON0_RXINE_Pos       0            /**< \brief (USBC_UPCON0) RXIN Interrupt Enable */
#define USBC_UPCON0_RXINE           (0x1u << USBC_UPCON0_RXINE_Pos)
#define USBC_UPCON0_TXOUTE_Pos      1            /**< \brief (USBC_UPCON0) TXOUT Interrupt Enable */
#define USBC_UPCON0_TXOUTE          (0x1u << USBC_UPCON0_TXOUTE_Pos)
#define USBC_UPCON0_TXSTPE_Pos      2            /**< \brief (USBC_UPCON0) TXSTP Interrupt Enable */
#define USBC_UPCON0_TXSTPE          (0x1u << USBC_UPCON0_TXSTPE_Pos)
#define USBC_UPCON0_PERRE_Pos       3            /**< \brief (USBC_UPCON0) PERR Interrupt Enable */
#define USBC_UPCON0_PERRE           (0x1u << USBC_UPCON0_PERRE_Pos)
#define USBC_UPCON0_NAKEDE_Pos      4            /**< \brief (USBC_UPCON0) NAKED Interrupt Enable */
#define USBC_UPCON0_NAKEDE          (0x1u << USBC_UPCON0_NAKEDE_Pos)
#define USBC_UPCON0_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON0) ERRORFI Interrupt Enable */
#define USBC_UPCON0_ERRORFIE        (0x1u << USBC_UPCON0_ERRORFIE_Pos)
#define USBC_UPCON0_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON0) RXTALLD Interrupt Enable */
#define USBC_UPCON0_RXSTALLDE       (0x1u << USBC_UPCON0_RXSTALLDE_Pos)
#define USBC_UPCON0_RAMACERE_Pos    10           /**< \brief (USBC_UPCON0) RAMACER Interrupt Enable */
#define USBC_UPCON0_RAMACERE        (0x1u << USBC_UPCON0_RAMACERE_Pos)
#define USBC_UPCON0_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON0) NBUSYBKInterrupt Enable */
#define USBC_UPCON0_NBUSYBKE        (0x1u << USBC_UPCON0_NBUSYBKE_Pos)
#define USBC_UPCON0_FIFOCON_Pos     14           /**< \brief (USBC_UPCON0) FIFO Control */
#define USBC_UPCON0_FIFOCON         (0x1u << USBC_UPCON0_FIFOCON_Pos)
#define USBC_UPCON0_PFREEZE_Pos     17           /**< \brief (USBC_UPCON0) Pipe Freeze */
#define USBC_UPCON0_PFREEZE         (0x1u << USBC_UPCON0_PFREEZE_Pos)
#define USBC_UPCON0_INITDTGL_Pos    18           /**< \brief (USBC_UPCON0) Data Toggle Initialization */
#define USBC_UPCON0_INITDTGL        (0x1u << USBC_UPCON0_INITDTGL_Pos)
#define USBC_UPCON0_INITBK_Pos      19           /**< \brief (USBC_UPCON0) Bank Initialization */
#define USBC_UPCON0_INITBK          (0x1u << USBC_UPCON0_INITBK_Pos)
#define USBC_UPCON0_MASK            0x000E547Fu  /**< \brief (USBC_UPCON0) MASK Register */

/* -------- USBC_UPCON1 : (USBC Offset: 0x5C4) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON1_OFFSET          0x5C4        /**< \brief (USBC_UPCON1 offset) Pipe Control Register */
#define USBC_UPCON1_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON1 reset_value) Pipe Control Register */

#define USBC_UPCON1_RXINE_Pos       0            /**< \brief (USBC_UPCON1) RXIN Interrupt Enable */
#define USBC_UPCON1_RXINE           (0x1u << USBC_UPCON1_RXINE_Pos)
#define USBC_UPCON1_TXOUTE_Pos      1            /**< \brief (USBC_UPCON1) TXOUT Interrupt Enable */
#define USBC_UPCON1_TXOUTE          (0x1u << USBC_UPCON1_TXOUTE_Pos)
#define USBC_UPCON1_TXSTPE_Pos      2            /**< \brief (USBC_UPCON1) TXSTP Interrupt Enable */
#define USBC_UPCON1_TXSTPE          (0x1u << USBC_UPCON1_TXSTPE_Pos)
#define USBC_UPCON1_PERRE_Pos       3            /**< \brief (USBC_UPCON1) PERR Interrupt Enable */
#define USBC_UPCON1_PERRE           (0x1u << USBC_UPCON1_PERRE_Pos)
#define USBC_UPCON1_NAKEDE_Pos      4            /**< \brief (USBC_UPCON1) NAKED Interrupt Enable */
#define USBC_UPCON1_NAKEDE          (0x1u << USBC_UPCON1_NAKEDE_Pos)
#define USBC_UPCON1_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON1) ERRORFI Interrupt Enable */
#define USBC_UPCON1_ERRORFIE        (0x1u << USBC_UPCON1_ERRORFIE_Pos)
#define USBC_UPCON1_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON1) RXTALLD Interrupt Enable */
#define USBC_UPCON1_RXSTALLDE       (0x1u << USBC_UPCON1_RXSTALLDE_Pos)
#define USBC_UPCON1_RAMACERE_Pos    10           /**< \brief (USBC_UPCON1) RAMACER Interrupt Enable */
#define USBC_UPCON1_RAMACERE        (0x1u << USBC_UPCON1_RAMACERE_Pos)
#define USBC_UPCON1_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON1) NBUSYBKInterrupt Enable */
#define USBC_UPCON1_NBUSYBKE        (0x1u << USBC_UPCON1_NBUSYBKE_Pos)
#define USBC_UPCON1_FIFOCON_Pos     14           /**< \brief (USBC_UPCON1) FIFO Control */
#define USBC_UPCON1_FIFOCON         (0x1u << USBC_UPCON1_FIFOCON_Pos)
#define USBC_UPCON1_PFREEZE_Pos     17           /**< \brief (USBC_UPCON1) Pipe Freeze */
#define USBC_UPCON1_PFREEZE         (0x1u << USBC_UPCON1_PFREEZE_Pos)
#define USBC_UPCON1_INITDTGL_Pos    18           /**< \brief (USBC_UPCON1) Data Toggle Initialization */
#define USBC_UPCON1_INITDTGL        (0x1u << USBC_UPCON1_INITDTGL_Pos)
#define USBC_UPCON1_INITBK_Pos      19           /**< \brief (USBC_UPCON1) Bank Initialization */
#define USBC_UPCON1_INITBK          (0x1u << USBC_UPCON1_INITBK_Pos)
#define USBC_UPCON1_MASK            0x000E547Fu  /**< \brief (USBC_UPCON1) MASK Register */

/* -------- USBC_UPCON2 : (USBC Offset: 0x5C8) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON2_OFFSET          0x5C8        /**< \brief (USBC_UPCON2 offset) Pipe Control Register */
#define USBC_UPCON2_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON2 reset_value) Pipe Control Register */

#define USBC_UPCON2_RXINE_Pos       0            /**< \brief (USBC_UPCON2) RXIN Interrupt Enable */
#define USBC_UPCON2_RXINE           (0x1u << USBC_UPCON2_RXINE_Pos)
#define USBC_UPCON2_TXOUTE_Pos      1            /**< \brief (USBC_UPCON2) TXOUT Interrupt Enable */
#define USBC_UPCON2_TXOUTE          (0x1u << USBC_UPCON2_TXOUTE_Pos)
#define USBC_UPCON2_TXSTPE_Pos      2            /**< \brief (USBC_UPCON2) TXSTP Interrupt Enable */
#define USBC_UPCON2_TXSTPE          (0x1u << USBC_UPCON2_TXSTPE_Pos)
#define USBC_UPCON2_PERRE_Pos       3            /**< \brief (USBC_UPCON2) PERR Interrupt Enable */
#define USBC_UPCON2_PERRE           (0x1u << USBC_UPCON2_PERRE_Pos)
#define USBC_UPCON2_NAKEDE_Pos      4            /**< \brief (USBC_UPCON2) NAKED Interrupt Enable */
#define USBC_UPCON2_NAKEDE          (0x1u << USBC_UPCON2_NAKEDE_Pos)
#define USBC_UPCON2_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON2) ERRORFI Interrupt Enable */
#define USBC_UPCON2_ERRORFIE        (0x1u << USBC_UPCON2_ERRORFIE_Pos)
#define USBC_UPCON2_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON2) RXTALLD Interrupt Enable */
#define USBC_UPCON2_RXSTALLDE       (0x1u << USBC_UPCON2_RXSTALLDE_Pos)
#define USBC_UPCON2_RAMACERE_Pos    10           /**< \brief (USBC_UPCON2) RAMACER Interrupt Enable */
#define USBC_UPCON2_RAMACERE        (0x1u << USBC_UPCON2_RAMACERE_Pos)
#define USBC_UPCON2_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON2) NBUSYBKInterrupt Enable */
#define USBC_UPCON2_NBUSYBKE        (0x1u << USBC_UPCON2_NBUSYBKE_Pos)
#define USBC_UPCON2_FIFOCON_Pos     14           /**< \brief (USBC_UPCON2) FIFO Control */
#define USBC_UPCON2_FIFOCON         (0x1u << USBC_UPCON2_FIFOCON_Pos)
#define USBC_UPCON2_PFREEZE_Pos     17           /**< \brief (USBC_UPCON2) Pipe Freeze */
#define USBC_UPCON2_PFREEZE         (0x1u << USBC_UPCON2_PFREEZE_Pos)
#define USBC_UPCON2_INITDTGL_Pos    18           /**< \brief (USBC_UPCON2) Data Toggle Initialization */
#define USBC_UPCON2_INITDTGL        (0x1u << USBC_UPCON2_INITDTGL_Pos)
#define USBC_UPCON2_INITBK_Pos      19           /**< \brief (USBC_UPCON2) Bank Initialization */
#define USBC_UPCON2_INITBK          (0x1u << USBC_UPCON2_INITBK_Pos)
#define USBC_UPCON2_MASK            0x000E547Fu  /**< \brief (USBC_UPCON2) MASK Register */

/* -------- USBC_UPCON3 : (USBC Offset: 0x5CC) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON3_OFFSET          0x5CC        /**< \brief (USBC_UPCON3 offset) Pipe Control Register */
#define USBC_UPCON3_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON3 reset_value) Pipe Control Register */

#define USBC_UPCON3_RXINE_Pos       0            /**< \brief (USBC_UPCON3) RXIN Interrupt Enable */
#define USBC_UPCON3_RXINE           (0x1u << USBC_UPCON3_RXINE_Pos)
#define USBC_UPCON3_TXOUTE_Pos      1            /**< \brief (USBC_UPCON3) TXOUT Interrupt Enable */
#define USBC_UPCON3_TXOUTE          (0x1u << USBC_UPCON3_TXOUTE_Pos)
#define USBC_UPCON3_TXSTPE_Pos      2            /**< \brief (USBC_UPCON3) TXSTP Interrupt Enable */
#define USBC_UPCON3_TXSTPE          (0x1u << USBC_UPCON3_TXSTPE_Pos)
#define USBC_UPCON3_PERRE_Pos       3            /**< \brief (USBC_UPCON3) PERR Interrupt Enable */
#define USBC_UPCON3_PERRE           (0x1u << USBC_UPCON3_PERRE_Pos)
#define USBC_UPCON3_NAKEDE_Pos      4            /**< \brief (USBC_UPCON3) NAKED Interrupt Enable */
#define USBC_UPCON3_NAKEDE          (0x1u << USBC_UPCON3_NAKEDE_Pos)
#define USBC_UPCON3_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON3) ERRORFI Interrupt Enable */
#define USBC_UPCON3_ERRORFIE        (0x1u << USBC_UPCON3_ERRORFIE_Pos)
#define USBC_UPCON3_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON3) RXTALLD Interrupt Enable */
#define USBC_UPCON3_RXSTALLDE       (0x1u << USBC_UPCON3_RXSTALLDE_Pos)
#define USBC_UPCON3_RAMACERE_Pos    10           /**< \brief (USBC_UPCON3) RAMACER Interrupt Enable */
#define USBC_UPCON3_RAMACERE        (0x1u << USBC_UPCON3_RAMACERE_Pos)
#define USBC_UPCON3_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON3) NBUSYBKInterrupt Enable */
#define USBC_UPCON3_NBUSYBKE        (0x1u << USBC_UPCON3_NBUSYBKE_Pos)
#define USBC_UPCON3_FIFOCON_Pos     14           /**< \brief (USBC_UPCON3) FIFO Control */
#define USBC_UPCON3_FIFOCON         (0x1u << USBC_UPCON3_FIFOCON_Pos)
#define USBC_UPCON3_PFREEZE_Pos     17           /**< \brief (USBC_UPCON3) Pipe Freeze */
#define USBC_UPCON3_PFREEZE         (0x1u << USBC_UPCON3_PFREEZE_Pos)
#define USBC_UPCON3_INITDTGL_Pos    18           /**< \brief (USBC_UPCON3) Data Toggle Initialization */
#define USBC_UPCON3_INITDTGL        (0x1u << USBC_UPCON3_INITDTGL_Pos)
#define USBC_UPCON3_INITBK_Pos      19           /**< \brief (USBC_UPCON3) Bank Initialization */
#define USBC_UPCON3_INITBK          (0x1u << USBC_UPCON3_INITBK_Pos)
#define USBC_UPCON3_MASK            0x000E547Fu  /**< \brief (USBC_UPCON3) MASK Register */

/* -------- USBC_UPCON4 : (USBC Offset: 0x5D0) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON4_OFFSET          0x5D0        /**< \brief (USBC_UPCON4 offset) Pipe Control Register */
#define USBC_UPCON4_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON4 reset_value) Pipe Control Register */

#define USBC_UPCON4_RXINE_Pos       0            /**< \brief (USBC_UPCON4) RXIN Interrupt Enable */
#define USBC_UPCON4_RXINE           (0x1u << USBC_UPCON4_RXINE_Pos)
#define USBC_UPCON4_TXOUTE_Pos      1            /**< \brief (USBC_UPCON4) TXOUT Interrupt Enable */
#define USBC_UPCON4_TXOUTE          (0x1u << USBC_UPCON4_TXOUTE_Pos)
#define USBC_UPCON4_TXSTPE_Pos      2            /**< \brief (USBC_UPCON4) TXSTP Interrupt Enable */
#define USBC_UPCON4_TXSTPE          (0x1u << USBC_UPCON4_TXSTPE_Pos)
#define USBC_UPCON4_PERRE_Pos       3            /**< \brief (USBC_UPCON4) PERR Interrupt Enable */
#define USBC_UPCON4_PERRE           (0x1u << USBC_UPCON4_PERRE_Pos)
#define USBC_UPCON4_NAKEDE_Pos      4            /**< \brief (USBC_UPCON4) NAKED Interrupt Enable */
#define USBC_UPCON4_NAKEDE          (0x1u << USBC_UPCON4_NAKEDE_Pos)
#define USBC_UPCON4_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON4) ERRORFI Interrupt Enable */
#define USBC_UPCON4_ERRORFIE        (0x1u << USBC_UPCON4_ERRORFIE_Pos)
#define USBC_UPCON4_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON4) RXTALLD Interrupt Enable */
#define USBC_UPCON4_RXSTALLDE       (0x1u << USBC_UPCON4_RXSTALLDE_Pos)
#define USBC_UPCON4_RAMACERE_Pos    10           /**< \brief (USBC_UPCON4) RAMACER Interrupt Enable */
#define USBC_UPCON4_RAMACERE        (0x1u << USBC_UPCON4_RAMACERE_Pos)
#define USBC_UPCON4_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON4) NBUSYBKInterrupt Enable */
#define USBC_UPCON4_NBUSYBKE        (0x1u << USBC_UPCON4_NBUSYBKE_Pos)
#define USBC_UPCON4_FIFOCON_Pos     14           /**< \brief (USBC_UPCON4) FIFO Control */
#define USBC_UPCON4_FIFOCON         (0x1u << USBC_UPCON4_FIFOCON_Pos)
#define USBC_UPCON4_PFREEZE_Pos     17           /**< \brief (USBC_UPCON4) Pipe Freeze */
#define USBC_UPCON4_PFREEZE         (0x1u << USBC_UPCON4_PFREEZE_Pos)
#define USBC_UPCON4_INITDTGL_Pos    18           /**< \brief (USBC_UPCON4) Data Toggle Initialization */
#define USBC_UPCON4_INITDTGL        (0x1u << USBC_UPCON4_INITDTGL_Pos)
#define USBC_UPCON4_INITBK_Pos      19           /**< \brief (USBC_UPCON4) Bank Initialization */
#define USBC_UPCON4_INITBK          (0x1u << USBC_UPCON4_INITBK_Pos)
#define USBC_UPCON4_MASK            0x000E547Fu  /**< \brief (USBC_UPCON4) MASK Register */

/* -------- USBC_UPCON5 : (USBC Offset: 0x5D4) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON5_OFFSET          0x5D4        /**< \brief (USBC_UPCON5 offset) Pipe Control Register */
#define USBC_UPCON5_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON5 reset_value) Pipe Control Register */

#define USBC_UPCON5_RXINE_Pos       0            /**< \brief (USBC_UPCON5) RXIN Interrupt Enable */
#define USBC_UPCON5_RXINE           (0x1u << USBC_UPCON5_RXINE_Pos)
#define USBC_UPCON5_TXOUTE_Pos      1            /**< \brief (USBC_UPCON5) TXOUT Interrupt Enable */
#define USBC_UPCON5_TXOUTE          (0x1u << USBC_UPCON5_TXOUTE_Pos)
#define USBC_UPCON5_TXSTPE_Pos      2            /**< \brief (USBC_UPCON5) TXSTP Interrupt Enable */
#define USBC_UPCON5_TXSTPE          (0x1u << USBC_UPCON5_TXSTPE_Pos)
#define USBC_UPCON5_PERRE_Pos       3            /**< \brief (USBC_UPCON5) PERR Interrupt Enable */
#define USBC_UPCON5_PERRE           (0x1u << USBC_UPCON5_PERRE_Pos)
#define USBC_UPCON5_NAKEDE_Pos      4            /**< \brief (USBC_UPCON5) NAKED Interrupt Enable */
#define USBC_UPCON5_NAKEDE          (0x1u << USBC_UPCON5_NAKEDE_Pos)
#define USBC_UPCON5_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON5) ERRORFI Interrupt Enable */
#define USBC_UPCON5_ERRORFIE        (0x1u << USBC_UPCON5_ERRORFIE_Pos)
#define USBC_UPCON5_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON5) RXTALLD Interrupt Enable */
#define USBC_UPCON5_RXSTALLDE       (0x1u << USBC_UPCON5_RXSTALLDE_Pos)
#define USBC_UPCON5_RAMACERE_Pos    10           /**< \brief (USBC_UPCON5) RAMACER Interrupt Enable */
#define USBC_UPCON5_RAMACERE        (0x1u << USBC_UPCON5_RAMACERE_Pos)
#define USBC_UPCON5_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON5) NBUSYBKInterrupt Enable */
#define USBC_UPCON5_NBUSYBKE        (0x1u << USBC_UPCON5_NBUSYBKE_Pos)
#define USBC_UPCON5_FIFOCON_Pos     14           /**< \brief (USBC_UPCON5) FIFO Control */
#define USBC_UPCON5_FIFOCON         (0x1u << USBC_UPCON5_FIFOCON_Pos)
#define USBC_UPCON5_PFREEZE_Pos     17           /**< \brief (USBC_UPCON5) Pipe Freeze */
#define USBC_UPCON5_PFREEZE         (0x1u << USBC_UPCON5_PFREEZE_Pos)
#define USBC_UPCON5_INITDTGL_Pos    18           /**< \brief (USBC_UPCON5) Data Toggle Initialization */
#define USBC_UPCON5_INITDTGL        (0x1u << USBC_UPCON5_INITDTGL_Pos)
#define USBC_UPCON5_INITBK_Pos      19           /**< \brief (USBC_UPCON5) Bank Initialization */
#define USBC_UPCON5_INITBK          (0x1u << USBC_UPCON5_INITBK_Pos)
#define USBC_UPCON5_MASK            0x000E547Fu  /**< \brief (USBC_UPCON5) MASK Register */

/* -------- USBC_UPCON6 : (USBC Offset: 0x5D8) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON6_OFFSET          0x5D8        /**< \brief (USBC_UPCON6 offset) Pipe Control Register */
#define USBC_UPCON6_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON6 reset_value) Pipe Control Register */

#define USBC_UPCON6_RXINE_Pos       0            /**< \brief (USBC_UPCON6) RXIN Interrupt Enable */
#define USBC_UPCON6_RXINE           (0x1u << USBC_UPCON6_RXINE_Pos)
#define USBC_UPCON6_TXOUTE_Pos      1            /**< \brief (USBC_UPCON6) TXOUT Interrupt Enable */
#define USBC_UPCON6_TXOUTE          (0x1u << USBC_UPCON6_TXOUTE_Pos)
#define USBC_UPCON6_TXSTPE_Pos      2            /**< \brief (USBC_UPCON6) TXSTP Interrupt Enable */
#define USBC_UPCON6_TXSTPE          (0x1u << USBC_UPCON6_TXSTPE_Pos)
#define USBC_UPCON6_PERRE_Pos       3            /**< \brief (USBC_UPCON6) PERR Interrupt Enable */
#define USBC_UPCON6_PERRE           (0x1u << USBC_UPCON6_PERRE_Pos)
#define USBC_UPCON6_NAKEDE_Pos      4            /**< \brief (USBC_UPCON6) NAKED Interrupt Enable */
#define USBC_UPCON6_NAKEDE          (0x1u << USBC_UPCON6_NAKEDE_Pos)
#define USBC_UPCON6_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON6) ERRORFI Interrupt Enable */
#define USBC_UPCON6_ERRORFIE        (0x1u << USBC_UPCON6_ERRORFIE_Pos)
#define USBC_UPCON6_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON6) RXTALLD Interrupt Enable */
#define USBC_UPCON6_RXSTALLDE       (0x1u << USBC_UPCON6_RXSTALLDE_Pos)
#define USBC_UPCON6_RAMACERE_Pos    10           /**< \brief (USBC_UPCON6) RAMACER Interrupt Enable */
#define USBC_UPCON6_RAMACERE        (0x1u << USBC_UPCON6_RAMACERE_Pos)
#define USBC_UPCON6_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON6) NBUSYBKInterrupt Enable */
#define USBC_UPCON6_NBUSYBKE        (0x1u << USBC_UPCON6_NBUSYBKE_Pos)
#define USBC_UPCON6_FIFOCON_Pos     14           /**< \brief (USBC_UPCON6) FIFO Control */
#define USBC_UPCON6_FIFOCON         (0x1u << USBC_UPCON6_FIFOCON_Pos)
#define USBC_UPCON6_PFREEZE_Pos     17           /**< \brief (USBC_UPCON6) Pipe Freeze */
#define USBC_UPCON6_PFREEZE         (0x1u << USBC_UPCON6_PFREEZE_Pos)
#define USBC_UPCON6_INITDTGL_Pos    18           /**< \brief (USBC_UPCON6) Data Toggle Initialization */
#define USBC_UPCON6_INITDTGL        (0x1u << USBC_UPCON6_INITDTGL_Pos)
#define USBC_UPCON6_INITBK_Pos      19           /**< \brief (USBC_UPCON6) Bank Initialization */
#define USBC_UPCON6_INITBK          (0x1u << USBC_UPCON6_INITBK_Pos)
#define USBC_UPCON6_MASK            0x000E547Fu  /**< \brief (USBC_UPCON6) MASK Register */

/* -------- USBC_UPCON7 : (USBC Offset: 0x5DC) (R/  32) Pipe Control Register -------- */
#define USBC_UPCON7_OFFSET          0x5DC        /**< \brief (USBC_UPCON7 offset) Pipe Control Register */
#define USBC_UPCON7_RESETVALUE      0x00000000   /**< \brief (USBC_UPCON7 reset_value) Pipe Control Register */

#define USBC_UPCON7_RXINE_Pos       0            /**< \brief (USBC_UPCON7) RXIN Interrupt Enable */
#define USBC_UPCON7_RXINE           (0x1u << USBC_UPCON7_RXINE_Pos)
#define USBC_UPCON7_TXOUTE_Pos      1            /**< \brief (USBC_UPCON7) TXOUT Interrupt Enable */
#define USBC_UPCON7_TXOUTE          (0x1u << USBC_UPCON7_TXOUTE_Pos)
#define USBC_UPCON7_TXSTPE_Pos      2            /**< \brief (USBC_UPCON7) TXSTP Interrupt Enable */
#define USBC_UPCON7_TXSTPE          (0x1u << USBC_UPCON7_TXSTPE_Pos)
#define USBC_UPCON7_PERRE_Pos       3            /**< \brief (USBC_UPCON7) PERR Interrupt Enable */
#define USBC_UPCON7_PERRE           (0x1u << USBC_UPCON7_PERRE_Pos)
#define USBC_UPCON7_NAKEDE_Pos      4            /**< \brief (USBC_UPCON7) NAKED Interrupt Enable */
#define USBC_UPCON7_NAKEDE          (0x1u << USBC_UPCON7_NAKEDE_Pos)
#define USBC_UPCON7_ERRORFIE_Pos    5            /**< \brief (USBC_UPCON7) ERRORFI Interrupt Enable */
#define USBC_UPCON7_ERRORFIE        (0x1u << USBC_UPCON7_ERRORFIE_Pos)
#define USBC_UPCON7_RXSTALLDE_Pos   6            /**< \brief (USBC_UPCON7) RXTALLD Interrupt Enable */
#define USBC_UPCON7_RXSTALLDE       (0x1u << USBC_UPCON7_RXSTALLDE_Pos)
#define USBC_UPCON7_RAMACERE_Pos    10           /**< \brief (USBC_UPCON7) RAMACER Interrupt Enable */
#define USBC_UPCON7_RAMACERE        (0x1u << USBC_UPCON7_RAMACERE_Pos)
#define USBC_UPCON7_NBUSYBKE_Pos    12           /**< \brief (USBC_UPCON7) NBUSYBKInterrupt Enable */
#define USBC_UPCON7_NBUSYBKE        (0x1u << USBC_UPCON7_NBUSYBKE_Pos)
#define USBC_UPCON7_FIFOCON_Pos     14           /**< \brief (USBC_UPCON7) FIFO Control */
#define USBC_UPCON7_FIFOCON         (0x1u << USBC_UPCON7_FIFOCON_Pos)
#define USBC_UPCON7_PFREEZE_Pos     17           /**< \brief (USBC_UPCON7) Pipe Freeze */
#define USBC_UPCON7_PFREEZE         (0x1u << USBC_UPCON7_PFREEZE_Pos)
#define USBC_UPCON7_INITDTGL_Pos    18           /**< \brief (USBC_UPCON7) Data Toggle Initialization */
#define USBC_UPCON7_INITDTGL        (0x1u << USBC_UPCON7_INITDTGL_Pos)
#define USBC_UPCON7_INITBK_Pos      19           /**< \brief (USBC_UPCON7) Bank Initialization */
#define USBC_UPCON7_INITBK          (0x1u << USBC_UPCON7_INITBK_Pos)
#define USBC_UPCON7_MASK            0x000E547Fu  /**< \brief (USBC_UPCON7) MASK Register */

/* -------- USBC_UPCON0SET : (USBC Offset: 0x5F0) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON0SET_OFFSET       0x5F0        /**< \brief (USBC_UPCON0SET offset) Pipe Control Set Register */
#define USBC_UPCON0SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON0SET reset_value) Pipe Control Set Register */

#define USBC_UPCON0SET_RXINES_Pos   0            /**< \brief (USBC_UPCON0SET) RXINE Set */
#define USBC_UPCON0SET_RXINES       (0x1u << USBC_UPCON0SET_RXINES_Pos)
#define USBC_UPCON0SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON0SET) TXOUTE Set */
#define USBC_UPCON0SET_TXOUTES      (0x1u << USBC_UPCON0SET_TXOUTES_Pos)
#define USBC_UPCON0SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON0SET) TXSTPE Set */
#define USBC_UPCON0SET_TXSTPES      (0x1u << USBC_UPCON0SET_TXSTPES_Pos)
#define USBC_UPCON0SET_PERRES_Pos   3            /**< \brief (USBC_UPCON0SET) PERRE Set */
#define USBC_UPCON0SET_PERRES       (0x1u << USBC_UPCON0SET_PERRES_Pos)
#define USBC_UPCON0SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON0SET) NAKEDE Set */
#define USBC_UPCON0SET_NAKEDES      (0x1u << USBC_UPCON0SET_NAKEDES_Pos)
#define USBC_UPCON0SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON0SET) ERRORFIE Set */
#define USBC_UPCON0SET_ERRORFIES    (0x1u << USBC_UPCON0SET_ERRORFIES_Pos)
#define USBC_UPCON0SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON0SET) RXSTALLDE Set */
#define USBC_UPCON0SET_RXSTALLDES   (0x1u << USBC_UPCON0SET_RXSTALLDES_Pos)
#define USBC_UPCON0SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON0SET) RAMACERE Set */
#define USBC_UPCON0SET_RAMACERES    (0x1u << USBC_UPCON0SET_RAMACERES_Pos)
#define USBC_UPCON0SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON0SET) NBUSYBKE Set */
#define USBC_UPCON0SET_NBUSYBKES    (0x1u << USBC_UPCON0SET_NBUSYBKES_Pos)
#define USBC_UPCON0SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON0SET) FIFOCON Set */
#define USBC_UPCON0SET_FIFOCONS     (0x1u << USBC_UPCON0SET_FIFOCONS_Pos)
#define USBC_UPCON0SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON0SET) PFREEZE Set */
#define USBC_UPCON0SET_PFREEZES     (0x1u << USBC_UPCON0SET_PFREEZES_Pos)
#define USBC_UPCON0SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON0SET) INITDTGL Set */
#define USBC_UPCON0SET_INITDTGLS    (0x1u << USBC_UPCON0SET_INITDTGLS_Pos)
#define USBC_UPCON0SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON0SET) INITBK Set */
#define USBC_UPCON0SET_INITBKS      (0x1u << USBC_UPCON0SET_INITBKS_Pos)
#define USBC_UPCON0SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON0SET) MASK Register */

/* -------- USBC_UPCON1SET : (USBC Offset: 0x5F4) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON1SET_OFFSET       0x5F4        /**< \brief (USBC_UPCON1SET offset) Pipe Control Set Register */
#define USBC_UPCON1SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON1SET reset_value) Pipe Control Set Register */

#define USBC_UPCON1SET_RXINES_Pos   0            /**< \brief (USBC_UPCON1SET) RXINE Set */
#define USBC_UPCON1SET_RXINES       (0x1u << USBC_UPCON1SET_RXINES_Pos)
#define USBC_UPCON1SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON1SET) TXOUTE Set */
#define USBC_UPCON1SET_TXOUTES      (0x1u << USBC_UPCON1SET_TXOUTES_Pos)
#define USBC_UPCON1SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON1SET) TXSTPE Set */
#define USBC_UPCON1SET_TXSTPES      (0x1u << USBC_UPCON1SET_TXSTPES_Pos)
#define USBC_UPCON1SET_PERRES_Pos   3            /**< \brief (USBC_UPCON1SET) PERRE Set */
#define USBC_UPCON1SET_PERRES       (0x1u << USBC_UPCON1SET_PERRES_Pos)
#define USBC_UPCON1SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON1SET) NAKEDE Set */
#define USBC_UPCON1SET_NAKEDES      (0x1u << USBC_UPCON1SET_NAKEDES_Pos)
#define USBC_UPCON1SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON1SET) ERRORFIE Set */
#define USBC_UPCON1SET_ERRORFIES    (0x1u << USBC_UPCON1SET_ERRORFIES_Pos)
#define USBC_UPCON1SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON1SET) RXSTALLDE Set */
#define USBC_UPCON1SET_RXSTALLDES   (0x1u << USBC_UPCON1SET_RXSTALLDES_Pos)
#define USBC_UPCON1SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON1SET) RAMACERE Set */
#define USBC_UPCON1SET_RAMACERES    (0x1u << USBC_UPCON1SET_RAMACERES_Pos)
#define USBC_UPCON1SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON1SET) NBUSYBKE Set */
#define USBC_UPCON1SET_NBUSYBKES    (0x1u << USBC_UPCON1SET_NBUSYBKES_Pos)
#define USBC_UPCON1SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON1SET) FIFOCON Set */
#define USBC_UPCON1SET_FIFOCONS     (0x1u << USBC_UPCON1SET_FIFOCONS_Pos)
#define USBC_UPCON1SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON1SET) PFREEZE Set */
#define USBC_UPCON1SET_PFREEZES     (0x1u << USBC_UPCON1SET_PFREEZES_Pos)
#define USBC_UPCON1SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON1SET) INITDTGL Set */
#define USBC_UPCON1SET_INITDTGLS    (0x1u << USBC_UPCON1SET_INITDTGLS_Pos)
#define USBC_UPCON1SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON1SET) INITBK Set */
#define USBC_UPCON1SET_INITBKS      (0x1u << USBC_UPCON1SET_INITBKS_Pos)
#define USBC_UPCON1SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON1SET) MASK Register */

/* -------- USBC_UPCON2SET : (USBC Offset: 0x5F8) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON2SET_OFFSET       0x5F8        /**< \brief (USBC_UPCON2SET offset) Pipe Control Set Register */
#define USBC_UPCON2SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON2SET reset_value) Pipe Control Set Register */

#define USBC_UPCON2SET_RXINES_Pos   0            /**< \brief (USBC_UPCON2SET) RXINE Set */
#define USBC_UPCON2SET_RXINES       (0x1u << USBC_UPCON2SET_RXINES_Pos)
#define USBC_UPCON2SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON2SET) TXOUTE Set */
#define USBC_UPCON2SET_TXOUTES      (0x1u << USBC_UPCON2SET_TXOUTES_Pos)
#define USBC_UPCON2SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON2SET) TXSTPE Set */
#define USBC_UPCON2SET_TXSTPES      (0x1u << USBC_UPCON2SET_TXSTPES_Pos)
#define USBC_UPCON2SET_PERRES_Pos   3            /**< \brief (USBC_UPCON2SET) PERRE Set */
#define USBC_UPCON2SET_PERRES       (0x1u << USBC_UPCON2SET_PERRES_Pos)
#define USBC_UPCON2SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON2SET) NAKEDE Set */
#define USBC_UPCON2SET_NAKEDES      (0x1u << USBC_UPCON2SET_NAKEDES_Pos)
#define USBC_UPCON2SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON2SET) ERRORFIE Set */
#define USBC_UPCON2SET_ERRORFIES    (0x1u << USBC_UPCON2SET_ERRORFIES_Pos)
#define USBC_UPCON2SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON2SET) RXSTALLDE Set */
#define USBC_UPCON2SET_RXSTALLDES   (0x1u << USBC_UPCON2SET_RXSTALLDES_Pos)
#define USBC_UPCON2SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON2SET) RAMACERE Set */
#define USBC_UPCON2SET_RAMACERES    (0x1u << USBC_UPCON2SET_RAMACERES_Pos)
#define USBC_UPCON2SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON2SET) NBUSYBKE Set */
#define USBC_UPCON2SET_NBUSYBKES    (0x1u << USBC_UPCON2SET_NBUSYBKES_Pos)
#define USBC_UPCON2SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON2SET) FIFOCON Set */
#define USBC_UPCON2SET_FIFOCONS     (0x1u << USBC_UPCON2SET_FIFOCONS_Pos)
#define USBC_UPCON2SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON2SET) PFREEZE Set */
#define USBC_UPCON2SET_PFREEZES     (0x1u << USBC_UPCON2SET_PFREEZES_Pos)
#define USBC_UPCON2SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON2SET) INITDTGL Set */
#define USBC_UPCON2SET_INITDTGLS    (0x1u << USBC_UPCON2SET_INITDTGLS_Pos)
#define USBC_UPCON2SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON2SET) INITBK Set */
#define USBC_UPCON2SET_INITBKS      (0x1u << USBC_UPCON2SET_INITBKS_Pos)
#define USBC_UPCON2SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON2SET) MASK Register */

/* -------- USBC_UPCON3SET : (USBC Offset: 0x5FC) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON3SET_OFFSET       0x5FC        /**< \brief (USBC_UPCON3SET offset) Pipe Control Set Register */
#define USBC_UPCON3SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON3SET reset_value) Pipe Control Set Register */

#define USBC_UPCON3SET_RXINES_Pos   0            /**< \brief (USBC_UPCON3SET) RXINE Set */
#define USBC_UPCON3SET_RXINES       (0x1u << USBC_UPCON3SET_RXINES_Pos)
#define USBC_UPCON3SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON3SET) TXOUTE Set */
#define USBC_UPCON3SET_TXOUTES      (0x1u << USBC_UPCON3SET_TXOUTES_Pos)
#define USBC_UPCON3SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON3SET) TXSTPE Set */
#define USBC_UPCON3SET_TXSTPES      (0x1u << USBC_UPCON3SET_TXSTPES_Pos)
#define USBC_UPCON3SET_PERRES_Pos   3            /**< \brief (USBC_UPCON3SET) PERRE Set */
#define USBC_UPCON3SET_PERRES       (0x1u << USBC_UPCON3SET_PERRES_Pos)
#define USBC_UPCON3SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON3SET) NAKEDE Set */
#define USBC_UPCON3SET_NAKEDES      (0x1u << USBC_UPCON3SET_NAKEDES_Pos)
#define USBC_UPCON3SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON3SET) ERRORFIE Set */
#define USBC_UPCON3SET_ERRORFIES    (0x1u << USBC_UPCON3SET_ERRORFIES_Pos)
#define USBC_UPCON3SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON3SET) RXSTALLDE Set */
#define USBC_UPCON3SET_RXSTALLDES   (0x1u << USBC_UPCON3SET_RXSTALLDES_Pos)
#define USBC_UPCON3SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON3SET) RAMACERE Set */
#define USBC_UPCON3SET_RAMACERES    (0x1u << USBC_UPCON3SET_RAMACERES_Pos)
#define USBC_UPCON3SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON3SET) NBUSYBKE Set */
#define USBC_UPCON3SET_NBUSYBKES    (0x1u << USBC_UPCON3SET_NBUSYBKES_Pos)
#define USBC_UPCON3SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON3SET) FIFOCON Set */
#define USBC_UPCON3SET_FIFOCONS     (0x1u << USBC_UPCON3SET_FIFOCONS_Pos)
#define USBC_UPCON3SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON3SET) PFREEZE Set */
#define USBC_UPCON3SET_PFREEZES     (0x1u << USBC_UPCON3SET_PFREEZES_Pos)
#define USBC_UPCON3SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON3SET) INITDTGL Set */
#define USBC_UPCON3SET_INITDTGLS    (0x1u << USBC_UPCON3SET_INITDTGLS_Pos)
#define USBC_UPCON3SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON3SET) INITBK Set */
#define USBC_UPCON3SET_INITBKS      (0x1u << USBC_UPCON3SET_INITBKS_Pos)
#define USBC_UPCON3SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON3SET) MASK Register */

/* -------- USBC_UPCON4SET : (USBC Offset: 0x600) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON4SET_OFFSET       0x600        /**< \brief (USBC_UPCON4SET offset) Pipe Control Set Register */
#define USBC_UPCON4SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON4SET reset_value) Pipe Control Set Register */

#define USBC_UPCON4SET_RXINES_Pos   0            /**< \brief (USBC_UPCON4SET) RXINE Set */
#define USBC_UPCON4SET_RXINES       (0x1u << USBC_UPCON4SET_RXINES_Pos)
#define USBC_UPCON4SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON4SET) TXOUTE Set */
#define USBC_UPCON4SET_TXOUTES      (0x1u << USBC_UPCON4SET_TXOUTES_Pos)
#define USBC_UPCON4SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON4SET) TXSTPE Set */
#define USBC_UPCON4SET_TXSTPES      (0x1u << USBC_UPCON4SET_TXSTPES_Pos)
#define USBC_UPCON4SET_PERRES_Pos   3            /**< \brief (USBC_UPCON4SET) PERRE Set */
#define USBC_UPCON4SET_PERRES       (0x1u << USBC_UPCON4SET_PERRES_Pos)
#define USBC_UPCON4SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON4SET) NAKEDE Set */
#define USBC_UPCON4SET_NAKEDES      (0x1u << USBC_UPCON4SET_NAKEDES_Pos)
#define USBC_UPCON4SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON4SET) ERRORFIE Set */
#define USBC_UPCON4SET_ERRORFIES    (0x1u << USBC_UPCON4SET_ERRORFIES_Pos)
#define USBC_UPCON4SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON4SET) RXSTALLDE Set */
#define USBC_UPCON4SET_RXSTALLDES   (0x1u << USBC_UPCON4SET_RXSTALLDES_Pos)
#define USBC_UPCON4SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON4SET) RAMACERE Set */
#define USBC_UPCON4SET_RAMACERES    (0x1u << USBC_UPCON4SET_RAMACERES_Pos)
#define USBC_UPCON4SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON4SET) NBUSYBKE Set */
#define USBC_UPCON4SET_NBUSYBKES    (0x1u << USBC_UPCON4SET_NBUSYBKES_Pos)
#define USBC_UPCON4SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON4SET) FIFOCON Set */
#define USBC_UPCON4SET_FIFOCONS     (0x1u << USBC_UPCON4SET_FIFOCONS_Pos)
#define USBC_UPCON4SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON4SET) PFREEZE Set */
#define USBC_UPCON4SET_PFREEZES     (0x1u << USBC_UPCON4SET_PFREEZES_Pos)
#define USBC_UPCON4SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON4SET) INITDTGL Set */
#define USBC_UPCON4SET_INITDTGLS    (0x1u << USBC_UPCON4SET_INITDTGLS_Pos)
#define USBC_UPCON4SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON4SET) INITBK Set */
#define USBC_UPCON4SET_INITBKS      (0x1u << USBC_UPCON4SET_INITBKS_Pos)
#define USBC_UPCON4SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON4SET) MASK Register */

/* -------- USBC_UPCON5SET : (USBC Offset: 0x604) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON5SET_OFFSET       0x604        /**< \brief (USBC_UPCON5SET offset) Pipe Control Set Register */
#define USBC_UPCON5SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON5SET reset_value) Pipe Control Set Register */

#define USBC_UPCON5SET_RXINES_Pos   0            /**< \brief (USBC_UPCON5SET) RXINE Set */
#define USBC_UPCON5SET_RXINES       (0x1u << USBC_UPCON5SET_RXINES_Pos)
#define USBC_UPCON5SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON5SET) TXOUTE Set */
#define USBC_UPCON5SET_TXOUTES      (0x1u << USBC_UPCON5SET_TXOUTES_Pos)
#define USBC_UPCON5SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON5SET) TXSTPE Set */
#define USBC_UPCON5SET_TXSTPES      (0x1u << USBC_UPCON5SET_TXSTPES_Pos)
#define USBC_UPCON5SET_PERRES_Pos   3            /**< \brief (USBC_UPCON5SET) PERRE Set */
#define USBC_UPCON5SET_PERRES       (0x1u << USBC_UPCON5SET_PERRES_Pos)
#define USBC_UPCON5SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON5SET) NAKEDE Set */
#define USBC_UPCON5SET_NAKEDES      (0x1u << USBC_UPCON5SET_NAKEDES_Pos)
#define USBC_UPCON5SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON5SET) ERRORFIE Set */
#define USBC_UPCON5SET_ERRORFIES    (0x1u << USBC_UPCON5SET_ERRORFIES_Pos)
#define USBC_UPCON5SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON5SET) RXSTALLDE Set */
#define USBC_UPCON5SET_RXSTALLDES   (0x1u << USBC_UPCON5SET_RXSTALLDES_Pos)
#define USBC_UPCON5SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON5SET) RAMACERE Set */
#define USBC_UPCON5SET_RAMACERES    (0x1u << USBC_UPCON5SET_RAMACERES_Pos)
#define USBC_UPCON5SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON5SET) NBUSYBKE Set */
#define USBC_UPCON5SET_NBUSYBKES    (0x1u << USBC_UPCON5SET_NBUSYBKES_Pos)
#define USBC_UPCON5SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON5SET) FIFOCON Set */
#define USBC_UPCON5SET_FIFOCONS     (0x1u << USBC_UPCON5SET_FIFOCONS_Pos)
#define USBC_UPCON5SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON5SET) PFREEZE Set */
#define USBC_UPCON5SET_PFREEZES     (0x1u << USBC_UPCON5SET_PFREEZES_Pos)
#define USBC_UPCON5SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON5SET) INITDTGL Set */
#define USBC_UPCON5SET_INITDTGLS    (0x1u << USBC_UPCON5SET_INITDTGLS_Pos)
#define USBC_UPCON5SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON5SET) INITBK Set */
#define USBC_UPCON5SET_INITBKS      (0x1u << USBC_UPCON5SET_INITBKS_Pos)
#define USBC_UPCON5SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON5SET) MASK Register */

/* -------- USBC_UPCON6SET : (USBC Offset: 0x608) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON6SET_OFFSET       0x608        /**< \brief (USBC_UPCON6SET offset) Pipe Control Set Register */
#define USBC_UPCON6SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON6SET reset_value) Pipe Control Set Register */

#define USBC_UPCON6SET_RXINES_Pos   0            /**< \brief (USBC_UPCON6SET) RXINE Set */
#define USBC_UPCON6SET_RXINES       (0x1u << USBC_UPCON6SET_RXINES_Pos)
#define USBC_UPCON6SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON6SET) TXOUTE Set */
#define USBC_UPCON6SET_TXOUTES      (0x1u << USBC_UPCON6SET_TXOUTES_Pos)
#define USBC_UPCON6SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON6SET) TXSTPE Set */
#define USBC_UPCON6SET_TXSTPES      (0x1u << USBC_UPCON6SET_TXSTPES_Pos)
#define USBC_UPCON6SET_PERRES_Pos   3            /**< \brief (USBC_UPCON6SET) PERRE Set */
#define USBC_UPCON6SET_PERRES       (0x1u << USBC_UPCON6SET_PERRES_Pos)
#define USBC_UPCON6SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON6SET) NAKEDE Set */
#define USBC_UPCON6SET_NAKEDES      (0x1u << USBC_UPCON6SET_NAKEDES_Pos)
#define USBC_UPCON6SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON6SET) ERRORFIE Set */
#define USBC_UPCON6SET_ERRORFIES    (0x1u << USBC_UPCON6SET_ERRORFIES_Pos)
#define USBC_UPCON6SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON6SET) RXSTALLDE Set */
#define USBC_UPCON6SET_RXSTALLDES   (0x1u << USBC_UPCON6SET_RXSTALLDES_Pos)
#define USBC_UPCON6SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON6SET) RAMACERE Set */
#define USBC_UPCON6SET_RAMACERES    (0x1u << USBC_UPCON6SET_RAMACERES_Pos)
#define USBC_UPCON6SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON6SET) NBUSYBKE Set */
#define USBC_UPCON6SET_NBUSYBKES    (0x1u << USBC_UPCON6SET_NBUSYBKES_Pos)
#define USBC_UPCON6SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON6SET) FIFOCON Set */
#define USBC_UPCON6SET_FIFOCONS     (0x1u << USBC_UPCON6SET_FIFOCONS_Pos)
#define USBC_UPCON6SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON6SET) PFREEZE Set */
#define USBC_UPCON6SET_PFREEZES     (0x1u << USBC_UPCON6SET_PFREEZES_Pos)
#define USBC_UPCON6SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON6SET) INITDTGL Set */
#define USBC_UPCON6SET_INITDTGLS    (0x1u << USBC_UPCON6SET_INITDTGLS_Pos)
#define USBC_UPCON6SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON6SET) INITBK Set */
#define USBC_UPCON6SET_INITBKS      (0x1u << USBC_UPCON6SET_INITBKS_Pos)
#define USBC_UPCON6SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON6SET) MASK Register */

/* -------- USBC_UPCON7SET : (USBC Offset: 0x60C) ( /W 32) Pipe Control Set Register -------- */
#define USBC_UPCON7SET_OFFSET       0x60C        /**< \brief (USBC_UPCON7SET offset) Pipe Control Set Register */
#define USBC_UPCON7SET_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON7SET reset_value) Pipe Control Set Register */

#define USBC_UPCON7SET_RXINES_Pos   0            /**< \brief (USBC_UPCON7SET) RXINE Set */
#define USBC_UPCON7SET_RXINES       (0x1u << USBC_UPCON7SET_RXINES_Pos)
#define USBC_UPCON7SET_TXOUTES_Pos  1            /**< \brief (USBC_UPCON7SET) TXOUTE Set */
#define USBC_UPCON7SET_TXOUTES      (0x1u << USBC_UPCON7SET_TXOUTES_Pos)
#define USBC_UPCON7SET_TXSTPES_Pos  2            /**< \brief (USBC_UPCON7SET) TXSTPE Set */
#define USBC_UPCON7SET_TXSTPES      (0x1u << USBC_UPCON7SET_TXSTPES_Pos)
#define USBC_UPCON7SET_PERRES_Pos   3            /**< \brief (USBC_UPCON7SET) PERRE Set */
#define USBC_UPCON7SET_PERRES       (0x1u << USBC_UPCON7SET_PERRES_Pos)
#define USBC_UPCON7SET_NAKEDES_Pos  4            /**< \brief (USBC_UPCON7SET) NAKEDE Set */
#define USBC_UPCON7SET_NAKEDES      (0x1u << USBC_UPCON7SET_NAKEDES_Pos)
#define USBC_UPCON7SET_ERRORFIES_Pos 5            /**< \brief (USBC_UPCON7SET) ERRORFIE Set */
#define USBC_UPCON7SET_ERRORFIES    (0x1u << USBC_UPCON7SET_ERRORFIES_Pos)
#define USBC_UPCON7SET_RXSTALLDES_Pos 6            /**< \brief (USBC_UPCON7SET) RXSTALLDE Set */
#define USBC_UPCON7SET_RXSTALLDES   (0x1u << USBC_UPCON7SET_RXSTALLDES_Pos)
#define USBC_UPCON7SET_RAMACERES_Pos 10           /**< \brief (USBC_UPCON7SET) RAMACERE Set */
#define USBC_UPCON7SET_RAMACERES    (0x1u << USBC_UPCON7SET_RAMACERES_Pos)
#define USBC_UPCON7SET_NBUSYBKES_Pos 12           /**< \brief (USBC_UPCON7SET) NBUSYBKE Set */
#define USBC_UPCON7SET_NBUSYBKES    (0x1u << USBC_UPCON7SET_NBUSYBKES_Pos)
#define USBC_UPCON7SET_FIFOCONS_Pos 14           /**< \brief (USBC_UPCON7SET) FIFOCON Set */
#define USBC_UPCON7SET_FIFOCONS     (0x1u << USBC_UPCON7SET_FIFOCONS_Pos)
#define USBC_UPCON7SET_PFREEZES_Pos 17           /**< \brief (USBC_UPCON7SET) PFREEZE Set */
#define USBC_UPCON7SET_PFREEZES     (0x1u << USBC_UPCON7SET_PFREEZES_Pos)
#define USBC_UPCON7SET_INITDTGLS_Pos 18           /**< \brief (USBC_UPCON7SET) INITDTGL Set */
#define USBC_UPCON7SET_INITDTGLS    (0x1u << USBC_UPCON7SET_INITDTGLS_Pos)
#define USBC_UPCON7SET_INITBKS_Pos  19           /**< \brief (USBC_UPCON7SET) INITBK Set */
#define USBC_UPCON7SET_INITBKS      (0x1u << USBC_UPCON7SET_INITBKS_Pos)
#define USBC_UPCON7SET_MASK         0x000E547Fu  /**< \brief (USBC_UPCON7SET) MASK Register */

/* -------- USBC_UPCON0CLR : (USBC Offset: 0x620) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON0CLR_OFFSET       0x620        /**< \brief (USBC_UPCON0CLR offset) Pipe Control Clear Register */
#define USBC_UPCON0CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON0CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON0CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON0CLR) RXINE Clear */
#define USBC_UPCON0CLR_RXINEC       (0x1u << USBC_UPCON0CLR_RXINEC_Pos)
#define USBC_UPCON0CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON0CLR) TXOUTE Clear */
#define USBC_UPCON0CLR_TXOUTEC      (0x1u << USBC_UPCON0CLR_TXOUTEC_Pos)
#define USBC_UPCON0CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON0CLR) TXSTPE Clear */
#define USBC_UPCON0CLR_TXSTPEC      (0x1u << USBC_UPCON0CLR_TXSTPEC_Pos)
#define USBC_UPCON0CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON0CLR) PERRE Clear */
#define USBC_UPCON0CLR_PERREC       (0x1u << USBC_UPCON0CLR_PERREC_Pos)
#define USBC_UPCON0CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON0CLR) NAKEDE Clear */
#define USBC_UPCON0CLR_NAKEDEC      (0x1u << USBC_UPCON0CLR_NAKEDEC_Pos)
#define USBC_UPCON0CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON0CLR) ERRORFIE Clear */
#define USBC_UPCON0CLR_ERRORFIEC    (0x1u << USBC_UPCON0CLR_ERRORFIEC_Pos)
#define USBC_UPCON0CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON0CLR) RXTALLDE Clear */
#define USBC_UPCON0CLR_RXSTALLDEC   (0x1u << USBC_UPCON0CLR_RXSTALLDEC_Pos)
#define USBC_UPCON0CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON0CLR) RAMACERE Clear */
#define USBC_UPCON0CLR_RAMACEREC    (0x1u << USBC_UPCON0CLR_RAMACEREC_Pos)
#define USBC_UPCON0CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON0CLR) NBUSYBKE Clear */
#define USBC_UPCON0CLR_NBUSYBKEC    (0x1u << USBC_UPCON0CLR_NBUSYBKEC_Pos)
#define USBC_UPCON0CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON0CLR) FIFOCON Clear */
#define USBC_UPCON0CLR_FIFOCONC     (0x1u << USBC_UPCON0CLR_FIFOCONC_Pos)
#define USBC_UPCON0CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON0CLR) PFREEZE Clear */
#define USBC_UPCON0CLR_PFREEZEC     (0x1u << USBC_UPCON0CLR_PFREEZEC_Pos)
#define USBC_UPCON0CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON0CLR) INITDTGL Clear */
#define USBC_UPCON0CLR_INITDTGLC    (0x1u << USBC_UPCON0CLR_INITDTGLC_Pos)
#define USBC_UPCON0CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON0CLR) INITBK Clear */
#define USBC_UPCON0CLR_INITBKC      (0x1u << USBC_UPCON0CLR_INITBKC_Pos)
#define USBC_UPCON0CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON0CLR) MASK Register */

/* -------- USBC_UPCON1CLR : (USBC Offset: 0x624) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON1CLR_OFFSET       0x624        /**< \brief (USBC_UPCON1CLR offset) Pipe Control Clear Register */
#define USBC_UPCON1CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON1CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON1CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON1CLR) RXINE Clear */
#define USBC_UPCON1CLR_RXINEC       (0x1u << USBC_UPCON1CLR_RXINEC_Pos)
#define USBC_UPCON1CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON1CLR) TXOUTE Clear */
#define USBC_UPCON1CLR_TXOUTEC      (0x1u << USBC_UPCON1CLR_TXOUTEC_Pos)
#define USBC_UPCON1CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON1CLR) TXSTPE Clear */
#define USBC_UPCON1CLR_TXSTPEC      (0x1u << USBC_UPCON1CLR_TXSTPEC_Pos)
#define USBC_UPCON1CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON1CLR) PERRE Clear */
#define USBC_UPCON1CLR_PERREC       (0x1u << USBC_UPCON1CLR_PERREC_Pos)
#define USBC_UPCON1CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON1CLR) NAKEDE Clear */
#define USBC_UPCON1CLR_NAKEDEC      (0x1u << USBC_UPCON1CLR_NAKEDEC_Pos)
#define USBC_UPCON1CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON1CLR) ERRORFIE Clear */
#define USBC_UPCON1CLR_ERRORFIEC    (0x1u << USBC_UPCON1CLR_ERRORFIEC_Pos)
#define USBC_UPCON1CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON1CLR) RXTALLDE Clear */
#define USBC_UPCON1CLR_RXSTALLDEC   (0x1u << USBC_UPCON1CLR_RXSTALLDEC_Pos)
#define USBC_UPCON1CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON1CLR) RAMACERE Clear */
#define USBC_UPCON1CLR_RAMACEREC    (0x1u << USBC_UPCON1CLR_RAMACEREC_Pos)
#define USBC_UPCON1CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON1CLR) NBUSYBKE Clear */
#define USBC_UPCON1CLR_NBUSYBKEC    (0x1u << USBC_UPCON1CLR_NBUSYBKEC_Pos)
#define USBC_UPCON1CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON1CLR) FIFOCON Clear */
#define USBC_UPCON1CLR_FIFOCONC     (0x1u << USBC_UPCON1CLR_FIFOCONC_Pos)
#define USBC_UPCON1CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON1CLR) PFREEZE Clear */
#define USBC_UPCON1CLR_PFREEZEC     (0x1u << USBC_UPCON1CLR_PFREEZEC_Pos)
#define USBC_UPCON1CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON1CLR) INITDTGL Clear */
#define USBC_UPCON1CLR_INITDTGLC    (0x1u << USBC_UPCON1CLR_INITDTGLC_Pos)
#define USBC_UPCON1CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON1CLR) INITBK Clear */
#define USBC_UPCON1CLR_INITBKC      (0x1u << USBC_UPCON1CLR_INITBKC_Pos)
#define USBC_UPCON1CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON1CLR) MASK Register */

/* -------- USBC_UPCON2CLR : (USBC Offset: 0x628) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON2CLR_OFFSET       0x628        /**< \brief (USBC_UPCON2CLR offset) Pipe Control Clear Register */
#define USBC_UPCON2CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON2CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON2CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON2CLR) RXINE Clear */
#define USBC_UPCON2CLR_RXINEC       (0x1u << USBC_UPCON2CLR_RXINEC_Pos)
#define USBC_UPCON2CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON2CLR) TXOUTE Clear */
#define USBC_UPCON2CLR_TXOUTEC      (0x1u << USBC_UPCON2CLR_TXOUTEC_Pos)
#define USBC_UPCON2CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON2CLR) TXSTPE Clear */
#define USBC_UPCON2CLR_TXSTPEC      (0x1u << USBC_UPCON2CLR_TXSTPEC_Pos)
#define USBC_UPCON2CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON2CLR) PERRE Clear */
#define USBC_UPCON2CLR_PERREC       (0x1u << USBC_UPCON2CLR_PERREC_Pos)
#define USBC_UPCON2CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON2CLR) NAKEDE Clear */
#define USBC_UPCON2CLR_NAKEDEC      (0x1u << USBC_UPCON2CLR_NAKEDEC_Pos)
#define USBC_UPCON2CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON2CLR) ERRORFIE Clear */
#define USBC_UPCON2CLR_ERRORFIEC    (0x1u << USBC_UPCON2CLR_ERRORFIEC_Pos)
#define USBC_UPCON2CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON2CLR) RXTALLDE Clear */
#define USBC_UPCON2CLR_RXSTALLDEC   (0x1u << USBC_UPCON2CLR_RXSTALLDEC_Pos)
#define USBC_UPCON2CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON2CLR) RAMACERE Clear */
#define USBC_UPCON2CLR_RAMACEREC    (0x1u << USBC_UPCON2CLR_RAMACEREC_Pos)
#define USBC_UPCON2CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON2CLR) NBUSYBKE Clear */
#define USBC_UPCON2CLR_NBUSYBKEC    (0x1u << USBC_UPCON2CLR_NBUSYBKEC_Pos)
#define USBC_UPCON2CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON2CLR) FIFOCON Clear */
#define USBC_UPCON2CLR_FIFOCONC     (0x1u << USBC_UPCON2CLR_FIFOCONC_Pos)
#define USBC_UPCON2CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON2CLR) PFREEZE Clear */
#define USBC_UPCON2CLR_PFREEZEC     (0x1u << USBC_UPCON2CLR_PFREEZEC_Pos)
#define USBC_UPCON2CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON2CLR) INITDTGL Clear */
#define USBC_UPCON2CLR_INITDTGLC    (0x1u << USBC_UPCON2CLR_INITDTGLC_Pos)
#define USBC_UPCON2CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON2CLR) INITBK Clear */
#define USBC_UPCON2CLR_INITBKC      (0x1u << USBC_UPCON2CLR_INITBKC_Pos)
#define USBC_UPCON2CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON2CLR) MASK Register */

/* -------- USBC_UPCON3CLR : (USBC Offset: 0x62C) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON3CLR_OFFSET       0x62C        /**< \brief (USBC_UPCON3CLR offset) Pipe Control Clear Register */
#define USBC_UPCON3CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON3CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON3CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON3CLR) RXINE Clear */
#define USBC_UPCON3CLR_RXINEC       (0x1u << USBC_UPCON3CLR_RXINEC_Pos)
#define USBC_UPCON3CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON3CLR) TXOUTE Clear */
#define USBC_UPCON3CLR_TXOUTEC      (0x1u << USBC_UPCON3CLR_TXOUTEC_Pos)
#define USBC_UPCON3CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON3CLR) TXSTPE Clear */
#define USBC_UPCON3CLR_TXSTPEC      (0x1u << USBC_UPCON3CLR_TXSTPEC_Pos)
#define USBC_UPCON3CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON3CLR) PERRE Clear */
#define USBC_UPCON3CLR_PERREC       (0x1u << USBC_UPCON3CLR_PERREC_Pos)
#define USBC_UPCON3CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON3CLR) NAKEDE Clear */
#define USBC_UPCON3CLR_NAKEDEC      (0x1u << USBC_UPCON3CLR_NAKEDEC_Pos)
#define USBC_UPCON3CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON3CLR) ERRORFIE Clear */
#define USBC_UPCON3CLR_ERRORFIEC    (0x1u << USBC_UPCON3CLR_ERRORFIEC_Pos)
#define USBC_UPCON3CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON3CLR) RXTALLDE Clear */
#define USBC_UPCON3CLR_RXSTALLDEC   (0x1u << USBC_UPCON3CLR_RXSTALLDEC_Pos)
#define USBC_UPCON3CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON3CLR) RAMACERE Clear */
#define USBC_UPCON3CLR_RAMACEREC    (0x1u << USBC_UPCON3CLR_RAMACEREC_Pos)
#define USBC_UPCON3CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON3CLR) NBUSYBKE Clear */
#define USBC_UPCON3CLR_NBUSYBKEC    (0x1u << USBC_UPCON3CLR_NBUSYBKEC_Pos)
#define USBC_UPCON3CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON3CLR) FIFOCON Clear */
#define USBC_UPCON3CLR_FIFOCONC     (0x1u << USBC_UPCON3CLR_FIFOCONC_Pos)
#define USBC_UPCON3CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON3CLR) PFREEZE Clear */
#define USBC_UPCON3CLR_PFREEZEC     (0x1u << USBC_UPCON3CLR_PFREEZEC_Pos)
#define USBC_UPCON3CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON3CLR) INITDTGL Clear */
#define USBC_UPCON3CLR_INITDTGLC    (0x1u << USBC_UPCON3CLR_INITDTGLC_Pos)
#define USBC_UPCON3CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON3CLR) INITBK Clear */
#define USBC_UPCON3CLR_INITBKC      (0x1u << USBC_UPCON3CLR_INITBKC_Pos)
#define USBC_UPCON3CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON3CLR) MASK Register */

/* -------- USBC_UPCON4CLR : (USBC Offset: 0x630) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON4CLR_OFFSET       0x630        /**< \brief (USBC_UPCON4CLR offset) Pipe Control Clear Register */
#define USBC_UPCON4CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON4CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON4CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON4CLR) RXINE Clear */
#define USBC_UPCON4CLR_RXINEC       (0x1u << USBC_UPCON4CLR_RXINEC_Pos)
#define USBC_UPCON4CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON4CLR) TXOUTE Clear */
#define USBC_UPCON4CLR_TXOUTEC      (0x1u << USBC_UPCON4CLR_TXOUTEC_Pos)
#define USBC_UPCON4CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON4CLR) TXSTPE Clear */
#define USBC_UPCON4CLR_TXSTPEC      (0x1u << USBC_UPCON4CLR_TXSTPEC_Pos)
#define USBC_UPCON4CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON4CLR) PERRE Clear */
#define USBC_UPCON4CLR_PERREC       (0x1u << USBC_UPCON4CLR_PERREC_Pos)
#define USBC_UPCON4CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON4CLR) NAKEDE Clear */
#define USBC_UPCON4CLR_NAKEDEC      (0x1u << USBC_UPCON4CLR_NAKEDEC_Pos)
#define USBC_UPCON4CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON4CLR) ERRORFIE Clear */
#define USBC_UPCON4CLR_ERRORFIEC    (0x1u << USBC_UPCON4CLR_ERRORFIEC_Pos)
#define USBC_UPCON4CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON4CLR) RXTALLDE Clear */
#define USBC_UPCON4CLR_RXSTALLDEC   (0x1u << USBC_UPCON4CLR_RXSTALLDEC_Pos)
#define USBC_UPCON4CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON4CLR) RAMACERE Clear */
#define USBC_UPCON4CLR_RAMACEREC    (0x1u << USBC_UPCON4CLR_RAMACEREC_Pos)
#define USBC_UPCON4CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON4CLR) NBUSYBKE Clear */
#define USBC_UPCON4CLR_NBUSYBKEC    (0x1u << USBC_UPCON4CLR_NBUSYBKEC_Pos)
#define USBC_UPCON4CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON4CLR) FIFOCON Clear */
#define USBC_UPCON4CLR_FIFOCONC     (0x1u << USBC_UPCON4CLR_FIFOCONC_Pos)
#define USBC_UPCON4CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON4CLR) PFREEZE Clear */
#define USBC_UPCON4CLR_PFREEZEC     (0x1u << USBC_UPCON4CLR_PFREEZEC_Pos)
#define USBC_UPCON4CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON4CLR) INITDTGL Clear */
#define USBC_UPCON4CLR_INITDTGLC    (0x1u << USBC_UPCON4CLR_INITDTGLC_Pos)
#define USBC_UPCON4CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON4CLR) INITBK Clear */
#define USBC_UPCON4CLR_INITBKC      (0x1u << USBC_UPCON4CLR_INITBKC_Pos)
#define USBC_UPCON4CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON4CLR) MASK Register */

/* -------- USBC_UPCON5CLR : (USBC Offset: 0x634) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON5CLR_OFFSET       0x634        /**< \brief (USBC_UPCON5CLR offset) Pipe Control Clear Register */
#define USBC_UPCON5CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON5CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON5CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON5CLR) RXINE Clear */
#define USBC_UPCON5CLR_RXINEC       (0x1u << USBC_UPCON5CLR_RXINEC_Pos)
#define USBC_UPCON5CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON5CLR) TXOUTE Clear */
#define USBC_UPCON5CLR_TXOUTEC      (0x1u << USBC_UPCON5CLR_TXOUTEC_Pos)
#define USBC_UPCON5CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON5CLR) TXSTPE Clear */
#define USBC_UPCON5CLR_TXSTPEC      (0x1u << USBC_UPCON5CLR_TXSTPEC_Pos)
#define USBC_UPCON5CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON5CLR) PERRE Clear */
#define USBC_UPCON5CLR_PERREC       (0x1u << USBC_UPCON5CLR_PERREC_Pos)
#define USBC_UPCON5CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON5CLR) NAKEDE Clear */
#define USBC_UPCON5CLR_NAKEDEC      (0x1u << USBC_UPCON5CLR_NAKEDEC_Pos)
#define USBC_UPCON5CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON5CLR) ERRORFIE Clear */
#define USBC_UPCON5CLR_ERRORFIEC    (0x1u << USBC_UPCON5CLR_ERRORFIEC_Pos)
#define USBC_UPCON5CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON5CLR) RXTALLDE Clear */
#define USBC_UPCON5CLR_RXSTALLDEC   (0x1u << USBC_UPCON5CLR_RXSTALLDEC_Pos)
#define USBC_UPCON5CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON5CLR) RAMACERE Clear */
#define USBC_UPCON5CLR_RAMACEREC    (0x1u << USBC_UPCON5CLR_RAMACEREC_Pos)
#define USBC_UPCON5CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON5CLR) NBUSYBKE Clear */
#define USBC_UPCON5CLR_NBUSYBKEC    (0x1u << USBC_UPCON5CLR_NBUSYBKEC_Pos)
#define USBC_UPCON5CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON5CLR) FIFOCON Clear */
#define USBC_UPCON5CLR_FIFOCONC     (0x1u << USBC_UPCON5CLR_FIFOCONC_Pos)
#define USBC_UPCON5CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON5CLR) PFREEZE Clear */
#define USBC_UPCON5CLR_PFREEZEC     (0x1u << USBC_UPCON5CLR_PFREEZEC_Pos)
#define USBC_UPCON5CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON5CLR) INITDTGL Clear */
#define USBC_UPCON5CLR_INITDTGLC    (0x1u << USBC_UPCON5CLR_INITDTGLC_Pos)
#define USBC_UPCON5CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON5CLR) INITBK Clear */
#define USBC_UPCON5CLR_INITBKC      (0x1u << USBC_UPCON5CLR_INITBKC_Pos)
#define USBC_UPCON5CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON5CLR) MASK Register */

/* -------- USBC_UPCON6CLR : (USBC Offset: 0x638) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON6CLR_OFFSET       0x638        /**< \brief (USBC_UPCON6CLR offset) Pipe Control Clear Register */
#define USBC_UPCON6CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON6CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON6CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON6CLR) RXINE Clear */
#define USBC_UPCON6CLR_RXINEC       (0x1u << USBC_UPCON6CLR_RXINEC_Pos)
#define USBC_UPCON6CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON6CLR) TXOUTE Clear */
#define USBC_UPCON6CLR_TXOUTEC      (0x1u << USBC_UPCON6CLR_TXOUTEC_Pos)
#define USBC_UPCON6CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON6CLR) TXSTPE Clear */
#define USBC_UPCON6CLR_TXSTPEC      (0x1u << USBC_UPCON6CLR_TXSTPEC_Pos)
#define USBC_UPCON6CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON6CLR) PERRE Clear */
#define USBC_UPCON6CLR_PERREC       (0x1u << USBC_UPCON6CLR_PERREC_Pos)
#define USBC_UPCON6CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON6CLR) NAKEDE Clear */
#define USBC_UPCON6CLR_NAKEDEC      (0x1u << USBC_UPCON6CLR_NAKEDEC_Pos)
#define USBC_UPCON6CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON6CLR) ERRORFIE Clear */
#define USBC_UPCON6CLR_ERRORFIEC    (0x1u << USBC_UPCON6CLR_ERRORFIEC_Pos)
#define USBC_UPCON6CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON6CLR) RXTALLDE Clear */
#define USBC_UPCON6CLR_RXSTALLDEC   (0x1u << USBC_UPCON6CLR_RXSTALLDEC_Pos)
#define USBC_UPCON6CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON6CLR) RAMACERE Clear */
#define USBC_UPCON6CLR_RAMACEREC    (0x1u << USBC_UPCON6CLR_RAMACEREC_Pos)
#define USBC_UPCON6CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON6CLR) NBUSYBKE Clear */
#define USBC_UPCON6CLR_NBUSYBKEC    (0x1u << USBC_UPCON6CLR_NBUSYBKEC_Pos)
#define USBC_UPCON6CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON6CLR) FIFOCON Clear */
#define USBC_UPCON6CLR_FIFOCONC     (0x1u << USBC_UPCON6CLR_FIFOCONC_Pos)
#define USBC_UPCON6CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON6CLR) PFREEZE Clear */
#define USBC_UPCON6CLR_PFREEZEC     (0x1u << USBC_UPCON6CLR_PFREEZEC_Pos)
#define USBC_UPCON6CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON6CLR) INITDTGL Clear */
#define USBC_UPCON6CLR_INITDTGLC    (0x1u << USBC_UPCON6CLR_INITDTGLC_Pos)
#define USBC_UPCON6CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON6CLR) INITBK Clear */
#define USBC_UPCON6CLR_INITBKC      (0x1u << USBC_UPCON6CLR_INITBKC_Pos)
#define USBC_UPCON6CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON6CLR) MASK Register */

/* -------- USBC_UPCON7CLR : (USBC Offset: 0x63C) ( /W 32) Pipe Control Clear Register -------- */
#define USBC_UPCON7CLR_OFFSET       0x63C        /**< \brief (USBC_UPCON7CLR offset) Pipe Control Clear Register */
#define USBC_UPCON7CLR_RESETVALUE   0x00000000   /**< \brief (USBC_UPCON7CLR reset_value) Pipe Control Clear Register */

#define USBC_UPCON7CLR_RXINEC_Pos   0            /**< \brief (USBC_UPCON7CLR) RXINE Clear */
#define USBC_UPCON7CLR_RXINEC       (0x1u << USBC_UPCON7CLR_RXINEC_Pos)
#define USBC_UPCON7CLR_TXOUTEC_Pos  1            /**< \brief (USBC_UPCON7CLR) TXOUTE Clear */
#define USBC_UPCON7CLR_TXOUTEC      (0x1u << USBC_UPCON7CLR_TXOUTEC_Pos)
#define USBC_UPCON7CLR_TXSTPEC_Pos  2            /**< \brief (USBC_UPCON7CLR) TXSTPE Clear */
#define USBC_UPCON7CLR_TXSTPEC      (0x1u << USBC_UPCON7CLR_TXSTPEC_Pos)
#define USBC_UPCON7CLR_PERREC_Pos   3            /**< \brief (USBC_UPCON7CLR) PERRE Clear */
#define USBC_UPCON7CLR_PERREC       (0x1u << USBC_UPCON7CLR_PERREC_Pos)
#define USBC_UPCON7CLR_NAKEDEC_Pos  4            /**< \brief (USBC_UPCON7CLR) NAKEDE Clear */
#define USBC_UPCON7CLR_NAKEDEC      (0x1u << USBC_UPCON7CLR_NAKEDEC_Pos)
#define USBC_UPCON7CLR_ERRORFIEC_Pos 5            /**< \brief (USBC_UPCON7CLR) ERRORFIE Clear */
#define USBC_UPCON7CLR_ERRORFIEC    (0x1u << USBC_UPCON7CLR_ERRORFIEC_Pos)
#define USBC_UPCON7CLR_RXSTALLDEC_Pos 6            /**< \brief (USBC_UPCON7CLR) RXTALLDE Clear */
#define USBC_UPCON7CLR_RXSTALLDEC   (0x1u << USBC_UPCON7CLR_RXSTALLDEC_Pos)
#define USBC_UPCON7CLR_RAMACEREC_Pos 10           /**< \brief (USBC_UPCON7CLR) RAMACERE Clear */
#define USBC_UPCON7CLR_RAMACEREC    (0x1u << USBC_UPCON7CLR_RAMACEREC_Pos)
#define USBC_UPCON7CLR_NBUSYBKEC_Pos 12           /**< \brief (USBC_UPCON7CLR) NBUSYBKE Clear */
#define USBC_UPCON7CLR_NBUSYBKEC    (0x1u << USBC_UPCON7CLR_NBUSYBKEC_Pos)
#define USBC_UPCON7CLR_FIFOCONC_Pos 14           /**< \brief (USBC_UPCON7CLR) FIFOCON Clear */
#define USBC_UPCON7CLR_FIFOCONC     (0x1u << USBC_UPCON7CLR_FIFOCONC_Pos)
#define USBC_UPCON7CLR_PFREEZEC_Pos 17           /**< \brief (USBC_UPCON7CLR) PFREEZE Clear */
#define USBC_UPCON7CLR_PFREEZEC     (0x1u << USBC_UPCON7CLR_PFREEZEC_Pos)
#define USBC_UPCON7CLR_INITDTGLC_Pos 18           /**< \brief (USBC_UPCON7CLR) INITDTGL Clear */
#define USBC_UPCON7CLR_INITDTGLC    (0x1u << USBC_UPCON7CLR_INITDTGLC_Pos)
#define USBC_UPCON7CLR_INITBKC_Pos  19           /**< \brief (USBC_UPCON7CLR) INITBK Clear */
#define USBC_UPCON7CLR_INITBKC      (0x1u << USBC_UPCON7CLR_INITBKC_Pos)
#define USBC_UPCON7CLR_MASK         0x000E547Fu  /**< \brief (USBC_UPCON7CLR) MASK Register */

/* -------- USBC_UPINRQ0 : (USBC Offset: 0x650) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ0_OFFSET         0x650        /**< \brief (USBC_UPINRQ0 offset) Pipe In Request */
#define USBC_UPINRQ0_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ0 reset_value) Pipe In Request */

#define USBC_UPINRQ0_INRQ_Pos       0            /**< \brief (USBC_UPINRQ0) IN Request Number before Freeze */
#define USBC_UPINRQ0_INRQ_Msk       (0xFFu << USBC_UPINRQ0_INRQ_Pos)
#define USBC_UPINRQ0_INRQ(value)    ((USBC_UPINRQ0_INRQ_Msk & ((value) << USBC_UPINRQ0_INRQ_Pos)))
#define USBC_UPINRQ0_INMODE_Pos     8            /**< \brief (USBC_UPINRQ0) IN Request Mode */
#define USBC_UPINRQ0_INMODE         (0x1u << USBC_UPINRQ0_INMODE_Pos)
#define USBC_UPINRQ0_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ0) MASK Register */

/* -------- USBC_UPINRQ1 : (USBC Offset: 0x654) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ1_OFFSET         0x654        /**< \brief (USBC_UPINRQ1 offset) Pipe In Request */
#define USBC_UPINRQ1_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ1 reset_value) Pipe In Request */

#define USBC_UPINRQ1_INRQ_Pos       0            /**< \brief (USBC_UPINRQ1) IN Request Number before Freeze */
#define USBC_UPINRQ1_INRQ_Msk       (0xFFu << USBC_UPINRQ1_INRQ_Pos)
#define USBC_UPINRQ1_INRQ(value)    ((USBC_UPINRQ1_INRQ_Msk & ((value) << USBC_UPINRQ1_INRQ_Pos)))
#define USBC_UPINRQ1_INMODE_Pos     8            /**< \brief (USBC_UPINRQ1) IN Request Mode */
#define USBC_UPINRQ1_INMODE         (0x1u << USBC_UPINRQ1_INMODE_Pos)
#define USBC_UPINRQ1_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ1) MASK Register */

/* -------- USBC_UPINRQ2 : (USBC Offset: 0x658) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ2_OFFSET         0x658        /**< \brief (USBC_UPINRQ2 offset) Pipe In Request */
#define USBC_UPINRQ2_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ2 reset_value) Pipe In Request */

#define USBC_UPINRQ2_INRQ_Pos       0            /**< \brief (USBC_UPINRQ2) IN Request Number before Freeze */
#define USBC_UPINRQ2_INRQ_Msk       (0xFFu << USBC_UPINRQ2_INRQ_Pos)
#define USBC_UPINRQ2_INRQ(value)    ((USBC_UPINRQ2_INRQ_Msk & ((value) << USBC_UPINRQ2_INRQ_Pos)))
#define USBC_UPINRQ2_INMODE_Pos     8            /**< \brief (USBC_UPINRQ2) IN Request Mode */
#define USBC_UPINRQ2_INMODE         (0x1u << USBC_UPINRQ2_INMODE_Pos)
#define USBC_UPINRQ2_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ2) MASK Register */

/* -------- USBC_UPINRQ3 : (USBC Offset: 0x65C) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ3_OFFSET         0x65C        /**< \brief (USBC_UPINRQ3 offset) Pipe In Request */
#define USBC_UPINRQ3_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ3 reset_value) Pipe In Request */

#define USBC_UPINRQ3_INRQ_Pos       0            /**< \brief (USBC_UPINRQ3) IN Request Number before Freeze */
#define USBC_UPINRQ3_INRQ_Msk       (0xFFu << USBC_UPINRQ3_INRQ_Pos)
#define USBC_UPINRQ3_INRQ(value)    ((USBC_UPINRQ3_INRQ_Msk & ((value) << USBC_UPINRQ3_INRQ_Pos)))
#define USBC_UPINRQ3_INMODE_Pos     8            /**< \brief (USBC_UPINRQ3) IN Request Mode */
#define USBC_UPINRQ3_INMODE         (0x1u << USBC_UPINRQ3_INMODE_Pos)
#define USBC_UPINRQ3_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ3) MASK Register */

/* -------- USBC_UPINRQ4 : (USBC Offset: 0x660) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ4_OFFSET         0x660        /**< \brief (USBC_UPINRQ4 offset) Pipe In Request */
#define USBC_UPINRQ4_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ4 reset_value) Pipe In Request */

#define USBC_UPINRQ4_INRQ_Pos       0            /**< \brief (USBC_UPINRQ4) IN Request Number before Freeze */
#define USBC_UPINRQ4_INRQ_Msk       (0xFFu << USBC_UPINRQ4_INRQ_Pos)
#define USBC_UPINRQ4_INRQ(value)    ((USBC_UPINRQ4_INRQ_Msk & ((value) << USBC_UPINRQ4_INRQ_Pos)))
#define USBC_UPINRQ4_INMODE_Pos     8            /**< \brief (USBC_UPINRQ4) IN Request Mode */
#define USBC_UPINRQ4_INMODE         (0x1u << USBC_UPINRQ4_INMODE_Pos)
#define USBC_UPINRQ4_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ4) MASK Register */

/* -------- USBC_UPINRQ5 : (USBC Offset: 0x664) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ5_OFFSET         0x664        /**< \brief (USBC_UPINRQ5 offset) Pipe In Request */
#define USBC_UPINRQ5_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ5 reset_value) Pipe In Request */

#define USBC_UPINRQ5_INRQ_Pos       0            /**< \brief (USBC_UPINRQ5) IN Request Number before Freeze */
#define USBC_UPINRQ5_INRQ_Msk       (0xFFu << USBC_UPINRQ5_INRQ_Pos)
#define USBC_UPINRQ5_INRQ(value)    ((USBC_UPINRQ5_INRQ_Msk & ((value) << USBC_UPINRQ5_INRQ_Pos)))
#define USBC_UPINRQ5_INMODE_Pos     8            /**< \brief (USBC_UPINRQ5) IN Request Mode */
#define USBC_UPINRQ5_INMODE         (0x1u << USBC_UPINRQ5_INMODE_Pos)
#define USBC_UPINRQ5_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ5) MASK Register */

/* -------- USBC_UPINRQ6 : (USBC Offset: 0x668) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ6_OFFSET         0x668        /**< \brief (USBC_UPINRQ6 offset) Pipe In Request */
#define USBC_UPINRQ6_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ6 reset_value) Pipe In Request */

#define USBC_UPINRQ6_INRQ_Pos       0            /**< \brief (USBC_UPINRQ6) IN Request Number before Freeze */
#define USBC_UPINRQ6_INRQ_Msk       (0xFFu << USBC_UPINRQ6_INRQ_Pos)
#define USBC_UPINRQ6_INRQ(value)    ((USBC_UPINRQ6_INRQ_Msk & ((value) << USBC_UPINRQ6_INRQ_Pos)))
#define USBC_UPINRQ6_INMODE_Pos     8            /**< \brief (USBC_UPINRQ6) IN Request Mode */
#define USBC_UPINRQ6_INMODE         (0x1u << USBC_UPINRQ6_INMODE_Pos)
#define USBC_UPINRQ6_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ6) MASK Register */

/* -------- USBC_UPINRQ7 : (USBC Offset: 0x66C) (R/W 32) Pipe In Request -------- */
#define USBC_UPINRQ7_OFFSET         0x66C        /**< \brief (USBC_UPINRQ7 offset) Pipe In Request */
#define USBC_UPINRQ7_RESETVALUE     0x00000001   /**< \brief (USBC_UPINRQ7 reset_value) Pipe In Request */

#define USBC_UPINRQ7_INRQ_Pos       0            /**< \brief (USBC_UPINRQ7) IN Request Number before Freeze */
#define USBC_UPINRQ7_INRQ_Msk       (0xFFu << USBC_UPINRQ7_INRQ_Pos)
#define USBC_UPINRQ7_INRQ(value)    ((USBC_UPINRQ7_INRQ_Msk & ((value) << USBC_UPINRQ7_INRQ_Pos)))
#define USBC_UPINRQ7_INMODE_Pos     8            /**< \brief (USBC_UPINRQ7) IN Request Mode */
#define USBC_UPINRQ7_INMODE         (0x1u << USBC_UPINRQ7_INMODE_Pos)
#define USBC_UPINRQ7_MASK           0x000001FFu  /**< \brief (USBC_UPINRQ7) MASK Register */

/* -------- USBC_USBCON : (USBC Offset: 0x800) (R/W 32) General Control Register -------- */
#define USBC_USBCON_OFFSET          0x800        /**< \brief (USBC_USBCON offset) General Control Register */
#define USBC_USBCON_RESETVALUE      0x01004000   /**< \brief (USBC_USBCON reset_value) General Control Register */

#define USBC_USBCON_FRZCLK_Pos      14           /**< \brief (USBC_USBCON) Freeze USB Clock */
#define USBC_USBCON_FRZCLK          (0x1u << USBC_USBCON_FRZCLK_Pos)
#define USBC_USBCON_USBE_Pos        15           /**< \brief (USBC_USBCON) USBC Enable */
#define USBC_USBCON_USBE            (0x1u << USBC_USBCON_USBE_Pos)
#define USBC_USBCON_UIMOD_Pos       24           /**< \brief (USBC_USBCON) USBC Mode */
#define USBC_USBCON_UIMOD           (0x1u << USBC_USBCON_UIMOD_Pos)
#define USBC_USBCON_MASK            0x0100C000u  /**< \brief (USBC_USBCON) MASK Register */

/* -------- USBC_USBSTA : (USBC Offset: 0x804) (R/  32) General Status Register -------- */
#define USBC_USBSTA_OFFSET          0x804        /**< \brief (USBC_USBSTA offset) General Status Register */
#define USBC_USBSTA_RESETVALUE      0x00010000   /**< \brief (USBC_USBSTA reset_value) General Status Register */

#define USBC_USBSTA_VBUSRQ_Pos      9            /**< \brief (USBC_USBSTA) VBus Request */
#define USBC_USBSTA_VBUSRQ          (0x1u << USBC_USBSTA_VBUSRQ_Pos)
#define USBC_USBSTA_SPEED_Pos       12           /**< \brief (USBC_USBSTA) Speed Status */
#define USBC_USBSTA_SPEED_Msk       (0x3u << USBC_USBSTA_SPEED_Pos)
#define USBC_USBSTA_SPEED(value)    ((USBC_USBSTA_SPEED_Msk & ((value) << USBC_USBSTA_SPEED_Pos)))
#define   USBC_USBSTA_SPEED_FULL    (0x0u << 12) /**< \brief (USBC_USBSTA)  */
#define   USBC_USBSTA_SPEED_HIGH    (0x1u << 12) /**< \brief (USBC_USBSTA)  */
#define   USBC_USBSTA_SPEED_LOW     (0x2u << 12) /**< \brief (USBC_USBSTA)  */
#define USBC_USBSTA_CLKUSABLE_Pos   14           /**< \brief (USBC_USBSTA) USB Clock Usable */
#define USBC_USBSTA_CLKUSABLE       (0x1u << USBC_USBSTA_CLKUSABLE_Pos)
#define USBC_USBSTA_SUSPEND_Pos     16           /**< \brief (USBC_USBSTA) Suspend module state */
#define USBC_USBSTA_SUSPEND         (0x1u << USBC_USBSTA_SUSPEND_Pos)
#define USBC_USBSTA_MASK            0x00017200u  /**< \brief (USBC_USBSTA) MASK Register */

/* -------- USBC_USBSTACLR : (USBC Offset: 0x808) ( /W 32) General Status Clear Register -------- */
#define USBC_USBSTACLR_OFFSET       0x808        /**< \brief (USBC_USBSTACLR offset) General Status Clear Register */
#define USBC_USBSTACLR_RESETVALUE   0x00000000   /**< \brief (USBC_USBSTACLR reset_value) General Status Clear Register */

#define USBC_USBSTACLR_RAMACERIC_Pos 8            /**< \brief (USBC_USBSTACLR) RAMACERI Clear */
#define USBC_USBSTACLR_RAMACERIC    (0x1u << USBC_USBSTACLR_RAMACERIC_Pos)
#define USBC_USBSTACLR_VBUSRQC_Pos  9            /**< \brief (USBC_USBSTACLR) VBUSRQ Clear */
#define USBC_USBSTACLR_VBUSRQC      (0x1u << USBC_USBSTACLR_VBUSRQC_Pos)
#define USBC_USBSTACLR_MASK         0x00000300u  /**< \brief (USBC_USBSTACLR) MASK Register */

/* -------- USBC_USBSTASET : (USBC Offset: 0x80C) ( /W 32) General Status Set Register -------- */
#define USBC_USBSTASET_OFFSET       0x80C        /**< \brief (USBC_USBSTASET offset) General Status Set Register */
#define USBC_USBSTASET_RESETVALUE   0x00000000   /**< \brief (USBC_USBSTASET reset_value) General Status Set Register */

#define USBC_USBSTASET_RAMACERIS_Pos 8            /**< \brief (USBC_USBSTASET) RAMACERI Set */
#define USBC_USBSTASET_RAMACERIS    (0x1u << USBC_USBSTASET_RAMACERIS_Pos)
#define USBC_USBSTASET_VBUSRQS_Pos  9            /**< \brief (USBC_USBSTASET) VBUSRQ Set */
#define USBC_USBSTASET_VBUSRQS      (0x1u << USBC_USBSTASET_VBUSRQS_Pos)
#define USBC_USBSTASET_MASK         0x00000300u  /**< \brief (USBC_USBSTASET) MASK Register */

/* -------- USBC_UVERS : (USBC Offset: 0x818) (R/  32) IP Version Register -------- */
#define USBC_UVERS_OFFSET           0x818        /**< \brief (USBC_UVERS offset) IP Version Register */
#define USBC_UVERS_RESETVALUE       0x00000310   /**< \brief (USBC_UVERS reset_value) IP Version Register */

#define USBC_UVERS_VERSION_Pos      0            /**< \brief (USBC_UVERS) Version Number */
#define USBC_UVERS_VERSION_Msk      (0xFFFu << USBC_UVERS_VERSION_Pos)
#define USBC_UVERS_VERSION(value)   ((USBC_UVERS_VERSION_Msk & ((value) << USBC_UVERS_VERSION_Pos)))
#define USBC_UVERS_VARIANT_Pos      16           /**< \brief (USBC_UVERS) Variant Number */
#define USBC_UVERS_VARIANT_Msk      (0x7u << USBC_UVERS_VARIANT_Pos)
#define USBC_UVERS_VARIANT(value)   ((USBC_UVERS_VARIANT_Msk & ((value) << USBC_UVERS_VARIANT_Pos)))
#define USBC_UVERS_MASK             0x00070FFFu  /**< \brief (USBC_UVERS) MASK Register */

/* -------- USBC_UFEATURES : (USBC Offset: 0x81C) (R/  32) IP Features Register -------- */
#define USBC_UFEATURES_OFFSET       0x81C        /**< \brief (USBC_UFEATURES offset) IP Features Register */
#define USBC_UFEATURES_RESETVALUE   0x00000007   /**< \brief (USBC_UFEATURES reset_value) IP Features Register */

#define USBC_UFEATURES_EPTNBRMAX_Pos 0            /**< \brief (USBC_UFEATURES) Maximum Number of Pipes/Endpints */
#define USBC_UFEATURES_EPTNBRMAX_Msk (0xFu << USBC_UFEATURES_EPTNBRMAX_Pos)
#define USBC_UFEATURES_EPTNBRMAX(value) ((USBC_UFEATURES_EPTNBRMAX_Msk & ((value) << USBC_UFEATURES_EPTNBRMAX_Pos)))
#define USBC_UFEATURES_UTMIMODE_Pos 8            /**< \brief (USBC_UFEATURES) UTMI Mode */
#define USBC_UFEATURES_UTMIMODE     (0x1u << USBC_UFEATURES_UTMIMODE_Pos)
#define USBC_UFEATURES_MASK         0x0000010Fu  /**< \brief (USBC_UFEATURES) MASK Register */

/* -------- USBC_UADDRSIZE : (USBC Offset: 0x820) (R/  32) IP PB address size Register -------- */
#define USBC_UADDRSIZE_OFFSET       0x820        /**< \brief (USBC_UADDRSIZE offset) IP PB address size Register */
#define USBC_UADDRSIZE_RESETVALUE   0x00001000   /**< \brief (USBC_UADDRSIZE reset_value) IP PB address size Register */

#define USBC_UADDRSIZE_UADDRSIZE_Pos 0            /**< \brief (USBC_UADDRSIZE) IP PB Address Size */
#define USBC_UADDRSIZE_UADDRSIZE_Msk (0xFFFFFFFFu << USBC_UADDRSIZE_UADDRSIZE_Pos)
#define USBC_UADDRSIZE_UADDRSIZE(value) ((USBC_UADDRSIZE_UADDRSIZE_Msk & ((value) << USBC_UADDRSIZE_UADDRSIZE_Pos)))
#define USBC_UADDRSIZE_MASK         0xFFFFFFFFu  /**< \brief (USBC_UADDRSIZE) MASK Register */

/* -------- USBC_UNAME1 : (USBC Offset: 0x824) (R/  32) IP Name Part One: HUSB -------- */
#define USBC_UNAME1_OFFSET          0x824        /**< \brief (USBC_UNAME1 offset) IP Name Part One: HUSB */
#define USBC_UNAME1_RESETVALUE      0x48555342   /**< \brief (USBC_UNAME1 reset_value) IP Name Part One: HUSB */

#define USBC_UNAME1_UNAME1_Pos      0            /**< \brief (USBC_UNAME1) IP Name Part One */
#define USBC_UNAME1_UNAME1_Msk      (0xFFFFFFFFu << USBC_UNAME1_UNAME1_Pos)
#define USBC_UNAME1_UNAME1(value)   ((USBC_UNAME1_UNAME1_Msk & ((value) << USBC_UNAME1_UNAME1_Pos)))
#define USBC_UNAME1_MASK            0xFFFFFFFFu  /**< \brief (USBC_UNAME1) MASK Register */

/* -------- USBC_UNAME2 : (USBC Offset: 0x828) (R/  32) IP Name Part Two: HOST -------- */
#define USBC_UNAME2_OFFSET          0x828        /**< \brief (USBC_UNAME2 offset) IP Name Part Two: HOST */
#define USBC_UNAME2_RESETVALUE      0x484F5354   /**< \brief (USBC_UNAME2 reset_value) IP Name Part Two: HOST */

#define USBC_UNAME2_UNAME2_Pos      0            /**< \brief (USBC_UNAME2) IP Name Part Two */
#define USBC_UNAME2_UNAME2_Msk      (0xFFFFFFFFu << USBC_UNAME2_UNAME2_Pos)
#define USBC_UNAME2_UNAME2(value)   ((USBC_UNAME2_UNAME2_Msk & ((value) << USBC_UNAME2_UNAME2_Pos)))
#define USBC_UNAME2_MASK            0xFFFFFFFFu  /**< \brief (USBC_UNAME2) MASK Register */

/* -------- USBC_USBFSM : (USBC Offset: 0x82C) (R/  32) USB internal finite state machine -------- */
#define USBC_USBFSM_OFFSET          0x82C        /**< \brief (USBC_USBFSM offset) USB internal finite state machine */
#define USBC_USBFSM_RESETVALUE      0x00000009   /**< \brief (USBC_USBFSM reset_value) USB internal finite state machine */

#define USBC_USBFSM_DRDSTATE_Pos    0            /**< \brief (USBC_USBFSM) DualRoleDevice state */
#define USBC_USBFSM_DRDSTATE_Msk    (0xFu << USBC_USBFSM_DRDSTATE_Pos)
#define USBC_USBFSM_DRDSTATE(value) ((USBC_USBFSM_DRDSTATE_Msk & ((value) << USBC_USBFSM_DRDSTATE_Pos)))
#define   USBC_USBFSM_DRDSTATE_A_IDLE (0x0u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_WAIT_VRISE (0x1u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_WAIT_BCON (0x2u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_HOST (0x3u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_SUSPEND (0x4u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_PERIPHERAL (0x5u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_WAIT_VFALL (0x6u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_VBUS_ERR (0x7u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_A_WAIT_DISCHARGE (0x8u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_IDLE (0x9u <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_PERIPHERAL (0xAu <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_WAIT_BEGIN_HNP (0xBu <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_WAIT_DISCHARGE (0xCu <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_WAIT_ACON (0xDu <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_HOST (0xEu <<  0) /**< \brief (USBC_USBFSM)  */
#define   USBC_USBFSM_DRDSTATE_B_SRP_INIT (0xFu <<  0) /**< \brief (USBC_USBFSM)  */
#define USBC_USBFSM_MASK            0x0000000Fu  /**< \brief (USBC_USBFSM) MASK Register */

/* -------- USBC_UDESC : (USBC Offset: 0x830) (R/W 32) Endpoint descriptor table -------- */
#define USBC_UDESC_OFFSET           0x830        /**< \brief (USBC_UDESC offset) Endpoint descriptor table */
#define USBC_UDESC_RESETVALUE       0x00000000   /**< \brief (USBC_UDESC reset_value) Endpoint descriptor table */

#define USBC_UDESC_UDESCA_Pos       0            /**< \brief (USBC_UDESC) USB Descriptor Address */
#define USBC_UDESC_UDESCA_Msk       (0xFFFFFFFFu << USBC_UDESC_UDESCA_Pos)
#define USBC_UDESC_UDESCA(value)    ((USBC_UDESC_UDESCA_Msk & ((value) << USBC_UDESC_UDESCA_Pos)))
#define USBC_UDESC_MASK             0xFFFFFFFFu  /**< \brief (USBC_UDESC) MASK Register */

/** \brief USBC hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   USBC_UDCON;         /**< \brief (USBC Offset: 0x000) Device General Control Register */
  RoReg   USBC_UDINT;         /**< \brief (USBC Offset: 0x004) Device Global Interupt Register */
  WoReg   USBC_UDINTCLR;      /**< \brief (USBC Offset: 0x008) Device Global Interrupt Clear Register */
  WoReg   USBC_UDINTSET;      /**< \brief (USBC Offset: 0x00C) Device Global Interrupt Set Regsiter */
  RoReg   USBC_UDINTE;        /**< \brief (USBC Offset: 0x010) Device Global Interrupt Enable Register */
  WoReg   USBC_UDINTECLR;     /**< \brief (USBC Offset: 0x014) Device Global Interrupt Enable Clear Register */
  WoReg   USBC_UDINTESET;     /**< \brief (USBC Offset: 0x018) Device Global Interrupt Enable Set Register */
  RwReg   USBC_UERST;         /**< \brief (USBC Offset: 0x01C) Endpoint Enable/Reset Register */
  RoReg   USBC_UDFNUM;        /**< \brief (USBC Offset: 0x020) Device Frame Number Register */
  RoReg8  Reserved1[0xDC];
  RwReg   USBC_UECFG0;        /**< \brief (USBC Offset: 0x100) Endpoint Configuration Register */
  RwReg   USBC_UECFG1;        /**< \brief (USBC Offset: 0x104) Endpoint Configuration Register */
  RwReg   USBC_UECFG2;        /**< \brief (USBC Offset: 0x108) Endpoint Configuration Register */
  RwReg   USBC_UECFG3;        /**< \brief (USBC Offset: 0x10C) Endpoint Configuration Register */
  RwReg   USBC_UECFG4;        /**< \brief (USBC Offset: 0x110) Endpoint Configuration Register */
  RwReg   USBC_UECFG5;        /**< \brief (USBC Offset: 0x114) Endpoint Configuration Register */
  RwReg   USBC_UECFG6;        /**< \brief (USBC Offset: 0x118) Endpoint Configuration Register */
  RwReg   USBC_UECFG7;        /**< \brief (USBC Offset: 0x11C) Endpoint Configuration Register */
  RoReg8  Reserved2[0x10];
  RoReg   USBC_UESTA0;        /**< \brief (USBC Offset: 0x130) Endpoint Status Register */
  RoReg   USBC_UESTA1;        /**< \brief (USBC Offset: 0x134) Endpoint Status Register */
  RoReg   USBC_UESTA2;        /**< \brief (USBC Offset: 0x138) Endpoint Status Register */
  RoReg   USBC_UESTA3;        /**< \brief (USBC Offset: 0x13C) Endpoint Status Register */
  RoReg   USBC_UESTA4;        /**< \brief (USBC Offset: 0x140) Endpoint Status Register */
  RoReg   USBC_UESTA5;        /**< \brief (USBC Offset: 0x144) Endpoint Status Register */
  RoReg   USBC_UESTA6;        /**< \brief (USBC Offset: 0x148) Endpoint Status Register */
  RoReg   USBC_UESTA7;        /**< \brief (USBC Offset: 0x14C) Endpoint Status Register */
  RoReg8  Reserved3[0x10];
  WoReg   USBC_UESTA0CLR;     /**< \brief (USBC Offset: 0x160) Endpoint Status Clear Register */
  WoReg   USBC_UESTA1CLR;     /**< \brief (USBC Offset: 0x164) Endpoint Status Clear Register */
  WoReg   USBC_UESTA2CLR;     /**< \brief (USBC Offset: 0x168) Endpoint Status Clear Register */
  WoReg   USBC_UESTA3CLR;     /**< \brief (USBC Offset: 0x16C) Endpoint Status Clear Register */
  WoReg   USBC_UESTA4CLR;     /**< \brief (USBC Offset: 0x170) Endpoint Status Clear Register */
  WoReg   USBC_UESTA5CLR;     /**< \brief (USBC Offset: 0x174) Endpoint Status Clear Register */
  WoReg   USBC_UESTA6CLR;     /**< \brief (USBC Offset: 0x178) Endpoint Status Clear Register */
  WoReg   USBC_UESTA7CLR;     /**< \brief (USBC Offset: 0x17C) Endpoint Status Clear Register */
  RoReg8  Reserved4[0x10];
  WoReg   USBC_UESTA0SET;     /**< \brief (USBC Offset: 0x190) Endpoint Status Set Register */
  WoReg   USBC_UESTA1SET;     /**< \brief (USBC Offset: 0x194) Endpoint Status Set Register */
  WoReg   USBC_UESTA2SET;     /**< \brief (USBC Offset: 0x198) Endpoint Status Set Register */
  WoReg   USBC_UESTA3SET;     /**< \brief (USBC Offset: 0x19C) Endpoint Status Set Register */
  WoReg   USBC_UESTA4SET;     /**< \brief (USBC Offset: 0x1A0) Endpoint Status Set Register */
  WoReg   USBC_UESTA5SET;     /**< \brief (USBC Offset: 0x1A4) Endpoint Status Set Register */
  WoReg   USBC_UESTA6SET;     /**< \brief (USBC Offset: 0x1A8) Endpoint Status Set Register */
  WoReg   USBC_UESTA7SET;     /**< \brief (USBC Offset: 0x1AC) Endpoint Status Set Register */
  RoReg8  Reserved5[0x10];
  RoReg   USBC_UECON0;        /**< \brief (USBC Offset: 0x1C0) Endpoint Control Register */
  RoReg   USBC_UECON1;        /**< \brief (USBC Offset: 0x1C4) Endpoint Control Register */
  RoReg   USBC_UECON2;        /**< \brief (USBC Offset: 0x1C8) Endpoint Control Register */
  RoReg   USBC_UECON3;        /**< \brief (USBC Offset: 0x1CC) Endpoint Control Register */
  RoReg   USBC_UECON4;        /**< \brief (USBC Offset: 0x1D0) Endpoint Control Register */
  RoReg   USBC_UECON5;        /**< \brief (USBC Offset: 0x1D4) Endpoint Control Register */
  RoReg   USBC_UECON6;        /**< \brief (USBC Offset: 0x1D8) Endpoint Control Register */
  RoReg   USBC_UECON7;        /**< \brief (USBC Offset: 0x1DC) Endpoint Control Register */
  RoReg8  Reserved6[0x10];
  WoReg   USBC_UECON0SET;     /**< \brief (USBC Offset: 0x1F0) Endpoint Control Set Register */
  WoReg   USBC_UECON1SET;     /**< \brief (USBC Offset: 0x1F4) Endpoint Control Set Register */
  WoReg   USBC_UECON2SET;     /**< \brief (USBC Offset: 0x1F8) Endpoint Control Set Register */
  WoReg   USBC_UECON3SET;     /**< \brief (USBC Offset: 0x1FC) Endpoint Control Set Register */
  WoReg   USBC_UECON4SET;     /**< \brief (USBC Offset: 0x200) Endpoint Control Set Register */
  WoReg   USBC_UECON5SET;     /**< \brief (USBC Offset: 0x204) Endpoint Control Set Register */
  WoReg   USBC_UECON6SET;     /**< \brief (USBC Offset: 0x208) Endpoint Control Set Register */
  WoReg   USBC_UECON7SET;     /**< \brief (USBC Offset: 0x20C) Endpoint Control Set Register */
  RoReg8  Reserved7[0x10];
  WoReg   USBC_UECON0CLR;     /**< \brief (USBC Offset: 0x220) Endpoint Control Clear Register */
  WoReg   USBC_UECON1CLR;     /**< \brief (USBC Offset: 0x224) TXINE Clear */
  WoReg   USBC_UECON2CLR;     /**< \brief (USBC Offset: 0x228) TXINE Clear */
  WoReg   USBC_UECON3CLR;     /**< \brief (USBC Offset: 0x22C) TXINE Clear */
  WoReg   USBC_UECON4CLR;     /**< \brief (USBC Offset: 0x230) TXINE Clear */
  WoReg   USBC_UECON5CLR;     /**< \brief (USBC Offset: 0x234) TXINE Clear */
  WoReg   USBC_UECON6CLR;     /**< \brief (USBC Offset: 0x238) TXINE Clear */
  WoReg   USBC_UECON7CLR;     /**< \brief (USBC Offset: 0x23C) TXINE Clear */
  RoReg8  Reserved8[0x1C0];
  RwReg   USBC_UHCON;         /**< \brief (USBC Offset: 0x400) Host General Control Register */
  RoReg   USBC_UHINT;         /**< \brief (USBC Offset: 0x404) Host Global Interrupt Register */
  WoReg   USBC_UHINTCLR;      /**< \brief (USBC Offset: 0x408) Host Global Interrrupt Clear Register */
  WoReg   USBC_UHINTSET;      /**< \brief (USBC Offset: 0x40C) Host Global Interrupt Set Register */
  RoReg   USBC_UHINTE;        /**< \brief (USBC Offset: 0x410) Host Global Interrupt Enable Register */
  WoReg   USBC_UHINTECLR;     /**< \brief (USBC Offset: 0x414) Host Global Interrupt Enable Clear Register */
  WoReg   USBC_UHINTESET;     /**< \brief (USBC Offset: 0x418) Host Global Interrupt Enable Set Register */
  RwReg   USBC_UPRST;         /**< \brief (USBC Offset: 0x41C) Pipe Reset Register */
  RwReg   USBC_UHFNUM;        /**< \brief (USBC Offset: 0x420) Host Frame Number Register */
  RwReg   USBC_UHSOFC;        /**< \brief (USBC Offset: 0x424) Host Start of Frame Control Register */
  RoReg8  Reserved9[0xD8];
  RwReg   USBC_UPCFG0;        /**< \brief (USBC Offset: 0x500) Pipe Configuration Register */
  RwReg   USBC_UPCFG1;        /**< \brief (USBC Offset: 0x504) Pipe Configuration Register */
  RwReg   USBC_UPCFG2;        /**< \brief (USBC Offset: 0x508) Pipe Configuration Register */
  RwReg   USBC_UPCFG3;        /**< \brief (USBC Offset: 0x50C) Pipe Configuration Register */
  RwReg   USBC_UPCFG4;        /**< \brief (USBC Offset: 0x510) Pipe Configuration Register */
  RwReg   USBC_UPCFG5;        /**< \brief (USBC Offset: 0x514) Pipe Configuration Register */
  RwReg   USBC_UPCFG6;        /**< \brief (USBC Offset: 0x518) Pipe Configuration Register */
  RwReg   USBC_UPCFG7;        /**< \brief (USBC Offset: 0x51C) Pipe Configuration Register */
  RoReg8  Reserved10[0x10];
  RoReg   USBC_UPSTA0;        /**< \brief (USBC Offset: 0x530) Pipe Status Register */
  RoReg   USBC_UPSTA1;        /**< \brief (USBC Offset: 0x534) Pipe Status Register */
  RoReg   USBC_UPSTA2;        /**< \brief (USBC Offset: 0x538) Pipe Status Register */
  RoReg   USBC_UPSTA3;        /**< \brief (USBC Offset: 0x53C) Pipe Status Register */
  RoReg   USBC_UPSTA4;        /**< \brief (USBC Offset: 0x540) Pipe Status Register */
  RoReg   USBC_UPSTA5;        /**< \brief (USBC Offset: 0x544) Pipe Status Register */
  RoReg   USBC_UPSTA6;        /**< \brief (USBC Offset: 0x548) Pipe Status Register */
  RoReg   USBC_UPSTA7;        /**< \brief (USBC Offset: 0x54C) Pipe Status Register */
  RoReg8  Reserved11[0x10];
  WoReg   USBC_UPSTA0CLR;     /**< \brief (USBC Offset: 0x560) Pipe Status Clear Register */
  WoReg   USBC_UPSTA1CLR;     /**< \brief (USBC Offset: 0x564) Pipe Status Clear Register */
  WoReg   USBC_UPSTA2CLR;     /**< \brief (USBC Offset: 0x568) Pipe Status Clear Register */
  WoReg   USBC_UPSTA3CLR;     /**< \brief (USBC Offset: 0x56C) Pipe Status Clear Register */
  WoReg   USBC_UPSTA4CLR;     /**< \brief (USBC Offset: 0x570) Pipe Status Clear Register */
  WoReg   USBC_UPSTA5CLR;     /**< \brief (USBC Offset: 0x574) Pipe Status Clear Register */
  WoReg   USBC_UPSTA6CLR;     /**< \brief (USBC Offset: 0x578) Pipe Status Clear Register */
  WoReg   USBC_UPSTA7CLR;     /**< \brief (USBC Offset: 0x57C) Pipe Status Clear Register */
  RoReg8  Reserved12[0x10];
  WoReg   USBC_UPSTA0SET;     /**< \brief (USBC Offset: 0x590) Pipe Status Set Register */
  WoReg   USBC_UPSTA1SET;     /**< \brief (USBC Offset: 0x594) Pipe Status Set Register */
  WoReg   USBC_UPSTA2SET;     /**< \brief (USBC Offset: 0x598) Pipe Status Set Register */
  WoReg   USBC_UPSTA3SET;     /**< \brief (USBC Offset: 0x59C) Pipe Status Set Register */
  WoReg   USBC_UPSTA4SET;     /**< \brief (USBC Offset: 0x5A0) Pipe Status Set Register */
  WoReg   USBC_UPSTA5SET;     /**< \brief (USBC Offset: 0x5A4) Pipe Status Set Register */
  WoReg   USBC_UPSTA6SET;     /**< \brief (USBC Offset: 0x5A8) Pipe Status Set Register */
  WoReg   USBC_UPSTA7SET;     /**< \brief (USBC Offset: 0x5AC) Pipe Status Set Register */
  RoReg8  Reserved13[0x10];
  RoReg   USBC_UPCON0;        /**< \brief (USBC Offset: 0x5C0) Pipe Control Register */
  RoReg   USBC_UPCON1;        /**< \brief (USBC Offset: 0x5C4) Pipe Control Register */
  RoReg   USBC_UPCON2;        /**< \brief (USBC Offset: 0x5C8) Pipe Control Register */
  RoReg   USBC_UPCON3;        /**< \brief (USBC Offset: 0x5CC) Pipe Control Register */
  RoReg   USBC_UPCON4;        /**< \brief (USBC Offset: 0x5D0) Pipe Control Register */
  RoReg   USBC_UPCON5;        /**< \brief (USBC Offset: 0x5D4) Pipe Control Register */
  RoReg   USBC_UPCON6;        /**< \brief (USBC Offset: 0x5D8) Pipe Control Register */
  RoReg   USBC_UPCON7;        /**< \brief (USBC Offset: 0x5DC) Pipe Control Register */
  RoReg8  Reserved14[0x10];
  WoReg   USBC_UPCON0SET;     /**< \brief (USBC Offset: 0x5F0) Pipe Control Set Register */
  WoReg   USBC_UPCON1SET;     /**< \brief (USBC Offset: 0x5F4) Pipe Control Set Register */
  WoReg   USBC_UPCON2SET;     /**< \brief (USBC Offset: 0x5F8) Pipe Control Set Register */
  WoReg   USBC_UPCON3SET;     /**< \brief (USBC Offset: 0x5FC) Pipe Control Set Register */
  WoReg   USBC_UPCON4SET;     /**< \brief (USBC Offset: 0x600) Pipe Control Set Register */
  WoReg   USBC_UPCON5SET;     /**< \brief (USBC Offset: 0x604) Pipe Control Set Register */
  WoReg   USBC_UPCON6SET;     /**< \brief (USBC Offset: 0x608) Pipe Control Set Register */
  WoReg   USBC_UPCON7SET;     /**< \brief (USBC Offset: 0x60C) Pipe Control Set Register */
  RoReg8  Reserved15[0x10];
  WoReg   USBC_UPCON0CLR;     /**< \brief (USBC Offset: 0x620) Pipe Control Clear Register */
  WoReg   USBC_UPCON1CLR;     /**< \brief (USBC Offset: 0x624) Pipe Control Clear Register */
  WoReg   USBC_UPCON2CLR;     /**< \brief (USBC Offset: 0x628) Pipe Control Clear Register */
  WoReg   USBC_UPCON3CLR;     /**< \brief (USBC Offset: 0x62C) Pipe Control Clear Register */
  WoReg   USBC_UPCON4CLR;     /**< \brief (USBC Offset: 0x630) Pipe Control Clear Register */
  WoReg   USBC_UPCON5CLR;     /**< \brief (USBC Offset: 0x634) Pipe Control Clear Register */
  WoReg   USBC_UPCON6CLR;     /**< \brief (USBC Offset: 0x638) Pipe Control Clear Register */
  WoReg   USBC_UPCON7CLR;     /**< \brief (USBC Offset: 0x63C) Pipe Control Clear Register */
  RoReg8  Reserved16[0x10];
  RwReg   USBC_UPINRQ0;       /**< \brief (USBC Offset: 0x650) Pipe In Request */
  RwReg   USBC_UPINRQ1;       /**< \brief (USBC Offset: 0x654) Pipe In Request */
  RwReg   USBC_UPINRQ2;       /**< \brief (USBC Offset: 0x658) Pipe In Request */
  RwReg   USBC_UPINRQ3;       /**< \brief (USBC Offset: 0x65C) Pipe In Request */
  RwReg   USBC_UPINRQ4;       /**< \brief (USBC Offset: 0x660) Pipe In Request */
  RwReg   USBC_UPINRQ5;       /**< \brief (USBC Offset: 0x664) Pipe In Request */
  RwReg   USBC_UPINRQ6;       /**< \brief (USBC Offset: 0x668) Pipe In Request */
  RwReg   USBC_UPINRQ7;       /**< \brief (USBC Offset: 0x66C) Pipe In Request */
  RoReg8  Reserved17[0x190];
  RwReg   USBC_USBCON;        /**< \brief (USBC Offset: 0x800) General Control Register */
  RoReg   USBC_USBSTA;        /**< \brief (USBC Offset: 0x804) General Status Register */
  WoReg   USBC_USBSTACLR;     /**< \brief (USBC Offset: 0x808) General Status Clear Register */
  WoReg   USBC_USBSTASET;     /**< \brief (USBC Offset: 0x80C) General Status Set Register */
  RoReg8  Reserved18[0x8];
  RoReg   USBC_UVERS;         /**< \brief (USBC Offset: 0x818) IP Version Register */
  RoReg   USBC_UFEATURES;     /**< \brief (USBC Offset: 0x81C) IP Features Register */
  RoReg   USBC_UADDRSIZE;     /**< \brief (USBC Offset: 0x820) IP PB address size Register */
  RoReg   USBC_UNAME1;        /**< \brief (USBC Offset: 0x824) IP Name Part One: HUSB */
  RoReg   USBC_UNAME2;        /**< \brief (USBC Offset: 0x828) IP Name Part Two: HOST */
  RoReg   USBC_USBFSM;        /**< \brief (USBC Offset: 0x82C) USB internal finite state machine */
  RwReg   USBC_UDESC;         /**< \brief (USBC Offset: 0x830) Endpoint descriptor table */
} Usbc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_USBC_COMPONENT_ */
