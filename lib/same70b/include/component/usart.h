/**
 * \file
 *
 * \brief Component description for USART
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
#ifndef _SAME70_USART_COMPONENT_H_
#define _SAME70_USART_COMPONENT_H_
#define _SAME70_USART_COMPONENT_         /**< \deprecated  Backward compatibility for ASF */

/** \addtogroup SAME_SAME70 Universal Synchronous Asynchronous Receiver Transmitter
 *  @{
 */
/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR USART */
/* ========================================================================== */
#ifndef COMPONENT_TYPEDEF_STYLE
  #define COMPONENT_TYPEDEF_STYLE 'R'  /**< Defines default style of typedefs for the component header files ('R' = RFO, 'N' = NTO)*/
#endif

#define USART_6089                       /**< (USART) Module ID */
#define REV_USART ZW                     /**< (USART) Module revision */

/* -------- US_CR : (USART Offset: 0x00) (/W 32) Control Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t :2;                        /**< bit:   0..1  Reserved */
    uint32_t RSTRX:1;                   /**< bit:      2  Reset Receiver                           */
    uint32_t RSTTX:1;                   /**< bit:      3  Reset Transmitter                        */
    uint32_t RXEN:1;                    /**< bit:      4  Receiver Enable                          */
    uint32_t RXDIS:1;                   /**< bit:      5  Receiver Disable                         */
    uint32_t TXEN:1;                    /**< bit:      6  Transmitter Enable                       */
    uint32_t TXDIS:1;                   /**< bit:      7  Transmitter Disable                      */
    uint32_t RSTSTA:1;                  /**< bit:      8  Reset Status Bits                        */
    uint32_t :23;                       /**< bit:  9..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct { // USART mode
    uint32_t :9;                        /**< bit:   0..8  Reserved */
    uint32_t STTBRK:1;                  /**< bit:      9  Start Break                              */
    uint32_t STPBRK:1;                  /**< bit:     10  Stop Break                               */
    uint32_t STTTO:1;                   /**< bit:     11  Clear TIMEOUT Flag and Start Timeout After Next Character Received */
    uint32_t SENDA:1;                   /**< bit:     12  Send Address                             */
    uint32_t RSTIT:1;                   /**< bit:     13  Reset Iterations                         */
    uint32_t RSTNACK:1;                 /**< bit:     14  Reset Non Acknowledge                    */
    uint32_t RETTO:1;                   /**< bit:     15  Start Timeout Immediately                */
    uint32_t DTREN:1;                   /**< bit:     16  Data Terminal Ready Enable               */
    uint32_t DTRDIS:1;                  /**< bit:     17  Data Terminal Ready Disable              */
    uint32_t RTSEN:1;                   /**< bit:     18  Request to Send Enable                   */
    uint32_t RTSDIS:1;                  /**< bit:     19  Request to Send Disable                  */
    uint32_t :12;                       /**< bit: 20..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // SPI mode
    uint32_t :18;                       /**< bit:  0..17  Reserved */
    uint32_t FCS:1;                     /**< bit:     18  Force SPI Chip Select                    */
    uint32_t RCS:1;                     /**< bit:     19  Release SPI Chip Select                  */
    uint32_t :12;                       /**< bit: 20..31  Reserved */
  } SPI;                                /**< Structure used for SPI mode access */
  struct { // LIN mode
    uint32_t :20;                       /**< bit:  0..19  Reserved */
    uint32_t LINABT:1;                  /**< bit:     20  Abort LIN Transmission                   */
    uint32_t LINWKUP:1;                 /**< bit:     21  Send LIN Wakeup Signal                   */
    uint32_t :10;                       /**< bit: 22..31  Reserved */
  } LIN;                                /**< Structure used for LIN mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_CR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_CR_OFFSET                        (0x00)                                        /**<  (US_CR) Control Register  Offset */

#define US_CR_RSTRX_Pos                     2                                              /**< (US_CR) Reset Receiver Position */
#define US_CR_RSTRX_Msk                     (_U_(0x1) << US_CR_RSTRX_Pos)                  /**< (US_CR) Reset Receiver Mask */
#define US_CR_RSTRX                         US_CR_RSTRX_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_RSTRX_Msk instead */
#define US_CR_RSTTX_Pos                     3                                              /**< (US_CR) Reset Transmitter Position */
#define US_CR_RSTTX_Msk                     (_U_(0x1) << US_CR_RSTTX_Pos)                  /**< (US_CR) Reset Transmitter Mask */
#define US_CR_RSTTX                         US_CR_RSTTX_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_RSTTX_Msk instead */
#define US_CR_RXEN_Pos                      4                                              /**< (US_CR) Receiver Enable Position */
#define US_CR_RXEN_Msk                      (_U_(0x1) << US_CR_RXEN_Pos)                   /**< (US_CR) Receiver Enable Mask */
#define US_CR_RXEN                          US_CR_RXEN_Msk                                 /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_RXEN_Msk instead */
#define US_CR_RXDIS_Pos                     5                                              /**< (US_CR) Receiver Disable Position */
#define US_CR_RXDIS_Msk                     (_U_(0x1) << US_CR_RXDIS_Pos)                  /**< (US_CR) Receiver Disable Mask */
#define US_CR_RXDIS                         US_CR_RXDIS_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_RXDIS_Msk instead */
#define US_CR_TXEN_Pos                      6                                              /**< (US_CR) Transmitter Enable Position */
#define US_CR_TXEN_Msk                      (_U_(0x1) << US_CR_TXEN_Pos)                   /**< (US_CR) Transmitter Enable Mask */
#define US_CR_TXEN                          US_CR_TXEN_Msk                                 /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_TXEN_Msk instead */
#define US_CR_TXDIS_Pos                     7                                              /**< (US_CR) Transmitter Disable Position */
#define US_CR_TXDIS_Msk                     (_U_(0x1) << US_CR_TXDIS_Pos)                  /**< (US_CR) Transmitter Disable Mask */
#define US_CR_TXDIS                         US_CR_TXDIS_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_TXDIS_Msk instead */
#define US_CR_RSTSTA_Pos                    8                                              /**< (US_CR) Reset Status Bits Position */
#define US_CR_RSTSTA_Msk                    (_U_(0x1) << US_CR_RSTSTA_Pos)                 /**< (US_CR) Reset Status Bits Mask */
#define US_CR_RSTSTA                        US_CR_RSTSTA_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_RSTSTA_Msk instead */
#define US_CR_MASK                          _U_(0x1FC)                                     /**< \deprecated (US_CR) Register MASK  (Use US_CR_Msk instead)  */
#define US_CR_Msk                           _U_(0x1FC)                                     /**< (US_CR) Register Mask  */

/* USART mode */
#define US_CR_USART_STTBRK_Pos              9                                              /**< (US_CR) Start Break Position */
#define US_CR_USART_STTBRK_Msk              (_U_(0x1) << US_CR_USART_STTBRK_Pos)           /**< (US_CR) Start Break Mask */
#define US_CR_USART_STTBRK                  US_CR_USART_STTBRK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_STTBRK_Msk instead */
#define US_CR_USART_STPBRK_Pos              10                                             /**< (US_CR) Stop Break Position */
#define US_CR_USART_STPBRK_Msk              (_U_(0x1) << US_CR_USART_STPBRK_Pos)           /**< (US_CR) Stop Break Mask */
#define US_CR_USART_STPBRK                  US_CR_USART_STPBRK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_STPBRK_Msk instead */
#define US_CR_USART_STTTO_Pos               11                                             /**< (US_CR) Clear TIMEOUT Flag and Start Timeout After Next Character Received Position */
#define US_CR_USART_STTTO_Msk               (_U_(0x1) << US_CR_USART_STTTO_Pos)            /**< (US_CR) Clear TIMEOUT Flag and Start Timeout After Next Character Received Mask */
#define US_CR_USART_STTTO                   US_CR_USART_STTTO_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_STTTO_Msk instead */
#define US_CR_USART_SENDA_Pos               12                                             /**< (US_CR) Send Address Position */
#define US_CR_USART_SENDA_Msk               (_U_(0x1) << US_CR_USART_SENDA_Pos)            /**< (US_CR) Send Address Mask */
#define US_CR_USART_SENDA                   US_CR_USART_SENDA_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_SENDA_Msk instead */
#define US_CR_USART_RSTIT_Pos               13                                             /**< (US_CR) Reset Iterations Position */
#define US_CR_USART_RSTIT_Msk               (_U_(0x1) << US_CR_USART_RSTIT_Pos)            /**< (US_CR) Reset Iterations Mask */
#define US_CR_USART_RSTIT                   US_CR_USART_RSTIT_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_RSTIT_Msk instead */
#define US_CR_USART_RSTNACK_Pos             14                                             /**< (US_CR) Reset Non Acknowledge Position */
#define US_CR_USART_RSTNACK_Msk             (_U_(0x1) << US_CR_USART_RSTNACK_Pos)          /**< (US_CR) Reset Non Acknowledge Mask */
#define US_CR_USART_RSTNACK                 US_CR_USART_RSTNACK_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_RSTNACK_Msk instead */
#define US_CR_USART_RETTO_Pos               15                                             /**< (US_CR) Start Timeout Immediately Position */
#define US_CR_USART_RETTO_Msk               (_U_(0x1) << US_CR_USART_RETTO_Pos)            /**< (US_CR) Start Timeout Immediately Mask */
#define US_CR_USART_RETTO                   US_CR_USART_RETTO_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_RETTO_Msk instead */
#define US_CR_USART_DTREN_Pos               16                                             /**< (US_CR) Data Terminal Ready Enable Position */
#define US_CR_USART_DTREN_Msk               (_U_(0x1) << US_CR_USART_DTREN_Pos)            /**< (US_CR) Data Terminal Ready Enable Mask */
#define US_CR_USART_DTREN                   US_CR_USART_DTREN_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_DTREN_Msk instead */
#define US_CR_USART_DTRDIS_Pos              17                                             /**< (US_CR) Data Terminal Ready Disable Position */
#define US_CR_USART_DTRDIS_Msk              (_U_(0x1) << US_CR_USART_DTRDIS_Pos)           /**< (US_CR) Data Terminal Ready Disable Mask */
#define US_CR_USART_DTRDIS                  US_CR_USART_DTRDIS_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_DTRDIS_Msk instead */
#define US_CR_USART_RTSEN_Pos               18                                             /**< (US_CR) Request to Send Enable Position */
#define US_CR_USART_RTSEN_Msk               (_U_(0x1) << US_CR_USART_RTSEN_Pos)            /**< (US_CR) Request to Send Enable Mask */
#define US_CR_USART_RTSEN                   US_CR_USART_RTSEN_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_RTSEN_Msk instead */
#define US_CR_USART_RTSDIS_Pos              19                                             /**< (US_CR) Request to Send Disable Position */
#define US_CR_USART_RTSDIS_Msk              (_U_(0x1) << US_CR_USART_RTSDIS_Pos)           /**< (US_CR) Request to Send Disable Mask */
#define US_CR_USART_RTSDIS                  US_CR_USART_RTSDIS_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_USART_RTSDIS_Msk instead */
#define US_CR_USART_MASK                    _U_(0xFFE00)                                   /**< \deprecated (US_CR_USART) Register MASK  (Use US_CR_USART_Msk instead)  */
#define US_CR_USART_Msk                     _U_(0xFFE00)                                   /**< (US_CR_USART) Register Mask  */

/* SPI mode */
#define US_CR_SPI_FCS_Pos                   18                                             /**< (US_CR) Force SPI Chip Select Position */
#define US_CR_SPI_FCS_Msk                   (_U_(0x1) << US_CR_SPI_FCS_Pos)                /**< (US_CR) Force SPI Chip Select Mask */
#define US_CR_SPI_FCS                       US_CR_SPI_FCS_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_SPI_FCS_Msk instead */
#define US_CR_SPI_RCS_Pos                   19                                             /**< (US_CR) Release SPI Chip Select Position */
#define US_CR_SPI_RCS_Msk                   (_U_(0x1) << US_CR_SPI_RCS_Pos)                /**< (US_CR) Release SPI Chip Select Mask */
#define US_CR_SPI_RCS                       US_CR_SPI_RCS_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_SPI_RCS_Msk instead */
#define US_CR_SPI_MASK                      _U_(0xC0000)                                   /**< \deprecated (US_CR_SPI) Register MASK  (Use US_CR_SPI_Msk instead)  */
#define US_CR_SPI_Msk                       _U_(0xC0000)                                   /**< (US_CR_SPI) Register Mask  */

/* LIN mode */
#define US_CR_LIN_LINABT_Pos                20                                             /**< (US_CR) Abort LIN Transmission Position */
#define US_CR_LIN_LINABT_Msk                (_U_(0x1) << US_CR_LIN_LINABT_Pos)             /**< (US_CR) Abort LIN Transmission Mask */
#define US_CR_LIN_LINABT                    US_CR_LIN_LINABT_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_LIN_LINABT_Msk instead */
#define US_CR_LIN_LINWKUP_Pos               21                                             /**< (US_CR) Send LIN Wakeup Signal Position */
#define US_CR_LIN_LINWKUP_Msk               (_U_(0x1) << US_CR_LIN_LINWKUP_Pos)            /**< (US_CR) Send LIN Wakeup Signal Mask */
#define US_CR_LIN_LINWKUP                   US_CR_LIN_LINWKUP_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CR_LIN_LINWKUP_Msk instead */
#define US_CR_LIN_MASK                      _U_(0x300000)                                  /**< \deprecated (US_CR_LIN) Register MASK  (Use US_CR_LIN_Msk instead)  */
#define US_CR_LIN_Msk                       _U_(0x300000)                                  /**< (US_CR_LIN) Register Mask  */


/* -------- US_MR : (USART Offset: 0x04) (R/W 32) Mode Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t USART_MODE:4;              /**< bit:   0..3  USART Mode of Operation                  */
    uint32_t USCLKS:2;                  /**< bit:   4..5  Clock Selection                          */
    uint32_t CHRL:2;                    /**< bit:   6..7  Character Length                         */
    uint32_t :10;                       /**< bit:  8..17  Reserved */
    uint32_t CLKO:1;                    /**< bit:     18  Clock Output Select                      */
    uint32_t :13;                       /**< bit: 19..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct { // USART mode
    uint32_t :8;                        /**< bit:   0..7  Reserved */
    uint32_t SYNC:1;                    /**< bit:      8  Synchronous Mode Select                  */
    uint32_t PAR:3;                     /**< bit:  9..11  Parity Type                              */
    uint32_t NBSTOP:2;                  /**< bit: 12..13  Number of Stop Bits                      */
    uint32_t CHMODE:2;                  /**< bit: 14..15  Channel Mode                             */
    uint32_t MSBF:1;                    /**< bit:     16  Bit Order                                */
    uint32_t MODE9:1;                   /**< bit:     17  9-bit Character Length                   */
    uint32_t :1;                        /**< bit:     18  Reserved */
    uint32_t OVER:1;                    /**< bit:     19  Oversampling Mode                        */
    uint32_t INACK:1;                   /**< bit:     20  Inhibit Non Acknowledge                  */
    uint32_t DSNACK:1;                  /**< bit:     21  Disable Successive NACK                  */
    uint32_t VAR_SYNC:1;                /**< bit:     22  Variable Synchronization of Command/Data Sync Start Frame Delimiter */
    uint32_t INVDATA:1;                 /**< bit:     23  Inverted Data                            */
    uint32_t MAX_ITERATION:3;           /**< bit: 24..26  Maximum Number of Automatic Iteration    */
    uint32_t :1;                        /**< bit:     27  Reserved */
    uint32_t FILTER:1;                  /**< bit:     28  Receive Line Filter                      */
    uint32_t MAN:1;                     /**< bit:     29  Manchester Encoder/Decoder Enable        */
    uint32_t MODSYNC:1;                 /**< bit:     30  Manchester Synchronization Mode          */
    uint32_t ONEBIT:1;                  /**< bit:     31  Start Frame Delimiter Selector           */
  } USART;                                /**< Structure used for USART mode access */
  struct { // SPI mode
    uint32_t :8;                        /**< bit:   0..7  Reserved */
    uint32_t CPHA:1;                    /**< bit:      8  SPI Clock Phase                          */
    uint32_t :7;                        /**< bit:  9..15  Reserved */
    uint32_t CPOL:1;                    /**< bit:     16  SPI Clock Polarity                       */
    uint32_t :3;                        /**< bit: 17..19  Reserved */
    uint32_t WRDBT:1;                   /**< bit:     20  Wait Read Data Before Transfer           */
    uint32_t :11;                       /**< bit: 21..31  Reserved */
  } SPI;                                /**< Structure used for SPI mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_MR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_MR_OFFSET                        (0x04)                                        /**<  (US_MR) Mode Register  Offset */

#define US_MR_USART_MODE_Pos                0                                              /**< (US_MR) USART Mode of Operation Position */
#define US_MR_USART_MODE_Msk                (_U_(0xF) << US_MR_USART_MODE_Pos)             /**< (US_MR) USART Mode of Operation Mask */
#define US_MR_USART_MODE(value)             (US_MR_USART_MODE_Msk & ((value) << US_MR_USART_MODE_Pos))
#define   US_MR_USART_MODE_NORMAL_Val       _U_(0x0)                                       /**< (US_MR) Normal mode  */
#define   US_MR_USART_MODE_RS485_Val        _U_(0x1)                                       /**< (US_MR) RS485  */
#define   US_MR_USART_MODE_HW_HANDSHAKING_Val _U_(0x2)                                       /**< (US_MR) Hardware handshaking  */
#define   US_MR_USART_MODE_MODEM_Val        _U_(0x3)                                       /**< (US_MR) Modem  */
#define   US_MR_USART_MODE_IS07816_T_0_Val  _U_(0x4)                                       /**< (US_MR) IS07816 Protocol: T = 0  */
#define   US_MR_USART_MODE_IS07816_T_1_Val  _U_(0x6)                                       /**< (US_MR) IS07816 Protocol: T = 1  */
#define   US_MR_USART_MODE_IRDA_Val         _U_(0x8)                                       /**< (US_MR) IrDA  */
#define   US_MR_USART_MODE_LON_Val          _U_(0x9)                                       /**< (US_MR) LON  */
#define   US_MR_USART_MODE_LIN_MASTER_Val   _U_(0xA)                                       /**< (US_MR) LIN Master mode  */
#define   US_MR_USART_MODE_LIN_SLAVE_Val    _U_(0xB)                                       /**< (US_MR) LIN Slave mode  */
#define   US_MR_USART_MODE_SPI_MASTER_Val   _U_(0xE)                                       /**< (US_MR) SPI Master mode (CLKO must be written to 1 and USCLKS = 0, 1 or 2)  */
#define   US_MR_USART_MODE_SPI_SLAVE_Val    _U_(0xF)                                       /**< (US_MR) SPI Slave mode  */
#define US_MR_USART_MODE_NORMAL             (US_MR_USART_MODE_NORMAL_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) Normal mode Position  */
#define US_MR_USART_MODE_RS485              (US_MR_USART_MODE_RS485_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) RS485 Position  */
#define US_MR_USART_MODE_HW_HANDSHAKING     (US_MR_USART_MODE_HW_HANDSHAKING_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) Hardware handshaking Position  */
#define US_MR_USART_MODE_MODEM              (US_MR_USART_MODE_MODEM_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) Modem Position  */
#define US_MR_USART_MODE_IS07816_T_0        (US_MR_USART_MODE_IS07816_T_0_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) IS07816 Protocol: T = 0 Position  */
#define US_MR_USART_MODE_IS07816_T_1        (US_MR_USART_MODE_IS07816_T_1_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) IS07816 Protocol: T = 1 Position  */
#define US_MR_USART_MODE_IRDA               (US_MR_USART_MODE_IRDA_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) IrDA Position  */
#define US_MR_USART_MODE_LON                (US_MR_USART_MODE_LON_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) LON Position  */
#define US_MR_USART_MODE_LIN_MASTER         (US_MR_USART_MODE_LIN_MASTER_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) LIN Master mode Position  */
#define US_MR_USART_MODE_LIN_SLAVE          (US_MR_USART_MODE_LIN_SLAVE_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) LIN Slave mode Position  */
#define US_MR_USART_MODE_SPI_MASTER         (US_MR_USART_MODE_SPI_MASTER_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) SPI Master mode (CLKO must be written to 1 and USCLKS = 0, 1 or 2) Position  */
#define US_MR_USART_MODE_SPI_SLAVE          (US_MR_USART_MODE_SPI_SLAVE_Val << US_MR_USART_MODE_Pos)  /**< (US_MR) SPI Slave mode Position  */
#define US_MR_USCLKS_Pos                    4                                              /**< (US_MR) Clock Selection Position */
#define US_MR_USCLKS_Msk                    (_U_(0x3) << US_MR_USCLKS_Pos)                 /**< (US_MR) Clock Selection Mask */
#define US_MR_USCLKS(value)                 (US_MR_USCLKS_Msk & ((value) << US_MR_USCLKS_Pos))
#define   US_MR_USCLKS_MCK_Val              _U_(0x0)                                       /**< (US_MR) Peripheral clock is selected  */
#define   US_MR_USCLKS_DIV_Val              _U_(0x1)                                       /**< (US_MR) Peripheral clock divided (DIV = 8) is selected  */
#define   US_MR_USCLKS_PCK_Val              _U_(0x2)                                       /**< (US_MR) PMC programmable clock (PCK) is selected. If the SCK pin is driven (CLKO = 1), the CD field must be greater than 1.  */
#define   US_MR_USCLKS_SCK_Val              _U_(0x3)                                       /**< (US_MR) Serial clock (SCK) is selected  */
#define US_MR_USCLKS_MCK                    (US_MR_USCLKS_MCK_Val << US_MR_USCLKS_Pos)     /**< (US_MR) Peripheral clock is selected Position  */
#define US_MR_USCLKS_DIV                    (US_MR_USCLKS_DIV_Val << US_MR_USCLKS_Pos)     /**< (US_MR) Peripheral clock divided (DIV = 8) is selected Position  */
#define US_MR_USCLKS_PCK                    (US_MR_USCLKS_PCK_Val << US_MR_USCLKS_Pos)     /**< (US_MR) PMC programmable clock (PCK) is selected. If the SCK pin is driven (CLKO = 1), the CD field must be greater than 1. Position  */
#define US_MR_USCLKS_SCK                    (US_MR_USCLKS_SCK_Val << US_MR_USCLKS_Pos)     /**< (US_MR) Serial clock (SCK) is selected Position  */
#define US_MR_CHRL_Pos                      6                                              /**< (US_MR) Character Length Position */
#define US_MR_CHRL_Msk                      (_U_(0x3) << US_MR_CHRL_Pos)                   /**< (US_MR) Character Length Mask */
#define US_MR_CHRL(value)                   (US_MR_CHRL_Msk & ((value) << US_MR_CHRL_Pos))
#define   US_MR_CHRL_5_BIT_Val              _U_(0x0)                                       /**< (US_MR) Character length is 5 bits  */
#define   US_MR_CHRL_6_BIT_Val              _U_(0x1)                                       /**< (US_MR) Character length is 6 bits  */
#define   US_MR_CHRL_7_BIT_Val              _U_(0x2)                                       /**< (US_MR) Character length is 7 bits  */
#define   US_MR_CHRL_8_BIT_Val              _U_(0x3)                                       /**< (US_MR) Character length is 8 bits  */
#define US_MR_CHRL_5_BIT                    (US_MR_CHRL_5_BIT_Val << US_MR_CHRL_Pos)       /**< (US_MR) Character length is 5 bits Position  */
#define US_MR_CHRL_6_BIT                    (US_MR_CHRL_6_BIT_Val << US_MR_CHRL_Pos)       /**< (US_MR) Character length is 6 bits Position  */
#define US_MR_CHRL_7_BIT                    (US_MR_CHRL_7_BIT_Val << US_MR_CHRL_Pos)       /**< (US_MR) Character length is 7 bits Position  */
#define US_MR_CHRL_8_BIT                    (US_MR_CHRL_8_BIT_Val << US_MR_CHRL_Pos)       /**< (US_MR) Character length is 8 bits Position  */
#define US_MR_CLKO_Pos                      18                                             /**< (US_MR) Clock Output Select Position */
#define US_MR_CLKO_Msk                      (_U_(0x1) << US_MR_CLKO_Pos)                   /**< (US_MR) Clock Output Select Mask */
#define US_MR_CLKO                          US_MR_CLKO_Msk                                 /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_CLKO_Msk instead */
#define US_MR_MASK                          _U_(0x400FF)                                   /**< \deprecated (US_MR) Register MASK  (Use US_MR_Msk instead)  */
#define US_MR_Msk                           _U_(0x400FF)                                   /**< (US_MR) Register Mask  */

