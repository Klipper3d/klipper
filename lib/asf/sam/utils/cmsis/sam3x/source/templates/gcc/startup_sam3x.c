/**
 * \file
 *
 * \brief Startup file for SAM3X.
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
#include "sam3xa.h"
#include "system_sam3x.h"

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
	(void*) Reset_Handler,

	(void*) NMI_Handler,
	(void*) HardFault_Handler,
	(void*) MemManage_Handler,
	(void*) BusFault_Handler,
	(void*) UsageFault_Handler,
	(void*) (0UL),           /* Reserved */
	(void*) (0UL),           /* Reserved */
	(void*) (0UL),           /* Reserved */
	(void*) (0UL),           /* Reserved */
	(void*) SVC_Handler,
	(void*) DebugMon_Handler,
	(void*) (0UL),           /* Reserved */
	(void*) PendSV_Handler,
	(void*) SysTick_Handler,

	/* Configurable interrupts */
	(void*) SUPC_Handler,    /*  0  Supply Controller */
	(void*) RSTC_Handler,    /*  1  Reset Controller */
	(void*) RTC_Handler,     /*  2  Real Time Clock */
	(void*) RTT_Handler,     /*  3  Real Time Timer */
	(void*) WDT_Handler,     /*  4  Watchdog Timer */
	(void*) PMC_Handler,     /*  5  PMC */
	(void*) EFC0_Handler,    /*  6  EFC 0 */
	(void*) EFC1_Handler,    /*  7  EFC 1 */
	(void*) UART_Handler,    /*  8  UART */
#ifdef _SAM3XA_SMC_INSTANCE_
	(void*) SMC_Handler,     /*  9  SMC */
#else
	(void*) (0UL),           /*  9 Reserved */
#endif /* _SAM3XA_SMC_INSTANCE_ */
#ifdef _SAM3XA_SDRAMC_INSTANCE_
	(void*) SDRAMC_Handler,  /* 10  SDRAMC */
#else
	(void*) (0UL),           /* 10 Reserved */
#endif /* _SAM3XA_SDRAMC_INSTANCE_ */
	(void*) PIOA_Handler,    /* 11 Parallel IO Controller A */
	(void*) PIOB_Handler,    /* 12 Parallel IO Controller B */
#ifdef _SAM3XA_PIOC_INSTANCE_
	(void*) PIOC_Handler,    /* 13 Parallel IO Controller C */
#else
	(void*) (0UL),           /* 13 Reserved */
#endif /* _SAM3XA_PIOC_INSTANCE_ */
#ifdef _SAM3XA_PIOD_INSTANCE_
	(void*) PIOD_Handler,    /* 14 Parallel IO Controller D */
#else
	(void*) (0UL),           /* 14 Reserved */
#endif /* _SAM3XA_PIOD_INSTANCE_ */
#ifdef _SAM3XA_PIOE_INSTANCE_
	(void*) PIOE_Handler,    /* 15 Parallel IO Controller E */
#else
	(void*) (0UL),           /* 15 Reserved */
#endif /* _SAM3XA_PIOE_INSTANCE_ */
#ifdef _SAM3XA_PIOF_INSTANCE_
	(void*) PIOF_Handler,    /* 16 Parallel IO Controller F */
#else
	(void*) (0UL),           /* 16 Reserved */
#endif /* _SAM3XA_PIOF_INSTANCE_ */
	(void*) USART0_Handler,  /* 17 USART 0 */
	(void*) USART1_Handler,  /* 18 USART 1 */
	(void*) USART2_Handler,  /* 19 USART 2 */
#ifdef _SAM3XA_USART3_INSTANCE_
	(void*) USART3_Handler,  /* 20 USART 3 */
#else
	(void*) (0UL),           /* 20 Reserved */
#endif /* _SAM3XA_USART3_INSTANCE_ */
	(void*) HSMCI_Handler,   /* 21 MCI */
	(void*) TWI0_Handler,    /* 22 TWI 0 */
	(void*) TWI1_Handler,    /* 23 TWI 1 */
	(void*) SPI0_Handler,    /* 24 SPI 0 */
#ifdef _SAM3XA_SPI1_INSTANCE_
	(void*) SPI1_Handler,    /* 25 SPI 1 */
#else
	(void*) (0UL),           /* 25 Reserved */
#endif /* _SAM3XA_SPI1_INSTANCE_ */
	(void*) SSC_Handler,     /* 26 SSC */
	(void*) TC0_Handler,     /* 27 Timer Counter 0 */
	(void*) TC1_Handler,     /* 28 Timer Counter 1 */
	(void*) TC2_Handler,     /* 29 Timer Counter 2 */
	(void*) TC3_Handler,     /* 30 Timer Counter 3 */
	(void*) TC4_Handler,     /* 31 Timer Counter 4 */
	(void*) TC5_Handler,     /* 32 Timer Counter 5 */
#ifdef _SAM3XA_TC2_INSTANCE_
	(void*) TC6_Handler,     /* 33 Timer Counter 6 */
	(void*) TC7_Handler,     /* 34 Timer Counter 7 */
	(void*) TC8_Handler,     /* 35 Timer Counter 8 */
#else
	(void*) (0UL),           /* 33 Reserved */
	(void*) (0UL),           /* 34 Reserved */
	(void*) (0UL),           /* 35 Reserved */
#endif /* _SAM3XA_TC2_INSTANCE_ */
	(void*) PWM_Handler,     /* 36 PWM */
	(void*) ADC_Handler,     /* 37 ADC controller */
	(void*) DACC_Handler,    /* 38 DAC controller */
	(void*) DMAC_Handler,    /* 39 DMA Controller */
	(void*) UOTGHS_Handler,  /* 40 USB OTG High Speed */
	(void*) TRNG_Handler,    /* 41 True Random Number Generator */
#ifdef _SAM3XA_EMAC_INSTANCE_
	(void*) EMAC_Handler,    /* 42 Ethernet MAC */
#else
	(void*) (0UL),           /* 42 Reserved */
#endif /* _SAM3XA_EMAC_INSTANCE_ */
	(void*) CAN0_Handler,    /* 43 CAN Controller 0 */
	(void*) CAN1_Handler     /* 44 CAN Controller 1 */
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

	if (((uint32_t) pSrc >= IRAM0_ADDR) && ((uint32_t) pSrc < NFC_RAM_ADDR)) {
		SCB->VTOR |= 1 << SCB_VTOR_TBLBASE_Pos;
	}

	/* Initialize the C library */
	__libc_init_array();

	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}
