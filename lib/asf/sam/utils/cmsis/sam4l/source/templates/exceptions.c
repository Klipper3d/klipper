/**
 * \file
 *
 * \brief This file contains the default exception handlers.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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
 * \par Purpose
 *
 * This file provides basic support for Cortex-M processor based
 * microcontrollers.
 *
 * \note
 * The exception handler has weak aliases.
 * As they are weak aliases, any function with the same name will override
 * this definition.
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "exceptions.h"

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/* @endcond */

#ifdef __GNUC__
/* Cortex-M core handlers */
void Reset_Handler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void ABDACB_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void ACIFC_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void ADCIFE_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void AESA_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void AST_ALARM_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void AST_CLKREADY_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void AST_OVF_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void AST_PER_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void AST_READY_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void BPM_Handler(void)          __attribute__ ((weak, alias("Dummy_Handler")));
void BSCIF_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void CATB_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void CRCCU_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void DACC_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_1_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_2_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_3_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_4_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_5_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_6_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_7_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void EIC_8_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void FREQM_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_0_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_1_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_10_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_11_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_2_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_3_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_4_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_5_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_6_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_7_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_8_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void GPIO_9_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void HFLASHC_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void IISC_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void LCDCA_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void PARC_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_0_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_1_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_10_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_11_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_12_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_13_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_14_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_15_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_2_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_3_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_4_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_5_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_6_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_7_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_8_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PDCA_9_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void PEVC_OV_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PEVC_TR_Handler(void)      __attribute__ ((weak, alias("Dummy_Handler")));
void PM_Handler(void)           __attribute__ ((weak, alias("Dummy_Handler")));
void SCIF_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void SPI_Handler(void)          __attribute__ ((weak, alias("Dummy_Handler")));
void TC00_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TC01_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TC02_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TC10_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TC11_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TC12_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TRNG_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void TWIM0_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TWIM1_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TWIM2_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TWIM3_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TWIS0_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void TWIS1_Handler(void)        __attribute__ ((weak, alias("Dummy_Handler")));
void USART0_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void USART1_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void USART2_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void USART3_Handler(void)       __attribute__ ((weak, alias("Dummy_Handler")));
void USBC_Handler(void)         __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler(void)          __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* __GNUC__ */

#ifdef __ICCARM__
/* Cortex-M3 core handlers */
#pragma weak Reset_Handler=Dummy_Handler
#pragma weak NMI_Handler=Dummy_Handler
#pragma weak HardFault_Handler=Dummy_Handler
#pragma weak MemManage_Handler=Dummy_Handler
#pragma weak BusFault_Handler=Dummy_Handler
#pragma weak UsageFault_Handler=Dummy_Handler
#pragma weak SVC_Handler=Dummy_Handler
#pragma weak DebugMon_Handler=Dummy_Handler
#pragma weak PendSV_Handler=Dummy_Handler
#pragma weak SysTick_Handler=Dummy_Handler

