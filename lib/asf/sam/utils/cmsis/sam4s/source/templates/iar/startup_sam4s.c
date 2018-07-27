/**
 * \file
 *
 * \brief Startup file for SAM4S.
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

#include "sam4s.h"
#include "system_sam4s.h"

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
#pragma weak EFC0_Handler=Dummy_Handler
#ifdef _SAM4S_EFC1_INSTANCE_
#  pragma weak EFC1_Handler=Dummy_Handler
#endif /* _SAM4S_EFC1_INSTANCE_ */
#pragma weak UART0_Handler=Dummy_Handler
#pragma weak UART1_Handler=Dummy_Handler
#pragma weak PIOA_Handler=Dummy_Handler
#pragma weak PIOB_Handler=Dummy_Handler
#ifdef _SAM4S_PIOC_INSTANCE_
#  pragma weak PIOC_Handler=Dummy_Handler
#endif /* _SAM4S_PIOC_INSTANCE_ */
#pragma weak USART0_Handler=Dummy_Handler
#ifdef _SAM4S_USART1_INSTANCE_
#  pragma weak USART1_Handler=Dummy_Handler
#endif /* _SAM4S_USART1_INSTANCE_ */
#ifdef _SAM4S_HSMCI_INSTANCE_
#  pragma weak HSMCI_Handler=Dummy_Handler
#endif /* _SAM4S_HSMCI_INSTANCE_ */
#pragma weak TWI0_Handler=Dummy_Handler
#pragma weak TWI1_Handler=Dummy_Handler
#pragma weak SPI_Handler=Dummy_Handler
#pragma weak SSC_Handler=Dummy_Handler
#pragma weak TC0_Handler=Dummy_Handler
#pragma weak TC1_Handler=Dummy_Handler
#pragma weak TC2_Handler=Dummy_Handler
#ifdef _SAM4S_TC1_INSTANCE_
#  pragma weak TC3_Handler=Dummy_Handler
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
#  pragma weak TC4_Handler=Dummy_Handler
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
#  pragma weak TC5_Handler=Dummy_Handler
#endif /* _SAM4S_TC1_INSTANCE_ */
#pragma weak ADC_Handler=Dummy_Handler
#ifdef _SAM4S_DACC_INSTANCE_
#  pragma weak DACC_Handler=Dummy_Handler
#endif /* _SAM4S_DACC_INSTANCE_ */
#pragma weak PWM_Handler=Dummy_Handler
#pragma weak CRCCU_Handler=Dummy_Handler
#pragma weak ACC_Handler=Dummy_Handler
#pragma weak UDP_Handler=Dummy_Handler

/* Exception Table */
#pragma language = extended
#pragma segment = "CSTACK"

/* The name "__vector_table" has special meaning for C-SPY: */
/* it is where the SP start value is found, and the NVIC vector */
/* table register (VTOR) is initialized to this address if != 0 */

