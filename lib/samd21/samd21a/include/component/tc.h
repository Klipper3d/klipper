/**
 * \file
 *
 * \brief Component description for TC
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

#ifndef _SAMD21_TC_COMPONENT_
#define _SAMD21_TC_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR TC */
/* ========================================================================== */
/** \addtogroup SAMD21_TC Basic Timer Counter */
/*@{*/

#define TC_U2212
#define REV_TC                      0x121

/* -------- TC_CTRLA : (TC Offset: 0x00) (R/W 16) Control A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint16_t ENABLE:1;         /*!< bit:      1  Enable                             */
    uint16_t MODE:2;           /*!< bit:  2.. 3  TC Mode                            */
    uint16_t :1;               /*!< bit:      4  Reserved                           */
    uint16_t WAVEGEN:2;        /*!< bit:  5.. 6  Waveform Generation Operation      */
    uint16_t :1;               /*!< bit:      7  Reserved                           */
    uint16_t PRESCALER:3;      /*!< bit:  8..10  Prescaler                          */
    uint16_t RUNSTDBY:1;       /*!< bit:     11  Run in Standby                     */
    uint16_t PRESCSYNC:2;      /*!< bit: 12..13  Prescaler and Counter Synchronization */
    uint16_t :2;               /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_CTRLA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_CTRLA_OFFSET             0x00         /**< \brief (TC_CTRLA offset) Control A */
#define TC_CTRLA_RESETVALUE         _U_(0x0000)  /**< \brief (TC_CTRLA reset_value) Control A */

#define TC_CTRLA_SWRST_Pos          0            /**< \brief (TC_CTRLA) Software Reset */
#define TC_CTRLA_SWRST              (_U_(0x1) << TC_CTRLA_SWRST_Pos)
#define TC_CTRLA_ENABLE_Pos         1            /**< \brief (TC_CTRLA) Enable */
#define TC_CTRLA_ENABLE             (_U_(0x1) << TC_CTRLA_ENABLE_Pos)
#define TC_CTRLA_MODE_Pos           2            /**< \brief (TC_CTRLA) TC Mode */
#define TC_CTRLA_MODE_Msk           (_U_(0x3) << TC_CTRLA_MODE_Pos)
#define TC_CTRLA_MODE(value)        (TC_CTRLA_MODE_Msk & ((value) << TC_CTRLA_MODE_Pos))
#define   TC_CTRLA_MODE_COUNT16_Val       _U_(0x0)   /**< \brief (TC_CTRLA) Counter in 16-bit mode */
#define   TC_CTRLA_MODE_COUNT8_Val        _U_(0x1)   /**< \brief (TC_CTRLA) Counter in 8-bit mode */
#define   TC_CTRLA_MODE_COUNT32_Val       _U_(0x2)   /**< \brief (TC_CTRLA) Counter in 32-bit mode */
#define TC_CTRLA_MODE_COUNT16       (TC_CTRLA_MODE_COUNT16_Val     << TC_CTRLA_MODE_Pos)
#define TC_CTRLA_MODE_COUNT8        (TC_CTRLA_MODE_COUNT8_Val      << TC_CTRLA_MODE_Pos)
#define TC_CTRLA_MODE_COUNT32       (TC_CTRLA_MODE_COUNT32_Val     << TC_CTRLA_MODE_Pos)
#define TC_CTRLA_WAVEGEN_Pos        5            /**< \brief (TC_CTRLA) Waveform Generation Operation */
#define TC_CTRLA_WAVEGEN_Msk        (_U_(0x3) << TC_CTRLA_WAVEGEN_Pos)
#define TC_CTRLA_WAVEGEN(value)     (TC_CTRLA_WAVEGEN_Msk & ((value) << TC_CTRLA_WAVEGEN_Pos))
#define   TC_CTRLA_WAVEGEN_NFRQ_Val       _U_(0x0)   /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_MFRQ_Val       _U_(0x1)   /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_NPWM_Val       _U_(0x2)   /**< \brief (TC_CTRLA)  */
#define   TC_CTRLA_WAVEGEN_MPWM_Val       _U_(0x3)   /**< \brief (TC_CTRLA)  */
#define TC_CTRLA_WAVEGEN_NFRQ       (TC_CTRLA_WAVEGEN_NFRQ_Val     << TC_CTRLA_WAVEGEN_Pos)
#define TC_CTRLA_WAVEGEN_MFRQ       (TC_CTRLA_WAVEGEN_MFRQ_Val     << TC_CTRLA_WAVEGEN_Pos)
#define TC_CTRLA_WAVEGEN_NPWM       (TC_CTRLA_WAVEGEN_NPWM_Val     << TC_CTRLA_WAVEGEN_Pos)
#define TC_CTRLA_WAVEGEN_MPWM       (TC_CTRLA_WAVEGEN_MPWM_Val     << TC_CTRLA_WAVEGEN_Pos)
#define TC_CTRLA_PRESCALER_Pos      8            /**< \brief (TC_CTRLA) Prescaler */
#define TC_CTRLA_PRESCALER_Msk      (_U_(0x7) << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER(value)   (TC_CTRLA_PRESCALER_Msk & ((value) << TC_CTRLA_PRESCALER_Pos))
#define   TC_CTRLA_PRESCALER_DIV1_Val     _U_(0x0)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC */
#define   TC_CTRLA_PRESCALER_DIV2_Val     _U_(0x1)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/2 */
#define   TC_CTRLA_PRESCALER_DIV4_Val     _U_(0x2)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/4 */
#define   TC_CTRLA_PRESCALER_DIV8_Val     _U_(0x3)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/8 */
#define   TC_CTRLA_PRESCALER_DIV16_Val    _U_(0x4)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/16 */
#define   TC_CTRLA_PRESCALER_DIV64_Val    _U_(0x5)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/64 */
#define   TC_CTRLA_PRESCALER_DIV256_Val   _U_(0x6)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/256 */
#define   TC_CTRLA_PRESCALER_DIV1024_Val  _U_(0x7)   /**< \brief (TC_CTRLA) Prescaler: GCLK_TC/1024 */
#define TC_CTRLA_PRESCALER_DIV1     (TC_CTRLA_PRESCALER_DIV1_Val   << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV2     (TC_CTRLA_PRESCALER_DIV2_Val   << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV4     (TC_CTRLA_PRESCALER_DIV4_Val   << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV8     (TC_CTRLA_PRESCALER_DIV8_Val   << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV16    (TC_CTRLA_PRESCALER_DIV16_Val  << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV64    (TC_CTRLA_PRESCALER_DIV64_Val  << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV256   (TC_CTRLA_PRESCALER_DIV256_Val << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_PRESCALER_DIV1024  (TC_CTRLA_PRESCALER_DIV1024_Val << TC_CTRLA_PRESCALER_Pos)
#define TC_CTRLA_RUNSTDBY_Pos       11           /**< \brief (TC_CTRLA) Run in Standby */
#define TC_CTRLA_RUNSTDBY           (_U_(0x1) << TC_CTRLA_RUNSTDBY_Pos)
#define TC_CTRLA_PRESCSYNC_Pos      12           /**< \brief (TC_CTRLA) Prescaler and Counter Synchronization */
#define TC_CTRLA_PRESCSYNC_Msk      (_U_(0x3) << TC_CTRLA_PRESCSYNC_Pos)
#define TC_CTRLA_PRESCSYNC(value)   (TC_CTRLA_PRESCSYNC_Msk & ((value) << TC_CTRLA_PRESCSYNC_Pos))
#define   TC_CTRLA_PRESCSYNC_GCLK_Val     _U_(0x0)   /**< \brief (TC_CTRLA) Reload or reset the counter on next generic clock */
#define   TC_CTRLA_PRESCSYNC_PRESC_Val    _U_(0x1)   /**< \brief (TC_CTRLA) Reload or reset the counter on next prescaler clock */
#define   TC_CTRLA_PRESCSYNC_RESYNC_Val   _U_(0x2)   /**< \brief (TC_CTRLA) Reload or reset the counter on next generic clock. Reset the prescaler counter */
#define TC_CTRLA_PRESCSYNC_GCLK     (TC_CTRLA_PRESCSYNC_GCLK_Val   << TC_CTRLA_PRESCSYNC_Pos)
#define TC_CTRLA_PRESCSYNC_PRESC    (TC_CTRLA_PRESCSYNC_PRESC_Val  << TC_CTRLA_PRESCSYNC_Pos)
#define TC_CTRLA_PRESCSYNC_RESYNC   (TC_CTRLA_PRESCSYNC_RESYNC_Val << TC_CTRLA_PRESCSYNC_Pos)
#define TC_CTRLA_MASK               _U_(0x3F6F)  /**< \brief (TC_CTRLA) MASK Register */

