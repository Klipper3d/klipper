/**
 * \file
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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


#ifndef _SAMG51_SUPC_COMPONENT_
#define _SAMG51_SUPC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Supply Controller */
/* ============================================================================= */
/** \addtogroup SAMG51_SUPC Supply Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Supc hardware registers */
typedef struct {
  __O  uint32_t SUPC_CR;      /**< \brief (Supc Offset: 0x00) Supply Controller Control Register */
  __IO uint32_t SUPC_SMMR;    /**< \brief (Supc Offset: 0x04) Supply Controller Supply Monitor Mode Register */
  __IO uint32_t SUPC_MR;      /**< \brief (Supc Offset: 0x08) Supply Controller Mode Register */
  __I  uint32_t Reserved1[2];
  __I  uint32_t SUPC_SR;      /**< \brief (Supc Offset: 0x14) Supply Controller Status Register */
} Supc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SUPC_CR : (SUPC Offset: 0x00) Supply Controller Control Register -------- */
#define SUPC_CR_ZERO (0x1u << 2) /**< \brief (SUPC_CR) Zero */
#define SUPC_CR_XTALSEL (0x1u << 3) /**< \brief (SUPC_CR) Crystal Oscillator Select */
#define   SUPC_CR_XTALSEL_NO_EFFECT (0x0u << 3) /**< \brief (SUPC_CR) No effect. */
#define   SUPC_CR_XTALSEL_CRYSTAL_SEL (0x1u << 3) /**< \brief (SUPC_CR) If KEY is correct, switches the slow clock on the crystal oscillator output. */
#define SUPC_CR_KEY_Pos 24
#define SUPC_CR_KEY_Msk (0xffu << SUPC_CR_KEY_Pos) /**< \brief (SUPC_CR) Password */
#define   SUPC_CR_KEY_PASSWD (0xA5u << 24) /**< \brief (SUPC_CR) Writing any other value in this field aborts the write operation.Always reads as 0. */
/* -------- SUPC_SMMR : (SUPC Offset: 0x04) Supply Controller Supply Monitor Mode Register -------- */
#define SUPC_SMMR_SMTH_Pos 0
#define SUPC_SMMR_SMTH_Msk (0xfu << SUPC_SMMR_SMTH_Pos) /**< \brief (SUPC_SMMR) Supply Monitor Threshold */
#define SUPC_SMMR_SMTH(value) ((SUPC_SMMR_SMTH_Msk & ((value) << SUPC_SMMR_SMTH_Pos)))
#define SUPC_SMMR_SMSMPL_Pos 8
#define SUPC_SMMR_SMSMPL_Msk (0x7u << SUPC_SMMR_SMSMPL_Pos) /**< \brief (SUPC_SMMR) Supply Monitor Sampling Period */
#define   SUPC_SMMR_SMSMPL_SMD (0x0u << 8) /**< \brief (SUPC_SMMR) Supply Monitor disabled */
#define   SUPC_SMMR_SMSMPL_CSM (0x1u << 8) /**< \brief (SUPC_SMMR) Continuous Supply Monitor */
#define   SUPC_SMMR_SMSMPL_32SLCK (0x2u << 8) /**< \brief (SUPC_SMMR) Supply Monitor enables one SLCK period every 32 SLCK periods */
#define   SUPC_SMMR_SMSMPL_256SLCK (0x3u << 8) /**< \brief (SUPC_SMMR) Supply Monitor enables one SLCK period every 256 SLCK periods */
#define   SUPC_SMMR_SMSMPL_2048SLCK (0x4u << 8) /**< \brief (SUPC_SMMR) Supply Monitor enables one SLCK period every 2,048 SLCK periods */
#define SUPC_SMMR_SMRSTEN (0x1u << 12) /**< \brief (SUPC_SMMR) Supply Monitor Reset Enable */
#define   SUPC_SMMR_SMRSTEN_NOT_ENABLE (0x0u << 12) /**< \brief (SUPC_SMMR) The core reset signal "vddcore_nreset" is not affected when a supply monitor detection occurs. */
#define   SUPC_SMMR_SMRSTEN_ENABLE (0x1u << 12) /**< \brief (SUPC_SMMR) The core reset signal, vddcore_nreset is asserted when a supply monitor detection occurs. */
#define SUPC_SMMR_SMIEN (0x1u << 13) /**< \brief (SUPC_SMMR) Supply Monitor Interrupt Enable */
#define   SUPC_SMMR_SMIEN_NOT_ENABLE (0x0u << 13) /**< \brief (SUPC_SMMR) The SUPC interrupt signal is not affected when a supply monitor detection occurs. */
#define   SUPC_SMMR_SMIEN_ENABLE (0x1u << 13) /**< \brief (SUPC_SMMR) The SUPC interrupt signal is asserted when a supply monitor detection occurs. */
/* -------- SUPC_MR : (SUPC Offset: 0x08) Supply Controller Mode Register -------- */
#define SUPC_MR_BODRSTEN (0x1u << 12) /**< \brief (SUPC_MR) POR Core Reset Enable */
#define   SUPC_MR_BODRSTEN_NOT_ENABLE (0x0u << 12) /**< \brief (SUPC_MR) The core reset signal "vddcore_nreset" is not affected when a brownout detection occurs. */
#define   SUPC_MR_BODRSTEN_ENABLE (0x1u << 12) /**< \brief (SUPC_MR) The core reset signal, vddcore_nreset is asserted when a brownout detection occurs. */
#define SUPC_MR_BODDIS (0x1u << 13) /**< \brief (SUPC_MR) POR Core Disable */
#define   SUPC_MR_BODDIS_ENABLE (0x0u << 13) /**< \brief (SUPC_MR) The core brownout detector is enabled. */
#define   SUPC_MR_BODDIS_DISABLE (0x1u << 13) /**< \brief (SUPC_MR) The core brownout detector is disabled. */
#define SUPC_MR_OSCBYPASS (0x1u << 20) /**< \brief (SUPC_MR) Oscillator Bypass */
#define   SUPC_MR_OSCBYPASS_NO_EFFECT (0x0u << 20) /**< \brief (SUPC_MR) No effect. Clock selection depends on XTALSEL value. */
#define   SUPC_MR_OSCBYPASS_BYPASS (0x1u << 20) /**< \brief (SUPC_MR) The 32 kHz crystal oscillator is selected and put in bypass mode. */
#define SUPC_MR_KEY_Pos 24
#define SUPC_MR_KEY_Msk (0xffu << SUPC_MR_KEY_Pos) /**< \brief (SUPC_MR) Password Key */
#define   SUPC_MR_KEY_PASSWD (0xA5u << 24) /**< \brief (SUPC_MR) Writing any other value in this field aborts the write operation.Always reads as 0. */
/* -------- SUPC_SR : (SUPC Offset: 0x14) Supply Controller Status Register -------- */
#define SUPC_SR_BODRSTS (0x1u << 3) /**< \brief (SUPC_SR) Brownout Detector Reset Status */
#define   SUPC_SR_BODRSTS_NO (0x0u << 3) /**< \brief (SUPC_SR) No core brownout rising edge event has been detected since the last read of the SUPC_SR. */
#define   SUPC_SR_BODRSTS_PRESENT (0x1u << 3) /**< \brief (SUPC_SR) At least one brownout output rising edge event has been detected since the last read of the SUPC_SR. */
#define SUPC_SR_SMRSTS (0x1u << 4) /**< \brief (SUPC_SR) Supply Monitor Reset Status */
#define   SUPC_SR_SMRSTS_NO (0x0u << 4) /**< \brief (SUPC_SR) No supply monitor detection has generated a core reset since the last read of the SUPC_SR. */
#define   SUPC_SR_SMRSTS_PRESENT (0x1u << 4) /**< \brief (SUPC_SR) At least one supply monitor detection has generated a core reset since the last read of the SUPC_SR. */
#define SUPC_SR_SMS (0x1u << 5) /**< \brief (SUPC_SR) Supply Monitor Status */
#define   SUPC_SR_SMS_NO (0x0u << 5) /**< \brief (SUPC_SR) No supply monitor detection since the last read of SUPC_SR. */
#define   SUPC_SR_SMS_PRESENT (0x1u << 5) /**< \brief (SUPC_SR) At least one supply monitor detection since the last read of SUPC_SR. */
#define SUPC_SR_SMOS (0x1u << 6) /**< \brief (SUPC_SR) Supply Monitor Output Status */
#define   SUPC_SR_SMOS_HIGH (0x0u << 6) /**< \brief (SUPC_SR) The supply monitor detected VDDIO higher than its threshold at its last measurement. */
#define   SUPC_SR_SMOS_LOW (0x1u << 6) /**< \brief (SUPC_SR) The supply monitor detected VDDIO lower than its threshold at its last measurement. */
#define SUPC_SR_OSCSEL (0x1u << 7) /**< \brief (SUPC_SR) 32-kHz Oscillator Selection Status */
#define   SUPC_SR_OSCSEL_RC (0x0u << 7) /**< \brief (SUPC_SR) The slow clock, SLCK is generated by the embedded 32 kHz RC oscillator. */
#define   SUPC_SR_OSCSEL_CRYST (0x1u << 7) /**< \brief (SUPC_SR) The slow clock, SLCK is generated by the 32 kHz crystal oscillator. */

/*@}*/


#endif /* _SAMG51_SUPC_COMPONENT_ */
