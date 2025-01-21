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

#ifndef _SAM4E_ACC_INSTANCE_
#define _SAM4E_ACC_INSTANCE_

/* ========== Register definition for ACC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_ACC_CR            (0x400BC000U) /**< \brief (ACC) Control Register */
#define REG_ACC_MR            (0x400BC004U) /**< \brief (ACC) Mode Register */
#define REG_ACC_IER           (0x400BC024U) /**< \brief (ACC) Interrupt Enable Register */
#define REG_ACC_IDR           (0x400BC028U) /**< \brief (ACC) Interrupt Disable Register */
#define REG_ACC_IMR           (0x400BC02CU) /**< \brief (ACC) Interrupt Mask Register */
#define REG_ACC_ISR           (0x400BC030U) /**< \brief (ACC) Interrupt Status Register */
#define REG_ACC_ACR           (0x400BC094U) /**< \brief (ACC) Analog Control Register */
#define REG_ACC_WPMR          (0x400BC0E4U) /**< \brief (ACC) Write Protect Mode Register */
#define REG_ACC_WPSR          (0x400BC0E8U) /**< \brief (ACC) Write Protect Status Register */
#else
#define REG_ACC_CR   (*(WoReg*)0x400BC000U) /**< \brief (ACC) Control Register */
#define REG_ACC_MR   (*(RwReg*)0x400BC004U) /**< \brief (ACC) Mode Register */
#define REG_ACC_IER  (*(WoReg*)0x400BC024U) /**< \brief (ACC) Interrupt Enable Register */
#define REG_ACC_IDR  (*(WoReg*)0x400BC028U) /**< \brief (ACC) Interrupt Disable Register */
#define REG_ACC_IMR  (*(RoReg*)0x400BC02CU) /**< \brief (ACC) Interrupt Mask Register */
#define REG_ACC_ISR  (*(RoReg*)0x400BC030U) /**< \brief (ACC) Interrupt Status Register */
#define REG_ACC_ACR  (*(RwReg*)0x400BC094U) /**< \brief (ACC) Analog Control Register */
#define REG_ACC_WPMR (*(RwReg*)0x400BC0E4U) /**< \brief (ACC) Write Protect Mode Register */
#define REG_ACC_WPSR (*(RoReg*)0x400BC0E8U) /**< \brief (ACC) Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_ACC_INSTANCE_ */