/* -------- TC_READREQ : (TC Offset: 0x02) (R/W 16) Read Request -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t ADDR:5;           /*!< bit:  0.. 4  Address                            */
    uint16_t :9;               /*!< bit:  5..13  Reserved                           */
    uint16_t RCONT:1;          /*!< bit:     14  Read Continuously                  */
    uint16_t RREQ:1;           /*!< bit:     15  Read Request                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_READREQ_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_READREQ_OFFSET           0x02         /**< \brief (TC_READREQ offset) Read Request */
#define TC_READREQ_RESETVALUE       _U_(0x0000)  /**< \brief (TC_READREQ reset_value) Read Request */

#define TC_READREQ_ADDR_Pos         0            /**< \brief (TC_READREQ) Address */
#define TC_READREQ_ADDR_Msk         (_U_(0x1F) << TC_READREQ_ADDR_Pos)
#define TC_READREQ_ADDR(value)      (TC_READREQ_ADDR_Msk & ((value) << TC_READREQ_ADDR_Pos))
#define TC_READREQ_RCONT_Pos        14           /**< \brief (TC_READREQ) Read Continuously */
#define TC_READREQ_RCONT            (_U_(0x1) << TC_READREQ_RCONT_Pos)
#define TC_READREQ_RREQ_Pos         15           /**< \brief (TC_READREQ) Read Request */
#define TC_READREQ_RREQ             (_U_(0x1) << TC_READREQ_RREQ_Pos)
#define TC_READREQ_MASK             _U_(0xC01F)  /**< \brief (TC_READREQ) MASK Register */

/* -------- TC_CTRLBCLR : (TC Offset: 0x04) (R/W  8) Control B Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DIR:1;            /*!< bit:      0  Counter Direction                  */
    uint8_t  :1;               /*!< bit:      1  Reserved                           */
    uint8_t  ONESHOT:1;        /*!< bit:      2  One-Shot                           */
    uint8_t  :3;               /*!< bit:  3.. 5  Reserved                           */
    uint8_t  CMD:2;            /*!< bit:  6.. 7  Command                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLBCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_CTRLBCLR_OFFSET          0x04         /**< \brief (TC_CTRLBCLR offset) Control B Clear */
#define TC_CTRLBCLR_RESETVALUE      _U_(0x02)    /**< \brief (TC_CTRLBCLR reset_value) Control B Clear */