/* Peripherals handlers */
#pragma weak ABDACB_Handler       = Dummy_Handler
#pragma weak ACIFC_Handler        = Dummy_Handler
#pragma weak ADCIFE_Handler       = Dummy_Handler
#pragma weak AESA_Handler         = Dummy_Handler
#pragma weak AST_ALARM_Handler    = Dummy_Handler
#pragma weak AST_CLKREADY_Handler = Dummy_Handler
#pragma weak AST_OVF_Handler      = Dummy_Handler
#pragma weak AST_PER_Handler      = Dummy_Handler
#pragma weak AST_READY_Handler    = Dummy_Handler
#pragma weak BPM_Handler          = Dummy_Handler
#pragma weak BSCIF_Handler        = Dummy_Handler
#pragma weak CATB_Handler         = Dummy_Handler
#pragma weak CRCCU_Handler        = Dummy_Handler
#pragma weak DACC_Handler         = Dummy_Handler
#pragma weak EIC_1_Handler        = Dummy_Handler
#pragma weak EIC_2_Handler        = Dummy_Handler
#pragma weak EIC_3_Handler        = Dummy_Handler
#pragma weak EIC_4_Handler        = Dummy_Handler
#pragma weak EIC_5_Handler        = Dummy_Handler
#pragma weak EIC_6_Handler        = Dummy_Handler
#pragma weak EIC_7_Handler        = Dummy_Handler
#pragma weak EIC_8_Handler        = Dummy_Handler
#pragma weak FREQM_Handler        = Dummy_Handler
#pragma weak GPIO_0_Handler       = Dummy_Handler
#pragma weak GPIO_1_Handler       = Dummy_Handler
#pragma weak GPIO_10_Handler      = Dummy_Handler
#pragma weak GPIO_11_Handler      = Dummy_Handler
#pragma weak GPIO_2_Handler       = Dummy_Handler
#pragma weak GPIO_3_Handler       = Dummy_Handler
#pragma weak GPIO_4_Handler       = Dummy_Handler
#pragma weak GPIO_5_Handler       = Dummy_Handler
#pragma weak GPIO_6_Handler       = Dummy_Handler
#pragma weak GPIO_7_Handler       = Dummy_Handler
#pragma weak GPIO_8_Handler       = Dummy_Handler
#pragma weak GPIO_9_Handler       = Dummy_Handler
#pragma weak HFLASHC_Handler      = Dummy_Handler
#pragma weak IISC_Handler         = Dummy_Handler
#pragma weak LCDCA_Handler        = Dummy_Handler
#pragma weak PARC_Handler         = Dummy_Handler
#pragma weak PDCA_0_Handler       = Dummy_Handler
#pragma weak PDCA_1_Handler       = Dummy_Handler
#pragma weak PDCA_10_Handler      = Dummy_Handler
#pragma weak PDCA_11_Handler      = Dummy_Handler
#pragma weak PDCA_12_Handler      = Dummy_Handler
#pragma weak PDCA_13_Handler      = Dummy_Handler
#pragma weak PDCA_14_Handler      = Dummy_Handler
#pragma weak PDCA_15_Handler      = Dummy_Handler
#pragma weak PDCA_2_Handler       = Dummy_Handler
#pragma weak PDCA_3_Handler       = Dummy_Handler
#pragma weak PDCA_4_Handler       = Dummy_Handler
#pragma weak PDCA_5_Handler       = Dummy_Handler
#pragma weak PDCA_6_Handler       = Dummy_Handler
#pragma weak PDCA_7_Handler       = Dummy_Handler
#pragma weak PDCA_8_Handler       = Dummy_Handler
#pragma weak PDCA_9_Handler       = Dummy_Handler
#pragma weak PEVC_OV_Handler      = Dummy_Handler
#pragma weak PEVC_TR_Handler      = Dummy_Handler
#pragma weak PM_Handler           = Dummy_Handler
#pragma weak SCIF_Handler         = Dummy_Handler
#pragma weak SPI_Handler          = Dummy_Handler
#pragma weak TC00_Handler        = Dummy_Handler
#pragma weak TC01_Handler        = Dummy_Handler
#pragma weak TC02_Handler        = Dummy_Handler
#pragma weak TC10_Handler        = Dummy_Handler
#pragma weak TC11_Handler        = Dummy_Handler
#pragma weak TC12_Handler        = Dummy_Handler
#pragma weak TRNG_Handler         = Dummy_Handler
#pragma weak TWIM0_Handler        = Dummy_Handler
#pragma weak TWIM1_Handler        = Dummy_Handler
#pragma weak TWIM2_Handler        = Dummy_Handler
#pragma weak TWIM3_Handler        = Dummy_Handler
#pragma weak TWIS0_Handler        = Dummy_Handler
#pragma weak TWIS1_Handler        = Dummy_Handler
#pragma weak USART0_Handler       = Dummy_Handler
#pragma weak USART1_Handler       = Dummy_Handler
#pragma weak USART2_Handler       = Dummy_Handler
#pragma weak USART3_Handler       = Dummy_Handler
#pragma weak USBC_Handler         = Dummy_Handler
#pragma weak WDT_Handler          = Dummy_Handler
#endif /* __ICCARM__ */

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
	while (1) {
	}
}

/* @cond 0 */
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/* @endcond */
