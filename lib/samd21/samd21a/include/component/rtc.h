/**
 * \file
 *
 * \brief Component description for RTC
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

#ifndef _SAMD21_RTC_COMPONENT_
#define _SAMD21_RTC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR RTC */
/* ========================================================================== */
/** \addtogroup SAMD21_RTC Real-Time Counter */
/*@{*/

#define RTC_U2202
#define REV_RTC                     0x101

/* -------- RTC_MODE0_CTRL : (RTC Offset: 0x00) (R/W 16) MODE0 MODE0 Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Operating Mode                     */
    uint16_t :3;               /*!< bit:  4.. 6  Reserved                           */
    uint16_t MATCHCLR:1;       /*!< bit:      7  Clear on Match                     */
    uint16_t PRESCALER:4;      /*!< bit:  8..11  Prescaler                          */
    uint16_t :4;               /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE0_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_CTRL_OFFSET       0x00         /**< \brief (RTC_MODE0_CTRL offset) MODE0 Control */
#define RTC_MODE0_CTRL_RESETVALUE   _U_(0x0000)  /**< \brief (RTC_MODE0_CTRL reset_value) MODE0 Control */

#define RTC_MODE0_CTRL_SWRST_Pos    0            /**< \brief (RTC_MODE0_CTRL) Software Reset */
#define RTC_MODE0_CTRL_SWRST        (_U_(0x1) << RTC_MODE0_CTRL_SWRST_Pos)
#define RTC_MODE0_CTRL_ENABLE_Pos   1            /**< \brief (RTC_MODE0_CTRL) Enable */
#define RTC_MODE0_CTRL_ENABLE       (_U_(0x1) << RTC_MODE0_CTRL_ENABLE_Pos)
#define RTC_MODE0_CTRL_MODE_Pos     2            /**< \brief (RTC_MODE0_CTRL) Operating Mode */
#define RTC_MODE0_CTRL_MODE_Msk     (_U_(0x3) << RTC_MODE0_CTRL_MODE_Pos)
#define RTC_MODE0_CTRL_MODE(value)  (RTC_MODE0_CTRL_MODE_Msk & ((value) << RTC_MODE0_CTRL_MODE_Pos))
#define   RTC_MODE0_CTRL_MODE_COUNT32_Val _U_(0x0)   /**< \brief (RTC_MODE0_CTRL) Mode 0: 32-bit Counter */
#define   RTC_MODE0_CTRL_MODE_COUNT16_Val _U_(0x1)   /**< \brief (RTC_MODE0_CTRL) Mode 1: 16-bit Counter */
#define   RTC_MODE0_CTRL_MODE_CLOCK_Val   _U_(0x2)   /**< \brief (RTC_MODE0_CTRL) Mode 2: Clock/Calendar */
#define RTC_MODE0_CTRL_MODE_COUNT32 (RTC_MODE0_CTRL_MODE_COUNT32_Val << RTC_MODE0_CTRL_MODE_Pos)
#define RTC_MODE0_CTRL_MODE_COUNT16 (RTC_MODE0_CTRL_MODE_COUNT16_Val << RTC_MODE0_CTRL_MODE_Pos)
#define RTC_MODE0_CTRL_MODE_CLOCK   (RTC_MODE0_CTRL_MODE_CLOCK_Val << RTC_MODE0_CTRL_MODE_Pos)
#define RTC_MODE0_CTRL_MATCHCLR_Pos 7            /**< \brief (RTC_MODE0_CTRL) Clear on Match */
#define RTC_MODE0_CTRL_MATCHCLR     (_U_(0x1) << RTC_MODE0_CTRL_MATCHCLR_Pos)
#define RTC_MODE0_CTRL_PRESCALER_Pos 8            /**< \brief (RTC_MODE0_CTRL) Prescaler */
#define RTC_MODE0_CTRL_PRESCALER_Msk (_U_(0xF) << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER(value) (RTC_MODE0_CTRL_PRESCALER_Msk & ((value) << RTC_MODE0_CTRL_PRESCALER_Pos))
#define   RTC_MODE0_CTRL_PRESCALER_DIV1_Val _U_(0x0)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/1 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV2_Val _U_(0x1)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/2 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV4_Val _U_(0x2)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/4 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV8_Val _U_(0x3)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/8 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV16_Val _U_(0x4)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/16 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV32_Val _U_(0x5)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/32 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV64_Val _U_(0x6)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/64 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV128_Val _U_(0x7)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/128 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV256_Val _U_(0x8)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/256 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV512_Val _U_(0x9)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/512 */
#define   RTC_MODE0_CTRL_PRESCALER_DIV1024_Val _U_(0xA)   /**< \brief (RTC_MODE0_CTRL) CLK_RTC_CNT = GCLK_RTC/1024 */
#define RTC_MODE0_CTRL_PRESCALER_DIV1 (RTC_MODE0_CTRL_PRESCALER_DIV1_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV2 (RTC_MODE0_CTRL_PRESCALER_DIV2_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV4 (RTC_MODE0_CTRL_PRESCALER_DIV4_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV8 (RTC_MODE0_CTRL_PRESCALER_DIV8_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV16 (RTC_MODE0_CTRL_PRESCALER_DIV16_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV32 (RTC_MODE0_CTRL_PRESCALER_DIV32_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV64 (RTC_MODE0_CTRL_PRESCALER_DIV64_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV128 (RTC_MODE0_CTRL_PRESCALER_DIV128_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV256 (RTC_MODE0_CTRL_PRESCALER_DIV256_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV512 (RTC_MODE0_CTRL_PRESCALER_DIV512_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_PRESCALER_DIV1024 (RTC_MODE0_CTRL_PRESCALER_DIV1024_Val << RTC_MODE0_CTRL_PRESCALER_Pos)
#define RTC_MODE0_CTRL_MASK         _U_(0x0F8F)  /**< \brief (RTC_MODE0_CTRL) MASK Register */

/* -------- RTC_MODE1_CTRL : (RTC Offset: 0x00) (R/W 16) MODE1 MODE1 Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Operating Mode                     */
    uint16_t :4;               /*!< bit:  4.. 7  Reserved                           */
    uint16_t PRESCALER:4;      /*!< bit:  8..11  Prescaler                          */
    uint16_t :4;               /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_CTRL_OFFSET       0x00         /**< \brief (RTC_MODE1_CTRL offset) MODE1 Control */
#define RTC_MODE1_CTRL_RESETVALUE   _U_(0x0000)  /**< \brief (RTC_MODE1_CTRL reset_value) MODE1 Control */

#define RTC_MODE1_CTRL_SWRST_Pos    0            /**< \brief (RTC_MODE1_CTRL) Software Reset */
#define RTC_MODE1_CTRL_SWRST        (_U_(0x1) << RTC_MODE1_CTRL_SWRST_Pos)
#define RTC_MODE1_CTRL_ENABLE_Pos   1            /**< \brief (RTC_MODE1_CTRL) Enable */
#define RTC_MODE1_CTRL_ENABLE       (_U_(0x1) << RTC_MODE1_CTRL_ENABLE_Pos)
#define RTC_MODE1_CTRL_MODE_Pos     2            /**< \brief (RTC_MODE1_CTRL) Operating Mode */
#define RTC_MODE1_CTRL_MODE_Msk     (_U_(0x3) << RTC_MODE1_CTRL_MODE_Pos)
#define RTC_MODE1_CTRL_MODE(value)  (RTC_MODE1_CTRL_MODE_Msk & ((value) << RTC_MODE1_CTRL_MODE_Pos))
#define   RTC_MODE1_CTRL_MODE_COUNT32_Val _U_(0x0)   /**< \brief (RTC_MODE1_CTRL) Mode 0: 32-bit Counter */
#define   RTC_MODE1_CTRL_MODE_COUNT16_Val _U_(0x1)   /**< \brief (RTC_MODE1_CTRL) Mode 1: 16-bit Counter */
#define   RTC_MODE1_CTRL_MODE_CLOCK_Val   _U_(0x2)   /**< \brief (RTC_MODE1_CTRL) Mode 2: Clock/Calendar */
#define RTC_MODE1_CTRL_MODE_COUNT32 (RTC_MODE1_CTRL_MODE_COUNT32_Val << RTC_MODE1_CTRL_MODE_Pos)
#define RTC_MODE1_CTRL_MODE_COUNT16 (RTC_MODE1_CTRL_MODE_COUNT16_Val << RTC_MODE1_CTRL_MODE_Pos)
#define RTC_MODE1_CTRL_MODE_CLOCK   (RTC_MODE1_CTRL_MODE_CLOCK_Val << RTC_MODE1_CTRL_MODE_Pos)
#define RTC_MODE1_CTRL_PRESCALER_Pos 8            /**< \brief (RTC_MODE1_CTRL) Prescaler */
#define RTC_MODE1_CTRL_PRESCALER_Msk (_U_(0xF) << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER(value) (RTC_MODE1_CTRL_PRESCALER_Msk & ((value) << RTC_MODE1_CTRL_PRESCALER_Pos))
#define   RTC_MODE1_CTRL_PRESCALER_DIV1_Val _U_(0x0)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/1 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV2_Val _U_(0x1)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/2 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV4_Val _U_(0x2)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/4 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV8_Val _U_(0x3)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/8 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV16_Val _U_(0x4)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/16 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV32_Val _U_(0x5)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/32 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV64_Val _U_(0x6)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/64 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV128_Val _U_(0x7)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/128 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV256_Val _U_(0x8)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/256 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV512_Val _U_(0x9)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/512 */
#define   RTC_MODE1_CTRL_PRESCALER_DIV1024_Val _U_(0xA)   /**< \brief (RTC_MODE1_CTRL) CLK_RTC_CNT = GCLK_RTC/1024 */
#define RTC_MODE1_CTRL_PRESCALER_DIV1 (RTC_MODE1_CTRL_PRESCALER_DIV1_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV2 (RTC_MODE1_CTRL_PRESCALER_DIV2_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV4 (RTC_MODE1_CTRL_PRESCALER_DIV4_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV8 (RTC_MODE1_CTRL_PRESCALER_DIV8_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV16 (RTC_MODE1_CTRL_PRESCALER_DIV16_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV32 (RTC_MODE1_CTRL_PRESCALER_DIV32_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV64 (RTC_MODE1_CTRL_PRESCALER_DIV64_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV128 (RTC_MODE1_CTRL_PRESCALER_DIV128_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV256 (RTC_MODE1_CTRL_PRESCALER_DIV256_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV512 (RTC_MODE1_CTRL_PRESCALER_DIV512_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_PRESCALER_DIV1024 (RTC_MODE1_CTRL_PRESCALER_DIV1024_Val << RTC_MODE1_CTRL_PRESCALER_Pos)
#define RTC_MODE1_CTRL_MASK         _U_(0x0F0F)  /**< \brief (RTC_MODE1_CTRL) MASK Register */

