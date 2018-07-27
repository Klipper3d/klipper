/**
 * \file
 *
 * \brief Startup file for SAM3U.
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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

#include "exceptions.h"
#include "sam3u.h"
#include "system_sam3u.h"

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
IntFunc exception_table[] = {

	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(IntFunc) (&_estack),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, 0, 0, 0,	          /*  Reserved */
	SVC_Handler,
	DebugMon_Handler,
	0,	                  /*  Reserved */
	PendSV_Handler,
	SysTick_Handler,

	/* Configurable interrupts */
	SUPC_Handler,      /* 0  Supply Controller */
	RSTC_Handler,      /* 1  Reset Controller */
	RTC_Handler,       /* 2  Real Time Clock */
	RTT_Handler,       /* 3  Real Time Timer */
	WDT_Handler,       /* 4  Watchdog Timer */
	PMC_Handler,       /* 5  PMC */
	EFC0_Handler,      /* 6  EEFC 0 */
	EFC1_Handler,      /* 7  EEFC 1 */
	UART_Handler,      /* 8  UART0 */
	SMC_Handler,       /* 9  SMC */
	PIOA_Handler,      /* 10 Parallel IO Controller A */
	PIOB_Handler,      /* 11 Parallel IO Controller B */
#ifdef ID_PIOC
	PIOC_Handler,      /* 12 Parallel IO Controller C */
#else
	Dummy_Handler,
#endif
	USART0_Handler,    /* 13 USART 0 */
	USART1_Handler,    /* 14 USART 1 */
	USART2_Handler,    /* 15 USART 2 */
#ifdef ID_USART3
	USART3_Handler,    /* 16 USART 3 */
#else
	Dummy_Handler,
#endif
	HSMCI_Handler,     /* 17 MCI */
	TWI0_Handler,      /* 18 TWI 0 */
	TWI1_Handler,      /* 19 TWI 1 */
	SPI_Handler,       /* 20 SPI */
	SSC_Handler,       /* 21 SSC */
	TC0_Handler,       /* 22 Timer Counter 0 */
	TC1_Handler,       /* 23 Timer Counter 1 */
	TC2_Handler,       /* 24 Timer Counter 2 */
	PWM_Handler,       /* 25 PWM */
	ADC12B_Handler,    /* 26 ADC12B controller */
	ADC_Handler,       /* 27 ADC controller */
	DMAC_Handler,      /* 28 DMA controller */
	UDPHS_Handler,     /* 29 USB High Speed Port */
	Dummy_Handler      /* 30 not used */
};

/* TEMPORARY PATCH FOR SCB */
#define SCB_VTOR_TBLBASE_Pos               29                            /*!< SCB VTOR: TBLBASE Position */
#define SCB_VTOR_TBLBASE_Msk               (1UL << SCB_VTOR_TBLBASE_Pos) /*!< SCB VTOR: TBLBASE Mask */

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

	if (((uint32_t) pSrc >= IRAM0_ADDR) && ((uint32_t) pSrc < IRAM0_ADDR + IRAM_SIZE)) {
		SCB->VTOR |= 1 << SCB_VTOR_TBLBASE_Pos;
	}

	/* Initialize the C library */
	__libc_init_array();

	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}
