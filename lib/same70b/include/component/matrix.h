/**
 * \file
 *
 * \brief Component description for MATRIX
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
#ifndef _SAME70_MATRIX_COMPONENT_H_
#define _SAME70_MATRIX_COMPONENT_H_
#define _SAME70_MATRIX_COMPONENT_         /**< \deprecated  Backward compatibility for ASF */

/** \addtogroup SAME_SAME70 AHB Bus Matrix
 *  @{
 */
/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR MATRIX */
/* ========================================================================== */
#ifndef COMPONENT_TYPEDEF_STYLE
  #define COMPONENT_TYPEDEF_STYLE 'R'  /**< Defines default style of typedefs for the component header files ('R' = RFO, 'N' = NTO)*/
#endif

#define MATRIX_11282                      /**< (MATRIX) Module ID */
#define REV_MATRIX L                      /**< (MATRIX) Module revision */

/* -------- MATRIX_PRAS : (MATRIX Offset: 0x00) (R/W 32) Priority Register A for Slave 0 -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t M0PR:2;                    /**< bit:   0..1  Master 0 Priority                        */
    uint32_t :2;                        /**< bit:   2..3  Reserved */
    uint32_t M1PR:2;                    /**< bit:   4..5  Master 1 Priority                        */
    uint32_t :2;                        /**< bit:   6..7  Reserved */
    uint32_t M2PR:2;                    /**< bit:   8..9  Master 2 Priority                        */
    uint32_t :2;                        /**< bit: 10..11  Reserved */
    uint32_t M3PR:2;                    /**< bit: 12..13  Master 3 Priority                        */
    uint32_t :2;                        /**< bit: 14..15  Reserved */
    uint32_t M4PR:2;                    /**< bit: 16..17  Master 4 Priority                        */
    uint32_t :2;                        /**< bit: 18..19  Reserved */
    uint32_t M5PR:2;                    /**< bit: 20..21  Master 5 Priority                        */
    uint32_t :2;                        /**< bit: 22..23  Reserved */
    uint32_t M6PR:2;                    /**< bit: 24..25  Master 6 Priority                        */
    uint32_t :6;                        /**< bit: 26..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_PRAS_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_PRAS_OFFSET                  (0x00)                                        /**<  (MATRIX_PRAS) Priority Register A for Slave 0  Offset */

#define MATRIX_PRAS_M0PR_Pos                0                                              /**< (MATRIX_PRAS) Master 0 Priority Position */
#define MATRIX_PRAS_M0PR_Msk                (_U_(0x3) << MATRIX_PRAS_M0PR_Pos)             /**< (MATRIX_PRAS) Master 0 Priority Mask */
#define MATRIX_PRAS_M0PR(value)             (MATRIX_PRAS_M0PR_Msk & ((value) << MATRIX_PRAS_M0PR_Pos))
#define MATRIX_PRAS_M1PR_Pos                4                                              /**< (MATRIX_PRAS) Master 1 Priority Position */
#define MATRIX_PRAS_M1PR_Msk                (_U_(0x3) << MATRIX_PRAS_M1PR_Pos)             /**< (MATRIX_PRAS) Master 1 Priority Mask */
#define MATRIX_PRAS_M1PR(value)             (MATRIX_PRAS_M1PR_Msk & ((value) << MATRIX_PRAS_M1PR_Pos))
#define MATRIX_PRAS_M2PR_Pos                8                                              /**< (MATRIX_PRAS) Master 2 Priority Position */
#define MATRIX_PRAS_M2PR_Msk                (_U_(0x3) << MATRIX_PRAS_M2PR_Pos)             /**< (MATRIX_PRAS) Master 2 Priority Mask */
#define MATRIX_PRAS_M2PR(value)             (MATRIX_PRAS_M2PR_Msk & ((value) << MATRIX_PRAS_M2PR_Pos))
#define MATRIX_PRAS_M3PR_Pos                12                                             /**< (MATRIX_PRAS) Master 3 Priority Position */
#define MATRIX_PRAS_M3PR_Msk                (_U_(0x3) << MATRIX_PRAS_M3PR_Pos)             /**< (MATRIX_PRAS) Master 3 Priority Mask */
#define MATRIX_PRAS_M3PR(value)             (MATRIX_PRAS_M3PR_Msk & ((value) << MATRIX_PRAS_M3PR_Pos))
#define MATRIX_PRAS_M4PR_Pos                16                                             /**< (MATRIX_PRAS) Master 4 Priority Position */
#define MATRIX_PRAS_M4PR_Msk                (_U_(0x3) << MATRIX_PRAS_M4PR_Pos)             /**< (MATRIX_PRAS) Master 4 Priority Mask */
#define MATRIX_PRAS_M4PR(value)             (MATRIX_PRAS_M4PR_Msk & ((value) << MATRIX_PRAS_M4PR_Pos))
#define MATRIX_PRAS_M5PR_Pos                20                                             /**< (MATRIX_PRAS) Master 5 Priority Position */
#define MATRIX_PRAS_M5PR_Msk                (_U_(0x3) << MATRIX_PRAS_M5PR_Pos)             /**< (MATRIX_PRAS) Master 5 Priority Mask */
#define MATRIX_PRAS_M5PR(value)             (MATRIX_PRAS_M5PR_Msk & ((value) << MATRIX_PRAS_M5PR_Pos))
#define MATRIX_PRAS_M6PR_Pos                24                                             /**< (MATRIX_PRAS) Master 6 Priority Position */
#define MATRIX_PRAS_M6PR_Msk                (_U_(0x3) << MATRIX_PRAS_M6PR_Pos)             /**< (MATRIX_PRAS) Master 6 Priority Mask */
#define MATRIX_PRAS_M6PR(value)             (MATRIX_PRAS_M6PR_Msk & ((value) << MATRIX_PRAS_M6PR_Pos))
#define MATRIX_PRAS_MASK                    _U_(0x3333333)                                 /**< \deprecated (MATRIX_PRAS) Register MASK  (Use MATRIX_PRAS_Msk instead)  */
#define MATRIX_PRAS_Msk                     _U_(0x3333333)                                 /**< (MATRIX_PRAS) Register Mask  */


/* -------- MATRIX_PRBS : (MATRIX Offset: 0x04) (R/W 32) Priority Register B for Slave 0 -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t M8PR:2;                    /**< bit:   0..1  Master 8 Priority                        */
    uint32_t :2;                        /**< bit:   2..3  Reserved */
    uint32_t M9PR:2;                    /**< bit:   4..5  Master 9 Priority                        */
    uint32_t :2;                        /**< bit:   6..7  Reserved */
    uint32_t M10PR:2;                   /**< bit:   8..9  Master 10 Priority                       */
    uint32_t :2;                        /**< bit: 10..11  Reserved */
    uint32_t M11PR:2;                   /**< bit: 12..13  Master 11 Priority                       */
    uint32_t :2;                        /**< bit: 14..15  Reserved */
    uint32_t M12PR:2;                   /**< bit: 16..17  Master 12 Priority                       */
    uint32_t :14;                       /**< bit: 18..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_PRBS_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_PRBS_OFFSET                  (0x04)                                        /**<  (MATRIX_PRBS) Priority Register B for Slave 0  Offset */

