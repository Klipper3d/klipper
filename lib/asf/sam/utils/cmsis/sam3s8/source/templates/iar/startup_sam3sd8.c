/**
 * \file
 *
 * \brief Startup file for SAM3SD8.
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
#include "sam3s8.h"
#include "system_sam3sd8.h"

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Exception Table */
#pragma language=extended
#pragma segment="CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
const intvec_elem __vector_table[] = {
	{.__ptr = __sfe("CSTACK")},
	Reset_Handler,

	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, 0, 0, 0,         /*  Reserved */
	SVC_Handler,
	DebugMon_Handler,
	0,                  /*  Reserved */
	PendSV_Handler,
	SysTick_Handler,

	/* Configurable interrupts */
	SUPC_Handler,    /* 0  Supply Controller */
	RSTC_Handler,    /* 1  Reset Controller */
	RTC_Handler,     /* 2  Real Time Clock */
	RTT_Handler,     /* 3  Real Time Timer */
	WDT_Handler,     /* 4  Watchdog Timer */
	PMC_Handler,     /* 5  PMC */
	EFC_Handler,	 /* 6  EFC */
	Dummy_Handler,   /* 7  Reserved */
	UART0_Handler,   /* 8  UART0 */
	UART1_Handler,   /* 9  UART1 */
#ifdef ID_SMC
	SMC_Handler,     /* 10 SMC */
#else
	Dummy_Handler,
#endif
	PIOA_Handler,    /* 11 Parallel IO Controller A */
	PIOB_Handler,    /* 12 Parallel IO Controller B */
#ifdef ID_PIOC
	PIOC_Handler,    /* 13 Parallel IO Controller C */
#else
	Dummy_Handler,
#endif
	USART0_Handler,  /* 14 USART 0 */
	USART1_Handler,  /* 15 USART 1 */
#ifdef ID_USART2
	USART2_Handler,   /* 16 USART 2 */
#else
	Dummy_Handler,
#endif
	Dummy_Handler,   /* 17 Reserved */
	HSMCI_Handler,   /* 18 HSMCI */
	TWI0_Handler,    /* 19 TWI 0 */
	TWI1_Handler,    /* 20 TWI 1 */
	SPI_Handler,     /* 21 SPI */
	SSC_Handler,     /* 22 SSC */
	TC0_Handler,     /* 23 Timer Counter 0 */
	TC1_Handler,     /* 24 Timer Counter 1 */
	TC2_Handler,     /* 25 Timer Counter 2 */
#ifdef ID_TC3
	TC3_Handler,     /* 26 Timer Counter 3 */
#else
	Dummy_Handler,
#endif
#ifdef ID_TC4
	TC4_Handler,     /* 27 Timer Counter 4 */
#else
	Dummy_Handler,
#endif
#ifdef ID_TC5
	TC5_Handler,     /* 28 Timer Counter 5 */
#else
	Dummy_Handler,
#endif
	ADC_Handler,     /* 29 ADC controller */
	DACC_Handler,    /* 30 DACC controller */
	PWM_Handler,     /* 31 PWM */
	CRCCU_Handler,   /* 32 CRC Calculation Unit */
	ACC_Handler,     /* 33 Analog Comparator */
	UDP_Handler,     /* 34 USB Device Port */
	Dummy_Handler    /* 35 not used */
};

/* EWARM 6.30 integrates CMSIS 2.10 (__CM3_CMSIS_VERSION 0x0210),
 * in which SCB_VTOR_TBLBASE_Msk not defined.
 */
#if (__VER__ >= 6030000)
/* TEMPORARY PATCH FOR SCB */
#define SCB_VTOR_TBLBASE_Pos               29                            /*!< SCB VTOR: TBLBASE Position */
#define SCB_VTOR_TBLBASE_Msk               (1UL << SCB_VTOR_TBLBASE_Pos) /*!< SCB VTOR: TBLBASE Mask */
#endif

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
	uint32_t *pSrc = __section_begin(".intvec");

	SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

	if (((uint32_t) pSrc >= IRAM_ADDR) && ((uint32_t) pSrc < IRAM_ADDR + IRAM_SIZE)) {
		SCB->VTOR |= 1 << SCB_VTOR_TBLBASE_Pos;
	}

	return 1; /* if return 0, the data sections will not be initialized */
}

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
void Reset_Handler(void)
{
	__iar_program_start();
}
