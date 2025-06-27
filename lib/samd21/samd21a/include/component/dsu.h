/**
 * \file
 *
 * \brief Component description for DSU
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

#ifndef _SAMD21_DSU_COMPONENT_
#define _SAMD21_DSU_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR DSU */
/* ========================================================================== */
/** \addtogroup SAMD21_DSU Device Service Unit */
/*@{*/

#define DSU_U2209
#define REV_DSU                     0x200

/* -------- DSU_CTRL : (DSU Offset: 0x0000) ( /W  8) Control -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  SWRST:1;          /*!< bit:      0  Software Reset                     */
    uint8_t  :1;               /*!< bit:      1  Reserved                           */
    uint8_t  CRC:1;            /*!< bit:      2  32-bit Cyclic Redundancy Check     */
    uint8_t  MBIST:1;          /*!< bit:      3  Memory Built-In Self-Test          */
    uint8_t  CE:1;             /*!< bit:      4  Chip Erase                         */
    uint8_t  :3;               /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_CTRL_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_CTRL_OFFSET             0x0000       /**< \brief (DSU_CTRL offset) Control */
#define DSU_CTRL_RESETVALUE         _U_(0x00)    /**< \brief (DSU_CTRL reset_value) Control */

#define DSU_CTRL_SWRST_Pos          0            /**< \brief (DSU_CTRL) Software Reset */
#define DSU_CTRL_SWRST              (_U_(0x1) << DSU_CTRL_SWRST_Pos)
#define DSU_CTRL_CRC_Pos            2            /**< \brief (DSU_CTRL) 32-bit Cyclic Redundancy Check */
#define DSU_CTRL_CRC                (_U_(0x1) << DSU_CTRL_CRC_Pos)
#define DSU_CTRL_MBIST_Pos          3            /**< \brief (DSU_CTRL) Memory Built-In Self-Test */
#define DSU_CTRL_MBIST              (_U_(0x1) << DSU_CTRL_MBIST_Pos)
#define DSU_CTRL_CE_Pos             4            /**< \brief (DSU_CTRL) Chip Erase */
#define DSU_CTRL_CE                 (_U_(0x1) << DSU_CTRL_CE_Pos)
#define DSU_CTRL_MASK               _U_(0x1D)    /**< \brief (DSU_CTRL) MASK Register */

/* -------- DSU_STATUSA : (DSU Offset: 0x0001) (R/W  8) Status A -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  DONE:1;           /*!< bit:      0  Done                               */
    uint8_t  CRSTEXT:1;        /*!< bit:      1  CPU Reset Phase Extension          */
    uint8_t  BERR:1;           /*!< bit:      2  Bus Error                          */
    uint8_t  FAIL:1;           /*!< bit:      3  Failure                            */
    uint8_t  PERR:1;           /*!< bit:      4  Protection Error                   */
    uint8_t  :3;               /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_STATUSA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_STATUSA_OFFSET          0x0001       /**< \brief (DSU_STATUSA offset) Status A */
#define DSU_STATUSA_RESETVALUE      _U_(0x00)    /**< \brief (DSU_STATUSA reset_value) Status A */

#define DSU_STATUSA_DONE_Pos        0            /**< \brief (DSU_STATUSA) Done */
#define DSU_STATUSA_DONE            (_U_(0x1) << DSU_STATUSA_DONE_Pos)
#define DSU_STATUSA_CRSTEXT_Pos     1            /**< \brief (DSU_STATUSA) CPU Reset Phase Extension */
#define DSU_STATUSA_CRSTEXT         (_U_(0x1) << DSU_STATUSA_CRSTEXT_Pos)
#define DSU_STATUSA_BERR_Pos        2            /**< \brief (DSU_STATUSA) Bus Error */
#define DSU_STATUSA_BERR            (_U_(0x1) << DSU_STATUSA_BERR_Pos)
#define DSU_STATUSA_FAIL_Pos        3            /**< \brief (DSU_STATUSA) Failure */
#define DSU_STATUSA_FAIL            (_U_(0x1) << DSU_STATUSA_FAIL_Pos)
#define DSU_STATUSA_PERR_Pos        4            /**< \brief (DSU_STATUSA) Protection Error */
#define DSU_STATUSA_PERR            (_U_(0x1) << DSU_STATUSA_PERR_Pos)
#define DSU_STATUSA_MASK            _U_(0x1F)    /**< \brief (DSU_STATUSA) MASK Register */

