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
#ifndef _SAM4C_UOTGHS_INSTANCE_
#define _SAM4C_UOTGHS_INSTANCE_

/* ========== Register definition for UOTGHS peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_UOTGHS_DEVCTRL                         (0x40020000U) /**< \brief (UOTGHS) Device General Control Register */
  #define REG_UOTGHS_DEVISR                          (0x40020004U) /**< \brief (UOTGHS) Device Global Interrupt Status Register */
  #define REG_UOTGHS_DEVICR                          (0x40020008U) /**< \brief (UOTGHS) Device Global Interrupt Clear Register */
  #define REG_UOTGHS_DEVIFR                          (0x4002000CU) /**< \brief (UOTGHS) Device Global Interrupt Set Register */
  #define REG_UOTGHS_DEVIMR                          (0x40020010U) /**< \brief (UOTGHS) Device Global Interrupt Mask Register */
  #define REG_UOTGHS_DEVIDR                          (0x40020014U) /**< \brief (UOTGHS) Device Global Interrupt Disable Register */
  #define REG_UOTGHS_DEVIER                          (0x40020018U) /**< \brief (UOTGHS) Device Global Interrupt Enable Register */
  #define REG_UOTGHS_DEVEPT                          (0x4002001CU) /**< \brief (UOTGHS) Device Endpoint Register */
  #define REG_UOTGHS_DEVFNUM                         (0x40020020U) /**< \brief (UOTGHS) Device Frame Number Register */
  #define REG_UOTGHS_DEVEPTCFG                       (0x40020100U) /**< \brief (UOTGHS) Device Endpoint Configuration Register (n = 0) */
  #define REG_UOTGHS_DEVEPTISR                       (0x40020130U) /**< \brief (UOTGHS) Device Endpoint Status Register (n = 0) */
  #define REG_UOTGHS_DEVEPTICR                       (0x40020160U) /**< \brief (UOTGHS) Device Endpoint Clear Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIFR                       (0x40020190U) /**< \brief (UOTGHS) Device Endpoint Set Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIMR                       (0x400201C0U) /**< \brief (UOTGHS) Device Endpoint Mask Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIER                       (0x400201F0U) /**< \brief (UOTGHS) Device Endpoint Enable Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIDR                       (0x40020220U) /**< \brief (UOTGHS) Device Endpoint Disable Register (n = 0) */
  #define REG_UOTGHS_DEVDMANXTDSC1                   (0x40020310U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 1) */
  #define REG_UOTGHS_DEVDMAADDRESS1                  (0x40020314U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 1) */
  #define REG_UOTGHS_DEVDMACONTROL1                  (0x40020318U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 1) */
  #define REG_UOTGHS_DEVDMASTATUS1                   (0x4002031CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 1) */
  #define REG_UOTGHS_DEVDMANXTDSC2                   (0x40020320U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 2) */
  #define REG_UOTGHS_DEVDMAADDRESS2                  (0x40020324U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 2) */
  #define REG_UOTGHS_DEVDMACONTROL2                  (0x40020328U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 2) */
  #define REG_UOTGHS_DEVDMASTATUS2                   (0x4002032CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 2) */
  #define REG_UOTGHS_DEVDMANXTDSC3                   (0x40020330U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 3) */
  #define REG_UOTGHS_DEVDMAADDRESS3                  (0x40020334U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 3) */
  #define REG_UOTGHS_DEVDMACONTROL3                  (0x40020338U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 3) */
  #define REG_UOTGHS_DEVDMASTATUS3                   (0x4002033CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 3) */
  #define REG_UOTGHS_DEVDMANXTDSC4                   (0x40020340U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 4) */
  #define REG_UOTGHS_DEVDMAADDRESS4                  (0x40020344U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 4) */
  #define REG_UOTGHS_DEVDMACONTROL4                  (0x40020348U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 4) */
  #define REG_UOTGHS_DEVDMASTATUS4                   (0x4002034CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 4) */
  #define REG_UOTGHS_HSTCTRL                         (0x40020400U) /**< \brief (UOTGHS) Host General Control Register */
  #define REG_UOTGHS_HSTISR                          (0x40020404U) /**< \brief (UOTGHS) Host Global Interrupt Status Register */
  #define REG_UOTGHS_HSTICR                          (0x40020408U) /**< \brief (UOTGHS) Host Global Interrupt Clear Register */
  #define REG_UOTGHS_HSTIFR                          (0x4002040CU) /**< \brief (UOTGHS) Host Global Interrupt Set Register */
  #define REG_UOTGHS_HSTIMR                          (0x40020410U) /**< \brief (UOTGHS) Host Global Interrupt Mask Register */
  #define REG_UOTGHS_HSTIDR                          (0x40020414U) /**< \brief (UOTGHS) Host Global Interrupt Disable Register */
  #define REG_UOTGHS_HSTIER                          (0x40020418U) /**< \brief (UOTGHS) Host Global Interrupt Enable Register */
  #define REG_UOTGHS_HSTPIP                          (0x4002041CU) /**< \brief (UOTGHS) Host Pipe Register */
  #define REG_UOTGHS_HSTFNUM                         (0x40020420U) /**< \brief (UOTGHS) Host Frame Number Register */
  #define REG_UOTGHS_HSTADDR1                        (0x40020424U) /**< \brief (UOTGHS) Host Address 1 Register */
  #define REG_UOTGHS_HSTADDR2                        (0x40020428U) /**< \brief (UOTGHS) Host Address 2 Register */
  #define REG_UOTGHS_HSTADDR3                        (0x4002042CU) /**< \brief (UOTGHS) Host Address 3 Register */
  #define REG_UOTGHS_HSTPIPCFG                       (0x40020500U) /**< \brief (UOTGHS) Host Pipe Configuration Register (n = 0) */
  #define REG_UOTGHS_HSTPIPISR                       (0x40020530U) /**< \brief (UOTGHS) Host Pipe Status Register (n = 0) */
  #define REG_UOTGHS_HSTPIPICR                       (0x40020560U) /**< \brief (UOTGHS) Host Pipe Clear Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIFR                       (0x40020590U) /**< \brief (UOTGHS) Host Pipe Set Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIMR                       (0x400205C0U) /**< \brief (UOTGHS) Host Pipe Mask Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIER                       (0x400205F0U) /**< \brief (UOTGHS) Host Pipe Enable Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIDR                       (0x40020620U) /**< \brief (UOTGHS) Host Pipe Disable Register (n = 0) */
  #define REG_UOTGHS_HSTPIPINRQ                      (0x40020650U) /**< \brief (UOTGHS) Host Pipe IN Request Register (n = 0) */
  #define REG_UOTGHS_HSTPIPERR                       (0x40020680U) /**< \brief (UOTGHS) Host Pipe Error Register (n = 0) */
  #define REG_UOTGHS_HSTDMANXTDSC1                   (0x40020710U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 1) */
  #define REG_UOTGHS_HSTDMAADDRESS1                  (0x40020714U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 1) */
  #define REG_UOTGHS_HSTDMACONTROL1                  (0x40020718U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 1) */
  #define REG_UOTGHS_HSTDMASTATUS1                   (0x4002071CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 1) */
  #define REG_UOTGHS_HSTDMANXTDSC2                   (0x40020720U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 2) */
  #define REG_UOTGHS_HSTDMAADDRESS2                  (0x40020724U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 2) */
  #define REG_UOTGHS_HSTDMACONTROL2                  (0x40020728U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 2) */
  #define REG_UOTGHS_HSTDMASTATUS2                   (0x4002072CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 2) */
  #define REG_UOTGHS_HSTDMANXTDSC3                   (0x40020730U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 3) */
  #define REG_UOTGHS_HSTDMAADDRESS3                  (0x40020734U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 3) */
  #define REG_UOTGHS_HSTDMACONTROL3                  (0x40020738U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 3) */
  #define REG_UOTGHS_HSTDMASTATUS3                   (0x4002073CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 3) */
  #define REG_UOTGHS_HSTDMANXTDSC4                   (0x40020740U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 4) */
  #define REG_UOTGHS_HSTDMAADDRESS4                  (0x40020744U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 4) */
  #define REG_UOTGHS_HSTDMACONTROL4                  (0x40020748U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 4) */
  #define REG_UOTGHS_HSTDMASTATUS4                   (0x4002074CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 4) */
  #define REG_UOTGHS_CTRL                            (0x40020800U) /**< \brief (UOTGHS) General Control Register */
  #define REG_UOTGHS_SR                              (0x40020804U) /**< \brief (UOTGHS) General Status Register */
  #define REG_UOTGHS_SCR                             (0x40020808U) /**< \brief (UOTGHS) General Status Clear Register */
  #define REG_UOTGHS_SFR                             (0x4002080CU) /**< \brief (UOTGHS) General Status Set Register */
  #define REG_UOTGHS_FSM                             (0x4002082CU) /**< \brief (UOTGHS) General Finite State Machine Register */
