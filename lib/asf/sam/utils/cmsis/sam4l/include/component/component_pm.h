/**
 * \file
 *
 * \brief Component description for PM
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

#ifndef _SAM4L_PM_COMPONENT_
#define _SAM4L_PM_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR PM */
/* ========================================================================== */
/** \addtogroup SAM4L_PM Power Manager */
/*@{*/

#define REV_PM                      0x441

/* -------- PM_MCCTRL : (PM Offset: 0x000) (R/W 32) Main Clock Control -------- */
#define PM_MCCTRL_OFFSET            0x000        /**< \brief (PM_MCCTRL offset) Main Clock Control */
#define PM_MCCTRL_RESETVALUE        0x00000000   /**< \brief (PM_MCCTRL reset_value) Main Clock Control */

#define PM_MCCTRL_MCSEL_Pos         0            /**< \brief (PM_MCCTRL) Main Clock Select */
#define PM_MCCTRL_MCSEL_Msk         (0x7u << PM_MCCTRL_MCSEL_Pos)
#define PM_MCCTRL_MCSEL(value)      ((PM_MCCTRL_MCSEL_Msk & ((value) << PM_MCCTRL_MCSEL_Pos)))
#define PM_MCCTRL_MASK              0x00000007u  /**< \brief (PM_MCCTRL) MASK Register */

/* -------- PM_CPUSEL : (PM Offset: 0x004) (R/W 32) CPU Clock Select -------- */
#define PM_CPUSEL_OFFSET            0x004        /**< \brief (PM_CPUSEL offset) CPU Clock Select */
#define PM_CPUSEL_RESETVALUE        0x00000000   /**< \brief (PM_CPUSEL reset_value) CPU Clock Select */

#define PM_CPUSEL_CPUSEL_Pos        0            /**< \brief (PM_CPUSEL) CPU Clock Select */
#define PM_CPUSEL_CPUSEL_Msk        (0x7u << PM_CPUSEL_CPUSEL_Pos)
#define PM_CPUSEL_CPUSEL(value)     ((PM_CPUSEL_CPUSEL_Msk & ((value) << PM_CPUSEL_CPUSEL_Pos)))
#define   PM_CPUSEL_CPUSEL_0        (0x0u <<  0) /**< \brief (PM_CPUSEL) fCPU:fmain. CPUDIV: */
#define   PM_CPUSEL_CPUSEL_1        (0x1u <<  0) /**< \brief (PM_CPUSEL) fCPU:fmain / 2^(CPUSEL+1) */
#define PM_CPUSEL_CPUDIV_Pos        7            /**< \brief (PM_CPUSEL) CPU Division */
#define PM_CPUSEL_CPUDIV            (0x1u << PM_CPUSEL_CPUDIV_Pos)
#define PM_CPUSEL_MASK              0x00000087u  /**< \brief (PM_CPUSEL) MASK Register */

/* -------- PM_PBASEL : (PM Offset: 0x00C) (R/W 32) PBA Clock Select -------- */
#define PM_PBASEL_OFFSET            0x00C        /**< \brief (PM_PBASEL offset) PBA Clock Select */
#define PM_PBASEL_RESETVALUE        0x00000000   /**< \brief (PM_PBASEL reset_value) PBA Clock Select */

#define PM_PBASEL_PBSEL_Pos         0            /**< \brief (PM_PBASEL) PBA Clock Select */
#define PM_PBASEL_PBSEL_Msk         (0x7u << PM_PBASEL_PBSEL_Pos)
#define PM_PBASEL_PBSEL(value)      ((PM_PBASEL_PBSEL_Msk & ((value) << PM_PBASEL_PBSEL_Pos)))
#define PM_PBASEL_PBDIV_Pos         7            /**< \brief (PM_PBASEL) PBA Division Select */
#define PM_PBASEL_PBDIV             (0x1u << PM_PBASEL_PBDIV_Pos)
#define PM_PBASEL_MASK              0x00000087u  /**< \brief (PM_PBASEL) MASK Register */

/* -------- PM_PBBSEL : (PM Offset: 0x010) (R/W 32) PBB Clock Select -------- */
#define PM_PBBSEL_OFFSET            0x010        /**< \brief (PM_PBBSEL offset) PBB Clock Select */
#define PM_PBBSEL_RESETVALUE        0x00000000   /**< \brief (PM_PBBSEL reset_value) PBB Clock Select */

#define PM_PBBSEL_PBSEL_Pos         0            /**< \brief (PM_PBBSEL) PBB Clock Select */
#define PM_PBBSEL_PBSEL_Msk         (0x7u << PM_PBBSEL_PBSEL_Pos)
#define PM_PBBSEL_PBSEL(value)      ((PM_PBBSEL_PBSEL_Msk & ((value) << PM_PBBSEL_PBSEL_Pos)))
#define PM_PBBSEL_PBDIV_Pos         7            /**< \brief (PM_PBBSEL) PBB Division Select */
#define PM_PBBSEL_PBDIV             (0x1u << PM_PBBSEL_PBDIV_Pos)
#define PM_PBBSEL_MASK              0x00000087u  /**< \brief (PM_PBBSEL) MASK Register */

/* -------- PM_PBCSEL : (PM Offset: 0x014) (R/W 32) PBC Clock Select -------- */
#define PM_PBCSEL_OFFSET            0x014        /**< \brief (PM_PBCSEL offset) PBC Clock Select */
#define PM_PBCSEL_RESETVALUE        0x00000000   /**< \brief (PM_PBCSEL reset_value) PBC Clock Select */

#define PM_PBCSEL_PBSEL_Pos         0            /**< \brief (PM_PBCSEL) PBC Clock Select */
#define PM_PBCSEL_PBSEL_Msk         (0x7u << PM_PBCSEL_PBSEL_Pos)
#define PM_PBCSEL_PBSEL(value)      ((PM_PBCSEL_PBSEL_Msk & ((value) << PM_PBCSEL_PBSEL_Pos)))
#define PM_PBCSEL_PBDIV_Pos         7            /**< \brief (PM_PBCSEL) PBC Division Select */
#define PM_PBCSEL_PBDIV             (0x1u << PM_PBCSEL_PBDIV_Pos)
#define PM_PBCSEL_MASK              0x00000087u  /**< \brief (PM_PBCSEL) MASK Register */

/* -------- PM_PBDSEL : (PM Offset: 0x018) (R/W 32) PBD Clock Select -------- */
#define PM_PBDSEL_OFFSET            0x018        /**< \brief (PM_PBDSEL offset) PBD Clock Select */
#define PM_PBDSEL_RESETVALUE        0x00000000   /**< \brief (PM_PBDSEL reset_value) PBD Clock Select */