/* -------- DSU_STATUSB : (DSU Offset: 0x0002) (R/   8) Status B -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint8_t  PROT:1;           /*!< bit:      0  Protected                          */
    uint8_t  DBGPRES:1;        /*!< bit:      1  Debugger Present                   */
    uint8_t  DCCD0:1;          /*!< bit:      2  Debug Communication Channel 0 Dirty */
    uint8_t  DCCD1:1;          /*!< bit:      3  Debug Communication Channel 1 Dirty */
    uint8_t  HPE:1;            /*!< bit:      4  Hot-Plugging Enable                */
    uint8_t  :3;               /*!< bit:  5.. 7  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  struct {
    uint8_t  :2;               /*!< bit:  0.. 1  Reserved                           */
    uint8_t  DCCD:2;           /*!< bit:  2.. 3  Debug Communication Channel x Dirty */
    uint8_t  :4;               /*!< bit:  4.. 7  Reserved                           */
  } vec;                       /*!< Structure used for vec  access                  */
  uint8_t reg;                 /*!< Type      used for register access              */
} DSU_STATUSB_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_STATUSB_OFFSET          0x0002       /**< \brief (DSU_STATUSB offset) Status B */
#define DSU_STATUSB_RESETVALUE      _U_(0x10)    /**< \brief (DSU_STATUSB reset_value) Status B */

#define DSU_STATUSB_PROT_Pos        0            /**< \brief (DSU_STATUSB) Protected */
#define DSU_STATUSB_PROT            (_U_(0x1) << DSU_STATUSB_PROT_Pos)
#define DSU_STATUSB_DBGPRES_Pos     1            /**< \brief (DSU_STATUSB) Debugger Present */
#define DSU_STATUSB_DBGPRES         (_U_(0x1) << DSU_STATUSB_DBGPRES_Pos)
#define DSU_STATUSB_DCCD0_Pos       2            /**< \brief (DSU_STATUSB) Debug Communication Channel 0 Dirty */
#define DSU_STATUSB_DCCD0           (_U_(1) << DSU_STATUSB_DCCD0_Pos)
#define DSU_STATUSB_DCCD1_Pos       3            /**< \brief (DSU_STATUSB) Debug Communication Channel 1 Dirty */
#define DSU_STATUSB_DCCD1           (_U_(1) << DSU_STATUSB_DCCD1_Pos)
#define DSU_STATUSB_DCCD_Pos        2            /**< \brief (DSU_STATUSB) Debug Communication Channel x Dirty */
#define DSU_STATUSB_DCCD_Msk        (_U_(0x3) << DSU_STATUSB_DCCD_Pos)
#define DSU_STATUSB_DCCD(value)     (DSU_STATUSB_DCCD_Msk & ((value) << DSU_STATUSB_DCCD_Pos))
#define DSU_STATUSB_HPE_Pos         4            /**< \brief (DSU_STATUSB) Hot-Plugging Enable */
#define DSU_STATUSB_HPE             (_U_(0x1) << DSU_STATUSB_HPE_Pos)
#define DSU_STATUSB_MASK            _U_(0x1F)    /**< \brief (DSU_STATUSB) MASK Register */

/* -------- DSU_ADDR : (DSU Offset: 0x0004) (R/W 32) Address -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :2;               /*!< bit:  0.. 1  Reserved                           */
    uint32_t ADDR:30;          /*!< bit:  2..31  Address                            */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_ADDR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_ADDR_OFFSET             0x0004       /**< \brief (DSU_ADDR offset) Address */
#define DSU_ADDR_RESETVALUE         _U_(0x00000000) /**< \brief (DSU_ADDR reset_value) Address */

#define DSU_ADDR_ADDR_Pos           2            /**< \brief (DSU_ADDR) Address */
#define DSU_ADDR_ADDR_Msk           (_U_(0x3FFFFFFF) << DSU_ADDR_ADDR_Pos)
#define DSU_ADDR_ADDR(value)        (DSU_ADDR_ADDR_Msk & ((value) << DSU_ADDR_ADDR_Pos))
#define DSU_ADDR_MASK               _U_(0xFFFFFFFC) /**< \brief (DSU_ADDR) MASK Register */

