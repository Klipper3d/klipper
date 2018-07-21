/**
 * \file
 *
 * \brief Startup file for SAM4N.
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

#include "sam4n.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void __libc_init_array(void);

/* Exception Table */
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(void *)(&_estack),

	(void *)Reset_Handler,
	(void *)NMI_Handler,
	(void *)HardFault_Handler,
	(void *)MemManage_Handler,
	(void *)BusFault_Handler,
	(void *)UsageFault_Handler,
	(void *)(0UL),    /* Reserved */
	(void *)(0UL),    /* Reserved */
	(void *)(0UL),    /* Reserved */
	(void *)(0UL),    /* Reserved */
	(void *)SVC_Handler,
	(void *)DebugMon_Handler,
	(void *)(0UL),    /* Reserved */
	(void *)PendSV_Handler,
	(void *)SysTick_Handler,

	/* Configurable interrupts */
	(void *)SUPC_Handler,    /* 0  Supply Controller */
	(void *)RSTC_Handler,    /* 1  Reset Controller */
	(void *)RTC_Handler,    /* 2  Real Time Clock */
	(void *)RTT_Handler,    /* 3  Real Time Timer */
	(void *)WDT_Handler,    /* 4  Watchdog Timer */
	(void *)PMC_Handler,    /* 5  Power Management Controller */
	(void *)EFC_Handler,    /* 6  Enhanced Flash Controller */
	(void *)(0UL),    /* 7 Reserved */
	(void *)UART0_Handler,    /* 8  UART 0 */
	(void *)UART1_Handler,    /* 9  UART 1 */
	(void *)UART2_Handler,    /* 10 UART 2 */
	(void *)PIOA_Handler,    /* 11 Parallel I/O Controller A */
	(void *)PIOB_Handler,    /* 12 Parallel I/O Controller B */
#ifdef _SAM4N_PIOC_INSTANCE_
	(void *)PIOC_Handler,    /* 13 Parallel I/O Controller C */
#else
	(void *)(0UL),    /* 13 Reserved */
#endif /* _SAM4N_PIOC_INSTANCE_ */
	(void *)USART0_Handler,    /* 14 USART 0 */
#ifdef _SAM4N_USART1_INSTANCE_
	(void *)USART1_Handler,    /* 15 USART 1 */
#else
	(void *)(0UL),    /* 15 Reserved */
#endif /* _SAM4N_USART1_INSTANCE_ */
	(void *)UART3_Handler,    /* 16 UARG 3 */
#ifdef _SAM4N_USART2_INSTANCE_
	(void *)USART2_Handler,    /* 17 USART 2 */
#else
	(void *)(0UL),    /* 17 Reserved */
#endif /* _SAM4N_USART2_INSTANCE_ */
	(void *)(0UL),    /* 18 Reserved */
	(void *)TWI0_Handler,    /* 19 Two Wire Interface 0 */
	(void *)TWI1_Handler,    /* 20 Two Wire Interface 1 */
	(void *)SPI_Handler,    /* 21 Serial Peripheral Interface */
	(void *)TWI2_Handler,    /* 22 Two Wire Interface 2 */
	(void *)TC0_Handler,    /* 23 Timer/Counter 0 */
	(void *)TC1_Handler,    /* 24 Timer/Counter 1 */
	(void *)TC2_Handler,    /* 25 Timer/Counter 2 */
#ifdef _SAM4N_TC1_INSTANCE_
	(void *)TC3_Handler,    /* 26 Timer/Counter 3 */
#else
	(void *)(0UL),    /* 26 Reserved */
#endif /* _SAM4N_TC1_INSTANCE_ */
#ifdef _SAM4N_TC1_INSTANCE_
	(void *)TC4_Handler,    /* 27 Timer/Counter 4 */
#else
	(void *)(0UL),    /* 27 Reserved */
#endif /* _SAM4N_TC1_INSTANCE_ */
#ifdef _SAM4N_TC1_INSTANCE_
	(void *)TC5_Handler,    /* 28 Timer/Counter 5 */
#else
	(void *)(0UL),    /* 28 Reserved */
#endif /* _SAM4N_TC1_INSTANCE_ */
	(void *)ADC_Handler,    /* 29 Analog To Digital Converter */
#ifdef _SAM4N_DACC_INSTANCE_
	(void *)DACC_Handler,    /* 30 Digital To Analog Converter */
#else
	(void *)(0UL),    /* 30 Reserved */
#endif /* _SAM4N_DACC_INSTANCE_ */
	(void *)PWM_Handler    /* 31 Pulse Width Modulation */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
	uint32_t *pSrc, *pDest;

	/* Initialize the relocate segment */
	pSrc = &_etext;
	pDest = &_srelocate;

	if (pSrc != pDest) {
		for (; pDest < &_erelocate;) {
			*pDest++ = *pSrc++;
		}
	}

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) {
		*pDest++ = 0;
	}

	/* Set the vector table base address */
	pSrc = (uint32_t *) & _sfixed;
	SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

	/* Initialize the C library */
	__libc_init_array();

	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}