#pragma section = ".intvec"
#pragma location = ".intvec"
const DeviceVectors __vector_table = {
	(void*) __sfe("CSTACK"),

	.pfnReset_Handler      = (void*) Reset_Handler,
	.pfnNMI_Handler        = (void*) NMI_Handler,
	.pfnHardFault_Handler  = (void*) HardFault_Handler,
	.pfnMemManage_Handler  = (void*) MemManage_Handler,
	.pfnBusFault_Handler   = (void*) BusFault_Handler,
	.pfnUsageFault_Handler = (void*) UsageFault_Handler,
	.pfnReserved1_Handler  = (void*) (0UL),          /* Reserved */
	.pfnReserved2_Handler  = (void*) (0UL),          /* Reserved */
	.pfnReserved3_Handler  = (void*) (0UL),          /* Reserved */
	.pfnReserved4_Handler  = (void*) (0UL),          /* Reserved */
	.pfnSVC_Handler        = (void*) SVC_Handler,
	.pfnDebugMon_Handler   = (void*) DebugMon_Handler,
	.pfnReserved5_Handler  = (void*) (0UL),          /* Reserved */
	.pfnPendSV_Handler     = (void*) PendSV_Handler,
	.pfnSysTick_Handler    = (void*) SysTick_Handler,

	/* Configurable interrupts */
	.pfnSUPC_Handler   = (void*) SUPC_Handler,   /* 0  Supply Controller */
	.pfnRSTC_Handler   = (void*) RSTC_Handler,   /* 1  Reset Controller */
	.pfnRTC_Handler    = (void*) RTC_Handler,    /* 2  Real Time Clock */
	.pfnRTT_Handler    = (void*) RTT_Handler,    /* 3  Real Time Timer */
	.pfnWDT_Handler    = (void*) WDT_Handler,    /* 4  Watchdog Timer */
	.pfnPMC_Handler    = (void*) PMC_Handler,    /* 5  Power Management Controller */
	.pfnEFC0_Handler   = (void*) EFC0_Handler,   /* 6  Enhanced Embedded Flash Controller 0 */
#ifdef _SAM4S_EFC1_INSTANCE_
	.pfnEFC1_Handler   = (void*) EFC1_Handler,   /* 7  Enhanced Embedded Flash Controller 1 */
#else
	.pvReserved7       = (void*) (0UL),          /* 7  Reserved */
#endif /* _SAM4S_EFC1_INSTANCE_ */
	.pfnUART0_Handler  = (void*) UART0_Handler,  /* 8  UART 0 */
	.pfnUART1_Handler  = (void*) UART1_Handler,  /* 9  UART 1 */
	.pvReserved10      = (void*) (0UL),          /* 10 Reserved */
	.pfnPIOA_Handler   = (void*) PIOA_Handler,   /* 11 Parallel I/O Controller A */
	.pfnPIOB_Handler   = (void*) PIOB_Handler,   /* 12 Parallel I/O Controller B */
#ifdef _SAM4S_PIOC_INSTANCE_
	.pfnPIOC_Handler   = (void*) PIOC_Handler,   /* 13 Parallel I/O Controller C */
#else
	.pvReserved13      = (void*) (0UL),          /* 13 Reserved */
#endif /* _SAM4S_PIOC_INSTANCE_ */
	.pfnUSART0_Handler = (void*) USART0_Handler, /* 14 USART 0 */
#ifdef _SAM4S_USART1_INSTANCE_
	.pfnUSART1_Handler = (void*) USART1_Handler, /* 15 USART 1 */
#else
	.pvReserved15      = (void*) (0UL),          /* 15 Reserved */
#endif /* _SAM4S_USART1_INSTANCE_ */
	.pvReserved16      = (void*) (0UL),          /* 16 Reserved */
	.pvReserved17      = (void*) (0UL),          /* 17 Reserved */
#ifdef _SAM4S_HSMCI_INSTANCE_
	.pfnHSMCI_Handler  = (void*) HSMCI_Handler,  /* 18 Multimedia Card Interface */
#else
	.pvReserved18      = (void*) (0UL),          /* 18 Reserved */
#endif /* _SAM4S_HSMCI_INSTANCE_ */
	.pfnTWI0_Handler   = (void*) TWI0_Handler,   /* 19 Two Wire Interface 0 */
	.pfnTWI1_Handler   = (void*) TWI1_Handler,   /* 20 Two Wire Interface 1 */
	.pfnSPI_Handler    = (void*) SPI_Handler,    /* 21 Serial Peripheral Interface */
	.pfnSSC_Handler    = (void*) SSC_Handler,    /* 22 Synchronous Serial Controller */
	.pfnTC0_Handler    = (void*) TC0_Handler,    /* 23 Timer/Counter 0 */
	.pfnTC1_Handler    = (void*) TC1_Handler,    /* 24 Timer/Counter 1 */
	.pfnTC2_Handler    = (void*) TC2_Handler,    /* 25 Timer/Counter 2 */
#ifdef _SAM4S_TC1_INSTANCE_
	.pfnTC3_Handler    = (void*) TC3_Handler,    /* 26 Timer/Counter 3 */
#else
	.pvReserved26      = (void*) (0UL),          /* 26 Reserved */
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
	.pfnTC4_Handler    = (void*) TC4_Handler,    /* 27 Timer/Counter 4 */
#else
	.pvReserved27      = (void*) (0UL),          /* 27 Reserved */
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
	.pfnTC5_Handler    = (void*) TC5_Handler,    /* 28 Timer/Counter 5 */
#else
	.pvReserved28      = (void*) (0UL),          /* 28 Reserved */
#endif /* _SAM4S_TC1_INSTANCE_ */
	.pfnADC_Handler    = (void*) ADC_Handler,    /* 29 Analog To Digital Converter */
#ifdef _SAM4S_DACC_INSTANCE_
	.pfnDACC_Handler   = (void*) DACC_Handler,   /* 30 Digital To Analog Converter */
#else
	.pvReserved30      = (void*) (0UL),          /* 30 Reserved */
#endif /* _SAM4S_DACC_INSTANCE_ */
	.pfnPWM_Handler    = (void*) PWM_Handler,    /* 31 Pulse Width Modulation */
	.pfnCRCCU_Handler  = (void*) CRCCU_Handler,  /* 32 CRC Calculation Unit */
	.pfnACC_Handler    = (void*) ACC_Handler,    /* 33 Analog Comparator */
	.pfnUDP_Handler    = (void*) UDP_Handler     /* 34 USB Device Port */
};

/**------------------------------------------------------------------------------
 * This is the code that gets called on processor reset. To initialize the
 * device.
 *------------------------------------------------------------------------------*/
int __low_level_init(void)
{
	uint32_t *pSrc = __section_begin(".intvec");

	SCB->VTOR = ((uint32_t) pSrc);

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