#define TC_CTRLBCLR_DIR_Pos         0            /**< \brief (TC_CTRLBCLR) Counter Direction */
#define TC_CTRLBCLR_DIR             (_U_(0x1) << TC_CTRLBCLR_DIR_Pos)
#define TC_CTRLBCLR_ONESHOT_Pos     2            /**< \brief (TC_CTRLBCLR) One-Shot */
#define TC_CTRLBCLR_ONESHOT         (_U_(0x1) << TC_CTRLBCLR_ONESHOT_Pos)
#define TC_CTRLBCLR_CMD_Pos         6            /**< \brief (TC_CTRLBCLR) Command */
#define TC_CTRLBCLR_CMD_Msk         (_U_(0x3) << TC_CTRLBCLR_CMD_Pos)
#define TC_CTRLBCLR_CMD(value)      (TC_CTRLBCLR_CMD_Msk & ((value) << TC_CTRLBCLR_CMD_Pos))
#define   TC_CTRLBCLR_CMD_NONE_Val        _U_(0x0)   /**< \brief (TC_CTRLBCLR) No action */
#define   TC_CTRLBCLR_CMD_RETRIGGER_Val   _U_(0x1)   /**< \brief (TC_CTRLBCLR) Force a start, restart or retrigger */
#define   TC_CTRLBCLR_CMD_STOP_Val        _U_(0x2)   /**< \brief (TC_CTRLBCLR) Force a stop */
#define TC_CTRLBCLR_CMD_NONE        (TC_CTRLBCLR_CMD_NONE_Val      << TC_CTRLBCLR_CMD_Pos)
#define TC_CTRLBCLR_CMD_RETRIGGER   (TC_CTRLBCLR_CMD_RETRIGGER_Val << TC_CTRLBCLR_CMD_Pos)
#define TC_CTRLBCLR_CMD_STOP        (TC_CTRLBCLR_CMD_STOP_Val      << TC_CTRLBCLR_CMD_Pos)
#define TC_CTRLBCLR_MASK            _U_(0xC5)    /**< \brief (TC_CTRLBCLR) MASK Register */

/* -------- TC_CTRLBSET : (TC Offset: 0x05) (R/W  8) Control B Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DIR:1;            /*!< bit:      0  Counter Direction                  */
    uint8_t  :1;               /*!< bit:      1  Reserved                           */
    uint8_t  ONESHOT:1;        /*!< bit:      2  One-Shot                           */
    uint8_t  :3;               /*!< bit:  3.. 5  Reserved                           */
    uint8_t  CMD:2;            /*!< bit:  6.. 7  Command                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLBSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_CTRLBSET_OFFSET          0x05         /**< \brief (TC_CTRLBSET offset) Control B Set */
#define TC_CTRLBSET_RESETVALUE      _U_(0x00)    /**< \brief (TC_CTRLBSET reset_value) Control B Set */

#define TC_CTRLBSET_DIR_Pos         0            /**< \brief (TC_CTRLBSET) Counter Direction */
#define TC_CTRLBSET_DIR             (_U_(0x1) << TC_CTRLBSET_DIR_Pos)
#define TC_CTRLBSET_ONESHOT_Pos     2            /**< \brief (TC_CTRLBSET) One-Shot */
#define TC_CTRLBSET_ONESHOT         (_U_(0x1) << TC_CTRLBSET_ONESHOT_Pos)
#define TC_CTRLBSET_CMD_Pos         6            /**< \brief (TC_CTRLBSET) Command */
#define TC_CTRLBSET_CMD_Msk         (_U_(0x3) << TC_CTRLBSET_CMD_Pos)
#define TC_CTRLBSET_CMD(value)      (TC_CTRLBSET_CMD_Msk & ((value) << TC_CTRLBSET_CMD_Pos))
#define   TC_CTRLBSET_CMD_NONE_Val        _U_(0x0)   /**< \brief (TC_CTRLBSET) No action */
#define   TC_CTRLBSET_CMD_RETRIGGER_Val   _U_(0x1)   /**< \brief (TC_CTRLBSET) Force a start, restart or retrigger */
#define   TC_CTRLBSET_CMD_STOP_Val        _U_(0x2)   /**< \brief (TC_CTRLBSET) Force a stop */
#define TC_CTRLBSET_CMD_NONE        (TC_CTRLBSET_CMD_NONE_Val      << TC_CTRLBSET_CMD_Pos)
#define TC_CTRLBSET_CMD_RETRIGGER   (TC_CTRLBSET_CMD_RETRIGGER_Val << TC_CTRLBSET_CMD_Pos)
#define TC_CTRLBSET_CMD_STOP        (TC_CTRLBSET_CMD_STOP_Val      << TC_CTRLBSET_CMD_Pos)
#define TC_CTRLBSET_MASK            _U_(0xC5)    /**< \brief (TC_CTRLBSET) MASK Register */

/* -------- TC_CTRLC : (TC Offset: 0x06) (R/W  8) Control C -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  INVEN0:1;         /*!< bit:      0  Output Waveform 0 Invert Enable    */
    uint8_t  INVEN1:1;         /*!< bit:      1  Output Waveform 1 Invert Enable    */
    uint8_t  :2;               /*!< bit:  2.. 3  Reserved                           */
    uint8_t  CPTEN0:1;         /*!< bit:      4  Capture Channel 0 Enable           */
    uint8_t  CPTEN1:1;         /*!< bit:      5  Capture Channel 1 Enable           */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  INVEN:2;          /*!< bit:  0.. 1  Output Waveform x Invert Enable    */
    uint8_t  :2;               /*!< bit:  2.. 3  Reserved                           */
    uint8_t  CPTEN:2;          /*!< bit:  4.. 5  Capture Channel x Enable           */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_CTRLC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_CTRLC_OFFSET             0x06         /**< \brief (TC_CTRLC offset) Control C */
#define TC_CTRLC_RESETVALUE         _U_(0x00)    /**< \brief (TC_CTRLC reset_value) Control C */

