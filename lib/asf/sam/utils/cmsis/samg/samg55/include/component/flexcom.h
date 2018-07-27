/**
 * \file
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

#ifndef _SAMG55_FLEXCOM_COMPONENT_
#define _SAMG55_FLEXCOM_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Flexible Serial Communication */
/* ============================================================================= */
/** \addtogroup SAMG55_FLEXCOM Flexible Serial Communication */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Flexcom hardware registers */
typedef struct {
  __IO uint32_t FLEXCOM_MR;   /**< \brief (Flexcom Offset: 0x0000) FLEXCOM Mode register */
  __I  uint32_t Reserved1[3];
  __I  uint32_t FLEXCOM_RHR;  /**< \brief (Flexcom Offset: 0x0010) FLEXCOM Receive Holding Register */
  __I  uint32_t Reserved2[3];
  __IO uint32_t FLEXCOM_THR;  /**< \brief (Flexcom Offset: 0x0020) FLEXCOM Transmit Holding Register */
} Flexcom;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- FLEXCOM_MR : (FLEXCOM Offset: 0x0000) FLEXCOM Mode register -------- */
#define FLEXCOM_MR_OPMODE_Pos 0
#define FLEXCOM_MR_OPMODE_Msk (0x3u << FLEXCOM_MR_OPMODE_Pos) /**< \brief (FLEXCOM_MR) FLEXCOM Operating Mode */
#define   FLEXCOM_MR_OPMODE_NO_COM (0x0u << 0) /**< \brief (FLEXCOM_MR) No communication */
#define   FLEXCOM_MR_OPMODE_USART (0x1u << 0) /**< \brief (FLEXCOM_MR) All related USART related protocols are selected (RS232, RS485, ISO7816, LIN,)All SPI/TWI related registers are not accessible and have no impact on IOs. */
#define   FLEXCOM_MR_OPMODE_SPI (0x2u << 0) /**< \brief (FLEXCOM_MR) SPI operating mode is selected.All USART/TWI related registers are not accessible and have no impact on IOs. */
#define   FLEXCOM_MR_OPMODE_TWI (0x3u << 0) /**< \brief (FLEXCOM_MR) All related TWI protocols are selected (TWI, SMBUS). All USART/SPI related registers are not accessible and have no impact on IOs. */
/* -------- FLEXCOM_RHR : (FLEXCOM Offset: 0x0010) FLEXCOM Receive Holding Register -------- */
#define FLEXCOM_RHR_RXDATA_Pos 0
#define FLEXCOM_RHR_RXDATA_Msk (0xffffu << FLEXCOM_RHR_RXDATA_Pos) /**< \brief (FLEXCOM_RHR) Receive Data */
/* -------- FLEXCOM_THR : (FLEXCOM Offset: 0x0020) FLEXCOM Transmit Holding Register -------- */
#define FLEXCOM_THR_TXDATA_Pos 0
#define FLEXCOM_THR_TXDATA_Msk (0xffffu << FLEXCOM_THR_TXDATA_Pos) /**< \brief (FLEXCOM_THR) Transmit Data */
#define FLEXCOM_THR_TXDATA(value) ((FLEXCOM_THR_TXDATA_Msk & ((value) << FLEXCOM_THR_TXDATA_Pos)))

/*@}*/


#endif /* _SAMG55_FLEXCOM_COMPONENT_ */