#define PM_PBDSEL_PBSEL_Pos         0            /**< \brief (PM_PBDSEL) PBD Clock Select */
#define PM_PBDSEL_PBSEL_Msk         (0x7u << PM_PBDSEL_PBSEL_Pos)
#define PM_PBDSEL_PBSEL(value)      ((PM_PBDSEL_PBSEL_Msk & ((value) << PM_PBDSEL_PBSEL_Pos)))
#define PM_PBDSEL_PBDIV_Pos         7            /**< \brief (PM_PBDSEL) PBD Division Select */
#define PM_PBDSEL_PBDIV             (0x1u << PM_PBDSEL_PBDIV_Pos)
#define PM_PBDSEL_MASK              0x00000087u  /**< \brief (PM_PBDSEL) MASK Register */

/* -------- PM_CPUMASK : (PM Offset: 0x020) (R/W 32) CPU Mask -------- */
#define PM_CPUMASK_OFFSET           0x020        /**< \brief (PM_CPUMASK offset) CPU Mask */
#define PM_CPUMASK_RESETVALUE       0x00000001   /**< \brief (PM_CPUMASK reset_value) CPU Mask */

#define PM_CPUMASK_OCD_Pos          0            /**< \brief (PM_CPUMASK) OCD CPU Clock Mask */
#define PM_CPUMASK_OCD              (0x1u << PM_CPUMASK_OCD_Pos)
#define PM_CPUMASK_MASK             0x00000001u  /**< \brief (PM_CPUMASK) MASK Register */

/* -------- PM_HSBMASK : (PM Offset: 0x024) (R/W 32) HSB Mask -------- */
#define PM_HSBMASK_OFFSET           0x024        /**< \brief (PM_HSBMASK offset) HSB Mask */
#define PM_HSBMASK_RESETVALUE       0x000001E2   /**< \brief (PM_HSBMASK reset_value) HSB Mask */

#define PM_HSBMASK_PDCA_Pos         0            /**< \brief (PM_HSBMASK) PDCA HSB Clock Mask */
#define PM_HSBMASK_PDCA             (0x1u << PM_HSBMASK_PDCA_Pos)
#define PM_HSBMASK_HFLASHC_Pos      1            /**< \brief (PM_HSBMASK) HFLASHC HSB Clock Mask */
#define PM_HSBMASK_HFLASHC          (0x1u << PM_HSBMASK_HFLASHC_Pos)
#define PM_HSBMASK_HRAMC1_Pos       2            /**< \brief (PM_HSBMASK) HRAMC1 HSB Clock Mask */
#define PM_HSBMASK_HRAMC1           (0x1u << PM_HSBMASK_HRAMC1_Pos)
#define PM_HSBMASK_USBC_Pos         3            /**< \brief (PM_HSBMASK) USBC HSB Clock Mask */
#define PM_HSBMASK_USBC             (0x1u << PM_HSBMASK_USBC_Pos)
#define PM_HSBMASK_CRCCU_Pos        4            /**< \brief (PM_HSBMASK) CRCCU HSB Clock Mask */
#define PM_HSBMASK_CRCCU            (0x1u << PM_HSBMASK_CRCCU_Pos)
#define PM_HSBMASK_HTOP0_Pos        5            /**< \brief (PM_HSBMASK) HTOP0 HSB Clock Mask */
#define PM_HSBMASK_HTOP0            (0x1u << PM_HSBMASK_HTOP0_Pos)
#define PM_HSBMASK_HTOP1_Pos        6            /**< \brief (PM_HSBMASK) HTOP1 HSB Clock Mask */
#define PM_HSBMASK_HTOP1            (0x1u << PM_HSBMASK_HTOP1_Pos)
#define PM_HSBMASK_HTOP2_Pos        7            /**< \brief (PM_HSBMASK) HTOP2 HSB Clock Mask */
#define PM_HSBMASK_HTOP2            (0x1u << PM_HSBMASK_HTOP2_Pos)
#define PM_HSBMASK_HTOP3_Pos        8            /**< \brief (PM_HSBMASK) HTOP3 HSB Clock Mask */
#define PM_HSBMASK_HTOP3            (0x1u << PM_HSBMASK_HTOP3_Pos)
#define PM_HSBMASK_AESA_Pos         9            /**< \brief (PM_HSBMASK) AESA HSB Clock Mask */
#define PM_HSBMASK_AESA             (0x1u << PM_HSBMASK_AESA_Pos)
#define PM_HSBMASK_MASK             0x000003FFu  /**< \brief (PM_HSBMASK) MASK Register */

/* -------- PM_PBAMASK : (PM Offset: 0x028) (R/W 32) PBA Mask -------- */
#define PM_PBAMASK_OFFSET           0x028        /**< \brief (PM_PBAMASK offset) PBA Mask */
#define PM_PBAMASK_RESETVALUE       0x00000000   /**< \brief (PM_PBAMASK reset_value) PBA Mask */

