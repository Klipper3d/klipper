/**
 * \file
 *
 * \brief GCC Startup file for SAM4L.
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
#include "sam4l.h"
#include "system_sam4l.h"

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
	0, 0, 0, 0,        /* Reserved */
	SVC_Handler,
	DebugMon_Handler,
	0,                 /* Reserved  */
	PendSV_Handler,
	SysTick_Handler,

	// Configurable interrupts
	HFLASHC_Handler,      // 0
	PDCA_0_Handler,       // 1
	PDCA_1_Handler,       // 2
	PDCA_2_Handler,       // 3
	PDCA_3_Handler,       // 4
	PDCA_4_Handler,       // 5
	PDCA_5_Handler,       // 6
	PDCA_6_Handler,       // 7
	PDCA_7_Handler,       // 8
	PDCA_8_Handler,       // 9
	PDCA_9_Handler,       // 10
	PDCA_10_Handler,      // 11
	PDCA_11_Handler,      // 12
	PDCA_12_Handler,      // 13
	PDCA_13_Handler,      // 14
	PDCA_14_Handler,      // 15
	PDCA_15_Handler,      // 16
	CRCCU_Handler,        // 17
	USBC_Handler,         // 18
	PEVC_TR_Handler,      // 19
	PEVC_OV_Handler,      // 20
	#ifdef AESA
	AESA_Handler,         // 21
	#else
	Dummy_Handler,
	#endif
	PM_Handler,           // 22
	SCIF_Handler,         // 23
	FREQM_Handler,        // 24
	GPIO_0_Handler,       // 25
	GPIO_1_Handler,       // 26
	GPIO_2_Handler,       // 27
	GPIO_3_Handler,       // 28
	GPIO_4_Handler,       // 29
	GPIO_5_Handler,       // 30
	GPIO_6_Handler,       // 31
	GPIO_7_Handler,       // 32
	GPIO_8_Handler,       // 33
	GPIO_9_Handler,       // 34
	GPIO_10_Handler,      // 35
	GPIO_11_Handler,      // 36
	BPM_Handler,          // 37
	BSCIF_Handler,        // 38
	AST_ALARM_Handler,    // 39
	AST_PER_Handler,      // 40
	AST_OVF_Handler,      // 41
	AST_READY_Handler,    // 42
	AST_CLKREADY_Handler, // 43
	WDT_Handler,          // 44
	EIC_1_Handler,        // 45
	EIC_2_Handler,        // 46
	EIC_3_Handler,        // 47
	EIC_4_Handler,        // 48
	EIC_5_Handler,        // 49
	EIC_6_Handler,        // 50
	EIC_7_Handler,        // 51
	EIC_8_Handler,        // 52
	IISC_Handler,         // 53
	SPI_Handler,          // 54
	TC00_Handler,         // 55
	TC01_Handler,         // 56
	TC02_Handler,         // 57
	TC10_Handler,         // 58
	TC11_Handler,         // 59
	TC12_Handler,         // 60
	TWIM0_Handler,        // 61
	TWIS0_Handler,        // 62
	TWIM1_Handler,        // 63
	TWIS1_Handler,        // 64
	USART0_Handler,       // 65
	USART1_Handler,       // 66
	USART2_Handler,       // 67
	USART3_Handler,       // 68
	ADCIFE_Handler,       // 69
	DACC_Handler,         // 70
	ACIFC_Handler,        // 71
	ABDACB_Handler,       // 72
	TRNG_Handler,         // 73
	PARC_Handler,         // 74
	CATB_Handler,         // 75
	Dummy_Handler,        // one not used
	TWIM2_Handler,        // 77
	TWIM3_Handler,        // 78
	#ifdef LCDCA
	LCDCA_Handler         // 79
	#else
	Dummy_Handler
	#endif
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
	pSrc = (uint32_t *) &_sfixed;
	SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

	/* Initialize the C library */
	__libc_init_array();

	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}