/* -------- RTC_MODE2_CTRL : (RTC Offset: 0x00) (R/W 16) MODE2 MODE2 Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  Operating Mode                     */
    uint16_t :2;               /*!< bit:  4.. 5  Reserved                           */
    uint16_t CLKREP:1;         /*!< bit:      6  Clock Representation               */
    uint16_t MATCHCLR:1;       /*!< bit:      7  Clear on Match                     */
    uint16_t PRESCALER:4;      /*!< bit:  8..11  Prescaler                          */
    uint16_t :4;               /*!< bit: 12..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE2_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_CTRL_OFFSET       0x00         /**< \brief (RTC_MODE2_CTRL offset) MODE2 Control */
#define RTC_MODE2_CTRL_RESETVALUE   _U_(0x0000)  /**< \brief (RTC_MODE2_CTRL reset_value) MODE2 Control */

#define RTC_MODE2_CTRL_SWRST_Pos    0            /**< \brief (RTC_MODE2_CTRL) Software Reset */
#define RTC_MODE2_CTRL_SWRST        (_U_(0x1) << RTC_MODE2_CTRL_SWRST_Pos)
#define RTC_MODE2_CTRL_ENABLE_Pos   1            /**< \brief (RTC_MODE2_CTRL) Enable */
#define RTC_MODE2_CTRL_ENABLE       (_U_(0x1) << RTC_MODE2_CTRL_ENABLE_Pos)
#define RTC_MODE2_CTRL_MODE_Pos     2            /**< \brief (RTC_MODE2_CTRL) Operating Mode */
#define RTC_MODE2_CTRL_MODE_Msk     (_U_(0x3) << RTC_MODE2_CTRL_MODE_Pos)
#define RTC_MODE2_CTRL_MODE(value)  (RTC_MODE2_CTRL_MODE_Msk & ((value) << RTC_MODE2_CTRL_MODE_Pos))
#define   RTC_MODE2_CTRL_MODE_COUNT32_Val _U_(0x0)   /**< \brief (RTC_MODE2_CTRL) Mode 0: 32-bit Counter */
#define   RTC_MODE2_CTRL_MODE_COUNT16_Val _U_(0x1)   /**< \brief (RTC_MODE2_CTRL) Mode 1: 16-bit Counter */
#define   RTC_MODE2_CTRL_MODE_CLOCK_Val   _U_(0x2)   /**< \brief (RTC_MODE2_CTRL) Mode 2: Clock/Calendar */
#define RTC_MODE2_CTRL_MODE_COUNT32 (RTC_MODE2_CTRL_MODE_COUNT32_Val << RTC_MODE2_CTRL_MODE_Pos)
#define RTC_MODE2_CTRL_MODE_COUNT16 (RTC_MODE2_CTRL_MODE_COUNT16_Val << RTC_MODE2_CTRL_MODE_Pos)
#define RTC_MODE2_CTRL_MODE_CLOCK   (RTC_MODE2_CTRL_MODE_CLOCK_Val << RTC_MODE2_CTRL_MODE_Pos)
#define RTC_MODE2_CTRL_CLKREP_Pos   6            /**< \brief (RTC_MODE2_CTRL) Clock Representation */
#define RTC_MODE2_CTRL_CLKREP       (_U_(0x1) << RTC_MODE2_CTRL_CLKREP_Pos)
#define RTC_MODE2_CTRL_MATCHCLR_Pos 7            /**< \brief (RTC_MODE2_CTRL) Clear on Match */
#define RTC_MODE2_CTRL_MATCHCLR     (_U_(0x1) << RTC_MODE2_CTRL_MATCHCLR_Pos)
#define RTC_MODE2_CTRL_PRESCALER_Pos 8            /**< \brief (RTC_MODE2_CTRL) Prescaler */
#define RTC_MODE2_CTRL_PRESCALER_Msk (_U_(0xF) << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER(value) (RTC_MODE2_CTRL_PRESCALER_Msk & ((value) << RTC_MODE2_CTRL_PRESCALER_Pos))
#define   RTC_MODE2_CTRL_PRESCALER_DIV1_Val _U_(0x0)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/1 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV2_Val _U_(0x1)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/2 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV4_Val _U_(0x2)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/4 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV8_Val _U_(0x3)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/8 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV16_Val _U_(0x4)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/16 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV32_Val _U_(0x5)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/32 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV64_Val _U_(0x6)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/64 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV128_Val _U_(0x7)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/128 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV256_Val _U_(0x8)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/256 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV512_Val _U_(0x9)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/512 */
#define   RTC_MODE2_CTRL_PRESCALER_DIV1024_Val _U_(0xA)   /**< \brief (RTC_MODE2_CTRL) CLK_RTC_CNT = GCLK_RTC/1024 */
#define RTC_MODE2_CTRL_PRESCALER_DIV1 (RTC_MODE2_CTRL_PRESCALER_DIV1_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV2 (RTC_MODE2_CTRL_PRESCALER_DIV2_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV4 (RTC_MODE2_CTRL_PRESCALER_DIV4_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV8 (RTC_MODE2_CTRL_PRESCALER_DIV8_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV16 (RTC_MODE2_CTRL_PRESCALER_DIV16_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV32 (RTC_MODE2_CTRL_PRESCALER_DIV32_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV64 (RTC_MODE2_CTRL_PRESCALER_DIV64_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV128 (RTC_MODE2_CTRL_PRESCALER_DIV128_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV256 (RTC_MODE2_CTRL_PRESCALER_DIV256_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV512 (RTC_MODE2_CTRL_PRESCALER_DIV512_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_PRESCALER_DIV1024 (RTC_MODE2_CTRL_PRESCALER_DIV1024_Val << RTC_MODE2_CTRL_PRESCALER_Pos)
#define RTC_MODE2_CTRL_MASK         _U_(0x0FCF)  /**< \brief (RTC_MODE2_CTRL) MASK Register */

/* -------- RTC_READREQ : (RTC Offset: 0x02) (R/W 16) Read Request -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t ADDR:6;           /*!< bit:  0.. 5  Address                            */
    uint16_t :8;               /*!< bit:  6..13  Reserved                           */
    uint16_t RCONT:1;          /*!< bit:     14  Read Continuously                  */
    uint16_t RREQ:1;           /*!< bit:     15  Read Request                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_READREQ_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_READREQ_OFFSET          0x02         /**< \brief (RTC_READREQ offset) Read Request */
#define RTC_READREQ_RESETVALUE      _U_(0x0010)  /**< \brief (RTC_READREQ reset_value) Read Request */

#define RTC_READREQ_ADDR_Pos        0            /**< \brief (RTC_READREQ) Address */
#define RTC_READREQ_ADDR_Msk        (_U_(0x3F) << RTC_READREQ_ADDR_Pos)
#define RTC_READREQ_ADDR(value)     (RTC_READREQ_ADDR_Msk & ((value) << RTC_READREQ_ADDR_Pos))
#define RTC_READREQ_RCONT_Pos       14           /**< \brief (RTC_READREQ) Read Continuously */
#define RTC_READREQ_RCONT           (_U_(0x1) << RTC_READREQ_RCONT_Pos)
#define RTC_READREQ_RREQ_Pos        15           /**< \brief (RTC_READREQ) Read Request */
#define RTC_READREQ_RREQ            (_U_(0x1) << RTC_READREQ_RREQ_Pos)
#define RTC_READREQ_MASK            _U_(0xC03F)  /**< \brief (RTC_READREQ) MASK Register */

/* -------- RTC_MODE0_EVCTRL : (RTC Offset: 0x04) (R/W 16) MODE0 MODE0 Event Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t PEREO0:1;         /*!< bit:      0  Periodic Interval 0 Event Output Enable */
    uint16_t PEREO1:1;         /*!< bit:      1  Periodic Interval 1 Event Output Enable */
    uint16_t PEREO2:1;         /*!< bit:      2  Periodic Interval 2 Event Output Enable */
    uint16_t PEREO3:1;         /*!< bit:      3  Periodic Interval 3 Event Output Enable */
    uint16_t PEREO4:1;         /*!< bit:      4  Periodic Interval 4 Event Output Enable */
    uint16_t PEREO5:1;         /*!< bit:      5  Periodic Interval 5 Event Output Enable */
    uint16_t PEREO6:1;         /*!< bit:      6  Periodic Interval 6 Event Output Enable */
    uint16_t PEREO7:1;         /*!< bit:      7  Periodic Interval 7 Event Output Enable */
    uint16_t CMPEO0:1;         /*!< bit:      8  Compare 0 Event Output Enable      */
    uint16_t :6;               /*!< bit:  9..14  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:     15  Overflow Event Output Enable       */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint16_t PEREO:8;          /*!< bit:  0.. 7  Periodic Interval x Event Output Enable */
    uint16_t CMPEO:1;          /*!< bit:      8  Compare x Event Output Enable      */
    uint16_t :7;               /*!< bit:  9..15  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE0_EVCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_EVCTRL_OFFSET     0x04         /**< \brief (RTC_MODE0_EVCTRL offset) MODE0 Event Control */
#define RTC_MODE0_EVCTRL_RESETVALUE _U_(0x0000)  /**< \brief (RTC_MODE0_EVCTRL reset_value) MODE0 Event Control */

