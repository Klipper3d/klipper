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

#ifndef _SAM4E_MATRIX_INSTANCE_
#define _SAM4E_MATRIX_INSTANCE_

/* ========== Register definition for MATRIX peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_MATRIX_MCFG              (0x400E0200U) /**< \brief (MATRIX) Master Configuration Register */
#define REG_MATRIX_SCFG              (0x400E0240U) /**< \brief (MATRIX) Slave Configuration Register */
#define REG_MATRIX_PRAS0             (0x400E0280U) /**< \brief (MATRIX) Priority Register A for Slave 0 */
#define REG_MATRIX_PRBS0             (0x400E0284U) /**< \brief (MATRIX) Priority Register B for Slave 0 */
#define REG_MATRIX_PRAS1             (0x400E0288U) /**< \brief (MATRIX) Priority Register A for Slave 1 */
#define REG_MATRIX_PRBS1             (0x400E028CU) /**< \brief (MATRIX) Priority Register B for Slave 1 */
#define REG_MATRIX_PRAS2             (0x400E0290U) /**< \brief (MATRIX) Priority Register A for Slave 2 */
#define REG_MATRIX_PRBS2             (0x400E0294U) /**< \brief (MATRIX) Priority Register B for Slave 2 */
#define REG_MATRIX_PRAS3             (0x400E0298U) /**< \brief (MATRIX) Priority Register A for Slave 3 */
#define REG_MATRIX_PRBS3             (0x400E029CU) /**< \brief (MATRIX) Priority Register B for Slave 3 */
#define REG_MATRIX_PRAS4             (0x400E02A0U) /**< \brief (MATRIX) Priority Register A for Slave 4 */
#define REG_MATRIX_PRBS4             (0x400E02A4U) /**< \brief (MATRIX) Priority Register B for Slave 4 */
#define REG_MATRIX_PRAS5             (0x400E02A8U) /**< \brief (MATRIX) Priority Register A for Slave 5 */
#define REG_MATRIX_PRBS5             (0x400E02ACU) /**< \brief (MATRIX) Priority Register B for Slave 5 */
#define REG_MATRIX_PRAS6             (0x400E02B0U) /**< \brief (MATRIX) Priority Register A for Slave 6 */
#define REG_MATRIX_PRBS6             (0x400E02B4U) /**< \brief (MATRIX) Priority Register B for Slave 6 */
#define REG_MATRIX_PRAS7             (0x400E02B8U) /**< \brief (MATRIX) Priority Register A for Slave 7 */
#define REG_MATRIX_PRBS7             (0x400E02BCU) /**< \brief (MATRIX) Priority Register B for Slave 7 */
#define REG_MATRIX_PRAS8             (0x400E02C0U) /**< \brief (MATRIX) Priority Register A for Slave 8 */
#define REG_MATRIX_PRBS8             (0x400E02C4U) /**< \brief (MATRIX) Priority Register B for Slave 8 */
#define REG_MATRIX_PRAS9             (0x400E02C8U) /**< \brief (MATRIX) Priority Register A for Slave 9 */
#define REG_MATRIX_PRBS9             (0x400E02CCU) /**< \brief (MATRIX) Priority Register B for Slave 9 */
#define REG_MATRIX_PRAS10            (0x400E02D0U) /**< \brief (MATRIX) Priority Register A for Slave 10 */
#define REG_MATRIX_PRBS10            (0x400E02D4U) /**< \brief (MATRIX) Priority Register B for Slave 10 */
#define REG_MATRIX_PRAS11            (0x400E02D8U) /**< \brief (MATRIX) Priority Register A for Slave 11 */
#define REG_MATRIX_PRBS11            (0x400E02DCU) /**< \brief (MATRIX) Priority Register B for Slave 11 */
#define REG_MATRIX_PRAS12            (0x400E02E0U) /**< \brief (MATRIX) Priority Register A for Slave 12 */
#define REG_MATRIX_PRBS12            (0x400E02E4U) /**< \brief (MATRIX) Priority Register B for Slave 12 */
#define REG_MATRIX_PRAS13            (0x400E02E8U) /**< \brief (MATRIX) Priority Register A for Slave 13 */
#define REG_MATRIX_PRBS13            (0x400E02ECU) /**< \brief (MATRIX) Priority Register B for Slave 13 */
#define REG_MATRIX_PRAS14            (0x400E02F0U) /**< \brief (MATRIX) Priority Register A for Slave 14 */
#define REG_MATRIX_PRBS14            (0x400E02F4U) /**< \brief (MATRIX) Priority Register B for Slave 14 */
#define REG_MATRIX_PRAS15            (0x400E02F8U) /**< \brief (MATRIX) Priority Register A for Slave 15 */
#define REG_MATRIX_PRBS15            (0x400E02FCU) /**< \brief (MATRIX) Priority Register B for Slave 15 */
#define REG_MATRIX_MRCR              (0x400E0300U) /**< \brief (MATRIX) Master Remap Control Register */
#define REG_MATRIX_SFR               (0x400E0310U) /**< \brief (MATRIX) Special Function Register */
#define REG_MATRIX_WPMR              (0x400E03E4U) /**< \brief (MATRIX) Write Protect Mode Register */
#define REG_MATRIX_WPSR              (0x400E03E8U) /**< \brief (MATRIX) Write Protect Status Register */
#else
#define REG_MATRIX_MCFG     (*(RwReg*)0x400E0200U) /**< \brief (MATRIX) Master Configuration Register */
#define REG_MATRIX_SCFG     (*(RwReg*)0x400E0240U) /**< \brief (MATRIX) Slave Configuration Register */
#define REG_MATRIX_PRAS0    (*(RwReg*)0x400E0280U) /**< \brief (MATRIX) Priority Register A for Slave 0 */
#define REG_MATRIX_PRBS0    (*(RwReg*)0x400E0284U) /**< \brief (MATRIX) Priority Register B for Slave 0 */
#define REG_MATRIX_PRAS1    (*(RwReg*)0x400E0288U) /**< \brief (MATRIX) Priority Register A for Slave 1 */
#define REG_MATRIX_PRBS1    (*(RwReg*)0x400E028CU) /**< \brief (MATRIX) Priority Register B for Slave 1 */
#define REG_MATRIX_PRAS2    (*(RwReg*)0x400E0290U) /**< \brief (MATRIX) Priority Register A for Slave 2 */
#define REG_MATRIX_PRBS2    (*(RwReg*)0x400E0294U) /**< \brief (MATRIX) Priority Register B for Slave 2 */
#define REG_MATRIX_PRAS3    (*(RwReg*)0x400E0298U) /**< \brief (MATRIX) Priority Register A for Slave 3 */
#define REG_MATRIX_PRBS3    (*(RwReg*)0x400E029CU) /**< \brief (MATRIX) Priority Register B for Slave 3 */
#define REG_MATRIX_PRAS4    (*(RwReg*)0x400E02A0U) /**< \brief (MATRIX) Priority Register A for Slave 4 */
#define REG_MATRIX_PRBS4    (*(RwReg*)0x400E02A4U) /**< \brief (MATRIX) Priority Register B for Slave 4 */
#define REG_MATRIX_PRAS5    (*(RwReg*)0x400E02A8U) /**< \brief (MATRIX) Priority Register A for Slave 5 */
#define REG_MATRIX_PRBS5    (*(RwReg*)0x400E02ACU) /**< \brief (MATRIX) Priority Register B for Slave 5 */
#define REG_MATRIX_PRAS6    (*(RwReg*)0x400E02B0U) /**< \brief (MATRIX) Priority Register A for Slave 6 */
#define REG_MATRIX_PRBS6    (*(RwReg*)0x400E02B4U) /**< \brief (MATRIX) Priority Register B for Slave 6 */
#define REG_MATRIX_PRAS7    (*(RwReg*)0x400E02B8U) /**< \brief (MATRIX) Priority Register A for Slave 7 */
#define REG_MATRIX_PRBS7    (*(RwReg*)0x400E02BCU) /**< \brief (MATRIX) Priority Register B for Slave 7 */
#define REG_MATRIX_PRAS8    (*(RwReg*)0x400E02C0U) /**< \brief (MATRIX) Priority Register A for Slave 8 */
#define REG_MATRIX_PRBS8    (*(RwReg*)0x400E02C4U) /**< \brief (MATRIX) Priority Register B for Slave 8 */
#define REG_MATRIX_PRAS9    (*(RwReg*)0x400E02C8U) /**< \brief (MATRIX) Priority Register A for Slave 9 */
#define REG_MATRIX_PRBS9    (*(RwReg*)0x400E02CCU) /**< \brief (MATRIX) Priority Register B for Slave 9 */
#define REG_MATRIX_PRAS10   (*(RwReg*)0x400E02D0U) /**< \brief (MATRIX) Priority Register A for Slave 10 */
#define REG_MATRIX_PRBS10   (*(RwReg*)0x400E02D4U) /**< \brief (MATRIX) Priority Register B for Slave 10 */
#define REG_MATRIX_PRAS11   (*(RwReg*)0x400E02D8U) /**< \brief (MATRIX) Priority Register A for Slave 11 */
#define REG_MATRIX_PRBS11   (*(RwReg*)0x400E02DCU) /**< \brief (MATRIX) Priority Register B for Slave 11 */
#define REG_MATRIX_PRAS12   (*(RwReg*)0x400E02E0U) /**< \brief (MATRIX) Priority Register A for Slave 12 */
#define REG_MATRIX_PRBS12   (*(RwReg*)0x400E02E4U) /**< \brief (MATRIX) Priority Register B for Slave 12 */
#define REG_MATRIX_PRAS13   (*(RwReg*)0x400E02E8U) /**< \brief (MATRIX) Priority Register A for Slave 13 */
#define REG_MATRIX_PRBS13   (*(RwReg*)0x400E02ECU) /**< \brief (MATRIX) Priority Register B for Slave 13 */
#define REG_MATRIX_PRAS14   (*(RwReg*)0x400E02F0U) /**< \brief (MATRIX) Priority Register A for Slave 14 */
#define REG_MATRIX_PRBS14   (*(RwReg*)0x400E02F4U) /**< \brief (MATRIX) Priority Register B for Slave 14 */
#define REG_MATRIX_PRAS15   (*(RwReg*)0x400E02F8U) /**< \brief (MATRIX) Priority Register A for Slave 15 */
#define REG_MATRIX_PRBS15   (*(RwReg*)0x400E02FCU) /**< \brief (MATRIX) Priority Register B for Slave 15 */
#define REG_MATRIX_MRCR     (*(RwReg*)0x400E0300U) /**< \brief (MATRIX) Master Remap Control Register */
#define REG_MATRIX_SFR      (*(RwReg*)0x400E0310U) /**< \brief (MATRIX) Special Function Register */
#define REG_MATRIX_WPMR     (*(RwReg*)0x400E03E4U) /**< \brief (MATRIX) Write Protect Mode Register */
#define REG_MATRIX_WPSR     (*(RoReg*)0x400E03E8U) /**< \brief (MATRIX) Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_MATRIX_INSTANCE_ */
