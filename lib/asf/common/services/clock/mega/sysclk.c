/**
 * \file
 *
 * \brief Chip-specific system clock management functions
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
#include <sysclk.h>

/* ! \name System Clock Initialization */
/* @{ */

/**
 *  Function to initialize the clock and disable clock for not required modules.
 */
void sysclk_init(void)
{
#if !MEGA_XX_UN0 && !MEGA_XX_UN1
	uint8_t *reg = (uint8_t *)&(POWER_REG_ADD);
	uint8_t i;
	/* Turn off all peripheral clocks that can be turned off.
	 * The debugWIRE system of some devices that shares system clock with the SPI module.
	 * Thus the PRSPI bit in the PRR register must not be set when debugging.
	 */
	for (i = 0; i < NUMBER_OF_POWER_REG; i++) {
		*(reg++) = 0xFF;
	}
#endif
#if !MEGA_UNSPECIFIED && !MEGA_XX
	/* Set up system clock prescalers if different from defaults */
	if ((CONFIG_SYSCLK_PSDIV != SYSCLK_PSDIV_8) ||
			(SYSCLK_PSDIV_8 != CLKPR)) {
		sysclk_set_prescalers(CONFIG_SYSCLK_PSDIV);
	}
#endif
}

/* @} */

/* ! \name System enable and disable module */
/* @{ */

/**
 * \brief Enable the clock to peripheral \a id on port \a port
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c power_red_id *definitions).
 * \param id The ID (bitmask) of the peripheral module to be enabled.
 */
void sysclk_enable_module(enum power_red_id port, uint8_t id)
{
#if !MEGA_UNSPECIFIED && !MEGA_XX
	uint8_t *reg = (uint8_t *)&(POWER_REG_ADD);
	irqflags_t flags = cpu_irq_save();

	if (port < NUMBER_OF_POWER_REG) {
		*(reg + port)  &= ~id;
	}
	cpu_irq_restore(flags);
#endif
}

/*
 * \brief Disable the clock to peripheral \a id on port \a port
 *
 * \param port ID of the port to which the module is connected (one of
 * the \c power_red_id *definitions).
 * \param id The ID (bit mask) of the peripheral module to be disabled.
 */
void sysclk_disable_module( enum power_red_id port, uint8_t id)
{
#if !MEGA_UNSPECIFIED && !MEGA_XX
	uint8_t *reg = (uint8_t *)&(POWER_REG_ADD);
	irqflags_t flags = cpu_irq_save();
	if (port < NUMBER_OF_POWER_REG) {
		*(reg + port) |= id;
	}
	cpu_irq_restore(flags);
#endif
}

/* @} */