/* USART mode */
#define US_MR_USART_SYNC_Pos                8                                              /**< (US_MR) Synchronous Mode Select Position */
#define US_MR_USART_SYNC_Msk                (_U_(0x1) << US_MR_USART_SYNC_Pos)             /**< (US_MR) Synchronous Mode Select Mask */
#define US_MR_USART_SYNC                    US_MR_USART_SYNC_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_SYNC_Msk instead */
#define US_MR_USART_PAR_Pos                 9                                              /**< (US_MR) Parity Type Position */
#define US_MR_USART_PAR_Msk                 (_U_(0x7) << US_MR_USART_PAR_Pos)              /**< (US_MR) Parity Type Mask */
#define US_MR_USART_PAR(value)              (US_MR_USART_PAR_Msk & ((value) << US_MR_USART_PAR_Pos))
#define   US_MR_USART_PAR_EVEN_Val          _U_(0x0)                                       /**< (US_MR) USART Even parity  */
#define   US_MR_USART_PAR_ODD_Val           _U_(0x1)                                       /**< (US_MR) USART Odd parity  */
#define   US_MR_USART_PAR_SPACE_Val         _U_(0x2)                                       /**< (US_MR) USART Parity forced to 0 (Space)  */
#define   US_MR_USART_PAR_MARK_Val          _U_(0x3)                                       /**< (US_MR) USART Parity forced to 1 (Mark)  */
#define   US_MR_USART_PAR_NO_Val            _U_(0x4)                                       /**< (US_MR) USART No parity  */
#define   US_MR_USART_PAR_MULTIDROP_Val     _U_(0x6)                                       /**< (US_MR) USART Multidrop mode  */
#define US_MR_USART_PAR_EVEN                (US_MR_USART_PAR_EVEN_Val << US_MR_USART_PAR_Pos)  /**< (US_MR) Even parity Position  */
#define US_MR_USART_PAR_ODD                 (US_MR_USART_PAR_ODD_Val << US_MR_USART_PAR_Pos)  /**< (US_MR) Odd parity Position  */
#define US_MR_USART_PAR_SPACE               (US_MR_USART_PAR_SPACE_Val << US_MR_USART_PAR_Pos)  /**< (US_MR) Parity forced to 0 (Space) Position  */
#define US_MR_USART_PAR_MARK                (US_MR_USART_PAR_MARK_Val << US_MR_USART_PAR_Pos)  /**< (US_MR) Parity forced to 1 (Mark) Position  */
#define US_MR_USART_PAR_NO                  (US_MR_USART_PAR_NO_Val << US_MR_USART_PAR_Pos)  /**< (US_MR) No parity Position  */
#define US_MR_USART_PAR_MULTIDROP           (US_MR_USART_PAR_MULTIDROP_Val << US_MR_USART_PAR_Pos)  /**< (US_MR) Multidrop mode Position  */
#define US_MR_USART_NBSTOP_Pos              12                                             /**< (US_MR) Number of Stop Bits Position */
#define US_MR_USART_NBSTOP_Msk              (_U_(0x3) << US_MR_USART_NBSTOP_Pos)           /**< (US_MR) Number of Stop Bits Mask */
#define US_MR_USART_NBSTOP(value)           (US_MR_USART_NBSTOP_Msk & ((value) << US_MR_USART_NBSTOP_Pos))
#define   US_MR_USART_NBSTOP_1_BIT_Val      _U_(0x0)                                       /**< (US_MR) USART 1 stop bit  */
#define   US_MR_USART_NBSTOP_1_5_BIT_Val    _U_(0x1)                                       /**< (US_MR) USART 1.5 stop bit (SYNC = 0) or reserved (SYNC = 1)  */
#define   US_MR_USART_NBSTOP_2_BIT_Val      _U_(0x2)                                       /**< (US_MR) USART 2 stop bits  */
#define US_MR_USART_NBSTOP_1_BIT            (US_MR_USART_NBSTOP_1_BIT_Val << US_MR_USART_NBSTOP_Pos)  /**< (US_MR) 1 stop bit Position  */
#define US_MR_USART_NBSTOP_1_5_BIT          (US_MR_USART_NBSTOP_1_5_BIT_Val << US_MR_USART_NBSTOP_Pos)  /**< (US_MR) 1.5 stop bit (SYNC = 0) or reserved (SYNC = 1) Position  */
#define US_MR_USART_NBSTOP_2_BIT            (US_MR_USART_NBSTOP_2_BIT_Val << US_MR_USART_NBSTOP_Pos)  /**< (US_MR) 2 stop bits Position  */
#define US_MR_USART_CHMODE_Pos              14                                             /**< (US_MR) Channel Mode Position */
#define US_MR_USART_CHMODE_Msk              (_U_(0x3) << US_MR_USART_CHMODE_Pos)           /**< (US_MR) Channel Mode Mask */
#define US_MR_USART_CHMODE(value)           (US_MR_USART_CHMODE_Msk & ((value) << US_MR_USART_CHMODE_Pos))
#define   US_MR_USART_CHMODE_NORMAL_Val     _U_(0x0)                                       /**< (US_MR) USART Normal mode  */
#define   US_MR_USART_CHMODE_AUTOMATIC_Val  _U_(0x1)                                       /**< (US_MR) USART Automatic Echo. Receiver input is connected to the TXD pin.  */
#define   US_MR_USART_CHMODE_LOCAL_LOOPBACK_Val _U_(0x2)                                       /**< (US_MR) USART Local Loopback. Transmitter output is connected to the Receiver Input.  */
#define   US_MR_USART_CHMODE_REMOTE_LOOPBACK_Val _U_(0x3)                                       /**< (US_MR) USART Remote Loopback. RXD pin is internally connected to the TXD pin.  */
#define US_MR_USART_CHMODE_NORMAL           (US_MR_USART_CHMODE_NORMAL_Val << US_MR_USART_CHMODE_Pos)  /**< (US_MR) Normal mode Position  */
#define US_MR_USART_CHMODE_AUTOMATIC        (US_MR_USART_CHMODE_AUTOMATIC_Val << US_MR_USART_CHMODE_Pos)  /**< (US_MR) Automatic Echo. Receiver input is connected to the TXD pin. Position  */
#define US_MR_USART_CHMODE_LOCAL_LOOPBACK   (US_MR_USART_CHMODE_LOCAL_LOOPBACK_Val << US_MR_USART_CHMODE_Pos)  /**< (US_MR) Local Loopback. Transmitter output is connected to the Receiver Input. Position  */
#define US_MR_USART_CHMODE_REMOTE_LOOPBACK  (US_MR_USART_CHMODE_REMOTE_LOOPBACK_Val << US_MR_USART_CHMODE_Pos)  /**< (US_MR) Remote Loopback. RXD pin is internally connected to the TXD pin. Position  */
#define US_MR_USART_MSBF_Pos                16                                             /**< (US_MR) Bit Order Position */
#define US_MR_USART_MSBF_Msk                (_U_(0x1) << US_MR_USART_MSBF_Pos)             /**< (US_MR) Bit Order Mask */
#define US_MR_USART_MSBF                    US_MR_USART_MSBF_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_MSBF_Msk instead */
#define US_MR_USART_MODE9_Pos               17                                             /**< (US_MR) 9-bit Character Length Position */
#define US_MR_USART_MODE9_Msk               (_U_(0x1) << US_MR_USART_MODE9_Pos)            /**< (US_MR) 9-bit Character Length Mask */
#define US_MR_USART_MODE9                   US_MR_USART_MODE9_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_MODE9_Msk instead */
#define US_MR_USART_OVER_Pos                19                                             /**< (US_MR) Oversampling Mode Position */
#define US_MR_USART_OVER_Msk                (_U_(0x1) << US_MR_USART_OVER_Pos)             /**< (US_MR) Oversampling Mode Mask */
#define US_MR_USART_OVER                    US_MR_USART_OVER_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_OVER_Msk instead */
#define US_MR_USART_INACK_Pos               20                                             /**< (US_MR) Inhibit Non Acknowledge Position */
#define US_MR_USART_INACK_Msk               (_U_(0x1) << US_MR_USART_INACK_Pos)            /**< (US_MR) Inhibit Non Acknowledge Mask */
#define US_MR_USART_INACK                   US_MR_USART_INACK_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_INACK_Msk instead */
#define US_MR_USART_DSNACK_Pos              21                                             /**< (US_MR) Disable Successive NACK Position */
#define US_MR_USART_DSNACK_Msk              (_U_(0x1) << US_MR_USART_DSNACK_Pos)           /**< (US_MR) Disable Successive NACK Mask */
#define US_MR_USART_DSNACK                  US_MR_USART_DSNACK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_DSNACK_Msk instead */
#define US_MR_USART_VAR_SYNC_Pos            22                                             /**< (US_MR) Variable Synchronization of Command/Data Sync Start Frame Delimiter Position */
#define US_MR_USART_VAR_SYNC_Msk            (_U_(0x1) << US_MR_USART_VAR_SYNC_Pos)         /**< (US_MR) Variable Synchronization of Command/Data Sync Start Frame Delimiter Mask */
#define US_MR_USART_VAR_SYNC                US_MR_USART_VAR_SYNC_Msk                       /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_VAR_SYNC_Msk instead */
#define US_MR_USART_INVDATA_Pos             23                                             /**< (US_MR) Inverted Data Position */
#define US_MR_USART_INVDATA_Msk             (_U_(0x1) << US_MR_USART_INVDATA_Pos)          /**< (US_MR) Inverted Data Mask */
#define US_MR_USART_INVDATA                 US_MR_USART_INVDATA_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_INVDATA_Msk instead */
#define US_MR_USART_MAX_ITERATION_Pos       24                                             /**< (US_MR) Maximum Number of Automatic Iteration Position */
#define US_MR_USART_MAX_ITERATION_Msk       (_U_(0x7) << US_MR_USART_MAX_ITERATION_Pos)    /**< (US_MR) Maximum Number of Automatic Iteration Mask */
#define US_MR_USART_MAX_ITERATION(value)    (US_MR_USART_MAX_ITERATION_Msk & ((value) << US_MR_USART_MAX_ITERATION_Pos))
#define US_MR_USART_FILTER_Pos              28                                             /**< (US_MR) Receive Line Filter Position */
#define US_MR_USART_FILTER_Msk              (_U_(0x1) << US_MR_USART_FILTER_Pos)           /**< (US_MR) Receive Line Filter Mask */
#define US_MR_USART_FILTER                  US_MR_USART_FILTER_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_FILTER_Msk instead */
#define US_MR_USART_MAN_Pos                 29                                             /**< (US_MR) Manchester Encoder/Decoder Enable Position */
#define US_MR_USART_MAN_Msk                 (_U_(0x1) << US_MR_USART_MAN_Pos)              /**< (US_MR) Manchester Encoder/Decoder Enable Mask */
#define US_MR_USART_MAN                     US_MR_USART_MAN_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_MAN_Msk instead */
#define US_MR_USART_MODSYNC_Pos             30                                             /**< (US_MR) Manchester Synchronization Mode Position */
#define US_MR_USART_MODSYNC_Msk             (_U_(0x1) << US_MR_USART_MODSYNC_Pos)          /**< (US_MR) Manchester Synchronization Mode Mask */
#define US_MR_USART_MODSYNC                 US_MR_USART_MODSYNC_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_MODSYNC_Msk instead */
#define US_MR_USART_ONEBIT_Pos              31                                             /**< (US_MR) Start Frame Delimiter Selector Position */
#define US_MR_USART_ONEBIT_Msk              (_U_(0x1) << US_MR_USART_ONEBIT_Pos)           /**< (US_MR) Start Frame Delimiter Selector Mask */
#define US_MR_USART_ONEBIT                  US_MR_USART_ONEBIT_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_USART_ONEBIT_Msk instead */
#define US_MR_USART_MASK                    _U_(0xF7FBFF00)                                /**< \deprecated (US_MR_USART) Register MASK  (Use US_MR_USART_Msk instead)  */
#define US_MR_USART_Msk                     _U_(0xF7FBFF00)                                /**< (US_MR_USART) Register Mask  */

/* SPI mode */
#define US_MR_SPI_CPHA_Pos                  8                                              /**< (US_MR) SPI Clock Phase Position */
#define US_MR_SPI_CPHA_Msk                  (_U_(0x1) << US_MR_SPI_CPHA_Pos)               /**< (US_MR) SPI Clock Phase Mask */
#define US_MR_SPI_CPHA                      US_MR_SPI_CPHA_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_SPI_CPHA_Msk instead */
#define US_MR_SPI_CPOL_Pos                  16                                             /**< (US_MR) SPI Clock Polarity Position */
#define US_MR_SPI_CPOL_Msk                  (_U_(0x1) << US_MR_SPI_CPOL_Pos)               /**< (US_MR) SPI Clock Polarity Mask */
#define US_MR_SPI_CPOL                      US_MR_SPI_CPOL_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_SPI_CPOL_Msk instead */
#define US_MR_SPI_WRDBT_Pos                 20                                             /**< (US_MR) Wait Read Data Before Transfer Position */
#define US_MR_SPI_WRDBT_Msk                 (_U_(0x1) << US_MR_SPI_WRDBT_Pos)              /**< (US_MR) Wait Read Data Before Transfer Mask */
#define US_MR_SPI_WRDBT                     US_MR_SPI_WRDBT_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MR_SPI_WRDBT_Msk instead */
#define US_MR_SPI_MASK                      _U_(0x110100)                                  /**< \deprecated (US_MR_SPI) Register MASK  (Use US_MR_SPI_Msk instead)  */
#define US_MR_SPI_Msk                       _U_(0x110100)                                  /**< (US_MR_SPI) Register Mask  */


/* -------- US_IER : (USART Offset: 0x08) (/W 32) Interrupt Enable Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RXRDY:1;                   /**< bit:      0  RXRDY Interrupt Enable                   */
    uint32_t TXRDY:1;                   /**< bit:      1  TXRDY Interrupt Enable                   */
    uint32_t :3;                        /**< bit:   2..4  Reserved */
    uint32_t OVRE:1;                    /**< bit:      5  Overrun Error Interrupt Enable           */
    uint32_t :3;                        /**< bit:   6..8  Reserved */
    uint32_t TXEMPTY:1;                 /**< bit:      9  TXEMPTY Interrupt Enable                 */
    uint32_t :22;                       /**< bit: 10..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct { // USART mode
    uint32_t :2;                        /**< bit:   0..1  Reserved */
    uint32_t RXBRK:1;                   /**< bit:      2  Receiver Break Interrupt Enable          */
    uint32_t :7;                        /**< bit:   3..9  Reserved */
    uint32_t ITER:1;                    /**< bit:     10  Max number of Repetitions Reached Interrupt Enable */
    uint32_t :2;                        /**< bit: 11..12  Reserved */
    uint32_t NACK:1;                    /**< bit:     13  Non Acknowledge Interrupt Enable         */
    uint32_t :2;                        /**< bit: 14..15  Reserved */
    uint32_t RIIC:1;                    /**< bit:     16  Ring Indicator Input Change Enable       */
    uint32_t DSRIC:1;                   /**< bit:     17  Data Set Ready Input Change Enable       */
    uint32_t DCDIC:1;                   /**< bit:     18  Data Carrier Detect Input Change Interrupt Enable */
    uint32_t CTSIC:1;                   /**< bit:     19  Clear to Send Input Change Interrupt Enable */
    uint32_t :4;                        /**< bit: 20..23  Reserved */
    uint32_t MANE:1;                    /**< bit:     24  Manchester Error Interrupt Enable        */
    uint32_t :7;                        /**< bit: 25..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // USART_LIN mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t FRAME:1;                   /**< bit:      6  Framing Error Interrupt Enable           */
    uint32_t PARE:1;                    /**< bit:      7  Parity Error Interrupt Enable            */
    uint32_t TIMEOUT:1;                 /**< bit:      8  Timeout Interrupt Enable                 */
    uint32_t :23;                       /**< bit:  9..31  Reserved */
  } USART_LIN;                                /**< Structure used for USART_LIN mode access */
  struct { // SPI mode
    uint32_t :19;                       /**< bit:  0..18  Reserved */
    uint32_t NSSE:1;                    /**< bit:     19  NSS Line (Driving CTS Pin) Rising or Falling Edge Event */
    uint32_t :12;                       /**< bit: 20..31  Reserved */
  } SPI;                                /**< Structure used for SPI mode access */
  struct { // LIN mode
    uint32_t :13;                       /**< bit:  0..12  Reserved */
    uint32_t LINBK:1;                   /**< bit:     13  LIN Break Sent or LIN Break Received Interrupt Enable */
    uint32_t LINID:1;                   /**< bit:     14  LIN Identifier Sent or LIN Identifier Received Interrupt Enable */
    uint32_t LINTC:1;                   /**< bit:     15  LIN Transfer Completed Interrupt Enable  */
    uint32_t :9;                        /**< bit: 16..24  Reserved */
    uint32_t LINBE:1;                   /**< bit:     25  LIN Bus Error Interrupt Enable           */
    uint32_t LINISFE:1;                 /**< bit:     26  LIN Inconsistent Synch Field Error Interrupt Enable */
    uint32_t LINIPE:1;                  /**< bit:     27  LIN Identifier Parity Interrupt Enable   */
    uint32_t LINCE:1;                   /**< bit:     28  LIN Checksum Error Interrupt Enable      */
    uint32_t LINSNRE:1;                 /**< bit:     29  LIN Slave Not Responding Error Interrupt Enable */
    uint32_t LINSTE:1;                  /**< bit:     30  LIN Synch Tolerance Error Interrupt Enable */
    uint32_t LINHTE:1;                  /**< bit:     31  LIN Header Timeout Error Interrupt Enable */
  } LIN;                                /**< Structure used for LIN mode access */
  struct { // LON mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t LSFE:1;                    /**< bit:      6  LON Short Frame Error Interrupt Enable   */
    uint32_t LCRCE:1;                   /**< bit:      7  LON CRC Error Interrupt Enable           */
    uint32_t :16;                       /**< bit:  8..23  Reserved */
    uint32_t LTXD:1;                    /**< bit:     24  LON Transmission Done Interrupt Enable   */
    uint32_t LCOL:1;                    /**< bit:     25  LON Collision Interrupt Enable           */
    uint32_t LFET:1;                    /**< bit:     26  LON Frame Early Termination Interrupt Enable */
    uint32_t LRXD:1;                    /**< bit:     27  LON Reception Done Interrupt Enable      */
    uint32_t LBLOVFE:1;                 /**< bit:     28  LON Backlog Overflow Error Interrupt Enable */
    uint32_t :3;                        /**< bit: 29..31  Reserved */
  } LON;                                /**< Structure used for LON mode access */
  struct { // LON_SPI mode
    uint32_t :10;                       /**< bit:   0..9  Reserved */
    uint32_t UNRE:1;                    /**< bit:     10  Underrun Error Interrupt Enable          */
    uint32_t :21;                       /**< bit: 11..31  Reserved */
  } LON_SPI;                                /**< Structure used for LON_SPI mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_IER_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_IER_OFFSET                       (0x08)                                        /**<  (US_IER) Interrupt Enable Register  Offset */

#define US_IER_RXRDY_Pos                    0                                              /**< (US_IER) RXRDY Interrupt Enable Position */
#define US_IER_RXRDY_Msk                    (_U_(0x1) << US_IER_RXRDY_Pos)                 /**< (US_IER) RXRDY Interrupt Enable Mask */
#define US_IER_RXRDY                        US_IER_RXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_RXRDY_Msk instead */
#define US_IER_TXRDY_Pos                    1                                              /**< (US_IER) TXRDY Interrupt Enable Position */
#define US_IER_TXRDY_Msk                    (_U_(0x1) << US_IER_TXRDY_Pos)                 /**< (US_IER) TXRDY Interrupt Enable Mask */
#define US_IER_TXRDY                        US_IER_TXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_TXRDY_Msk instead */
#define US_IER_OVRE_Pos                     5                                              /**< (US_IER) Overrun Error Interrupt Enable Position */
#define US_IER_OVRE_Msk                     (_U_(0x1) << US_IER_OVRE_Pos)                  /**< (US_IER) Overrun Error Interrupt Enable Mask */
#define US_IER_OVRE                         US_IER_OVRE_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_OVRE_Msk instead */
#define US_IER_TXEMPTY_Pos                  9                                              /**< (US_IER) TXEMPTY Interrupt Enable Position */
#define US_IER_TXEMPTY_Msk                  (_U_(0x1) << US_IER_TXEMPTY_Pos)               /**< (US_IER) TXEMPTY Interrupt Enable Mask */
#define US_IER_TXEMPTY                      US_IER_TXEMPTY_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_TXEMPTY_Msk instead */
#define US_IER_MASK                         _U_(0x223)                                     /**< \deprecated (US_IER) Register MASK  (Use US_IER_Msk instead)  */
#define US_IER_Msk                          _U_(0x223)                                     /**< (US_IER) Register Mask  */

/* USART mode */
#define US_IER_USART_RXBRK_Pos              2                                              /**< (US_IER) Receiver Break Interrupt Enable Position */
#define US_IER_USART_RXBRK_Msk              (_U_(0x1) << US_IER_USART_RXBRK_Pos)           /**< (US_IER) Receiver Break Interrupt Enable Mask */
#define US_IER_USART_RXBRK                  US_IER_USART_RXBRK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_RXBRK_Msk instead */
#define US_IER_USART_ITER_Pos               10                                             /**< (US_IER) Max number of Repetitions Reached Interrupt Enable Position */
#define US_IER_USART_ITER_Msk               (_U_(0x1) << US_IER_USART_ITER_Pos)            /**< (US_IER) Max number of Repetitions Reached Interrupt Enable Mask */
#define US_IER_USART_ITER                   US_IER_USART_ITER_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_ITER_Msk instead */
#define US_IER_USART_NACK_Pos               13                                             /**< (US_IER) Non Acknowledge Interrupt Enable Position */
#define US_IER_USART_NACK_Msk               (_U_(0x1) << US_IER_USART_NACK_Pos)            /**< (US_IER) Non Acknowledge Interrupt Enable Mask */
#define US_IER_USART_NACK                   US_IER_USART_NACK_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_NACK_Msk instead */
#define US_IER_USART_RIIC_Pos               16                                             /**< (US_IER) Ring Indicator Input Change Enable Position */
#define US_IER_USART_RIIC_Msk               (_U_(0x1) << US_IER_USART_RIIC_Pos)            /**< (US_IER) Ring Indicator Input Change Enable Mask */
#define US_IER_USART_RIIC                   US_IER_USART_RIIC_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_RIIC_Msk instead */
#define US_IER_USART_DSRIC_Pos              17                                             /**< (US_IER) Data Set Ready Input Change Enable Position */
#define US_IER_USART_DSRIC_Msk              (_U_(0x1) << US_IER_USART_DSRIC_Pos)           /**< (US_IER) Data Set Ready Input Change Enable Mask */
#define US_IER_USART_DSRIC                  US_IER_USART_DSRIC_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_DSRIC_Msk instead */
#define US_IER_USART_DCDIC_Pos              18                                             /**< (US_IER) Data Carrier Detect Input Change Interrupt Enable Position */
#define US_IER_USART_DCDIC_Msk              (_U_(0x1) << US_IER_USART_DCDIC_Pos)           /**< (US_IER) Data Carrier Detect Input Change Interrupt Enable Mask */
#define US_IER_USART_DCDIC                  US_IER_USART_DCDIC_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_DCDIC_Msk instead */
#define US_IER_USART_CTSIC_Pos              19                                             /**< (US_IER) Clear to Send Input Change Interrupt Enable Position */
#define US_IER_USART_CTSIC_Msk              (_U_(0x1) << US_IER_USART_CTSIC_Pos)           /**< (US_IER) Clear to Send Input Change Interrupt Enable Mask */
#define US_IER_USART_CTSIC                  US_IER_USART_CTSIC_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_CTSIC_Msk instead */
#define US_IER_USART_MANE_Pos               24                                             /**< (US_IER) Manchester Error Interrupt Enable Position */
#define US_IER_USART_MANE_Msk               (_U_(0x1) << US_IER_USART_MANE_Pos)            /**< (US_IER) Manchester Error Interrupt Enable Mask */
#define US_IER_USART_MANE                   US_IER_USART_MANE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_MANE_Msk instead */
#define US_IER_USART_MASK                   _U_(0x10F2404)                                 /**< \deprecated (US_IER_USART) Register MASK  (Use US_IER_USART_Msk instead)  */
#define US_IER_USART_Msk                    _U_(0x10F2404)                                 /**< (US_IER_USART) Register Mask  */