#define RTC_MODE0_EVCTRL_PEREO0_Pos 0            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 0 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO0     (_U_(1) << RTC_MODE0_EVCTRL_PEREO0_Pos)
#define RTC_MODE0_EVCTRL_PEREO1_Pos 1            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 1 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO1     (_U_(1) << RTC_MODE0_EVCTRL_PEREO1_Pos)
#define RTC_MODE0_EVCTRL_PEREO2_Pos 2            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 2 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO2     (_U_(1) << RTC_MODE0_EVCTRL_PEREO2_Pos)
#define RTC_MODE0_EVCTRL_PEREO3_Pos 3            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 3 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO3     (_U_(1) << RTC_MODE0_EVCTRL_PEREO3_Pos)
#define RTC_MODE0_EVCTRL_PEREO4_Pos 4            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 4 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO4     (_U_(1) << RTC_MODE0_EVCTRL_PEREO4_Pos)
#define RTC_MODE0_EVCTRL_PEREO5_Pos 5            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 5 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO5     (_U_(1) << RTC_MODE0_EVCTRL_PEREO5_Pos)
#define RTC_MODE0_EVCTRL_PEREO6_Pos 6            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 6 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO6     (_U_(1) << RTC_MODE0_EVCTRL_PEREO6_Pos)
#define RTC_MODE0_EVCTRL_PEREO7_Pos 7            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval 7 Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO7     (_U_(1) << RTC_MODE0_EVCTRL_PEREO7_Pos)
#define RTC_MODE0_EVCTRL_PEREO_Pos  0            /**< \brief (RTC_MODE0_EVCTRL) Periodic Interval x Event Output Enable */
#define RTC_MODE0_EVCTRL_PEREO_Msk  (_U_(0xFF) << RTC_MODE0_EVCTRL_PEREO_Pos)
#define RTC_MODE0_EVCTRL_PEREO(value) (RTC_MODE0_EVCTRL_PEREO_Msk & ((value) << RTC_MODE0_EVCTRL_PEREO_Pos))
#define RTC_MODE0_EVCTRL_CMPEO0_Pos 8            /**< \brief (RTC_MODE0_EVCTRL) Compare 0 Event Output Enable */
#define RTC_MODE0_EVCTRL_CMPEO0     (_U_(1) << RTC_MODE0_EVCTRL_CMPEO0_Pos)
#define RTC_MODE0_EVCTRL_CMPEO_Pos  8            /**< \brief (RTC_MODE0_EVCTRL) Compare x Event Output Enable */
#define RTC_MODE0_EVCTRL_CMPEO_Msk  (_U_(0x1) << RTC_MODE0_EVCTRL_CMPEO_Pos)
#define RTC_MODE0_EVCTRL_CMPEO(value) (RTC_MODE0_EVCTRL_CMPEO_Msk & ((value) << RTC_MODE0_EVCTRL_CMPEO_Pos))
#define RTC_MODE0_EVCTRL_OVFEO_Pos  15           /**< \brief (RTC_MODE0_EVCTRL) Overflow Event Output Enable */
#define RTC_MODE0_EVCTRL_OVFEO      (_U_(0x1) << RTC_MODE0_EVCTRL_OVFEO_Pos)
#define RTC_MODE0_EVCTRL_MASK       _U_(0x81FF)  /**< \brief (RTC_MODE0_EVCTRL) MASK Register */

/* -------- RTC_MODE1_EVCTRL : (RTC Offset: 0x04) (R/W 16) MODE1 MODE1 Event Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t PEREO0:1;         /*!< bit:      0  Periodic Interval 0 Event Output Enable */
    uint16_t PEREO1:1;         /*!< bit:      1  Periodic Interval 1 Event Output Enable */
    uint16_t PEREO2:1;         /*!< bit:      2  Periodic Interval 2 Event Output Enable */
    uint16_t PEREO3:1;         /*!< bit:      3  Periodic Interval 3 Event Output Enable */
    uint16_t PEREO4:1;         /*!< bit:      4  Periodic Interval 4 Event Output Enable */
    uint16_t PEREO5:1;         /*!< bit:      5  Periodic Interval 5 Event Output Enable */
    uint16_t PEREO6:1;         /*!< bit:      6  Periodic Interval 6 Event Output Enable */
    uint16_t PEREO7:1;         /*!< bit:      7  Periodic Interval 7 Event Output Enable */
    uint16_t CMPEO0:1;         /*!< bit:      8  Compare 0 Event Output Enable      */
    uint16_t CMPEO1:1;         /*!< bit:      9  Compare 1 Event Output Enable      */
    uint16_t :5;               /*!< bit: 10..14  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:     15  Overflow Event Output Enable       */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint16_t PEREO:8;          /*!< bit:  0.. 7  Periodic Interval x Event Output Enable */
    uint16_t CMPEO:2;          /*!< bit:  8.. 9  Compare x Event Output Enable      */
    uint16_t :6;               /*!< bit: 10..15  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_EVCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_EVCTRL_OFFSET     0x04         /**< \brief (RTC_MODE1_EVCTRL offset) MODE1 Event Control */
#define RTC_MODE1_EVCTRL_RESETVALUE _U_(0x0000)  /**< \brief (RTC_MODE1_EVCTRL reset_value) MODE1 Event Control */

#define RTC_MODE1_EVCTRL_PEREO0_Pos 0            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 0 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO0     (_U_(1) << RTC_MODE1_EVCTRL_PEREO0_Pos)
#define RTC_MODE1_EVCTRL_PEREO1_Pos 1            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 1 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO1     (_U_(1) << RTC_MODE1_EVCTRL_PEREO1_Pos)
#define RTC_MODE1_EVCTRL_PEREO2_Pos 2            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 2 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO2     (_U_(1) << RTC_MODE1_EVCTRL_PEREO2_Pos)
#define RTC_MODE1_EVCTRL_PEREO3_Pos 3            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 3 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO3     (_U_(1) << RTC_MODE1_EVCTRL_PEREO3_Pos)
#define RTC_MODE1_EVCTRL_PEREO4_Pos 4            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 4 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO4     (_U_(1) << RTC_MODE1_EVCTRL_PEREO4_Pos)
#define RTC_MODE1_EVCTRL_PEREO5_Pos 5            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 5 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO5     (_U_(1) << RTC_MODE1_EVCTRL_PEREO5_Pos)
#define RTC_MODE1_EVCTRL_PEREO6_Pos 6            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 6 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO6     (_U_(1) << RTC_MODE1_EVCTRL_PEREO6_Pos)
#define RTC_MODE1_EVCTRL_PEREO7_Pos 7            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval 7 Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO7     (_U_(1) << RTC_MODE1_EVCTRL_PEREO7_Pos)
#define RTC_MODE1_EVCTRL_PEREO_Pos  0            /**< \brief (RTC_MODE1_EVCTRL) Periodic Interval x Event Output Enable */
#define RTC_MODE1_EVCTRL_PEREO_Msk  (_U_(0xFF) << RTC_MODE1_EVCTRL_PEREO_Pos)
#define RTC_MODE1_EVCTRL_PEREO(value) (RTC_MODE1_EVCTRL_PEREO_Msk & ((value) << RTC_MODE1_EVCTRL_PEREO_Pos))
#define RTC_MODE1_EVCTRL_CMPEO0_Pos 8            /**< \brief (RTC_MODE1_EVCTRL) Compare 0 Event Output Enable */
#define RTC_MODE1_EVCTRL_CMPEO0     (_U_(1) << RTC_MODE1_EVCTRL_CMPEO0_Pos)
#define RTC_MODE1_EVCTRL_CMPEO1_Pos 9            /**< \brief (RTC_MODE1_EVCTRL) Compare 1 Event Output Enable */
#define RTC_MODE1_EVCTRL_CMPEO1     (_U_(1) << RTC_MODE1_EVCTRL_CMPEO1_Pos)
#define RTC_MODE1_EVCTRL_CMPEO_Pos  8            /**< \brief (RTC_MODE1_EVCTRL) Compare x Event Output Enable */
#define RTC_MODE1_EVCTRL_CMPEO_Msk  (_U_(0x3) << RTC_MODE1_EVCTRL_CMPEO_Pos)
#define RTC_MODE1_EVCTRL_CMPEO(value) (RTC_MODE1_EVCTRL_CMPEO_Msk & ((value) << RTC_MODE1_EVCTRL_CMPEO_Pos))
#define RTC_MODE1_EVCTRL_OVFEO_Pos  15           /**< \brief (RTC_MODE1_EVCTRL) Overflow Event Output Enable */
#define RTC_MODE1_EVCTRL_OVFEO      (_U_(0x1) << RTC_MODE1_EVCTRL_OVFEO_Pos)
#define RTC_MODE1_EVCTRL_MASK       _U_(0x83FF)  /**< \brief (RTC_MODE1_EVCTRL) MASK Register */

/* -------- RTC_MODE2_EVCTRL : (RTC Offset: 0x04) (R/W 16) MODE2 MODE2 Event Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t PEREO0:1;         /*!< bit:      0  Periodic Interval 0 Event Output Enable */
    uint16_t PEREO1:1;         /*!< bit:      1  Periodic Interval 1 Event Output Enable */
    uint16_t PEREO2:1;         /*!< bit:      2  Periodic Interval 2 Event Output Enable */
    uint16_t PEREO3:1;         /*!< bit:      3  Periodic Interval 3 Event Output Enable */
    uint16_t PEREO4:1;         /*!< bit:      4  Periodic Interval 4 Event Output Enable */
    uint16_t PEREO5:1;         /*!< bit:      5  Periodic Interval 5 Event Output Enable */
    uint16_t PEREO6:1;         /*!< bit:      6  Periodic Interval 6 Event Output Enable */
    uint16_t PEREO7:1;         /*!< bit:      7  Periodic Interval 7 Event Output Enable */
    uint16_t ALARMEO0:1;       /*!< bit:      8  Alarm 0 Event Output Enable        */
    uint16_t :6;               /*!< bit:  9..14  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:     15  Overflow Event Output Enable       */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint16_t PEREO:8;          /*!< bit:  0.. 7  Periodic Interval x Event Output Enable */
    uint16_t ALARMEO:1;        /*!< bit:      8  Alarm x Event Output Enable        */
    uint16_t :7;               /*!< bit:  9..15  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE2_EVCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_EVCTRL_OFFSET     0x04         /**< \brief (RTC_MODE2_EVCTRL offset) MODE2 Event Control */
#define RTC_MODE2_EVCTRL_RESETVALUE _U_(0x0000)  /**< \brief (RTC_MODE2_EVCTRL reset_value) MODE2 Event Control */

