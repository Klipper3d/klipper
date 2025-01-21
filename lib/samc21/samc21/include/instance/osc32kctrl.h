/**
 * \file
 *
 * \brief Instance description for OSC32KCTRL
 *
 * Copyright (c) 2018 Microchip Technology Inc.
 *
 * \asf_license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the Licence at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \asf_license_stop
 *
 */

#ifndef _SAMC21_OSC32KCTRL_INSTANCE_
#define _SAMC21_OSC32KCTRL_INSTANCE_

/* ========== Register definition for OSC32KCTRL peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_OSC32KCTRL_INTENCLR    (0x40001400) /**< \brief (OSC32KCTRL) Interrupt Enable Clear */
#define REG_OSC32KCTRL_INTENSET    (0x40001404) /**< \brief (OSC32KCTRL) Interrupt Enable Set */
#define REG_OSC32KCTRL_INTFLAG     (0x40001408) /**< \brief (OSC32KCTRL) Interrupt Flag Status and Clear */
#define REG_OSC32KCTRL_STATUS      (0x4000140C) /**< \brief (OSC32KCTRL) Power and Clocks Status */
#define REG_OSC32KCTRL_RTCCTRL     (0x40001410) /**< \brief (OSC32KCTRL) Clock selection */
#define REG_OSC32KCTRL_XOSC32K     (0x40001414) /**< \brief (OSC32KCTRL) 32kHz External Crystal Oscillator (XOSC32K) Control */
#define REG_OSC32KCTRL_CFDCTRL     (0x40001416) /**< \brief (OSC32KCTRL) Clock Failure Detector Control */
#define REG_OSC32KCTRL_EVCTRL      (0x40001417) /**< \brief (OSC32KCTRL) Event Control */
#define REG_OSC32KCTRL_OSC32K      (0x40001418) /**< \brief (OSC32KCTRL) 32kHz Internal Oscillator (OSC32K) Control */
#define REG_OSC32KCTRL_OSCULP32K   (0x4000141C) /**< \brief (OSC32KCTRL) 32kHz Ultra Low Power Internal Oscillator (OSCULP32K) Control */
#else
#define REG_OSC32KCTRL_INTENCLR    (*(RwReg  *)0x40001400UL) /**< \brief (OSC32KCTRL) Interrupt Enable Clear */
#define REG_OSC32KCTRL_INTENSET    (*(RwReg  *)0x40001404UL) /**< \brief (OSC32KCTRL) Interrupt Enable Set */
#define REG_OSC32KCTRL_INTFLAG     (*(RwReg  *)0x40001408UL) /**< \brief (OSC32KCTRL) Interrupt Flag Status and Clear */
#define REG_OSC32KCTRL_STATUS      (*(RoReg  *)0x4000140CUL) /**< \brief (OSC32KCTRL) Power and Clocks Status */
#define REG_OSC32KCTRL_RTCCTRL     (*(RwReg  *)0x40001410UL) /**< \brief (OSC32KCTRL) Clock selection */
#define REG_OSC32KCTRL_XOSC32K     (*(RwReg16*)0x40001414UL) /**< \brief (OSC32KCTRL) 32kHz External Crystal Oscillator (XOSC32K) Control */
#define REG_OSC32KCTRL_CFDCTRL     (*(RwReg8 *)0x40001416UL) /**< \brief (OSC32KCTRL) Clock Failure Detector Control */
#define REG_OSC32KCTRL_EVCTRL      (*(RwReg8 *)0x40001417UL) /**< \brief (OSC32KCTRL) Event Control */
#define REG_OSC32KCTRL_OSC32K      (*(RwReg  *)0x40001418UL) /**< \brief (OSC32KCTRL) 32kHz Internal Oscillator (OSC32K) Control */
#define REG_OSC32KCTRL_OSCULP32K   (*(RwReg  *)0x4000141CUL) /**< \brief (OSC32KCTRL) 32kHz Ultra Low Power Internal Oscillator (OSCULP32K) Control */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for OSC32KCTRL peripheral ========== */
#define OSC32KCTRL_OSC32K_COARSE_CALIB_MSB 6       

#endif /* _SAMC21_OSC32KCTRL_INSTANCE_ */
