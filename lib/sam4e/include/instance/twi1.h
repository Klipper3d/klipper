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

#ifndef _SAM4E_TWI1_INSTANCE_
#define _SAM4E_TWI1_INSTANCE_

/* ========== Register definition for TWI1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_TWI1_CR                    (0x400AC000U) /**< \brief (TWI1) Control Register */
#define REG_TWI1_MMR                   (0x400AC004U) /**< \brief (TWI1) Master Mode Register */
#define REG_TWI1_SMR                   (0x400AC008U) /**< \brief (TWI1) Slave Mode Register */
#define REG_TWI1_IADR                  (0x400AC00CU) /**< \brief (TWI1) Internal Address Register */
#define REG_TWI1_CWGR                  (0x400AC010U) /**< \brief (TWI1) Clock Waveform Generator Register */
#define REG_TWI1_SR                    (0x400AC020U) /**< \brief (TWI1) Status Register */
#define REG_TWI1_IER                   (0x400AC024U) /**< \brief (TWI1) Interrupt Enable Register */
#define REG_TWI1_IDR                   (0x400AC028U) /**< \brief (TWI1) Interrupt Disable Register */
#define REG_TWI1_IMR                   (0x400AC02CU) /**< \brief (TWI1) Interrupt Mask Register */
#define REG_TWI1_RHR                   (0x400AC030U) /**< \brief (TWI1) Receive Holding Register */
#define REG_TWI1_THR                   (0x400AC034U) /**< \brief (TWI1) Transmit Holding Register */
#define REG_TWI1_WPROT_MODE            (0x400AC0E4U) /**< \brief (TWI1) Protection Mode Register */
#define REG_TWI1_WPROT_STATUS          (0x400AC0E8U) /**< \brief (TWI1) Protection Status Register */
#define REG_TWI1_RPR                   (0x400AC100U) /**< \brief (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR                   (0x400AC104U) /**< \brief (TWI1) Receive Counter Register */
#define REG_TWI1_TPR                   (0x400AC108U) /**< \brief (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR                   (0x400AC10CU) /**< \brief (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR                  (0x400AC110U) /**< \brief (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR                  (0x400AC114U) /**< \brief (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR                  (0x400AC118U) /**< \brief (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR                  (0x400AC11CU) /**< \brief (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR                  (0x400AC120U) /**< \brief (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR                  (0x400AC124U) /**< \brief (TWI1) Transfer Status Register */
#else
#define REG_TWI1_CR           (*(WoReg*)0x400AC000U) /**< \brief (TWI1) Control Register */
#define REG_TWI1_MMR          (*(RwReg*)0x400AC004U) /**< \brief (TWI1) Master Mode Register */
#define REG_TWI1_SMR          (*(RwReg*)0x400AC008U) /**< \brief (TWI1) Slave Mode Register */
#define REG_TWI1_IADR         (*(RwReg*)0x400AC00CU) /**< \brief (TWI1) Internal Address Register */
#define REG_TWI1_CWGR         (*(RwReg*)0x400AC010U) /**< \brief (TWI1) Clock Waveform Generator Register */
#define REG_TWI1_SR           (*(RoReg*)0x400AC020U) /**< \brief (TWI1) Status Register */
#define REG_TWI1_IER          (*(WoReg*)0x400AC024U) /**< \brief (TWI1) Interrupt Enable Register */
#define REG_TWI1_IDR          (*(WoReg*)0x400AC028U) /**< \brief (TWI1) Interrupt Disable Register */
#define REG_TWI1_IMR          (*(RoReg*)0x400AC02CU) /**< \brief (TWI1) Interrupt Mask Register */
#define REG_TWI1_RHR          (*(RoReg*)0x400AC030U) /**< \brief (TWI1) Receive Holding Register */
#define REG_TWI1_THR          (*(WoReg*)0x400AC034U) /**< \brief (TWI1) Transmit Holding Register */
#define REG_TWI1_WPROT_MODE   (*(RwReg*)0x400AC0E4U) /**< \brief (TWI1) Protection Mode Register */
#define REG_TWI1_WPROT_STATUS (*(RoReg*)0x400AC0E8U) /**< \brief (TWI1) Protection Status Register */
#define REG_TWI1_RPR          (*(RwReg*)0x400AC100U) /**< \brief (TWI1) Receive Pointer Register */
#define REG_TWI1_RCR          (*(RwReg*)0x400AC104U) /**< \brief (TWI1) Receive Counter Register */
#define REG_TWI1_TPR          (*(RwReg*)0x400AC108U) /**< \brief (TWI1) Transmit Pointer Register */
#define REG_TWI1_TCR          (*(RwReg*)0x400AC10CU) /**< \brief (TWI1) Transmit Counter Register */
#define REG_TWI1_RNPR         (*(RwReg*)0x400AC110U) /**< \brief (TWI1) Receive Next Pointer Register */
#define REG_TWI1_RNCR         (*(RwReg*)0x400AC114U) /**< \brief (TWI1) Receive Next Counter Register */
#define REG_TWI1_TNPR         (*(RwReg*)0x400AC118U) /**< \brief (TWI1) Transmit Next Pointer Register */
#define REG_TWI1_TNCR         (*(RwReg*)0x400AC11CU) /**< \brief (TWI1) Transmit Next Counter Register */
#define REG_TWI1_PTCR         (*(WoReg*)0x400AC120U) /**< \brief (TWI1) Transfer Control Register */
#define REG_TWI1_PTSR         (*(RoReg*)0x400AC124U) /**< \brief (TWI1) Transfer Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_TWI1_INSTANCE_ */