#define MATRIX_PRBS_M8PR_Pos                0                                              /**< (MATRIX_PRBS) Master 8 Priority Position */
#define MATRIX_PRBS_M8PR_Msk                (_U_(0x3) << MATRIX_PRBS_M8PR_Pos)             /**< (MATRIX_PRBS) Master 8 Priority Mask */
#define MATRIX_PRBS_M8PR(value)             (MATRIX_PRBS_M8PR_Msk & ((value) << MATRIX_PRBS_M8PR_Pos))
#define MATRIX_PRBS_M9PR_Pos                4                                              /**< (MATRIX_PRBS) Master 9 Priority Position */
#define MATRIX_PRBS_M9PR_Msk                (_U_(0x3) << MATRIX_PRBS_M9PR_Pos)             /**< (MATRIX_PRBS) Master 9 Priority Mask */
#define MATRIX_PRBS_M9PR(value)             (MATRIX_PRBS_M9PR_Msk & ((value) << MATRIX_PRBS_M9PR_Pos))
#define MATRIX_PRBS_M10PR_Pos               8                                              /**< (MATRIX_PRBS) Master 10 Priority Position */
#define MATRIX_PRBS_M10PR_Msk               (_U_(0x3) << MATRIX_PRBS_M10PR_Pos)            /**< (MATRIX_PRBS) Master 10 Priority Mask */
#define MATRIX_PRBS_M10PR(value)            (MATRIX_PRBS_M10PR_Msk & ((value) << MATRIX_PRBS_M10PR_Pos))
#define MATRIX_PRBS_M11PR_Pos               12                                             /**< (MATRIX_PRBS) Master 11 Priority Position */
#define MATRIX_PRBS_M11PR_Msk               (_U_(0x3) << MATRIX_PRBS_M11PR_Pos)            /**< (MATRIX_PRBS) Master 11 Priority Mask */
#define MATRIX_PRBS_M11PR(value)            (MATRIX_PRBS_M11PR_Msk & ((value) << MATRIX_PRBS_M11PR_Pos))
#define MATRIX_PRBS_M12PR_Pos               16                                             /**< (MATRIX_PRBS) Master 12 Priority Position */
#define MATRIX_PRBS_M12PR_Msk               (_U_(0x3) << MATRIX_PRBS_M12PR_Pos)            /**< (MATRIX_PRBS) Master 12 Priority Mask */
#define MATRIX_PRBS_M12PR(value)            (MATRIX_PRBS_M12PR_Msk & ((value) << MATRIX_PRBS_M12PR_Pos))
#define MATRIX_PRBS_MASK                    _U_(0x33333)                                   /**< \deprecated (MATRIX_PRBS) Register MASK  (Use MATRIX_PRBS_Msk instead)  */
#define MATRIX_PRBS_Msk                     _U_(0x33333)                                   /**< (MATRIX_PRBS) Register Mask  */


/* -------- MATRIX_MCFG : (MATRIX Offset: 0x00) (R/W 32) Master Configuration Register 0 -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t ULBT:3;                    /**< bit:   0..2  Undefined Length Burst Type              */
    uint32_t :29;                       /**< bit:  3..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_MCFG_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_MCFG_OFFSET                  (0x00)                                        /**<  (MATRIX_MCFG) Master Configuration Register 0  Offset */

#define MATRIX_MCFG_ULBT_Pos                0                                              /**< (MATRIX_MCFG) Undefined Length Burst Type Position */
#define MATRIX_MCFG_ULBT_Msk                (_U_(0x7) << MATRIX_MCFG_ULBT_Pos)             /**< (MATRIX_MCFG) Undefined Length Burst Type Mask */
#define MATRIX_MCFG_ULBT(value)             (MATRIX_MCFG_ULBT_Msk & ((value) << MATRIX_MCFG_ULBT_Pos))
#define   MATRIX_MCFG_ULBT_UNLTD_LENGTH_Val _U_(0x0)                                       /**< (MATRIX_MCFG) Unlimited Length Burst-No predicted end of burst is generated, therefore INCR bursts coming from this master can only be broken if the Slave Slot Cycle Limit is reached. If the Slot Cycle Limit is not reached, the burst is normally completed by the master, at the latest, on the next AHB 1-Kbyte address boundary, allowing up to 256-beat word bursts or 128-beat double-word bursts.This value should not be used in the very particular case of a master capable of performing back-to-back undefined length bursts on a single slave, since this could indefinitely freeze the slave arbitration and thus prevent another master from accessing this slave.  */
#define   MATRIX_MCFG_ULBT_SINGLE_ACCESS_Val _U_(0x1)                                       /**< (MATRIX_MCFG) Single Access-The undefined length burst is treated as a succession of single accesses, allowing re-arbitration at each beat of the INCR burst or bursts sequence.  */
#define   MATRIX_MCFG_ULBT_4BEAT_BURST_Val  _U_(0x2)                                       /**< (MATRIX_MCFG) 4-beat Burst-The undefined length burst or bursts sequence is split into 4-beat bursts or less, allowing re-arbitration every 4 beats.  */
#define   MATRIX_MCFG_ULBT_8BEAT_BURST_Val  _U_(0x3)                                       /**< (MATRIX_MCFG) 8-beat Burst-The undefined length burst or bursts sequence is split into 8-beat bursts or less, allowing re-arbitration every 8 beats.  */
#define   MATRIX_MCFG_ULBT_16BEAT_BURST_Val _U_(0x4)                                       /**< (MATRIX_MCFG) 16-beat Burst-The undefined length burst or bursts sequence is split into 16-beat bursts or less, allowing re-arbitration every 16 beats.  */
#define   MATRIX_MCFG_ULBT_32BEAT_BURST_Val _U_(0x5)                                       /**< (MATRIX_MCFG) 32-beat Burst -The undefined length burst or bursts sequence is split into 32-beat bursts or less, allowing re-arbitration every 32 beats.  */
#define   MATRIX_MCFG_ULBT_64BEAT_BURST_Val _U_(0x6)                                       /**< (MATRIX_MCFG) 64-beat Burst-The undefined length burst or bursts sequence is split into 64-beat bursts or less, allowing re-arbitration every 64 beats.  */
#define   MATRIX_MCFG_ULBT_128BEAT_BURST_Val _U_(0x7)                                       /**< (MATRIX_MCFG) 128-beat Burst-The undefined length burst or bursts sequence is split into 128-beat bursts or less, allowing re-arbitration every 128 beats.  */
#define MATRIX_MCFG_ULBT_UNLTD_LENGTH       (MATRIX_MCFG_ULBT_UNLTD_LENGTH_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) Unlimited Length Burst-No predicted end of burst is generated, therefore INCR bursts coming from this master can only be broken if the Slave Slot Cycle Limit is reached. If the Slot Cycle Limit is not reached, the burst is normally completed by the master, at the latest, on the next AHB 1-Kbyte address boundary, allowing up to 256-beat word bursts or 128-beat double-word bursts.This value should not be used in the very particular case of a master capable of performing back-to-back undefined length bursts on a single slave, since this could indefinitely freeze the slave arbitration and thus prevent another master from accessing this slave. Position  */
#define MATRIX_MCFG_ULBT_SINGLE_ACCESS      (MATRIX_MCFG_ULBT_SINGLE_ACCESS_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) Single Access-The undefined length burst is treated as a succession of single accesses, allowing re-arbitration at each beat of the INCR burst or bursts sequence. Position  */
#define MATRIX_MCFG_ULBT_4BEAT_BURST        (MATRIX_MCFG_ULBT_4BEAT_BURST_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) 4-beat Burst-The undefined length burst or bursts sequence is split into 4-beat bursts or less, allowing re-arbitration every 4 beats. Position  */
#define MATRIX_MCFG_ULBT_8BEAT_BURST        (MATRIX_MCFG_ULBT_8BEAT_BURST_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) 8-beat Burst-The undefined length burst or bursts sequence is split into 8-beat bursts or less, allowing re-arbitration every 8 beats. Position  */
#define MATRIX_MCFG_ULBT_16BEAT_BURST       (MATRIX_MCFG_ULBT_16BEAT_BURST_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) 16-beat Burst-The undefined length burst or bursts sequence is split into 16-beat bursts or less, allowing re-arbitration every 16 beats. Position  */
#define MATRIX_MCFG_ULBT_32BEAT_BURST       (MATRIX_MCFG_ULBT_32BEAT_BURST_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) 32-beat Burst -The undefined length burst or bursts sequence is split into 32-beat bursts or less, allowing re-arbitration every 32 beats. Position  */
#define MATRIX_MCFG_ULBT_64BEAT_BURST       (MATRIX_MCFG_ULBT_64BEAT_BURST_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) 64-beat Burst-The undefined length burst or bursts sequence is split into 64-beat bursts or less, allowing re-arbitration every 64 beats. Position  */
#define MATRIX_MCFG_ULBT_128BEAT_BURST      (MATRIX_MCFG_ULBT_128BEAT_BURST_Val << MATRIX_MCFG_ULBT_Pos)  /**< (MATRIX_MCFG) 128-beat Burst-The undefined length burst or bursts sequence is split into 128-beat bursts or less, allowing re-arbitration every 128 beats. Position  */
#define MATRIX_MCFG_MASK                    _U_(0x07)                                      /**< \deprecated (MATRIX_MCFG) Register MASK  (Use MATRIX_MCFG_Msk instead)  */
#define MATRIX_MCFG_Msk                     _U_(0x07)                                      /**< (MATRIX_MCFG) Register Mask  */