/* USART_LIN mode */
#define US_IER_USART_LIN_FRAME_Pos          6                                              /**< (US_IER) Framing Error Interrupt Enable Position */
#define US_IER_USART_LIN_FRAME_Msk          (_U_(0x1) << US_IER_USART_LIN_FRAME_Pos)       /**< (US_IER) Framing Error Interrupt Enable Mask */
#define US_IER_USART_LIN_FRAME              US_IER_USART_LIN_FRAME_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_LIN_FRAME_Msk instead */
#define US_IER_USART_LIN_PARE_Pos           7                                              /**< (US_IER) Parity Error Interrupt Enable Position */
#define US_IER_USART_LIN_PARE_Msk           (_U_(0x1) << US_IER_USART_LIN_PARE_Pos)        /**< (US_IER) Parity Error Interrupt Enable Mask */
#define US_IER_USART_LIN_PARE               US_IER_USART_LIN_PARE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_LIN_PARE_Msk instead */
#define US_IER_USART_LIN_TIMEOUT_Pos        8                                              /**< (US_IER) Timeout Interrupt Enable Position */
#define US_IER_USART_LIN_TIMEOUT_Msk        (_U_(0x1) << US_IER_USART_LIN_TIMEOUT_Pos)     /**< (US_IER) Timeout Interrupt Enable Mask */
#define US_IER_USART_LIN_TIMEOUT            US_IER_USART_LIN_TIMEOUT_Msk                   /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_USART_LIN_TIMEOUT_Msk instead */
#define US_IER_USART_LIN_MASK               _U_(0x1C0)                                     /**< \deprecated (US_IER_USART_LIN) Register MASK  (Use US_IER_USART_LIN_Msk instead)  */
#define US_IER_USART_LIN_Msk                _U_(0x1C0)                                     /**< (US_IER_USART_LIN) Register Mask  */

/* SPI mode */
#define US_IER_SPI_NSSE_Pos                 19                                             /**< (US_IER) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Position */
#define US_IER_SPI_NSSE_Msk                 (_U_(0x1) << US_IER_SPI_NSSE_Pos)              /**< (US_IER) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Mask */
#define US_IER_SPI_NSSE                     US_IER_SPI_NSSE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_SPI_NSSE_Msk instead */
#define US_IER_SPI_MASK                     _U_(0x80000)                                   /**< \deprecated (US_IER_SPI) Register MASK  (Use US_IER_SPI_Msk instead)  */
#define US_IER_SPI_Msk                      _U_(0x80000)                                   /**< (US_IER_SPI) Register Mask  */

/* LIN mode */
#define US_IER_LIN_LINBK_Pos                13                                             /**< (US_IER) LIN Break Sent or LIN Break Received Interrupt Enable Position */
#define US_IER_LIN_LINBK_Msk                (_U_(0x1) << US_IER_LIN_LINBK_Pos)             /**< (US_IER) LIN Break Sent or LIN Break Received Interrupt Enable Mask */
#define US_IER_LIN_LINBK                    US_IER_LIN_LINBK_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINBK_Msk instead */
#define US_IER_LIN_LINID_Pos                14                                             /**< (US_IER) LIN Identifier Sent or LIN Identifier Received Interrupt Enable Position */
#define US_IER_LIN_LINID_Msk                (_U_(0x1) << US_IER_LIN_LINID_Pos)             /**< (US_IER) LIN Identifier Sent or LIN Identifier Received Interrupt Enable Mask */
#define US_IER_LIN_LINID                    US_IER_LIN_LINID_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINID_Msk instead */
#define US_IER_LIN_LINTC_Pos                15                                             /**< (US_IER) LIN Transfer Completed Interrupt Enable Position */
#define US_IER_LIN_LINTC_Msk                (_U_(0x1) << US_IER_LIN_LINTC_Pos)             /**< (US_IER) LIN Transfer Completed Interrupt Enable Mask */
#define US_IER_LIN_LINTC                    US_IER_LIN_LINTC_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINTC_Msk instead */
#define US_IER_LIN_LINBE_Pos                25                                             /**< (US_IER) LIN Bus Error Interrupt Enable Position */
#define US_IER_LIN_LINBE_Msk                (_U_(0x1) << US_IER_LIN_LINBE_Pos)             /**< (US_IER) LIN Bus Error Interrupt Enable Mask */
#define US_IER_LIN_LINBE                    US_IER_LIN_LINBE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINBE_Msk instead */
#define US_IER_LIN_LINISFE_Pos              26                                             /**< (US_IER) LIN Inconsistent Synch Field Error Interrupt Enable Position */
#define US_IER_LIN_LINISFE_Msk              (_U_(0x1) << US_IER_LIN_LINISFE_Pos)           /**< (US_IER) LIN Inconsistent Synch Field Error Interrupt Enable Mask */
#define US_IER_LIN_LINISFE                  US_IER_LIN_LINISFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINISFE_Msk instead */
#define US_IER_LIN_LINIPE_Pos               27                                             /**< (US_IER) LIN Identifier Parity Interrupt Enable Position */
#define US_IER_LIN_LINIPE_Msk               (_U_(0x1) << US_IER_LIN_LINIPE_Pos)            /**< (US_IER) LIN Identifier Parity Interrupt Enable Mask */
#define US_IER_LIN_LINIPE                   US_IER_LIN_LINIPE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINIPE_Msk instead */
#define US_IER_LIN_LINCE_Pos                28                                             /**< (US_IER) LIN Checksum Error Interrupt Enable Position */
#define US_IER_LIN_LINCE_Msk                (_U_(0x1) << US_IER_LIN_LINCE_Pos)             /**< (US_IER) LIN Checksum Error Interrupt Enable Mask */
#define US_IER_LIN_LINCE                    US_IER_LIN_LINCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINCE_Msk instead */
#define US_IER_LIN_LINSNRE_Pos              29                                             /**< (US_IER) LIN Slave Not Responding Error Interrupt Enable Position */
#define US_IER_LIN_LINSNRE_Msk              (_U_(0x1) << US_IER_LIN_LINSNRE_Pos)           /**< (US_IER) LIN Slave Not Responding Error Interrupt Enable Mask */
#define US_IER_LIN_LINSNRE                  US_IER_LIN_LINSNRE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINSNRE_Msk instead */
#define US_IER_LIN_LINSTE_Pos               30                                             /**< (US_IER) LIN Synch Tolerance Error Interrupt Enable Position */
#define US_IER_LIN_LINSTE_Msk               (_U_(0x1) << US_IER_LIN_LINSTE_Pos)            /**< (US_IER) LIN Synch Tolerance Error Interrupt Enable Mask */
#define US_IER_LIN_LINSTE                   US_IER_LIN_LINSTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINSTE_Msk instead */
#define US_IER_LIN_LINHTE_Pos               31                                             /**< (US_IER) LIN Header Timeout Error Interrupt Enable Position */
#define US_IER_LIN_LINHTE_Msk               (_U_(0x1) << US_IER_LIN_LINHTE_Pos)            /**< (US_IER) LIN Header Timeout Error Interrupt Enable Mask */
#define US_IER_LIN_LINHTE                   US_IER_LIN_LINHTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LIN_LINHTE_Msk instead */
#define US_IER_LIN_MASK                     _U_(0xFE00E000)                                /**< \deprecated (US_IER_LIN) Register MASK  (Use US_IER_LIN_Msk instead)  */
#define US_IER_LIN_Msk                      _U_(0xFE00E000)                                /**< (US_IER_LIN) Register Mask  */

/* LON mode */
#define US_IER_LON_LSFE_Pos                 6                                              /**< (US_IER) LON Short Frame Error Interrupt Enable Position */
#define US_IER_LON_LSFE_Msk                 (_U_(0x1) << US_IER_LON_LSFE_Pos)              /**< (US_IER) LON Short Frame Error Interrupt Enable Mask */
#define US_IER_LON_LSFE                     US_IER_LON_LSFE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LSFE_Msk instead */
#define US_IER_LON_LCRCE_Pos                7                                              /**< (US_IER) LON CRC Error Interrupt Enable Position */
#define US_IER_LON_LCRCE_Msk                (_U_(0x1) << US_IER_LON_LCRCE_Pos)             /**< (US_IER) LON CRC Error Interrupt Enable Mask */
#define US_IER_LON_LCRCE                    US_IER_LON_LCRCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LCRCE_Msk instead */
#define US_IER_LON_LTXD_Pos                 24                                             /**< (US_IER) LON Transmission Done Interrupt Enable Position */
#define US_IER_LON_LTXD_Msk                 (_U_(0x1) << US_IER_LON_LTXD_Pos)              /**< (US_IER) LON Transmission Done Interrupt Enable Mask */
#define US_IER_LON_LTXD                     US_IER_LON_LTXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LTXD_Msk instead */
#define US_IER_LON_LCOL_Pos                 25                                             /**< (US_IER) LON Collision Interrupt Enable Position */
#define US_IER_LON_LCOL_Msk                 (_U_(0x1) << US_IER_LON_LCOL_Pos)              /**< (US_IER) LON Collision Interrupt Enable Mask */
#define US_IER_LON_LCOL                     US_IER_LON_LCOL_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LCOL_Msk instead */
#define US_IER_LON_LFET_Pos                 26                                             /**< (US_IER) LON Frame Early Termination Interrupt Enable Position */
#define US_IER_LON_LFET_Msk                 (_U_(0x1) << US_IER_LON_LFET_Pos)              /**< (US_IER) LON Frame Early Termination Interrupt Enable Mask */
#define US_IER_LON_LFET                     US_IER_LON_LFET_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LFET_Msk instead */
#define US_IER_LON_LRXD_Pos                 27                                             /**< (US_IER) LON Reception Done Interrupt Enable Position */
#define US_IER_LON_LRXD_Msk                 (_U_(0x1) << US_IER_LON_LRXD_Pos)              /**< (US_IER) LON Reception Done Interrupt Enable Mask */
#define US_IER_LON_LRXD                     US_IER_LON_LRXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LRXD_Msk instead */
#define US_IER_LON_LBLOVFE_Pos              28                                             /**< (US_IER) LON Backlog Overflow Error Interrupt Enable Position */
#define US_IER_LON_LBLOVFE_Msk              (_U_(0x1) << US_IER_LON_LBLOVFE_Pos)           /**< (US_IER) LON Backlog Overflow Error Interrupt Enable Mask */
#define US_IER_LON_LBLOVFE                  US_IER_LON_LBLOVFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_LBLOVFE_Msk instead */
#define US_IER_LON_MASK                     _U_(0x1F0000C0)                                /**< \deprecated (US_IER_LON) Register MASK  (Use US_IER_LON_Msk instead)  */
#define US_IER_LON_Msk                      _U_(0x1F0000C0)                                /**< (US_IER_LON) Register Mask  */

/* LON_SPI mode */
#define US_IER_LON_SPI_UNRE_Pos             10                                             /**< (US_IER) Underrun Error Interrupt Enable Position */
#define US_IER_LON_SPI_UNRE_Msk             (_U_(0x1) << US_IER_LON_SPI_UNRE_Pos)          /**< (US_IER) Underrun Error Interrupt Enable Mask */
#define US_IER_LON_SPI_UNRE                 US_IER_LON_SPI_UNRE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IER_LON_SPI_UNRE_Msk instead */
#define US_IER_LON_SPI_MASK                 _U_(0x400)                                     /**< \deprecated (US_IER_LON_SPI) Register MASK  (Use US_IER_LON_SPI_Msk instead)  */
#define US_IER_LON_SPI_Msk                  _U_(0x400)                                     /**< (US_IER_LON_SPI) Register Mask  */


/* -------- US_IDR : (USART Offset: 0x0c) (/W 32) Interrupt Disable Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RXRDY:1;                   /**< bit:      0  RXRDY Interrupt Disable                  */
    uint32_t TXRDY:1;                   /**< bit:      1  TXRDY Interrupt Disable                  */
    uint32_t :3;                        /**< bit:   2..4  Reserved */
    uint32_t OVRE:1;                    /**< bit:      5  Overrun Error Interrupt Enable           */
    uint32_t :3;                        /**< bit:   6..8  Reserved */
    uint32_t TXEMPTY:1;                 /**< bit:      9  TXEMPTY Interrupt Disable                */
    uint32_t :6;                        /**< bit: 10..15  Reserved */
    uint32_t RIIC:1;                    /**< bit:     16  Ring Indicator Input Change Disable      */
    uint32_t DSRIC:1;                   /**< bit:     17  Data Set Ready Input Change Disable      */
    uint32_t DCDIC:1;                   /**< bit:     18  Data Carrier Detect Input Change Interrupt Disable */
    uint32_t :13;                       /**< bit: 19..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct { // USART mode
    uint32_t :2;                        /**< bit:   0..1  Reserved */
    uint32_t RXBRK:1;                   /**< bit:      2  Receiver Break Interrupt Disable         */
    uint32_t :7;                        /**< bit:   3..9  Reserved */
    uint32_t ITER:1;                    /**< bit:     10  Max Number of Repetitions Reached Interrupt Disable */
    uint32_t :2;                        /**< bit: 11..12  Reserved */
    uint32_t NACK:1;                    /**< bit:     13  Non Acknowledge Interrupt Disable        */
    uint32_t :5;                        /**< bit: 14..18  Reserved */
    uint32_t CTSIC:1;                   /**< bit:     19  Clear to Send Input Change Interrupt Disable */
    uint32_t :4;                        /**< bit: 20..23  Reserved */
    uint32_t MANE:1;                    /**< bit:     24  Manchester Error Interrupt Disable       */
    uint32_t :7;                        /**< bit: 25..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // USART_LIN mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t FRAME:1;                   /**< bit:      6  Framing Error Interrupt Disable          */
    uint32_t PARE:1;                    /**< bit:      7  Parity Error Interrupt Disable           */
    uint32_t TIMEOUT:1;                 /**< bit:      8  Timeout Interrupt Disable                */
    uint32_t :23;                       /**< bit:  9..31  Reserved */
  } USART_LIN;                                /**< Structure used for USART_LIN mode access */
  struct { // SPI mode
    uint32_t :19;                       /**< bit:  0..18  Reserved */
    uint32_t NSSE:1;                    /**< bit:     19  NSS Line (Driving CTS Pin) Rising or Falling Edge Event */
    uint32_t :12;                       /**< bit: 20..31  Reserved */
  } SPI;                                /**< Structure used for SPI mode access */
  struct { // LIN mode
    uint32_t :13;                       /**< bit:  0..12  Reserved */
    uint32_t LINBK:1;                   /**< bit:     13  LIN Break Sent or LIN Break Received Interrupt Disable */
    uint32_t LINID:1;                   /**< bit:     14  LIN Identifier Sent or LIN Identifier Received Interrupt Disable */
    uint32_t LINTC:1;                   /**< bit:     15  LIN Transfer Completed Interrupt Disable */
    uint32_t :9;                        /**< bit: 16..24  Reserved */
    uint32_t LINBE:1;                   /**< bit:     25  LIN Bus Error Interrupt Disable          */
    uint32_t LINISFE:1;                 /**< bit:     26  LIN Inconsistent Synch Field Error Interrupt Disable */
    uint32_t LINIPE:1;                  /**< bit:     27  LIN Identifier Parity Interrupt Disable  */
    uint32_t LINCE:1;                   /**< bit:     28  LIN Checksum Error Interrupt Disable     */
    uint32_t LINSNRE:1;                 /**< bit:     29  LIN Slave Not Responding Error Interrupt Disable */
    uint32_t LINSTE:1;                  /**< bit:     30  LIN Synch Tolerance Error Interrupt Disable */
    uint32_t LINHTE:1;                  /**< bit:     31  LIN Header Timeout Error Interrupt Disable */
  } LIN;                                /**< Structure used for LIN mode access */
  struct { // LON mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t LSFE:1;                    /**< bit:      6  LON Short Frame Error Interrupt Disable  */
    uint32_t LCRCE:1;                   /**< bit:      7  LON CRC Error Interrupt Disable          */
    uint32_t :16;                       /**< bit:  8..23  Reserved */
    uint32_t LTXD:1;                    /**< bit:     24  LON Transmission Done Interrupt Disable  */
    uint32_t LCOL:1;                    /**< bit:     25  LON Collision Interrupt Disable          */
    uint32_t LFET:1;                    /**< bit:     26  LON Frame Early Termination Interrupt Disable */
    uint32_t LRXD:1;                    /**< bit:     27  LON Reception Done Interrupt Disable     */
    uint32_t LBLOVFE:1;                 /**< bit:     28  LON Backlog Overflow Error Interrupt Disable */
    uint32_t :3;                        /**< bit: 29..31  Reserved */
  } LON;                                /**< Structure used for LON mode access */
  struct { // LON_SPI mode
    uint32_t :10;                       /**< bit:   0..9  Reserved */
    uint32_t UNRE:1;                    /**< bit:     10  SPI Underrun Error Interrupt Disable     */
    uint32_t :21;                       /**< bit: 11..31  Reserved */
  } LON_SPI;                                /**< Structure used for LON_SPI mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_IDR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_IDR_OFFSET                       (0x0C)                                        /**<  (US_IDR) Interrupt Disable Register  Offset */

#define US_IDR_RXRDY_Pos                    0                                              /**< (US_IDR) RXRDY Interrupt Disable Position */
#define US_IDR_RXRDY_Msk                    (_U_(0x1) << US_IDR_RXRDY_Pos)                 /**< (US_IDR) RXRDY Interrupt Disable Mask */
#define US_IDR_RXRDY                        US_IDR_RXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_RXRDY_Msk instead */
#define US_IDR_TXRDY_Pos                    1                                              /**< (US_IDR) TXRDY Interrupt Disable Position */
#define US_IDR_TXRDY_Msk                    (_U_(0x1) << US_IDR_TXRDY_Pos)                 /**< (US_IDR) TXRDY Interrupt Disable Mask */
#define US_IDR_TXRDY                        US_IDR_TXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_TXRDY_Msk instead */
#define US_IDR_OVRE_Pos                     5                                              /**< (US_IDR) Overrun Error Interrupt Enable Position */
#define US_IDR_OVRE_Msk                     (_U_(0x1) << US_IDR_OVRE_Pos)                  /**< (US_IDR) Overrun Error Interrupt Enable Mask */
#define US_IDR_OVRE                         US_IDR_OVRE_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_OVRE_Msk instead */
#define US_IDR_TXEMPTY_Pos                  9                                              /**< (US_IDR) TXEMPTY Interrupt Disable Position */
#define US_IDR_TXEMPTY_Msk                  (_U_(0x1) << US_IDR_TXEMPTY_Pos)               /**< (US_IDR) TXEMPTY Interrupt Disable Mask */
#define US_IDR_TXEMPTY                      US_IDR_TXEMPTY_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_TXEMPTY_Msk instead */
#define US_IDR_RIIC_Pos                     16                                             /**< (US_IDR) Ring Indicator Input Change Disable Position */
#define US_IDR_RIIC_Msk                     (_U_(0x1) << US_IDR_RIIC_Pos)                  /**< (US_IDR) Ring Indicator Input Change Disable Mask */
#define US_IDR_RIIC                         US_IDR_RIIC_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_RIIC_Msk instead */
#define US_IDR_DSRIC_Pos                    17                                             /**< (US_IDR) Data Set Ready Input Change Disable Position */
#define US_IDR_DSRIC_Msk                    (_U_(0x1) << US_IDR_DSRIC_Pos)                 /**< (US_IDR) Data Set Ready Input Change Disable Mask */
#define US_IDR_DSRIC                        US_IDR_DSRIC_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_DSRIC_Msk instead */
#define US_IDR_DCDIC_Pos                    18                                             /**< (US_IDR) Data Carrier Detect Input Change Interrupt Disable Position */
#define US_IDR_DCDIC_Msk                    (_U_(0x1) << US_IDR_DCDIC_Pos)                 /**< (US_IDR) Data Carrier Detect Input Change Interrupt Disable Mask */
#define US_IDR_DCDIC                        US_IDR_DCDIC_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_DCDIC_Msk instead */
#define US_IDR_MASK                         _U_(0x70223)                                   /**< \deprecated (US_IDR) Register MASK  (Use US_IDR_Msk instead)  */
#define US_IDR_Msk                          _U_(0x70223)                                   /**< (US_IDR) Register Mask  */

/* USART mode */
#define US_IDR_USART_RXBRK_Pos              2                                              /**< (US_IDR) Receiver Break Interrupt Disable Position */
#define US_IDR_USART_RXBRK_Msk              (_U_(0x1) << US_IDR_USART_RXBRK_Pos)           /**< (US_IDR) Receiver Break Interrupt Disable Mask */
#define US_IDR_USART_RXBRK                  US_IDR_USART_RXBRK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_RXBRK_Msk instead */
#define US_IDR_USART_ITER_Pos               10                                             /**< (US_IDR) Max Number of Repetitions Reached Interrupt Disable Position */
#define US_IDR_USART_ITER_Msk               (_U_(0x1) << US_IDR_USART_ITER_Pos)            /**< (US_IDR) Max Number of Repetitions Reached Interrupt Disable Mask */
#define US_IDR_USART_ITER                   US_IDR_USART_ITER_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_ITER_Msk instead */
#define US_IDR_USART_NACK_Pos               13                                             /**< (US_IDR) Non Acknowledge Interrupt Disable Position */
#define US_IDR_USART_NACK_Msk               (_U_(0x1) << US_IDR_USART_NACK_Pos)            /**< (US_IDR) Non Acknowledge Interrupt Disable Mask */
#define US_IDR_USART_NACK                   US_IDR_USART_NACK_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_NACK_Msk instead */
#define US_IDR_USART_CTSIC_Pos              19                                             /**< (US_IDR) Clear to Send Input Change Interrupt Disable Position */
#define US_IDR_USART_CTSIC_Msk              (_U_(0x1) << US_IDR_USART_CTSIC_Pos)           /**< (US_IDR) Clear to Send Input Change Interrupt Disable Mask */
#define US_IDR_USART_CTSIC                  US_IDR_USART_CTSIC_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_CTSIC_Msk instead */
#define US_IDR_USART_MANE_Pos               24                                             /**< (US_IDR) Manchester Error Interrupt Disable Position */
#define US_IDR_USART_MANE_Msk               (_U_(0x1) << US_IDR_USART_MANE_Pos)            /**< (US_IDR) Manchester Error Interrupt Disable Mask */
#define US_IDR_USART_MANE                   US_IDR_USART_MANE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_MANE_Msk instead */
#define US_IDR_USART_MASK                   _U_(0x1082404)                                 /**< \deprecated (US_IDR_USART) Register MASK  (Use US_IDR_USART_Msk instead)  */
#define US_IDR_USART_Msk                    _U_(0x1082404)                                 /**< (US_IDR_USART) Register Mask  */