/* -------- DSU_LENGTH : (DSU Offset: 0x0008) (R/W 32) Length -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t :2;               /*!< bit:  0.. 1  Reserved                           */
    uint32_t LENGTH:30;        /*!< bit:  2..31  Length                             */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_LENGTH_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_LENGTH_OFFSET           0x0008       /**< \brief (DSU_LENGTH offset) Length */
#define DSU_LENGTH_RESETVALUE       _U_(0x00000000) /**< \brief (DSU_LENGTH reset_value) Length */

#define DSU_LENGTH_LENGTH_Pos       2            /**< \brief (DSU_LENGTH) Length */
#define DSU_LENGTH_LENGTH_Msk       (_U_(0x3FFFFFFF) << DSU_LENGTH_LENGTH_Pos)
#define DSU_LENGTH_LENGTH(value)    (DSU_LENGTH_LENGTH_Msk & ((value) << DSU_LENGTH_LENGTH_Pos))
#define DSU_LENGTH_MASK             _U_(0xFFFFFFFC) /**< \brief (DSU_LENGTH) MASK Register */

/* -------- DSU_DATA : (DSU Offset: 0x000C) (R/W 32) Data -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t DATA:32;          /*!< bit:  0..31  Data                               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DATA_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_DATA_OFFSET             0x000C       /**< \brief (DSU_DATA offset) Data */
#define DSU_DATA_RESETVALUE         _U_(0x00000000) /**< \brief (DSU_DATA reset_value) Data */

#define DSU_DATA_DATA_Pos           0            /**< \brief (DSU_DATA) Data */
#define DSU_DATA_DATA_Msk           (_U_(0xFFFFFFFF) << DSU_DATA_DATA_Pos)
#define DSU_DATA_DATA(value)        (DSU_DATA_DATA_Msk & ((value) << DSU_DATA_DATA_Pos))
#define DSU_DATA_MASK               _U_(0xFFFFFFFF) /**< \brief (DSU_DATA) MASK Register */

/* -------- DSU_DCC : (DSU Offset: 0x0010) (R/W 32) Debug Communication Channel n -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t DATA:32;          /*!< bit:  0..31  Data                               */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DCC_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_DCC_OFFSET              0x0010       /**< \brief (DSU_DCC offset) Debug Communication Channel n */
#define DSU_DCC_RESETVALUE          _U_(0x00000000) /**< \brief (DSU_DCC reset_value) Debug Communication Channel n */

#define DSU_DCC_DATA_Pos            0            /**< \brief (DSU_DCC) Data */
#define DSU_DCC_DATA_Msk            (_U_(0xFFFFFFFF) << DSU_DCC_DATA_Pos)
#define DSU_DCC_DATA(value)         (DSU_DCC_DATA_Msk & ((value) << DSU_DCC_DATA_Pos))
#define DSU_DCC_MASK                _U_(0xFFFFFFFF) /**< \brief (DSU_DCC) MASK Register */

/* -------- DSU_DID : (DSU Offset: 0x0018) (R/  32) Device Identification -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t DEVSEL:8;         /*!< bit:  0.. 7  Device Select                      */
    uint32_t REVISION:4;       /*!< bit:  8..11  Revision                           */
    uint32_t DIE:4;            /*!< bit: 12..15  Die Identification                 */
    uint32_t SERIES:6;         /*!< bit: 16..21  Product Series                     */
    uint32_t :1;               /*!< bit:     22  Reserved                           */
    uint32_t FAMILY:5;         /*!< bit: 23..27  Product Family                     */
    uint32_t PROCESSOR:4;      /*!< bit: 28..31  Processor                          */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_DID_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_DID_OFFSET              0x0018       /**< \brief (DSU_DID offset) Device Identification */

