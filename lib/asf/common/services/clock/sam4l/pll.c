/**
 * \file
 *
 * \brief Chip-specific PLL implementation
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
#include <compiler.h>
#include <pll.h>

#define SCIF_UNLOCK_PLL_REG(pll_id) \
	do { \
		SCIF->SCIF_UNLOCK = SCIF_UNLOCK_KEY(0xAAu)                     \
			| SCIF_UNLOCK_ADDR((uint32_t)&SCIF->SCIF_PLL[0].SCIF_PLL \
			+ (4 * pll_id) - (uint32_t)SCIF);                      \
	} while (0)

void pll_config_write(const struct pll_config *cfg, uint32_t pll_id)
{
	irqflags_t flags;

	Assert(pll_id < NR_PLLS);

	flags = cpu_irq_save();

	SCIF_UNLOCK_PLL_REG(pll_id);
	SCIF->SCIF_PLL[pll_id].SCIF_PLL  = cfg->ctrl;
	cpu_irq_restore(flags);
}

void pll_enable(const struct pll_config *cfg, uint32_t pll_id)
{
	irqflags_t flags;

	Assert(pll_id < NR_PLLS);

	flags = cpu_irq_save();
	SCIF_UNLOCK_PLL_REG(pll_id);
	SCIF->SCIF_PLL[pll_id].SCIF_PLL  = cfg->ctrl | SCIF_PLL_PLLEN;
	cpu_irq_restore(flags);
}

void pll_disable(uint32_t pll_id)
{
	irqflags_t flags;

	Assert(pll_id < NR_PLLS);

	flags = cpu_irq_save();
	SCIF_UNLOCK_PLL_REG(pll_id);
	SCIF->SCIF_PLL[pll_id].SCIF_PLL  = 0;
	cpu_irq_restore(flags);
}