/* USART_LIN mode */
#define US_IDR_USART_LIN_FRAME_Pos          6                                              /**< (US_IDR) Framing Error Interrupt Disable Position */
#define US_IDR_USART_LIN_FRAME_Msk          (_U_(0x1) << US_IDR_USART_LIN_FRAME_Pos)       /**< (US_IDR) Framing Error Interrupt Disable Mask */
#define US_IDR_USART_LIN_FRAME              US_IDR_USART_LIN_FRAME_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_LIN_FRAME_Msk instead */
#define US_IDR_USART_LIN_PARE_Pos           7                                              /**< (US_IDR) Parity Error Interrupt Disable Position */
#define US_IDR_USART_LIN_PARE_Msk           (_U_(0x1) << US_IDR_USART_LIN_PARE_Pos)        /**< (US_IDR) Parity Error Interrupt Disable Mask */
#define US_IDR_USART_LIN_PARE               US_IDR_USART_LIN_PARE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_LIN_PARE_Msk instead */
#define US_IDR_USART_LIN_TIMEOUT_Pos        8                                              /**< (US_IDR) Timeout Interrupt Disable Position */
#define US_IDR_USART_LIN_TIMEOUT_Msk        (_U_(0x1) << US_IDR_USART_LIN_TIMEOUT_Pos)     /**< (US_IDR) Timeout Interrupt Disable Mask */
#define US_IDR_USART_LIN_TIMEOUT            US_IDR_USART_LIN_TIMEOUT_Msk                   /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_USART_LIN_TIMEOUT_Msk instead */
#define US_IDR_USART_LIN_MASK               _U_(0x1C0)                                     /**< \deprecated (US_IDR_USART_LIN) Register MASK  (Use US_IDR_USART_LIN_Msk instead)  */
#define US_IDR_USART_LIN_Msk                _U_(0x1C0)                                     /**< (US_IDR_USART_LIN) Register Mask  */

/* SPI mode */
#define US_IDR_SPI_NSSE_Pos                 19                                             /**< (US_IDR) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Position */
#define US_IDR_SPI_NSSE_Msk                 (_U_(0x1) << US_IDR_SPI_NSSE_Pos)              /**< (US_IDR) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Mask */
#define US_IDR_SPI_NSSE                     US_IDR_SPI_NSSE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_SPI_NSSE_Msk instead */
#define US_IDR_SPI_MASK                     _U_(0x80000)                                   /**< \deprecated (US_IDR_SPI) Register MASK  (Use US_IDR_SPI_Msk instead)  */
#define US_IDR_SPI_Msk                      _U_(0x80000)                                   /**< (US_IDR_SPI) Register Mask  */

/* LIN mode */
#define US_IDR_LIN_LINBK_Pos                13                                             /**< (US_IDR) LIN Break Sent or LIN Break Received Interrupt Disable Position */
#define US_IDR_LIN_LINBK_Msk                (_U_(0x1) << US_IDR_LIN_LINBK_Pos)             /**< (US_IDR) LIN Break Sent or LIN Break Received Interrupt Disable Mask */
#define US_IDR_LIN_LINBK                    US_IDR_LIN_LINBK_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINBK_Msk instead */
#define US_IDR_LIN_LINID_Pos                14                                             /**< (US_IDR) LIN Identifier Sent or LIN Identifier Received Interrupt Disable Position */
#define US_IDR_LIN_LINID_Msk                (_U_(0x1) << US_IDR_LIN_LINID_Pos)             /**< (US_IDR) LIN Identifier Sent or LIN Identifier Received Interrupt Disable Mask */
#define US_IDR_LIN_LINID                    US_IDR_LIN_LINID_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINID_Msk instead */
#define US_IDR_LIN_LINTC_Pos                15                                             /**< (US_IDR) LIN Transfer Completed Interrupt Disable Position */
#define US_IDR_LIN_LINTC_Msk                (_U_(0x1) << US_IDR_LIN_LINTC_Pos)             /**< (US_IDR) LIN Transfer Completed Interrupt Disable Mask */
#define US_IDR_LIN_LINTC                    US_IDR_LIN_LINTC_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINTC_Msk instead */
#define US_IDR_LIN_LINBE_Pos                25                                             /**< (US_IDR) LIN Bus Error Interrupt Disable Position */
#define US_IDR_LIN_LINBE_Msk                (_U_(0x1) << US_IDR_LIN_LINBE_Pos)             /**< (US_IDR) LIN Bus Error Interrupt Disable Mask */
#define US_IDR_LIN_LINBE                    US_IDR_LIN_LINBE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINBE_Msk instead */
#define US_IDR_LIN_LINISFE_Pos              26                                             /**< (US_IDR) LIN Inconsistent Synch Field Error Interrupt Disable Position */
#define US_IDR_LIN_LINISFE_Msk              (_U_(0x1) << US_IDR_LIN_LINISFE_Pos)           /**< (US_IDR) LIN Inconsistent Synch Field Error Interrupt Disable Mask */
#define US_IDR_LIN_LINISFE                  US_IDR_LIN_LINISFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINISFE_Msk instead */
#define US_IDR_LIN_LINIPE_Pos               27                                             /**< (US_IDR) LIN Identifier Parity Interrupt Disable Position */
#define US_IDR_LIN_LINIPE_Msk               (_U_(0x1) << US_IDR_LIN_LINIPE_Pos)            /**< (US_IDR) LIN Identifier Parity Interrupt Disable Mask */
#define US_IDR_LIN_LINIPE                   US_IDR_LIN_LINIPE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINIPE_Msk instead */
#define US_IDR_LIN_LINCE_Pos                28                                             /**< (US_IDR) LIN Checksum Error Interrupt Disable Position */
#define US_IDR_LIN_LINCE_Msk                (_U_(0x1) << US_IDR_LIN_LINCE_Pos)             /**< (US_IDR) LIN Checksum Error Interrupt Disable Mask */
#define US_IDR_LIN_LINCE                    US_IDR_LIN_LINCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINCE_Msk instead */
#define US_IDR_LIN_LINSNRE_Pos              29                                             /**< (US_IDR) LIN Slave Not Responding Error Interrupt Disable Position */
#define US_IDR_LIN_LINSNRE_Msk              (_U_(0x1) << US_IDR_LIN_LINSNRE_Pos)           /**< (US_IDR) LIN Slave Not Responding Error Interrupt Disable Mask */
#define US_IDR_LIN_LINSNRE                  US_IDR_LIN_LINSNRE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINSNRE_Msk instead */
#define US_IDR_LIN_LINSTE_Pos               30                                             /**< (US_IDR) LIN Synch Tolerance Error Interrupt Disable Position */
#define US_IDR_LIN_LINSTE_Msk               (_U_(0x1) << US_IDR_LIN_LINSTE_Pos)            /**< (US_IDR) LIN Synch Tolerance Error Interrupt Disable Mask */
#define US_IDR_LIN_LINSTE                   US_IDR_LIN_LINSTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINSTE_Msk instead */
#define US_IDR_LIN_LINHTE_Pos               31                                             /**< (US_IDR) LIN Header Timeout Error Interrupt Disable Position */
#define US_IDR_LIN_LINHTE_Msk               (_U_(0x1) << US_IDR_LIN_LINHTE_Pos)            /**< (US_IDR) LIN Header Timeout Error Interrupt Disable Mask */
#define US_IDR_LIN_LINHTE                   US_IDR_LIN_LINHTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LIN_LINHTE_Msk instead */
#define US_IDR_LIN_MASK                     _U_(0xFE00E000)                                /**< \deprecated (US_IDR_LIN) Register MASK  (Use US_IDR_LIN_Msk instead)  */
#define US_IDR_LIN_Msk                      _U_(0xFE00E000)                                /**< (US_IDR_LIN) Register Mask  */

/* LON mode */
#define US_IDR_LON_LSFE_Pos                 6                                              /**< (US_IDR) LON Short Frame Error Interrupt Disable Position */
#define US_IDR_LON_LSFE_Msk                 (_U_(0x1) << US_IDR_LON_LSFE_Pos)              /**< (US_IDR) LON Short Frame Error Interrupt Disable Mask */
#define US_IDR_LON_LSFE                     US_IDR_LON_LSFE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LSFE_Msk instead */
#define US_IDR_LON_LCRCE_Pos                7                                              /**< (US_IDR) LON CRC Error Interrupt Disable Position */
#define US_IDR_LON_LCRCE_Msk                (_U_(0x1) << US_IDR_LON_LCRCE_Pos)             /**< (US_IDR) LON CRC Error Interrupt Disable Mask */
#define US_IDR_LON_LCRCE                    US_IDR_LON_LCRCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LCRCE_Msk instead */
#define US_IDR_LON_LTXD_Pos                 24                                             /**< (US_IDR) LON Transmission Done Interrupt Disable Position */
#define US_IDR_LON_LTXD_Msk                 (_U_(0x1) << US_IDR_LON_LTXD_Pos)              /**< (US_IDR) LON Transmission Done Interrupt Disable Mask */
#define US_IDR_LON_LTXD                     US_IDR_LON_LTXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LTXD_Msk instead */
#define US_IDR_LON_LCOL_Pos                 25                                             /**< (US_IDR) LON Collision Interrupt Disable Position */
#define US_IDR_LON_LCOL_Msk                 (_U_(0x1) << US_IDR_LON_LCOL_Pos)              /**< (US_IDR) LON Collision Interrupt Disable Mask */
#define US_IDR_LON_LCOL                     US_IDR_LON_LCOL_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LCOL_Msk instead */
#define US_IDR_LON_LFET_Pos                 26                                             /**< (US_IDR) LON Frame Early Termination Interrupt Disable Position */
#define US_IDR_LON_LFET_Msk                 (_U_(0x1) << US_IDR_LON_LFET_Pos)              /**< (US_IDR) LON Frame Early Termination Interrupt Disable Mask */
#define US_IDR_LON_LFET                     US_IDR_LON_LFET_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LFET_Msk instead */
#define US_IDR_LON_LRXD_Pos                 27                                             /**< (US_IDR) LON Reception Done Interrupt Disable Position */
#define US_IDR_LON_LRXD_Msk                 (_U_(0x1) << US_IDR_LON_LRXD_Pos)              /**< (US_IDR) LON Reception Done Interrupt Disable Mask */
#define US_IDR_LON_LRXD                     US_IDR_LON_LRXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LRXD_Msk instead */
#define US_IDR_LON_LBLOVFE_Pos              28                                             /**< (US_IDR) LON Backlog Overflow Error Interrupt Disable Position */
#define US_IDR_LON_LBLOVFE_Msk              (_U_(0x1) << US_IDR_LON_LBLOVFE_Pos)           /**< (US_IDR) LON Backlog Overflow Error Interrupt Disable Mask */
#define US_IDR_LON_LBLOVFE                  US_IDR_LON_LBLOVFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_LBLOVFE_Msk instead */
#define US_IDR_LON_MASK                     _U_(0x1F0000C0)                                /**< \deprecated (US_IDR_LON) Register MASK  (Use US_IDR_LON_Msk instead)  */
#define US_IDR_LON_Msk                      _U_(0x1F0000C0)                                /**< (US_IDR_LON) Register Mask  */

/* LON_SPI mode */
#define US_IDR_LON_SPI_UNRE_Pos             10                                             /**< (US_IDR) SPI Underrun Error Interrupt Disable Position */
#define US_IDR_LON_SPI_UNRE_Msk             (_U_(0x1) << US_IDR_LON_SPI_UNRE_Pos)          /**< (US_IDR) SPI Underrun Error Interrupt Disable Mask */
#define US_IDR_LON_SPI_UNRE                 US_IDR_LON_SPI_UNRE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IDR_LON_SPI_UNRE_Msk instead */
#define US_IDR_LON_SPI_MASK                 _U_(0x400)                                     /**< \deprecated (US_IDR_LON_SPI) Register MASK  (Use US_IDR_LON_SPI_Msk instead)  */
#define US_IDR_LON_SPI_Msk                  _U_(0x400)                                     /**< (US_IDR_LON_SPI) Register Mask  */


/* -------- US_IMR : (USART Offset: 0x10) (R/ 32) Interrupt Mask Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RXRDY:1;                   /**< bit:      0  RXRDY Interrupt Mask                     */
    uint32_t TXRDY:1;                   /**< bit:      1  TXRDY Interrupt Mask                     */
    uint32_t :3;                        /**< bit:   2..4  Reserved */
    uint32_t OVRE:1;                    /**< bit:      5  Overrun Error Interrupt Mask             */
    uint32_t :3;                        /**< bit:   6..8  Reserved */
    uint32_t TXEMPTY:1;                 /**< bit:      9  TXEMPTY Interrupt Mask                   */
    uint32_t :6;                        /**< bit: 10..15  Reserved */
    uint32_t RIIC:1;                    /**< bit:     16  Ring Indicator Input Change Mask         */
    uint32_t DSRIC:1;                   /**< bit:     17  Data Set Ready Input Change Mask         */
    uint32_t DCDIC:1;                   /**< bit:     18  Data Carrier Detect Input Change Interrupt Mask */
    uint32_t :13;                       /**< bit: 19..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct { // USART mode
    uint32_t :2;                        /**< bit:   0..1  Reserved */
    uint32_t RXBRK:1;                   /**< bit:      2  Receiver Break Interrupt Mask            */
    uint32_t :7;                        /**< bit:   3..9  Reserved */
    uint32_t ITER:1;                    /**< bit:     10  Max Number of Repetitions Reached Interrupt Mask */
    uint32_t :2;                        /**< bit: 11..12  Reserved */
    uint32_t NACK:1;                    /**< bit:     13  Non Acknowledge Interrupt Mask           */
    uint32_t :5;                        /**< bit: 14..18  Reserved */
    uint32_t CTSIC:1;                   /**< bit:     19  Clear to Send Input Change Interrupt Mask */
    uint32_t :4;                        /**< bit: 20..23  Reserved */
    uint32_t MANE:1;                    /**< bit:     24  Manchester Error Interrupt Mask          */
    uint32_t :7;                        /**< bit: 25..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // USART_LIN mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t FRAME:1;                   /**< bit:      6  Framing Error Interrupt Mask             */
    uint32_t PARE:1;                    /**< bit:      7  Parity Error Interrupt Mask              */
    uint32_t TIMEOUT:1;                 /**< bit:      8  Timeout Interrupt Mask                   */
    uint32_t :23;                       /**< bit:  9..31  Reserved */
  } USART_LIN;                                /**< Structure used for USART_LIN mode access */
  struct { // SPI mode
    uint32_t :19;                       /**< bit:  0..18  Reserved */
    uint32_t NSSE:1;                    /**< bit:     19  NSS Line (Driving CTS Pin) Rising or Falling Edge Event */
    uint32_t :12;                       /**< bit: 20..31  Reserved */
  } SPI;                                /**< Structure used for SPI mode access */
  struct { // LIN mode
    uint32_t :13;                       /**< bit:  0..12  Reserved */
    uint32_t LINBK:1;                   /**< bit:     13  LIN Break Sent or LIN Break Received Interrupt Mask */
    uint32_t LINID:1;                   /**< bit:     14  LIN Identifier Sent or LIN Identifier Received Interrupt Mask */
    uint32_t LINTC:1;                   /**< bit:     15  LIN Transfer Completed Interrupt Mask    */
    uint32_t :9;                        /**< bit: 16..24  Reserved */
    uint32_t LINBE:1;                   /**< bit:     25  LIN Bus Error Interrupt Mask             */
    uint32_t LINISFE:1;                 /**< bit:     26  LIN Inconsistent Synch Field Error Interrupt Mask */
    uint32_t LINIPE:1;                  /**< bit:     27  LIN Identifier Parity Interrupt Mask     */
    uint32_t LINCE:1;                   /**< bit:     28  LIN Checksum Error Interrupt Mask        */
    uint32_t LINSNRE:1;                 /**< bit:     29  LIN Slave Not Responding Error Interrupt Mask */
    uint32_t LINSTE:1;                  /**< bit:     30  LIN Synch Tolerance Error Interrupt Mask */
    uint32_t LINHTE:1;                  /**< bit:     31  LIN Header Timeout Error Interrupt Mask  */
  } LIN;                                /**< Structure used for LIN mode access */
  struct { // LON mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t LSFE:1;                    /**< bit:      6  LON Short Frame Error Interrupt Mask     */
    uint32_t LCRCE:1;                   /**< bit:      7  LON CRC Error Interrupt Mask             */
    uint32_t :16;                       /**< bit:  8..23  Reserved */
    uint32_t LTXD:1;                    /**< bit:     24  LON Transmission Done Interrupt Mask     */
    uint32_t LCOL:1;                    /**< bit:     25  LON Collision Interrupt Mask             */
    uint32_t LFET:1;                    /**< bit:     26  LON Frame Early Termination Interrupt Mask */
    uint32_t LRXD:1;                    /**< bit:     27  LON Reception Done Interrupt Mask        */
    uint32_t LBLOVFE:1;                 /**< bit:     28  LON Backlog Overflow Error Interrupt Mask */
    uint32_t :3;                        /**< bit: 29..31  Reserved */
  } LON;                                /**< Structure used for LON mode access */
  struct { // LON_SPI mode
    uint32_t :10;                       /**< bit:   0..9  Reserved */
    uint32_t UNRE:1;                    /**< bit:     10  SPI Underrun Error Interrupt Mask        */
    uint32_t :21;                       /**< bit: 11..31  Reserved */
  } LON_SPI;                                /**< Structure used for LON_SPI mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_IMR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_IMR_OFFSET                       (0x10)                                        /**<  (US_IMR) Interrupt Mask Register  Offset */

#define US_IMR_RXRDY_Pos                    0                                              /**< (US_IMR) RXRDY Interrupt Mask Position */
#define US_IMR_RXRDY_Msk                    (_U_(0x1) << US_IMR_RXRDY_Pos)                 /**< (US_IMR) RXRDY Interrupt Mask Mask */
#define US_IMR_RXRDY                        US_IMR_RXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_RXRDY_Msk instead */
#define US_IMR_TXRDY_Pos                    1                                              /**< (US_IMR) TXRDY Interrupt Mask Position */
#define US_IMR_TXRDY_Msk                    (_U_(0x1) << US_IMR_TXRDY_Pos)                 /**< (US_IMR) TXRDY Interrupt Mask Mask */
#define US_IMR_TXRDY                        US_IMR_TXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_TXRDY_Msk instead */
#define US_IMR_OVRE_Pos                     5                                              /**< (US_IMR) Overrun Error Interrupt Mask Position */
#define US_IMR_OVRE_Msk                     (_U_(0x1) << US_IMR_OVRE_Pos)                  /**< (US_IMR) Overrun Error Interrupt Mask Mask */
#define US_IMR_OVRE                         US_IMR_OVRE_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_OVRE_Msk instead */
#define US_IMR_TXEMPTY_Pos                  9                                              /**< (US_IMR) TXEMPTY Interrupt Mask Position */
#define US_IMR_TXEMPTY_Msk                  (_U_(0x1) << US_IMR_TXEMPTY_Pos)               /**< (US_IMR) TXEMPTY Interrupt Mask Mask */
#define US_IMR_TXEMPTY                      US_IMR_TXEMPTY_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_TXEMPTY_Msk instead */
#define US_IMR_RIIC_Pos                     16                                             /**< (US_IMR) Ring Indicator Input Change Mask Position */
#define US_IMR_RIIC_Msk                     (_U_(0x1) << US_IMR_RIIC_Pos)                  /**< (US_IMR) Ring Indicator Input Change Mask Mask */
#define US_IMR_RIIC                         US_IMR_RIIC_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_RIIC_Msk instead */
#define US_IMR_DSRIC_Pos                    17                                             /**< (US_IMR) Data Set Ready Input Change Mask Position */
#define US_IMR_DSRIC_Msk                    (_U_(0x1) << US_IMR_DSRIC_Pos)                 /**< (US_IMR) Data Set Ready Input Change Mask Mask */
#define US_IMR_DSRIC                        US_IMR_DSRIC_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_DSRIC_Msk instead */
#define US_IMR_DCDIC_Pos                    18                                             /**< (US_IMR) Data Carrier Detect Input Change Interrupt Mask Position */
#define US_IMR_DCDIC_Msk                    (_U_(0x1) << US_IMR_DCDIC_Pos)                 /**< (US_IMR) Data Carrier Detect Input Change Interrupt Mask Mask */
#define US_IMR_DCDIC                        US_IMR_DCDIC_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_DCDIC_Msk instead */
#define US_IMR_MASK                         _U_(0x70223)                                   /**< \deprecated (US_IMR) Register MASK  (Use US_IMR_Msk instead)  */
#define US_IMR_Msk                          _U_(0x70223)                                   /**< (US_IMR) Register Mask  */

/* USART mode */
#define US_IMR_USART_RXBRK_Pos              2                                              /**< (US_IMR) Receiver Break Interrupt Mask Position */
#define US_IMR_USART_RXBRK_Msk              (_U_(0x1) << US_IMR_USART_RXBRK_Pos)           /**< (US_IMR) Receiver Break Interrupt Mask Mask */
#define US_IMR_USART_RXBRK                  US_IMR_USART_RXBRK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_RXBRK_Msk instead */
#define US_IMR_USART_ITER_Pos               10                                             /**< (US_IMR) Max Number of Repetitions Reached Interrupt Mask Position */
#define US_IMR_USART_ITER_Msk               (_U_(0x1) << US_IMR_USART_ITER_Pos)            /**< (US_IMR) Max Number of Repetitions Reached Interrupt Mask Mask */
#define US_IMR_USART_ITER                   US_IMR_USART_ITER_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_ITER_Msk instead */
#define US_IMR_USART_NACK_Pos               13                                             /**< (US_IMR) Non Acknowledge Interrupt Mask Position */
#define US_IMR_USART_NACK_Msk               (_U_(0x1) << US_IMR_USART_NACK_Pos)            /**< (US_IMR) Non Acknowledge Interrupt Mask Mask */
#define US_IMR_USART_NACK                   US_IMR_USART_NACK_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_NACK_Msk instead */
#define US_IMR_USART_CTSIC_Pos              19                                             /**< (US_IMR) Clear to Send Input Change Interrupt Mask Position */
#define US_IMR_USART_CTSIC_Msk              (_U_(0x1) << US_IMR_USART_CTSIC_Pos)           /**< (US_IMR) Clear to Send Input Change Interrupt Mask Mask */
#define US_IMR_USART_CTSIC                  US_IMR_USART_CTSIC_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_CTSIC_Msk instead */
#define US_IMR_USART_MANE_Pos               24                                             /**< (US_IMR) Manchester Error Interrupt Mask Position */
#define US_IMR_USART_MANE_Msk               (_U_(0x1) << US_IMR_USART_MANE_Pos)            /**< (US_IMR) Manchester Error Interrupt Mask Mask */
#define US_IMR_USART_MANE                   US_IMR_USART_MANE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_MANE_Msk instead */
#define US_IMR_USART_MASK                   _U_(0x1082404)                                 /**< \deprecated (US_IMR_USART) Register MASK  (Use US_IMR_USART_Msk instead)  */
#define US_IMR_USART_Msk                    _U_(0x1082404)                                 /**< (US_IMR_USART) Register Mask  */

/* USART_LIN mode */
#define US_IMR_USART_LIN_FRAME_Pos          6                                              /**< (US_IMR) Framing Error Interrupt Mask Position */
#define US_IMR_USART_LIN_FRAME_Msk          (_U_(0x1) << US_IMR_USART_LIN_FRAME_Pos)       /**< (US_IMR) Framing Error Interrupt Mask Mask */
#define US_IMR_USART_LIN_FRAME              US_IMR_USART_LIN_FRAME_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_LIN_FRAME_Msk instead */
#define US_IMR_USART_LIN_PARE_Pos           7                                              /**< (US_IMR) Parity Error Interrupt Mask Position */
#define US_IMR_USART_LIN_PARE_Msk           (_U_(0x1) << US_IMR_USART_LIN_PARE_Pos)        /**< (US_IMR) Parity Error Interrupt Mask Mask */
#define US_IMR_USART_LIN_PARE               US_IMR_USART_LIN_PARE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_LIN_PARE_Msk instead */
#define US_IMR_USART_LIN_TIMEOUT_Pos        8                                              /**< (US_IMR) Timeout Interrupt Mask Position */
#define US_IMR_USART_LIN_TIMEOUT_Msk        (_U_(0x1) << US_IMR_USART_LIN_TIMEOUT_Pos)     /**< (US_IMR) Timeout Interrupt Mask Mask */
#define US_IMR_USART_LIN_TIMEOUT            US_IMR_USART_LIN_TIMEOUT_Msk                   /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_USART_LIN_TIMEOUT_Msk instead */
#define US_IMR_USART_LIN_MASK               _U_(0x1C0)                                     /**< \deprecated (US_IMR_USART_LIN) Register MASK  (Use US_IMR_USART_LIN_Msk instead)  */
#define US_IMR_USART_LIN_Msk                _U_(0x1C0)                                     /**< (US_IMR_USART_LIN) Register Mask  */