/* -------- MATRIX_SCFG : (MATRIX Offset: 0x40) (R/W 32) Slave Configuration Register 0 -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t SLOT_CYCLE:9;              /**< bit:   0..8  Maximum Bus Grant Duration for Masters   */
    uint32_t :7;                        /**< bit:  9..15  Reserved */
    uint32_t DEFMSTR_TYPE:2;            /**< bit: 16..17  Default Master Type                      */
    uint32_t FIXED_DEFMSTR:4;           /**< bit: 18..21  Fixed Default Master                     */
    uint32_t :10;                       /**< bit: 22..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_SCFG_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_SCFG_OFFSET                  (0x40)                                        /**<  (MATRIX_SCFG) Slave Configuration Register 0  Offset */

#define MATRIX_SCFG_SLOT_CYCLE_Pos          0                                              /**< (MATRIX_SCFG) Maximum Bus Grant Duration for Masters Position */
#define MATRIX_SCFG_SLOT_CYCLE_Msk          (_U_(0x1FF) << MATRIX_SCFG_SLOT_CYCLE_Pos)     /**< (MATRIX_SCFG) Maximum Bus Grant Duration for Masters Mask */
#define MATRIX_SCFG_SLOT_CYCLE(value)       (MATRIX_SCFG_SLOT_CYCLE_Msk & ((value) << MATRIX_SCFG_SLOT_CYCLE_Pos))
#define MATRIX_SCFG_DEFMSTR_TYPE_Pos        16                                             /**< (MATRIX_SCFG) Default Master Type Position */
#define MATRIX_SCFG_DEFMSTR_TYPE_Msk        (_U_(0x3) << MATRIX_SCFG_DEFMSTR_TYPE_Pos)     /**< (MATRIX_SCFG) Default Master Type Mask */
#define MATRIX_SCFG_DEFMSTR_TYPE(value)     (MATRIX_SCFG_DEFMSTR_TYPE_Msk & ((value) << MATRIX_SCFG_DEFMSTR_TYPE_Pos))
#define   MATRIX_SCFG_DEFMSTR_TYPE_NONE_Val _U_(0x0)                                       /**< (MATRIX_SCFG) No Default Master-At the end of the current slave access, if no other master request is pending, the slave is disconnected from all masters.This results in a one clock cycle latency for the first access of a burst transfer or for a single access.  */
#define   MATRIX_SCFG_DEFMSTR_TYPE_LAST_Val _U_(0x1)                                       /**< (MATRIX_SCFG) Last Default Master-At the end of the current slave access, if no other master request is pending, the slave stays connected to the last master having accessed it.This results in not having one clock cycle latency when the last master tries to access the slave again.  */
#define   MATRIX_SCFG_DEFMSTR_TYPE_FIXED_Val _U_(0x2)                                       /**< (MATRIX_SCFG) Fixed Default Master-At the end of the current slave access, if no other master request is pending, the slave connects to the fixed master the number that has been written in the FIXED_DEFMSTR field.This results in not having one clock cycle latency when the fixed master tries to access the slave again.  */
#define MATRIX_SCFG_DEFMSTR_TYPE_NONE       (MATRIX_SCFG_DEFMSTR_TYPE_NONE_Val << MATRIX_SCFG_DEFMSTR_TYPE_Pos)  /**< (MATRIX_SCFG) No Default Master-At the end of the current slave access, if no other master request is pending, the slave is disconnected from all masters.This results in a one clock cycle latency for the first access of a burst transfer or for a single access. Position  */
#define MATRIX_SCFG_DEFMSTR_TYPE_LAST       (MATRIX_SCFG_DEFMSTR_TYPE_LAST_Val << MATRIX_SCFG_DEFMSTR_TYPE_Pos)  /**< (MATRIX_SCFG) Last Default Master-At the end of the current slave access, if no other master request is pending, the slave stays connected to the last master having accessed it.This results in not having one clock cycle latency when the last master tries to access the slave again. Position  */
#define MATRIX_SCFG_DEFMSTR_TYPE_FIXED      (MATRIX_SCFG_DEFMSTR_TYPE_FIXED_Val << MATRIX_SCFG_DEFMSTR_TYPE_Pos)  /**< (MATRIX_SCFG) Fixed Default Master-At the end of the current slave access, if no other master request is pending, the slave connects to the fixed master the number that has been written in the FIXED_DEFMSTR field.This results in not having one clock cycle latency when the fixed master tries to access the slave again. Position  */
#define MATRIX_SCFG_FIXED_DEFMSTR_Pos       18                                             /**< (MATRIX_SCFG) Fixed Default Master Position */
#define MATRIX_SCFG_FIXED_DEFMSTR_Msk       (_U_(0xF) << MATRIX_SCFG_FIXED_DEFMSTR_Pos)    /**< (MATRIX_SCFG) Fixed Default Master Mask */
#define MATRIX_SCFG_FIXED_DEFMSTR(value)    (MATRIX_SCFG_FIXED_DEFMSTR_Msk & ((value) << MATRIX_SCFG_FIXED_DEFMSTR_Pos))
#define MATRIX_SCFG_MASK                    _U_(0x3F01FF)                                  /**< \deprecated (MATRIX_SCFG) Register MASK  (Use MATRIX_SCFG_Msk instead)  */
#define MATRIX_SCFG_Msk                     _U_(0x3F01FF)                                  /**< (MATRIX_SCFG) Register Mask  */