#define RTC_MODE2_EVCTRL_PEREO0_Pos 0            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 0 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO0     (_U_(1) << RTC_MODE2_EVCTRL_PEREO0_Pos)
#define RTC_MODE2_EVCTRL_PEREO1_Pos 1            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 1 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO1     (_U_(1) << RTC_MODE2_EVCTRL_PEREO1_Pos)
#define RTC_MODE2_EVCTRL_PEREO2_Pos 2            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 2 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO2     (_U_(1) << RTC_MODE2_EVCTRL_PEREO2_Pos)
#define RTC_MODE2_EVCTRL_PEREO3_Pos 3            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 3 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO3     (_U_(1) << RTC_MODE2_EVCTRL_PEREO3_Pos)
#define RTC_MODE2_EVCTRL_PEREO4_Pos 4            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 4 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO4     (_U_(1) << RTC_MODE2_EVCTRL_PEREO4_Pos)
#define RTC_MODE2_EVCTRL_PEREO5_Pos 5            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 5 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO5     (_U_(1) << RTC_MODE2_EVCTRL_PEREO5_Pos)
#define RTC_MODE2_EVCTRL_PEREO6_Pos 6            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 6 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO6     (_U_(1) << RTC_MODE2_EVCTRL_PEREO6_Pos)
#define RTC_MODE2_EVCTRL_PEREO7_Pos 7            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval 7 Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO7     (_U_(1) << RTC_MODE2_EVCTRL_PEREO7_Pos)
#define RTC_MODE2_EVCTRL_PEREO_Pos  0            /**< \brief (RTC_MODE2_EVCTRL) Periodic Interval x Event Output Enable */
#define RTC_MODE2_EVCTRL_PEREO_Msk  (_U_(0xFF) << RTC_MODE2_EVCTRL_PEREO_Pos)
#define RTC_MODE2_EVCTRL_PEREO(value) (RTC_MODE2_EVCTRL_PEREO_Msk & ((value) << RTC_MODE2_EVCTRL_PEREO_Pos))
#define RTC_MODE2_EVCTRL_ALARMEO0_Pos 8            /**< \brief (RTC_MODE2_EVCTRL) Alarm 0 Event Output Enable */
#define RTC_MODE2_EVCTRL_ALARMEO0   (_U_(1) << RTC_MODE2_EVCTRL_ALARMEO0_Pos)
#define RTC_MODE2_EVCTRL_ALARMEO_Pos 8            /**< \brief (RTC_MODE2_EVCTRL) Alarm x Event Output Enable */
#define RTC_MODE2_EVCTRL_ALARMEO_Msk (_U_(0x1) << RTC_MODE2_EVCTRL_ALARMEO_Pos)
#define RTC_MODE2_EVCTRL_ALARMEO(value) (RTC_MODE2_EVCTRL_ALARMEO_Msk & ((value) << RTC_MODE2_EVCTRL_ALARMEO_Pos))
#define RTC_MODE2_EVCTRL_OVFEO_Pos  15           /**< \brief (RTC_MODE2_EVCTRL) Overflow Event Output Enable */
#define RTC_MODE2_EVCTRL_OVFEO      (_U_(0x1) << RTC_MODE2_EVCTRL_OVFEO_Pos)
#define RTC_MODE2_EVCTRL_MASK       _U_(0x81FF)  /**< \brief (RTC_MODE2_EVCTRL) MASK Register */

/* -------- RTC_MODE0_INTENCLR : (RTC Offset: 0x06) (R/W  8) MODE0 MODE0 Interrupt Enable Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0  Compare 0 Interrupt Enable         */
    uint8_t  :5;               /*!< bit:  1.. 5  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready Interrupt Enable */
    uint8_t  OVF:1;            /*!< bit:      7  Overflow Interrupt Enable          */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  CMP:1;            /*!< bit:      0  Compare x Interrupt Enable         */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE0_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_INTENCLR_OFFSET   0x06         /**< \brief (RTC_MODE0_INTENCLR offset) MODE0 Interrupt Enable Clear */
#define RTC_MODE0_INTENCLR_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE0_INTENCLR reset_value) MODE0 Interrupt Enable Clear */

#define RTC_MODE0_INTENCLR_CMP0_Pos 0            /**< \brief (RTC_MODE0_INTENCLR) Compare 0 Interrupt Enable */
#define RTC_MODE0_INTENCLR_CMP0     (_U_(1) << RTC_MODE0_INTENCLR_CMP0_Pos)
#define RTC_MODE0_INTENCLR_CMP_Pos  0            /**< \brief (RTC_MODE0_INTENCLR) Compare x Interrupt Enable */
#define RTC_MODE0_INTENCLR_CMP_Msk  (_U_(0x1) << RTC_MODE0_INTENCLR_CMP_Pos)
#define RTC_MODE0_INTENCLR_CMP(value) (RTC_MODE0_INTENCLR_CMP_Msk & ((value) << RTC_MODE0_INTENCLR_CMP_Pos))
#define RTC_MODE0_INTENCLR_SYNCRDY_Pos 6            /**< \brief (RTC_MODE0_INTENCLR) Synchronization Ready Interrupt Enable */
#define RTC_MODE0_INTENCLR_SYNCRDY  (_U_(0x1) << RTC_MODE0_INTENCLR_SYNCRDY_Pos)
#define RTC_MODE0_INTENCLR_OVF_Pos  7            /**< \brief (RTC_MODE0_INTENCLR) Overflow Interrupt Enable */
#define RTC_MODE0_INTENCLR_OVF      (_U_(0x1) << RTC_MODE0_INTENCLR_OVF_Pos)
#define RTC_MODE0_INTENCLR_MASK     _U_(0xC1)    /**< \brief (RTC_MODE0_INTENCLR) MASK Register */

/* -------- RTC_MODE1_INTENCLR : (RTC Offset: 0x06) (R/W  8) MODE1 MODE1 Interrupt Enable Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0  Compare 0 Interrupt Enable         */
    uint8_t  CMP1:1;           /*!< bit:      1  Compare 1 Interrupt Enable         */
    uint8_t  :4;               /*!< bit:  2.. 5  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready Interrupt Enable */
    uint8_t  OVF:1;            /*!< bit:      7  Overflow Interrupt Enable          */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  CMP:2;            /*!< bit:  0.. 1  Compare x Interrupt Enable         */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE1_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_INTENCLR_OFFSET   0x06         /**< \brief (RTC_MODE1_INTENCLR offset) MODE1 Interrupt Enable Clear */
#define RTC_MODE1_INTENCLR_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE1_INTENCLR reset_value) MODE1 Interrupt Enable Clear */

#define RTC_MODE1_INTENCLR_CMP0_Pos 0            /**< \brief (RTC_MODE1_INTENCLR) Compare 0 Interrupt Enable */
#define RTC_MODE1_INTENCLR_CMP0     (_U_(1) << RTC_MODE1_INTENCLR_CMP0_Pos)
#define RTC_MODE1_INTENCLR_CMP1_Pos 1            /**< \brief (RTC_MODE1_INTENCLR) Compare 1 Interrupt Enable */
#define RTC_MODE1_INTENCLR_CMP1     (_U_(1) << RTC_MODE1_INTENCLR_CMP1_Pos)
#define RTC_MODE1_INTENCLR_CMP_Pos  0            /**< \brief (RTC_MODE1_INTENCLR) Compare x Interrupt Enable */
#define RTC_MODE1_INTENCLR_CMP_Msk  (_U_(0x3) << RTC_MODE1_INTENCLR_CMP_Pos)
#define RTC_MODE1_INTENCLR_CMP(value) (RTC_MODE1_INTENCLR_CMP_Msk & ((value) << RTC_MODE1_INTENCLR_CMP_Pos))
#define RTC_MODE1_INTENCLR_SYNCRDY_Pos 6            /**< \brief (RTC_MODE1_INTENCLR) Synchronization Ready Interrupt Enable */
#define RTC_MODE1_INTENCLR_SYNCRDY  (_U_(0x1) << RTC_MODE1_INTENCLR_SYNCRDY_Pos)
#define RTC_MODE1_INTENCLR_OVF_Pos  7            /**< \brief (RTC_MODE1_INTENCLR) Overflow Interrupt Enable */
#define RTC_MODE1_INTENCLR_OVF      (_U_(0x1) << RTC_MODE1_INTENCLR_OVF_Pos)
#define RTC_MODE1_INTENCLR_MASK     _U_(0xC3)    /**< \brief (RTC_MODE1_INTENCLR) MASK Register */

/* -------- RTC_MODE2_INTENCLR : (RTC Offset: 0x06) (R/W  8) MODE2 MODE2 Interrupt Enable Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  ALARM0:1;         /*!< bit:      0  Alarm 0 Interrupt Enable           */
    uint8_t  :5;               /*!< bit:  1.. 5  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready Interrupt Enable */
    uint8_t  OVF:1;            /*!< bit:      7  Overflow Interrupt Enable          */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  ALARM:1;          /*!< bit:      0  Alarm x Interrupt Enable           */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE2_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_INTENCLR_OFFSET   0x06         /**< \brief (RTC_MODE2_INTENCLR offset) MODE2 Interrupt Enable Clear */
#define RTC_MODE2_INTENCLR_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE2_INTENCLR reset_value) MODE2 Interrupt Enable Clear */

#define RTC_MODE2_INTENCLR_ALARM0_Pos 0            /**< \brief (RTC_MODE2_INTENCLR) Alarm 0 Interrupt Enable */
#define RTC_MODE2_INTENCLR_ALARM0   (_U_(1) << RTC_MODE2_INTENCLR_ALARM0_Pos)
#define RTC_MODE2_INTENCLR_ALARM_Pos 0            /**< \brief (RTC_MODE2_INTENCLR) Alarm x Interrupt Enable */
#define RTC_MODE2_INTENCLR_ALARM_Msk (_U_(0x1) << RTC_MODE2_INTENCLR_ALARM_Pos)
#define RTC_MODE2_INTENCLR_ALARM(value) (RTC_MODE2_INTENCLR_ALARM_Msk & ((value) << RTC_MODE2_INTENCLR_ALARM_Pos))
#define RTC_MODE2_INTENCLR_SYNCRDY_Pos 6            /**< \brief (RTC_MODE2_INTENCLR) Synchronization Ready Interrupt Enable */
#define RTC_MODE2_INTENCLR_SYNCRDY  (_U_(0x1) << RTC_MODE2_INTENCLR_SYNCRDY_Pos)
#define RTC_MODE2_INTENCLR_OVF_Pos  7            /**< \brief (RTC_MODE2_INTENCLR) Overflow Interrupt Enable */
#define RTC_MODE2_INTENCLR_OVF      (_U_(0x1) << RTC_MODE2_INTENCLR_OVF_Pos)
#define RTC_MODE2_INTENCLR_MASK     _U_(0xC1)    /**< \brief (RTC_MODE2_INTENCLR) MASK Register */

/* -------- RTC_MODE0_INTENSET : (RTC Offset: 0x07) (R/W  8) MODE0 MODE0 Interrupt Enable Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0  Compare 0 Interrupt Enable         */
    uint8_t  :5;               /*!< bit:  1.. 5  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready Interrupt Enable */
    uint8_t  OVF:1;            /*!< bit:      7  Overflow Interrupt Enable          */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  CMP:1;            /*!< bit:      0  Compare x Interrupt Enable         */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE0_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_INTENSET_OFFSET   0x07         /**< \brief (RTC_MODE0_INTENSET offset) MODE0 Interrupt Enable Set */
#define RTC_MODE0_INTENSET_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE0_INTENSET reset_value) MODE0 Interrupt Enable Set */