/* SPI mode */
#define US_IMR_SPI_NSSE_Pos                 19                                             /**< (US_IMR) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Position */
#define US_IMR_SPI_NSSE_Msk                 (_U_(0x1) << US_IMR_SPI_NSSE_Pos)              /**< (US_IMR) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Mask */
#define US_IMR_SPI_NSSE                     US_IMR_SPI_NSSE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_SPI_NSSE_Msk instead */
#define US_IMR_SPI_MASK                     _U_(0x80000)                                   /**< \deprecated (US_IMR_SPI) Register MASK  (Use US_IMR_SPI_Msk instead)  */
#define US_IMR_SPI_Msk                      _U_(0x80000)                                   /**< (US_IMR_SPI) Register Mask  */

/* LIN mode */
#define US_IMR_LIN_LINBK_Pos                13                                             /**< (US_IMR) LIN Break Sent or LIN Break Received Interrupt Mask Position */
#define US_IMR_LIN_LINBK_Msk                (_U_(0x1) << US_IMR_LIN_LINBK_Pos)             /**< (US_IMR) LIN Break Sent or LIN Break Received Interrupt Mask Mask */
#define US_IMR_LIN_LINBK                    US_IMR_LIN_LINBK_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINBK_Msk instead */
#define US_IMR_LIN_LINID_Pos                14                                             /**< (US_IMR) LIN Identifier Sent or LIN Identifier Received Interrupt Mask Position */
#define US_IMR_LIN_LINID_Msk                (_U_(0x1) << US_IMR_LIN_LINID_Pos)             /**< (US_IMR) LIN Identifier Sent or LIN Identifier Received Interrupt Mask Mask */
#define US_IMR_LIN_LINID                    US_IMR_LIN_LINID_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINID_Msk instead */
#define US_IMR_LIN_LINTC_Pos                15                                             /**< (US_IMR) LIN Transfer Completed Interrupt Mask Position */
#define US_IMR_LIN_LINTC_Msk                (_U_(0x1) << US_IMR_LIN_LINTC_Pos)             /**< (US_IMR) LIN Transfer Completed Interrupt Mask Mask */
#define US_IMR_LIN_LINTC                    US_IMR_LIN_LINTC_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINTC_Msk instead */
#define US_IMR_LIN_LINBE_Pos                25                                             /**< (US_IMR) LIN Bus Error Interrupt Mask Position */
#define US_IMR_LIN_LINBE_Msk                (_U_(0x1) << US_IMR_LIN_LINBE_Pos)             /**< (US_IMR) LIN Bus Error Interrupt Mask Mask */
#define US_IMR_LIN_LINBE                    US_IMR_LIN_LINBE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINBE_Msk instead */
#define US_IMR_LIN_LINISFE_Pos              26                                             /**< (US_IMR) LIN Inconsistent Synch Field Error Interrupt Mask Position */
#define US_IMR_LIN_LINISFE_Msk              (_U_(0x1) << US_IMR_LIN_LINISFE_Pos)           /**< (US_IMR) LIN Inconsistent Synch Field Error Interrupt Mask Mask */
#define US_IMR_LIN_LINISFE                  US_IMR_LIN_LINISFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINISFE_Msk instead */
#define US_IMR_LIN_LINIPE_Pos               27                                             /**< (US_IMR) LIN Identifier Parity Interrupt Mask Position */
#define US_IMR_LIN_LINIPE_Msk               (_U_(0x1) << US_IMR_LIN_LINIPE_Pos)            /**< (US_IMR) LIN Identifier Parity Interrupt Mask Mask */
#define US_IMR_LIN_LINIPE                   US_IMR_LIN_LINIPE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINIPE_Msk instead */
#define US_IMR_LIN_LINCE_Pos                28                                             /**< (US_IMR) LIN Checksum Error Interrupt Mask Position */
#define US_IMR_LIN_LINCE_Msk                (_U_(0x1) << US_IMR_LIN_LINCE_Pos)             /**< (US_IMR) LIN Checksum Error Interrupt Mask Mask */
#define US_IMR_LIN_LINCE                    US_IMR_LIN_LINCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINCE_Msk instead */
#define US_IMR_LIN_LINSNRE_Pos              29                                             /**< (US_IMR) LIN Slave Not Responding Error Interrupt Mask Position */
#define US_IMR_LIN_LINSNRE_Msk              (_U_(0x1) << US_IMR_LIN_LINSNRE_Pos)           /**< (US_IMR) LIN Slave Not Responding Error Interrupt Mask Mask */
#define US_IMR_LIN_LINSNRE                  US_IMR_LIN_LINSNRE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINSNRE_Msk instead */
#define US_IMR_LIN_LINSTE_Pos               30                                             /**< (US_IMR) LIN Synch Tolerance Error Interrupt Mask Position */
#define US_IMR_LIN_LINSTE_Msk               (_U_(0x1) << US_IMR_LIN_LINSTE_Pos)            /**< (US_IMR) LIN Synch Tolerance Error Interrupt Mask Mask */
#define US_IMR_LIN_LINSTE                   US_IMR_LIN_LINSTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINSTE_Msk instead */
#define US_IMR_LIN_LINHTE_Pos               31                                             /**< (US_IMR) LIN Header Timeout Error Interrupt Mask Position */
#define US_IMR_LIN_LINHTE_Msk               (_U_(0x1) << US_IMR_LIN_LINHTE_Pos)            /**< (US_IMR) LIN Header Timeout Error Interrupt Mask Mask */
#define US_IMR_LIN_LINHTE                   US_IMR_LIN_LINHTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LIN_LINHTE_Msk instead */
#define US_IMR_LIN_MASK                     _U_(0xFE00E000)                                /**< \deprecated (US_IMR_LIN) Register MASK  (Use US_IMR_LIN_Msk instead)  */
#define US_IMR_LIN_Msk                      _U_(0xFE00E000)                                /**< (US_IMR_LIN) Register Mask  */

/* LON mode */
#define US_IMR_LON_LSFE_Pos                 6                                              /**< (US_IMR) LON Short Frame Error Interrupt Mask Position */
#define US_IMR_LON_LSFE_Msk                 (_U_(0x1) << US_IMR_LON_LSFE_Pos)              /**< (US_IMR) LON Short Frame Error Interrupt Mask Mask */
#define US_IMR_LON_LSFE                     US_IMR_LON_LSFE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LSFE_Msk instead */
#define US_IMR_LON_LCRCE_Pos                7                                              /**< (US_IMR) LON CRC Error Interrupt Mask Position */
#define US_IMR_LON_LCRCE_Msk                (_U_(0x1) << US_IMR_LON_LCRCE_Pos)             /**< (US_IMR) LON CRC Error Interrupt Mask Mask */
#define US_IMR_LON_LCRCE                    US_IMR_LON_LCRCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LCRCE_Msk instead */
#define US_IMR_LON_LTXD_Pos                 24                                             /**< (US_IMR) LON Transmission Done Interrupt Mask Position */
#define US_IMR_LON_LTXD_Msk                 (_U_(0x1) << US_IMR_LON_LTXD_Pos)              /**< (US_IMR) LON Transmission Done Interrupt Mask Mask */
#define US_IMR_LON_LTXD                     US_IMR_LON_LTXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LTXD_Msk instead */
#define US_IMR_LON_LCOL_Pos                 25                                             /**< (US_IMR) LON Collision Interrupt Mask Position */
#define US_IMR_LON_LCOL_Msk                 (_U_(0x1) << US_IMR_LON_LCOL_Pos)              /**< (US_IMR) LON Collision Interrupt Mask Mask */
#define US_IMR_LON_LCOL                     US_IMR_LON_LCOL_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LCOL_Msk instead */
#define US_IMR_LON_LFET_Pos                 26                                             /**< (US_IMR) LON Frame Early Termination Interrupt Mask Position */
#define US_IMR_LON_LFET_Msk                 (_U_(0x1) << US_IMR_LON_LFET_Pos)              /**< (US_IMR) LON Frame Early Termination Interrupt Mask Mask */
#define US_IMR_LON_LFET                     US_IMR_LON_LFET_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LFET_Msk instead */
#define US_IMR_LON_LRXD_Pos                 27                                             /**< (US_IMR) LON Reception Done Interrupt Mask Position */
#define US_IMR_LON_LRXD_Msk                 (_U_(0x1) << US_IMR_LON_LRXD_Pos)              /**< (US_IMR) LON Reception Done Interrupt Mask Mask */
#define US_IMR_LON_LRXD                     US_IMR_LON_LRXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LRXD_Msk instead */
#define US_IMR_LON_LBLOVFE_Pos              28                                             /**< (US_IMR) LON Backlog Overflow Error Interrupt Mask Position */
#define US_IMR_LON_LBLOVFE_Msk              (_U_(0x1) << US_IMR_LON_LBLOVFE_Pos)           /**< (US_IMR) LON Backlog Overflow Error Interrupt Mask Mask */
#define US_IMR_LON_LBLOVFE                  US_IMR_LON_LBLOVFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_LBLOVFE_Msk instead */
#define US_IMR_LON_MASK                     _U_(0x1F0000C0)                                /**< \deprecated (US_IMR_LON) Register MASK  (Use US_IMR_LON_Msk instead)  */
#define US_IMR_LON_Msk                      _U_(0x1F0000C0)                                /**< (US_IMR_LON) Register Mask  */

/* LON_SPI mode */
#define US_IMR_LON_SPI_UNRE_Pos             10                                             /**< (US_IMR) SPI Underrun Error Interrupt Mask Position */
#define US_IMR_LON_SPI_UNRE_Msk             (_U_(0x1) << US_IMR_LON_SPI_UNRE_Pos)          /**< (US_IMR) SPI Underrun Error Interrupt Mask Mask */
#define US_IMR_LON_SPI_UNRE                 US_IMR_LON_SPI_UNRE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_IMR_LON_SPI_UNRE_Msk instead */
#define US_IMR_LON_SPI_MASK                 _U_(0x400)                                     /**< \deprecated (US_IMR_LON_SPI) Register MASK  (Use US_IMR_LON_SPI_Msk instead)  */
#define US_IMR_LON_SPI_Msk                  _U_(0x400)                                     /**< (US_IMR_LON_SPI) Register Mask  */


/* -------- US_CSR : (USART Offset: 0x14) (R/ 32) Channel Status Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RXRDY:1;                   /**< bit:      0  Receiver Ready (cleared by reading US_RHR) */
    uint32_t TXRDY:1;                   /**< bit:      1  Transmitter Ready (cleared by writing US_THR) */
    uint32_t :3;                        /**< bit:   2..4  Reserved */
    uint32_t OVRE:1;                    /**< bit:      5  Overrun Error (cleared by writing a one to bit US_CR.RSTSTA) */
    uint32_t :3;                        /**< bit:   6..8  Reserved */
    uint32_t TXEMPTY:1;                 /**< bit:      9  Transmitter Empty (cleared by writing US_THR) */
    uint32_t :6;                        /**< bit: 10..15  Reserved */
    uint32_t RIIC:1;                    /**< bit:     16  Ring Indicator Input Change Flag (cleared on read) */
    uint32_t DSRIC:1;                   /**< bit:     17  Data Set Ready Input Change Flag (cleared on read) */
    uint32_t DCDIC:1;                   /**< bit:     18  Data Carrier Detect Input Change Flag (cleared on read) */
    uint32_t :13;                       /**< bit: 19..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  struct { // USART mode
    uint32_t :2;                        /**< bit:   0..1  Reserved */
    uint32_t RXBRK:1;                   /**< bit:      2  Break Received/End of Break (cleared by writing a one to bit US_CR.RSTSTA) */
    uint32_t :7;                        /**< bit:   3..9  Reserved */
    uint32_t ITER:1;                    /**< bit:     10  Max Number of Repetitions Reached (cleared by writing a one to bit US_CR.RSTIT) */
    uint32_t :2;                        /**< bit: 11..12  Reserved */
    uint32_t NACK:1;                    /**< bit:     13  Non Acknowledge Interrupt (cleared by writing a one to bit US_CR.RSTNACK) */
    uint32_t :5;                        /**< bit: 14..18  Reserved */
    uint32_t CTSIC:1;                   /**< bit:     19  Clear to Send Input Change Flag (cleared on read) */
    uint32_t RI:1;                      /**< bit:     20  Image of RI Input                        */
    uint32_t DSR:1;                     /**< bit:     21  Image of DSR Input                       */
    uint32_t DCD:1;                     /**< bit:     22  Image of DCD Input                       */
    uint32_t CTS:1;                     /**< bit:     23  Image of CTS Input                       */
    uint32_t MANERR:1;                  /**< bit:     24  Manchester Error (cleared by writing a one to the bit US_CR.RSTSTA) */
    uint32_t :7;                        /**< bit: 25..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // USART_LIN mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t FRAME:1;                   /**< bit:      6  Framing Error (cleared by writing a one to bit US_CR.RSTSTA) */
    uint32_t PARE:1;                    /**< bit:      7  Parity Error (cleared by writing a one to bit US_CR.RSTSTA) */
    uint32_t TIMEOUT:1;                 /**< bit:      8  Receiver Timeout (cleared by writing a one to bit US_CR.STTTO) */
    uint32_t :23;                       /**< bit:  9..31  Reserved */
  } USART_LIN;                                /**< Structure used for USART_LIN mode access */
  struct { // SPI mode
    uint32_t :19;                       /**< bit:  0..18  Reserved */
    uint32_t NSSE:1;                    /**< bit:     19  NSS Line (Driving CTS Pin) Rising or Falling Edge Event */
    uint32_t :3;                        /**< bit: 20..22  Reserved */
    uint32_t NSS:1;                     /**< bit:     23  Image of NSS Line                        */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } SPI;                                /**< Structure used for SPI mode access */
  struct { // LIN mode
    uint32_t :13;                       /**< bit:  0..12  Reserved */
    uint32_t LINBK:1;                   /**< bit:     13  LIN Break Sent or LIN Break Received     */
    uint32_t LINID:1;                   /**< bit:     14  LIN Identifier Sent or LIN Identifier Received */
    uint32_t LINTC:1;                   /**< bit:     15  LIN Transfer Completed                   */
    uint32_t :7;                        /**< bit: 16..22  Reserved */
    uint32_t LINBLS:1;                  /**< bit:     23  LIN Bus Line Status                      */
    uint32_t :1;                        /**< bit:     24  Reserved */
    uint32_t LINBE:1;                   /**< bit:     25  LIN Bus Error                            */
    uint32_t LINISFE:1;                 /**< bit:     26  LIN Inconsistent Synch Field Error       */
    uint32_t LINIPE:1;                  /**< bit:     27  LIN Identifier Parity Error              */
    uint32_t LINCE:1;                   /**< bit:     28  LIN Checksum Error                       */
    uint32_t LINSNRE:1;                 /**< bit:     29  LIN Slave Not Responding Error Interrupt Mask */
    uint32_t LINSTE:1;                  /**< bit:     30  LIN Synch Tolerance Error                */
    uint32_t LINHTE:1;                  /**< bit:     31  LIN Header Timeout Error                 */
  } LIN;                                /**< Structure used for LIN mode access */
  struct { // LON mode
    uint32_t :6;                        /**< bit:   0..5  Reserved */
    uint32_t LSFE:1;                    /**< bit:      6  LON Short Frame Error                    */
    uint32_t LCRCE:1;                   /**< bit:      7  LON CRC Error                            */
    uint32_t :16;                       /**< bit:  8..23  Reserved */
    uint32_t LTXD:1;                    /**< bit:     24  LON Transmission End Flag                */
    uint32_t LCOL:1;                    /**< bit:     25  LON Collision Detected Flag              */
    uint32_t LFET:1;                    /**< bit:     26  LON Frame Early Termination              */
    uint32_t LRXD:1;                    /**< bit:     27  LON Reception End Flag                   */
    uint32_t LBLOVFE:1;                 /**< bit:     28  LON Backlog Overflow Error               */
    uint32_t :3;                        /**< bit: 29..31  Reserved */
  } LON;                                /**< Structure used for LON mode access */
  struct { // LON_SPI mode
    uint32_t :10;                       /**< bit:   0..9  Reserved */
    uint32_t UNRE:1;                    /**< bit:     10  SPI Underrun Error                       */
    uint32_t :21;                       /**< bit: 11..31  Reserved */
  } LON_SPI;                                /**< Structure used for LON_SPI mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_CSR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_CSR_OFFSET                       (0x14)                                        /**<  (US_CSR) Channel Status Register  Offset */

#define US_CSR_RXRDY_Pos                    0                                              /**< (US_CSR) Receiver Ready (cleared by reading US_RHR) Position */
#define US_CSR_RXRDY_Msk                    (_U_(0x1) << US_CSR_RXRDY_Pos)                 /**< (US_CSR) Receiver Ready (cleared by reading US_RHR) Mask */
#define US_CSR_RXRDY                        US_CSR_RXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_RXRDY_Msk instead */
#define US_CSR_TXRDY_Pos                    1                                              /**< (US_CSR) Transmitter Ready (cleared by writing US_THR) Position */
#define US_CSR_TXRDY_Msk                    (_U_(0x1) << US_CSR_TXRDY_Pos)                 /**< (US_CSR) Transmitter Ready (cleared by writing US_THR) Mask */
#define US_CSR_TXRDY                        US_CSR_TXRDY_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_TXRDY_Msk instead */
#define US_CSR_OVRE_Pos                     5                                              /**< (US_CSR) Overrun Error (cleared by writing a one to bit US_CR.RSTSTA) Position */
#define US_CSR_OVRE_Msk                     (_U_(0x1) << US_CSR_OVRE_Pos)                  /**< (US_CSR) Overrun Error (cleared by writing a one to bit US_CR.RSTSTA) Mask */
#define US_CSR_OVRE                         US_CSR_OVRE_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_OVRE_Msk instead */
#define US_CSR_TXEMPTY_Pos                  9                                              /**< (US_CSR) Transmitter Empty (cleared by writing US_THR) Position */
#define US_CSR_TXEMPTY_Msk                  (_U_(0x1) << US_CSR_TXEMPTY_Pos)               /**< (US_CSR) Transmitter Empty (cleared by writing US_THR) Mask */
#define US_CSR_TXEMPTY                      US_CSR_TXEMPTY_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_TXEMPTY_Msk instead */
#define US_CSR_RIIC_Pos                     16                                             /**< (US_CSR) Ring Indicator Input Change Flag (cleared on read) Position */
#define US_CSR_RIIC_Msk                     (_U_(0x1) << US_CSR_RIIC_Pos)                  /**< (US_CSR) Ring Indicator Input Change Flag (cleared on read) Mask */
#define US_CSR_RIIC                         US_CSR_RIIC_Msk                                /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_RIIC_Msk instead */
#define US_CSR_DSRIC_Pos                    17                                             /**< (US_CSR) Data Set Ready Input Change Flag (cleared on read) Position */
#define US_CSR_DSRIC_Msk                    (_U_(0x1) << US_CSR_DSRIC_Pos)                 /**< (US_CSR) Data Set Ready Input Change Flag (cleared on read) Mask */
#define US_CSR_DSRIC                        US_CSR_DSRIC_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_DSRIC_Msk instead */
#define US_CSR_DCDIC_Pos                    18                                             /**< (US_CSR) Data Carrier Detect Input Change Flag (cleared on read) Position */
#define US_CSR_DCDIC_Msk                    (_U_(0x1) << US_CSR_DCDIC_Pos)                 /**< (US_CSR) Data Carrier Detect Input Change Flag (cleared on read) Mask */
#define US_CSR_DCDIC                        US_CSR_DCDIC_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_DCDIC_Msk instead */
#define US_CSR_MASK                         _U_(0x70223)                                   /**< \deprecated (US_CSR) Register MASK  (Use US_CSR_Msk instead)  */
#define US_CSR_Msk                          _U_(0x70223)                                   /**< (US_CSR) Register Mask  */

/* USART mode */
#define US_CSR_USART_RXBRK_Pos              2                                              /**< (US_CSR) Break Received/End of Break (cleared by writing a one to bit US_CR.RSTSTA) Position */
#define US_CSR_USART_RXBRK_Msk              (_U_(0x1) << US_CSR_USART_RXBRK_Pos)           /**< (US_CSR) Break Received/End of Break (cleared by writing a one to bit US_CR.RSTSTA) Mask */
#define US_CSR_USART_RXBRK                  US_CSR_USART_RXBRK_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_RXBRK_Msk instead */
#define US_CSR_USART_ITER_Pos               10                                             /**< (US_CSR) Max Number of Repetitions Reached (cleared by writing a one to bit US_CR.RSTIT) Position */
#define US_CSR_USART_ITER_Msk               (_U_(0x1) << US_CSR_USART_ITER_Pos)            /**< (US_CSR) Max Number of Repetitions Reached (cleared by writing a one to bit US_CR.RSTIT) Mask */
#define US_CSR_USART_ITER                   US_CSR_USART_ITER_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_ITER_Msk instead */
#define US_CSR_USART_NACK_Pos               13                                             /**< (US_CSR) Non Acknowledge Interrupt (cleared by writing a one to bit US_CR.RSTNACK) Position */
#define US_CSR_USART_NACK_Msk               (_U_(0x1) << US_CSR_USART_NACK_Pos)            /**< (US_CSR) Non Acknowledge Interrupt (cleared by writing a one to bit US_CR.RSTNACK) Mask */
#define US_CSR_USART_NACK                   US_CSR_USART_NACK_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_NACK_Msk instead */
#define US_CSR_USART_CTSIC_Pos              19                                             /**< (US_CSR) Clear to Send Input Change Flag (cleared on read) Position */
#define US_CSR_USART_CTSIC_Msk              (_U_(0x1) << US_CSR_USART_CTSIC_Pos)           /**< (US_CSR) Clear to Send Input Change Flag (cleared on read) Mask */
#define US_CSR_USART_CTSIC                  US_CSR_USART_CTSIC_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_CTSIC_Msk instead */
#define US_CSR_USART_RI_Pos                 20                                             /**< (US_CSR) Image of RI Input Position */
#define US_CSR_USART_RI_Msk                 (_U_(0x1) << US_CSR_USART_RI_Pos)              /**< (US_CSR) Image of RI Input Mask */
#define US_CSR_USART_RI                     US_CSR_USART_RI_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_RI_Msk instead */
#define US_CSR_USART_DSR_Pos                21                                             /**< (US_CSR) Image of DSR Input Position */
#define US_CSR_USART_DSR_Msk                (_U_(0x1) << US_CSR_USART_DSR_Pos)             /**< (US_CSR) Image of DSR Input Mask */
#define US_CSR_USART_DSR                    US_CSR_USART_DSR_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_DSR_Msk instead */
#define US_CSR_USART_DCD_Pos                22                                             /**< (US_CSR) Image of DCD Input Position */
#define US_CSR_USART_DCD_Msk                (_U_(0x1) << US_CSR_USART_DCD_Pos)             /**< (US_CSR) Image of DCD Input Mask */
#define US_CSR_USART_DCD                    US_CSR_USART_DCD_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_DCD_Msk instead */
#define US_CSR_USART_CTS_Pos                23                                             /**< (US_CSR) Image of CTS Input Position */
#define US_CSR_USART_CTS_Msk                (_U_(0x1) << US_CSR_USART_CTS_Pos)             /**< (US_CSR) Image of CTS Input Mask */
#define US_CSR_USART_CTS                    US_CSR_USART_CTS_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_CTS_Msk instead */
#define US_CSR_USART_MANERR_Pos             24                                             /**< (US_CSR) Manchester Error (cleared by writing a one to the bit US_CR.RSTSTA) Position */
#define US_CSR_USART_MANERR_Msk             (_U_(0x1) << US_CSR_USART_MANERR_Pos)          /**< (US_CSR) Manchester Error (cleared by writing a one to the bit US_CR.RSTSTA) Mask */
#define US_CSR_USART_MANERR                 US_CSR_USART_MANERR_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_MANERR_Msk instead */
#define US_CSR_USART_MASK                   _U_(0x1F82404)                                 /**< \deprecated (US_CSR_USART) Register MASK  (Use US_CSR_USART_Msk instead)  */
#define US_CSR_USART_Msk                    _U_(0x1F82404)                                 /**< (US_CSR_USART) Register Mask  */