#define TC_CTRLC_INVEN0_Pos         0            /**< \brief (TC_CTRLC) Output Waveform 0 Invert Enable */
#define TC_CTRLC_INVEN0             (_U_(1) << TC_CTRLC_INVEN0_Pos)
#define TC_CTRLC_INVEN1_Pos         1            /**< \brief (TC_CTRLC) Output Waveform 1 Invert Enable */
#define TC_CTRLC_INVEN1             (_U_(1) << TC_CTRLC_INVEN1_Pos)
#define TC_CTRLC_INVEN_Pos          0            /**< \brief (TC_CTRLC) Output Waveform x Invert Enable */
#define TC_CTRLC_INVEN_Msk          (_U_(0x3) << TC_CTRLC_INVEN_Pos)
#define TC_CTRLC_INVEN(value)       (TC_CTRLC_INVEN_Msk & ((value) << TC_CTRLC_INVEN_Pos))
#define TC_CTRLC_CPTEN0_Pos         4            /**< \brief (TC_CTRLC) Capture Channel 0 Enable */
#define TC_CTRLC_CPTEN0             (_U_(1) << TC_CTRLC_CPTEN0_Pos)
#define TC_CTRLC_CPTEN1_Pos         5            /**< \brief (TC_CTRLC) Capture Channel 1 Enable */
#define TC_CTRLC_CPTEN1             (_U_(1) << TC_CTRLC_CPTEN1_Pos)
#define TC_CTRLC_CPTEN_Pos          4            /**< \brief (TC_CTRLC) Capture Channel x Enable */
#define TC_CTRLC_CPTEN_Msk          (_U_(0x3) << TC_CTRLC_CPTEN_Pos)
#define TC_CTRLC_CPTEN(value)       (TC_CTRLC_CPTEN_Msk & ((value) << TC_CTRLC_CPTEN_Pos))
#define TC_CTRLC_MASK               _U_(0x33)    /**< \brief (TC_CTRLC) MASK Register */

/* -------- TC_DBGCTRL : (TC Offset: 0x08) (R/W  8) Debug Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DBGRUN:1;         /*!< bit:      0  Debug Run Mode                     */
    uint8_t  :7;               /*!< bit:  1.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_DBGCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_DBGCTRL_OFFSET           0x08         /**< \brief (TC_DBGCTRL offset) Debug Control */
#define TC_DBGCTRL_RESETVALUE       _U_(0x00)    /**< \brief (TC_DBGCTRL reset_value) Debug Control */

#define TC_DBGCTRL_DBGRUN_Pos       0            /**< \brief (TC_DBGCTRL) Debug Run Mode */
#define TC_DBGCTRL_DBGRUN           (_U_(0x1) << TC_DBGCTRL_DBGRUN_Pos)
#define TC_DBGCTRL_MASK             _U_(0x01)    /**< \brief (TC_DBGCTRL) MASK Register */

/* -------- TC_EVCTRL : (TC Offset: 0x0A) (R/W 16) Event Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t EVACT:3;          /*!< bit:  0.. 2  Event Action                       */
    uint16_t :1;               /*!< bit:      3  Reserved                           */
    uint16_t TCINV:1;          /*!< bit:      4  TC Inverted Event Input            */
    uint16_t TCEI:1;           /*!< bit:      5  TC Event Input                     */
    uint16_t :2;               /*!< bit:  6.. 7  Reserved                           */
    uint16_t OVFEO:1;          /*!< bit:      8  Overflow/Underflow Event Output Enable */
    uint16_t :3;               /*!< bit:  9..11  Reserved                           */
    uint16_t MCEO0:1;          /*!< bit:     12  Match or Capture Channel 0 Event Output Enable */
    uint16_t MCEO1:1;          /*!< bit:     13  Match or Capture Channel 1 Event Output Enable */
    uint16_t :2;               /*!< bit: 14..15  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint16_t :12;              /*!< bit:  0..11  Reserved                           */
    uint16_t MCEO:2;           /*!< bit: 12..13  Match or Capture Channel x Event Output Enable */
    uint16_t :2;               /*!< bit: 14..15  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_EVCTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_EVCTRL_OFFSET            0x0A         /**< \brief (TC_EVCTRL offset) Event Control */
#define TC_EVCTRL_RESETVALUE        _U_(0x0000)  /**< \brief (TC_EVCTRL reset_value) Event Control */

#define TC_EVCTRL_EVACT_Pos         0            /**< \brief (TC_EVCTRL) Event Action */
#define TC_EVCTRL_EVACT_Msk         (_U_(0x7) << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_EVACT(value)      (TC_EVCTRL_EVACT_Msk & ((value) << TC_EVCTRL_EVACT_Pos))
#define   TC_EVCTRL_EVACT_OFF_Val         _U_(0x0)   /**< \brief (TC_EVCTRL) Event action disabled */
#define   TC_EVCTRL_EVACT_RETRIGGER_Val   _U_(0x1)   /**< \brief (TC_EVCTRL) Start, restart or retrigger TC on event */
#define   TC_EVCTRL_EVACT_COUNT_Val       _U_(0x2)   /**< \brief (TC_EVCTRL) Count on event */
#define   TC_EVCTRL_EVACT_START_Val       _U_(0x3)   /**< \brief (TC_EVCTRL) Start TC on event */
#define   TC_EVCTRL_EVACT_PPW_Val         _U_(0x5)   /**< \brief (TC_EVCTRL) Period captured in CC0, pulse width in CC1 */
#define   TC_EVCTRL_EVACT_PWP_Val         _U_(0x6)   /**< \brief (TC_EVCTRL) Period captured in CC1, pulse width in CC0 */
#define TC_EVCTRL_EVACT_OFF         (TC_EVCTRL_EVACT_OFF_Val       << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_EVACT_RETRIGGER   (TC_EVCTRL_EVACT_RETRIGGER_Val << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_EVACT_COUNT       (TC_EVCTRL_EVACT_COUNT_Val     << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_EVACT_START       (TC_EVCTRL_EVACT_START_Val     << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_EVACT_PPW         (TC_EVCTRL_EVACT_PPW_Val       << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_EVACT_PWP         (TC_EVCTRL_EVACT_PWP_Val       << TC_EVCTRL_EVACT_Pos)
#define TC_EVCTRL_TCINV_Pos         4            /**< \brief (TC_EVCTRL) TC Inverted Event Input */
#define TC_EVCTRL_TCINV             (_U_(0x1) << TC_EVCTRL_TCINV_Pos)
#define TC_EVCTRL_TCEI_Pos          5            /**< \brief (TC_EVCTRL) TC Event Input */
#define TC_EVCTRL_TCEI              (_U_(0x1) << TC_EVCTRL_TCEI_Pos)
#define TC_EVCTRL_OVFEO_Pos         8            /**< \brief (TC_EVCTRL) Overflow/Underflow Event Output Enable */
#define TC_EVCTRL_OVFEO             (_U_(0x1) << TC_EVCTRL_OVFEO_Pos)
#define TC_EVCTRL_MCEO0_Pos         12           /**< \brief (TC_EVCTRL) Match or Capture Channel 0 Event Output Enable */
#define TC_EVCTRL_MCEO0             (_U_(1) << TC_EVCTRL_MCEO0_Pos)
#define TC_EVCTRL_MCEO1_Pos         13           /**< \brief (TC_EVCTRL) Match or Capture Channel 1 Event Output Enable */
#define TC_EVCTRL_MCEO1             (_U_(1) << TC_EVCTRL_MCEO1_Pos)
#define TC_EVCTRL_MCEO_Pos          12           /**< \brief (TC_EVCTRL) Match or Capture Channel x Event Output Enable */
#define TC_EVCTRL_MCEO_Msk          (_U_(0x3) << TC_EVCTRL_MCEO_Pos)
#define TC_EVCTRL_MCEO(value)       (TC_EVCTRL_MCEO_Msk & ((value) << TC_EVCTRL_MCEO_Pos))
#define TC_EVCTRL_MASK              _U_(0x3137)  /**< \brief (TC_EVCTRL) MASK Register */