#define PM_PBAMASK_IISC_Pos         0            /**< \brief (PM_PBAMASK) IISC PB Clock Mask */
#define PM_PBAMASK_IISC             (0x1u << PM_PBAMASK_IISC_Pos)
#define PM_PBAMASK_SPI_Pos          1            /**< \brief (PM_PBAMASK) SPI PB Clock Mask */
#define PM_PBAMASK_SPI              (0x1u << PM_PBAMASK_SPI_Pos)
#define PM_PBAMASK_TC0_Pos          2            /**< \brief (PM_PBAMASK) TC0 PB Clock Mask */
#define PM_PBAMASK_TC0              (0x1u << PM_PBAMASK_TC0_Pos)
#define PM_PBAMASK_TC1_Pos          3            /**< \brief (PM_PBAMASK) TC1 PB Clock Mask */
#define PM_PBAMASK_TC1              (0x1u << PM_PBAMASK_TC1_Pos)
#define PM_PBAMASK_TWIM0_Pos        4            /**< \brief (PM_PBAMASK) TWIM0 PB Clock Mask */
#define PM_PBAMASK_TWIM0            (0x1u << PM_PBAMASK_TWIM0_Pos)
#define PM_PBAMASK_TWIS0_Pos        5            /**< \brief (PM_PBAMASK) TWIS0 PB Clock Mask */
#define PM_PBAMASK_TWIS0            (0x1u << PM_PBAMASK_TWIS0_Pos)
#define PM_PBAMASK_TWIM1_Pos        6            /**< \brief (PM_PBAMASK) TWIM1 PB Clock Mask */
#define PM_PBAMASK_TWIM1            (0x1u << PM_PBAMASK_TWIM1_Pos)
#define PM_PBAMASK_TWIS1_Pos        7            /**< \brief (PM_PBAMASK) TWIS1 PB Clock Mask */
#define PM_PBAMASK_TWIS1            (0x1u << PM_PBAMASK_TWIS1_Pos)
#define PM_PBAMASK_USART0_Pos       8            /**< \brief (PM_PBAMASK) USART0 PB Clock Mask */
#define PM_PBAMASK_USART0           (0x1u << PM_PBAMASK_USART0_Pos)
#define PM_PBAMASK_USART1_Pos       9            /**< \brief (PM_PBAMASK) USART1 PB Clock Mask */
#define PM_PBAMASK_USART1           (0x1u << PM_PBAMASK_USART1_Pos)
#define PM_PBAMASK_USART2_Pos       10           /**< \brief (PM_PBAMASK) USART2 PB Clock Mask */
#define PM_PBAMASK_USART2           (0x1u << PM_PBAMASK_USART2_Pos)
#define PM_PBAMASK_USART3_Pos       11           /**< \brief (PM_PBAMASK) USART3 PB Clock Mask */
#define PM_PBAMASK_USART3           (0x1u << PM_PBAMASK_USART3_Pos)
#define PM_PBAMASK_ADCIFE_Pos       12           /**< \brief (PM_PBAMASK) ADCIFE PB Clock Mask */
#define PM_PBAMASK_ADCIFE           (0x1u << PM_PBAMASK_ADCIFE_Pos)
#define PM_PBAMASK_DACC_Pos         13           /**< \brief (PM_PBAMASK) DACC PB Clock Mask */
#define PM_PBAMASK_DACC             (0x1u << PM_PBAMASK_DACC_Pos)
#define PM_PBAMASK_ACIFC_Pos        14           /**< \brief (PM_PBAMASK) ACIFC PB Clock Mask */
#define PM_PBAMASK_ACIFC            (0x1u << PM_PBAMASK_ACIFC_Pos)
#define PM_PBAMASK_GLOC_Pos         15           /**< \brief (PM_PBAMASK) GLOC PB Clock Mask */
#define PM_PBAMASK_GLOC             (0x1u << PM_PBAMASK_GLOC_Pos)
#define PM_PBAMASK_ABDACB_Pos       16           /**< \brief (PM_PBAMASK) ABDACB PB Clock Mask */
#define PM_PBAMASK_ABDACB           (0x1u << PM_PBAMASK_ABDACB_Pos)
#define PM_PBAMASK_TRNG_Pos         17           /**< \brief (PM_PBAMASK) TRNG PB Clock Mask */
#define PM_PBAMASK_TRNG             (0x1u << PM_PBAMASK_TRNG_Pos)
#define PM_PBAMASK_PARC_Pos         18           /**< \brief (PM_PBAMASK) PARC PB Clock Mask */
#define PM_PBAMASK_PARC             (0x1u << PM_PBAMASK_PARC_Pos)
#define PM_PBAMASK_CATB_Pos         19           /**< \brief (PM_PBAMASK) CATB PB Clock Mask */
#define PM_PBAMASK_CATB             (0x1u << PM_PBAMASK_CATB_Pos)
#define PM_PBAMASK_TWIM2_Pos        21           /**< \brief (PM_PBAMASK) TWIM2 PB Clock Mask */
#define PM_PBAMASK_TWIM2            (0x1u << PM_PBAMASK_TWIM2_Pos)
#define PM_PBAMASK_TWIM3_Pos        22           /**< \brief (PM_PBAMASK) TWIM3 PB Clock Mask */
#define PM_PBAMASK_TWIM3            (0x1u << PM_PBAMASK_TWIM3_Pos)
#define PM_PBAMASK_LCDCA_Pos        23           /**< \brief (PM_PBAMASK) LCDCA PB Clock Mask */
#define PM_PBAMASK_LCDCA            (0x1u << PM_PBAMASK_LCDCA_Pos)
#define PM_PBAMASK_MASK             0x00EFFFFFu  /**< \brief (PM_PBAMASK) MASK Register */

/* -------- PM_PBBMASK : (PM Offset: 0x02C) (R/W 32) PBB Mask -------- */
#define PM_PBBMASK_OFFSET           0x02C        /**< \brief (PM_PBBMASK offset) PBB Mask */
#define PM_PBBMASK_RESETVALUE       0x00000001   /**< \brief (PM_PBBMASK reset_value) PBB Mask */

#define PM_PBBMASK_HFLASHC_Pos      0            /**< \brief (PM_PBBMASK) HFLASHC PB Clock Mask */
#define PM_PBBMASK_HFLASHC          (0x1u << PM_PBBMASK_HFLASHC_Pos)
#define PM_PBBMASK_HCACHE_Pos       1            /**< \brief (PM_PBBMASK) HCACHE PB Clock Mask */
#define PM_PBBMASK_HCACHE           (0x1u << PM_PBBMASK_HCACHE_Pos)
#define PM_PBBMASK_HMATRIX_Pos      2            /**< \brief (PM_PBBMASK) HMATRIX PB Clock Mask */
#define PM_PBBMASK_HMATRIX          (0x1u << PM_PBBMASK_HMATRIX_Pos)
#define PM_PBBMASK_PDCA_Pos         3            /**< \brief (PM_PBBMASK) PDCA PB Clock Mask */
#define PM_PBBMASK_PDCA             (0x1u << PM_PBBMASK_PDCA_Pos)
#define PM_PBBMASK_CRCCU_Pos        4            /**< \brief (PM_PBBMASK) CRCCU PB Clock Mask */
#define PM_PBBMASK_CRCCU            (0x1u << PM_PBBMASK_CRCCU_Pos)
#define PM_PBBMASK_USBC_Pos         5            /**< \brief (PM_PBBMASK) USBC PB Clock Mask */
#define PM_PBBMASK_USBC             (0x1u << PM_PBBMASK_USBC_Pos)
#define PM_PBBMASK_PEVC_Pos         6            /**< \brief (PM_PBBMASK) PEVC PB Clock Mask */
#define PM_PBBMASK_PEVC             (0x1u << PM_PBBMASK_PEVC_Pos)
#define PM_PBBMASK_MASK             0x0000007Fu  /**< \brief (PM_PBBMASK) MASK Register */

/* -------- PM_PBCMASK : (PM Offset: 0x030) (R/W 32) PBC Mask -------- */
#define PM_PBCMASK_OFFSET           0x030        /**< \brief (PM_PBCMASK offset) PBC Mask */
#define PM_PBCMASK_RESETVALUE       0x0000001F   /**< \brief (PM_PBCMASK reset_value) PBC Mask */