/* -------- MATRIX_MRCR : (MATRIX Offset: 0x100) (R/W 32) Master Remap Control Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RCB0:1;                    /**< bit:      0  Remap Command Bit for Master 0           */
    uint32_t RCB1:1;                    /**< bit:      1  Remap Command Bit for Master 1           */
    uint32_t RCB2:1;                    /**< bit:      2  Remap Command Bit for Master 2           */
    uint32_t RCB3:1;                    /**< bit:      3  Remap Command Bit for Master 3           */
    uint32_t RCB4:1;                    /**< bit:      4  Remap Command Bit for Master 4           */
    uint32_t RCB5:1;                    /**< bit:      5  Remap Command Bit for Master 5           */
    uint32_t RCB6:1;                    /**< bit:      6  Remap Command Bit for Master 6           */
    uint32_t :1;                        /**< bit:      7  Reserved */
    uint32_t RCB8:1;                    /**< bit:      8  Remap Command Bit for Master 8           */
    uint32_t RCB9:1;                    /**< bit:      9  Remap Command Bit for Master 9           */
    uint32_t RCB10:1;                   /**< bit:     10  Remap Command Bit for Master 10          */
    uint32_t RCB11:1;                   /**< bit:     11  Remap Command Bit for Master 11          */
    uint32_t RCB12:1;                   /**< bit:     12  Remap Command Bit for Master 12          */
    uint32_t :19;                       /**< bit: 13..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct {
    uint32_t RCB:12;                    /**< bit:  0..11  Remap Command Bit for Master x2          */
    uint32_t :20;                       /**< bit: 12..31 Reserved */
  } vec;                                /**< Structure used for vec  access  */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_MRCR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_MRCR_OFFSET                  (0x100)                                       /**<  (MATRIX_MRCR) Master Remap Control Register  Offset */

#define MATRIX_MRCR_RCB0_Pos                0                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 0 Position */
#define MATRIX_MRCR_RCB0_Msk                (_U_(0x1) << MATRIX_MRCR_RCB0_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 0 Mask */
#define MATRIX_MRCR_RCB0                    MATRIX_MRCR_RCB0_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB0_Msk instead */
#define MATRIX_MRCR_RCB1_Pos                1                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 1 Position */
#define MATRIX_MRCR_RCB1_Msk                (_U_(0x1) << MATRIX_MRCR_RCB1_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 1 Mask */
#define MATRIX_MRCR_RCB1                    MATRIX_MRCR_RCB1_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB1_Msk instead */
#define MATRIX_MRCR_RCB2_Pos                2                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 2 Position */
#define MATRIX_MRCR_RCB2_Msk                (_U_(0x1) << MATRIX_MRCR_RCB2_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 2 Mask */
#define MATRIX_MRCR_RCB2                    MATRIX_MRCR_RCB2_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB2_Msk instead */
#define MATRIX_MRCR_RCB3_Pos                3                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 3 Position */
#define MATRIX_MRCR_RCB3_Msk                (_U_(0x1) << MATRIX_MRCR_RCB3_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 3 Mask */
#define MATRIX_MRCR_RCB3                    MATRIX_MRCR_RCB3_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB3_Msk instead */
#define MATRIX_MRCR_RCB4_Pos                4                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 4 Position */
#define MATRIX_MRCR_RCB4_Msk                (_U_(0x1) << MATRIX_MRCR_RCB4_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 4 Mask */
#define MATRIX_MRCR_RCB4                    MATRIX_MRCR_RCB4_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB4_Msk instead */
#define MATRIX_MRCR_RCB5_Pos                5                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 5 Position */
#define MATRIX_MRCR_RCB5_Msk                (_U_(0x1) << MATRIX_MRCR_RCB5_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 5 Mask */
#define MATRIX_MRCR_RCB5                    MATRIX_MRCR_RCB5_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB5_Msk instead */
#define MATRIX_MRCR_RCB6_Pos                6                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 6 Position */
#define MATRIX_MRCR_RCB6_Msk                (_U_(0x1) << MATRIX_MRCR_RCB6_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 6 Mask */
#define MATRIX_MRCR_RCB6                    MATRIX_MRCR_RCB6_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB6_Msk instead */
#define MATRIX_MRCR_RCB8_Pos                8                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 8 Position */
#define MATRIX_MRCR_RCB8_Msk                (_U_(0x1) << MATRIX_MRCR_RCB8_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 8 Mask */
#define MATRIX_MRCR_RCB8                    MATRIX_MRCR_RCB8_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB8_Msk instead */
#define MATRIX_MRCR_RCB9_Pos                9                                              /**< (MATRIX_MRCR) Remap Command Bit for Master 9 Position */
#define MATRIX_MRCR_RCB9_Msk                (_U_(0x1) << MATRIX_MRCR_RCB9_Pos)             /**< (MATRIX_MRCR) Remap Command Bit for Master 9 Mask */
#define MATRIX_MRCR_RCB9                    MATRIX_MRCR_RCB9_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB9_Msk instead */
#define MATRIX_MRCR_RCB10_Pos               10                                             /**< (MATRIX_MRCR) Remap Command Bit for Master 10 Position */
#define MATRIX_MRCR_RCB10_Msk               (_U_(0x1) << MATRIX_MRCR_RCB10_Pos)            /**< (MATRIX_MRCR) Remap Command Bit for Master 10 Mask */
#define MATRIX_MRCR_RCB10                   MATRIX_MRCR_RCB10_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB10_Msk instead */
#define MATRIX_MRCR_RCB11_Pos               11                                             /**< (MATRIX_MRCR) Remap Command Bit for Master 11 Position */
#define MATRIX_MRCR_RCB11_Msk               (_U_(0x1) << MATRIX_MRCR_RCB11_Pos)            /**< (MATRIX_MRCR) Remap Command Bit for Master 11 Mask */
#define MATRIX_MRCR_RCB11                   MATRIX_MRCR_RCB11_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB11_Msk instead */
#define MATRIX_MRCR_RCB12_Pos               12                                             /**< (MATRIX_MRCR) Remap Command Bit for Master 12 Position */
#define MATRIX_MRCR_RCB12_Msk               (_U_(0x1) << MATRIX_MRCR_RCB12_Pos)            /**< (MATRIX_MRCR) Remap Command Bit for Master 12 Mask */
#define MATRIX_MRCR_RCB12                   MATRIX_MRCR_RCB12_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_MRCR_RCB12_Msk instead */
#define MATRIX_MRCR_MASK                    _U_(0x1F7F)                                    /**< \deprecated (MATRIX_MRCR) Register MASK  (Use MATRIX_MRCR_Msk instead)  */
#define MATRIX_MRCR_Msk                     _U_(0x1F7F)                                    /**< (MATRIX_MRCR) Register Mask  */

