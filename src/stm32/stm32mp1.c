// Code to setup clocks on stm32h7
//
// Copyright (C) 2020 Konstantin Vogel <konstantin.vogel@gmx.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "autoconf.h" // CONFIG_CLOCK_REF_FREQ
#include "board/armcm_boot.h" // VectorTable
#include "board/armcm_reset.h" // try_request_canboot
#include "board/irq.h" // irq_disable
#include "board/misc.h" // bootloader_request
#include "command.h" // DECL_CONSTANT_STR
#include "internal.h" // get_pclock_frequency
#include "sched.h" // sched_main

#include <stdint.h>
#include <stm32mp1xx.h>

#ifdef CONFIG_STM32_RPMSG
#include "rpmsg.h"
#endif

#define RCC_MCUSSOURCE_HSI              0U
#define RCC_MCUSSOURCE_HSE              RCC_MSSCKSELR_MCUSSRC_0
#define RCC_MCUSSOURCE_CSI              RCC_MSSCKSELR_MCUSSRC_1
#define RCC_MCUSSOURCE_PLL3             (RCC_MSSCKSELR_MCUSSRC_1 | RCC_MSSCKSELR_MCUSSRC_0)

#define RCC_PLL3SOURCE_HSI              0U
#define RCC_PLL3SOURCE_HSE              RCC_RCK3SELR_PLL3SRC_0
#define RCC_PLL3SOURCE_CSI              RCC_RCK3SELR_PLL3SRC_1
#define RCC_PLL3SOURCE_OFF              (RCC_RCK3SELR_PLL3SRC_1 | RCC_RCK3SELR_PLL3SRC_0)

#define RCC_FLAG_HSIRDY           ( (RCC->OCRDYR & RCC_OCRDYR_HSIRDY)     == RCC_OCRDYR_HSIRDY )
#define RCC_FLAG_HSIDIVRDY        ( (RCC->OCRDYR & RCC_OCRDYR_HSIDIVRDY)  == RCC_OCRDYR_HSIDIVRDY )
#define RCC_FLAG_CSIRDY           ( (RCC->OCRDYR & RCC_OCRDYR_CSIRDY)     == RCC_OCRDYR_CSIRDY )
#define RCC_FLAG_HSERDY           ( (RCC->OCRDYR & RCC_OCRDYR_HSERDY)     == RCC_OCRDYR_HSERDY )
#define RCC_FLAG_AXICKRDY         ( (RCC->OCRDYR & RCC_OCRDYR_AXICKRDY)   == RCC_OCRDYR_AXICKRDY )
#define RCC_FLAG_CKREST           ( (RCC->OCRDYR & RCC_OCRDYR_CKREST)     == RCC_OCRDYR_CKREST )

#define RCC_MCU_DIV1                   0U
#define RCC_MCU_DIV2                   RCC_MCUDIVR_MCUDIV_0
#define RCC_MCU_DIV4                   RCC_MCUDIVR_MCUDIV_1
#define RCC_MCU_DIV8                   (RCC_MCUDIVR_MCUDIV_1 | RCC_MCUDIVR_MCUDIV_0)
#define RCC_MCU_DIV16                  RCC_MCUDIVR_MCUDIV_2
#define RCC_MCU_DIV32                  (RCC_MCUDIVR_MCUDIV_2 | RCC_MCUDIVR_MCUDIV_0)
#define RCC_MCU_DIV64                  (RCC_MCUDIVR_MCUDIV_2 | RCC_MCUDIVR_MCUDIV_1)
#define RCC_MCU_DIV128                 (RCC_MCUDIVR_MCUDIV_2 | RCC_MCUDIVR_MCUDIV_1 | RCC_MCUDIVR_MCUDIV_0)
#define RCC_MCU_DIV256                 RCC_MCUDIVR_MCUDIV_3
#define RCC_MCU_DIV512                 (RCC_MCUDIVR_MCUDIV_3 | RCC_MCUDIVR_MCUDIV_0)

#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bits for pre-emption priority
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bits for pre-emption priority
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority
                                                                 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority
                                                                 0 bits for subpriority */

