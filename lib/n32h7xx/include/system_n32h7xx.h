/*****************************************************************************
 * Copyright (c) 2026, Nsing Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
*\*\file system_n32h7xx.h
*\*\author Nsing
*\*\version v1.0.0
*\*\copyright Copyright (c) 2026, Nsing Technologies Inc. All rights reserved.
 */
#ifndef __SYSTEM_N32H7XX_H__
#define __SYSTEM_N32H7XX_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/** _System */

/** Power supply source configuration **/
#define PWR_SUPPLY_MODE_MASK               (PWR_SYSCTRL4_MLDOEN | PWR_SYSCTRL4_DCDCEN | PWR_SYSCTRL4_VCORESRC | PWR_SYSCTRL4_DCDCFRCEN)
#define PWR_LDO_SUPPLY                     (PWR_SYSCTRL4_MLDOEN)             /* Core domains are supplied from the LDO  */
#define PWR_DIRECT_SMPS_SUPPLY             (PWR_SYSCTRL4_DCDCEN)             /* Core domains are supplied from the SMPS */
#define PWR_EXTERNAL_SOURCE_SUPPLY         (PWR_SYSCTRL4_VCORESRC)           /* The SMPS and the LDO are Bypassed. The Core domains are supplied from an external source */

/** NRST Analog and Digital Filter configuration **/
#define PWR_RST_AGFBPEN_MAST              (PWR_SYSCTRL1_AGF_ARSTOBP)
#define PWR_RST_DGFBPEN_MAST              (PWR_SYSCTRL1_NRST_DGFBP)
#define PWR_RST_DGF_CNT_MAST              (PWR_SYSCTRL1_NRST_DGFCNT)
#define PWR_RST_DGF_CNT_DEFAULT           ((uint32_t)0x200000U)

extern uint32_t SystemCoreClock; /* System Clock Frequency (Core Clock) */

extern void SystemInit(void);
#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_N32H7XX_H__ */