#define PM_PBCMASK_PM_Pos           0            /**< \brief (PM_PBCMASK) PM PB Clock Mask */
#define PM_PBCMASK_PM               (0x1u << PM_PBCMASK_PM_Pos)
#define PM_PBCMASK_CHIPID_Pos       1            /**< \brief (PM_PBCMASK) CHIPID PB Clock Mask */
#define PM_PBCMASK_CHIPID           (0x1u << PM_PBCMASK_CHIPID_Pos)
#define PM_PBCMASK_SCIF_Pos         2            /**< \brief (PM_PBCMASK) SCIF PB Clock Mask */
#define PM_PBCMASK_SCIF             (0x1u << PM_PBCMASK_SCIF_Pos)
#define PM_PBCMASK_FREQM_Pos        3            /**< \brief (PM_PBCMASK) FREQM PB Clock Mask */
#define PM_PBCMASK_FREQM            (0x1u << PM_PBCMASK_FREQM_Pos)
#define PM_PBCMASK_GPIO_Pos         4            /**< \brief (PM_PBCMASK) GPIO PB Clock Mask */
#define PM_PBCMASK_GPIO             (0x1u << PM_PBCMASK_GPIO_Pos)
#define PM_PBCMASK_MASK             0x0000001Fu  /**< \brief (PM_PBCMASK) MASK Register */

/* -------- PM_PBDMASK : (PM Offset: 0x034) (R/W 32) PBD Mask -------- */
#define PM_PBDMASK_OFFSET           0x034        /**< \brief (PM_PBDMASK offset) PBD Mask */
#define PM_PBDMASK_RESETVALUE       0x0000003F   /**< \brief (PM_PBDMASK reset_value) PBD Mask */

#define PM_PBDMASK_BPM_Pos          0            /**< \brief (PM_PBDMASK) BPM PB Clock Mask */
#define PM_PBDMASK_BPM              (0x1u << PM_PBDMASK_BPM_Pos)
#define PM_PBDMASK_BSCIF_Pos        1            /**< \brief (PM_PBDMASK) BSCIF PB Clock Mask */
#define PM_PBDMASK_BSCIF            (0x1u << PM_PBDMASK_BSCIF_Pos)
#define PM_PBDMASK_AST_Pos          2            /**< \brief (PM_PBDMASK) AST PB Clock Mask */
#define PM_PBDMASK_AST              (0x1u << PM_PBDMASK_AST_Pos)
#define PM_PBDMASK_WDT_Pos          3            /**< \brief (PM_PBDMASK) WDT PB Clock Mask */
#define PM_PBDMASK_WDT              (0x1u << PM_PBDMASK_WDT_Pos)
#define PM_PBDMASK_EIC_Pos          4            /**< \brief (PM_PBDMASK) EIC PB Clock Mask */
#define PM_PBDMASK_EIC              (0x1u << PM_PBDMASK_EIC_Pos)
#define PM_PBDMASK_PICOUART_Pos     5            /**< \brief (PM_PBDMASK) PICOUART PB Clock Mask */
#define PM_PBDMASK_PICOUART         (0x1u << PM_PBDMASK_PICOUART_Pos)
#define PM_PBDMASK_MASK             0x0000003Fu  /**< \brief (PM_PBDMASK) MASK Register */

/* -------- PM_PBADIVMASK : (PM Offset: 0x040) (R/W 32) PBA Divided Clock Mask -------- */
#define PM_PBADIVMASK_OFFSET        0x040        /**< \brief (PM_PBADIVMASK offset) PBA Divided Clock Mask */
#define PM_PBADIVMASK_RESETVALUE    0x00000000   /**< \brief (PM_PBADIVMASK reset_value) PBA Divided Clock Mask */
#define PM_PBADIVMASK_MASK          0xFFFFFFFFu  /**< \brief (PM_PBADIVMASK) MASK Register */

/* -------- PM_CFDCTRL : (PM Offset: 0x054) (R/W 32) Clock Failure Detector Control -------- */
#define PM_CFDCTRL_OFFSET           0x054        /**< \brief (PM_CFDCTRL offset) Clock Failure Detector Control */
#define PM_CFDCTRL_RESETVALUE       0x00000000   /**< \brief (PM_CFDCTRL reset_value) Clock Failure Detector Control */

#define PM_CFDCTRL_CFDEN_Pos        0            /**< \brief (PM_CFDCTRL) Clock Failure Detection Enable */
#define PM_CFDCTRL_CFDEN            (0x1u << PM_CFDCTRL_CFDEN_Pos)
#define PM_CFDCTRL_SFV_Pos          31           /**< \brief (PM_CFDCTRL) Store Final Value */
#define PM_CFDCTRL_SFV              (0x1u << PM_CFDCTRL_SFV_Pos)
#define PM_CFDCTRL_MASK             0x80000001u  /**< \brief (PM_CFDCTRL) MASK Register */

/* -------- PM_UNLOCK : (PM Offset: 0x058) ( /W 32) Unlock Register -------- */
#define PM_UNLOCK_OFFSET            0x058        /**< \brief (PM_UNLOCK offset) Unlock Register */
#define PM_UNLOCK_RESETVALUE        0x00000000   /**< \brief (PM_UNLOCK reset_value) Unlock Register */

#define PM_UNLOCK_ADDR_Pos          0            /**< \brief (PM_UNLOCK) Unlock Address */
#define PM_UNLOCK_ADDR_Msk          (0x3FFu << PM_UNLOCK_ADDR_Pos)
#define PM_UNLOCK_ADDR(value)       ((PM_UNLOCK_ADDR_Msk & ((value) << PM_UNLOCK_ADDR_Pos)))
#define PM_UNLOCK_KEY_Pos           24           /**< \brief (PM_UNLOCK) Unlock Key */
#define PM_UNLOCK_KEY_Msk           (0xFFu << PM_UNLOCK_KEY_Pos)
#define PM_UNLOCK_KEY(value)        ((PM_UNLOCK_KEY_Msk & ((value) << PM_UNLOCK_KEY_Pos)))
#define PM_UNLOCK_MASK              0xFF0003FFu  /**< \brief (PM_UNLOCK) MASK Register */

/* -------- PM_IER : (PM Offset: 0x0C0) ( /W 32) Interrupt Enable Register -------- */
#define PM_IER_OFFSET               0x0C0        /**< \brief (PM_IER offset) Interrupt Enable Register */
#define PM_IER_RESETVALUE           0x00000000   /**< \brief (PM_IER reset_value) Interrupt Enable Register */