#define MATRIX_MRCR_RCB_Pos                 0                                              /**< (MATRIX_MRCR Position) Remap Command Bit for Master x2 */
#define MATRIX_MRCR_RCB_Msk                 (_U_(0xFFF) << MATRIX_MRCR_RCB_Pos)            /**< (MATRIX_MRCR Mask) RCB */
#define MATRIX_MRCR_RCB(value)              (MATRIX_MRCR_RCB_Msk & ((value) << MATRIX_MRCR_RCB_Pos))  

/* -------- CCFG_CAN0 : (MATRIX Offset: 0x110) (R/W 32) CAN0 Configuration Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :16;                       /**< bit:  0..15  Reserved */
    uint32_t CAN0DMABA:16;              /**< bit: 16..31  CAN0 DMA Base Address                    */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} CCFG_CAN0_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CCFG_CAN0_OFFSET                    (0x110)                                       /**<  (CCFG_CAN0) CAN0 Configuration Register  Offset */

#define CCFG_CAN0_CAN0DMABA_Pos             16                                             /**< (CCFG_CAN0) CAN0 DMA Base Address Position */
#define CCFG_CAN0_CAN0DMABA_Msk             (_U_(0xFFFF) << CCFG_CAN0_CAN0DMABA_Pos)       /**< (CCFG_CAN0) CAN0 DMA Base Address Mask */
#define CCFG_CAN0_CAN0DMABA(value)          (CCFG_CAN0_CAN0DMABA_Msk & ((value) << CCFG_CAN0_CAN0DMABA_Pos))
#define CCFG_CAN0_MASK                      _U_(0xFFFF0000)                                /**< \deprecated (CCFG_CAN0) Register MASK  (Use CCFG_CAN0_Msk instead)  */
#define CCFG_CAN0_Msk                       _U_(0xFFFF0000)                                /**< (CCFG_CAN0) Register Mask  */


/* -------- CCFG_SYSIO : (MATRIX Offset: 0x114) (R/W 32) System I/O and CAN1 Configuration Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :4;                        /**< bit:   0..3  Reserved */
    uint32_t SYSIO4:1;                  /**< bit:      4  PB4 or TDI Assignment                    */
    uint32_t SYSIO5:1;                  /**< bit:      5  PB5 or TDO/TRACESWO Assignment           */
    uint32_t SYSIO6:1;                  /**< bit:      6  PB6 or TMS/SWDIO Assignment              */
    uint32_t SYSIO7:1;                  /**< bit:      7  PB7 or TCK/SWCLK Assignment              */
    uint32_t :4;                        /**< bit:  8..11  Reserved */
    uint32_t SYSIO12:1;                 /**< bit:     12  PB12 or ERASE Assignment                 */
    uint32_t :3;                        /**< bit: 13..15  Reserved */
    uint32_t CAN1DMABA:16;              /**< bit: 16..31  CAN1 DMA Base Address                    */
  } bit;                                /**< Structure used for bit  access */
  struct {
    uint32_t :4;                        /**< bit:   0..3  Reserved */
    uint32_t SYSIO:5;                   /**< bit:   4..8  PB4 or TDI Assignment                    */
    uint32_t :23;                       /**< bit:  9..31 Reserved */
  } vec;                                /**< Structure used for vec  access  */
  uint32_t reg;                         /**< Type used for register access */
} CCFG_SYSIO_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CCFG_SYSIO_OFFSET                   (0x114)                                       /**<  (CCFG_SYSIO) System I/O and CAN1 Configuration Register  Offset */

#define CCFG_SYSIO_SYSIO4_Pos               4                                              /**< (CCFG_SYSIO) PB4 or TDI Assignment Position */
#define CCFG_SYSIO_SYSIO4_Msk               (_U_(0x1) << CCFG_SYSIO_SYSIO4_Pos)            /**< (CCFG_SYSIO) PB4 or TDI Assignment Mask */
#define CCFG_SYSIO_SYSIO4                   CCFG_SYSIO_SYSIO4_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SYSIO_SYSIO4_Msk instead */
#define CCFG_SYSIO_SYSIO5_Pos               5                                              /**< (CCFG_SYSIO) PB5 or TDO/TRACESWO Assignment Position */
#define CCFG_SYSIO_SYSIO5_Msk               (_U_(0x1) << CCFG_SYSIO_SYSIO5_Pos)            /**< (CCFG_SYSIO) PB5 or TDO/TRACESWO Assignment Mask */
#define CCFG_SYSIO_SYSIO5                   CCFG_SYSIO_SYSIO5_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SYSIO_SYSIO5_Msk instead */
#define CCFG_SYSIO_SYSIO6_Pos               6                                              /**< (CCFG_SYSIO) PB6 or TMS/SWDIO Assignment Position */
#define CCFG_SYSIO_SYSIO6_Msk               (_U_(0x1) << CCFG_SYSIO_SYSIO6_Pos)            /**< (CCFG_SYSIO) PB6 or TMS/SWDIO Assignment Mask */
#define CCFG_SYSIO_SYSIO6                   CCFG_SYSIO_SYSIO6_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SYSIO_SYSIO6_Msk instead */
#define CCFG_SYSIO_SYSIO7_Pos               7                                              /**< (CCFG_SYSIO) PB7 or TCK/SWCLK Assignment Position */
#define CCFG_SYSIO_SYSIO7_Msk               (_U_(0x1) << CCFG_SYSIO_SYSIO7_Pos)            /**< (CCFG_SYSIO) PB7 or TCK/SWCLK Assignment Mask */
#define CCFG_SYSIO_SYSIO7                   CCFG_SYSIO_SYSIO7_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SYSIO_SYSIO7_Msk instead */
#define CCFG_SYSIO_SYSIO12_Pos              12                                             /**< (CCFG_SYSIO) PB12 or ERASE Assignment Position */
#define CCFG_SYSIO_SYSIO12_Msk              (_U_(0x1) << CCFG_SYSIO_SYSIO12_Pos)           /**< (CCFG_SYSIO) PB12 or ERASE Assignment Mask */
#define CCFG_SYSIO_SYSIO12                  CCFG_SYSIO_SYSIO12_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SYSIO_SYSIO12_Msk instead */
#define CCFG_SYSIO_CAN1DMABA_Pos            16                                             /**< (CCFG_SYSIO) CAN1 DMA Base Address Position */
#define CCFG_SYSIO_CAN1DMABA_Msk            (_U_(0xFFFF) << CCFG_SYSIO_CAN1DMABA_Pos)      /**< (CCFG_SYSIO) CAN1 DMA Base Address Mask */
#define CCFG_SYSIO_CAN1DMABA(value)         (CCFG_SYSIO_CAN1DMABA_Msk & ((value) << CCFG_SYSIO_CAN1DMABA_Pos))
#define CCFG_SYSIO_MASK                     _U_(0xFFFF10F0)                                /**< \deprecated (CCFG_SYSIO) Register MASK  (Use CCFG_SYSIO_Msk instead)  */
#define CCFG_SYSIO_Msk                      _U_(0xFFFF10F0)                                /**< (CCFG_SYSIO) Register Mask  */

