/**
 * \file
 *
 * \brief Component description for UTMI
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
#ifndef _SAME70_UTMI_COMPONENT_H_
#define _SAME70_UTMI_COMPONENT_H_
#define _SAME70_UTMI_COMPONENT_         /**< \deprecated  Backward compatibility for ASF */

/** \addtogroup SAME_SAME70 USB Transmitter Interface Macrocell
 *  @{
 */
/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR UTMI */
/* ========================================================================== */
#ifndef COMPONENT_TYPEDEF_STYLE
  #define COMPONENT_TYPEDEF_STYLE 'R'  /**< Defines default style of typedefs for the component header files ('R' = RFO, 'N' = NTO)*/
#endif

#define UTMI_11300                      /**< (UTMI) Module ID */
#define REV_UTMI A                      /**< (UTMI) Module revision */

/* -------- UTMI_OHCIICR : (UTMI Offset: 0x10) (R/W 32) OHCI Interrupt Configuration Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RES0:1;                    /**< bit:      0  USB PORTx Reset                          */
    uint32_t :3;                        /**< bit:   1..3  Reserved */
    uint32_t ARIE:1;                    /**< bit:      4  OHCI Asynchronous Resume Interrupt Enable */
    uint32_t APPSTART:1;                /**< bit:      5  Reserved                                 */
    uint32_t :17;                       /**< bit:  6..22  Reserved */
    uint32_t UDPPUDIS:1;                /**< bit:     23  USB Device Pull-up Disable               */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct {
    uint32_t RES:1;                     /**< bit:      0  USB PORTx Reset                          */
    uint32_t :31;                       /**< bit:  1..31 Reserved */
  } vec;                                /**< Structure used for vec  access  */
  uint32_t reg;                         /**< Type used for register access */
} UTMI_OHCIICR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UTMI_OHCIICR_OFFSET                 (0x10)                                        /**<  (UTMI_OHCIICR) OHCI Interrupt Configuration Register  Offset */

#define UTMI_OHCIICR_RES0_Pos               0                                              /**< (UTMI_OHCIICR) USB PORTx Reset Position */
#define UTMI_OHCIICR_RES0_Msk               (_U_(0x1) << UTMI_OHCIICR_RES0_Pos)            /**< (UTMI_OHCIICR) USB PORTx Reset Mask */
#define UTMI_OHCIICR_RES0                   UTMI_OHCIICR_RES0_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use UTMI_OHCIICR_RES0_Msk instead */
#define UTMI_OHCIICR_ARIE_Pos               4                                              /**< (UTMI_OHCIICR) OHCI Asynchronous Resume Interrupt Enable Position */
#define UTMI_OHCIICR_ARIE_Msk               (_U_(0x1) << UTMI_OHCIICR_ARIE_Pos)            /**< (UTMI_OHCIICR) OHCI Asynchronous Resume Interrupt Enable Mask */
#define UTMI_OHCIICR_ARIE                   UTMI_OHCIICR_ARIE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use UTMI_OHCIICR_ARIE_Msk instead */
#define UTMI_OHCIICR_APPSTART_Pos           5                                              /**< (UTMI_OHCIICR) Reserved Position */
#define UTMI_OHCIICR_APPSTART_Msk           (_U_(0x1) << UTMI_OHCIICR_APPSTART_Pos)        /**< (UTMI_OHCIICR) Reserved Mask */
#define UTMI_OHCIICR_APPSTART               UTMI_OHCIICR_APPSTART_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UTMI_OHCIICR_APPSTART_Msk instead */
#define UTMI_OHCIICR_UDPPUDIS_Pos           23                                             /**< (UTMI_OHCIICR) USB Device Pull-up Disable Position */
#define UTMI_OHCIICR_UDPPUDIS_Msk           (_U_(0x1) << UTMI_OHCIICR_UDPPUDIS_Pos)        /**< (UTMI_OHCIICR) USB Device Pull-up Disable Mask */
#define UTMI_OHCIICR_UDPPUDIS               UTMI_OHCIICR_UDPPUDIS_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use UTMI_OHCIICR_UDPPUDIS_Msk instead */
#define UTMI_OHCIICR_MASK                   _U_(0x800031)                                  /**< \deprecated (UTMI_OHCIICR) Register MASK  (Use UTMI_OHCIICR_Msk instead)  */
#define UTMI_OHCIICR_Msk                    _U_(0x800031)                                  /**< (UTMI_OHCIICR) Register Mask  */