#define PM_IER_CFD_Pos              0            /**< \brief (PM_IER) Clock Failure Detected Interrupt Enable */
#define PM_IER_CFD                  (0x1u << PM_IER_CFD_Pos)
#define PM_IER_CKRDY_Pos            5            /**< \brief (PM_IER) Clock Ready Interrupt Enable */
#define PM_IER_CKRDY                (0x1u << PM_IER_CKRDY_Pos)
#define PM_IER_WAKE_Pos             8            /**< \brief (PM_IER) Wake up Interrupt Enable */
#define PM_IER_WAKE                 (0x1u << PM_IER_WAKE_Pos)
#define   PM_IER_WAKE_0             (0x0u <<  8) /**< \brief (PM_IER) No effect */
#define   PM_IER_WAKE_1             (0x1u <<  8) /**< \brief (PM_IER) Disable Interrupt. */
#define PM_IER_AE_Pos               31           /**< \brief (PM_IER) Access Error Interrupt Enable */
#define PM_IER_AE                   (0x1u << PM_IER_AE_Pos)
#define PM_IER_MASK                 0x80000121u  /**< \brief (PM_IER) MASK Register */

/* -------- PM_IDR : (PM Offset: 0x0C4) ( /W 32) Interrupt Disable Register -------- */
#define PM_IDR_OFFSET               0x0C4        /**< \brief (PM_IDR offset) Interrupt Disable Register */
#define PM_IDR_RESETVALUE           0x00000000   /**< \brief (PM_IDR reset_value) Interrupt Disable Register */

#define PM_IDR_CFD_Pos              0            /**< \brief (PM_IDR) Clock Failure Detected Interrupt Disable */
#define PM_IDR_CFD                  (0x1u << PM_IDR_CFD_Pos)
#define PM_IDR_CKRDY_Pos            5            /**< \brief (PM_IDR) Clock Ready Interrupt Disable */
#define PM_IDR_CKRDY                (0x1u << PM_IDR_CKRDY_Pos)
#define PM_IDR_WAKE_Pos             8            /**< \brief (PM_IDR) Wake up Interrupt Disable */
#define PM_IDR_WAKE                 (0x1u << PM_IDR_WAKE_Pos)
#define   PM_IDR_WAKE_0             (0x0u <<  8) /**< \brief (PM_IDR) No effect */
#define   PM_IDR_WAKE_1             (0x1u <<  8) /**< \brief (PM_IDR) Disable Interrupt. */
#define PM_IDR_AE_Pos               31           /**< \brief (PM_IDR) Access Error Interrupt Disable */
#define PM_IDR_AE                   (0x1u << PM_IDR_AE_Pos)
#define PM_IDR_MASK                 0x80000121u  /**< \brief (PM_IDR) MASK Register */

/* -------- PM_IMR : (PM Offset: 0x0C8) (R/  32) Interrupt Mask Register -------- */
#define PM_IMR_OFFSET               0x0C8        /**< \brief (PM_IMR offset) Interrupt Mask Register */
#define PM_IMR_RESETVALUE           0x00000000   /**< \brief (PM_IMR reset_value) Interrupt Mask Register */

#define PM_IMR_CFD_Pos              0            /**< \brief (PM_IMR) Clock Failure Detected Interrupt Mask */
#define PM_IMR_CFD                  (0x1u << PM_IMR_CFD_Pos)
#define PM_IMR_CKRDY_Pos            5            /**< \brief (PM_IMR) Clock Ready Interrupt Mask */
#define PM_IMR_CKRDY                (0x1u << PM_IMR_CKRDY_Pos)
#define PM_IMR_WAKE_Pos             8            /**< \brief (PM_IMR) Wake up Interrupt Mask */
#define PM_IMR_WAKE                 (0x1u << PM_IMR_WAKE_Pos)
#define   PM_IMR_WAKE_0             (0x0u <<  8) /**< \brief (PM_IMR) No effect */
#define   PM_IMR_WAKE_1             (0x1u <<  8) /**< \brief (PM_IMR) Disable Interrupt. */
#define PM_IMR_AE_Pos               31           /**< \brief (PM_IMR) Access Error Interrupt Mask */
#define PM_IMR_AE                   (0x1u << PM_IMR_AE_Pos)
#define PM_IMR_MASK                 0x80000121u  /**< \brief (PM_IMR) MASK Register */

/* -------- PM_ISR : (PM Offset: 0x0CC) (R/  32) Interrupt Status Register -------- */
#define PM_ISR_OFFSET               0x0CC        /**< \brief (PM_ISR offset) Interrupt Status Register */
#define PM_ISR_RESETVALUE           0x00000000   /**< \brief (PM_ISR reset_value) Interrupt Status Register */

#define PM_ISR_CFD_Pos              0            /**< \brief (PM_ISR) Clock Failure Detected Interrupt Status */
#define PM_ISR_CFD                  (0x1u << PM_ISR_CFD_Pos)
#define PM_ISR_CKRDY_Pos            5            /**< \brief (PM_ISR) Clock Ready Interrupt Status */
#define PM_ISR_CKRDY                (0x1u << PM_ISR_CKRDY_Pos)
#define PM_ISR_WAKE_Pos             8            /**< \brief (PM_ISR) Wake up Interrupt Status */
#define PM_ISR_WAKE                 (0x1u << PM_ISR_WAKE_Pos)
#define   PM_ISR_WAKE_0             (0x0u <<  8) /**< \brief (PM_ISR) No effect */
#define   PM_ISR_WAKE_1             (0x1u <<  8) /**< \brief (PM_ISR) Disable Interrupt. */
#define PM_ISR_AE_Pos               31           /**< \brief (PM_ISR) Access Error Interrupt Status */
#define PM_ISR_AE                   (0x1u << PM_ISR_AE_Pos)
#define PM_ISR_MASK                 0x80000121u  /**< \brief (PM_ISR) MASK Register */

/* -------- PM_ICR : (PM Offset: 0x0D0) ( /W 32) Interrupt Clear Register -------- */
#define PM_ICR_OFFSET               0x0D0        /**< \brief (PM_ICR offset) Interrupt Clear Register */
#define PM_ICR_RESETVALUE           0x00000000   /**< \brief (PM_ICR reset_value) Interrupt Clear Register */

#define PM_ICR_CFD_Pos              0            /**< \brief (PM_ICR) Clock Failure Detected Interrupt Status Clear */
#define PM_ICR_CFD                  (0x1u << PM_ICR_CFD_Pos)
#define PM_ICR_CKRDY_Pos            5            /**< \brief (PM_ICR) Clock Ready Interrupt Status Clear */
#define PM_ICR_CKRDY                (0x1u << PM_ICR_CKRDY_Pos)
#define PM_ICR_WAKE_Pos             8            /**< \brief (PM_ICR) Wake up Interrupt Status Clear */
#define PM_ICR_WAKE                 (0x1u << PM_ICR_WAKE_Pos)
#define PM_ICR_AE_Pos               31           /**< \brief (PM_ICR) Access Error Interrupt Status Clear */
#define PM_ICR_AE                   (0x1u << PM_ICR_AE_Pos)
#define PM_ICR_MASK                 0x80000121u  /**< \brief (PM_ICR) MASK Register */