#define DSU_DID_DEVSEL_Pos          0            /**< \brief (DSU_DID) Device Select */
#define DSU_DID_DEVSEL_Msk          (_U_(0xFF) << DSU_DID_DEVSEL_Pos)
#define DSU_DID_DEVSEL(value)       (DSU_DID_DEVSEL_Msk & ((value) << DSU_DID_DEVSEL_Pos))
#define DSU_DID_REVISION_Pos        8            /**< \brief (DSU_DID) Revision */
#define DSU_DID_REVISION_Msk        (_U_(0xF) << DSU_DID_REVISION_Pos)
#define DSU_DID_REVISION(value)     (DSU_DID_REVISION_Msk & ((value) << DSU_DID_REVISION_Pos))
#define DSU_DID_DIE_Pos             12           /**< \brief (DSU_DID) Die Identification */
#define DSU_DID_DIE_Msk             (_U_(0xF) << DSU_DID_DIE_Pos)
#define DSU_DID_DIE(value)          (DSU_DID_DIE_Msk & ((value) << DSU_DID_DIE_Pos))
#define DSU_DID_SERIES_Pos          16           /**< \brief (DSU_DID) Product Series */
#define DSU_DID_SERIES_Msk          (_U_(0x3F) << DSU_DID_SERIES_Pos)
#define DSU_DID_SERIES(value)       (DSU_DID_SERIES_Msk & ((value) << DSU_DID_SERIES_Pos))
#define DSU_DID_FAMILY_Pos          23           /**< \brief (DSU_DID) Product Family */
#define DSU_DID_FAMILY_Msk          (_U_(0x1F) << DSU_DID_FAMILY_Pos)
#define DSU_DID_FAMILY(value)       (DSU_DID_FAMILY_Msk & ((value) << DSU_DID_FAMILY_Pos))
#define DSU_DID_PROCESSOR_Pos       28           /**< \brief (DSU_DID) Processor */
#define DSU_DID_PROCESSOR_Msk       (_U_(0xF) << DSU_DID_PROCESSOR_Pos)
#define DSU_DID_PROCESSOR(value)    (DSU_DID_PROCESSOR_Msk & ((value) << DSU_DID_PROCESSOR_Pos))
#define DSU_DID_MASK                _U_(0xFFBFFFFF) /**< \brief (DSU_DID) MASK Register */

/* -------- DSU_ENTRY : (DSU Offset: 0x1000) (R/  32) CoreSight ROM Table Entry 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t EPRES:1;          /*!< bit:      0  Entry Present                      */
    uint32_t FMT:1;            /*!< bit:      1  Format                             */
    uint32_t :10;              /*!< bit:  2..11  Reserved                           */
    uint32_t ADDOFF:20;        /*!< bit: 12..31  Address Offset                     */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_ENTRY_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_ENTRY_OFFSET            0x1000       /**< \brief (DSU_ENTRY offset) CoreSight ROM Table Entry 0 */
#define DSU_ENTRY_RESETVALUE        _U_(0x9F9FC002) /**< \brief (DSU_ENTRY reset_value) CoreSight ROM Table Entry 0 */

#define DSU_ENTRY_EPRES_Pos         0            /**< \brief (DSU_ENTRY) Entry Present */
#define DSU_ENTRY_EPRES             (_U_(0x1) << DSU_ENTRY_EPRES_Pos)
#define DSU_ENTRY_FMT_Pos           1            /**< \brief (DSU_ENTRY) Format */
#define DSU_ENTRY_FMT               (_U_(0x1) << DSU_ENTRY_FMT_Pos)
#define DSU_ENTRY_ADDOFF_Pos        12           /**< \brief (DSU_ENTRY) Address Offset */
#define DSU_ENTRY_ADDOFF_Msk        (_U_(0xFFFFF) << DSU_ENTRY_ADDOFF_Pos)
#define DSU_ENTRY_ADDOFF(value)     (DSU_ENTRY_ADDOFF_Msk & ((value) << DSU_ENTRY_ADDOFF_Pos))
#define DSU_ENTRY_MASK              _U_(0xFFFFF003) /**< \brief (DSU_ENTRY) MASK Register */

/* -------- DSU_ENTRY1 : (DSU Offset: 0x1004) (R/  32) CoreSight ROM Table Entry 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_ENTRY1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_ENTRY1_OFFSET           0x1004       /**< \brief (DSU_ENTRY1 offset) CoreSight ROM Table Entry 1 */
#define DSU_ENTRY1_RESETVALUE       _U_(0x00003002) /**< \brief (DSU_ENTRY1 reset_value) CoreSight ROM Table Entry 1 */
#define DSU_ENTRY1_MASK             _U_(0xFFFFFFFF) /**< \brief (DSU_ENTRY1) MASK Register */