#else
  #define REG_UOTGHS_DEVCTRL        (*(__IO uint32_t*)0x40020000U) /**< \brief (UOTGHS) Device General Control Register */
  #define REG_UOTGHS_DEVISR         (*(__I  uint32_t*)0x40020004U) /**< \brief (UOTGHS) Device Global Interrupt Status Register */
  #define REG_UOTGHS_DEVICR         (*(__O  uint32_t*)0x40020008U) /**< \brief (UOTGHS) Device Global Interrupt Clear Register */
  #define REG_UOTGHS_DEVIFR         (*(__O  uint32_t*)0x4002000CU) /**< \brief (UOTGHS) Device Global Interrupt Set Register */
  #define REG_UOTGHS_DEVIMR         (*(__I  uint32_t*)0x40020010U) /**< \brief (UOTGHS) Device Global Interrupt Mask Register */
  #define REG_UOTGHS_DEVIDR         (*(__O  uint32_t*)0x40020014U) /**< \brief (UOTGHS) Device Global Interrupt Disable Register */
  #define REG_UOTGHS_DEVIER         (*(__O  uint32_t*)0x40020018U) /**< \brief (UOTGHS) Device Global Interrupt Enable Register */
  #define REG_UOTGHS_DEVEPT         (*(__IO uint32_t*)0x4002001CU) /**< \brief (UOTGHS) Device Endpoint Register */
  #define REG_UOTGHS_DEVFNUM        (*(__I  uint32_t*)0x40020020U) /**< \brief (UOTGHS) Device Frame Number Register */
  #define REG_UOTGHS_DEVEPTCFG      (*(__IO uint32_t*)0x40020100U) /**< \brief (UOTGHS) Device Endpoint Configuration Register (n = 0) */
  #define REG_UOTGHS_DEVEPTISR      (*(__I  uint32_t*)0x40020130U) /**< \brief (UOTGHS) Device Endpoint Status Register (n = 0) */
  #define REG_UOTGHS_DEVEPTICR      (*(__O  uint32_t*)0x40020160U) /**< \brief (UOTGHS) Device Endpoint Clear Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIFR      (*(__O  uint32_t*)0x40020190U) /**< \brief (UOTGHS) Device Endpoint Set Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIMR      (*(__I  uint32_t*)0x400201C0U) /**< \brief (UOTGHS) Device Endpoint Mask Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIER      (*(__O  uint32_t*)0x400201F0U) /**< \brief (UOTGHS) Device Endpoint Enable Register (n = 0) */
  #define REG_UOTGHS_DEVEPTIDR      (*(__O  uint32_t*)0x40020220U) /**< \brief (UOTGHS) Device Endpoint Disable Register (n = 0) */
  #define REG_UOTGHS_DEVDMANXTDSC1  (*(__IO uint32_t*)0x40020310U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 1) */
  #define REG_UOTGHS_DEVDMAADDRESS1 (*(__IO uint32_t*)0x40020314U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 1) */
  #define REG_UOTGHS_DEVDMACONTROL1 (*(__IO uint32_t*)0x40020318U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 1) */
  #define REG_UOTGHS_DEVDMASTATUS1  (*(__IO uint32_t*)0x4002031CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 1) */
  #define REG_UOTGHS_DEVDMANXTDSC2  (*(__IO uint32_t*)0x40020320U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 2) */
  #define REG_UOTGHS_DEVDMAADDRESS2 (*(__IO uint32_t*)0x40020324U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 2) */
  #define REG_UOTGHS_DEVDMACONTROL2 (*(__IO uint32_t*)0x40020328U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 2) */
  #define REG_UOTGHS_DEVDMASTATUS2  (*(__IO uint32_t*)0x4002032CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 2) */
  #define REG_UOTGHS_DEVDMANXTDSC3  (*(__IO uint32_t*)0x40020330U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 3) */
  #define REG_UOTGHS_DEVDMAADDRESS3 (*(__IO uint32_t*)0x40020334U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 3) */
  #define REG_UOTGHS_DEVDMACONTROL3 (*(__IO uint32_t*)0x40020338U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 3) */
  #define REG_UOTGHS_DEVDMASTATUS3  (*(__IO uint32_t*)0x4002033CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 3) */
  #define REG_UOTGHS_DEVDMANXTDSC4  (*(__IO uint32_t*)0x40020340U) /**< \brief (UOTGHS) Device DMA Channel Next Descriptor Address Register (n = 4) */
  #define REG_UOTGHS_DEVDMAADDRESS4 (*(__IO uint32_t*)0x40020344U) /**< \brief (UOTGHS) Device DMA Channel Address Register (n = 4) */
  #define REG_UOTGHS_DEVDMACONTROL4 (*(__IO uint32_t*)0x40020348U) /**< \brief (UOTGHS) Device DMA Channel Control Register (n = 4) */
  #define REG_UOTGHS_DEVDMASTATUS4  (*(__IO uint32_t*)0x4002034CU) /**< \brief (UOTGHS) Device DMA Channel Status Register (n = 4) */
  #define REG_UOTGHS_HSTCTRL        (*(__IO uint32_t*)0x40020400U) /**< \brief (UOTGHS) Host General Control Register */
  #define REG_UOTGHS_HSTISR         (*(__I  uint32_t*)0x40020404U) /**< \brief (UOTGHS) Host Global Interrupt Status Register */
  #define REG_UOTGHS_HSTICR         (*(__O  uint32_t*)0x40020408U) /**< \brief (UOTGHS) Host Global Interrupt Clear Register */
  #define REG_UOTGHS_HSTIFR         (*(__O  uint32_t*)0x4002040CU) /**< \brief (UOTGHS) Host Global Interrupt Set Register */
  #define REG_UOTGHS_HSTIMR         (*(__I  uint32_t*)0x40020410U) /**< \brief (UOTGHS) Host Global Interrupt Mask Register */
  #define REG_UOTGHS_HSTIDR         (*(__O  uint32_t*)0x40020414U) /**< \brief (UOTGHS) Host Global Interrupt Disable Register */
  #define REG_UOTGHS_HSTIER         (*(__O  uint32_t*)0x40020418U) /**< \brief (UOTGHS) Host Global Interrupt Enable Register */
  #define REG_UOTGHS_HSTPIP         (*(__IO uint32_t*)0x4002041CU) /**< \brief (UOTGHS) Host Pipe Register */
  #define REG_UOTGHS_HSTFNUM        (*(__IO uint32_t*)0x40020420U) /**< \brief (UOTGHS) Host Frame Number Register */
  #define REG_UOTGHS_HSTADDR1       (*(__IO uint32_t*)0x40020424U) /**< \brief (UOTGHS) Host Address 1 Register */
  #define REG_UOTGHS_HSTADDR2       (*(__IO uint32_t*)0x40020428U) /**< \brief (UOTGHS) Host Address 2 Register */
  #define REG_UOTGHS_HSTADDR3       (*(__IO uint32_t*)0x4002042CU) /**< \brief (UOTGHS) Host Address 3 Register */
  #define REG_UOTGHS_HSTPIPCFG      (*(__IO uint32_t*)0x40020500U) /**< \brief (UOTGHS) Host Pipe Configuration Register (n = 0) */
  #define REG_UOTGHS_HSTPIPISR      (*(__I  uint32_t*)0x40020530U) /**< \brief (UOTGHS) Host Pipe Status Register (n = 0) */
  #define REG_UOTGHS_HSTPIPICR      (*(__O  uint32_t*)0x40020560U) /**< \brief (UOTGHS) Host Pipe Clear Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIFR      (*(__O  uint32_t*)0x40020590U) /**< \brief (UOTGHS) Host Pipe Set Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIMR      (*(__I  uint32_t*)0x400205C0U) /**< \brief (UOTGHS) Host Pipe Mask Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIER      (*(__O  uint32_t*)0x400205F0U) /**< \brief (UOTGHS) Host Pipe Enable Register (n = 0) */
  #define REG_UOTGHS_HSTPIPIDR      (*(__O  uint32_t*)0x40020620U) /**< \brief (UOTGHS) Host Pipe Disable Register (n = 0) */
  #define REG_UOTGHS_HSTPIPINRQ     (*(__IO uint32_t*)0x40020650U) /**< \brief (UOTGHS) Host Pipe IN Request Register (n = 0) */
  #define REG_UOTGHS_HSTPIPERR      (*(__IO uint32_t*)0x40020680U) /**< \brief (UOTGHS) Host Pipe Error Register (n = 0) */
  #define REG_UOTGHS_HSTDMANXTDSC1  (*(__IO uint32_t*)0x40020710U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 1) */
  #define REG_UOTGHS_HSTDMAADDRESS1 (*(__IO uint32_t*)0x40020714U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 1) */
  #define REG_UOTGHS_HSTDMACONTROL1 (*(__IO uint32_t*)0x40020718U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 1) */
  #define REG_UOTGHS_HSTDMASTATUS1  (*(__IO uint32_t*)0x4002071CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 1) */
  #define REG_UOTGHS_HSTDMANXTDSC2  (*(__IO uint32_t*)0x40020720U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 2) */
  #define REG_UOTGHS_HSTDMAADDRESS2 (*(__IO uint32_t*)0x40020724U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 2) */
  #define REG_UOTGHS_HSTDMACONTROL2 (*(__IO uint32_t*)0x40020728U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 2) */
  #define REG_UOTGHS_HSTDMASTATUS2  (*(__IO uint32_t*)0x4002072CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 2) */
  #define REG_UOTGHS_HSTDMANXTDSC3  (*(__IO uint32_t*)0x40020730U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 3) */
  #define REG_UOTGHS_HSTDMAADDRESS3 (*(__IO uint32_t*)0x40020734U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 3) */
  #define REG_UOTGHS_HSTDMACONTROL3 (*(__IO uint32_t*)0x40020738U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 3) */
  #define REG_UOTGHS_HSTDMASTATUS3  (*(__IO uint32_t*)0x4002073CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 3) */
  #define REG_UOTGHS_HSTDMANXTDSC4  (*(__IO uint32_t*)0x40020740U) /**< \brief (UOTGHS) Host DMA Channel Next Descriptor Address Register (n = 4) */
  #define REG_UOTGHS_HSTDMAADDRESS4 (*(__IO uint32_t*)0x40020744U) /**< \brief (UOTGHS) Host DMA Channel Address Register (n = 4) */
  #define REG_UOTGHS_HSTDMACONTROL4 (*(__IO uint32_t*)0x40020748U) /**< \brief (UOTGHS) Host DMA Channel Control Register (n = 4) */
  #define REG_UOTGHS_HSTDMASTATUS4  (*(__IO uint32_t*)0x4002074CU) /**< \brief (UOTGHS) Host DMA Channel Status Register (n = 4) */
  #define REG_UOTGHS_CTRL           (*(__IO uint32_t*)0x40020800U) /**< \brief (UOTGHS) General Control Register */
  #define REG_UOTGHS_SR             (*(__I  uint32_t*)0x40020804U) /**< \brief (UOTGHS) General Status Register */
  #define REG_UOTGHS_SCR            (*(__O  uint32_t*)0x40020808U) /**< \brief (UOTGHS) General Status Clear Register */
  #define REG_UOTGHS_SFR            (*(__O  uint32_t*)0x4002080CU) /**< \brief (UOTGHS) General Status Set Register */
  #define REG_UOTGHS_FSM            (*(__I  uint32_t*)0x4002082CU) /**< \brief (UOTGHS) General Finite State Machine Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _SAM4C_UOTGHS_INSTANCE_ */
