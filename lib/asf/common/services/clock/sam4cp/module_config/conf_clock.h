/**
 * \file
 *
 * \brief SAM4CP clock configuration.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

// ===== System Clock (MCK) Source Options
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_XTAL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_BYPASS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_4M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_8M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_12M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_XTAL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_BYPASS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK

// ===== System Clock (MCK) Prescaler Options   (Fmck = Fsys / (SYSCLK_PRES))
#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_2
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_4
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_8
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_16
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_32
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_64
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_3

// ===== PLL0 (A) Options   (8.192M = (32.768K * 250) / 1)
//#define CONFIG_PLL0_SOURCE          PLLA_SRC_SLCK_32K_XTAL

// ===== PLL1 (B) Options   (Fpll = (Fclk * PLL_mul) / PLL_div)
// Use mul and div effective values here.
#define CONFIG_PLL1_SOURCE          PLLB_SRC_MAINCK_BYPASS
#define CONFIG_PLL1_MUL             (240000000UL / BOARD_FREQ_MAINCK_BYPASS)
#define CONFIG_PLL1_DIV             2

// ===== Coprocessor System Clock (CPMCK) Options
// Fcpmck = Fcpclk_source / CPCLK_PRES

// Note:
// CONFIG_CPCLK_ENABLE  MUST be defined if using peripherals on bus matrix 1.
#define CONFIG_CPCLK_ENABLE

// Coprocessor System Clock Source Options
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_SLCK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_MAINCK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLACK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLBCK
#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_MCK

// Coprocessor System Clock Prescaler Options (CPCLK_PRES may be 1 to 16).
#define CONFIG_CPCLK_PRES           1

// ===== Main processor frequency (MCK)
// - Bypass mode frequency: 10MHz
// - System clock source: PLLB
// - System clock prescaler: 1 (divided by 1)
// - PLLB source: BYPASS
// - PLLB output: BYPASS * 24 / 2
// - System clock: 10 * 24 / 2 / 1 = 120MHz
//
// ===== Coprocessor frequency (CPMCK)
// - Coprocessor system clock source: MCK
// - Coprocessor system clock prescaler: 1 (divided by 1)
// - Coprocessor system clock: 120MHz / 1 = 120MHz


#endif /* CONF_CLOCK_H_INCLUDED */