#define CCFG_SYSIO_SYSIO_Pos                4                                              /**< (CCFG_SYSIO Position) PB4 or TDI Assignment */
#define CCFG_SYSIO_SYSIO_Msk                (_U_(0x1F) << CCFG_SYSIO_SYSIO_Pos)            /**< (CCFG_SYSIO Mask) SYSIO */
#define CCFG_SYSIO_SYSIO(value)             (CCFG_SYSIO_SYSIO_Msk & ((value) << CCFG_SYSIO_SYSIO_Pos))  

/* -------- CCFG_PCCR : (MATRIX Offset: 0x118) (R/W 32) Peripheral Clock Configuration Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :20;                       /**< bit:  0..19  Reserved */
    uint32_t TC0CC:1;                   /**< bit:     20  TC0 Clock Configuration                  */
    uint32_t I2SC0CC:1;                 /**< bit:     21  I2SC0 Clock Configuration                */
    uint32_t I2SC1CC:1;                 /**< bit:     22  I2SC1 Clock Configuration                */
    uint32_t :9;                        /**< bit: 23..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} CCFG_PCCR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CCFG_PCCR_OFFSET                    (0x118)                                       /**<  (CCFG_PCCR) Peripheral Clock Configuration Register  Offset */

#define CCFG_PCCR_TC0CC_Pos                 20                                             /**< (CCFG_PCCR) TC0 Clock Configuration Position */
#define CCFG_PCCR_TC0CC_Msk                 (_U_(0x1) << CCFG_PCCR_TC0CC_Pos)              /**< (CCFG_PCCR) TC0 Clock Configuration Mask */
#define CCFG_PCCR_TC0CC                     CCFG_PCCR_TC0CC_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_PCCR_TC0CC_Msk instead */
#define CCFG_PCCR_I2SC0CC_Pos               21                                             /**< (CCFG_PCCR) I2SC0 Clock Configuration Position */
#define CCFG_PCCR_I2SC0CC_Msk               (_U_(0x1) << CCFG_PCCR_I2SC0CC_Pos)            /**< (CCFG_PCCR) I2SC0 Clock Configuration Mask */
#define CCFG_PCCR_I2SC0CC                   CCFG_PCCR_I2SC0CC_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_PCCR_I2SC0CC_Msk instead */
#define CCFG_PCCR_I2SC1CC_Pos               22                                             /**< (CCFG_PCCR) I2SC1 Clock Configuration Position */
#define CCFG_PCCR_I2SC1CC_Msk               (_U_(0x1) << CCFG_PCCR_I2SC1CC_Pos)            /**< (CCFG_PCCR) I2SC1 Clock Configuration Mask */
#define CCFG_PCCR_I2SC1CC                   CCFG_PCCR_I2SC1CC_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_PCCR_I2SC1CC_Msk instead */
#define CCFG_PCCR_MASK                      _U_(0x700000)                                  /**< \deprecated (CCFG_PCCR) Register MASK  (Use CCFG_PCCR_Msk instead)  */
#define CCFG_PCCR_Msk                       _U_(0x700000)                                  /**< (CCFG_PCCR) Register Mask  */


/* -------- CCFG_DYNCKG : (MATRIX Offset: 0x11c) (R/W 32) Dynamic Clock Gating Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t MATCKG:1;                  /**< bit:      0  MATRIX Dynamic Clock Gating              */
    uint32_t BRIDCKG:1;                 /**< bit:      1  Bridge Dynamic Clock Gating Enable       */
    uint32_t EFCCKG:1;                  /**< bit:      2  EFC Dynamic Clock Gating Enable          */
    uint32_t :29;                       /**< bit:  3..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} CCFG_DYNCKG_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CCFG_DYNCKG_OFFSET                  (0x11C)                                       /**<  (CCFG_DYNCKG) Dynamic Clock Gating Register  Offset */

#define CCFG_DYNCKG_MATCKG_Pos              0                                              /**< (CCFG_DYNCKG) MATRIX Dynamic Clock Gating Position */
#define CCFG_DYNCKG_MATCKG_Msk              (_U_(0x1) << CCFG_DYNCKG_MATCKG_Pos)           /**< (CCFG_DYNCKG) MATRIX Dynamic Clock Gating Mask */
#define CCFG_DYNCKG_MATCKG                  CCFG_DYNCKG_MATCKG_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_DYNCKG_MATCKG_Msk instead */
#define CCFG_DYNCKG_BRIDCKG_Pos             1                                              /**< (CCFG_DYNCKG) Bridge Dynamic Clock Gating Enable Position */
#define CCFG_DYNCKG_BRIDCKG_Msk             (_U_(0x1) << CCFG_DYNCKG_BRIDCKG_Pos)          /**< (CCFG_DYNCKG) Bridge Dynamic Clock Gating Enable Mask */
#define CCFG_DYNCKG_BRIDCKG                 CCFG_DYNCKG_BRIDCKG_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_DYNCKG_BRIDCKG_Msk instead */
#define CCFG_DYNCKG_EFCCKG_Pos              2                                              /**< (CCFG_DYNCKG) EFC Dynamic Clock Gating Enable Position */
#define CCFG_DYNCKG_EFCCKG_Msk              (_U_(0x1) << CCFG_DYNCKG_EFCCKG_Pos)           /**< (CCFG_DYNCKG) EFC Dynamic Clock Gating Enable Mask */
#define CCFG_DYNCKG_EFCCKG                  CCFG_DYNCKG_EFCCKG_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_DYNCKG_EFCCKG_Msk instead */
#define CCFG_DYNCKG_MASK                    _U_(0x07)                                      /**< \deprecated (CCFG_DYNCKG) Register MASK  (Use CCFG_DYNCKG_Msk instead)  */
#define CCFG_DYNCKG_Msk                     _U_(0x07)                                      /**< (CCFG_DYNCKG) Register Mask  */


