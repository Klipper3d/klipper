/**
 * \file
 *
 * \brief Instance description for TCC1
 *
 * Copyright (c) 2018 Microchip Technology Inc.
 *
 * \asf_license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the Licence at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \asf_license_stop
 *
 */

#ifndef _SAMD21_TCC1_INSTANCE_
#define _SAMD21_TCC1_INSTANCE_

/* ========== Register definition for TCC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TCC1_CTRLA             (0x42002400) /**< \brief (TCC1) Control A */
#define REG_TCC1_CTRLBCLR          (0x42002404) /**< \brief (TCC1) Control B Clear */
#define REG_TCC1_CTRLBSET          (0x42002405) /**< \brief (TCC1) Control B Set */
#define REG_TCC1_SYNCBUSY          (0x42002408) /**< \brief (TCC1) Synchronization Busy */
#define REG_TCC1_FCTRLA            (0x4200240C) /**< \brief (TCC1) Recoverable Fault A Configuration */
#define REG_TCC1_FCTRLB            (0x42002410) /**< \brief (TCC1) Recoverable Fault B Configuration */
#define REG_TCC1_DRVCTRL           (0x42002418) /**< \brief (TCC1) Driver Control */
#define REG_TCC1_DBGCTRL           (0x4200241E) /**< \brief (TCC1) Debug Control */
#define REG_TCC1_EVCTRL            (0x42002420) /**< \brief (TCC1) Event Control */
#define REG_TCC1_INTENCLR          (0x42002424) /**< \brief (TCC1) Interrupt Enable Clear */
#define REG_TCC1_INTENSET          (0x42002428) /**< \brief (TCC1) Interrupt Enable Set */
#define REG_TCC1_INTFLAG           (0x4200242C) /**< \brief (TCC1) Interrupt Flag Status and Clear */
#define REG_TCC1_STATUS            (0x42002430) /**< \brief (TCC1) Status */
#define REG_TCC1_COUNT             (0x42002434) /**< \brief (TCC1) Count */
#define REG_TCC1_PATT              (0x42002438) /**< \brief (TCC1) Pattern */
#define REG_TCC1_WAVE              (0x4200243C) /**< \brief (TCC1) Waveform Control */
#define REG_TCC1_PER               (0x42002440) /**< \brief (TCC1) Period */
#define REG_TCC1_CC0               (0x42002444) /**< \brief (TCC1) Compare and Capture 0 */
#define REG_TCC1_CC1               (0x42002448) /**< \brief (TCC1) Compare and Capture 1 */
#define REG_TCC1_PATTB             (0x42002464) /**< \brief (TCC1) Pattern Buffer */
#define REG_TCC1_WAVEB             (0x42002468) /**< \brief (TCC1) Waveform Control Buffer */
#define REG_TCC1_PERB              (0x4200246C) /**< \brief (TCC1) Period Buffer */
#define REG_TCC1_CCB0              (0x42002470) /**< \brief (TCC1) Compare and Capture Buffer 0 */
#define REG_TCC1_CCB1              (0x42002474) /**< \brief (TCC1) Compare and Capture Buffer 1 */
#else
#define REG_TCC1_CTRLA             (*(RwReg  *)0x42002400UL) /**< \brief (TCC1) Control A */
#define REG_TCC1_CTRLBCLR          (*(RwReg8 *)0x42002404UL) /**< \brief (TCC1) Control B Clear */
#define REG_TCC1_CTRLBSET          (*(RwReg8 *)0x42002405UL) /**< \brief (TCC1) Control B Set */
#define REG_TCC1_SYNCBUSY          (*(RoReg  *)0x42002408UL) /**< \brief (TCC1) Synchronization Busy */
#define REG_TCC1_FCTRLA            (*(RwReg  *)0x4200240CUL) /**< \brief (TCC1) Recoverable Fault A Configuration */
#define REG_TCC1_FCTRLB            (*(RwReg  *)0x42002410UL) /**< \brief (TCC1) Recoverable Fault B Configuration */
#define REG_TCC1_DRVCTRL           (*(RwReg  *)0x42002418UL) /**< \brief (TCC1) Driver Control */
#define REG_TCC1_DBGCTRL           (*(RwReg8 *)0x4200241EUL) /**< \brief (TCC1) Debug Control */
#define REG_TCC1_EVCTRL            (*(RwReg  *)0x42002420UL) /**< \brief (TCC1) Event Control */
#define REG_TCC1_INTENCLR          (*(RwReg  *)0x42002424UL) /**< \brief (TCC1) Interrupt Enable Clear */
#define REG_TCC1_INTENSET          (*(RwReg  *)0x42002428UL) /**< \brief (TCC1) Interrupt Enable Set */
#define REG_TCC1_INTFLAG           (*(RwReg  *)0x4200242CUL) /**< \brief (TCC1) Interrupt Flag Status and Clear */
#define REG_TCC1_STATUS            (*(RwReg  *)0x42002430UL) /**< \brief (TCC1) Status */
#define REG_TCC1_COUNT             (*(RwReg  *)0x42002434UL) /**< \brief (TCC1) Count */
#define REG_TCC1_PATT              (*(RwReg16*)0x42002438UL) /**< \brief (TCC1) Pattern */
#define REG_TCC1_WAVE              (*(RwReg  *)0x4200243CUL) /**< \brief (TCC1) Waveform Control */
#define REG_TCC1_PER               (*(RwReg  *)0x42002440UL) /**< \brief (TCC1) Period */
#define REG_TCC1_CC0               (*(RwReg  *)0x42002444UL) /**< \brief (TCC1) Compare and Capture 0 */
#define REG_TCC1_CC1               (*(RwReg  *)0x42002448UL) /**< \brief (TCC1) Compare and Capture 1 */
#define REG_TCC1_PATTB             (*(RwReg16*)0x42002464UL) /**< \brief (TCC1) Pattern Buffer */
#define REG_TCC1_WAVEB             (*(RwReg  *)0x42002468UL) /**< \brief (TCC1) Waveform Control Buffer */
#define REG_TCC1_PERB              (*(RwReg  *)0x4200246CUL) /**< \brief (TCC1) Period Buffer */
#define REG_TCC1_CCB0              (*(RwReg  *)0x42002470UL) /**< \brief (TCC1) Compare and Capture Buffer 0 */
#define REG_TCC1_CCB1              (*(RwReg  *)0x42002474UL) /**< \brief (TCC1) Compare and Capture Buffer 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for TCC1 peripheral ========== */
#define TCC1_CC_NUM                 2        // Number of Compare/Capture units
#define TCC1_DITHERING              1        // Dithering feature implemented
#define TCC1_DMAC_ID_MC_0           19
#define TCC1_DMAC_ID_MC_1           20
#define TCC1_DMAC_ID_MC_LSB         19
#define TCC1_DMAC_ID_MC_MSB         20
#define TCC1_DMAC_ID_MC_SIZE        2
#define TCC1_DMAC_ID_OVF            18       // DMA overflow/underflow/retrigger trigger
#define TCC1_DTI                    0        // Dead-Time-Insertion feature implemented
#define TCC1_EXT                    24       // Coding of implemented extended features
#define TCC1_GCLK_ID                26       // Index of Generic Clock
#define TCC1_OTMX                   0        // Output Matrix feature implemented
#define TCC1_OW_NUM                 4        // Number of Output Waveforms
#define TCC1_PG                     1        // Pattern Generation feature implemented
#define TCC1_SIZE                   24      
#define TCC1_SWAP                   0        // DTI outputs swap feature implemented
#define TCC1_TYPE                   0        // TCC type 0 : NA, 1 : Master, 2 : Slave

#endif /* _SAMD21_TCC1_INSTANCE_ */
