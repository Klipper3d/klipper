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

#ifndef _SAM4E_AFEC1_INSTANCE_
#define _SAM4E_AFEC1_INSTANCE_

/* ========== Register definition for AFEC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AFEC1_CR              (0x400B4000U) /**< \brief (AFEC1) Control Register */
#define REG_AFEC1_MR              (0x400B4004U) /**< \brief (AFEC1) Mode Register */
#define REG_AFEC1_EMR             (0x400B4008U) /**< \brief (AFEC1) Extended Mode Register */
#define REG_AFEC1_SEQ1R           (0x400B400CU) /**< \brief (AFEC1) Channel Sequence 1 Register */
#define REG_AFEC1_SEQ2R           (0x400B4010U) /**< \brief (AFEC1) Channel Sequence 2 Register */
#define REG_AFEC1_CHER            (0x400B4014U) /**< \brief (AFEC1) Channel Enable Register */
#define REG_AFEC1_CHDR            (0x400B4018U) /**< \brief (AFEC1) Channel Disable Register */
#define REG_AFEC1_CHSR            (0x400B401CU) /**< \brief (AFEC1) Channel Status Register */
#define REG_AFEC1_LCDR            (0x400B4020U) /**< \brief (AFEC1) Last Converted Data Register */
#define REG_AFEC1_IER             (0x400B4024U) /**< \brief (AFEC1) Interrupt Enable Register */
#define REG_AFEC1_IDR             (0x400B4028U) /**< \brief (AFEC1) Interrupt Disable Register */
#define REG_AFEC1_IMR             (0x400B402CU) /**< \brief (AFEC1) Interrupt Mask Register */
#define REG_AFEC1_ISR             (0x400B4030U) /**< \brief (AFEC1) Interrupt Status Register */
#define REG_AFEC1_OVER            (0x400B404CU) /**< \brief (AFEC1) Overrun Status Register */
#define REG_AFEC1_CWR             (0x400B4050U) /**< \brief (AFEC1) Compare Window Register */
#define REG_AFEC1_CGR             (0x400B4054U) /**< \brief (AFEC1) Channel Gain Register */
#define REG_AFEC1_CDOR            (0x400B405CU) /**< \brief (AFEC1) Channel DC Offset Register */
#define REG_AFEC1_DIFFR           (0x400B4060U) /**< \brief (AFEC1) Channel Differential Register */
#define REG_AFEC1_CSELR           (0x400B4064U) /**< \brief (AFEC1) Channel Register Selection */
#define REG_AFEC1_CDR             (0x400B4068U) /**< \brief (AFEC1) Channel Data Register */
#define REG_AFEC1_COCR            (0x400B406CU) /**< \brief (AFEC1) Channel Offset Compensation Register */
#define REG_AFEC1_TEMPMR          (0x400B4070U) /**< \brief (AFEC1) Temperature Sensor Mode Register */
#define REG_AFEC1_TEMPCWR          (0x400B4074U) /**< \brief (AFEC1) Temperature Compare Window Register */
#define REG_AFEC1_ACR             (0x400B4094U) /**< \brief (AFEC1) Analog Control Register */
#define REG_AFEC1_WPMR            (0x400B40E4U) /**< \brief (AFEC1) Write Protect Mode Register */
#define REG_AFEC1_WPSR            (0x400B40E8U) /**< \brief (AFEC1) Write Protect Status Register */
#define REG_AFEC1_RPR             (0x400B4100U) /**< \brief (AFEC1) Receive Pointer Register */
#define REG_AFEC1_RCR             (0x400B4104U) /**< \brief (AFEC1) Receive Counter Register */
#define REG_AFEC1_RNPR            (0x400B4110U) /**< \brief (AFEC1) Receive Next Pointer Register */
#define REG_AFEC1_RNCR            (0x400B4114U) /**< \brief (AFEC1) Receive Next Counter Register */
#define REG_AFEC1_PTCR            (0x400B4120U) /**< \brief (AFEC1) Transfer Control Register */
#define REG_AFEC1_PTSR            (0x400B4124U) /**< \brief (AFEC1) Transfer Status Register */
#else
#define REG_AFEC1_CR     (*(WoReg*)0x400B4000U) /**< \brief (AFEC1) Control Register */
#define REG_AFEC1_MR     (*(RwReg*)0x400B4004U) /**< \brief (AFEC1) Mode Register */
#define REG_AFEC1_EMR    (*(RwReg*)0x400B4008U) /**< \brief (AFEC1) Extended Mode Register */
#define REG_AFEC1_SEQ1R  (*(RwReg*)0x400B400CU) /**< \brief (AFEC1) Channel Sequence 1 Register */
#define REG_AFEC1_SEQ2R  (*(RwReg*)0x400B4010U) /**< \brief (AFEC1) Channel Sequence 2 Register */
#define REG_AFEC1_CHER   (*(WoReg*)0x400B4014U) /**< \brief (AFEC1) Channel Enable Register */
#define REG_AFEC1_CHDR   (*(WoReg*)0x400B4018U) /**< \brief (AFEC1) Channel Disable Register */
#define REG_AFEC1_CHSR   (*(RoReg*)0x400B401CU) /**< \brief (AFEC1) Channel Status Register */
#define REG_AFEC1_LCDR   (*(RoReg*)0x400B4020U) /**< \brief (AFEC1) Last Converted Data Register */
#define REG_AFEC1_IER    (*(WoReg*)0x400B4024U) /**< \brief (AFEC1) Interrupt Enable Register */
#define REG_AFEC1_IDR    (*(WoReg*)0x400B4028U) /**< \brief (AFEC1) Interrupt Disable Register */
#define REG_AFEC1_IMR    (*(RoReg*)0x400B402CU) /**< \brief (AFEC1) Interrupt Mask Register */
#define REG_AFEC1_ISR    (*(RoReg*)0x400B4030U) /**< \brief (AFEC1) Interrupt Status Register */
#define REG_AFEC1_OVER   (*(RoReg*)0x400B404CU) /**< \brief (AFEC1) Overrun Status Register */
#define REG_AFEC1_CWR    (*(RwReg*)0x400B4050U) /**< \brief (AFEC1) Compare Window Register */
#define REG_AFEC1_CGR    (*(RwReg*)0x400B4054U) /**< \brief (AFEC1) Channel Gain Register */
#define REG_AFEC1_CDOR   (*(RwReg*)0x400B405CU) /**< \brief (AFEC1) Channel DC Offset Register */
#define REG_AFEC1_DIFFR  (*(RwReg*)0x400B4060U) /**< \brief (AFEC1) Channel Differential Register */
#define REG_AFEC1_CSELR  (*(RoReg*)0x400B4064U) /**< \brief (AFEC1) Channel Register Selection */
#define REG_AFEC1_CDR    (*(RoReg*)0x400B4068U) /**< \brief (AFEC1) Channel Data Register */
#define REG_AFEC1_COCR   (*(RoReg*)0x400B406CU) /**< \brief (AFEC1) Channel Offset Compensation Register */
#define REG_AFEC1_TEMPMR (*(RwReg*)0x400B4070U) /**< \brief (AFEC1) Temperature Sensor Mode Register */
#define REG_AFEC1_TEMPCWR (*(RwReg*)0x400B4074U) /**< \brief (AFEC1) Temperature Compare Window Register */
#define REG_AFEC1_ACR    (*(RwReg*)0x400B4094U) /**< \brief (AFEC1) Analog Control Register */
#define REG_AFEC1_WPMR   (*(RwReg*)0x400B40E4U) /**< \brief (AFEC1) Write Protect Mode Register */
#define REG_AFEC1_WPSR   (*(RoReg*)0x400B40E8U) /**< \brief (AFEC1) Write Protect Status Register */
#define REG_AFEC1_RPR    (*(RwReg*)0x400B4100U) /**< \brief (AFEC1) Receive Pointer Register */
#define REG_AFEC1_RCR    (*(RwReg*)0x400B4104U) /**< \brief (AFEC1) Receive Counter Register */
#define REG_AFEC1_RNPR   (*(RwReg*)0x400B4110U) /**< \brief (AFEC1) Receive Next Pointer Register */
#define REG_AFEC1_RNCR   (*(RwReg*)0x400B4114U) /**< \brief (AFEC1) Receive Next Counter Register */
#define REG_AFEC1_PTCR   (*(WoReg*)0x400B4120U) /**< \brief (AFEC1) Transfer Control Register */
#define REG_AFEC1_PTSR   (*(RoReg*)0x400B4124U) /**< \brief (AFEC1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_AFEC1_INSTANCE_ */