/* -------- CCFG_SMCNFCS : (MATRIX Offset: 0x124) (R/W 32) SMC NAND Flash Chip Select Configuration Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t SMC_NFCS0:1;               /**< bit:      0  SMC NAND Flash Chip Select 0 Assignment  */
    uint32_t SMC_NFCS1:1;               /**< bit:      1  SMC NAND Flash Chip Select 1 Assignment  */
    uint32_t SMC_NFCS2:1;               /**< bit:      2  SMC NAND Flash Chip Select 2 Assignment  */
    uint32_t SMC_NFCS3:1;               /**< bit:      3  SMC NAND Flash Chip Select 3 Assignment  */
    uint32_t SDRAMEN:1;                 /**< bit:      4  SDRAM Enable                             */
    uint32_t :27;                       /**< bit:  5..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct {
    uint32_t SMC_NFCS:4;                /**< bit:   0..3  SMC NAND Flash Chip Select x Assignment  */
    uint32_t :28;                       /**< bit:  4..31 Reserved */
  } vec;                                /**< Structure used for vec  access  */
  uint32_t reg;                         /**< Type used for register access */
} CCFG_SMCNFCS_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define CCFG_SMCNFCS_OFFSET                 (0x124)                                       /**<  (CCFG_SMCNFCS) SMC NAND Flash Chip Select Configuration Register  Offset */

#define CCFG_SMCNFCS_SMC_NFCS0_Pos          0                                              /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 0 Assignment Position */
#define CCFG_SMCNFCS_SMC_NFCS0_Msk          (_U_(0x1) << CCFG_SMCNFCS_SMC_NFCS0_Pos)       /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 0 Assignment Mask */
#define CCFG_SMCNFCS_SMC_NFCS0              CCFG_SMCNFCS_SMC_NFCS0_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SMCNFCS_SMC_NFCS0_Msk instead */
#define CCFG_SMCNFCS_SMC_NFCS1_Pos          1                                              /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 1 Assignment Position */
#define CCFG_SMCNFCS_SMC_NFCS1_Msk          (_U_(0x1) << CCFG_SMCNFCS_SMC_NFCS1_Pos)       /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 1 Assignment Mask */
#define CCFG_SMCNFCS_SMC_NFCS1              CCFG_SMCNFCS_SMC_NFCS1_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SMCNFCS_SMC_NFCS1_Msk instead */
#define CCFG_SMCNFCS_SMC_NFCS2_Pos          2                                              /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 2 Assignment Position */
#define CCFG_SMCNFCS_SMC_NFCS2_Msk          (_U_(0x1) << CCFG_SMCNFCS_SMC_NFCS2_Pos)       /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 2 Assignment Mask */
#define CCFG_SMCNFCS_SMC_NFCS2              CCFG_SMCNFCS_SMC_NFCS2_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SMCNFCS_SMC_NFCS2_Msk instead */
#define CCFG_SMCNFCS_SMC_NFCS3_Pos          3                                              /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 3 Assignment Position */
#define CCFG_SMCNFCS_SMC_NFCS3_Msk          (_U_(0x1) << CCFG_SMCNFCS_SMC_NFCS3_Pos)       /**< (CCFG_SMCNFCS) SMC NAND Flash Chip Select 3 Assignment Mask */
#define CCFG_SMCNFCS_SMC_NFCS3              CCFG_SMCNFCS_SMC_NFCS3_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SMCNFCS_SMC_NFCS3_Msk instead */
#define CCFG_SMCNFCS_SDRAMEN_Pos            4                                              /**< (CCFG_SMCNFCS) SDRAM Enable Position */
#define CCFG_SMCNFCS_SDRAMEN_Msk            (_U_(0x1) << CCFG_SMCNFCS_SDRAMEN_Pos)         /**< (CCFG_SMCNFCS) SDRAM Enable Mask */
#define CCFG_SMCNFCS_SDRAMEN                CCFG_SMCNFCS_SDRAMEN_Msk                       /**< \deprecated Old style mask definition for 1 bit bitfield. Use CCFG_SMCNFCS_SDRAMEN_Msk instead */
#define CCFG_SMCNFCS_MASK                   _U_(0x1F)                                      /**< \deprecated (CCFG_SMCNFCS) Register MASK  (Use CCFG_SMCNFCS_Msk instead)  */
#define CCFG_SMCNFCS_Msk                    _U_(0x1F)                                      /**< (CCFG_SMCNFCS) Register Mask  */

#define CCFG_SMCNFCS_SMC_NFCS_Pos           0                                              /**< (CCFG_SMCNFCS Position) SMC NAND Flash Chip Select x Assignment */
#define CCFG_SMCNFCS_SMC_NFCS_Msk           (_U_(0xF) << CCFG_SMCNFCS_SMC_NFCS_Pos)        /**< (CCFG_SMCNFCS Mask) SMC_NFCS */
#define CCFG_SMCNFCS_SMC_NFCS(value)        (CCFG_SMCNFCS_SMC_NFCS_Msk & ((value) << CCFG_SMCNFCS_SMC_NFCS_Pos))  

/* -------- MATRIX_WPMR : (MATRIX Offset: 0x1e4) (R/W 32) Write Protection Mode Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t WPEN:1;                    /**< bit:      0  Write Protection Enable                  */
    uint32_t :7;                        /**< bit:   1..7  Reserved */
    uint32_t WPKEY:24;                  /**< bit:  8..31  Write Protection Key                     */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_WPMR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_WPMR_OFFSET                  (0x1E4)                                       /**<  (MATRIX_WPMR) Write Protection Mode Register  Offset */

#define MATRIX_WPMR_WPEN_Pos                0                                              /**< (MATRIX_WPMR) Write Protection Enable Position */
#define MATRIX_WPMR_WPEN_Msk                (_U_(0x1) << MATRIX_WPMR_WPEN_Pos)             /**< (MATRIX_WPMR) Write Protection Enable Mask */
#define MATRIX_WPMR_WPEN                    MATRIX_WPMR_WPEN_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_WPMR_WPEN_Msk instead */
#define MATRIX_WPMR_WPKEY_Pos               8                                              /**< (MATRIX_WPMR) Write Protection Key Position */
#define MATRIX_WPMR_WPKEY_Msk               (_U_(0xFFFFFF) << MATRIX_WPMR_WPKEY_Pos)       /**< (MATRIX_WPMR) Write Protection Key Mask */
#define MATRIX_WPMR_WPKEY(value)            (MATRIX_WPMR_WPKEY_Msk & ((value) << MATRIX_WPMR_WPKEY_Pos))
#define   MATRIX_WPMR_WPKEY_PASSWD_Val      _U_(0x4D4154)                                  /**< (MATRIX_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit.Always reads as 0.  */
#define MATRIX_WPMR_WPKEY_PASSWD            (MATRIX_WPMR_WPKEY_PASSWD_Val << MATRIX_WPMR_WPKEY_Pos)  /**< (MATRIX_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit.Always reads as 0. Position  */
#define MATRIX_WPMR_MASK                    _U_(0xFFFFFF01)                                /**< \deprecated (MATRIX_WPMR) Register MASK  (Use MATRIX_WPMR_Msk instead)  */
#define MATRIX_WPMR_Msk                     _U_(0xFFFFFF01)                                /**< (MATRIX_WPMR) Register Mask  */