#define RTC_MODE0_INTENSET_CMP0_Pos 0            /**< \brief (RTC_MODE0_INTENSET) Compare 0 Interrupt Enable */
#define RTC_MODE0_INTENSET_CMP0     (_U_(1) << RTC_MODE0_INTENSET_CMP0_Pos)
#define RTC_MODE0_INTENSET_CMP_Pos  0            /**< \brief (RTC_MODE0_INTENSET) Compare x Interrupt Enable */
#define RTC_MODE0_INTENSET_CMP_Msk  (_U_(0x1) << RTC_MODE0_INTENSET_CMP_Pos)
#define RTC_MODE0_INTENSET_CMP(value) (RTC_MODE0_INTENSET_CMP_Msk & ((value) << RTC_MODE0_INTENSET_CMP_Pos))
#define RTC_MODE0_INTENSET_SYNCRDY_Pos 6            /**< \brief (RTC_MODE0_INTENSET) Synchronization Ready Interrupt Enable */
#define RTC_MODE0_INTENSET_SYNCRDY  (_U_(0x1) << RTC_MODE0_INTENSET_SYNCRDY_Pos)
#define RTC_MODE0_INTENSET_OVF_Pos  7            /**< \brief (RTC_MODE0_INTENSET) Overflow Interrupt Enable */
#define RTC_MODE0_INTENSET_OVF      (_U_(0x1) << RTC_MODE0_INTENSET_OVF_Pos)
#define RTC_MODE0_INTENSET_MASK     _U_(0xC1)    /**< \brief (RTC_MODE0_INTENSET) MASK Register */

/* -------- RTC_MODE1_INTENSET : (RTC Offset: 0x07) (R/W  8) MODE1 MODE1 Interrupt Enable Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CMP0:1;           /*!< bit:      0  Compare 0 Interrupt Enable         */
    uint8_t  CMP1:1;           /*!< bit:      1  Compare 1 Interrupt Enable         */
    uint8_t  :4;               /*!< bit:  2.. 5  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready Interrupt Enable */
    uint8_t  OVF:1;            /*!< bit:      7  Overflow Interrupt Enable          */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  CMP:2;            /*!< bit:  0.. 1  Compare x Interrupt Enable         */
    uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE1_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_INTENSET_OFFSET   0x07         /**< \brief (RTC_MODE1_INTENSET offset) MODE1 Interrupt Enable Set */
#define RTC_MODE1_INTENSET_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE1_INTENSET reset_value) MODE1 Interrupt Enable Set */

#define RTC_MODE1_INTENSET_CMP0_Pos 0            /**< \brief (RTC_MODE1_INTENSET) Compare 0 Interrupt Enable */
#define RTC_MODE1_INTENSET_CMP0     (_U_(1) << RTC_MODE1_INTENSET_CMP0_Pos)
#define RTC_MODE1_INTENSET_CMP1_Pos 1            /**< \brief (RTC_MODE1_INTENSET) Compare 1 Interrupt Enable */
#define RTC_MODE1_INTENSET_CMP1     (_U_(1) << RTC_MODE1_INTENSET_CMP1_Pos)
#define RTC_MODE1_INTENSET_CMP_Pos  0            /**< \brief (RTC_MODE1_INTENSET) Compare x Interrupt Enable */
#define RTC_MODE1_INTENSET_CMP_Msk  (_U_(0x3) << RTC_MODE1_INTENSET_CMP_Pos)
#define RTC_MODE1_INTENSET_CMP(value) (RTC_MODE1_INTENSET_CMP_Msk & ((value) << RTC_MODE1_INTENSET_CMP_Pos))
#define RTC_MODE1_INTENSET_SYNCRDY_Pos 6            /**< \brief (RTC_MODE1_INTENSET) Synchronization Ready Interrupt Enable */
#define RTC_MODE1_INTENSET_SYNCRDY  (_U_(0x1) << RTC_MODE1_INTENSET_SYNCRDY_Pos)
#define RTC_MODE1_INTENSET_OVF_Pos  7            /**< \brief (RTC_MODE1_INTENSET) Overflow Interrupt Enable */
#define RTC_MODE1_INTENSET_OVF      (_U_(0x1) << RTC_MODE1_INTENSET_OVF_Pos)
#define RTC_MODE1_INTENSET_MASK     _U_(0xC3)    /**< \brief (RTC_MODE1_INTENSET) MASK Register */

/* -------- RTC_MODE2_INTENSET : (RTC Offset: 0x07) (R/W  8) MODE2 MODE2 Interrupt Enable Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  ALARM0:1;         /*!< bit:      0  Alarm 0 Interrupt Enable           */
    uint8_t  :5;               /*!< bit:  1.. 5  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready Interrupt Enable */
    uint8_t  OVF:1;            /*!< bit:      7  Overflow Interrupt Enable          */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  ALARM:1;          /*!< bit:      0  Alarm x Interrupt Enable           */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE2_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_INTENSET_OFFSET   0x07         /**< \brief (RTC_MODE2_INTENSET offset) MODE2 Interrupt Enable Set */
#define RTC_MODE2_INTENSET_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE2_INTENSET reset_value) MODE2 Interrupt Enable Set */

#define RTC_MODE2_INTENSET_ALARM0_Pos 0            /**< \brief (RTC_MODE2_INTENSET) Alarm 0 Interrupt Enable */
#define RTC_MODE2_INTENSET_ALARM0   (_U_(1) << RTC_MODE2_INTENSET_ALARM0_Pos)
#define RTC_MODE2_INTENSET_ALARM_Pos 0            /**< \brief (RTC_MODE2_INTENSET) Alarm x Interrupt Enable */
#define RTC_MODE2_INTENSET_ALARM_Msk (_U_(0x1) << RTC_MODE2_INTENSET_ALARM_Pos)
#define RTC_MODE2_INTENSET_ALARM(value) (RTC_MODE2_INTENSET_ALARM_Msk & ((value) << RTC_MODE2_INTENSET_ALARM_Pos))
#define RTC_MODE2_INTENSET_SYNCRDY_Pos 6            /**< \brief (RTC_MODE2_INTENSET) Synchronization Ready Interrupt Enable */
#define RTC_MODE2_INTENSET_SYNCRDY  (_U_(0x1) << RTC_MODE2_INTENSET_SYNCRDY_Pos)
#define RTC_MODE2_INTENSET_OVF_Pos  7            /**< \brief (RTC_MODE2_INTENSET) Overflow Interrupt Enable */
#define RTC_MODE2_INTENSET_OVF      (_U_(0x1) << RTC_MODE2_INTENSET_OVF_Pos)
#define RTC_MODE2_INTENSET_MASK     _U_(0xC1)    /**< \brief (RTC_MODE2_INTENSET) MASK Register */

/* -------- RTC_MODE0_INTFLAG : (RTC Offset: 0x08) (R/W  8) MODE0 MODE0 Interrupt Flag Status and Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { // __I to avoid read-modify-write on write-to-clear register
  struct {
    __I uint8_t  CMP0:1;           /*!< bit:      0  Compare 0                          */
    __I uint8_t  :5;               /*!< bit:  1.. 5  Reserved                           */
    __I uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready              */
    __I uint8_t  OVF:1;            /*!< bit:      7  Overflow                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    __I uint8_t  CMP:1;            /*!< bit:      0  Compare x                          */
    __I uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE0_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_INTFLAG_OFFSET    0x08         /**< \brief (RTC_MODE0_INTFLAG offset) MODE0 Interrupt Flag Status and Clear */
#define RTC_MODE0_INTFLAG_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE0_INTFLAG reset_value) MODE0 Interrupt Flag Status and Clear */

#define RTC_MODE0_INTFLAG_CMP0_Pos  0            /**< \brief (RTC_MODE0_INTFLAG) Compare 0 */
#define RTC_MODE0_INTFLAG_CMP0      (_U_(1) << RTC_MODE0_INTFLAG_CMP0_Pos)
#define RTC_MODE0_INTFLAG_CMP_Pos   0            /**< \brief (RTC_MODE0_INTFLAG) Compare x */
#define RTC_MODE0_INTFLAG_CMP_Msk   (_U_(0x1) << RTC_MODE0_INTFLAG_CMP_Pos)
#define RTC_MODE0_INTFLAG_CMP(value) (RTC_MODE0_INTFLAG_CMP_Msk & ((value) << RTC_MODE0_INTFLAG_CMP_Pos))
#define RTC_MODE0_INTFLAG_SYNCRDY_Pos 6            /**< \brief (RTC_MODE0_INTFLAG) Synchronization Ready */
#define RTC_MODE0_INTFLAG_SYNCRDY   (_U_(0x1) << RTC_MODE0_INTFLAG_SYNCRDY_Pos)
#define RTC_MODE0_INTFLAG_OVF_Pos   7            /**< \brief (RTC_MODE0_INTFLAG) Overflow */
#define RTC_MODE0_INTFLAG_OVF       (_U_(0x1) << RTC_MODE0_INTFLAG_OVF_Pos)
#define RTC_MODE0_INTFLAG_MASK      _U_(0xC1)    /**< \brief (RTC_MODE0_INTFLAG) MASK Register */

/* -------- RTC_MODE1_INTFLAG : (RTC Offset: 0x08) (R/W  8) MODE1 MODE1 Interrupt Flag Status and Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { // __I to avoid read-modify-write on write-to-clear register
  struct {
    __I uint8_t  CMP0:1;           /*!< bit:      0  Compare 0                          */
    __I uint8_t  CMP1:1;           /*!< bit:      1  Compare 1                          */
    __I uint8_t  :4;               /*!< bit:  2.. 5  Reserved                           */
    __I uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready              */
    __I uint8_t  OVF:1;            /*!< bit:      7  Overflow                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    __I uint8_t  CMP:2;            /*!< bit:  0.. 1  Compare x                          */
    __I uint8_t  :6;               /*!< bit:  2.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE1_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_INTFLAG_OFFSET    0x08         /**< \brief (RTC_MODE1_INTFLAG offset) MODE1 Interrupt Flag Status and Clear */
#define RTC_MODE1_INTFLAG_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE1_INTFLAG reset_value) MODE1 Interrupt Flag Status and Clear */