/* -------- DSU_END : (DSU Offset: 0x1008) (R/  32) CoreSight ROM Table End -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t END:32;           /*!< bit:  0..31  End Marker                         */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_END_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_END_OFFSET              0x1008       /**< \brief (DSU_END offset) CoreSight ROM Table End */
#define DSU_END_RESETVALUE          _U_(0x00000000) /**< \brief (DSU_END reset_value) CoreSight ROM Table End */

#define DSU_END_END_Pos             0            /**< \brief (DSU_END) End Marker */
#define DSU_END_END_Msk             (_U_(0xFFFFFFFF) << DSU_END_END_Pos)
#define DSU_END_END(value)          (DSU_END_END_Msk & ((value) << DSU_END_END_Pos))
#define DSU_END_MASK                _U_(0xFFFFFFFF) /**< \brief (DSU_END) MASK Register */

/* -------- DSU_MEMTYPE : (DSU Offset: 0x1FCC) (R/  32) CoreSight ROM Table Memory Type -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t SMEMP:1;          /*!< bit:      0  System Memory Present              */
    uint32_t :31;              /*!< bit:  1..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_MEMTYPE_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_MEMTYPE_OFFSET          0x1FCC       /**< \brief (DSU_MEMTYPE offset) CoreSight ROM Table Memory Type */
#define DSU_MEMTYPE_RESETVALUE      _U_(0x00000000) /**< \brief (DSU_MEMTYPE reset_value) CoreSight ROM Table Memory Type */

#define DSU_MEMTYPE_SMEMP_Pos       0            /**< \brief (DSU_MEMTYPE) System Memory Present */
#define DSU_MEMTYPE_SMEMP           (_U_(0x1) << DSU_MEMTYPE_SMEMP_Pos)
#define DSU_MEMTYPE_MASK            _U_(0x00000001) /**< \brief (DSU_MEMTYPE) MASK Register */

/* -------- DSU_PID4 : (DSU Offset: 0x1FD0) (R/  32) Peripheral Identification 4 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t JEPCC:4;          /*!< bit:  0.. 3  JEP-106 Continuation Code          */
    uint32_t FKBC:4;           /*!< bit:  4.. 7  4KB Count                          */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID4_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_PID4_OFFSET             0x1FD0       /**< \brief (DSU_PID4 offset) Peripheral Identification 4 */
#define DSU_PID4_RESETVALUE         _U_(0x00000000) /**< \brief (DSU_PID4 reset_value) Peripheral Identification 4 */

#define DSU_PID4_JEPCC_Pos          0            /**< \brief (DSU_PID4) JEP-106 Continuation Code */
#define DSU_PID4_JEPCC_Msk          (_U_(0xF) << DSU_PID4_JEPCC_Pos)
#define DSU_PID4_JEPCC(value)       (DSU_PID4_JEPCC_Msk & ((value) << DSU_PID4_JEPCC_Pos))
#define DSU_PID4_FKBC_Pos           4            /**< \brief (DSU_PID4) 4KB Count */
#define DSU_PID4_FKBC_Msk           (_U_(0xF) << DSU_PID4_FKBC_Pos)
#define DSU_PID4_FKBC(value)        (DSU_PID4_FKBC_Msk & ((value) << DSU_PID4_FKBC_Pos))
#define DSU_PID4_MASK               _U_(0x000000FF) /**< \brief (DSU_PID4) MASK Register */

/* -------- DSU_PID0 : (DSU Offset: 0x1FE0) (R/  32) Peripheral Identification 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t PARTNBL:8;        /*!< bit:  0.. 7  Part Number Low                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_PID0_OFFSET             0x1FE0       /**< \brief (DSU_PID0 offset) Peripheral Identification 0 */
#define DSU_PID0_RESETVALUE         _U_(0x000000D0) /**< \brief (DSU_PID0 reset_value) Peripheral Identification 0 */

#define DSU_PID0_PARTNBL_Pos        0            /**< \brief (DSU_PID0) Part Number Low */
#define DSU_PID0_PARTNBL_Msk        (_U_(0xFF) << DSU_PID0_PARTNBL_Pos)
#define DSU_PID0_PARTNBL(value)     (DSU_PID0_PARTNBL_Msk & ((value) << DSU_PID0_PARTNBL_Pos))
#define DSU_PID0_MASK               _U_(0x000000FF) /**< \brief (DSU_PID0) MASK Register */