/* -------- MATRIX_WPSR : (MATRIX Offset: 0x1e8) (R/ 32) Write Protection Status Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t WPVS:1;                    /**< bit:      0  Write Protection Violation Status        */
    uint32_t :7;                        /**< bit:   1..7  Reserved */
    uint32_t WPVSRC:16;                 /**< bit:  8..23  Write Protection Violation Source        */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} MATRIX_WPSR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define MATRIX_WPSR_OFFSET                  (0x1E8)                                       /**<  (MATRIX_WPSR) Write Protection Status Register  Offset */

#define MATRIX_WPSR_WPVS_Pos                0                                              /**< (MATRIX_WPSR) Write Protection Violation Status Position */
#define MATRIX_WPSR_WPVS_Msk                (_U_(0x1) << MATRIX_WPSR_WPVS_Pos)             /**< (MATRIX_WPSR) Write Protection Violation Status Mask */
#define MATRIX_WPSR_WPVS                    MATRIX_WPSR_WPVS_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use MATRIX_WPSR_WPVS_Msk instead */
#define MATRIX_WPSR_WPVSRC_Pos              8                                              /**< (MATRIX_WPSR) Write Protection Violation Source Position */
#define MATRIX_WPSR_WPVSRC_Msk              (_U_(0xFFFF) << MATRIX_WPSR_WPVSRC_Pos)        /**< (MATRIX_WPSR) Write Protection Violation Source Mask */
#define MATRIX_WPSR_WPVSRC(value)           (MATRIX_WPSR_WPVSRC_Msk & ((value) << MATRIX_WPSR_WPVSRC_Pos))
#define MATRIX_WPSR_MASK                    _U_(0xFFFF01)                                  /**< \deprecated (MATRIX_WPSR) Register MASK  (Use MATRIX_WPSR_Msk instead)  */
#define MATRIX_WPSR_Msk                     _U_(0xFFFF01)                                  /**< (MATRIX_WPSR) Register Mask  */


#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'R'
/** \brief MATRIX_PR hardware registers */
typedef struct {  
  __IO uint32_t MATRIX_PRAS;    /**< (MATRIX_PR Offset: 0x00) Priority Register A for Slave 0 */
  __IO uint32_t MATRIX_PRBS;    /**< (MATRIX_PR Offset: 0x04) Priority Register B for Slave 0 */
} MatrixPr;

#define MATRIXPR_NUMBER 9
/** \brief MATRIX hardware registers */
typedef struct {  
  __IO uint32_t MATRIX_MCFG[13]; /**< (MATRIX Offset: 0x00) Master Configuration Register 0 */
  __I  uint8_t                        Reserved1[12];
  __IO uint32_t MATRIX_SCFG[9]; /**< (MATRIX Offset: 0x40) Slave Configuration Register 0 */
  __I  uint8_t                        Reserved2[28];
       MatrixPr MatrixPr[MATRIXPR_NUMBER]; /**< Offset: 0x80 Priority Register A for Slave 0 */
  __I  uint8_t                        Reserved3[56];
  __IO uint32_t MATRIX_MRCR;    /**< (MATRIX Offset: 0x100) Master Remap Control Register */
  __I  uint8_t                        Reserved4[12];
  __IO uint32_t CCFG_CAN0;      /**< (MATRIX Offset: 0x110) CAN0 Configuration Register */
  __IO uint32_t CCFG_SYSIO;     /**< (MATRIX Offset: 0x114) System I/O and CAN1 Configuration Register */
  __IO uint32_t CCFG_PCCR;      /**< (MATRIX Offset: 0x118) Peripheral Clock Configuration Register */
  __IO uint32_t CCFG_DYNCKG;    /**< (MATRIX Offset: 0x11C) Dynamic Clock Gating Register */
  __I  uint8_t                        Reserved5[4];
  __IO uint32_t CCFG_SMCNFCS;   /**< (MATRIX Offset: 0x124) SMC NAND Flash Chip Select Configuration Register */
  __I  uint8_t                        Reserved6[188];
  __IO uint32_t MATRIX_WPMR;    /**< (MATRIX Offset: 0x1E4) Write Protection Mode Register */
  __I  uint32_t MATRIX_WPSR;    /**< (MATRIX Offset: 0x1E8) Write Protection Status Register */
} Matrix;

#elif COMPONENT_TYPEDEF_STYLE == 'N'
/** \brief MATRIX_PR hardware registers */
typedef struct {  
  __IO MATRIX_PRAS_Type               MATRIX_PRAS;    /**< Offset: 0x00 (R/W  32) Priority Register A for Slave 0 */
  __IO MATRIX_PRBS_Type               MATRIX_PRBS;    /**< Offset: 0x04 (R/W  32) Priority Register B for Slave 0 */
} MatrixPr;

/** \brief MATRIX hardware registers */
typedef struct {  
  __IO MATRIX_MCFG_Type               MATRIX_MCFG[13]; /**< Offset: 0x00 (R/W  32) Master Configuration Register 0 */
  __I  uint8_t                        Reserved1[12];
  __IO MATRIX_SCFG_Type               MATRIX_SCFG[9]; /**< Offset: 0x40 (R/W  32) Slave Configuration Register 0 */
  __I  uint8_t                        Reserved2[28];
       MatrixPr                       MatrixPr[9];    /**< Offset: 0x80 Priority Register A for Slave 0 */
  __I  uint8_t                        Reserved3[56];
  __IO MATRIX_MRCR_Type               MATRIX_MRCR;    /**< Offset: 0x100 (R/W  32) Master Remap Control Register */
  __I  uint8_t                        Reserved4[12];
  __IO CCFG_CAN0_Type                 CCFG_CAN0;      /**< Offset: 0x110 (R/W  32) CAN0 Configuration Register */
  __IO CCFG_SYSIO_Type                CCFG_SYSIO;     /**< Offset: 0x114 (R/W  32) System I/O and CAN1 Configuration Register */
  __IO CCFG_PCCR_Type                 CCFG_PCCR;      /**< Offset: 0x118 (R/W  32) Peripheral Clock Configuration Register */
  __IO CCFG_DYNCKG_Type               CCFG_DYNCKG;    /**< Offset: 0x11C (R/W  32) Dynamic Clock Gating Register */
  __I  uint8_t                        Reserved5[4];
  __IO CCFG_SMCNFCS_Type              CCFG_SMCNFCS;   /**< Offset: 0x124 (R/W  32) SMC NAND Flash Chip Select Configuration Register */
  __I  uint8_t                        Reserved6[188];
  __IO MATRIX_WPMR_Type               MATRIX_WPMR;    /**< Offset: 0x1E4 (R/W  32) Write Protection Mode Register */
  __I  MATRIX_WPSR_Type               MATRIX_WPSR;    /**< Offset: 0x1E8 (R/   32) Write Protection Status Register */
} Matrix;

#else /* COMPONENT_TYPEDEF_STYLE */
#error Unknown component typedef style
#endif /* COMPONENT_TYPEDEF_STYLE */

#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
/** @}  end of AHB Bus Matrix */

#endif /* _SAME70_MATRIX_COMPONENT_H_ */
