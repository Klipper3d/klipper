/**
 * \file
 *
 * \brief This file contains the default exception handlers.
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
/* Cortex-M4 core handlers */
void Reset_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void NMI_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void ADC_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void EFC_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void MEM2MEM_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void PIOA_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void PIOB_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void PMC_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void RSTC_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void RTC_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void RTT_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void SPI_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void TWI2_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void SUPC_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void TC0_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void TC1_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void TC2_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void TWI0_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void TWI1_Handler(void)   __attribute__ ((weak, alias("Dummy_Handler")));
void UART0_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void UART1_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void USART_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WDT_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void ARM_Handler(void)    __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP0_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP1_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP2_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP3_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP4_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP5_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP6_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP7_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP8_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP9_Handler(void)  __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP10_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP11_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP12_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP13_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP14_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
void WKUP15_Handler(void) __attribute__ ((weak, alias("Dummy_Handler")));
#endif /* __GNUC__ */

#ifdef __ICCARM__
/* Cortex-M4 core handlers */
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
#pragma weak ADC_Handler=Dummy_Handler
#pragma weak EFC_Handler=Dummy_Handler
#pragma weak MEM2MEM_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#pragma weak PMC_Handler=Dummy_Handler
#pragma weak RSTC_Handler=Dummy_Handler
#pragma weak RTC_Handler=Dummy_Handler
#pragma weak RTT_Handler=Dummy_Handler
#pragma weak SPI_Handler=Dummy_Handler
#pragma weak SUPC_Handler=Dummy_Handler
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#pragma weak TWI0_Handler=Dummy_Handler
#pragma weak TWI1_Handler=Dummy_Handler
#pragma weak TWI2_Handler=Dummy_Handler
#pragma weak UART0_Handler=Dummy_Handler
#pragma weak UART1_Handler=Dummy_Handler
#pragma weak USART_Handler=Dummy_Handler
#pragma weak WDT_Handler=Dummy_Handler
#pragma weak ARM_Handler=Dummy_Handler
#pragma weak WKUP0_Handler=Dummy_Handler
#pragma weak WKUP1_Handler=Dummy_Handler
#pragma weak WKUP2_Handler=Dummy_Handler
#pragma weak WKUP3_Handler=Dummy_Handler
#pragma weak WKUP4_Handler=Dummy_Handler
#pragma weak WKUP5_Handler=Dummy_Handler
#pragma weak WKUP6_Handler=Dummy_Handler
#pragma weak WKUP7_Handler=Dummy_Handler
#pragma weak WKUP8_Handler=Dummy_Handler
#pragma weak WKUP9_Handler=Dummy_Handler
#pragma weak WKUP10_Handler=Dummy_Handler
#pragma weak WKUP11_Handler=Dummy_Handler
#pragma weak WKUP12_Handler=Dummy_Handler
#pragma weak WKUP13_Handler=Dummy_Handler
#pragma weak WKUP14_Handler=Dummy_Handler
#pragma weak WKUP15_Handler=Dummy_Handler
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