/* -------- TC_INTENCLR : (TC Offset: 0x0C) (R/W  8) Interrupt Enable Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  Overflow Interrupt Enable          */
    uint8_t  ERR:1;            /*!< bit:      1  Error Interrupt Enable             */
    uint8_t  :1;               /*!< bit:      2  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      3  Synchronization Ready Interrupt Enable */
    uint8_t  MC0:1;            /*!< bit:      4  Match or Capture Channel 0 Interrupt Enable */
    uint8_t  MC1:1;            /*!< bit:      5  Match or Capture Channel 1 Interrupt Enable */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  :4;               /*!< bit:  0.. 3  Reserved                           */
    uint8_t  MC:2;             /*!< bit:  4.. 5  Match or Capture Channel x Interrupt Enable */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTENCLR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_INTENCLR_OFFSET          0x0C         /**< \brief (TC_INTENCLR offset) Interrupt Enable Clear */
#define TC_INTENCLR_RESETVALUE      _U_(0x00)    /**< \brief (TC_INTENCLR reset_value) Interrupt Enable Clear */

#define TC_INTENCLR_OVF_Pos         0            /**< \brief (TC_INTENCLR) Overflow Interrupt Enable */
#define TC_INTENCLR_OVF             (_U_(0x1) << TC_INTENCLR_OVF_Pos)
#define TC_INTENCLR_ERR_Pos         1            /**< \brief (TC_INTENCLR) Error Interrupt Enable */
#define TC_INTENCLR_ERR             (_U_(0x1) << TC_INTENCLR_ERR_Pos)
#define TC_INTENCLR_SYNCRDY_Pos     3            /**< \brief (TC_INTENCLR) Synchronization Ready Interrupt Enable */
#define TC_INTENCLR_SYNCRDY         (_U_(0x1) << TC_INTENCLR_SYNCRDY_Pos)
#define TC_INTENCLR_MC0_Pos         4            /**< \brief (TC_INTENCLR) Match or Capture Channel 0 Interrupt Enable */
#define TC_INTENCLR_MC0             (_U_(1) << TC_INTENCLR_MC0_Pos)
#define TC_INTENCLR_MC1_Pos         5            /**< \brief (TC_INTENCLR) Match or Capture Channel 1 Interrupt Enable */
#define TC_INTENCLR_MC1             (_U_(1) << TC_INTENCLR_MC1_Pos)
#define TC_INTENCLR_MC_Pos          4            /**< \brief (TC_INTENCLR) Match or Capture Channel x Interrupt Enable */
#define TC_INTENCLR_MC_Msk          (_U_(0x3) << TC_INTENCLR_MC_Pos)
#define TC_INTENCLR_MC(value)       (TC_INTENCLR_MC_Msk & ((value) << TC_INTENCLR_MC_Pos))
#define TC_INTENCLR_MASK            _U_(0x3B)    /**< \brief (TC_INTENCLR) MASK Register */

/* -------- TC_INTENSET : (TC Offset: 0x0D) (R/W  8) Interrupt Enable Set -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  OVF:1;            /*!< bit:      0  Overflow Interrupt Enable          */
    uint8_t  ERR:1;            /*!< bit:      1  Error Interrupt Enable             */
    uint8_t  :1;               /*!< bit:      2  Reserved                           */
    uint8_t  SYNCRDY:1;        /*!< bit:      3  Synchronization Ready Interrupt Enable */
    uint8_t  MC0:1;            /*!< bit:      4  Match or Capture Channel 0 Interrupt Enable */
    uint8_t  MC1:1;            /*!< bit:      5  Match or Capture Channel 1 Interrupt Enable */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  :4;               /*!< bit:  0.. 3  Reserved                           */
    uint8_t  MC:2;             /*!< bit:  4.. 5  Match or Capture Channel x Interrupt Enable */
    uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTENSET_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_INTENSET_OFFSET          0x0D         /**< \brief (TC_INTENSET offset) Interrupt Enable Set */
#define TC_INTENSET_RESETVALUE      _U_(0x00)    /**< \brief (TC_INTENSET reset_value) Interrupt Enable Set */

