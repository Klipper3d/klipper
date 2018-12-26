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

#ifndef _SAM4E_AFEC0_INSTANCE_
#define _SAM4E_AFEC0_INSTANCE_

/* ========== Register definition for AFEC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AFEC0_CR              (0x400B0000U) /**< \brief (AFEC0) Control Register */
#define REG_AFEC0_MR              (0x400B0004U) /**< \brief (AFEC0) Mode Register */
#define REG_AFEC0_EMR             (0x400B0008U) /**< \brief (AFEC0) Extended Mode Register */
#define REG_AFEC0_SEQ1R           (0x400B000CU) /**< \brief (AFEC0) Channel Sequence 1 Register */
#define REG_AFEC0_SEQ2R           (0x400B0010U) /**< \brief (AFEC0) Channel Sequence 2 Register */
#define REG_AFEC0_CHER            (0x400B0014U) /**< \brief (AFEC0) Channel Enable Register */
#define REG_AFEC0_CHDR            (0x400B0018U) /**< \brief (AFEC0) Channel Disable Register */
#define REG_AFEC0_CHSR            (0x400B001CU) /**< \brief (AFEC0) Channel Status Register */
#define REG_AFEC0_LCDR            (0x400B0020U) /**< \brief (AFEC0) Last Converted Data Register */
#define REG_AFEC0_IER             (0x400B0024U) /**< \brief (AFEC0) Interrupt Enable Register */
#define REG_AFEC0_IDR             (0x400B0028U) /**< \brief (AFEC0) Interrupt Disable Register */
#define REG_AFEC0_IMR             (0x400B002CU) /**< \brief (AFEC0) Interrupt Mask Register */
#define REG_AFEC0_ISR             (0x400B0030U) /**< \brief (AFEC0) Interrupt Status Register */
#define REG_AFEC0_OVER            (0x400B004CU) /**< \brief (AFEC0) Overrun Status Register */
#define REG_AFEC0_CWR             (0x400B0050U) /**< \brief (AFEC0) Compare Window Register */
#define REG_AFEC0_CGR             (0x400B0054U) /**< \brief (AFEC0) Channel Gain Register */
#define REG_AFEC0_CDOR            (0x400B005CU) /**< \brief (AFEC0) Channel DC Offset Register */
#define REG_AFEC0_DIFFR           (0x400B0060U) /**< \brief (AFEC0) Channel Differential Register */
#define REG_AFEC0_CSELR           (0x400B0064U) /**< \brief (AFEC0) Channel Register Selection */
#define REG_AFEC0_CDR             (0x400B0068U) /**< \brief (AFEC0) Channel Data Register */
#define REG_AFEC0_COCR            (0x400B006CU) /**< \brief (AFEC0) Channel Offset Compensation Register */
#define REG_AFEC0_TEMPMR          (0x400B0070U) /**< \brief (AFEC0) Temperature Sensor Mode Register */
#define REG_AFEC0_TEMPCWR          (0x400B0074U) /**< \brief (AFEC0) Temperature Compare Window Register */
#define REG_AFEC0_ACR             (0x400B0094U) /**< \brief (AFEC0) Analog Control Register */
#define REG_AFEC0_WPMR            (0x400B00E4U) /**< \brief (AFEC0) Write Protect Mode Register */
#define REG_AFEC0_WPSR            (0x400B00E8U) /**< \brief (AFEC0) Write Protect Status Register */
#define REG_AFEC0_RPR             (0x400B0100U) /**< \brief (AFEC0) Receive Pointer Register */
#define REG_AFEC0_RCR             (0x400B0104U) /**< \brief (AFEC0) Receive Counter Register */
#define REG_AFEC0_RNPR            (0x400B0110U) /**< \brief (AFEC0) Receive Next Pointer Register */
#define REG_AFEC0_RNCR            (0x400B0114U) /**< \brief (AFEC0) Receive Next Counter Register */
#define REG_AFEC0_PTCR            (0x400B0120U) /**< \brief (AFEC0) Transfer Control Register */
#define REG_AFEC0_PTSR            (0x400B0124U) /**< \brief (AFEC0) Transfer Status Register */
#else
#define REG_AFEC0_CR     (*(WoReg*)0x400B0000U) /**< \brief (AFEC0) Control Register */
#define REG_AFEC0_MR     (*(RwReg*)0x400B0004U) /**< \brief (AFEC0) Mode Register */
#define REG_AFEC0_EMR    (*(RwReg*)0x400B0008U) /**< \brief (AFEC0) Extended Mode Register */
#define REG_AFEC0_SEQ1R  (*(RwReg*)0x400B000CU) /**< \brief (AFEC0) Channel Sequence 1 Register */
#define REG_AFEC0_SEQ2R  (*(RwReg*)0x400B0010U) /**< \brief (AFEC0) Channel Sequence 2 Register */
#define REG_AFEC0_CHER   (*(WoReg*)0x400B0014U) /**< \brief (AFEC0) Channel Enable Register */
#define REG_AFEC0_CHDR   (*(WoReg*)0x400B0018U) /**< \brief (AFEC0) Channel Disable Register */
#define REG_AFEC0_CHSR   (*(RoReg*)0x400B001CU) /**< \brief (AFEC0) Channel Status Register */
#define REG_AFEC0_LCDR   (*(RoReg*)0x400B0020U) /**< \brief (AFEC0) Last Converted Data Register */
#define REG_AFEC0_IER    (*(WoReg*)0x400B0024U) /**< \brief (AFEC0) Interrupt Enable Register */
#define REG_AFEC0_IDR    (*(WoReg*)0x400B0028U) /**< \brief (AFEC0) Interrupt Disable Register */
#define REG_AFEC0_IMR    (*(RoReg*)0x400B002CU) /**< \brief (AFEC0) Interrupt Mask Register */
#define REG_AFEC0_ISR    (*(RoReg*)0x400B0030U) /**< \brief (AFEC0) Interrupt Status Register */
#define REG_AFEC0_OVER   (*(RoReg*)0x400B004CU) /**< \brief (AFEC0) Overrun Status Register */
#define REG_AFEC0_CWR    (*(RwReg*)0x400B0050U) /**< \brief (AFEC0) Compare Window Register */
#define REG_AFEC0_CGR    (*(RwReg*)0x400B0054U) /**< \brief (AFEC0) Channel Gain Register */
#define REG_AFEC0_CDOR   (*(RwReg*)0x400B005CU) /**< \brief (AFEC0) Channel DC Offset Register */
#define REG_AFEC0_DIFFR  (*(RwReg*)0x400B0060U) /**< \brief (AFEC0) Channel Differential Register */
#define REG_AFEC0_CSELR  (*(RoReg*)0x400B0064U) /**< \brief (AFEC0) Channel Register Selection */
#define REG_AFEC0_CDR    (*(RoReg*)0x400B0068U) /**< \brief (AFEC0) Channel Data Register */
#define REG_AFEC0_COCR   (*(RoReg*)0x400B006CU) /**< \brief (AFEC0) Channel Offset Compensation Register */
#define REG_AFEC0_TEMPMR (*(RwReg*)0x400B0070U) /**< \brief (AFEC0) Temperature Sensor Mode Register */
#define REG_AFEC0_TEMPCWR (*(RwReg*)0x400B0074U) /**< \brief (AFEC0) Temperature Compare Window Register */
#define REG_AFEC0_ACR    (*(RwReg*)0x400B0094U) /**< \brief (AFEC0) Analog Control Register */
#define REG_AFEC0_WPMR   (*(RwReg*)0x400B00E4U) /**< \brief (AFEC0) Write Protect Mode Register */
#define REG_AFEC0_WPSR   (*(RoReg*)0x400B00E8U) /**< \brief (AFEC0) Write Protect Status Register */
#define REG_AFEC0_RPR    (*(RwReg*)0x400B0100U) /**< \brief (AFEC0) Receive Pointer Register */
#define REG_AFEC0_RCR    (*(RwReg*)0x400B0104U) /**< \brief (AFEC0) Receive Counter Register */
#define REG_AFEC0_RNPR   (*(RwReg*)0x400B0110U) /**< \brief (AFEC0) Receive Next Pointer Register */
#define REG_AFEC0_RNCR   (*(RwReg*)0x400B0114U) /**< \brief (AFEC0) Receive Next Counter Register */
#define REG_AFEC0_PTCR   (*(WoReg*)0x400B0120U) /**< \brief (AFEC0) Transfer Control Register */
#define REG_AFEC0_PTSR   (*(RoReg*)0x400B0124U) /**< \brief (AFEC0) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_AFEC0_INSTANCE_ */