#define RTC_MODE1_INTFLAG_CMP0_Pos  0            /**< \brief (RTC_MODE1_INTFLAG) Compare 0 */
#define RTC_MODE1_INTFLAG_CMP0      (_U_(1) << RTC_MODE1_INTFLAG_CMP0_Pos)
#define RTC_MODE1_INTFLAG_CMP1_Pos  1            /**< \brief (RTC_MODE1_INTFLAG) Compare 1 */
#define RTC_MODE1_INTFLAG_CMP1      (_U_(1) << RTC_MODE1_INTFLAG_CMP1_Pos)
#define RTC_MODE1_INTFLAG_CMP_Pos   0            /**< \brief (RTC_MODE1_INTFLAG) Compare x */
#define RTC_MODE1_INTFLAG_CMP_Msk   (_U_(0x3) << RTC_MODE1_INTFLAG_CMP_Pos)
#define RTC_MODE1_INTFLAG_CMP(value) (RTC_MODE1_INTFLAG_CMP_Msk & ((value) << RTC_MODE1_INTFLAG_CMP_Pos))
#define RTC_MODE1_INTFLAG_SYNCRDY_Pos 6            /**< \brief (RTC_MODE1_INTFLAG) Synchronization Ready */
#define RTC_MODE1_INTFLAG_SYNCRDY   (_U_(0x1) << RTC_MODE1_INTFLAG_SYNCRDY_Pos)
#define RTC_MODE1_INTFLAG_OVF_Pos   7            /**< \brief (RTC_MODE1_INTFLAG) Overflow */
#define RTC_MODE1_INTFLAG_OVF       (_U_(0x1) << RTC_MODE1_INTFLAG_OVF_Pos)
#define RTC_MODE1_INTFLAG_MASK      _U_(0xC3)    /**< \brief (RTC_MODE1_INTFLAG) MASK Register */

/* -------- RTC_MODE2_INTFLAG : (RTC Offset: 0x08) (R/W  8) MODE2 MODE2 Interrupt Flag Status and Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { // __I to avoid read-modify-write on write-to-clear register
  struct {
    __I uint8_t  ALARM0:1;         /*!< bit:      0  Alarm 0                            */
    __I uint8_t  :5;               /*!< bit:  1.. 5  Reserved                           */
    __I uint8_t  SYNCRDY:1;        /*!< bit:      6  Synchronization Ready              */
    __I uint8_t  OVF:1;            /*!< bit:      7  Overflow                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    __I uint8_t  ALARM:1;          /*!< bit:      0  Alarm x                            */
    __I uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE2_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_INTFLAG_OFFSET    0x08         /**< \brief (RTC_MODE2_INTFLAG offset) MODE2 Interrupt Flag Status and Clear */
#define RTC_MODE2_INTFLAG_RESETVALUE _U_(0x00)    /**< \brief (RTC_MODE2_INTFLAG reset_value) MODE2 Interrupt Flag Status and Clear */

#define RTC_MODE2_INTFLAG_ALARM0_Pos 0            /**< \brief (RTC_MODE2_INTFLAG) Alarm 0 */
#define RTC_MODE2_INTFLAG_ALARM0    (_U_(1) << RTC_MODE2_INTFLAG_ALARM0_Pos)
#define RTC_MODE2_INTFLAG_ALARM_Pos 0            /**< \brief (RTC_MODE2_INTFLAG) Alarm x */
#define RTC_MODE2_INTFLAG_ALARM_Msk (_U_(0x1) << RTC_MODE2_INTFLAG_ALARM_Pos)
#define RTC_MODE2_INTFLAG_ALARM(value) (RTC_MODE2_INTFLAG_ALARM_Msk & ((value) << RTC_MODE2_INTFLAG_ALARM_Pos))
#define RTC_MODE2_INTFLAG_SYNCRDY_Pos 6            /**< \brief (RTC_MODE2_INTFLAG) Synchronization Ready */
#define RTC_MODE2_INTFLAG_SYNCRDY   (_U_(0x1) << RTC_MODE2_INTFLAG_SYNCRDY_Pos)
#define RTC_MODE2_INTFLAG_OVF_Pos   7            /**< \brief (RTC_MODE2_INTFLAG) Overflow */
#define RTC_MODE2_INTFLAG_OVF       (_U_(0x1) << RTC_MODE2_INTFLAG_OVF_Pos)
#define RTC_MODE2_INTFLAG_MASK      _U_(0xC1)    /**< \brief (RTC_MODE2_INTFLAG) MASK Register */

/* -------- RTC_STATUS : (RTC Offset: 0x0A) (R/W  8) Status -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :7;               /*!< bit:  0.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_STATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_STATUS_OFFSET           0x0A         /**< \brief (RTC_STATUS offset) Status */
#define RTC_STATUS_RESETVALUE       _U_(0x00)    /**< \brief (RTC_STATUS reset_value) Status */

#define RTC_STATUS_SYNCBUSY_Pos     7            /**< \brief (RTC_STATUS) Synchronization Busy */
#define RTC_STATUS_SYNCBUSY         (_U_(0x1) << RTC_STATUS_SYNCBUSY_Pos)
#define RTC_STATUS_MASK             _U_(0x80)    /**< \brief (RTC_STATUS) MASK Register */

/* -------- RTC_DBGCTRL : (RTC Offset: 0x0B) (R/W  8) Debug Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0  Run During Debug                   */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_DBGCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_DBGCTRL_OFFSET          0x0B         /**< \brief (RTC_DBGCTRL offset) Debug Control */
#define RTC_DBGCTRL_RESETVALUE      _U_(0x00)    /**< \brief (RTC_DBGCTRL reset_value) Debug Control */

#define RTC_DBGCTRL_DBGRUN_Pos      0            /**< \brief (RTC_DBGCTRL) Run During Debug */
#define RTC_DBGCTRL_DBGRUN          (_U_(0x1) << RTC_DBGCTRL_DBGRUN_Pos)
#define RTC_DBGCTRL_MASK            _U_(0x01)    /**< \brief (RTC_DBGCTRL) MASK Register */

/* -------- RTC_FREQCORR : (RTC Offset: 0x0C) (R/W  8) Frequency Correction -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  VALUE:7;          /*!< bit:  0.. 6  Correction Value                   */
    uint8_t  SIGN:1;           /*!< bit:      7  Correction Sign                    */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_FREQCORR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_FREQCORR_OFFSET         0x0C         /**< \brief (RTC_FREQCORR offset) Frequency Correction */
#define RTC_FREQCORR_RESETVALUE     _U_(0x00)    /**< \brief (RTC_FREQCORR reset_value) Frequency Correction */

#define RTC_FREQCORR_VALUE_Pos      0            /**< \brief (RTC_FREQCORR) Correction Value */
#define RTC_FREQCORR_VALUE_Msk      (_U_(0x7F) << RTC_FREQCORR_VALUE_Pos)
#define RTC_FREQCORR_VALUE(value)   (RTC_FREQCORR_VALUE_Msk & ((value) << RTC_FREQCORR_VALUE_Pos))
#define RTC_FREQCORR_SIGN_Pos       7            /**< \brief (RTC_FREQCORR) Correction Sign */
#define RTC_FREQCORR_SIGN           (_U_(0x1) << RTC_FREQCORR_SIGN_Pos)
#define RTC_FREQCORR_MASK           _U_(0xFF)    /**< \brief (RTC_FREQCORR) MASK Register */

/* -------- RTC_MODE0_COUNT : (RTC Offset: 0x10) (R/W 32) MODE0 MODE0 Counter Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t COUNT:32;         /*!< bit:  0..31  Counter Value                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE0_COUNT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_COUNT_OFFSET      0x10         /**< \brief (RTC_MODE0_COUNT offset) MODE0 Counter Value */
#define RTC_MODE0_COUNT_RESETVALUE  _U_(0x00000000) /**< \brief (RTC_MODE0_COUNT reset_value) MODE0 Counter Value */

#define RTC_MODE0_COUNT_COUNT_Pos   0            /**< \brief (RTC_MODE0_COUNT) Counter Value */
#define RTC_MODE0_COUNT_COUNT_Msk   (_U_(0xFFFFFFFF) << RTC_MODE0_COUNT_COUNT_Pos)
#define RTC_MODE0_COUNT_COUNT(value) (RTC_MODE0_COUNT_COUNT_Msk & ((value) << RTC_MODE0_COUNT_COUNT_Pos))
#define RTC_MODE0_COUNT_MASK        _U_(0xFFFFFFFF) /**< \brief (RTC_MODE0_COUNT) MASK Register */

/* -------- RTC_MODE1_COUNT : (RTC Offset: 0x10) (R/W 16) MODE1 MODE1 Counter Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t COUNT:16;         /*!< bit:  0..15  Counter Value                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_COUNT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_COUNT_OFFSET      0x10         /**< \brief (RTC_MODE1_COUNT offset) MODE1 Counter Value */
#define RTC_MODE1_COUNT_RESETVALUE  _U_(0x0000)  /**< \brief (RTC_MODE1_COUNT reset_value) MODE1 Counter Value */

#define RTC_MODE1_COUNT_COUNT_Pos   0            /**< \brief (RTC_MODE1_COUNT) Counter Value */
#define RTC_MODE1_COUNT_COUNT_Msk   (_U_(0xFFFF) << RTC_MODE1_COUNT_COUNT_Pos)
#define RTC_MODE1_COUNT_COUNT(value) (RTC_MODE1_COUNT_COUNT_Msk & ((value) << RTC_MODE1_COUNT_COUNT_Pos))
#define RTC_MODE1_COUNT_MASK        _U_(0xFFFF)  /**< \brief (RTC_MODE1_COUNT) MASK Register */

/* -------- RTC_MODE2_CLOCK : (RTC Offset: 0x10) (R/W 32) MODE2 MODE2 Clock Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t SECOND:6;         /*!< bit:  0.. 5  Second                             */
    uint32_t MINUTE:6;         /*!< bit:  6..11  Minute                             */
    uint32_t HOUR:5;           /*!< bit: 12..16  Hour                               */
    uint32_t DAY:5;            /*!< bit: 17..21  Day                                */
    uint32_t MONTH:4;          /*!< bit: 22..25  Month                              */
    uint32_t YEAR:6;           /*!< bit: 26..31  Year                               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE2_CLOCK_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_CLOCK_OFFSET      0x10         /**< \brief (RTC_MODE2_CLOCK offset) MODE2 Clock Value */
#define RTC_MODE2_CLOCK_RESETVALUE  _U_(0x00000000) /**< \brief (RTC_MODE2_CLOCK reset_value) MODE2 Clock Value */

