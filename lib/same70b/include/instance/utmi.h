/**
 * \file
 *
 * \brief Instance description for UTMI
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
#ifndef _SAME70_UTMI_INSTANCE_H_
#define _SAME70_UTMI_INSTANCE_H_

/* ========== Register definition for UTMI peripheral ========== */
#if (defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))

#define REG_UTMI_OHCIICR        (0x400E0410) /**< (UTMI) OHCI Interrupt Configuration Register */
#define REG_UTMI_CKTRIM         (0x400E0430) /**< (UTMI) UTMI Clock Trimming Register */

#else

#define REG_UTMI_OHCIICR        (*(__IO uint32_t*)0x400E0410U) /**< (UTMI) OHCI Interrupt Configuration Register */
#define REG_UTMI_CKTRIM         (*(__IO uint32_t*)0x400E0430U) /**< (UTMI) UTMI Clock Trimming Register */

#endif /* (defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
#endif /* _SAME70_UTMI_INSTANCE_ */