/* USART_LIN mode */
#define US_CSR_USART_LIN_FRAME_Pos          6                                              /**< (US_CSR) Framing Error (cleared by writing a one to bit US_CR.RSTSTA) Position */
#define US_CSR_USART_LIN_FRAME_Msk          (_U_(0x1) << US_CSR_USART_LIN_FRAME_Pos)       /**< (US_CSR) Framing Error (cleared by writing a one to bit US_CR.RSTSTA) Mask */
#define US_CSR_USART_LIN_FRAME              US_CSR_USART_LIN_FRAME_Msk                     /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_LIN_FRAME_Msk instead */
#define US_CSR_USART_LIN_PARE_Pos           7                                              /**< (US_CSR) Parity Error (cleared by writing a one to bit US_CR.RSTSTA) Position */
#define US_CSR_USART_LIN_PARE_Msk           (_U_(0x1) << US_CSR_USART_LIN_PARE_Pos)        /**< (US_CSR) Parity Error (cleared by writing a one to bit US_CR.RSTSTA) Mask */
#define US_CSR_USART_LIN_PARE               US_CSR_USART_LIN_PARE_Msk                      /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_LIN_PARE_Msk instead */
#define US_CSR_USART_LIN_TIMEOUT_Pos        8                                              /**< (US_CSR) Receiver Timeout (cleared by writing a one to bit US_CR.STTTO) Position */
#define US_CSR_USART_LIN_TIMEOUT_Msk        (_U_(0x1) << US_CSR_USART_LIN_TIMEOUT_Pos)     /**< (US_CSR) Receiver Timeout (cleared by writing a one to bit US_CR.STTTO) Mask */
#define US_CSR_USART_LIN_TIMEOUT            US_CSR_USART_LIN_TIMEOUT_Msk                   /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_USART_LIN_TIMEOUT_Msk instead */
#define US_CSR_USART_LIN_MASK               _U_(0x1C0)                                     /**< \deprecated (US_CSR_USART_LIN) Register MASK  (Use US_CSR_USART_LIN_Msk instead)  */
#define US_CSR_USART_LIN_Msk                _U_(0x1C0)                                     /**< (US_CSR_USART_LIN) Register Mask  */

/* SPI mode */
#define US_CSR_SPI_NSSE_Pos                 19                                             /**< (US_CSR) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Position */
#define US_CSR_SPI_NSSE_Msk                 (_U_(0x1) << US_CSR_SPI_NSSE_Pos)              /**< (US_CSR) NSS Line (Driving CTS Pin) Rising or Falling Edge Event Mask */
#define US_CSR_SPI_NSSE                     US_CSR_SPI_NSSE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_SPI_NSSE_Msk instead */
#define US_CSR_SPI_NSS_Pos                  23                                             /**< (US_CSR) Image of NSS Line Position */
#define US_CSR_SPI_NSS_Msk                  (_U_(0x1) << US_CSR_SPI_NSS_Pos)               /**< (US_CSR) Image of NSS Line Mask */
#define US_CSR_SPI_NSS                      US_CSR_SPI_NSS_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_SPI_NSS_Msk instead */
#define US_CSR_SPI_MASK                     _U_(0x880000)                                  /**< \deprecated (US_CSR_SPI) Register MASK  (Use US_CSR_SPI_Msk instead)  */
#define US_CSR_SPI_Msk                      _U_(0x880000)                                  /**< (US_CSR_SPI) Register Mask  */

/* LIN mode */
#define US_CSR_LIN_LINBK_Pos                13                                             /**< (US_CSR) LIN Break Sent or LIN Break Received Position */
#define US_CSR_LIN_LINBK_Msk                (_U_(0x1) << US_CSR_LIN_LINBK_Pos)             /**< (US_CSR) LIN Break Sent or LIN Break Received Mask */
#define US_CSR_LIN_LINBK                    US_CSR_LIN_LINBK_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINBK_Msk instead */
#define US_CSR_LIN_LINID_Pos                14                                             /**< (US_CSR) LIN Identifier Sent or LIN Identifier Received Position */
#define US_CSR_LIN_LINID_Msk                (_U_(0x1) << US_CSR_LIN_LINID_Pos)             /**< (US_CSR) LIN Identifier Sent or LIN Identifier Received Mask */
#define US_CSR_LIN_LINID                    US_CSR_LIN_LINID_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINID_Msk instead */
#define US_CSR_LIN_LINTC_Pos                15                                             /**< (US_CSR) LIN Transfer Completed Position */
#define US_CSR_LIN_LINTC_Msk                (_U_(0x1) << US_CSR_LIN_LINTC_Pos)             /**< (US_CSR) LIN Transfer Completed Mask */
#define US_CSR_LIN_LINTC                    US_CSR_LIN_LINTC_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINTC_Msk instead */
#define US_CSR_LIN_LINBLS_Pos               23                                             /**< (US_CSR) LIN Bus Line Status Position */
#define US_CSR_LIN_LINBLS_Msk               (_U_(0x1) << US_CSR_LIN_LINBLS_Pos)            /**< (US_CSR) LIN Bus Line Status Mask */
#define US_CSR_LIN_LINBLS                   US_CSR_LIN_LINBLS_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINBLS_Msk instead */
#define US_CSR_LIN_LINBE_Pos                25                                             /**< (US_CSR) LIN Bus Error Position */
#define US_CSR_LIN_LINBE_Msk                (_U_(0x1) << US_CSR_LIN_LINBE_Pos)             /**< (US_CSR) LIN Bus Error Mask */
#define US_CSR_LIN_LINBE                    US_CSR_LIN_LINBE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINBE_Msk instead */
#define US_CSR_LIN_LINISFE_Pos              26                                             /**< (US_CSR) LIN Inconsistent Synch Field Error Position */
#define US_CSR_LIN_LINISFE_Msk              (_U_(0x1) << US_CSR_LIN_LINISFE_Pos)           /**< (US_CSR) LIN Inconsistent Synch Field Error Mask */
#define US_CSR_LIN_LINISFE                  US_CSR_LIN_LINISFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINISFE_Msk instead */
#define US_CSR_LIN_LINIPE_Pos               27                                             /**< (US_CSR) LIN Identifier Parity Error Position */
#define US_CSR_LIN_LINIPE_Msk               (_U_(0x1) << US_CSR_LIN_LINIPE_Pos)            /**< (US_CSR) LIN Identifier Parity Error Mask */
#define US_CSR_LIN_LINIPE                   US_CSR_LIN_LINIPE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINIPE_Msk instead */
#define US_CSR_LIN_LINCE_Pos                28                                             /**< (US_CSR) LIN Checksum Error Position */
#define US_CSR_LIN_LINCE_Msk                (_U_(0x1) << US_CSR_LIN_LINCE_Pos)             /**< (US_CSR) LIN Checksum Error Mask */
#define US_CSR_LIN_LINCE                    US_CSR_LIN_LINCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINCE_Msk instead */
#define US_CSR_LIN_LINSNRE_Pos              29                                             /**< (US_CSR) LIN Slave Not Responding Error Interrupt Mask Position */
#define US_CSR_LIN_LINSNRE_Msk              (_U_(0x1) << US_CSR_LIN_LINSNRE_Pos)           /**< (US_CSR) LIN Slave Not Responding Error Interrupt Mask Mask */
#define US_CSR_LIN_LINSNRE                  US_CSR_LIN_LINSNRE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINSNRE_Msk instead */
#define US_CSR_LIN_LINSTE_Pos               30                                             /**< (US_CSR) LIN Synch Tolerance Error Position */
#define US_CSR_LIN_LINSTE_Msk               (_U_(0x1) << US_CSR_LIN_LINSTE_Pos)            /**< (US_CSR) LIN Synch Tolerance Error Mask */
#define US_CSR_LIN_LINSTE                   US_CSR_LIN_LINSTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINSTE_Msk instead */
#define US_CSR_LIN_LINHTE_Pos               31                                             /**< (US_CSR) LIN Header Timeout Error Position */
#define US_CSR_LIN_LINHTE_Msk               (_U_(0x1) << US_CSR_LIN_LINHTE_Pos)            /**< (US_CSR) LIN Header Timeout Error Mask */
#define US_CSR_LIN_LINHTE                   US_CSR_LIN_LINHTE_Msk                          /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LIN_LINHTE_Msk instead */
#define US_CSR_LIN_MASK                     _U_(0xFE80E000)                                /**< \deprecated (US_CSR_LIN) Register MASK  (Use US_CSR_LIN_Msk instead)  */
#define US_CSR_LIN_Msk                      _U_(0xFE80E000)                                /**< (US_CSR_LIN) Register Mask  */

/* LON mode */
#define US_CSR_LON_LSFE_Pos                 6                                              /**< (US_CSR) LON Short Frame Error Position */
#define US_CSR_LON_LSFE_Msk                 (_U_(0x1) << US_CSR_LON_LSFE_Pos)              /**< (US_CSR) LON Short Frame Error Mask */
#define US_CSR_LON_LSFE                     US_CSR_LON_LSFE_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LSFE_Msk instead */
#define US_CSR_LON_LCRCE_Pos                7                                              /**< (US_CSR) LON CRC Error Position */
#define US_CSR_LON_LCRCE_Msk                (_U_(0x1) << US_CSR_LON_LCRCE_Pos)             /**< (US_CSR) LON CRC Error Mask */
#define US_CSR_LON_LCRCE                    US_CSR_LON_LCRCE_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LCRCE_Msk instead */
#define US_CSR_LON_LTXD_Pos                 24                                             /**< (US_CSR) LON Transmission End Flag Position */
#define US_CSR_LON_LTXD_Msk                 (_U_(0x1) << US_CSR_LON_LTXD_Pos)              /**< (US_CSR) LON Transmission End Flag Mask */
#define US_CSR_LON_LTXD                     US_CSR_LON_LTXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LTXD_Msk instead */
#define US_CSR_LON_LCOL_Pos                 25                                             /**< (US_CSR) LON Collision Detected Flag Position */
#define US_CSR_LON_LCOL_Msk                 (_U_(0x1) << US_CSR_LON_LCOL_Pos)              /**< (US_CSR) LON Collision Detected Flag Mask */
#define US_CSR_LON_LCOL                     US_CSR_LON_LCOL_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LCOL_Msk instead */
#define US_CSR_LON_LFET_Pos                 26                                             /**< (US_CSR) LON Frame Early Termination Position */
#define US_CSR_LON_LFET_Msk                 (_U_(0x1) << US_CSR_LON_LFET_Pos)              /**< (US_CSR) LON Frame Early Termination Mask */
#define US_CSR_LON_LFET                     US_CSR_LON_LFET_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LFET_Msk instead */
#define US_CSR_LON_LRXD_Pos                 27                                             /**< (US_CSR) LON Reception End Flag Position */
#define US_CSR_LON_LRXD_Msk                 (_U_(0x1) << US_CSR_LON_LRXD_Pos)              /**< (US_CSR) LON Reception End Flag Mask */
#define US_CSR_LON_LRXD                     US_CSR_LON_LRXD_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LRXD_Msk instead */
#define US_CSR_LON_LBLOVFE_Pos              28                                             /**< (US_CSR) LON Backlog Overflow Error Position */
#define US_CSR_LON_LBLOVFE_Msk              (_U_(0x1) << US_CSR_LON_LBLOVFE_Pos)           /**< (US_CSR) LON Backlog Overflow Error Mask */
#define US_CSR_LON_LBLOVFE                  US_CSR_LON_LBLOVFE_Msk                         /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_LBLOVFE_Msk instead */
#define US_CSR_LON_MASK                     _U_(0x1F0000C0)                                /**< \deprecated (US_CSR_LON) Register MASK  (Use US_CSR_LON_Msk instead)  */
#define US_CSR_LON_Msk                      _U_(0x1F0000C0)                                /**< (US_CSR_LON) Register Mask  */

/* LON_SPI mode */
#define US_CSR_LON_SPI_UNRE_Pos             10                                             /**< (US_CSR) SPI Underrun Error Position */
#define US_CSR_LON_SPI_UNRE_Msk             (_U_(0x1) << US_CSR_LON_SPI_UNRE_Pos)          /**< (US_CSR) SPI Underrun Error Mask */
#define US_CSR_LON_SPI_UNRE                 US_CSR_LON_SPI_UNRE_Msk                        /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_CSR_LON_SPI_UNRE_Msk instead */
#define US_CSR_LON_SPI_MASK                 _U_(0x400)                                     /**< \deprecated (US_CSR_LON_SPI) Register MASK  (Use US_CSR_LON_SPI_Msk instead)  */
#define US_CSR_LON_SPI_Msk                  _U_(0x400)                                     /**< (US_CSR_LON_SPI) Register Mask  */


/* -------- US_RHR : (USART Offset: 0x18) (R/ 32) Receive Holding Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t RXCHR:9;                   /**< bit:   0..8  Received Character                       */
    uint32_t :6;                        /**< bit:  9..14  Reserved */
    uint32_t RXSYNH:1;                  /**< bit:     15  Received Sync                            */
    uint32_t :16;                       /**< bit: 16..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_RHR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_RHR_OFFSET                       (0x18)                                        /**<  (US_RHR) Receive Holding Register  Offset */

#define US_RHR_RXCHR_Pos                    0                                              /**< (US_RHR) Received Character Position */
#define US_RHR_RXCHR_Msk                    (_U_(0x1FF) << US_RHR_RXCHR_Pos)               /**< (US_RHR) Received Character Mask */
#define US_RHR_RXCHR(value)                 (US_RHR_RXCHR_Msk & ((value) << US_RHR_RXCHR_Pos))
#define US_RHR_RXSYNH_Pos                   15                                             /**< (US_RHR) Received Sync Position */
#define US_RHR_RXSYNH_Msk                   (_U_(0x1) << US_RHR_RXSYNH_Pos)                /**< (US_RHR) Received Sync Mask */
#define US_RHR_RXSYNH                       US_RHR_RXSYNH_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_RHR_RXSYNH_Msk instead */
#define US_RHR_MASK                         _U_(0x81FF)                                    /**< \deprecated (US_RHR) Register MASK  (Use US_RHR_Msk instead)  */
#define US_RHR_Msk                          _U_(0x81FF)                                    /**< (US_RHR) Register Mask  */


/* -------- US_THR : (USART Offset: 0x1c) (/W 32) Transmit Holding Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t TXCHR:9;                   /**< bit:   0..8  Character to be Transmitted              */
    uint32_t :6;                        /**< bit:  9..14  Reserved */
    uint32_t TXSYNH:1;                  /**< bit:     15  Sync Field to be Transmitted             */
    uint32_t :16;                       /**< bit: 16..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_THR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_THR_OFFSET                       (0x1C)                                        /**<  (US_THR) Transmit Holding Register  Offset */

#define US_THR_TXCHR_Pos                    0                                              /**< (US_THR) Character to be Transmitted Position */
#define US_THR_TXCHR_Msk                    (_U_(0x1FF) << US_THR_TXCHR_Pos)               /**< (US_THR) Character to be Transmitted Mask */
#define US_THR_TXCHR(value)                 (US_THR_TXCHR_Msk & ((value) << US_THR_TXCHR_Pos))
#define US_THR_TXSYNH_Pos                   15                                             /**< (US_THR) Sync Field to be Transmitted Position */
#define US_THR_TXSYNH_Msk                   (_U_(0x1) << US_THR_TXSYNH_Pos)                /**< (US_THR) Sync Field to be Transmitted Mask */
#define US_THR_TXSYNH                       US_THR_TXSYNH_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_THR_TXSYNH_Msk instead */
#define US_THR_MASK                         _U_(0x81FF)                                    /**< \deprecated (US_THR) Register MASK  (Use US_THR_Msk instead)  */
#define US_THR_Msk                          _U_(0x81FF)                                    /**< (US_THR) Register Mask  */


/* -------- US_BRGR : (USART Offset: 0x20) (R/W 32) Baud Rate Generator Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t CD:16;                     /**< bit:  0..15  Clock Divider                            */
    uint32_t FP:3;                      /**< bit: 16..18  Fractional Part                          */
    uint32_t :13;                       /**< bit: 19..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_BRGR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_BRGR_OFFSET                      (0x20)                                        /**<  (US_BRGR) Baud Rate Generator Register  Offset */

#define US_BRGR_CD_Pos                      0                                              /**< (US_BRGR) Clock Divider Position */
#define US_BRGR_CD_Msk                      (_U_(0xFFFF) << US_BRGR_CD_Pos)                /**< (US_BRGR) Clock Divider Mask */
#define US_BRGR_CD(value)                   (US_BRGR_CD_Msk & ((value) << US_BRGR_CD_Pos))
#define US_BRGR_FP_Pos                      16                                             /**< (US_BRGR) Fractional Part Position */
#define US_BRGR_FP_Msk                      (_U_(0x7) << US_BRGR_FP_Pos)                   /**< (US_BRGR) Fractional Part Mask */
#define US_BRGR_FP(value)                   (US_BRGR_FP_Msk & ((value) << US_BRGR_FP_Pos))
#define US_BRGR_MASK                        _U_(0x7FFFF)                                   /**< \deprecated (US_BRGR) Register MASK  (Use US_BRGR_Msk instead)  */
#define US_BRGR_Msk                         _U_(0x7FFFF)                                   /**< (US_BRGR) Register Mask  */


/* -------- US_RTOR : (USART Offset: 0x24) (R/W 32) Receiver Timeout Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t TO:17;                     /**< bit:  0..16  Timeout Value                            */
    uint32_t :15;                       /**< bit: 17..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_RTOR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_RTOR_OFFSET                      (0x24)                                        /**<  (US_RTOR) Receiver Timeout Register  Offset */

#define US_RTOR_TO_Pos                      0                                              /**< (US_RTOR) Timeout Value Position */
#define US_RTOR_TO_Msk                      (_U_(0x1FFFF) << US_RTOR_TO_Pos)               /**< (US_RTOR) Timeout Value Mask */
#define US_RTOR_TO(value)                   (US_RTOR_TO_Msk & ((value) << US_RTOR_TO_Pos))
#define US_RTOR_MASK                        _U_(0x1FFFF)                                   /**< \deprecated (US_RTOR) Register MASK  (Use US_RTOR_Msk instead)  */
#define US_RTOR_Msk                         _U_(0x1FFFF)                                   /**< (US_RTOR) Register Mask  */


/* -------- US_TTGR : (USART Offset: 0x28) (R/W 32) Transmitter Timeguard Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct { // USART mode
    uint32_t TG:8;                      /**< bit:   0..7  Timeguard Value                          */
    uint32_t :24;                       /**< bit:  8..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // LON mode
    uint32_t PCYCLE:24;                 /**< bit:  0..23  LON PCYCLE Length                        */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } LON;                                /**< Structure used for LON mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_TTGR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_TTGR_OFFSET                      (0x28)                                        /**<  (US_TTGR) Transmitter Timeguard Register  Offset */

#define US_TTGR_MASK                        _U_(0x00)                                      /**< \deprecated (US_TTGR) Register MASK  (Use US_TTGR_Msk instead)  */
#define US_TTGR_Msk                         _U_(0x00)                                      /**< (US_TTGR) Register Mask  */

/* USART mode */
#define US_TTGR_USART_TG_Pos                0                                              /**< (US_TTGR) Timeguard Value Position */
#define US_TTGR_USART_TG_Msk                (_U_(0xFF) << US_TTGR_USART_TG_Pos)            /**< (US_TTGR) Timeguard Value Mask */
#define US_TTGR_USART_TG(value)             (US_TTGR_USART_TG_Msk & ((value) << US_TTGR_USART_TG_Pos))
#define US_TTGR_USART_MASK                  _U_(0xFF)                                      /**< \deprecated (US_TTGR_USART) Register MASK  (Use US_TTGR_USART_Msk instead)  */
#define US_TTGR_USART_Msk                   _U_(0xFF)                                      /**< (US_TTGR_USART) Register Mask  */

/* LON mode */
#define US_TTGR_LON_PCYCLE_Pos              0                                              /**< (US_TTGR) LON PCYCLE Length Position */
#define US_TTGR_LON_PCYCLE_Msk              (_U_(0xFFFFFF) << US_TTGR_LON_PCYCLE_Pos)      /**< (US_TTGR) LON PCYCLE Length Mask */
#define US_TTGR_LON_PCYCLE(value)           (US_TTGR_LON_PCYCLE_Msk & ((value) << US_TTGR_LON_PCYCLE_Pos))
#define US_TTGR_LON_MASK                    _U_(0xFFFFFF)                                  /**< \deprecated (US_TTGR_LON) Register MASK  (Use US_TTGR_LON_Msk instead)  */
#define US_TTGR_LON_Msk                     _U_(0xFFFFFF)                                  /**< (US_TTGR_LON) Register Mask  */


/* -------- US_FIDI : (USART Offset: 0x40) (R/W 32) FI DI Ratio Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct { // USART mode
    uint32_t FI_DI_RATIO:16;            /**< bit:  0..15  FI Over DI Ratio Value                   */
    uint32_t :16;                       /**< bit: 16..31  Reserved */
  } USART;                                /**< Structure used for USART mode access */
  struct { // LON mode
    uint32_t BETA2:24;                  /**< bit:  0..23  LON BETA2 Length                         */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } LON;                                /**< Structure used for LON mode access */
  uint32_t reg;                         /**< Type used for register access */
} US_FIDI_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_FIDI_OFFSET                      (0x40)                                        /**<  (US_FIDI) FI DI Ratio Register  Offset */

#define US_FIDI_MASK                        _U_(0x00)                                      /**< \deprecated (US_FIDI) Register MASK  (Use US_FIDI_Msk instead)  */
#define US_FIDI_Msk                         _U_(0x00)                                      /**< (US_FIDI) Register Mask  */

/* USART mode */
#define US_FIDI_USART_FI_DI_RATIO_Pos       0                                              /**< (US_FIDI) FI Over DI Ratio Value Position */
#define US_FIDI_USART_FI_DI_RATIO_Msk       (_U_(0xFFFF) << US_FIDI_USART_FI_DI_RATIO_Pos)  /**< (US_FIDI) FI Over DI Ratio Value Mask */
#define US_FIDI_USART_FI_DI_RATIO(value)    (US_FIDI_USART_FI_DI_RATIO_Msk & ((value) << US_FIDI_USART_FI_DI_RATIO_Pos))
#define US_FIDI_USART_MASK                  _U_(0xFFFF)                                    /**< \deprecated (US_FIDI_USART) Register MASK  (Use US_FIDI_USART_Msk instead)  */
#define US_FIDI_USART_Msk                   _U_(0xFFFF)                                    /**< (US_FIDI_USART) Register Mask  */

/* LON mode */
#define US_FIDI_LON_BETA2_Pos               0                                              /**< (US_FIDI) LON BETA2 Length Position */
#define US_FIDI_LON_BETA2_Msk               (_U_(0xFFFFFF) << US_FIDI_LON_BETA2_Pos)       /**< (US_FIDI) LON BETA2 Length Mask */
#define US_FIDI_LON_BETA2(value)            (US_FIDI_LON_BETA2_Msk & ((value) << US_FIDI_LON_BETA2_Pos))
#define US_FIDI_LON_MASK                    _U_(0xFFFFFF)                                  /**< \deprecated (US_FIDI_LON) Register MASK  (Use US_FIDI_LON_Msk instead)  */
#define US_FIDI_LON_Msk                     _U_(0xFFFFFF)                                  /**< (US_FIDI_LON) Register Mask  */


/* -------- US_NER : (USART Offset: 0x44) (R/ 32) Number of Errors Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t NB_ERRORS:8;               /**< bit:   0..7  Number of Errors                         */
    uint32_t :24;                       /**< bit:  8..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_NER_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_NER_OFFSET                       (0x44)                                        /**<  (US_NER) Number of Errors Register  Offset */

#define US_NER_NB_ERRORS_Pos                0                                              /**< (US_NER) Number of Errors Position */
#define US_NER_NB_ERRORS_Msk                (_U_(0xFF) << US_NER_NB_ERRORS_Pos)            /**< (US_NER) Number of Errors Mask */
#define US_NER_NB_ERRORS(value)             (US_NER_NB_ERRORS_Msk & ((value) << US_NER_NB_ERRORS_Pos))
#define US_NER_MASK                         _U_(0xFF)                                      /**< \deprecated (US_NER) Register MASK  (Use US_NER_Msk instead)  */
#define US_NER_Msk                          _U_(0xFF)                                      /**< (US_NER) Register Mask  */


