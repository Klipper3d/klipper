/**
 * \file
 *
 * \brief Instance description for RSWDT
 *
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 * \license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \license_stop
 *
 */

/* file generated from device description version 2019-01-18T21:19:59Z */
#ifndef _SAME70_RSWDT_INSTANCE_H_
#define _SAME70_RSWDT_INSTANCE_H_

/* ========== Register definition for RSWDT peripheral ========== */
#if (defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))

#define REG_RSWDT_CR            (0x400E1900) /**< (RSWDT) Control Register */
#define REG_RSWDT_MR            (0x400E1904) /**< (RSWDT) Mode Register */
#define REG_RSWDT_SR            (0x400E1908) /**< (RSWDT) Status Register */

#else

#define REG_RSWDT_CR            (*(__O  uint32_t*)0x400E1900U) /**< (RSWDT) Control Register */
#define REG_RSWDT_MR            (*(__IO uint32_t*)0x400E1904U) /**< (RSWDT) Mode Register */
#define REG_RSWDT_SR            (*(__I  uint32_t*)0x400E1908U) /**< (RSWDT) Status Register */

#endif /* (defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance Parameter definitions for RSWDT peripheral ========== */
#define RSWDT_INSTANCE_ID                        63         

#endif /* _SAME70_RSWDT_INSTANCE_ */