#define RTC_MODE2_CLOCK_SECOND_Pos  0            /**< \brief (RTC_MODE2_CLOCK) Second */
#define RTC_MODE2_CLOCK_SECOND_Msk  (_U_(0x3F) << RTC_MODE2_CLOCK_SECOND_Pos)
#define RTC_MODE2_CLOCK_SECOND(value) (RTC_MODE2_CLOCK_SECOND_Msk & ((value) << RTC_MODE2_CLOCK_SECOND_Pos))
#define RTC_MODE2_CLOCK_MINUTE_Pos  6            /**< \brief (RTC_MODE2_CLOCK) Minute */
#define RTC_MODE2_CLOCK_MINUTE_Msk  (_U_(0x3F) << RTC_MODE2_CLOCK_MINUTE_Pos)
#define RTC_MODE2_CLOCK_MINUTE(value) (RTC_MODE2_CLOCK_MINUTE_Msk & ((value) << RTC_MODE2_CLOCK_MINUTE_Pos))
#define RTC_MODE2_CLOCK_HOUR_Pos    12           /**< \brief (RTC_MODE2_CLOCK) Hour */
#define RTC_MODE2_CLOCK_HOUR_Msk    (_U_(0x1F) << RTC_MODE2_CLOCK_HOUR_Pos)
#define RTC_MODE2_CLOCK_HOUR(value) (RTC_MODE2_CLOCK_HOUR_Msk & ((value) << RTC_MODE2_CLOCK_HOUR_Pos))
#define   RTC_MODE2_CLOCK_HOUR_AM_Val     _U_(0x0)   /**< \brief (RTC_MODE2_CLOCK) AM when CLKREP in 12-hour */
#define   RTC_MODE2_CLOCK_HOUR_PM_Val     _U_(0x10)   /**< \brief (RTC_MODE2_CLOCK) PM when CLKREP in 12-hour */
#define RTC_MODE2_CLOCK_HOUR_AM     (RTC_MODE2_CLOCK_HOUR_AM_Val   << RTC_MODE2_CLOCK_HOUR_Pos)
#define RTC_MODE2_CLOCK_HOUR_PM     (RTC_MODE2_CLOCK_HOUR_PM_Val   << RTC_MODE2_CLOCK_HOUR_Pos)
#define RTC_MODE2_CLOCK_DAY_Pos     17           /**< \brief (RTC_MODE2_CLOCK) Day */
#define RTC_MODE2_CLOCK_DAY_Msk     (_U_(0x1F) << RTC_MODE2_CLOCK_DAY_Pos)
#define RTC_MODE2_CLOCK_DAY(value)  (RTC_MODE2_CLOCK_DAY_Msk & ((value) << RTC_MODE2_CLOCK_DAY_Pos))
#define RTC_MODE2_CLOCK_MONTH_Pos   22           /**< \brief (RTC_MODE2_CLOCK) Month */
#define RTC_MODE2_CLOCK_MONTH_Msk   (_U_(0xF) << RTC_MODE2_CLOCK_MONTH_Pos)
#define RTC_MODE2_CLOCK_MONTH(value) (RTC_MODE2_CLOCK_MONTH_Msk & ((value) << RTC_MODE2_CLOCK_MONTH_Pos))
#define RTC_MODE2_CLOCK_YEAR_Pos    26           /**< \brief (RTC_MODE2_CLOCK) Year */
#define RTC_MODE2_CLOCK_YEAR_Msk    (_U_(0x3F) << RTC_MODE2_CLOCK_YEAR_Pos)
#define RTC_MODE2_CLOCK_YEAR(value) (RTC_MODE2_CLOCK_YEAR_Msk & ((value) << RTC_MODE2_CLOCK_YEAR_Pos))
#define RTC_MODE2_CLOCK_MASK        _U_(0xFFFFFFFF) /**< \brief (RTC_MODE2_CLOCK) MASK Register */

/* -------- RTC_MODE1_PER : (RTC Offset: 0x14) (R/W 16) MODE1 MODE1 Counter Period -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t PER:16;           /*!< bit:  0..15  Counter Period                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_PER_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_PER_OFFSET        0x14         /**< \brief (RTC_MODE1_PER offset) MODE1 Counter Period */
#define RTC_MODE1_PER_RESETVALUE    _U_(0x0000)  /**< \brief (RTC_MODE1_PER reset_value) MODE1 Counter Period */

#define RTC_MODE1_PER_PER_Pos       0            /**< \brief (RTC_MODE1_PER) Counter Period */
#define RTC_MODE1_PER_PER_Msk       (_U_(0xFFFF) << RTC_MODE1_PER_PER_Pos)
#define RTC_MODE1_PER_PER(value)    (RTC_MODE1_PER_PER_Msk & ((value) << RTC_MODE1_PER_PER_Pos))
#define RTC_MODE1_PER_MASK          _U_(0xFFFF)  /**< \brief (RTC_MODE1_PER) MASK Register */

/* -------- RTC_MODE0_COMP : (RTC Offset: 0x18) (R/W 32) MODE0 MODE0 Compare n Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t COMP:32;          /*!< bit:  0..31  Compare Value                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE0_COMP_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE0_COMP_OFFSET       0x18         /**< \brief (RTC_MODE0_COMP offset) MODE0 Compare n Value */
#define RTC_MODE0_COMP_RESETVALUE   _U_(0x00000000) /**< \brief (RTC_MODE0_COMP reset_value) MODE0 Compare n Value */

#define RTC_MODE0_COMP_COMP_Pos     0            /**< \brief (RTC_MODE0_COMP) Compare Value */
#define RTC_MODE0_COMP_COMP_Msk     (_U_(0xFFFFFFFF) << RTC_MODE0_COMP_COMP_Pos)
#define RTC_MODE0_COMP_COMP(value)  (RTC_MODE0_COMP_COMP_Msk & ((value) << RTC_MODE0_COMP_COMP_Pos))
#define RTC_MODE0_COMP_MASK         _U_(0xFFFFFFFF) /**< \brief (RTC_MODE0_COMP) MASK Register */

/* -------- RTC_MODE1_COMP : (RTC Offset: 0x18) (R/W 16) MODE1 MODE1 Compare n Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t COMP:16;          /*!< bit:  0..15  Compare Value                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} RTC_MODE1_COMP_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE1_COMP_OFFSET       0x18         /**< \brief (RTC_MODE1_COMP offset) MODE1 Compare n Value */
#define RTC_MODE1_COMP_RESETVALUE   _U_(0x0000)  /**< \brief (RTC_MODE1_COMP reset_value) MODE1 Compare n Value */

#define RTC_MODE1_COMP_COMP_Pos     0            /**< \brief (RTC_MODE1_COMP) Compare Value */
#define RTC_MODE1_COMP_COMP_Msk     (_U_(0xFFFF) << RTC_MODE1_COMP_COMP_Pos)
#define RTC_MODE1_COMP_COMP(value)  (RTC_MODE1_COMP_COMP_Msk & ((value) << RTC_MODE1_COMP_COMP_Pos))
#define RTC_MODE1_COMP_MASK         _U_(0xFFFF)  /**< \brief (RTC_MODE1_COMP) MASK Register */

/* -------- RTC_MODE2_ALARM : (RTC Offset: 0x18) (R/W 32) MODE2 MODE2_ALARM Alarm n Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t SECOND:6;         /*!< bit:  0.. 5  Second                             */
    uint32_t MINUTE:6;         /*!< bit:  6..11  Minute                             */
    uint32_t HOUR:5;           /*!< bit: 12..16  Hour                               */
    uint32_t DAY:5;            /*!< bit: 17..21  Day                                */
    uint32_t MONTH:4;          /*!< bit: 22..25  Month                              */
    uint32_t YEAR:6;           /*!< bit: 26..31  Year                               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} RTC_MODE2_ALARM_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_ALARM_OFFSET      0x18         /**< \brief (RTC_MODE2_ALARM offset) MODE2_ALARM Alarm n Value */
#define RTC_MODE2_ALARM_RESETVALUE  _U_(0x00000000) /**< \brief (RTC_MODE2_ALARM reset_value) MODE2_ALARM Alarm n Value */

#define RTC_MODE2_ALARM_SECOND_Pos  0            /**< \brief (RTC_MODE2_ALARM) Second */
#define RTC_MODE2_ALARM_SECOND_Msk  (_U_(0x3F) << RTC_MODE2_ALARM_SECOND_Pos)
#define RTC_MODE2_ALARM_SECOND(value) (RTC_MODE2_ALARM_SECOND_Msk & ((value) << RTC_MODE2_ALARM_SECOND_Pos))
#define RTC_MODE2_ALARM_MINUTE_Pos  6            /**< \brief (RTC_MODE2_ALARM) Minute */
#define RTC_MODE2_ALARM_MINUTE_Msk  (_U_(0x3F) << RTC_MODE2_ALARM_MINUTE_Pos)
#define RTC_MODE2_ALARM_MINUTE(value) (RTC_MODE2_ALARM_MINUTE_Msk & ((value) << RTC_MODE2_ALARM_MINUTE_Pos))
#define RTC_MODE2_ALARM_HOUR_Pos    12           /**< \brief (RTC_MODE2_ALARM) Hour */
#define RTC_MODE2_ALARM_HOUR_Msk    (_U_(0x1F) << RTC_MODE2_ALARM_HOUR_Pos)
#define RTC_MODE2_ALARM_HOUR(value) (RTC_MODE2_ALARM_HOUR_Msk & ((value) << RTC_MODE2_ALARM_HOUR_Pos))
#define   RTC_MODE2_ALARM_HOUR_AM_Val     _U_(0x0)   /**< \brief (RTC_MODE2_ALARM) Morning hour */
#define   RTC_MODE2_ALARM_HOUR_PM_Val     _U_(0x10)   /**< \brief (RTC_MODE2_ALARM) Afternoon hour */
#define RTC_MODE2_ALARM_HOUR_AM     (RTC_MODE2_ALARM_HOUR_AM_Val   << RTC_MODE2_ALARM_HOUR_Pos)
#define RTC_MODE2_ALARM_HOUR_PM     (RTC_MODE2_ALARM_HOUR_PM_Val   << RTC_MODE2_ALARM_HOUR_Pos)
#define RTC_MODE2_ALARM_DAY_Pos     17           /**< \brief (RTC_MODE2_ALARM) Day */
#define RTC_MODE2_ALARM_DAY_Msk     (_U_(0x1F) << RTC_MODE2_ALARM_DAY_Pos)
#define RTC_MODE2_ALARM_DAY(value)  (RTC_MODE2_ALARM_DAY_Msk & ((value) << RTC_MODE2_ALARM_DAY_Pos))
#define RTC_MODE2_ALARM_MONTH_Pos   22           /**< \brief (RTC_MODE2_ALARM) Month */
#define RTC_MODE2_ALARM_MONTH_Msk   (_U_(0xF) << RTC_MODE2_ALARM_MONTH_Pos)
#define RTC_MODE2_ALARM_MONTH(value) (RTC_MODE2_ALARM_MONTH_Msk & ((value) << RTC_MODE2_ALARM_MONTH_Pos))
#define RTC_MODE2_ALARM_YEAR_Pos    26           /**< \brief (RTC_MODE2_ALARM) Year */
#define RTC_MODE2_ALARM_YEAR_Msk    (_U_(0x3F) << RTC_MODE2_ALARM_YEAR_Pos)
#define RTC_MODE2_ALARM_YEAR(value) (RTC_MODE2_ALARM_YEAR_Msk & ((value) << RTC_MODE2_ALARM_YEAR_Pos))
#define RTC_MODE2_ALARM_MASK        _U_(0xFFFFFFFF) /**< \brief (RTC_MODE2_ALARM) MASK Register */