/* -------- US_IF : (USART Offset: 0x4c) (R/W 32) IrDA Filter Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t IRDA_FILTER:8;             /**< bit:   0..7  IrDA Filter                              */
    uint32_t :24;                       /**< bit:  8..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_IF_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_IF_OFFSET                        (0x4C)                                        /**<  (US_IF) IrDA Filter Register  Offset */

#define US_IF_IRDA_FILTER_Pos               0                                              /**< (US_IF) IrDA Filter Position */
#define US_IF_IRDA_FILTER_Msk               (_U_(0xFF) << US_IF_IRDA_FILTER_Pos)           /**< (US_IF) IrDA Filter Mask */
#define US_IF_IRDA_FILTER(value)            (US_IF_IRDA_FILTER_Msk & ((value) << US_IF_IRDA_FILTER_Pos))
#define US_IF_MASK                          _U_(0xFF)                                      /**< \deprecated (US_IF) Register MASK  (Use US_IF_Msk instead)  */
#define US_IF_Msk                           _U_(0xFF)                                      /**< (US_IF) Register Mask  */


/* -------- US_MAN : (USART Offset: 0x50) (R/W 32) Manchester Configuration Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t TX_PL:4;                   /**< bit:   0..3  Transmitter Preamble Length              */
    uint32_t :4;                        /**< bit:   4..7  Reserved */
    uint32_t TX_PP:2;                   /**< bit:   8..9  Transmitter Preamble Pattern             */
    uint32_t :2;                        /**< bit: 10..11  Reserved */
    uint32_t TX_MPOL:1;                 /**< bit:     12  Transmitter Manchester Polarity          */
    uint32_t :3;                        /**< bit: 13..15  Reserved */
    uint32_t RX_PL:4;                   /**< bit: 16..19  Receiver Preamble Length                 */
    uint32_t :4;                        /**< bit: 20..23  Reserved */
    uint32_t RX_PP:2;                   /**< bit: 24..25  Receiver Preamble Pattern detected       */
    uint32_t :2;                        /**< bit: 26..27  Reserved */
    uint32_t RX_MPOL:1;                 /**< bit:     28  Receiver Manchester Polarity             */
    uint32_t ONE:1;                     /**< bit:     29  Must Be Set to 1                         */
    uint32_t DRIFT:1;                   /**< bit:     30  Drift Compensation                       */
    uint32_t RXIDLEV:1;                 /**< bit:     31  Receiver Idle Value                      */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_MAN_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_MAN_OFFSET                       (0x50)                                        /**<  (US_MAN) Manchester Configuration Register  Offset */

#define US_MAN_TX_PL_Pos                    0                                              /**< (US_MAN) Transmitter Preamble Length Position */
#define US_MAN_TX_PL_Msk                    (_U_(0xF) << US_MAN_TX_PL_Pos)                 /**< (US_MAN) Transmitter Preamble Length Mask */
#define US_MAN_TX_PL(value)                 (US_MAN_TX_PL_Msk & ((value) << US_MAN_TX_PL_Pos))
#define US_MAN_TX_PP_Pos                    8                                              /**< (US_MAN) Transmitter Preamble Pattern Position */
#define US_MAN_TX_PP_Msk                    (_U_(0x3) << US_MAN_TX_PP_Pos)                 /**< (US_MAN) Transmitter Preamble Pattern Mask */
#define US_MAN_TX_PP(value)                 (US_MAN_TX_PP_Msk & ((value) << US_MAN_TX_PP_Pos))
#define   US_MAN_TX_PP_ALL_ONE_Val          _U_(0x0)                                       /**< (US_MAN) The preamble is composed of '1's  */
#define   US_MAN_TX_PP_ALL_ZERO_Val         _U_(0x1)                                       /**< (US_MAN) The preamble is composed of '0's  */
#define   US_MAN_TX_PP_ZERO_ONE_Val         _U_(0x2)                                       /**< (US_MAN) The preamble is composed of '01's  */
#define   US_MAN_TX_PP_ONE_ZERO_Val         _U_(0x3)                                       /**< (US_MAN) The preamble is composed of '10's  */
#define US_MAN_TX_PP_ALL_ONE                (US_MAN_TX_PP_ALL_ONE_Val << US_MAN_TX_PP_Pos)  /**< (US_MAN) The preamble is composed of '1's Position  */
#define US_MAN_TX_PP_ALL_ZERO               (US_MAN_TX_PP_ALL_ZERO_Val << US_MAN_TX_PP_Pos)  /**< (US_MAN) The preamble is composed of '0's Position  */
#define US_MAN_TX_PP_ZERO_ONE               (US_MAN_TX_PP_ZERO_ONE_Val << US_MAN_TX_PP_Pos)  /**< (US_MAN) The preamble is composed of '01's Position  */
#define US_MAN_TX_PP_ONE_ZERO               (US_MAN_TX_PP_ONE_ZERO_Val << US_MAN_TX_PP_Pos)  /**< (US_MAN) The preamble is composed of '10's Position  */
#define US_MAN_TX_MPOL_Pos                  12                                             /**< (US_MAN) Transmitter Manchester Polarity Position */
#define US_MAN_TX_MPOL_Msk                  (_U_(0x1) << US_MAN_TX_MPOL_Pos)               /**< (US_MAN) Transmitter Manchester Polarity Mask */
#define US_MAN_TX_MPOL                      US_MAN_TX_MPOL_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MAN_TX_MPOL_Msk instead */
#define US_MAN_RX_PL_Pos                    16                                             /**< (US_MAN) Receiver Preamble Length Position */
#define US_MAN_RX_PL_Msk                    (_U_(0xF) << US_MAN_RX_PL_Pos)                 /**< (US_MAN) Receiver Preamble Length Mask */
#define US_MAN_RX_PL(value)                 (US_MAN_RX_PL_Msk & ((value) << US_MAN_RX_PL_Pos))
#define US_MAN_RX_PP_Pos                    24                                             /**< (US_MAN) Receiver Preamble Pattern detected Position */
#define US_MAN_RX_PP_Msk                    (_U_(0x3) << US_MAN_RX_PP_Pos)                 /**< (US_MAN) Receiver Preamble Pattern detected Mask */
#define US_MAN_RX_PP(value)                 (US_MAN_RX_PP_Msk & ((value) << US_MAN_RX_PP_Pos))
#define   US_MAN_RX_PP_ALL_ONE_Val          _U_(0x0)                                       /**< (US_MAN) The preamble is composed of '1's  */
#define   US_MAN_RX_PP_ALL_ZERO_Val         _U_(0x1)                                       /**< (US_MAN) The preamble is composed of '0's  */
#define   US_MAN_RX_PP_ZERO_ONE_Val         _U_(0x2)                                       /**< (US_MAN) The preamble is composed of '01's  */
#define   US_MAN_RX_PP_ONE_ZERO_Val         _U_(0x3)                                       /**< (US_MAN) The preamble is composed of '10's  */
#define US_MAN_RX_PP_ALL_ONE                (US_MAN_RX_PP_ALL_ONE_Val << US_MAN_RX_PP_Pos)  /**< (US_MAN) The preamble is composed of '1's Position  */
#define US_MAN_RX_PP_ALL_ZERO               (US_MAN_RX_PP_ALL_ZERO_Val << US_MAN_RX_PP_Pos)  /**< (US_MAN) The preamble is composed of '0's Position  */
#define US_MAN_RX_PP_ZERO_ONE               (US_MAN_RX_PP_ZERO_ONE_Val << US_MAN_RX_PP_Pos)  /**< (US_MAN) The preamble is composed of '01's Position  */
#define US_MAN_RX_PP_ONE_ZERO               (US_MAN_RX_PP_ONE_ZERO_Val << US_MAN_RX_PP_Pos)  /**< (US_MAN) The preamble is composed of '10's Position  */
#define US_MAN_RX_MPOL_Pos                  28                                             /**< (US_MAN) Receiver Manchester Polarity Position */
#define US_MAN_RX_MPOL_Msk                  (_U_(0x1) << US_MAN_RX_MPOL_Pos)               /**< (US_MAN) Receiver Manchester Polarity Mask */
#define US_MAN_RX_MPOL                      US_MAN_RX_MPOL_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MAN_RX_MPOL_Msk instead */
#define US_MAN_ONE_Pos                      29                                             /**< (US_MAN) Must Be Set to 1 Position */
#define US_MAN_ONE_Msk                      (_U_(0x1) << US_MAN_ONE_Pos)                   /**< (US_MAN) Must Be Set to 1 Mask */
#define US_MAN_ONE                          US_MAN_ONE_Msk                                 /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MAN_ONE_Msk instead */
#define US_MAN_DRIFT_Pos                    30                                             /**< (US_MAN) Drift Compensation Position */
#define US_MAN_DRIFT_Msk                    (_U_(0x1) << US_MAN_DRIFT_Pos)                 /**< (US_MAN) Drift Compensation Mask */
#define US_MAN_DRIFT                        US_MAN_DRIFT_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MAN_DRIFT_Msk instead */
#define US_MAN_RXIDLEV_Pos                  31                                             /**< (US_MAN) Receiver Idle Value Position */
#define US_MAN_RXIDLEV_Msk                  (_U_(0x1) << US_MAN_RXIDLEV_Pos)               /**< (US_MAN) Receiver Idle Value Mask */
#define US_MAN_RXIDLEV                      US_MAN_RXIDLEV_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_MAN_RXIDLEV_Msk instead */
#define US_MAN_MASK                         _U_(0xF30F130F)                                /**< \deprecated (US_MAN) Register MASK  (Use US_MAN_Msk instead)  */
#define US_MAN_Msk                          _U_(0xF30F130F)                                /**< (US_MAN) Register Mask  */


/* -------- US_LINMR : (USART Offset: 0x54) (R/W 32) LIN Mode Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t NACT:2;                    /**< bit:   0..1  LIN Node Action                          */
    uint32_t PARDIS:1;                  /**< bit:      2  Parity Disable                           */
    uint32_t CHKDIS:1;                  /**< bit:      3  Checksum Disable                         */
    uint32_t CHKTYP:1;                  /**< bit:      4  Checksum Type                            */
    uint32_t DLM:1;                     /**< bit:      5  Data Length Mode                         */
    uint32_t FSDIS:1;                   /**< bit:      6  Frame Slot Mode Disable                  */
    uint32_t WKUPTYP:1;                 /**< bit:      7  Wakeup Signal Type                       */
    uint32_t DLC:8;                     /**< bit:  8..15  Data Length Control                      */
    uint32_t PDCM:1;                    /**< bit:     16  DMAC Mode                                */
    uint32_t SYNCDIS:1;                 /**< bit:     17  Synchronization Disable                  */
    uint32_t :14;                       /**< bit: 18..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LINMR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LINMR_OFFSET                     (0x54)                                        /**<  (US_LINMR) LIN Mode Register  Offset */

#define US_LINMR_NACT_Pos                   0                                              /**< (US_LINMR) LIN Node Action Position */
#define US_LINMR_NACT_Msk                   (_U_(0x3) << US_LINMR_NACT_Pos)                /**< (US_LINMR) LIN Node Action Mask */
#define US_LINMR_NACT(value)                (US_LINMR_NACT_Msk & ((value) << US_LINMR_NACT_Pos))
#define   US_LINMR_NACT_PUBLISH_Val         _U_(0x0)                                       /**< (US_LINMR) The USART transmits the response.  */
#define   US_LINMR_NACT_SUBSCRIBE_Val       _U_(0x1)                                       /**< (US_LINMR) The USART receives the response.  */
#define   US_LINMR_NACT_IGNORE_Val          _U_(0x2)                                       /**< (US_LINMR) The USART does not transmit and does not receive the response.  */
#define US_LINMR_NACT_PUBLISH               (US_LINMR_NACT_PUBLISH_Val << US_LINMR_NACT_Pos)  /**< (US_LINMR) The USART transmits the response. Position  */
#define US_LINMR_NACT_SUBSCRIBE             (US_LINMR_NACT_SUBSCRIBE_Val << US_LINMR_NACT_Pos)  /**< (US_LINMR) The USART receives the response. Position  */
#define US_LINMR_NACT_IGNORE                (US_LINMR_NACT_IGNORE_Val << US_LINMR_NACT_Pos)  /**< (US_LINMR) The USART does not transmit and does not receive the response. Position  */
#define US_LINMR_PARDIS_Pos                 2                                              /**< (US_LINMR) Parity Disable Position */
#define US_LINMR_PARDIS_Msk                 (_U_(0x1) << US_LINMR_PARDIS_Pos)              /**< (US_LINMR) Parity Disable Mask */
#define US_LINMR_PARDIS                     US_LINMR_PARDIS_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_PARDIS_Msk instead */
#define US_LINMR_CHKDIS_Pos                 3                                              /**< (US_LINMR) Checksum Disable Position */
#define US_LINMR_CHKDIS_Msk                 (_U_(0x1) << US_LINMR_CHKDIS_Pos)              /**< (US_LINMR) Checksum Disable Mask */
#define US_LINMR_CHKDIS                     US_LINMR_CHKDIS_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_CHKDIS_Msk instead */
#define US_LINMR_CHKTYP_Pos                 4                                              /**< (US_LINMR) Checksum Type Position */
#define US_LINMR_CHKTYP_Msk                 (_U_(0x1) << US_LINMR_CHKTYP_Pos)              /**< (US_LINMR) Checksum Type Mask */
#define US_LINMR_CHKTYP                     US_LINMR_CHKTYP_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_CHKTYP_Msk instead */
#define US_LINMR_DLM_Pos                    5                                              /**< (US_LINMR) Data Length Mode Position */
#define US_LINMR_DLM_Msk                    (_U_(0x1) << US_LINMR_DLM_Pos)                 /**< (US_LINMR) Data Length Mode Mask */
#define US_LINMR_DLM                        US_LINMR_DLM_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_DLM_Msk instead */
#define US_LINMR_FSDIS_Pos                  6                                              /**< (US_LINMR) Frame Slot Mode Disable Position */
#define US_LINMR_FSDIS_Msk                  (_U_(0x1) << US_LINMR_FSDIS_Pos)               /**< (US_LINMR) Frame Slot Mode Disable Mask */
#define US_LINMR_FSDIS                      US_LINMR_FSDIS_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_FSDIS_Msk instead */
#define US_LINMR_WKUPTYP_Pos                7                                              /**< (US_LINMR) Wakeup Signal Type Position */
#define US_LINMR_WKUPTYP_Msk                (_U_(0x1) << US_LINMR_WKUPTYP_Pos)             /**< (US_LINMR) Wakeup Signal Type Mask */
#define US_LINMR_WKUPTYP                    US_LINMR_WKUPTYP_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_WKUPTYP_Msk instead */
#define US_LINMR_DLC_Pos                    8                                              /**< (US_LINMR) Data Length Control Position */
#define US_LINMR_DLC_Msk                    (_U_(0xFF) << US_LINMR_DLC_Pos)                /**< (US_LINMR) Data Length Control Mask */
#define US_LINMR_DLC(value)                 (US_LINMR_DLC_Msk & ((value) << US_LINMR_DLC_Pos))
#define US_LINMR_PDCM_Pos                   16                                             /**< (US_LINMR) DMAC Mode Position */
#define US_LINMR_PDCM_Msk                   (_U_(0x1) << US_LINMR_PDCM_Pos)                /**< (US_LINMR) DMAC Mode Mask */
#define US_LINMR_PDCM                       US_LINMR_PDCM_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_PDCM_Msk instead */
#define US_LINMR_SYNCDIS_Pos                17                                             /**< (US_LINMR) Synchronization Disable Position */
#define US_LINMR_SYNCDIS_Msk                (_U_(0x1) << US_LINMR_SYNCDIS_Pos)             /**< (US_LINMR) Synchronization Disable Mask */
#define US_LINMR_SYNCDIS                    US_LINMR_SYNCDIS_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LINMR_SYNCDIS_Msk instead */
#define US_LINMR_MASK                       _U_(0x3FFFF)                                   /**< \deprecated (US_LINMR) Register MASK  (Use US_LINMR_Msk instead)  */
#define US_LINMR_Msk                        _U_(0x3FFFF)                                   /**< (US_LINMR) Register Mask  */


/* -------- US_LINIR : (USART Offset: 0x58) (R/W 32) LIN Identifier Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t IDCHR:8;                   /**< bit:   0..7  Identifier Character                     */
    uint32_t :24;                       /**< bit:  8..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LINIR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LINIR_OFFSET                     (0x58)                                        /**<  (US_LINIR) LIN Identifier Register  Offset */

#define US_LINIR_IDCHR_Pos                  0                                              /**< (US_LINIR) Identifier Character Position */
#define US_LINIR_IDCHR_Msk                  (_U_(0xFF) << US_LINIR_IDCHR_Pos)              /**< (US_LINIR) Identifier Character Mask */
#define US_LINIR_IDCHR(value)               (US_LINIR_IDCHR_Msk & ((value) << US_LINIR_IDCHR_Pos))
#define US_LINIR_MASK                       _U_(0xFF)                                      /**< \deprecated (US_LINIR) Register MASK  (Use US_LINIR_Msk instead)  */
#define US_LINIR_Msk                        _U_(0xFF)                                      /**< (US_LINIR) Register Mask  */


/* -------- US_LINBRR : (USART Offset: 0x5c) (R/ 32) LIN Baud Rate Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t LINCD:16;                  /**< bit:  0..15  Clock Divider after Synchronization      */
    uint32_t LINFP:3;                   /**< bit: 16..18  Fractional Part after Synchronization    */
    uint32_t :13;                       /**< bit: 19..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LINBRR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LINBRR_OFFSET                    (0x5C)                                        /**<  (US_LINBRR) LIN Baud Rate Register  Offset */

#define US_LINBRR_LINCD_Pos                 0                                              /**< (US_LINBRR) Clock Divider after Synchronization Position */
#define US_LINBRR_LINCD_Msk                 (_U_(0xFFFF) << US_LINBRR_LINCD_Pos)           /**< (US_LINBRR) Clock Divider after Synchronization Mask */
#define US_LINBRR_LINCD(value)              (US_LINBRR_LINCD_Msk & ((value) << US_LINBRR_LINCD_Pos))
#define US_LINBRR_LINFP_Pos                 16                                             /**< (US_LINBRR) Fractional Part after Synchronization Position */
#define US_LINBRR_LINFP_Msk                 (_U_(0x7) << US_LINBRR_LINFP_Pos)              /**< (US_LINBRR) Fractional Part after Synchronization Mask */
#define US_LINBRR_LINFP(value)              (US_LINBRR_LINFP_Msk & ((value) << US_LINBRR_LINFP_Pos))
#define US_LINBRR_MASK                      _U_(0x7FFFF)                                   /**< \deprecated (US_LINBRR) Register MASK  (Use US_LINBRR_Msk instead)  */
#define US_LINBRR_Msk                       _U_(0x7FFFF)                                   /**< (US_LINBRR) Register Mask  */


/* -------- US_LONMR : (USART Offset: 0x60) (R/W 32) LON Mode Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t COMMT:1;                   /**< bit:      0  LON comm_type Parameter Value            */
    uint32_t COLDET:1;                  /**< bit:      1  LON Collision Detection Feature          */
    uint32_t TCOL:1;                    /**< bit:      2  Terminate Frame upon Collision Notification */
    uint32_t CDTAIL:1;                  /**< bit:      3  LON Collision Detection on Frame Tail    */
    uint32_t DMAM:1;                    /**< bit:      4  LON DMA Mode                             */
    uint32_t LCDS:1;                    /**< bit:      5  LON Collision Detection Source           */
    uint32_t :10;                       /**< bit:  6..15  Reserved */
    uint32_t EOFS:8;                    /**< bit: 16..23  End of Frame Condition Size              */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONMR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONMR_OFFSET                     (0x60)                                        /**<  (US_LONMR) LON Mode Register  Offset */

#define US_LONMR_COMMT_Pos                  0                                              /**< (US_LONMR) LON comm_type Parameter Value Position */
#define US_LONMR_COMMT_Msk                  (_U_(0x1) << US_LONMR_COMMT_Pos)               /**< (US_LONMR) LON comm_type Parameter Value Mask */
#define US_LONMR_COMMT                      US_LONMR_COMMT_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONMR_COMMT_Msk instead */
#define US_LONMR_COLDET_Pos                 1                                              /**< (US_LONMR) LON Collision Detection Feature Position */
#define US_LONMR_COLDET_Msk                 (_U_(0x1) << US_LONMR_COLDET_Pos)              /**< (US_LONMR) LON Collision Detection Feature Mask */
#define US_LONMR_COLDET                     US_LONMR_COLDET_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONMR_COLDET_Msk instead */
#define US_LONMR_TCOL_Pos                   2                                              /**< (US_LONMR) Terminate Frame upon Collision Notification Position */
#define US_LONMR_TCOL_Msk                   (_U_(0x1) << US_LONMR_TCOL_Pos)                /**< (US_LONMR) Terminate Frame upon Collision Notification Mask */
#define US_LONMR_TCOL                       US_LONMR_TCOL_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONMR_TCOL_Msk instead */
#define US_LONMR_CDTAIL_Pos                 3                                              /**< (US_LONMR) LON Collision Detection on Frame Tail Position */
#define US_LONMR_CDTAIL_Msk                 (_U_(0x1) << US_LONMR_CDTAIL_Pos)              /**< (US_LONMR) LON Collision Detection on Frame Tail Mask */
#define US_LONMR_CDTAIL                     US_LONMR_CDTAIL_Msk                            /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONMR_CDTAIL_Msk instead */
#define US_LONMR_DMAM_Pos                   4                                              /**< (US_LONMR) LON DMA Mode Position */
#define US_LONMR_DMAM_Msk                   (_U_(0x1) << US_LONMR_DMAM_Pos)                /**< (US_LONMR) LON DMA Mode Mask */
#define US_LONMR_DMAM                       US_LONMR_DMAM_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONMR_DMAM_Msk instead */
#define US_LONMR_LCDS_Pos                   5                                              /**< (US_LONMR) LON Collision Detection Source Position */
#define US_LONMR_LCDS_Msk                   (_U_(0x1) << US_LONMR_LCDS_Pos)                /**< (US_LONMR) LON Collision Detection Source Mask */
#define US_LONMR_LCDS                       US_LONMR_LCDS_Msk                              /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONMR_LCDS_Msk instead */
#define US_LONMR_EOFS_Pos                   16                                             /**< (US_LONMR) End of Frame Condition Size Position */
#define US_LONMR_EOFS_Msk                   (_U_(0xFF) << US_LONMR_EOFS_Pos)               /**< (US_LONMR) End of Frame Condition Size Mask */
#define US_LONMR_EOFS(value)                (US_LONMR_EOFS_Msk & ((value) << US_LONMR_EOFS_Pos))
#define US_LONMR_MASK                       _U_(0xFF003F)                                  /**< \deprecated (US_LONMR) Register MASK  (Use US_LONMR_Msk instead)  */
#define US_LONMR_Msk                        _U_(0xFF003F)                                  /**< (US_LONMR) Register Mask  */


/* -------- US_LONPR : (USART Offset: 0x64) (R/W 32) LON Preamble Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t LONPL:14;                  /**< bit:  0..13  LON Preamble Length                      */
    uint32_t :18;                       /**< bit: 14..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONPR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONPR_OFFSET                     (0x64)                                        /**<  (US_LONPR) LON Preamble Register  Offset */

#define US_LONPR_LONPL_Pos                  0                                              /**< (US_LONPR) LON Preamble Length Position */
#define US_LONPR_LONPL_Msk                  (_U_(0x3FFF) << US_LONPR_LONPL_Pos)            /**< (US_LONPR) LON Preamble Length Mask */
#define US_LONPR_LONPL(value)               (US_LONPR_LONPL_Msk & ((value) << US_LONPR_LONPL_Pos))
#define US_LONPR_MASK                       _U_(0x3FFF)                                    /**< \deprecated (US_LONPR) Register MASK  (Use US_LONPR_Msk instead)  */
#define US_LONPR_Msk                        _U_(0x3FFF)                                    /**< (US_LONPR) Register Mask  */