static uint32_t RCC_GetPLL3PFreq() {
	uint32_t pllsource = 0, pll3m = 1, pll3fracen = 0, hsivalue = 0;
	float fracn1, pll3vco = 0;

	pllsource = (uint32_t) (RCC->RCK3SELR & RCC_RCK3SELR_PLL3SRC);
	pll3m = ((RCC->PLL3CFGR1 & RCC_PLL3CFGR1_DIVM3) >> RCC_PLL3CFGR1_DIVM3_Pos)
			+ 1U;
	pll3fracen = (RCC->PLL3FRACR & RCC_PLL3FRACR_FRACLE)
			>> RCC_PLL3FRACR_FRACLE_Pos;
	fracn1 =
			(float) (pll3fracen
					* ((RCC->PLL3FRACR & RCC_PLL3FRACR_FRACV)
							>> RCC_PLL3FRACR_FRACV_Pos));
	pll3vco = (float) ((float) ((RCC->PLL3CFGR1 & RCC_PLL3CFGR1_DIVN) + 1U)
			+ (fracn1 / (float) 0x2000));  //Intermediary value
	switch (pllsource) {
	case RCC_PLL3SOURCE_HSI: /* HSI used as PLL clock source */
		if (RCC_FLAG_HSIDIVRDY) {
			hsivalue = (HSI_VALUE
					>> ((uint32_t) (RCC->HSICFGR & RCC_HSICFGR_HSIDIV)));
			pll3vco *= (float) (hsivalue / pll3m);
		} else {
			pll3vco *= (float) (HSI_VALUE / pll3m);
		}
		break;

	case RCC_PLL3SOURCE_HSE: /* HSE used as PLL clock source */
		pll3vco *= (float) (HSE_VALUE / pll3m);
		break;

	case RCC_PLL3SOURCE_CSI: /* CSI used as PLL clock source */
		pll3vco *= (float) (CSI_VALUE / pll3m);
		break;

	case RCC_PLL3SOURCE_OFF: /* No clock source for PLL */
		pll3vco = 0;
		break;
	}

	return (uint32_t) (pll3vco
			/ ((float) (((RCC->PLL3CFGR2 & RCC_PLL3CFGR2_DIVP)
					>> RCC_PLL3CFGR2_DIVP_Pos) + 1U)));
}

static uint32_t RCC_GetMCUSSFreq() {
	uint32_t mcussfreq = 0;

	switch (((uint32_t) (RCC->MSSCKSELR & RCC_MSSCKSELR_MCUSSRC))) {
	case RCC_MCUSSOURCE_PLL3:
		mcussfreq = RCC_GetPLL3PFreq();
		break;

	case RCC_MCUSSOURCE_HSI:
		mcussfreq = (HSI_VALUE
				>> ((uint32_t) (RCC->HSICFGR & RCC_HSICFGR_HSIDIV)));
		break;

	case RCC_MCUSSOURCE_HSE:
		mcussfreq = HSE_VALUE;
		break;

	case RCC_MCUSSOURCE_CSI:
		mcussfreq = CSI_VALUE;
		break;
	}

	return mcussfreq;
}

static uint32_t RCC_GetSystemCoreClockFreq(void) {
	uint32_t mcudiv = (uint32_t) (RCC->MCUDIVR & RCC_MCUDIVR_MCUDIV);
	if (mcudiv > RCC_MCU_DIV512) {
		mcudiv = RCC_MCU_DIV512;
	}

	return RCC_GetMCUSSFreq() >> mcudiv;
}

void clock_setup() {
	SystemCoreClock = RCC_GetSystemCoreClockFreq();
}

void IrqTrap(const char *name);

struct cline lookup_clock_line(uint32_t periph_base) {
#define CLKLINE(PERIPH) \
	if (periph_base >= PERIPH##_PERIPH_BASE) { \
		uint32_t bit = 1 << ((periph_base - PERIPH##_PERIPH_BASE) / 0x400); \
		return (struct cline){.en=&RCC->MC_##PERIPH##ENSETR, .rst=&RCC->PERIPH##RSTSETR, .bit=bit}; \
	}

	CLKLINE(APB5);
	CLKLINE(APB5);
	CLKLINE(AHB6);
	// Not used CLKLINE(GPV);
	CLKLINE(AHB5);
	// Not used CLKLINE(APB_DEBUG);
	CLKLINE(APB3);
	CLKLINE(AHB4);
	CLKLINE(AHB3);
	CLKLINE(AHB2);
	CLKLINE(APB2);
	CLKLINE(APB1);

	IrqTrap("ASSERT FAILED");
}

uint32_t get_pclock_frequency(uint32_t periph_base) {
	/** Note: when CM4 core is started in production mode
	 *  clock tree is configured by device tree.
	 *  To support configurations with different
	 *  clock speeds lookup similar to RCC_GetMCUSSFreq()
	 *  should be performed
	 */
	return SystemCoreClock;
}

// Enable a GPIO peripheral clock
void gpio_clock_enable(GPIO_TypeDef *regs) {
	uint32_t rcc_pos = ((uint32_t) regs - AHB4_PERIPH_BASE) / 0x400;
	RCC->MC_AHB4ENSETR |= 1 << rcc_pos;
}

void SystemInit();

void armcm_main(void) {
	SystemInit();
	SCB->VTOR = (uint32_t) VectorTable;

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	clock_setup();

	RCC->MC_AHB3ENSETR = RCC_MC_AHB3ENSETR_HSEMEN;
#ifdef CONFIG_STM32_RPMSG
	ipcc_init();
	rpmsg_ns_create();
#endif

	sched_main();
}