#define TC_INTENSET_OVF_Pos         0            /**< \brief (TC_INTENSET) Overflow Interrupt Enable */
#define TC_INTENSET_OVF             (_U_(0x1) << TC_INTENSET_OVF_Pos)
#define TC_INTENSET_ERR_Pos         1            /**< \brief (TC_INTENSET) Error Interrupt Enable */
#define TC_INTENSET_ERR             (_U_(0x1) << TC_INTENSET_ERR_Pos)
#define TC_INTENSET_SYNCRDY_Pos     3            /**< \brief (TC_INTENSET) Synchronization Ready Interrupt Enable */
#define TC_INTENSET_SYNCRDY         (_U_(0x1) << TC_INTENSET_SYNCRDY_Pos)
#define TC_INTENSET_MC0_Pos         4            /**< \brief (TC_INTENSET) Match or Capture Channel 0 Interrupt Enable */
#define TC_INTENSET_MC0             (_U_(1) << TC_INTENSET_MC0_Pos)
#define TC_INTENSET_MC1_Pos         5            /**< \brief (TC_INTENSET) Match or Capture Channel 1 Interrupt Enable */
#define TC_INTENSET_MC1             (_U_(1) << TC_INTENSET_MC1_Pos)
#define TC_INTENSET_MC_Pos          4            /**< \brief (TC_INTENSET) Match or Capture Channel x Interrupt Enable */
#define TC_INTENSET_MC_Msk          (_U_(0x3) << TC_INTENSET_MC_Pos)
#define TC_INTENSET_MC(value)       (TC_INTENSET_MC_Msk & ((value) << TC_INTENSET_MC_Pos))
#define TC_INTENSET_MASK            _U_(0x3B)    /**< \brief (TC_INTENSET) MASK Register */

/* -------- TC_INTFLAG : (TC Offset: 0x0E) (R/W  8) Interrupt Flag Status and Clear -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union { // __I to avoid read-modify-write on write-to-clear register
  struct {
    __I uint8_t  OVF:1;            /*!< bit:      0  Overflow                           */
    __I uint8_t  ERR:1;            /*!< bit:      1  Error                              */
    __I uint8_t  :1;               /*!< bit:      2  Reserved                           */
    __I uint8_t  SYNCRDY:1;        /*!< bit:      3  Synchronization Ready              */
    __I uint8_t  MC0:1;            /*!< bit:      4  Match or Capture Channel 0         */
    __I uint8_t  MC1:1;            /*!< bit:      5  Match or Capture Channel 1         */
    __I uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    __I uint8_t  :4;               /*!< bit:  0.. 3  Reserved                           */
    __I uint8_t  MC:2;             /*!< bit:  4.. 5  Match or Capture Channel x         */
    __I uint8_t  :2;               /*!< bit:  6.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_INTFLAG_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_INTFLAG_OFFSET           0x0E         /**< \brief (TC_INTFLAG offset) Interrupt Flag Status and Clear */
#define TC_INTFLAG_RESETVALUE       _U_(0x00)    /**< \brief (TC_INTFLAG reset_value) Interrupt Flag Status and Clear */

#define TC_INTFLAG_OVF_Pos          0            /**< \brief (TC_INTFLAG) Overflow */
#define TC_INTFLAG_OVF              (_U_(0x1) << TC_INTFLAG_OVF_Pos)
#define TC_INTFLAG_ERR_Pos          1            /**< \brief (TC_INTFLAG) Error */
#define TC_INTFLAG_ERR              (_U_(0x1) << TC_INTFLAG_ERR_Pos)
#define TC_INTFLAG_SYNCRDY_Pos      3            /**< \brief (TC_INTFLAG) Synchronization Ready */
#define TC_INTFLAG_SYNCRDY          (_U_(0x1) << TC_INTFLAG_SYNCRDY_Pos)
#define TC_INTFLAG_MC0_Pos          4            /**< \brief (TC_INTFLAG) Match or Capture Channel 0 */
#define TC_INTFLAG_MC0              (_U_(1) << TC_INTFLAG_MC0_Pos)
#define TC_INTFLAG_MC1_Pos          5            /**< \brief (TC_INTFLAG) Match or Capture Channel 1 */
#define TC_INTFLAG_MC1              (_U_(1) << TC_INTFLAG_MC1_Pos)
#define TC_INTFLAG_MC_Pos           4            /**< \brief (TC_INTFLAG) Match or Capture Channel x */
#define TC_INTFLAG_MC_Msk           (_U_(0x3) << TC_INTFLAG_MC_Pos)
#define TC_INTFLAG_MC(value)        (TC_INTFLAG_MC_Msk & ((value) << TC_INTFLAG_MC_Pos))
#define TC_INTFLAG_MASK             _U_(0x3B)    /**< \brief (TC_INTFLAG) MASK Register */

/* -------- TC_STATUS : (TC Offset: 0x0F) (R/   8) Status -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  :3;               /*!< bit:  0.. 2  Reserved                           */
    uint8_t  STOP:1;           /*!< bit:      3  Stop                               */
    uint8_t  SLAVE:1;          /*!< bit:      4  Slave                              */
    uint8_t  :2;               /*!< bit:  5.. 6  Reserved                           */
    uint8_t  SYNCBUSY:1;       /*!< bit:      7  Synchronization Busy               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_STATUS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_STATUS_OFFSET            0x0F         /**< \brief (TC_STATUS offset) Status */
#define TC_STATUS_RESETVALUE        _U_(0x08)    /**< \brief (TC_STATUS reset_value) Status */

#define TC_STATUS_STOP_Pos          3            /**< \brief (TC_STATUS) Stop */
#define TC_STATUS_STOP              (_U_(0x1) << TC_STATUS_STOP_Pos)
#define TC_STATUS_SLAVE_Pos         4            /**< \brief (TC_STATUS) Slave */
#define TC_STATUS_SLAVE             (_U_(0x1) << TC_STATUS_SLAVE_Pos)
#define TC_STATUS_SYNCBUSY_Pos      7            /**< \brief (TC_STATUS) Synchronization Busy */
#define TC_STATUS_SYNCBUSY          (_U_(0x1) << TC_STATUS_SYNCBUSY_Pos)
#define TC_STATUS_MASK              _U_(0x98)    /**< \brief (TC_STATUS) MASK Register */

