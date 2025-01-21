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

#ifndef _SAM4E_SMC_INSTANCE_
#define _SAM4E_SMC_INSTANCE_

/* ========== Register definition for SMC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SMC_SETUP0          (0x40060000U) /**< \brief (SMC) SMC Setup Register (CS_number = 0) */
#define REG_SMC_PULSE0          (0x40060004U) /**< \brief (SMC) SMC Pulse Register (CS_number = 0) */
#define REG_SMC_CYCLE0          (0x40060008U) /**< \brief (SMC) SMC Cycle Register (CS_number = 0) */
#define REG_SMC_MODE0           (0x4006000CU) /**< \brief (SMC) SMC Mode Register (CS_number = 0) */
#define REG_SMC_SETUP1          (0x40060010U) /**< \brief (SMC) SMC Setup Register (CS_number = 1) */
#define REG_SMC_PULSE1          (0x40060014U) /**< \brief (SMC) SMC Pulse Register (CS_number = 1) */
#define REG_SMC_CYCLE1          (0x40060018U) /**< \brief (SMC) SMC Cycle Register (CS_number = 1) */
#define REG_SMC_MODE1           (0x4006001CU) /**< \brief (SMC) SMC Mode Register (CS_number = 1) */
#define REG_SMC_SETUP2          (0x40060020U) /**< \brief (SMC) SMC Setup Register (CS_number = 2) */
#define REG_SMC_PULSE2          (0x40060024U) /**< \brief (SMC) SMC Pulse Register (CS_number = 2) */
#define REG_SMC_CYCLE2          (0x40060028U) /**< \brief (SMC) SMC Cycle Register (CS_number = 2) */
#define REG_SMC_MODE2           (0x4006002CU) /**< \brief (SMC) SMC Mode Register (CS_number = 2) */
#define REG_SMC_SETUP3          (0x40060030U) /**< \brief (SMC) SMC Setup Register (CS_number = 3) */
#define REG_SMC_PULSE3          (0x40060034U) /**< \brief (SMC) SMC Pulse Register (CS_number = 3) */
#define REG_SMC_CYCLE3          (0x40060038U) /**< \brief (SMC) SMC Cycle Register (CS_number = 3) */
#define REG_SMC_MODE3           (0x4006003CU) /**< \brief (SMC) SMC Mode Register (CS_number = 3) */
#define REG_SMC_OCMS            (0x40060080U) /**< \brief (SMC) SMC OCMS MODE Register */
#define REG_SMC_KEY1            (0x40060084U) /**< \brief (SMC) SMC OCMS KEY1 Register */
#define REG_SMC_KEY2            (0x40060088U) /**< \brief (SMC) SMC OCMS KEY2 Register */
#define REG_SMC_WPMR            (0x400600E4U) /**< \brief (SMC) SMC Write Protect Mode Register */
#define REG_SMC_WPSR            (0x400600E8U) /**< \brief (SMC) SMC Write Protect Status Register */
#else
#define REG_SMC_SETUP0 (*(RwReg*)0x40060000U) /**< \brief (SMC) SMC Setup Register (CS_number = 0) */
#define REG_SMC_PULSE0 (*(RwReg*)0x40060004U) /**< \brief (SMC) SMC Pulse Register (CS_number = 0) */
#define REG_SMC_CYCLE0 (*(RwReg*)0x40060008U) /**< \brief (SMC) SMC Cycle Register (CS_number = 0) */
#define REG_SMC_MODE0  (*(RwReg*)0x4006000CU) /**< \brief (SMC) SMC Mode Register (CS_number = 0) */
#define REG_SMC_SETUP1 (*(RwReg*)0x40060010U) /**< \brief (SMC) SMC Setup Register (CS_number = 1) */
#define REG_SMC_PULSE1 (*(RwReg*)0x40060014U) /**< \brief (SMC) SMC Pulse Register (CS_number = 1) */
#define REG_SMC_CYCLE1 (*(RwReg*)0x40060018U) /**< \brief (SMC) SMC Cycle Register (CS_number = 1) */
#define REG_SMC_MODE1  (*(RwReg*)0x4006001CU) /**< \brief (SMC) SMC Mode Register (CS_number = 1) */
#define REG_SMC_SETUP2 (*(RwReg*)0x40060020U) /**< \brief (SMC) SMC Setup Register (CS_number = 2) */
#define REG_SMC_PULSE2 (*(RwReg*)0x40060024U) /**< \brief (SMC) SMC Pulse Register (CS_number = 2) */
#define REG_SMC_CYCLE2 (*(RwReg*)0x40060028U) /**< \brief (SMC) SMC Cycle Register (CS_number = 2) */
#define REG_SMC_MODE2  (*(RwReg*)0x4006002CU) /**< \brief (SMC) SMC Mode Register (CS_number = 2) */
#define REG_SMC_SETUP3 (*(RwReg*)0x40060030U) /**< \brief (SMC) SMC Setup Register (CS_number = 3) */
#define REG_SMC_PULSE3 (*(RwReg*)0x40060034U) /**< \brief (SMC) SMC Pulse Register (CS_number = 3) */
#define REG_SMC_CYCLE3 (*(RwReg*)0x40060038U) /**< \brief (SMC) SMC Cycle Register (CS_number = 3) */
#define REG_SMC_MODE3  (*(RwReg*)0x4006003CU) /**< \brief (SMC) SMC Mode Register (CS_number = 3) */
#define REG_SMC_OCMS   (*(RwReg*)0x40060080U) /**< \brief (SMC) SMC OCMS MODE Register */
#define REG_SMC_KEY1   (*(WoReg*)0x40060084U) /**< \brief (SMC) SMC OCMS KEY1 Register */
#define REG_SMC_KEY2   (*(WoReg*)0x40060088U) /**< \brief (SMC) SMC OCMS KEY2 Register */
#define REG_SMC_WPMR   (*(RwReg*)0x400600E4U) /**< \brief (SMC) SMC Write Protect Mode Register */
#define REG_SMC_WPSR   (*(RoReg*)0x400600E8U) /**< \brief (SMC) SMC Write Protect Status Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4E_SMC_INSTANCE_ */
