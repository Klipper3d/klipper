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

#ifndef _SAM4E_AES_INSTANCE_
#define _SAM4E_AES_INSTANCE_

/* ========== Register definition for AES peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AES_CR                 (0x40004000U) /**< \brief (AES) Control Register */
#define REG_AES_MR                 (0x40004004U) /**< \brief (AES) Mode Register */
#define REG_AES_IER                (0x40004010U) /**< \brief (AES) Interrupt Enable Register */
#define REG_AES_IDR                (0x40004014U) /**< \brief (AES) Interrupt Disable Register */
#define REG_AES_IMR                (0x40004018U) /**< \brief (AES) Interrupt Mask Register */
#define REG_AES_ISR                (0x4000401CU) /**< \brief (AES) Interrupt Status Register */
#define REG_AES_KEYWR              (0x40004020U) /**< \brief (AES) Key Word Register */
#define REG_AES_IDATAR             (0x40004040U) /**< \brief (AES) Input Data Register */
#define REG_AES_ODATAR             (0x40004050U) /**< \brief (AES) Output Data Register */
#define REG_AES_IVR                (0x40004060U) /**< \brief (AES) Initialization Vector Register */
#else
#define REG_AES_CR        (*(WoReg*)0x40004000U) /**< \brief (AES) Control Register */
#define REG_AES_MR        (*(RwReg*)0x40004004U) /**< \brief (AES) Mode Register */
#define REG_AES_IER       (*(WoReg*)0x40004010U) /**< \brief (AES) Interrupt Enable Register */
#define REG_AES_IDR       (*(WoReg*)0x40004014U) /**< \brief (AES) Interrupt Disable Register */
#define REG_AES_IMR       (*(RoReg*)0x40004018U) /**< \brief (AES) Interrupt Mask Register */
#define REG_AES_ISR       (*(RoReg*)0x4000401CU) /**< \brief (AES) Interrupt Status Register */
#define REG_AES_KEYWR     (*(WoReg*)0x40004020U) /**< \brief (AES) Key Word Register */
#define REG_AES_IDATAR    (*(WoReg*)0x40004040U) /**< \brief (AES) Input Data Register */
#define REG_AES_ODATAR    (*(RoReg*)0x40004050U) /**< \brief (AES) Output Data Register */
#define REG_AES_IVR       (*(WoReg*)0x40004060U) /**< \brief (AES) Initialization Vector Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_AES_INSTANCE_ */
