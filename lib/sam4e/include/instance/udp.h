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

#ifndef _SAM4E_UDP_INSTANCE_
#define _SAM4E_UDP_INSTANCE_

/* ========== Register definition for UDP peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_UDP_FRM_NUM           (0x40084000U) /**< \brief (UDP) Frame Number Register */
#define REG_UDP_GLB_STAT          (0x40084004U) /**< \brief (UDP) Global State Register */
#define REG_UDP_FADDR             (0x40084008U) /**< \brief (UDP) Function Address Register */
#define REG_UDP_IER               (0x40084010U) /**< \brief (UDP) Interrupt Enable Register */
#define REG_UDP_IDR               (0x40084014U) /**< \brief (UDP) Interrupt Disable Register */
#define REG_UDP_IMR               (0x40084018U) /**< \brief (UDP) Interrupt Mask Register */
#define REG_UDP_ISR               (0x4008401CU) /**< \brief (UDP) Interrupt Status Register */
#define REG_UDP_ICR               (0x40084020U) /**< \brief (UDP) Interrupt Clear Register */
#define REG_UDP_RST_EP            (0x40084028U) /**< \brief (UDP) Reset Endpoint Register */
#define REG_UDP_CSR               (0x40084030U) /**< \brief (UDP) Endpoint Control and Status Register */
#define REG_UDP_FDR               (0x40084050U) /**< \brief (UDP) Endpoint FIFO Data Register */
#define REG_UDP_TXVC              (0x40084074U) /**< \brief (UDP) Transceiver Control Register */
#else
#define REG_UDP_FRM_NUM  (*(RoReg*)0x40084000U) /**< \brief (UDP) Frame Number Register */
#define REG_UDP_GLB_STAT (*(RwReg*)0x40084004U) /**< \brief (UDP) Global State Register */
#define REG_UDP_FADDR    (*(RwReg*)0x40084008U) /**< \brief (UDP) Function Address Register */
#define REG_UDP_IER      (*(WoReg*)0x40084010U) /**< \brief (UDP) Interrupt Enable Register */
#define REG_UDP_IDR      (*(WoReg*)0x40084014U) /**< \brief (UDP) Interrupt Disable Register */
#define REG_UDP_IMR      (*(RoReg*)0x40084018U) /**< \brief (UDP) Interrupt Mask Register */
#define REG_UDP_ISR      (*(RoReg*)0x4008401CU) /**< \brief (UDP) Interrupt Status Register */
#define REG_UDP_ICR      (*(WoReg*)0x40084020U) /**< \brief (UDP) Interrupt Clear Register */
#define REG_UDP_RST_EP   (*(RwReg*)0x40084028U) /**< \brief (UDP) Reset Endpoint Register */
#define REG_UDP_CSR      (*(RwReg*)0x40084030U) /**< \brief (UDP) Endpoint Control and Status Register */
#define REG_UDP_FDR      (*(RwReg*)0x40084050U) /**< \brief (UDP) Endpoint FIFO Data Register */
#define REG_UDP_TXVC     (*(RwReg*)0x40084074U) /**< \brief (UDP) Transceiver Control Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_UDP_INSTANCE_ */