/* -------- PM_SR : (PM Offset: 0x0D4) (R/  32) Status Register -------- */
#define PM_SR_OFFSET                0x0D4        /**< \brief (PM_SR offset) Status Register */
#define PM_SR_RESETVALUE            0x00000000   /**< \brief (PM_SR reset_value) Status Register */

#define PM_SR_CFD_Pos               0            /**< \brief (PM_SR) Clock Failure Detected */
#define PM_SR_CFD                   (0x1u << PM_SR_CFD_Pos)
#define PM_SR_OCP_Pos               1            /**< \brief (PM_SR) Over Clock Detected */
#define PM_SR_OCP                   (0x1u << PM_SR_OCP_Pos)
#define PM_SR_CKRDY_Pos             5            /**< \brief (PM_SR) Clock Ready */
#define PM_SR_CKRDY                 (0x1u << PM_SR_CKRDY_Pos)
#define PM_SR_WAKE_Pos              8            /**< \brief (PM_SR) Wake up */
#define PM_SR_WAKE                  (0x1u << PM_SR_WAKE_Pos)
#define   PM_SR_WAKE_0              (0x0u <<  8) /**< \brief (PM_SR) No effect */
#define   PM_SR_WAKE_1              (0x1u <<  8) /**< \brief (PM_SR) Disable Interrupt. */
#define PM_SR_PERRDY_Pos            28           /**< \brief (PM_SR) Peripheral Ready */
#define PM_SR_PERRDY                (0x1u << PM_SR_PERRDY_Pos)
#define PM_SR_AE_Pos                31           /**< \brief (PM_SR) Access Error */
#define PM_SR_AE                    (0x1u << PM_SR_AE_Pos)
#define PM_SR_MASK                  0x90000123u  /**< \brief (PM_SR) MASK Register */

/* -------- PM_PPCR : (PM Offset: 0x160) (R/W 32) Peripheral Power Control Register -------- */
#define PM_PPCR_OFFSET              0x160        /**< \brief (PM_PPCR offset) Peripheral Power Control Register */
#define PM_PPCR_RESETVALUE          0x000001FE   /**< \brief (PM_PPCR reset_value) Peripheral Power Control Register */

#define PM_PPCR_RSTPUN_Pos          0            /**< \brief (PM_PPCR) Reset Pullup */
#define PM_PPCR_RSTPUN              (0x1u << PM_PPCR_RSTPUN_Pos)
#define PM_PPCR_CATBRCMASK_Pos      1            /**< \brief (PM_PPCR) CAT Request Clock Mask */
#define PM_PPCR_CATBRCMASK          (0x1u << PM_PPCR_CATBRCMASK_Pos)
#define PM_PPCR_ACIFCRCMASK_Pos     2            /**< \brief (PM_PPCR) ACIFC Request Clock Mask */
#define PM_PPCR_ACIFCRCMASK         (0x1u << PM_PPCR_ACIFCRCMASK_Pos)
#define PM_PPCR_ASTRCMASK_Pos       3            /**< \brief (PM_PPCR) AST Request Clock Mask */
#define PM_PPCR_ASTRCMASK           (0x1u << PM_PPCR_ASTRCMASK_Pos)
#define PM_PPCR_TWIS0RCMASK_Pos     4            /**< \brief (PM_PPCR) TWIS0 Request Clock Mask */
#define PM_PPCR_TWIS0RCMASK         (0x1u << PM_PPCR_TWIS0RCMASK_Pos)
#define PM_PPCR_TWIS1RCMASK_Pos     5            /**< \brief (PM_PPCR) TWIS1 Request Clock Mask */
#define PM_PPCR_TWIS1RCMASK         (0x1u << PM_PPCR_TWIS1RCMASK_Pos)
#define PM_PPCR_PEVCRCMASK_Pos      6            /**< \brief (PM_PPCR) PEVC Request Clock Mask */
#define PM_PPCR_PEVCRCMASK          (0x1u << PM_PPCR_PEVCRCMASK_Pos)
#define PM_PPCR_ADCIFERCMASK_Pos    7            /**< \brief (PM_PPCR) ADCIFE Request Clock Mask */
#define PM_PPCR_ADCIFERCMASK        (0x1u << PM_PPCR_ADCIFERCMASK_Pos)
#define PM_PPCR_VREGRCMASK_Pos      8            /**< \brief (PM_PPCR) VREG Request Clock Mask */
#define PM_PPCR_VREGRCMASK          (0x1u << PM_PPCR_VREGRCMASK_Pos)
#define PM_PPCR_FWBGREF_Pos         9            /**< \brief (PM_PPCR) Flash Wait BGREF */
#define PM_PPCR_FWBGREF             (0x1u << PM_PPCR_FWBGREF_Pos)
#define PM_PPCR_FWBOD18_Pos         10           /**< \brief (PM_PPCR) Flash Wait BOD18 */
#define PM_PPCR_FWBOD18             (0x1u << PM_PPCR_FWBOD18_Pos)
#define PM_PPCR_MASK                0x000007FFu  /**< \brief (PM_PPCR) MASK Register */

/* -------- PM_RCAUSE : (PM Offset: 0x180) (R/  32) Reset Cause Register -------- */
#define PM_RCAUSE_OFFSET            0x180        /**< \brief (PM_RCAUSE offset) Reset Cause Register */
#define PM_RCAUSE_RESETVALUE        0x00000000   /**< \brief (PM_RCAUSE reset_value) Reset Cause Register */

#define PM_RCAUSE_POR_Pos           0            /**< \brief (PM_RCAUSE) Power-on Reset */
#define PM_RCAUSE_POR               (0x1u << PM_RCAUSE_POR_Pos)
#define PM_RCAUSE_BOD_Pos           1            /**< \brief (PM_RCAUSE) Brown-out Reset */
#define PM_RCAUSE_BOD               (0x1u << PM_RCAUSE_BOD_Pos)
#define PM_RCAUSE_EXT_Pos           2            /**< \brief (PM_RCAUSE) External Reset Pin */
#define PM_RCAUSE_EXT               (0x1u << PM_RCAUSE_EXT_Pos)
#define PM_RCAUSE_WDT_Pos           3            /**< \brief (PM_RCAUSE) Watchdog Reset */
#define PM_RCAUSE_WDT               (0x1u << PM_RCAUSE_WDT_Pos)
#define PM_RCAUSE_OCDRST_Pos        8            /**< \brief (PM_RCAUSE) OCD Reset */
#define PM_RCAUSE_OCDRST            (0x1u << PM_RCAUSE_OCDRST_Pos)
#define PM_RCAUSE_POR33_Pos         10           /**< \brief (PM_RCAUSE) Power-on Reset */
#define PM_RCAUSE_POR33             (0x1u << PM_RCAUSE_POR33_Pos)
#define PM_RCAUSE_BOD33_Pos         13           /**< \brief (PM_RCAUSE) Brown-out 3.3V Reset */
#define PM_RCAUSE_BOD33             (0x1u << PM_RCAUSE_BOD33_Pos)
#define PM_RCAUSE_MASK              0x0000250Fu  /**< \brief (PM_RCAUSE) MASK Register */