/* -------- DSU_PID1 : (DSU Offset: 0x1FE4) (R/  32) Peripheral Identification 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t PARTNBH:4;        /*!< bit:  0.. 3  Part Number High                   */
    uint32_t JEPIDCL:4;        /*!< bit:  4.. 7  Low part of the JEP-106 Identity Code */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_PID1_OFFSET             0x1FE4       /**< \brief (DSU_PID1 offset) Peripheral Identification 1 */
#define DSU_PID1_RESETVALUE         _U_(0x000000FC) /**< \brief (DSU_PID1 reset_value) Peripheral Identification 1 */

#define DSU_PID1_PARTNBH_Pos        0            /**< \brief (DSU_PID1) Part Number High */
#define DSU_PID1_PARTNBH_Msk        (_U_(0xF) << DSU_PID1_PARTNBH_Pos)
#define DSU_PID1_PARTNBH(value)     (DSU_PID1_PARTNBH_Msk & ((value) << DSU_PID1_PARTNBH_Pos))
#define DSU_PID1_JEPIDCL_Pos        4            /**< \brief (DSU_PID1) Low part of the JEP-106 Identity Code */
#define DSU_PID1_JEPIDCL_Msk        (_U_(0xF) << DSU_PID1_JEPIDCL_Pos)
#define DSU_PID1_JEPIDCL(value)     (DSU_PID1_JEPIDCL_Msk & ((value) << DSU_PID1_JEPIDCL_Pos))
#define DSU_PID1_MASK               _U_(0x000000FF) /**< \brief (DSU_PID1) MASK Register */

/* -------- DSU_PID2 : (DSU Offset: 0x1FE8) (R/  32) Peripheral Identification 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t JEPIDCH:3;        /*!< bit:  0.. 2  JEP-106 Identity Code High         */
    uint32_t JEPU:1;           /*!< bit:      3  JEP-106 Identity Code is used      */
    uint32_t REVISION:4;       /*!< bit:  4.. 7  Revision Number                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_PID2_OFFSET             0x1FE8       /**< \brief (DSU_PID2 offset) Peripheral Identification 2 */
#define DSU_PID2_RESETVALUE         _U_(0x00000009) /**< \brief (DSU_PID2 reset_value) Peripheral Identification 2 */

#define DSU_PID2_JEPIDCH_Pos        0            /**< \brief (DSU_PID2) JEP-106 Identity Code High */
#define DSU_PID2_JEPIDCH_Msk        (_U_(0x7) << DSU_PID2_JEPIDCH_Pos)
#define DSU_PID2_JEPIDCH(value)     (DSU_PID2_JEPIDCH_Msk & ((value) << DSU_PID2_JEPIDCH_Pos))
#define DSU_PID2_JEPU_Pos           3            /**< \brief (DSU_PID2) JEP-106 Identity Code is used */
#define DSU_PID2_JEPU               (_U_(0x1) << DSU_PID2_JEPU_Pos)
#define DSU_PID2_REVISION_Pos       4            /**< \brief (DSU_PID2) Revision Number */
#define DSU_PID2_REVISION_Msk       (_U_(0xF) << DSU_PID2_REVISION_Pos)
#define DSU_PID2_REVISION(value)    (DSU_PID2_REVISION_Msk & ((value) << DSU_PID2_REVISION_Pos))
#define DSU_PID2_MASK               _U_(0x000000FF) /**< \brief (DSU_PID2) MASK Register */

/* -------- DSU_PID3 : (DSU Offset: 0x1FEC) (R/  32) Peripheral Identification 3 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t CUSMOD:4;         /*!< bit:  0.. 3  ARM CUSMOD                         */
    uint32_t REVAND:4;         /*!< bit:  4.. 7  Revision Number                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_PID3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_PID3_OFFSET             0x1FEC       /**< \brief (DSU_PID3 offset) Peripheral Identification 3 */
#define DSU_PID3_RESETVALUE         _U_(0x00000000) /**< \brief (DSU_PID3 reset_value) Peripheral Identification 3 */

