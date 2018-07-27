/**
 * \file
 *
 * \brief Startup file for SAM4C.
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

#include "sam4c.h"

typedef void (*intfunc) (void);
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

void __iar_program_start(void);
int __low_level_init(void);

/* Default empty handler */
void Dummy_Handler(void);

/* Cortex-M4 core handlers */
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
#pragma weak SUPC_Handler=Dummy_Handler
#pragma weak RSTC_Handler=Dummy_Handler
#pragma weak RTC_Handler=Dummy_Handler
#pragma weak RTT_Handler=Dummy_Handler
#pragma weak WDT_Handler=Dummy_Handler
#pragma weak PMC_Handler=Dummy_Handler
#ifdef _SAM4C_EFC_INSTANCE_
#pragma weak EFC_Handler=Dummy_Handler
#else
#pragma weak EFC0_Handler=Dummy_Handler
#pragma weak EFC1_Handler=Dummy_Handler
#endif
#pragma weak UART0_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#ifdef _SAM4C_PIOD_INSTANCE_
#pragma weak PIOD_Handler=Dummy_Handler
#endif
#pragma weak USART0_Handler=Dummy_Handler
#pragma weak USART1_Handler=Dummy_Handler
#pragma weak USART2_Handler=Dummy_Handler
#pragma weak USART3_Handler=Dummy_Handler
#pragma weak USART4_Handler=Dummy_Handler
#pragma weak TWI0_Handler=Dummy_Handler
#pragma weak TWI1_Handler=Dummy_Handler
#pragma weak SPI0_Handler=Dummy_Handler
#ifdef _SAM4C_UOTGHS_INSTANCE_
#pragma weak UOTGHS_Handler=Dummy_Handler
#endif
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#pragma weak TC3_Handler=Dummy_Handler
#pragma weak TC4_Handler=Dummy_Handler
#pragma weak TC5_Handler=Dummy_Handler
#pragma weak ADC_Handler=Dummy_Handler
#pragma weak ARM1_Handler=Dummy_Handler
#pragma weak IPC0_Handler=Dummy_Handler
#pragma weak SLCDC_Handler=Dummy_Handler
#pragma weak TRNG_Handler=Dummy_Handler
#pragma weak ICM_Handler=Dummy_Handler
#pragma weak CPKCC_Handler=Dummy_Handler
#pragma weak AES_Handler=Dummy_Handler
#pragma weak PIOC_Handler=Dummy_Handler
#pragma weak UART1_Handler=Dummy_Handler
#pragma weak IPC1_Handler=Dummy_Handler
#pragma weak SPI1_Handler=Dummy_Handler
#pragma weak PWM_Handler=Dummy_Handler

/* Exception Table */
#pragma language = extended
#pragma segment = "CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
// [startup_vector_table]
const DeviceVectors __vector_table = {
	(void*) __sfe("CSTACK"),

	(void*) Reset_Handler,
	(void*) NMI_Handler,
	(void*) HardFault_Handler,
	(void*) MemManage_Handler,
	(void*) BusFault_Handler,
	(void*) UsageFault_Handler,
	(void*) (0UL),          /* Reserved */
	(void*) (0UL),          /* Reserved */
	(void*) (0UL),          /* Reserved */
	(void*) (0UL),          /* Reserved */
	(void*) SVC_Handler,
	(void*) DebugMon_Handler,
	(void*) (0UL),          /* Reserved */
	(void*) PendSV_Handler,
	(void*) SysTick_Handler,

	/* Configurable interrupts */
	(void*) SUPC_Handler,   /* 0  Supply Controller */
	(void*) RSTC_Handler,   /* 1  Reset Controller */
	(void*) RTC_Handler,    /* 2  Real Time Clock */
	(void*) RTT_Handler,    /* 3  Real Time Timer */
	(void*) WDT_Handler,    /* 4  Watchdog Timer */
	(void*) PMC_Handler,    /* 5  Power Management Controller */
#ifdef _SAM4C_EFC_INSTANCE_
	(void*) EFC_Handler,    /* 6  Enhanced Embedded Flash Controller */
	(void*) (0UL),          /* 7  Reserved */
#else
	(void*) EFC0_Handler,    /* 6  Enhanced Embedded Flash Controller */
	(void*) EFC1_Handler,    /* 7  Enhanced Embedded Flash Controller */
#endif
	(void*) UART0_Handler,  /* 8  UART 0 */
	(void*) (0UL),          /* 9  Reserved */
	(void*) (0UL),          /* 10 Reserved */
	(void*) PIOA_Handler,   /* 11 Parallel I/O Controller A */
	(void*) PIOB_Handler,   /* 12 Parallel I/O Controller B */
#ifdef _SAM4C_PIOD_INSTANCE_
    (void*) PIOD_Handler,   /* 13 Parallel I/O Controller D */
#else
	(void*) (0UL),          /* 13 Reserved */
#endif

	(void*) USART0_Handler, /* 14 USART 0 */
	(void*) USART1_Handler, /* 15 USART 1 */
	(void*) USART2_Handler, /* 16 USART 2 */
	(void*) USART3_Handler, /* 17 USART 3 */
	(void*) USART4_Handler, /* 18 USART 4 */
	(void*) TWI0_Handler,   /* 19 Two Wire Interface 0 */
	(void*) TWI1_Handler,   /* 20 Two Wire Interface 1 */
	(void*) SPI0_Handler,   /* 21 Serial Peripheral Interface 0 */
#ifdef _SAM4C_UOTGHS_INSTANCE_
	(void*) UOTGHS_Handler,   /* 22 Serial Peripheral Interface 0 */
#else
	(void*) (0UL),          /* 22 Reserved */
#endif
	
	(void*) TC0_Handler,    /* 23 Timer/Counter 0 */
	(void*) TC1_Handler,    /* 24 Timer/Counter 1 */
	(void*) TC2_Handler,    /* 25 Timer/Counter 2 */
	(void*) TC3_Handler,    /* 26 Timer/Counter 3 */
	(void*) TC4_Handler,    /* 27 Timer/Counter 4 */
	(void*) TC5_Handler,    /* 28 Timer/Counter 5 */
	(void*) ADC_Handler,    /* 29 Analog To Digital Converter */
	(void*) ARM1_Handler,   /* 30 FPU signals : FPIXC, FPOFC, FPUFC, FPIOC,
			FPDZC, FPIDC, FPIXC */
	(void*) IPC0_Handler,   /* 31 Interprocessor communication 0 */
	(void*) SLCDC_Handler,  /* 32 Segment LCD Controller */
	(void*) TRNG_Handler,   /* 33 True Random Generator */
	(void*) ICM_Handler,    /* 34 Integrity Check Module */
	(void*) CPKCC_Handler,  /* 35 Public Key Cryptography Controller */
	(void*) AES_Handler,    /* 36 Advanced Enhanced Standard */
	(void*) PIOC_Handler,   /* 37 Parallel I/O Controller C */
	(void*) UART1_Handler,  /* 38 UART 1 */
	(void*) IPC1_Handler,   /* 39 Interprocessor communication 1 */
	(void*) SPI1_Handler,   /* 40 Serial Peripheral Interface 1 */
	(void*) PWM_Handler,    /* 41 Pulse Width Modulation */
};
// [startup_vector_table]

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
	uint32_t *pSrc = __section_begin(".intvec");

	SCB->VTOR = (uint32_t)pSrc;

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

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
	while (1) {
	}
}