#define UTMI_OHCIICR_RES_Pos                0                                              /**< (UTMI_OHCIICR Position) USB PORTx Reset */
#define UTMI_OHCIICR_RES_Msk                (_U_(0x1) << UTMI_OHCIICR_RES_Pos)             /**< (UTMI_OHCIICR Mask) RES */
#define UTMI_OHCIICR_RES(value)             (UTMI_OHCIICR_RES_Msk & ((value) << UTMI_OHCIICR_RES_Pos))  

/* -------- UTMI_CKTRIM : (UTMI Offset: 0x30) (R/W 32) UTMI Clock Trimming Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t FREQ:2;                    /**< bit:   0..1  UTMI Reference Clock Frequency           */
    uint32_t :30;                       /**< bit:  2..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} UTMI_CKTRIM_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define UTMI_CKTRIM_OFFSET                  (0x30)                                        /**<  (UTMI_CKTRIM) UTMI Clock Trimming Register  Offset */

#define UTMI_CKTRIM_FREQ_Pos                0                                              /**< (UTMI_CKTRIM) UTMI Reference Clock Frequency Position */
#define UTMI_CKTRIM_FREQ_Msk                (_U_(0x3) << UTMI_CKTRIM_FREQ_Pos)             /**< (UTMI_CKTRIM) UTMI Reference Clock Frequency Mask */
#define UTMI_CKTRIM_FREQ(value)             (UTMI_CKTRIM_FREQ_Msk & ((value) << UTMI_CKTRIM_FREQ_Pos))
#define   UTMI_CKTRIM_FREQ_XTAL12_Val       _U_(0x0)                                       /**< (UTMI_CKTRIM) 12 MHz reference clock  */
#define   UTMI_CKTRIM_FREQ_XTAL16_Val       _U_(0x1)                                       /**< (UTMI_CKTRIM) 16 MHz reference clock  */
#define UTMI_CKTRIM_FREQ_XTAL12             (UTMI_CKTRIM_FREQ_XTAL12_Val << UTMI_CKTRIM_FREQ_Pos)  /**< (UTMI_CKTRIM) 12 MHz reference clock Position  */
#define UTMI_CKTRIM_FREQ_XTAL16             (UTMI_CKTRIM_FREQ_XTAL16_Val << UTMI_CKTRIM_FREQ_Pos)  /**< (UTMI_CKTRIM) 16 MHz reference clock Position  */
#define UTMI_CKTRIM_MASK                    _U_(0x03)                                      /**< \deprecated (UTMI_CKTRIM) Register MASK  (Use UTMI_CKTRIM_Msk instead)  */
#define UTMI_CKTRIM_Msk                     _U_(0x03)                                      /**< (UTMI_CKTRIM) Register Mask  */


#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'R'
/** \brief UTMI hardware registers */
typedef struct {  
  __I  uint8_t                        Reserved1[16];
  __IO uint32_t UTMI_OHCIICR;   /**< (UTMI Offset: 0x10) OHCI Interrupt Configuration Register */
  __I  uint8_t                        Reserved2[28];
  __IO uint32_t UTMI_CKTRIM;    /**< (UTMI Offset: 0x30) UTMI Clock Trimming Register */
} Utmi;

#elif COMPONENT_TYPEDEF_STYLE == 'N'
/** \brief UTMI hardware registers */
typedef struct {  
  __I  uint8_t                        Reserved1[16];
  __IO UTMI_OHCIICR_Type              UTMI_OHCIICR;   /**< Offset: 0x10 (R/W  32) OHCI Interrupt Configuration Register */
  __I  uint8_t                        Reserved2[28];
  __IO UTMI_CKTRIM_Type               UTMI_CKTRIM;    /**< Offset: 0x30 (R/W  32) UTMI Clock Trimming Register */
} Utmi;

#else /* COMPONENT_TYPEDEF_STYLE */
#error Unknown component typedef style
#endif /* COMPONENT_TYPEDEF_STYLE */

#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
/** @}  end of USB Transmitter Interface Macrocell */

#endif /* _SAME70_UTMI_COMPONENT_H_ */