#define DSU_PID3_CUSMOD_Pos         0            /**< \brief (DSU_PID3) ARM CUSMOD */
#define DSU_PID3_CUSMOD_Msk         (_U_(0xF) << DSU_PID3_CUSMOD_Pos)
#define DSU_PID3_CUSMOD(value)      (DSU_PID3_CUSMOD_Msk & ((value) << DSU_PID3_CUSMOD_Pos))
#define DSU_PID3_REVAND_Pos         4            /**< \brief (DSU_PID3) Revision Number */
#define DSU_PID3_REVAND_Msk         (_U_(0xF) << DSU_PID3_REVAND_Pos)
#define DSU_PID3_REVAND(value)      (DSU_PID3_REVAND_Msk & ((value) << DSU_PID3_REVAND_Pos))
#define DSU_PID3_MASK               _U_(0x000000FF) /**< \brief (DSU_PID3) MASK Register */

/* -------- DSU_CID0 : (DSU Offset: 0x1FF0) (R/  32) Component Identification 0 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t PREAMBLEB0:8;     /*!< bit:  0.. 7  Preamble Byte 0                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID0_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_CID0_OFFSET             0x1FF0       /**< \brief (DSU_CID0 offset) Component Identification 0 */
#define DSU_CID0_RESETVALUE         _U_(0x0000000D) /**< \brief (DSU_CID0 reset_value) Component Identification 0 */

#define DSU_CID0_PREAMBLEB0_Pos     0            /**< \brief (DSU_CID0) Preamble Byte 0 */
#define DSU_CID0_PREAMBLEB0_Msk     (_U_(0xFF) << DSU_CID0_PREAMBLEB0_Pos)
#define DSU_CID0_PREAMBLEB0(value)  (DSU_CID0_PREAMBLEB0_Msk & ((value) << DSU_CID0_PREAMBLEB0_Pos))
#define DSU_CID0_MASK               _U_(0x000000FF) /**< \brief (DSU_CID0) MASK Register */

/* -------- DSU_CID1 : (DSU Offset: 0x1FF4) (R/  32) Component Identification 1 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t PREAMBLE:4;       /*!< bit:  0.. 3  Preamble                           */
    uint32_t CCLASS:4;         /*!< bit:  4.. 7  Component Class                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID1_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_CID1_OFFSET             0x1FF4       /**< \brief (DSU_CID1 offset) Component Identification 1 */
#define DSU_CID1_RESETVALUE         _U_(0x00000010) /**< \brief (DSU_CID1 reset_value) Component Identification 1 */

#define DSU_CID1_PREAMBLE_Pos       0            /**< \brief (DSU_CID1) Preamble */
#define DSU_CID1_PREAMBLE_Msk       (_U_(0xF) << DSU_CID1_PREAMBLE_Pos)
#define DSU_CID1_PREAMBLE(value)    (DSU_CID1_PREAMBLE_Msk & ((value) << DSU_CID1_PREAMBLE_Pos))
#define DSU_CID1_CCLASS_Pos         4            /**< \brief (DSU_CID1) Component Class */
#define DSU_CID1_CCLASS_Msk         (_U_(0xF) << DSU_CID1_CCLASS_Pos)
#define DSU_CID1_CCLASS(value)      (DSU_CID1_CCLASS_Msk & ((value) << DSU_CID1_CCLASS_Pos))
#define DSU_CID1_MASK               _U_(0x000000FF) /**< \brief (DSU_CID1) MASK Register */

/* -------- DSU_CID2 : (DSU Offset: 0x1FF8) (R/  32) Component Identification 2 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t PREAMBLEB2:8;     /*!< bit:  0.. 7  Preamble Byte 2                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID2_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_CID2_OFFSET             0x1FF8       /**< \brief (DSU_CID2 offset) Component Identification 2 */
#define DSU_CID2_RESETVALUE         _U_(0x00000005) /**< \brief (DSU_CID2 reset_value) Component Identification 2 */

#define DSU_CID2_PREAMBLEB2_Pos     0            /**< \brief (DSU_CID2) Preamble Byte 2 */
#define DSU_CID2_PREAMBLEB2_Msk     (_U_(0xFF) << DSU_CID2_PREAMBLEB2_Pos)
#define DSU_CID2_PREAMBLEB2(value)  (DSU_CID2_PREAMBLEB2_Msk & ((value) << DSU_CID2_PREAMBLEB2_Pos))
#define DSU_CID2_MASK               _U_(0x000000FF) /**< \brief (DSU_CID2) MASK Register */