/* -------- TC_COUNT16_COUNT : (TC Offset: 0x10) (R/W 16) COUNT16 COUNT16 Counter Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t COUNT:16;         /*!< bit:  0..15  Count Value                        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_COUNT16_COUNT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT16_COUNT_OFFSET     0x10         /**< \brief (TC_COUNT16_COUNT offset) COUNT16 Counter Value */
#define TC_COUNT16_COUNT_RESETVALUE _U_(0x0000)  /**< \brief (TC_COUNT16_COUNT reset_value) COUNT16 Counter Value */

#define TC_COUNT16_COUNT_COUNT_Pos  0            /**< \brief (TC_COUNT16_COUNT) Count Value */
#define TC_COUNT16_COUNT_COUNT_Msk  (_U_(0xFFFF) << TC_COUNT16_COUNT_COUNT_Pos)
#define TC_COUNT16_COUNT_COUNT(value) (TC_COUNT16_COUNT_COUNT_Msk & ((value) << TC_COUNT16_COUNT_COUNT_Pos))
#define TC_COUNT16_COUNT_MASK       _U_(0xFFFF)  /**< \brief (TC_COUNT16_COUNT) MASK Register */

/* -------- TC_COUNT32_COUNT : (TC Offset: 0x10) (R/W 32) COUNT32 COUNT32 Counter Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t COUNT:32;         /*!< bit:  0..31  Count Value                        */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} TC_COUNT32_COUNT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT32_COUNT_OFFSET     0x10         /**< \brief (TC_COUNT32_COUNT offset) COUNT32 Counter Value */
#define TC_COUNT32_COUNT_RESETVALUE _U_(0x00000000) /**< \brief (TC_COUNT32_COUNT reset_value) COUNT32 Counter Value */

#define TC_COUNT32_COUNT_COUNT_Pos  0            /**< \brief (TC_COUNT32_COUNT) Count Value */
#define TC_COUNT32_COUNT_COUNT_Msk  (_U_(0xFFFFFFFF) << TC_COUNT32_COUNT_COUNT_Pos)
#define TC_COUNT32_COUNT_COUNT(value) (TC_COUNT32_COUNT_COUNT_Msk & ((value) << TC_COUNT32_COUNT_COUNT_Pos))
#define TC_COUNT32_COUNT_MASK       _U_(0xFFFFFFFF) /**< \brief (TC_COUNT32_COUNT) MASK Register */

/* -------- TC_COUNT8_COUNT : (TC Offset: 0x10) (R/W  8) COUNT8 COUNT8 Counter Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  COUNT:8;          /*!< bit:  0.. 7  Counter Value                      */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_COUNT_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT8_COUNT_OFFSET      0x10         /**< \brief (TC_COUNT8_COUNT offset) COUNT8 Counter Value */
#define TC_COUNT8_COUNT_RESETVALUE  _U_(0x00)    /**< \brief (TC_COUNT8_COUNT reset_value) COUNT8 Counter Value */

#define TC_COUNT8_COUNT_COUNT_Pos   0            /**< \brief (TC_COUNT8_COUNT) Counter Value */
#define TC_COUNT8_COUNT_COUNT_Msk   (_U_(0xFF) << TC_COUNT8_COUNT_COUNT_Pos)
#define TC_COUNT8_COUNT_COUNT(value) (TC_COUNT8_COUNT_COUNT_Msk & ((value) << TC_COUNT8_COUNT_COUNT_Pos))
#define TC_COUNT8_COUNT_MASK        _U_(0xFF)    /**< \brief (TC_COUNT8_COUNT) MASK Register */

/* -------- TC_COUNT8_PER : (TC Offset: 0x14) (R/W  8) COUNT8 COUNT8 Period Value -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  PER:8;            /*!< bit:  0.. 7  Period Value                       */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_PER_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT8_PER_OFFSET        0x14         /**< \brief (TC_COUNT8_PER offset) COUNT8 Period Value */
#define TC_COUNT8_PER_RESETVALUE    _U_(0xFF)    /**< \brief (TC_COUNT8_PER reset_value) COUNT8 Period Value */

#define TC_COUNT8_PER_PER_Pos       0            /**< \brief (TC_COUNT8_PER) Period Value */
#define TC_COUNT8_PER_PER_Msk       (_U_(0xFF) << TC_COUNT8_PER_PER_Pos)
#define TC_COUNT8_PER_PER(value)    (TC_COUNT8_PER_PER_Msk & ((value) << TC_COUNT8_PER_PER_Pos))
#define TC_COUNT8_PER_MASK          _U_(0xFF)    /**< \brief (TC_COUNT8_PER) MASK Register */

/* -------- TC_COUNT16_CC : (TC Offset: 0x18) (R/W 16) COUNT16 COUNT16 Compare/Capture -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint16_t CC:16;            /*!< bit:  0..15  Compare/Capture Value              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint16_t reg;                /*!< Type      used for register access              */
} TC_COUNT16_CC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT16_CC_OFFSET        0x18         /**< \brief (TC_COUNT16_CC offset) COUNT16 Compare/Capture */
#define TC_COUNT16_CC_RESETVALUE    _U_(0x0000)  /**< \brief (TC_COUNT16_CC reset_value) COUNT16 Compare/Capture */

#define TC_COUNT16_CC_CC_Pos        0            /**< \brief (TC_COUNT16_CC) Compare/Capture Value */
#define TC_COUNT16_CC_CC_Msk        (_U_(0xFFFF) << TC_COUNT16_CC_CC_Pos)
#define TC_COUNT16_CC_CC(value)     (TC_COUNT16_CC_CC_Msk & ((value) << TC_COUNT16_CC_CC_Pos))
#define TC_COUNT16_CC_MASK          _U_(0xFFFF)  /**< \brief (TC_COUNT16_CC) MASK Register */

/* -------- TC_COUNT32_CC : (TC Offset: 0x18) (R/W 32) COUNT32 COUNT32 Compare/Capture -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t CC:32;            /*!< bit:  0..31  Compare/Capture Value              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} TC_COUNT32_CC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT32_CC_OFFSET        0x18         /**< \brief (TC_COUNT32_CC offset) COUNT32 Compare/Capture */
#define TC_COUNT32_CC_RESETVALUE    _U_(0x00000000) /**< \brief (TC_COUNT32_CC reset_value) COUNT32 Compare/Capture */