/* -------- PM_WCAUSE : (PM Offset: 0x184) (R/  32) Wake Cause Register -------- */
#define PM_WCAUSE_OFFSET            0x184        /**< \brief (PM_WCAUSE offset) Wake Cause Register */
#define PM_WCAUSE_RESETVALUE        0x00000000   /**< \brief (PM_WCAUSE reset_value) Wake Cause Register */

#define PM_WCAUSE_TWI_SLAVE_0_Pos   0            /**< \brief (PM_WCAUSE) Two-wire Slave Interface 0 */
#define PM_WCAUSE_TWI_SLAVE_0       (0x1u << PM_WCAUSE_TWI_SLAVE_0_Pos)
#define PM_WCAUSE_TWI_SLAVE_1_Pos   1            /**< \brief (PM_WCAUSE) Two-wire Slave Interface 1 */
#define PM_WCAUSE_TWI_SLAVE_1       (0x1u << PM_WCAUSE_TWI_SLAVE_1_Pos)
#define PM_WCAUSE_USBC_Pos          2            /**< \brief (PM_WCAUSE) USB Device and Embedded Host Interface */
#define PM_WCAUSE_USBC              (0x1u << PM_WCAUSE_USBC_Pos)
#define PM_WCAUSE_PSOK_Pos          3            /**< \brief (PM_WCAUSE) Power Scaling OK */
#define PM_WCAUSE_PSOK              (0x1u << PM_WCAUSE_PSOK_Pos)
#define PM_WCAUSE_BOD18_IRQ_Pos     4            /**< \brief (PM_WCAUSE) BOD18 Interrupt */
#define PM_WCAUSE_BOD18_IRQ         (0x1u << PM_WCAUSE_BOD18_IRQ_Pos)
#define PM_WCAUSE_BOD33_IRQ_Pos     5            /**< \brief (PM_WCAUSE) BOD33 Interrupt */
#define PM_WCAUSE_BOD33_IRQ         (0x1u << PM_WCAUSE_BOD33_IRQ_Pos)
#define PM_WCAUSE_PICOUART_Pos      6            /**< \brief (PM_WCAUSE) Picopower UART */
#define PM_WCAUSE_PICOUART          (0x1u << PM_WCAUSE_PICOUART_Pos)
#define PM_WCAUSE_LCDCA_Pos         7            /**< \brief (PM_WCAUSE) LCD Controller */
#define PM_WCAUSE_LCDCA             (0x1u << PM_WCAUSE_LCDCA_Pos)
#define PM_WCAUSE_EIC_Pos           16           /**< \brief (PM_WCAUSE) External Interrupt Controller */
#define PM_WCAUSE_EIC               (0x1u << PM_WCAUSE_EIC_Pos)
#define PM_WCAUSE_AST_Pos           17           /**< \brief (PM_WCAUSE) Asynchronous Timer */
#define PM_WCAUSE_AST               (0x1u << PM_WCAUSE_AST_Pos)
#define PM_WCAUSE_MASK              0x000300FFu  /**< \brief (PM_WCAUSE) MASK Register */

/* -------- PM_AWEN : (PM Offset: 0x188) (R/W 32) Asynchronous Wake Enable -------- */
#define PM_AWEN_OFFSET              0x188        /**< \brief (PM_AWEN offset) Asynchronous Wake Enable */
#define PM_AWEN_RESETVALUE          0x00000000   /**< \brief (PM_AWEN reset_value) Asynchronous Wake Enable */

#define PM_AWEN_AWEN_Pos            0            /**< \brief (PM_AWEN) Asynchronous Wake Up */
#define PM_AWEN_AWEN_Msk            (0xFFFFFFFFu << PM_AWEN_AWEN_Pos)
#define PM_AWEN_AWEN(value)         ((PM_AWEN_AWEN_Msk & ((value) << PM_AWEN_AWEN_Pos)))
#define PM_AWEN_MASK                0xFFFFFFFFu  /**< \brief (PM_AWEN) MASK Register */

/* -------- PM_OBS : (PM Offset: 0x190) (R/W 32) Obsvervability -------- */
#define PM_OBS_OFFSET               0x190        /**< \brief (PM_OBS offset) Obsvervability */
#define PM_OBS_RESETVALUE           0x00000000   /**< \brief (PM_OBS reset_value) Obsvervability */

#define PM_OBS_MASK                 0x00000000u  /**< \brief (PM_OBS) MASK Register */

/* -------- PM_FASTSLEEP : (PM Offset: 0x194) (R/W 32) Fast Sleep Register -------- */
#define PM_FASTSLEEP_OFFSET         0x194        /**< \brief (PM_FASTSLEEP offset) Fast Sleep Register */
#define PM_FASTSLEEP_RESETVALUE     0x00000000   /**< \brief (PM_FASTSLEEP reset_value) Fast Sleep Register */

#define PM_FASTSLEEP_OSC_Pos        0            /**< \brief (PM_FASTSLEEP) Oscillator */
#define PM_FASTSLEEP_OSC            (0x1u << PM_FASTSLEEP_OSC_Pos)
#define PM_FASTSLEEP_PLL_Pos        8            /**< \brief (PM_FASTSLEEP) PLL */
#define PM_FASTSLEEP_PLL            (0x1u << PM_FASTSLEEP_PLL_Pos)
#define PM_FASTSLEEP_FASTRCOSC_Pos  16           /**< \brief (PM_FASTSLEEP) RC80 or FLO */
#define PM_FASTSLEEP_FASTRCOSC_Msk  (0x1Fu << PM_FASTSLEEP_FASTRCOSC_Pos)
#define PM_FASTSLEEP_FASTRCOSC(value) ((PM_FASTSLEEP_FASTRCOSC_Msk & ((value) << PM_FASTSLEEP_FASTRCOSC_Pos)))
#define PM_FASTSLEEP_DFLL_Pos       24           /**< \brief (PM_FASTSLEEP) DFLL */
#define PM_FASTSLEEP_DFLL           (0x1u << PM_FASTSLEEP_DFLL_Pos)
#define PM_FASTSLEEP_MASK           0x011F0101u  /**< \brief (PM_FASTSLEEP) MASK Register */