/* -------- DSU_CID3 : (DSU Offset: 0x1FFC) (R/  32) Component Identification 3 -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
  struct {
    uint32_t PREAMBLEB3:8;     /*!< bit:  0.. 7  Preamble Byte 3                    */
    uint32_t :24;              /*!< bit:  8..31  Reserved                           */
  } bit;                       /*!< Structure used for bit  access                  */
  uint32_t reg;                /*!< Type      used for register access              */
} DSU_CID3_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define DSU_CID3_OFFSET             0x1FFC       /**< \brief (DSU_CID3 offset) Component Identification 3 */
#define DSU_CID3_RESETVALUE         _U_(0x000000B1) /**< \brief (DSU_CID3 reset_value) Component Identification 3 */

#define DSU_CID3_PREAMBLEB3_Pos     0            /**< \brief (DSU_CID3) Preamble Byte 3 */
#define DSU_CID3_PREAMBLEB3_Msk     (_U_(0xFF) << DSU_CID3_PREAMBLEB3_Pos)
#define DSU_CID3_PREAMBLEB3(value)  (DSU_CID3_PREAMBLEB3_Msk & ((value) << DSU_CID3_PREAMBLEB3_Pos))
#define DSU_CID3_MASK               _U_(0x000000FF) /**< \brief (DSU_CID3) MASK Register */

/** \brief DSU hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
  __O  DSU_CTRL_Type             CTRL;        /**< \brief Offset: 0x0000 ( /W  8) Control */
  __IO DSU_STATUSA_Type          STATUSA;     /**< \brief Offset: 0x0001 (R/W  8) Status A */
  __I  DSU_STATUSB_Type          STATUSB;     /**< \brief Offset: 0x0002 (R/   8) Status B */
       RoReg8                    Reserved1[0x1];
  __IO DSU_ADDR_Type             ADDR;        /**< \brief Offset: 0x0004 (R/W 32) Address */
  __IO DSU_LENGTH_Type           LENGTH;      /**< \brief Offset: 0x0008 (R/W 32) Length */
  __IO DSU_DATA_Type             DATA;        /**< \brief Offset: 0x000C (R/W 32) Data */
  __IO DSU_DCC_Type              DCC[2];      /**< \brief Offset: 0x0010 (R/W 32) Debug Communication Channel n */
  __I  DSU_DID_Type              DID;         /**< \brief Offset: 0x0018 (R/  32) Device Identification */
       RoReg8                    Reserved2[0xFE4];
  __I  DSU_ENTRY_Type            ENTRY;       /**< \brief Offset: 0x1000 (R/  32) CoreSight ROM Table Entry 0 */
  __I  DSU_ENTRY1_Type           ENTRY1;      /**< \brief Offset: 0x1004 (R/  32) CoreSight ROM Table Entry 1 */
  __I  DSU_END_Type              END;         /**< \brief Offset: 0x1008 (R/  32) CoreSight ROM Table End */
       RoReg8                    Reserved3[0xFC0];
  __I  DSU_MEMTYPE_Type          MEMTYPE;     /**< \brief Offset: 0x1FCC (R/  32) CoreSight ROM Table Memory Type */
  __I  DSU_PID4_Type             PID4;        /**< \brief Offset: 0x1FD0 (R/  32) Peripheral Identification 4 */
       RoReg8                    Reserved4[0xC];
  __I  DSU_PID0_Type             PID0;        /**< \brief Offset: 0x1FE0 (R/  32) Peripheral Identification 0 */
  __I  DSU_PID1_Type             PID1;        /**< \brief Offset: 0x1FE4 (R/  32) Peripheral Identification 1 */
  __I  DSU_PID2_Type             PID2;        /**< \brief Offset: 0x1FE8 (R/  32) Peripheral Identification 2 */
  __I  DSU_PID3_Type             PID3;        /**< \brief Offset: 0x1FEC (R/  32) Peripheral Identification 3 */
  __I  DSU_CID0_Type             CID0;        /**< \brief Offset: 0x1FF0 (R/  32) Component Identification 0 */
  __I  DSU_CID1_Type             CID1;        /**< \brief Offset: 0x1FF4 (R/  32) Component Identification 1 */
  __I  DSU_CID2_Type             CID2;        /**< \brief Offset: 0x1FF8 (R/  32) Component Identification 2 */
  __I  DSU_CID3_Type             CID3;        /**< \brief Offset: 0x1FFC (R/  32) Component Identification 3 */
} Dsu;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_DSU_COMPONENT_ */