/* -------- US_LONDL : (USART Offset: 0x68) (R/W 32) LON Data Length Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t LONDL:8;                   /**< bit:   0..7  LON Data Length                          */
    uint32_t :24;                       /**< bit:  8..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONDL_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONDL_OFFSET                     (0x68)                                        /**<  (US_LONDL) LON Data Length Register  Offset */

#define US_LONDL_LONDL_Pos                  0                                              /**< (US_LONDL) LON Data Length Position */
#define US_LONDL_LONDL_Msk                  (_U_(0xFF) << US_LONDL_LONDL_Pos)              /**< (US_LONDL) LON Data Length Mask */
#define US_LONDL_LONDL(value)               (US_LONDL_LONDL_Msk & ((value) << US_LONDL_LONDL_Pos))
#define US_LONDL_MASK                       _U_(0xFF)                                      /**< \deprecated (US_LONDL) Register MASK  (Use US_LONDL_Msk instead)  */
#define US_LONDL_Msk                        _U_(0xFF)                                      /**< (US_LONDL) Register Mask  */


/* -------- US_LONL2HDR : (USART Offset: 0x6c) (R/W 32) LON L2HDR Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t BLI:6;                     /**< bit:   0..5  LON Backlog Increment                    */
    uint32_t ALTP:1;                    /**< bit:      6  LON Alternate Path Bit                   */
    uint32_t PB:1;                      /**< bit:      7  LON Priority Bit                         */
    uint32_t :24;                       /**< bit:  8..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONL2HDR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONL2HDR_OFFSET                  (0x6C)                                        /**<  (US_LONL2HDR) LON L2HDR Register  Offset */

#define US_LONL2HDR_BLI_Pos                 0                                              /**< (US_LONL2HDR) LON Backlog Increment Position */
#define US_LONL2HDR_BLI_Msk                 (_U_(0x3F) << US_LONL2HDR_BLI_Pos)             /**< (US_LONL2HDR) LON Backlog Increment Mask */
#define US_LONL2HDR_BLI(value)              (US_LONL2HDR_BLI_Msk & ((value) << US_LONL2HDR_BLI_Pos))
#define US_LONL2HDR_ALTP_Pos                6                                              /**< (US_LONL2HDR) LON Alternate Path Bit Position */
#define US_LONL2HDR_ALTP_Msk                (_U_(0x1) << US_LONL2HDR_ALTP_Pos)             /**< (US_LONL2HDR) LON Alternate Path Bit Mask */
#define US_LONL2HDR_ALTP                    US_LONL2HDR_ALTP_Msk                           /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONL2HDR_ALTP_Msk instead */
#define US_LONL2HDR_PB_Pos                  7                                              /**< (US_LONL2HDR) LON Priority Bit Position */
#define US_LONL2HDR_PB_Msk                  (_U_(0x1) << US_LONL2HDR_PB_Pos)               /**< (US_LONL2HDR) LON Priority Bit Mask */
#define US_LONL2HDR_PB                      US_LONL2HDR_PB_Msk                             /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_LONL2HDR_PB_Msk instead */
#define US_LONL2HDR_MASK                    _U_(0xFF)                                      /**< \deprecated (US_LONL2HDR) Register MASK  (Use US_LONL2HDR_Msk instead)  */
#define US_LONL2HDR_Msk                     _U_(0xFF)                                      /**< (US_LONL2HDR) Register Mask  */


/* -------- US_LONBL : (USART Offset: 0x70) (R/ 32) LON Backlog Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t LONBL:6;                   /**< bit:   0..5  LON Node Backlog Value                   */
    uint32_t :26;                       /**< bit:  6..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONBL_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONBL_OFFSET                     (0x70)                                        /**<  (US_LONBL) LON Backlog Register  Offset */

#define US_LONBL_LONBL_Pos                  0                                              /**< (US_LONBL) LON Node Backlog Value Position */
#define US_LONBL_LONBL_Msk                  (_U_(0x3F) << US_LONBL_LONBL_Pos)              /**< (US_LONBL) LON Node Backlog Value Mask */
#define US_LONBL_LONBL(value)               (US_LONBL_LONBL_Msk & ((value) << US_LONBL_LONBL_Pos))
#define US_LONBL_MASK                       _U_(0x3F)                                      /**< \deprecated (US_LONBL) Register MASK  (Use US_LONBL_Msk instead)  */
#define US_LONBL_Msk                        _U_(0x3F)                                      /**< (US_LONBL) Register Mask  */


/* -------- US_LONB1TX : (USART Offset: 0x74) (R/W 32) LON Beta1 Tx Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t BETA1TX:24;                /**< bit:  0..23  LON Beta1 Length after Transmission      */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONB1TX_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONB1TX_OFFSET                   (0x74)                                        /**<  (US_LONB1TX) LON Beta1 Tx Register  Offset */

#define US_LONB1TX_BETA1TX_Pos              0                                              /**< (US_LONB1TX) LON Beta1 Length after Transmission Position */
#define US_LONB1TX_BETA1TX_Msk              (_U_(0xFFFFFF) << US_LONB1TX_BETA1TX_Pos)      /**< (US_LONB1TX) LON Beta1 Length after Transmission Mask */
#define US_LONB1TX_BETA1TX(value)           (US_LONB1TX_BETA1TX_Msk & ((value) << US_LONB1TX_BETA1TX_Pos))
#define US_LONB1TX_MASK                     _U_(0xFFFFFF)                                  /**< \deprecated (US_LONB1TX) Register MASK  (Use US_LONB1TX_Msk instead)  */
#define US_LONB1TX_Msk                      _U_(0xFFFFFF)                                  /**< (US_LONB1TX) Register Mask  */


/* -------- US_LONB1RX : (USART Offset: 0x78) (R/W 32) LON Beta1 Rx Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t BETA1RX:24;                /**< bit:  0..23  LON Beta1 Length after Reception         */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONB1RX_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONB1RX_OFFSET                   (0x78)                                        /**<  (US_LONB1RX) LON Beta1 Rx Register  Offset */

#define US_LONB1RX_BETA1RX_Pos              0                                              /**< (US_LONB1RX) LON Beta1 Length after Reception Position */
#define US_LONB1RX_BETA1RX_Msk              (_U_(0xFFFFFF) << US_LONB1RX_BETA1RX_Pos)      /**< (US_LONB1RX) LON Beta1 Length after Reception Mask */
#define US_LONB1RX_BETA1RX(value)           (US_LONB1RX_BETA1RX_Msk & ((value) << US_LONB1RX_BETA1RX_Pos))
#define US_LONB1RX_MASK                     _U_(0xFFFFFF)                                  /**< \deprecated (US_LONB1RX) Register MASK  (Use US_LONB1RX_Msk instead)  */
#define US_LONB1RX_Msk                      _U_(0xFFFFFF)                                  /**< (US_LONB1RX) Register Mask  */


/* -------- US_LONPRIO : (USART Offset: 0x7c) (R/W 32) LON Priority Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t PSNB:7;                    /**< bit:   0..6  LON Priority Slot Number                 */
    uint32_t :1;                        /**< bit:      7  Reserved */
    uint32_t NPS:7;                     /**< bit:  8..14  LON Node Priority Slot                   */
    uint32_t :17;                       /**< bit: 15..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_LONPRIO_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_LONPRIO_OFFSET                   (0x7C)                                        /**<  (US_LONPRIO) LON Priority Register  Offset */

#define US_LONPRIO_PSNB_Pos                 0                                              /**< (US_LONPRIO) LON Priority Slot Number Position */
#define US_LONPRIO_PSNB_Msk                 (_U_(0x7F) << US_LONPRIO_PSNB_Pos)             /**< (US_LONPRIO) LON Priority Slot Number Mask */
#define US_LONPRIO_PSNB(value)              (US_LONPRIO_PSNB_Msk & ((value) << US_LONPRIO_PSNB_Pos))
#define US_LONPRIO_NPS_Pos                  8                                              /**< (US_LONPRIO) LON Node Priority Slot Position */
#define US_LONPRIO_NPS_Msk                  (_U_(0x7F) << US_LONPRIO_NPS_Pos)              /**< (US_LONPRIO) LON Node Priority Slot Mask */
#define US_LONPRIO_NPS(value)               (US_LONPRIO_NPS_Msk & ((value) << US_LONPRIO_NPS_Pos))
#define US_LONPRIO_MASK                     _U_(0x7F7F)                                    /**< \deprecated (US_LONPRIO) Register MASK  (Use US_LONPRIO_Msk instead)  */
#define US_LONPRIO_Msk                      _U_(0x7F7F)                                    /**< (US_LONPRIO) Register Mask  */


/* -------- US_IDTTX : (USART Offset: 0x80) (R/W 32) LON IDT Tx Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t IDTTX:24;                  /**< bit:  0..23  LON Indeterminate Time after Transmission (comm_type = 1 mode only) */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_IDTTX_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_IDTTX_OFFSET                     (0x80)                                        /**<  (US_IDTTX) LON IDT Tx Register  Offset */

#define US_IDTTX_IDTTX_Pos                  0                                              /**< (US_IDTTX) LON Indeterminate Time after Transmission (comm_type = 1 mode only) Position */
#define US_IDTTX_IDTTX_Msk                  (_U_(0xFFFFFF) << US_IDTTX_IDTTX_Pos)          /**< (US_IDTTX) LON Indeterminate Time after Transmission (comm_type = 1 mode only) Mask */
#define US_IDTTX_IDTTX(value)               (US_IDTTX_IDTTX_Msk & ((value) << US_IDTTX_IDTTX_Pos))
#define US_IDTTX_MASK                       _U_(0xFFFFFF)                                  /**< \deprecated (US_IDTTX) Register MASK  (Use US_IDTTX_Msk instead)  */
#define US_IDTTX_Msk                        _U_(0xFFFFFF)                                  /**< (US_IDTTX) Register Mask  */


/* -------- US_IDTRX : (USART Offset: 0x84) (R/W 32) LON IDT Rx Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t IDTRX:24;                  /**< bit:  0..23  LON Indeterminate Time after Reception (comm_type = 1 mode only) */
    uint32_t :8;                        /**< bit: 24..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_IDTRX_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_IDTRX_OFFSET                     (0x84)                                        /**<  (US_IDTRX) LON IDT Rx Register  Offset */

#define US_IDTRX_IDTRX_Pos                  0                                              /**< (US_IDTRX) LON Indeterminate Time after Reception (comm_type = 1 mode only) Position */
#define US_IDTRX_IDTRX_Msk                  (_U_(0xFFFFFF) << US_IDTRX_IDTRX_Pos)          /**< (US_IDTRX) LON Indeterminate Time after Reception (comm_type = 1 mode only) Mask */
#define US_IDTRX_IDTRX(value)               (US_IDTRX_IDTRX_Msk & ((value) << US_IDTRX_IDTRX_Pos))
#define US_IDTRX_MASK                       _U_(0xFFFFFF)                                  /**< \deprecated (US_IDTRX) Register MASK  (Use US_IDTRX_Msk instead)  */
#define US_IDTRX_Msk                        _U_(0xFFFFFF)                                  /**< (US_IDTRX) Register Mask  */


/* -------- US_ICDIFF : (USART Offset: 0x88) (R/W 32) IC DIFF Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t ICDIFF:4;                  /**< bit:   0..3  IC Differentiator Number                 */
    uint32_t :28;                       /**< bit:  4..31  Reserved */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_ICDIFF_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_ICDIFF_OFFSET                    (0x88)                                        /**<  (US_ICDIFF) IC DIFF Register  Offset */

#define US_ICDIFF_ICDIFF_Pos                0                                              /**< (US_ICDIFF) IC Differentiator Number Position */
#define US_ICDIFF_ICDIFF_Msk                (_U_(0xF) << US_ICDIFF_ICDIFF_Pos)             /**< (US_ICDIFF) IC Differentiator Number Mask */
#define US_ICDIFF_ICDIFF(value)             (US_ICDIFF_ICDIFF_Msk & ((value) << US_ICDIFF_ICDIFF_Pos))
#define US_ICDIFF_MASK                      _U_(0x0F)                                      /**< \deprecated (US_ICDIFF) Register MASK  (Use US_ICDIFF_Msk instead)  */
#define US_ICDIFF_Msk                       _U_(0x0F)                                      /**< (US_ICDIFF) Register Mask  */


/* -------- US_WPMR : (USART Offset: 0xe4) (R/W 32) Write Protection Mode Register -------- */
#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'N'
typedef union { 
  struct {
    uint32_t WPEN:1;                    /**< bit:      0  Write Protection Enable                  */
    uint32_t :7;                        /**< bit:   1..7  Reserved */
    uint32_t WPKEY:24;                  /**< bit:  8..31  Write Protection Key                     */
  } bit;                                /**< Structure used for bit  access */
  uint32_t reg;                         /**< Type used for register access */
} US_WPMR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_WPMR_OFFSET                      (0xE4)                                        /**<  (US_WPMR) Write Protection Mode Register  Offset */

#define US_WPMR_WPEN_Pos                    0                                              /**< (US_WPMR) Write Protection Enable Position */
#define US_WPMR_WPEN_Msk                    (_U_(0x1) << US_WPMR_WPEN_Pos)                 /**< (US_WPMR) Write Protection Enable Mask */
#define US_WPMR_WPEN                        US_WPMR_WPEN_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_WPMR_WPEN_Msk instead */
#define US_WPMR_WPKEY_Pos                   8                                              /**< (US_WPMR) Write Protection Key Position */
#define US_WPMR_WPKEY_Msk                   (_U_(0xFFFFFF) << US_WPMR_WPKEY_Pos)           /**< (US_WPMR) Write Protection Key Mask */
#define US_WPMR_WPKEY(value)                (US_WPMR_WPKEY_Msk & ((value) << US_WPMR_WPKEY_Pos))
#define   US_WPMR_WPKEY_PASSWD_Val          _U_(0x555341)                                  /**< (US_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0.  */
#define US_WPMR_WPKEY_PASSWD                (US_WPMR_WPKEY_PASSWD_Val << US_WPMR_WPKEY_Pos)  /**< (US_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. Position  */
#define US_WPMR_MASK                        _U_(0xFFFFFF01)                                /**< \deprecated (US_WPMR) Register MASK  (Use US_WPMR_Msk instead)  */
#define US_WPMR_Msk                         _U_(0xFFFFFF01)                                /**< (US_WPMR) Register Mask  */


/* -------- US_WPSR : (USART Offset: 0xe8) (R/ 32) Write Protection Status Register -------- */
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
} US_WPSR_Type;
#endif
#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */

#define US_WPSR_OFFSET                      (0xE8)                                        /**<  (US_WPSR) Write Protection Status Register  Offset */

#define US_WPSR_WPVS_Pos                    0                                              /**< (US_WPSR) Write Protection Violation Status Position */
#define US_WPSR_WPVS_Msk                    (_U_(0x1) << US_WPSR_WPVS_Pos)                 /**< (US_WPSR) Write Protection Violation Status Mask */
#define US_WPSR_WPVS                        US_WPSR_WPVS_Msk                               /**< \deprecated Old style mask definition for 1 bit bitfield. Use US_WPSR_WPVS_Msk instead */
#define US_WPSR_WPVSRC_Pos                  8                                              /**< (US_WPSR) Write Protection Violation Source Position */
#define US_WPSR_WPVSRC_Msk                  (_U_(0xFFFF) << US_WPSR_WPVSRC_Pos)            /**< (US_WPSR) Write Protection Violation Source Mask */
#define US_WPSR_WPVSRC(value)               (US_WPSR_WPVSRC_Msk & ((value) << US_WPSR_WPVSRC_Pos))
#define US_WPSR_MASK                        _U_(0xFFFF01)                                  /**< \deprecated (US_WPSR) Register MASK  (Use US_WPSR_Msk instead)  */
#define US_WPSR_Msk                         _U_(0xFFFF01)                                  /**< (US_WPSR) Register Mask  */


#if !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__))
#if COMPONENT_TYPEDEF_STYLE == 'R'
/** \brief USART hardware registers */
typedef struct {  
  __O  uint32_t US_CR;          /**< (USART Offset: 0x00) Control Register */
  __IO uint32_t US_MR;          /**< (USART Offset: 0x04) Mode Register */
  __O  uint32_t US_IER;         /**< (USART Offset: 0x08) Interrupt Enable Register */
  __O  uint32_t US_IDR;         /**< (USART Offset: 0x0C) Interrupt Disable Register */
  __I  uint32_t US_IMR;         /**< (USART Offset: 0x10) Interrupt Mask Register */
  __I  uint32_t US_CSR;         /**< (USART Offset: 0x14) Channel Status Register */
  __I  uint32_t US_RHR;         /**< (USART Offset: 0x18) Receive Holding Register */
  __O  uint32_t US_THR;         /**< (USART Offset: 0x1C) Transmit Holding Register */
  __IO uint32_t US_BRGR;        /**< (USART Offset: 0x20) Baud Rate Generator Register */
  __IO uint32_t US_RTOR;        /**< (USART Offset: 0x24) Receiver Timeout Register */
  __IO uint32_t US_TTGR;        /**< (USART Offset: 0x28) Transmitter Timeguard Register */
  __I  uint8_t                        Reserved1[20];
  __IO uint32_t US_FIDI;        /**< (USART Offset: 0x40) FI DI Ratio Register */
  __I  uint32_t US_NER;         /**< (USART Offset: 0x44) Number of Errors Register */
  __I  uint8_t                        Reserved2[4];
  __IO uint32_t US_IF;          /**< (USART Offset: 0x4C) IrDA Filter Register */
  __IO uint32_t US_MAN;         /**< (USART Offset: 0x50) Manchester Configuration Register */
  __IO uint32_t US_LINMR;       /**< (USART Offset: 0x54) LIN Mode Register */
  __IO uint32_t US_LINIR;       /**< (USART Offset: 0x58) LIN Identifier Register */
  __I  uint32_t US_LINBRR;      /**< (USART Offset: 0x5C) LIN Baud Rate Register */
  __IO uint32_t US_LONMR;       /**< (USART Offset: 0x60) LON Mode Register */
  __IO uint32_t US_LONPR;       /**< (USART Offset: 0x64) LON Preamble Register */
  __IO uint32_t US_LONDL;       /**< (USART Offset: 0x68) LON Data Length Register */
  __IO uint32_t US_LONL2HDR;    /**< (USART Offset: 0x6C) LON L2HDR Register */
  __I  uint32_t US_LONBL;       /**< (USART Offset: 0x70) LON Backlog Register */
  __IO uint32_t US_LONB1TX;     /**< (USART Offset: 0x74) LON Beta1 Tx Register */
  __IO uint32_t US_LONB1RX;     /**< (USART Offset: 0x78) LON Beta1 Rx Register */
  __IO uint32_t US_LONPRIO;     /**< (USART Offset: 0x7C) LON Priority Register */
  __IO uint32_t US_IDTTX;       /**< (USART Offset: 0x80) LON IDT Tx Register */
  __IO uint32_t US_IDTRX;       /**< (USART Offset: 0x84) LON IDT Rx Register */
  __IO uint32_t US_ICDIFF;      /**< (USART Offset: 0x88) IC DIFF Register */
  __I  uint8_t                        Reserved3[88];
  __IO uint32_t US_WPMR;        /**< (USART Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t US_WPSR;        /**< (USART Offset: 0xE8) Write Protection Status Register */
} Usart;

#elif COMPONENT_TYPEDEF_STYLE == 'N'
/** \brief USART hardware registers */
typedef struct {  
  __O  US_CR_Type                     US_CR;          /**< Offset: 0x00 ( /W  32) Control Register */
  __IO US_MR_Type                     US_MR;          /**< Offset: 0x04 (R/W  32) Mode Register */
  __O  US_IER_Type                    US_IER;         /**< Offset: 0x08 ( /W  32) Interrupt Enable Register */
  __O  US_IDR_Type                    US_IDR;         /**< Offset: 0x0C ( /W  32) Interrupt Disable Register */
  __I  US_IMR_Type                    US_IMR;         /**< Offset: 0x10 (R/   32) Interrupt Mask Register */
  __I  US_CSR_Type                    US_CSR;         /**< Offset: 0x14 (R/   32) Channel Status Register */
  __I  US_RHR_Type                    US_RHR;         /**< Offset: 0x18 (R/   32) Receive Holding Register */
  __O  US_THR_Type                    US_THR;         /**< Offset: 0x1C ( /W  32) Transmit Holding Register */
  __IO US_BRGR_Type                   US_BRGR;        /**< Offset: 0x20 (R/W  32) Baud Rate Generator Register */
  __IO US_RTOR_Type                   US_RTOR;        /**< Offset: 0x24 (R/W  32) Receiver Timeout Register */
  __IO US_TTGR_Type                   US_TTGR;        /**< Offset: 0x28 (R/W  32) Transmitter Timeguard Register */
  __I  uint8_t                        Reserved1[20];
  __IO US_FIDI_Type                   US_FIDI;        /**< Offset: 0x40 (R/W  32) FI DI Ratio Register */
  __I  US_NER_Type                    US_NER;         /**< Offset: 0x44 (R/   32) Number of Errors Register */
  __I  uint8_t                        Reserved2[4];
  __IO US_IF_Type                     US_IF;          /**< Offset: 0x4C (R/W  32) IrDA Filter Register */
  __IO US_MAN_Type                    US_MAN;         /**< Offset: 0x50 (R/W  32) Manchester Configuration Register */
  __IO US_LINMR_Type                  US_LINMR;       /**< Offset: 0x54 (R/W  32) LIN Mode Register */
  __IO US_LINIR_Type                  US_LINIR;       /**< Offset: 0x58 (R/W  32) LIN Identifier Register */
  __I  US_LINBRR_Type                 US_LINBRR;      /**< Offset: 0x5C (R/   32) LIN Baud Rate Register */
  __IO US_LONMR_Type                  US_LONMR;       /**< Offset: 0x60 (R/W  32) LON Mode Register */
  __IO US_LONPR_Type                  US_LONPR;       /**< Offset: 0x64 (R/W  32) LON Preamble Register */
  __IO US_LONDL_Type                  US_LONDL;       /**< Offset: 0x68 (R/W  32) LON Data Length Register */
  __IO US_LONL2HDR_Type               US_LONL2HDR;    /**< Offset: 0x6C (R/W  32) LON L2HDR Register */
  __I  US_LONBL_Type                  US_LONBL;       /**< Offset: 0x70 (R/   32) LON Backlog Register */
  __IO US_LONB1TX_Type                US_LONB1TX;     /**< Offset: 0x74 (R/W  32) LON Beta1 Tx Register */
  __IO US_LONB1RX_Type                US_LONB1RX;     /**< Offset: 0x78 (R/W  32) LON Beta1 Rx Register */
  __IO US_LONPRIO_Type                US_LONPRIO;     /**< Offset: 0x7C (R/W  32) LON Priority Register */
  __IO US_IDTTX_Type                  US_IDTTX;       /**< Offset: 0x80 (R/W  32) LON IDT Tx Register */
  __IO US_IDTRX_Type                  US_IDTRX;       /**< Offset: 0x84 (R/W  32) LON IDT Rx Register */
  __IO US_ICDIFF_Type                 US_ICDIFF;      /**< Offset: 0x88 (R/W  32) IC DIFF Register */
  __I  uint8_t                        Reserved3[88];
  __IO US_WPMR_Type                   US_WPMR;        /**< Offset: 0xE4 (R/W  32) Write Protection Mode Register */
  __I  US_WPSR_Type                   US_WPSR;        /**< Offset: 0xE8 (R/   32) Write Protection Status Register */
} Usart;

#else /* COMPONENT_TYPEDEF_STYLE */
#error Unknown component typedef style
#endif /* COMPONENT_TYPEDEF_STYLE */

#endif /* !(defined(__ASSEMBLER__) || defined(__IAR_SYSTEMS_ASM__)) */
/** @}  end of Universal Synchronous Asynchronous Receiver Transmitter */

#if !(defined(DO_NOT_USE_DEPRECATED_MACROS))
#include "deprecated/usart.h"
#endif /* DO_NOT_USE_DEPRECATED_MACROS */
#endif /* _SAME70_USART_COMPONENT_H_ */
