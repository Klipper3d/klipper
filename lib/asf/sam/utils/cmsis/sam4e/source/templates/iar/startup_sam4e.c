/**
 * \file
 *
 * \brief Startup file for SAM4E.
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
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "exceptions.h"
#include "sam4e.h"
#include "system_sam4e.h"

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
	{Reset_Handler},

	{NMI_Handler},
	{HardFault_Handler},
	{MemManage_Handler},
	{BusFault_Handler},
	{UsageFault_Handler},
	{0}, {0}, {0}, {0},   /*  Reserved */
	{SVC_Handler},
	{DebugMon_Handler},
	{0},                   /*  Reserved */
	{PendSV_Handler},
	{SysTick_Handler},

	/* Configurable interrupts */
	{SUPC_Handler},   /* 0  Supply Controller */
	{RSTC_Handler},   /* 1  Reset Controller */
	{RTC_Handler},    /* 2  Real Time Clock */
	{RTT_Handler},    /* 3  Real Time Timer */
	{WDT_Handler},    /* 4  Watchdog/Dual Watchdog Timer */
	{PMC_Handler},    /* 5  Power Management Controller */
	{EFC_Handler},    /* 6  Enhanced Embedded Flash Controller */
	{UART0_Handler},  /* 7  UART 0 */
	{Dummy_Handler},
	{PIOA_Handler},   /* 9  Parallel I/O Controller A */
	{PIOB_Handler},   /* 10 Parallel I/O Controller B */
#ifdef _SAM4E_PIOC_INSTANCE_
	{PIOC_Handler},   /* 11 Parallel I/O Controller C */
#else
	{Dummy_Handler},
#endif
#ifdef _SAM4E_PIOD_INSTANCE_
	{PIOD_Handler},   /* 12 Parallel I/O Controller D */
#else
	{Dummy_Handler},
#endif
#ifdef _SAM4E_PIOE_INSTANCE_
	{PIOE_Handler},   /* 13 Parallel I/O Controller E */
#else
	{Dummy_Handler},
#endif
	{USART0_Handler}, /* 14 USART 0 */
	{USART1_Handler}, /* 15 USART 1 */
	{HSMCI_Handler},  /* 16 Multimedia Card Interface */
	{TWI0_Handler},   /* 17 Two Wire Interface 0 */
	{TWI1_Handler},   /* 18 Two Wire Interface 1 */
	{SPI_Handler},    /* 19 Serial Peripheral Interface */
	{DMAC_Handler},   /* 20 DMAC */
	{TC0_Handler},    /* 21 Timer/Counter 0 */
	{TC1_Handler},    /* 22 Timer/Counter 1 */
	{TC2_Handler},    /* 23 Timer/Counter 2 */
#ifdef _SAM4E_TC1_INSTANCE_
	{TC3_Handler},    /* 24 Timer/Counter 3 */
	{TC4_Handler},    /* 25 Timer/Counter 4 */
	{TC5_Handler},    /* 26 Timer/Counter 5 */
#else
	{Dummy_Handler},
	{Dummy_Handler},
	{Dummy_Handler},
#endif
#ifdef _SAM4E_TC2_INSTANCE_
	{TC6_Handler},    /* 27 Timer/Counter 6 */
	{TC7_Handler},    /* 28 Timer/Counter 7 */
	{TC8_Handler},    /* 29 Timer/Counter 8 */
#else
	{Dummy_Handler},
	{Dummy_Handler},
	{Dummy_Handler},
#endif
	{AFEC0_Handler},  /* 30 Analog Front End 0 */
	{AFEC1_Handler},  /* 31 Analog Front End 1 */
	{DACC_Handler},   /* 32 Digital To Analog Converter */
	{ACC_Handler},    /* 33 Analog Comparator */
	{ARM_Handler},    /* 34 FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC */
	{UDP_Handler},    /* 35 USB DEVICE */
	{PWM_Handler},    /* 36 PWM */
	{CAN0_Handler},   /* 37 CAN0 */
#ifdef _SAM4E_CAN1_INSTANCE_
	{CAN1_Handler},   /* 38 CAN1 */
#else
	{Dummy_Handler},
#endif
	{AES_Handler},    /* 39 AES */
	{Dummy_Handler},
	{Dummy_Handler},
	{Dummy_Handler},
	{Dummy_Handler},
#ifdef _SAM4E_GMAC_INSTANCE_
	{GMAC_Handler},   /* 44 GMAC */
#else
	{Dummy_Handler},
#endif
	{UART1_Handler}   /* 45 UART */
};

/* EWARM 6.30 integrates CMSIS 2.10 (__CM4_CMSIS_VERSION 0x0210),
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

	if (((uint32_t) pSrc >= IRAM_ADDR) && ((uint32_t) pSrc < (uint32_t) IRAM_ADDR + (uint32_t) IRAM_SIZE)) {
		SCB->VTOR |= (uint32_t) (1 << SCB_VTOR_TBLBASE_Pos);
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