/* -------- PM_CONFIG : (PM Offset: 0x3F8) (R/  32) Configuration Register -------- */
#define PM_CONFIG_OFFSET            0x3F8        /**< \brief (PM_CONFIG offset) Configuration Register */
#define PM_CONFIG_RESETVALUE        0x0000000F   /**< \brief (PM_CONFIG reset_value) Configuration Register */

#define PM_CONFIG_PBA_Pos           0            /**< \brief (PM_CONFIG) APBA Implemented */
#define PM_CONFIG_PBA               (0x1u << PM_CONFIG_PBA_Pos)
#define PM_CONFIG_PBB_Pos           1            /**< \brief (PM_CONFIG) APBB Implemented */
#define PM_CONFIG_PBB               (0x1u << PM_CONFIG_PBB_Pos)
#define PM_CONFIG_PBC_Pos           2            /**< \brief (PM_CONFIG) APBC Implemented */
#define PM_CONFIG_PBC               (0x1u << PM_CONFIG_PBC_Pos)
#define PM_CONFIG_PBD_Pos           3            /**< \brief (PM_CONFIG) APBD Implemented */
#define PM_CONFIG_PBD               (0x1u << PM_CONFIG_PBD_Pos)
#define PM_CONFIG_HSBPEVC_Pos       7            /**< \brief (PM_CONFIG) HSB PEVC Clock Implemented */
#define PM_CONFIG_HSBPEVC           (0x1u << PM_CONFIG_HSBPEVC_Pos)
#define PM_CONFIG_MASK              0x0000008Fu  /**< \brief (PM_CONFIG) MASK Register */

/* -------- PM_VERSION : (PM Offset: 0x3FC) (R/  32) Version Register -------- */
#define PM_VERSION_OFFSET           0x3FC        /**< \brief (PM_VERSION offset) Version Register */
#define PM_VERSION_RESETVALUE       0x00000441   /**< \brief (PM_VERSION reset_value) Version Register */

#define PM_VERSION_VERSION_Pos      0            /**< \brief (PM_VERSION) Version number */
#define PM_VERSION_VERSION_Msk      (0xFFFu << PM_VERSION_VERSION_Pos)
#define PM_VERSION_VERSION(value)   ((PM_VERSION_VERSION_Msk & ((value) << PM_VERSION_VERSION_Pos)))
#define PM_VERSION_VARIANT_Pos      16           /**< \brief (PM_VERSION) Variant number */
#define PM_VERSION_VARIANT_Msk      (0xFu << PM_VERSION_VARIANT_Pos)
#define PM_VERSION_VARIANT(value)   ((PM_VERSION_VARIANT_Msk & ((value) << PM_VERSION_VARIANT_Pos)))
#define PM_VERSION_MASK             0x000F0FFFu  /**< \brief (PM_VERSION) MASK Register */

/** \brief PM hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  RwReg   PM_MCCTRL;          /**< \brief (PM Offset: 0x000) Main Clock Control */
  RwReg   PM_CPUSEL;          /**< \brief (PM Offset: 0x004) CPU Clock Select */
  RoReg8  Reserved1[0x4];
  RwReg   PM_PBASEL;          /**< \brief (PM Offset: 0x00C) PBA Clock Select */
  RwReg   PM_PBBSEL;          /**< \brief (PM Offset: 0x010) PBB Clock Select */
  RwReg   PM_PBCSEL;          /**< \brief (PM Offset: 0x014) PBC Clock Select */
  RwReg   PM_PBDSEL;          /**< \brief (PM Offset: 0x018) PBD Clock Select */
  RoReg8  Reserved2[0x4];
  RwReg   PM_CPUMASK;         /**< \brief (PM Offset: 0x020) CPU Mask */
  RwReg   PM_HSBMASK;         /**< \brief (PM Offset: 0x024) HSB Mask */
  RwReg   PM_PBAMASK;         /**< \brief (PM Offset: 0x028) PBA Mask */
  RwReg   PM_PBBMASK;         /**< \brief (PM Offset: 0x02C) PBB Mask */
  RwReg   PM_PBCMASK;         /**< \brief (PM Offset: 0x030) PBC Mask */
  RwReg   PM_PBDMASK;         /**< \brief (PM Offset: 0x034) PBD Mask */
  RoReg8  Reserved3[0x8];
  RwReg   PM_PBADIVMASK;      /**< \brief (PM Offset: 0x040) PBA Divided Clock Mask */
  RoReg8  Reserved4[0x10];
  RwReg   PM_CFDCTRL;         /**< \brief (PM Offset: 0x054) Clock Failure Detector Control */
  WoReg   PM_UNLOCK;          /**< \brief (PM Offset: 0x058) Unlock Register */
  RoReg8  Reserved5[0x64];
  WoReg   PM_IER;             /**< \brief (PM Offset: 0x0C0) Interrupt Enable Register */
  WoReg   PM_IDR;             /**< \brief (PM Offset: 0x0C4) Interrupt Disable Register */
  RoReg   PM_IMR;             /**< \brief (PM Offset: 0x0C8) Interrupt Mask Register */
  RoReg   PM_ISR;             /**< \brief (PM Offset: 0x0CC) Interrupt Status Register */
  WoReg   PM_ICR;             /**< \brief (PM Offset: 0x0D0) Interrupt Clear Register */
  RoReg   PM_SR;              /**< \brief (PM Offset: 0x0D4) Status Register */
  RoReg8  Reserved6[0x88];
  RwReg   PM_PPCR;            /**< \brief (PM Offset: 0x160) Peripheral Power Control Register */
  RoReg8  Reserved7[0x1C];
  RoReg   PM_RCAUSE;          /**< \brief (PM Offset: 0x180) Reset Cause Register */
  RoReg   PM_WCAUSE;          /**< \brief (PM Offset: 0x184) Wake Cause Register */
  RwReg   PM_AWEN;            /**< \brief (PM Offset: 0x188) Asynchronous Wake Enable */
  RoReg8  Reserved8[0x4];
  RwReg   PM_OBS;             /**< \brief (PM Offset: 0x190) Obsvervability */
  RwReg   PM_FASTSLEEP;       /**< \brief (PM Offset: 0x194) Fast Sleep Register */
  RoReg8  Reserved9[0x260];
  RoReg   PM_CONFIG;          /**< \brief (PM Offset: 0x3F8) Configuration Register */
  RoReg   PM_VERSION;         /**< \brief (PM Offset: 0x3FC) Version Register */
} Pm;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAM4L_PM_COMPONENT_ */