/* -------- RTC_MODE2_MASK : (RTC Offset: 0x1C) (R/W  8) MODE2 MODE2_ALARM Alarm n Mask -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SEL:3;            /*!< bit:  0.. 2  Alarm Mask Selection               */
    uint8_t  :5;               /*!< bit:  3.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} RTC_MODE2_MASK_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define RTC_MODE2_MASK_OFFSET       0x1C         /**< \brief (RTC_MODE2_MASK offset) MODE2_ALARM Alarm n Mask */
#define RTC_MODE2_MASK_RESETVALUE   _U_(0x00)    /**< \brief (RTC_MODE2_MASK reset_value) MODE2_ALARM Alarm n Mask */

#define RTC_MODE2_MASK_SEL_Pos      0            /**< \brief (RTC_MODE2_MASK) Alarm Mask Selection */
#define RTC_MODE2_MASK_SEL_Msk      (_U_(0x7) << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL(value)   (RTC_MODE2_MASK_SEL_Msk & ((value) << RTC_MODE2_MASK_SEL_Pos))
#define   RTC_MODE2_MASK_SEL_OFF_Val      _U_(0x0)   /**< \brief (RTC_MODE2_MASK) Alarm Disabled */
#define   RTC_MODE2_MASK_SEL_SS_Val       _U_(0x1)   /**< \brief (RTC_MODE2_MASK) Match seconds only */
#define   RTC_MODE2_MASK_SEL_MMSS_Val     _U_(0x2)   /**< \brief (RTC_MODE2_MASK) Match seconds and minutes only */
#define   RTC_MODE2_MASK_SEL_HHMMSS_Val   _U_(0x3)   /**< \brief (RTC_MODE2_MASK) Match seconds, minutes, and hours only */
#define   RTC_MODE2_MASK_SEL_DDHHMMSS_Val _U_(0x4)   /**< \brief (RTC_MODE2_MASK) Match seconds, minutes, hours, and days only */
#define   RTC_MODE2_MASK_SEL_MMDDHHMMSS_Val _U_(0x5)   /**< \brief (RTC_MODE2_MASK) Match seconds, minutes, hours, days, and months only */
#define   RTC_MODE2_MASK_SEL_YYMMDDHHMMSS_Val _U_(0x6)   /**< \brief (RTC_MODE2_MASK) Match seconds, minutes, hours, days, months, and years */
#define RTC_MODE2_MASK_SEL_OFF      (RTC_MODE2_MASK_SEL_OFF_Val    << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL_SS       (RTC_MODE2_MASK_SEL_SS_Val     << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL_MMSS     (RTC_MODE2_MASK_SEL_MMSS_Val   << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL_HHMMSS   (RTC_MODE2_MASK_SEL_HHMMSS_Val << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL_DDHHMMSS (RTC_MODE2_MASK_SEL_DDHHMMSS_Val << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL_MMDDHHMMSS (RTC_MODE2_MASK_SEL_MMDDHHMMSS_Val << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_SEL_YYMMDDHHMMSS (RTC_MODE2_MASK_SEL_YYMMDDHHMMSS_Val << RTC_MODE2_MASK_SEL_Pos)
#define RTC_MODE2_MASK_MASK         _U_(0x07)    /**< \brief (RTC_MODE2_MASK) MASK Register */

/** \brief RtcMode2Alarm hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __IO RTC_MODE2_ALARM_Type      ALARM;       /**< \brief Offset: 0x00 (R/W 32) MODE2_ALARM Alarm n Value */
  __IO RTC_MODE2_MASK_Type       MASK;        /**< \brief Offset: 0x04 (R/W  8) MODE2_ALARM Alarm n Mask */
       RoReg8                    Reserved1[0x3];
} RtcMode2Alarm;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief RTC_MODE0 hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { /* 32-bit Counter with Single 32-bit Compare */
  __IO RTC_MODE0_CTRL_Type       CTRL;        /**< \brief Offset: 0x00 (R/W 16) MODE0 Control */
  __IO RTC_READREQ_Type          READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request */
  __IO RTC_MODE0_EVCTRL_Type     EVCTRL;      /**< \brief Offset: 0x04 (R/W 16) MODE0 Event Control */
  __IO RTC_MODE0_INTENCLR_Type   INTENCLR;    /**< \brief Offset: 0x06 (R/W  8) MODE0 Interrupt Enable Clear */
  __IO RTC_MODE0_INTENSET_Type   INTENSET;    /**< \brief Offset: 0x07 (R/W  8) MODE0 Interrupt Enable Set */
  __IO RTC_MODE0_INTFLAG_Type    INTFLAG;     /**< \brief Offset: 0x08 (R/W  8) MODE0 Interrupt Flag Status and Clear */
       RoReg8                    Reserved1[0x1];
  __IO RTC_STATUS_Type           STATUS;      /**< \brief Offset: 0x0A (R/W  8) Status */
  __IO RTC_DBGCTRL_Type          DBGCTRL;     /**< \brief Offset: 0x0B (R/W  8) Debug Control */
  __IO RTC_FREQCORR_Type         FREQCORR;    /**< \brief Offset: 0x0C (R/W  8) Frequency Correction */
       RoReg8                    Reserved2[0x3];
  __IO RTC_MODE0_COUNT_Type      COUNT;       /**< \brief Offset: 0x10 (R/W 32) MODE0 Counter Value */
       RoReg8                    Reserved3[0x4];
  __IO RTC_MODE0_COMP_Type       COMP[1];     /**< \brief Offset: 0x18 (R/W 32) MODE0 Compare n Value */
} RtcMode0;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief RTC_MODE1 hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { /* 16-bit Counter with Two 16-bit Compares */
  __IO RTC_MODE1_CTRL_Type       CTRL;        /**< \brief Offset: 0x00 (R/W 16) MODE1 Control */
  __IO RTC_READREQ_Type          READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request */
  __IO RTC_MODE1_EVCTRL_Type     EVCTRL;      /**< \brief Offset: 0x04 (R/W 16) MODE1 Event Control */
  __IO RTC_MODE1_INTENCLR_Type   INTENCLR;    /**< \brief Offset: 0x06 (R/W  8) MODE1 Interrupt Enable Clear */
  __IO RTC_MODE1_INTENSET_Type   INTENSET;    /**< \brief Offset: 0x07 (R/W  8) MODE1 Interrupt Enable Set */
  __IO RTC_MODE1_INTFLAG_Type    INTFLAG;     /**< \brief Offset: 0x08 (R/W  8) MODE1 Interrupt Flag Status and Clear */
       RoReg8                    Reserved1[0x1];
  __IO RTC_STATUS_Type           STATUS;      /**< \brief Offset: 0x0A (R/W  8) Status */
  __IO RTC_DBGCTRL_Type          DBGCTRL;     /**< \brief Offset: 0x0B (R/W  8) Debug Control */
  __IO RTC_FREQCORR_Type         FREQCORR;    /**< \brief Offset: 0x0C (R/W  8) Frequency Correction */
       RoReg8                    Reserved2[0x3];
  __IO RTC_MODE1_COUNT_Type      COUNT;       /**< \brief Offset: 0x10 (R/W 16) MODE1 Counter Value */
       RoReg8                    Reserved3[0x2];
  __IO RTC_MODE1_PER_Type        PER;         /**< \brief Offset: 0x14 (R/W 16) MODE1 Counter Period */
       RoReg8                    Reserved4[0x2];
  __IO RTC_MODE1_COMP_Type       COMP[2];     /**< \brief Offset: 0x18 (R/W 16) MODE1 Compare n Value */
} RtcMode1;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief RTC_MODE2 hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { /* Clock/Calendar with Alarm */
  __IO RTC_MODE2_CTRL_Type       CTRL;        /**< \brief Offset: 0x00 (R/W 16) MODE2 Control */
  __IO RTC_READREQ_Type          READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request */
  __IO RTC_MODE2_EVCTRL_Type     EVCTRL;      /**< \brief Offset: 0x04 (R/W 16) MODE2 Event Control */
  __IO RTC_MODE2_INTENCLR_Type   INTENCLR;    /**< \brief Offset: 0x06 (R/W  8) MODE2 Interrupt Enable Clear */
  __IO RTC_MODE2_INTENSET_Type   INTENSET;    /**< \brief Offset: 0x07 (R/W  8) MODE2 Interrupt Enable Set */
  __IO RTC_MODE2_INTFLAG_Type    INTFLAG;     /**< \brief Offset: 0x08 (R/W  8) MODE2 Interrupt Flag Status and Clear */
       RoReg8                    Reserved1[0x1];
  __IO RTC_STATUS_Type           STATUS;      /**< \brief Offset: 0x0A (R/W  8) Status */
  __IO RTC_DBGCTRL_Type          DBGCTRL;     /**< \brief Offset: 0x0B (R/W  8) Debug Control */
  __IO RTC_FREQCORR_Type         FREQCORR;    /**< \brief Offset: 0x0C (R/W  8) Frequency Correction */
       RoReg8                    Reserved2[0x3];
  __IO RTC_MODE2_CLOCK_Type      CLOCK;       /**< \brief Offset: 0x10 (R/W 32) MODE2 Clock Value */
       RoReg8                    Reserved3[0x4];
       RtcMode2Alarm             Mode2Alarm[1]; /**< \brief Offset: 0x18 RtcMode2Alarm groups [ALARM_NUM] */
} RtcMode2;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
       RtcMode0                  MODE0;       /**< \brief Offset: 0x00 32-bit Counter with Single 32-bit Compare */
       RtcMode1                  MODE1;       /**< \brief Offset: 0x00 16-bit Counter with Two 16-bit Compares */
       RtcMode2                  MODE2;       /**< \brief Offset: 0x00 Clock/Calendar with Alarm */
} Rtc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_RTC_COMPONENT_ */