#define TC_COUNT32_CC_CC_Pos        0            /**< \brief (TC_COUNT32_CC) Compare/Capture Value */
#define TC_COUNT32_CC_CC_Msk        (_U_(0xFFFFFFFF) << TC_COUNT32_CC_CC_Pos)
#define TC_COUNT32_CC_CC(value)     (TC_COUNT32_CC_CC_Msk & ((value) << TC_COUNT32_CC_CC_Pos))
#define TC_COUNT32_CC_MASK          _U_(0xFFFFFFFF) /**< \brief (TC_COUNT32_CC) MASK Register */

/* -------- TC_COUNT8_CC : (TC Offset: 0x18) (R/W  8) COUNT8 COUNT8 Compare/Capture -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  CC:8;             /*!< bit:  0.. 7  Compare/Capture Value              */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} TC_COUNT8_CC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define TC_COUNT8_CC_OFFSET         0x18         /**< \brief (TC_COUNT8_CC offset) COUNT8 Compare/Capture */
#define TC_COUNT8_CC_RESETVALUE     _U_(0x00)    /**< \brief (TC_COUNT8_CC reset_value) COUNT8 Compare/Capture */

#define TC_COUNT8_CC_CC_Pos         0            /**< \brief (TC_COUNT8_CC) Compare/Capture Value */
#define TC_COUNT8_CC_CC_Msk         (_U_(0xFF) << TC_COUNT8_CC_CC_Pos)
#define TC_COUNT8_CC_CC(value)      (TC_COUNT8_CC_CC_Msk & ((value) << TC_COUNT8_CC_CC_Pos))
#define TC_COUNT8_CC_MASK           _U_(0xFF)    /**< \brief (TC_COUNT8_CC) MASK Register */

/** \brief TC_COUNT8 hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { /* 8-bit Counter Mode */
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control A */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control B Clear */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control B Set */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control C */
       RoReg8                    Reserved1[0x1];
  __IO TC_DBGCTRL_Type           DBGCTRL;     /**< \brief Offset: 0x08 (R/W  8) Debug Control */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Status and Clear */
  __I  TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/   8) Status */
  __IO TC_COUNT8_COUNT_Type      COUNT;       /**< \brief Offset: 0x10 (R/W  8) COUNT8 Counter Value */
       RoReg8                    Reserved3[0x3];
  __IO TC_COUNT8_PER_Type        PER;         /**< \brief Offset: 0x14 (R/W  8) COUNT8 Period Value */
       RoReg8                    Reserved4[0x3];
  __IO TC_COUNT8_CC_Type         CC[2];       /**< \brief Offset: 0x18 (R/W  8) COUNT8 Compare/Capture */
} TcCount8;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief TC_COUNT16 hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { /* 16-bit Counter Mode */
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control A */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control B Clear */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control B Set */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control C */
       RoReg8                    Reserved1[0x1];
  __IO TC_DBGCTRL_Type           DBGCTRL;     /**< \brief Offset: 0x08 (R/W  8) Debug Control */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Status and Clear */
  __I  TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/   8) Status */
  __IO TC_COUNT16_COUNT_Type     COUNT;       /**< \brief Offset: 0x10 (R/W 16) COUNT16 Counter Value */
       RoReg8                    Reserved3[0x6];
  __IO TC_COUNT16_CC_Type        CC[2];       /**< \brief Offset: 0x18 (R/W 16) COUNT16 Compare/Capture */
} TcCount16;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief TC_COUNT32 hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { /* 32-bit Counter Mode */
  __IO TC_CTRLA_Type             CTRLA;       /**< \brief Offset: 0x00 (R/W 16) Control A */
  __IO TC_READREQ_Type           READREQ;     /**< \brief Offset: 0x02 (R/W 16) Read Request */
  __IO TC_CTRLBCLR_Type          CTRLBCLR;    /**< \brief Offset: 0x04 (R/W  8) Control B Clear */
  __IO TC_CTRLBSET_Type          CTRLBSET;    /**< \brief Offset: 0x05 (R/W  8) Control B Set */
  __IO TC_CTRLC_Type             CTRLC;       /**< \brief Offset: 0x06 (R/W  8) Control C */
       RoReg8                    Reserved1[0x1];
  __IO TC_DBGCTRL_Type           DBGCTRL;     /**< \brief Offset: 0x08 (R/W  8) Debug Control */
       RoReg8                    Reserved2[0x1];
  __IO TC_EVCTRL_Type            EVCTRL;      /**< \brief Offset: 0x0A (R/W 16) Event Control */
  __IO TC_INTENCLR_Type          INTENCLR;    /**< \brief Offset: 0x0C (R/W  8) Interrupt Enable Clear */
  __IO TC_INTENSET_Type          INTENSET;    /**< \brief Offset: 0x0D (R/W  8) Interrupt Enable Set */
  __IO TC_INTFLAG_Type           INTFLAG;     /**< \brief Offset: 0x0E (R/W  8) Interrupt Flag Status and Clear */
  __I  TC_STATUS_Type            STATUS;      /**< \brief Offset: 0x0F (R/   8) Status */
  __IO TC_COUNT32_COUNT_Type     COUNT;       /**< \brief Offset: 0x10 (R/W 32) COUNT32 Counter Value */
       RoReg8                    Reserved3[0x4];
  __IO TC_COUNT32_CC_Type        CC[2];       /**< \brief Offset: 0x18 (R/W 32) COUNT32 Compare/Capture */
} TcCount32;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
       TcCount8                  COUNT8;      /**< \brief Offset: 0x00 8-bit Counter Mode */
       TcCount16                 COUNT16;     /**< \brief Offset: 0x00 16-bit Counter Mode */
       TcCount32                 COUNT32;     /**< \brief Offset: 0x00 32-bit Counter Mode */
} Tc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_TC_COMPONENT_ */
